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

#include "Render/PopcornFXMeshVertexFactory.h"
#include "Render/PopcornFXVertexFactoryShaderParameters.h"
#include "Render/DrawCall.h"

#include "StaticMeshResources.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_delegates.h>
#include <hh_kernel/include/kr_containers_array.h>
#include <hh_kernel/include/kr_workingbuffer.h>
#include <pk_drawers/include/dr_mesh.h>
#include <pk_drawers/include/dr_mesh_cpu.h>

class CParticleDrawCall_BaseMesh;
struct SDrawerFlags;
class CParticleRenderManager;

//----------------------------------------------------------------------------

void		ParticleDrawCall_Mesh_BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Mesh &renderer);

//----------------------------------------------------------------------------
//
// Billboarder Mesh
//
//----------------------------------------------------------------------------

class CParticleBillboarder_Mesh_CPU
{
public:
	struct SMeshDescOutputDrawCall
	{
		hh_u32									m_ParticleOffset;
		hh_u32									m_ParticleCount;
		CParticleDrawCall_BaseMesh				*m_DrawCall;
		SMeshDescOutputDrawCall() : m_ParticleOffset(0), m_DrawCall(null) { }
	};

	struct SMeshDescOutputBuffers
	{
		TStridedMemoryView<CFloat4x4>	m_Matrices;
		TStridedMemoryView<CFloat4>		m_Param_DiffuseColors;
	};

	const CParticleRenderManager			*m_RenderManager;

	SMeshDescOutputDrawCall						m_SingleOutputDrawCall; // faster if single mesh
	PopcornFX::TArray<SMeshDescOutputDrawCall>	m_MeshOutputDrawCalls; // no meshId if Empty(), use m_SingleOutputDrawCall
	PopcornFX::TArray<SMeshDescOutputBuffers>	m_Mapped_Buffers;

	hh_u32									m_Count_Param_DiffuseColors;

	hh_u32									m_TotalParticleCount;
	PopcornFX::Drawers::CMesh_CPU			m_BB;
	PopcornFX::Drawers::CMesh_Exec_Matrices					m_Exec_Matrices;
	PopcornFX::Drawers::CMesh_Exec_MaterialParam			m_Exec_Colors;

	PopcornFX::CFloat4x4					_m_Dummy;

	void			Clear()
	{
		m_TotalParticleCount = 0;
	}

	void			ClearOnFail();

	bool			Prepare(CParticleRenderManager *renderManager, const PopcornFX::Drawers::SMesh_DrawRequest &dr);

	hh_u32			MeshParticleCount(hh_u32 meshi) const { return m_BB.HasMeshIds() ? m_BB.PerMeshParticleCount()[meshi] : m_BB.TotalParticleCount(); }

	void			SetFutureOutputDrawCall(hh_u32 meshi, CParticleDrawCall_BaseMesh *drawCall, hh_u32 particleOffsetInBuffer, hh_u32 particleCount);

	bool			LaunchBillboarding(PopcornFX::CRendererSubView &views);

	void			FinishBillboarding();

private:
	bool			SetOutputBuffers(hh_u32 meshi, SMeshDescOutputDrawCall &meshDc);

};

//----------------------------------------------------------------------------
//
// Draw Calls
//
//----------------------------------------------------------------------------

class CParticleDrawCall_BaseMesh : public CParticleAbstractDrawCall
{
public:
	~CParticleDrawCall_BaseMesh() { }

	const CParticleRenderManager				*m_RenderManager;

	hh_u32			m_TotalParticleCount;

	bool			m_Instanced;
	bool			m_Prepared;
	bool			m_HasFailed;

	bool			m_Has_Param_DiffuseColors;

	CPooledVertexBuffer				m_Instanced_Matrices;
	CPooledVertexBuffer				m_Instanced_Param_DiffuseColors;
	PopcornFX::CWorkingBuffer		m_NonInstanced_Wb;

	TStridedMemoryView<CFloat4x4>	m_Mapped_Matrices;
	TStridedMemoryView<CFloat4>		m_Mapped_Param_DiffuseColors;

	void			ClearBuffers()
	{
		m_Instanced_Matrices.UnmapAndClear();
		m_Instanced_Param_DiffuseColors.UnmapAndClear();
		m_Mapped_Matrices.Clear();
		m_Mapped_Param_DiffuseColors.Clear();
	}

	void			Clear()
	{
		if (m_TotalParticleCount == 0)
			return;
		DC_Clear();
		m_TotalParticleCount = 0;
		ClearBuffers();
	}

	bool			Init(CParticleRenderManager *renderManager);

	bool			PrepareBuffersIFN();

	static bool		PreSetupMeshData(FPopcornFXMeshVertexFactory::FDataType &outMeshData, const FStaticMeshLODResources &meshResources);

private:
	bool			_PrepareBuffersIFN(); // lazy
};

//----------------------------------------------------------------------------

class CParticleDrawCall_MeshSingleSection : public CParticleDrawCall_BaseMesh
{
public:
	const CParticleMaterialDesc_MeshSingleSection_Render	*m_MaterialDesc;

	PopcornFX::TArray<CParticleBillboarder_Mesh_CPU*>		m_BillboardersToWait;

	class FPopcornFXMeshCollector_2				*m_CurrentOneFrameCollector = null;
	const FStaticMeshLODResources				*m_RenderResources;
	hh_u32										m_SectionIndex;
	FPopcornFXMeshVertexFactory::FDataType		m_VFData;

	bool			Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_MeshSingleSection_Render *desc);
	void			AssocBillboader(CParticleBillboarder_Mesh_CPU *billboarder, hh_u32 meshi);

	virtual bool	StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame) override { return true; }
	virtual void	DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount) override;

};

//----------------------------------------------------------------------------

class CParticleDrawCall_MeshMultiSection : public CParticleDrawCall_BaseMesh
{
public:
	struct SSection
	{
		hh_u32							m_SectionIndex;
		FMaterialRenderProxy			*m_MaterialRenderProxy;
		bool							m_CastShadow;
		//CParticleMaterialDesc_Render	m_MatDesc;
	};

public:
	const CParticleMaterialDesc_MeshMultiSection_Render	*m_MaterialDesc;

	CParticleBillboarder_Mesh_CPU				*m_BillboarderToWait;
	class FPopcornFXMeshCollector_2				*m_CurrentOneFrameCollector = null;

	PopcornFX::TArray<SSection>					m_Sections;

	const FStaticMeshLODResources				*m_RenderResources;
	FPopcornFXMeshVertexFactory::FDataType		m_VFData;

	bool			Init(CParticleRenderManager *renderManager, CParticleBillboarder_Mesh_CPU *billboarder, hh_u32 meshDesci, const CParticleMaterialDesc_MeshMultiSection_Render *matDesc);

	//bool			Init(CParticleRenderManager *renderManager, const CParticleMaterialDesc_MeshSingleSection *desc);
	//void			AssocBillboader(CParticleBillboarder_Mesh_CPU *billboarder, hh_u32 meshi);

	virtual bool	StartDrawCall(PopcornFX::CRendererSubView &views, bool newFrame) override { return true; }
	virtual void	DrawCall(PopcornFX::CRendererSubView &views, hh_u32 bbViewMask, hh_u32 elementStart, hh_u32 elementCount) override;

};
