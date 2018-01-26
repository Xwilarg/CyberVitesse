#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/12/01 12:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_POOLS_DEFAULT_H__
#define __KR_THREADS_POOLS_DEFAULT_H__

#include "hh_kernel/include/kr_thread_pool.h"

#if	(PK_THREADPOOL_HAS_INTERNAL_IMPLEM != 0)

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Thread Pools / worker threads
//
//----------------------------------------------------------------------------

#ifdef	HH_HAS_THREAD_AWAKER
#	define HH_THREADPOOL_USE_AWAKER
#endif

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(WorkerThread);
HH_FORWARD_DECLARE(AsynchronousJob);

namespace Threads
{
	struct	SThreadContext;
}

//----------------------------------------------------------------------------

struct	SWorkerThreadStats
{
	enum	EFlags
	{
		Asleep = 0x1,
	};
	hh_u32						m_Flags;
	hh_u32						m_ProcessedJobsCount;
	CThreadManager::EPriority	m_Priority;
};

//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT SWorkerThreadBucket
{
	TLocklessStack<PAsynchronousJob>		m_PulledJobs;
	TSemiDynamicArray<PWorkerThread, 8>		m_ScavengingTargets;
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CWorkerThreadPool : public Threads::CAbstractPool
{
private:
protected:
	friend class	CWorkerThread;

	CThreadManager::EPriority			m_WorkersPriority;
	TArray<PWorkerThread>				m_WorkerThreads;

#if 0
	TLockedQueue_RW<PAsynchronousJob>	m_Jobs;
#elif 0
	TLocklessQueue<PAsynchronousJob>	m_Jobs;
#else
	TLockedQueue_CS<PAsynchronousJob>	m_Jobs;
#endif

#ifdef	HH_THREADPOOL_USE_AWAKER
	Threads::CAwaker					m_ThreadWaker;
#endif
	TAtomic<hh_u32>						m_SpinForTasks;

	TArray<Threads::SThreadContext*>	m_WaitForObject_ThreadContexts;

	bool								m_WaitsProcessesTasks;

	Threads::SThreadContext*			_AquireWaitForObjectThreadContext();
	void								_ReleaseWaitForObjectThreadContext(Threads::SThreadContext *context);

	void								_RemoveThread(hh_u32 id);
#if	(PK_THREADS_WAITS_ASSERTS != 0)
	void								_WaitAssert(const char *prettyName, hh_u32 time);
#endif

public:
	CWorkerThreadPool();
	virtual ~CWorkerThreadPool();

	hh_u32								WorkerThreadsCount() const { return m_WorkerThreads.Count(); }
	CThreadManager::EPriority			WorkersPriority() { return m_WorkersPriority; }

	void								SetWorkersCount(hh_u32 threadCount);
	void								SetWorkersPriority(CThreadManager::EPriority priority);

	void								GetWorkersSchedule(TArray<CThreadManager::EPriority> &outputPriorities) const;
	void								ScheduleWorkers(const TStridedMemoryView<CThreadManager::EPriority> &priorities);
	void								GetWorkersSchedule(TArray<CGenericAffinityMask> &outputAffinities) const;
	void								ScheduleWorkers(const TStridedMemoryView<CGenericAffinityMask> &affinities);
	void								GetWorkersSchedule(TArray<TPair<CThreadManager::EPriority, CGenericAffinityMask> > &outputSchedules) const;
	void								ScheduleWorkers(const TStridedMemoryView<TPair<CThreadManager::EPriority, CGenericAffinityMask> > &schedules);

	CWorkerThread						*AddWorker();	// will use the default priority/affinity declared above.
	CWorkerThread						*AddWorker(CThreadManager::EPriority priority, const CGenericAffinityMask *affinity = null, bool nameThreadIFN = true);

	struct	SNumaWorkersConfig
	{
		const CGenericAffinityMask			*m_Affinity; // null will use CPU::Caps().ProcessAffinity()
		CThreadManager::EPriority			m_WorkersPriorities;
		hh_i32								m_MainThreadCpuId; // -1 will use m_Affinity->FirstBitSet()
		bool								m_DisableWorkerOnMainThread;
		hh_u32								m_AdditionalWorkersPerLogicalProcessor;
		bool								m_RestrainAdditionalWorkersPriorities;

		SNumaWorkersConfig()
		:	m_Affinity(null)
		,	m_WorkersPriorities(CThreadManager::Priority_High)
		,	m_MainThreadCpuId(-1)
		,	m_DisableWorkerOnMainThread(true)
		,	m_AdditionalWorkersPerLogicalProcessor(0)
		,	m_RestrainAdditionalWorkersPriorities(false)
		{}
	};

	// 1 worker per logical core with affinity on a single core for each, with SNumaWorkersConfig subtleties
	bool								AddNUMAWorkers(const SNumaWorkersConfig &config = SNumaWorkersConfig());
	bool								AddNUMAWorkers_FromAffinityMask(const CGenericAffinityMask &affinity,
																		CThreadManager::EPriority workersPriorities,
																		hh_u32 additionalWorkersPerLogicalProcessor = 0,
																		bool restrainAdditionalWorkersPriorities = false)
	{
		SNumaWorkersConfig				config;
		config.m_Affinity = &affinity;
		config.m_WorkersPriorities = workersPriorities;
		config.m_AdditionalWorkersPerLogicalProcessor = additionalWorkersPerLogicalProcessor;
		config.m_RestrainAdditionalWorkersPriorities = restrainAdditionalWorkersPriorities;
		return AddNUMAWorkers(config);
	}

	// N workers with "affinity" for everyone
	bool								AddFullAffinityWorkers(hh_u32 workerCount, const CGenericAffinityMask &affinity, CThreadManager::EPriority workersPriorities);

	void								StartWorkers();

	void								FillWorkerThreadsStats(TArray<SWorkerThreadStats> &outputStats) const;
	hh_u32								ActiveWorkerThreadsCount();
	hh_u32								JobsCount() const { return m_Jobs.Count(); }

	// if true, WaitForObject will process tasks will waiting (true by default)
	void								SetWaitsProcessesTasks(bool waitsProcessesTasks) { m_WaitsProcessesTasks = waitsProcessesTasks; }
	bool								WaitsProcessesTasks() const { return m_WaitsProcessesTasks; }

	// CAbstractPool overridable interface
	virtual void						ExpectTasks(bool shouldExpectTasks) override;
	virtual void						SubmitTask(Threads::CAbstractTask *task) override;
	virtual void						KickWorkers(bool force = false) override;
	virtual void						KickTasks(bool force = false) override;
	virtual void						WaitForObject(Threads::CAbstractWaitObject &waitObject) override;

//	hh_u32								SpinsForTasks() const { return m_SpinForTasks; }

#if	(KR_THREADPOOL_PRETTYDEBUG != 0)
	virtual void						_LogPrettyDebug(const char *name) const override;
#endif
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// (PK_THREADPOOL_HAS_INTERNAL_IMPLEM != 0)

#endif // __KR_THREADS_POOLS_H__
