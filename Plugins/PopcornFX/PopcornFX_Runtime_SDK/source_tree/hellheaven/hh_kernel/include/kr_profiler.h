#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/15 23:09 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_PROFILER_H__
#define	__KR_PROFILER_H__

#include "hh_maths/include/hh_maths.h"
#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_timers.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

#ifndef	KR_PROFILER_ENABLED
#	ifdef	HH_RETAIL
#		define	KR_PROFILER_ENABLED	0
#	else
#		if	!defined(HH_PS3) /*&& !defined(HH_X360)*/ && !defined(HH_PSP2)
#			define	KR_PROFILER_ENABLED	1
#		else
#			define	KR_PROFILER_ENABLED	0
#		endif
#	endif
#endif

//----------------------------------------------------------------------------

#if	defined(HH_COMPILER_GCC) || defined(HH_COMPILER_GHS)
#	define	HH_DEFAULT_PROFILE_NAME			__FUNCTION__
#	define	HH_DEFAULT_PROFILE_LOCATION		__FILE__
#else
#	define	HH_DEFAULT_PROFILE_NAME			__FUNCTION__ "::" STRINGIFY(__COUNTER__)
#	define	HH_DEFAULT_PROFILE_LOCATION		__FILE__ "::" __FUNCTION__
#endif

#define	HH_ACTIVE_NAMEDSCOPERPROFILE(__name, __color, __uniqueCount, __flags, __enabled) \
									static const PopcornFX::Profiler::SNodeDescriptor	GLUE(__PROFILER_NAME_, __uniqueCount) \
									(__name, HH_DEFAULT_PROFILE_LOCATION, __color, hh_u32(__flags)); \
									static bool GLUE(__PROFILE_ENABLED_, __uniqueCount) = __enabled; \
									PopcornFX::Profiler::CScopedNode GLUE(__PROFILER_Key_, __uniqueCount) \
									(PopcornFX::Profiler::MainEngineProfiler(), &GLUE(__PROFILER_NAME_, __uniqueCount), GLUE(__PROFILE_ENABLED_, __uniqueCount))

#define	HH_ACTIVE_NAMEDSCOPERPROFILE_OPTIONAL(__name, __color, __uniqueCount, __flags, __enabled) \
									static const PopcornFX::Profiler::SNodeDescriptor	GLUE(__PROFILER_NAME_, __uniqueCount) \
									(__name, HH_DEFAULT_PROFILE_LOCATION, __color, hh_u32(__flags)); \
									PopcornFX::Profiler::CScopedNode GLUE(__PROFILER_Key_, __uniqueCount) \
									(PopcornFX::Profiler::MainEngineProfiler(), &GLUE(__PROFILER_NAME_, __uniqueCount), __enabled)

#if	(KR_PROFILER_ENABLED == 0)
#	define	HH_CUSTOMSCOPEDPROFILE(__name, __color, __flags, __enabled)
#	define	HH_NAMEDSCOPEDPROFILE_C(__name, __color)
#	define	HH_NAMEDSCOPEDPROFILE_OPTIONAL(__name, __color, __enabled)
#	define	HH_NAMEDSCOPEDPROFILE(__name)
#else
#	define	HH_CUSTOMSCOPEDPROFILE(__name, __color, __flags, __enabled)	HH_ACTIVE_NAMEDSCOPERPROFILE(__name, __color, __COUNTER__, __flags, __enabled)
#	define	HH_NAMEDSCOPEDPROFILE_C(__name, __color)					HH_ACTIVE_NAMEDSCOPERPROFILE(__name, __color, __COUNTER__, PopcornFX::Profiler::SNodeDescriptor::None, true)
#	define	HH_NAMEDSCOPEDPROFILE_OPTIONAL(__name, __color, __enabled)	HH_ACTIVE_NAMEDSCOPERPROFILE_OPTIONAL(__name, __color, __COUNTER__, PopcornFX::Profiler::SNodeDescriptor::None, __enabled)
#	define	HH_NAMEDSCOPEDPROFILE(__name)								HH_ACTIVE_NAMEDSCOPERPROFILE(__name, PopcornFX::Profiler::SNodeDescriptor::AutogenColor(__name), __COUNTER__, PopcornFX::Profiler::SNodeDescriptor::None, true)
#endif

