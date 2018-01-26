#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2003/07/02 05:25 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_PARALLEL_SORT_H__
#define	__KR_PARALLEL_SORT_H__

#include <hh_kernel/include/kr_sort.h>
#include <hh_kernel/include/kr_scheduler.h>
#include <hh_kernel/include/kr_threads_job_pool.h>
#include <hh_kernel/include/kr_workingbuffer.h>
#include <hh_kernel/include/kr_containers_array.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

// ifdef jobs all sort jobs are added firsts (linear algo)
// ifndef jobs are added deep-first (recurse algo)
//#define		HH_PARALLELSORT_LINEARSPAWN

struct	SParallelSort_Batch
{
	hh_u32		m_Offset;
	hh_u32		m_Count;
};

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
class	TParallelSort : public _PolicyWrapper
{
public:
	typedef _PolicyWrapper						PolicyWrapper;
	typedef TParallelSort<PolicyWrapper>		Self;
	typedef	typename PolicyWrapper::SSortData			SSortData;
	typedef	typename PolicyWrapper::SSortKeyExtractor	SSortKeyExtractor;

protected:
	using PolicyWrapper::ReadyToSpawn;
	using PolicyWrapper::FillSource;
	using PolicyWrapper::Finished;

public:
	class	CJob : public CAsynchronousPooledJob
	{
		PK_THREADPOOL_PRETTYDEBUG_JOB("TParallelSort<>::CJob");

		friend class TParallelSort<PolicyWrapper>;
	public:
		CJob() : m_Self(null), m_Exec(null) { }
		~CJob() { }
		virtual bool	_VirtualLaunch(Threads::SThreadContext &threadContext) override
		{
			HH_ASSERT(m_Self != null && m_Exec != null);
			(m_Self->*m_Exec)(*this);
			return true;
		}
#ifdef	HH_PARALLELSORT_LINEARSPAWN
		hh_u32			SrcLevel() const { return m_Level; }
		hh_u32			DstLevel() const { HH_ASSERT(m_Level > 0); return m_Level - 1; }
#else
		hh_u32			SrcLevel() const { return m_Level; }
		hh_u32			DstLevel() const { HH_ASSERT(m_Level > 0); return m_Level + 1; }
#endif

	private:
		Self			*m_Self;
		void			(Self::*m_Exec)(const CJob &job);
		hh_u32			m_Level;
		hh_u32			m_Start;
		hh_u32			m_Count;
		hh_u32			m_Count1; // to merge
		hh_u32			m_Count2; // to merge
	};
	HH_DECLARE_REFPTRCLASS(Job);

	typedef TAsynchronousJobPool<CJob>	CJobPool;

public:
	TParallelSort() : m_LastJobPool(null) { }
	~TParallelSort() { }

	PJob				SpawnJobs(hh_u32 batchSize, CJobPool *jobPool);
	void				ImmediateSort();
	void				Clean() { m_WorkingBuffer.Clean(); }

private:
	void				_NewJob(PJob &job) { HH_ASSERT(m_LastJobPool != null); job = m_LastJobPool->PullJob(); }
	PJob				Spawn(hh_u32 start, hh_u32 offset, hh_u32 level);

	void				_Sort(const CJob &job);
	void				_Merge(const CJob &job);
	void				_Finish(const CJob &job);

	CJobPool			*m_LastJobPool;

	CWorkingBuffer				m_WorkingBuffer;
	TStaticArray<TMemoryView<SSortData>, 2>		m_Wbs;
	hh_u32						m_TotalCount;
	hh_u32						m_BatchSize;

