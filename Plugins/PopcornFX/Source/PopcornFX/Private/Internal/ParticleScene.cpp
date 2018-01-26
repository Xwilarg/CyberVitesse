//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "ParticleScene.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXSceneComponent.h"
#include "World/PopcornFXSceneProxy.h"
#include "PopcornFXEmitterComponent.h"
#include "Assets/PopcornFXRendererMaterial.h"
#include "Render/RendererSubView.h"
#include "PopcornFXAudioSampling.h"
#include "Render/ParticleDrawer_Sound.h"
#include "PopcornFXStats.h"
#include "Render/MyRHIStuff.h"

#ifdef WITH_APEX
#	undef WITH_APEX
#	define WITH_APEX		0
#endif

#if WITH_PHYSX
#	include "PhysXIncludes.h"
#	include "PhysXPublic.h"
#	if (ENGINE_MINOR_VERSION >= 15) // PhysicsFiltering.h public since 4.15
#		include "PhysicsFiltering.h"
#	endif
#endif // WITH_PHYSX

#include "Collision.h"
#include "PhysicsPublic.h"

#if (ENGINE_MINOR_VERSION >= 18)
#	include "CustomPhysXPayload.h"
#else
#	include "Components/DestructibleComponent.h" // FDestructibleChunkInfo
#endif	// (ENGINE_MINOR_VERSION >= 18)

#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "RenderingThread.h"
#include "Engine/LocalPlayer.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#if WITH_EDITOR
#	include "Editor.h"
#	include "LevelEditorViewport.h"
#endif // WITH_EDITOR

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_mediums.h>
#include <hh_particles/include/ps_actions.h>
#include <hh_particles/include/ps_renderers_classes.h>
#include <hh_particles/include/Renderers/Std/renderer_std.h>
#include <hh_particles/include/ps_renderers.h>
#include <hh_particles/include/ps_stats.h>
#include <hh_particles/include/ps_scene.h>
#include <hh_particles/include/Renderers/Std/renderer_std.h>

#include <pk_drawers/include/dr_scene.h>
#include <hh_version.h>

#include <hh_kernel/include/kr_profiler.h>
#include <hh_kernel/include/kr_containers_onstack.h>

#include <hh_particles/include/Updaters/CPU/updater_cpu.h>
#include <hh_particles/include/Updaters/Auto/updater_auto.h>
#if (PK_GPU_D3D11 == 1)
#	include <hh_particles/include/Updaters/D3D11/updater_d3d11.h>
#endif

// BuildParticleObjectTreeViewName
#include <hh_particles/include/ps_debug.h>

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXScene, Log, All);

#define ENABLE_IF_GORE_LOCK(...)
//#define ENABLE_IF_GORE_LOCK(...)		__VA_ARGS__

//#define DEBUG_GORE_LOCK(...)	__VA_ARGS__
#define DEBUG_GORE_LOCK(...)

//----------------------------------------------------------------------------

#if (PK_HAS_GPU == 1)
#	define SCENE_IF_GPU(...)		do { if (m_EnableGPU) {  __VA_ARGS__; } } while (0)
#else
#	define SCENE_IF_GPU(...)		do { } while (0)
#endif

//----------------------------------------------------------------------------

// static
CParticleScene	*CParticleScene::CreateNew(const UPopcornFXSceneComponent *sceneComp)
{
	CParticleScene	*newScene = null;
	if (IPopcornFXPlugin::IsAvailable())
	{
		// we dono if SafeDelete() will be called before PopcornFX shutdown,
		// so use `new` because HH_DELETE might not be available at SafeDelete() time
		newScene = new CParticleScene;
		if (newScene != null)
		{
			if (!newScene->InternalSetup(sceneComp))
			{
				delete newScene;
				newScene = null;
			}
		}
	}
	return newScene;
}

//----------------------------------------------------------------------------

// static
void	CParticleScene::SafeDelete(CParticleScene *&scene)
{
	if (scene != null)
		delete scene;
	scene = null;
}

//----------------------------------------------------------------------------

CParticleScene::CParticleScene()
	: m_ParticleMediumCollection(null)
	, m_FillAudioBuffers(null)
#if (PK_HAS_GPU == 1)
	, m_EnableGPU(false)
#endif
#if (PK_GPU_D3D11 == 1)
	, m_EnableD3D11(true)
#endif
	, m_RaiseEventScope(false)
	, m_CurrentEventIndex(0)
{
	m_RenderTimer.Start();
	m_VertexBufferPool = new CVertexBufferPool();
	m_IndexBufferPool = new CIndexBufferPool();
	m_GPUVertexBufferPool = new CVertexBufferPool();
	m_GPUVertexBufferPool->m_ForGPU = true;
	m_GPUIndexBufferPool = new CIndexBufferPool();
	m_GPUIndexBufferPool->m_ForGPU = true;

	m_VertexBufferPoolStats = new PopcornFX::SBufferPool_Stats();

	m_SPID_SpawnerID.Reset("SpawnerID");
}

//----------------------------------------------------------------------------

CParticleScene::~CParticleScene()
{
	{
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GoreLockUpdateTime);
		ENABLE_IF_GORE_LOCK(m_GoreLock.Lock());
	}

	Clear();

	//PopcornFX::Scheduler::ThreadPool()->_LogPrettyDebug("~CParticleScene");

	m_RenderManager->Clean();
	HH_ASSERT(m_RenderManager->RefCount() == 1);
	m_RenderManager = null;

	SCENE_IF_GPU(GPU_Destroy());

	if (m_ParticleMediumCollection != null)
	{
		HH_DELETE(m_ParticleMediumCollection);
		m_ParticleMediumCollection = null;
	}
	if (m_BBScene != null)
		HH_DELETE(m_BBScene);

	m_FillAudioBuffers = null;

	// Release buffers needs to be in Rendering Thread
	ENQUEUE_UNIQUE_RENDER_COMMAND_FOURPARAMETER(
		ReleaseCommand,
		CVertexBufferPool*, vpool, m_VertexBufferPool,
		CIndexBufferPool*, ipool, m_IndexBufferPool,
		CVertexBufferPool*, gpuvpool, m_GPUVertexBufferPool,
		CIndexBufferPool*, gpuipool, m_GPUIndexBufferPool,
		{
			delete vpool;
			delete ipool;
			delete gpuvpool;
			delete gpuipool;
		});
	m_VertexBufferPool = null;
	m_IndexBufferPool = null;
	m_GPUVertexBufferPool = null;
	m_GPUIndexBufferPool = null;

	ENABLE_IF_GORE_LOCK(m_GoreLock.Unlock());

	FlushRenderingCommands();

	delete m_VertexBufferPoolStats;
}

//----------------------------------------------------------------------------

bool	CParticleScene::InternalSetup(const UPopcornFXSceneComponent *sceneComp)
{
	HH_ASSERT(sceneComp != null);
	m_SceneComponent = sceneComp;

#if (PK_HAS_GPU != 0)
	EPopcornFXConfigPreferedSimulation::Type	simulation = sceneComp->ResolvedSimulationSettings().PreferedSimulation;
	m_EnableGPU = (simulation == EPopcornFXConfigPreferedSimulation::GPU);
	//m_EnableGPU = false;
#endif


	m_ParticleMediumCollection = HH_NEW(PopcornFX::CParticleMediumCollection);
	if (!HH_VERIFY(m_ParticleMediumCollection != null))
		return false;

	m_BBScene = HH_NEW(PopcornFX::Drawers::CScene());
	if (m_BBScene == null)
	{
		HH_DELETE(m_ParticleMediumCollection);
		m_ParticleMediumCollection = null;
		return false;
	}

	{
		PopcornFX::CParticleRenderManager_Std	*renderManager = (PopcornFX::CParticleRenderManager_Std*)(m_ParticleMediumCollection->RenderManager());
		auto									*mapping = renderManager->DrawerMapping();

		mapping->ClearMappings();

		typedef	CParticleDrawer_Sound	Drawer_Sound;

		//mapping->SetRemap<PopcornFX::CParticleRenderer_Billboard>(
		//	PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(this, &CParticleScene::BuildRemapperWrapper<Drawer_Billboard>));
		//mapping->SetRemap<PopcornFX::CParticleRenderer_Ribbon>(
		//	PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(this, &CParticleScene::BuildRemapperWrapper<Drawer_Ribbon>));
		//mapping->SetRemap<PopcornFX::CParticleRenderer_Mesh>(
		//	PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(this, &CParticleScene::BuildRemapperWrapper<Drawer_Mesh>));
		//mapping->SetRemap<PopcornFX::CParticleRenderer_Light>(
		//	PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(this, &CParticleScene::BuildRemapperWrapper<Drawer_Light>));

#if (ENGINE_MINOR_VERSION >= 16)
		// FAudioDevice::GetActiveSounds() now checks IsInAudioThread() !
		mapping->SetRemap<PopcornFX::CParticleRenderer_Sound>(
			PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(this, &CParticleScene::BuildRemapperWrapper<Drawer_Sound>));
#else
		mapping->SetRemap<PopcornFX::CParticleRenderer_Sound>(
			PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(this, &CParticleScene::BuildRemapperWrapper<Drawer_Sound>));
#endif
	}

	HH_ASSERT(m_RenderManager == null);
	m_RenderManager = NewParticleRenderManager();
	if (!HH_VERIFY(m_RenderManager != null))
		return false;

	m_RenderManager->InstallToMediumCollection(*m_ParticleMediumCollection);

	m_ParticleMediumCollection->EnableBounds(true);
	m_ParticleMediumCollection->SetScene(this);

	m_ParticleMediumCollectionID = PopcornFX::CActionParameters::NameGUID<PopcornFX::CParticleMediumCollection*>("ParticleMediumCollection");
	m_SpawnTransformsID = PopcornFX::CActionParameters::NameGUID<PopcornFX::SSpawnTransformsPack>("SpawnTransforms");
	m_AttributesID = PopcornFX::CActionParameters::NameGUID<PopcornFX::SAttributesContainer**>("AttributesContainer");
	m_PositionsID = PopcornFX::CActionParameters::NameGUID<CFloat3>("Position");
	m_TransformsID = PopcornFX::CActionParameters::NameGUID<CFloat4x4>("Transforms");

	SCENE_IF_GPU(
		if (!GPU_InitIFN())
			m_EnableGPU = false
	);

	return true;
}

//----------------------------------------------------------------------------

void	CParticleScene::Clear()
{
	HH_ASSERT(FPopcornFXPlugin::IsMainThread());

	HH_ASSERT(m_ParticleMediumCollection != null && !m_ParticleMediumCollection->UpdatePending());

	{
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("Clear %p start"), this));
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GoreLockUpdateTime);
		ENABLE_IF_GORE_LOCK(m_GoreLock.Lock());
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... Clear %p locked"), this));
	}

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_UpdateLock);

		m_RenderManager->Clear();

		HH_ASSERT(m_ParticleMediumCollection != null);
		m_ParticleMediumCollection->ClearAllViews();
		m_ParticleMediumCollection->Clear();

		FPopcornFXPlugin::IncTotalParticleCount(-m_LastTotalParticleCount);
		m_LastTotalParticleCount = 0;

		Emitters_Clear();
		_Clear();
	}

	ENABLE_IF_GORE_LOCK(m_GoreLock.Unlock());
	DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... Clear %p stop"), this));
}

//----------------------------------------------------------------------------

const UPopcornFXSceneComponent		*CParticleScene::SceneComponent() const
{
	HH_ASSERT(m_SceneComponent != null && m_SceneComponent->IsValidLowLevel());
	return m_SceneComponent;
}

//----------------------------------------------------------------------------

//bool	CParticleScene::AddAndRunActionInstance(const PopcornFX::PActionInstance &instance, float startDelay)
bool	CParticleScene::AddActionInstance(const PopcornFX::PActionInstance &instance, float startDelay)
{
	HH_ASSERT(FPopcornFXPlugin::IsMainThread()); // cannot be called async

	//HH_CRITICAL_SECTION_SCOPEDLOCK(m_GoreLock);

	//PopcornFX::CParticleEffectPreloader(m_ParticleMediumCollection).PreloadEffect(instance->ParentFactory());

	//return m_ParticleMediumCollection->ActionExecutor()->AddAndRunActionInstance(instance, startDelay);
	m_ParticleMediumCollection->ActionExecutor()->AddActionInstance(instance, startDelay);
	return true;
}

//----------------------------------------------------------------------------

//void	CParticleScene::PreloadEffect(FPopcornFXEffect *effect)
//{
//}

//----------------------------------------------------------------------------

void	CParticleScene::Debug_GoreLock_Lock()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::Debug_GoreLock_Lock", POPCORNFX_UE_PROFILER_COLOR);
	{
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("DEBUG %p")));
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GoreLockDebugTime);
		ENABLE_IF_GORE_LOCK(m_GoreLock.Lock());
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... DEBUG %d locked"), this));
	}
}

void	CParticleScene::Debug_GoreLock_Unlock()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::Debug_GoreLock_Unlock", POPCORNFX_UE_PROFILER_COLOR);
	DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("DEBUG unlock %p")));
	ENABLE_IF_GORE_LOCK(m_GoreLock.Unlock());
}

//----------------------------------------------------------------------------

