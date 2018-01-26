#pragma once

//----------------------------------------------------------------------------
// Created on Wed 04/09/2014 15:08 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_THREADS_RWLOCKS_WIN_IMPL_H__
#define	__KR_THREADS_RWLOCKS_WIN_IMPL_H__

#ifndef	__KR_THREADS_RWLOCK_H__
#	error	kr_threads_rwlock.impl.h must not be included manually, include kr_threads_rwlock.h instead
#endif
#if	!defined(HH_THREADS_WIN32) && !defined(HH_THREADS_XBOX) && !defined(HH_THREADS_WINRT) && !defined(HH_THREADS_DURANGO)
#	error	This file is for Windows/X360/Durango/WinRT only
#endif

#if	(PK_THREADS_HAS_RWLOCK != 0)

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Threads
{

	//----------------------------------------------------------------------------

	HH_FORCEINLINE CRWLock::CRWLock()
	{
		InitializeSRWLock(&m_Lock);
#if	(KR_THREADS_STATS_CAPTURE == 1)
		CThreadStats::RegisterRWLockCreation();
#endif
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE CRWLock::~CRWLock()
	{
#if	(KR_THREADS_STATS_CAPTURE == 1)
		CThreadStats::RegisterRWLockDestruction();
#endif
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE void		CRWLock::LockRead() const
	{
		HH_CUSTOMSCOPEDPROFILE("lockread::idle", PopcornFX::Profiler::Color_Idle, PopcornFX::Profiler::SNodeDescriptor::Idle, true);
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		CThreadStats::RegisterRLock();
#endif
		AcquireSRWLockShared(&m_Lock);
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE bool		CRWLock::TryLockRead() const
	{
		HH_NAMEDSCOPEDPROFILE_C("trylockread", PopcornFX::Profiler::Color_Idle);	// not really an idle node: we won't block
		if (0 != TryAcquireSRWLockShared(&m_Lock))
		{
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
			CThreadStats::RegisterRLock();
#endif
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE void		CRWLock::UnlockRead() const
	{
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		CThreadStats::RegisterRUnlock();
#endif
		ReleaseSRWLockShared(&m_Lock);
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE void		CRWLock::LockWrite()
	{
		HH_CUSTOMSCOPEDPROFILE("lockwrite::idle", PopcornFX::Profiler::Color_Idle, PopcornFX::Profiler::SNodeDescriptor::Idle, true);
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		CThreadStats::RegisterWLock();
#endif
		AcquireSRWLockExclusive(&m_Lock);
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE bool		CRWLock::TryLockWrite()
	{
		HH_NAMEDSCOPEDPROFILE_C("trylockwrite", PopcornFX::Profiler::Color_Idle);	// not really an idle node: we won't block
		if (0 != TryAcquireSRWLockExclusive(&m_Lock))
		{
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
			CThreadStats::RegisterWLock();
#endif
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------

	HH_FORCEINLINE void		CRWLock::UnlockWrite()
	{
#if	(KR_THREADS_STATS_CAPTURE_RUNS == 1)
		CThreadStats::RegisterWUnlock();
#endif
		ReleaseSRWLockExclusive(&m_Lock);
	}

	//----------------------------------------------------------------------------

} // namespace Threads

//----------------------------------------------------------------------------
__PK_API_END

#endif	// (PK_THREADS_HAS_RWLOCK != 0)

#endif // __KR_THREADS_RWLOCKS_WIN_IMPL_H__