#define	HH_SCOPEDPROFILE()								HH_NAMEDSCOPEDPROFILE(HH_DEFAULT_PROFILE_NAME)
#define	HH_SCOPEDPROFILE_C(__color)						HH_NAMEDSCOPEDPROFILE_C(HH_DEFAULT_PROFILE_NAME, __color)
#define	HH_SCOPEDPROFILE_OPTIONAL(__color, __enabled)	HH_NAMEDSCOPEDPROFILE_OPTIONAL(HH_DEFAULT_PROFILE_NAME, __color, __enabled)
#define	HH_SCOPEDPROFILEIDLE()							HH_CUSTOMSCOPEDPROFILE("idle", PopcornFX::Profiler::Color_Idle, PopcornFX::Profiler::SNodeDescriptor::Idle, true)
#define	HH_SCOPEDPROFILESPIN(__name)					HH_CUSTOMSCOPEDPROFILE(__name, PopcornFX::Profiler::Color_Spin, PopcornFX::Profiler::SNodeDescriptor::Idle, true)

//----------------------------------------------------------------------------

namespace	Profiler
{
	class	CProfiler;

	extern HH_KERNEL_EXPORT CProfiler				*MainEngineProfiler();
	extern HH_KERNEL_EXPORT HH_ALIGN(0x10) CFloat4	Color_Idle;
	extern HH_KERNEL_EXPORT HH_ALIGN(0x10) CFloat4	Color_Spin;

	//----------------------------------------------------------------------------

	struct	HH_KERNEL_EXPORT SNodeDescriptor
	{
		enum	EFlags
		{
			None			= 0x0,
			Idle			= 0x10000000,
			Syscall			= 0x20000000,
			TaskGroup_Mask	= 0x0FFFFFFF,
		};

		CFloat4			m_Color;
		const char		*m_Name;
		const char		*m_FileAndFunction;
		hh_u32			m_Flags;

		SNodeDescriptor(const char *name, const char *fileAndFunction, const CFloat4 &color, hh_u32 flags);
		SNodeDescriptor(const char *name, const char *fileAndFunction, const CFloat3 &color, hh_u32 flags);

		static CFloat4	AutogenColor(const char *name);
	};

	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CScopedNode
	{
	private:
		CProfiler		*m_Profiler;

	public:
		CScopedNode(CProfiler *profiler, const SNodeDescriptor *nodeDescriptor, bool enable);
		~CScopedNode();
	};

	//----------------------------------------------------------------------------

#if	(KR_PROFILER_ENABLED != 0)

	class	HH_KERNEL_EXPORT CScopedLoggingTimer
	{
	private:
		CTimer			m_Timer;
		const char		*m_Name;
	public:
		CScopedLoggingTimer(const char *name)
		:	m_Name(name)
		{
			m_Timer.Start();
		}
		~CScopedLoggingTimer();
	};

	class	HH_KERNEL_EXPORT CScopedLoggingCyclesTimer
	{
	private:
		CTimerCPUCycles	m_Timer;
		const char		*m_Name;
	public:
		CScopedLoggingCyclesTimer(const char *name)
		:	m_Name(name)
		{
			m_Timer.Start();
		}
		~CScopedLoggingCyclesTimer();
	};

#	define	HH_SCOPEDLOGGEDPROFILE(__name)			PopcornFX::Profiler::CScopedLoggingTimer			GLUE(_locprof_, __COUNTER__)(__name)
#	define	HH_SCOPEDLOGGEDPROFILECYCLES(__name)	PopcornFX::Profiler::CScopedLoggingCyclesTimer		GLUE(_locprof_, __COUNTER__)(__name)

#else

#	define	HH_SCOPEDLOGGEDPROFILE(__name)
#	define	HH_SCOPEDLOGGEDPROFILECYCLES(__name)

#endif

