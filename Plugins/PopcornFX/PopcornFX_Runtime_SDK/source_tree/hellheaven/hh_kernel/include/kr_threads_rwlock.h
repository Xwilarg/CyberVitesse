#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2014/09/03 15:00 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_RWLOCK_H__
#define __KR_THREADS_RWLOCK_H__

#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_threads_atomic.h"
#include "hh_kernel/include/kr_threads_stats.h"

#if	!defined(PK_THREADS_HAS_RWLOCK)
#	if		(defined(HH_WIN32) && (_WIN32_WINNT >= _WIN32_WINNT_VISTA)) || \
			/*defined(HH_WINRT) ||*/ \
			/*defined(HH_X360) ||*/ \
			/*defined(HH_PS3) ||*/ \
			/*defined(HH_PSP2) ||*/ \
			defined(HH_DURANGO) || \
			defined(HH_ORBIS) || \
			defined(HH_LINUX) || \
			defined(HH_ANDROID) || \
			defined(HH_MACOSX) || \
			defined(HH_IOS)
#		define	PK_THREADS_HAS_RWLOCK		1
#	else
#		define	PK_THREADS_HAS_RWLOCK		0
#	endif
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Threads
{

	//----------------------------------------------------------------------------
	//
	//	CRWLock
	//
	//----------------------------------------------------------------------------

#if	(PK_THREADS_HAS_RWLOCK != 0)
	class	CRWLock : public CNonCopyable
	{
	private:
#	if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
		mutable SRWLOCK				m_Lock;	// Cannot be copied, makes CRWLock non-copyable, we don't want to alloc this.
#	elif	defined(HH_THREADS_ORBIS)
		mutable ScePthreadRwlock	m_Lock;
#	elif	defined(HH_THREADS_POSIX)
		mutable pthread_rwlock_t	m_Lock;
#	else
#		error	unsupported Threading API
#	endif

	public:
		CRWLock();
		~CRWLock();
		void	LockRead() const;
		bool	TryLockRead() const;
		void	UnlockRead() const;
		void	LockWrite();
		bool	TryLockWrite();
		void	UnlockWrite();
	};

#else

	class	CRWLock : CNonCopyable
	{
	private:
		CCriticalSection	m_CS;
	public:
		CRWLock() {}
		~CRWLock() {}
		void	LockRead() const { m_CS.Lock(); }
		bool	TryLockRead() const { return m_CS.TryLock(); }
		void	UnlockRead() const { m_CS.Unlock(); }
		void	LockWrite() { m_CS.Lock(); }
		bool	TryLockWrite() { return m_CS.TryLock(); }
		void	UnlockWrite() { m_CS.Unlock(); }
	};

#endif // PK_THREADS_HAS_RWLOCK != 0

	//----------------------------------------------------------------------------
	//
	//	CRWSpinLock
	//	! Spin Locks
	//
	//----------------------------------------------------------------------------

	class	CRWSpinLock
	{
	private:
		enum
		{
			kReadLock = 0x7FFFFFFFU,
			kWriteLock = 0x80000000U
		};

		mutable TAtomic<hh_u32>		m_Atom;

	public:
		CRWSpinLock() : m_Atom(0) {}

		HH_FORCEINLINE void		LockRead() const
		{
			hh_u32	oldAtom = (hh_u32(m_Atom) & hh_u32(kReadLock));
			if (m_Atom.SetValueIfEqualTo(oldAtom + 1U, oldAtom) != oldAtom)
			{
				HH_SCOPEDPROFILESPIN("CMWMRSpinLock::LockRead Spin");
				oldAtom = (hh_u32(m_Atom) & hh_u32(kReadLock));
				hh_u32	itCounter = 0;
				while (m_Atom.SetValueIfEqualTo(oldAtom + 1U, oldAtom) != oldAtom)
				{
					CCurrentThread::Yield(itCounter++);
					oldAtom = (hh_u32(m_Atom) & hh_u32(kReadLock));
				}
			}
			Memory::FullBarrier();	// read-acquire
		}

		HH_FORCEINLINE void		UnlockRead() const
		{
			HH_ASSERT((m_Atom & hh_u32(kWriteLock)) == 0U);
			HH_ASSERT(m_Atom >= 1U);
			Memory::FullBarrier();	// write-release
			--m_Atom;
		}

		HH_FORCEINLINE void		LockWrite()
		{
			if (m_Atom.SetValueIfEqualTo(hh_u32(kWriteLock), 0U) != 0U)
			{
				HH_SCOPEDPROFILESPIN("CMWMRSpinLock::LockWrite Spin");
				hh_u32	itCounter = 0;
				while (m_Atom.SetValueIfEqualTo(hh_u32(kWriteLock), 0U) != 0U)
				{
					CCurrentThread::Yield(itCounter++);
				}
			}
			Memory::FullBarrier();	// read-acquire
		}

		HH_FORCEINLINE void		UnlockWrite()
		{
			HH_ASSERT(m_Atom == kWriteLock);
			Memory::FullBarrier();	// write-release
			m_Atom = 0;
		}

		HH_FORCEINLINE void		UnlockRead_LockWrite()
		{
			HH_ASSERT(m_Atom >= 1U);
			Memory::FullBarrier();	// write-release
			if (m_Atom.SetValueIfEqualTo(hh_u32(kWriteLock), 1U) != 1U)
			{
				HH_SCOPEDPROFILESPIN("CMWMRSpinLock::UnlockRead_LockWrite Spin");
				hh_u32	itCounter = 0;
				while (m_Atom.SetValueIfEqualTo(hh_u32(kWriteLock), 1U) != 1U)
				{
					CCurrentThread::Yield(itCounter++);
				}
			}
			Memory::FullBarrier();	// read-acquire
		}
	};

	//----------------------------------------------------------------------------

} // namespace	Threads