void	CParticleScene::StartUpdate(float dt, enum ELevelTick tickType)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::StartUpdate", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(FPopcornFXPlugin::IsMainThread());

	{
		bool		enableLocalizedPages = false;
		bool		enableByDefault = false;
		switch (m_SceneComponent->ResolvedSimulationSettings().LocalizedPagesMode)
		{
		case EPopcornFXLocalizedPagesMode::Disable: break;
		case EPopcornFXLocalizedPagesMode::EnableDefaultsToOn:
			enableByDefault = true; // no break !
		case EPopcornFXLocalizedPagesMode::EnableDefaultsToOff:
			enableLocalizedPages = true;
			break;
		}
		if (enableLocalizedPages != m_ParticleMediumCollection->HasLocalizedPages() ||
			enableByDefault != m_ParticleMediumCollection->DefaultPageLocalization())
			m_ParticleMediumCollection->EnableLocalizedPages(enableLocalizedPages, enableByDefault);
	}


	// make sure last update if finished
	// should happen only when the Render has not been done
	//if (m_ParticleMediumCollection->UpdatePending())
	//{
	//	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_PreUpdateFenceTime);
	//	m_ParticleMediumCollection->UpdateFence();
	//}

	Views_PreUpdate();

	// By default, register a view at world's origin
	if (m_ParticleMediumCollection->RawViews().Empty())
	{
		const PopcornFX::CGuid	viewId = m_ParticleMediumCollection->RegisterView();
		if (HH_VERIFY(viewId.Valid()))
		{
			static const CInt2	kViewportSize = CInt2(128, 128);
			m_ParticleMediumCollection->UpdateView(0, CFloat4x4::IDENTITY, CFloat4x4::IDENTITY, kViewportSize);
		}
	}

	Collisions_PreUpdate();

	// update registered EmitterComponents
	{
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_UpdateEmittersTime);
		Emitters_PreUpdate(dt, tickType);
	}
	// Update the PostCollideAction action instances
	SpawnedActionInstances_PreUpdate(dt);
	
	if (m_FillAudioBuffers != null)
		m_FillAudioBuffers->PreUpdate();

	// start PopcornFX update
	{
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_StartUpdateTime);

		INC_DWORD_STAT_BY(STAT_PopcornFX_NewParticleCount, m_ParticleMediumCollection->Stats().m_TotalNewParticleCount);
		INC_DWORD_STAT_BY(STAT_PopcornFX_DeadParticleCount, m_ParticleMediumCollection->Stats().m_TotalDeadParticleCount);

		INC_DWORD_STAT_BY(STAT_PopcornFX_ActionCount, m_ParticleMediumCollection->ActionExecutor()->ComputeTotalActionCount());

		INC_DWORD_STAT_BY(STAT_PopcornFX_MediumCount, m_ParticleMediumCollection->Mediums().Count());
		INC_DWORD_STAT_BY(STAT_PopcornFX_RenderMediumCount, m_ParticleMediumCollection->RenderMediums().Count());

		// BEFORE m_UpdateLock LOCK
		SCENE_IF_GPU(GPU_PreUpdate());

		HH_CRITICAL_SECTION_SCOPEDLOCK(m_UpdateLock);

		//{
		//	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::StartUpdate FlushRenderingCommands", POPCORNFX_UE_PROFILER_COLOR);
		//	FlushRenderingCommands();
		//}

		// [LOCK GORE]
		{
			DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("UPDATE %p %d start"), this, m_DrawerRenderCount));
			SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GoreLockUpdateTime);
			ENABLE_IF_GORE_LOCK(m_GoreLock.Lock());
			DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... UPDATE %p %d locked"), this, m_DrawerRenderCount));
		}

		m_LastUpdate = GFrameCounter;

		{
			m_RenderManager->GameThread_PreUpdate(this, *m_ParticleMediumCollection);
		}

		m_ParticleMediumCollection->Stats().Reset();
		// m_ParticleMediumCollection->EnableBounds(true); // enabled only once at startup

		m_ParticleMediumCollection->Update(dt);

		SCENE_IF_GPU(GPU_PreUpdateFence());

		Emitters_DuringUpdate(dt, tickType);

		m_ParticleMediumCollection->UpdateFence();

		SCENE_IF_GPU(GPU_PostUpdate());

		// update bounds and total particle count
		{
			SCOPE_CYCLE_COUNTER(STAT_PopcornFX_UpdateBoundsTime);
			hh_i32				totalParticleCount = 0;
			bool				boundsInit = false;
			PopcornFX::CAABB	bounds = PopcornFX::CAABB::DEGENERATED;
			for (uint32 i = 0; i < m_ParticleMediumCollection->Mediums().Count(); i++)
			{
				const PopcornFX::CParticleMedium	*medium = m_ParticleMediumCollection->Mediums()[i].Get();
				//const hh_u32						particleCount = medium->ParticleStreamForRendering().ParticleCount();
				const hh_u32						particleCount = medium->ParticleStorage()->ActiveParticleCount();
				if (particleCount == 0)
					continue;
				const PopcornFX::CAABB	&mediumBounds = medium->CachedBounds();
				const bool				validBoundingBox =
					mediumBounds.Min().IsFinite() && mediumBounds.Max().IsFinite() &&
					PopcornFX::All(mediumBounds.Min() > -1.0e19f) && PopcornFX::All(mediumBounds.Max() < 1.0e19f);
#if	(HH_ASSERTS_IN_RELEASE != 0)
				if (!validBoundingBox)
				{
					PopcornFX::CString			prettyName;
					if (!PopcornFX::BuildParticleObjectTreeViewName(prettyName, medium->Descriptor()))
						prettyName = medium->Descriptor()->RawName();
					//UE_LOG(LogPopcornFXScene, Warning, TEXT("Problematic/infinite particles bounding box in '%s', see wiki problematic particles"), prettyName.Data());
					HH_RELEASE_ASSERT_MESSAGE(validBoundingBox, "Problematic/infinite particles bounding box in '%s', see wiki problematic particles", prettyName.Data());
				}
#endif
				if (validBoundingBox)
				{
					bounds.Add(mediumBounds);
					boundsInit = true;
				}
				totalParticleCount += particleCount;
			}
			HH_RELEASE_ASSERT(
				!boundsInit ||
				bounds.Min().IsFinite() && bounds.Max().IsFinite() &&
				PopcornFX::All(PopcornFX::HHAbs(bounds.Min()) < 10e20f) &&
				PopcornFX::All(PopcornFX::HHAbs(bounds.Max()) < 10e20f));
			//if (boundsInit &&
			//	bounds.Min().IsFinite() && bounds.Max().IsFinite() &&
			//	PopcornFX::All(PopcornFX::HHAbs(bounds.Min()) < 10e20f) &&
			//	PopcornFX::All(PopcornFX::HHAbs(bounds.Max()) < 10e20f))
			//	m_CachedBounds.SetExactBounds(bounds);
			//else
			//	m_CachedBounds.SetExactBounds(PopcornFX::CAABB::ZERO);
			if (boundsInit)
				m_CachedBounds.SetExactBounds(bounds);
			else
				m_CachedBounds.SetExactBounds(PopcornFX::CAABB::ZERO);
			m_CachedBounds.Update();

			m_Bounds = ToUE(m_CachedBounds.CachedBounds() * FPopcornFXPlugin::GlobalScale());


			//m_Bounds = m_Bounds.ExpandBy(10000);


			FPopcornFXPlugin::IncTotalParticleCount(totalParticleCount - m_LastTotalParticleCount);
			m_LastTotalParticleCount = totalParticleCount;
			INC_DWORD_STAT_BY(STAT_PopcornFX_ParticleCount, totalParticleCount);
		}

		{
			m_RenderManager->GameThread_EndUpdate(this, *m_ParticleMediumCollection);
		}

		ENABLE_IF_GORE_LOCK(m_GoreLock.Unlock());
		// [UNLOCK GORE]

		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... UPDATE %p %d stop"), this, m_DrawerRenderCount));

	}


	// no need to gore lock this

	m_UpdateSubView.Setup_PostUpdate();
	m_ParticleMediumCollection->Render(m_UpdateSubView, CFloat3::ZERO, false);

	_PostUpdate();
}

//----------------------------------------------------------------------------

void	CParticleScene::SendRenderDynamicData_Concurrent()
{
	FPopcornFXPlugin::RegisterCurrentThreadAsUserIFN(); // SendRenderDynamicData_Concurrent can be run un UE's task threads

	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::SendRenderDynamicData", POPCORNFX_UE_PROFILER_COLOR);

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_UpdateLock); // should not happen, but just in case
		m_RenderManager->ConcurrentThread_SendRenderDynamicData(this, *m_ParticleMediumCollection);
	}
}

//----------------------------------------------------------------------------
//
//
//
// Render stuff
//
//
//
//----------------------------------------------------------------------------

void	CParticleScene::GetDynamicMeshElements(
	const FPopcornFXSceneProxy *sceneProxy,
	const ::TArray<const FSceneView*>& Views,
	const FSceneViewFamily& ViewFamily,
	uint32 VisibilityMap,
	FMeshElementCollector& Collector)
{
	if (!IsInActualRenderingThread()) // GetDynamicMeshElements is called on the game thread when resizing viewports
		return;

	FPopcornFXPlugin::RegisterRenderThreadIFN();

	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::GetDynamicMeshElements", POPCORNFX_UE_PROFILER_COLOR);

	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GDMETime);

	SCENE_IF_GPU(GPU_PreRender());

	INC_DWORD_STAT_BY(STAT_PopcornFX_ViewCount, Views.Num());

	if (!m_RenderSubView.Setup_GetDynamicMeshElements(sceneProxy, Views, ViewFamily, VisibilityMap, Collector))
		return;

	if (!HH_VERIFY(m_RenderSubView.BBViews().Count() > 0))
		return;

	const bool		isMainPass = (m_RenderSubView.RenderPass() == CRendererSubView::RenderPass_Main);
	if (isMainPass || m_RenderManager->m_NeedsBuildNewFrameToRender)
	{
		GarbageRenderPools();

#if POPCORNFX_RENDER_DEBUG
		// only for main pass
		// ! unsafe access to m_SceneComponent ?!
		if (m_SceneComponent->bRender_FreezeBillboardingMatrix)
		{
			if (!m_IsFreezedBillboardingMatrix)
			{
				m_IsFreezedBillboardingMatrix = true;
				m_FreezedBillboardingMatrix = m_RenderSubView.BBView(0).m_BillboardingMatrix;
			}
			else
			{
				m_RenderSubView.BBView(0).m_BillboardingMatrix = m_FreezedBillboardingMatrix;
			}
		}
		else if (m_IsFreezedBillboardingMatrix)
		{
			m_IsFreezedBillboardingMatrix = false;
		}
#endif

		m_RenderManager->RenderThread_BuildAndRenderNewFrame(m_RenderSubView);
	}
	else
	{
		m_RenderManager->RenderThread_RenderLastFrame(m_RenderSubView);
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::GatherSimpleLights(const FSceneViewFamily& ViewFamily, FSimpleLightArray& OutParticleLights) const
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::GatherSimpleLights", POPCORNFX_UE_PROFILER_COLOR);
	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GatherSimpleLightsTime);
	HH_ASSERT(IsInRenderingThread());

	const int32		posCount = m_LightPositions.Count();
	const int32		dataCount = m_LightDatas.Count();
	if (posCount == 0 || dataCount == 0)
		return;

	const int32		startPos = OutParticleLights.PerViewData.Num();
	const int32		startData = OutParticleLights.InstanceData.Num();
	OutParticleLights.PerViewData.SetNumUninitialized(startPos + posCount);
	OutParticleLights.InstanceData.SetNumUninitialized(startData + dataCount);

	if (HH_VERIFY(OutParticleLights.PerViewData.Num() == startPos + posCount) &&
		HH_VERIFY(OutParticleLights.InstanceData.Num() == startData + dataCount))
	{
		PopcornFX::Mem::CopyT(&(OutParticleLights.PerViewData[startPos]), &(m_LightPositions[0]), posCount);
		PopcornFX::Mem::CopyT(&(OutParticleLights.InstanceData[startData]), &(m_LightDatas[0]), dataCount);
	}
}

//----------------------------------------------------------------------------

bool	CParticleScene::PostUpdate_ShouldMarkRenderStateDirty() const
{
	return m_RenderManager->m_LastCollectedMaterialsChanged;
}

//----------------------------------------------------------------------------

void	CParticleScene::GetUsedMaterials(TArray<UMaterialInterface*> &outMaterials, bool bGetDebugMaterials)
{
	// @FIXME: GetUsedMaterials will missmatch with the truly used material in render for a frame or two.
	// because it happens we sometime render a frame late/early.
	// (this why FPopcornFXSceneProxy::bVerifyUsedMaterials = false)

	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::GetUsedMaterials", POPCORNFX_UE_PROFILER_COLOR);
	if (m_ParticleMediumCollection == null)
		return;
	const int32		num = m_RenderManager->m_LastCollectedUsedMaterials.Num();
	if (num == 0)
		return;
	outMaterials.Reserve(outMaterials.Num() + num);
	for (int32 i = 0; i < num; ++i)
	{
		UMaterialInterface	*mat = m_RenderManager->m_LastCollectedUsedMaterials[i].Get();
		// Material could have been deleted between the collect and GetUsedMaterials
		// Found ~reproducable crash when reimporting while UPopcornFXEffect editor and
		// one the material of the effect is open (with renderer mesh)
		if (mat == null)
			continue;
		outMaterials.Push(mat);
	}
}

//----------------------------------------------------------------------------

//#define	ENABLE_POOL_STATS

void	CParticleScene::GarbageRenderPools()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::GarbageRenderPools", POPCORNFX_UE_PROFILER_COLOR);
	float			currentTime = m_RenderTimer.Read();
	float			gcElapsedTime = currentTime - m_LastBufferGCTime;

	{
		m_VertexBufferPool->FrameTick();
		m_IndexBufferPool->FrameTick();
		m_GPUVertexBufferPool->FrameTick();
		m_GPUIndexBufferPool->FrameTick();
	}

#ifdef	ENABLE_POOL_STATS
	{
		m_VertexBufferPool->AccumStats(m_VertexBufferPoolStats);
	}
#endif

	if (gcElapsedTime > 2.f)
	{
		m_LastBufferGCTime = currentTime;

		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GCPoolsTime);
		m_VertexBufferPool->GarbageCollect();
		m_IndexBufferPool->GarbageCollect();
		m_GPUVertexBufferPool->GarbageCollect();
		m_GPUIndexBufferPool->GarbageCollect();

#ifdef	ENABLE_POOL_STATS
		m_VertexBufferPoolStats->LogPrettyStats("VB");
		m_VertexBufferPoolStats->Clear();
#endif
	} // GC pools
}

//----------------------------------------------------------------------------

void	CParticleScene::OnDrawer_BeginRender(IPopcornFXDrawer_UE4 *drawer)
{
	DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("DRAWER %p %d start"), this, m_DrawerRenderCount));
	if (m_DrawerRenderCount == 0) // first drawer comes in
	{
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... DRAWER %p %d start FIRST"), this, m_DrawerRenderCount));
		//ENABLE_IF_GORE_LOCK(m_GoreLock.Lock());
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("DRAWER %p %d start FIRST"), this, m_DrawerRenderCount));
	}
	m_DrawerRenderCount++;
}

//----------------------------------------------------------------------------

void	CParticleScene::OnDrawer_EndRender(IPopcornFXDrawer_UE4 *drawer)
{
	DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("DRAWER %p %d kicked"), this, m_DrawerRenderCount));
	HH_ASSERT(m_DrawerRenderCount > 0);
	m_DrawerRenderCount--;
	if (m_DrawerRenderCount == 0) // last drawer finished
	{
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("... DRAWER %p %d start LAST"), this, m_DrawerRenderCount));
		//ENABLE_IF_GORE_LOCK(m_GoreLock.Unlock());
		DEBUG_GORE_LOCK(UE_LOG(LogPopcornFXScene, Warning, TEXT("DRAWER %p %d start LAST"), this, m_DrawerRenderCount));
	}
}

//----------------------------------------------------------------------------
//
//
//
// Internal emitters management
//
//
//
//----------------------------------------------------------------------------

// static
CParticleScene::SEmitterRegister	CParticleScene::SEmitterRegister::Invalid = CParticleScene::SEmitterRegister();

//----------------------------------------------------------------------------

bool	CParticleScene::Emitter_IsRegistered(UPopcornFXEmitterComponent *emitter)
{
	HH_CRITICAL_SECTION_SCOPEDLOCK(m_EmittersLock);
	return m_Emitters.Contains(emitter);
}

//----------------------------------------------------------------------------

bool	CParticleScene::Emitter_Register(UPopcornFXEmitterComponent *emitter)
{
	HH_CRITICAL_SECTION_SCOPEDLOCK(m_EmittersLock);
	HH_ASSERT(!m_Emitters.Contains(emitter));
	PopcornFX::CGuid	emitterId = m_Emitters.Insert(SEmitterRegister(emitter));
	HH_ASSERT(emitterId.Valid());
	if (emitterId.Valid())
		emitter->Scene_OnRegistered(this, emitterId);
	return emitterId.Valid();
}

//----------------------------------------------------------------------------

bool	CParticleScene::Emitter_Unregister(UPopcornFXEmitterComponent *emitter)
{
	const PopcornFX::CGuid	emitterId = emitter->Scene_EmitterId();
	bool					ok = false;
	HH_ASSERT(emitterId.Valid());
	if (emitterId.Valid())
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_EmittersLock);
		HH_ASSERT(emitterId < m_Emitters.Count());
		HH_ASSERT(m_Emitters.IndexOf(emitter) == emitterId);
		HH_ASSERT(m_Emitters[emitterId] == emitter);
		if (emitterId < m_Emitters.Count() && m_Emitters[emitterId] == emitter)
		{
			m_Emitters.Remove(emitterId);
			ok = true;
		}
	}
	// always unregister !
	emitter->Scene_OnUnregistered(this);
	return ok;
}

//----------------------------------------------------------------------------

void	CParticleScene::Emitters_PreUpdate(float dt, enum ELevelTick tickType)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::Emitters_PreUpdate", POPCORNFX_UE_PROFILER_COLOR);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_EmittersLock);
	if (m_Emitters.UsedCount() == 0)
		return;
	INC_DWORD_STAT_BY(STAT_PopcornFX_EmitterUpdateCount, m_Emitters.UsedCount());
	for (uint32 emitteri = 0; emitteri < m_Emitters.Count(); ++emitteri)
	{
		SEmitterRegister		&emitter = m_Emitters[emitteri];
		if (emitter.Valid())
			emitter.m_Emitter->Scene_PreUpdate(this, dt, tickType);
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::Emitters_DuringUpdate(float dt, enum ELevelTick tickType)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::Emitters_DuringUpdate", POPCORNFX_UE_PROFILER_COLOR);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_EmittersLock);
	if (m_Emitters.UsedCount() == 0)
		return;
	INC_DWORD_STAT_BY(STAT_PopcornFX_EmitterUpdateCount, m_Emitters.UsedCount());
	for (uint32 emitteri = 0; emitteri < m_Emitters.Count(); ++emitteri)
	{
		SEmitterRegister		&emitter = m_Emitters[emitteri];
		if (emitter.Valid())
			emitter.m_Emitter->Scene_DuringUpdate(this, dt, tickType);
	}
}


//----------------------------------------------------------------------------

void	CParticleScene::Emitters_Clear()
{
	HH_CRITICAL_SECTION_SCOPEDLOCK(m_EmittersLock);
	if (m_Emitters.UsedCount() == 0)
		return;
	for (uint32 emitteri = 0; emitteri < m_Emitters.Count(); ++emitteri)
	{
		SEmitterRegister		&emitter = m_Emitters[emitteri];
		if (emitter.Valid())
		{
			emitter.m_Emitter->Scene_OnUnregistered(this);
		}
	}
	m_Emitters.Clear();
}

//----------------------------------------------------------------------------

