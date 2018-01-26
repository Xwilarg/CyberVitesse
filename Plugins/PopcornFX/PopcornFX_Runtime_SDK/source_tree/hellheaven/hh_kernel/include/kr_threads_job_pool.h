#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2011/05/23 02:56 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_JOB_POOLS_H__
#define __KR_THREADS_JOB_POOLS_H__

#include "hh_kernel/include/kr_thread_pool.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

//#define	DISABLE_JOB_POOL
#define	USE_LOCKED_FREELIST

//----------------------------------------------------------------------------

namespace	Internal
{
#ifndef	DISABLE_JOB_POOL

	template<typename _Type, hh_u32 _PoolSize = 32>
	struct	TPoolRack
	{
		HH_STATIC_ASSERT(_PoolSize <= 256);	// we use u8 indices

		TPoolRack<_Type, _PoolSize>	*m_Next;
#ifdef	USE_LOCKED_FREELIST
		Threads::CCriticalSection	m_Lock;
		hh_u32						m_FreeCount;
#else
		TAtomic<hh_u32>				m_FreeCount;
#endif
		hh_u8						m_FreeList[_PoolSize];
		_Type						m_Pool[_PoolSize];

		TPoolRack()
		:	m_Next(null)
		,	m_FreeCount(_PoolSize)
		{
			for (hh_u32 i = 0; i < _PoolSize; i++)
			{
#ifdef	USE_LOCKED_FREELIST
				m_FreeList[i] = i;
				m_Pool[i]._SetupJobPool(&m_FreeCount, i);
#else
				m_FreeList[i] = 0;
				m_Pool[i]._SetupJobPool(m_FreeList + i);
#endif
			}
		}

		~TPoolRack()
		{
			HH_ONLY_IF_ASSERTS(
				for (hh_u32 i = 0; i < _PoolSize; ++i)
					HH_ASSERT_MESSAGE(m_Pool[_PoolSize - i - 1].RefCount() == 0, "Deleting referenced job, need to unref all jobs before deleting job pool");
			)
		}

		CGuid	GetFreeSlot()
		{
			if (m_FreeCount != 0)	// no need to lock before, if another thread returns a job in-between, we'll be very short on space anyway, so we don't care allocating a new pool rack
			{
#ifdef	USE_LOCKED_FREELIST
				HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
				return m_FreeList[--m_FreeCount];
#else
				for (hh_u32 i = 0; i < _PoolSize; i++)
				{
					if (m_FreeList[i] == 0)
					{
						if (0 == Atomic::Interlocked_CMPXCHG(m_FreeList + i, hh_u8(1), hh_u8(0)))
						{
							return i;
						}
					}
				}
#endif
			}
			return CGuid::INVALID;
		}

		hh_u32	GetFreeSlotBatch(CGuid *indexBatch, hh_u32 count)
		{
			if (m_FreeCount != 0 && count != 0)	// no need to lock before, if another thread returns a job in-between, we'll be very short on space anyway, so we don't care allocating a new pool rack
			{
#ifdef	USE_LOCKED_FREELIST
				HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);

				hh_u32	stop = HHMin(m_FreeCount, count);
				for (hh_u32 i = 0; i < stop; i++)
				{
					indexBatch[i] = m_FreeList[--m_FreeCount];
				}
//				m_FreeCount -= stop;
				return stop;
#else
				hh_u32	doneCount = 0;
				for (hh_u32 i = 0; i < _PoolSize; i++)
				{
					if (m_FreeList[i] == 0)
					{
						if (0 == Atomic::Interlocked_CMPXCHG(m_FreeList + i, hh_u8(1), hh_u8(0)))
						{
							indexBatch[doneCount++] = i;
							if (doneCount == count)
								return count;
						}
					}
				}
				return doneCount;
#endif
			}
			return 0;
		}

	};
#endif
}

//----------------------------------------------------------------------------
//
//	pooled job. inherit from this instead of 'CAsynchronousJob'
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CAsynchronousPooledJob : public CAsynchronousJob
{
private:
#ifndef	DISABLE_JOB_POOL
#ifdef	USE_LOCKED_FREELIST
	hh_u32			*m_EntryPoint;	// no need for TAtomic here: we set to true in a task, but we test and clear in the main thread only
	hh_u32			m_SelfIndex;
#else
	hh_u8			*m_UsedFlagPtr;
#endif
#endif

protected:
	virtual void	_OnRefptrDestruct() override;

public:
	CAsynchronousPooledJob()
	:	CAsynchronousJob()
#ifndef	DISABLE_JOB_POOL
#ifdef	USE_LOCKED_FREELIST
	,	m_EntryPoint(null)
	,	m_SelfIndex(hh_u32(-1))
#else
	,	m_UsedFlagPtr(null)
#endif
#endif
	{
	}

#ifndef	DISABLE_JOB_POOL
#ifdef	USE_LOCKED_FREELIST
	void	_SetupJobPool(hh_u32 *freeCountPtr, hh_u32 selfIndex)
	{
		// hack: we don't want to be dependent on the template parameters of the pool container here.
		// so we store a pointer to the free-count, and we know the free slots 'end' is at:
		// (hh_u8*)entryPointPtr - sizeof(void*).
		// we can get away with this because we push-back in reverse order, like the machine stack.
		// similarly, we will compute our current index in '_OnRefptrDestruct' by subtracting:
		// this - (typeof<this>*)(entryPointPtr + 1)
		m_EntryPoint = freeCountPtr;
		m_SelfIndex = selfIndex;
	}
#else
	void	_SetupJobPool(hh_u8 *usedFlagPtr)
	{
		m_UsedFlagPtr = usedFlagPtr;
	}
#endif

#ifdef	USE_LOCKED_FREELIST
	HH_ONLY_IF_ASSERTS(
		bool	_CorrectlySetup(CGuid id) const
		{
			return m_EntryPoint != null && m_SelfIndex == id && DependenciesJF() == 1 && /*m_Flags == JF_None && */DependentObjectsCount() == 0;
		}
	)
#else
	HH_ONLY_IF_ASSERTS(
		bool	_CorrectlySetup(CGuid id) const
		{
			return m_UsedFlagPtr != null && DependenciesJF() == 1 && /*m_Flags == JF_None && */DependentObjectsCount() == 0;
		}
	)
#endif
#endif
};

