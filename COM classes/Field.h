/**************************************************************************************
 * File name: Field.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of the CField
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

// CField
class ATL_NO_VTABLE CField : 
	public CComObjectRootEx<CComObjectThreadModel>,
	public CComCoClass<CField, &CLSID_Field>,
	public IDispatchImpl<IField, &IID_IField, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	CField()
	{
		_pUnkMarshaler = NULL;
		_key = SysAllocString(L"");
		_name = SysAllocString(L"");
		_alias = SysAllocString(L"");
		_expression = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_width = 10;
		_precision = 10;
		_type = STRING_FIELD;
		_isUpdated = false;
		_table = NULL;
		_visible = VARIANT_TRUE;
		_joinId = -1;
		_modified = VARIANT_FALSE;
		gReferenceCounter.AddRef(tkInterface::idField);
	}

	~CField()
	{
		::SysFreeString(_key);
		::SysFreeString(_name);
		::SysFreeString(_alias);
		::SysFreeString(_expression);

		if( _globalCallback )
			_globalCallback->Release();
		_globalCallback = NULL;
		gReferenceCounter.Release(tkInterface::idField);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_FIELD)

	DECLARE_NOT_AGGREGATABLE(CField)

	BEGIN_COM_MAP(CField)
		COM_INTERFACE_ENTRY(IField)
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

// IField
public:
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_GlobalCallback)(/*[out, retval]*/ ICallback * *pVal);
	STDMETHOD(put_GlobalCallback)(/*[in]*/ ICallback * newVal);
	STDMETHOD(get_ErrorMsg)(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LastErrorCode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ FieldType *pVal);
	STDMETHOD(put_Type)(/*[in]*/ FieldType newVal);
	STDMETHOD(get_Precision)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Precision)(/*[in]*/ long newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Width)(/*[in]*/ long newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(Clone)(/*[out, retval]*/ IField** retVal);
	STDMETHOD(get_Visible)(VARIANT_BOOL* pVal);
	STDMETHOD(put_Visible)(VARIANT_BOOL newVal);
	STDMETHOD(get_Alias)(BSTR* pVal);
	STDMETHOD(put_Alias)(BSTR newVal);
	STDMETHOD(get_Joined)(VARIANT_BOOL* pVal);
	STDMETHOD(get_Expression)(BSTR* pVal);
	STDMETHOD(put_Expression)(BSTR newVal);
	STDMETHOD(get_Modified)(VARIANT_BOOL* pVal);
	STDMETHOD(put_Modified)(VARIANT_BOOL newVal);

private:
	ITable* _table;		// no references should be added here
	ICallback * _globalCallback;
	FieldType _type;
	BSTR _key;
	BSTR _name;
	BSTR _alias;
	BSTR _expression;
	long _lastErrorCode;
	long _precision;
	long _width;
	int _joinId;
	bool _isUpdated;
	VARIANT_BOOL _visible;
	VARIANT_BOOL _modified;

private:
	bool CheckTableEditingState();

public:
	void ErrorMessage(long ErrorCode);
	ITable* GetTable() { return _table; }
	void SetTable(ITable* table) { _table = table; }
	bool GetIsUpdated() { return _isUpdated; }
	void SetIsUpdated(bool value) { _isUpdated = value; }

	int GetJoinId() {return _joinId; }
	void SetJoinId(int value) { _joinId = value; }
};

OBJECT_ENTRY_AUTO(__uuidof(Field), CField)