void	CParticleScene::Views_PreUpdate()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::Views_PreUpdate", POPCORNFX_UE_PROFILER_COLOR);

	// This method is auto detect views for each local player (or editor viewport)

	HH_ASSERT(GEngine != null);
	const USceneComponent	*sceneComponent = SceneComponent();
	if (!HH_VERIFY(sceneComponent != null))
		return;
	HH_ASSERT(sceneComponent->GetWorld() != null);

	const float		scaleUEToPk = FPopcornFXPlugin::GlobalScaleRcp();
	UWorld			*world = sceneComponent->GetWorld();

	m_ParticleMediumCollection->ClearAllViews();

	const EWorldType::Type	worldType = world->WorldType;
	if (worldType == EWorldType::Inactive ||
		worldType == EWorldType::None)
		return;

	if (worldType == EWorldType::Game ||
		worldType == EWorldType::PIE)
	{
		auto	playerIt = GEngine->GetLocalPlayerIterator(world);
		while (playerIt)
		{
			const ULocalPlayer		*localPlayer = *playerIt;
			if (localPlayer != null && localPlayer->ViewportClient != null)
			{
				const PopcornFX::CGuid		playerViewId = m_ParticleMediumCollection->RegisterView();
				if (!HH_VERIFY(playerViewId.Valid()))
					return;

				FViewport					*viewport = localPlayer->ViewportClient->Viewport;
				if (!HH_VERIFY(viewport != null))
					return;
				CInt2						viewportSize = ToPk(viewport->GetSizeXY());
				FSceneViewProjectionData	projectionData;
				HH_ASSERT(viewportSize.x() > 0 && viewportSize.y() > 0);
				{
					HH_NAMEDSCOPEDPROFILE_C("CParticleScene::Views_PreUpdate GetProjectionData", POPCORNFX_UE_PROFILER_COLOR);
					if (!HH_VERIFY(localPlayer->GetProjectionData(viewport, eSSP_FULL, projectionData)))
						return;
				}
				const FMatrix	worldToView = FTranslationMatrix(-projectionData.ViewOrigin * scaleUEToPk) * projectionData.ViewRotationMatrix;
				FMatrix			patchedWorldToView = worldToView;

				// PopcornFX expects view forward to be "backward", DirectX style
				// (this is to match the PopcornFX Editor behavior)
				patchedWorldToView.M[0][2] = -patchedWorldToView.M[0][2];
				patchedWorldToView.M[1][2] = -patchedWorldToView.M[1][2];
				patchedWorldToView.M[2][2] = -patchedWorldToView.M[2][2];
				patchedWorldToView.M[3][2] = -patchedWorldToView.M[3][2];

				m_ParticleMediumCollection->UpdateView(playerViewId,
					ToPk(patchedWorldToView),
					ToPk(patchedWorldToView * projectionData.ProjectionMatrix),
					viewportSize);
			}
			++playerIt;
		}
	}
#if WITH_EDITOR
	else if (worldType == EWorldType::EditorPreview ||
			 worldType == EWorldType::GamePreview ||
			 worldType == EWorldType::Editor)
	{
		FViewport				*viewport = null;
		FEditorViewportClient	*viewportClient = null;

		TArray<FEditorViewportClient*>	allClientsCopy = GEditor->AllViewportClients;

		// First, check if we have a FLevelEditorViewportClient (special case)
		bool			levelEditorViewport = false;
		const hh_u32	levelViewportCount = GEditor->LevelViewportClients.Num();
		for (hh_u32 iViewport = 0; iViewport < levelViewportCount; ++iViewport)
		{
			FLevelEditorViewportClient	*client = GEditor->LevelViewportClients[iViewport];
			if (client == null)
				continue;
			allClientsCopy.Remove(client); // Make sure we don't re-iterate over level editor viewport clients next loop
			if (client->GetWorld() == world && client == GCurrentLevelEditingViewportClient)
			{
				viewportClient = client;
				viewport = viewportClient->Viewport;
				levelEditorViewport = true;
				break;
			}
		}

		// If not, can be any kind of viewport
		if (!levelEditorViewport)
		{
			const hh_u32	viewportCount = allClientsCopy.Num();
			for (hh_u32 iViewport = 0; iViewport < viewportCount; ++iViewport)
			{
				FEditorViewportClient	*client = allClientsCopy[iViewport];
				if (client == null)
					continue;
				if (client->GetWorld() == world)
				{
					viewportClient = client;
					viewport = viewportClient->Viewport;
					break;
				}
			}
		}

		// We should at least have one valid viewport
		if (!HH_VERIFY(viewport != null && viewportClient != null))
			return;

		const PopcornFX::CGuid		viewId = m_ParticleMediumCollection->RegisterView();
		if (!HH_VERIFY(viewId.Valid()))
			return;

		FSceneViewProjectionData	projectionData;
		FMinimalViewInfo			viewInfo;
		CInt2						viewportSize = ToPk(viewport->GetSizeXY());

		UCameraComponent	*camera = null;
		if (levelEditorViewport)
			camera = ((FLevelEditorViewportClient*)viewportClient)->GetCameraComponentForView();
		if (HH_PREDICT_LIKELY(camera == null))
		{
			viewInfo.Location = viewportClient->GetViewLocation();
			viewInfo.Rotation = viewportClient->GetViewRotation();
			viewInfo.FOV = viewportClient->ViewFOV;
		}
		else // Viewport locked to a camera actor
		{
			FMinimalViewInfo	view;
			camera->GetCameraView(world->GetDeltaSeconds(), view);

			viewInfo.Location = camera->GetComponentTransform().GetLocation();
			viewInfo.Rotation = camera->GetComponentTransform().Rotator();
			viewInfo.FOV = view.FOV;
		}

		projectionData.ViewOrigin = viewInfo.Location;
		projectionData.SetViewRectangle(FIntRect(0, 0, viewport->GetSizeXY().X, viewport->GetSizeXY().Y));
		projectionData.ViewRotationMatrix = FInverseRotationMatrix(viewInfo.Rotation) * FMatrix(
			FPlane(0, 0, 1, 0),
			FPlane(1, 0, 0, 0),
			FPlane(0, 1, 0, 0),
			FPlane(0, 0, 0, 1));

		const EAspectRatioAxisConstraint	aspectRatioAxisConstraint = GetDefault<ULevelEditorViewportSettings>()->AspectRatioAxisConstraint;
		FMinimalViewInfo::CalculateProjectionMatrixGivenView(viewInfo, aspectRatioAxisConstraint, viewport, projectionData);

		const FMatrix	worldToView = FTranslationMatrix(-projectionData.ViewOrigin * scaleUEToPk) * projectionData.ViewRotationMatrix;
		FMatrix			patchedWorldToView = worldToView;

		// PopcornFX expects view forward to be "backward", DirectX style
		// (this is to match the PopcornFX Editor behavior)
		patchedWorldToView.M[0][2] = -patchedWorldToView.M[0][2];
		patchedWorldToView.M[1][2] = -patchedWorldToView.M[1][2];
		patchedWorldToView.M[2][2] = -patchedWorldToView.M[2][2];
		patchedWorldToView.M[3][2] = -patchedWorldToView.M[3][2];

		m_ParticleMediumCollection->UpdateView(0,
			ToPk(patchedWorldToView),
			ToPk(patchedWorldToView * projectionData.ProjectionMatrix),
			viewportSize);
	}
#endif // WITH_EDITOR
	else
	{
		HH_ASSERT_NOT_REACHED();
	}
}

//----------------------------------------------------------------------------
//
//
//
// Collisions
//
//
//
//----------------------------------------------------------------------------

DECLARE_CYCLE_STAT_EXTERN(TEXT("RayTracePacket"), STAT_PopcornFX_RayTracePacketTime, STATGROUP_PopcornFX, );
DEFINE_STAT(STAT_PopcornFX_RayTracePacketTime);

//----------------------------------------------------------------------------
#if WITH_PHYSX
//----------------------------------------------------------------------------

namespace
{
#if (ENGINE_MINOR_VERSION >= 14)
	using namespace physx;
#endif

#if (ENGINE_MINOR_VERSION < 15) // Runtime/Engine/Private/Collision/PhysicsFiltering.h public since 4.15
	enum EPhysXFilterDataFlags
	{
		EPDF_SimpleCollision	=	0x0001,
		EPDF_ComplexCollision	=	0x0002,
		EPDF_CCD				=	0x0004,
		EPDF_ContactNotify		=	0x0008,
		EPDF_StaticShape		=	0x0010,
		EPDF_ModifyContacts		=   0x0020
	};

#	if (ENGINE_MINOR_VERSION >= 11)
	// see Runtime/Engine/Private/Collision/PhysicsFiltering.h 118 (4.15)
	inline ECollisionChannel GetCollisionChannel(uint32 Word3)
	{
		uint32 NonFlagMask = Word3 >> 23;
		uint32 ChannelMask = NonFlagMask & 0x1F;	//we only want the first 5 bits because there's only 32 channels: 0b11111
		return (ECollisionChannel)ChannelMask;
	}
#	else
	// did not exists <= 4.10 anyway
	inline ECollisionChannel GetCollisionChannel(uint32 Word3)
	{
		return (ECollisionChannel)(Word3 >> 24);
	}
#	endif

#endif // (ENGINE_MINOR_VERSION < 15)

	PxQueryHitType::Enum	_PopcornFXRaycastPreFilter_Simple(PxFilterData filterData, PxFilterData shapeData)
	{
		const ECollisionChannel		shapeChannel = GetCollisionChannel(shapeData.word3);
		const uint32				shapeChannelBit = ECC_TO_BITFIELD(shapeChannel);
		const uint32				filterChannelMask = filterData.word1;

		// Cull overlap requests immediately
		//if (shapeChannel == ECollisionChannel::ECC_OverlapAll_Deprecated)
		//	return PxQueryHitType::eNONE;

		if (!(filterChannelMask & shapeChannelBit))
			return PxQueryHitType::eNONE;

		const uint32		shapeFlags = shapeData.word3 & 0xFFFFFF;
		const uint32		filterFlags = filterData.word3 & 0xFFFFFF;
		const uint32		commonFlags = shapeFlags & filterFlags;

		if (!(commonFlags & (EPDF_SimpleCollision | EPDF_ComplexCollision)))
			return PxQueryHitType::eNONE;

		// ObjectQuery only
		// we should not need to check other flags
		// (see FPxQueryFilterCallback::CalcQueryHitType)

		return PxQueryHitType::eBLOCK;
	}

	PxQueryHitType::Enum		_PopcornFXRaycastPreFilter(PxFilterData filterData, PxFilterData shapeData, const void *constantBlock, PxU32 constantBlockSize, PxHitFlags &hitFlags)
	{
		return _PopcornFXRaycastPreFilter_Simple(filterData, shapeData);
	}

	// see Runtime/Engine/Private/Collision/PhysXCollision.h 120
	// see Runtime/Engine/Private/Collision/PhysXCollision.cpp 609
	class FPopcornFXPxQueryFilterCallback : public PxSceneQueryFilterCallback
	{
	public:
		FPopcornFXPxQueryFilterCallback() {}
		virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxSceneQueryFlags& queryFlags) override
		{
			if (!shape) // can happen since 4.13
			{
				//UE_LOG(LogCollision, Warning, TEXT("Invalid shape encountered in FPxQueryFilterCallback::preFilter, actor: %p, filterData: %x %x %x %x"), actor, filterData.word0, filterData.word1, filterData.word2, filterData.word3);
				return PxQueryHitType::eNONE;
			}
			const PxFilterData		shapeData = shape->getQueryFilterData();
			return _PopcornFXRaycastPreFilter_Simple(filterData, shapeData);
		}
		virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxSceneQueryHit& hit) override
		{
			// Currently not used
			return PxQueryHitType::eBLOCK;
		}
	};

	UPrimitiveComponent			*_PhysXExtractHitComponent(const PxRaycastHit &hit)
	{
		// see Runtime/Engine/Private/Collision/CollisionConversions.cpp
		// SetHitResultFromShapeAndFaceIndex

#if (ENGINE_MINOR_VERSION >= 18)
		const FBodyInstance	*bodyInstance = FPhysxUserData::Get<FBodyInstance>(hit.actor->userData);
		if (bodyInstance != null)
		{
			bodyInstance = bodyInstance->GetOriginalBodyInstance(hit.shape);
			HH_ASSERT(bodyInstance != null);
			return bodyInstance->OwnerComponent.Get();
		}
		const FCustomPhysXPayload	*customPayload = FPhysxUserData::Get<FCustomPhysXPayload>(hit.shape->userData);
		if (customPayload != null)
			return customPayload->GetOwningComponent().Get();
		// Invalid user datas
		return null;
#else
		// This can happen in some cases where APEX moves shapes internally to another actor ( ex. FormExtended structures )
		UPrimitiveComponent		*primitive = FPhysxUserData::Get<UPrimitiveComponent>(hit.shape->userData);
		if (primitive != null)
		{
			return primitive;
		}
		FDestructibleChunkInfo	*chunkInfo = FPhysxUserData::Get<FDestructibleChunkInfo>(hit.shape->userData);
		if (chunkInfo != null && (primitive = chunkInfo->OwningComponent.Get()) != null)
		{
			return primitive;
		}
		FBodyInstance			*bodyInstance = FPhysxUserData::Get<FBodyInstance>(hit.shape->userData);
		if (bodyInstance != null && (primitive = bodyInstance->OwnerComponent.Get()) != null)
		{
			return primitive;
		}
		chunkInfo = FPhysxUserData::Get<FDestructibleChunkInfo>(hit.actor->userData);
		if (chunkInfo != null && (primitive = chunkInfo->OwningComponent.Get()) != null)
		{
			return primitive;
		}
		bodyInstance = FPhysxUserData::Get<FBodyInstance>(hit.actor->userData);
		if (bodyInstance != null && (primitive = bodyInstance->OwnerComponent.Get()) != null)
		{
			return primitive;
		}
		return null;
#endif	// (ENGINE_MINOR_VERSION >= 18)
	}

	UPhysicalMaterial			*_PhysXExtractPhysicalMaterial(const PxRaycastHit &hit)
	{
		UPhysicalMaterial	*umat = null;
		// Query the physical material
		const PxMaterial	*pMat = hit.shape->getMaterialFromInternalFaceIndex(hit.faceIndex);
		if (pMat != null)
			umat = FPhysxUserData::Get<UPhysicalMaterial>(pMat->userData);
		return umat;
	}


} // namespace

#if 0

bool	CParticleScene::ConvertPhysXHit(const PxRaycastHit *hit, FHitResult &outResult, bool queryPhysicalMaterial)
{
#if (ENGINE_MINOR_VERSION >= 14)
	using namespace physx;
#endif

	FBodyInstance			*bodyInstance = FPhysxUserData::Get<FBodyInstance>(hit->shape->userData);
	FDestructibleChunkInfo	*chunkInfo = FPhysxUserData::Get<FDestructibleChunkInfo>(hit->shape->userData);
	UPrimitiveComponent		*primitive = FPhysxUserData::Get<UPrimitiveComponent>(hit->shape->userData);

	if (bodyInstance == null && chunkInfo == null)
	{
		bodyInstance = FPhysxUserData::Get<FBodyInstance>(hit->actor->userData);
		chunkInfo = FPhysxUserData::Get<FDestructibleChunkInfo>(hit->actor->userData);
	}

	UPrimitiveComponent		*owner = chunkInfo != null ? chunkInfo->OwningComponent.Get() : null;
	if (owner == null && bodyInstance != null)
		owner = bodyInstance->OwnerComponent.Get();

	if (primitive != null && owner != primitive)
		owner = primitive;

	if (!HH_VERIFY(owner != null))
		return false;

	outResult.Actor = owner->GetOwner();
	outResult.Component = owner;

	if (queryPhysicalMaterial)
	{
		// Query the physical material
		const PxMaterial	*pMat = hit->shape->getMaterialFromInternalFaceIndex(hit->faceIndex);
		if (pMat != null)
			outResult.PhysMaterial = FPhysxUserData::Get<UPhysicalMaterial>(pMat->userData);
	}

	// Impact information
	outResult.ImpactPoint = _Reinterpret<FVector>(hit->position);
	outResult.Normal = _Reinterpret<FVector>(hit->normal).GetSafeNormal();

	outResult.ImpactNormal = outResult.Normal;

	// HH_ASSERT(ToPk(outResult.ImpactNormal).IsNormalized(10e-3f));

	// Hit information
	outResult.bBlockingHit = true;
	//outResult.bStartPenetrating = false;

	return true;
}

#endif // 0

#endif // WITH_PHYSX
//----------------------------------------------------------------------------

//#define RAYTRACE_PROFILE

#ifdef	RAYTRACE_PROFILE
#	define RAYTRACE_PROFILE_DECLARE				HH_PROFILESTATS_DECLARE
#	define RAYTRACE_PROFILE_CAPTURE				HH_PROFILESTATS_CAPTURE
#	define RAYTRACE_PROFILE_CAPTURE_N			HH_PROFILESTATS_CAPTURE_N
#	define RAYTRACE_PROFILE_CAPTURE_CYCLES		HH_PROFILESTATS_CAPTURE_CYCLES
#	define RAYTRACE_PROFILE_CAPTURE_CYCLES_N	HH_PROFILESTATS_CAPTURE_CYCLES_N
#else
#	define RAYTRACE_PROFILE_DECLARE(...)
#	define RAYTRACE_PROFILE_CAPTURE(...)
#	define RAYTRACE_PROFILE_CAPTURE_N(...)
#	define RAYTRACE_PROFILE_CAPTURE_CYCLES(...)
#	define RAYTRACE_PROFILE_CAPTURE_CYCLES_N(...)
#endif