	TArray<PJob>				m_Jobs;
};

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
void	TParallelSort<_PolicyWrapper>::ImmediateSort()
{
	const hh_u32	elementCount = PolicyWrapper::ReadyToSpawn(1 << 12); // whatever
	if (!HH_VERIFY(elementCount > 0))
		return;

	const hh_u32	alignedCount = Mem::Align<0x10>(elementCount) +0x10;
	SSortData		*wb = m_WorkingBuffer.GetSome<SSortData>(alignedCount * 2);
	if (!HH_VERIFY(wb != null))
		return;

	TMemoryView<SSortData>			src;
	const TMemoryView<SSortData>	srcWb(wb, elementCount);
	const TMemoryView<SSortData>	dst(wb + alignedCount, elementCount);
	HH_ASSERT_ALIGNED16(srcWb.Data());
	HH_ASSERT_ALIGNED16(dst.Data());
	{
		PolicyWrapper::FillSource(0, elementCount, src, srcWb);
		HH_ASSERT(!src.Empty());
	}
	HH_ASSERT_ALIGNED16(src.Data());
	{
		TRadixSort<Sort_InBuffer>::Sort11<SSortKeyExtractor>(src.Data(), src.DataEnd(), dst.Data());
	}
	PolicyWrapper::Finished(dst);
}

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
typename TParallelSort<_PolicyWrapper>::PJob	TParallelSort<_PolicyWrapper>::SpawnJobs(hh_u32 minBatchSize, CJobPool *jobPool)
{
	HH_ASSERT(jobPool != null);
	m_LastJobPool = jobPool;

	const hh_u32	elementCount = PolicyWrapper::ReadyToSpawn(minBatchSize);
	if (!HH_VERIFY(elementCount > 0))
		return null;

	const hh_u32	alignedCount = Mem::Align<0x10>(elementCount) + 0x10;
	SSortData		*wb = m_WorkingBuffer.GetSome<SSortData>(alignedCount * 2);
	if (!HH_VERIFY(wb != null))
		return null;

	m_Wbs[0] = TMemoryView<SSortData>(wb, elementCount);
	m_Wbs[1] = TMemoryView<SSortData>(wb + alignedCount, elementCount);
	m_TotalCount = elementCount;
	m_BatchSize = minBatchSize;

	PJob	lastJob = Spawn(0, elementCount, 1);
	PJob	finish;
	_NewJob(finish);

	finish->m_Self = this;
	finish->m_Exec = &Self::_Finish;
	finish->m_Level = lastJob->DstLevel();
	finish->m_Start = 0;
	finish->m_Count = m_TotalCount;
	finish->AddDependency(lastJob);

	Threads::CAbstractPool	*threadPool = Scheduler::ThreadPool();
	lastJob->AddToPool(threadPool);
	lastJob = finish;

	return lastJob;
}

//----------------------------------------------------------------------------
#ifdef	HH_PARALLELSORT_LINEARSPAWN

template<typename _PolicyWrapper>
typename TParallelSort<_PolicyWrapper>::PJob		TParallelSort<_PolicyWrapper>::Spawn(hh_u32 , hh_u32 , hh_u32 )
{
	HH_ASSERT(m_TotalCount > 0);

	HH_RELEASE_ASSERT(IntegerTools::IsPowerOfTwoNz(m_BatchSize));
	const hh_u32			batchPow = IntegerTools::Log2(m_BatchSize);
	const hh_u32			countPow = IntegerTools::Log2(m_TotalCount);
	const hh_u32			levels = HHMax(countPow, batchPow) - batchPow;

	const hh_u32			batchCount = 1 << levels;

	{
		m_Jobs.Clear();

		hh_u32				totalJobs = 0;
		for (hh_u32 i = 0; i <= levels; ++i)
			totalJobs += 1 << i;
		if (!m_Jobs.Resize(totalJobs))
		{
			HH_ASSERT_NOT_REACHED();
			return null;
		}
	}

	Threads::CAbstractPool	*threadPool = Scheduler::ThreadPool();

	hh_u32	lastJobOffset = 0;
	hh_u32	jobOffset = 0;

	hh_u32	batchCountAlig = Mem::Align<0x10>(m_TotalCount / batchCount);
	hh_u32	off = 0;
	for (hh_u32 i = 0; i < batchCount; ++i)
	{
		PJob	&job = m_Jobs[jobOffset++];
		_NewJob(job);
		job->m_Self = this;
		job->m_Exec = &Self::_Sort;
		job->m_Level = 1;
		job->m_Start = off;
		job->m_Count = batchCountAlig;
		off += batchCountAlig;
	}
	m_Jobs[batchCount - 1]->m_Count += (m_TotalCount - off); // last job a bit larger

	for (hh_u32 level = 1; level <= levels; ++level)
	{
		const hh_u32	mergeCount = 1 << (levels - level);
		HH_ONLY_IF_ASSERTS(hh_u32	_jobOffset = jobOffset);
		for (hh_u32 i = 0; i < mergeCount; ++i)
		{
			PJob	&job1 = m_Jobs[lastJobOffset++];
			PJob	&job2 = m_Jobs[lastJobOffset++];
			PJob	&merge = m_Jobs[jobOffset++];

			_NewJob(merge);
			merge->m_Self = this;
			merge->m_Exec = &Self::_Merge;
			merge->m_Level = level + 1;
			HH_ASSERT(job2->m_Start == job1->m_Start + job1->m_Count);
			merge->m_Start = job1->m_Start;
			merge->m_Count = job1->m_Count + job2->m_Count;
			merge->m_Count1 = job1->m_Count;
			merge->m_Count2 = job2->m_Count;

			merge->AddDependency(job1);
			merge->AddDependency(job2);
			job1->AddToPool(threadPool);
			job2->AddToPool(threadPool);
		}
		HH_ASSERT(lastJobOffset == _jobOffset);
	}
	HH_ASSERT(jobOffset == m_Jobs.Count());
	PJob		lastJob = m_Jobs[jobOffset - 1];
	HH_ASSERT(lastJob != null);

	// all jobs expect lastJob should have been added
	m_Jobs.Clear();

	return lastJob;
}

