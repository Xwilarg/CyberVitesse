//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "DrawCalls_Billboard.h"

#include "PopcornFXStats.h"
#include "World/PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"
#include "Render/MyRHIStuff.h"
#include "Render/PopcornFXBillboarderBillboardCS.h"
#include "Render/RenderManager.h"
#include "Render/DrawCallSliceHelper.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/Storage/MainMemory/storage_ram.h>

//#if (PK_GPU_D3D11 == 1)
//#	include <hh_particles/include/Storage/D3D11/storage_d3d11.h>
//#endif


//----------------------------------------------------------------------------
//
// CParticleDrawCall_Billboard
//
//----------------------------------------------------------------------------

//static
void	CParticleDrawCall_Billboard::BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Billboard &renderer)
{
	bool		supportGPU = true;
	supportGPU = supportGPU && renderer.AlphaRemapper().Empty();
	flags.Set(SDrawerFlags::SupportGPUParticles, supportGPU);
}

// static
void	CParticleDrawCall_Billboard::BuildCompatFlags(SMaterialCompatFlags &flags, CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc, const PopcornFX::Drawers::SBillboard_DrawRequest *dr)
{
	const PopcornFX::CParticleRenderer_Billboard::EBillboardMode		bbMode = dr->m_Mode;
	const hh_u32	vpp = PopcornFX::Drawers::BBModeVPP(bbMode);
	HH_ASSERT(vpp == 4 || vpp == 6);
	const bool		isGPU = dr->StreamToRender_MainMemory() == null;
	const bool		needSort = matDesc->m_Material_NeedSort;
	const bool		isCapsule = (vpp == 6);
	enum
	{
		Compat_GPU = 0,
		Compat_IsGPUCapsuleSorted,
	};
	flags.Set(Compat_GPU, isGPU); // Draw Call with both GPU+CPU not implemented
	flags.Set(Compat_IsGPUCapsuleSorted, isGPU & isCapsule & needSort);
}

bool	CParticleDrawCall_Billboard::Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_Render *matDesc)
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
	m_BBGPU = null;
	HH_ASSERT(Empty());
	return true;
}

bool	CParticleDrawCall_Billboard::Add(const PopcornFX::Drawers::SBillboard_DrawRequest *dr)
{
	const PopcornFX::CParticleRenderer_Billboard::EBillboardMode		bbMode = dr->m_Mode;
	const hh_u32			vpp = PopcornFX::Drawers::BBModeVPP(bbMode);
	const hh_u32			ipp = PopcornFX::Drawers::BBModeIPP(bbMode);
	const hh_u32			bbModeMask = (1U << bbMode);
	const hh_u32			pcount = dr->ParticleCount();
	HH_ASSERT(vpp > 0 && ipp > 0 && pcount > 0);

	const bool				first = (m_TotalParticleCount == 0);

	// A single draw call could contain both GPU and CPU billboarders
	const bool				isGPU = (dr->StreamToRender_MainMemory() == null && dr->Valid());
	HH_ASSERT(isGPU || dr->StreamToRender_MainMemory() != null);
	const bool				doCPUBB = !isGPU;
	const bool				doGPUBB = isGPU && !m_MaterialDesc->m_HasAlphaRemapper;

	// If GPU and need sort, vpp must be the same !
	if (doGPUBB)
	{
		if (m_BBGPU != null && m_MaterialDesc->m_Material_NeedSort)
			HH_ASSERT(((m_BBGPU->m_BBModeMask & PopcornFX::Drawers::BillboardBBModeMask_VPP6) != 0) == (vpp == 6));
	}

	if (doCPUBB && m_BBCPU == null)
	{
		if (m_BBGPU != null)
			HH_ASSERT_NOT_REACHED(); // Not implemented, yet?
		m_BBCPU = m_RenderManager->m_BillboardCPUBBPool.PushBackReserved();
		HH_ASSERT(m_BBCPU != null); // reserved !
		HH_ASSERT(m_BBCPU->Empty());
		HH_ASSERT(m_BBCPU->m_DrawRequests.Empty());
	}
	else if (doGPUBB && m_BBGPU == null)
	{
		if (m_BBCPU != null)
			HH_ASSERT_NOT_REACHED(); // Not implemented, yet?
		m_BBGPU = m_RenderManager->m_BillboardGPUBBPool.PushBackReserved();
		HH_ASSERT(m_BBGPU != null); // reserved !
		HH_ASSERT(m_BBGPU->Empty());
		HH_ASSERT(m_BBGPU->m_DrawRequests.Empty());
	}

	// TotalPrimitiveCount is Indices Count !
	m_DC_TotalElementCount += pcount * ipp;
	const PopcornFX::CAABB		&drBB = dr->StreamToRender().BBox();
	IF_DEBUG_BBOX(HH_ASSERT(drBB.IsFinite() && drBB.Valid());)
	if (drBB.IsFinite() && drBB.Valid())
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
	else if (doGPUBB)
	{
		m_BBGPU->m_BBModeMask |= bbModeMask;
		m_BBGPU->m_TotalParticleCount += pcount;
		m_BBGPU->m_TotalVertexCount += pcount * vpp;
		m_BBGPU->m_TotalIndexCount += pcount * ipp;

		m_BBGPU->m_DrawRequests.PushBack(*dr);
	}

	return true;

}