void	CParticleScene::RayTracePacket(
	const PopcornFX::Colliders::STraceFilter &traceFilter,
	const PopcornFX::Colliders::SRayPacket &packet,
	const PopcornFX::Colliders::STracePacket &results)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::RayTracePacket", POPCORNFX_UE_PROFILER_COLOR);

	//return;

	//HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaytraceLock);

	RAYTRACE_PROFILE_DECLARE(
		//10000,
		50000,
		RayTrace,
		RayTrace_PX_Scene_Lock,
		RayTrace_PX_CreateBatch,
		RayTrace_PX_BuildQuery,
		RayTrace_PX_Exec,
		RayTrace_Results_Hit,
		RayTrace_Results_Hit_Comp,
		RayTrace_Results_Hit_Mat
		);

	const hh_u32	resCount = results.Count();

	RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace, resCount);

	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_RayTracePacketTime);

	HH_ASSERT(packet.m_RayDirectionsAndLengths_Aligned16.Count() == resCount);
	HH_ASSERT(packet.m_RayOrigins_Aligned16.Count() == resCount);
	HH_ASSERT(results.m_HitTimes_Aligned16 != null);
	HH_ASSERT(results.m_ContactNormals_Aligned16 != null);

	HH_ASSERT_MESSAGE(results.m_ContactPoints_Aligned16 == null, "Not implemented");

	//HH_ASSERT(results.m_ContactObjects_Aligned16 != null);

	const bool		queryPhysicalMaterial = m_SceneComponent->ResolvedSimulationSettings().bEnablePhysicalMaterials;
	const float		scalePkToUE = FPopcornFXPlugin::GlobalScale();
	const float		scaleUEToPk = FPopcornFXPlugin::GlobalScaleRcp();

	// Get collision params
	const int32		objectTypesToQuery = m_SceneComponent->GetCollisionObjectTypesToQuery(traceFilter.m_FilterFlags);
	if (objectTypesToQuery == 0) // no collision
		return;

	// False by default for now, we should extract this from traceFilter.m_FilterFlags
	const bool			traceComplexGeometry = true;

#if WITH_PHYSX

#if (ENGINE_MINOR_VERSION >= 14)
	using namespace physx;
#endif

//#	define IF_ASSERTS_PHYSX(...)	HH_ONLY_IF_ASSERTS(__VA_ARGS__)
#	define IF_ASSERTS_PHYSX(...)

	PxScene			* const physxScene = m_CurrentPhysxScene;
	if (!HH_VERIFY(physxScene != null))
		return;
	PxHitFlags					outFlags = IF_ASSERTS_PHYSX(PxHitFlag::ePOSITION | ) PxHitFlag::eDISTANCE | PxHitFlag::eNORMAL;

	PxFilterData				filterData;
	filterData.word0 = 0; // ECollisionQuery::ObjectQuery;
	filterData.word1 = objectTypesToQuery;
	filterData.word3 = EPDF_SimpleCollision;
	if (traceComplexGeometry)
		filterData.word3 |= EPDF_ComplexCollision;

#if (ENGINE_MINOR_VERSION >= 14)
#	define PxSceneQueryFilterFlag		PxQueryFlag
#endif
	PxSceneQueryFilterData		queryFilterData(filterData, PxSceneQueryFilterFlag::eSTATIC | PxSceneQueryFilterFlag::eDYNAMIC | PxSceneQueryFilterFlag::ePREFILTER);

/****

Bench Pk_PhysicsBench.pkfx
50,000 particles
speed 5 m/s

// single raycast, 1 worker threads no affinity prio normal
[INFO][UE4]>                  RayTrace : 3186626    1661.219 cycles/call
[INFO][UE4]>    RayTrace_PX_Scene_Lock : 3186626       2.744 cycles/call
[INFO][UE4]>   RayTrace_PX_CreateBatch : 0             0.000 us/call
[INFO][UE4]>    RayTrace_PX_BuildQuery : 3186626       5.872 cycles/call
[INFO][UE4]>          RayTrace_PX_Exec : 3185783    1522.972 cycles/call
[INFO][UE4]>      RayTrace_Results_Hit : 42310       323.567 cycles/call
[INFO][UE4]> RayTrace_Results_Hit_Comp : 42310       222.571 cycles/call
[INFO][UE4]>  RayTrace_Results_Hit_Mat : 0             0.000 us/call

// batch query, 1 worker threads no affinity prio normal
[INFO][UE4]>                  RayTrace : 3188779    2256.339 cycles/call
[INFO][UE4]>    RayTrace_PX_Scene_Lock : 3188779       3.656 cycles/call
[INFO][UE4]>   RayTrace_PX_CreateBatch : 3188779      37.641 cycles/call
[INFO][UE4]>    RayTrace_PX_BuildQuery : 3188779     403.921 cycles/call
[INFO][UE4]>          RayTrace_PX_Exec : 3188779    1726.117 cycles/call
[INFO][UE4]>      RayTrace_Results_Hit : 55939       380.799 cycles/call
[INFO][UE4]> RayTrace_Results_Hit_Comp : 55939       301.805 cycles/call
[INFO][UE4]>  RayTrace_Results_Hit_Mat : 0             0.000 us/call


// single raycast, default 6 worker threads no aff prio normal
[INFO][UE4]>                  RayTrace : 3188526    5235.209 cycles/call
[INFO][UE4]>    RayTrace_PX_Scene_Lock : 3188526       7.105 cycles/call
[INFO][UE4]>   RayTrace_PX_CreateBatch : 0             0.000 us/call
[INFO][UE4]>    RayTrace_PX_BuildQuery : 3188526      10.969 cycles/call
[INFO][UE4]>          RayTrace_PX_Exec : 3188183    5022.164 cycles/call
[INFO][UE4]>      RayTrace_Results_Hit : 24767      1170.227 cycles/call
[INFO][UE4]> RayTrace_Results_Hit_Comp : 24767      1029.273 cycles/call
[INFO][UE4]>  RayTrace_Results_Hit_Mat : 0             0.000 us/call

// batch query, default 6 worker threads no aff prio normal
[INFO][UE4]>                  RayTrace : 3188626    5218.253 cycles/call
[INFO][UE4]>    RayTrace_PX_Scene_Lock : 3188626       8.677 cycles/call
[INFO][UE4]>   RayTrace_PX_CreateBatch : 3188626     117.457 cycles/call
[INFO][UE4]>    RayTrace_PX_BuildQuery : 3188626     565.234 cycles/call
[INFO][UE4]>          RayTrace_PX_Exec : 3188626    4302.141 cycles/call
[INFO][UE4]>      RayTrace_Results_Hit : 24651      1208.287 cycles/call
[INFO][UE4]> RayTrace_Results_Hit_Comp : 24651      1068.593 cycles/call
[INFO][UE4]>  RayTrace_Results_Hit_Mat : 0             0.000 us/call

****/

#define DO_PHYSX_BATCH_QUERY	0

#if (DO_PHYSX_BATCH_QUERY == 0)

	FPopcornFXPxQueryFilterCallback		queryCallback; // PreFilter

	hh_u32			hitResultBufferCurrentIndex = 0;
	struct FRaycastBufferAndIndex
	{
		hh_u32				m_RayIndex;
		PxRaycastBuffer		m_Buffer;
	};
	HH_STACKMEMORYVIEW(FRaycastBufferAndIndex, hitResultBuffers, resCount);

	{
		RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_Scene_Lock, resCount);
		// !! LOCK physics scene
		physxScene->lockRead();
	}
	{
		for (hh_u32 rayi = 0; rayi < resCount; ++rayi)
		{
			CFloat3		start;
			CFloat3		rayDir;
			float		rayLen;
			{
				RAYTRACE_PROFILE_CAPTURE_CYCLES(RayTrace_PX_BuildQuery);
				const CFloat4		&_rayDirAndLen = packet.m_RayDirectionsAndLengths_Aligned16[rayi];
				// PhysX collides even with rayLen == 0
				// PopcornFX input rayLen == 0 means ignore it
				if (_rayDirAndLen.w() <= 0)
					continue;
				start = packet.m_RayOrigins_Aligned16[rayi].xyz() * scalePkToUE;
				rayDir = _rayDirAndLen.xyz();
				rayLen = _rayDirAndLen.w() * scalePkToUE;
			}

			FRaycastBufferAndIndex		&resultBufferAndIndex = hitResultBuffers[hitResultBufferCurrentIndex];
			{
				RAYTRACE_PROFILE_CAPTURE_CYCLES(RayTrace_PX_Exec);
				PxRaycastBuffer		&outResultBuffer = resultBufferAndIndex.m_Buffer;

				physxScene->raycast(_Reinterpret<PxVec3>(start), _Reinterpret<PxVec3>(rayDir), rayLen, outResultBuffer, outFlags, queryFilterData, &queryCallback);

				// negative rays do happen, maybe because of physx float precision issues ?
				const float			kMinLenEpsilon = -1e-03f; // 10 micrometer still acceptable ?!
				HH_ASSERT(outResultBuffer.block.distance >= kMinLenEpsilon);
				outResultBuffer.block.distance = PopcornFX::HHMax(outResultBuffer.block.distance, 0.f);

				if (HH_PREDICT_LIKELY(!outResultBuffer.hasBlock))
					continue;
			}
			resultBufferAndIndex.m_RayIndex = rayi;
			++hitResultBufferCurrentIndex;
		}
	}

	void			** contactObjects = results.m_ContactObjects_Aligned16;
	void			** contactSurfaces = queryPhysicalMaterial ? results.m_ContactSurfaces_Aligned16 : null;

	const hh_u32	hitResultCount = hitResultBufferCurrentIndex;
	for (hh_u32 hiti = 0; hiti < hitResultCount; ++hiti)
	{
		RAYTRACE_PROFILE_CAPTURE_CYCLES(RayTrace_Results_Hit);

		const FRaycastBufferAndIndex	&resultBufferAndIndex = hitResultBuffers[hiti];
		const hh_u32					rayi = resultBufferAndIndex.m_RayIndex;
		HH_ASSERT(rayi < resCount);
		const PxRaycastHit				&hit = resultBufferAndIndex.m_Buffer.block;
		HH_ASSERT(resultBufferAndIndex.m_Buffer.hasBlock);

		const CFloat4		&_rayDirAndLen = packet.m_RayDirectionsAndLengths_Aligned16[rayi];
		const float			rayLen = _rayDirAndLen.w();
		const CFloat3		rayDir = _rayDirAndLen.xyz();

		const float			hitTime = hit.distance * scaleUEToPk;
		// PhysX sometimes collides farther than asked !!
		IF_ASSERTS_PHYSX(HH_ASSERT(hitTime < rayLen);)
		// hitTime = PopcornFX::HHMin(hitTime, rayLen);

		results.m_HitTimes_Aligned16[rayi] = hitTime;

		IF_ASSERTS_PHYSX(
			const CFloat3	hitPos = _Reinterpret<CFloat3>(hit.position) * scaleUEToPk;
			const float		computedTime = (hitPos - packet.m_RayOrigins_Aligned16[rayi].xyz()).Length();
			HH_ASSERT(PopcornFX::HHAbs(computedTime - hitTime) < 1.0e-3f);
		)

		// Patch PhysX invalid normal !!!!
		CFloat3			normal = _Reinterpret<CFloat3>(hit.normal);
		IF_ASSERTS_PHYSX(HH_ASSERT(normal.IsNormalized(10e-3f)));
		{
			static const float		kThreshold = 1.0e-3f * 1.0e-3f;
			const float				lenSq = normal.LengthSquared();
			if (PopcornFX::HHAbs(lenSq - 1.0f) >= kThreshold)
			{
				if (lenSq <= kThreshold || !PopcornFX::TNumericTraits<float>::IsFinite(lenSq))
					normal = - rayDir;
				else
					normal /= PopcornFX::HHSqrt(lenSq);
			}
			HH_ASSERT(normal.IsNormalized(10e-3f));
		}
		results.m_ContactNormals_Aligned16[rayi].xyz() = normal;

		if (contactObjects != null)
		{
			RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_Results_Hit_Comp, 1);
			contactObjects[rayi] = _PhysXExtractHitComponent(hit);
		}
		if (contactSurfaces != null)
		{
			RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_Results_Hit_Mat, 1);
			contactSurfaces[rayi] = _PhysXExtractPhysicalMaterial(hit);
		}
	}

	// !! UNLOCK physics scene
	physxScene->unlockRead();

#else // if DO_PHYSX_BATCH_QUERY

	HH_STACKMEMORYVIEW(PxRaycastQueryResult, queryResults, resCount);
	HH_STACKMEMORYVIEW(PxRaycastHit, hitResults, resCount);

	{
		RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_Scene_Lock, resCount);
		// !! LOCK physics scene
		physxScene->lockRead();
	}

	// create physx batch
	PxBatchQuery	*raycastBatchQuery = null;
	{
		RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_CreateBatch, resCount);

		// Sweep requests are not implemented yet
		PxBatchQueryDesc	desc(resCount, 0, 0);
		desc.queryMemory.userRaycastResultBuffer = queryResults.Data();// queryResults.GetData();
		desc.queryMemory.userRaycastTouchBuffer = hitResults.Data();// hitResults.GetData();
		desc.queryMemory.raycastTouchBufferSize = resCount;
		desc.preFilterShader = &_PopcornFXRaycastPreFilter;
		desc.runOnSpu = false;

		{
			{
				//RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_CreateBatch_Lock, resCount);
				m_RaytraceLock.Lock();
			}
			HH_FIXME("This should be lockWrite");
			//
			// If there is someone else doing createBatchQuery in parallel, there will be datarace
			// A good way to fix would be to create a cacheBatchQueries[ThreadID]
			//
			raycastBatchQuery = physxScene->createBatchQuery(desc);
			m_RaytraceLock.Unlock();
		}
		if (!HH_VERIFY(raycastBatchQuery != null))
		{
			physxScene->unlockRead();
			return;
		}
	}

	{
		RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_BuildQuery, resCount);
		for (hh_u32 rayi = 0; rayi < resCount; ++rayi)
		{
			const CFloat4	&_rayDirAndLen = packet.m_RayDirectionsAndLengths_Aligned16[rayi];
			const CFloat3	start = packet.m_RayOrigins_Aligned16[rayi].xyz() * scalePkToUE;
			const CFloat3	rayDir = _rayDirAndLen.xyz();
			const float		rayLen = _rayDirAndLen.w() * scalePkToUE;
			raycastBatchQuery->raycast(_Reinterpret<PxVec3>(start), _Reinterpret<PxVec3>(rayDir), rayLen, 0, outFlags, queryFilterData);
		}
	}
	{
		RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_Exec, resCount);
		// Execute queries
		raycastBatchQuery->execute();
	}

	// everything should be in buffers now, release the query ASAP
	{
		{
			//RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_PX_Release_Lock, resCount);
			m_RaytraceLock.Lock();
		}
		raycastBatchQuery->release();
		m_RaytraceLock.Unlock();
	}
	raycastBatchQuery = null;

	// !! UNLOCK physics scene
	physxScene->unlockRead();

	void			** contactObjects = results.m_ContactObjects_Aligned16;
	void			** contactSurfaces = queryPhysicalMaterial ? results.m_ContactSurfaces_Aligned16 : null;

	{
		//RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_Results, resCount);
		for (hh_u32 rayi = 0; rayi < resCount; ++rayi)
		{
			const PxRaycastQueryResult	&result = queryResults[rayi];
			const bool					hasHit = result.hasBlock;
			if (HH_PREDICT_LIKELY(!hasHit))
				continue;

			const PxRaycastHit			&hit = result.block;

			const CFloat4		&_rayDirAndLen = packet.m_RayDirectionsAndLengths_Aligned16[rayi];
			const float			rayLen = _rayDirAndLen.w();
			const CFloat3		rayDir = _rayDirAndLen.xyz();

			// PhysX collides even with rayLen == 0
			// input rayLen == 0 means ignore it
			if (rayLen == 0)
				continue;

			RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_Results_Hit, 1);

			const float		hitTime = hit.distance * scaleUEToPk;
			// PhysX sometimes collides farther than asked !!
			IF_ASSERTS_PHYSX(HH_ASSERT(hitTime < rayLen);)
			// hitTime = PopcornFX::HHMin(hitTime, rayLen);

			results.m_HitTimes_Aligned16[rayi] = hitTime;

			IF_ASSERTS_PHYSX(
				const CFloat3	hitPos = _Reinterpret<CFloat3>(hit.position) * scaleUEToPk;
				const float		computedTime = (hitPos - packet.m_RayOrigins_Aligned16[rayi].xyz()).Length();
				HH_ASSERT(PopcornFX::HHAbs(computedTime - hitTime) < 1.0e-3f);
			)

			// Patch PhysX invalid normal !!!!
			CFloat3			normal = _Reinterpret<CFloat3>(hit.normal);
			IF_ASSERTS_PHYSX(HH_ASSERT(normal.IsNormalized(10e-3f)));
			{
				static const float		kThreshold = 1.0e-3f * 1.0e-3f;
				const float				lenSq = normal.LengthSquared();
				if (PopcornFX::HHAbs(lenSq - 1.0f) >= kThreshold)
				{
					if (lenSq <= kThreshold || !PopcornFX::TNumericTraits<float>::IsFinite(lenSq))
						normal = - rayDir;
					else
						normal /= PopcornFX::HHSqrt(lenSq);
				}
				HH_ASSERT(normal.IsNormalized(10e-3f));
			}
			results.m_ContactNormals_Aligned16[rayi].xyz() = normal;

			if (contactObjects != null)
			{
				RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_Results_Hit_Comp, 1);
				contactObjects[rayi] = _PhysXExtractHitComponent(hit);
			}
			if (contactSurfaces != null)
			{
				RAYTRACE_PROFILE_CAPTURE_CYCLES_N(RayTrace_Results_Hit_Mat, 1);
				contactSurfaces[rayi] = _PhysXExtractPhysicalMaterial(hit);
			}
		}
	}

