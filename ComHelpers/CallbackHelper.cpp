#include "StdAfx.h"
#include "CallbackHelper.h"

// ReSharper disable CppUseAuto

// *****************************************************************
//	   GDALProgressCallback()
// *****************************************************************
int CPL_STDCALL GDALProgressCallback(const double dfComplete, const char* pszMessage, void *pData)
{
	CallbackParams* const params = static_cast<CallbackParams*>(pData);

	// No need to check the presence of local callback, 
	// global application callback can be used as a fallback.
	// There is no need to pass it as a parameter from each method.
	if (params != nullptr)
	{
		const long percent = long(dfComplete * 100.0);
		CallbackHelper::Progress(params->cBack, percent, params->sMsg);
	}
	return TRUE;
}

// ********************************************************************
//		DisplayProgress()
// ********************************************************************
void CallbackHelper::Progress(ICallback* localCback, int index, int count, const char* message, long& lastPercent)
{
	Progress(localCback, index, static_cast<double>(count), message, m_globalSettings.emptyBstr, lastPercent);
}

void CallbackHelper::Progress(ICallback* localCback, int index, int count, const char* message, BSTR& key, long& lastPercent)
{
	Progress(localCback, index, static_cast<double>(count), message, key, lastPercent);
}

// ********************************************************************
//		GetCurrent()
// ********************************************************************
ICallback* CallbackHelper::GetCurrent(ICallback* localCback)
{
	if (m_globalSettings.overrideLocalCallback)
	{
		return m_globalSettings.callback ? m_globalSettings.callback : localCback;
	}
	else
	{
		return localCback ? localCback : m_globalSettings.callback;
	}
}

// ********************************************************************
//		DisplayProgress()
// ********************************************************************
void CallbackHelper::Progress(ICallback* localCback, int index, double count, const char* message, BSTR& key, long& lastPercent)
{
	ICallback* callback = GetCurrent(localCback);
	if (!callback) return;

	const long newpercent = static_cast<long>(static_cast<double>(index + 1) / count * 100);
	if (newpercent > lastPercent)
	{
		lastPercent = newpercent;
		const CComBSTR bstrMsg(message);
		callback->Progress(key, newpercent, bstrMsg);
	}
}

// ********************************************************************
//		DisplayProgress()
// ********************************************************************
void CallbackHelper::Progress(ICallback* localCback, int percent, const char* message, BSTR& key)
{
	ICallback* callback = GetCurrent(localCback);
	if (!callback) return;

	const CComBSTR bstrMsg(message);
	callback->Progress(key, percent, bstrMsg);
}

// ********************************************************************
//		DisplayProgress()
// ********************************************************************
void CallbackHelper::Progress(ICallback* localCback, int percent, const char* message)
{
	ICallback* callback = GetCurrent(localCback);
	if (!callback) return;

	if (!message) message = "";
	const CComBSTR bstrMsg(message);
	callback->Progress(m_globalSettings.emptyBstr, percent, bstrMsg);
}

// ********************************************************************
//		DisplayProgressCompleted()
// ********************************************************************
void CallbackHelper::ProgressCompleted(ICallback* localCback, BSTR& key)
{
	ICallback* callback = GetCurrent(localCback);
	if (!callback) return;

	const CComBSTR bstrMsg("Completed");
	callback->Progress(key, 100, bstrMsg);
	callback->Progress(key, 0, m_globalSettings.emptyBstr);
}

// ********************************************************************
//		DisplayProgressCompleted()
// ********************************************************************
void CallbackHelper::ProgressCompleted(ICallback* localCback)
{
	ProgressCompleted(localCback, m_globalSettings.emptyBstr);
}

// ********************************************************************
//		DisplayErrorMsg()
// ********************************************************************
void CallbackHelper::ErrorMsg(const CString className, ICallback* localCback, BSTR& key, const char* message, ...)
{
	ICallback* callback = GetCurrent(localCback);

	if (callback || Debug::IsDebugMode())
	{
		if (strcmp(message, "No Error") == 0) return;

		TCHAR buffer[1024];
		va_list args;
		va_start(args, message);
		vsprintf(buffer, message, args);
		CString s = buffer;

		s = className + ": " + s;
		const CComBSTR bstr(s);

		if (callback) {
			callback->Error(key, bstr);
		}
		else {
			Debug::WriteError(s);
		}
	}
}
void CallbackHelper::ErrorMsg(CString className, ICallback* localCallback, CString key, const char* message, ...)
{
	if (localCallback || Debug::IsDebugMode())
	{
		CComBSTR bstrKey(key);
		ErrorMsg(className, localCallback, bstrKey.m_str, message);
	}
}

void CallbackHelper::ErrorMsg(const char* message)
{
	const CString s = message;
	ErrorMsg(s);
}

void CallbackHelper::ErrorMsg(CString message)
{
	if (m_globalSettings.callback)
	{
		const CComBSTR bstr(message);
		m_globalSettings.callback->Error(m_globalSettings.emptyBstr, bstr);
	}
	else {
		if (Debug::IsDebugMode())
			Debug::WriteError(message);
	}
}

// ****************************************************************
//		AssertionFailed
// ****************************************************************
void CallbackHelper::AssertionFailed(CString message)
{
	message = "Assertion failed: " + message;
	if (m_globalSettings.callback)
	{
		const CComBSTR bstr(message);
		m_globalSettings.callback->Error(m_globalSettings.emptyBstr, bstr);
	}
	else {
		if (Debug::IsDebugMode())
			Debug::WriteError(message);
	}
}

// ReSharper restore CppUseAuto
