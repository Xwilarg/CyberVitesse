#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2011/04/02 17:27 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_ENGINE_CONFIG_H__
#define __HBO_ENGINE_CONFIG_H__

#include "hh_base_object/include/hbo_object.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBaseObjectModuleInitializer;

//----------------------------------------------------------------------------

class	HH_BASEOBJECT_EXPORT CBaseObjectEngineConfig
{
private:
//	static CBaseObjectEngineConfig	*m_Self;

	TArray<const CBaseObjectModuleInitializer *>	m_Initializers;
	HBO::CContext									*m_Context;
	PBaseObjectFile									m_ConfigFile;
	CString											m_OriginalFilePath;
	CString											m_OriginalBasePack;

protected:
public:
	CBaseObjectEngineConfig(const char *path, const char *originaBasePack);
	~CBaseObjectEngineConfig();

//	static bool		Instantiate();	// FIXME: move these two somewhere where the standard user can't call them
//	static void		Release();

	bool		EngineConfigRegisterModule(const CBaseObjectModuleInitializer *initializer);
	bool		EngineConfigUnregisterModule(const CBaseObjectModuleInitializer *initializer);

	void		EngineConfigLoad(const CString &engineConfigFilePath);
	void		EngineConfigSave(const CString &engineConfigFilePath);
};

//----------------------------------------------------------------------------

#define	HH_MODULE_CONFIG_DECLARE_EXPORTED(__name, __export)	extern __export C ## __name	*g_ ## __name
#define	HH_MODULE_CONFIG_DECLARE(__name)					HH_MODULE_CONFIG_DECLARE_EXPORTED(__name, NOTHING)

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_ENGINE_CONFIG_H__
