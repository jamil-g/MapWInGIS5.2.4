//********************************************************************************************************
// File name: Utils_Projections.cpp
// Description: CUtils class implementation of functions related to Projection strings.
//********************************************************************************************************

#include "stdafx.h"
#include "Utils.h"

// path to projection csv
static CString csvPath;
// single instance of Projected Coordinate System string mappings
static unordered_map<int, CString> pcsStrings;

// determine the path of this OCX, independent of the calling executable
// https://stackoverflow.com/questions/6924195/get-dll-path-at-runtime
//
CStringW thisOcxPath()
{
	CStringW thisPath = L"";
	WCHAR path[MAX_PATH];
	HMODULE hm;
	if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPWSTR) &thisOcxPath, &hm))
	{
		GetModuleFileNameW(hm, path, sizeof(path));
		PathRemoveFileSpecW(path);
		thisPath = CStringW(path);
		if (!thisPath.IsEmpty() &&
			thisPath.GetAt(thisPath.GetLength() - 1) != '\\')
			thisPath += L"\\";
	}

	return thisPath;
}

bool CUtils::LoadProjectionStrings()
{
	static bool bLoaded = false;

	if (!bLoaded)
	{
		// the gdal-data directory is right below the registered OCX
		csvPath = thisOcxPath() + "gdal-data\\pcs.csv";

		CStdioFile file;
		if (!file.Open(csvPath, CFile::modeRead | CFile::typeText))
		{
			// no error message here. instead, send error to callback when user calls lookup functions
			return false;
		}

		CString nextLine;
		// throw away the first line
		file.ReadString(nextLine);
		// now read all definitions
		while (file.ReadString(nextLine))
		{
			// continue on error
			try
			{
				CString code, name, restOfLine;
				// find the first comma
				int commaPosition = nextLine.Find(",", 0);
				// strip off the SRID
				code = nextLine.Left(commaPosition);
				// the rest of the line (account for leading quote symbol)
				restOfLine = nextLine.Right(nextLine.GetLength() - commaPosition - 1 - 1);
				// strip off all to the right of the name (using the closing quote symbol)
				int quotePosition = restOfLine.Find("\"", 0);
				// this is the name
				name = restOfLine.Left(quotePosition);
				// add to PCS mapping
				pcsStrings.insert(std::pair<int, CString>(atoi((LPCSTR)code), name));
			}
			catch (...)
			{
			}
		}
		file.Close();

		////////////////////////////////////////////////////
		// now process and append the esri-extra definitions
		////////////////////////////////////////////////////
		csvPath = thisOcxPath() + "gdal-data\\esri_extra.wkt";

		if (file.Open(csvPath, CFile::modeRead | CFile::typeText))
		{
			CString nextLine;
			// throw away the first line
			file.ReadString(nextLine);
			// now read all definitions
			while (file.ReadString(nextLine))
			{
				// continue on error
				try
				{
					// bypass comments
					if (nextLine.Left(1) == "#") continue;
					CString code, name, restOfLine;
					// find the first comma
					int commaPosition = nextLine.Find(",", 0);
					// bypass Geographic Projections (GEOGCS immediately following the comma)
					if (nextLine.Find("GEOGCS") == commaPosition + 1) continue;
					// strip off the SRID
					code = nextLine.Left(commaPosition);
					// the rest of the line (account for PROJCS[ and the leading quote symbol)
					restOfLine = nextLine.Right(nextLine.GetLength() - commaPosition -1 - 7 - 1);
					// strip off all to the right of the name (using the closing quote symbol)
					int quotePosition = restOfLine.Find("\"", 0);
					// this is the name
					name = restOfLine.Left(quotePosition);
					// add to PCS mapping
					pcsStrings.insert(std::pair<int, CString>(atoi((LPCSTR)code), name));
				}
				catch (...)
				{
				}
			}
			file.Close();
		}
		else
		{
			// not sure if we should raise any error in this case, 
			// since at least all of the EPSG codes have been loaded...
		}

		//
		bLoaded = true;
	}
	return bLoaded;
}

CString CUtils::customErrorMessage()
{
	CString msg;
	msg.Format("Unable to reference projection list. Failed to load GDAL Projection list '%s'", csvPath);
	return msg;
}

// return the name of the projection specified by the Nad83 enumeration
STDMETHODIMP CUtils::GetNAD83ProjectionName(tkNad83Projection projectionID, BSTR* retVal)
{
	return GetProjectionNameByID((int)projectionID, retVal);
}

