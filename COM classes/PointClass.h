/**************************************************************************************
 * File name: PointClass.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of CPointClass
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

// CPointClass
// In contrast with other classes we are using CComSingleThreadModel base class here.
// CComMultiThreadModel increases memory usage by 24 more bytes which is unacceptable.
class ATL_NO_VTABLE CPointClass : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPointClass, &CLSID_Point>,
	public IDispatchImpl<IPoint, &IID_IPoint, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	CPointClass()
	{	
		_pUnkMarshaler = NULL;
		_x = 0.0;
		_y = 0.0;
		_z = 0.0;
		_m = 0.0;
		//gReferenceCounter.AddRef(tkInterface::idPoint);     // it's additional overhead, use for debugging only
	}
	~CPointClass()
	{	
		//gReferenceCounter.Release(tkInterface::idPoint);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_POINT)

	DECLARE_NOT_AGGREGATABLE(CPointClass)

	BEGIN_COM_MAP(CPointClass)
		COM_INTERFACE_ENTRY(IPoint)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, _pUnkMarshaler.p)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &_pUnkMarshaler.p);
		return S_OK;
	}

	void FinalRelease()
	{
		_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> _pUnkMarshaler;


// IPoint
public:
	STDMETHOD(get_M)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_M)(/*[in]*/ double newVal);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_GlobalCallback)(/*[out, retval]*/ ICallback * *pVal);
	STDMETHOD(put_GlobalCallback)(/*[in]*/ ICallback * newVal);
	STDMETHOD(get_ErrorMsg)(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LastErrorCode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Z)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Z)(/*[in]*/ double newVal);
	STDMETHOD(get_Y)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Y)(/*[in]*/ double newVal);
	STDMETHOD(get_X)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_X)(/*[in]*/ double newVal);
	STDMETHOD(Clone)(IPoint** retVal);
	STDMETHOD(Set)(/*[in]*/ double newX, /*[in]*/ double newY, /*[in, optional]*/ double newZ);

private:
	double _x;
	double _y;
	double _z;
	double _m;

};

OBJECT_ENTRY_AUTO(__uuidof(Point), CPointClass)
