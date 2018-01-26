//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "DrawCalls_Mesh.h"

#include "PopcornFXStats.h"
#include "World/PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"
#include "Render/MyRHIStuff.h"
#include "Render/RenderManager.h"
#include "Render/DrawCalls_Ribbon.h"

#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/Storage/MainMemory/storage_ram.h>

//#if (PK_GPU_D3D11 == 1)
//#	include <hh_particles/include/Storage/D3D11/storage_d3d11.h>
//#endif

//----------------------------------------------------------------------------

void		ParticleDrawCall_Mesh_BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Mesh &renderer)
{
	flags.Set(SDrawerFlags::SupportGPUParticles, false);
}

//----------------------------------------------------------------------------
//
// CParticleBillboarder_Mesh_CPU
//
//----------------------------------------------------------------------------

void	CParticleBillboarder_Mesh_CPU::ClearOnFail()
{
	m_BB.WaitTasks();
	for (hh_u32 meshi = 0; meshi < m_MeshOutputDrawCalls.Count(); ++meshi)
	{
		if (m_MeshOutputDrawCalls[meshi].m_DrawCall != null)
		{
			m_MeshOutputDrawCalls[meshi].m_DrawCall->Clear(); // INVALIDATE
			m_MeshOutputDrawCalls[meshi].m_DrawCall = null;
		}
	}
}

//----------------------------------------------------------------------------

bool	CParticleBillboarder_Mesh_CPU::Prepare(CParticleRenderManager *renderManager, const PopcornFX::Drawers::SMesh_DrawRequest &dr)
{
	_m_Dummy = CFloat4x4::ZERO;

	m_RenderManager = renderManager;

	if (!m_BB.Prepare(TMemoryView<const PopcornFX::Drawers::SMesh_DrawRequest>(&dr, 1)))
		return false;

	m_MeshOutputDrawCalls.Clear();
	m_Mapped_Buffers.Clear();

	m_TotalParticleCount = 0;

	const hh_u32		meshCount = dr.m_MeshDescs.Count();
	HH_ASSERT(meshCount > 0);
	if (!HH_VERIFY(m_MeshOutputDrawCalls.Resize(meshCount)))
		return false;
	if (!HH_VERIFY(m_Mapped_Buffers.Resize(meshCount)))
		return false;

	return true;
}

void	CParticleBillboarder_Mesh_CPU::SetFutureOutputDrawCall(hh_u32 meshi, CParticleDrawCall_BaseMesh *drawCall, hh_u32 particleOffsetInBuffer, hh_u32 particleCount)
{
	SMeshDescOutputDrawCall		&output = m_MeshOutputDrawCalls[meshi];

	HH_ASSERT(output.m_ParticleOffset == 0);
	HH_ASSERT(output.m_DrawCall == null);
	output.m_ParticleOffset = particleOffsetInBuffer;
	output.m_ParticleCount = particleCount;
	output.m_DrawCall = drawCall;

	m_TotalParticleCount += particleCount;
}

bool	CParticleBillboarder_Mesh_CPU::LaunchBillboarding(PopcornFX::CRendererSubView &views)
{
	m_Count_Param_DiffuseColors = 0;

	if (m_TotalParticleCount == 0)
		return false;

	HH_ONLY_IF_ASSERTS(hh_u32		activeDrawCalls = 0;)
	const hh_u32		meshCount = m_MeshOutputDrawCalls.Empty() ? 1 : m_MeshOutputDrawCalls.Count();
	for (hh_u32 meshi = 0; meshi < meshCount; ++meshi)
	{
		SMeshDescOutputDrawCall		&meshDc = m_MeshOutputDrawCalls[meshi];
		if (!SetOutputBuffers(meshi, meshDc))
			return false;
		HH_ONLY_IF_ASSERTS(activeDrawCalls += (meshDc.m_DrawCall != null);)
	}
	HH_ASSERT(m_Count_Param_DiffuseColors == activeDrawCalls); // every draw call should match that, exept non-existing ones

	HH_ASSERT(m_Mapped_Buffers.Count() == meshCount);

	{
		m_Exec_Matrices.Clear();
		m_Exec_Matrices.m_PositionsScale = views.GlobalScale();
		m_Exec_Matrices.m_PerMeshMatrices = TStridedMemoryView<TStridedMemoryView<CFloat4x4> >(
			&(m_Mapped_Buffers[0].m_Matrices), m_Mapped_Buffers.Count(), sizeof(m_Mapped_Buffers[0]));

		if (!m_Exec_Matrices.SetupedAndAddExec(m_BB))
			return false;
	}

	if (m_Count_Param_DiffuseColors > 0)
	{
		m_Exec_Colors.Clear();

		if (!m_Exec_Colors.SetupedAndAddExec(
				m_BB,
				m_RenderManager->m_DiffuseColorStringId,
				TStridedMemoryView<const TStridedMemoryView<CFloat4> >(
					&(m_Mapped_Buffers[0].m_Param_DiffuseColors), m_Mapped_Buffers.Count(), sizeof(m_Mapped_Buffers[0])),
				CFloat4(1.f)
			))
			return false;
	}

	CParticleScene	*const scene = views.SceneProxy()->ParticleSceneToRender();
	m_BB.LaunchTasks(scene->BBScene(), false);

	return true;
}

