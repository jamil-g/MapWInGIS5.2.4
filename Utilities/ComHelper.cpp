/**************************************************************************************
* File name: ComHelper.cpp
*
* Project: MapWindow Open Source (MapWinGis ActiveX control)
* Description: 
*
**************************************************************************************
* The contents of this file are subject to the Mozilla Public License Version 1.1
* (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at http://www.mozilla.org/mpl/
* See the License for the specific language governing rights and limitations
* under the License.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
**************************************************************************************
* Contributor(s):
* (Open source contributors should list themselves and their modifications here). */
// june 2017 PaulM - Added GdalUtils

#include "stdafx.h"
#include "ComHelper.h"

// ********************************************************
//	  ComHelper::SetRef
// ********************************************************
// Sets new instance of COM object to the given pointer. Takes care to release the old reference
bool ComHelper::SetRef(IDispatch* newVal, IDispatch** oldVal, bool allowNull)
{
	if (*oldVal == NULL)
	{
		if (newVal)
		{
			(*oldVal) = newVal;
			newVal->AddRef();
		}
	}
	else if ((*oldVal) != newVal)
	{
		if (newVal == NULL && !allowNull)
		{
			// NULL reference isn't allowed
			return false;
		}
		else
		{
			if (*oldVal)
			{
				(*oldVal)->Release();
				(*oldVal) = NULL;
			}
			(*oldVal) = newVal;

			if (newVal)
			{
				newVal->AddRef();
			}
		}
	}
	else
	{
		// do nothing, the reference is in place
	}
	return true;
}

