#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2009/02/02 7:02 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_BASICS_H__
#define __KR_THREADS_BASICS_H__

#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_threads_atomic.h"
#include "hh_kernel/include/kr_profiler.h"

// CNonCopyable
#include "hh_template_metaprogramming.h"

#if		defined(HH_THREADS_POSIX)
#	include <pthread.h>
#	include <semaphore.h>
#	if	defined(HH_LINUX)
#		include <sys/syscall.h>
#		if	defined(SYS_futex)
#			define	HH_HAS_FUTEX
#		endif
#	endif

#elif	defined(HH_THREADS_PS3)
#	if		(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#		include <sys/ppu_thread.h>
#		include <semaphore.h>
#	elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
//#		include <cell/atomic.h>
#	endif

#elif	defined(HH_THREADS_ORBIS)
#	include <semaphore.h>
#	include <kernel.h>
#	include <sceerror.h>

#elif	defined(HH_THREADS_PSP2)
#	include <kernel.h>

#elif	defined(HH_THREADS_NX)
#	include <nn/os/os_Mutex.h>
#	include <nn/os/os_Event.h>
#	include <nn/os/os_Semaphore.h>
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type> class	TMemoryView;

// sanity check:

#if defined(HH_THREADS_WIN32) && !defined(HH_WIN32)
#	error	corrupt threads config
#elif defined(HH_THREADS_DURANGO) && !defined(HH_DURANGO)
#	error	corrupt threads config
#elif defined(HH_THREADS_WINRT) && !defined(HH_WINRT)
#	error	corrupt threads config
#endif


#if	(	defined(HH_THREADS_WIN32) || \
		/*defined(HH_THREADS_XBOX) ||*/ \
		defined(HH_THREADS_WINRT) || \
		defined(HH_THREADS_DURANGO) || \
		defined(HH_THREADS_ORBIS) || \
		defined(HH_THREADS_POSIX) \
	) && !defined(HH_MACOSX) // too slow for now
#	define	HH_HAS_THREAD_AWAKER
#endif

//----------------------------------------------------------------------------

//typedef	CSmallGuid	CThreadID;
typedef	CGuid	CThreadID;

//----------------------------------------------------------------------------

template<typename _Type>
class	TScopedLock : CNonCopyable
{
private:
	_Type	&m_Lockable;
protected:
public:
	TScopedLock(_Type &lockable) : m_Lockable(lockable) { m_Lockable.Lock(); }
	~TScopedLock() { m_Lockable.Unlock(); }
};

//----------------------------------------------------------------------------

#define	HH_SCOPEDLOCK(__prim)				PopcornFX::TScopedLock<typename PopcornFX::TType_RemoveReference<PK_DECLTYPE(__prim)>::Type>	GLUE(__local_lock_, __COUNTER__)(__prim);
#define	HH_LOCK_EXPRESSION(__prim, __expr)	do { HH_SCOPEDLOCK(__prim); __expr; } while (0)

#define	HH_CRITICAL_SECTION_SCOPEDLOCK			HH_SCOPEDLOCK
#define	HH_CRITICAL_SECTION_LOCKED_EXPRESSION	HH_LOCK_EXPRESSION

//----------------------------------------------------------------------------

namespace	Threads
{

