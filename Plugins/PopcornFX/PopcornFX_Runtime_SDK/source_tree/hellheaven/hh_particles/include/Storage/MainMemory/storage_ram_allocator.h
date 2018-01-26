#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2010/07/14 19:32 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PS_STORAGE_RAM_ALLOCATOR_H__
#define __PS_STORAGE_RAM_ALLOCATOR_H__

#include "hh_particles/include/ps_config.h"

#include <hh_kernel/include/kr_threads_basics.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleRawPage_MainMemory);
class	CParticlePageAllocator;

//----------------------------------------------------------------------------

class	CParticleRawPage_MainMemory : public CRefCountedObject
{
public:
	CParticleRawPage_MainMemory() : m_Data(null), m_Size(0), m_Allocator(null) { }
	CParticleRawPage_MainMemory(CParticlePageAllocator *allocator, hh_u8 *data, hh_u32 size) : m_Data(data), m_Size(size), m_Allocator(allocator) { }
	virtual ~CParticleRawPage_MainMemory() { Clear(); }

	bool		Valid() const { return m_Data != null; }

	bool		Empty() const
	{
		HH_ASSERT((m_Data == null && m_Size == 0 && m_Allocator == null) ||
			(m_Data != null && m_Size != 0 && m_Allocator != null));
		return m_Data == null;
	}
	void		Clear();
	void		SetAlloc(CParticlePageAllocator *allocator, hh_u8 *data, hh_u32 size)
	{
		if (!Empty())
			Clear();
		m_Data = data;
		m_Size = size;
		m_Allocator = allocator;
	}

	hh_u8			*RawDataForWriting() { HH_ASSERT(m_ReadersCount == 0); return m_Data; }
	const hh_u8		*RawData() const { return m_Data; }
	hh_u32			Size() const { return m_Size; }

	bool			Contains(void *ptr, hh_u32 sizeInBytes = 0) const { return hh_ureg(ptr) >= hh_ureg(m_Data) && hh_ureg(ptr) + hh_ureg(sizeInBytes) <= hh_ureg(m_Data) + hh_ureg(m_Size); }

	hh_u32			ReadersCount() const { return m_ReadersCount; }
	void			IncReadersCount() const { ++m_ReadersCount; }
	bool			DecReadersCount() const { return (--m_ReadersCount) == 0; }

	PParticleRawPage_MainMemory		AllocateNewOfSameSize() const;

private:
	hh_u8						*m_Data;
	hh_u32						m_Size;
	mutable TAtomic<hh_u32>		m_ReadersCount;
	CParticlePageAllocator		*m_Allocator;
};
HH_DECLARE_REFPTRCLASS(ParticleRawPage_MainMemory);

//----------------------------------------------------------------------------

struct	CParticlePageAllocator_Stats;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticlePageAllocator
{
public:
	static const hh_u32				kPoolSlotCount = 64;	// number of size entries in the pool allocator
	static const hh_u32				MinAllocSize = HH_PARTICLES_ALLOCATOR_PAGESIZE_MIN;
	static const hh_u32				MaxAllocSize = HH_PARTICLES_ALLOCATOR_PAGESIZE_MAX;

private:
	static hh_u32					_SizeToSlotId(hh_u32 sizeInBytes);
	static hh_u32					_SlotIdToSize(hh_u32 slotId);

	struct	SBlock
	{
		hh_u8	*m_Ptr;
		hh_u32	m_Date;

		SBlock(hh_u8 *ptr, hh_u32 date) : m_Ptr(ptr), m_Date(date) {}
	};

	struct	SBlockPool
	{
		hh_u32			m_BlockSize;
		TArray<SBlock>	m_Pool;

		SBlockPool(hh_u32 blockSize = 0) : m_BlockSize(blockSize) {}
	};

	TArray<SBlockPool>						m_Blocks;
	Threads::CCriticalSection				m_Lock;
	hh_u32									m_TotalMemoryUsed;
	hh_u32									m_CurrentDate;

	CParticlePageAllocator_Stats			*m_Stats;

public:
	CParticlePageAllocator();
	~CParticlePageAllocator();

	bool							Setup();
	bool							Valid() const;

	hh_u8							*_Alloc(hh_u32 size);
	void							_Retire(hh_u8 *ptr, hh_u32 size);

	PParticleRawPage_MainMemory		Alloc(hh_u32 size);

	void							CleanAllFreePages();

	void							Update();

	hh_u32							SpaceLeft() const;

	hh_u32							TotalMemoryAllocated() const { return m_TotalMemoryUsed; }
	hh_u32							TotalMemoryAvailable() const;

	// returns false if disabled
	bool							GetTotalStats(CParticlePageAllocator_Stats &out) const;
};

//----------------------------------------------------------------------------

struct	CParticlePageAllocator_Stats
{
	// Allocated ---> Used <---> Unused ---> Freed

	hh_u64		m_Allocated;
	hh_u64		m_AllocCount;
	hh_u64		m_Freed;
	hh_u64		m_FreeCount;
	hh_u64		m_Used;
	hh_u64		m_Unused;
	hh_u64		m_RealUsed;
	hh_u64		m_RealUnused;

	hh_u32		m_SlotSizes[CParticlePageAllocator::kPoolSlotCount];
	hh_u32		m_SlotCounts[CParticlePageAllocator::kPoolSlotCount];

	CParticlePageAllocator_Stats() { Clear(); }

	void		Clear();

	void		operator = (const CParticlePageAllocator_Stats &src);
	void		operator += (const CParticlePageAllocator_Stats &src);
	void		operator -= (const CParticlePageAllocator_Stats &src);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PS_STORAGE_RAM_ALLOCATOR_H__