	//----------------------------------------------------------------------------

	// Unsafe ! for debuging purposes only !
	// only if profiler is active, will:
	// MainEngineProfiler()->Reset();
	// MainEngineProfiler()->BuildReport(...);
	// WriteProfileReport(..., pkprOutputPath);
	bool			DebugWriteCurrentFrameProfilerReportIFP(const char *pkprOutputPath);

	//----------------------------------------------------------------------------

} // namespace	Profiler

//----------------------------------------------------------------------------

#if	!defined(HH_RETAIL)

// theses are thread-safe

#	define	HH_PROFILESTATS_DECLARE(__limit, ...)				HH_PROFILESTATS_DECLARE_N(__limit, __COUNTER__, __VA_ARGS__)
#	define	HH_PROFILESTATS_CAPTURE(__name)						PopcornFX::Profiler::Internal::SScopedTimerAccum<PopcornFX::CTimer>				GLUE(TIMERACC_, __COUNTER__)(&(_PROFILESTATS2.m_Data.__name))
#	define	HH_PROFILESTATS_CAPTURE_N(__name, __count)			PopcornFX::Profiler::Internal::SScopedTimerAccum<PopcornFX::CTimer>				GLUE(TIMERACC_, __COUNTER__)(&(_PROFILESTATS2.m_Data.__name), __count)
#	define	HH_PROFILESTATS_CAPTURE_CYCLES(__name)				PopcornFX::Profiler::Internal::SScopedTimerAccum<PopcornFX::CTimerCPUCycles>	GLUE(TIMERACC_, __COUNTER__)(&(_PROFILESTATS2.m_Data.__name))
#	define	HH_PROFILESTATS_CAPTURE_CYCLES_N(__name, __count)	PopcornFX::Profiler::Internal::SScopedTimerAccum<PopcornFX::CTimerCPUCycles>	GLUE(TIMERACC_, __COUNTER__)(&(_PROFILESTATS2.m_Data.__name), __count)

namespace	Profiler
{
	namespace	Internal
	{
		enum	ETimerType
		{
			Timer_None = 0,
			Timer_Seconds,
			Timer_Cycles,
		};
		struct	SScopedTimerData
		{
			float		m_TimingAcc;
			hh_u32		m_Count;
			ETimerType	m_Type;

			SScopedTimerData() : m_TimingAcc(0.0f), m_Count(0), m_Type(Timer_None) {}

			HH_FORCEINLINE void	Add(CTimer &timer, hh_u32 count)
			{
				m_TimingAcc += timer.Stop();
				m_Count += count;
				m_Type = Timer_Seconds;
			}
			HH_FORCEINLINE void	Add(CTimerCPUCycles &timer, hh_u32 count)
			{
				m_TimingAcc += timer.Stop();
				m_Count += count;
				m_Type = Timer_Cycles;
			}
		};

		template<typename _TimerType>
		struct	SScopedTimerAccum
		{
			_TimerType			m_Timer;
			SScopedTimerData	*m_Data;
			hh_u32				m_Count;
			SScopedTimerAccum(SScopedTimerData *dst, hh_u32 count = 1) : m_Data(dst), m_Count(count) { m_Timer.Start(); }
			~SScopedTimerAccum() { m_Data->Add(m_Timer, m_Count); }
		};
	}
}

#	define	HH_VA_APPLY_ARGS(__stub, __args)	__stub __args

#	if 1
		// full generic implem