#endif // DO_PHYSX_BATCH_QUERY

#endif // WITH_PHYSX
}

//----------------------------------------------------------------------------

void	CParticleScene::RayTracePacketTemporal(
	const PopcornFX::Colliders::STraceFilter &traceFilter,
	const PopcornFX::Colliders::SRayPacket &packet,
	const PopcornFX::Colliders::STracePacket &results)
{
	RayTracePacket(traceFilter, packet, results);
}

//----------------------------------------------------------------------------

// static
CParticleScene::SSpawnedActionInstance	CParticleScene::SSpawnedActionInstance::Invalid = CParticleScene::SSpawnedActionInstance();

//----------------------------------------------------------------------------

void		CParticleScene::SSpawnedActionInstance::_OnDeathNotifier(const PopcornFX::PActionInstance & action)
{
	HH_ASSERT(m_Instance == action);
	if (m_Instance != null)
		m_Instance->m_DeathNotifier -= PopcornFX::FastDelegate<void(const PopcornFX::PActionInstance &)>(this, &SSpawnedActionInstance::_OnDeathNotifier);
	m_Instance = null;
	// will be removed in PostUpdate
}

//----------------------------------------------------------------------------

void	CParticleScene::Collisions_PreUpdate()
{
#if WITH_PHYSX
	m_CurrentPhysxScene = null;
	if (SceneComponent() != null)
	{
		const UWorld	*world = m_SceneComponent->GetWorld();
		if (HH_VERIFY(world != null))
		{
			FPhysScene		*worldPhysScene = world->GetPhysicsScene();
			if (worldPhysScene != null)
			{
				m_CurrentPhysxScene = worldPhysScene->GetPhysXScene(PST_Sync);
			}
		}
	}
#endif // WITH_PHYSX
}

//----------------------------------------------------------------------------

void	CParticleScene::_Clear_Collisions()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_Clear_Collisions", POPCORNFX_UE_PROFILER_COLOR);
	if (m_SpawnedActionInstances.UsedCount() == 0)
		return;
	for (hh_u32 iInstance = 0; iInstance < m_SpawnedActionInstances.Count(); ++iInstance)
	{
		SSpawnedActionInstance	&instance = m_SpawnedActionInstances[iInstance];
		if (!instance.Valid())
			continue;
		if (HH_VERIFY(instance.m_Instance == null))
			continue;
		instance.m_Instance->m_DeathNotifier -= PopcornFX::FastDelegate<void(const PopcornFX::PActionInstance &)>(&instance, &SSpawnedActionInstance::_OnDeathNotifier);
	}
	m_SpawnedActionInstances.Clear();
}

//----------------------------------------------------------------------------

void	CParticleScene::_PostUpdate_Collisions()
{
}

//----------------------------------------------------------------------------

void	CParticleScene::ResolveContactMaterials(
	const TMemoryView<void * const> &contactObjects,
	const TMemoryView<void * const> &contactSurfaces,
	const TMemoryView<PopcornFX::Colliders::SSurfaceProperties> &outSurfaceProperties) const
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::ResolveContactMaterials", POPCORNFX_UE_PROFILER_COLOR);

	//Super::ResolveContactMaterials(contactObjects, contactSurfaces, outSurfaceProperties);
	//return;

	//HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaytraceLock);

	const bool		queryPhysicalMaterial = m_SceneComponent->ResolvedSimulationSettings().bEnablePhysicalMaterials;
	if (!queryPhysicalMaterial)
		return; // no need to use kDefaultSurface, everyone returns or no one.

	HH_ASSERT(contactObjects.Count() == contactSurfaces.Count());
	HH_ASSERT(contactObjects.Count() == outSurfaceProperties.Count());

	static const PopcornFX::Colliders::SSurfaceProperties		kDefaultSurface;

	const hh_u32	materialCount = contactSurfaces.Count();
	for (hh_u32 iMaterial = 0; iMaterial < materialCount; ++iMaterial)
	{
		PopcornFX::Colliders::SSurfaceProperties	&surface = outSurfaceProperties[iMaterial];
		surface = kDefaultSurface;

		const UPhysicalMaterial		*pMat = reinterpret_cast<const UPhysicalMaterial*>(contactSurfaces[iMaterial]);
		if (pMat == null)
			continue;

		surface.m_Restitution = pMat->Restitution;
		surface.m_StaticFriction = pMat->Friction;
		surface.m_DynamicFriction = surface.m_StaticFriction;
		surface.m_SurfaceType = pMat->SurfaceType.GetValue();

#define REMAP_COMBINE_MODE(__member, __val)													\
		switch (__val)																		\
		{																					\
			case	EFrictionCombineMode::Average:											\
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Average;	\
				break;																		\
			case	EFrictionCombineMode::Min:												\
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Min;		\
				break;																		\
			case	EFrictionCombineMode::Multiply:											\
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Multiply;\
				break;																		\
			case	EFrictionCombineMode::Max:												\
				surface.__member = PopcornFX::Colliders::ECombineMode::Combine_Max;		\
				break;																		\
			default:																		\
				HH_ASSERT_NOT_REACHED();													\
				break;																		\
		}

		REMAP_COMBINE_MODE(m_FrictionCombineMode, pMat->FrictionCombineMode.GetValue());
		REMAP_COMBINE_MODE(m_RestitutionCombineMode, pMat->RestitutionCombineMode.GetValue());

#undef REMAP_COMBINE_MODE
	}
}

//----------------------------------------------------------------------------

#if WITH_PHYSX

namespace
{
#if (ENGINE_MINOR_VERSION >= 14)
	using namespace physx;
#endif

	HH_FORCEINLINE PopcornFX::CFloat3		ToPk(const PxVec3 &v)
	{
		return _Reinterpret<PopcornFX::CFloat3>(v);
	}
}

#endif // WITH_PHYSX

void	CParticleScene::GetDynamicStates(
	const TMemoryView<void* const> &contactObjects,
	const TMemoryView<PopcornFX::CollidableObject::SDynamicState> &outStates) const
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::GetDynamicStates", POPCORNFX_UE_PROFILER_COLOR);

	//Super::GetDynamicStates(contactObjects, outStates);
	//return;

	//HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaytraceLock);

	HH_ASSERT(contactObjects.Count() == outStates.Count());
	if (!HH_VERIFY(contactObjects.Count() > 0))
		return;

	static const PopcornFX::CollidableObject::SDynamicState		kStaticState;

	const float		scaleUEToPk = FPopcornFXPlugin::GlobalScaleRcp();
	const float		scaleUEToPkRcp = FPopcornFXPlugin::GlobalScale();
	const hh_u32	stateCount = contactObjects.Count();

#if WITH_PHYSX
#if (ENGINE_MINOR_VERSION >= 14)
	using namespace physx;
#endif

	PxScene			* const physxScene = m_CurrentPhysxScene;
	if (!HH_VERIFY(physxScene != null))
		return;
#endif // WITH_PHYSX

	for (hh_u32 iState = 0; iState < stateCount; ++iState)
	{
		PopcornFX::CollidableObject::SDynamicState		&state = outStates[iState];
		state = kStaticState;

		UPrimitiveComponent		*component = reinterpret_cast<UPrimitiveComponent*>(contactObjects[iState]);

		if (component == null || component->Mobility != EComponentMobility::Movable)// !component->BodyInstance.bSimulatePhysics)
			continue;

		FBodyInstance			*BI = component->GetBodyInstance();
		if (BI == null)
			continue;

		// see WarnInvalidPhysicsOperations_Internal
		// see Runtime/Engine/Private/PrimitiveComponentPhysics.cpp
		// ECollisionEnabled::Type		CollisionEnabled = BI->GetCollisionEnabled();
		if (!BI->bSimulatePhysics)
			continue;
		//else if (CollisionEnabled == ECollisionEnabled::NoCollision || CollisionEnabled == ECollisionEnabled::QueryOnly)	//shapes need to be simulating
		//	continue;

#if WITH_PHYSX

		// see Runtime/Engine/Public/PhysXPublic.h
		// GetPhysXSceneFromIndex() is not exported !!! so we cannot use ExecuteOnPxRigidBodyReadOnly

		// Each call to BI->Get...() will lock and all at each call
		// do it only once here

		physxScene->lockRead();

		const physx::PxRigidBody		*PRigidBody = null;
#if (ENGINE_MINOR_VERSION >= 14)
		PRigidBody = BI->GetPxRigidBody_AssumesLocked();
#else
		if (const physx::PxRigidActor* RigidActor = BI->GetPxRigidActor_AssumesLocked())
			PRigidBody = RigidActor->isRigidBody();
#endif
		if (PRigidBody != null)
		{
			state.m_LinearVelocity = ToPk(PRigidBody->getLinearVelocity()) * scaleUEToPk;
			state.m_AngularVelocity = ToPk(PRigidBody->getAngularVelocity());

			const PxTransform		TrCOMToLocal = PRigidBody->getCMassLocalPose();
			const PxTransform		TrLocalToWorld = PRigidBody->getGlobalPose();
			const physx::PxMat33	COMToWorld = physx::PxMat33(TrLocalToWorld.q * TrCOMToLocal.q);
			const physx::PxMat33	WorldToCOM = COMToWorld.getTranspose();	// Safe, COMToWorld is orthonormal, built from a quat

			state.m_CenterOfMass = ToPk(TrLocalToWorld.transform(TrCOMToLocal.p)) * scaleUEToPk;
			state.m_InvMass = PRigidBody->getInvMass();

			// Inertia tensor has units of mass.distance^2, computed from mass integral r^2.dm,
			// therefore, to rescale it by distance, we need to apply the squared scale.
			// (and inverse squared scale, as we're scaling the inverse inertia tensor)
			const physx::PxVec3		invMomentOfInertia = PRigidBody->getMassSpaceInvInertiaTensor() * (scaleUEToPkRcp * scaleUEToPkRcp);
			const CFloat3			pkInvMOI = ToPk(invMomentOfInertia);

			state.m_InvInertiaTensor = _Reinterpret<PopcornFX::CFloat3x3>(WorldToCOM);
			state.m_InvInertiaTensor.XAxis() *= pkInvMOI;
			state.m_InvInertiaTensor.YAxis() *= pkInvMOI;
			state.m_InvInertiaTensor.ZAxis() *= pkInvMOI;
		}

		physxScene->unlockRead();

#else // No PhysX
		state.m_LinearVelocity = ToPk(BI->GetUnrealWorldVelocity() * scaleUEToPk);
		state.m_AngularVelocity = ToPk(FVector::DegreesToRadians(BI->GetUnrealWorldAngularVelocity()));
		state.m_CenterOfMass = ToPk(BI->GetCOMPosition() * scaleUEToPk);
		state.m_InvMass = 1.0f / BI->GetBodyMass();
		HH_ASSERT(TNumericTraits<float>::IsFinite(state.m_InvMass));	// bodyMass cannot be zero !

		const float		kRcpTolerance = 1.0e-10f;
		const FVector	inertiaTensor = BI->GetBodyInertiaTensor();
		const FVector	invInertiaTensor = FVector(	inertiaTensor.X > kRcpTolerance ? 1.0f / inertiaTensor.X : 0.0f,
													inertiaTensor.Y > kRcpTolerance ? 1.0f / inertiaTensor.Y : 0.0f,
													inertiaTensor.Z > kRcpTolerance ? 1.0f / inertiaTensor.Z : 0.0f);

		// Inertia tensor has units of mass.distance^2, computed from mass integral r^2.dm,
		// therefore, to rescale it by distance, we need to apply the squared scale.
		// (and inverse squared scale, as we're scaling the inverse inertia tensor)
		const FVector	invScaledInertiaTensor = invInertiaTensor * (scaleUEToPkRcp * scaleUEToPkRcp);
		state.m_InvInertiaTensor.StrippedXAxis().x() = invScaledInertiaTensor.X;
		state.m_InvInertiaTensor.StrippedYAxis().y() = invScaledInertiaTensor.Y;
		state.m_InvInertiaTensor.StrippedZAxis().z() = invScaledInertiaTensor.Z;
#endif

	}
}

//----------------------------------------------------------------------------

