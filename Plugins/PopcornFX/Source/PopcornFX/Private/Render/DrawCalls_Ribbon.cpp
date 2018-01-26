//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "DrawCalls_Ribbon.h"

#include "PopcornFXStats.h"
#include "World/PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"
#include "Render/MyRHIStuff.h"
#include "Render/RenderManager.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/Storage/MainMemory/storage_ram.h>

//#if (PK_GPU_D3D11 == 1)
//#	include <hh_particles/include/Storage/D3D11/storage_d3d11.h>
//#endif


//----------------------------------------------------------------------------
//
// CParticleDrawCall_Ribbon
//
//----------------------------------------------------------------------------

//static
void	CParticleDrawCall_Ribbon::BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Ribbon &renderer)
{
	flags.Set(SDrawerFlags::SupportGPUParticles, false);
}

// static
void	CParticleDrawCall_Ribbon::BuildCompatFlags(SMaterialCompatFlags &flags, CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc, const PopcornFX::Drawers::SRibbon_DrawRequest *dr)
{
	// GPU IS NOT SUPPOERTED
	HH_ASSERT(dr->StreamToRender_MainMemory() != null);
	// other flags are compatible
}

bool	CParticleDrawCall_Ribbon::Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc)
{
	// /!\ Important: or it will crash later in UE
	if (!HH_VERIFY(FPopcornFXVertexFactory::IsCompatible(matDesc->m_MaterialInterface)))
	{
		HH_TODO("Message incompatible material !");
		return false;
	}

	m_DC_TotalElementCount = 0;
	m_DC_TotalBBox = PopcornFX::CAABB::DEGENERATED;
	m_DC_DrawOrder = matDesc->m_DrawOrder;
	m_DC_IsSolid = matDesc->m_Material_IsSolid;
	m_DC_CastShadow = matDesc->m_CastShadow;

	m_RenderManager = renderManager;
	m_MaterialDesc = matDesc;
	m_BBModeMask = 0;
	m_TotalParticleCount = 0;
	m_TotalVertexCount = 0;
	m_TotalIndexCount = 0;
	m_BBCPU = null;
	//m_BBGPU = null;
	HH_ASSERT(Empty());
	return true;
}

bool	CParticleDrawCall_Ribbon::Add(const PopcornFX::Drawers::SRibbon_DrawRequest *dr)
{
	const PopcornFX::CParticleRenderer_Ribbon::EBillboardMode		bbMode = dr->m_Mode;
	//const hh_u32			vpp = PopcornFX::Drawers::BBModeVPP(bbMode);
	//const hh_u32			ipp = PopcornFX::Drawers::BBModeIPP(bbMode);
	const hh_u32			vpp = 4;
	const hh_u32			ipp = 6;
	const hh_u32			bbModeMask = (1U << bbMode);
	const hh_u32			pcount = dr->ParticleCount();
	HH_ASSERT(vpp > 0 && ipp > 0 && pcount > 0);

	const bool				first = (m_TotalParticleCount == 0);

	if (first)
	{
		m_Quality = dr->m_Quality;
		m_ShaderRotateTexture = (dr->m_Quality != PopcornFX::CParticleRenderer_Ribbon::Simple && dr->m_RotateTexture);
	}
	else
	{
		if (m_Quality != dr->m_Quality)
			return false;
		if (m_ShaderRotateTexture != (dr->m_Quality != PopcornFX::CParticleRenderer_Ribbon::Simple && dr->m_RotateTexture))
			return false;
	}

	bool					doCPUBB = true;

	if (doCPUBB && m_BBCPU == null)
	{
		m_BBCPU = m_RenderManager->m_RibbonCPUBBPool.PushBackReserved();
		HH_ASSERT(m_BBCPU != null); // reserved !
		HH_ASSERT(m_BBCPU->Empty());
		HH_ASSERT(m_BBCPU->m_DrawRequests.Empty());
	}

	// TotalPrimitiveCount is Indices Count !
	m_DC_TotalElementCount += pcount * ipp;
	const PopcornFX::CAABB		&drBB = dr->StreamToRender().BBox();
	IF_DEBUG_BBOX(HH_ASSERT(drBB.IsFinite() && drBB.Valid());)
	if (drBB.Valid() && drBB.IsFinite())
		m_DC_TotalBBox.Add(drBB);

	m_BBModeMask |= bbModeMask;
	m_TotalParticleCount += pcount;
	m_TotalVertexCount += pcount * vpp;
	m_TotalIndexCount += pcount * ipp;

	if (doCPUBB)
	{
		m_BBCPU->m_BBModeMask |= bbModeMask;
		m_BBCPU->m_TotalParticleCount += pcount;
		m_BBCPU->m_TotalVertexCount += pcount * vpp;
		m_BBCPU->m_TotalIndexCount += pcount * ipp;

		m_BBCPU->m_DrawRequests.PushBack(*dr);
	}
	else
		HH_ASSERT_NOT_REACHED();

	return true;

}