#		define	HH_VA_MACRO_COUNT_IMPL2(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, __count, ...)	__count
#		define	HH_VA_MACRO_COUNT_IMPL(__args)		HH_VA_MACRO_COUNT_IMPL2 __args
#		define	HH_VA_MACRO_COUNT(...)				HH_VA_MACRO_COUNT_IMPL((__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#		define	HH_VA_APPLY_MACRO_STUB1(__m, __1)										__m(__1)
#		define	HH_VA_APPLY_MACRO_STUB2(__m, __1, __2)									__m(__1), __m(__2)
#		define	HH_VA_APPLY_MACRO_STUB3(__m, __1, __2, __3)								__m(__1), __m(__2), __m(__3)
#		define	HH_VA_APPLY_MACRO_STUB4(__m, __1, __2, __3, __4)						__m(__1), __m(__2), __m(__3), __m(__4)
#		define	HH_VA_APPLY_MACRO_STUB5(__m, __1, __2, __3, __4, __5)					__m(__1), __m(__2), __m(__3), __m(__4), __m(__5)
#		define	HH_VA_APPLY_MACRO_STUB6(__m, __1, __2, __3, __4, __5, __6)				__m(__1), __m(__2), __m(__3), __m(__4), __m(__5), __m(__6)
#		define	HH_VA_APPLY_MACRO_STUB7(__m, __1, __2, __3, __4, __5, __6, __7)			__m(__1), __m(__2), __m(__3), __m(__4), __m(__5), __m(__6), __m(__7)
#		define	HH_VA_APPLY_MACRO_STUB8(__m, __1, __2, __3, __4, __5, __6, __7, __8)	__m(__1), __m(__2), __m(__3), __m(__4), __m(__5), __m(__6), __m(__7), __m(__8)
#		define	HH_VA_APPLY_MACRO_STUB9(__m, __1, __2, __3, __4, __5, __6, __7, __8, __9)	__m(__1), __m(__2), __m(__3), __m(__4), __m(__5), __m(__6), __m(__7), __m(__8), __m(__9)
#		define	HH_VA_APPLY_MACRO_STUB10(__m, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10)	__m(__1), __m(__2), __m(__3), __m(__4), __m(__5), __m(__6), __m(__7), __m(__8), __m(__9), __m(__10)

#		define	GLUE_LVL2(__x, __y)					GLUE(__x, __y)
#		define	HH_VA_APPLY_MACRO3(__stub, __args)	HH_VA_APPLY_ARGS(__stub, __args)
#		define	HH_VA_APPLY_MACRO(__macro, ...)		HH_VA_APPLY_MACRO3(GLUE_LVL2(HH_VA_APPLY_MACRO_STUB, HH_VA_MACRO_COUNT(__VA_ARGS__)), (__macro, __VA_ARGS__))
#		define	HH_VA_STRINGIFY(...)				HH_VA_APPLY_MACRO(STRINGIFY, __VA_ARGS__)
#	else
		// simple version : generates empty strings, but enough for what we need.
#		define	HH_VA_APPLY_MACRO_STUB10(__1, __2, __3, __4, __5, __6, __7, __8, __9, __10, ...)	#__1, #__2, #__3, #__4, #__5, #__6, #__7, #__8, #__9, #__10
#		define	HH_VA_STRINGIFY(...)				HH_VA_APPLY_ARGS(HH_VA_APPLY_MACRO_STUB10, (__VA_ARGS__,,,,,,,,,,))
#	endif

