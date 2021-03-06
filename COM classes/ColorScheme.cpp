/**************************************************************************************
 * File name: ColorScheme.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Implementation of CColorScheme
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
 // Sergei Leschinski 2 jul 2010 - created the file

#include "stdafx.h"
#include "ColorScheme.h"
#include "Macros.h"

// ***************************************************************
//	 SetColors()
// ***************************************************************
STDMETHODIMP CColorScheme::SetColors (OLE_COLOR Color1, OLE_COLOR Color2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_breaks.clear();

	ColorBreak br1;
	br1.value = 0.0;
	br1.color = Color1;
	_breaks.push_back(br1);

	ColorBreak br2;
	br2.value = 1.0;
	br2.color = Color2;
	_breaks.push_back(br2);

	return S_OK;
}

// ***************************************************************
//	 SetColors2()
// ***************************************************************
STDMETHODIMP CColorScheme::SetColors2 (tkMapColor Color1, tkMapColor Color2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetColors(BGR_TO_RGB(Color1), BGR_TO_RGB(Color2));
	return S_OK;
}

// ***************************************************************
//	 SetColors3()
// ***************************************************************
STDMETHODIMP CColorScheme::SetColors3 (short MinRed, short MinGreen, short MinBlue, short MaxRed, short MaxGreen, short MaxBlue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if (MinRed > MaxRed || MinGreen > MaxGreen || MinBlue > MaxBlue)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return S_OK;
	}
	if (MinRed < 0)		MinRed = 0;
	if (MinGreen < 0)	MinGreen = 0;
	if (MinBlue < 0)	MinBlue = 0;
	if (MaxRed > 255)	MaxRed = 255;
	if (MaxGreen > 255)	MaxGreen = 255;
	if (MaxBlue > 255)	MaxBlue = 255;
	
	OLE_COLOR color1 = RGB(MinRed, MinGreen, MinBlue);
	OLE_COLOR color2 = RGB(MaxRed, MaxGreen, MaxBlue);
	SetColors(color1, color2);
	return S_OK;
}

// ***************************************************************
//	 SetColors4()
// ***************************************************************
STDMETHODIMP CColorScheme::SetColors4 (PredefinedColorScheme Scheme)
{
	vector<OLE_COLOR> colors;
	
	switch (Scheme)
	{
		case SummerMountains:
			colors.push_back(RGB(10, 100, 10));
			colors.push_back(RGB(153, 125, 25));
			colors.push_back(RGB(255, 255, 255));
			break;
		case FallLeaves:
			colors.push_back(RGB(10, 100, 10));
			colors.push_back(RGB(199, 130, 61));
			colors.push_back(RGB(241, 220, 133));
				break;				
		case Desert:
			colors.push_back(RGB(211, 206, 97));
			colors.push_back(RGB(139, 120, 112));
			colors.push_back(RGB(255, 255, 255));
				break;				
		case Glaciers:
			colors.push_back(RGB(105, 171, 224));
			colors.push_back(RGB(162, 234, 240));
			colors.push_back(RGB(255, 255, 255));
				break;				
		case Meadow:
			colors.push_back(RGB(68, 128, 71));
			colors.push_back(RGB(43, 91, 30));
			colors.push_back(RGB(167, 220, 168));
				break;				
		case ValleyFires:
			colors.push_back(RGB(164, 0, 0));
			colors.push_back(RGB(255, 128, 64));
			colors.push_back(RGB(255, 255, 191));
				break;				
		case DeadSea:
			colors.push_back(RGB(51, 137, 208));
			colors.push_back(RGB(226, 227, 166));
			colors.push_back(RGB(151, 146, 117));
				break;				
		case Highway1:
			colors.push_back(RGB(51, 137, 208));
			colors.push_back(RGB(214, 207, 124));
			colors.push_back(RGB(54, 152, 69));
				break;		
		case Rainbow:
		case ReversedRainbow:
				
			tkMapColor colorNames[] = { tkMapColor::Red, 
										tkMapColor::Orange, 
										tkMapColor::Yellow,
										tkMapColor::LightGreen, 
										tkMapColor::LightBlue, 
										tkMapColor::DodgerBlue, 
										tkMapColor::DarkBlue };
			for (int i = 0; i < 7; i++)
			{
				OLE_COLOR color;
				GetUtils()->ColorByName(colorNames[i], &color);
				colors.push_back(color);
			}

			if (Scheme == ReversedRainbow)
			{
				std::reverse(colors.begin(), colors.end());
			}
	}

	_breaks.clear();

	int size = colors.size();
	for (int i = 0; i < size; i++)
	{
		ColorBreak br;
		br.color = colors[i];
		br.value = i  * 1.0 / (size - 1);
		_breaks.push_back(br);
	}
	return S_OK;
}

// ***************************************************************
//	 Add()
// ***************************************************************
STDMETHODIMP CColorScheme::AddBreak(double Value, OLE_COLOR Color) //, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if (Value > 1.0) 
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return S_OK;
	}
	else if (Value < 0.0) 
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return S_OK;
	}
	
	ColorBreak br;
	if (_breaks.size() == 0)	
	{
		br.value = 0.0;
	}
	else
	{
		for (unsigned int i = 0; i < _breaks.size(); i++)
		{
			if (Value <= _breaks[i].value)
			{
				// (value of the next break must be greater than previous)
				ErrorMessage(tkINVALID_PARAMETER_VALUE);		
				return S_OK;
			}
			br.value = Value;
		}
	}
	br.color = Color;
	_breaks.push_back(br);
	return S_OK;
}

// ***************************************************************
//	 Remove()
// ***************************************************************
STDMETHODIMP CColorScheme::Remove(long Index, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index >= 0 && Index < (long)_breaks.size())
	{
		_breaks.erase(_breaks.begin() + Index);
		*retVal = VARIANT_TRUE;
	}
	else
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);	
		*retVal = VARIANT_FALSE;
	}
	return S_OK;
}

// ***************************************************************
//	 get_NumBreaks()
// ***************************************************************
STDMETHODIMP CColorScheme::get_NumBreaks(long * retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _breaks.size();
	return S_OK;
}

// ***************************************************************
//	 Clear()
// ***************************************************************
STDMETHODIMP CColorScheme::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_breaks.clear();
	return S_OK;
}

// ********************************************************************
//	 GenerateRandomColor()
// ********************************************************************
STDMETHODIMP CColorScheme::get_RandomColor(double Value, OLE_COLOR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	short r1 = 0, g1 = 0, b1 = 0, r2 = 0, g2 = 0, b2 = 0;

	if ( _breaks.size() == 1)
	{
		*retVal = _breaks[0].color;
	}
	else if ( _breaks.size() == 0 )
	{
		r1 = g1 = b1 = 0;
		r2 = g2 = b2 = 255;
	}
	else
	{
		if (Value > 1.0)	Value = 1.0;
		if (Value < 0.0)	Value = 0.0;
		
		//seeking the break in which the specified value falls
		int index = 0;
		for (unsigned int i = 1; i < _breaks.size(); i++)
		{
			if (Value >= _breaks[i - 1].value &&
				Value <= _breaks[i].value)
			{
				index = i;
				break;
			}
		}
		
		// the specified value is too big (there is no such big value in color scheme), we'll use the last interval in this case
		if (index == 0)
		{
			index = _breaks.size() - 1;
			Value = _breaks[index].value;
		}

		r1 = GetRValue(_breaks[index - 1].color);
		g1 = GetGValue(_breaks[index - 1].color);
		b1 = GetBValue(_breaks[index - 1].color);
		r2 = GetRValue(_breaks[index].color);
		g2 = GetGValue(_breaks[index].color);
		b2 = GetBValue(_breaks[index].color);
	}
	
	short r = r1 + short(double(rand()/double(RAND_MAX) * (r2 - r1)));
	short g = g1 + short(double(rand()/double(RAND_MAX) * (g2 - g1)));
	short b = b1 + short(double(rand()/double(RAND_MAX) * (b2 - b1)));
	
	OLE_COLOR color = RGB(r, g, b);
	*retVal = color;
	return S_OK;
}

// ********************************************************************
//	 GenerateRandomColor()
// ********************************************************************
STDMETHODIMP CColorScheme::get_GraduatedColor(double Value, OLE_COLOR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if ( _breaks.size() == 0 )
	{
		*retVal = RGB(255, 255, 255);
		return S_OK;
	}
	else if ( _breaks.size() == 1)
	{
		*retVal = _breaks[0].color;
		return S_OK;
	}
	else
	{
		if (Value > 1.0)	Value = 1.0;
		if (Value < 0.0)	Value = 0.0;
		
		//seeking the break in which the specified value falls
		int index = 0;
		for (unsigned int i = 1; i < _breaks.size(); i++)
		{
			if (Value >= _breaks[i - 1].value &&
				Value <= _breaks[i].value)
			{
				index = i;
				break;
			}
		}
		
		// the specified value is to big (there is no such big value in color scheme), we'll use the last interval in this case
		if (index == 0)
		{
			index = _breaks.size() - 1;
			Value = _breaks[index].value;
		}
		
		short r1, g1, b1, r2, g2, b2;
		r1 = GetRValue(_breaks[index - 1].color);
		g1 = GetGValue(_breaks[index - 1].color);
		b1 = GetBValue(_breaks[index - 1].color);
		r2 = GetRValue(_breaks[index].color);
		g2 = GetGValue(_breaks[index].color);
		b2 = GetBValue(_breaks[index].color);
		
		double ratio = (Value - _breaks[index - 1].value)/(_breaks[index].value - _breaks[index - 1].value);
	
		long r, g, b ;
		r = r1 + static_cast<long>(double(r2 - r1) * ratio);
		g = g1 + static_cast<long>(double(g2 - g1) * ratio);
		b = b1 + static_cast<long>(double(b2 - b1) * ratio);
		OLE_COLOR color = RGB(r, g, b);
		*retVal = color;
		return S_OK;
	}
}

// ********************************************************************
//	 get_BreakColor()
// ********************************************************************
STDMETHODIMP CColorScheme::get_BreakColor (long Index, OLE_COLOR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())	
	if (Index >= 0 && Index < (long)_breaks.size())
	{
		*retVal = _breaks[Index].color;
	}
	else
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
	return S_OK;
}

// ********************************************************************
//	 put_BreakColor()
// ********************************************************************
STDMETHODIMP CColorScheme::put_BreakColor (long Index, OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())	
	if (Index >= 0 && Index < (long)_breaks.size())
	{
		_breaks[Index].color = newVal;
	}
	else
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);

	return S_OK;
}

// ********************************************************************
//	 get_BreakValue()
// ********************************************************************
STDMETHODIMP CColorScheme::get_BreakValue (long Index, double* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())	
	if (Index >= 0 && Index < (long)_breaks.size())
	{
		*retVal = _breaks[Index].value;
	}
	else
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);

	return S_OK;
}

//***********************************************************************/
//*		get_Key()
//***********************************************************************/
STDMETHODIMP CColorScheme::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return S_OK;
}
STDMETHODIMP CColorScheme::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_key);
	USES_CONVERSION;
	_key = OLE2BSTR(newVal);
	return S_OK;
}

//***********************************************************************/
//*			ErrorMessage()
//***********************************************************************/
void CColorScheme::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("ColorScheme", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}
STDMETHODIMP CColorScheme::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return S_OK;
}
STDMETHODIMP CColorScheme::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return S_OK;
}
STDMETHODIMP CColorScheme::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	if( _globalCallback != NULL ) 
		_globalCallback->AddRef();
	return S_OK;
}
STDMETHODIMP CColorScheme::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ComHelper::SetRef(newVal, (IDispatch**)&_globalCallback);
	return S_OK;
}

// ***********************************************************************
// *		Reverse()
// ***********************************************************************
STDMETHODIMP CColorScheme::Reverse()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::reverse(_breaks.begin(), _breaks.end());

	for (size_t i = 0; i < _breaks.size(); i++)
	{
		_breaks[i].value = 1.0 - _breaks[i].value;
	}

	return S_OK;
}