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

#include "Render/PopcornFXVertexFactory.h"
#include "Render/PopcornFXVertexFactoryShaderParameters.h"
#include "Render/DrawCall.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_delegates.h>
#include <hh_kernel/include/kr_containers_array.h>
#include <pk_drawers/include/dr_ribbon.h>
#include <pk_drawers/include/dr_ribbon_cpu.h>

class CParticleRenderManager;
struct SDrawerFlags;
struct SMaterialCompatFlags;

//----------------------------------------------------------------------------

class CParticleBillboarder_Ribbon_CPU
{
public:
	class CViewDependent
	{
	public:
		CFloat4x4				m_BBMatrix;
		CPooledVertexBuffer		m_Positions;
		CPooledVertexBuffer		m_Normals;
		CPooledVertexBuffer		m_Tangents;
		CPooledVertexBuffer		m_UVFactors;
		PopcornFX::Drawers::CRibbon_Exec_Positions		m_Exex_PNT;
		void			ClearBuffers()
		{
			m_Positions.UnmapAndClear();
			m_Normals.UnmapAndClear();
			m_Tangents.UnmapAndClear();
			m_UVFactors.UnmapAndClear();
		}
	};

	hh_u32						m_BBModeMask = 0;
	hh_u32						m_TotalParticleCount = 0;
	hh_u32						m_TotalVertexCount = 0;
	hh_u32						m_TotalIndexCount = 0;

	PopcornFX::TArray<PopcornFX::Drawers::SRibbon_DrawRequest>	m_DrawRequests;

	PopcornFX::CParticleRenderer_Ribbon::EQuality		m_Quality;
	bool						m_ShaderRotateTexture;

	bool						m_NeedSort = false;
	CPooledIndexBuffer			m_Indices;
	CPooledVertexBuffer			m_Colors;
	CPooledVertexBuffer			m_Texcoords;
	CPooledVertexBuffer			m_Texcoord2s;
	CPooledVertexBuffer			m_AtlasIDs;
	CPooledVertexBuffer			m_AlphaCursors;
	CPooledVertexBuffer			m_UVRemaps;
	PopcornFX::TStaticArray<CViewDependent, PopcornFX::CRendererSubView::kMaxViews>		m_ViewDependents;
	hh_u32						m_ViewCount = 0;

	PopcornFX::Drawers::CRibbon_CPU		m_BB;

	PopcornFX::Drawers::CRibbon_Exec_Colors				m_Exec_Colors;
	PopcornFX::Drawers::CRibbon_Exec_Texcoords			m_Exec_Texcoords;
	PopcornFX::Drawers::CRibbon_Exec_UVRemap			m_Exec_UVRemaps;
	PopcornFX::Drawers::CRibbon_Exec_AlphaCursor		m_Exec_AlphaCursors;
	PopcornFX::Drawers::CRibbon_Exec_FillSortIndices	m_Exec_Indices;

	bool						m_BBing = false;

	~CParticleBillboarder_Ribbon_CPU() { HH_ASSERT(!m_BBing); }

	bool		Empty() const { return m_TotalParticleCount == 0; }
	void		Clear()
	{
		if (m_BBing)
			FinishBillboarding();
		if (m_TotalParticleCount != 0)
		{
			ClearBuffers();
			m_BBModeMask = 0;
			m_TotalParticleCount = 0;
			m_TotalVertexCount = 0;
			m_TotalIndexCount = 0;
			m_DrawRequests.Clear();
		}
		HH_ASSERT(m_DrawRequests.Empty());
	}
	void		ClearBuffers()
	{
		m_Indices.UnmapAndClear();
		m_Colors.UnmapAndClear();
		m_Texcoords.UnmapAndClear();
		m_Texcoord2s.UnmapAndClear();
		m_AtlasIDs.UnmapAndClear();
		m_AlphaCursors.UnmapAndClear();
		m_UVRemaps.UnmapAndClear();
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
			m_ViewDependents[i].ClearBuffers();
	}

	bool		LaunchBillboarding(PopcornFX::CRendererSubView &views);
	void		FinishBillboarding();
};

//----------------------------------------------------------------------------

class CParticleDrawCall_Ribbon : public CParticleAbstractDrawCall
{
public:
	class CViewDependent
	{
	public:
		CFloat4x4				m_BBMatrix;
		CPooledVertexBuffer		m_Positions;
		CPooledVertexBuffer		m_Normals;
		CPooledVertexBuffer		m_Tangents;
		CPooledVertexBuffer		m_UVFactors;

		void			Clear()
		{
			m_Positions.UnmapAndClear();
			m_Normals.UnmapAndClear();
			m_Tangents.UnmapAndClear();
			m_UVFactors.UnmapAndClear();
		}
	};

	CParticleRenderManager				*m_RenderManager;

	const CParticleMaterialDesc_Render	*m_MaterialDesc;

	hh_u32						m_BBModeMask = 0;
	hh_u32						m_TotalParticleCount = 0;
	hh_u32						m_TotalVertexCount = 0;
	hh_u32						m_TotalIndexCount = 0;

	PopcornFX::CParticleRenderer_Ribbon::EQuality		m_Quality;
	bool						m_ShaderRotateTexture;

	CParticleBillboarder_Ribbon_CPU		*m_BBCPU = null;
	//CParticleBillboarder_Ribbon_GPU		*m_BBGPU = null;

	CPooledIndexBuffer			m_Indices;
	CPooledVertexBuffer			m_Colors;
	CPooledVertexBuffer			m_Texcoords;
	CPooledVertexBuffer			m_Texcoord2s;
	CPooledVertexBuffer			m_AtlasIDs;
	CPooledVertexBuffer			m_AlphaCursors;
	CPooledVertexBuffer			m_UVRemaps;
	PopcornFX::TStaticArray<CViewDependent, PopcornFX::CRendererSubView::kMaxViews>		m_ViewDependents;
	hh_u32						m_ViewCount = 0;

	FPopcornFXBatchElementParams	m_UserData;

	//bool			Empty() const { return m_TotalParticleCount == 0; }
	void			Clear()
	{
		if (m_BBCPU != null)
		{
			m_BBCPU->Clear();
			m_BBCPU = null;
		}
		//if (m_BBGPU != null)
		//{
		//	m_BBGPU->Clear();
		//	m_BBGPU = null;
		//}

		if (m_TotalParticleCount == 0)
			return;

		DC_Clear();

		m_BBModeMask = 0;
		m_TotalParticleCount = 0;
		m_TotalVertexCount = 0;
		m_TotalIndexCount = 0;
		
		m_Indices.UnmapAndClear();
		m_Colors.UnmapAndClear();
		m_Texcoords.UnmapAndClear();
		m_Texcoord2s.UnmapAndClear();
		m_AtlasIDs.UnmapAndClear();
		m_AlphaCursors.UnmapAndClear();
		m_UVRemaps.UnmapAndClear();
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
			m_ViewDependents[i].Clear();
		m_ViewCount = 0;
		
		//m_MaterialRenderProxy = null;
	}

	static void		BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Ribbon &renderer);
	static void		BuildCompatFlags(SMaterialCompatFlags &flags, CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc, const PopcornFX::Drawers::SRibbon_DrawRequest *dr);

	bool			Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc);
	bool			Add(const PopcornFX::Drawers::SRibbon_DrawRequest *dr);

	bool			StartDrawCallMain(PopcornFX::CRendererSubView &views);
	bool			StartDrawCallViewDep(PopcornFX::CRendererSubView &views, bool mainBillboarding);

	virtual bool	StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame) override;
	virtual void	DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount) override;

};
