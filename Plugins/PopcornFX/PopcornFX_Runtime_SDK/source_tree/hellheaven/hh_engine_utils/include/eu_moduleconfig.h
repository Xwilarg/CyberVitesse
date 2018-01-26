#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2008/01/28 12:24 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __EU_MODULECONFIG_H__
#define __EU_MODULECONFIG_H__

#include "hh_base_object/include/hbo_engine_config.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

#define	HH_MODULECONFIG_KERNEL_CLASS(__subConfig)		"Kernel::" __subConfig
#define	HH_MODULECONFIG_ENGINE_CLASS(__subConfig)		"Engine::" __subConfig

#define	HH_MODULECONFIG_BASE_CLASS(__subConfig)			HH_MODULECONFIG_ENGINE_CLASS("Base::" __subConfig)
#define	HH_MODULECONFIG_GRAPHICS_CLASS(__subConfig)		HH_MODULECONFIG_ENGINE_CLASS("Graphics::" __subConfig)
#define	HH_MODULECONFIG_PHYSICS_CLASS(__subConfig)		HH_MODULECONFIG_ENGINE_CLASS("Physics::" __subConfig)
#define	HH_MODULECONFIG_SOUND_CLASS(__subConfig)		HH_MODULECONFIG_ENGINE_CLASS("Sound::" __subConfig)
#define	HH_MODULECONFIG_INPUT_CLASS(__subConfig)		HH_MODULECONFIG_ENGINE_CLASS("Input::" __subConfig)
#define	HH_MODULECONFIG_ENTITY_CLASS(__subConfig)		HH_MODULECONFIG_ENGINE_CLASS("Entities::" __subConfig)

//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CModuleConfigEngine), public CBaseObject
{
private:
protected:
public:
	CModuleConfigEngine() : HBO_CONSTRUCT(CModuleConfigEngine) {}
	~CModuleConfigEngine() {}

	virtual const char	*ConfigClassString() const { return null; }

	HBO_CLASS_DECLARATION();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __EU_MODULECONFIG_H__
