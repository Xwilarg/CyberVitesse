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

#ifndef	__PS_UPDATER_CPU_H__
#define	__PS_UPDATER_CPU_H__

#include "hh_particles/include/ps_updater.h"
#include "hh_particles/include/ps_object_remapper.h"
#include "hh_particles/include/Kernels/CPU/kernel_cpu.h"
#include "hh_particles/include/Samplers/CPU/sampler_cpu.h"
#include "hh_particles/include/Storage/MainMemory/storage_ram.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleEvaluator_CPU);
HH_FORWARD_DECLARE(AsynchronousJob);

//----------------------------------------------------------------------------

namespace	Threads
{
	struct	SThreadContext;
}

class	CParticleUpdateManager_CPU;
class	CParticleEvaluationContext;
class	CParticleContextInstance;
class	CParticleContextScene;
//class	CParticleEvaluationContext_CPU;
struct	SParticleEvaluationCache;
struct	SParticleEvaluationBuffer;
struct	SParticleSpawnerFields;
class	IParticleScene;

//----------------------------------------------------------------------------

class	CAbstractRandomGenerator;
#if	(PK_HAS_DETERMINISM != 0)
	typedef	CAbstractRandomGenerator	CAbstractGenerator;
#else
	typedef	CRandomGenerator			CAbstractGenerator;