//----------------------------------------------------------------------------
//
//	job pool
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _PoolSize = 32>
class	TAsynchronousJobPool
{
private:
#ifndef	DISABLE_JOB_POOL

	typedef	Internal::TPoolRack<_Type, _PoolSize>	SPoolRack;

	Threads::CCriticalSection	m_Lock;
	SPoolRack					m_PoolList;	// first pool is located within the object itself, don't bother allocating it

	void	_DestroyPoolChain(SPoolRack *pool)
	{
		while (pool != null)
		{
			SPoolRack	*nextPool = pool->m_Next;
			HH_DELETE(pool);
			pool = nextPool;
		}
	}
#endif

public:
	TAsynchronousJobPool()
	{
	}

	~TAsynchronousJobPool()
	{
#ifndef	DISABLE_JOB_POOL
		_DestroyPoolChain(m_PoolList.m_Next);
		m_PoolList.m_Next = null;
#endif
	}

#ifdef	DISABLE_JOB_POOL

	TRefPtr<_Type>	PullJob()
	{
		return HH_NEW(_Type);
	}
	bool			BatchedPullJob(const TMemoryView<TRefPtr<_Type> > &outJobBatch)
	{
		for (hh_u32 i = 0; i < outJobList.Count(); i++)
		{
			outJobList[i] = PullJob();
			if (outJobList[i] == null)
				return false;
		}
		return true;
	}

#else

	TRefPtr<_Type>	PullJob()
	{
		SPoolRack	*curPool = &m_PoolList;
#ifdef	HH_DEBUG
		hh_u32		currentRackCount = 0;
#endif

		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		do
		{
			const CGuid	id = curPool->GetFreeSlot();
			if (id.Valid())
			{
				HH_ASSERT(curPool->m_Pool[id]._CorrectlySetup(id));
				return curPool->m_Pool + id;
			}

#ifdef	HH_DEBUG
			currentRackCount++;
#endif

			curPool = curPool->m_Next;
		} while (curPool != null);

#ifdef	HH_DEBUG
		CLog::Log(HH_INFO, "Growing job pool by %d elements (%d -> %d)", _PoolSize, currentRackCount * _PoolSize, (currentRackCount + 1) * _PoolSize);
#endif

		// no free job found. we need to add a new pool
		curPool = HH_NEW_ALIGNED(SPoolRack, 128);
		if (curPool != null)
		{
			curPool->m_Next = m_PoolList.m_Next;
			m_PoolList.m_Next = curPool;

			//curPool->m_UsedFlagPtr[0] = 0;
			const CGuid	id = curPool->GetFreeSlot();
			if (id.Valid())
			{
				HH_ASSERT(curPool->m_Pool[id]._CorrectlySetup(id));
				return curPool->m_Pool + id;
			}
		}
		return null;
	}

	// returns true when all the requested jobs were provided.
	// if false is returned, an allocation failed, at least one job is null, but there might be some valid ones...
	bool			BatchedPullJob(const TMemoryView<TRefPtr<_Type> > &outJobBatch)
	{
		static const hh_u32	kIndexBatchSize = 32;
		CGuid				indexBatch[kIndexBatchSize];

		SPoolRack			*curPool = &m_PoolList;
		const hh_u32		totalCount = outJobBatch.Count();
		hh_u32				doneCount = 0;
#ifdef	HH_DEBUG
		hh_u32				currentRackCount = 0;
#endif

		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		do
		{
			const hh_u32	leftCount = totalCount - doneCount;
			const hh_u32	successfulFetches = curPool->GetFreeSlotBatch(indexBatch, HHMin(leftCount, kIndexBatchSize));

			for (hh_u32 i = 0; i < successfulFetches; i++)
			{
				outJobBatch[doneCount++] = curPool->m_Pool + indexBatch[i];
				HH_ASSERT(outJobBatch[doneCount - 1]->_CorrectlySetup(indexBatch[i]));
			}

#ifdef	HH_DEBUG
			currentRackCount++;
#endif

			curPool = curPool->m_Next;
		} while (curPool != null && doneCount != totalCount);

		while (doneCount != totalCount)
		{
#ifdef	HH_DEBUG
			CLog::Log(HH_INFO, "Growing job pool by %d elements (%d -> %d)", _PoolSize, currentRackCount * _PoolSize, (currentRackCount + 1) * _PoolSize);
#endif

			// no free job found. we need to add a new pool
			curPool = HH_NEW_ALIGNED(SPoolRack, 128);
			if (curPool != null)
			{
				curPool->m_Next = m_PoolList.m_Next;
				m_PoolList.m_Next = curPool;

				const hh_u32	leftCount = totalCount - doneCount;
				const hh_u32	successfulFetches = curPool->GetFreeSlotBatch(indexBatch, HHMin(leftCount, kIndexBatchSize));

				for (hh_u32 i = 0; i < successfulFetches; i++)
				{
					outJobBatch[doneCount++] = curPool->m_Pool + indexBatch[i];
					HH_ASSERT(outJobBatch[doneCount - 1]->_CorrectlySetup(indexBatch[i]));
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
#endif
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_THREADS_JOB_POOLS_H__
