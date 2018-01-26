//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"

#include "Render/RendererSubView.h"
#include "Render/PopcornFXBuffer.h"
#include "Render/RenderManagerBase.h"
#include "PopcornFXSettings.h"
#include "PopcornFXTypes.h"

#include "Engine/EngineTypes.h"
#include "Math/BoxSphereBounds.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Components/DecalComponent.h" // required for TWeakObjectPtr to work
#include "Components/AudioComponent.h" // required for TWeakObjectPtr to work
#include "PrimitiveSceneProxy.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_mediums.h>
#include <hh_particles/include/ps_scene.h>
#include <hh_particles/include/ps_system.h>
#include <hh_particles/include/ps_bounds.h>
#include <hh_kernel/include/kr_timers.h>

class	IPopcornFXDrawer_UE4;
class	UPopcornFXSceneComponent;
class	FPopcornFXSceneProxy;

#if WITH_PHYSX
namespace	physx
{
	struct	PxRaycastHit;
	class	PxScene;
}
#endif // WITH_PHYSX

FWD_PK_API_BEGIN
class	CParticleMediumCollection;
class	CParticleRenderMedium;
class	CParticleDrawer;
class	CParticleDrawer_Std;
class	CParticleRenderManager;
class	CParticleRenderManager_Std;
class	CActionFactoryDecalSpawner;
namespace Drawers
{
	class	CScene;
}
HH_FORWARD_DECLARE(ParticleUpdaterImmediateTaskD3D11);
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class	CParticleScene : public PopcornFX::IParticleScene
{
public:
	typedef PopcornFX::IParticleScene	Super;

	static CParticleScene	*CreateNew(const UPopcornFXSceneComponent *sceneComp);
	static void				SafeDelete(CParticleScene *&scene);

private:
	CParticleScene();
	~CParticleScene();

public:
	void					Clear();

	const UPopcornFXSceneComponent		*SceneComponent() const;

	//bool					AddAndRunActionInstance(const PopcornFX::PActionInstance &instance, float startDelay = 0.0f);
	bool					AddActionInstance(const PopcornFX::PActionInstance &instance, float startDelay = 0.0f);
	//void					PreloadEffect(FPopcornFXEffect *effect);

	void					StartUpdate(float dt, enum ELevelTick tickType);
	void					SendRenderDynamicData_Concurrent();
	bool					PostUpdate_ShouldMarkRenderStateDirty() const;
	void					GetUsedMaterials(TArray<UMaterialInterface*> &outMaterials, bool bGetDebugMaterials);

	void					GatherSimpleLights(const FSceneViewFamily& ViewFamily, FSimpleLightArray& OutParticleLights) const;

	void					GetDynamicMeshElements(const FPopcornFXSceneProxy *sceneProxy, const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector);

	void					Debug_GoreLock_Lock();
	void					Debug_GoreLock_Unlock();

	PopcornFX::CParticleMediumCollection		*Unsafe_ParticleMediumCollection() { return m_ParticleMediumCollection; }
	const PopcornFX::CParticleMediumCollection	*Unsafe_ParticleMediumCollection() const { return m_ParticleMediumCollection; }

	//const PopcornFX::CAABB	&PkSpaceParticleBounds() const { return m_CachedBounds.CachedBounds(); }
	const FBoxSphereBounds	&Bounds() const { return m_Bounds; }
	//uint32					LastUpdateFrameNumber() const { return m_LastUpdateFrameNumber; }

	const PopcornFX::CStringId	&SPID_SpawnerID() const { return m_SPID_SpawnerID; }

	PopcornFX::CGuid		ParticleMediumCollectionID() const { return m_ParticleMediumCollectionID; }
	PopcornFX::CGuid		SpawnTransformsID() const { return m_SpawnTransformsID; }
	PopcornFX::CGuid		AttributesID() const { return m_AttributesID; }
	PopcornFX::CGuid		PositionsID() const { return m_PositionsID; }
	PopcornFX::CGuid		TransformsID() const { return m_TransformsID; }

	hh_u32					LastUpdatedParticleCount() const { if (m_LastTotalParticleCount < 0) return 0; return hh_u32(m_LastTotalParticleCount); }

private:
	bool										InternalSetup(const UPopcornFXSceneComponent *sceneComp);

	//----------------------------------------------------------------------------

private:
#if (PK_HAS_GPU != 0)
	bool										m_EnableGPU;
#endif
	PopcornFX::CParticleMediumCollection		*m_ParticleMediumCollection;
	PopcornFX::CRendererSubView					m_SubView;

public:
	PopcornFX::Threads::CCriticalSection		m_UpdateLock;

	PopcornFX::Threads::CCriticalSection		m_GoreLock;

private:
	PopcornFX::CSmartCachedBounds				m_CachedBounds;
	FBoxSphereBounds							m_Bounds;
	hh_i32										m_LastTotalParticleCount = 0;

	PopcornFX::CStringId						m_SPID_SpawnerID;

	PopcornFX::CGuid							m_ParticleMediumCollectionID;
	PopcornFX::CGuid							m_SpawnTransformsID;
	PopcornFX::CGuid							m_AttributesID;
	PopcornFX::CGuid							m_PositionsID;
	PopcornFX::CGuid							m_TransformsID;

	//uint32										m_LastUpdateFrameNumber;

	const UPopcornFXSceneComponent				*m_SceneComponent;

	//----------------------------------------------------------------------------
	//
	// Render stuff
	//
	//----------------------------------------------------------------------------
public:
	PopcornFX::Drawers::CScene		*m_BBScene = null;
	PopcornFX::Drawers::CScene		&BBScene() const { HH_ASSERT(m_BBScene != null); return *m_BBScene; }

	// internal drawer use:
	void				OnDrawer_BeginRender(IPopcornFXDrawer_UE4 *drawer);
	void				OnDrawer_EndRender(IPopcornFXDrawer_UE4 *drawer);

	CVertexBufferPool			&VBPool() { return *m_VertexBufferPool; }
	CIndexBufferPool			&IBPool() { return *m_IndexBufferPool; }
	CVertexBufferPool			&GPUVBPool() { return *m_GPUVertexBufferPool; }
	CIndexBufferPool			&GPUIBPool() { return *m_GPUIndexBufferPool; }

	const CVertexBufferPool		&VBPool() const { return *m_VertexBufferPool; }
	const CIndexBufferPool		&IBPool() const { return *m_IndexBufferPool; }
	const CVertexBufferPool		&GPUVBPool() const { return *m_GPUVertexBufferPool; }
	const CIndexBufferPool		&GPUIBPool() const { return *m_GPUIndexBufferPool; }

private:
	template <typename _Drawer>
	PopcornFX::CParticleDrawer_Std		*BuildRemapperWrapper(PopcornFX::CParticleRenderMedium *rMedium, PopcornFX::CParticleRenderManager_Std *manager)
	{
		return _Drawer::BuildRemapper(this, rMedium, manager);
	}

	void				GarbageRenderPools();

	//----------------------------------------------------------------------------

	void				Views_PreUpdate();

	//----------------------------------------------------------------------------

	hh_i32				m_DrawerRenderCount = 0;

	PopcornFX::CRendererSubView		m_RenderSubView;
	PopcornFX::CRendererSubView		m_UpdateSubView;

	uint64							m_LastUpdate = 0;

#if	POPCORNFX_RENDER_DEBUG
	bool							m_IsFreezedBillboardingMatrix = false;
	PopcornFX::CFloat4x4			m_FreezedBillboardingMatrix;
#endif

	PopcornFX::CTimer				m_RenderTimer;
	float							m_LastBufferGCTime = 0;
	CVertexBufferPool				*m_VertexBufferPool;
	CIndexBufferPool				*m_IndexBufferPool;
	CVertexBufferPool				*m_GPUVertexBufferPool;
	CIndexBufferPool				*m_GPUIndexBufferPool;

	struct PopcornFX::SBufferPool_Stats		*m_VertexBufferPoolStats;

	PParticleRenderManagerBase		m_RenderManager;

	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//
	// Internal emitters management
	//
	//----------------------------------------------------------------------------
public:
	struct SEmitterRegister
	{
		class UPopcornFXEmitterComponent			*m_Emitter;
		SEmitterRegister(class UPopcornFXEmitterComponent *emitter) : m_Emitter(emitter) { HH_ASSERT(Valid()); }
		SEmitterRegister() : m_Emitter(null) { }
		HH_FORCEINLINE bool						operator == (class UPopcornFXEmitterComponent *other) const { return m_Emitter == other; }
		HH_FORCEINLINE bool						Valid() const { return m_Emitter != null; }
		static SEmitterRegister					Invalid; // needed by TChunkedSlotArray
	};
	bool				Emitter_IsRegistered(class UPopcornFXEmitterComponent *emitter);
	bool				Emitter_Register(class UPopcornFXEmitterComponent *emitter);
	bool				Emitter_Unregister(class UPopcornFXEmitterComponent *emitter);

private:
	void				Emitters_PreUpdate(float dt, enum ELevelTick tickType);
	void				Emitters_DuringUpdate(float dt, enum ELevelTick tickType);
	void				Emitters_Clear();

	PopcornFX::Threads::CCriticalSection			m_EmittersLock;
	PopcornFX::TChunkedSlotArray<SEmitterRegister>	m_Emitters;

public:
	PopcornFX::TArray<FSimpleLightPerViewEntry>		m_LightPositions;
	PopcornFX::TArray<FSimpleLightEntry>			m_LightDatas;

private:

	//----------------------------------------------------------------------------
	//
	//
	//
	//----------------------------------------------------------------------------

	void		_PostUpdate()
	{
		_PostUpdate_Collisions();
		_PostUpdate_Decals();
		_PostUpdate_Sounds();
		_PostUpdate_Events();
	}

	void		_Clear()
	{
		_Clear_Collisions();
		_Clear_Decals();
		_Clear_Sounds();
		_Clear_Events();
	}

	//----------------------------------------------------------------------------
	//
	// Collisions
	//
	//----------------------------------------------------------------------------

	virtual void			RayTracePacket(
		const PopcornFX::Colliders::STraceFilter &traceFilter,
		const PopcornFX::Colliders::SRayPacket &packet,
		const PopcornFX::Colliders::STracePacket &results) override;

	virtual void			RayTracePacketTemporal(
		const PopcornFX::Colliders::STraceFilter &traceFilter,
		const PopcornFX::Colliders::SRayPacket &packet,
		const PopcornFX::Colliders::STracePacket &results) override;

#if 0
#if WITH_PHYSX
	static bool	ConvertPhysXHit(const physx::PxRaycastHit *hit, FHitResult &outResult, bool queryPhysicalMaterial);
#endif // WITH_PHYSX
#endif // 0

	PopcornFX::Threads::CCriticalSection		m_RaytraceLock;

	void					Collisions_PreUpdate();
	void					_Clear_Collisions();
	void					_PostUpdate_Collisions();

	virtual void	ResolveContactMaterials(
		const TMemoryView<void * const> &contactObjects,
		const TMemoryView<void * const> &contactSurfaces,
		const TMemoryView<PopcornFX::Colliders::SSurfaceProperties> &outSurfaceProperties) const override;

	virtual void	GetDynamicStates(
		const TMemoryView<void* const> &contactObjects,
		const TMemoryView<PopcornFX::CollidableObject::SDynamicState> &outStates) const override;

	virtual void	AddGlobalImpulses(
		const TMemoryView<void * const> &contactObjects,
		const TStridedMemoryView<const CFloat3> &globalImpulses,
		const TStridedMemoryView<const CFloat3> &globalImpactLocations) override;

	PopcornFX::Threads::CCriticalSection		m_ImpulseLock;

	virtual void	InstantiatePostCollideAction(
		void *contactObject,
		void *contactSurface,
		const PopcornFX::CActionFactory *collideTrigger,
		PopcornFX::CActionExecutor *whereToRun,
		const PopcornFX::PActionParameters &parameters,
		PopcornFX::Threads::SThreadContext &threadCtx,
		float timeSinceEndOfFrame,
		float hitFractionInFrame,
		bool isSticky) override;

	struct	SSpawnedActionInstance
	{
		bool								m_Valid;
		PopcornFX::PActionInstance			m_Instance;
		TWeakObjectPtr<UPrimitiveComponent>	m_Primitive;

		CFloat4x4	m_LocalTransforms;
		CFloat4x4	m_CurrentWorldTransforms;
		CFloat4x4	m_PreviousWorldTransforms;
		CFloat3		m_CurrentWorldVelocity;
		CFloat3		m_PreviousWorldVelocity;

		void		_OnDeathNotifier(const PopcornFX::PActionInstance & action);

		SSpawnedActionInstance() : m_Valid(false) {}
		HH_FORCEINLINE bool				Valid() const { return m_Valid; }
		static SSpawnedActionInstance	Invalid;
	};

#if WITH_PHYSX
	physx::PxScene												*m_CurrentPhysxScene = null;
#endif
	PopcornFX::Threads::CCriticalSection						m_SpawnedActionInstancesLock;
	PopcornFX::TChunkedSlotArray<SSpawnedActionInstance, 256>	m_SpawnedActionInstances;

	void	SpawnedActionInstances_PreUpdate(float deltaTime);

	//----------------------------------------------------------------------------
	//
	// Decals
	//
	//----------------------------------------------------------------------------

	void					_Clear_Decals();
	void					_PostUpdate_Decals();

	virtual void			SpawnDecals(
		const SDecalDescriptor &descriptor,
		const TMemoryView<void * const> &contactObjects,
		const TMemoryView<void * const> &contactSurfaces,
		const TMemoryView<const CFloat4> &contactPositions,
		const TMemoryView<const CFloat4> &hitNormals,
		const TMemoryView<const float> &sizes,
		const TMemoryView<const CFloat4> &colors) override;

	struct	SDecalSpawnRequest
	{
		const PopcornFX::CActionFactoryDecalSpawner	*m_ActionFactory;
		FVector				m_Pos;
		FVector				m_Size;
		FRotator			m_Rotation;
		FLinearColor		m_LinearColor;
		bool				m_HasColor;
		union
		{
			UPrimitiveComponent	*m_Component;
			hh_u64				_align_m_Component;
		};
		hh_u32				_padding[2];
	};

	struct	SPendingDecal
	{
		TWeakObjectPtr<UDecalComponent>	m_Component;
		float							m_KillDate;

		SPendingDecal(UDecalComponent *component, float killDate)
			: m_Component(component)
			, m_KillDate(killDate)
		{ }
	};

	PopcornFX::Threads::CCriticalSection		m_DecalSpawnRequestsLock;
	PopcornFX::TArray<SPendingDecal>			m_PendingDecals;
	PopcornFX::TArray<SDecalSpawnRequest>		m_DecalSpawnRequests;

	//----------------------------------------------------------------------------
	//
	// Sounds
	//
	//----------------------------------------------------------------------------

	void						_Clear_Sounds();
	void						_PostUpdate_Sounds();

	virtual void				*StartSound(const SSoundDescriptor &descriptor) override;
	virtual bool				UpdateSound(void *sound, const CFloat3 &worldPosition, float volume) override;
	virtual void				StopSound(void *sound) override;

	struct	SSoundInstance
	{
		PopcornFX::CGuid				m_Index;
		bool							m_Spawned : 1;
		bool							m_UpdateMe : 1;
		bool							m_DestroyMe : 1;
		TWeakObjectPtr<UAudioComponent>	m_AudioComponent;

		PopcornFX::IParticleScene::SSoundDescriptor		m_Desc;

		SSoundInstance() : m_Spawned(false), m_DestroyMe(false) {}

		HH_FORCEINLINE bool				Valid() const { return m_Index.Valid(); }
		static SSoundInstance			Invalid; // needed by TChunkedSlotArray

	};

	PopcornFX::Threads::CCriticalSection				m_SoundsCriticalSection;
	PopcornFX::TChunkedSlotArray<SSoundInstance, 64>	m_SoundSpawnRequests;

	virtual	TMemoryView<const float * const>	GetAudioSpectrum(PopcornFX::CStringId channelGroup, hh_u32 &outBaseCount) const override;
	virtual	TMemoryView<const float * const>	GetAudioWaveform(PopcornFX::CStringId channelGroup, hh_u32 &outBaseCount) const override;

public:
	class IPopcornFXFillAudioBuffers	*m_FillAudioBuffers;
private:

#if (PK_HAS_GPU != 0)
	//----------------------------------------------------------------------------
	//
	// GPU Stuff
	//
	//----------------------------------------------------------------------------

private:
	bool			GPU_InitIFN();
	void			GPU_Destroy();
	void			GPU_PreRender();
	void			GPU_PreUpdate();
	void			GPU_PreUpdateFence();
	void			GPU_PostUpdate();

	//----------------------------------------------------------------------------
	// D3D11
	//----------------------------------------------------------------------------
#if (PK_GPU_D3D11 == 1)
public:
	bool							D3D11Ready() const { return m_EnableD3D11 && m_D3D11_DeferedContext != null; }
	struct ID3D11Device				*D3D11_Device() const { HH_ASSERT(D3D11Ready()); return m_D3D11_Device; }
	struct ID3D11DeviceContext		*D3D11_DeferedContext() const { HH_ASSERT(D3D11Ready()); return m_D3D11_DeferedContext; }

private:
	bool			D3D11_InitIFN();
	void			D3D11_Destroy();
	void			D3D11_PreRender();
	void			D3D11_PreUpdate();
	void			D3D11_PreUpdateFence();
	void			D3D11_PostUpdate();

	void			D3D11_EnqueueImmediateTask(const PopcornFX::PParticleUpdaterImmediateTaskD3D11 &task);

private:
	bool							m_EnableD3D11;
	struct ID3D11Device				*m_D3D11_Device = null;
	struct ID3D11DeviceContext		*m_D3D11_DeferedContext = null;

public:
	PopcornFX::Threads::CCriticalSection									m_D3D11_Tasks_Lock;
	PopcornFX::TArray<PopcornFX::PParticleUpdaterImmediateTaskD3D11>		m_CurrentUpdate_D3D11_Tasks;
	PopcornFX::TArray<PopcornFX::PParticleUpdaterImmediateTaskD3D11>		m_Exec_D3D11_Tasks;

#endif //(PK_GPU_D3D11 == 1)

#endif // (PK_HAS_GPU != 0)

	//----------------------------------------------------------------------------
	//
	// Events
	//
	//----------------------------------------------------------------------------
public:
	struct	SPopcornFXEventListener
	{
		typedef class FPopcornFXRaiseEventSignature	RaiseEventDelegate;

		PopcornFX::CStringId					m_EventName;
		PopcornFX::TArray<RaiseEventDelegate>	m_Delegates;

		SPopcornFXEventListener(const PopcornFX::CStringId &eventName);
	};

	struct	SPopcornFXEventListenerAssoc
	{
		SPopcornFXEventListener	*m_Event;
		hh_u32					m_RaisedCount;
		bool					m_ValidScope;

		SPopcornFXEventListenerAssoc(SPopcornFXEventListener *eventListener, bool validScope)
		:	m_Event(eventListener)
		,	m_RaisedCount(0)
		,	m_ValidScope(validScope) { }
	};

	struct	SPopcornFXFieldListenerValue
	{
		union
		{
			uint32	m_ValueInt[4];
			float	m_ValueFloat[4];
		};
		bool	m_FoundField;
	};

	struct	SPopcornFXFieldListener
	{
		FName								m_FieldName;
		PopcornFX::CStringId				m_CachedFieldName;
		EPopcornFXParticleFieldType::Type	m_FieldType;

		PopcornFX::TArray<SPopcornFXFieldListenerValue>	m_Values;

		SPopcornFXFieldListener(const FName &fieldName, EPopcornFXParticleFieldType::Type fieldType);
	};

	bool	RegisterEventListener(
		const PopcornFX::CStringId &eventName,
		class FPopcornFXRaiseEventSignature callback);

	void	UnregisterEventListener(
		const PopcornFX::CStringId &eventName,
		class FPopcornFXRaiseEventSignature callback);

	void	RegisterFieldListeners();

	bool	GetFieldValue(
		const FName &fieldName,
		void *outValue,
		EPopcornFXParticleFieldType::Type expectedFieldType) const;

private:
	virtual void	RaiseEvent(
		const SEventDescriptor &descriptor,
		const PopcornFX::CParticleMediumCollection *mediumCollection,
		const PopcornFX::CParticlePageView *pageView,
		PopcornFX::TMemoryView<const hh_u32> indices) override;

	void	ClearPendingEvents_NoLock();
	void	_Clear_Events();
	void	_PostUpdate_Events();

private:
	template <typename _OutType>
	void	FillFieldListener(
		SPopcornFXFieldListener &fieldListener,
		const PopcornFX::TArray<int32> &particleIndices,
		const PopcornFX::CParticlePageView *pageView);

	void	RetrieveParticleFields(
		const PopcornFX::TArray<int32> &particleIndices,
		const PopcornFX::CParticlePageView *pageView);

private:
	bool											m_RaiseEventScope;
	hh_u32											m_CurrentEventIndex;
	PopcornFX::Threads::CCriticalSection			m_RaiseEventLock;
	PopcornFX::TArray<SPopcornFXEventListener>		m_EventListeners;
	PopcornFX::TArray<SPopcornFXFieldListener>		m_FieldListeners;
	PopcornFX::TArray<SPopcornFXEventListenerAssoc>	m_PendingEventAssocs;
};
