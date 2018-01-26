#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2009/05/28 13:32 by Benjamin Baldacci
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONFIG_BASE_H__
#define __KR_CONFIG_BASE_H__

#include "hh_engine_utils/include/eu_moduleconfig.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CModuleConfigCore), public CModuleConfigEngine
{
private:
	HBO_FIELD(float,	MaxFPS);
	HBO_FIELD(float,	FPSDelay);
	HBO_FIELD(hh_i32,	FPSFrameSmooth);
	HBO_FIELD(hh_u32,	LogLevel);

	bool				OnLogLevelModified();

protected:
	virtual bool		VirtualLink() override;
public:
	CModuleConfigCore() : HBO_CONSTRUCT(CModuleConfigCore) {}
	~CModuleConfigCore() {}

	virtual const char	*ConfigClassString() const override { return HH_MODULECONFIG_KERNEL_CLASS("Core"); }
	static void			SelfRegister(CBaseObjectEngineConfig *confToUse);
	static void			SelfUnregister(CBaseObjectEngineConfig *confToUse);

	HBO_CLASS_DECLARATION();
};

HH_DECLARE_REFPTRCLASS(ModuleConfigCore);
HH_MODULE_CONFIG_DECLARE_EXPORTED(ModuleConfigCore, HH_ENGINEUTILS_EXPORT);

//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CModuleConfigMem), public CModuleConfigEngine
{
private:
	HBO_FIELD(bool,		EnableDebugAllocatorDump);
	HBO_FIELD(CString,	DebugAllocatorDumpFile);
	HBO_FIELD(CString,	SourceFilesToWatch);

	bool				OnDebugAllocatorDumpFileModified();
	bool				OnEnableDebugAllocatorDumpModified();
	bool				OnFilesToWatchModified(const CString &oldFilesString);
	TArray<CString>		m_WatchedLocationsStorage;

protected:
	virtual bool		VirtualLink() override;

public:
	CModuleConfigMem() : HBO_CONSTRUCT(CModuleConfigMem) {}
	~CModuleConfigMem();

	virtual const char	*ConfigClassString() const override { return HH_MODULECONFIG_KERNEL_CLASS("Mem"); }
	static void			SelfRegister(CBaseObjectEngineConfig *confToUse);
	static void			SelfUnregister(CBaseObjectEngineConfig *confToUse);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ModuleConfigMem);
HH_MODULE_CONFIG_DECLARE_EXPORTED(ModuleConfigMem, HH_ENGINEUTILS_EXPORT);

//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CModuleConfigCoreDebug), public CModuleConfigEngine
{
private:
	HBO_FIELD(bool,		DebugBool_0);
	HBO_FIELD(bool,		DebugBool_1);
	HBO_FIELD(bool,		DebugBool_2);
	HBO_FIELD(bool,		DebugBool_3);
	HBO_FIELD(bool,		DebugBool_4);
	HBO_FIELD(bool,		DebugBool_5);
	HBO_FIELD(bool,		DebugBool_6);
	HBO_FIELD(bool,		DebugBool_7);
	HBO_FIELD(float,	DebugScalar_0);
	HBO_FIELD(float,	DebugScalar_1);
	HBO_FIELD(float,	DebugScalar_2);
	HBO_FIELD(float,	DebugScalar_3);
	HBO_FIELD(float,	DebugScalar_4);
	HBO_FIELD(float,	DebugScalar_5);
	HBO_FIELD(float,	DebugScalar_6);
	HBO_FIELD(float,	DebugScalar_7);

protected:
public:
	CModuleConfigCoreDebug() : HBO_CONSTRUCT(CModuleConfigCoreDebug) {}
	~CModuleConfigCoreDebug() {}

	virtual const char	*ConfigClassString() const override { return HH_MODULECONFIG_KERNEL_CLASS("CoreDebug"); }
	static void			SelfRegister(CBaseObjectEngineConfig *confToUse);
	static void			SelfUnregister(CBaseObjectEngineConfig *confToUse);
	HBO_CLASS_DECLARATION();
};

HH_DECLARE_REFPTRCLASS(ModuleConfigCoreDebug);
HH_MODULE_CONFIG_DECLARE_EXPORTED(ModuleConfigCoreDebug, HH_ENGINEUTILS_EXPORT);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONFIG_BASE_H__
