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

#ifndef	__KR_TIMERS_WIN_IMPL_H__
#define	__KR_TIMERS_WIN_IMPL_H__

#ifndef	__KR_TIMERS_INL__
#	error	kr_timers.win32.inl must not be included manually, include kr_timers.h instead
#endif
#if	!defined(HH_WINDOWS) && !defined(HH_X360)
#	error	This file is for Windows/X360 only
#endif

#if	defined(HH_WINDOWS)
#	include <intrin.h>
#endif

#define	KR_TIMERS_SAFE_TIMERS

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Windows implementation
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TBaseTimer
{
private:
	double			m_Frequency;
	hh_u64			m_St;

	HH_STATIC_ASSERT(sizeof(LARGE_INTEGER) == sizeof(hh_u64));
	static double	_InternalFrequencySample()
	{
		hh_u64	freq;
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		if (freq != 0)
			return 1.0 / static_cast<double>(freq);
		HH_ASSERT_NOT_REACHED();
		return 1e-6;
	}

	HH_FORCEINLINE static hh_u64	_InternalSample()
	{
		hh_u64	counter;
		::QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		return counter;
	}

protected:
	HH_FORCEINLINE void				_InternalStart()
	{
		m_St = _InternalSample();
	}

	HH_FORCEINLINE _Type			_InternalStop()
	{
		return _InternalRead();
	}

	_Type							_InternalRead() const
	{
		hh_u64	lint = _InternalSample();	// do this first so we may minimize the extra-time due to our internals computations...

#ifdef	KR_TIMERS_SAFE_TIMERS
		// may be true in some (very) rare cases, when calling timer reads very frequently in very small timesteps... will avoid getting negative timings
		if (m_St >= lint)
			lint = m_St + 1;
#endif
		return _Type(m_Frequency * static_cast<double>(lint - m_St));
	}

public:
	TBaseTimer()
	:	m_St(0)
	,	m_Frequency(_InternalFrequencySample())
	{
		HH_ASSERT(m_Frequency != 0.0);
	}
	~TBaseTimer() {}
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _TCycles>
HH_FORCEINLINE _TCycles	TTimerCPUCycles<_TCycles>::RawCPUCycles(bool serializeCPU /*= false*/)
{
#if	(HH_CPU_ARCH == HH_CPU_X86)
	if (serializeCPU)
	{
		int	dummy[4];
		__cpuid(dummy, 0);
	}
	return __rdtsc();
#else
	(void)serializeCPU;	// kill unused arg warning
	//return __mftb();	// this has a known problem, see the Xbox360 documentation
	LARGE_INTEGER	ticks;
	QueryPerformanceCounter(&ticks);
	return ticks.QuadPart;
#endif
}

//----------------------------------------------------------------------------

template<typename _TCycles>
HH_FORCEINLINE _TCycles	TTimerCPUCycles<_TCycles>::RawCPUCyclesToRealCycles(_TCycles cycles)
{
#if	(HH_CPU_MODEL == HH_CPU_XENON)
	// the timebase frequency is 50000000
	// the Xenon runs @ 3.2 GHz
	// this gives 3200/50 = 64 cycles.
	return cycles * 64;
#else
	return cycles;
#endif
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_TIMERS_WIN_IMPL_H__
