//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "Logging/TokenizedMessage.h"
#include "Logging/LogVerbosity.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_log_listeners.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFX, Log, All);

//----------------------------------------------------------------------------

namespace
{
	const EMessageSeverity::Type			kSeverityToUE[] = {
		EMessageSeverity::Info,				//	Level_Info = 0,
		EMessageSeverity::Warning,			//	Level_Warning,
		EMessageSeverity::Error,			//	Level_Error,
		EMessageSeverity::CriticalError,	//	Level_ErrorCritical,
		EMessageSeverity::Error,			//	Level_ErrorInternal,	// triggered by kr_assert, if you need to use one explicitely, prefer using an assert, or a release_assert
		EMessageSeverity::CriticalError,	//	Level_ErrorInferno,
		EMessageSeverity::Info,				//	Level_None,	// used mainly to disable all logging when calling 'SetGlobalLogLevel()'
	};

	const ELogVerbosity::Type				kLogVerbosityToUE[] = {
		ELogVerbosity::Log,					//	Level_Info = 0,
		ELogVerbosity::Warning,				//	Level_Warning,
		// Errors will block pakaging
		ELogVerbosity::Warning, // ELogVerbosity::Error,				//	Level_Error,
		ELogVerbosity::Warning,				//	Level_ErrorCritical,
		ELogVerbosity::Fatal,				//	Level_ErrorInternal,	// triggered by kr_assert, if you need to use one explicitely, prefer using an assert, or a release_assert
		ELogVerbosity::Fatal,				//	Level_ErrorInferno,
		ELogVerbosity::Log,					//	Level_None,	// used mainly to disable all logging when calling 'SetGlobalLogLevel()'
	};

	HH_STATIC_ASSERT(HH_ARRAY_COUNT(kSeverityToUE) == HH_ARRAY_COUNT(kLogVerbosityToUE));

	class	CLogListenerUE : public PopcornFX::ILogListener
	{
	public:
		CLogListenerUE()
		{
		}

		virtual void	Notify(PopcornFX::CLog::ELogLevel level, PopcornFX::CGuid logClass, const char *message) override
		{
#ifndef HH_RETAIL
			HH_ASSERT(level < HH_ARRAY_COUNT(kSeverityToUE));
			PopcornFX::CString	s = PopcornFX::CString::Format("[%d][%s]> %s", level, PopcornFX::CLog::LogClassToString(logClass), message);
			if (!LogPopcornFX.IsSuppressed(kLogVerbosityToUE[level]))
			{
				const ELogVerbosity::Type		logLevel = kLogVerbosityToUE[level];
				FMsg::Logf(__FILE__, __LINE__, LogPopcornFX.GetCategoryName(), logLevel, TEXT("%s"), ANSI_TO_TCHAR(s.Data()));
			}
#endif
		}
	private:
	};
} // namespace

//----------------------------------------------------------------------------

void	AddDefaultGlobalListenersOverride(void *userHandle)
{
	PopcornFX::CLog::AddGlobalListener(HH_NEW(CLogListenerUE));
}

//----------------------------------------------------------------------------
