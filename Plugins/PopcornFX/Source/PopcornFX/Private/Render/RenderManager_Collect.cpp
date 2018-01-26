//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "RenderManager.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXStats.h"
#include "Render/RenderManager.h"
#include "Assets/PopcornFXRendererMaterial.h"
#include "World/PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Engine/StaticMesh.h"

#include "PopcornFXSDK.h"

DECLARE_DWORD_COUNTER_STAT(TEXT("Total Active Collected Frame"), STAT_PopcornFX_ActiveCollectedFrame, STATGROUP_PopcornFX);

//----------------------------------------------------------------------------
//
// Collected Frame Pool
//
//----------------------------------------------------------------------------

static const hh_u32			kMaxQueuedCollectedFrame = 4;

class CParticleCollectedFrameToRenderPool
{
public:
	CParticleCollectedFrameToRenderPool();
	~CParticleCollectedFrameToRenderPool();

	CParticleCollectedFrameToRender		*AquireFrame();
	void								ReleaseFrame(CParticleCollectedFrameToRender *frame);

	void								SafeReleaseFrame(CParticleCollectedFrameToRender *&frame)
	{
		if (frame != null)
		{
			ReleaseFrame(frame);
			frame = null;
		}
	}

	STAT(PopcornFX::TAtomic<hh_u32>			m_StatActiveCollectedFrame = 0;)

private:

	// (using memoryview to avoid TStaticArray access overhead, and protecting against resize etc...)
	TMemoryView<CParticleCollectedFrameToRender>			m_Pool;
	TMemoryView<PopcornFX::TAtomic<bool> >					m_SlotUsed;


	PopcornFX::TStaticArray<PopcornFX::TAtomic<bool>, kMaxQueuedCollectedFrame>			_m_SlotUsed;
	PopcornFX::TStaticArray<CParticleCollectedFrameToRender, kMaxQueuedCollectedFrame>	_m_Pool;
};

CParticleCollectedFrameToRenderPool::CParticleCollectedFrameToRenderPool()
{
	m_Pool = _m_Pool;
	m_SlotUsed = _m_SlotUsed;
	for (hh_u32 i = 0; i < m_SlotUsed.Count(); ++i)
		m_SlotUsed[i] = false;
}

CParticleCollectedFrameToRenderPool::~CParticleCollectedFrameToRenderPool()
{
	for (hh_u32 i = 0; i < m_SlotUsed.Count(); ++i)
		HH_ASSERT_MESSAGE(!m_SlotUsed[i], "You must release all frames before deleting pool");
}


CParticleCollectedFrameToRender		*CParticleCollectedFrameToRenderPool::AquireFrame()
{
	STAT(++m_StatActiveCollectedFrame;)
//	return HH_NEW(CParticleCollectedFrameToRender);
	for (hh_u32 i = 0; i < m_SlotUsed.Count(); ++i)
	{
		if (m_SlotUsed[i].SetValueIfEqualTo(true, false) == false) // returns old value
			return &(m_Pool[i]);
	}
	HH_ASSERT_MESSAGE(false, "exceeded the maximum of %d pending frame to render", kMaxQueuedCollectedFrame);
	return null;
}

void	CParticleCollectedFrameToRenderPool::ReleaseFrame(CParticleCollectedFrameToRender *frame)
{
	STAT(--m_StatActiveCollectedFrame;)
//	HH_DELETE(frame); return;
	frame->Clear();
	const hh_u32		id = (frame - m_Pool.Data());
	HH_ASSERT(id < m_SlotUsed.Count());
	HH_ASSERT(&(m_Pool[id]) == frame);
	HH_ASSERT(m_SlotUsed[id] == true);
	m_SlotUsed[id] = false;
}

//----------------------------------------------------------------------------
//
// CParticleRenderManager
//
// Game Thread Collect stuff
//
//----------------------------------------------------------------------------

void	CParticleRenderManager::Collect_Clean()
{
	if (m_FramePool == null)
		return;
	FlushRenderingCommands(); // so we can safely release m_Rendering_LockMe
	m_FramePool->SafeReleaseFrame(m_Rendering_LockMe);
	m_FramePool->SafeReleaseFrame(m_Collecting);
	HH_SAFE_DELETE(m_FramePool);
}