bool	CParticleDrawCall_Ribbon::StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame)
{
	if (newFrame)
		return StartDrawCallMain(views);
	return StartDrawCallViewDep(views, false);
}

bool	CParticleDrawCall_Ribbon::StartDrawCallMain(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Ribbon::StartDrawCall");

	const auto			&bbViews = views.BBViews();
	CParticleScene		*const scene = views.SceneProxy()->ParticleSceneToRender();

	CVertexBufferPool	*cpuVBPool = &scene->VBPool();
	CIndexBufferPool	*cpuIBPool = &scene->IBPool();
	CVertexBufferPool	*mainVBPool = null;
	CIndexBufferPool	*mainIBPool = null;
	const bool			mainIsGPU = false;
	if (mainIsGPU)
	{
		mainVBPool = &scene->GPUVBPool();
		mainIBPool = &scene->GPUIBPool();
	}
	else
	{
		mainVBPool = cpuVBPool;
		mainIBPool = cpuIBPool;
	}

	const bool		isShadowPass = (views.RenderPass() == CRendererSubView::RenderPass_Shadow);
	HH_ASSERT(views.RenderPass() == CRendererSubView::RenderPass_Main); // Main pass should be main pass !

	const bool		needNorTan = m_MaterialDesc->m_Material_NeedNorTan;
	const bool		_needSort = m_MaterialDesc->m_Material_NeedSort;
	const bool		hasSoftAnim = m_MaterialDesc->m_HasSoftAnim;
	const bool		hasAlphaRemapper = m_MaterialDesc->m_HasAlphaRemapper;
	const bool		castShadows = m_MaterialDesc->m_CastShadow;

	const bool		simpleQuality = (m_Quality == PopcornFX::CParticleRenderer_Ribbon::Simple);
	const bool		shaderRotate = m_ShaderRotateTexture;

	const bool		needColors = true;
	const bool		needTexcoords = true;
	const bool		needTexcoord2s = hasSoftAnim;
	const bool		needAtlasIDs = hasSoftAnim;
	const bool		needAtlasCursor = hasAlphaRemapper;

	const bool		needUVRemaps = !simpleQuality;
	//const bool		needUVFactors = !simpleQuality;

	m_UserData.m_NeedNorTan = needNorTan;
	m_UserData.m_IsSoftAnimBlended = hasSoftAnim;
	m_UserData.m_IsHighQuality = !simpleQuality;
	m_UserData.m_IsRotated = shaderRotate;

	const hh_u32		totalVertexCount = m_TotalVertexCount;
	const hh_u32		totalIndexCount = m_TotalIndexCount;

	if (!mainVBPool->AllocateIf(needColors, m_Colors, totalVertexCount, sizeof(CFloat4)))
		return false;
	if (!mainVBPool->AllocateIf(needTexcoords, m_Texcoords, totalVertexCount, sizeof(CFloat2)))
		return false;
	if (!mainVBPool->AllocateIf(needTexcoord2s, m_Texcoord2s, totalVertexCount, sizeof(CFloat2)))
		return false;
	if (!mainVBPool->AllocateIf(needAtlasIDs, m_AtlasIDs, totalVertexCount, sizeof(float)))
		return false;
	if (!mainVBPool->AllocateIf(needAtlasCursor, m_AlphaCursors, totalVertexCount, sizeof(float)))
		return false;
	if (!mainVBPool->AllocateIf(needUVRemaps, m_UVRemaps, totalVertexCount, sizeof(CFloat4)))
		return false;

	if (m_BBCPU != null)
	{
		HH_ASSERT(!mainIsGPU); // not implem

		m_BBCPU->m_Quality = m_Quality;
		m_BBCPU->m_ShaderRotateTexture = m_ShaderRotateTexture;

		m_BBCPU->m_Colors = m_Colors;
		m_BBCPU->m_Texcoords = m_Texcoords;
		m_BBCPU->m_Texcoord2s = m_Texcoord2s;
		m_BBCPU->m_AtlasIDs = m_AtlasIDs;
		m_BBCPU->m_AlphaCursors = m_AlphaCursors;
		m_BBCPU->m_UVRemaps = m_UVRemaps;
	}

	return StartDrawCallViewDep(views, true);
}

