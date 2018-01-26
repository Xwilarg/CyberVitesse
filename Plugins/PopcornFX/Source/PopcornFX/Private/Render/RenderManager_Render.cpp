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
#include "Materials/MaterialInstanceConstant.h"

#include "Engine/StaticMesh.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_sort.h>

#if defined(HH_DEBUG)
static volatile bool		s_EnableMaterialMerge = true;
static volatile bool		s_EnableDrawCallSlicesMerge = true;
#else
static const bool			s_EnableMaterialMerge = true;
static const bool			s_EnableDrawCallSlicesMerge = true;
#endif

namespace EHeavyDebugModeMask
{
	enum Type
	{
		Particles	= (1U << 0),
		Mediums		= (1U << 1),
		Pages		= (1U << 2),
	};
}

//----------------------------------------------------------------------------
//
// CParticleRenderManager
//
// Render Thread build and call Draw Calls
//
//----------------------------------------------------------------------------

void		CParticleRenderManager::RenderThread_BuildAndRenderNewFrame(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::RenderThread_BuildAndRenderNewFrame", POPCORNFX_UE_PROFILER_COLOR);
	HH_ASSERT(IsInRenderingThread());
	m_NeedsBuildNewFrameToRender = false;

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RenderingLock);
	if (m_Rendering_LockMe == null)
		return;

	m_IsNewFrame = true;
	m_Rendering_LockMe->m_Built = true;
	SetupRender(views, true/*new frame*/);

	// Note: must be called even if empty

	BuildDrawCalls_Billboard();
	BuildDrawCalls_Ribbon();
	BuildDrawCalls_Mesh();
	BuildDrawCalls_Light();

	StartAndCallDrawCalls();

	m_IsNewFrame = false;
}

void		CParticleRenderManager::RenderThread_RenderLastFrame(PopcornFX::CRendererSubView &views)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::RenderThread_RenderLastFrame", POPCORNFX_UE_PROFILER_COLOR);
	HH_ASSERT(IsInRenderingThread());
	HH_ASSERT(!m_NeedsBuildNewFrameToRender);
	HH_ASSERT(!m_IsNewFrame);

	HH_CRITICAL_SECTION_SCOPEDLOCK(m_RenderingLock);
	if (m_Rendering_LockMe == null)
		return;
	HH_ASSERT(m_Rendering_LockMe->m_Built);

	// Note: must be called even if empty

	SetupRender(views, false/*not a new frame*/);
	StartAndCallDrawCalls();
}

void	CParticleRenderManager::SetupRender(PopcornFX::CRendererSubView &views, bool newFrame)
{
	if (newFrame)
	{
		m_Views = &views;

		m_FeatureLevel = views.ViewFamily()->GetFeatureLevel();

		const float		toPkScale = FPopcornFXPlugin::GlobalScaleRcp();

		// Fix settings and
		// Transform inplace to PopcornFX Space

		m_RenderSettings = m_Rendering_LockMe->m_RenderSettings;

		if (m_RenderSettings.FarCullDistance > 0)
			m_RenderSettings.FarCullDistance *= toPkScale;

		switch (m_RenderSettings.RenderCullMethod)
		{
		case EPopcornFXRenderCullMethod::None:
			m_CullTest_DrawRequest = false;
			break;
		case EPopcornFXRenderCullMethod::DrawRequestsAgainstFrustrum:
			m_CullTest_DrawRequest = true;
			break;
		}

		switch (m_RenderSettings.DrawCallSortMethod)
		{
		case EPopcornFXDrawCallSortMethod::PerSlicedDrawCalls:
			m_RenderSettings.SlicedDraw_PreferedSliceDepth *= toPkScale;
			m_RenderSettings.SlicedDraw_PreferedSliceDepth = PopcornFX::HHMax(1.f, m_RenderSettings.SlicedDraw_PreferedSliceDepth); // 1 cm min
			break;
		//case EPopcornFXDrawCallSortMethod::None:
		//case EPopcornFXDrawCallSortMethod::PerDrawCalls:
		//case EPopcornFXDrawCallSortMethod::PerPageDrawCalls:
		default:
			m_RenderSettings.SlicedDraw_PreferedSliceDepth = -1.0f;
			break;
		}

		m_RenderSettings.Shadows_ExtendCullBBoxes = PopcornFX::HHMax(0, m_RenderSettings.Shadows_ExtendCullBBoxes) * toPkScale;

		m_Can_CastShadows = views.HasShadowPass();
	}
	else
	{
		HH_ASSERT(m_Views == &views);
		HH_ASSERT(m_FeatureLevel == views.ViewFamily()->GetFeatureLevel());
	}
}

void	CParticleRenderManager::StartAndCallDrawCalls()
{
	HH_ASSERT(m_Rendering_LockMe != null);
	HH_ASSERT(m_Rendering_LockMe->m_Built);

	++m_Rendering_LockMe->m_DrawCalledCount;

	m_SolidDrawCalls.Clear();
	m_TransDrawCalls.Clear();

	const CParticleCollectedFrameToRender		&frame = *m_Rendering_LockMe;
	if (frame.Empty())
		return;

	// kind-of
	// There might be more Mesh draw call than exepected !
	const hh_u32		kindofDrawCallCount =
		frame.m_BillboardDrs.Count() +
		frame.m_RibbonDrs.Count() +
		frame.m_MeshDescMaterialDescs.Count();
	if (!HH_VERIFY(m_SolidDrawCalls.Reserve(kindofDrawCallCount)) ||
		!HH_VERIFY(m_TransDrawCalls.Reserve(kindofDrawCallCount)))
		return;

	StartDrawCalls_Billboard();
	StartDrawCalls_Ribbon();
	StartDrawCalls_Mesh();
	//StartDrawCalls_Light();

	CallDrawCalls();
}

//----------------------------------------------------------------------------
//
// Render CParticleRenderManager
//
//----------------------------------------------------------------------------

bool	CParticleRenderManager::CullTest(const PopcornFX::Drawers::SBase_DrawRequest &dr, bool castShadow)
{
	HH_NAMEDSCOPEDPROFILE("CParticleRenderManager::Cull");

	PopcornFX::CAABB	bb = dr.StreamToRender().BBox();
	if (!bb.IsFinite())
		return false;
	if (!bb.Valid())
		return false;
	if (castShadow)
	{
		if (m_RenderSettings.bShadows_DisableCull)
			return false;
		bb.Min() -= m_RenderSettings.Shadows_ExtendCullBBoxes;
		bb.Max() += m_RenderSettings.Shadows_ExtendCullBBoxes;
	}

	const CFloat3		origin = bb.Center();
	const CFloat3		extent = bb.Extent();

	if (m_RenderSettings.FarCullDistance > 0)
	{
		const auto		&bbViews = m_Views->BBViews();
		const bool		cull =
			PopcornFX::TestCullAABB(
				bb,
				m_RenderSettings.FarCullDistance * m_RenderSettings.FarCullDistance,
				TStridedMemoryView<const CFloat3>(&bbViews[0].m_BillboardingMatrix.StrippedZAxis(), bbViews.Count(), sizeof(bbViews[0])),
				TStridedMemoryView<const CFloat3>(&bbViews[0].m_BillboardingMatrix.StrippedTranslations(), bbViews.Count(), sizeof(bbViews[0]))
				);
		if (cull)
		{
			INC_DWORD_STAT_BY(STAT_PopcornFX_CulledDrawReqCount, 1);
			return true;
		}
	}

	// return false;

	const float			scale = FPopcornFXPlugin::GlobalScale();
	const FVector		ueOrigin = _Reinterpret<FVector>(origin) * scale;
	const FVector		ueExtent = _Reinterpret<FVector>(extent) * scale;
	const auto			&sceneViews = m_Views->SceneViews();
	for (hh_u32 viewi = 0; viewi < sceneViews.Count(); ++viewi)
	{
		if (!sceneViews[viewi].m_ToRender)
			continue;
		const FSceneView		*view = sceneViews[viewi].m_SceneView;
		bool					fullyContained;
		if (view->ViewFrustum.IntersectBox(ueOrigin, ueExtent, fullyContained))
			return false;
	}

	INC_DWORD_STAT_BY(STAT_PopcornFX_CulledDrawReqCount, 1);
	return true;
}

