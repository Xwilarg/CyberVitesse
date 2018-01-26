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

#ifndef	__PS_STORAGE_H__
#define	__PS_STORAGE_H__

#include "hh_particles/include/ps_descriptor.h"
#include "hh_particles/include/ps_stream.h"
#include "hh_particles/include/ps_bounds.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticleMedium;
class	CParticleSpatialMedium;
class	CParticleMediumCollection;
class	CParticleStorageManager;

//----------------------------------------------------------------------------
//
//	ParticleStorage. at the moment, main memory storage.
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleStorage : public CNonCopyable
{
protected:
	CParticleStorageManager						*m_Manager;
	CSmartCachedBounds							m_CachedBounds;
	bool										m_HasBounds;

	bool										m_HasDoubleBufferForRendering;
	bool										m_HasDoubleBufferManualUpdate;

	void										_EnableBoundsImpl(bool enabled);

public:
	CParticleStorage(CParticleStorageManager *manager);
	virtual ~CParticleStorage() {}

	virtual bool								BindStreamDef(const SParticleStreamDef *streamDef, CParticleDescriptor::EStorageSizeHint storageSizeHint) = 0;
	virtual const SParticleStreamDef			*GetStreamDef() const = 0;
	virtual bool								Update(float dt) = 0;	// returns false if empty, true if not-empty
	virtual void								Clear() = 0;
	virtual void								Clean() = 0;
	virtual hh_u32								ActiveParticleCount() const = 0;
	virtual bool								Empty() const = 0;		// watchout, might be expensive depending on the implementation
	virtual bool								EmptyForRendering() const = 0;	// do not check pending particles (eg: new borns)

	virtual CParticleStreamAbstract				&ParticleStream() = 0;
	virtual const CParticleStreamAbstract		&ParticleStream() const = 0;
	virtual CParticleStreamAbstract				&ParticleStreamForRendering() = 0;
	virtual const CParticleStreamAbstract		&ParticleStreamForRendering() const = 0;

	virtual CParticlePageView					LockAllocator_AndAllocateParticlesIFP(hh_u32 wishedCount, hh_u32 minCountHint = 0) = 0;	// on failure, returns an empty page. the function may allocate less than 'wishedCount' contiguous particles, and may need to be called multiple times
	virtual void								UnlockAllocator() = 0;

	bool										HasBounds() const { return m_HasBounds; }
	virtual void								EnableBounds(bool enabled) { _EnableBoundsImpl(enabled); }
	CSmartCachedBounds							&SmartCachedBounds() { return m_CachedBounds; }
	const CAABB									&ExactBounds() const { return m_CachedBounds.ExactBounds(); }
	const CAABB									&CachedBounds() const { return m_CachedBounds.CachedBounds(); }
	CSmartCachedBounds							&Bounds() { return m_CachedBounds; }

	bool										HasDoubleBufferForRendering() const { return m_HasDoubleBufferForRendering; }
	bool										HasDoubleBufferManualUpdate() const { return m_HasDoubleBufferManualUpdate; }
	virtual void								SetHasDoubleBufferForRendering(bool enabled) = 0;
	virtual void								SetHasDoubleBufferManualUpdate(bool enabled) = 0;

	virtual void								UpdateDoubleBuffer() = 0;

	CParticleStorageManager						*Manager() const { return m_Manager; }
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleStorageManager : public CNonCopyable
{
private:
	CStringId					m_StorageClass;

public:
	CParticleStorageManager(const CStringId &storageClass);
	virtual ~CParticleStorageManager();

	const CStringId				&StorageClass() const { return m_StorageClass; }
	virtual void				Clear() = 0;
	virtual void				Clean() = 0;

	virtual void				Update(float dt) = 0;
	virtual CParticleStorage	*NewStorage() = 0;
};

//----------------------------------------------------------------------------
//
//	Spatial storage manager
//
//----------------------------------------------------------------------------

class	CParticleSpatialStorageManager;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleSpatialStorage : public CNonCopyable
{
protected:
	CParticleSpatialStorageManager		*m_Manager;

public:
	CParticleSpatialStorage(CParticleSpatialStorageManager *manager);
	virtual ~CParticleSpatialStorage() {}

	virtual void						Clear() = 0;
	virtual void						Clean() = 0;
	virtual void						Update(float dt) = 0;

	CParticleSpatialStorageManager		*Manager() const { return m_Manager; }
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleSpatialStorageManager : public CNonCopyable
{
private:
	CStringId						m_SpatialStorageClass;

public:
	CParticleSpatialStorageManager(const CStringId &spatialStorageClass);
	virtual ~CParticleSpatialStorageManager();

	const CStringId					&SpatialStorageClass() const { return m_SpatialStorageClass; }

	virtual void					SetOwner(CParticleMediumCollection *owner) = 0;
	virtual void					Clear() = 0;
	virtual void					Clean() = 0;

	virtual void					Update(float dt) = 0;
	virtual CParticleSpatialStorage	*NewSpatialStorage(CParticleSpatialMedium *owner, const SParticleDeclaration &decl) = 0; // covariant
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_STORAGE_H__

