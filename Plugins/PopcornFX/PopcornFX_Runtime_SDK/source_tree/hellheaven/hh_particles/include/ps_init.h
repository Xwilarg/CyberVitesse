#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2010/06/30 15:59 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_INIT_H__
#define	__PS_INIT_H__

#include "hh_particles/include/ps_config.h"

#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hellheaven_api_library.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CHHParticlesBase
{
public:
	struct	Config
	{
		bool			m_GloballyEnableSpawnerIDs;
		bool			m_MeshRenderersHavePrevXForms;

		Config()
		:	m_GloballyEnableSpawnerIDs(false)
		,	m_MeshRenderersHavePrevXForms(false)
		{ }
	};

	// Won't be effective for any previously loaded FX
	static void			UnsafeSetGloballyEnableSpawnerIDs(bool enable);
	static void			UnsafeSetMeshRenderersHavePrevXForms(bool enable);

protected:
	static bool			InternalStartup(const Config &config);
	static bool			InternalShutdown();

	static bool			m_Active;
public:
	static const char	*LibraryName() { return "HH-Particles"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CHHParticlesBase>	CHHParticles;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
//template class HH_PARTICLES_EXPORT		THHLibrary<CHHParticlesBase>;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_INIT_H__