//----------------------------------------------------------------------------

template<typename _Type>
class	TScopedLockRead : CNonCopyable
{
private:
	_Type	&m_Object;
public:
	TScopedLockRead(_Type &object) : m_Object(object) { m_Object.LockRead(); }
	~TScopedLockRead() { m_Object.UnlockRead(); }
};

template<typename _Type>
class	TScopedLockWrite : CNonCopyable
{
private:
	_Type	&m_Object;
public:
	TScopedLockWrite(_Type &object) : m_Object(object) { m_Object.LockWrite(); }
	~TScopedLockWrite() { m_Object.UnlockWrite(); }
};

//----------------------------------------------------------------------------
#if 1	// switch to #if 0 to make scopedlock macros work with all prim types

#define	HH_SCOPEDLOCK_READ(__prim)	PopcornFX::TScopedLockRead<typename TType_RemoveReference<PK_DECLTYPE(__prim)>::Type>	GLUE(__local_lock_, __COUNTER__)(__prim);
#define	HH_SCOPEDLOCK_WRITE(__prim)	PopcornFX::TScopedLockWrite<typename TType_RemoveReference<PK_DECLTYPE(__prim)>::Type>	GLUE(__local_lock_, __COUNTER__)(__prim);

#define	HH_LOCKREAD_EXPRESSION(__prim, __expr)	do { HH_SCOPEDLOCK_READ(__prim); __expr; } while (0)
#define	HH_LOCKWRITE_EXPRESSION(__prim, __expr)	do { HH_SCOPEDLOCK_WRITE(__prim); __expr; } while (0)

//----------------------------------------------------------------------------
#else

template<bool _Write>
class	TScopedLock<Threads::CRWLock, _Write>	: public TScopedLockWrite<Threads::CRWLock> { public: TScopedLock(Threads::CRWLock &object) : TScopedLockWrite<Threads::CRWLock>(object) {} };
template<>
class	TScopedLock<Threads::CRWLock, false>	: public TScopedLockRead<Threads::CRWLock> { public: TScopedLock(Threads::CRWLock &object) : TScopedLockRead<Threads::CRWLock>(object) {} };

