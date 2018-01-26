#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/01/08 11:34 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------


#include "hh_toolkit/include/hh_toolkit_version.h"
#include "hellheaven_api_library.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	PK_BILLBOARDER_EXPORT CPKBillboarderBase
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
	static const char	*LibraryName() { return "PK-Billboarder"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CPKBillboarderBase>	CPKBillboarder;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
//template class HH_PARTICLES_EXPORT		THHLibrary<CHHParticlesBase>;

//----------------------------------------------------------------------------
__PK_API_END
