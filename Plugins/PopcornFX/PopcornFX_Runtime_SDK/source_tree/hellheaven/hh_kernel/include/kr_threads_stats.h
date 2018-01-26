#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2014/06/28 14:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_STATS_H__
#define __KR_THREADS_STATS_H__

#include <include/hh_memory.h>						// for cache-line infos
#include <hh_kernel/include/kr_threads_basics.h>	// for CThreadID

#if	!defined(KR_THREADS_STATS_CAPTURE)
#	if	!defined(HH_RETAIL)
#		define	KR_THREADS_STATS_CAPTURE	1
#	else
#		define	KR_THREADS_STATS_CAPTURE	0
#	endif
#endif

#if	!defined(KR_THREADS_STATS_CAPTURE_RUNS)
#	if	defined(HH_DEBUG) && (KR_THREADS_STATS_CAPTURE == 1)
#		define	KR_THREADS_STATS_CAPTURE_RUNS	1
#	else
#		define	KR_THREADS_STATS_CAPTURE_RUNS	0
#	endif
#endif

#if (KR_THREADS_STATS_CAPTURE_RUNS == 1) && (KR_THREADS_STATS_CAPTURE == 0)
#	error	configuration corrupted: KR_THREADS_STATS_CAPTURE_RUNS needs KR_THREADS_STATS_CAPTURE
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CThreadStats
{
public:
#if	(KR_THREADS_STATS_CAPTURE == 1)
	// per-PKThread stats:
	struct	SStat
	{
		hh_u32				m_ThreadCreateCount;
		hh_u32				m_ThreadDestroyCount;
		hh_u32				m_CSCreateCount;
		hh_u32				m_CSDestroyCount;
		hh_u32				m_RWLockCreateCount;
		hh_u32				m_RWLockDestroyCount;
		hh_u32				m_EventCreateCount;
		hh_u32				m_EventDestroyCount;

#	if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		hh_u32				m_CSLockCount;
		hh_u32				m_CSUnlockCount;
		hh_u32				m_RLockCount;
		hh_u32				m_RUnlockCount;
		hh_u32				m_WLockCount;
		hh_u32				m_WUnlockCount;
		hh_u32				m_EventResetCount;
		hh_u32				m_EventTriggerCount;
#	endif

#if	(KR_THREADS_STATS_CAPTURE_RUNS == 0) || defined(HH_PS3) || defined(HH_X360)	// no more need for padding, we should have either 1*8 or 2*8 u32, which is aligned to cache-line size anyway
		hh_u8				m_Padding[
#	if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
										(((8+8) * sizeof(hh_u32) + Memory::CacheLineSize - 1) & ~(Memory::CacheLineSize - 1)) -
										  (8+8) * sizeof(hh_u32)
#	else
										Memory::CacheLineSize - 8 * sizeof(hh_u32)
#	endif
										];
#endif
	};
	HH_STATIC_ASSERT((sizeof(SStat) % Memory::CacheLineSize) == 0);
	HH_STATIC_ASSERT(Memory::CacheLineSize <= 128);	// we align to 128 below
	static HH_ALIGN(128) SStat		m_ThreadStats[128];

	// These are 'external' stats in case some popcorn thread-primitives are called from threads that are not registered
	// as a popcorn thread (ie: popcorn wasn't told this thread exists, so it's not aware of it, and calling
	// CCurrentThread::ThreadID() from it yields CGuid::INVALID).
	static TAtomic<hh_u32>	m_ExtThreadCreateCount;
	static TAtomic<hh_u32>	m_ExtThreadDestroyCount;
	static TAtomic<hh_u32>	m_ExtCSCreateCount;
	static TAtomic<hh_u32>	m_ExtCSDestroyCount;
	static TAtomic<hh_u32>	m_ExtRWLockCreateCount;
	static TAtomic<hh_u32>	m_ExtRWLockDestroyCount;
	static TAtomic<hh_u32>	m_ExtEventCreateCount;
	static TAtomic<hh_u32>	m_ExtEventDestroyCount;

#	if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
	static TAtomic<hh_u32>	m_ExtCSLockCount;
	static TAtomic<hh_u32>	m_ExtCSUnlockCount;
	static TAtomic<hh_u32>	m_ExtRLockCount;
	static TAtomic<hh_u32>	m_ExtRUnlockCount;
	static TAtomic<hh_u32>	m_ExtWLockCount;
	static TAtomic<hh_u32>	m_ExtWUnlockCount;
	static TAtomic<hh_u32>	m_ExtEventResetCount;
	static TAtomic<hh_u32>	m_ExtEventTriggerCount;
#	endif

	static void				RegisterThreadCreation();
	static void				RegisterThreadDestruction();
	static void				RegisterCSCreation();
	static void				RegisterCSDestruction();
	static void				RegisterRWLockCreation();
	static void				RegisterRWLockDestruction();
	static void				RegisterEVCreation();
	static void				RegisterEVDestruction();

#	if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
	static CThreadID		RegisterCSLock();
	static CThreadID		RegisterCSUnlock();
	static CThreadID		RegisterRLock();
	static CThreadID		RegisterRUnlock();
	static CThreadID		RegisterWLock();
	static CThreadID		RegisterWUnlock();
	static CThreadID		RegisterEVReset();
	static CThreadID		RegisterEVTrigger();
#	endif
#endif // (KR_THREADS_STATS_CAPTURE == 1)

	static void					Reset();
	static void					FrameReset();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_THREADS_STATS_H__