#endif

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleRandomContext
{
private:
	CAbstractRandomGenerator	*m_RandomSampler;
	CAbstractRandomGenerator	*m_RandomSamplerFast;

public:
	CParticleRandomContext() : m_RandomSampler(null), m_RandomSamplerFast(null) {}

	void						SetRandomSamplers(CAbstractRandomGenerator *prng, CAbstractRandomGenerator *prngFast) { HH_ASSERT(prng != null && prngFast != null); m_RandomSampler = prng; m_RandomSamplerFast = prngFast; }

	CAbstractRandomGenerator	&RandomSampler() const { HH_ASSERT(m_RandomSampler != null); return *m_RandomSampler; }
	CAbstractRandomGenerator	&RandomSamplerFast() const { HH_ASSERT(m_RandomSamplerFast != null); return *m_RandomSamplerFast; }
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleContextInstance
{
private:
	CParticleEvaluationContext	*m_ParentEvaluationContext;
	CParticleContextScene		*m_ParticleContextScene;
	CParticleMedium				*m_Medium;
	CParticlePageView			*m_Page;

public:
	CParticleContextInstance() : m_ParentEvaluationContext(null), m_ParticleContextScene(null), m_Medium(null), m_Page(null) {}

	void	Setup(	CParticleMedium				*medium,
					CParticlePageView			*page,
					CParticleContextScene		*particleContextScene,
					CParticleEvaluationContext	*parentCtx);

	const CParticleContextScene	*ParticleContextScene() const { return m_ParticleContextScene; }
	CParticleMedium				*Medium() const { return m_Medium; }
	CParticlePageView			*Page() const { return m_Page; }

	// FIXME: TEMP SCAFFOLDING : Abstraction leak. This defeats the purpose of specializing the different contexts,
	// as it allows the caller to gain access to all other contexts through the parent eval context.
	CParticleEvaluationContext	*ParentEvaluationContext() const { return m_ParentEvaluationContext; }
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleContextScene
{
private:
	CParticleEvaluationContext	*m_ParentEvaluationContext;
	CParticleMediumCollection	*m_MediumCollection;
	const SAttributesContainer	*m_AttributesContainer;

public:
	CParticleContextScene() : m_ParentEvaluationContext(null), m_MediumCollection(null), m_AttributesContainer(null) {}

	void	Setup(	CParticleMediumCollection	*mediumCollection,
					const SAttributesContainer	*attribContainer,
					CParticleEvaluationContext	*parentCtx);

	CParticleMediumCollection	*MediumCollection() const { return m_MediumCollection; }
	IParticleScene				*Scene() const;

	// TEMP (hack): This is only used to access overridden attribute samplers from spawn scripts
	const SAttributesContainer	*AttributesContainer() const { return m_AttributesContainer; }

	// FIXME: TEMP SCAFFOLDING : Abstraction leak. This defeats the purpose of specializing the different contexts,
	// as it allows the caller to gain access to all other contexts through the parent eval context.
	CParticleEvaluationContext	*ParentEvaluationContext() const { return m_ParentEvaluationContext; }
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleEvaluationContext : public CNonCopyable
{
private:
	struct	SParticleEvaluationPoolElement
	{
		hh_u32	m_RefCounts;
		hh_u32	m_RawByteCount;
		void	*m_RawMem;
		~SParticleEvaluationPoolElement()
		{
			HH_ASSERT(m_RefCounts == 0);
			HH_ASSERT(m_RawMem != null);
			HH_FREE(m_RawMem);
		}
	};

	struct	SNamedCache
	{
		bool		m_Used;
		CStringId	m_Name;
		hh_u32		m_Capacity;
		hh_u8		*m_RawMem;

		SNamedCache() : m_Used(false), m_Capacity(0), m_RawMem(null) {}
		~SNamedCache()
		{
			HH_FREE(m_RawMem);
		}
	};

	mutable TArray<SParticleEvaluationPoolElement>	m_EvalBufferPool;
	mutable TArray<SNamedCache>						m_NamedCaches;

	CParticleContextInstance						m_ParticleContextInstance;
	CParticleContextScene							m_ParticleContextScene;

	CGuid						_AllocRawCache(hh_u32 byteCount) const;

public:
	enum	EEvalRequestLocation
	{
		EvalOriginatedIn_Setup = 0,	// restricts direct access to particle streams for reading
		EvalOriginatedIn_Evolve,
		EvalOriginatedIn_Evolve_Newborns,
	};

	Threads::SThreadContext		*m_ThreadCtx;
	CParticleMedium				*m_Medium;
	CParticlePageView			*m_Page;
//	const SAttributesContainer	*m_AttributesContainer;
	SParticleSpawnerFields		*m_SpawnerFields;
	float						m_ElapsedTime;
	EEvalRequestLocation		m_Location;
	const CBaseObject			*m_RunningHboScope;

	CParticleEvaluationContext();
	~CParticleEvaluationContext();

	void	Setup(	Threads::SThreadContext		*threadCtx,
					CParticleMedium				*medium,
					const SAttributesContainer	*attribContainer,
					CParticlePageView			*page,
					EEvalRequestLocation		location,
					float						actualCurrentDate);

	bool				Valid() const { return m_Medium != null; }
	bool				Empty() const { return m_Page == null; }

	const CParticleContextInstance	*ParticleContextInstance() const { return &m_ParticleContextInstance; }
	const CParticleContextScene		*ParticleContextScene() const { return &m_ParticleContextScene; }

//	SParticleEvaluationBufferCached	*AllocLocalCache();

	template<typename _Type>
	bool				AllocLocalCache(SParticleEvaluationCache &targetCache, hh_u32 count) const;

	template<typename _Type>
	bool				AllocLocalCache(SParticleEvaluationCache &targetCache) const
	{
		return AllocLocalCache<_Type>(targetCache, CacheElementCount());
	}

	void				AddCacheRef(hh_u32 slot) const
	{
		m_EvalBufferPool[slot].m_RefCounts++;
	}
	void				DelCacheRef(hh_u32 slot) const
	{
		HH_ASSERT(m_EvalBufferPool[slot].m_RefCounts > 0);
		m_EvalBufferPool[slot].m_RefCounts--;
	}

	hh_u32				CacheElementCount() const;
	bool				AllocNamedCache(CStringId name, hh_u32 byteCount, SParticleEvaluationBuffer &targetCache) const;
	bool				MapNamedCache(CStringId name, hh_u32 byteCount, SParticleEvaluationBuffer &targetCache) const;
	void				ReleaseAllNamedCaches() const;
};

//----------------------------------------------------------------------------
//
//	Evaluation caches/buffers
//
//----------------------------------------------------------------------------

struct	SParticleEvaluationBuffer
{
public:
	SStridedMemoryViewRawStorage	m_Storage;
	EBaseTypeID						m_TypeId;

	SParticleEvaluationBuffer(EBaseTypeID typeId, void *rawPtr, hh_u32 count, hh_u32 stride)
	:	m_TypeId(typeId)
	{
		m_Storage.m_RawDataPtr = (hh_u8*)rawPtr;
		m_Storage.m_Count = count;
		m_Storage.m_Stride = stride;
	}

	template<typename _Type>
	TStridedMemoryView<_Type>	StreamView() const
	{
		if (TBaseTypeID<_Type>::TypeID == m_TypeId)
			return *reinterpret_cast<const TStridedMemoryView<_Type>*>(&m_Storage);
		return TStridedMemoryView<_Type>();
	}

	SParticleEvaluationBuffer	Slice(hh_u32 start, hh_u32 count) const
	{
		SParticleEvaluationBuffer	out = *this;
		HH_ASSERT(start + count <= m_Storage.m_Count);
		out.m_Storage.m_RawDataPtr = Mem::AdvanceRawPointer(out.m_Storage.m_RawDataPtr, start * out.m_Storage.m_Stride);
		out.m_Storage.m_Count = count;
		return out;
	}

	void	Clear()
	{
		m_Storage.m_Count = 0;
		m_Storage.m_RawDataPtr = 0;
		m_Storage.m_Stride = 0;
		m_TypeId = BaseType_Void;
	}
};

//----------------------------------------------------------------------------

struct	SParticleEvaluationCache
{
	const CParticleEvaluationContext	*m_Context;
	SParticleEvaluationBuffer			m_Buffer;
	hh_u32								m_Slot;

	SParticleEvaluationCache()
	:	m_Context(null)
	,	m_Buffer(CBaseType<void>::TypeID, null, 0, 0)
	,	m_Slot(0)
	{
	}
	~SParticleEvaluationCache()
	{
		ReleaseIFN();
	}

	bool	Valid() const { return m_Context != null; }

	template<typename _Type>
	void	Setup(const CParticleEvaluationContext *context, hh_u32 slotId, const TStridedMemoryView<_Type> &mview)
	{
//		ReleaseIFN();
		m_Buffer = SParticleEvaluationBuffer(TBaseTypeID<_Type>::TypeID, mview.Data(), mview.Count(), mview.Stride());
		m_Slot = slotId;
		m_Context = context;
		HH_ASSERT(m_Context != null);
		m_Context->AddCacheRef(m_Slot);
	}
	void	ReleaseIFN();
};

//----------------------------------------------------------------------------
//
//	Implementation
//
//----------------------------------------------------------------------------

template<typename _Type>
bool	CParticleEvaluationContext::AllocLocalCache(SParticleEvaluationCache &targetCache, hh_u32 elementsCount) const
{
	HH_ASSERT(m_Page != null && !targetCache.Valid());	// the target cache must not be already allocated
//	targetCache.ReleaseIFN();
	CGuid	index = _AllocRawCache(elementsCount * sizeof(_Type));
	if (!index.Valid())
		return false;

	// FIXME: handle failure... SParticleEvaluationCache should handle invalid stuff
	TStridedMemoryView<_Type>	buffer((_Type*)m_EvalBufferPool[index].m_RawMem, elementsCount, sizeof(_Type));
	targetCache.Setup(this, index, buffer);
	return true;
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

struct	SParticleImplementationCPU
{
	TArray<PParticleSamplerCPU>			m_RuntimeSamplers;
};

//----------------------------------------------------------------------------

class	CParticleUpdater_CPU : public CParticleUpdater
{
protected:
public:	// temp scaffolding
	typedef	bool						(*FnFieldEvaluator)(CParticleEvaluationContext &context, CParticlePageView &page, CGuid fieldId, const SFieldInitializer *self);
	CParticleMediumCollection			*m_Owner;	// temp scaffolding
	CParticleMedium						*m_Owner2;	// temp scaffolding

//protected:	// TEMP public
	PAsynchronousJob					m_FinalUpdateJob;
	Threads::CCriticalSection			m_FinalUpdateJobLock;

protected:
	const SParticleDeclaration			*m_ParticleDeclaration;
	PCParticleState						m_SourceState;
	hh_u32								m_ActualCurrentFrame;

	PAsynchronousJob					_BuildParticleStreamUpdateTask(	CParticleStorage_MainMemory	*storage,
																		CParticleStream_MainMemory	&stream,
																		float						dt,
																		PAsynchronousJob			prevSerialTask,
																		hh_u32						actualCurrentFrame,
																		float						actualCurrentDate);

	void								_TrashDeadParticles(CParticleStream_MainMemory &stream, CParticlePage &page, hh_u32 firstDeadParticleIndex) const;

	TArray<FnFieldEvaluator>			m_FieldInitializers;

	bool								_MapRuntimeKernels(const SParticleDeclaration &decl, const CParticleState *hboState);
	bool								_MapRuntimeSamplers(const SParticleDeclaration &decl);
	bool								_RebuildFieldsSetup(const SParticleDeclaration &decl);
	bool								_SetupAfterImplementation();

public:
	// runtime state
	TArray<PParticleKernelCPU_Evolver>	m_RuntimeKernels;
	SParticleImplementationCPU			m_RuntimeImplementation;

	bool								m_NeedsEvolveOnDeath;
	bool								m_NeedsSpawnerIDs;

	PCParticleEvaluator_CPU				m_RuntimeSpawnEvaluator;

	void								_SerialEvolveFromThread(Threads::SThreadContext								&threadContext,
																CParticleStorage_MainMemory							*storage,
																CParticlePageView									&page,
																const TStridedMemoryView<const float>				&dtList,
																CParticleEvaluationContext::EEvalRequestLocation	evalLocation,
																float												actualCurrentDate) const;

	void								_TrashAndSubEvolveDeadParticlesIFN(	Threads::SThreadContext								&threadContext,
																			CParticleStorage_MainMemory							*storage,
																			CParticlePage										&page,
																			float												dt,
																			hh_u32												firstDeadParticleIndex,
																			CParticleEvaluationContext::EEvalRequestLocation	evalLocation,
																			float												actualCurrentDate) const;

	void								_TrashAndSubEvolveDeadParticlesIFN(	Threads::SThreadContext								&threadContext,
																			CParticleStorage_MainMemory							*storage,
																			CParticlePage										&page,
																			const TStridedMemoryView<const float>				&dts,
																			hh_u32												firstDeadParticleIndex,
																			CParticleEvaluationContext::EEvalRequestLocation	evalLocation,
																			float												actualCurrentDate) const;

	void								_Evolve(CParticleEvaluationContext &context, CParticleRandomContext &randContext, const TStridedMemoryView<const float> &dt, hh_u32 pid, hh_u32 fid) const;

	void								_UpdateFence();

public:
	CParticleUpdater_CPU(CParticleUpdateManager_CPU *manager);
	~CParticleUpdater_CPU();

	bool								_Bind(CParticleMedium *owner, const CParticleState *state, const SParticleDeclaration &decl, bool evolveOnDeath);

	virtual void						Clear() override;
	virtual void						Clean() override;

	virtual void						PreUpdate(float dt, CParticleStorage *storage) override;
	virtual void						UpdateLaunchAsync(float dt, CParticleStorage *storage) override;

	virtual void						SetupStream(CParticleEvaluationContext &context, CParticleRandomContext &randContext) const override;

	const SParticleImplementationCPU	&RuntimeImplementation() const { return m_RuntimeImplementation; }
	const PAsynchronousJob				&GetFinalUpdateJob() const { return m_FinalUpdateJob; }

	CParticleUpdateManager_CPU			*Manager() const { return checked_cast<CParticleUpdateManager_CPU*>(m_Manager); }
};

//----------------------------------------------------------------------------

class	CParticleUpdateManager_CPU : public CParticleUpdateManager
{
private:
	typedef	TObjectMapping<TRemappableObject<CParticleEvolver>, CbEvolverRemapperCPU>	TEvolverMapping;
	typedef	TObjectMapping<TRemappableObject<CParticleSampler>, CbSamplerRemapperCPU>	TSamplerMapping;
private:
	TEvolverMapping								*m_EvolverMapping;
	TSamplerMapping								*m_SamplerMapping;
	CParticleStorageManager_MainMemory			*m_StorageManager;

public:	// Scaffolding
	Threads::CEvent								m_TasksCompletionEvent;
protected:
	PAsynchronousJob							m_FinalUpdateJob;
	Threads::CCriticalSection					m_FinalUpdateJobLock;

	bool										_Setup();

protected:
	CParticleUpdateManager_CPU(const CStringId &className);	// only way to create a manager is to use the 'New()' static method

public:
	~CParticleUpdateManager_CPU();

	static bool									RegisterForLink();	// Scripts

	virtual void								SetOwner(CParticleMediumCollection *owner) override;
	virtual void								Clear() override;
	virtual void								Clean() override;

	virtual void								Update(float dt) override;
	virtual void								PostUpdate() override;
	virtual void								UpdateFence() override;
	virtual bool								UpdatePending() override;
	virtual CParticleUpdater_CPU				*NewUpdater(CParticleMedium *owner, CParticleState *state, const SParticleDeclaration &decl, bool evolveOnDeath) override;	// covariant
	virtual CParticleStorageManager_MainMemory	*StorageManager() const override { return m_StorageManager; } // covariant

	static CParticleUpdateManager_CPU			*New();
	static CStringId							DefaultUpdateClass();

	const TEvolverMapping						*EvolverMapping() const { return m_EvolverMapping; }
	TEvolverMapping								*EvolverMapping() { return m_EvolverMapping; }	// FIXME: ideally, don't expose this

	const TSamplerMapping						*SamplerMapping() const { return m_SamplerMapping; }
	TSamplerMapping								*SamplerMapping() { return m_SamplerMapping; }	// FIXME: ideally, don't expose this

public: // UpdaterCPU tasks access
	void										AddPostUpdateDependency(CAsynchronousJob *job);
	HH_FORCEINLINE void							AddPostUpdateDependency(const PAsynchronousJob &job) { AddPostUpdateDependency(job.Get()); }
	void										AddFinalUpdateDependency(CAsynchronousJob *job);
	HH_FORCEINLINE void							AddFinalUpdateDependency(const PAsynchronousJob &job) { AddFinalUpdateDependency(job.Get()); }
	const PAsynchronousJob						&GetFinalSyncTask() const { return m_FinalUpdateJob; }

	float										m_LastDt;
	CParticleMediumCollection					*m_Owner;
private:
	PAsynchronousJob							m_ManagerPostUpdate;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_UPDATER_CPU_H__

