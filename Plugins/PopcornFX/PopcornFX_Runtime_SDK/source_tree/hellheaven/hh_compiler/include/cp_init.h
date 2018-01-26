#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2012/01/07 07:27 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__CP_INIT_H__
#define	__CP_INIT_H__

#include "hh_compiler/include/cp_config.h"

#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hellheaven_api_library.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_COMPILER_EXPORT CHHCompilerBase
{
public:
	struct	Config
	{
	};

protected:
	static bool			InternalStartup(const Config &config);
	static bool			InternalShutdown();

	static bool			m_Active;
public:
	static const char	*LibraryName() { return "HH-Compiler"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CHHCompilerBase>		CHHCompiler;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
// template class HH_COMPILER_EXPORT		THHLibrary<CHHCompilerBase>;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __CP_INIT_H__