bool	CParticleDrawCall_Billboard::StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame)
{
	if (newFrame)
		return StartDrawCallMain(views);

	return StartDrawCallViewDep(views, false);
}

bool	CParticleDrawCall_Billboard::StartDrawCallMain(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Billboard::StartDrawCallMain");

	CParticleScene		*const scene = views.SceneProxy()->ParticleSceneToRender();

	CVertexBufferPool	*mainVBPool = null;
	CIndexBufferPool	*mainIBPool = null;
	const bool			mainHasGPU = m_BBGPU != null;
	HH_ASSERT(m_BBCPU == null || !mainHasGPU);
	if (mainHasGPU)
	{
		mainVBPool = &scene->GPUVBPool();
		mainIBPool = &scene->GPUIBPool();
	}
	else
	{
		mainVBPool = &scene->VBPool();
		mainIBPool = &scene->IBPool();
	}
	HH_ASSERT(mainVBPool != null);
	HH_ASSERT(mainIBPool != null);

	//const bool		isShadowPass = (views.RenderPass() == CRendererSubView::RenderPass_Shadow);
	HH_ASSERT(views.RenderPass() == CRendererSubView::RenderPass_Main); // Main pass should be main pass !

	const bool		needNorTan = m_MaterialDesc->m_Material_NeedNorTan;
	//const bool		_needSort = m_MaterialDesc->m_Material_NeedSort;
	const bool		hasSoftAnim = m_MaterialDesc->m_HasSoftAnim;
	const bool		hasAlphaRemapper = m_MaterialDesc->m_HasAlphaRemapper;

	const bool		needColors = true;
	const bool		needTexcoords = true;
	const bool		needTexcoord2s = hasSoftAnim;
	const bool		needAtlasIDs = hasSoftAnim;
	const bool		needAlphaCursors = hasAlphaRemapper;

	m_UserData.m_NeedNorTan = needNorTan;
	m_UserData.m_IsSoftAnimBlended = hasSoftAnim;

	const hh_u32		totalVertexCount = m_TotalVertexCount;
	//const hh_u32		totalIndexCount = m_TotalIndexCount;

	const uint32		colorStride = m_BBGPU != null && PK_BILLBOARDER_CS_OUTPUT_PACK_COLOR_F16 ? 8 : 16;
	const uint32		texcoordStride = m_BBGPU != null && PK_BILLBOARDER_CS_OUTPUT_PACK_TEXCOORD ? 4 : 8;
	// cpu gpu at the same time is not implemented
	// among other things !! colorStride, texcoordStride !! will missmatch
	HH_RELEASE_ASSERT((m_BBCPU != null) != (m_BBGPU != null));

	if (!mainVBPool->AllocateIf(needColors, m_Colors, totalVertexCount, colorStride))
		return false;
	if (!mainVBPool->AllocateIf(needTexcoords, m_Texcoords, totalVertexCount, texcoordStride))
		return false;
	if (!mainVBPool->AllocateIf(needTexcoord2s, m_Texcoord2s, totalVertexCount, texcoordStride))
		return false;

	if (!mainVBPool->AllocateIf(needAtlasIDs, m_AtlasIDs, totalVertexCount, sizeof(float)))
		return false;

	if (!mainVBPool->AllocateIf(needAlphaCursors, m_AlphaCursors, totalVertexCount, sizeof(float)))
		return false;

	if (m_BBCPU != null)
	{
		HH_ASSERT(!mainHasGPU); // not implem

		m_BBCPU->m_Colors = m_Colors;
		m_BBCPU->m_Texcoords = m_Texcoords;
		m_BBCPU->m_Texcoord2s = m_Texcoord2s;
		m_BBCPU->m_AtlasIDs = m_AtlasIDs;
		m_BBCPU->m_AlphaCursors = m_AlphaCursors;
	}
	else if (m_BBGPU != null)
	{
		HH_ASSERT(mainHasGPU);

		m_BBGPU->m_Colors = m_Colors;
		m_BBGPU->m_Texcoords = m_Texcoords;
		m_BBGPU->m_Texcoord2s = m_Texcoord2s;
		m_BBGPU->m_AtlasIDs = m_AtlasIDs;
		m_BBGPU->m_AlphaCursors = m_AlphaCursors;
	}

	return StartDrawCallViewDep(views, true);
}