void	CParticleScene::AddGlobalImpulses(
	const TMemoryView<void * const> &contactObjects,
	const TStridedMemoryView<const CFloat3> &globalImpulses,
	const TStridedMemoryView<const CFloat3> &globalImpactLocations)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::AddGlobalImpulses", POPCORNFX_UE_PROFILER_COLOR);

	//Super::AddGlobalImpulses(contactObjects, globalImpulses, globalImpactLocations);
	//return;

	//HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaytraceLock);

	HH_ASSERT(contactObjects.Count() == globalImpulses.Count());
	HH_ASSERT(contactObjects.Count() == globalImpactLocations.Count());

	if (!HH_VERIFY(contactObjects.Count() > 0))
		return;

	const float		scalePkToUE = FPopcornFXPlugin::GlobalScale();
	const hh_u32	impulseCount = globalImpulses.Count();

	hh_u32					lastStart = 0;
	UPrimitiveComponent		*lastComponent = reinterpret_cast<UPrimitiveComponent*>(contactObjects[0]);

	for (hh_u32 iImpulse = 0; iImpulse < impulseCount; ++iImpulse)
	{
		UPrimitiveComponent		*component = reinterpret_cast<UPrimitiveComponent*>(contactObjects[iImpulse]);
		if (component == lastComponent)
			continue;

		const hh_u32			processStart = lastStart;
		const hh_u32			processEnd = iImpulse;
		UPrimitiveComponent		*processComponent = lastComponent;
		lastComponent = component;
		lastStart = iImpulse;

		if (processComponent != null &&
			processComponent->Mobility == EComponentMobility::Movable &&
			processComponent->IsSimulatingPhysics())
		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_ImpulseLock);
			for (hh_u32 i = processStart; i < processEnd; ++i)
				processComponent->AddImpulseAtLocation(ToUE(globalImpulses[i] * scalePkToUE), ToUE(globalImpactLocations[i] * scalePkToUE));
		}
	}
	if (lastComponent != null)
	{
		const hh_u32			processStart = lastStart;
		const hh_u32			processEnd = impulseCount;
		UPrimitiveComponent		*processComponent = lastComponent;
		if (processComponent != null &&
			processComponent->Mobility == EComponentMobility::Movable &&
			processComponent->IsSimulatingPhysics())
		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_ImpulseLock);
			for (hh_u32 i = processStart; i < processEnd; ++i)
				processComponent->AddImpulseAtLocation(ToUE(globalImpulses[i] * scalePkToUE), ToUE(globalImpactLocations[i] * scalePkToUE));
		}
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::InstantiatePostCollideAction(
	void *contactObject,
	void *contactSurface,
	const PopcornFX::CActionFactory *collideTrigger,
	PopcornFX::CActionExecutor *whereToRun,
	const PopcornFX::PActionParameters &parameters,
	PopcornFX::Threads::SThreadContext &threadCtx,
	float timeSinceEndOfFrame,
	float hitFractionInFrame,
	bool isSticky)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::InstantiatePostCollideAction", POPCORNFX_UE_PROFILER_COLOR);

	//Super::InstantiatePostCollideAction(contactObject, contactSurface, collideTrigger, whereToRun, parameters, timeSinceEndOfFrame, hitFractionInFrame, isSticky);
	//return;

	//HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaytraceLock);

	UPrimitiveComponent		*primitive = null;
	bool					customSpawn = false;
	if (isSticky)
	{
		primitive = reinterpret_cast<UPrimitiveComponent*>(contactObject);
		if (HH_VERIFY(primitive != null) &&
			primitive->Mobility == EComponentMobility::Movable)
			customSpawn = true;
	}
	if (!customSpawn)
	{
		Super::InstantiatePostCollideAction(contactObject, contactSurface, collideTrigger, whereToRun, parameters, threadCtx, timeSinceEndOfFrame, hitFractionInFrame, false);
		return;
	}

	PopcornFX::PActionParameters	actionParams = PopcornFX::CActionParameters::Alloc();
	if (!HH_VERIFY(actionParams != null))
		return;
	if (!HH_VERIFY(actionParams->DeepCopy(*parameters)))
		return;

	CFloat4x4	localToWorld = ToPk(primitive->GetComponentTransform().ToMatrixWithScale());
	//CFloat4x4	inverse = ToPk(primitive->ComponentToWorld.Inverse().ToMatrixWithScale());
	CFloat4x4	inverse = localToWorld.Inverse();
	CFloat4x4	localTransforms = CFloat4x4::IDENTITY;
	const float	scaleUEToPk = FPopcornFXPlugin::GlobalScaleRcp();

	inverse.StrippedTranslations() *= scaleUEToPk;
	localToWorld.StrippedTranslations() *= scaleUEToPk;
	const PopcornFX::CGuid	transformsId = actionParams->FindParameterId(TransformsID());
	if (transformsId.Valid())
	{
		CFloat4x4 const	*pTransforms = actionParams->ParameterFromId<CFloat4x4>(transformsId);
		if (HH_VERIFY(pTransforms != null))
			localTransforms = *pTransforms * inverse;
	}
	else
	{
		const PopcornFX::CGuid	positionsId = actionParams->FindParameterId(PositionsID());
		if (positionsId.Valid())
		{
			CFloat3 const	*pPositions = actionParams->ParameterFromId<CFloat3>(positionsId);
			if (HH_VERIFY(pPositions != null))
				localTransforms.StrippedTranslations() = inverse.TransformVector(*pPositions);
		}
	}

	// This action transforms will be updated by the scene on the game thread
	// Otherwise we would have to create a scene component and attach it to the contact object
	// Which wouldn't work in 'Editor' view : same issue as with decal spawn -> makes the package dirty.
	// And we can't either create a UPopcornFXEmitterComponent

	SSpawnedActionInstance		preActionInstance;
	preActionInstance.m_Valid = true;
	preActionInstance.m_LocalTransforms = localTransforms;
	preActionInstance.m_CurrentWorldTransforms = localTransforms * localToWorld;
	preActionInstance.m_PreviousWorldTransforms = preActionInstance.m_CurrentWorldTransforms;
	preActionInstance.m_CurrentWorldVelocity = ToPk(primitive->GetPhysicsLinearVelocity() * scaleUEToPk);
	preActionInstance.m_PreviousWorldVelocity = preActionInstance.m_CurrentWorldVelocity;
	preActionInstance.m_Primitive = primitive;

	PopcornFX::CGuid			instanceId;
	SSpawnedActionInstance		*actionInstance = null;
	{
		// avoid locking across InstantiateAndRun: can be really heavy
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_SpawnedActionInstancesLock);
		instanceId = m_SpawnedActionInstances.Insert(preActionInstance);
		if (!HH_VERIFY(instanceId.Valid()))
			return;
		actionInstance = &(m_SpawnedActionInstances[instanceId]);
	}

	HH_ASSERT(actionInstance->Valid());

	PopcornFX::SSpawnTransformsPack	sptp;
	sptp.m_WorldTr_Current = &actionInstance->m_CurrentWorldTransforms;
	sptp.m_WorldTr_Previous = &actionInstance->m_PreviousWorldTransforms;
	sptp.m_WorldVel_Current = &actionInstance->m_CurrentWorldVelocity;
	sptp.m_WorldVel_Previous = &actionInstance->m_PreviousWorldVelocity;
	actionParams->AddParameter(sptp, SpawnTransformsID());

	PopcornFX::PActionInstance	instance = collideTrigger->InstantiateAndRun(whereToRun, actionParams, timeSinceEndOfFrame, &threadCtx);
	if (instance == null || instance->Dead())
	{
		// no need to update anything, the instance is already dead
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_SpawnedActionInstancesLock);
		m_SpawnedActionInstances.Remove(instanceId);
	}
	else
	{
		actionInstance->m_Instance = instance;
		instance->m_DeathNotifier += PopcornFX::FastDelegate<void(const PopcornFX::PActionInstance &)>(actionInstance, &SSpawnedActionInstance::_OnDeathNotifier);
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::SpawnedActionInstances_PreUpdate(float deltaTime)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::SpawnedActionInstances_PreUpdate", POPCORNFX_UE_PROFILER_COLOR);
	if (m_SpawnedActionInstances.UsedCount() == 0)
		return;

	const float	scaleUEToPk = FPopcornFXPlugin::GlobalScaleRcp();
	for (hh_u32 iInstance = 0; iInstance < m_SpawnedActionInstances.Count(); ++iInstance)
	{
		SSpawnedActionInstance	&instance = m_SpawnedActionInstances[iInstance];
		if (!instance.Valid())
			continue;
		if (instance.m_Instance == null || instance.m_Instance->Dead() || !instance.m_Primitive.IsValid())
			m_SpawnedActionInstances.Remove(iInstance--);
		else
		{
			CFloat4x4	localToWorld = ToPk(instance.m_Primitive->GetComponentTransform().ToMatrixWithScale());

			localToWorld.StrippedTranslations() *= scaleUEToPk;
			instance.m_PreviousWorldTransforms = instance.m_CurrentWorldTransforms;
			instance.m_CurrentWorldTransforms = instance.m_LocalTransforms * localToWorld;
			instance.m_PreviousWorldVelocity = instance.m_CurrentWorldVelocity;
			instance.m_CurrentWorldVelocity = (instance.m_CurrentWorldTransforms.StrippedTranslations() - instance.m_PreviousWorldTransforms.StrippedTranslations()) / deltaTime;
		}
	}
}

//----------------------------------------------------------------------------
//
//
//
// Decals
//
//
//
//----------------------------------------------------------------------------

DECLARE_CYCLE_STAT_EXTERN(TEXT("PostUpdate_Decals"), STAT_PopcornFX_PostUpdate_Decals, STATGROUP_PopcornFX, );
DEFINE_STAT(STAT_PopcornFX_PostUpdate_Decals);

//----------------------------------------------------------------------------

void	CParticleScene::_PostUpdate_Decals()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_PostUpdate_Decals", POPCORNFX_UE_PROFILER_COLOR);
	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_PostUpdate_Decals);
	const UWorld	*world = m_SceneComponent->GetWorld();
	if (!HH_VERIFY(world != null))
		return;

	if (!HH_VERIFY(m_SceneComponent != null))
		return;

	const float			currentDate = world->GetTimeSeconds();

	// Destroy dead m_PendingDecals
	for (hh_u32 iDecal = 0; iDecal < m_PendingDecals.Count(); ++iDecal)
	{
		SPendingDecal		&decal = m_PendingDecals[iDecal];
		if (decal.m_KillDate > currentDate)
			continue;
		if (decal.m_Component.IsValid())
			decal.m_Component->DestroyComponent();
		m_PendingDecals.Remove(iDecal--);
	}

	const float			decalMaxLifeTime = m_SceneComponent->ResolvedSimulationSettings().DecalsMaxLifeSpan;

	const PopcornFX::CActionFactoryDecalSpawner	*currentFactory = null;
	UMaterialInstanceConstant	*defaultMaterial = null;
	UPopcornFXRendererMaterial	*rendererMat = null;

	const hh_u32		decalCount = m_DecalSpawnRequests.Count();
	hh_u32				decali = 0;
	while (decali < decalCount)
	{
		if (currentFactory != m_DecalSpawnRequests[decali].m_ActionFactory)
		{
			currentFactory = m_DecalSpawnRequests[decali].m_ActionFactory;
			rendererMat = UPopcornFXRendererMaterial::FindMe(currentFactory);

			if (rendererMat != null)
				defaultMaterial = rendererMat->GetInstance(0, false);
			else
				defaultMaterial = null;
		}

		if (defaultMaterial == null)
		{
			++decali;
			continue;
		}

		const bool	isGameWorld = world->IsGameWorld();
		while (decali < decalCount && currentFactory == m_DecalSpawnRequests[decali].m_ActionFactory)
		{
			const SDecalSpawnRequest	&decalSpawnRequest = m_DecalSpawnRequests[decali++];

			UMaterialInstance	*material = null;
			if (!decalSpawnRequest.m_HasColor)
				material = defaultMaterial;
			else
			{
				// To handle "DecalColor", we know have to create a dynamic material instance per particle.
				UMaterialInstanceDynamic	*mid = rendererMat->CreateInstance(0);
				if (mid == null)
					continue;
				mid->SetVectorParameterValue(FPopcornFXPlugin::Name_DecalColor(), decalSpawnRequest.m_LinearColor);
				material = mid;
			}

			HH_ASSERT(material != null);

			// Specify 0 as lifespan because we handle ourselves decals life (otherwise UE adds timers -> overkill)
			UDecalComponent	*decalComponent = UGameplayStatics::SpawnDecalAtLocation(m_SceneComponent->GetOwner(), material, decalSpawnRequest.m_Size, decalSpawnRequest.m_Pos, decalSpawnRequest.m_Rotation, 0.0f);
			if (!HH_VERIFY(decalComponent != null))
				continue;
			// Try to attach to the component
			USceneComponent		*component = decalSpawnRequest.m_Component;
			if (isGameWorld && component != null)
				decalComponent->AttachToComponent(component, FAttachmentTransformRules::KeepWorldTransform);

			m_PendingDecals.PushBack(SPendingDecal(decalComponent, currentDate + decalMaxLifeTime));
		}
	}

	m_DecalSpawnRequests.Clear();
}

//----------------------------------------------------------------------------

void	CParticleScene::_Clear_Decals()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_Clear_Decals", POPCORNFX_UE_PROFILER_COLOR);
	m_DecalSpawnRequests.Clear();
	for (hh_u32 i = 0; i < m_PendingDecals.Count(); ++i)
	{
		SPendingDecal		&decal = m_PendingDecals[i];
		if (decal.m_Component.IsValid())
			decal.m_Component->DestroyComponent();
	}
	m_PendingDecals.Clear();
}

//----------------------------------------------------------------------------

void	CParticleScene::SpawnDecals(
	const SDecalDescriptor &descriptor,
	const PopcornFX::TMemoryView<void * const> &contactObjects,
	const PopcornFX::TMemoryView<void * const> &contactSurfaces,
	const PopcornFX::TMemoryView<const CFloat4> &contactPositions,
	const PopcornFX::TMemoryView<const CFloat4> &contactNormals,
	const PopcornFX::TMemoryView<const float> &sizes,
	const PopcornFX::TMemoryView<const CFloat4> &colors)
{

	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::SpawnDecals", POPCORNFX_UE_PROFILER_COLOR);
	if (!HH_VERIFY(m_SceneComponent != null))
		return;
	hh_u32	decalsCount = contactObjects.Count();

	const float			scalePkToUE = FPopcornFXPlugin::GlobalScale();

	for (hh_u32 iDecal = 0; iDecal < decalsCount; ++iDecal)
	{
		UPrimitiveComponent	*component = reinterpret_cast<UPrimitiveComponent*>(contactObjects[iDecal]);
		if (component == null)
			continue;

		const bool		hasColors = !colors.Empty();
		const CFloat4	color = hasColors ? colors[iDecal] : CFloat4::ONE;
		const FVector	normal = -ToUE(contactNormals[iDecal].xyz());

		SDecalSpawnRequest		decalSpawnRequest;
		decalSpawnRequest.m_ActionFactory = descriptor.m_ActionFactory;
		decalSpawnRequest.m_Pos = ToUE(contactPositions[iDecal].xyz()) * scalePkToUE;
		decalSpawnRequest.m_Size = ToUE(sizes[iDecal] * descriptor.m_BoxScale_A16 + descriptor.m_BoxConstant_A16.xyz()) * scalePkToUE;
		decalSpawnRequest.m_Component = component;
		decalSpawnRequest.m_HasColor = hasColors;
		decalSpawnRequest.m_LinearColor = FLinearColor(color.x(), color.y(), color.z(), color.w());
		decalSpawnRequest.m_Rotation = normal.Rotation();

		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_DecalSpawnRequestsLock);
			HH_VERIFY(m_DecalSpawnRequests.PushBack(decalSpawnRequest).Valid());
		}
	}
}

//----------------------------------------------------------------------------
//
//
//
// Sounds
//
//
//
//----------------------------------------------------------------------------

//static
CParticleScene::SSoundInstance		CParticleScene::SSoundInstance::Invalid = CParticleScene::SSoundInstance();

//----------------------------------------------------------------------------

void	CParticleScene::_PostUpdate_Sounds()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_PostUpdate_Sounds", POPCORNFX_UE_PROFILER_COLOR);
	if (!HH_VERIFY(m_SceneComponent != null))
		return;

	if (m_FillAudioBuffers != null)
		m_FillAudioBuffers->PostUpdate();

	UWorld	*world = m_SceneComponent->GetWorld();
	if (world == null || !world->bAllowAudioPlayback)
		return;

	if (m_SoundSpawnRequests.UsedCount() == 0)
		return;

	for (hh_u32 soundIndex = 0; soundIndex < m_SoundSpawnRequests.Count(); ++soundIndex)
	{
		SSoundInstance		*soundInstance = &(m_SoundSpawnRequests[soundIndex]);
		if (!soundInstance->Valid())
			continue;

		HH_ASSERT(soundInstance->m_Index == soundIndex);
		HH_ASSERT(&(m_SoundSpawnRequests[soundInstance->m_Index]) == soundInstance);

		if (soundInstance->m_Spawned && !soundInstance->m_UpdateMe && !soundInstance->m_DestroyMe)
			continue;

		UAudioComponent		*audioComponent = soundInstance->m_AudioComponent.Get();

		if (soundInstance->m_DestroyMe)
		{
			if (audioComponent != null)
				audioComponent->DestroyComponent();
			soundInstance->m_AudioComponent = null;
			m_SoundSpawnRequests.Remove(soundIndex--);
			continue;
		}

		const bool		newSound = !soundInstance->m_Spawned;
		if (newSound)
		{
			soundInstance->m_Spawned = true; // if somthing fails, will be removed from Update > Stop
			HH_ASSERT(audioComponent == null);
			soundInstance->m_AudioComponent = null; // just in case
			const FString	soundPath = FPopcornFXPlugin::Get().BuildPathFromPkPath(soundInstance->m_Desc.m_Path, true);
			USoundBase		*sound = ::LoadObject<USoundBase>(null, *soundPath);
			if (sound == null)
			{
				UE_LOG(LogPopcornFXScene, Warning, TEXT("Couldn't play '%s', sound wasn't found"), ANSI_TO_TCHAR(soundInstance->m_Desc.m_Path));
				// dont Remove here, let the next update now the sound has stoped/failed
				continue;
			}

#if (ENGINE_MINOR_VERSION >= 14)
			{
				FAudioDevice::FCreateComponentParams		params(world);
				params.bPlay = false;
				params.bStopWhenOwnerDestroyed = true;
				audioComponent = FAudioDevice::CreateComponent(sound, params);
			}
#else
			audioComponent = FAudioDevice::CreateComponent(sound, world, null, /*bPlay:*/false, /*bStopWhenOwnerDestroyed*/true);
#endif
			if (!HH_VERIFY(audioComponent != null))
			{
				// dont Remove here, let the next update know the sound has stoped/failed
				continue;
			}
			audioComponent->bAutoDestroy = true;
			soundInstance->m_AudioComponent = audioComponent;
		}

		if (audioComponent == null)
		{
			soundInstance->m_Spawned = true; // if somthing fails, will be removed from Update > Stop
			soundInstance->m_AudioComponent = null;
			// dont Remove here, let the next update know the sound has stoped/failed
			continue;
		}

		const CFloat3	worldPosition = soundInstance->m_Desc.m_WorldPosition;

		HH_ASSERT(worldPosition.IsFinite()); // This shouldn't happen at spawn

		// Must run on GameThread (at least since 4.13)
		audioComponent->SetWorldLocation(ToUE(worldPosition));
		audioComponent->SetVolumeMultiplier(soundInstance->m_Desc.m_Volume);
		soundInstance->m_UpdateMe = false;

		if (newSound)
			audioComponent->Play(soundInstance->m_Desc.m_StartTimeOffsetInSeconds);

		// @TODO m_PlayTimeInSeconds
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::_Clear_Sounds()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_Clear_Sounds", POPCORNFX_UE_PROFILER_COLOR);

	for (hh_u32 soundIndex = 0; soundIndex < m_SoundSpawnRequests.Count(); ++soundIndex)
	{
		SSoundInstance		*soundInstance = &(m_SoundSpawnRequests[soundIndex]);
		if (!soundInstance->Valid())
			continue;
		HH_ASSERT(soundInstance->m_Index == soundIndex);
		HH_ASSERT(&(m_SoundSpawnRequests[soundInstance->m_Index]) == soundInstance);
		if (soundInstance->m_AudioComponent.IsValid())
			soundInstance->m_AudioComponent->DestroyComponent();
		soundInstance->m_AudioComponent = null;
	}
	m_SoundSpawnRequests.Clear();
}

//----------------------------------------------------------------------------

