/**************************************************************************************
 * File name: TileProviders.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of CTileProviders
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
#include "BaseProvider.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#define TILE_PROVIDER_COUNT 23

class ATL_NO_VTABLE CTileProviders :
	public CComObjectRootEx<CComObjectThreadModel>,
	public CComCoClass<CTileProviders, &CLSID_TileProviders>,
	public IDispatchImpl<ITileProviders, &IID_ITileProviders, &LIBID_MapWinGIS, VERSION_MAJOR, VERSION_MINOR>
{
public:
	CTileProviders()
		: _tiles(NULL)
	{
		_pUnkMarshaler = NULL;
		_key = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;

		for (int i = 0; i <= TILE_PROVIDER_COUNT; i++)
		{
			// we don't care about the value returned; a provider is added to the list, that's all needed
			getProviderCore((tkTileProvider)i);
		}
	}

	~CTileProviders()
	{
		SysFreeString(_key);

		for (size_t i = 0; i < _providers.size(); i++) {
			_providers[i]->ClearSubProviders();
		}

		for (size_t i = 0; i < _providers.size(); i++)	{
			delete _providers[i];
		}
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_TILEPROVIDERS)
	BEGIN_COM_MAP(CTileProviders)
		COM_INTERFACE_ENTRY(ITileProviders)
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
	STDMETHOD(get_LastErrorCode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ErrorMsg)(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GlobalCallback)(/*[out, retval]*/ ICallback * *pVal);
	STDMETHOD(put_GlobalCallback)(/*[in]*/ ICallback * newVal);
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Key)(/*[in]*/ BSTR newVal);
	
	STDMETHOD(Remove)(LONG provider, VARIANT_BOOL clearCache, VARIANT_BOOL* retVal);
	STDMETHOD(Clear)(VARIANT_BOOL clearCache);
	STDMETHOD(get_Count)(LONG* pVal);
	STDMETHOD(Add)(int Id, BSTR name, BSTR urlPattern, tkTileProjection projection, int minZoom, int maxZoom, BSTR copyright, VARIANT_BOOL* retVal);
	
	STDMETHOD(get_Id)(int Index, LONG* retVal);
	STDMETHOD(get_Name)(int Index, BSTR* retVal);
	STDMETHOD(put_Name)(int Index, BSTR pVal);
	STDMETHOD(get_UrlPattern)(int Index, BSTR* retVal);
	STDMETHOD(get_Projection)(int Index, tkTileProjection* retVal);
	STDMETHOD(get_MinZoom)(int Index, int* retVal);
	STDMETHOD(get_MaxZoom)(int Index, int* retVal);
	STDMETHOD(get_IsCustom)(int Index, VARIANT_BOOL* retVal);
	STDMETHOD(get_Version)(int Index, BSTR* retVal);
	STDMETHOD(put_Version)(int Index, BSTR newVal);
	STDMETHOD(get_IndexByProvider)(tkTileProvider provider, int* retVal);
	STDMETHOD(get_IndexByProviderId)(int provider, int* retVal);
	STDMETHOD(get_Language)(int Index, BSTR* retVal);
	STDMETHOD(put_Language)(int Index, BSTR twoLetterCode);
	STDMETHOD(get_GeographicBounds)(int Index, IExtents** pVal);
	STDMETHOD(put_GeographicBounds)(int Index, IExtents* newVal);

private:	
	ITiles* _tiles;	// reference the parent
	vector<BaseProvider*> _providers;
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;

private:
	void ErrorMessage(long ErrorCode);
	BaseProvider* getProviderCore(tkTileProvider providerId);
	bool ValidateProviderIndex(int index);

public:
	vector<BaseProvider*>* GetList() { return &_providers;	}
	void put_Tiles(ITiles* tiles){	_tiles = tiles;}
	BaseProvider* get_Provider(int providerId);
	CStringW get_CopyrightNotice(tkTileProvider provider);
	CString get_LicenseUrl(tkTileProvider provider);
};

OBJECT_ENTRY_AUTO(__uuidof(TileProviders), CTileProviders)