template<bool _Write>
class	TScopedLock<const Threads::CRWLock, _Write>	: public TScopedLockWrite<const Threads::CRWLock> { public: TScopedLock(const Threads::CRWLock &object) : TScopedLockWrite<const Threads::CRWLock>(object) {} };
template<>
class	TScopedLock<const Threads::CRWLock, false>	: public TScopedLockRead<const Threads::CRWLock> { public: TScopedLock(const Threads::CRWLock &object) : TScopedLockRead<const Threads::CRWLock>(object) {} };

template<bool _Write>
class	TScopedLock<Threads::CRWSpinLock, _Write>	: public TScopedLockWrite<Threads::CRWSpinLock> { public: TScopedLock(Threads::CRWSpinLock &object) : TScopedLockWrite<Threads::CRWSpinLock>(object) {} };
template<>
class	TScopedLock<Threads::CRWSpinLock, false>	: public TScopedLockRead<Threads::CRWSpinLock> { public: TScopedLock(Threads::CRWSpinLock &object) : TScopedLockRead<Threads::CRWSpinLock>(object) {} };

template<bool _Write>
class	TScopedLock<const Threads::CRWSpinLock, _Write>	: public TScopedLockWrite<const Threads::CRWSpinLock> { public: TScopedLock(const Threads::CRWSpinLock &object) : TScopedLockWrite<const Threads::CRWSpinLock>(object) {} };
template<>
class	TScopedLock<const Threads::CRWSpinLock, false>	: public TScopedLockRead<const Threads::CRWSpinLock> { public: TScopedLock(const Threads::CRWSpinLock &object) : TScopedLockRead<const Threads::CRWSpinLock>(object) {} };

//----------------------------------------------------------------------------

#	if defined(HH_COMPILER_GCC)
#		define	HH_SCOPEDLOCK_READ(__prim)	PopcornFX::TScopedLock<TType_RemoveReference<typeof(__prim)>::Type, false>		GLUE(__local_lock_, __COUNTER__)(__prim);
#		define	HH_SCOPEDLOCK_WRITE(__prim)	PopcornFX::TScopedLock<TType_RemoveReference<typeof(__prim)>::Type, true>		GLUE(__local_lock_, __COUNTER__)(__prim);
#	else
#		define	HH_SCOPEDLOCK_READ(__prim)	PopcornFX::TScopedLock<TType_RemoveReference<decltype(__prim)>::Type, false>	GLUE(__local_lock_, __COUNTER__)(__prim);
#		define	HH_SCOPEDLOCK_WRITE(__prim)	PopcornFX::TScopedLock<TType_RemoveReference<decltype(__prim)>::Type, true>		GLUE(__local_lock_, __COUNTER__)(__prim);
#	endif
#endif

//----------------------------------------------------------------------------
__PK_API_END

#if	(PK_THREADS_HAS_RWLOCK != 0)

#	if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
#		include	"hh_kernel/include/platforms/win32/kr_threads_rwlock.impl.h"
#	elif	defined(HH_THREADS_POSIX)
#		include	"hh_kernel/include/platforms/linux/kr_threads_rwlock.impl.h"
//#	elif	defined(HH_THREADS_PS3)
//#		include	"hh_kernel/include/platforms/ps3/kr_threads_rwlock.impl.h"
#	elif	defined(HH_THREADS_ORBIS)
#		include	"hh_kernel/include/platforms/orbis/kr_threads_rwlock.impl.h"
//#	elif	defined(HH_THREADS_PSP2)
//#		include	"hh_kernel/include/platforms/psp2/kr_threads_rwlock.impl.h"
#	else
#		error	unsupported Threading API
#	endif

#endif // PK_THREADS_HAS_RWLOCK != 0

#endif // __KR_THREADS_RWLOCK_H__
