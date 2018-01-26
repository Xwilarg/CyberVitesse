#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2004/03/25 10:47 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_MEDIUMS_H__
#define	__PS_MEDIUMS_H__

#include "hh_particles/include/ps_config.h"
#include "hh_particles/include/ps_stream.h"
#include "hh_particles/include/ps_bounds.h"

#include <hh_engine_utils/include/kr_actions.h>
#include <hh_kernel/include/kr_callbacks.h>
#include <hh_kernel/include/kr_containers_array_chunked.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleDescriptor);
HH_FORWARD_DECLARE(ParticleSpatialDescriptor);
HH_FORWARD_DECLARE(ParticleRuntimeState);
HH_FORWARD_DECLARE(ParticleRenderer);
HH_FORWARD_DECLARE(ParticleDrawer);
HH_FORWARD_DECLARE(AsynchronousJob);
HH_FORWARD_DECLARE(ActionInstanceParticleSpawnerBase);
HH_FORWARD_DECLARE(CompilerIR);

class	CActionFactoryParticleSpawnerBase;
class	CRendererSubView;
class	CParticlesStats;
class	CMediumStats;
struct	SDebugParticleInfos;
struct	SRenderMediumSortElement;
class	IParticleScene;					// include <ps_scene.h>
class	CParticleUpdater;				// include <ps_updater.h>
class	CParticleUpdateManager;			// include <ps_updater.h>
class	CParticleStorage;				// include <ps_storage.h>
class	CParticleStorageManager;		// include <ps_storage.h>
class	CParticleSpatialStorage;		// include <ps_storage.h>
class	CParticleSpatialStorageManager;	// include <ps_storage.h>
class	CParticleRenderManager;			// include <ps_renderer.h>

#if	(HH_PARTICLES_BUILD_DEBUG_HISTORY != 0)
class	CParticleDebugHistory;			// include <ps_debug.h>
#endif

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleMedium);
HH_FORWARD_DECLARE(ParticleSpatialMedium);
HH_FORWARD_DECLARE(ParticleRenderMedium);
HH_FORWARD_DECLARE(ParticleEvaluator_CPU);

struct	SParticleImplementationCPU;
class	CParticleMediumCollection;