#	define	HH_APPLY_STRINGIFIED_OPERATOR_1_5_IMPL(__op, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, ...)	__op(#__1), __op(#__2), __op(#__3), __op(#__4), __op(#__5)
#	define	HH_APPLY_STRINGIFIED_OPERATOR_6_10_IMPL(__op, __1, __2, __3, __4, __5, __6, __7, __8, __9, __10, ...)	__op(#__6), __op(#__7), __op(#__8), __op(#__9), __op(#__10)
#	define	HH_APPLY_STRINGIFIED_OPERATOR_1_5(__operator, ...)	HH_VA_APPLY_ARGS(HH_APPLY_STRINGIFIED_OPERATOR_1_5_IMPL, (__operator, __VA_ARGS__,,,,,,,,,,))
#	define	HH_APPLY_STRINGIFIED_OPERATOR_6_10(__operator, ...)	HH_VA_APPLY_ARGS(HH_APPLY_STRINGIFIED_OPERATOR_6_10_IMPL, (__operator, __VA_ARGS__,,,,,,,,,,))

#	define	HH_PROFILESTATS_DECLARE_FORMATTER(__str0, __off0, __str1, __off1) \
						char	m_Format0[HH_ARRAY_COUNT(__str0)]; \
						char	m_Format1[HH_ARRAY_COUNT(__str1)]; \
						SFormatter() \
						{ \
							PopcornFX::SNativeStringUtils::StrCpyStatic(m_Format0, __str0); \
							PopcornFX::SNativeStringUtils::StrCpyStatic(m_Format1, __str1); \
							m_Format0[__off0 + 0] = m_Format1[__off1 + 0] = kSourceNameMaxLength_Digit1; \
							m_Format0[__off0 + 1] = m_Format1[__off1 + 1] = kSourceNameMaxLength_Digit0; \
						}