bool	CParticleDrawCall_Ribbon::StartDrawCallViewDep(PopcornFX::CRendererSubView &views, bool mainBillboarding)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Ribbon::StartDrawCall");

	const auto			&bbViews = views.BBViews();
	CParticleScene		*const scene = views.SceneProxy()->ParticleSceneToRender();

	CVertexBufferPool	*cpuVBPool = &scene->VBPool();
	CIndexBufferPool	*cpuIBPool = &scene->IBPool();
	CVertexBufferPool	*mainVBPool = null;
	CIndexBufferPool	*mainIBPool = null;
	const bool			mainIsGPU = false;
	if (mainIsGPU)
	{
		mainVBPool = &scene->GPUVBPool();
		mainIBPool = &scene->GPUIBPool();
	}
	else
	{
		mainVBPool = cpuVBPool;
		mainIBPool = cpuIBPool;
	}

	const bool		isShadowPass = (views.RenderPass() == CRendererSubView::RenderPass_Shadow);
	const bool		needNorTan = m_MaterialDesc->m_Material_NeedNorTan;
	const bool		_needSort = m_MaterialDesc->m_Material_NeedSort;

	const bool		simpleQuality = (m_Quality == PopcornFX::CParticleRenderer_Ribbon::Simple);
	const bool		shaderRotate = m_ShaderRotateTexture;

	const bool		needIndices = true;
	const bool		needSort = needIndices & _needSort & !isShadowPass;
	const bool		needPositions = true;
	const bool		needNormals = needNorTan & !isShadowPass;
	const bool		needTangents = needNorTan & !isShadowPass;
	const bool		needUVFactors = !simpleQuality;

	// User Data must not change here
	//m_UserData.m_NeedNorTan = needNorTan;
	//m_UserData.m_IsSoftAnimBlended = hasSoftAnim;
	//m_UserData.m_IsHighQuality = !simpleQuality;
	//m_UserData.m_IsRotated = shaderRotate;

	bool				hasCPUBBTasks = mainBillboarding;

	const hh_u32		totalVertexCount = m_TotalVertexCount;
	const hh_u32		totalIndexCount = m_TotalIndexCount;

	bool				mainLargeIndices = false;
	const bool			rebuildIndices = needIndices && !m_Indices.Valid();
	HH_ASSERT(rebuildIndices || !needIndices || m_Indices->AllocatedSize() == totalIndexCount * m_Indices->AllocatedStride());
	if (rebuildIndices && !mainIBPool->Allocate(m_Indices, mainLargeIndices, totalIndexCount))
		return false;

	m_ViewCount = bbViews.Count();
	if (m_BBCPU != null)
	{
		HH_ASSERT(!mainIsGPU); // not implem

		HH_ASSERT(m_BBCPU->m_Quality == m_Quality);
		HH_ASSERT(m_BBCPU->m_ShaderRotateTexture == m_ShaderRotateTexture);

		m_BBCPU->m_ViewCount = m_ViewCount;

		m_BBCPU->m_NeedSort = needSort;

		HH_ASSERT(!m_BBCPU->m_Indices.Valid());
		if (rebuildIndices)
			m_BBCPU->m_Indices = m_Indices;

		hasCPUBBTasks |= rebuildIndices;
	}

	enum
	{
		BBModeMask_NonViewDep = (
			(1U << PopcornFX::CParticleRenderer_Ribbon::AxisSideAlignedQuad)
		)
	};
	const bool		isViewDep =
		(m_BBModeMask & ~BBModeMask_NonViewDep) != 0;

	if (mainBillboarding || isViewDep)
	{
		for (hh_u32 i = 0; i < m_ViewCount; ++i)
		{
			CViewDependent		&viewDep = m_ViewDependents[i];

			const bool			sameMatrix = viewDep.m_BBMatrix.Equals(bbViews[i].m_BillboardingMatrix);
			if (!mainBillboarding && sameMatrix)
				continue;

			viewDep.m_BBMatrix = bbViews[i].m_BillboardingMatrix;

			if (!mainVBPool->AllocateIf(needPositions, viewDep.m_Positions, totalVertexCount, sizeof(CFloat4)))
				return false;
			if (!mainVBPool->AllocateIf(needNormals, viewDep.m_Normals, totalVertexCount, sizeof(CFloat4)))
				return false;
			if (!mainVBPool->AllocateIf(needTangents, viewDep.m_Tangents, totalVertexCount, sizeof(CFloat4)))
				return false;
			if (!mainVBPool->AllocateIf(needUVFactors, viewDep.m_UVFactors, totalVertexCount, sizeof(CFloat4)))
				return false;

			if (m_BBCPU != null)
			{
				HH_ASSERT(!mainIsGPU); // not implem

				CParticleBillboarder_Ribbon_CPU::CViewDependent		&bbViewDep = m_BBCPU->m_ViewDependents[i];
				bbViewDep.m_BBMatrix = viewDep.m_BBMatrix;

				HH_ASSERT(!bbViewDep.m_Positions.Valid());
				if (needPositions)
					bbViewDep.m_Positions = viewDep.m_Positions;
				HH_ASSERT(!bbViewDep.m_Normals.Valid());
				if (needNormals)
					bbViewDep.m_Normals = viewDep.m_Normals;
				HH_ASSERT(!bbViewDep.m_Tangents.Valid());
				if (needTangents)
					bbViewDep.m_Tangents = viewDep.m_Tangents;
				HH_ASSERT(!bbViewDep.m_UVFactors.Valid());
				if (needUVFactors)
					bbViewDep.m_UVFactors = viewDep.m_UVFactors;

				HH_ASSERT(needPositions);
				hasCPUBBTasks = true; // at least positions anyway
			}
		}
	}

	if (hasCPUBBTasks)
	{
		if (!m_BBCPU->LaunchBillboarding(views))
			return false;
	}

	return true;
}