bool	CParticleDrawCall_Billboard::StartDrawCallViewDep(PopcornFX::CRendererSubView &views, bool mainBillboarding)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Billboard::StartDrawCallViewDep");

	const auto			&bbViews = views.BBViews();
	CParticleScene		*const scene = views.SceneProxy()->ParticleSceneToRender();

	CVertexBufferPool	*mainVBPool = null;
	CIndexBufferPool	*mainIBPool = null;
	const bool			mainHasGPU = m_BBGPU != null;
	const bool			mainHasCPU = m_BBCPU != null;
	HH_ASSERT(m_BBCPU == null || !mainHasGPU);
	HH_ASSERT(mainHasCPU || mainHasGPU);
	if (mainHasGPU)
	{
		mainVBPool = &scene->GPUVBPool();
		mainIBPool = &scene->GPUIBPool();
	}
	else
	{
		mainVBPool = &scene->VBPool();
		mainIBPool = &scene->IBPool();
	}
	HH_ASSERT(mainVBPool != null);
	HH_ASSERT(mainIBPool != null);

	const bool		isShadowPass = (views.RenderPass() == CRendererSubView::RenderPass_Shadow);

	const bool		needNorTan = m_MaterialDesc->m_Material_NeedNorTan;
	const bool		_needSort = m_MaterialDesc->m_Material_NeedSort;

	const bool		genDrawSlices = (m_RenderManager->m_RenderSettings.SlicedDraw_PreferedSliceDepth > 0) & !isShadowPass;

	const bool		needSort = (_needSort | genDrawSlices) & !isShadowPass;
	const bool		needPositions = true;
	const bool		needNormals = needNorTan & !isShadowPass;
	const bool		needTangents = needNorTan & !isShadowPass;

	// User Data must not change here
	// m_UserData.m_NeedNorTan = needNorTan;

	bool				hasCPUBBTasks = mainBillboarding && m_BBCPU != null;
	bool				hasGPUBB = mainBillboarding && m_BBGPU != null;

	const hh_u32		totalVertexCount = m_TotalVertexCount;
	const hh_u32		totalIndexCount = m_TotalIndexCount;

	bool				mainLargeIndices = false;
	const bool			rebuildIndices = !m_Indices.Valid();
	HH_ASSERT(rebuildIndices || m_Indices->AllocatedSize() == totalIndexCount * m_Indices->AllocatedStride());
	if (rebuildIndices && !mainIBPool->Allocate(m_Indices, mainLargeIndices, totalIndexCount))
		return false;

	m_ViewCount = bbViews.Count();

	HH_ASSERT((m_BBCPU != null) ^ (m_BBGPU != null));

	if (m_BBCPU != null)
	{
		HH_ASSERT(!mainHasGPU); // not implem

		m_BBCPU->m_RenderManager = m_RenderManager;
		m_BBCPU->m_ViewCount = m_ViewCount;
		m_BBCPU->m_NeedSort = needSort;
		m_BBCPU->m_GenDrawSlices = genDrawSlices;

		HH_ASSERT(!m_BBCPU->m_Indices.Valid());
		if (rebuildIndices)
		{
			m_BBCPU->m_Indices = m_Indices;
			hasCPUBBTasks = true;
		}
	}
	else if (m_BBGPU != null)
	{
		HH_ASSERT(!mainHasCPU); // not implem

		m_BBGPU->m_RenderManager = m_RenderManager;
		m_BBGPU->m_ViewCount = m_ViewCount;
		m_BBGPU->m_NeedSort = needSort;
		m_BBGPU->m_GenDrawSlices = genDrawSlices;
		HH_ASSERT(!m_BBGPU->m_Indices.Valid());
		if (rebuildIndices)
		{
			m_BBGPU->m_Indices = m_Indices;
			hasGPUBB = true;
		}
	}

	enum
	{
		BBModeMask_NonViewDep = (
			(1U << PopcornFX::CParticleRenderer_Billboard::PlanarAlignedQuad)
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

			const uint32		ptnStride = m_BBGPU != null && PK_BILLBOARDER_CS_OUTPUT_PACK_PTN ? 12 : 16;
			// cpu gpu at the same time is not implemented
			// among other things !! ptnStride !! will missmatch:
			HH_RELEASE_ASSERT((m_BBCPU != null) != (m_BBGPU != null));

			if (!mainVBPool->AllocateIf(needPositions, viewDep.m_Positions, totalVertexCount, ptnStride))
				return false;
			if (!mainVBPool->AllocateIf(needNormals, viewDep.m_Normals, totalVertexCount, ptnStride))
				return false;
			if (!mainVBPool->AllocateIf(needTangents, viewDep.m_Tangents, totalVertexCount, ptnStride))
				return false;

			if (m_BBCPU != null)
			{
				HH_ASSERT(!mainHasGPU); // not implem

				CParticleBillboarder_Billboard_CPU::CViewDependent		&bbViewDep = m_BBCPU->m_ViewDependents[i];
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

				HH_ASSERT(needPositions);
				hasCPUBBTasks = true; // at least positions anyway
			}
			else if (m_BBGPU != null)
			{
				// Todo: Setup offsets for CPGPU draw calls

				HH_ASSERT(!mainHasCPU); // not implem

				CParticleBillboarder_Billboard_GPU::CViewDependent		&bbViewDep = m_BBGPU->m_ViewDependents[i];
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

				HH_ASSERT(needPositions);
				hasGPUBB = true;
			}
		}
	}

	if (m_BBCPU != null && hasCPUBBTasks)
	{
		HH_ASSERT(m_BBGPU == null);
		if (!m_BBCPU->LaunchBillboarding(views))
			return false;
	}
	if (m_BBGPU != null && hasGPUBB)
	{
		HH_ASSERT(m_BBCPU == null);
		m_BBGPU->LaunchBillboarding(views);
	}

	return true;
}