#	define	HH_PROFILESTATS_DECLARE_N(__limit, __counter, ...) \
	static struct GLUE(SProfStats_, __counter) { PopcornFX::hh_u32 m_Counter; struct { PopcornFX::Profiler::Internal::SScopedTimerData __VA_ARGS__; } m_Data; PopcornFX::Threads::CCriticalSection m_Lock; } _PROFILESTATS; \
	/* second non-static local structure to avoid locking for each source */ \
	struct GLUE(SProfStats2_, __counter) { struct { PopcornFX::Profiler::Internal::SScopedTimerData __VA_ARGS__; } m_Data; } _PROFILESTATS2; \
	struct SProfPrinter \
	{ \
		GLUE(SProfStats_, __counter)	*m_Stats; \
		GLUE(SProfStats2_, __counter)	*m_StatsAcc; \
		SProfPrinter(GLUE(SProfStats_, __counter) *stats, GLUE(SProfStats2_, __counter) *statsAcc) : m_Stats(stats), m_StatsAcc(statsAcc) {} \
		~SProfPrinter() \
		{ \
			enum { kCount = sizeof(m_Stats->m_Data) / sizeof(PopcornFX::Profiler::Internal::SScopedTimerData) }; \
			HH_STATIC_ASSERT(sizeof(m_Stats->m_Data) == sizeof(m_StatsAcc->m_Data) && kCount == sizeof(m_StatsAcc->m_Data) / sizeof(PopcornFX::Profiler::Internal::SScopedTimerData)); \
			PopcornFX::Profiler::Internal::SScopedTimerData			*rawDataSource = reinterpret_cast<PopcornFX::Profiler::Internal::SScopedTimerData*>(&m_Stats->m_Data); \
			const PopcornFX::Profiler::Internal::SScopedTimerData	*rawDataAcc = reinterpret_cast<const PopcornFX::Profiler::Internal::SScopedTimerData*>(&m_StatsAcc->m_Data); \
			{ \
				/*accumulate the results:*/ \
				HH_CRITICAL_SECTION_SCOPEDLOCK(m_Stats->m_Lock); \
				for (PopcornFX::hh_u32 i = 0; i < kCount; i++) \
				{ \
					rawDataSource[i].m_TimingAcc += rawDataAcc[i].m_TimingAcc; \
					rawDataSource[i].m_Count += rawDataAcc[i].m_Count; \
					if (rawDataAcc[i].m_Type != PopcornFX::Profiler::Internal::Timer_None) \
						rawDataSource[i].m_Type = rawDataAcc[i].m_Type; \
				} \
				/*increment total capture count and if necessary dump the stats*/ \
				PopcornFX::hh_u32	counter = ++m_Stats->m_Counter; \
				if (counter >= __limit) \
				{ \
					/* time to dump infos */ \
					/* braces arount the static assert are a hack for earlier versions of GCC that do not support __COUNTER__ or static_assert */ \
					{ HH_STATIC_ASSERT((sizeof(m_Stats->m_Data) % sizeof(PopcornFX::Profiler::Internal::SScopedTimerData)) == 0); } \
					static const char	*kSourceNames[] = { HH_VA_STRINGIFY(__VA_ARGS__) }; \
					enum \
					{ \
						kSourceNameMaxLength =	PopcornFX::TIntegerTemplateTools::Min< \
													PopcornFX::TIntegerTemplateTools::Max< \
														PopcornFX::TIntegerTemplateTools::Max<HH_APPLY_STRINGIFIED_OPERATOR_1_5(sizeof, __VA_ARGS__)>::Value, \
														PopcornFX::TIntegerTemplateTools::Max<HH_APPLY_STRINGIFIED_OPERATOR_6_10(sizeof, __VA_ARGS__)>::Value, \
														11 /* max 10 : we need two chars, and 11 instead of 10 because we -1 right after*/ \
													>::Value - 1, \
													99 \
												>::Value, \
						kSourceNameMaxLength_Digit0 = '0' + kSourceNameMaxLength % 10, \
						kSourceNameMaxLength_Digit1 = '0' + kSourceNameMaxLength / 10 \
					}; \
					static const struct	SFormatter { \
						HH_PROFILESTATS_DECLARE_FORMATTER(	"%__s : #Calls       Timing", 1, \
															"%__s : %-9d %5d.%03d %s/call", 1) \
					}	kFormatter; \
					HH_STATIC_ASSERT(HH_ARRAY_COUNT(kSourceNames) == kCount); \
					PopcornFX::CLog::Log(HH_INFO,	"profile stats (%d iterations):", counter); \
					PopcornFX::CLog::Log(HH_INFO,	kFormatter.m_Format0, "Name"); \
					for (PopcornFX::hh_u32 i = 0; i < kCount; i++) \
					{ \
						const PopcornFX::Profiler::Internal::ETimerType	type = rawDataSource[i].m_Type; \
						const float		timeScale = (type == PopcornFX::Profiler::Internal::Timer_Seconds) ? 1.0e+6f : 1.0f; \
						const float		t = (rawDataSource[i].m_Count == 0) ? 0 : rawDataSource[i].m_TimingAcc * timeScale / float(rawDataSource[i].m_Count); \
						const PopcornFX::hh_u32		tInt = PopcornFX::hh_u32(t); \
						const PopcornFX::hh_u32		tFrac = PopcornFX::hh_u32(fmodf(t, 1.0f) * 1000.0f); \
						PopcornFX::CLog::Log(HH_INFO, kFormatter.m_Format1, kSourceNames[i], rawDataSource[i].m_Count, \
						tInt, tFrac, (type == PopcornFX::Profiler::Internal::Timer_Cycles) ? "cycles" : "us"); \
					} \
					m_Stats->m_Counter = 0; \
					memset(&m_Stats->m_Data, 0, sizeof(m_Stats->m_Data)); \
				} \
			} \
		} \
	} __PROFILESTATS_PRINTER(&_PROFILESTATS, &_PROFILESTATS2)

#else

#	define	HH_PROFILESTATS_DECLARE(...)
#	define	HH_PROFILESTATS_CAPTURE(__name)
#	define	HH_PROFILESTATS_CAPTURE_N(__name, __count)
#	define	HH_PROFILESTATS_CAPTURE_CYCLES(__name)
#	define	HH_PROFILESTATS_CAPTURE_CYCLES_N(__name, __count)

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_PROFILER_H__
