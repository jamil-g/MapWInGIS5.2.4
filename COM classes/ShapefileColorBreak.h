/**************************************************************************************
 * File name: ShapefileColorBreak.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: declaration of CShapefileColorBreak
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

#ifdef OLD_API 

#ifndef __SHAPEFILELEGENDBREAK_H_
#define __SHAPEFILELEGENDBREAK_H_

/////////////////////////////////////////////////////////////////////////////
// CShapefileColorBreak
class ATL_NO_VTABLE CShapefileColorBreak : 
	public CComObjectRootEx<CComObjectThreadModel>,
	public CComCoClass<CShapefileColorBreak, &CLSID_ShapefileColorBreak>,
	public IDispatchImpl<IShapefileColorBreak, &IID_IShapefileColorBreak, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	CShapefileColorBreak()
	{
		_pUnkMarshaler = NULL;
		VariantInit(&_startValue);
		VariantInit(&_endValue);
		_visible = VARIANT_TRUE;
	}
	~CShapefileColorBreak()
	{	
		VariantClear(&_startValue);
		VariantClear(&_endValue);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SHAPEFILELEGENDBREAK)

	BEGIN_COM_MAP(CShapefileColorBreak)
		COM_INTERFACE_ENTRY(IShapefileColorBreak)
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

// IShapefileColorBreak
public:
	STDMETHOD(get_EndColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_EndColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_StartColor)(/*[out, retval]*/ OLE_COLOR *pVal);
	STDMETHOD(put_StartColor)(/*[in]*/ OLE_COLOR newVal);
	STDMETHOD(get_EndValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_EndValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_StartValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_StartValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Caption)(BSTR* pVal);
	STDMETHOD(put_Caption)(BSTR newVal);

private:
	VARIANT _startValue;
	VARIANT _endValue;
	CString _caption;
	OLE_COLOR _startColor;
	OLE_COLOR _endColor;
	VARIANT_BOOL _visible;
	
};

#endif //__SHAPEFILELEGENDBREAK_H_

#endif