bool	CParticleDrawCall_Billboard::GetSlices(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, PopcornFX::TMemoryView<const SDrawCallSlice> &outSlicesView)
{
	if (m_BBCPU != null)
	{
		HH_ASSERT(m_BBGPU == null);
		m_BBCPU->FinishBillboarding();
		if (m_BBCPU->m_DrawSlicesOk)
		{
			outSlicesView = m_BBCPU->m_DrawSlices.View();
			return true;
		}
	}
	else if (m_BBGPU != null)
	{
		//HH_ASSERT(m_BBCPU == null);
		//m_BBGPU->FinishBillboarding();
	}
	return false;
}

class FPopcornFXBillobardCollector_3 : public FOneFrameResource
{
public:
	FPopcornFXVertexFactory		m_VertexFactory;

	CPooledIndexBuffer			m_IndexBufferForRefCount;

	~FPopcornFXBillobardCollector_3()
	{
		m_VertexFactory.ReleaseResource();
	}
};

void	CParticleDrawCall_Billboard::DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Billboard::DrawCall");

	if (m_BBCPU != null)
	{
		HH_ASSERT(m_BBGPU == null);
		m_BBCPU->FinishBillboarding();
	}
	else if (m_BBGPU != null)
	{
		HH_ASSERT(m_BBCPU == null);
		m_BBGPU->FinishBillboarding();
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

		FPopcornFXBillobardCollector_3	*collectorRes = null;
		FPopcornFXVertexFactory			*vertexFactory = null;

		{
			HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_Billboard::DrawCall CollectorRes");
		
			collectorRes = &(views.Collector()->AllocateOneFrameResource<FPopcornFXBillobardCollector_3>());
			collectorRes->m_IndexBufferForRefCount = m_Indices;

			// !! if the vertexFactory is CACHED:
			// be carefull streams could change Strides and/or formats on the fly !
			vertexFactory = &collectorRes->m_VertexFactory;
			vertexFactory->m_Positions.Setup(viewDep.m_Positions);
			vertexFactory->m_Normals.Setup(viewDep.m_Normals);
			vertexFactory->m_Tangents.Setup(viewDep.m_Tangents);
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
		HH_ASSERT(elementCount >= 3);
		meshElement.NumPrimitives = elementCount / 3;
		HH_ASSERT(meshElement.NumPrimitives > 0);
		//meshElement.NumInstances = 1;
		meshElement.MinVertexIndex = 0;
		meshElement.MaxVertexIndex = m_TotalVertexCount - 1;
		meshElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(localToWorld, views.SceneProxy()->GetBounds(), views.SceneProxy()->GetLocalBounds(), false, views.SceneProxy()->UseEditorDepthTest());
		meshElement.UserData = &m_UserData;

// #if (ENGINE_MINOR_VERSION >= 11)
// 		meshElement.NumInstances = 1;
// 		meshElement.bIsInstancedMesh = false;
// #endif

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
// CParticleBillboarder_Billboard_CPU
//
//----------------------------------------------------------------------------

bool	CParticleBillboarder_Billboard_CPU::LaunchBillboarding(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Billboard_CPU::LaunchBillboarding");

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
		m_BB.AddExecPage(&m_Exec_Colors);
		hasTasks = true;
	}

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
		m_BB.AddExecPage(&m_Exec_Texcoords);
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
		m_BB.AddExecPage(&m_Exec_AlphaCursors);
		hasTasks = true;
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

		m_BB.AddExecPage(&execView.m_Exex_PNT);

		hasTasks = true;
	}

	HH_ASSERT(!m_GenDrawSlices || m_Indices.Valid());

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
		m_Exec_Indices.m_OverrideSort = (doSort ? 1 : 0);
		HH_ASSERT(!doSort || !mappedPositionsToSort.Empty());
		m_Exec_Indices.m_Positions = mappedPositionsToSort;
		m_Exec_Indices.m_SortDirection = sortDirection;

		if (m_GenDrawSlices)
		{
			m_BB.m_NeedPositionsCollection = true; // sort will do that anyway
			m_Exec_Indices.m_PostSortCb = PopcornFX::Drawers::CbBillboard_Exec_Indices_PostSort(this, &CParticleBillboarder_Billboard_CPU::PostSort_GenSlices);
		}

		void		*wb = m_IndicesWb.Get(m_Exec_Indices.WorkingBufferSize(m_BB));

		if (!m_Exec_Indices.SetupedAndAddExec(m_BB, wb, m_IndicesWb.Capacity())) // could fail
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

void	CParticleBillboarder_Billboard_CPU::FinishBillboarding()
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Billboard_CPU::FinishBillboarding");
	if (!m_BBing)
		return;
	m_BBing = false;
	m_BB.WaitTasks();
	m_BB.Clear();

	// only clear buffers
	ClearBuffers();
}