//----------------------------------------------------------------------------
//
//	Particle medium
//	main simulation medium
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleMedium : public CRefCountedObject
{
private:
	CParticleMediumCollection					*m_Owner;
	CParticleStorage							*m_ParticleStorage;
	CParticleUpdater							*m_ParticleUpdater;

	TArray<WParticleRenderMedium>				m_BoundRenderMediums;

	// FIXME: not ultra-cool to have this here... will force reloads of the cache line containing this member on all threads when changing the counter
	// (ie: all the time when under heavy spawning load)
	mutable TAtomic<hh_u32>						m_ParticleIdsCounter;

	hh_u32										m_UniqueKeyForParentID;

#if	(HH_PARTICLES_HAS_STATS != 0)
	CMediumStats								*m_Stats;
#endif
	TAtomic<bool>								m_Empty;

	WCParticleDescriptor						m_Descriptor;
	CString										m_StateName;

	float										m_MaxParticleRenderingRadiusScale;
	float										m_MaxParticleRenderingRadiusConstant;

	CFloat3										m_TEMP_HACK_LastCamPos;

#if	(HH_PARTICLES_BUILD_DEBUG_HISTORY != 0)
	CParticleDebugHistory						*m_DebugHistory;
	CGuid										_HistoryFrameToShow() const;
#endif

	void										_OnDescriptorChanged(const CParticleDescriptor *descriptor);
	void										_OnDescriptorDestroyed(const CParticleDescriptor *descriptor);

public:
	CParticleMedium(CParticleMediumCollection *owner);
	~CParticleMedium();

	void										Clear();
	bool										Bind(const CParticleDescriptor *descriptor, hh_u32 stateID);

	bool										Empty() const;
	void										NotifyNotEmpty();
	bool										EmptyForRendering() const;

	void										RegisterRenderMedium(CParticleRenderMedium *rm) const;	// const: temp workaround
	void										UnregisterRenderMedium(CParticleRenderMedium *rm) const;

	mutable TCallbackChainUnordered<void(const CParticleMedium*)>	m_OnMediumRemoving;
	mutable TCallbackChainUnordered<void(const CParticleMedium*)>	m_OnMediumDestroyed;		// when called, the medium will be undergoing destruction, and in a potentially unsafe access state.

	void										PreUpdate(float dt);
	void										Update(float dt);

	bool										HasBounds() const;
	void										EnableBounds(bool enabled);
	const CAABB									&ExactBounds() const;
	const CAABB									&CachedBounds() const;

	bool										HasLocalizedPages() const;

	const CParticleDescriptor					*Descriptor() const;
	const CString								&StateName() const { return m_StateName; }

	CParticleStorage							*ParticleStorage() const;
	CParticleUpdater							*ParticleUpdater() const;

	CParticleMediumCollection					*Owner() const { return m_Owner; }
#if	(HH_PARTICLES_HAS_STATS != 0)
	CMediumStats								*Stats() const { return m_Stats; }	// can be null
#endif

	void										GetDebugParticleInfos(SDebugParticleInfos &infos) const;
	hh_u32										GetRenderMediumCount() const;
#if	(HH_PARTICLES_BUILD_DEBUG_HISTORY != 0)
	CParticleDebugHistory						*GetDebugHistory() const;
	CParticleDebugHistory						*GetDebugHistoryAndCreateIFN();
	void										ClearDebugHistory();
#endif

	void										FillParticleDrawersArray(TArray<TPair<CParticleDrawer *, const CParticleRenderer *> > &outDrawers) const;

	void										UpdateParticleRenderingMetricsIFN(float maxRadiusScale, float maxRadiusConstant);
	CFloat2										ParticleRenderingMetricsModifier() const { return CFloat2(m_MaxParticleRenderingRadiusScale, m_MaxParticleRenderingRadiusConstant); }

	bool										ParticlesNeedEvolveOnDeath() const;
//	hh_u32										RegisterSpawnerID(CActionInstanceParticleSpawnerBase *spawner);

	const CFloat3								&__TEMP_HACK_LastCamPos() const { return m_TEMP_HACK_LastCamPos; }				// FIXME ! TEMP HACK
	void										__TEMP_HACK_SetLastCamPos(const CFloat3 &pos) { m_TEMP_HACK_LastCamPos = pos; }	// FIXME ! TEMP HACK

	const TArray<WParticleRenderMedium>			&BoundRenderMediums() const { return m_BoundRenderMediums; }

	// ParentID bit field: { medium index: 11; particle id: 21; }
	enum
	{
		kParentIDBits_MediumId = 11,	// 2,048 mediums
		kParentIDBits_ParticleId = 21,	// 2,097,152 particles
	};
	enum
	{
		kParentIDBits_MediumId_Mask = ((1 << kParentIDBits_MediumId) - 1),
		kParentIDBits_ParticleId_Mask = ((1 << kParentIDBits_ParticleId) - 1),
	};

	void										GetNextParticleSelfIDs(const TStridedMemoryView<hh_u32> &selfIDs) const;

	HH_FORCEINLINE hh_u32						BuildParticleParentID(hh_u32 selfId) const { return (m_UniqueKeyForParentID << kParentIDBits_ParticleId) | (selfId & kParentIDBits_ParticleId_Mask); }
	void										SetUniqueKeyForParentID(hh_u32 id) { m_UniqueKeyForParentID = id; }
};
HH_DECLARE_REFPTRCLASS(ParticleMedium)

