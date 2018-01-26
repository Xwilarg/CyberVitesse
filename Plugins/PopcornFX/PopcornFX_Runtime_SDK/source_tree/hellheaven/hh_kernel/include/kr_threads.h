#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2004/03/18 15:29 by Remy Saissy
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_THREADS_H__
#define	__KR_THREADS_H__

#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_threads_basics.h"
#include "hh_kernel/include/kr_containers_array.h"
#include "hh_kernel/include/kr_string.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Thread manager
//
//	Notes:
//	- AffinityCoreMask
//		each bit represents a CPU core. 0 is an invalid value. if you want to run on all cores, set the mask to something like 0xFFFFFFFF
//
//----------------------------------------------------------------------------

class	CThreadLocalStorage;
class	CThreadsInternals;
class	CThread;

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CGenericAffinityMask
{
private:
	typedef	hh_u32							GranularityType;
	static const hh_u32						GranularityShift = TIntegerTemplateTools::Log2<sizeof(GranularityType) << 3>::Value;
	static const hh_u32						GranularityMask = (1 << GranularityShift) - 1;

	hh_u32									m_SetBitsCount;
	hh_u32									m_FirstAndLastBits;

	TSemiDynamicArray<GranularityType, 1>	m_Affinity;

public:
	CGenericAffinityMask() : m_SetBitsCount(0), m_FirstAndLastBits(0x0000FFFF) {}
	~CGenericAffinityMask() {}

	void									Clear();

	bool									operator == (const CGenericAffinityMask &other) const;
	HH_FORCEINLINE bool						operator != (const CGenericAffinityMask &other) const { return !operator == (other); }

	HH_FORCEINLINE hh_u32					NumBitsSet() const { return m_SetBitsCount; }
	HH_FORCEINLINE hh_u32					FirstBitSet() const { return m_FirstAndLastBits & 0xFFFF; }
	HH_FORCEINLINE hh_u32					LastBitSet() const { return m_FirstAndLastBits >> 16; }

	void									SetAffinityBlock32(hh_u32 block32bitId, hh_u32 cpuX32);		// sets 32 bits at a time
	hh_u32									AffinityBlock32Count() const;
	hh_u32									AffinityBlock32(hh_u32 block32bitId) const;					// gets 32 bits at a time

	void									SetAffinityBlock64(hh_u32 block64bitId, hh_u64 cpuX64) { SetAffinityBlock32(block64bitId / 2, cpuX64 & 0xFFFFFFFF); SetAffinityBlock32(1 + block64bitId / 2, cpuX64 >> 32); }
	hh_u32									AffinityBlock64Count() const { return AffinityBlock32Count() == 0 ? 0 : (AffinityBlock32Count() + 1) / 2; }
	hh_u64									AffinityBlock64(hh_u32 block64bitId) const { return hh_u64(AffinityBlock32(block64bitId / 2)) | (hh_u64(AffinityBlock32(1 + block64bitId / 2)) << 32); }

	void									SetAffinityBit(hh_u32 logicalProcessorId);
	void									ClearAffinityBit(hh_u32 logicalProcessorId);
	bool									TestAffinityBit(hh_u32 logicalProcessorId) const
	{
		const hh_u32	slot = logicalProcessorId >> GranularityShift;
		if (slot < m_Affinity.Count())
		{
			return (m_Affinity[slot] & (1 << (logicalProcessorId & GranularityMask))) != 0;
		}
		return false;
	}

	const CGenericAffinityMask				&operator |= (const CGenericAffinityMask &other);
	const CGenericAffinityMask				&operator &= (const CGenericAffinityMask &other);
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CThreadManager /* : CSealedObject //CNonInheritable */
{
private:
public:
	enum	EPriority
	{
		Priority_BackgroundLow = 0,
		Priority_BackgroundHigh,
		Priority_Normal,
		Priority_High,
		Priority_Critical,

		__MaxThreadPriorities,
	};

	enum	EProcessPriority
	{
		Process_Low = 0,
		Process_Normal,
		Process_High,
		Process_Critical,

		__MaxProcessPriorities
	};

	enum
	{
		MaxThreadCount = 128,
	};

	static const char				* const s_ThreadPriorityStrings[CThreadManager::__MaxThreadPriorities];
	static const char				* const s_ProcessPriorityStrings[CThreadManager::__MaxProcessPriorities];

	static bool						IsMainThread(CThreadID threadId);	// popcorn considers its 'main thread' to be the thread where core libs 'Startup' has been called.
	static CThreadLocalStorage		*FetchTLS(CThreadID threadId);
//	static void						UnregisterAllUserThreads();			// removes all user-threads registered with 'CCurrentThread::RegisterUserThread()'

	static CThreadID				GetParentThreadID(CThreadID threadId);
	static EPriority				GetPriority(CThreadID threadId);
	static void						SetPriority(CThreadID threadId, EPriority priority);
	static void						GetAffinity(CThreadID threadId, CGenericAffinityMask &outAffinity);
	static bool						SetAffinity(CThreadID threadId, const CGenericAffinityMask &affinity);
	static bool						SetTargetProcessor(CThreadID threadId, hh_u32 logicalProcessorId);

	static EProcessPriority			GetProcessPriority();
	static bool						SetProcessPriority(EProcessPriority level);	// be careful with this one

	static CThread					*MapThread(CThreadID threadId);	// returns null if not a CThread

	static bool						UnsafeUnregisterUserThread(CThreadID threadId);	// use at shutdown
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CCurrentThread
{
public:
	static CThreadID					ThreadID();
	static bool							IsMainThread();
	static bool							IsRegistered();
	static CThreadID					RegisterUserThread();
	static bool							UnregisterUserThread();

	static void							Yield(hh_u32 retryCounter = 0xFF);
	static void							Sleep(hh_u32 milliseconds);

	static CThreadID					GetParentThreadID();
	static CThreadManager::EPriority	GetPriority();
	static void							SetPriority(CThreadManager::EPriority priority);
	static void							GetAffinity(CGenericAffinityMask &outAffinity);
	static bool							SetAffinity(const CGenericAffinityMask &affinity);
	static bool							SetTargetProcessor(hh_u32 logicalProcessorId);

	static CThread						*MapThread();	// returns null if not a CThread
};

//----------------------------------------------------------------------------
//
//	Thread
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CThread : public CRefCountedObject
{
public:
#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
	typedef HANDLE				ThreadHandleType;
	typedef DWORD				UniqueIdentifierType;
#elif	defined(HH_THREADS_POSIX)
	typedef pthread_t			ThreadHandleType;
	typedef pthread_t			UniqueIdentifierType;
#elif	defined(HH_THREADS_PS3)
	typedef sys_ppu_thread_t	ThreadHandleType;
	typedef sys_ppu_thread_t	UniqueIdentifierType;
#elif	defined(HH_THREADS_ORBIS)
	typedef ScePthread			ThreadHandleType;
	typedef ScePthread			UniqueIdentifierType;
#elif	defined(HH_THREADS_PSP2)
	typedef SceUID				ThreadHandleType;
	typedef SceUID				UniqueIdentifierType;
#elif	defined(HH_THREADS_NX)
	typedef nn::os::ThreadType	*ThreadHandleType;
	typedef nn::os::ThreadType	*UniqueIdentifierType;
#else
#	error	unsupported Threading API
#endif

private:
	friend class				CThreadsInternals;

	CThreadID					m_HHThreadID;
	CThreadManager::EPriority	m_Priority;
	CGenericAffinityMask		m_Affinity;

#if	(KR_THREADS_ENABLE_DEBUG_NAMES != 0)
	CString						m_DebugName;
#endif

	// OS-dependent stuff
#if		defined(HH_THREADS_WIN32) || defined(HH_THREADS_XBOX) || defined(HH_THREADS_WINRT) || defined(HH_THREADS_DURANGO)
	static DWORD __stdcall		ThreadStartAddress(void *param);
#elif	defined(HH_THREADS_POSIX)
	static void					*ThreadStartAddress(void *param);
	pthread_mutex_t				m_ThreadStartLocker;
#elif	defined(HH_THREADS_PS3)
	static void					ThreadStartAddress(uint64_t param);
	sys_mutex_t					m_ThreadStartLocker;
#elif	defined(HH_THREADS_ORBIS)
	static void					*ThreadStartAddress(void *param);
	ScePthreadMutex				m_ThreadStartLocker;
#elif	defined(HH_THREADS_PSP2)
	static SceInt32				ThreadStartAddress(SceSize argSize, void *pArgBlock);
	SceUID						m_ThreadStartLocker;
	void						*m_ThreadExecutionBuffer;
#elif	defined(HH_THREADS_NX)
	static void					ThreadStartAddress(void *param);
	nn::os::ThreadType			*m_ThreadStorage;
	void						*m_ThreadStack;
#else
#	error	unsupported Threading API
#endif

	// OS-independent interface starts here:
	UniqueIdentifierType		m_GUID;
	UniqueIdentifierType		m_CreatorGUID;
	ThreadHandleType			m_ThreadHandle;
	hh_u32						m_StackSize;
	volatile bool				m_Started;

protected:
	virtual void				Run() = 0;
	//void						HardKill();	// WARNING: evil, use with caution. this is a brutal, totally unclean termination process, can leave locked critical sections, etc...

public:
	CThread(CThreadManager::EPriority priority = CThreadManager::Priority_Normal, const CGenericAffinityMask *affinity = null, hh_u32 stackSize = 0);	// if 'affinity' is null, will be affinitized on all available logical CPUs
	virtual ~CThread();

	virtual bool				Valid() const { return true; }	// to be overriden if something can fail during construction

	void						Start();
	bool						Started() const volatile { return m_Started; }

	bool						Wait(hh_u32 milliseconds);	// returns true if the thread returned before 'milliseconds'
	void						WaitForCompletion();

	//void						PauseThread();
	//bool						UnPauseThread(bool force = false);	// returns true if the thread has been successfully awaken, false otherwise

	CThreadID					ThreadID() const { return m_HHThreadID; }	// returns CThreadID::INVALID if the thread isn't, testable with val.Valid()
	CThreadLocalStorage			*ThreadLocalStorage() { HH_ASSERT(m_HHThreadID.Valid()); if (m_HHThreadID.Valid()) return CThreadManager::FetchTLS(m_HHThreadID.Valid()); return null; }

	CThreadManager::EPriority	Priority() const;
	void						SetPriority(CThreadManager::EPriority priority);
	const CGenericAffinityMask	&Affinity() const;
	bool						SetAffinity(const CGenericAffinityMask &affinity);
	bool						SetTargetProcessor(hh_u32 logicalProcessorId);

	void						SetDebugThreadName(const char *name);	// does nothing if (KR_THREADS_ENABLE_DEBUG_NAMES == 0)
	const CString				&GetDebugThreadName() const;
};
HH_DECLARE_REFPTRCLASS(Thread);

//----------------------------------------------------------------------------
__PK_API_END

#endif
