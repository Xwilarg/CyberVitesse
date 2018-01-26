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
#include "Assets/PopcornFXRendererMaterial.h"
#include "RHIDefinitions.h"

#include "PopcornFXSDK.h"
#include <hh_maths/include/hh_maths_primitives.h>

//----------------------------------------------------------------------------

class	UMaterialInterface;
class	UMaterialInstanceConstant;
class	UStaticMesh;
struct	FStaticMeshLODResources;
class	FMaterialRenderProxy;

struct	FPopcornFXSubRendererMaterial;

//----------------------------------------------------------------------------

class CParticleMaterialDesc_Game
{
public:
	const UPopcornFXRendererMaterial								*m_RendererMaterial = null;
	uint32		m_RendererSubMaterialId;
	int32		m_DrawOrder;

	bool		m_HasSoftAnim : 1;
	bool		m_HasAlphaRemapper : 1;
	bool		m_CastShadow : 1;

	bool		SetupGame(UPopcornFXRendererMaterial *rendererMaterial, uint32 subMaterialId);

	// for Meshes
	uint32		MeshDescId() const { return m_RendererSubMaterialId; }

	bool		MaterialIsValid() const
	{
		if (m_RendererMaterial == null)
			return false;
		return HH_VERIFY(m_RendererMaterial->IsValidLowLevel());
	}

	void		Clear()
	{
		m_RendererMaterial = null;
	}
};

//----------------------------------------------------------------------------

class CParticleMaterialDesc_MeshDesc_Game : public CParticleMaterialDesc_Game
{
public:
	hh_i32			m_StaticMeshSubMeshId;
	bool			SetupGame(UPopcornFXRendererMaterial *rendererMaterial, uint32 subMaterialId);
};

//----------------------------------------------------------------------------

class CParticleMaterialDesc_Render : public CParticleMaterialDesc_Game
{
public:
	UMaterialInterface			*m_MaterialInterface = null;
	FMaterialRenderProxy		*m_MaterialRenderProxy = null;
	bool						m_Material_IsSolid : 1;
	bool						m_Material_NeedNorTan : 1;
	bool						m_Material_NeedSort : 1;

	bool		SetupFromGame(const CParticleMaterialDesc_Game &gameMat);
	bool		ResolveMaterial(UMaterialInterface *materialInterface, ERHIFeatureLevel::Type featureLevel);

	void		Clear()
	{
		CParticleMaterialDesc_Game::Clear();
		m_MaterialInterface = null;
		m_MaterialRenderProxy = null;
	}
};


//----------------------------------------------------------------------------

class CParticleMaterialDesc_MeshSingleSection_Render : public CParticleMaterialDesc_Render
{
public:
	bool							m_IsDefaultMeshMaterial;
	const UStaticMesh				*m_StaticMesh;
	const FStaticMeshLODResources	*m_RenderResources;
	hh_u32							m_LODIndex;
	hh_u32							m_StaticMeshSingleSectionId;

	void		Clear()
	{
		CParticleMaterialDesc_Render::Clear();
	}

};

//----------------------------------------------------------------------------

class CParticleMaterialDesc_MeshMultiSection_Render : public CParticleMaterialDesc_Render
{
public:
	const UStaticMesh				*m_StaticMesh;
	const FStaticMeshLODResources	*m_RenderResources;
	hh_u32							m_LODIndex;
	hh_u32							m_SectionStart;
	hh_u32							m_SectionCount;

	void	Clear()
	{
		CParticleMaterialDesc_Render::Clear();
	}
};

struct SDrawCallSlice
{
	float		m_Depth;
	uint32		m_ElementStart;
	uint32		m_ElementCount;
};


//----------------------------------------------------------------------------

class CParticleAbstractDrawCall
{
public:
	hh_u32				m_DC_TotalElementCount;
	PopcornFX::CAABB	m_DC_TotalBBox;
	int32				m_DC_DrawOrder = 0;
	bool				m_DC_IsSolid = false;
	bool				m_DC_CastShadow = false;

	virtual ~CParticleAbstractDrawCall() { }

	bool				Empty() const { return m_DC_TotalElementCount == 0; }
	bool				IsSolid() const { return m_DC_IsSolid; }
	bool				CastShadow() const { return m_DC_CastShadow; }

	void				DC_Clear()
	{
		m_DC_TotalElementCount = 0;
	}

	virtual bool		StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame) = 0;
	virtual void		DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount) = 0;

	virtual bool		GetSlices(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, PopcornFX::TMemoryView<const SDrawCallSlice> &outSlicesView)
	{
		return false;
	}
};

//----------------------------------------------------------------------------
