// FileManager.h : Declaration of the CFileManager
#pragma once

// CFileManager
class ATL_NO_VTABLE CFileManager :
	public CComObjectRootEx<CComObjectThreadModel>,
	public CComCoClass<CFileManager, &CLSID_FileManager>,
	public IDispatchImpl<IFileManager, &IID_IFileManager, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	CFileManager()
	{
		_pUnkMarshaler = NULL;
		_key = SysAllocString(L"");
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_lastOpenStrategy = fosAutoDetect;
		_lastOpenIsSuccess = true;
		_lastOpenFilename = L"";
	};
	~CFileManager()
	{
		SysFreeString(_key);
		if (_globalCallback)
			_globalCallback->Release();
	};

	DECLARE_REGISTRY_RESOURCEID(IDR_FILEMANAGER)

	BEGIN_COM_MAP(CFileManager)
		COM_INTERFACE_ENTRY(IFileManager)
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
	STDMETHOD(get_IsSupportedBy)(BSTR Filename, tkSupportType supportType, VARIANT_BOOL* retVal);
	STDMETHOD(get_IsSupported)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_IsRgbImage)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_IsGrid)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_IsVectorLayer)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_OpenStrategy)(BSTR Filename, tkFileOpenStrategy* retVal);
	STDMETHOD(get_CanOpenAs)(BSTR Filename, tkFileOpenStrategy strategy, VARIANT_BOOL* retVal);
	STDMETHOD(get_HasProjection)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_GeoProjection)(BSTR Filename, IGeoProjection** retVal);
	STDMETHOD(get_IsSameProjection)(BSTR Filename, IGeoProjection* projection, VARIANT_BOOL* retVal);
	STDMETHOD(Open)(BSTR Filename, tkFileOpenStrategy openStrategy, ICallback* callback, IDispatch** retVal);
	STDMETHOD(OpenShapefile)(BSTR Filename, ICallback* callback, IShapefile** retVal);
	STDMETHOD(OpenRaster)(BSTR Filename, tkFileOpenStrategy openStrategy, ICallback* callback, IImage** retVal);
	STDMETHOD(DeleteDatasource)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_ErrorMsg)(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LastErrorCode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_GlobalCallback)(/*[out, retval]*/ ICallback * *pVal);
	STDMETHOD(put_GlobalCallback)(/*[in]*/ ICallback * newVal);
	STDMETHOD(get_LastOpenStrategy)(tkFileOpenStrategy* retVal);
	STDMETHOD(get_LastOpenFilename)(BSTR* retVal);
	STDMETHOD(get_LastOpenIsSuccess)(VARIANT_BOOL* retVal);
	STDMETHOD(get_HasGdalOverviews)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(ClearGdalOverviews)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(BuildGdalOverviews)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_NeedsGdalOverviews)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(RemoveProxyForGrid)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(get_HasValidProxyForGrid)(BSTR Filename, VARIANT_BOOL* retVal);
	STDMETHOD(OpenFromDatabase)(BSTR connectionString, BSTR layerNameOrQuery, IOgrLayer** retVal);
	STDMETHOD(OpenVectorLayer)(BSTR Filename, ShpfileType preferedShapeType, VARIANT_BOOL forUpdate, IOgrLayer** retVal);
	STDMETHOD(OpenVectorDatasource)(BSTR Filename, IOgrDatasource** retVal);
	STDMETHOD(get_CdlgFilter)(BSTR* pVal);
	STDMETHOD(get_CdlgRasterFilter)(BSTR* pVal);
	STDMETHOD(get_CdlgVectorFilter)(BSTR* pVal);
	STDMETHOD(get_SupportedGdalFormats)(BSTR* pVal);

private:
	tkFileOpenStrategy _lastOpenStrategy;
	bool _lastOpenIsSuccess;
	CStringW _lastOpenFilename;
	CString test;
	ICallback * _globalCallback;
	long _lastErrorCode;
	BSTR _key;

private:
	tkFileOpenStrategy CFileManager::get_OpenStrategyCore(BSTR Filename);
	void ErrorMessage(long ErrorCode);
	bool IsShapefile(CStringW filename);
	CString GetFilter(OpenFileDialogFilter filter);
	
};

OBJECT_ENTRY_AUTO(__uuidof(FileManager), CFileManager)