void	CParticleBillboarder_Billboard_CPU::PostSort_GenSlices(const PopcornFX::Drawers::CBillboard_CPU &bb, const PopcornFX::Drawers::CBillboard_Exec_Indices &exec)
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Billboard_CPU::PostSort_GenSlices");

	m_DrawSlices.Clear();
	m_DrawSlicesOk = false;

	if (exec.m_IndexStream.Count() == 0)
	{
		m_DrawSlicesOk = true;
		return;
	}

	const CFloat4x4			&bbMatrix = m_ViewDependents[0].m_BBMatrix;

	SDrawCallSliceHelper	helper;
	helper.m_Indices = exec.m_IndexStream.RawData();
	helper.m_PrimitiveSize = 6;
	helper.m_LargeIndices = exec.m_IndexStream.Large();
	helper.m_IndexCount = exec.m_IndexStream.Count();
	helper.m_VPP4_ParticleCount = bb.VPP4_ParticleCount();
	helper.m_TotalVertexCount = bb.TotalVertexCount();
	helper.m_ViewPos = bbMatrix.StrippedTranslations();
	helper.m_ViewAxis = -bbMatrix.StrippedZAxis();
	helper.m_PositionCollection = m_BB.PositionsCollection();
	helper.m_OutDrawCallSlices = &m_DrawSlices;
	helper.m_RenderSettings = &(m_RenderManager->m_RenderSettings);

	const bool	ok = helper.GenSlices();

	m_DrawSlicesOk |= ok;
}

//----------------------------------------------------------------------------
//
// CParticleBillboarder_Billboard_GPU
//
//----------------------------------------------------------------------------

