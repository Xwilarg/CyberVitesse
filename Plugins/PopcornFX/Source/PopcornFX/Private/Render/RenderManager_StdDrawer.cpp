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

#include "Engine/StaticMesh.h"

#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------
//
// CParticleRenderManager
// CParticleDrawer_RenderManagerMaster
//
// PopcornFX Std Drawer implem
//
//----------------------------------------------------------------------------

static void		_BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer &renderer)
{
	const PopcornFX::HBO::CHandler	*handler = renderer.Handler();
	if (handler == PopcornFX::CParticleRenderer_Billboard::m_Handler)
		CParticleDrawCall_Billboard::BuildDrawerFlags(flags, renderManager, static_cast<const PopcornFX::CParticleRenderer_Billboard&>(renderer));
	else if (handler == PopcornFX::CParticleRenderer_Ribbon::m_Handler)
		CParticleDrawCall_Ribbon::BuildDrawerFlags(flags, renderManager, static_cast<const PopcornFX::CParticleRenderer_Ribbon&>(renderer));
	else if (handler == PopcornFX::CParticleRenderer_Mesh::m_Handler)
		ParticleDrawCall_Mesh_BuildDrawerFlags(flags, renderManager, static_cast<const PopcornFX::CParticleRenderer_Mesh&>(renderer));
	else if (handler == PopcornFX::CParticleRenderer_Light::m_Handler)
		ParticleDrawCall_Light_BuildDrawerFlags(flags, renderManager, static_cast<const PopcornFX::CParticleRenderer_Light&>(renderer));
	else
		HH_RELEASE_ASSERT_NOT_REACHED();
}

bool	CParticleRenderManager::_Drawer_Setup(CParticleDrawer_RenderManagerMaster *drawer)
{
	HH_ASSERT(!drawer->RenderMedium()->MediumsAndRenderers().Empty());

	const PopcornFX::CParticleRenderer		*renderer = drawer->RenderMedium()->MediumsAndRenderers()[0].m_Renderer.Get();
	HH_ASSERT(renderer != null);

	SDrawerFlags		flags = drawer->DrawerFlags();
	_BuildDrawerFlags(flags, this, *renderer);
	drawer->SetDrawerFlags(flags);

	if (UPopcornFXRendererMaterial::RendererClassHasMaterial(renderer->Handler()))
		drawer->m_RendererMaterial.Reset(UPopcornFXRendererMaterial::FindMe(renderer));

	return true;
}

PopcornFX::CParticleDrawer_Std		*CParticleRenderManager::Drawer_BuildRemapperWrapper(PopcornFX::CParticleRenderMedium *rMedium, PopcornFX::CParticleRenderManager_Std *manager)
{
	CParticleDrawer_RenderManagerMaster			*drawer = HH_NEW(CParticleDrawer_RenderManagerMaster(this, rMedium));
	if (!HH_VERIFY(drawer != null))
		return null;
	if (!_Drawer_Setup(drawer))
	{
		HH_DELETE(drawer);
		return null;
	}
	return drawer;
}

void	CParticleRenderManager::Drawer_HarvestStuff(const CParticleDrawer_RenderManagerMaster *drawer, const PopcornFX::CParticleRenderer *baseRendererDesc, float &outMaxScale, float &outMaxConstant)
{
	const PopcornFX::CParticleRenderer_Mesh	*rendererDesc = PopcornFX::HBO::Cast<const PopcornFX::CParticleRenderer_Mesh>(baseRendererDesc);
	if (rendererDesc == null)
		return;
	outMaxConstant = rendererDesc->StaticPositionOffset().Length();

	const UPopcornFXRendererMaterial	*material = drawer->m_RendererMaterial.Get();
	if (material == null)
		return;

	static const float		kMinimumMeshSize = 0.1; // 10cm

	float					allMeshesMaxSize = kMinimumMeshSize;
	const uint32			subMatCount = material->SubMaterialCount();
	for (uint32 i = 0; i < subMatCount; ++i)
	{
		const FPopcornFXSubRendererMaterial	*subMat = material->GetSubMaterial(i);
		if (subMat == null || subMat->StaticMesh == null)
			continue;
		const FBoxSphereBounds	&meshBounds = subMat->StaticMesh->ExtendedBounds;
		float					maxExtent = PopcornFX::HHMax(meshBounds.BoxExtent.X, meshBounds.BoxExtent.Y, meshBounds.BoxExtent.Z);
		maxExtent *= FPopcornFXPlugin::GlobalScaleRcp();
		allMeshesMaxSize = PopcornFX::HHMax(allMeshesMaxSize, maxExtent);
	}

	const float		rendererScale = PopcornFX::HHMax(rendererDesc->Scale().x(), rendererDesc->Scale().y(), rendererDesc->Scale().z());
	outMaxScale = rendererScale * allMeshesMaxSize;
	return;
}

bool	CParticleRenderManager::Drawer_AreRenderersCompatible(const CParticleDrawer_RenderManagerMaster *drawer, const PopcornFX::CParticleRenderer *rendererA, const PopcornFX::CParticleRenderer *rendererB)
{
	HH_ASSERT(drawer->Manager() == this);

	HH_ASSERT(rendererA != null);
	HH_ASSERT(rendererB != null);
	if (rendererA == rendererB)
		return true;

	if (rendererA->Handler() != rendererB->Handler())
		return false;

#if 1
	// We now split all drawers
	// because we use it to cache the m_RendererMaterial ! (unique per-Renderer)

	// TODO: could still merge if compatible RendererMaterial,
	// but we would need to propagate RendererMaterial changes to the
	// runtime to re-check for AreRenderersCompatible.

	return false;
#else
	/*
		Flags are used to keep GPU compatible and GPU non-compatible particles separated
	*/

	HH_ONLY_IF_ASSERTS({
		SDrawerFlags		flagsA;
	_BuildDrawerFlags(flagsA, this, *rendererA);
	HH_ASSERT(flagsA == drawer->DrawerFlags());
	});

	SDrawerFlags			flagsB;
	_BuildDrawerFlags(flagsB, this, *rendererB);
	if (!drawer->DrawerFlags().IsCompatible(flagsB))
		return false;

	return true;
#endif
}

bool	CParticleRenderManager::Drawer_GetStoragePrefs(const CParticleDrawer_RenderManagerMaster *drawer, hh_u32 &outSupportedLocations, hh_u32 &outPrefferedLocations)
{
	HH_ASSERT(drawer->Manager() == this);
	const SDrawerFlags			&flags = drawer->DrawerFlags();
	outSupportedLocations = (1 << PopcornFX::CParticleDescriptor::StorageLocation_CPU);
	if (flags.Has(SDrawerFlags::SupportGPUParticles))
	{
		outSupportedLocations |= (1 << PopcornFX::CParticleDescriptor::StorageLocation_GPU);
	}
	HH_TODO("use render settings to defines storage preferences");
	outPrefferedLocations = outSupportedLocations;
	return true;
}