bool	CParticleBillboarder_Mesh_CPU::SetOutputBuffers(hh_u32 meshi, SMeshDescOutputDrawCall &meshDc)
{
	// null drawCall means 0 particles
	if (meshDc.m_DrawCall == null)
	{
		const hh_u32		pcount = m_BB.MeshParticleCount(meshi);
		if (pcount > 0)
		{
			// patchup dummy view for billboard to not crash
			m_Mapped_Buffers[meshi].m_Matrices = TStridedMemoryView<CFloat4x4>(&_m_Dummy, pcount, 0);
			m_Mapped_Buffers[meshi].m_Param_DiffuseColors = TStridedMemoryView<CFloat4>(&_m_Dummy.XAxis(), pcount, 0);
		}
		else
		{
			m_Mapped_Buffers[meshi].m_Matrices.Clear();
			m_Mapped_Buffers[meshi].m_Param_DiffuseColors.Clear();
		}
		return true;
	}

	if (!meshDc.m_DrawCall->PrepareBuffersIFN())
		return false;

	HH_ASSERT(!meshDc.m_DrawCall->m_Mapped_Matrices.Empty());

	m_Mapped_Buffers[meshi].m_Matrices = meshDc.m_DrawCall->m_Mapped_Matrices.Slice(meshDc.m_ParticleOffset, meshDc.m_ParticleCount);

	if (meshDc.m_DrawCall->m_Mapped_Param_DiffuseColors.Empty())
	{
		HH_ASSERT(m_Count_Param_DiffuseColors == 0);
		m_Mapped_Buffers[meshi].m_Param_DiffuseColors.Clear();
	}
	else
	{
		++m_Count_Param_DiffuseColors;
		m_Mapped_Buffers[meshi].m_Param_DiffuseColors = meshDc.m_DrawCall->m_Mapped_Param_DiffuseColors.Slice(meshDc.m_ParticleOffset, meshDc.m_ParticleCount);
	}

	return true;
}

void	CParticleBillboarder_Mesh_CPU::FinishBillboarding()
{
	m_BB.WaitTasks();
}

//----------------------------------------------------------------------------
//
// CParticleDrawCall_BaseMesh
//
//----------------------------------------------------------------------------

bool	CParticleDrawCall_BaseMesh::Init(CParticleRenderManager *renderManager)
{
	m_DC_TotalElementCount = 0;
	m_DC_TotalBBox = PopcornFX::CAABB::DEGENERATED;
	m_DC_IsSolid = false; // will be patched later
	m_DC_CastShadow = false;

	m_HasFailed = false;

	m_RenderManager = renderManager;
	m_TotalParticleCount = 0;
	m_Instanced = FPopcornFXMeshVertexFactory::IsInstanced(m_RenderManager->m_FeatureLevel);
	m_Prepared = false;

	m_Has_Param_DiffuseColors = false; // setup later

	return true;
}

//----------------------------------------------------------------------------