class FPopcornFXRibbonCollector_3 : public FOneFrameResource
{
public:
	FPopcornFXVertexFactory		m_VertexFactory;

	CPooledIndexBuffer			m_IndexBufferForRefCount;

	~FPopcornFXRibbonCollector_3()
	{
		m_VertexFactory.ReleaseResource();
	}
};

void	CParticleDrawCall_Ribbon::DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Ribbon::DrawCall");

	if (m_BBCPU)
	{
		m_BBCPU->FinishBillboarding();
	}

	CParticleScene	*const scene = views.SceneProxy()->ParticleSceneToRender();
	const auto		&sceneViews = views.SceneViews();
	const auto		&bbViews = views.BBViews();
	HH_ASSERT(bbViews.Count() == m_ViewCount);

	FMatrix			localToWorld = FMatrix::Identity;
	if (views.GlobalScale() != 1.f)
		localToWorld *= views.GlobalScale();

	for (hh_u32 bbViewi = 0; bbViewi < m_ViewCount; ++bbViewi)
	{
		if ((bbViewMask & (1U << bbViewi)) == 0)
			continue;

		const PopcornFX::CRendererSubView::SBBView	&bbView = bbViews[bbViewi];

		const hh_u32			viewMask = bbView.m_ViewsMask;
		HH_ASSERT(viewMask != 0);

		hh_u32					firstViewi = 0;
		while ((viewMask & (1U << firstViewi)) == 0)
			++firstViewi;

		const auto		&firstSceneView = sceneViews[firstViewi];
		HH_ASSERT(firstSceneView.m_ToRender);
		HH_ASSERT(firstSceneView.m_BBViewIndex == bbViewi);

		const CViewDependent			&viewDep = m_ViewDependents[bbViewi];

		FPopcornFXRibbonCollector_3		*collectorRes = null;
		FPopcornFXVertexFactory			*vertexFactory = null;
		{
			HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Ribbon::DrawCall CollectorRes");
			
			collectorRes = &(views.Collector()->AllocateOneFrameResource<FPopcornFXRibbonCollector_3>());
			collectorRes->m_IndexBufferForRefCount = m_Indices;

			vertexFactory = &collectorRes->m_VertexFactory;
			vertexFactory->m_Positions.Setup(viewDep.m_Positions);
			vertexFactory->m_Normals.Setup(viewDep.m_Normals);
			vertexFactory->m_Tangents.Setup(viewDep.m_Tangents);
			vertexFactory->m_UVFactors.Setup(viewDep.m_UVFactors);
#if POPCORNFX_RENDER_DEBUG
			if (m_RenderManager->m_OverrideColorVertexBuffer != null)
			{
				vertexFactory->m_Colors.Setup(m_RenderManager->m_OverrideColorVertexBuffer, m_RenderManager->m_OverrideColorBufferOffset, 0);
			}
			else
#endif // POPCORNFX_RENDER_DEBUG
			{
				vertexFactory->m_Colors.Setup(m_Colors);
			}
			vertexFactory->m_Texcoords.Setup(m_Texcoords);
			vertexFactory->m_Texcoord2s.Setup(m_Texcoord2s);
			vertexFactory->m_AtlasIDs.Setup(m_AtlasIDs);
			vertexFactory->m_AlphaCursors.Setup(m_AlphaCursors);
			vertexFactory->m_UVScalesAndOffsets.Setup(m_UVRemaps);

			HH_ASSERT(!vertexFactory->IsInitialized());
			vertexFactory->InitResource();
		}

		if (!HH_VERIFY(vertexFactory->IsInitialized()))
			return;

		FMeshBatch			&meshBatch = views.Collector()->AllocateMesh();

		meshBatch.VertexFactory = vertexFactory;
		meshBatch.CastShadow = CastShadow();
		meshBatch.bUseAsOccluder = false;
		meshBatch.ReverseCulling = views.SceneProxy()->IsLocalToWorldDeterminantNegative();
		meshBatch.Type = PT_TriangleList;
		meshBatch.DepthPriorityGroup = views.SceneProxy()->GetDepthPriorityGroup(firstSceneView.m_SceneView);
		//meshBatch.bCanApplyViewModeOverrides = true;
		meshBatch.bUseWireframeSelectionColoring = views.SceneProxy()->IsSelected();
		meshBatch.bCanApplyViewModeOverrides = true;

#if POPCORNFX_RENDER_DEBUG
		if (m_RenderManager->m_OverrideAllMaterialRenderProxy != null)
		{
			meshBatch.MaterialRenderProxy = m_RenderManager->m_OverrideAllMaterialRenderProxy;
		}
		else
#endif // POPCORNFX_RENDER_DEBUG
		{
			meshBatch.MaterialRenderProxy = m_MaterialDesc->m_MaterialRenderProxy;
		}

		FMeshBatchElement &meshElement = meshBatch.Elements[0];
		//meshElement.IndexBuffer = m_IndexBuffer.IndexBuffer;
		//meshElement.FirstIndex = m_IndexBuffer.FirstIndex;
		meshElement.IndexBuffer = m_Indices.Buffer();
		HH_ASSERT(m_DC_TotalElementCount == m_TotalIndexCount);
		HH_ASSERT(elementStart + elementCount <= m_TotalIndexCount);
		meshElement.FirstIndex = elementStart;
		meshElement.NumPrimitives = elementCount / 3;
		//meshElement.NumInstances = 1;
		meshElement.MinVertexIndex = 0;
		meshElement.MaxVertexIndex = m_TotalVertexCount - 1;
		meshElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(localToWorld, views.SceneProxy()->GetBounds(), views.SceneProxy()->GetLocalBounds(), false, views.SceneProxy()->UseEditorDepthTest());
		meshElement.UserData = &m_UserData;

		for (hh_u32 viewi = firstViewi; viewi < sceneViews.Count(); ++viewi)
		{
			if ((viewMask & (1U << viewi)) == 0)
				continue;
			HH_ASSERT(sceneViews[viewi].m_ToRender);
			HH_ASSERT(sceneViews[viewi].m_BBViewIndex == bbViewi);
			INC_DWORD_STAT_BY(STAT_PopcornFX_DrawCallsCount, 1);
			views.Collector()->AddMesh(viewi, meshBatch);
		}
	}

}