void	CParticleRenderManager::GameThread_PreUpdate(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::GameThread_PreUpdate", POPCORNFX_UE_PROFILER_COLOR);

	//HH_ASSERT(IsInGameThread());
	m_LastCollectedMaterialsChanged = false;
	m_CollectedUsedMaterialCount = 0;

	if (m_FramePool == null)
	{
		m_FramePool = HH_NEW(CParticleCollectedFrameToRenderPool);
		if (!HH_VERIFY(m_FramePool != null))
			return;
	}

	if (!/*HH_VERIFY*/(m_Collecting == null)) // happens when SendRenderDynamicData has not been called
	{
		m_FramePool->SafeReleaseFrame(m_Collecting);
	}

	const UPopcornFXSceneComponent		*sceneComp = scene->SceneComponent();
	check(sceneComp);
	const FPopcornFXRenderSettings		&renderSettings = sceneComp->ResolvedRenderSettings();

	// Optim:
	// Try and Release the RenderThread's collected frame
	// only if we think the RenderThread wont use it anymore
	if (renderSettings.bEnableEarlyFrameRelease)
	{
		CParticleCollectedFrameToRender		*toRelease = null;
		if (m_RenderingLock.TryLock())
		{
			const hh_u32		expectedDrawCalledCount = PopcornFX::HHMax(1U, m_LastFrameDrawCalledCount);
			if (m_Rendering_LockMe != null &&
				m_Rendering_LockMe->m_DrawCalledCount >= expectedDrawCalledCount)
			{
				if (m_Rendering_LockMe->m_Built) // if actually rendered
					m_LastFrameDrawCalledCount = m_Rendering_LockMe->m_DrawCalledCount;
				toRelease = m_Rendering_LockMe;
				m_Rendering_LockMe = null;
			}
			m_RenderingLock.Unlock();
		}
		else
		{
			// render thread is using it, dont touch it
		}

		if (toRelease != null)
		{
			m_FramePool->SafeReleaseFrame(toRelease);
		}
	}

	// Updating this stat at pre-update
	// Because this is where if there is more than one, there will most probably be copied
	INC_DWORD_STAT_BY(STAT_PopcornFX_ActiveCollectedFrame, m_FramePool->m_StatActiveCollectedFrame);
}

void	CParticleRenderManager::GameThread_EndUpdate(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::GameThread_EndUpdate", POPCORNFX_UE_PROFILER_COLOR);

	if (m_FramePool == null)
		return;

	if (!HH_VERIFY(m_Collecting == null))
		m_FramePool->SafeReleaseFrame(m_Collecting);

	m_Collecting = m_FramePool->AquireFrame();
	if (HH_VERIFY(m_Collecting != null))
	{
		CParticleCollectedFrameToRender		&frame = *m_Collecting;

		frame.m_CollectedFrameId = ++m_CollectedFrameCount;
		frame.m_EarlyReleaseCleared = false;

		// Get settings
		{
			const UPopcornFXSettings	*settings = FPopcornFXPlugin::Get().Settings();
			check(settings);
			const UPopcornFXSceneComponent	*sceneComp = scene->SceneComponent();
			check(sceneComp);
			m_Collecting->m_RenderSettings = sceneComp->ResolvedRenderSettings();

#if POPCORNFX_RENDER_DEBUG
			m_Collecting->m_HeavyDebugMode = sceneComp->HeavyDebugMode;
			m_Collecting->m_OverrideColorByDrawCall = sceneComp->bRender_OverrideColorByDrawCall;
			m_Collecting->m_OverrideDebugColorCount = PopcornFX::HHMin(PopcornFX::HHMax(2, sceneComp->Render_OverrideDebugColorCount), 10000);
			m_Collecting->m_OverrideAllMaterial = sceneComp->Render_OverrideAllMaterial;
			if (m_Collecting->m_OverrideAllMaterial != null)
				AddCollectedUsedMaterial(m_Collecting->m_OverrideAllMaterial);
#endif
		}

		m_RenderersWalker.Walk(collection, *this);

		if (m_Collecting->Empty())
			m_Collecting->Clear(); // dont release, we still want to notify render thread that there is nothing
	}

	// Note: used for VerifyUsedMaterials, so this cannot be done in SendRenderDynamicData because too late.
	if (m_LastCollectedUsedMaterials.Num() != m_CollectedUsedMaterialCount)
	{
		m_LastCollectedMaterialsChanged = true;
		m_LastCollectedUsedMaterials.SetNum(m_CollectedUsedMaterialCount, false);
	}
}