bool	CParticleDrawCall_BaseMesh::PrepareBuffersIFN()
{
	const bool		wasPrepared = m_Prepared;
	if (!wasPrepared)
		m_Prepared = true;

	if (wasPrepared)
	{
		if (m_Mapped_Matrices.Empty()) // failed
			return false;
		HH_ASSERT(m_Has_Param_DiffuseColors == !m_Mapped_Param_DiffuseColors.Empty());
		return true;
	}
	if (!_PrepareBuffersIFN())
	{
		//ClearBuffers();
		m_HasFailed = true;
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------

bool	CParticleDrawCall_BaseMesh::_PrepareBuffersIFN()
{
	CParticleScene		*const scene = m_RenderManager->m_Views->SceneProxy()->ParticleSceneToRender();

	if (m_Instanced)
	{
		CVertexBufferPool		*mainVBPool = &scene->VBPool();
		//CIndexBufferPool		*mainIBPool = &scene->IBPool();

		if (!mainVBPool->AllocateIf(true, m_Instanced_Matrices, m_TotalParticleCount, sizeof(CFloat4x4)))
			return false;
		if (!mainVBPool->AllocateIf(m_Has_Param_DiffuseColors, m_Instanced_Param_DiffuseColors, m_TotalParticleCount, sizeof(CFloat4)))
			return false;

		m_Mapped_Matrices = TStridedMemoryView<CFloat4x4>(0, m_TotalParticleCount);
		if (!m_Instanced_Matrices->Map(m_Mapped_Matrices))
			return false;

		if (m_Has_Param_DiffuseColors)
		{
			m_Mapped_Param_DiffuseColors = TStridedMemoryView<CFloat4>(0, m_TotalParticleCount);
			if (!m_Instanced_Param_DiffuseColors->Map(m_Mapped_Param_DiffuseColors))
				return false;
		}
	}
	else
	{
		const hh_u32	alignedCount = PopcornFX::Mem::Align<0x10>(m_TotalParticleCount + 0x10);
		hh_u32			wbSizeInBytes = 0;
		wbSizeInBytes += alignedCount * sizeof(CFloat4x4);
		if (m_Has_Param_DiffuseColors)
			wbSizeInBytes += alignedCount * sizeof(CFloat4);

		hh_u8		*wb = m_NonInstanced_Wb.GetSome<hh_u8>(wbSizeInBytes);
		if (wb == null)
			return false;
		
		m_Mapped_Matrices = TStridedMemoryView<CFloat4x4>(reinterpret_cast<CFloat4x4*>(wb), m_TotalParticleCount, sizeof(CFloat4x4));
		wb += alignedCount * m_Mapped_Matrices.Stride();

		if (m_Has_Param_DiffuseColors)
		{
			m_Mapped_Param_DiffuseColors = TStridedMemoryView<CFloat4>(reinterpret_cast<CFloat4*>(wb), m_TotalParticleCount, sizeof(CFloat4));
			wb += alignedCount * m_Mapped_Param_DiffuseColors.Stride();
		}
	}

	return true;
}

//----------------------------------------------------------------------------

//static
bool	CParticleDrawCall_BaseMesh::PreSetupMeshData(FPopcornFXMeshVertexFactory::FDataType &outMeshData, const FStaticMeshLODResources &meshResources)
{
	outMeshData.bInitialized = false;

	FPopcornFXMeshVertexFactory::FDataType		&Data = outMeshData;
	const FStaticMeshLODResources				&LODResources = meshResources;

#if (ENGINE_MINOR_VERSION >= 12)

	// see Engine/Private/Particles/ParticleSystemRender.cpp 2326

	Data.PositionComponent = FVertexStreamComponent(
		&LODResources.PositionVertexBuffer,
		STRUCT_OFFSET(FPositionVertex,Position),
		LODResources.PositionVertexBuffer.GetStride(),
		VET_Float3
		);

	uint32 TangentXOffset = 0;
	uint32 TangetnZOffset = 0;
	uint32 UVsBaseOffset = 0;

	SELECT_STATIC_MESH_VERTEX_TYPE(
		LODResources.VertexBuffer.GetUseHighPrecisionTangentBasis(),
		LODResources.VertexBuffer.GetUseFullPrecisionUVs(),
		LODResources.VertexBuffer.GetNumTexCoords(),
		{
			TangentXOffset = STRUCT_OFFSET(VertexType, TangentX);
			TangetnZOffset = STRUCT_OFFSET(VertexType, TangentZ);
			UVsBaseOffset = STRUCT_OFFSET(VertexType, UVs);
		});

	Data.TangentBasisComponents[0] = FVertexStreamComponent(
		&LODResources.VertexBuffer,
		TangentXOffset,
		LODResources.VertexBuffer.GetStride(),
		LODResources.VertexBuffer.GetUseHighPrecisionTangentBasis() ?
			TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::HighPrecision>::VertexElementType : 
			TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::Default>::VertexElementType
		);

	Data.TangentBasisComponents[1] = FVertexStreamComponent(
		&LODResources.VertexBuffer,
		TangetnZOffset,
		LODResources.VertexBuffer.GetStride(),
		LODResources.VertexBuffer.GetUseHighPrecisionTangentBasis() ?
			TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::HighPrecision>::VertexElementType : 
			TStaticMeshVertexTangentTypeSelector<EStaticMeshVertexTangentBasisType::Default>::VertexElementType
		);

	Data.TextureCoordinates.Empty();

	uint32 UVSizeInBytes = LODResources.VertexBuffer.GetUseFullPrecisionUVs() ?
		sizeof(TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::HighPrecision>::UVsTypeT) : sizeof(TStaticMeshVertexUVsTypeSelector<EStaticMeshVertexUVType::Default>::UVsTypeT);

	EVertexElementType UVVertexElementType = LODResources.VertexBuffer.GetUseFullPrecisionUVs() ?
		VET_Float2 : VET_Half2;

	uint32 NumTexCoords = FMath::Min<uint32>(LODResources.VertexBuffer.GetNumTexCoords(), MAX_TEXCOORDS);
	for (uint32 UVIndex = 0; UVIndex < NumTexCoords; UVIndex++)
	{
		Data.TextureCoordinates.Add(FVertexStreamComponent(
			&LODResources.VertexBuffer,
			UVsBaseOffset + UVSizeInBytes * UVIndex,
			LODResources.VertexBuffer.GetStride(),
			UVVertexElementType
			));
	}

	if(LODResources.ColorVertexBuffer.GetNumVertices() > 0)
	{
		Data.VertexColorComponent = FVertexStreamComponent(
			&LODResources.ColorVertexBuffer,
			0,
			LODResources.ColorVertexBuffer.GetStride(),
			VET_Color
			);
	}


#else // if < 12

	outMeshData.PositionComponent = FVertexStreamComponent(
		&meshResources.PositionVertexBuffer,
		STRUCT_OFFSET(FPositionVertex,Position),
		meshResources.PositionVertexBuffer.GetStride(),
		VET_Float3
		);

	outMeshData.TangentBasisComponents[0] = FVertexStreamComponent(
		&meshResources.VertexBuffer,
		STRUCT_OFFSET(FStaticMeshFullVertex, TangentX),
		meshResources.VertexBuffer.GetStride(),
		VET_PackedNormal
		);

	outMeshData.TangentBasisComponents[1] = FVertexStreamComponent(
		&meshResources.VertexBuffer,
		STRUCT_OFFSET(FStaticMeshFullVertex, TangentZ),
		meshResources.VertexBuffer.GetStride(),
		VET_PackedNormal
		);

	outMeshData.TextureCoordinates.Empty();
	if (!meshResources.VertexBuffer.GetUseFullPrecisionUVs())
	{
		uint32 NumTexCoords = FMath::Min<uint32>(meshResources.VertexBuffer.GetNumTexCoords(), MAX_TEXCOORDS);
		for (uint32 UVIndex = 0; UVIndex < NumTexCoords; UVIndex++)
		{
			outMeshData.TextureCoordinates.Add(FVertexStreamComponent(
				&meshResources.VertexBuffer,
				STRUCT_OFFSET(TStaticMeshFullVertexFloat16UVs<MAX_TEXCOORDS>, UVs) + sizeof(FVector2DHalf) * UVIndex,
				meshResources.VertexBuffer.GetStride(),
				VET_Half2
				));
		}
	}
	else
	{
		for (uint32 UVIndex = 0; UVIndex < meshResources.VertexBuffer.GetNumTexCoords(); UVIndex++)
		{
			outMeshData.TextureCoordinates.Add(FVertexStreamComponent(
				&meshResources.VertexBuffer,
				STRUCT_OFFSET(TStaticMeshFullVertexFloat32UVs<MAX_TEXCOORDS>, UVs) + sizeof(FVector2D) * UVIndex,
				meshResources.VertexBuffer.GetStride(),
				VET_Float2
				));
		}
	}

	if (meshResources.ColorVertexBuffer.GetNumVertices() > 0)
	{
		outMeshData.VertexColorComponent = FVertexStreamComponent(
			&meshResources.ColorVertexBuffer,
			0,
			meshResources.ColorVertexBuffer.GetStride(),
			VET_Color
			);
	}
	else
	{
		outMeshData.VertexColorComponent = FVertexStreamComponent();
	}
#endif

	// Not yet initialized, particle instance data will be added later
	// outMeshData.bInitialized = true;

	return true;
}

//----------------------------------------------------------------------------
//
// CParticleDrawCall_MeshSingleSection
//
//----------------------------------------------------------------------------

bool	CParticleDrawCall_MeshSingleSection::Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_MeshSingleSection_Render *desc)
{
	m_BillboardersToWait.Clear();

	m_CurrentOneFrameCollector = null;

	// /!\ Important: or it will crash later in UE
	if (!HH_VERIFY(FPopcornFXMeshVertexFactory::IsCompatible(desc->m_MaterialInterface)))
	{
		HH_TODO("Message incompatible material !");
		return false;
	}

	if (!CParticleDrawCall_BaseMesh::Init(renderManager))
		return false;

	m_MaterialDesc = desc;

	m_DC_DrawOrder = m_MaterialDesc->m_DrawOrder;
	m_DC_IsSolid = m_MaterialDesc->m_Material_IsSolid;
	m_DC_CastShadow = m_MaterialDesc->m_CastShadow;

	HH_TODO("DETECT Particle color material usage ? in MaterialDesc ?? Maybe in CParticleMaterialDesc_Render::ResolveRenderMaterial ?");
	m_Has_Param_DiffuseColors = true;

	HH_ASSERT(m_MaterialDesc->m_StaticMesh != null && m_MaterialDesc->m_StaticMesh->HasValidRenderData());

	m_RenderResources = m_MaterialDesc->m_RenderResources;
	m_SectionIndex = m_MaterialDesc->m_StaticMeshSingleSectionId;

	if (!CParticleDrawCall_BaseMesh::PreSetupMeshData(m_VFData, *m_RenderResources))
		return false;


	return true;
}

//----------------------------------------------------------------------------

void	CParticleDrawCall_MeshSingleSection::AssocBillboader(CParticleBillboarder_Mesh_CPU *billboarder, hh_u32 meshi)
{
	const hh_u32		particleOffset = m_TotalParticleCount;
	const hh_u32		particleCount = billboarder->MeshParticleCount(meshi);

	if (!HH_VERIFY(m_BillboardersToWait.PushBack(billboarder).Valid()))
	{
		// null will ignore the draw call
		billboarder->SetFutureOutputDrawCall(meshi, null, 0, 0);
		return;
	}

	billboarder->SetFutureOutputDrawCall(meshi, this, particleOffset, particleCount);

	m_DC_TotalElementCount += particleCount;
	m_TotalParticleCount += particleCount;
	HH_ASSERT(billboarder->m_BB.DrawRequests().Count() == 1);
	const PopcornFX::CAABB		&drBB = billboarder->m_BB.DrawRequests()[0].StreamToRender().BBox();
	IF_DEBUG_BBOX(HH_ASSERT(drBB.IsFinite() && drBB.Valid());)
	if (drBB.Valid() && drBB.IsFinite())
		m_DC_TotalBBox.Add(drBB);

	return;
}

//----------------------------------------------------------------------------

class FPopcornFXMeshCollector_2 : public FOneFrameResource
{
public:
	FPopcornFXMeshVertexFactory		m_VertexFactory;

	FPopcornFXMeshUserData			m_UserData;

	FPopcornFXMeshCollector_2() {}

	~FPopcornFXMeshCollector_2()
	{
		m_VertexFactory.ReleaseResource();
	}
};

//----------------------------------------------------------------------------

void	CParticleDrawCall_MeshSingleSection::DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_MeshDesc::DrawCall");

	{
		for (hh_u32 bbi = 0; bbi < m_BillboardersToWait.Count(); ++bbi)
		{
			if (m_BillboardersToWait[bbi] != null)
			{
				m_BillboardersToWait[bbi]->FinishBillboarding();
				m_BillboardersToWait[bbi] = null;
			}
		}
		m_BillboardersToWait.Clear();
	}

	m_Instanced_Matrices.Unmap();
	m_Instanced_Param_DiffuseColors.Unmap();

	if (m_HasFailed)
		return;


	const FPopcornFXSceneProxy		&sceneProxy = *views.SceneProxy();
	CParticleScene					*const scene = sceneProxy.ParticleSceneToRender();
	const auto						&sceneViews = views.SceneViews();
	const auto						&bbViews = views.BBViews();

	FMatrix							localToWorld = FMatrix::Identity;
	//if (views.GlobalScale() != 1.f)
	//	localToWorld *= views.GlobalScale();

	const ERHIFeatureLevel::Type	featureLevel = m_RenderManager->m_FeatureLevel;

	//const bool			isShadowPass = (view.RenderPass() == CRendererSubView::RenderPass_Shadow);

	const hh_u32					particleCount = m_TotalParticleCount;
	if (particleCount == 0)
		return;

	if (!m_VFData.bInitialized)
	{
		if (m_Instanced)
		{
			m_VFData.m_InstancedMatrices.Setup(m_Instanced_Matrices);
#if POPCORNFX_RENDER_DEBUG
			if (m_RenderManager->m_OverrideColorVertexBuffer != null)
			{
				m_VFData.m_InstancedColors.Setup(m_RenderManager->m_OverrideColorVertexBuffer, m_RenderManager->m_OverrideColorBufferOffset, 0);
			}
			else
#endif
			{
				m_VFData.m_InstancedColors.Setup(m_Instanced_Param_DiffuseColors);
			}
		}
		m_VFData.bInitialized = true;
	}

	FPopcornFXMeshCollector_2		*collectorRes = m_CurrentOneFrameCollector;
	FPopcornFXMeshVertexFactory		*vertexFactory = null;

	if (collectorRes == null)
	{
		collectorRes = &(views.Collector()->AllocateOneFrameResource<FPopcornFXMeshCollector_2>());
		check(collectorRes != null);
		m_CurrentOneFrameCollector = collectorRes;

		vertexFactory = &(collectorRes->m_VertexFactory);
		vertexFactory->SetFeatureLevel(featureLevel); // Make sure the renderer and the vertex factory share the same feature level
		vertexFactory->SetData(m_VFData);
		vertexFactory->InitResource();

		collectorRes->m_UserData.m_Instanced = m_Instanced;
		if (!m_Instanced)
		{
			HH_ASSERT(!m_Mapped_Matrices.Empty());
			HH_ASSERT(!m_Mapped_Param_DiffuseColors.Empty());
			collectorRes->m_UserData.m_Instance_Matrices = m_Mapped_Matrices;
#if POPCORNFX_RENDER_DEBUG
			if (m_RenderManager->m_OverrideColorBuffer != null)
			{
				collectorRes->m_UserData.m_Instance_Param_DiffuseColors =
					TStridedMemoryView<const CFloat4>(
						PopcornFX::Mem::AdvanceRawPointer(reinterpret_cast<const CFloat4*>(m_RenderManager->m_OverrideColorBuffer), m_RenderManager->m_OverrideColorBufferOffset),
						m_Mapped_Param_DiffuseColors.Count(),
						0
				);
			}
			else
#endif
			{
				collectorRes->m_UserData.m_Instance_Param_DiffuseColors = m_Mapped_Param_DiffuseColors;
			}
		}
	}
	else
	{
		vertexFactory = &(collectorRes->m_VertexFactory);
	}

	if (!HH_VERIFY(vertexFactory->IsInitialized()))
		return;

	FMeshBatch		&Mesh = views.Collector()->AllocateMesh();

	Mesh.VertexFactory = vertexFactory;
	Mesh.DynamicVertexData = NULL;
	Mesh.LCI = NULL;
	Mesh.bUseAsOccluder = false;
	Mesh.UseDynamicData = false;
	Mesh.ReverseCulling = sceneProxy.IsLocalToWorldDeterminantNegative();
	Mesh.CastShadow = CastShadow();
	Mesh.DepthPriorityGroup = sceneProxy.GetDepthPriorityGroup(sceneViews[0].m_SceneView);
	Mesh.Type = PT_TriangleList;

#if POPCORNFX_RENDER_DEBUG
	if (m_RenderManager->m_OverrideAllMaterialRenderProxy != null)
	{
		Mesh.MaterialRenderProxy = m_RenderManager->m_OverrideAllMaterialRenderProxy;
	}
	else
#endif // POPCORNFX_RENDER_DEBUG
	{
		Mesh.MaterialRenderProxy = m_MaterialDesc->m_MaterialRenderProxy;
	}
	Mesh.bCanApplyViewModeOverrides = true;

	if (!HH_VERIFY(m_SectionIndex < hh_u32(m_RenderResources->Sections.Num())))
		return;
	const FStaticMeshSection	&Section = m_RenderResources->Sections[m_SectionIndex];
	if (!HH_VERIFY(Section.NumTriangles > 0))
		return;

	FMeshBatchElement		&BatchElement = Mesh.Elements[0];

	//BatchElement.PrimitiveUniformBufferResource =
	BatchElement.PrimitiveUniformBuffer =
		CreatePrimitiveUniformBufferImmediate(localToWorld, sceneProxy.GetBounds(), sceneProxy.GetLocalBounds(), false, sceneProxy.UseEditorDepthTest());
	//&sceneProxy.GetWorldSpacePrimitiveUniformBuffer();
	BatchElement.FirstIndex = Section.FirstIndex;
	BatchElement.MinVertexIndex = Section.MinVertexIndex;
	BatchElement.MaxVertexIndex = Section.MaxVertexIndex;
	BatchElement.NumInstances = m_Instanced ? particleCount : 1;

	BatchElement.IndexBuffer = &m_RenderResources->IndexBuffer;
	BatchElement.FirstIndex = Section.FirstIndex;
	BatchElement.NumPrimitives = Section.NumTriangles;

	BatchElement.UserData = &collectorRes->m_UserData;
	BatchElement.UserIndex = 0;

#if (ENGINE_MINOR_VERSION >= 11)
	BatchElement.bIsInstancedMesh = m_Instanced;
#endif

	if (!m_Instanced)
	{
		Mesh.Elements.Reserve(particleCount);
		for (hh_u32 batchi = 1; batchi < particleCount; ++batchi)
		{
			FMeshBatchElement* NextElement = new(Mesh.Elements) FMeshBatchElement();
			*NextElement = Mesh.Elements[0];
			NextElement->UserIndex = batchi;
		}
	}

	for (hh_u32 viewi = 0; viewi < sceneViews.Count(); ++viewi)
	{
		if ((bbViewMask & (1U << sceneViews[viewi].m_BBViewIndex)) == 0)
			continue;
		INC_DWORD_STAT_BY(STAT_PopcornFX_DrawCallsCount, 1);
		views.Collector()->AddMesh(viewi, Mesh);
	}
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

bool	CParticleDrawCall_MeshMultiSection::Init(CParticleRenderManager *renderManager, CParticleBillboarder_Mesh_CPU *billboarder, hh_u32 meshDesci, const CParticleMaterialDesc_MeshMultiSection_Render *matDesc)
{
	m_CurrentOneFrameCollector = null;

	// /!\ Important: or it will crash later in UE
	if (matDesc->m_MaterialInterface != null && // can be NULL !
		!HH_VERIFY(FPopcornFXMeshVertexFactory::IsCompatible(matDesc->m_MaterialInterface)))
	{
		HH_TODO("Message incompatible material !");
		return false;
	}

	if (!CParticleDrawCall_BaseMesh::Init(renderManager))
		return false;

	m_MaterialDesc = matDesc;
	m_BillboarderToWait = billboarder;
	m_RenderResources = m_MaterialDesc->m_RenderResources;

	if (!m_Sections.Resize(m_MaterialDesc->m_SectionCount))
		return false;

	if (!CParticleDrawCall_BaseMesh::PreSetupMeshData(m_VFData, *m_RenderResources))
		return false;

	bool		hasTrans = false;

	for (hh_u32 i = 0; i < m_MaterialDesc->m_SectionCount; ++i)
	{
		SSection		&section = m_Sections[i];
		section.m_SectionIndex = m_MaterialDesc->m_SectionStart + i;

		HH_TODO("if m_MaterialRenderProxy != null, could be drawn all sections at once ??");

		//section.m_MatDesc.Clear();

		if (m_MaterialDesc->m_MaterialRenderProxy != null)
		{
			section.m_MaterialRenderProxy = m_MaterialDesc->m_MaterialRenderProxy;
			section.m_CastShadow = m_MaterialDesc->m_CastShadow;
			HH_ASSERT(section.m_MaterialRenderProxy != null);
			hasTrans |= !(m_MaterialDesc->m_Material_IsSolid);
		}
		else
		{
			UMaterialInterface					*materialInterface = null;
			materialInterface = m_MaterialDesc->m_StaticMesh->GetMaterial(m_RenderResources->Sections[section.m_SectionIndex].MaterialIndex);
			HH_ASSERT(materialInterface != null);
			if (materialInterface != null &&
				!HH_VERIFY(FPopcornFXMeshVertexFactory::IsCompatible(materialInterface))) // !! IMPORTANT, or will crash later
			{
				HH_TODO("Message incompatible material !");
				return false;
			}
			CParticleMaterialDesc_Render		tmpMatDesc;
			if (!tmpMatDesc.SetupFromGame(*m_MaterialDesc) ||
				!tmpMatDesc.ResolveMaterial(materialInterface, m_RenderManager->m_FeatureLevel))
			{
				return false;
			}

			section.m_CastShadow = tmpMatDesc.m_CastShadow;
			section.m_MaterialRenderProxy = tmpMatDesc.m_MaterialRenderProxy;
			HH_ASSERT(section.m_MaterialRenderProxy != null);
			hasTrans |= !(tmpMatDesc.m_Material_IsSolid);
		}
		m_DC_CastShadow |= section.m_CastShadow;
	}

	m_DC_IsSolid = !hasTrans;

	HH_TODO("DETECT Particle color material usage ?");
	m_Has_Param_DiffuseColors = true;

	const hh_u32		particleOffset = 0;
	const hh_u32		particleCount = billboarder->m_BB.MeshParticleCount(meshDesci);
	HH_ASSERT(particleCount > 0);

	billboarder->SetFutureOutputDrawCall(meshDesci, this, particleOffset, particleCount);

	m_DC_TotalElementCount = particleCount;
	m_TotalParticleCount = particleCount;

	HH_ASSERT(billboarder->m_BB.DrawRequests().Count() == 1);
	const PopcornFX::CAABB		&drBB = billboarder->m_BB.DrawRequests()[0].StreamToRender().BBox();
	IF_DEBUG_BBOX(HH_ASSERT(drBB.IsFinite() && drBB.Valid());)
	if (drBB.IsFinite() && drBB.IsFinite())
		m_DC_TotalBBox.Add(drBB);

	return true;
}

//----------------------------------------------------------------------------

void	CParticleDrawCall_MeshMultiSection::DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount)
{
	HH_NAMEDSCOPEDPROFILE("CParticleDrawCall_MeshDesc::DrawCall");

	{
		if (m_BillboarderToWait != null)
		{
			m_BillboarderToWait->FinishBillboarding();
			m_BillboarderToWait = null;
		}
	}

	m_Instanced_Matrices.Unmap();
	m_Instanced_Param_DiffuseColors.Unmap();

	if (m_HasFailed)
		return;

	HH_ASSERT(bbViewMask > 0);

	const FPopcornFXSceneProxy		&sceneProxy = *views.SceneProxy();
	CParticleScene					*const scene = sceneProxy.ParticleSceneToRender();
	const auto						&sceneViews = views.SceneViews();
	const auto						&bbViews = views.BBViews();

	FMatrix			localToWorld = FMatrix::Identity;
	//if (views.GlobalScale() != 1.f)
	//	localToWorld *= views.GlobalScale();

	const ERHIFeatureLevel::Type	featureLevel = m_RenderManager->m_FeatureLevel;

	//const bool			isShadowPass = (view.RenderPass() == CRendererSubView::RenderPass_Shadow);

	const hh_u32					particleCount = m_TotalParticleCount;
	if (particleCount == 0)
		return;

	if (!m_VFData.bInitialized)
	{
		if (m_Instanced)
		{
			m_VFData.m_InstancedMatrices.Setup(m_Instanced_Matrices);
#if POPCORNFX_RENDER_DEBUG
			if (m_RenderManager->m_OverrideColorVertexBuffer != null)
			{
				m_VFData.m_InstancedColors.Setup(m_RenderManager->m_OverrideColorVertexBuffer, m_RenderManager->m_OverrideColorBufferOffset, 0);
			}
			else
#endif
			{
				m_VFData.m_InstancedColors.Setup(m_Instanced_Param_DiffuseColors);
			}
		}
		m_VFData.bInitialized = true;
	}

	FPopcornFXMeshCollector_2		*collectorRes = m_CurrentOneFrameCollector;
	FPopcornFXMeshVertexFactory		*vertexFactory = null;

	if (collectorRes == null)
	{
		collectorRes = &(views.Collector()->AllocateOneFrameResource<FPopcornFXMeshCollector_2>());
		check(collectorRes != null);
		m_CurrentOneFrameCollector = collectorRes;

		vertexFactory = &(collectorRes->m_VertexFactory);
		vertexFactory->SetFeatureLevel(featureLevel); // Make sure the renderer and the vertex factory share the same feature level
		vertexFactory->SetData(m_VFData);
		vertexFactory->InitResource();

		collectorRes->m_UserData.m_Instanced = m_Instanced;
		if (!m_Instanced)
		{
			HH_ASSERT(!m_Mapped_Matrices.Empty());
			HH_ASSERT(!m_Mapped_Param_DiffuseColors.Empty());
			collectorRes->m_UserData.m_Instance_Matrices = m_Mapped_Matrices;
#if POPCORNFX_RENDER_DEBUG
			if (m_RenderManager->m_OverrideColorBuffer != null)
			{
				collectorRes->m_UserData.m_Instance_Param_DiffuseColors =
					TStridedMemoryView<const CFloat4>(
						PopcornFX::Mem::AdvanceRawPointer(reinterpret_cast<const CFloat4*>(m_RenderManager->m_OverrideColorBuffer), m_RenderManager->m_OverrideColorBufferOffset),
						m_Mapped_Param_DiffuseColors.Count(),
						0
						);
			}
			else
#endif
			{
				collectorRes->m_UserData.m_Instance_Param_DiffuseColors = m_Mapped_Param_DiffuseColors;
			}
		}
	}
	else
	{
		vertexFactory = &(collectorRes->m_VertexFactory);
	}

	if (!HH_VERIFY(vertexFactory->IsInitialized()))
		return;

	for (hh_u32 seci = 0; seci < m_Sections.Count(); ++seci)
	{
		SSection		&section = m_Sections[seci];

		if (views.RenderPass() == CRendererSubView::RenderPass_Shadow && !section.m_CastShadow)
			continue;

		FMeshBatch		&Mesh = views.Collector()->AllocateMesh();

		Mesh.VertexFactory = vertexFactory;
		Mesh.DynamicVertexData = NULL;
		Mesh.LCI = NULL;
		Mesh.bUseAsOccluder = false;
		Mesh.UseDynamicData = false;
		Mesh.ReverseCulling = sceneProxy.IsLocalToWorldDeterminantNegative();
		Mesh.CastShadow = section.m_CastShadow;
		Mesh.DepthPriorityGroup = sceneProxy.GetDepthPriorityGroup(sceneViews[0].m_SceneView);
		Mesh.Type = PT_TriangleList;
#if POPCORNFX_RENDER_DEBUG
		if (m_RenderManager->m_OverrideAllMaterialRenderProxy != null)
		{
			Mesh.MaterialRenderProxy = m_RenderManager->m_OverrideAllMaterialRenderProxy;
		}
		else
#endif // POPCORNFX_RENDER_DEBUG
		{
			Mesh.MaterialRenderProxy = section.m_MaterialRenderProxy;
		}
		Mesh.bCanApplyViewModeOverrides = true;

		if (!HH_VERIFY(section.m_SectionIndex < hh_u32(m_RenderResources->Sections.Num())))
			return;

		const FStaticMeshSection	&Section = m_RenderResources->Sections[section.m_SectionIndex];
		if (!HH_VERIFY(Section.NumTriangles > 0))
			return;

		FMeshBatchElement		&BatchElement = Mesh.Elements[0];

		//BatchElement.PrimitiveUniformBufferResource =
		BatchElement.PrimitiveUniformBuffer =
			CreatePrimitiveUniformBufferImmediate(localToWorld, sceneProxy.GetBounds(), sceneProxy.GetLocalBounds(), false, sceneProxy.UseEditorDepthTest());
		//&sceneProxy.GetWorldSpacePrimitiveUniformBuffer();
		BatchElement.FirstIndex = Section.FirstIndex;
		BatchElement.MinVertexIndex = Section.MinVertexIndex;
		BatchElement.MaxVertexIndex = Section.MaxVertexIndex;
		BatchElement.NumInstances = m_Instanced ? particleCount : 1;

		BatchElement.IndexBuffer = &m_RenderResources->IndexBuffer;
		BatchElement.FirstIndex = Section.FirstIndex;
		BatchElement.NumPrimitives = Section.NumTriangles;

		BatchElement.UserData = &collectorRes->m_UserData;
		BatchElement.UserIndex = 0;

#if (ENGINE_MINOR_VERSION >= 11)
		BatchElement.bIsInstancedMesh = m_Instanced;
#endif

		if (!m_Instanced)
		{
			Mesh.Elements.Reserve(particleCount);
			for (hh_u32 batchi = 1; batchi < particleCount; ++batchi)
			{
				FMeshBatchElement* NextElement = new(Mesh.Elements) FMeshBatchElement();
				*NextElement = Mesh.Elements[0];
				NextElement->UserIndex = batchi;
			}
		}

		for (hh_u32 viewi = 0; viewi < sceneViews.Count(); ++viewi)
		{
			if ((bbViewMask & (1U << sceneViews[viewi].m_BBViewIndex)) == 0)
				continue;
			INC_DWORD_STAT_BY(STAT_PopcornFX_DrawCallsCount, 1);
			views.Collector()->AddMesh(viewi, Mesh);
		}
	} // for (sections)
}

//----------------------------------------------------------------------------