bool	CParticleBillboarder_Billboard_GPU::LaunchBillboarding(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Billboard_GPU::LaunchBillboarding");

#if (PK_HAS_GPU != 0)
	FRHICommandListImmediate			&RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();

	HH_ASSERT(m_DrawRequests.Count() > 0);

	CParticleScene		*const scene = views.SceneProxy()->ParticleSceneToRender();
	auto				&gpuvbpool = scene->GPUVBPool();

	TMemoryView<const CFloat4>					atlasRects;
	HH_ONLY_IF_ASSERTS(PopcornFX::CString		checkAlasPath);
	// HH_TODO("hot reload atlas");
	if (m_DrawRequests[0].m_Atlas != null)
	{
		//HH_FIXME("Atlas access should be Update thread !");
		if (!HH_VERIFY(m_AtlasRects.LoadRects(m_DrawRequests[0].m_Atlas->m_RectangleList.m_RectsFp32)))
			return false;
		HH_ASSERT(m_AtlasRects.Loaded());
		HH_ONLY_IF_ASSERTS(checkAlasPath = m_DrawRequests[0].UnsafeRenderer()->AtlasDefinition());
	}

	const hh_u32	totalParticlesCount = m_TotalParticleCount;
	const hh_u32	totalVerticesCount = m_TotalVertexCount;
	const hh_u32	totalIndicesCount = m_TotalIndexCount;

	const hh_u32	baseParticlesOffset = 0;//m_ParticlesOffset;
	const hh_u32	baseVerticesOffset = 0;//m_VerticesOffset;
	const hh_u32	baseIndicesOffset = 0;//m_IndicesOffset;

	bool			_needSort = m_Indices.Valid() && m_NeedSort;

#if (PK_GPU_D3D11 == 1)
	if (_needSort && !m_Sorter.Prepare(gpuvbpool, totalParticlesCount))
#endif // (PK_GPU_D3D11 == 1)
		_needSort = false;
	const bool		needSort = _needSort;

	hh_u32			next_particlesOffset = 0;
	hh_u32			next_indicesOffset = 0;
	hh_u32			next_verticesOffset = 0;

	using namespace PopcornFXBillboarder;

	if (needSort)
	{
		MYGPUMARKER("PopcornFX Render Billboard Sort");

#if (PK_GPU_D3D11 == 1)
		HH_ASSERT(m_Sorter.Ready());

		FPopcornFXSortComputeShader_GenKeys_Params		genKeysParams;
		HH_ASSERT(m_ViewCount > 0);
		genKeysParams.m_SortOrigin = m_ViewDependents[0].m_BBMatrix.WAxis();

		for (hh_u32 dri = 0; dri < m_DrawRequests.Count(); ++dri)
		{
			const PopcornFX::Drawers::SBillboard_DrawRequest	&dr = m_DrawRequests[dri];

			const hh_u32		particlesCount = dr.ParticleCount();
			HH_ASSERT(particlesCount > 0); // DrawRequest's job

			const hh_u32	vpp = PopcornFX::Drawers::BBModeVPP(dr.m_Mode);
			const hh_u32	ipp = PopcornFX::Drawers::BBModeIPP(dr.m_Mode);
			HH_ASSERT(vpp > 0);

			const hh_u32	bbType = PopcornFXBillboarder::BillboarderModeToType(dr.m_Mode);
			if (!HH_VERIFY(ipp > 0) ||
				!HH_VERIFY(bbType < EBillboarder::_Count))
				continue;

			const hh_u32			verticesOffset = next_verticesOffset;
			const hh_u32			indicesOffset = next_indicesOffset;
			const hh_u32			particlesOffset = next_particlesOffset;
			next_verticesOffset += particlesCount * vpp;
			next_indicesOffset += particlesCount * ipp;
			next_particlesOffset += particlesCount;

			HH_ASSERT(dr.StreamToRender_MainMemory() == null);
			const PopcornFX::CParticleStreamToRender_D3D11		*stream = static_cast<const PopcornFX::CParticleStreamToRender_D3D11*>(&(dr.StreamToRender()));
			HH_ASSERT(dr.m_PositionStreamId.Valid()); // DrawRequest's job
			const FShaderResourceViewRHIRef	inPositions = StreamBufferSRVToRHI<CFloat3, 12>(stream, dr.m_PositionStreamId);
			HH_ASSERT(IsValidRef(inPositions)); // ...

			// @TODO: does each bbView needs it's own sorted indices ????

			genKeysParams.m_Count = particlesCount; // will fill the rest with infinity
			genKeysParams.m_IndexStart = verticesOffset;
			genKeysParams.m_IndexStep = vpp;
			genKeysParams.m_InputOffset = 0; // FUTURE stream pages
			genKeysParams.m_InputPositions = inPositions;

			m_Sorter.DispatchGenIndiceBatch(RHICmdList, genKeysParams);
		}

		m_Sorter.DispatchSort(RHICmdList);

		HH_ASSERT(next_verticesOffset == totalVerticesCount);
		HH_ASSERT(next_indicesOffset == totalIndicesCount);
		HH_ASSERT(next_particlesOffset == totalParticlesCount);
#endif // (PK_GPU_D3D11 == 1)
		next_particlesOffset = 0;
		next_indicesOffset = 0;
		next_verticesOffset = 0;
	}


	const ERHIFeatureLevel::Type						featureLevel = ERHIFeatureLevel::SM5;
	TShaderMapRef< FBillboarderCS_Std >	billboarderCS(GetGlobalShaderMap(featureLevel));
	HH_FIXME("asserts real feature level is SM5, if not, we should have never been this far here");
	FBillboarderCS_Params		_params;
	FBillboarderCS_Params		*params = &_params;

	params->m_RendererAtlasRectCount = m_AtlasRects.m_AtlasRectsCount;
	params->m_RendererAtlasBuffer = m_AtlasRects.m_AtlasBufferSRV;

	{
		MYGPUMARKER("PopcornFX Render Billboard Billboarding");

		for (hh_u32 dri = 0; dri < m_DrawRequests.Count(); ++dri)
		{

			HH_ONLY_IF_ASSERTS(checkAlasPath == m_DrawRequests[dri].UnsafeRenderer()->AtlasDefinition());

			const PopcornFX::Drawers::SBillboard_DrawRequest	&dr = m_DrawRequests[dri];
			const hh_u32		particlesCount = dr.ParticleCount();
			HH_ASSERT(particlesCount > 0); // DrawRequest's job

			const hh_u32	vpp = PopcornFX::Drawers::BBModeVPP(dr.m_Mode);
			const hh_u32	ipp = PopcornFX::Drawers::BBModeIPP(dr.m_Mode);
			HH_ASSERT(vpp > 0);

			const hh_u32	bbType = PopcornFXBillboarder::BillboarderModeToType(dr.m_Mode);
			if (!HH_VERIFY(ipp > 0) ||
				!HH_VERIFY(bbType < EBillboarder::_Count))
				continue;

			const hh_u32			verticesOffset = next_verticesOffset;
			const hh_u32			indicesOffset = next_indicesOffset;
			const hh_u32			particlesOffset = next_particlesOffset;
			next_verticesOffset += particlesCount * vpp;
			next_indicesOffset += particlesCount * ipp;
			next_particlesOffset += particlesCount;

			HH_ASSERT(dr.StreamToRender_MainMemory() == null);
			const PopcornFX::CParticleStreamToRender_D3D11		*stream = static_cast<const PopcornFX::CParticleStreamToRender_D3D11*>(&(dr.StreamToRender()));
			HH_ASSERT(dr.m_PositionStreamId.Valid()); // DrawRequest's job

			const FShaderResourceViewRHIRef		inPositions = StreamBufferSRVToRHI<CFloat3, 12>(stream, dr.m_PositionStreamId);
			HH_ASSERT(IsValidRef(inPositions)); // ...

			params->m_BillboarderType = bbType;
			params->m_InIndicesOffset = baseParticlesOffset + particlesOffset;
			params->m_InputOffset = 0; // FUTURE stream pages
			params->m_OutputVertexOffset = baseVerticesOffset + verticesOffset;
			params->m_OutputIndexOffset = baseIndicesOffset + indicesOffset;

			params->m_ParticleCount = particlesCount;
			params->m_LiveParticleCount = StreamBufferSRVToRHI(stream->StreamSizeBuf(), 16);

			{
				const float		aspectRatio = dr.m_AspectRatio;
				CFloat2			ar = CFloat2(1.0f);
				if (aspectRatio > 1.0f)
					ar.y() = 1.0f / aspectRatio;
				else
					ar.x() = aspectRatio;
				params->m_RendererAspectRatio = ar;
			}
			params->m_RendererAxisScale = dr.m_AxisScale;
			params->m_RendererFlags = 0;
			params->m_RendererFlags |= (dr.m_VFlipUVs ? (1 << ERendererFlag::FlipV) : 0);
			params->m_RendererFlags |= (dr.m_SoftAnimationBlending ? (1 << ERendererFlag::SoftAnimationBlending) : 0);
			params->m_RendererNormalsBendingFactor = PopcornFX::HHMin(dr.m_NormalsBendingFactor, 0.99f);	// don't generate fully flat normals, this will interpolate crappyly on the GPU

			params->SetOutput(EOutput::OutIndices, m_Indices);
			params->SetOutput(EOutput::OutColors, m_Colors);
			params->SetOutput(EOutput::OutTexcoords, m_Texcoords);
			params->SetOutput(EOutput::OutTexcoord2s, m_Texcoord2s);
			params->SetOutput(EOutput::OutAtlasIDs, m_AtlasIDs);

#if (PK_GPU_D3D11 == 1)
			HH_ASSERT(m_Sorter.Ready() == needSort);
#endif

			params->SetInput(EInput::InIndices, needSort,
#if (PK_GPU_D3D11 == 1)
				m_Sorter.Ready() ? m_Sorter.SortedValuesSRV() :
#endif // (PK_GPU_D3D11 == 1)
				null);

			params->SetInput(EInput::InPositions, inPositions);
			if (dr.m_SizeFloat2)
			{
				params->SetInputIFP<CFloat2, 8>(EInput::In2Sizes, stream, dr.m_SizeStreamId, CFloat4(dr.m_ConstantRadius));
				params->SetInput(EInput::InSizes, null);
			}
			else
			{
				params->SetInput(EInput::In2Sizes, null);
				params->SetInputIFP<float, 4>(EInput::InSizes, stream, dr.m_SizeStreamId, CFloat4(dr.m_ConstantRadius));
			}
			params->SetInputIFP<CFloat4, 16>(EInput::InColors, stream, dr.m_ColorStreamId, CFloat4(1));
			params->SetInputIFP<float, 4>(EInput::InRotations, stream, dr.m_RotationStreamId, CFloat4(0.f));
			params->SetInputIFP<CFloat3, 12>(EInput::InAxis0s, stream, dr.m_Axis0StreamId, CFloat4(0, 0, 1, 0));
			params->SetInputIFP<CFloat3, 12>(EInput::InAxis1s, stream, dr.m_Axis1StreamId, CFloat4(0, 0, 1, 0));
			params->SetInputIFP<float, 4>(EInput::InTextureIds, stream, dr.m_TextureIDStreamId, CFloat4(0.f));

			bool				firstViewExecuted = false;

			for (hh_u32 viewi = 0; viewi < m_ViewCount; ++viewi)
			{
				CParticleBillboarder_Billboard_GPU::CViewDependent		&viewDep = m_ViewDependents[viewi];

				params->m_BillboardingMatrix = viewDep.m_BBMatrix;

				params->SetOutput(EOutput::OutPositions, viewDep.m_Positions);
				params->SetOutput(EOutput::OutNormals, viewDep.m_Normals);
				params->SetOutput(EOutput::OutTangents, viewDep.m_Tangents);

				billboarderCS->Dispatch(RHICmdList, *params);

				//params->SetOutput(EOutput::OutPositions, null);
				//params->SetOutput(EOutput::OutNormals, null);
				//params->SetOutput(EOutput::OutTangents, null);

				// Unset common params: only do them once
				if (!firstViewExecuted)
				{
					params->SetOutput(EOutput::OutIndices, null);
					params->SetOutput(EOutput::OutColors, null);
					params->SetOutput(EOutput::OutTexcoords, null);
					params->SetOutput(EOutput::OutTexcoord2s, null);
					params->SetOutput(EOutput::OutAtlasIDs, null);
					firstViewExecuted = true;
				}
			}
		} // for DRs
	} // GPU marker

	HH_ASSERT(next_verticesOffset == totalVerticesCount);
	HH_ASSERT(next_indicesOffset == totalIndicesCount);
	HH_ASSERT(next_particlesOffset == totalParticlesCount);
#endif // (PK_HAS_GPU != 0)

	return true;
}