void	CParticleRenderManager::ConcurrentThread_SendRenderDynamicData(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::ConcurrentThread_SendRenderDynamicData", POPCORNFX_UE_PROFILER_COLOR);

	if (m_FramePool == null)
		return;

	CParticleCollectedFrameToRender			*frameToRender = m_Collecting;
	m_Collecting = null;

	if (frameToRender != null)
	{
		ENQUEUE_RENDER_COMMAND(PopcornFXRenderManager_SendRenderDynamicData)(
			[this, frameToRender](FRHICommandListImmediate& RHICmdList)
			{
				HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::Pop Collected Frame", POPCORNFX_UE_PROFILER_COLOR);

				HH_ASSERT(IsInRenderingThread());

				{
					HH_CRITICAL_SECTION_SCOPEDLOCK(m_RenderingLock);
					if (m_Rendering_LockMe != null)
					{
						m_LastFrameDrawCalledCount = m_Rendering_LockMe->m_DrawCalledCount;
						m_FramePool->SafeReleaseFrame(m_Rendering_LockMe); // outside lock ?
					}
					m_Rendering_LockMe = frameToRender;
				}

				m_NeedsBuildNewFrameToRender = true;
			}
		);
	}
}

//----------------------------------------------------------------------------

bool	CParticleRenderManager::RenderersWalkReserveAtMost(PopcornFX::Drawers::ERendererClass rendererClass, hh_u32 count)
{
	CParticleCollectedFrameToRender		&frame = *m_Collecting;

	if (rendererClass == PopcornFX::Drawers::Renderer_Billboard)
	{
		return frame.m_BillboardDrs.Reserve(frame.m_BillboardDrs.Count() + count) &&
			frame.m_BillboardMaterialDescs.Reserve(frame.m_BillboardMaterialDescs.Count() + count);
	}
	else if (rendererClass == PopcornFX::Drawers::Renderer_Ribbon)
	{
		return frame.m_RibbonDrs.Reserve(frame.m_RibbonDrs.Count() + count) &&
			frame.m_RibbonMaterialDescs.Reserve(frame.m_RibbonMaterialDescs.Count() + count);
	}
	else if (rendererClass == PopcornFX::Drawers::Renderer_Mesh)
	{
		return frame.m_MeshDrs.Reserve(frame.m_MeshDrs.Count() + count) &&
			frame.m_MeshDescMaterialDescs.Reserve(frame.m_MeshDescMaterialDescs.Count() + count);
	}
	else if (rendererClass == PopcornFX::Drawers::Renderer_Light)
	{
		return frame.m_LightDrs.Reserve(frame.m_LightDrs.Count() + count);
	}

	//if (rendererClass == PopcornFX::Drawers::Renderer_Ribbon)
	//{
	//	return frame.m_RibbonDrs.Reserve(frame.m_RibbonDrs.Count() + count) &&
	//		frame.m_RibbonToMat.Reserve(frame.m_RibbonToMat.Count() + count);
	//}
	HH_ASSERT_NOT_REACHED();
	return false;
}

