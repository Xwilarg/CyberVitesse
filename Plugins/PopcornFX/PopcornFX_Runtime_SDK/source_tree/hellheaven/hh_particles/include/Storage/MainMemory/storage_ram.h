#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2012/12/05 01:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_STORAGE_RAM_H__
#define	__PS_STORAGE_RAM_H__

#include "hh_particles/include/ps_storage.h"
#include "hh_particles/include/Storage/MainMemory/storage_ram_stream.h"

#include <hh_kernel/include/kr_threads_rwlock.h>

// define this to use a reader writer spin lock on PageAllocator instead of a full lock
#define	USE_STORAGE_RWSPINLOCK

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticlePageAllocator;
class	CParticleStorageManager_MainMemory;

//----------------------------------------------------------------------------

class	CParticleStorage_MainMemory : public CParticleStorage
{
protected:
	CParticleDescriptor::EStorageSizeHint		m_PrefferedStorageSize;
	hh_u32										m_PageSize_Min;
	hh_u32										m_PageSize_Max;

	hh_u32										m_HighestPageUsage;
	float										m_TimeSinceLastPageResize;

	// warning: should match the values in pv_particles.h
	CParticleStream_MainMemory					m_NewParticles[2];
#ifndef	USE_STORAGE_RWSPINLOCK
	Threads::CCriticalSection					m_NewParticlesLock;
#else
	Threads::CRWSpinLock						m_NewParticlesRWLock;
#endif

	hh_u32										m_NewParticlesForAdd;

	CParticleStream_MainMemory					m_ParticleStream;
	CParticleStream_MainMemory					m_ParticleStreamForRendering;

	bool										_Empty() const;

public:
	CParticleStorage_MainMemory(CParticleStorageManager_MainMemory *manager);
	virtual ~CParticleStorage_MainMemory();

	virtual bool								BindStreamDef(const SParticleStreamDef *streamDef, CParticleDescriptor::EStorageSizeHint storageSizeHint) override;
	virtual const SParticleStreamDef			*GetStreamDef() const override;
	virtual bool								Update(float dt) override;	// returns false if empty, true if not-empty
	virtual void								Clear() override;
	virtual void								Clean() override;
	virtual hh_u32								ActiveParticleCount() const override;
	virtual bool								Empty() const override { return _Empty(); }
	virtual bool								EmptyForRendering() const override { return ParticleStreamForRendering().Empty(); }

	virtual CParticleStream_MainMemory			&ParticleStream() override { return m_ParticleStream; }
	virtual const CParticleStream_MainMemory	&ParticleStream() const override { return m_ParticleStream; }
	virtual CParticleStream_MainMemory			&ParticleStreamForRendering() override;
	virtual const CParticleStream_MainMemory	&ParticleStreamForRendering() const override;

	// These are memory-specific, remove them:
	virtual CParticlePageView					LockAllocator_AndAllocateParticlesIFP(hh_u32 wishedCount, hh_u32 minCountHint = 0) override;	// on failure, returns an empty page. the function may allocate less than 'wishedCount' contiguous particles, and may need to be called multiple times
	virtual void								UnlockAllocator() override;


	virtual void								SetHasDoubleBufferForRendering(bool enabled) override;
	virtual void								SetHasDoubleBufferManualUpdate(bool enabled) override;
	virtual void								UpdateDoubleBuffer() override;

	// MainMemory-specific:
//	CParticleStream_MainMemory					&ParticleStream() { return m_ParticleStream; }
//	const CParticleStream_MainMemory			&ParticleStream() const { return m_ParticleStream; }
//	CParticleStream_MainMemory					&ParticleStreamForRendering();
//	const CParticleStream_MainMemory			&ParticleStreamForRendering() const;
	void										ManualSwapParticleStreamForRendering(CParticleStream_MainMemory &stream);

	hh_u32										PageSize() const { return m_ParticleStream.PageSize(); }
	bool										SetPageSize(hh_u32 pageSize);	// returns true if the size was changed, false if already at that size, or can't resize to that size.

	void										GarbageCollectOldPages(bool defrag);

	void										_RebuildBounds();

	void										_SwapNewParticleBuffers();
	void										_FlipDoubleBufferForRendering();

	bool										NewParticleBufferForAdd_Empty() const;
	bool										NewParticleBufferForUpdate_Empty() const;

	bool										MergeUpdatedParticles(CParticleStream_MainMemory *stream, bool localized);

	CParticleStorageManager_MainMemory			*Manager() const { return checked_cast<CParticleStorageManager_MainMemory*>(m_Manager); }

	// temp public during scaffolding
	CParticleStream_MainMemory					*_NewParticleBufferForAdd() { return m_NewParticles + m_NewParticlesForAdd; }
	CParticleStream_MainMemory					*_NewParticleBufferForUpdate() { return m_NewParticles + 1 - m_NewParticlesForAdd; }
	const CParticleStream_MainMemory			*_NewParticleBufferForAdd() const { return m_NewParticles + m_NewParticlesForAdd; }
	const CParticleStream_MainMemory			*_NewParticleBufferForUpdate() const { return m_NewParticles + 1 - m_NewParticlesForAdd; }
};

//----------------------------------------------------------------------------

class	CParticleStorageManager_MainMemory : public CParticleStorageManager
{
private:
	bool										_Setup();
	CParticleStorageManager_MainMemory(const CStringId &className);
public:
	virtual ~CParticleStorageManager_MainMemory();

	static CParticleStorageManager_MainMemory	*New();

	virtual void								Clear() override;
	virtual void								Clean() override;
	virtual void								Update(float dt) override;
	virtual CParticleStorage_MainMemory			*NewStorage() override;	// covariant return type

	static CParticlePageAllocator				*GlobalPageAllocator();
	CParticlePageAllocator						*PageAllocator() const { return GlobalPageAllocator(); }

	static CStringId							DefaultStorageClass();
};


//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_STORAGE_RAM_H__

