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

#ifndef	__HB_INIT_H__
#define	__HB_INIT_H__

#include "hh_base_object/include/hbo_config.h"

#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hellheaven_api_library.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_BASEOBJECT_EXPORT CHHBaseObjectBase
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
	static const char	*LibraryName() { return "HH-BaseObject"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CHHBaseObjectBase>	CHHBaseObject;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
// template class HH_BASEOBJECT_EXPORT		THHLibrary<CHHBaseObjectBase>;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __HB_INIT_H__