void	CParticleRenderManager::RenderersWalk(const PopcornFX::CParticleRenderer_Billboard &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer)
{
	CParticleCollectedFrameToRender		&frame = *m_Collecting;

	CParticleDrawer_RenderManagerMaster *myDrawer = PopcornFX::checked_cast<CParticleDrawer_RenderManagerMaster*>(&drawer);
	HH_ASSERT(myDrawer->m_Manager == this);
	UPopcornFXRendererMaterial			*rendererMaterial = myDrawer->m_RendererMaterial.Get();
	if (rendererMaterial == null)
		return;

	const PopcornFX::CParticleRenderer_Billboard::EBillboardMode		bbMode = renderer.BillboardMode();
	const hh_u32				vpp = PopcornFX::Drawers::BBModeVPP(bbMode);
	if (vpp == 0)
		return;

	const PopcornFX::CGuid			mati = frame.m_BillboardMaterialDescs.PushBack();
	HH_ASSERT(mati.Valid()); // reserved !
	CParticleMaterialDesc_Game		&mat = frame.m_BillboardMaterialDescs[mati];
	const uint32					subMati = 0;
	if (!mat.SetupGame(rendererMaterial, subMati))
	{
		frame.m_BillboardMaterialDescs.PopBackAndDiscard();
		return;
	}

	{
		const FPopcornFXSubRendererMaterial		*rendererSubMat = mat.m_RendererMaterial->GetSubMaterial(subMati);
		if (rendererSubMat != null && rendererSubMat->MaterialInstance != null)
			AddCollectedUsedMaterial(rendererSubMat->MaterialInstance);
	}

	// cannot be undone
	if (!frame.m_BillboardDrs.LockAndLoad(medium, renderer))
	{
		frame.m_BillboardMaterialDescs.PopBackAndDiscard();
		return;
	}
	HH_ASSERT(frame.m_BillboardDrs.Count() == frame.m_BillboardMaterialDescs.Count());

	++frame.m_TotalDrCount;
}

void	CParticleRenderManager::RenderersWalk(const PopcornFX::CParticleRenderer_Ribbon &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer)
{
	CParticleCollectedFrameToRender		&frame = *m_Collecting;

	CParticleDrawer_RenderManagerMaster *myDrawer = PopcornFX::checked_cast<CParticleDrawer_RenderManagerMaster*>(&drawer);
	HH_ASSERT(myDrawer->m_Manager == this);
	UPopcornFXRendererMaterial			*rendererMaterial = myDrawer->m_RendererMaterial.Get();
	if (rendererMaterial == null)
		return;

	const PopcornFX::CGuid			mati = frame.m_RibbonMaterialDescs.PushBack();
	HH_ASSERT(mati.Valid()); // reserved !
	CParticleMaterialDesc_Game		&mat = frame.m_RibbonMaterialDescs[mati];
	const uint32					subMati = 0;
	if (!mat.SetupGame(rendererMaterial, subMati))
	{
		frame.m_RibbonMaterialDescs.PopBackAndDiscard();
		return;
	}

	{
		const FPopcornFXSubRendererMaterial		*rendererSubMat = mat.m_RendererMaterial->GetSubMaterial(subMati);
		if (rendererSubMat != null && rendererSubMat->MaterialInstance != null)
			AddCollectedUsedMaterial(rendererSubMat->MaterialInstance);
	}

	// cannot be undone
	if (!frame.m_RibbonDrs.LockAndLoad(medium, renderer))
	{
		frame.m_RibbonMaterialDescs.PopBackAndDiscard();
		return;
	}
	HH_ASSERT(frame.m_RibbonDrs.Count() == frame.m_RibbonMaterialDescs.Count());

	++frame.m_TotalDrCount;
}