//----------------------------------------------------------------------------

void	CParticleBillboarder_Billboard_GPU::FinishBillboarding()
{
	HH_NAMEDSCOPEDPROFILE("CParticleBillboarder_Billboard_GPU::FinishBillboarding");
	ClearBuffers();
}

//----------------------------------------------------------------------------
//
// FAtlasRectVertexBuffer
//
//----------------------------------------------------------------------------

bool	FAtlasRectsVertexBuffer::LoadRects(const TMemoryView<const CFloat4> &rects)
{
	if (rects.Empty())
	{
		Clear();
		return false;
	}
	if (!_LoadRects(rects))
	{
		Clear();
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------

bool	FAtlasRectsVertexBuffer::_LoadRects(const TMemoryView<const CFloat4> &rects)
{
	const hh_u32			bytes = rects.CoveredBytes();

	if (m_AtlasBuffer == null || bytes > m_AtlasBufferCapacity)
	{
		m_AtlasBufferCapacity = PopcornFX::Mem::Align<0x100>(bytes + 0x10);
		FRHIResourceCreateInfo	info;
		//m_AtlasBuffer = RHICreateVertexBuffer(m_AtlasBufferCapacity, BUF_Dynamic | BUF_ByteAddressBuffer | BUF_ShaderResource, info);
		m_AtlasBuffer = RHICreateStructuredBuffer(sizeof(CFloat4), m_AtlasBufferCapacity, BUF_Dynamic | BUF_ByteAddressBuffer | BUF_ShaderResource, info);
		if (!HH_VERIFY(IsValidRef(m_AtlasBuffer)))
			return false;
		m_AtlasBufferSRV = RHICreateShaderResourceView(m_AtlasBuffer);
		//m_AtlasBufferSRV = RHICreateShaderResourceView(m_AtlasBuffer, sizeof(hh_u32), PF_R32_UINT); // Buffer<uint> view
		//m_AtlasBufferSRV = RHICreateShaderResourceView(m_AtlasBuffer); // Raw view
		if (!HH_VERIFY(IsValidRef(m_AtlasBufferSRV)))
			return false;
	}
	else
	{
		HH_ASSERT(m_AtlasBufferSRV != null);
	}

	//void		*map = RHILockVertexBuffer(m_AtlasBuffer, 0, bytes, RLM_WriteOnly);
	void		*map = RHILockStructuredBuffer(m_AtlasBuffer, 0, bytes, RLM_WriteOnly);
	if (!HH_VERIFY(map != null))
		return false;
	PopcornFX::Mem::Copy(map, rects.Data(), bytes);
	//RHIUnlockVertexBuffer(m_AtlasBuffer);
	RHIUnlockStructuredBuffer(m_AtlasBuffer);

	m_AtlasRectsCount = rects.Count();
	return true;
}

//----------------------------------------------------------------------------
