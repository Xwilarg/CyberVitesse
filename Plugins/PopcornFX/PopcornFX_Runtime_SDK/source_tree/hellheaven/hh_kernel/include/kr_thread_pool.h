#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2009/01/23 9:12 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_POOLS_H__
#define __KR_THREADS_POOLS_H__

#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_containers_lockless.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Thread Pools / worker threads
//
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(AsynchronousJob);

//----------------------------------------------------------------------------

namespace	Threads
{

	struct	SThreadContext;

	class	HH_KERNEL_EXPORT CAbstractTask : public CRefCountedObjectWithDestructionNotification
	{
	public:
		virtual ~CAbstractTask() {}

		// Called by workers when processing the task
		virtual void		Run(SThreadContext &threadContext) = 0;

#if	(KR_THREADPOOL_PRETTYDEBUG != 0)
		virtual CString		_PrettyDebugName() const { return "UnnamedTask"; }
#endif
	};
	HH_DECLARE_REFPTRCLASS(AbstractTask);

	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CAbstractWaitObject
	{
	public:
		virtual void		Wait() = 0;
		// returns true if finished, false if still needs to wait
		virtual bool		TryWait() = 0;
		virtual bool		TimedWait(hh_u32 milliseconds) = 0;

#if	(KR_THREADPOOL_PRETTYDEBUG != 0)
		virtual CString		_PrettyDebugName() const { return "UnnamedWaitObject"; }
#endif
	};

	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CAbstractPool : public CNonCopyable
	{
	public:
		virtual ~CAbstractPool() {}

		// Tells the workers to expect tasks and keep spinning, or not
		virtual void	ExpectTasks(bool shouldExpectTasks) { }

		// These can be called by tasks to enqueue other dependent tasks
		virtual void	SubmitTask(CAbstractTask *task) = 0;
		virtual void	SubmitTask(const PAbstractTask &task) { SubmitTask(task.Get()); }

		// kick all workers
		// if force == false, won't kick anything if ExpectTasks(true) was called
		virtual void	KickWorkers(bool force = false) { }

		// optimized KickWorkers to only kick workers for tasks
		// if force == false, won't kick anything if ExpectTasks(true) was called
		virtual void	KickTasks(bool force = false) { KickWorkers(force); }

		// Can be used to execute tasks while waiting for waitObject
		// Can be called on update or render thread, but not by workers (so not recursively)
		virtual void	WaitForObject(CAbstractWaitObject &waitObject) { waitObject.Wait(); }

#if	(KR_THREADPOOL_PRETTYDEBUG != 0)
		virtual void	_LogPrettyDebug(const char *name) const { }
#endif
	};

}

//----------------------------------------------------------------------------
//
//	Internal job.
//
//----------------------------------------------------------------------------

#if	(KR_THREADPOOL_PRETTYDEBUG != 0)
#	define PK_THREADPOOL_PRETTYDEBUG_JOB(__name)	virtual CString		_PrettyDebugName() const override { return __name; }
#else
#	define PK_THREADPOOL_PRETTYDEBUG_JOB(__name)
#endif

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CAsynchronousJob : public Threads::CAbstractTask, public TLocklessListElement<TRefPtr<CAsynchronousJob> >
{
private:
	bool									_RemoveDependentRef(Threads::CAbstractPool *pool);

private:
	TAtomic<hh_u32>							m_Dependencies;
	TSemiDynamicArray<PAsynchronousJob, 2>	m_DependentObjects;	// objects that have to wait for our completion to be processed

	virtual bool							_VirtualLaunch(Threads::SThreadContext &threadContext) = 0;
	virtual void							_OnRefptrDestruct() override;

protected:
	void									_Reset();
	bool									_LaunchReady(Threads::SThreadContext &threadContext);

public:
	CAsynchronousJob();
	virtual ~CAsynchronousJob();

	enum	EJobFlags
	{
		JF_Cancelled	= 0x40000000,
		JF_Done			= 0x80000000,

		JF_FULLMASK		= JF_Cancelled | JF_Done,
		JF_CLEARMASK	= ~JF_FULLMASK,
	};

	HH_FORCEINLINE hh_u32		DependenciesJF() const { return m_Dependencies; }
	HH_FORCEINLINE hh_u32		DependentObjectsCount() const { return m_DependentObjects.Count(); }

	void						AddDependency(CAsynchronousJob *job);
	HH_FORCEINLINE void			AddDependency(const PAsynchronousJob &job) { AddDependency(job.Get()); }


	// These are debug-only, only used inside asserts.
	bool						PendingExecution() const { return (m_Dependencies & JF_CLEARMASK) > 0 && (m_Dependencies & JF_FULLMASK) == 0; }
	bool						Ready() const { return (m_Dependencies & JF_CLEARMASK) == 0 && (m_Dependencies & JF_FULLMASK) == 0; }
	bool						Done() const { return (m_Dependencies & JF_Done) != 0; }

	// Called by worker threads when processing the task
	virtual void				Run(Threads::SThreadContext &threadContext) override { _LaunchReady(threadContext); }

	void						AddToPool(Threads::CAbstractPool *targetPool);

#if	(KR_THREADPOOL_PRETTYDEBUG != 0)
	virtual CString				_PrettyDebugName() const override;
	void						_LogPrettyDebug(const CString &prefix, bool printDependencies) const;
#endif
};
HH_DECLARE_REFPTRCLASS(AsynchronousJob);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_THREADS_POOLS_H__
