#pragma once

//----------------------------------------------------------------------------
// Started on Sun 2010/04/02 11:52 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__GE_INIT_H__
#define	__GE_INIT_H__

#include "hh_geometrics/include/ge_config.h"

#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hellheaven_api_library.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CHHGeometricsBase
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
	static const char	*LibraryName() { return "HH-Geometrics"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CHHGeometricsBase>	CHHGeometrics;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
// template class	HH_GEOMETRICS_EXPORT THHLibrary<CHHGeometricsBase>;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __GE_INIT_H__
