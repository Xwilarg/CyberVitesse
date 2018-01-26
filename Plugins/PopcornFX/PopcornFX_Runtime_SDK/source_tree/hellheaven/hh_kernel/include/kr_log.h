#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/01 03:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_LOG_H__
#define	__KR_LOG_H__

#include "hh_kernel/include/kr_string.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

#ifndef		PV_MODULE_NAME
#	define	PV_MODULE_NAME	"Default"
#endif
#ifndef		PV_MODULE_SYM
#	define	PV_MODULE_SYM	Main
#endif

//----------------------------------------------------------------------------
//
//	example usage:
//	CLog::Log(HH_WARNING, "stuff @ %p is broken! %d %.3f", address, 10, 42.69f);
//	CLog::Log(HH_ERROR_CRITICAL, "out of memory");
//	...
//
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE_INTERFACE(Logger);
HH_FORWARD_DECLARE_INTERFACE(LogListener);

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CLog
{
private:
	friend class	CLogInternals;

	static bool		Startup();
	static void		Shutdown();
public:

	enum	ELogLevel
	{
		Level_Info = 0,
		Level_Warning,
		Level_Error,
		Level_ErrorCritical,
		Level_ErrorInternal,	// triggered by kr_assert, if you need to use one explicitely, prefer using an assert, or a release_assert
		Level_ErrorInferno,

		Level_None,	// used mainly to disable all logging when calling 'SetGlobalLogLevel()'
		__MaxLogLevels
	};

	enum	EModuleBootMode
	{
		MBoot_StartupBegin = 0,
		MBoot_StartupEnd,
		MBoot_ShutdownBegin,
		MBoot_ShutdownEnd,
	};

	static void					AddGlobalListener(PLogListener listener);
	static void					RemoveGlobalListener(PLogListener listener);
	static void					NotifyAllGlobalListeners(CLog::ELogLevel level, CGuid logClass, const char *message);

	static CGuid				RegisterLogClass(const char *logClass);
	static bool					UnregisterLogClass(CGuid logClassGUID, const char *logClass);
	static const char			*LogClassToString(CGuid logClass);

	static void					SetGlobalLogger(PLogger logger);	// passing null will bind the default engine logger
	static void					SetGlobalLogLevel(ELogLevel level);

#ifndef	HH_RETAIL

	static void					Log(ELogLevel level, CGuid logClass, const char *format, ...) PK_COMPILER_ATTRIB_PRINTF(3, 4);
	static void					Log(ELogLevel level, CGuid logClass, const CString &string);

#else
	// entirely disable the logging calls inside the code.
	// some of the compilers we use don't like that at all:
//	HH_FORCEINLINE static void	Log(ELogLevel level, CGuid logClass, const char *format, ...) {}	// should compile-out to nothing, except potential calls in arg-passing

	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const CString &) {}
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *) {}
	template<typename _T0>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0) {}
	template<typename _T0, typename _T1>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1) {}
	template<typename _T0, typename _T1, typename _T2>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2) {}
	template<typename _T0, typename _T1, typename _T2, typename _T3>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2, _T3) {}
	template<typename _T0, typename _T1, typename _T2, typename _T3, typename _T4>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2, _T3, _T4) {}
	template<typename _T0, typename _T1, typename _T2, typename _T3, typename _T4, typename _T5>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2, _T3, _T4, _T5) {}
	template<typename _T0, typename _T1, typename _T2, typename _T3, typename _T4, typename _T5, typename _T6>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2, _T3, _T4, _T5, _T6) {}
	template<typename _T0, typename _T1, typename _T2, typename _T3, typename _T4, typename _T5, typename _T6, typename _T7>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7) {}
	template<typename _T0, typename _T1, typename _T2, typename _T3, typename _T4, typename _T5, typename _T6, typename _T7, typename _T8>
	HH_FORCEINLINE static void	Log(ELogLevel, CGuid, const char *, _T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8) {}

#endif

	static void					LogModuleBootMessage(CLog::EModuleBootMode level, CGuid logClass, const char *moduleInitName);
	static void					LogCoreDllBootMessage(const char *message);
};

//----------------------------------------------------------------------------

#define	HH_INFO					PopcornFX::CLog::Level_Info,			PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME()
#define	HH_WARNING				PopcornFX::CLog::Level_Warning,			PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME()
#define	HH_ERROR				PopcornFX::CLog::Level_Error,			PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME()
#define	HH_ERROR_CRITICAL		PopcornFX::CLog::Level_ErrorCritical,	PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME()
#define	HH_ERROR_INFERNO		PopcornFX::CLog::Level_ErrorInferno,	PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME()

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT ILogListener : public CRefCountedObject
{
public:
	virtual ~ILogListener() {}

	virtual void	OnAttach(PLogger attachedTo) { (void)attachedTo; }
	virtual void	OnDetach(PLogger detachedFrom) { (void)detachedFrom; }

	virtual void	Notify(CLog::ELogLevel level, CGuid logClass, const char *message) = 0;
};

//----------------------------------------------------------------------------
// one logger per log target class, ex: global engine log, graphics log, application's AI log, etc.. ?

class	HH_KERNEL_EXPORT ILogger : public CRefCountedObject
{
public:
	virtual ~ILogger() {}

	virtual void	AddListener(PLogListener listener) = 0;
	virtual void	RemoveListener(PLogListener listener) = 0;

	virtual void	Log(CLog::ELogLevel level, CGuid logClass, const char *message) = 0;
	virtual void	LogModuleBootMessage(CLog::EModuleBootMode level, CGuid logClass, const char *moduleInitName) = 0;
	virtual void	LogCoreDllBootMessage(const char *message) = 0;
};
HH_DECLARE_REFPTRINTERFACE(Logger);

//----------------------------------------------------------------------------

namespace	Logger
{
	typedef void	(*FnAddListeners)(void *userHandle);
}

//----------------------------------------------------------------------------

#define	HH_LOG_MODULE_CLASS_GUID_NAME()	GLUE(g_LogModuleClass_, PV_MODULE_SYM)

#define	HH_LOG_MODULE_DEFINE()			namespace PopcornFX { extern CGuid	HH_LOG_MODULE_CLASS_GUID_NAME(); }
#define	HH_LOG_MODULE_DECLARE()			namespace PopcornFX { CGuid			HH_LOG_MODULE_CLASS_GUID_NAME(); }

#define	HH_LOG_MODULE_INIT_START		do { \
											PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME() = PopcornFX::CLog::RegisterLogClass(PV_MODULE_NAME); \
											PopcornFX::CLog::LogModuleBootMessage(PopcornFX::CLog::MBoot_StartupBegin, PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME(), PV_MODULE_NAME); \
										} while (0)

#define	HH_LOG_MODULE_INIT_END			PopcornFX::CLog::LogModuleBootMessage(PopcornFX::CLog::MBoot_StartupEnd,	PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME(), PV_MODULE_NAME)

#define	HH_LOG_MODULE_RELEASE_START		PopcornFX::CLog::LogModuleBootMessage(PopcornFX::CLog::MBoot_ShutdownBegin,	PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME(), PV_MODULE_NAME)
#define	HH_LOG_MODULE_RELEASE_END		PopcornFX::CLog::LogModuleBootMessage(PopcornFX::CLog::MBoot_ShutdownEnd,	PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME(), PV_MODULE_NAME); PopcornFX::CLog::UnregisterLogClass(PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME(), PV_MODULE_NAME); PopcornFX::HH_LOG_MODULE_CLASS_GUID_NAME() = CGuid::INVALID

//----------------------------------------------------------------------------
__PK_API_END

#endif