// return the name of the projection specified by the Wgs84 enumeration
STDMETHODIMP CUtils::GetWGS84ProjectionName(tkWgs84Projection projectionID, BSTR* retVal)
{
	return GetProjectionNameByID((int)projectionID, retVal);
}

// return the name of any projection specified by the SRID, which include the Nad83 and Wgs84, 
// but also includes those not specified by the enumerations, such as NAD27, NAD83 Harn, Beijing, Pulkova, etc.
STDMETHODIMP CUtils::GetProjectionNameByID(int SRID, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		// list check
		if (pcsStrings.empty())
		{
			ErrorMessage(tkFILE_NOT_OPEN, customErrorMessage());
			*retVal = A2BSTR("");
		}
		else
		{
			// return string mapped to specified ID (use 'at' rather than [] to check existence)
			*retVal = A2BSTR((LPCSTR)pcsStrings.at(SRID));
		}
	}
	catch (...)
	{
		// 'at' method will throw exception if key does not exist
		CString msg;
		msg.Format("Invalid projection ID specified: {0}", SRID);
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS, msg);
		*retVal = A2BSTR("");
	}

	return S_OK;
}

STDMETHODIMP CUtils::GetProjectionList(tkProjectionSet projectionSets, VARIANT* list, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// guilty until proven innocent
	*retVal = VARIANT_FALSE;

	// list check
	if (pcsStrings.empty())
	{
		ErrorMessage(tkFILE_NOT_OPEN, customErrorMessage());
		*retVal = VARIANT_FALSE;
	}
	else
	{
		SAFEARRAY FAR* psa = NULL;
		SAFEARRAYBOUND sabound[1];
		sabound[0].lLbound = 0;

		int theSize = 0;
		if ((projectionSets & psAll_Projections) == psAll_Projections)
		{
			theSize = pcsStrings.size();
		}
		else
		{
			// may include NAD 83 strings
			if ((projectionSets & psNAD83_Subset) == psNAD83_Subset)
			{
				for each (pair<int, CString> p in pcsStrings)
				{
					// account for both pcs.csv and esri_extra formats (state plane only)
					// NOTE that this enumeration is not a contiguous sequence of number, so for now, 
					// for simplicity, rather than repeatedly enumerating to check for containment,
					// we will narrow is down slightly by only including those with a known prefix.
					if (p.second.Left(6) == "NAD83 " || p.second.Left(10) == "NAD_1983_S") theSize++;
				}
				//theSize += 881;
			}
			// may also include WGS 84 strings
			if ((projectionSets & psWGS84_Subset) == psWGS84_Subset)
			{
				for each (pair<int, CString> p in pcsStrings)
				{
					// account for both pcs.csv and esri_extra formats
					// NOTE that this enumeration is not a contiguous sequence of number, so for now, 
					// for simplicity, rather than repeatedly enumerating to check for containment,
					// we will narrow is down slightly by only including those with a known prefix.
					if (p.second.Left(6) == "WGS 84" || p.second.Left(8) == "WGS_1984") theSize++;
				}
				//theSize += 248;
			}
		}

		// allocate the array
		sabound[0].cElements = theSize;
		psa = SafeArrayCreate(VT_BSTR, 1, sabound);

		if (psa)
		{
			BSTR* pBSTR = NULL;
			SafeArrayAccessData(psa, (void **)&pBSTR);

			CComBSTR comBSTR;
			int j = 0;
			for each (pair<int, CString> p in pcsStrings)
			{
				// include ?
				if (((projectionSets & psAll_Projections) == psAll_Projections) ||
					(((projectionSets & psNAD83_Subset) == psNAD83_Subset) && (p.second.Left(6) == "NAD83 " || p.second.Left(10) == "NAD_1983_S")) ||
					(((projectionSets & psWGS84_Subset) == psWGS84_Subset) && (p.second.Left(6) == "WGS 84" || p.second.Left(8) == "WGS_1984")))
				{
					// create concatenated string
					CString msg;
					msg.Format("%d,%s", p.first, p.second);
					comBSTR.Append((LPCSTR)msg);
					// copy to array
					pBSTR[j++] = comBSTR.Copy();
					// empty the string
					comBSTR.Empty();
				}
			}

			SafeArrayUnaccessData(psa);

			list->vt = VT_ARRAY | VT_BSTR;
			list->parray = psa;

			// something is there
			*retVal = VARIANT_TRUE;
		}
		else
		{
			// failed to allocate array
			ErrorMessage(tkCANT_ALLOC_MEMORY);
		}
	}

	return S_OK;
}
