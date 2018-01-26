#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2008/06/28 10:06 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HELLHEAVEN_API_LIBRARY_H__
#define	__HELLHEAVEN_API_LIBRARY_H__

#if	defined(HH_WINDOWS)
#	include <assert.h>
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>
class	THHLibrary : public _Type
{
private:
	static bool			m_Started;
#if		defined(HH_WINDOWS)
	static HINSTANCE	m_ModuleInstance;
#else
	//FIXME:
#endif

public:
#if		defined(HH_WINDOWS)
	static void	RegisterLibraryEntryPoint(HINSTANCE hInst)
	{
		if (m_ModuleInstance == NULL)
			m_ModuleInstance = hInst;
		else
		{
			assert(m_ModuleInstance == hInst);
		}
	}
#else
	//FIXME:
#endif

	static bool	Startup(const SDllVersion &version, const typename _Type::Config &config);
	static bool	Shutdown();

	static bool	Started() { return m_Started; }
};

//----------------------------------------------------------------------------

template<typename _Type>
bool	THHLibrary<_Type>::m_Started = false;

#if		defined(HH_WINDOWS)
template<typename _Type>
HINSTANCE	THHLibrary<_Type>::m_ModuleInstance = NULL;
#else
//FIXME:
#endif

//----------------------------------------------------------------------------

template<typename _Type>
bool	THHLibrary<_Type>::Startup(const SDllVersion &version, const typename _Type::Config &config)
{
	(void)version;
#ifndef HH_API_LIB
#if	defined(HH_WINDOWS)
	//HH_ASSERT_MESSAGE(m_ModuleInstance != NULL, "DllMain() was not called!\n");
	if (m_ModuleInstance != null && HHTKGetVersionInfo(m_ModuleInstance) != version)
	{
		//HH_ASSERT_MESSAGE__AWAKE(0, "Dll version mismatch ...");
		return false;
	}
#else
	HH_FIXME("implement this");
#endif
#endif
	if (m_Started == true)
	{
#if	defined(HH_WINDOWS)
		char	buffer[2048];
		SNativeStringUtils::SPrintf(buffer, "%s ALREADY INITIALIZED, SUCCESSIVE CALLS TO Startup() WITHOUT MATCHING CALLS TO Shutdown() ARE FORBIDDEN!\n", _Type::LibraryName());
		OutputDebugStringA(buffer);
#endif
		return false;
	}

	if (_Type::InternalStartup(config))
	{
		m_Started = true;
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------

template<typename _Type>
bool	THHLibrary<_Type>::Shutdown()
{
	if (m_Started == false)
	{
#if	defined(HH_WINDOWS)
		char	buffer[2048];
		SNativeStringUtils::SPrintf(buffer, "%s ALREADY RELEASED, OR NOT YET INITIALIZED, SUCCESSIVE CALLS TO Shutdown() WITHOUT MATCHING CALLS TO Startup() ARE FORBIDDEN!\n", _Type::LibraryName());
		OutputDebugStringA(buffer);
#endif
		return false;
	}

	if (_Type::InternalShutdown())
	{
		m_Started = false;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HELLHEAVEN_API_LIBRARY_H__
