/**************************************************************************************
 * File name: Chart.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of CChart
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
 // Sergei Leschinski (lsu) 25 june 2010 - created the file.

#pragma once
#include "ShapeRecord.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CChart
class ATL_NO_VTABLE CChart :
	public CComObjectRootEx<CComObjectThreadModel>,
	public CComCoClass<CChart, &CLSID_Chart>,
	public IDispatchImpl<IChart, &IID_IChart, &LIBID_MapWinGIS, /*wMajor =*/ VERSION_MAJOR, /*wMinor =*/ VERSION_MINOR>
{
public:
	CChart()
	{
		_canDelete = true;
		_chartData = new CChartInfo();
		_pUnkMarshaler = NULL;
	}
	~CChart()
	{
		if (_chartData != NULL && _canDelete) 
			delete _chartData;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_CHART)

	BEGIN_COM_MAP(CChart)
		COM_INTERFACE_ENTRY(IChart)
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
	STDMETHOD(get_PositionX)(double* retVal);
	STDMETHOD(put_PositionX)(double newVal);
	STDMETHOD(get_PositionY)(double* retVal);
	STDMETHOD(put_PositionY)(double newVal);
	STDMETHOD(get_Visible)(VARIANT_BOOL* retVal);
	STDMETHOD(put_Visible)(VARIANT_BOOL newVal);
	STDMETHOD(get_IsDrawn)(VARIANT_BOOL* retVal);
	STDMETHOD(put_IsDrawn)(VARIANT_BOOL newVal);
	STDMETHOD(get_ScreenExtents)(IExtents** retval);
	
private:
	CChartInfo* _chartData;
	bool _canDelete;		// CChartInfo can be allocated locally, then we need to delete it
							// For example, when client create CChart class with new command.
							// In case CChartInfo is a pointer to the element of CShapefile vector we must not delete it
							// For a example if a client receive reference to some existing chart and then releases 
							// CChart class.
public:
	char* get_ChartData();
	void put_ChartData(char* newVal);
};

OBJECT_ENTRY_AUTO(__uuidof(Chart), CChart)