void	CParticleRenderManager::RenderersWalk(const PopcornFX::CParticleRenderer_Mesh &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer)
{
	CParticleCollectedFrameToRender		&frame = *m_Collecting;

	CParticleDrawer_RenderManagerMaster *myDrawer = PopcornFX::checked_cast<CParticleDrawer_RenderManagerMaster*>(&drawer);
	HH_ASSERT(myDrawer->m_Manager == this);
	UPopcornFXRendererMaterial			*rendererMaterial = myDrawer->m_RendererMaterial.Get();
	if (rendererMaterial == null)
		return;

	const hh_u32		meshCount = renderer.Meshes().Count();
	if (meshCount == 0)
		return;

	if (!HH_VERIFY(hh_u32(rendererMaterial->SubMaterials.Num()) == meshCount))
		return;

	const hh_u32				baseMeshDescMati = frame.m_MeshDescMaterialDescs.Count();
	if (!frame.m_MeshDescMaterialDescs.Reserve(baseMeshDescMati + meshCount))
		return;

	for (hh_u32 meshi = 0; meshi < meshCount; ++meshi)
	{
		UStaticMesh			*staticMesh = rendererMaterial->SubMaterials[meshi].StaticMesh;
		if (!HH_VERIFY(staticMesh != null) ||
			!HH_VERIFY(staticMesh->HasValidRenderData()))
		{
			HH_TODO("Log Message invalid mesh");
			// remove all PushBack ones
			frame.m_MeshDescMaterialDescs.Resize(baseMeshDescMati);
			return;
		}

		const PopcornFX::CGuid					matDesci = frame.m_MeshDescMaterialDescs.PushBack();
		HH_ASSERT(matDesci == baseMeshDescMati + meshi);
		CParticleMaterialDesc_MeshDesc_Game		&matDesc = frame.m_MeshDescMaterialDescs[matDesci];
		//FPopcornFXSubRendererMaterial			*subMat = rendererMaterial->GetSubMaterial(meshi);
		if (!matDesc.SetupGame(rendererMaterial, meshi))
		{
			// remove all PushBack ones
			frame.m_MeshDescMaterialDescs.Resize(baseMeshDescMati);
			return;
		}
		{
			const FPopcornFXSubRendererMaterial		*rendererSubMat = matDesc.m_RendererMaterial->GetSubMaterial(meshi);
			if (rendererSubMat != null && rendererSubMat->MaterialInstance != null)
				AddCollectedUsedMaterial(rendererSubMat->MaterialInstance);
		}
		// If SubMeshId < 0, *might* be multi section draw call (StaticMesh can have only one section)
		if (matDesc.m_StaticMeshSubMeshId < 0)
			++frame.m_MaxMultiSectionMeshDrawCallCount;
		++frame.m_TotalDrCount;
	}

	// cannot be undone
	if (!frame.m_MeshDrs.LockAndLoad(medium, renderer))
	{
		// remove all PushBack ones
		frame.m_MeshDescMaterialDescs.Resize(baseMeshDescMati);
		return;
	}
	//HH_ASSERT(frame.m_MeshDrs.Count() == ??);

}

void	CParticleRenderManager::RenderersWalk(const PopcornFX::CParticleRenderer_Light &renderer, const PopcornFX::CParticleMedium &medium, PopcornFX::CParticleDrawer &drawer)
{
	CParticleCollectedFrameToRender		&frame = *m_Collecting;

//	CParticleDrawer_RenderManagerMaster *myDrawer = PopcornFX::checked_cast<CParticleDrawer_RenderManagerMaster*>(&drawer);
//	HH_ASSERT(myDrawer->m_Manager == this);
//	UPopcornFXRendererMaterial			*rendererMaterial = myDrawer->m_RendererMaterial;
//	if (rendererMaterial == null)
//		return;

	if (!frame.m_LightDrs.LockAndLoad(medium, renderer))
		return;

	++frame.m_TotalDrCount;
}

//----------------------------------------------------------------------------

void	CParticleRenderManager::AddCollectedUsedMaterial(UMaterialInterface *materialInstance)
{
	const hh_u32	currentId = m_CollectedUsedMaterialCount++;
	bool			changedAndPushBack = false;
	if (m_LastCollectedMaterialsChanged)
		changedAndPushBack = true;
	else
	{
		if (currentId >= hh_u32(m_LastCollectedUsedMaterials.Num()))
			changedAndPushBack = true;
		else if (m_LastCollectedUsedMaterials[currentId] != materialInstance)
		{
			m_LastCollectedUsedMaterials.SetNum(currentId);
			changedAndPushBack = true;
		}
	}
	if (changedAndPushBack)
	{
		m_LastCollectedMaterialsChanged = true;
		m_LastCollectedUsedMaterials.Push(materialInstance);
	}
}
