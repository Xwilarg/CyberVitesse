#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2003/07/02 2:52 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_TIMERS_INL__
#define	__KR_TIMERS_INL__

#ifndef	__KR_TIMERS_H__
#	error	kr_timers.inl must not be included manually, include kr_timers.h instead
#endif

#if		defined(HH_WINDOWS) || defined(HH_X360)
#	include "hh_kernel/include/platforms/win32/kr_timers.impl.h"
#elif	defined(HH_LINUX) || defined(HH_MACOSX)
#	include "hh_kernel/include/platforms/linux/kr_timers.impl.h"
#elif	defined(HH_ANDROID)
#	include "hh_kernel/include/platforms/android/kr_timers.impl.h"
#elif	defined(HH_IOS)
#	include "hh_kernel/include/platforms/ios/kr_timers.impl.h"
#elif	defined(HH_PS3)
#	include "hh_kernel/include/platforms/ps3/kr_timers.impl.h"
#elif	defined(HH_ORBIS)
#	include "hh_kernel/include/platforms/orbis/kr_timers.impl.h"
#elif	defined(HH_PSP2)
#	include "hh_kernel/include/platforms/psp2/kr_timers.impl.h"
#elif	defined(HH_NX)
#	include "hh_kernel/include/platforms/nx/kr_timers.impl.h"
#else
#	error	unsupported Timers API
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

namespace	Timer
{
	namespace	Internal
	{
		extern hh_u32		HH_KERNEL_EXPORT g_TimerCPUCyclesOverhead[2];	// [0] unserialized, [1] serialized. index directly with the 'serializeCPU' parameter
	}
}

//----------------------------------------------------------------------------

template<typename _TCycles>
TTimerCPUCycles<_TCycles>::TTimerCPUCycles()
:	m_CyclesStart(0)
,	m_CyclesEnd(0)
{
}

//----------------------------------------------------------------------------

template<typename _TCycles>
HH_FORCEINLINE void		TTimerCPUCycles<_TCycles>::Start(bool serializeCPU /*= false*/)
{
	m_CyclesEnd = 0;
	m_CyclesStart = RawCPUCycles(serializeCPU);
}

//----------------------------------------------------------------------------

template<typename _TCycles>
HH_FORCEINLINE _TCycles	TTimerCPUCycles<_TCycles>::Stop(bool serializeCPU /*= false*/)
{
	_TSignedCycles	cycles = RawCPUCycles(serializeCPU);
	if (m_CyclesEnd == 0)	// not stopped yet
	{
		HH_PARANOID_ASSERT(hh_u32(serializeCPU) == 0 || hh_u32(serializeCPU) == 1);	// unfortunately, the standard doesn't guarantee anything
		m_CyclesEnd = cycles - Timer::Internal::g_TimerCPUCyclesOverhead[serializeCPU];
	}
	cycles = m_CyclesEnd - m_CyclesStart;

	// as this is meant to be used to profile stuff that take quite some time,
	// the thread may have been rescheduled into a core that wasn't synchronized with the original one.
	// make sure we return something not totally meaningless:
	return RawCPUCyclesToRealCycles((cycles >= 1) ? cycles : 1);
}

//----------------------------------------------------------------------------

template<typename _TCycles>
HH_FORCEINLINE _TCycles	TTimerCPUCycles<_TCycles>::Read(bool serializeCPU /*= false*/) const
{
	_TSignedCycles	cycles = m_CyclesEnd;
	if (cycles == 0)
	{
		HH_PARANOID_ASSERT(hh_u32(serializeCPU) == 0 || hh_u32(serializeCPU) == 1);	// unfortunately, the standard doesn't guarantee anything
		cycles = RawCPUCycles(serializeCPU) - Timer::Internal::g_TimerCPUCyclesOverhead[serializeCPU];
	}
	cycles -= m_CyclesStart;
	return RawCPUCyclesToRealCycles((cycles >= 1) ? cycles : 1);
}

//----------------------------------------------------------------------------

template<typename _TCycles>
HH_FORCEINLINE _TCycles	TTimerCPUCycles<_TCycles>::RawProcessorTimerOverhead(bool serializeCPU /*= false*/)
{
	const volatile _TCycles	a = RawCPUCycles(serializeCPU);
	const volatile _TCycles	b = RawCPUCycles(serializeCPU);
	return b - a;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_TIMERS_INL__

