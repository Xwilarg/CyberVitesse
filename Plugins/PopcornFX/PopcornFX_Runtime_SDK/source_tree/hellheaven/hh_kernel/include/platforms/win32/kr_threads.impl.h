#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2010/09/08 12:46 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_THREADS_WIN_IMPL_H__
#define	__KR_THREADS_WIN_IMPL_H__

#ifndef	__KR_THREADS_BASICS_H__
#	error	kr_threads.impl.h must not be included manually, include kr_threads.h instead
#endif
#if	!defined(HH_THREADS_WIN32) && !defined(HH_THREADS_XBOX) && !defined(HH_THREADS_WINRT) && !defined(HH_THREADS_DURANGO)
#	error	This file is for Windows/X360/Durango/WinRT only
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Windows implementation
//
//----------------------------------------------------------------------------

namespace	Threads
{
#if	(KR_PROFILER_ENABLED == 0)	// otherwise, defined in the .cpp
	HH_FORCEINLINE void		CCriticalSection::Lock() const
	{
		HH_CUSTOMSCOPEDPROFILE("lock::idle", PopcornFX::Profiler::Color_Idle, PopcornFX::Profiler::SNodeDescriptor::Idle, true);
#	if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		CThreadStats::RegisterCSLock();
#	endif
		::EnterCriticalSection(&m_CS);
	}
#endif

	HH_FORCEINLINE bool		CCriticalSection::TryLock() const
	{
		HH_CUSTOMSCOPEDPROFILE("trylock::idle", PopcornFX::Profiler::Color_Idle, PopcornFX::Profiler::SNodeDescriptor::Idle, true);
		if (::TryEnterCriticalSection(&m_CS) != FALSE)
		{
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
			CThreadStats::RegisterCSLock();
#endif
			return true;
		}
		return false;
	}

	HH_FORCEINLINE void		CCriticalSection::Unlock() const
	{
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		CThreadStats::RegisterCSUnlock();
#endif
#if	(KR_PROFILER_ENABLED != 0)
		m_Owner = CThreadID::INVALID;
#endif
		::LeaveCriticalSection(&m_CS);
	}
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_THREADS_WIN_IMPL_H__