	//----------------------------------------------------------------------------
	//
	//	Critical sections
	//
	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CCriticalSection : public CNonCopyable
	{
	private:
#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
		// Windows/X360/XboxOne/WinRT
		mutable CRITICAL_SECTION	m_CS;
#	if	(KR_PROFILER_ENABLED != 0)
		mutable volatile hh_u32		m_Owner;
#	endif
#elif	defined(HH_THREADS_POSIX)
		// Posix
		mutable pthread_mutex_t		m_CS;
#	if	(KR_PROFILER_ENABLED != 0)
		mutable volatile hh_u32		m_Owner;
#	endif
#elif	defined(HH_THREADS_PS3)
		// PlayStation 3
#	if	(HH_CRITICAL_SECTION_PS3_USE_LWMUTEX == 0)
		sys_mutex_t			m_CS;
#	else
		HH_STATIC_ASSERT(sizeof(hh_u64) == 8);
		hh_u64				m_CS_Storage[(sizeof(sys_lwmutex_t) + 7) / 8];	// force 8-byte alignment & round up
		sys_lwmutex_t		*CSPTR() const { return Mem::Align<8>((sys_lwmutex_t*)&m_CS_Storage[0]); }
#	endif
#elif	defined(HH_THREADS_ORBIS)
		mutable ScePthreadMutex		m_CS;
#	if	(KR_PROFILER_ENABLED != 0)
		mutable volatile hh_u32		m_Owner;
#	endif
#elif	defined(HH_THREADS_PSP2)
		// PlayStation-Vita
#	if	(HH_CRITICAL_SECTION_PSP2_USE_LWMUTEX == 0)
		SceUID				m_CS;
#	else
		HH_STATIC_ASSERT(sizeof(hh_u64) == 8);
		hh_u64					m_CS_Storage[(sizeof(SceKernelLwMutexWork) + 7) / 8];	// force 8-byte alignment & round up
		SceKernelLwMutexWork	*CSPTR() const
		{
#ifdef	HH_COMPILER_SNC
			// GOD DAMMIT SNC !!!
			// workaround compiler bug
			SceKernelLwMutexWork	* volatile p = (SceKernelLwMutexWork*)&m_CS_Storage[0];
			return Mem::Align<8>(p);
#else
			return Mem::Align<8>((SceKernelLwMutexWork*)&m_CS_Storage[0]);
#endif
		}
#	endif
#elif	defined(HH_THREADS_NX)
		mutable nn::os::Mutex		*m_CS; // pointer because nn::os::Mutex is non trivially moveable, but CCriticalSection must be.
#	if	(KR_PROFILER_ENABLED != 0)
		mutable volatile hh_u32		m_Owner;
#	endif
#else
#	error	unsupported Threading API
#endif

	protected:
	public:
		CCriticalSection();
		~CCriticalSection();

		// NOTE: These are const. in practise they can be used in const methods accessing thread-shared members in read-only mode,
		// that still need locking to work correctly. and almost every lock ends up being declared mutable anyway...
		// due to the inherent thread-safety of this class, all lock/unlock methods are const.
		void	Lock() const;
		bool	TryLock() const;
		void	Unlock() const;
	};

	//----------------------------------------------------------------------------
	//
	//	signalable events
	//
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// simple triggerable event
	// -> threads waiting on this event keep waiting while the event is not triggered
	// waiting on an event created in a non-signalled state will block the waiting thread
	// until another thread triggers the event. so be careful.

	class	HH_KERNEL_EXPORT CEvent : public CNonCopyable
	{
	private:
#if	defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
		HANDLE						m_Handle;
#elif	defined(HH_THREADS_POSIX)// || defined(HH_THREADS_PS3)
#	if	defined(HH_THREADS_SEM_MACOSX)
		mutable semaphore_t			m_Semaphore;
#	else
		mutable sem_t				m_Semaphore;
#	endif
#elif	defined(HH_THREADS_PS3)
		mutable sem_t				m_Semaphore;
#elif	defined(HH_THREADS_ORBIS)
		mutable SceKernelEventFlag	m_Event;
#elif	defined(HH_THREADS_PSP2)
		SceUID			m_Handle;
#elif	defined(HH_THREADS_NX)
		mutable nn::os::Event		*m_Event; // pointer because nn::os::Event is non trivially moveable, but CEvent must be.
#else
#	error	unsupported Threading API
#endif

	public:
		CEvent(bool startInSignalledState = false);
		~CEvent();

		void			Trigger();

		// Wait waits and reset the event
		void			Wait() const;
		static void		Wait(const TMemoryView<const CEvent> &eventList);

		// retrun true if was triggered
		// (Event is reset if triggered)
		bool			TimedWait(hh_u32 milliseconds) const;
		bool			TryWait() const;
	};


	//----------------------------------------------------------------------------
	//	gets signalled when the counter goes from a non-zero count to zero.
	//	-> threads waiting on this event keep waiting while the counter is nonzero
	//	-> Acquire()-ing the counter increments it
	//	-> Release()-ing the counter decrements it
	//	each Acquire() should be matched with a Release()
	//	a call to Release() with no matching call to Acquire() is invalid.

