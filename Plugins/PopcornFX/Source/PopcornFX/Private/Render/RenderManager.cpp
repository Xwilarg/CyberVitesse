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

#include "PopcornFXStats.h"
#include "Assets/PopcornFXRendererMaterial.h"
#include "World/PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_sort.h>

//----------------------------------------------------------------------------
//
// CParticleRenderManager
//
//----------------------------------------------------------------------------

CParticleRenderManager::CParticleRenderManager()
{
	m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Billboard);
	m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Ribbon);
	m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Mesh);
	m_RenderersWalker.Enable(PopcornFX::Drawers::Renderer_Light);

	m_DiffuseColorStringId = PopcornFX::CStringId("DiffuseColor");
}

CParticleRenderManager::~CParticleRenderManager()
{
	HH_ASSERT(m_Collecting == null);
	HH_ASSERT(m_Rendering_LockMe == null);
	HH_ASSERT(m_FramePool == null);
}

void	CParticleRenderManager::Clean()
{
#if POPCORNFX_RENDER_DEBUG
	ENQUEUE_RENDER_COMMAND(PopcornFXRenderManager_Clean)(
		[this](FRHICommandListImmediate& RHICmdList)
		{
			m_DebugColorsVertexBuffer.ReleaseResource();
			m_DebugColorsVertexBuffer.VertexBufferRHI = null;
			m_OverrideAllMaterialRenderProxy = null;
		});
	FlushRenderingCommands();
#endif

	m_LastCollectedUsedMaterials.SetNum(0, false/* dont free mem */);

	Collect_Clean();
}

void	CParticleRenderManager::Clear()
{
	HH_ASSERT(m_Collecting == null);
}

void	CParticleRenderManager::InstallToMediumCollection(PopcornFX::CParticleMediumCollection &collection)
{
	PopcornFX::CParticleRenderManager_Std	*renderManager = (PopcornFX::CParticleRenderManager_Std*)(collection.RenderManager());
	auto									*mapping = renderManager->DrawerMapping();
#define	X_RENDERER_CLASSES(__name)																\
	if (m_RenderersWalker.Enabled(PopcornFX::Drawers::Renderer_ ## __name))						\
	{																							\
		HH_ASSERT_MESSAGE(!mapping->IsCustomRemapped<PopcornFX::CParticleRenderer_ ## __name>(), "Renderer already remapped by someone else !");\
		mapping->SetRemap<PopcornFX::CParticleRenderer_ ## __name>(								\
			PopcornFX::FastDelegate<PopcornFX::CbDrawerRemapperStd>(							\
				this, &CParticleRenderManager::Drawer_BuildRemapperWrapper)						\
		);																						\
	}
	PK_EXEC_X_RENDERER_CLASSES()
#undef	X_RENDERER_CLASSES
}

//----------------------------------------------------------------------------
//
// CParticleCollectedFrameToRender
//
//----------------------------------------------------------------------------

CParticleCollectedFrameToRender::CParticleCollectedFrameToRender()
:	m_TotalDrCount(0)
,	m_MaxMultiSectionMeshDrawCallCount(0)
{
}

CParticleCollectedFrameToRender::~CParticleCollectedFrameToRender()
{
	Clear();
}

void		CParticleCollectedFrameToRender::Clear()
{
	m_TotalDrCount = 0;
	m_Built = false;
	m_DrawCalledCount = 0;
	m_BillboardDrs.Clear();
	m_BillboardMaterialDescs.Clear();
	m_RibbonDrs.Clear();
	m_RibbonMaterialDescs.Clear();
	m_MeshDrs.Clear();
	m_MeshDescMaterialDescs.Clear();
	m_MaxMultiSectionMeshDrawCallCount = 0;
	m_LightDrs.Clear();
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

PParticleRenderManagerBase		NewParticleRenderManager()
{
	return HH_NEW(CParticleRenderManager);
}