bool	CParticleRenderManager::MergeDrawCall(const PopcornFX::Drawers::SBillboard_DrawRequest &dr, const CParticleMaterialDesc_Game &matDesc)
{
	HH_NAMEDSCOPEDPROFILE("CParticleRenderManager::MergeDrawCall Billboard");

	if (!HH_VERIFY(matDesc.MaterialIsValid()))
		return false;

	const FPopcornFXSubRendererMaterial		*rendererSubMat = matDesc.m_RendererMaterial->GetSubMaterial(0);
	if (!HH_VERIFY(rendererSubMat != null))
		return false;
	UMaterialInstanceConstant				*materialInstance = matDesc.m_RendererMaterial->GetInstance(0, true);
	if (!HH_VERIFY(materialInstance != null))
		return false;

	CParticleMaterialDesc_Render			tmpRenderMaterialDesc;
	if (!tmpRenderMaterialDesc.SetupFromGame(matDesc) ||
		!tmpRenderMaterialDesc.ResolveMaterial(materialInstance, m_FeatureLevel))
		return false;

	SMaterialCompatFlags			compatFlags;
	CParticleDrawCall_Billboard::BuildCompatFlags(compatFlags, this, &tmpRenderMaterialDesc, &dr);

	const bool	isGPU = dr.StreamToRender_MainMemory() == null;
	HH_ASSERT(isGPU || dr.StreamToRender_MainMemory() != null);
	if (s_EnableMaterialMerge)
	{
		for (hh_u32 dcMatDesci = 0; dcMatDesci < m_BillboardDrawCallsMatDescs.Count(); ++dcMatDesci)
		{
			SMaterialDescMerged			&matDescMerge = m_BillboardDrawCallsMatDescs[dcMatDesci];

			// should never have been added
			HH_ASSERT(matDescMerge.m_MatDesc.MaterialIsValid());

			if (!compatFlags.IsCompatible(matDescMerge.m_CompatFlags))
				continue;

			if (tmpRenderMaterialDesc.m_RendererMaterial != matDescMerge.m_MatDesc.m_RendererMaterial)
			{
				const FPopcornFXSubRendererMaterial		*otherRendererSubMat = matDescMerge.m_MatDesc.m_RendererMaterial->GetSubMaterial(0);
				HH_ASSERT(otherRendererSubMat != null); // should not have been added !
				if (!rendererSubMat->RenderThread_SameMaterial_Billboard(*otherRendererSubMat))
					continue;
			}

			CParticleDrawCall_Billboard		&dc = m_BillboardDrawCalls[dcMatDesci];
			if (!dc.Add(&dr))
				continue;

			return true;
		}
	}

	// not merged, create new one

	SMaterialDescMerged				&matDescMerge = *m_BillboardDrawCallsMatDescs.PushBackReserved();
	matDescMerge.m_CompatFlags = compatFlags;
	matDescMerge.m_MatDesc = tmpRenderMaterialDesc;

	CParticleDrawCall_Billboard		&dc = *m_BillboardDrawCalls.PushBackReserved();

	HH_ASSERT(m_BillboardDrawCallsMatDescs.Count() == m_BillboardDrawCalls.Count());

	if (!dc.Init(this, &matDescMerge.m_MatDesc))
	{
		m_BillboardDrawCalls.PopBackAndDiscard();
		m_BillboardDrawCallsMatDescs.PopBackAndDiscard();
		return false;
	}

	HH_ONLY_IF_ASSERTS(const bool dcAddRes = )dc.Add(&dr);
	HH_ASSERT(dcAddRes);

	return true;
}

bool	CParticleRenderManager::MergeDrawCall(const PopcornFX::Drawers::SRibbon_DrawRequest &dr, const CParticleMaterialDesc_Game &matDesc)
{
	HH_NAMEDSCOPEDPROFILE("CParticleRenderManager::MergeDrawCall Ribbon");

	if (!HH_VERIFY(matDesc.MaterialIsValid()))
		return false;

	const FPopcornFXSubRendererMaterial		*rendererSubMat = matDesc.m_RendererMaterial->GetSubMaterial(0);
	if (!HH_VERIFY(rendererSubMat != null))
		return false;
	UMaterialInstanceConstant				*materialInstance = matDesc.m_RendererMaterial->GetInstance(0, true);
	if (!HH_VERIFY(materialInstance != null))
		return false;

	CParticleMaterialDesc_Render			tmpRenderMaterialDesc;
	if (!tmpRenderMaterialDesc.SetupFromGame(matDesc) ||
		!tmpRenderMaterialDesc.ResolveMaterial(materialInstance, m_FeatureLevel))
		return false;

	SMaterialCompatFlags			compatFlags;
	CParticleDrawCall_Ribbon::BuildCompatFlags(compatFlags, this, &tmpRenderMaterialDesc, &dr);

	if (s_EnableMaterialMerge)
	{
		for (hh_u32 dcMatDesci = 0; dcMatDesci < m_RibbonDrawCallsMatDescs.Count(); ++dcMatDesci)
		{
			SMaterialDescMerged		&matDescMerge = m_RibbonDrawCallsMatDescs[dcMatDesci];

			// should never have been added
			HH_ASSERT(matDescMerge.m_MatDesc.MaterialIsValid());

			if (!compatFlags.IsCompatible(matDescMerge.m_CompatFlags))
				continue;

			if (tmpRenderMaterialDesc.m_RendererMaterial != matDescMerge.m_MatDesc.m_RendererMaterial)
			{
				const FPopcornFXSubRendererMaterial		*otherRendererSubMat = matDescMerge.m_MatDesc.m_RendererMaterial->GetSubMaterial(0);
				HH_ASSERT(otherRendererSubMat != null); // should not have been added !
				if (!rendererSubMat->RenderThread_SameMaterial_Billboard(*otherRendererSubMat)) // Same params !
					continue;
			}

			CParticleDrawCall_Ribbon		&dc = m_RibbonDrawCalls[dcMatDesci];
			if (!dc.Add(&dr))
				continue;

			return true;
		}
	}

	// not merged, create new one

	SMaterialDescMerged				&matDescMerge = *m_RibbonDrawCallsMatDescs.PushBackReserved();
	matDescMerge.m_CompatFlags = compatFlags;
	matDescMerge.m_MatDesc = tmpRenderMaterialDesc;

	CParticleDrawCall_Ribbon		&dc = *m_RibbonDrawCalls.PushBackReserved();

	HH_ASSERT(m_RibbonDrawCallsMatDescs.Count() == m_RibbonDrawCalls.Count());

	if (!dc.Init(this, &matDescMerge.m_MatDesc))
	{
		m_RibbonDrawCalls.PopBackAndDiscard();
		m_RibbonDrawCallsMatDescs.PopBackAndDiscard();
		return false;
	}

	HH_ONLY_IF_ASSERTS(const bool dcAddRes = )dc.Add(&dr);
	HH_ASSERT(dcAddRes);

	return true;
}

//----------------------------------------------------------------------------
//
// Billboard
//
//----------------------------------------------------------------------------

