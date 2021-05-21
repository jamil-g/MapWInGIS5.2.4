/**************************************************************************************
 * File name: ShapeWrapperCOM.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of CShapeWrapperCOM class
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
 
#pragma once
#include "ShapeInterfaces.h"

class CShapeWrapperCOM : public IShapeWrapper
{
private:
	CShapeWrapperCOM()
		: _xMin(0.0), _yMin(0.0), _xMax(0.0), _yMax(0.0), _zMin(0.0), _mMin(0.0), _zMax(0.0), _mMax(0.0),
		_boundsChanged(true), _lastErrorCode(tkNO_ERROR)
	{
	}

public:
	CShapeWrapperCOM(ShpfileType shpType)
		: CShapeWrapperCOM() 
	{
		put_ShapeType(shpType);
	}

	CShapeWrapperCOM(char* data, int recordLength)
		: CShapeWrapperCOM() 	
	{
		put_RawData(data, recordLength);
	}

	virtual ~CShapeWrapperCOM()
	{
		Clear();
	}
	
private:	
	// type
	ShpfileType _shapeType;
	
	// bounds
	double _xMin;
	double _yMin;
	double _xMax;
	double _yMax;

	double _zMin;
	double _mMin;
	double _zMax;
	double _mMax;
	bool _boundsChanged;
	short _lastErrorCode;

	std::vector<IPoint *> _points;
	std::vector<long> _parts;

public:
	ShapeWrapperType get_WrapperType() { return swtCom; }

	int get_PointCount(){ return _points.size(); }
	int get_PartCount(){ return _parts.size(); }

	// type
	ShpfileType get_ShapeType(){ return _shapeType; }
	ShpfileType get_ShapeType2D(){ return ShapeUtility::Convert2D(_shapeType); }
	bool put_ShapeType(ShpfileType shpType);
	
	// bounds
	void RefreshBounds();
	void RefreshBoundsXY();
	bool get_BoundsXY(double& xMin, double& xMax, double& yMin, double& yMax);
	bool get_Bounds(double& xMin, double& xMax, double& yMin, double& yMax, 
					double& zMin, double& zMax, double& mMin, double& mMax);
	
	void get_XYFast(int PointIndex, double& x, double& y);
	bool get_PointXY(int PointIndex, double& x, double& y);
	bool put_PointXY(int PointIndex, double x, double y);
	bool get_PointZ(int PointIndex, double& z);
	bool get_PointM(int PointIndex, double& m);
	bool put_PointZ(int PointIndex, double z);
	bool put_PointM(int PointIndex, double m);
	bool get_PointXYZM(int pointIndex, double& x, double& y, double& z, double& m);
	
	// COM points
	IPoint* get_Point(long Index);
	bool put_Point(long Index, IPoint* pnt);

	// changing size
	void Clear();
	bool InsertPoint(int PointIndex, IPoint* pnt);
	bool InsertPointXY(int PointIndex, double x, double y);
	bool InsertPointXYZM(int PointIndex, double x, double y, double z, double m);
	bool DeletePoint(int Pointindex);
	
	// parts
	bool InsertPart(int PartIndex, int PointIndex);
	bool DeletePart(int PartIndex);
	int get_PartStartPoint(int PartIndex);
	int get_PartEndPoint(int PartIndex);
	bool put_PartStartPoint(long PartIndex, long newVal);
	
	bool PointInRing(int partIndex, double pointX, double pointY);
	void ReversePoints(long startIndex, long endIndex);

	int* get_RawData();
	bool put_RawData(char* shapeData, int length);

	int get_LastErrorCode()
	{
		int code = _lastErrorCode;
		_lastErrorCode = tkNO_ERROR;
		return code;
	}
};