#else // HH_PARALLELSORT_LINEARSPAWN

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
typename TParallelSort<_PolicyWrapper>::PJob	TParallelSort<_PolicyWrapper>::Spawn(hh_u32 start, hh_u32 count, hh_u32 level)
{
	HH_ASSERT(count > 0);
	HH_ASSERT(start + count <= m_TotalCount);

	const hh_u32	c1 = Mem::Align<0x10>((count / 2) + 1);
	//HH_ASSERT(c1 <= count);
	const hh_u32	c2 = count - c1;
	if (c1 <= m_BatchSize || c2 <= m_BatchSize)
	{
		PJob		job;
		_NewJob(job);
		job->m_Self = this;
		job->m_Exec = &Self::_Sort;
		job->m_Level = level;
		job->m_Start = start;
		job->m_Count = count;
		return job;
	}

	PJob		job1 = Spawn(start, c1, level + 1);
	PJob		job2 = Spawn(start + c1, c2, level + 1);

	HH_ASSERT(job1->m_Count + job2->m_Count == count);
	HH_ASSERT(job2->m_Start == job1->m_Start + job1->m_Count);

	PJob		merge;
	_NewJob(merge);
	merge->m_Self = this;
	merge->m_Exec = &Self::_Merge;
	merge->m_Level = level;
	merge->m_Start = start;
	merge->m_Count = count;
	merge->m_Count1 = job1->m_Count;
	merge->m_Count2 = job2->m_Count;

	merge->AddDependency(job1);
	merge->AddDependency(job2);

	Threads::CAbstractPool	*threadPool = Scheduler::ThreadPool();
	job1->AddToPool(threadPool);
	job2->AddToPool(threadPool);
	return merge;
}

#endif // HH_PARALLELSORT_LINEARSPAWN

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
void	TParallelSort<_PolicyWrapper>::_Sort(const CJob &job)
{
	HH_SCOPEDPROFILE();
	TMemoryView<SSortData>	src;
	TMemoryView<SSortData>	srcWb = m_Wbs[job.SrcLevel() % 2].Slice(job.m_Start, job.m_Count);
	TMemoryView<SSortData>	dst = m_Wbs[job.DstLevel() % 2].Slice(job.m_Start, job.m_Count);
	HH_ASSERT_ALIGNED16(srcWb.Data());
	HH_ASSERT_ALIGNED16(dst.Data());
	{
		PolicyWrapper::FillSource(job.m_Start, job.m_Count, src, srcWb);
		HH_ASSERT(!src.Empty());
	}
	HH_ASSERT_ALIGNED16(src.Data());
	{
		TRadixSort<Sort_InBuffer>::Sort11<SSortKeyExtractor>(src.Data(), src.DataEnd(), dst.Data());
	}
}

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
void	TParallelSort<_PolicyWrapper>::_Merge(const CJob &job)
{
	HH_SCOPEDPROFILE();
	TMemoryView<SSortData>	src1 = m_Wbs[job.SrcLevel() % 2].Slice(job.m_Start, job.m_Count1);
	TMemoryView<SSortData>	src2 = m_Wbs[job.SrcLevel() % 2].Slice(job.m_Start + job.m_Count1, job.m_Count2);
	TMemoryView<SSortData>	dst = m_Wbs[job.DstLevel() % 2].Slice(job.m_Start, job.m_Count);
	HH_ASSERT_ALIGNED16(src1.Data());
	HH_ASSERT_ALIGNED16(src2.Data());
	HH_ASSERT_ALIGNED16(dst.Data());

	const SSortData			* s1 = src1.Data();
	const SSortData			* const s1end = src1.DataEnd();
	const SSortData			* s2 = src2.Data();
	const SSortData			* const s2end = src2.DataEnd();
	SSortData				*d = dst.Data();

	while (s1 < s1end && s2 < s2end)
	{
		const bool		take1 = SSortKeyExtractor::ExtractKey(*s1) < SSortKeyExtractor::ExtractKey(*s2);
		*d = (take1 ? *s1 : *s2);
		(take1 ? ++s1 : ++s2);
		// s1 = s1 + take1;
		// s2 = s2 + (1 - take1);
		++d;
	}
	while (s1 < s1end)
	{
		*d = *s1;
		++s1;
		++d;
	}
	while (s2 < s2end)
	{
		*d = *s2;
		++s2;
		++d;
	}
}

//----------------------------------------------------------------------------

template<typename _PolicyWrapper>
void	TParallelSort<_PolicyWrapper>::_Finish(const CJob &job)
{
	HH_SCOPEDPROFILE();
	PolicyWrapper::Finished(m_Wbs[job.SrcLevel() % 2]);
}

__PK_API_END
//----------------------------------------------------------------------------

#endif // __KR_PARALLEL_SORT_H__