//----------------------------------------------------------------------------
//
//	Particle spatial medium
//	spatial-query medium
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleSpatialMedium : public CRefCountedObject
{
private:
	CParticleMediumCollection					*m_Owner;
	CParticleSpatialStorage						*m_Storage;

	WCParticleSpatialDescriptor					m_Descriptor;

	void										_OnDescriptorChanged(const CParticleSpatialDescriptor *descriptor);
	void										_OnDescriptorDestroyed(const CParticleSpatialDescriptor *descriptor);

public:
	CParticleSpatialMedium(CParticleMediumCollection *owner, hh_u32 indexInMediumCollection);
	~CParticleSpatialMedium();

	void										Clear();
	bool										Bind(const CParticleSpatialDescriptor *descriptor);

	mutable TCallbackChainUnordered<void(const CParticleSpatialMedium*)>	m_OnMediumRemoving;
	mutable TCallbackChainUnordered<void(const CParticleSpatialMedium*)>	m_OnMediumDestroyed;	// when called, the medium will be undergoing destruction, and in a potentially unsafe access state.

	void										PreUpdate(float dt);
	void										Update(float dt);

	const CParticleSpatialDescriptor			*Descriptor() const;
	CParticleMediumCollection					*Owner() const { return m_Owner; }

	CParticleSpatialStorage						*SpatialStorage() const { return m_Storage; }
};
HH_DECLARE_REFPTRCLASS(ParticleSpatialMedium)

//----------------------------------------------------------------------------
//
//	render-medium
//	can batch together multiple particle mediums to render them in one large batch
//	if their renderers are compatible.
//
//----------------------------------------------------------------------------