	class	HH_KERNEL_EXPORT CCountedEvent : public CNonCopyable
	{
	private:
		TAtomic<hh_u32>				m_Counter;

#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
		HANDLE						m_WaitingHandle;
#elif	defined(HH_THREADS_POSIX) || defined(HH_THREADS_PS3)
		sem_t						m_Semaphore;
#elif	defined(HH_THREADS_ORBIS)
		sem_t						m_Semaphore;
#elif	defined(HH_THREADS_PSP2)
		SceUID						m_Semaphore;
#elif	defined(HH_THREADS_NX)
		mutable nn::os::Semaphore	*m_Semaphore; // pointer because nn::os::Semaphore is non trivially moveable, but CCountedEvent must be.
#else
#	error	unsupported Threading API
#endif

	public:
		CCountedEvent();
		~CCountedEvent();

		void			Acquire();	// increments the counter
		void			Release();	// decrements the counter

		hh_u32			Counter() const { return m_Counter; }

		void			Wait() const;
		static void		Wait(const TMemoryView<const CCountedEvent> &eventList);
	};

	//----------------------------------------------------------------------------
	//
	//	Awaker / sleeper
	//
	//----------------------------------------------------------------------------

#ifdef	HH_HAS_THREAD_AWAKER

	class	CAwaker : public CNonCopyable
	{
#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
		HH_STATIC_ASSERT(sizeof(CONDITION_VARIABLE) == sizeof(HANDLE));
		mutable HANDLE					m_CondVar;
#elif	defined(HH_THREADS_ORBIS)
		mutable hh_u32					m_WakeCount;
		mutable ScePthreadMutex			m_Mutex;
		mutable ScePthreadCond			m_CondVar;
#elif	defined(HH_THREADS_POSIX)
#	if		defined(HH_HAS_FUTEX)
		mutable volatile hh_u32			m_Futex;
#	else
		mutable hh_u32					m_WakeCount;
		mutable pthread_mutex_t			m_Mutex;
		mutable pthread_cond_t			m_CondVar;
#	endif
#else
#	error	not implemented
#endif

	public:
		class	CSleeper : public CNonCopyable
		{
		private:
#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
			// Windows/X360/XboxOne/WinRT
			hh_u32							m_WakeCount;
			mutable CRITICAL_SECTION		m_Mutex;
#elif	defined(HH_THREADS_ORBIS)
			hh_u32							m_LastWakeCount;
#elif	defined(HH_THREADS_POSIX)
#	if		defined(HH_HAS_FUTEX)
			hh_u32							m_LastFutex;
#	else
			hh_u32							m_LastWakeCount;
#	endif
#else
#	error	not implemented
#endif

		public:
			CSleeper();
			~CSleeper();
			void		Sleep(const CAwaker &awaker, float timeOutSeconds = -1.f);
			void		FlagForWakeUp();
		};

		CAwaker();
		~CAwaker();

		void			WakeUpOneFlagged();
		void			WakeUpAllFlagged();
	};

#endif	// HH_HAS_THREAD_AWAKER

};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_threads_stats.h"

#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
#	include	"hh_kernel/include/platforms/win32/kr_threads.impl.h"
#elif	defined(HH_THREADS_POSIX)
#	include	"hh_kernel/include/platforms/linux/kr_threads.impl.h"
#elif	defined(HH_THREADS_PS3)
#	include	"hh_kernel/include/platforms/ps3/kr_threads.impl.h"
#elif	defined(HH_THREADS_ORBIS)
#	include	"hh_kernel/include/platforms/orbis/kr_threads.impl.h"
#elif	defined(HH_THREADS_PSP2)
#	include	"hh_kernel/include/platforms/psp2/kr_threads.impl.h"
#elif	defined(HH_THREADS_NX)
#	include	"hh_kernel/include/platforms/nx/kr_threads.impl.h"
#else
#	error	unsupported Threading API
#endif

#endif // __KR_THREADS_BASICS_H__