void	*CParticleScene::StartSound(const SSoundDescriptor &descriptor)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::StartSound", POPCORNFX_UE_PROFILER_COLOR);
	SSoundInstance	*soundInstance = null;
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_SoundsCriticalSection);
		PopcornFX::CGuid		soundi = m_SoundSpawnRequests.Insert(SSoundInstance());
		if (!HH_VERIFY(soundi.Valid()))
			return null;
		m_SoundSpawnRequests[soundi].m_Index = soundi; // make it valid while locked
		HH_ASSERT(m_SoundSpawnRequests[soundi].Valid());

		soundInstance = &(m_SoundSpawnRequests[soundi]);
	}

	soundInstance->m_Spawned = false;
	soundInstance->m_DestroyMe = false;
	soundInstance->m_UpdateMe = false;
	soundInstance->m_AudioComponent = null;

	const float		scalePkToUE = FPopcornFXPlugin::GlobalScale();

	soundInstance->m_Desc = descriptor;
	soundInstance->m_Desc.m_WorldPosition *= scalePkToUE;
	if (!HH_VERIFY(soundInstance->m_Desc.m_WorldPosition.IsFinite()))
		soundInstance->m_Desc.m_WorldPosition = CFloat3::ZERO;

	return soundInstance;
}

//----------------------------------------------------------------------------

bool	CParticleScene::UpdateSound(void *sound, const CFloat3 &worldPosition, float volume)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::UpdateSound", POPCORNFX_UE_PROFILER_COLOR);
	HH_ASSERT(sound != null);

	SSoundInstance		*soundInstance = (SSoundInstance*)sound;

	HH_ONLY_IF_ASSERTS({
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_SoundsCriticalSection);
		HH_ASSERT(&(m_SoundSpawnRequests[soundInstance->m_Index]) == soundInstance);
	});

	bool			updateMe = false;

	const float		scalePkToUE = FPopcornFXPlugin::GlobalScale();
	CFloat3			newWorldLocation = worldPosition * scalePkToUE;

	if (!HH_VERIFY(newWorldLocation.IsFinite()))
		newWorldLocation = CFloat3::ZERO;

	if (soundInstance->m_Desc.m_WorldPosition != newWorldLocation)
	{
		soundInstance->m_Desc.m_WorldPosition = newWorldLocation;
		updateMe = true;
	}
	if (soundInstance->m_Desc.m_Volume != volume)
	{
		soundInstance->m_Desc.m_Volume = volume;
		updateMe = true;
	}

	if (!soundInstance->m_Spawned) // not spawned yet, wait for it
		return true;

	if (updateMe)
		soundInstance->m_UpdateMe = true;

	UAudioComponent		*audioComponent = soundInstance->m_AudioComponent.Get();

	// IsPlaying should be thread safe (4.13)
	if (audioComponent == null || !audioComponent->IsPlaying())
		return false; // will StopSound()

	return true;
}

//----------------------------------------------------------------------------

void	CParticleScene::StopSound(void *sound)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::StopSound", POPCORNFX_UE_PROFILER_COLOR);
	HH_ASSERT(sound != null);

	SSoundInstance	*soundInstance = (SSoundInstance*)sound;

	HH_ONLY_IF_ASSERTS({
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_SoundsCriticalSection);
		HH_ASSERT(&(m_SoundSpawnRequests[soundInstance->m_Index]) == soundInstance);
	});

	soundInstance->m_Spawned = false;
	soundInstance->m_DestroyMe = true;
}

//----------------------------------------------------------------------------

TMemoryView<const float * const>	CParticleScene::GetAudioSpectrum(PopcornFX::CStringId channelGroup, hh_u32 &outBaseCount) const
{
	if (channelGroup.Empty() || m_FillAudioBuffers == null)
		return TMemoryView<const float * const>();

	const FName			channelName(ANSI_TO_TCHAR(channelGroup.ToString().Data()));
	const float * const	*rawSpectrumData = m_FillAudioBuffers->AsyncGetAudioSpectrum(channelName, outBaseCount);
	if (rawSpectrumData == null || outBaseCount == 0)
		return TMemoryView<const float * const>();

	const hh_u32	pyramidSize = PopcornFX::IntegerTools::Log2(outBaseCount) + 1;
	return TMemoryView<const float * const>(rawSpectrumData, pyramidSize);
}

//----------------------------------------------------------------------------

TMemoryView<const float * const>	CParticleScene::GetAudioWaveform(PopcornFX::CStringId channelGroup, hh_u32 &outBaseCount) const
{
	if (channelGroup.Empty() || m_FillAudioBuffers == null)
		return TMemoryView<const float * const>();

	const FName			channelName(ANSI_TO_TCHAR(channelGroup.ToString().Data()));
	const float * const	*rawWaveformData = m_FillAudioBuffers->AsyncGetAudioWaveform(channelName, outBaseCount);
	if (rawWaveformData == null || outBaseCount == 0)
		return TMemoryView<const float * const>();

	const hh_u32	pyramidSize = PopcornFX::IntegerTools::Log2(outBaseCount) + 1;
	return TMemoryView<const float * const>(rawWaveformData, pyramidSize);
}

//----------------------------------------------------------------------------
//
//
//
// GPU stuff
//
//
//
//----------------------------------------------------------------------------

#if (PK_HAS_GPU == 1)

//----------------------------------------------------------------------------

bool	CParticleScene::GPU_InitIFN()
{
	bool		ok = false;
	if (false) { }
#if (PK_GPU_D3D11 == 1)
	else if (m_EnableD3D11)
		ok = D3D11_InitIFN();
#endif
	return ok;
}

//----------------------------------------------------------------------------

void	CParticleScene::GPU_Destroy()
{
#if (PK_GPU_D3D11 == 1)
	if (m_EnableD3D11)
		D3D11_Destroy();
#endif
}

//----------------------------------------------------------------------------

void	CParticleScene::GPU_PreRender()
{
	if (false) {}
#if (PK_GPU_D3D11 == 1)
	else if (D3D11Ready())
		D3D11_PreRender();
#endif
}

//----------------------------------------------------------------------------

void	CParticleScene::GPU_PreUpdate()
{
	if (false) {}
#if (PK_GPU_D3D11 == 1)
	else if (D3D11Ready())
		D3D11_PreUpdate();
#endif
}

//----------------------------------------------------------------------------

void	CParticleScene::GPU_PreUpdateFence()
{
	if (false) {}
#if (PK_GPU_D3D11 == 1)
	if (D3D11Ready())
		D3D11_PreUpdateFence();
#endif
}

//----------------------------------------------------------------------------

void	CParticleScene::GPU_PostUpdate()
{
	if (false) {}
#if (PK_GPU_D3D11 == 1)
	if (D3D11Ready())
		D3D11_PostUpdate();
#endif
}

//----------------------------------------------------------------------------

#endif // (PK_HAS_GPU == 1)

//----------------------------------------------------------------------------
//
// D3D11
//
//----------------------------------------------------------------------------
#if (PK_GPU_D3D11 == 1)

namespace
{

	hh_u32	D3D11_GetRefCount(IUnknown *o)
	{
		const hh_u32		r = o->AddRef();
		HH_ASSERT(r > 0);
		o->Release();
		return r - 1;
	}

	PopcornFX::CParticleUpdateManager_D3D11		*GetUpdateManagerD3D11_IFP(PopcornFX::CParticleUpdateManager *abstractUpdateManager)
	{
		if (abstractUpdateManager->UpdateClass() == PopcornFX::CParticleUpdateManager_D3D11::DefaultUpdateClass())
		{
			return PopcornFX::checked_cast<PopcornFX::CParticleUpdateManager_D3D11*>(abstractUpdateManager);
		}
		else if (abstractUpdateManager->UpdateClass() == PopcornFX::CParticleUpdateManager_Auto::DefaultUpdateClass())
		{
			PopcornFX::CParticleUpdateManager_Auto	*autoUpdateManager = static_cast<PopcornFX::CParticleUpdateManager_Auto*>(abstractUpdateManager);
			return autoUpdateManager->GetUpdater_D3D11();
		}
		return null;
	}

	struct SFetchD3D11Context
	{
		bool							volatile m_Finished = false;
		struct ID3D11Device				* volatile m_Device = null;
		struct ID3D11DeviceContext		* volatile m_DeferedContext = null;

		void		Fetch()
		{
			m_Device = (ID3D11Device*)RHIGetNativeDevice();
			if (!HH_VERIFY(m_Device != null))
			{
				m_Finished = true;
				return;
			}
			ID3D11DeviceContext		*deferedContext = null;
			HRESULT hr;
			hr = m_Device->CreateDeferredContext(0, &deferedContext);
			if (!HH_VERIFY(!FAILED(hr)))
			{
				m_Finished = true;
				return;
			}
			HH_ASSERT(deferedContext != null);
			m_DeferedContext = deferedContext;
			m_Finished = true;
		}
	};
} // namespace

//----------------------------------------------------------------------------

bool	CParticleScene::D3D11_InitIFN()
{
	HH_ASSERT(m_EnableD3D11);

	if (D3D11Ready())
		return true;

	HH_ASSERT(m_D3D11_Device == null && m_D3D11_DeferedContext == null);

	PopcornFX::CParticleUpdateManager_D3D11			*updateManager_D3D11 = GetUpdateManagerD3D11_IFP(m_ParticleMediumCollection->UpdateManager());
	if (!HH_VERIFY(updateManager_D3D11 != null))
	{
		m_EnableD3D11 = false;
		return false;
	}
	HH_ASSERT(updateManager_D3D11->GPU_CommandQueues().Empty());

	SFetchD3D11Context		*fetch = new SFetchD3D11Context;

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		PopcornFXFetchDeferredContext,
		SFetchD3D11Context*, fetch, fetch,
		{
			fetch->Fetch();
		});

	FlushRenderingCommands();

	if (!HH_RELEASE_VERIFY(fetch->m_Finished))
	{
		m_EnableD3D11 = false;
		delete fetch;
		return false;
	}

	if (!HH_VERIFY(fetch->m_Device != null && fetch->m_DeferedContext != null))
	{
		m_EnableD3D11 = false;
		delete fetch;
		return false;
	}

	//m_D3D11_Device = const_cast<struct ID3D11Device*>(fetch->m_Device);
	//m_D3D11_DeferedContext = const_cast<struct ID3D11DeviceContext*>(fetch->m_DeferedContext);
	m_D3D11_Device = fetch->m_Device;
	m_D3D11_DeferedContext = fetch->m_DeferedContext;

	delete fetch;

	HH_ASSERT(D3D11_GetRefCount(m_D3D11_DeferedContext) == 1);

	//updateManager_D3D11->BindD3D11(m_D3D11_Device, TStridedMemoryView<ID3D11DeviceContext * const>(m_D3D11_DeferedContext));
	updateManager_D3D11->BindD3D11(
		m_D3D11_Device,
		PopcornFX::CParticleUpdateManager_D3D11::CbEnqueueImmediateTask(this, &CParticleScene::D3D11_EnqueueImmediateTask)
	);

	updateManager_D3D11->BindD3D11CommandQueues(
		TStridedMemoryView<ID3D11DeviceContext * const>(&m_D3D11_DeferedContext, 1)
	);

	return true;
}

//----------------------------------------------------------------------------

PopcornFX::TAtomic<hh_u32>				g_D3D11_PendingTasks = 0;

#define PK_IMMEDIATE_TASK_ARRAY		1

//----------------------------------------------------------------------------

#if PK_IMMEDIATE_TASK_ARRAY
static void		_D3D11_ExecuteImmTasksArray(CParticleScene *self)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_D3D11_ExecuteImmTasksArray", POPCORNFX_UE_PROFILER_COLOR);

	auto			&m_D3D11_Tasks_Lock = self->m_D3D11_Tasks_Lock;
	auto			&m_Exec_D3D11_Tasks = self->m_Exec_D3D11_Tasks;
	auto			&m_UpdateLock = self->m_UpdateLock;

	ID3D11Device			*device = (ID3D11Device*)RHIGetNativeDevice();
	if (!HH_RELEASE_VERIFY(device != null))
		return;
	ID3D11DeviceContext		*immContext = null;
	device->GetImmediateContext(&immContext);
	if (!HH_RELEASE_VERIFY(immContext != null))
		return;

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_UpdateLock);
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_D3D11_Tasks_Lock);
		MYGPUMARKER("PopcornFX Update Task");
		for (hh_u32 i = 0; i < m_Exec_D3D11_Tasks.Count(); ++i)
		{
			m_Exec_D3D11_Tasks[i]->Execute(immContext);
		}
		m_Exec_D3D11_Tasks.Clear();
	}

	SAFE_RELEASE(immContext); // GetImmediateContext AddRefs
}
#else
static void		_D3D11_ExecuteImmediateTask(CParticleScene *unsafeScene, const PopcornFX::PParticleUpdaterImmediateTaskD3D11 &task)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_D3D11_ExecuteImmediateTask", POPCORNFX_UE_PROFILER_COLOR);

	--g_D3D11_PendingTasks;

	ENABLE_IF_GORE_LOCK(HH_CRITICAL_SECTION_SCOPEDLOCK(unsafeScene->m_GoreLock));

	ID3D11Device			*device = (ID3D11Device*)RHIGetNativeDevice();
	if (!HH_RELEASE_VERIFY(device != null))
		return;
	ID3D11DeviceContext		*immContext = null;
	device->GetImmediateContext(&immContext);
	if (!HH_RELEASE_VERIFY(immContext != null))
		return;

	{
		MYGPUMARKER("PopcornFX Update Task");
		task->Execute(immContext);
	}

	SAFE_RELEASE(immContext); // GetImmediateContext AddRefs
}
#endif

//----------------------------------------------------------------------------

void	CParticleScene::D3D11_EnqueueImmediateTask(const PopcornFX::PParticleUpdaterImmediateTaskD3D11 &task)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::D3D11_EnqueueImmediateTask", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(m_EnableD3D11);
	if (!HH_RELEASE_VERIFY(D3D11Ready()))
		return;

	HH_RELEASE_ASSERT(task != null);

#if PK_IMMEDIATE_TASK_ARRAY

	m_CurrentUpdate_D3D11_Tasks.PushBack(task);

#else

	++g_D3D11_PendingTasks;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		PopcornFX_D3D11_EnqueueImmediateTask,
		CParticleScene*, self, this,
		PopcornFX::PParticleUpdaterImmediateTaskD3D11, task, task,
		{
			_D3D11_ExecuteImmediateTask(self, task);
		});

	FlushRenderingCommands();
	HH_ASSERT(g_D3D11_PendingTasks == 0);
	//HH_ASSERT(task->Done());

#endif

}

//----------------------------------------------------------------------------

void	CParticleScene::D3D11_Destroy() // GPU_Destroy()
{
	if (m_ParticleMediumCollection != null)
	{
		PopcornFX::CParticleUpdateManager_D3D11		*updateManager_D3D11 = GetUpdateManagerD3D11_IFP(m_ParticleMediumCollection->UpdateManager());
		if (updateManager_D3D11 != null &&
			!updateManager_D3D11->GPU_CommandQueues().Empty())
		{
			updateManager_D3D11->BindD3D11(null, null);
		}
	}
	if (m_D3D11_DeferedContext != null)
	{
		HH_ASSERT(D3D11_GetRefCount(m_D3D11_DeferedContext) == 1);
		SAFE_RELEASE(m_D3D11_DeferedContext);
		m_D3D11_DeferedContext = null;
	}
	m_D3D11_Device = null;

#if PK_IMMEDIATE_TASK_ARRAY
	HH_CRITICAL_SECTION_SCOPEDLOCK(m_D3D11_Tasks_Lock);
	// @FIXME: if this is not for MediumCollection Clear or Destroy, there will be bugs
	m_Exec_D3D11_Tasks.Clear();
	m_CurrentUpdate_D3D11_Tasks.Clear();
#endif

}

//----------------------------------------------------------------------------

DECLARE_CYCLE_STAT_EXTERN(TEXT("GPU PreUpdate FlushRenderingCommands"), STAT_PopcornFX_GPU_PreUpdate_Flush, STATGROUP_PopcornFX, );
DEFINE_STAT(STAT_PopcornFX_GPU_PreUpdate_Flush);

void	CParticleScene::D3D11_PreUpdate() // GPU_PreUpdate()
{
	HH_ASSERT(D3D11Ready());

#if PK_IMMEDIATE_TASK_ARRAY

	hh_u32		pendingTasksCount = m_Exec_D3D11_Tasks.Count();
	if (pendingTasksCount > 0)
	{
		// make sure with a lock
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_D3D11_Tasks_Lock);
		pendingTasksCount = m_Exec_D3D11_Tasks.Count();
	}

	// We NEED to wait tasks or the Runtime will FREEZE !

	if (pendingTasksCount > 0)
	{
		UE_LOG(LogPopcornFXScene, Warning, TEXT("FlushRenderingCommands for last frame pending GPU immediate tasks (%d tasks) ... "), pendingTasksCount);

		{
			SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GPU_PreUpdate_Flush);
			FlushRenderingCommands();
		}

		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_D3D11_Tasks_Lock);
			HH_RELEASE_ASSERT(m_Exec_D3D11_Tasks.Empty());
		}
	}

