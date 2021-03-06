/**************************************************************************************
 * File name: ShapefileCategory.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: declaration of CShapefileCategory - a class representing a particular
 * of shapfile visualization scheme
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
 // lsu 9 may 2010 - created the file

#pragma once
#include "ShapeDrawingOptions.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CShapefileCategory
class ATL_NO_VTABLE CShapefileCategory :
	public CComObjectRootEx<CComObjectThreadModel>,
	public CComCoClass<CShapefileCategory, &CLSID_ShapefileCategory>,
	public IDispatchImpl<IShapefileCategory, &IID_IShapefileCategory, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	CShapefileCategory()
	{
		_pUnkMarshaler = NULL;
		_name = SysAllocString(L"");
		_expression = SysAllocString(L"");
		_priority = - 1;
		_drawingOptions = NULL;
		CoCreateInstance(CLSID_ShapeDrawingOptions,NULL,CLSCTX_INPROC_SERVER,IID_IShapeDrawingOptions,(void**)&_drawingOptions);
		_categories = NULL;
		_categoryValue = cvExpression;
		gReferenceCounter.AddRef(tkInterface::idShapefileCategory);
	}
	
	~CShapefileCategory()
	{
		::SysFreeString(_name);
		::SysFreeString(_expression);
		if(_drawingOptions != NULL)
			_drawingOptions->Release();
		gReferenceCounter.Release(tkInterface::idShapefileCategory);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SHAPEFILECATEGORY)

	BEGIN_COM_MAP(CShapefileCategory)
		COM_INTERFACE_ENTRY(IShapefileCategory)
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

public:
	STDMETHOD(get_Name)(BSTR* retval);
	STDMETHOD(put_Name)(BSTR newVal);
	STDMETHOD(get_Expression)(BSTR* retval);
	STDMETHOD(put_Expression)(BSTR newVal);
	STDMETHOD(get_DrawingOptions)(IShapeDrawingOptions** retval);
	STDMETHOD(put_DrawingOptions)(IShapeDrawingOptions* newVal);
	STDMETHOD(get_Priority)(LONG* retval)							{*retval = _priority;			return S_OK;};
	STDMETHOD(put_Priority)(LONG newVal)							{_priority = newVal;			return S_OK;};
	STDMETHOD(get_ValueType)(tkCategoryValue* pVal);
	STDMETHOD(put_ValueType)(tkCategoryValue newVal);
	STDMETHOD(get_MinValue)(VARIANT* pVal);
	STDMETHOD(put_MinValue)(VARIANT newVal);
	STDMETHOD(get_MaxValue)(VARIANT* pVal);
	STDMETHOD(put_MaxValue)(VARIANT newVal);

private:
	BSTR _name;
	BSTR _expression;
	IShapeDrawingOptions* _drawingOptions;
	long _priority;
	IShapefileCategories* _categories;
	tkCategoryValue _categoryValue;
	CComVariant _minValue;
	CComVariant _maxValue;
	
public:
	CDrawingOptionsEx* get_UnderlyingOptions();
	void put_underlyingOptions(CDrawingOptionsEx*);
	void put_parentCollection(IShapefileCategories* categories) {_categories = categories;}
	tkCategoryValue GetCategoryValue() {return _categoryValue; }
	CComVariant* GetMinValue() { return &_minValue; }
	CComVariant* GetMaxValue() { return &_maxValue; }
};
OBJECT_ENTRY_AUTO(__uuidof(ShapefileCategory), CShapefileCategory)
