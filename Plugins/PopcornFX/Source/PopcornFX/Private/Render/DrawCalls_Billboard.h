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
#include "Render/PopcornFXSortComputeShader.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_delegates.h>
#include <hh_kernel/include/kr_containers_array.h>
#include <pk_drawers/include/dr_billboard.h>
#include <pk_drawers/include/dr_billboard_cpu.h>

//----------------------------------------------------------------------------

class CParticleRenderManager;
struct SMaterialCompatFlags;
struct SDrawerFlags;

//----------------------------------------------------------------------------

class CParticleBillboarder_Billboard_CPU
{
public:
	class CViewDependent
	{
	public:
		CFloat4x4				m_BBMatrix;
		CPooledVertexBuffer		m_Positions;
		CPooledVertexBuffer		m_Normals;
		CPooledVertexBuffer		m_Tangents;
		PopcornFX::Drawers::CBillboard_Exec_PositionsNormals		m_Exex_PNT;
		void			ClearBuffers()
		{
			m_Positions.UnmapAndClear();
			m_Normals.UnmapAndClear();
			m_Tangents.UnmapAndClear();
		}
	};

	hh_u32						m_BBModeMask = 0;
	hh_u32						m_TotalParticleCount = 0;
	hh_u32						m_TotalVertexCount = 0;
	hh_u32						m_TotalIndexCount = 0;

	PopcornFX::TArray<PopcornFX::Drawers::SBillboard_DrawRequest>	m_DrawRequests;

	CParticleRenderManager		*m_RenderManager;

	bool						m_NeedSort = false;
	bool						m_GenDrawSlices;

	CPooledIndexBuffer			m_Indices;
	CPooledVertexBuffer			m_Colors;
	CPooledVertexBuffer			m_Texcoords;
	CPooledVertexBuffer			m_Texcoord2s;
	CPooledVertexBuffer			m_AtlasIDs;
	CPooledVertexBuffer			m_AlphaCursors;
	PopcornFX::TStaticArray<CViewDependent, PopcornFX::CRendererSubView::kMaxViews>		m_ViewDependents;
	hh_u32						m_ViewCount = 0;

	PopcornFX::CWorkingBuffer	m_IndicesWb;

	PopcornFX::Drawers::CBillboard_CPU		m_BB;

	PopcornFX::Drawers::CBillboard_Exec_Colors			m_Exec_Colors;
	PopcornFX::Drawers::CBillboard_Exec_Texcoords		m_Exec_Texcoords;
	PopcornFX::Drawers::CBillboard_Exec_AlphaCursor		m_Exec_AlphaCursors;
	PopcornFX::Drawers::CBillboard_Exec_Indices			m_Exec_Indices;

	PopcornFX::TArray<SDrawCallSlice>	m_DrawSlices;
	bool								m_DrawSlicesOk = false;
	hh_u32								m_SliceMaxStackDepth;

	bool						m_BBing = false;

	~CParticleBillboarder_Billboard_CPU() { HH_ASSERT(!m_BBing); }

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
			m_DrawSlices.Clear();
			m_DrawSlicesOk = false;
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
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
			m_ViewDependents[i].ClearBuffers();
	}

	bool		LaunchBillboarding(PopcornFX::CRendererSubView &views);
	void		FinishBillboarding();

	void		PostSort_GenSlices(const PopcornFX::Drawers::CBillboard_CPU &bb, const PopcornFX::Drawers::CBillboard_Exec_Indices &exec);
};

//----------------------------------------------------------------------------

HH_FIXME("Should be build from PopcornFXFile")
struct FAtlasRectsVertexBuffer
{
	//FVertexBufferRHIRef					m_AtlasBuffer;
	FStructuredBufferRHIRef				m_AtlasBuffer;
	FShaderResourceViewRHIRef			m_AtlasBufferSRV;
	hh_u32								m_AtlasRectsCount = 0;
	hh_u32								m_AtlasBufferCapacity = 0;

	bool		Loaded() const { return m_AtlasRectsCount > 0; }
	void		Clear() { m_AtlasBuffer = null; m_AtlasBufferSRV = null; m_AtlasRectsCount = 0; m_AtlasBufferCapacity = 0; }
	bool		LoadRects(const TMemoryView<const CFloat4> &rects);

private:
	bool		_LoadRects(const TMemoryView<const CFloat4> &rects);
};

//----------------------------------------------------------------------------

class CParticleBillboarder_Billboard_GPU
{
public:
	class CViewDependent
	{
	public:
		CFloat4x4				m_BBMatrix;
		CPooledVertexBuffer		m_Positions;
		CPooledVertexBuffer		m_Normals;
		CPooledVertexBuffer		m_Tangents;
		void			ClearBuffers()
		{
			m_Positions.UnmapAndClear();
			m_Normals.UnmapAndClear();
			m_Tangents.UnmapAndClear();
		}
	};

