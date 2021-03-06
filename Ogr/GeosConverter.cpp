//********************************************************************************************************
//File name: GeosConverter.cpp
//Description: 
//********************************************************************************************************
//The contents of this file are subject to the Mozilla Public License Version 1.1 (the "License"); 
//you may not use this file except in compliance with the License. You may obtain a copy of the License at 
//http://www.mozilla.org/MPL/ 
//Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF 
//ANY KIND, either express or implied. See the License for the specific language governing rights and 
//limitations under the License. 
//
//The Original Code is MapWindow Open Source. 
//
//The Initial Developer of this version of the Original Code is Daniel P. Ames using portions created by 
//Utah State University and the Idaho National Engineering and Environmental Lab that were released as 
//public domain in March 2004.  
//
//Contributor(s): (Open source contributors should list themselves and their modifications here). 
// -------------------------------------------------------------------------------------------------------

#include "StdAfx.h"
#include "GeosConverter.h"
#include "GeosHelper.h"
#include "OgrConverter.h"

// ReSharper disable CppUseAuto

// *********************************************************************
//		DoBuffer()
// *********************************************************************
GEOSGeometry* DoBuffer(DOUBLE distance, long nQuadSegments, GEOSGeometry* gsGeom)
{
	__try
	{
		return GeosHelper::Buffer(gsGeom, distance, nQuadSegments);
	}
	__except (1)
	{
		return nullptr;
	}
}

// *********************************************************************
//		GEOSGeom2Shape()
// *********************************************************************
bool GeosConverter::GeomToShapes(GEOSGeom gsGeom, vector<IShape*>* vShapes, bool isM)
{
	bool substitute = false;
	bool has25D = false;	

	if (!GeosHelper::IsValid(gsGeom))
	{
		// during buffer operation shape looses its Z coordinate, but during conversion back to 
		// shape it should be restored or shape type check won't be passed 
		has25D = GeosHelper::HasZ(gsGeom);
	
		// would be better to pass units explicitly
		// Fixing MWGIS-59: Fixup makes new shape larger: 
		// GEOSGeometry* gsNew = DoBuffer(m_globalSettings.GetInvalidShapeBufferDistance(umMeters) / 1000.0, 30, gsGeom);
		GEOSGeometry* const gsNew = DoBuffer(0, 30, gsGeom);
		if (gsNew && GeosHelper::IsValid(gsNew))
		{
			gsGeom = gsNew;
			substitute = true;    // it will be deleted below
		}
	}

	bool result = false;
	OGRGeometry* const oGeom = GeosHelper::CreateFromGEOS(gsGeom);
	if (oGeom)
	{
		char* const type = GeosHelper::GetGeometryType(gsGeom);
	    const CString s = type;
		GeosHelper::Free(type);

        // PM: It seems oForceType is never used:
	    // auto oForceType = wkbNone;
		// if (s == "LinearRing" && oGeom->getGeometryType() != wkbLinearRing)
		// 	oForceType = wkbLinearRing;

		result = OgrConverter::GeometryToShapes(oGeom, vShapes, isM, wkbNone, has25D);
		OGRGeometryFactory::destroyGeometry(oGeom);
	}

	if (substitute)
		GeosHelper::DestroyGeometry(gsGeom);

	return result;
}

// *********************************************************************
//			Shape2GEOSGeom()
// *********************************************************************
//  Converts MapWinGis shape to GEOS geometry
GEOSGeom GeosConverter::ShapeToGeom(IShape* shp)
{
	OGRGeometry* const oGeom = OgrConverter::ShapeToGeometry(shp);
	if (oGeom != nullptr)
	{
		GEOSGeometry* const result = GeosHelper::ExportToGeos(oGeom);
		OGRGeometryFactory::destroyGeometry(oGeom);
		return result;
	}
    return nullptr;
}

// *****************************************************
//		SimplifyPolygon()
// *****************************************************
// A polygon is expected as input; multi-polygons should be split into parts before treating with this routine
GEOSGeometry* GeosConverter::SimplifyPolygon(const GEOSGeometry *gsGeom, double tolerance)
{
	const GEOSGeometry* gsRing = GeosHelper::GetExteriorRing(gsGeom);	// no memory is allocated there
    // ReSharper disable once CppLocalVariableMayBeConst
    GEOSGeom gsPoly = GeosHelper::TopologyPreserveSimplify(gsRing, tolerance);		// memory allocation

	if (!gsPoly)
		return nullptr;

	std::vector<GEOSGeom> holes;
	for (int n = 0; n < GeosHelper::GetNumInteriorRings(gsGeom); n++)
	{
		gsRing = GeosHelper::GetInteriorRingN(gsGeom, n);				// no memory is allocated there
		if (gsRing)
		{
			GEOSGeom gsOut = GeosHelper::TopologyPreserveSimplify(gsRing, tolerance);	// memory allocation
			if (gsOut)
			{
				char* type = GeosHelper::GetGeometryType(gsOut);
			    const CString s = type;
				GeosHelper::Free(type);
				if (s == "LinearRing")
					holes.push_back(gsOut);
			}
		}
	}

	GEOSGeometry *gsNew;
	if (!holes.empty())
	{
		gsNew = GeosHelper::CreatePolygon(gsPoly, &holes[0], holes.size()); // memory allocation (should be released by caller)
	}
	else
	{
		gsNew = GeosHelper::CreatePolygon(gsPoly, nullptr, 0);
	}
	return gsNew;
}

