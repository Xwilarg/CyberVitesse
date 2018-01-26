#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/01/08 11:34 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include <hh_kernel/include/kr_refptr.h>
#include <hh_kernel/include/kr_memoryviews.h>
#include <hh_kernel/include/kr_containers_array.h>
#include <hh_kernel/include/kr_containers_onstack.h> // for TMesh_Exec_MaterialParam
#include <hh_particles/include/ps_renderers_classes.h>

#include "pk_drawers/include/dr_common.h"
#include "pk_drawers/include/dr_mesh.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBillboarder;
struct	SParticleStreamDef;

namespace Drawers
{
	class	CScene;
	struct	SMesh_ExecPage;

	//----------------------------------------------------------------------------
	//
	// CMesh_CPU: CPU tasks billboards meshes
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CMesh_CPU
	{
	public:
		// A single CMesh_CPU must only contain compatible Renderer/DrawRequests
		static bool		AreBillboardingCompatible(const CParticleRenderer_Mesh *rendererA, const CParticleRenderer_Mesh *rendererB HH_ONLY_IF_ASSERTS(COMMA bool asserts = false));

		typedef hh_u8	hh_umeshid;

		CMesh_CPU();
		~CMesh_CPU();

		void			Clean(); // Clear and release resources
		void			Clear();
		void			ClearExecTasks();

		// Prepare for draw requests
		// (will clear ExecTasks and flags)
		bool			Prepare(const TMemoryView<const SMesh_DrawRequest> &drawRequets);

		const TMemoryView<const SMesh_DrawRequest>			&DrawRequests() const { return m_DrawRequests; }
		bool			Empty() const { return m_TotalParticleCount == 0; }
		hh_u32			TotalParticleCount() const { return m_TotalParticleCount; }
		hh_u32			MeshCount() const { return m_MeshCount; }
		hh_u32			ActiveMeshCount() const { return m_ActiveMeshCount; }

		// if not !HasMeshIds(), it's always 0
		hh_u32							HasMeshIds() const { return m_HasMeshIds; }
		// available if computeCountAndIndicesIFN and HasMeshIds()
		TMemoryView<const hh_umeshid>	MeshIndices() const { return m_MeshIndices.View(); }
		TMemoryView<const hh_u32>		PerMeshParticleCount() const { return m_PerMeshParticleCount.View(); }

		hh_u32							MeshParticleCount(hh_u32 meshi) const { return !m_HasMeshIds ? m_TotalParticleCount : m_PerMeshParticleCount[meshi]; }

		// Setup tasks callbacks
		template <typename _TObj> void AddExec(_TObj *functor) { m_ExecTasks += FastDelegate<void(const CMesh_CPU&)>(functor, &_TObj::operator ()); }

		void			LaunchTasks(CScene &scene, bool kickJobs = true);

		bool			PendingTasks();
		void			WaitTasks();

	public:
		TUnlockedCallbackChain<void(const CMesh_CPU&)>			m_ExecTasks;

	private:
		bool			ComputeMeshCountAndIndices();
		void			Exec_TriggerFinish(const CMesh_CPU &self);

	private:
		bool									m_ThreadedTasks;
		TMemoryView<const SMesh_DrawRequest>	m_DrawRequests;
		hh_u32					m_MeshCount;
		hh_u32					m_ActiveMeshCount;
		hh_u32					m_TotalParticleCount;
		bool					m_HasMeshIds;
		TArray<hh_umeshid>		m_MeshIndices;
		TArray<hh_u32>			m_PerMeshParticleCount;

		Threads::CEvent			m_CompletionEvent;
		TAtomic<bool>			m_Completed;

	};

	//----------------------------------------------------------------------------
	//
	//	Mesh Exec
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CMesh_Exec_Matrices
	{
	public:
		float												m_MeshUniformScale;
		float												m_PositionsScale;
		TStridedMemoryView<TStridedMemoryView<CFloat4x4> >	m_PerMeshMatrices;

		CMesh_Exec_Matrices() : m_MeshUniformScale(1.f), m_PositionsScale(1.f) { }
		void	Clear() { Mem::Reinit(*this); }

		bool	SetupedAndAddExec(CMesh_CPU &bb);

	private:
		friend class CMesh_CPU;
		void	operator()(const CMesh_CPU &batch);
	};

	//----------------------------------------------------------------------------
	//
	//	CMesh_Exec_MaterialParam
	//
	//----------------------------------------------------------------------------

	class	CMesh_Exec_MaterialParam
	{
	public:
		CMesh_Exec_MaterialParam();

		void		Clear() { Mem::Reinit(*this); }

		template <typename _ParamType>
		bool		SetupedAndAddExec(CMesh_CPU &bb, CStringId materialParamName, const TStridedMemoryView<const TStridedMemoryView<_ParamType> > &perMeshOutputValues, const _ParamType &defaultValue);

		template <typename _Type> _Type			&DefaultValue() { HH_ASSERT(TBaseTypeID<_Type>::TypeID == m_Type); return *reinterpret_cast<_Type*>(m_DefaultValue); }
		template <typename _Type> const _Type	&DefaultValue() const { HH_ASSERT(TBaseTypeID<_Type>::TypeID == m_Type); return *reinterpret_cast<const _Type*>(m_DefaultValue); }

	private:
		bool		_SetupedAndAddExec(CMesh_CPU &bb, CStringId materialParamName);
		friend class CMesh_CPU;
		void		operator()(const CMesh_CPU &batch);

	private:
		friend struct _CMesh_Exec_MaterialParam;

		EBaseTypeID			m_Type;
		CStringId			m_MaterialName;
		hh_u32				m_DefaultValue[4];
		TStridedMemoryView<const SStridedMemoryViewRawStorage> m_RawPerMeshValues;
	};

	//----------------------------------------------------------------------------

	template <typename _ParamType>
	bool		CMesh_Exec_MaterialParam::SetupedAndAddExec(CMesh_CPU &bb, CStringId materialParamName, const TStridedMemoryView<const TStridedMemoryView<_ParamType> > &perMeshOutputValues, const _ParamType &defaultValue)
	{
		HH_ASSERT(bb.MeshCount() > 0);
		HH_STATIC_ASSERT(sizeof(_ParamType) <= sizeof(m_DefaultValue));
		m_Type = TBaseTypeID<_ParamType>::TypeID;
		m_MaterialName = materialParamName;
		DefaultValue<_ParamType>() = defaultValue;
		if (!HH_VERIFY(perMeshOutputValues.Count() == bb.MeshCount()))
		{
			m_RawPerMeshValues.Clear();
			return false;
		}
		HH_STATIC_ASSERT(sizeof(TStridedMemoryView<_ParamType>) == sizeof(SStridedMemoryViewRawStorage));
		m_RawPerMeshValues = TStridedMemoryView<const SStridedMemoryViewRawStorage>(
			reinterpret_cast<const SStridedMemoryViewRawStorage*>(perMeshOutputValues.Data()),
			perMeshOutputValues.Count(),
			perMeshOutputValues.Stride()
		);
		if (!_SetupedAndAddExec(bb, materialParamName))
		{
			m_RawPerMeshValues.Clear();
			return false;
		}
		return true;
	}


} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