bool	CParticleRenderManager::BuildDrawCalls_Billboard()
{
	HH_SCOPEDPROFILE();

	CParticleCollectedFrameToRender				&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView					&views = *m_Views;

	HH_ASSERT(frame.m_BillboardDrs.Count() == frame.m_BillboardMaterialDescs.Count());
	const hh_u32			drCount = frame.m_BillboardDrs.Count();
	m_BillboardDrawCalls.Clear();
	m_BillboardDrawCallsMatDescs.Clear();

	m_BillboardCPUBBPool.Clear();
	m_BillboardGPUBBPool.Clear();

	if (drCount == 0)
		return true;

	// We will keep pointers on those, must be reserved now
	if (!HH_VERIFY(m_BillboardDrawCalls.Reserve(drCount))) // at most
		return false;
	if (!HH_VERIFY(m_BillboardDrawCallsMatDescs.Reserve(drCount))) // at most
		return false;
	if (!HH_VERIFY(m_BillboardCPUBBPool.Reserve(drCount))) // at most
		return false;
	if (!HH_VERIFY(m_BillboardGPUBBPool.Reserve(drCount))) // at most
		return false;

	TMemoryView<const PopcornFX::Drawers::SBillboard_DrawRequest>	drawRequests = frame.m_BillboardDrs.View();

	INC_DWORD_STAT_BY(STAT_PopcornFX_DrawRequestsCount, drawRequests.Count());

	const bool		canCastShadows = m_Can_CastShadows & m_RenderSettings.bEnableBillboardShadows;

	HH_ASSERT(drawRequests.Count() == frame.m_BillboardMaterialDescs.Count());
	for (hh_u32 dri = 0; dri < drawRequests.Count(); ++dri)
	{
		const PopcornFX::Drawers::SBillboard_DrawRequest	&dr = drawRequests[dri];
		CParticleMaterialDesc_Game							&matDesc = frame.m_BillboardMaterialDescs[dri];
		// Patch up CastShadow
		matDesc.m_CastShadow &= canCastShadows;

		if (m_CullTest_DrawRequest && CullTest(dr, matDesc.m_CastShadow))
			continue;

		MergeDrawCall(dr, matDesc);
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleRenderManager::StartDrawCalls_Billboard()
{
	HH_SCOPEDPROFILE();

	const CParticleCollectedFrameToRender		&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView					&views = *m_Views;

	if (m_BillboardDrawCalls.Empty())
		return false;

	bool			hasDC = false;

	const bool		isShadowPass = views.RenderPass() == CRendererSubView::RenderPass_Shadow;

	for (hh_u32 dci = 0; dci < m_BillboardDrawCalls.Count(); ++dci)
	{
		CParticleDrawCall_Billboard			&dc = m_BillboardDrawCalls[dci];

		if (dc.Empty())
			continue;
		if (isShadowPass && !dc.CastShadow())
			continue;
		if (!dc.StartDrawCall(views, m_IsNewFrame))
		{
			dc.Clear();
			HH_ASSERT(static_cast<const CParticleAbstractDrawCall&>(dc).Empty());
			continue;
		}
		HH_ASSERT(m_BillboardDrawCalls.Count() == m_BillboardDrawCallsMatDescs.Count()); // those match
		hasDC = true; // Not sure about this when in GPU..
		if (dc.IsSolid())
			m_SolidDrawCalls.PushBack(&dc);
		else
			m_TransDrawCalls.PushBack(&dc);
	}

	if (hasDC)
		PopcornFX::Scheduler::ThreadPool()->KickTasks();
	return true;
}

//----------------------------------------------------------------------------
//
// Ribbon
//
//----------------------------------------------------------------------------

bool	CParticleRenderManager::BuildDrawCalls_Ribbon()
{
	HH_SCOPEDPROFILE();

	CParticleCollectedFrameToRender				&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView					&views = *m_Views;

	HH_ASSERT(frame.m_RibbonDrs.Count() == frame.m_RibbonMaterialDescs.Count());
	const hh_u32			drCount = frame.m_RibbonDrs.Count();
	m_RibbonDrawCalls.Clear();
	m_RibbonDrawCallsMatDescs.Clear();

	m_RibbonCPUBBPool.Clear();
	//m_RibbonGPUBBPool.Clear();

	if (drCount == 0)
		return true;

	// We will keep pointers on those, must be reserved now
	if (!HH_VERIFY(m_RibbonDrawCalls.Reserve(drCount))) // at most
		return false;
	if (!HH_VERIFY(m_RibbonDrawCallsMatDescs.Reserve(drCount))) // at most
		return false;
	if (!HH_VERIFY(m_RibbonCPUBBPool.Reserve(drCount))) // at most
		return false;
	//if (!HH_VERIFY(m_RibbonGPUBBPool.Reserve(drCount))) // at most
	//	return false;

	TMemoryView<const PopcornFX::Drawers::SRibbon_DrawRequest>	drawRequests = frame.m_RibbonDrs.View();

	INC_DWORD_STAT_BY(STAT_PopcornFX_DrawRequestsCount, drawRequests.Count());

	const bool		canCastShadows = m_Can_CastShadows & m_RenderSettings.bEnableRibbonShadows;

	HH_ASSERT(drawRequests.Count() == frame.m_RibbonMaterialDescs.Count());
	for (hh_u32 dri = 0; dri < drawRequests.Count(); ++dri)
	{
		const PopcornFX::Drawers::SRibbon_DrawRequest	&dr = drawRequests[dri];
		CParticleMaterialDesc_Game						&matDesc = frame.m_RibbonMaterialDescs[dri];
		// Patch up CastShadow
		matDesc.m_CastShadow &= canCastShadows;

		if (m_CullTest_DrawRequest && CullTest(dr, matDesc.m_CastShadow))
			continue;

		MergeDrawCall(dr, matDesc);
	}

	return true;
}

bool	CParticleRenderManager::StartDrawCalls_Ribbon()
{
	HH_SCOPEDPROFILE();

	const CParticleCollectedFrameToRender	&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView				&views = *m_Views;

	if (m_RibbonDrawCalls.Empty())
		return false;

	bool			hasDC = false;

	for (hh_u32 dci = 0; dci < m_RibbonDrawCalls.Count(); ++dci)
	{
		CParticleDrawCall_Ribbon			&dc = m_RibbonDrawCalls[dci];
		if (dc.Empty())
			continue;
		if (views.RenderPass() == CRendererSubView::RenderPass_Shadow && !dc.CastShadow())
			continue;
		if (!dc.StartDrawCall(views, m_IsNewFrame))
		{
			dc.Clear();
			HH_ASSERT(static_cast<const CParticleAbstractDrawCall&>(dc).Empty());
			continue;
		}
		HH_ASSERT(m_RibbonDrawCalls.Count() == m_RibbonDrawCallsMatDescs.Count()); // those match
		hasDC = true;
		if (dc.IsSolid())
			m_SolidDrawCalls.PushBack(&dc);
		else
			m_TransDrawCalls.PushBack(&dc);
	}

	if (hasDC)
		PopcornFX::Scheduler::ThreadPool()->KickTasks();

	return true;
}

//----------------------------------------------------------------------------
//
// Mesh
//
//----------------------------------------------------------------------------

bool	CParticleRenderManager::BuildDrawCalls_Mesh()
{
	HH_SCOPEDPROFILE();

	CParticleCollectedFrameToRender		&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView			&views = *m_Views;

	HH_ASSERT(frame.m_MeshDescMaterialDescs.Count() >= frame.m_MeshDrs.Count());

	// Mesh Draw Calls are per *Mesh Descriptor* ! (not per draw request)

	const hh_u32			totalMeshDescCount = frame.m_MeshDescMaterialDescs.Count();
	const hh_u32			drCount = frame.m_MeshDrs.Count();

	m_MeshSingleSectionDrawCalls.Clear();
	m_MeshSingleSectionMatDescs.Clear();
	m_MeshMultiSectionDrawCalls.Clear();
	m_MeshMultiSectionMatDescs.Clear();
	m_MeshCPUBB.Clear();

	if (totalMeshDescCount == 0)
		return true;

	// Pre-Reserve everything, so pointer can be kept safely
	if (!m_MeshSingleSectionDrawCalls.Reserve(totalMeshDescCount))
		return false;
	if (!m_MeshSingleSectionMatDescs.Reserve(totalMeshDescCount))
		return false;
	if (!m_MeshMultiSectionDrawCalls.Reserve(frame.m_MaxMultiSectionMeshDrawCallCount))
		return false;
	if (!m_MeshMultiSectionMatDescs.Reserve(frame.m_MaxMultiSectionMeshDrawCallCount))
		return false;
	// 1 BB per DR
	if (!m_MeshCPUBB.Reserve(drCount))
		return false;

	const bool				canCastShadows = m_Can_CastShadows & m_RenderSettings.bEnableMeshShadows;

	hh_u32					matDescOffset = 0;
	hh_u32					matDescOffsetNext = 0;

	for (hh_u32 dri = 0; dri < drCount; ++dri)
	{
		matDescOffset = matDescOffsetNext;
		const hh_u32		meshCount = frame.m_MeshDrs[dri].m_MeshDescs.Count();
		matDescOffsetNext += meshCount; // avoid += miss

		INC_DWORD_STAT_BY(STAT_PopcornFX_DrawRequestsCount, meshCount);

		bool				hasAnyShadow = false;
		for (hh_u32 meshDesci = 0; meshDesci < meshCount; ++meshDesci)
		{
			CParticleMaterialDesc_MeshDesc_Game			&matDesc = frame.m_MeshDescMaterialDescs[matDescOffset + meshDesci];
			HH_ASSERT(matDesc.MeshDescId() == meshDesci);
			// Patch up CastShadow
			matDesc.m_CastShadow &= canCastShadows;
			if (matDesc.m_CastShadow)
				hasAnyShadow = true;
		}
		// CullTest
		if (m_CullTest_DrawRequest && CullTest(frame.m_MeshDrs[dri], hasAnyShadow))
			continue;

		// 1 BB per DR
		CParticleBillboarder_Mesh_CPU	&bb = *m_MeshCPUBB.PushBackReserved();
		if (!bb.Prepare(this, frame.m_MeshDrs[dri]))
		{
			m_MeshCPUBB.PopBackAndDiscard();
			continue;
		}

		bool		culled = false;
		bool		culledShadow = false;
		bool		hasSomethingToRender = true;

		const PopcornFX::Drawers::CMesh_CPU				&cpubb = bb.m_BB;
		HH_ASSERT(cpubb.DrawRequests().Count() == 1);
		HH_ASSERT(cpubb.MeshCount() == meshCount);
		const PopcornFX::Drawers::SMesh_DrawRequest		&dr = cpubb.DrawRequests()[0];
		HH_ASSERT(dr.m_MeshDescs.Count() == meshCount);
		if (cpubb.HasMeshIds())
		{
			// Do not emit draw calls for empty mesh descs
			TMemoryView<const hh_u32>	perMeshCount = cpubb.PerMeshParticleCount();
			HH_ASSERT(dr.m_MeshDescs.Count() == frame.m_MeshDrs[dri].m_MeshDescs.Count());
			for (hh_u32 meshDesci = 0; meshDesci < meshCount; ++meshDesci)
			{
				if (perMeshCount[meshDesci] == 0)
					continue;
				const CParticleMaterialDesc_MeshDesc_Game			&matDesc = frame.m_MeshDescMaterialDescs[matDescOffset + meshDesci];
				HH_ASSERT(matDesc.MeshDescId() == meshDesci);
				hasSomethingToRender |= _StartRender_MeshDesc(bb, matDesc, meshDesci);
			}
		}
		else
		{
			HH_ASSERT(meshCount == 1);
			const hh_u32			meshDesci = 0;
			const CParticleMaterialDesc_MeshDesc_Game			&matDesc = frame.m_MeshDescMaterialDescs[matDescOffset + meshDesci];
			HH_ASSERT(matDesc.MeshDescId() == meshDesci);
			hasSomethingToRender |= _StartRender_MeshDesc(bb, matDesc, meshDesci);
		}
		if (!hasSomethingToRender)
		{
			m_MeshCPUBB.PopBackAndDiscard();
			continue;
		}
	}

	for (hh_u32 bbi = 0; bbi < m_MeshCPUBB.Count(); ++bbi)
	{
		if (!m_MeshCPUBB[bbi].LaunchBillboarding(views))
			m_MeshCPUBB[bbi].Clear();
	}

	PopcornFX::Scheduler::ThreadPool()->KickTasks();

	return true;
}

bool	CParticleRenderManager::StartDrawCalls_Mesh()
{
	HH_SCOPEDPROFILE();

	const CParticleCollectedFrameToRender		&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView					&views = *m_Views;

	if (m_MeshSingleSectionDrawCalls.Empty() && m_MeshMultiSectionDrawCalls.Empty())
		return false;

	for (hh_u32 dci = 0; dci < m_MeshSingleSectionDrawCalls.Count(); ++dci)
	{
		CParticleDrawCall_MeshSingleSection		&dc = m_MeshSingleSectionDrawCalls[dci];
		if (dc.Empty())
			continue;
		if (views.RenderPass() == CRendererSubView::RenderPass_Shadow && !dc.CastShadow())
			continue;
		//if (!dc.StartDrawCall(views, m_IsNewFrame))
		//{
		//	dc.Clear();
		//	HH_ASSERT(static_cast<const CParticleAbstractDrawCall&>(dc).Empty());
		//	continue;
		//}
		if (dc.IsSolid())
			m_SolidDrawCalls.PushBack(&dc);
		else
			m_TransDrawCalls.PushBack(&dc);
	}

	for (hh_u32 dci = 0; dci < m_MeshMultiSectionDrawCalls.Count(); ++dci)
	{
		CParticleDrawCall_MeshMultiSection		&dc = m_MeshMultiSectionDrawCalls[dci];
		if (dc.Empty())
			continue;
		if (views.RenderPass() == CRendererSubView::RenderPass_Shadow && !dc.CastShadow())
			continue;
		//if (!dc.StartDrawCall(views, m_IsNewFrame))
		//{
		//	dc.Clear();
		//	HH_ASSERT(static_cast<const CParticleAbstractDrawCall&>(dc).Empty());
		//	continue;
		//}
		if (dc.IsSolid())
			m_SolidDrawCalls.PushBack(&dc);
		else
			m_TransDrawCalls.PushBack(&dc);
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleRenderManager::_StartRender_MeshDesc(CParticleBillboarder_Mesh_CPU &bb, const CParticleMaterialDesc_MeshDesc_Game &originalMatDesc, hh_u32 meshDesci)
{
	const CParticleCollectedFrameToRender			&frame = *m_Rendering_LockMe;

	const PopcornFX::Drawers::CMesh_CPU				&cpubb = bb.m_BB;
	const hh_u32									meshCount = cpubb.MeshCount();
	const PopcornFX::Drawers::SMesh_DrawRequest		&dr = cpubb.DrawRequests()[0];
	HH_ASSERT(dr.m_MeshDescs.Count() == meshCount);

	HH_ASSERT(originalMatDesc.MeshDescId() == meshDesci);

	if (!HH_VERIFY(originalMatDesc.MaterialIsValid()))
		return false;

	const FPopcornFXSubRendererMaterial				*rendererSubMat = originalMatDesc.m_RendererMaterial->GetSubMaterial(meshDesci);
	if (!HH_VERIFY(rendererSubMat != null))
		return false;


	UStaticMesh				*staticMesh = rendererSubMat->StaticMesh;
	if (!HH_VERIFY(staticMesh != null) ||
		!HH_VERIFY(staticMesh->HasValidRenderData()))
		return false;
	HH_ASSERT(staticMesh->RenderData != null);

	const int32				_LODIndex = rendererSubMat->StaticMeshLOD + m_RenderSettings.MeshParticlesLODBias;

	check(staticMesh->RenderData->LODResources.Num() > 0);
	const hh_u32			LODIndex = PopcornFX::HHClamp(_LODIndex, 0, staticMesh->RenderData->LODResources.Num() - 1);

	const FStaticMeshLODResources	&renderResources = staticMesh->RenderData->LODResources[LODIndex];

	UMaterialInstanceConstant	*rendererMatMaterialInstance = originalMatDesc.m_RendererMaterial->GetInstance(meshDesci, true);
	//// /!\ Important: or it will crash later in UE
	//HH_TODO("The Draw Call should check that !");
	//if (rendererMatMaterialInstance != null &&
	//	!HH_VERIFY(FPopcornFXMeshVertexFactory::IsCompatible(rendererMatMaterialInstance)))
	//{
	//	HH_TODO("Message incompatible material !");
	//	rendererMatMaterialInstance = null;
	//	// return; // let it fall back on default material
	//}

	const hh_u32	totalSectionCount = renderResources.Sections.Num();
	HH_ASSERT(totalSectionCount > 0);
	hh_u32			renderSectionStart = 0;
	hh_u32			renderSectionCount = 0;
	if (originalMatDesc.m_StaticMeshSubMeshId < 0)
	{
		renderSectionStart = 0;
		renderSectionCount = totalSectionCount;
	}
	else
	{
		renderSectionStart = PopcornFX::HHMin(hh_u32(originalMatDesc.m_StaticMeshSubMeshId), totalSectionCount - 1);
		renderSectionCount = 1;
	}

	HH_TODO("Multi sections but Single material could be render as Single !?");

	if (renderSectionCount == 1)
	{
		// Single Section Draw Call

		UMaterialInterface		*finalMaterialInterface = null;
		bool					usingDefaultMeshMaterial;

		if (rendererMatMaterialInstance != null)
		{
			usingDefaultMeshMaterial = false;
			finalMaterialInterface = rendererMatMaterialInstance;
			// compat already checked
		}
		else
		{
			usingDefaultMeshMaterial = true;
			finalMaterialInterface = staticMesh->GetMaterial(renderResources.Sections[renderSectionStart].MaterialIndex);
			HH_ASSERT(finalMaterialInterface != null);
			//// /!\ Important: or it will crash later in UE
			//HH_TODO("The Draw Call should check that !");
			//if (finalMaterialInterface != null &&
			//	!HH_VERIFY(FPopcornFXMeshVertexFactory::IsCompatible(finalMaterialInterface))) // !! IMPORTANT, or will crash later
			//{
			//	HH_TODO("Message incompatible material !");
			//	finalMaterialInterface = null;
			//}
		}
		if (!HH_VERIFY(finalMaterialInterface != null))
			return false;

		HH_ASSERT(m_MeshSingleSectionDrawCalls.Count() == m_MeshSingleSectionMatDescs.Count());

		// Find mergeable draw call

		PopcornFX::CGuid		drawCalli;
		if (s_EnableMaterialMerge)
		{
			for (hh_u32 matDesci = 0; matDesci < m_MeshSingleSectionMatDescs.Count(); ++matDesci)
			{
				const CParticleMaterialDesc_MeshSingleSection_Render	&otherMatDesc = m_MeshSingleSectionMatDescs[matDesci].m_MatDesc;
				if (otherMatDesc.m_StaticMesh != staticMesh ||
					otherMatDesc.m_LODIndex != LODIndex ||
					otherMatDesc.m_StaticMeshSingleSectionId != renderSectionStart)
					continue;
				HH_ASSERT(otherMatDesc.m_RenderResources == &renderResources);

				// should never have been added
				HH_ASSERT(otherMatDesc.MaterialIsValid());

				if (finalMaterialInterface != otherMatDesc.m_MaterialInterface)
					continue;
				// if one XOR the other use a default UStaticMesh material, dont merge them
				if (usingDefaultMeshMaterial != otherMatDesc.m_IsDefaultMeshMaterial)
					continue;
				if (!usingDefaultMeshMaterial && !otherMatDesc.m_IsDefaultMeshMaterial)
				{
					const FPopcornFXSubRendererMaterial		*otherRendererSubMat = otherMatDesc.m_RendererMaterial->GetSubMaterial(otherMatDesc.MeshDescId());
					HH_ASSERT(otherRendererSubMat != null); // should not have been added !
					if (!rendererSubMat->RenderThread_SameMaterial_ParamsOnly_Mesh(*otherRendererSubMat))
						continue;
				}
				else
				{
					HH_ASSERT(usingDefaultMeshMaterial && otherMatDesc.m_IsDefaultMeshMaterial);
					// they both are using default StaticMaterial
					// we don't touch their material parameters, so there are always compatible
				}

				drawCalli = matDesci;
				break;
			}
		}

		if (!drawCalli.Valid())
		{
			HH_ASSERT(m_MeshSingleSectionDrawCalls.Count() == m_MeshSingleSectionMatDescs.Count()); // those match

			drawCalli = m_MeshSingleSectionDrawCalls.Count();
			SMaterialDescMerged_Mesh				&mergedDesc = *m_MeshSingleSectionMatDescs.PushBackReserved();
			mergedDesc.m_IsGPU = false;
			HH_ASSERT(meshDesci == originalMatDesc.MeshDescId());
			if (!mergedDesc.m_MatDesc.SetupFromGame(originalMatDesc) ||
				!mergedDesc.m_MatDesc.ResolveMaterial(finalMaterialInterface, m_FeatureLevel))
			{
				m_MeshSingleSectionMatDescs.PopBackAndDiscard();
				return false;
			}

			mergedDesc.m_MatDesc.m_IsDefaultMeshMaterial = usingDefaultMeshMaterial;
			mergedDesc.m_MatDesc.m_StaticMesh = staticMesh;
			mergedDesc.m_MatDesc.m_RenderResources = &renderResources;
			mergedDesc.m_MatDesc.m_LODIndex = LODIndex;
			mergedDesc.m_MatDesc.m_StaticMeshSingleSectionId = renderSectionStart;

			CParticleDrawCall_MeshSingleSection		&dc = *m_MeshSingleSectionDrawCalls.PushBackReserved();
			if (!dc.Init(this, &mergedDesc.m_MatDesc))
			{
				m_MeshSingleSectionMatDescs.PopBackAndDiscard();
				m_MeshSingleSectionDrawCalls.PopBackAndDiscard();
				return false;
			}
		}

		CParticleDrawCall_MeshSingleSection			&dc = m_MeshSingleSectionDrawCalls[drawCalli];
		dc.AssocBillboader(&bb, meshDesci);
	}
	else
	{
		// Multi Section Draw Call

		HH_ASSERT(m_MeshMultiSectionMatDescs.Count() == m_MeshMultiSectionDrawCalls.Count());

		CParticleMaterialDesc_MeshMultiSection_Render	&matDesc = *m_MeshMultiSectionMatDescs.PushBackReserved();

		if (!matDesc.SetupFromGame(originalMatDesc))
		{
			m_MeshMultiSectionMatDescs.PopBackAndDiscard();
			return false;
		}
		if (rendererMatMaterialInstance != null &&
			!matDesc.ResolveMaterial(rendererMatMaterialInstance, m_FeatureLevel))
		{
			// failed, but should later fallback on StaticMesh own Materials
		}

		matDesc.m_StaticMesh = staticMesh;
		matDesc.m_RenderResources = &renderResources;
		matDesc.m_LODIndex = LODIndex;
		matDesc.m_SectionStart = renderSectionStart;
		HH_ASSERT(renderSectionCount > 1);
		matDesc.m_SectionCount = renderSectionCount;

		CParticleDrawCall_MeshMultiSection		&dc = *m_MeshMultiSectionDrawCalls.PushBackReserved();

		if (!dc.Init(this, &bb, meshDesci, &matDesc))
		{
			m_MeshMultiSectionMatDescs.PopBackAndDiscard();
			m_MeshMultiSectionDrawCalls.PopBackAndDiscard();
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------
//
// Lights
//
//----------------------------------------------------------------------------

bool	CParticleRenderManager::BuildDrawCalls_Light()
{
	HH_SCOPEDPROFILE();

	CParticleCollectedFrameToRender				&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView					&views = *m_Views;

	return ParticleDrawCall_GatherLights(this, views, frame.m_LightDrs);
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

static uint64		_DrawOrderDepthToKey(int32 drawOrder, float depth)
{
	union { float f; int32 i; uint32 u; } fui;
	fui.f = depth;
	const uint32	signMask = fui.i >> 31;
	const uint32	key = fui.u ^ (signMask | 0x80000000U);
	const uint64	k = (uint64(0x7FFFFFFFU - uint32(drawOrder)) << 32) | uint64(key);
	return k;
}

struct SDrawCallSliceDraw
{
	uint64							m_Key;
	CParticleAbstractDrawCall		*m_DrawCall;
	uint32		m_ElementStart;
	uint32		m_ElementCount;
	HH_FORCEINLINE bool		operator < (const SDrawCallSliceDraw &other) const { return m_Key > other.m_Key; } // back to front
	HH_FORCEINLINE bool		operator <= (const SDrawCallSliceDraw &other) const { return m_Key >= other.m_Key; }
	HH_FORCEINLINE bool		operator == (const SDrawCallSliceDraw &other) const { return m_Key == other.m_Key; }
};

void	CParticleRenderManager::CallDrawCalls()
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleRenderManager::CallDrawCalls", POPCORNFX_UE_PROFILER_COLOR);

	const CParticleCollectedFrameToRender		&frame = *m_Rendering_LockMe;
	PopcornFX::CRendererSubView					&views = *m_Views;
	const auto									&bbViews = views.BBViews();

#if POPCORNFX_RENDER_DEBUG
	if (frame.m_OverrideAllMaterial)
	{
		m_OverrideAllMaterialRenderProxy = frame.m_OverrideAllMaterial->GetRenderProxy(false, false);
	}
	else if (m_OverrideAllMaterialRenderProxy != null)
		m_OverrideAllMaterialRenderProxy = null;

	bool		debugColorDrawCalls = frame.m_OverrideColorByDrawCall;
	if (debugColorDrawCalls &&
		(!m_DebugColorsVertexBuffer.IsInitialized() || m_DebugColorsBuffer.Num() != frame.m_OverrideDebugColorCount))
	{
		HH_ASSERT(frame.m_OverrideDebugColorCount >= 2);
		m_DebugColorsBuffer.SetNumUninitialized(frame.m_OverrideDebugColorCount);
		m_DebugColorsBuffer.SetAllowCPUAccess(true); // mesh "cpu" instances can acces that

		for (int32 i = 0; i < m_DebugColorsBuffer.Num(); ++i)
			m_DebugColorsBuffer[i] = FLinearColor(360.f * float(i) / float(m_DebugColorsBuffer.Num()), 1.f, 1.f, 1.f).HSVToLinearRGB();
		FRHIResourceCreateInfo			createInfo(&m_DebugColorsBuffer);
		m_DebugColorsVertexBuffer.VertexBufferRHI = RHICreateVertexBuffer(m_DebugColorsBuffer.GetResourceDataSize(), BUF_Static | BUF_ZeroStride, createInfo);
		m_DebugColorsVertexBuffer.InitResource();

		debugColorDrawCalls = HH_VERIFY(m_DebugColorsVertexBuffer.IsInitialized() && m_DebugColorsVertexBuffer.VertexBufferRHI != null);
	}

#endif // POPCORNFX_RENDER_DEBUG


	bool		hasSolid = !m_SolidDrawCalls.Empty();
	bool		hasTrans = !m_TransDrawCalls.Empty();

	const bool		sortDrawCalls = (m_RenderSettings.DrawCallSortMethod == EPopcornFXDrawCallSortMethod::PerDrawCalls);
	const bool		sortSlicedDrawCalls = (m_RenderSettings.DrawCallSortMethod == EPopcornFXDrawCallSortMethod::PerSlicedDrawCalls);

	hh_u32		totalDrawCalls = 0;

	totalDrawCalls += m_SolidDrawCalls.Count();

	PopcornFX::TArray<SDrawCallSliceDraw>		allDrawCallSlices;

	if (hasTrans && sortDrawCalls)
	{
		HH_NAMEDSCOPEDPROFILE("Build Projections");
		for (hh_u32 bbViewi = 0; bbViewi < bbViews.Count(); ++bbViewi)
		{
			SProjections		&projs = m_ViewsProjections[bbViewi];
			projs.m_Depths.Clear();
			HH_ASSERT(bbViews[bbViewi].m_ViewsMask > 0);
			const CFloat3		viewPos = bbViews[bbViewi].m_BillboardingMatrix.StrippedTranslations();
			const CFloat3		viewVec = -bbViews[bbViewi].m_BillboardingMatrix.StrippedZAxis();

			if (!projs.m_Depths.Reserve(m_TransDrawCalls.Count()))
				continue;

			for (hh_u32 dci = 0; dci < m_TransDrawCalls.Count(); ++dci)
			{
				CParticleAbstractDrawCall		&dc = *(m_TransDrawCalls[dci]);
				if (dc.Empty())
					continue;
				HH_ASSERT(dc.m_DC_TotalElementCount > 0);
				IF_DEBUG_BBOX(HH_ASSERT(dc.m_DC_TotalBBox.IsFinite() && dc.m_DC_TotalBBox.Valid());)
					const PopcornFX::CAABB		&bb = dc.m_DC_TotalBBox;

				// closest depth
				//const CFloat3				viewToCenter = viewPos - bb.Center();
				//const CFloat3				absv = PopcornFX::HHAbs(viewToCenter);
				//const CFloat3				distVec = absv - PopcornFX::HHMin(absv, bb.Extent() * 0.5f);
				//const float					nearDepth = distVec.LengthSquared();

				const CFloat3				nearCorner(
					viewVec.x() < 0.0f ? bb.Max().x() : bb.Min().x(),
					viewVec.y() < 0.0f ? bb.Max().y() : bb.Min().y(),
					viewVec.z() < 0.0f ? bb.Max().z() : bb.Min().z()
					);
				const float					nearDepth = viewVec.Dot(nearCorner - viewPos);

				//if (distSq < 10e-5f) // inside the bbox
				//	distSq = viewToCenter.LengthSquared(); // sort by center
				const hh_i32				drawOrder = dc.m_DC_DrawOrder;
				projs.m_Depths.PushBack(SProjection(_DrawOrderDepthToKey(drawOrder, nearDepth), &dc));
			}

			PopcornFX::QuickSort(projs.m_Depths.Begin(), projs.m_Depths.End());

			totalDrawCalls += projs.m_Depths.Count();
		}
	}
	else if (hasTrans && sortSlicedDrawCalls)
	{
		HH_NAMEDSCOPEDPROFILE("Build Slices");

		const CFloat3		viewPos = bbViews[0].m_BillboardingMatrix.StrippedTranslations();
		const CFloat3		viewVec = -bbViews[0].m_BillboardingMatrix.StrippedZAxis();

		for (hh_u32 dci = 0; dci < m_TransDrawCalls.Count(); ++dci)
		{
			const hh_u32					baseTotalSlicesCount = allDrawCallSlices.Count();
			CParticleAbstractDrawCall		&dc = *(m_TransDrawCalls[dci]);
			if (dc.Empty())
				continue;
			PopcornFX::TMemoryView<const SDrawCallSlice>	slices;
			if (!dc.GetSlices(views, ~hh_u32(0), slices))
			{
				// Slicing failed !
				// Full slice draw request
				const PopcornFX::CGuid		i = allDrawCallSlices.PushBack();
				if (HH_VERIFY(i.Valid()))
				{
					SDrawCallSliceDraw			&dstSlice = allDrawCallSlices[i];

					const PopcornFX::CAABB		&bb = dc.m_DC_TotalBBox;
					const CFloat3				nearCorner(
						viewVec.x() < 0.0f ? bb.Max().x() : bb.Min().x(),
						viewVec.y() < 0.0f ? bb.Max().y() : bb.Min().y(),
						viewVec.z() < 0.0f ? bb.Max().z() : bb.Min().z()
						);
					const float					nearDepth = viewVec.Dot(nearCorner - viewPos);

					const hh_i32				drawOrder = dc.m_DC_DrawOrder;
					dstSlice.m_Key = _DrawOrderDepthToKey(drawOrder, nearDepth);
					dstSlice.m_DrawCall = &dc;
					dstSlice.m_ElementStart = 0;
					dstSlice.m_ElementCount = dc.m_DC_TotalElementCount;
				}
			}
			else if (!slices.Empty() && // can be legit
				HH_VERIFY(allDrawCallSlices.Resize(baseTotalSlicesCount + slices.Count())))
			{
				for (hh_u32 i = 0; i < slices.Count(); ++i)
				{
					const SDrawCallSlice	&srcSlice = slices[i];
					SDrawCallSliceDraw		&dstSlice = allDrawCallSlices[baseTotalSlicesCount + i];
					const hh_i32			drawOrder = dc.m_DC_DrawOrder;
					dstSlice.m_Key = _DrawOrderDepthToKey(drawOrder, srcSlice.m_Depth);
					dstSlice.m_DrawCall = &dc;
					dstSlice.m_ElementStart = srcSlice.m_ElementStart;
					dstSlice.m_ElementCount = srcSlice.m_ElementCount;
					HH_ASSERT(dstSlice.m_ElementStart + dstSlice.m_ElementCount <= dc.m_DC_TotalElementCount);
				}
			}
		}
		{
			HH_NAMEDSCOPEDPROFILE("Sort And Merge Slices");

			PopcornFX::QuickSort(allDrawCallSlices.Begin(), allDrawCallSlices.End());

			// TODO multi-views
			// TODO do not split if non-sort dependent drawcall (additive) is in between (less draw call, wont change render)

			// Merge draw calls
			hh_u32				newSliceCount = 0;
			for (hh_u32 i = 0; i < allDrawCallSlices.Count(); ++i)
			{
				// (copy)
				SDrawCallSliceDraw		merged = allDrawCallSlices[i];

				// merge contiguous slice of the same draw call
				if (s_EnableDrawCallSlicesMerge && i + 1 < allDrawCallSlices.Count())
				{
					// Sort is not stable
					hh_u32		end = merged.m_ElementStart + merged.m_ElementCount;
					hh_u32		j = i + 1;
					for (; j < allDrawCallSlices.Count() && allDrawCallSlices[j].m_DrawCall == merged.m_DrawCall; ++j)
					{
						merged.m_ElementStart = PopcornFX::HHMin(merged.m_ElementStart, allDrawCallSlices[j].m_ElementStart);
						end = PopcornFX::HHMax(end, allDrawCallSlices[j].m_ElementStart + allDrawCallSlices[j].m_ElementCount);
						merged.m_ElementCount += allDrawCallSlices[j].m_ElementCount;
					}
					HH_ASSERT(merged.m_ElementStart < end);
					// Should not happen, means there are missing slices
					if (HH_VERIFY(end - merged.m_ElementStart == merged.m_ElementCount))
					{
						i = j - 1; // so next one will be j
					}
				}

				allDrawCallSlices[newSliceCount++] = merged;
			}
			allDrawCallSlices.Resize(newSliceCount);
			totalDrawCalls += newSliceCount;
		}
	}
	else
	{
		totalDrawCalls += m_TransDrawCalls.Count();
	}

	//
	// Actual DrawCalls:
	//


#if POPCORNFX_RENDER_DEBUG
	hh_u32			drawCallId = 0;
#	define DEBUG_PRE_DRAWCALL()	do {																\
			if (debugColorDrawCalls)																\
			{																						\
				HH_ASSERT(drawCallId < totalDrawCalls);												\
				hh_u32		frontToBacki = (totalDrawCalls - drawCallId) - 1;						\
				++drawCallId;																		\
				m_OverrideColorBufferOffset = (frontToBacki % m_DebugColorsBuffer.Num()) * 0x10;	\
				m_OverrideColorVertexBuffer = &m_DebugColorsVertexBuffer;							\
				m_OverrideColorBuffer = m_DebugColorsBuffer.GetResourceData();						\
			}																						\
		} while (0)
#else
#	define DEBUG_PRE_DRAWCALL()
#endif

	if (hasSolid)
	{
		HH_NAMEDSCOPEDPROFILE("DrawCall Solid");
		const hh_u32			fullBBViewMask = ~hh_u32(0);
		for (hh_u32 dci = 0; dci < m_SolidDrawCalls.Count(); ++dci)
		{
			CParticleAbstractDrawCall			&dc = *m_SolidDrawCalls[dci];
			if (dc.Empty())
				continue;
			HH_ASSERT(dc.m_DC_TotalElementCount > 0);
			DEBUG_PRE_DRAWCALL();
			dc.DrawCall(views, fullBBViewMask, 0, dc.m_DC_TotalElementCount);
		}
	}

	if (hasTrans)
	{
		if (sortDrawCalls)
		{
			HH_NAMEDSCOPEDPROFILE("DrawCall Trans SortDC");
			for (hh_u32 bbViewi = 0; bbViewi < bbViews.Count(); ++bbViewi)
			{
				SProjections		&projs = m_ViewsProjections[bbViewi];
				if (projs.m_Depths.Empty())
					continue;
				for (hh_u32 depthi = 0; depthi < projs.m_Depths.Count(); ++depthi)
				{
					CParticleAbstractDrawCall			&dc = *projs.m_Depths[depthi].m_DrawCall;
					if (dc.Empty())
						continue;
					HH_ASSERT(dc.m_DC_TotalElementCount > 0);
					DEBUG_PRE_DRAWCALL();
					dc.DrawCall(views, (1U << bbViewi), 0, dc.m_DC_TotalElementCount);
				}
			}
		}
		else if (sortSlicedDrawCalls)
		{
			HH_NAMEDSCOPEDPROFILE("DrawCall Trans SortSlices");
			const hh_u32	fullBBViewMask = ~hh_u32(0);

			for (hh_u32 i = 0; i < allDrawCallSlices.Count(); ++i)
			{
				SDrawCallSliceDraw		&slice = allDrawCallSlices[i];
				if (!HH_VERIFY(slice.m_ElementCount > 0))
					continue;
				DEBUG_PRE_DRAWCALL();
				slice.m_DrawCall->DrawCall(views, fullBBViewMask, slice.m_ElementStart, slice.m_ElementCount);
			}
		}
		else
		{
			HH_NAMEDSCOPEDPROFILE("DrawCall Trans SortNone");
			const hh_u32	fullBBViewMask = ~hh_u32(0);
			for (hh_u32 dci = 0; dci < m_TransDrawCalls.Count(); ++dci)
			{
				CParticleAbstractDrawCall		&dc = *(m_TransDrawCalls[dci]);
				if (dc.Empty())
					continue;
				HH_ASSERT(dc.m_DC_TotalElementCount > 0);
				DEBUG_PRE_DRAWCALL();
				dc.DrawCall(views, fullBBViewMask, 0, dc.m_DC_TotalElementCount);
			}
		}
	}

#if POPCORNFX_RENDER_DEBUG
	if (debugColorDrawCalls)
	{
		HH_ASSERT(drawCallId == totalDrawCalls);
		m_OverrideColorVertexBuffer = null;
		m_OverrideColorBuffer = null;
	}

	if (views.RenderPass() == CRendererSubView::RenderPass_Main &&
		frame.m_HeavyDebugMode != EPopcornFXHeavyDebugMode::None)
	{
		HH_NAMEDSCOPEDPROFILE("DrawHeavyDebug");

		const EPopcornFXHeavyDebugMode::Type	debugMode = static_cast<EPopcornFXHeavyDebugMode::Type>(frame.m_HeavyDebugMode);
		uint32					debugModeMask = 0;
		switch (debugMode)
		{
		case EPopcornFXHeavyDebugMode::None: break;
		case EPopcornFXHeavyDebugMode::ParticlePoints: debugModeMask |= EHeavyDebugModeMask::Particles; break;
		case EPopcornFXHeavyDebugMode::MediumsBBox: debugModeMask |= EHeavyDebugModeMask::Mediums; break;
		case EPopcornFXHeavyDebugMode::PagesBBox: debugModeMask |= EHeavyDebugModeMask::Pages; break;
		}
		const FPopcornFXSceneProxy		*sceneProxy = views.SceneProxy();
		for (hh_u32 viewi = 0; viewi < views.SceneViews().Count(); ++viewi)
		{
			if (views.SceneViews()[viewi].m_ToRender)
				DrawHeavyDebug(sceneProxy, views.Collector()->GetPDI(viewi), views.SceneViews()[viewi].m_SceneView, debugModeMask);
		}
	}
#endif

	return;
}

//----------------------------------------------------------------------------

#if POPCORNFX_RENDER_DEBUG

namespace
{
	inline FLinearColor		_MediumColorFromIndex(uint32 index)
	{
		return FLinearColor(FGenericPlatformMath::Frac(float(index) * 0.07777f) * 360.f, 0.9f, 0.9f, 0.8f).HSVToLinearRGB();
	}

	void	_RenderBoundsColor(FPrimitiveDrawInterface* PDI, const FBoxSphereBounds& Bounds, const FLinearColor &color)
	{
		const ESceneDepthPriorityGroup	DepthPriority = SDPG_Foreground;
		const FColor					Color = color.ToFColor(false);
		const FBox						Box = Bounds.GetBox();
		const float						thickness = 2.f;
		FVector	B[2], P, Q;
		int32 i, j;
		B[0] = Box.Min;
		B[1] = Box.Max;
		for (i = 0; i<2; i++) for (j = 0; j<2; j++)
		{
			P.X = B[i].X; Q.X = B[i].X;
			P.Y = B[j].Y; Q.Y = B[j].Y;
			P.Z = B[0].Z; Q.Z = B[1].Z;
			PDI->DrawLine(P, Q, Color, DepthPriority, thickness);

			P.Y = B[i].Y; Q.Y = B[i].Y;
			P.Z = B[j].Z; Q.Z = B[j].Z;
			P.X = B[0].X; Q.X = B[1].X;
			PDI->DrawLine(P, Q, Color, DepthPriority, thickness);

			P.Z = B[i].Z; Q.Z = B[i].Z;
			P.X = B[j].X; Q.X = B[j].X;
			P.Y = B[0].Y; Q.Y = B[1].Y;
			PDI->DrawLine(P, Q, Color, DepthPriority, thickness);
		}

		//DrawWireBox(PDI, , col, DrawBoundsDPG);
		//DrawCircle(PDI, Bounds.Origin, FVector(1, 0, 0), FVector(0, 1, 0), col, Bounds.SphereRadius, 32, DrawBoundsDPG);
		//DrawCircle(PDI, Bounds.Origin, FVector(1, 0, 0), FVector(0, 0, 1), col, Bounds.SphereRadius, 32, DrawBoundsDPG);
		//DrawCircle(PDI, Bounds.Origin, FVector(0, 1, 0), FVector(0, 0, 1), col, Bounds.SphereRadius, 32, DrawBoundsDPG);
	}


	void			_DrawHeavyDebug(
		const FPopcornFXSceneProxy *sceneProxy,
		FPrimitiveDrawInterface* PDI,
		const FSceneView* view,
		uint32 debugModeMask,
		uint32 globalIndex,
		const PopcornFX::Drawers::SBase_DrawRequest &dr)
	{
		const PopcornFX::CGuid				positionStreamId = dr.m_PositionStreamId;
		if (!positionStreamId.Valid())
			return;

		const FLinearColor			color = _MediumColorFromIndex(globalIndex);

		const PopcornFX::CParticleStreamToRender	*baseStream = &dr.StreamToRender();

		if (debugModeMask & EHeavyDebugModeMask::Mediums)
		{
			_RenderBoundsColor(
				PDI,
				ToUE(baseStream->BBox() * FPopcornFXPlugin::GlobalScale()),
				color);
		}

		if ((debugModeMask & (EHeavyDebugModeMask::Particles | EHeavyDebugModeMask::Pages)) == 0)
			return;

		// Draw particles as points for MainMemory streams
		const PopcornFX::CParticleStreamToRender_MainMemory		*stream = dr.StreamToRender_MainMemory();
		if (stream != null)
		{
			static volatile float		pointSize = 5.f;
			const float					scale = FPopcornFXPlugin::GlobalScale();
			static volatile uint8		depthPrio = SDPG_World;

			for (uint32 pagei = 0; pagei < stream->PageCount(); pagei++)
			{
				const PopcornFX::CParticlePageToRender_MainMemory	*page = stream->Page(pagei);
				HH_ASSERT(page != null);

				if (debugModeMask & EHeavyDebugModeMask::Pages)
				{
					_RenderBoundsColor(
						PDI,
						ToUE(page->BBox() * FPopcornFXPlugin::GlobalScale()),
						color);
				}

				if (debugModeMask & EHeavyDebugModeMask::Particles)
				{
					TStridedMemoryView<const CFloat3>	positions = page->StreamForReading<CFloat3>(positionStreamId);
					for (uint32 parti = 0; parti < positions.Count(); ++parti)
					{
						CFloat3		pos = positions[parti] * scale;
						PDI->DrawPoint(ToUE(pos), color, pointSize, depthPrio);
					}
				}
			}
		}

		//{
		//	//RenderBounds(PDI, view->ViewFamily->EngineShowFlags, sceneProxy->GetBounds(), true);
		//	const auto		&Bounds = sceneProxy->GetBounds();
		//	DrawWireBox(PDI, Bounds.GetBox(), FColor(255, 255, 0), depthPrio);
		//	DrawCircle(PDI, Bounds.Origin, FVector(1, 0, 0), FVector(0, 1, 0), FColor(255, 255, 0), Bounds.SphereRadius, 32, depthPrio);
		//	DrawCircle(PDI, Bounds.Origin, FVector(1, 0, 0), FVector(0, 0, 1), FColor(255, 255, 0), Bounds.SphereRadius, 32, depthPrio);
		//	DrawCircle(PDI, Bounds.Origin, FVector(0, 1, 0), FVector(0, 0, 1), FColor(255, 255, 0), Bounds.SphereRadius, 32, depthPrio);
		//}
	}
}

void	CParticleRenderManager::DrawHeavyDebug(const FPopcornFXSceneProxy *sceneProxy, FPrimitiveDrawInterface* PDI, const FSceneView* view, uint32 debugModeMask)
{
	//PDI->DrawPoint(FVector(scale, 0, 0), FLinearColor::Red, pointSize, depthPrio);
	//PDI->DrawPoint(FVector(0, scale, 0), FLinearColor::Green, pointSize, depthPrio);
	//PDI->DrawPoint(FVector(0, 0, scale), FLinearColor::Blue, pointSize, depthPrio);

	const CParticleCollectedFrameToRender		&frame = *m_Rendering_LockMe;

	uint32			globalIndex = 0;

	for (uint32 dri = 0; dri < frame.m_BillboardDrs.Count(); ++dri)
	{
		_DrawHeavyDebug(sceneProxy, PDI, view, debugModeMask, globalIndex++, frame.m_BillboardDrs[dri]);
	}
	for (uint32 dri = 0; dri < frame.m_RibbonDrs.Count(); ++dri)
	{
		_DrawHeavyDebug(sceneProxy, PDI, view, debugModeMask, globalIndex++, frame.m_RibbonDrs[dri]);
	}
	for (uint32 dri = 0; dri < frame.m_MeshDrs.Count(); ++dri)
	{
		_DrawHeavyDebug(sceneProxy, PDI, view, debugModeMask, globalIndex++, frame.m_MeshDrs[dri]);
	}
	for (uint32 dri = 0; dri < frame.m_LightDrs.Count(); ++dri)
	{
		_DrawHeavyDebug(sceneProxy, PDI, view, debugModeMask, globalIndex++, frame.m_LightDrs[dri]);
	}

	// global bounds
	sceneProxy->RenderBounds(PDI, view->Family->EngineShowFlags, sceneProxy->GetBounds(), true);
}

#endif // POPCORNFX_RENDER_DEBUG