#endif
	return;
}

//----------------------------------------------------------------------------

void	CParticleScene::D3D11_PreUpdateFence() // GPU_PreUpdateFence()
{
	HH_ASSERT(D3D11Ready());
}

//----------------------------------------------------------------------------

void	CParticleScene::D3D11_PostUpdate() // GPU_PostUpdate()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::D3D11_PostUpdate", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(D3D11Ready());

#if PK_IMMEDIATE_TASK_ARRAY

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_D3D11_Tasks_Lock);

		m_Exec_D3D11_Tasks.Reserve(m_Exec_D3D11_Tasks.Count() + m_CurrentUpdate_D3D11_Tasks.Count());

		for (hh_u32 i = 0; i < m_CurrentUpdate_D3D11_Tasks.Count(); ++i)
			m_Exec_D3D11_Tasks.PushBack(m_CurrentUpdate_D3D11_Tasks[i]);

		m_CurrentUpdate_D3D11_Tasks.Clear();
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		PopcornFX_D3D11_EnqueueImmediateTasksArray,
		CParticleScene*, self, this,
		{
			_D3D11_ExecuteImmTasksArray(self);
		});

	//FlushRenderingCommands();

#else
#endif
}

//----------------------------------------------------------------------------

DECLARE_CYCLE_STAT_EXTERN(TEXT("GPU PreRender Execute"), STAT_PopcornFX_GPU_PreRender_Exec, STATGROUP_PopcornFX, );
DEFINE_STAT(STAT_PopcornFX_GPU_PreRender_Exec);

void	CParticleScene::D3D11_PreRender() // GPU_PreRender()
{
	HH_ASSERT(D3D11Ready());
#if PK_IMMEDIATE_TASK_ARRAY

#	if 1
	hh_u32		pendingTasksCount = m_Exec_D3D11_Tasks.Count();
	if (pendingTasksCount > 0)
	{
		// make sure with a lock
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_D3D11_Tasks_Lock);
		pendingTasksCount = m_Exec_D3D11_Tasks.Count();
	}

	if (pendingTasksCount > 0)
	{
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_GPU_PreRender_Exec);
		_D3D11_ExecuteImmTasksArray(this);
	}
#	endif

#endif
	return;
}

//----------------------------------------------------------------------------

#endif // (PK_GPU_D3D11 == 1)

//----------------------------------------------------------------------------
//
//
//
// Events
//
//
//
//----------------------------------------------------------------------------

DECLARE_CYCLE_STAT_EXTERN(TEXT("RaiseEvent"), STAT_PopcornFX_RaiseEventTime, STATGROUP_PopcornFX, );
DEFINE_STAT(STAT_PopcornFX_RaiseEventTime);

//----------------------------------------------------------------------------

CParticleScene::SPopcornFXEventListener::SPopcornFXEventListener(const PopcornFX::CStringId &eventName)
:	m_EventName(eventName)
{

}

CParticleScene::SPopcornFXFieldListener::SPopcornFXFieldListener(const FName &fieldName, EPopcornFXParticleFieldType::Type fieldType)
:	m_FieldName(fieldName)
,	m_CachedFieldName(PopcornFX::CStringId(TCHAR_TO_ANSI(*fieldName.ToString())))
,	m_FieldType(fieldType)
{
}

//----------------------------------------------------------------------------

bool	operator==(const PopcornFX::CStringId &eventName, const CParticleScene::SPopcornFXEventListener &other)
{
	return eventName == other.m_EventName;
}

//----------------------------------------------------------------------------

bool	operator==(const FName &fieldName, const CParticleScene::SPopcornFXFieldListener &other)
{
	return fieldName == other.m_FieldName;
}

//----------------------------------------------------------------------------

void	CParticleScene::_Clear_Events()
{
	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	m_FieldListeners.Clear();
	m_PendingEventAssocs.Clear();
	m_EventListeners.Clear();
}

//----------------------------------------------------------------------------

void	CParticleScene::ClearPendingEvents_NoLock()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::ClearPendingEvents", POPCORNFX_UE_PROFILER_COLOR);
	const hh_u32	flCount = m_FieldListeners.Count();
	for (hh_u32 iFieldListener = 0; iFieldListener < flCount; ++iFieldListener)
		m_FieldListeners[iFieldListener].m_Values.Clear();
	m_PendingEventAssocs.Clear();
}

//----------------------------------------------------------------------------

void	CParticleScene::_PostUpdate_Events()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_PostUpdate_Events", POPCORNFX_UE_PROFILER_COLOR);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	m_CurrentEventIndex = 0;

	const hh_u32	elAssocCount = m_PendingEventAssocs.Count();
	for (hh_u32 iAssoc = 0; iAssoc < elAssocCount; ++iAssoc)
	{
		const SPopcornFXEventListenerAssoc	&assoc = m_PendingEventAssocs[iAssoc];
		const hh_u32	eventCount = assoc.m_RaisedCount;
		const bool		validScope = assoc.m_ValidScope;

		HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_PostUpdate_Events_TreatEventAssoc", POPCORNFX_UE_PROFILER_COLOR);
		m_RaiseEventScope = validScope;
		for (hh_u32 iEvent = 0; iEvent < eventCount; ++iEvent)
		{
			const hh_u32	delegateCount = assoc.m_Event->m_Delegates.Count();
			for (hh_u32 iDelegate = 0; iDelegate < delegateCount; ++iDelegate)
			{
				HH_NAMEDSCOPEDPROFILE_C("CParticleScene::_PostUpdate_Events_TreatSingleDelegate", POPCORNFX_UE_PROFILER_COLOR);

				// The object this delegate was bound onto have probably been destroyed
				if (!assoc.m_Event->m_Delegates[iDelegate].IsBound())
					continue;
				assoc.m_Event->m_Delegates[iDelegate].Execute();
			}
			m_CurrentEventIndex += static_cast<int32>(validScope);
		}
	}
	m_RaiseEventScope = false;
	m_CurrentEventIndex = 0;
	ClearPendingEvents_NoLock();
}

//----------------------------------------------------------------------------

void	CParticleScene::RaiseEvent(
	const SEventDescriptor &descriptor,
	const PopcornFX::CParticleMediumCollection *mediumCollection,
	const PopcornFX::CParticlePageView *pageView,
	PopcornFX::TMemoryView<const hh_u32> indices)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::RaiseEvent", POPCORNFX_UE_PROFILER_COLOR);
	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_RaiseEventTime);

	HH_ASSERT(mediumCollection != null);
	if (!HH_VERIFY(m_SceneComponent != null))
		return;
	HH_ASSERT(m_SceneComponent->GetWorld() != null);
	if (!m_SceneComponent->GetWorld()->IsGameWorld())
		return;
	if (!m_SceneComponent->ResolvedSimulationSettings().bEnableEventListeners)
		return;

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	const PopcornFX::CStringId		eventNameId = descriptor.m_EventName;
	const PopcornFX::CGuid			eventListenerIndex = m_EventListeners.IndexOf(eventNameId);
	// If an event listener was registered for this specific event
	if (!eventListenerIndex.Valid())
	{
		return;
	}

	SPopcornFXEventListenerAssoc	newAssoc(&m_EventListeners[eventListenerIndex], pageView != null);

	if (pageView == null)
	{
		newAssoc.m_RaisedCount = indices.Count();
	}
	else
	{
		PopcornFX::TArray<int32>	particleIndices;

		HH_NAMEDSCOPEDPROFILE_C("CParticleScene::RaiseEvent_FillParticleIndices", POPCORNFX_UE_PROFILER_COLOR);
		// Fill in the particle indices
		const hh_u32	indicesCount = indices.Count();
		if (indicesCount > 0)
		{
			newAssoc.m_RaisedCount = indicesCount;
			if (HH_VERIFY(particleIndices.Resize(newAssoc.m_RaisedCount)))
			{
				for (hh_u32 iParticle = 0; iParticle < newAssoc.m_RaisedCount; ++iParticle)
					particleIndices[iParticle] = indices[iParticle];
			}
		}
		else
		{
			newAssoc.m_RaisedCount = pageView->ParticleCount();
			if (HH_VERIFY(particleIndices.Resize(newAssoc.m_RaisedCount)))
			{
				for (hh_u32 iParticle = 0; iParticle < newAssoc.m_RaisedCount; ++iParticle)
					particleIndices[iParticle] = iParticle;
			}
		}

		// Now that we have the particle indices, recover the particle fields
		RetrieveParticleFields(particleIndices, pageView);
	}

	if (!HH_VERIFY(m_PendingEventAssocs.PushBack(newAssoc).Valid()))
	{
		// FAIL
	}
}

//----------------------------------------------------------------------------

template <typename _OutType>
void	CParticleScene::FillFieldListener(
	SPopcornFXFieldListener &fieldListener,
	const PopcornFX::TArray<int32> &particleIndices,
	const PopcornFX::CParticlePageView *pageView)
{
	const hh_u32	valueOffset = fieldListener.m_Values.Count();
	const hh_u32	particleCount = particleIndices.Count();
	if (!HH_VERIFY(fieldListener.m_Values.Resize(valueOffset + particleCount)))
		return;
	bool	validStream = false;
	const PopcornFX::CGuid	streamId = pageView->StreamId(fieldListener.m_CachedFieldName);
	if (streamId.Valid())
	{
		const PopcornFX::SParticleStreamField	&fieldStream = pageView->RawStream(streamId);

		validStream = PopcornFX::TBaseTypeID<_OutType>::TypeID == fieldStream.m_Definition.m_Type;
	}
	const PopcornFX::TStridedMemoryView<const _OutType>	&fields = validStream ? pageView->StreamForReading<_OutType>(pageView->StreamId(fieldListener.m_CachedFieldName)) : PopcornFX::TStridedMemoryView<const _OutType>();

	const bool	foundField = !fields.Empty();
	for (hh_u32 iParticle = 0; iParticle < particleCount; ++iParticle)
	{
		SPopcornFXFieldListenerValue	&value = fieldListener.m_Values[valueOffset + iParticle];

		value.m_FoundField = foundField;
		if (foundField)
			FGenericPlatformMemory::Memcpy(value.m_ValueInt, &fields[particleIndices[iParticle]], sizeof(_OutType));
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::RetrieveParticleFields(const PopcornFX::TArray<int32> &particleIndices, const PopcornFX::CParticlePageView *pageView)
{
	HH_ASSERT(pageView != null);

	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::RetrieveParticleFields", POPCORNFX_UE_PROFILER_COLOR);

	const hh_u32	flCount = m_FieldListeners.Count();
	for (hh_u32 iFieldListener = 0; iFieldListener < flCount; ++iFieldListener)
	{
		SPopcornFXFieldListener	&fieldListener = m_FieldListeners[iFieldListener];

		const EPopcornFXParticleFieldType::Type	fieldType = static_cast<EPopcornFXParticleFieldType::Type>(fieldListener.m_FieldType);
		switch (fieldType)
		{
			case	EPopcornFXParticleFieldType::Float:
				FillFieldListener<float>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Float2:
				FillFieldListener<PopcornFX::CFloat2>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Float3:
				FillFieldListener<PopcornFX::CFloat3>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Float4:
				FillFieldListener<PopcornFX::CFloat4>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Int:
				FillFieldListener<hh_i32>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Int2:
				FillFieldListener<PopcornFX::CInt2>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Int3:
				FillFieldListener<PopcornFX::CInt3>(fieldListener, particleIndices, pageView);
				break;
			case	EPopcornFXParticleFieldType::Int4:
				FillFieldListener<PopcornFX::CInt4>(fieldListener, particleIndices, pageView);
				break;
			default:
				HH_ASSERT_NOT_REACHED();
				break;
		}
	}
}

//----------------------------------------------------------------------------

bool	CParticleScene::RegisterEventListener(const PopcornFX::CStringId &eventName, FPopcornFXRaiseEventSignature callback)
{
	HH_ASSERT(IsInGameThread());
	if (!HH_VERIFY(m_SceneComponent != null))
		return false;
	HH_ASSERT(m_SceneComponent->GetWorld() != null);
	if (!m_SceneComponent->GetWorld()->IsGameWorld())
		return false;
	HH_ASSERT(m_SceneComponent->ResolvedSimulationSettings().bEnableEventListeners);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	PopcornFX::CGuid	eventListenerIndex = m_EventListeners.IndexOf(eventName);
	if (!eventListenerIndex.Valid())
	{
		// First listener registered for this event, register it.
		eventListenerIndex = m_EventListeners.PushBack(eventName);
	}
	if (HH_VERIFY(eventListenerIndex.Valid()))
	{
		if (!HH_VERIFY(m_EventListeners[eventListenerIndex].m_Delegates.PushBack(callback).Valid()))
		{
			// FAIL
		}
	}
	return true;
}

//----------------------------------------------------------------------------

void	CParticleScene::UnregisterEventListener(const PopcornFX::CStringId &eventName, FPopcornFXRaiseEventSignature callback)
{
	HH_ASSERT(IsInGameThread());
	HH_ASSERT(m_SceneComponent->ResolvedSimulationSettings().bEnableEventListeners || m_EventListeners.Empty());
	if (m_EventListeners.Empty())
		return;

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	const PopcornFX::CGuid	eventListenerIndex = m_EventListeners.IndexOf(eventName);
	if (eventListenerIndex.Valid())
	{
		SPopcornFXEventListener	&eventListener = m_EventListeners[eventListenerIndex];
		const hh_u32	delegateCount = eventListener.m_Delegates.Count();
		for (hh_u32 iDelegate = 0; iDelegate < delegateCount; ++iDelegate)
		{
			if (eventListener.m_Delegates[iDelegate] == callback)
			{
				eventListener.m_Delegates.Remove(iDelegate);
				ClearPendingEvents_NoLock();
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------

void	CParticleScene::RegisterFieldListeners()
{
	HH_ASSERT(IsInGameThread());
	HH_ASSERT(m_SceneComponent != null);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	const hh_u32	flCount = m_SceneComponent->FieldListeners.Num();
	for (hh_u32 iFieldListener = 0; iFieldListener < flCount; ++iFieldListener)
	{
		const FPopcornFXFieldListener	&fieldListener = m_SceneComponent->FieldListeners[iFieldListener];
		const FName						&flName = fieldListener.FieldName;
		if (!flName.IsValid() || flName.IsNone())
			continue;
		const PopcornFX::CGuid	flIndex = m_FieldListeners.IndexOf(flName);
		if (flIndex.Valid())
			continue;
		if (!HH_VERIFY(m_FieldListeners.PushBack(SPopcornFXFieldListener(flName, fieldListener.FieldType.GetValue())).Valid()))
		{
			// FAIL
		}
	}
}

//----------------------------------------------------------------------------

bool	CParticleScene::GetFieldValue(const FName &fieldName, void *outValue, EPopcornFXParticleFieldType::Type expectedFieldType) const
{
	HH_ASSERT(IsInGameThread());
	HH_ASSERT(!fieldName.IsNone() && fieldName.IsValid());
	HH_ASSERT(outValue != null);

	HH_NAMEDSCOPEDPROFILE_C("CParticleScene::GetFieldValue", POPCORNFX_UE_PROFILER_COLOR);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RaiseEventLock);

	const PopcornFX::CGuid	fieldIndex = m_FieldListeners.IndexOf(fieldName);
	if (!m_RaiseEventScope)
	{
		UE_LOG(LogPopcornFXScene, Warning, TEXT("Get Particle Field: Invalid scope"));
		return false;
	}
	if (!fieldIndex.Valid())
	{
		UE_LOG(LogPopcornFXScene, Warning, TEXT("Get Particle Field: Cannot retrieve particle field '%s', no field listener registered."), *fieldName.ToString());
		return false;
	}
	const SPopcornFXFieldListener	&fieldListener = m_FieldListeners[fieldIndex];
	if (fieldListener.m_FieldType != expectedFieldType)
	{
		UE_LOG(LogPopcornFXScene, Warning, TEXT("Get Particle Field: mismatching types for field '%s'"), *fieldName.ToString());
		return false;
	}
	if (!HH_VERIFY(m_CurrentEventIndex < m_FieldListeners[fieldIndex].m_Values.Count()))
	{
		UE_LOG(LogPopcornFXScene, Warning, TEXT("Get Particle Field: Couldn't find field '%s' (field doesn't exist or invalid FieldType)"), *fieldName.ToString());
		return false;
	}
	const SPopcornFXFieldListenerValue	&value = fieldListener.m_Values[m_CurrentEventIndex];
	if (!value.m_FoundField)
	{
		UE_LOG(LogPopcornFXScene, Warning, TEXT("Get Particle Field: Couldn't find field '%s' (field doesn't exist or invalid FieldType)"), *fieldName.ToString());
		return false;
	}
	const uint32	fieldDimension = EPopcornFXParticleFieldType::Dim(expectedFieldType);
	FGenericPlatformMemory::Memcpy(outValue, value.m_ValueInt, sizeof(int32) * fieldDimension);
	return true;
}

//----------------------------------------------------------------------------