// ********************************************************
//	  CreateInstance
// ********************************************************
HRESULT ComHelper::CreateInstance(tkInterface interfaceId, IDispatch** retVal)
{
	HRESULT result = S_FALSE;
	void* val = NULL;
	switch (interfaceId)
	{
		case tkInterface::idPoint:
			ComHelper::CreatePoint((IPoint**)&val);
			break;
		case tkInterface::idShape:
			ComHelper::CreateShape((IShape**)&val);
			break;
		case tkInterface::idExtents:
			ComHelper::CreateExtents((IExtents**)&val);
			break;
		case tkInterface::idField:
			result = CoCreateInstance(CLSID_Field, NULL, CLSCTX_INPROC_SERVER, IID_IField, (void**)&val);
			break;
		case tkInterface::idLabel:
			result = CoCreateInstance(CLSID_Label, NULL, CLSCTX_INPROC_SERVER, IID_ILabel, (void**)&val);
			break;
		case tkInterface::idChart:
			result = CoCreateInstance(CLSID_Chart, NULL, CLSCTX_INPROC_SERVER, IID_IChart, (void**)&val);
			break;
		case tkInterface::idChartField:
			result = CoCreateInstance(CLSID_ChartField, NULL, CLSCTX_INPROC_SERVER, IID_IChartField, (void**)&val);
			break;
		case tkInterface::idCharts:
			result = CoCreateInstance(CLSID_Charts, NULL, CLSCTX_INPROC_SERVER, IID_ICharts, (void**)&val);
			break;
		case tkInterface::idColorScheme:
			result = CoCreateInstance(CLSID_ColorScheme, NULL, CLSCTX_INPROC_SERVER, IID_IColorScheme, (void**)&val);
			break;
		case tkInterface::idDrawingRectangle:
			result = CoCreateInstance(CLSID_DrawingRectangle, NULL, CLSCTX_INPROC_SERVER, IID_IDrawingRectangle, (void**)&val);
			break;
		case tkInterface::idESRIGridManager:
			result = CoCreateInstance(CLSID_ESRIGridManager, NULL, CLSCTX_INPROC_SERVER, IID_IESRIGridManager, (void**)&val);
			break;
		case tkInterface::idExpression:
			result = CoCreateInstance(CLSID_Expression, NULL, CLSCTX_INPROC_SERVER, IID_IExpression, (void**)&val);
			break;
		case tkInterface::idFieldStatOperations:
			result = CoCreateInstance(CLSID_FieldStatOperations, NULL, CLSCTX_INPROC_SERVER, IID_IFieldStatOperations, (void**)&val);
			break;
		case tkInterface::idFileManager:
			result = CoCreateInstance(CLSID_FileManager, NULL, CLSCTX_INPROC_SERVER, IID_IFileManager, (void**)&val);
			break;
		case tkInterface::idFunction:
			result = CoCreateInstance(CLSID_Function, NULL, CLSCTX_INPROC_SERVER, IID_IFunction, (void**)&val);
			break;
		case tkInterface::idGdalDataset:
			result = CoCreateInstance(CLSID_GdalDataset, NULL, CLSCTX_INPROC_SERVER, IID_IGdalDataset, (void**)&val);
			break;
		case tkInterface::idGdalDriver:
			result = CoCreateInstance(CLSID_GdalDriver, NULL, CLSCTX_INPROC_SERVER, IID_IGdalDriver, (void**)&val);
			break;
		case tkInterface::idGdalDriverManager:
			result = CoCreateInstance(CLSID_GdalDriverManager, NULL, CLSCTX_INPROC_SERVER, IID_IGdalDriverManager, (void**)&val);
			break;
		case tkInterface::idGdalRasterBand:
			result = CoCreateInstance(CLSID_GdalRasterBand, NULL, CLSCTX_INPROC_SERVER, IID_IGdalRasterBand, (void**)&val);
			break;
		case tkInterface::idGeoProjection:
			result = CoCreateInstance(CLSID_GeoProjection, NULL, CLSCTX_INPROC_SERVER, IID_IGeoProjection, (void**)&val);
			break;
		case tkInterface::idGlobalSettings:
			result = CoCreateInstance(CLSID_GlobalSettings, NULL, CLSCTX_INPROC_SERVER, IID_IGlobalSettings, (void**)&val);
			break;
		case tkInterface::idGrid:
			result = CoCreateInstance(CLSID_Grid, NULL, CLSCTX_INPROC_SERVER, IID_IGrid, (void**)&val);
			break;
		case tkInterface::idGridColorBreak:
			result = CoCreateInstance(CLSID_GridColorBreak, NULL, CLSCTX_INPROC_SERVER, IID_IGridColorBreak, (void**)&val);
			break;
		case tkInterface::idGridColorScheme:
			result = CoCreateInstance(CLSID_GridColorScheme, NULL, CLSCTX_INPROC_SERVER, IID_IGridColorScheme, (void**)&val);
			break;
		case tkInterface::idGridHeader:
			result = CoCreateInstance(CLSID_GridHeader, NULL, CLSCTX_INPROC_SERVER, IID_IGridHeader, (void**)&val);
			break;
		case tkInterface::idHistogram:
			result = CoCreateInstance(CLSID_Histogram, NULL, CLSCTX_INPROC_SERVER, IID_IHistogram, (void**)&val);
			break;
		case tkInterface::idIdentifier:
			result = CoCreateInstance(CLSID_Identifier, NULL, CLSCTX_INPROC_SERVER, IID_IIdentifier, (void**)&val);
			break;
		case tkInterface::idImage:
			result = CoCreateInstance(CLSID_Image, NULL, CLSCTX_INPROC_SERVER, IID_IImage, (void**)&val);
			break;
		case tkInterface::idLabelCategory:
			result = CoCreateInstance(CLSID_LabelCategory, NULL, CLSCTX_INPROC_SERVER, IID_ILabelCategory, (void**)&val);
			break;
		case tkInterface::idLabels:
			result = CoCreateInstance(CLSID_Labels, NULL, CLSCTX_INPROC_SERVER, IID_ILabels, (void**)&val);
			break;
		case tkInterface::idLinePattern:
			result = CoCreateInstance(CLSID_LinePattern, NULL, CLSCTX_INPROC_SERVER, IID_ILinePattern, (void**)&val);
			break;
		case tkInterface::idLineSegment:
			result = CoCreateInstance(CLSID_LineSegment, NULL, CLSCTX_INPROC_SERVER, IID_ILineSegment, (void**)&val);
			break;
		case tkInterface::idMeasuring:
			result = CoCreateInstance(CLSID_Measuring, NULL, CLSCTX_INPROC_SERVER, IID_IMeasuring, (void**)&val);
			break;
		case tkInterface::idOgrDatasource:
			result = CoCreateInstance(CLSID_OgrDatasource, NULL, CLSCTX_INPROC_SERVER, IID_IOgrDatasource, (void**)&val);
			break;
		case tkInterface::idOgrLayer:
			result = CoCreateInstance(CLSID_OgrLayer, NULL, CLSCTX_INPROC_SERVER, IID_IOgrLayer, (void**)&val);
			break;	
		case tkInterface::idSelectionList:
			result = CoCreateInstance(CLSID_SelectionList, NULL, CLSCTX_INPROC_SERVER, IID_ISelectionList, (void**)&val);
			break;
		case tkInterface::idShapeDrawingOptions:
			result = CoCreateInstance(CLSID_ShapeDrawingOptions, NULL, CLSCTX_INPROC_SERVER, IID_IShapeDrawingOptions, (void**)&val);
			break;
		case tkInterface::idShapeEditor:
			result = CoCreateInstance(CLSID_ShapeEditor, NULL, CLSCTX_INPROC_SERVER, IID_IShapeEditor, (void**)&val);
			break;
		case tkInterface::idShapefile:
			result = CoCreateInstance(CLSID_Shapefile, NULL, CLSCTX_INPROC_SERVER, IID_IShapefile, (void**)&val);
			break;
		case tkInterface::idShapefileCategories:
			result = CoCreateInstance(CLSID_ShapefileCategories, NULL, CLSCTX_INPROC_SERVER, IID_IShapefileCategories, (void**)&val);
			break;
		case tkInterface::idShapefileCategory:
			result = CoCreateInstance(CLSID_ShapefileCategory, NULL, CLSCTX_INPROC_SERVER, IID_IShapefileCategory, (void**)&val);
			break;
#ifdef OLD_API
		case tkInterface::idShapefileColorBreak:
			result = CoCreateInstance(CLSID_ShapefileColorBreak, NULL, CLSCTX_INPROC_SERVER, IID_IShapefileColorBreak, (void**)&val);
			break;
		case tkInterface::idShapefileColorScheme:
			result = CoCreateInstance(CLSID_ShapefileColorScheme, NULL, CLSCTX_INPROC_SERVER, IID_IShapefileColorScheme, (void**)&val);
			break;
#endif
		case tkInterface::idShapeNetwork:
			result = CoCreateInstance(CLSID_ShapeNetwork, NULL, CLSCTX_INPROC_SERVER, IID_IShapeNetwork, (void**)&val);
			break;
		case tkInterface::idShapeValidationInfo:
			result = CoCreateInstance(CLSID_ShapeValidationInfo, NULL, CLSCTX_INPROC_SERVER, IID_IShapeValidationInfo, (void**)&val);
			break;
		case tkInterface::idTable:
			result = CoCreateInstance(CLSID_Table, NULL, CLSCTX_INPROC_SERVER, IID_ITable, (void**)&val);
			break;
		case tkInterface::idTileProviders:
			result = CoCreateInstance(CLSID_TileProviders, NULL, CLSCTX_INPROC_SERVER, IID_ITileProviders, (void**)&val);
			break;
		case tkInterface::idTiles:
			result = CoCreateInstance(CLSID_Tiles, NULL, CLSCTX_INPROC_SERVER, IID_ITiles, (void**)&val);
			break;
		case tkInterface::idTin:
			result = CoCreateInstance(CLSID_Tin, NULL, CLSCTX_INPROC_SERVER, IID_ITin, (void**)&val);
			break;
		case tkInterface::idUndoList:
			result = CoCreateInstance(CLSID_UndoList, NULL, CLSCTX_INPROC_SERVER, IID_IUndoList, (void**)&val);
			break;
		case tkInterface::idUtils:
			result = CoCreateInstance(CLSID_Utils, NULL, CLSCTX_INPROC_SERVER, IID_IUtils, (void**)&val);
			break;
		case tkInterface::idVector:
			result = CoCreateInstance(CLSID_Vector, NULL, CLSCTX_INPROC_SERVER, IID_IVector, (void**)&val);
			break;
		case tkInterface::idWmsLayer:
			result = CoCreateInstance(CLSID_WmsLayer, NULL, CLSCTX_INPROC_SERVER, IID_IWmsLayer, (void**)&val);
			break;
		case tkInterface::idGdalUtils:
			result = CoCreateInstance(CLSID_GdalUtils, NULL, CLSCTX_INPROC_SERVER, IID_IGdalUtils, (void**)&val);
			break;
	}
	*retVal = val ? (IDispatch*)val : NULL;
	return result;
}

