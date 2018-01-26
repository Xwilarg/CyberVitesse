//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "DrawCall.h"

#include "Assets/PopcornFXRendererMaterial.h"

#include "Materials/MaterialInterface.h"
#include "MaterialShared.h"

//----------------------------------------------------------------------------
//
// MaterialDesc Game
//
//----------------------------------------------------------------------------

bool	CParticleMaterialDesc_Game::SetupGame(UPopcornFXRendererMaterial *rendererMaterial, uint32 subMaterialId)
{
	HH_ASSERT(rendererMaterial != null);
	m_RendererMaterial = rendererMaterial;
	if (!HH_VERIFY(MaterialIsValid()))
		return false;
	m_RendererSubMaterialId = subMaterialId;
	const FPopcornFXSubRendererMaterial		*rendererSubMat = m_RendererMaterial->GetSubMaterial(m_RendererSubMaterialId);
	if (!HH_VERIFY(rendererSubMat != null))
	{
		Clear();
		return false;
	}
	m_HasSoftAnim = (rendererSubMat->SoftAnimBlending != 0);
	m_HasAlphaRemapper = (rendererSubMat->TextureAlphaRemapper != 0);
	m_CastShadow = (rendererSubMat->CastShadow != 0);
	m_DrawOrder = rendererSubMat->DrawOrder;
	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDesc_MeshDesc_Game::SetupGame(UPopcornFXRendererMaterial *rendererMaterial, uint32 subMaterialId)
{
	if (!CParticleMaterialDesc_Game::SetupGame(rendererMaterial, subMaterialId))
		return false;
	const FPopcornFXSubRendererMaterial		*rendererSubMat = m_RendererMaterial->GetSubMaterial(m_RendererSubMaterialId);
	check(rendererSubMat != null);
	m_StaticMeshSubMeshId = rendererSubMat->SubMeshId;
	return true;
}

//----------------------------------------------------------------------------
//
// MaterialDesc Render
//
//----------------------------------------------------------------------------

bool	CParticleMaterialDesc_Render::SetupFromGame(const CParticleMaterialDesc_Game &gameMat)
{
	m_RendererMaterial = gameMat.m_RendererMaterial;
	m_RendererSubMaterialId = gameMat.m_RendererSubMaterialId;
	if (!HH_VERIFY(MaterialIsValid()))
		return false;
	m_HasSoftAnim = gameMat.m_HasSoftAnim;
	m_HasAlphaRemapper = gameMat.m_HasAlphaRemapper;
	m_CastShadow = gameMat.m_CastShadow;
	m_DrawOrder = gameMat.m_DrawOrder;
	return true;
}

//----------------------------------------------------------------------------

bool	CParticleMaterialDesc_Render::ResolveMaterial(UMaterialInterface *materialInterface, ERHIFeatureLevel::Type featureLevel)
{
	check(materialInterface);
	m_MaterialInterface = materialInterface;
	m_MaterialRenderProxy = materialInterface->GetRenderProxy(false, false);
	if (!HH_VERIFY(m_MaterialRenderProxy != null))
		return false;
	const FMaterial		*fmaterial = m_MaterialRenderProxy->GetMaterial(featureLevel);
	if (!HH_VERIFY(fmaterial != null))
		return false;
	const EBlendMode	materialBlendMode = fmaterial->GetBlendMode();
	m_Material_IsSolid = (materialBlendMode == BLEND_Opaque || materialBlendMode == BLEND_Masked);
	if (m_Material_IsSolid)
	{
		m_Material_NeedNorTan = (fmaterial->GetShadingModel() != MSM_Unlit);
	}
	else
	{
		HH_FIXME("Very bad detection of Need Normal Tangent");
		m_Material_NeedNorTan = (fmaterial->GetShadingModel() != MSM_Unlit && fmaterial->GetTranslucencyLightingMode() != TLM_VolumetricNonDirectional);
	}
	m_Material_NeedSort = (materialBlendMode == EBlendMode::BLEND_Translucent);
	return true;
}

//----------------------------------------------------------------------------

