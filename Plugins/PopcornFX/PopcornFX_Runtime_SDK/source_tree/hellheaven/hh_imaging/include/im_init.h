#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2010/09/04 15:09 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__IM_INIT_H__
#define	__IM_INIT_H__

#include "hh_imaging/include/im_config.h"

#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hellheaven_api_library.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_IMAGING_EXPORT CHHImagingBase
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
	static const char	*LibraryName() { return "HH-Imaging"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CHHImagingBase>	CHHImaging;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
// template class	HH_IMAGING_EXPORT THHLibrary<CHHImagingBase>;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __IM_INIT_H__