// ********************************************************
//     GetInterfaceName()
// ********************************************************
CString ComHelper::GetInterfaceName(tkInterface id)
{
	switch (id)
	{
		case idChart: 			return "Chart";
		case idChartField:		return "ChartField";
		case idCharts:			return "Charts";
		case idColorScheme:		return "ColorScheme";
		case idESRIGridManager:	return "ESRIGridManager";
		case idExtents:			return "Extents";
		case idField:			return "Field";
		case idFieldStatOperations:			return "FieldStatOperations";
		case idFileManager:		return "FileManager";
		case idGeoProjection:	return "GeoProjection";
		case idGlobalSettings:	return "GlobalSettings";
		case idGrid:			return "Grid:";
		case idGridColorBreak:	return "GridColorBreak";
		case idGridColorScheme:	return "GridColorScheme";
		case idGridHeader:		return "GridHeader";
		case idIdentifier:      return "Identifier";
		case idImage:			return "Image";
		case idLabelCategory:	return "LabelCategory";
		case idLabel:			return "Label";
		case idLabels:			return "Labels";
		case idLinePattern:		return "LinePattern";
		case idLineSegment:		return "LineSegment";
		case idMeasuring:		return "Measuring";
		case idPoint:			return "Point";
		case idShape:			return "Shape";
		case idShapeDrawingOptions:		return "ShapeDrawingOptions";
		case idShapefile:		return "Shapefile";
		case idShapefileCategories:		return "ShapefileCategories";
		case idShapefileCategory:		return "ShapefileCategory";
		case idShapefileColorBreak:		return "ShapefileColorBreak";
		case idShapefileColorScheme:	return "ShapefileColorScheme";
		case idShapeNetwork:			return "ShapeNetwork";
		case idShapeValidationInfo:		return "ShapeValidationInfo";
		case idTable:			return "Table";
		case idTileProviders:	return "TileProviders";
		case idTiles:			return "Tiles";
		case idTin:				return "Tin";
		case idUtils:			return "Utils";
		case idVector:			return "Vector";
		case idOgrDatasource:   return "OgrDatasource";
		case idOgrLayer:		return "OgrLayer";
		case idShapeEditor:		return "ShapeEditor";
		case idUndoList:		return "UndoList";
		case idWmsLayer:		return "WmsLayer";
		case idGdalUtils:		return "GdalUtils";
		default:				return "Unknown";
	}
}

// ********************************************************
//     CreatePoint()
// ********************************************************
void ComHelper::CreatePoint(IPoint** point)
{
	m_factory.pointFactory->CreateInstance(NULL, IID_IPoint, (void**)point);
}

// ********************************************************
//     CreateShape()
// ********************************************************
void ComHelper::CreateShape(IShape** shp)
{
	m_factory.shapeFactory->CreateInstance(NULL, IID_IShape, (void**)shp);
}

// ********************************************************
//     CreateExtents()
// ********************************************************
void ComHelper::CreateExtents(IExtents** box)
{
	m_factory.extentsFactory->CreateInstance(NULL, IID_IExtents, (void**)box);
}