// *****************************************************
//		NormalizeSplitResults()
// *****************************************************
void GeosConverter::NormalizeSplitResults(GEOSGeometry* result, GEOSGeometry* subject, ShpfileType shpType, vector<GEOSGeometry*>& results)
{
	if (!result) return;

    const int numGeoms = GeosHelper::GetNumGeometries(result);
	if (numGeoms > 1)
	{
		if (shpType == SHP_POLYGON)
		{
			for (int i = 0; i < numGeoms; i++)
			{
				const GEOSGeometry* polygon = GeosHelper::GetGeometryN(result, i);
				GEOSGeometry* intersect = GeosHelper::Intersection(subject, polygon);

				if (!intersect)
					continue;

				double intersectArea;
				GeosHelper::Area(intersect, &intersectArea);
				GeosHelper::DestroyGeometry(intersect);

				double polyArea;
				GeosHelper::Area(polygon, &polyArea);

			    const double areaRatio = intersectArea / polyArea;
				if (areaRatio > 0.99 && areaRatio < 1.01) {
					GEOSGeometry* clone = GeosHelper::CloneGeometry(polygon);
					if (clone) {
						results.push_back(clone);
					}
				}
			}

		}
		else {
			for (int i = 0; i < numGeoms; i++) {
				const GEOSGeometry* line = GeosHelper::GetGeometryN(result, i);
				GEOSGeometry* clone = GeosHelper::CloneGeometry(line);
				if (clone) {
					results.push_back(clone);
				}
			}
		}
	}
}

// ********************************************************************
//		MergeGeosGeometries
// ********************************************************************
// Returns GEOS geometry which is result of the union operation for the geometries passed
GEOSGeometry* GeosConverter::MergeGeometries(vector<GEOSGeometry*>& data, ICallback* callback, bool deleteInput /*= true*/, bool displayProgress /*= true*/)
{
	if (data.empty())
		return nullptr;

	USES_CONVERSION;
	GEOSGeometry* g1 = nullptr;
	GEOSGeometry* g2 = nullptr;

	bool stop = false;
	int count = 0;	// number of union operation performed
	long percent = 0;

    const int size = data.size();
	int depth = 0;

	if (size == 1)
	{
		// no need for calculation
		if (deleteInput)
			return data[0];	 // no need to clone; it will be exactly the same
	    GEOSGeometry* geomTemp = GeosHelper::CloneGeometry(data[0]);
	    return geomTemp;
	}

	while (!stop)
	{
		stop = true;

		for (int i = 0; i < size; i++)
		{
			if (data[i] != nullptr)
			{
				if (!g1)
				{
					g1 = data[i];
					data[i] = nullptr;
				}
				else
				{
					g2 = data[i];
					data[i] = nullptr;
				}

				if (g2 != nullptr)
				{
					GEOSGeometry* geom = GeosHelper::Union(g1, g2);
					data[i] = geom;		// placing the resulting geometry back for further processing

					if (deleteInput || depth > 0)	// in clipping operation geometries are used several times
						// so the intial geometries should be intact (depth == 0)
						// in other cases (Buffer, Dissolve) the geometries can be deleted in place
					{
						GeosHelper::DestroyGeometry(g1);
						GeosHelper::DestroyGeometry(g2);
					}

					g1 = nullptr;
					g2 = nullptr;
					count++;
					stop = false;		// in case there is at least one union occurred, we shall run once more

					if (displayProgress)
						CallbackHelper::Progress(callback, count, size, "Merging shapes...", percent);
				}

				// it is the last geometry, unpaired one, not the only one, it's the initial and must not be deleted
				if (i == size - 1 && !stop && g2 == nullptr && g1 != nullptr && depth == 0 && !deleteInput)
				{
					// we need to clone it, to be able to apply unified memory management afterwards
					// when depth > 0 all interim geometries are deleted, while this one should be preserved
					GEOSGeometry* geomTemp = GeosHelper::CloneGeometry(g1);
					g1 = geomTemp;
				}
			}
		}
		depth++;
	}
	return g1;
}

// ReSharper restore CppUseAuto