//----------------------------------------------------------------------------
//
// CParticleBillboarder_Ribbon_CPU
//
//----------------------------------------------------------------------------

bool	CParticleBillboarder_Ribbon_CPU::LaunchBillboarding(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Ribbon_CPU::LaunchBillboarding");

	bool				hasTasks = false;

	const hh_u32		vertexCount = m_TotalVertexCount;
	const hh_u32		indexCount = m_TotalIndexCount;

	HH_ASSERT(vertexCount > 0 && indexCount > 0);

	if (!m_BB.Prepare(m_DrawRequests.View()))
	{
		return false;
	}

	HH_ASSERT(m_BB.TotalVertexCount() == vertexCount);
	HH_ASSERT(m_BB.TotalIndexCount() == indexCount);

	if (m_Colors.Valid())
	{
		TStridedMemoryView<CFloat4>		colors(0, vertexCount);
		if (!m_Colors->Map(colors))
			return false;
		m_Exec_Colors.Clear();
		m_Exec_Colors.m_Colors = colors;
		m_BB.AddExecFull(&m_Exec_Colors);
		hasTasks = true;
	}

	HH_ASSERT(m_ViewCount > 0);
	const bool		hasUVFactors = m_ViewDependents[0].m_UVFactors.Valid();

	if (m_Texcoords.Valid())
	{
		m_Exec_Texcoords.Clear();
		if (m_Texcoords.Valid())
		{
			TStridedMemoryView<CFloat2>		mapped(0, vertexCount);
			if (!m_Texcoords->Map(mapped))
				return false;
			m_Exec_Texcoords.m_Texcoords = mapped;
		}
#if 0
		if (m_Texcoord2s.Valid())
		{
			TStridedMemoryView<CFloat2>		mapped(0, vertexCount);
			if (!m_Texcoord2s->Map(mapped))
				return false;
			m_Exec_Texcoords.m_Texcoord2s = mapped;
		}
		if (m_AtlasIDs.Valid())
		{
			TStridedMemoryView<float>		mapped(0, vertexCount);
			if (!m_AtlasIDs->Map(mapped))
				return false;
			m_Exec_Texcoords.m_AtlasIds = mapped.ToMemoryViewIFP();
		}
#endif

		m_Exec_Texcoords.m_ForUVFactor = hasUVFactors;

		m_BB.AddExecFull(&m_Exec_Texcoords);
		hasTasks = true;
	}
	else
		HH_ASSERT(!m_Texcoord2s.Valid() && !m_AtlasIDs.Valid());

	if (m_AlphaCursors.Valid())
	{
		{
			TStridedMemoryView<float>		mapped(0, vertexCount);
			if (!m_AlphaCursors->Map(mapped))
				return false;
			m_Exec_AlphaCursors.m_AlphaCursors = mapped.ToMemoryViewIFP();
		}
		m_BB.AddExecFull(&m_Exec_AlphaCursors);
		hasTasks = true;
	}

	if (m_UVRemaps.Valid())
	{
		m_Exec_UVRemaps.Clear();
		TStridedMemoryView<CFloat4>		mapped(0, vertexCount);
		if (!m_UVRemaps->Map(mapped))
			return false;
		m_Exec_UVRemaps.m_UVRemap = mapped;
		m_BB.AddExecFull(&m_Exec_UVRemaps);
	}

	TStridedMemoryView<CFloat3, 0x10>		mappedPositionsToSort;
	CFloat3									sortDirection;

	for (hh_u32 viewi = 0; viewi < m_ViewCount; ++viewi)
	{
		CViewDependent		&execView = m_ViewDependents[viewi];
		if (!execView.m_Positions.Valid())
		{
			HH_ASSERT(!execView.m_Normals.Valid());
			HH_ASSERT(!execView.m_Tangents.Valid());
			continue;
		}

		execView.m_Exex_PNT.Clear();
		execView.m_Exex_PNT.m_BillboardingMatrix = execView.m_BBMatrix;
		execView.m_Exex_PNT.m_GlobalScale = 1.f; // done in draw's view matrix
		if (execView.m_Positions.Valid())
		{
			TStridedMemoryView<CFloat3, 0x10>		mapped(0, vertexCount, 0x10);
			if (!execView.m_Positions->Map(mapped))
				return false;
			execView.m_Exex_PNT.m_Positions = mapped;
			if (mappedPositionsToSort.Empty())
			{
				mappedPositionsToSort = mapped;
				sortDirection = execView.m_BBMatrix.StrippedZAxis();
			}
		}
		if (execView.m_Normals.Valid())
		{
			TStridedMemoryView<CFloat3, 0x10>		mapped(0, vertexCount, 0x10);
			if (!execView.m_Normals->Map(mapped))
				return false;
			execView.m_Exex_PNT.m_Normals = mapped;
		}
		if (execView.m_Tangents.Valid())
		{
			TStridedMemoryView<CFloat4, 0x10>		mapped(0, vertexCount, 0x10);
			if (!execView.m_Tangents->Map(mapped))
				return false;
			execView.m_Exex_PNT.m_Tangents = mapped;
		}
		if (execView.m_UVFactors.Valid())
		{
			TStridedMemoryView<CFloat4, 0x10>		mapped(0, vertexCount, 0x10);
			if (!execView.m_UVFactors->Map(mapped))
				return false;
			execView.m_Exex_PNT.m_UVFactors4 = mapped;
		}

		m_BB.AddExecBatch(&execView.m_Exex_PNT);

		hasTasks = true;
	}

	if (m_Indices.Valid())// || m_IndicesForGen.Valid())
	{
		bool			doSort = false;

		void			*indices = null;
		bool			largeIndices = false;
		//HH_ASSERT(!m_IndicesForGen.Valid());
		//if (m_Indices.Valid())
		{
			//HH_ASSERT(!m_IndicesForGen.Valid());
			if (!m_Indices->Map(indices, largeIndices, indexCount))
				return false;
			doSort = m_NeedSort;
		}
		//else
		//{
		//	HH_ASSERT(m_IndicesForGen.Valid());
		//	HH_ASSERT(!m_Indices.Valid());
		//	largeIndices = m_IndicesForGenLarge;
		//	indices = m_IndicesForGen->Map(indexCount, (largeIndices ? sizeof(hh_u32) : sizeof(hh_u16)));
		//	doSort = false;
		//}

		m_Exec_Indices.Clear();
		if (!m_Exec_Indices.m_IndexStream.Setup(indices, indexCount, largeIndices))
			return false;
		HH_ASSERT(!doSort || !mappedPositionsToSort.Empty());
		m_Exec_Indices.m_Positions = mappedPositionsToSort;
		m_Exec_Indices.m_SortDirection = sortDirection;

		if (!m_Exec_Indices.SetupedAndAddExec(m_BB, (doSort ? 1 : 0))) // could fail
			return false;

		hasTasks = true;
	}

	if (!hasTasks)
		return true; // return false means failure

	if (hasTasks)
	{
		CParticleScene	*const scene = views.SceneProxy()->ParticleSceneToRender();
		m_BB.LaunchTasks(scene->BBScene(), false);
		m_BBing = true;
	}

	return true;
}

void	CParticleBillboarder_Ribbon_CPU::FinishBillboarding()
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Ribbon_CPU::FinishBillboarding");
	if (!m_BBing)
		return;
	m_BBing = false;
	m_BB.WaitTasks();
	// only clear buffers
	ClearBuffers();
}