struct	SParticleRenderMediumAssoc
{
	WCParticleMedium				m_MediumToRender;
	WCParticleRenderer				m_Renderer;
	SParticleRenderMediumAssoc(const CParticleRenderer *renderer, const CParticleMedium *mediumToRender);
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleRenderMedium : public CRefCountedObject
{
public:
	typedef SParticleRenderMediumAssoc		SMediumRendererAssoc;

private:
	CParticleMediumCollection			*m_Owner;
	PParticleDrawer						m_ParticleDrawer;
	TArray<SMediumRendererAssoc>		m_MediumsToRender;
	bool								m_Empty;

	CGuid								_FindRendererAndMediumPair(const CParticleRenderer *renderer, const CParticleMedium *medium);
	CGuid								_FindRenderer(const CParticleRenderer *renderer);
	CGuid								_FindMedium(const CParticleMedium *medium);

protected:
	void								_OnMediumDestroyed(const CParticleMedium *medium);
	void								_OnRendererModified(const CParticleRenderer *renderer);
	void								_OnRendererDestroyed(const CParticleRenderer *renderer);

public:
	CParticleRenderMedium(CParticleMediumCollection *owner);
	~CParticleRenderMedium();

	void								Clear();
	bool								Bind(const CParticleRenderer *renderer, const CParticleMedium *mediumToRender);
	bool								Unbound();
	void								UnbindAll();

	mutable TCallbackChainUnordered<void(const CParticleRenderMedium*)>	m_OnMediumDestroyed;

	bool								CompatibleWith(const CParticleRenderer *renderer) const;
	void								NotifyMediumModification(const CParticleMedium *medium);

	hh_i32								DrawOrder() const;
	CParticleMediumCollection			*Owner() const { return m_Owner; }
	const TArray<SMediumRendererAssoc>	&MediumsAndRenderers() const { return m_MediumsToRender; }
	bool								NeedsRender() const { return !m_MediumsToRender.Empty(); }
	CParticleDrawer						*ParticleDrawer() const;
	bool								HasRefToMedium(const CParticleMedium *medium) const;

	void								FillDrawerArrayForMedium(const CParticleMedium *medium, TArray<TPair<CParticleDrawer *, const CParticleRenderer *> > &outDrawers) const;
	void								HarvestParticleRadiusVisibilityCoefficients(float &outMaxScale, float &outMaxConstant) const;

	void								PreUpdate(float dt);
	void								Update(float dt);
	void								PreRender(CRendererSubView &view) const;
	void								StartRender(CRendererSubView &view) const;
	void								KickRender(CRendererSubView &view) const;

	float								ComputeSortKey(CRendererSubView &view, const CFloat3 &viewPosition) const;

	bool								Empty() const;
	void								NotifyNotEmpty();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderMedium);

//----------------------------------------------------------------------------
//
//	main particle manager.
//	holds all particle datastructures for a whole scene.
//	only one instance needed at all times for typical game-use
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleMediumCollection
{
public:

	struct	SSpawnerRef
	{
		PActionInstanceParticleSpawnerBase		m_Spawner;
		CParticleMedium const					*m_Medium;
		SSpawnerRef(CActionInstanceParticleSpawnerBase *spawner, const CParticleMedium *medium);
		~SSpawnerRef();
		HH_FORCEINLINE bool						operator == (CActionInstanceParticleSpawnerBase *spawner) const { return m_Spawner == spawner; }
		HH_FORCEINLINE bool						Valid() const { return m_Spawner != null; }
		static SSpawnerRef						Invalid; // needed by TChunkedSlotArray
	};

	struct	SLayerImplem
	{
		const CActionFactoryParticleSpawnerBase	*m_ParentFactory;
		PParticleEvaluator_CPU					m_Evaluator;
		PCCompilerIR							m_IR;
//		const SParticleImplementationCPU		*m_Implementation;
		SParticleImplementationCPU				*m_Implementation;
//		SParticleImplementationCPU				m_Implementation;
		SLayerImplem();
		~SLayerImplem();
	};

	struct	SViewData
	{
		CFloat4x4	m_WorldToView;
		CFloat4x4	m_ViewToWorld;	// m_WorldToView.Inverse()
		CFloat4x4	m_WorldToProj;
		CFloat4x4	m_ProjToWorld;	// m_WorldToProj.Inverse()
		CInt2		m_Resolution;

		// Computed from the view matrices
		float		m_FovHorizontal;
		float		m_FovVertical;
//		float		m_Near;
//		float		m_Far;

		SViewData();
		~SViewData();
	};

private:
	TArray<PParticleMedium>						m_ParticleMediums;
	TArray<PParticleSpatialMedium>				m_ParticleSpatialMediums;
	TArray<PParticleRenderMedium>				m_ParticleRenderMediums;

	// UpdateFence() const will clear that
	mutable TArray<PParticleMedium>				m_ActiveParticleMediumsUpdated;

	TArray<PParticleMedium>						m_ActiveParticleMediums;
//	TArray<PParticleSpatialMedium>				m_ActiveParticleSpatialMediums;
	TArray<PParticleRenderMedium>				m_ActiveParticleRenderMediums;
	mutable TArray<SRenderMediumSortElement>	m_RenderMediumsForKick;

	Threads::CCriticalSection					m_ActiveParticleMediumsLock;

	IParticleScene								*m_Scene;
	CActionExecutor								m_Executor;
	double										m_ElapsedTime;
	CParticlesStats								*m_Stats;
	TAtomic<bool>								m_ScanRenderMediumsForDestruction;
	bool										m_StatsTimingsEnabled;
	bool										m_SpinForTasksEnabled;
	bool										m_HasBounds;
	bool										m_HasLocalizedPages;
	bool										m_DefaultPageLocalization;
	bool										m_HasSpawnerIDs;
	bool										m_HasRenderSort;
	bool										m_IsInEditorMode;
	CFloat3										m_TEMP_HACK_LastCamPos;

#if	(HH_PARTICLES_BUILD_DEBUG_HISTORY != 0)
	hh_u32										m_HistorySize;
	hh_u32										m_HistoryCursor;
#endif

	Threads::CRWLock							m_FindOrCreateMediumLock;
	Threads::CRWLock							m_FindOrCreateSpatialMediumLock;
	Threads::CRWLock							m_FindOrCreateRenderMediumLock;

	CParticleUpdateManager						*m_UpdateManager;
	CParticleSpatialStorageManager				*m_SpatialManager;
	CParticleRenderManager						*m_RenderManager;

	bool										m_OwnsUpdateManager;
	bool										m_OwnsSpatialManager;
	bool										m_OwnsRenderManager;

	Threads::CCriticalSection					m_SpawnerListLock;
	TChunkedSlotArray<SSpawnerRef, 512>			m_SpawnerList;

	TSlotArray<CGuid>							m_ViewList;
	TArray<SViewData, TArrayCacheAligned>		m_ViewListData;

protected:
	void										_OnRenderMediumDestroyed(const CParticleRenderMedium *medium);
	void										_CleanupRenderMediumsIFN();
	CGuid										_ThreadSafeSpawnerList_Insert(const CParticleMedium *medium, CActionInstanceParticleSpawnerBase *spawner);
	void										_ThreadSafeSpawnerList_Remove(CGuid slot);

	void										_IncrementSpawnerIDRefsIFN(const TMemoryView<const hh_u32> &ids, bool isParticleRef);	// ignores 'CGuid::INVALID' values
	void										_IncrementSpawnerIDRefIFN(hh_u32 spawnerID, hh_u32 incCount, bool isParticleRef);
	void										_DecrementSpawnerIDRefsIFN(const TMemoryView<const hh_u32> &ids, bool isParticleRef);
	void										_DecrementSpawnerIDRefIFN(hh_u32 spawnerID, hh_u32 decCount, bool isParticleRef);

public:
	CParticleMediumCollection(	CParticleUpdateManager				*updateManager	= null,
								CParticleSpatialStorageManager		*spatialManager = null,
								CParticleRenderManager				*renderManager	= null);
	~CParticleMediumCollection();

	Threads::CRWLock							&MediumLock() { return m_FindOrCreateMediumLock; }
	Threads::CRWLock							&SpatialMediumLock() { return m_FindOrCreateSpatialMediumLock; }
	Threads::CRWLock							&RenderMediumLock() { return m_FindOrCreateRenderMediumLock; }

	const Threads::CRWLock						&MediumLock() const { return m_FindOrCreateMediumLock; }
	const Threads::CRWLock						&SpatialMediumLock() const { return m_FindOrCreateSpatialMediumLock; }
	const Threads::CRWLock						&RenderMediumLock() const { return m_FindOrCreateRenderMediumLock; }

	TCallbackChainUnordered<void(CParticleMedium*)>				m_OnMediumAdded;
	TCallbackChainUnordered<void(const CParticleMedium*)>		m_OnMediumRemoving;
//	TCallbackChainUnordered<void(CParticleSpatialMedium*)>		m_OnSpatialMediumAdded;
//	TCallbackChainUnordered<void(const CParticleSpatialMedium*)>m_OnSpatialMediumRemoving;
	TCallbackChainUnordered<void(CParticleRenderMedium*)>		m_OnRenderMediumAdded;
	TCallbackChainUnordered<void(const CParticleRenderMedium*)>	m_OnRenderMediumRemoving;

	void										Clear();	// keeps the already existing mediums and render mediums, but clears all the particles they contain.
	void										Clean();	// removes everything, the medium collection will be totally empty of any mediums.

	bool										HasLocalizedPages() const { return m_HasLocalizedPages; }
	bool										DefaultPageLocalization() const { return m_DefaultPageLocalization; }
	void										EnableLocalizedPages(bool enabled, bool localizedByDefault);

	bool										HasBounds() const { return m_HasBounds; }
	void										EnableBounds(bool enabled);

	// Removed due to bad design
	// Must now be setuped at CHHParticlesBase::Startup with CHHParticlesBase::Config::m_GloballyEnableSpawnerIDs
	// void										EnableSpawnerIDs(bool enabled);

	bool										HasRenderSort() const { return m_HasRenderSort; }
	void										EnableRenderSort(bool enabled);

	double										ElapsedTime() const { return m_ElapsedTime; }

#if	(HH_PARTICLES_BUILD_DEBUG_HISTORY != 0)
	hh_u32										HistorySize() const { return m_HistorySize; }
	void										SetHistorySize(hh_u32 size);
	hh_u32										HistoryCursor() const { return m_HistoryCursor; }
	void										SetHistoryCursor(hh_u32 cursor);
#endif

	CParticleUpdateManager						*UpdateManager() const { return m_UpdateManager; }
	CParticleSpatialStorageManager				*SpatialManager() const { return m_SpatialManager; }
	CParticleRenderManager						*RenderManager() const { return m_RenderManager; }

	CParticleMedium								*FindOrCreateCompatibleMedium(const CParticleDescriptor *descriptor, hh_u32 stateID);
	CParticleSpatialMedium						*FindOrCreateCompatibleSpatialMedium(const CParticleSpatialDescriptor *descriptor);
	CParticleRenderMedium						*FindOrCreateCompatibleRenderMedium(const CParticleRenderer *renderer, const CParticleMedium *medium);

	// to use these, you must make sure these are called inside Lock/UnlockRead() or Lock/UnlockWrite() pairs on the appropriate '*MediumLock()' object
	CParticleMedium								*FindCompatibleMedium_NoLock(const CParticleDescriptor *descriptor, hh_u32 stateID);
	CParticleSpatialMedium						*FindCompatibleSpatialMedium_NoLock(const CParticleSpatialDescriptor *descriptor);
	CParticleRenderMedium						*FindCompatibleRenderMedium_NoLock(const CParticleRenderer *descriptor, const CParticleRenderMedium *ignore = null);

	void										FillDrawerArrayForMedium(const CParticleMedium *medium, TArray<TPair<CParticleDrawer *, const CParticleRenderer *> > &outDrawers) const;
	void										FillRenderMediumArrayForMedium(const CParticleMedium *medium, TArray<CParticleRenderMedium *> &outRenderMediums) const;
	hh_u32										GetRenderMediumCountForMedium(const CParticleMedium *medium) const;

	void										NotifyMediumModificationInRenderMediums(const CParticleMedium *medium);
	void										RemoveMedium(const CParticleMedium *medium);
	void										RemoveSpatialMedium(const CParticleSpatialMedium *medium);
	void										RemoveRenderMedium(const CParticleRenderMedium *medium);

	void										SetActiveState(CParticleMedium *medium, bool active);

	CActionExecutor								*ActionExecutor() { return &m_Executor; }

	hh_u32										RegisterSpawnerID(const CParticleMedium *medium, CActionInstanceParticleSpawnerBase *spawner);
	bool										UnregisterSpawnerID(hh_u32 spawnerID HH_ONLY_IN_DEBUG(COMMA CActionInstanceParticleSpawnerBase *spawner));
	const SSpawnerRef							*MapSpawnerID(hh_u32 spawnerID) const;
	const CActionInstanceParticleSpawnerBase	*ResolveSpawnerFromSpawnerID(hh_u32 spawnerID) const;

	void										IncrementSpawnerID_InstanceRefsIFN(const TMemoryView<const hh_u32> &ids);	// ignores 'CGuid::INVALID' values
	void										IncrementSpawnerID_InstanceRefIFN(hh_u32 spawnerID, hh_u32 incCount);
	void										DecrementSpawnerID_InstanceRefsIFN(const TMemoryView<const hh_u32> &ids);
	void										DecrementSpawnerID_InstanceRefIFN(hh_u32 spawnerID, hh_u32 decCount);

	void										IncrementSpawnerID_ParticleRefsIFN(const TMemoryView<const hh_u32> &ids);	// ignores 'CGuid::INVALID' values
	void										IncrementSpawnerID_ParticleRefIFN(hh_u32 spawnerID, hh_u32 incCount);
	void										DecrementSpawnerID_ParticleRefsIFN(const TMemoryView<const hh_u32> &ids);
	void										DecrementSpawnerID_ParticleRefIFN(hh_u32 spawnerID, hh_u32 decCount);

	void										TerminateChildSpawners(hh_u32 spawnerID);
	void										UnsafeClearMediumsSpawners(const CParticleMedium *medium); // Should only be called if particle doesn't exists anymore
	Threads::CCriticalSection					&SpawnerListLock() { return m_SpawnerListLock; }
	TChunkedSlotArray<SSpawnerRef, 512>			&SpawnerList() { return m_SpawnerList; }
	const TChunkedSlotArray<SSpawnerRef, 512>	&SpawnerList() const { return m_SpawnerList; }

	void										SetScene(IParticleScene *scene) { m_Scene = scene; }
	IParticleScene								*Scene() const { return m_Scene; }

	void										SetEditorMode(bool isInEditorMode) { m_IsInEditorMode = isInEditorMode; }
	bool										IsInEditorMode() const { return m_IsInEditorMode; }

	// These view functions are unprotected and must be called when no async update is in progress
	void										ClearAllViews();
	CGuid										RegisterView();
	void										UnregisterView(CGuid viewSlot);
	void										UpdateView(CGuid viewSlot, const CFloat4x4 &worldToView, const CFloat4x4 &worldToProj, const CUint2 &viewportDimensions = CUint2::ONE);
	void										UpdateView(CGuid viewSlot, const CFloat4x4 &worldToView, const CFloat4x4 &viewToWorld, const CFloat4x4 &worldToProj, const CFloat4x4 &projToWorld, const CUint2 &viewportDimensions = CUint2::ONE);
	const SViewData								&MapView(CGuid viewSlot) const;
	SViewData									&MapView(CGuid viewSlot);
	TMemoryView<const SViewData>				RawViews() const { return m_ViewListData; }

	CParticlesStats								&Stats() { return *m_Stats; }
	const CParticlesStats						&Stats() const { return *m_Stats; }
	void										EnableStatsTimingCapture(bool enabled) { m_StatsTimingsEnabled = enabled; }
	bool										HasStatsTimingCapture() const { return m_StatsTimingsEnabled; }
	void										EnableSpinForTasks(bool enabled) { m_SpinForTasksEnabled = enabled; }
	bool										HasSpinForTasks() const { return m_SpinForTasksEnabled; }

	TMemoryView<const PParticleMedium>			Mediums() const { return m_ParticleMediums; }
	TMemoryView<const PParticleSpatialMedium>	SpatialMediums() const { return m_ParticleSpatialMediums; }
	TMemoryView<const PParticleRenderMedium>	RenderMediums() const { return m_ParticleRenderMediums; }

	TMemoryView<const PParticleMedium>			_ActiveMediums_NoLock() const { return m_ActiveParticleMediums; }
	TMemoryView<const PParticleRenderMedium>	_ActiveRenderMediums_NoLock() const { return m_ActiveParticleRenderMediums; }

	void										Update(float dt);
	void										UpdateFence() const;
	bool										UpdatePending() const;	// Like UpdateFence(), but non-blocking. returns false if all update tasks have completed, true if there are some left to process.

	void										PreRender(CRendererSubView &view) const;
	void										StartRender(CRendererSubView &view, const CFloat3 &viewPosInMediumSpace, bool isMainRender) const;
	void										KickRender(CRendererSubView &view, const CFloat3 &viewPosInMediumSpace, bool isMainRender) const;

	// shortcut
	void										Render(CRendererSubView &view, const CFloat3 &viewPosInMediumSpace, bool isMainRender) const
	{
		StartRender(view, viewPosInMediumSpace, isMainRender);
		KickRender(view, viewPosInMediumSpace, isMainRender);
	}

	void										TransformAllParticles(const CFloat4x4 &transforms);

	const CFloat3								&__TEMP_HACK_LastCamPos() const { return m_TEMP_HACK_LastCamPos; }				// FIXME ! TEMP HACK
	void										__TEMP_HACK_SetLastCamPos(const CFloat3 &pos) { m_TEMP_HACK_LastCamPos = pos; }	// FIXME ! TEMP HACK

	// FIXME: Not necessary to have this here!
	// move this to a helper header, and make these functions "process" the medium collection, not be part of it.
	void										GetAllSpawnersWithSameAttributes(const CActionInstanceParticleSpawnerBase *spawner, TArray<const CParticleMediumCollection::SSpawnerRef*> &foundSpawners) const;


	// Sanity checks, in case of mystical SpawnerID bug, or SAttributesContainer crash
	bool										Sanity_AssertNoChildSpawners(hh_u32 spawnerID) const;
	bool										Sanity_AssertNoSpawnerRefs(struct SAttributesContainer const * const *attribContainer) const;

	// FIXME: Temp scaffolding
	TArray<SLayerImplem>						m_LayerImplemMap;
	Threads::CCriticalSection					m_LayerImplemMapLock;

	// Internal
	void										_Update_0(float dt);
	void										_Update_1(float dt);
};

//----------------------------------------------------------------------------

HH_REGISTER_RTTI(CParticleMediumCollection);

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ActionFactoryParticleSpawnerBase);

class	CParticleEffectPreloader : public CNonCopyable
{
private:
	CParticleMediumCollection					*m_MediumCollection;
	TArray<PCParticleDescriptor>				m_ChildDescriptors;
	TArray<PCActionFactoryParticleSpawnerBase>	m_ChildActions;

public:
	CParticleEffectPreloader(CParticleMediumCollection *mCollection);
	~CParticleEffectPreloader();

	void	PreloadEffect(const PCBaseObject &root);	// pass the root Spawner or Effect
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_MEDIUMS_H__