	hh_u32						m_BBModeMask = 0;
	hh_u32						m_TotalParticleCount = 0;
	hh_u32						m_TotalVertexCount = 0;
	hh_u32						m_TotalIndexCount = 0;

	PopcornFX::TArray<PopcornFX::Drawers::SBillboard_DrawRequest>	m_DrawRequests;

	FAtlasRectsVertexBuffer		m_AtlasRects;

	bool						m_NeedSort = false;
	bool						m_GenDrawSlices = false;

	CParticleRenderManager		*m_RenderManager;

	CPooledIndexBuffer			m_Indices;
	CPooledVertexBuffer			m_Colors;
	CPooledVertexBuffer			m_Texcoords;
	CPooledVertexBuffer			m_Texcoord2s;
	CPooledVertexBuffer			m_AtlasIDs;
	CPooledVertexBuffer			m_AlphaCursors;
	PopcornFX::TStaticArray<CViewDependent, PopcornFX::CRendererSubView::kMaxViews>		m_ViewDependents;
	hh_u32						m_ViewCount = 0;

#if (PK_GPU_D3D11 == 1)
	FPopcornFXSortComputeShader_Sorter	m_Sorter;
#endif // (PK_GPU_D3D11 == 1)

	bool		Empty() const { return m_TotalParticleCount == 0; }

	void		Clear()
	{
		if (m_TotalParticleCount == 0)
			return;
		m_BBModeMask = 0;
		m_TotalParticleCount = 0;
		m_TotalVertexCount = 0;
		m_TotalIndexCount = 0;
		m_DrawRequests.Clear();
		m_Indices.UnmapAndClear();
		m_Colors.UnmapAndClear();
		m_Texcoords.UnmapAndClear();
		m_Texcoord2s.UnmapAndClear();
		m_AtlasIDs.UnmapAndClear();
		m_AlphaCursors.UnmapAndClear();
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
			m_ViewDependents[i].ClearBuffers();
		m_ViewCount = 0;
#if (PK_GPU_D3D11 == 1)
		m_Sorter.Clear();
#endif
	}
	void		ClearBuffers()
	{
		m_Colors.UnmapAndClear();
		m_Texcoords.UnmapAndClear();
		m_Texcoord2s.UnmapAndClear();
		m_AtlasIDs.UnmapAndClear();
		m_Indices.UnmapAndClear();
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
			m_ViewDependents[i].ClearBuffers();
	}

	bool		LaunchBillboarding(PopcornFX::CRendererSubView &views);
	void		FinishBillboarding();
};

//----------------------------------------------------------------------------

class CParticleDrawCall_Billboard : public CParticleAbstractDrawCall
{
public:
	class CViewDependent
	{
	public:
		CFloat4x4				m_BBMatrix;
		CPooledVertexBuffer		m_Positions;
		CPooledVertexBuffer		m_Normals;
		CPooledVertexBuffer		m_Tangents;
		void			Clear()
		{
			m_Positions.UnmapAndClear();
			m_Normals.UnmapAndClear();
			m_Tangents.UnmapAndClear();
		}
	};

	CParticleRenderManager			*m_RenderManager;

	const CParticleMaterialDesc_Render	*m_MaterialDesc;

	hh_u32						m_BBModeMask = 0;
	hh_u32						m_TotalParticleCount = 0;
	hh_u32						m_TotalVertexCount = 0;
	hh_u32						m_TotalIndexCount = 0;

	CParticleBillboarder_Billboard_CPU		*m_BBCPU = null;
	CParticleBillboarder_Billboard_GPU		*m_BBGPU = null;

	CPooledIndexBuffer			m_Indices;
	CPooledVertexBuffer			m_Colors;
	CPooledVertexBuffer			m_Texcoords;
	CPooledVertexBuffer			m_Texcoord2s;
	CPooledVertexBuffer			m_AtlasIDs;
	CPooledVertexBuffer			m_AlphaCursors;
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
		if (m_BBGPU != null)
		{
			m_BBGPU->Clear();
			m_BBGPU = null;
		}

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
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
			m_ViewDependents[i].Clear();
		m_ViewCount = 0;
		
		//m_MaterialRenderProxy = null;
	}

	static void		BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Billboard &renderer);
	static void		BuildCompatFlags(SMaterialCompatFlags &flags, CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc, const PopcornFX::Drawers::SBillboard_DrawRequest *dr);

	bool			Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc);
	bool			Add(const PopcornFX::Drawers::SBillboard_DrawRequest *dr);

	bool			StartDrawCallMain(PopcornFX::CRendererSubView &views);
	bool			StartDrawCallViewDep(PopcornFX::CRendererSubView &views, bool mainBillboarding);


	virtual bool	StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame) override;
	virtual bool	GetSlices(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, PopcornFX::TMemoryView<const SDrawCallSlice> &outSlicesView) override;
	virtual void	DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount) override;


};
