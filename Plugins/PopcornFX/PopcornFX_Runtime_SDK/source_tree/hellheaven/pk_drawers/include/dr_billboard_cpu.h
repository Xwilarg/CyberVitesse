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
#include <hh_kernel/include/kr_workingbuffer.h>
#include <hh_particles/include/ps_renderers_classes.h>

#include "pk_drawers/include/dr_common.h"
#include "pk_drawers/include/dr_billboard.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBillboarder;
struct	SParticleStreamDef;

namespace Drawers
{
	class	CScene;
	struct	SBillboard_ExecPage;


	//----------------------------------------------------------------------------
	//
	// CBillboard_CPU: CPU tasks billboards billboards
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CBillboard_CPU : public CNonCopyable
	{
	public:
		// A single CBillboard_CPU must only contain compatible Renderer/DrawRequests
		static bool		AreBillboardingCompatible(const CParticleRenderer_Billboard *rendererA, const CParticleRenderer_Billboard *rendererB HH_ONLY_IF_ASSERTS(COMMA bool asserts = false));

		CBillboard_CPU();
		~CBillboard_CPU();

		void			Clean(); // Clear and release resources
		void			Clear();
		void			ClearExecTasks();

		// Prepare for draw requests
		// (will clear ExecTasks and flags)
		bool			Prepare(const TMemoryView<const SBillboard_DrawRequest> &drawRequets);

		// Available after Prepare():

		TMemoryView<const SBillboard_DrawRequest * const>	DrawRequests() const { return m_DrawRequests.View(); }

		bool			Empty() const { return m_TotalParticleCount == 0; }
		hh_u32			TotalParticleCount() const { return m_TotalParticleCount; }
		hh_u32			TotalVertexCount() const { return m_TotalVertexCount; }
		hh_u32			TotalIndexCount() const { return m_TotalIndexCount; }

		// Draw requests are sorted ["all vpp4" then "all vpp6"]
		hh_u32			VPP4_DrawRequestCount() const { return m_VPP4_DrawRequestCount; }
		hh_u32			VPP4_ParticleCount() const { return m_VPP4_ParticleCount; }
		hh_u32			VPP6_DrawRequestCount() const { return m_DrawRequests.Count() - VPP4_DrawRequestCount(); }
		hh_u32			VPP6_ParticleCount() const { return m_TotalParticleCount - VPP4_ParticleCount(); }

		bool			NeedSort() const { return m_NeedSort; }
		bool			IndicesPerParticleDiffers() const { return m_IndicesPerParticleDiffers; }
		bool			VertexPerParticleDiffers() const { return m_IndicesPerParticleDiffers; } // same
		bool			SoftAnimationBlending() const { return m_SoftAnimationBlending; }

		// Setup tasks callbacks
		// async called for each pages
		template <typename _TObj> void AddExecPage(_TObj *functor) { m_ExecTasks_Page += FastDelegate<void(const SBillboard_ExecPage&)>(functor, &_TObj::operator ()); }
		// async called once (while ExecPages runs at the same time)
		template <typename _TObj> void AddExecDuringPages(_TObj *functor) { m_ExecTasks_DuringPages += FastDelegate<void(const CBillboard_CPU&)>(functor, &_TObj::operator ()); }
		// async called once after all ExecPages
		template <typename _TObj> void AddExecAfterPages(_TObj *functor) { m_ExecTasks_AfterPages += FastDelegate<void(const CBillboard_CPU&)>(functor, &_TObj::operator ()); }

		// Fullfiled after LaunchTasks
		bool			m_NeedPositionsCollection; // = false

		void			LaunchTasks(CScene &scene, bool kickJobs = true);

		TMemoryView< const TStridedMemoryView<const CFloat3> >	PositionsCollection() const { HH_ASSERT(m_NeedPositionsCollection); HH_ASSERT(!m_PositionsCollection.Empty()); return m_PositionsCollection.View(); }
		const CBillboarder										*BillboarderForIndexPattern() const { HH_ASSERT(!IndicesPerParticleDiffers()); return m_BBForIndexPattern; }

		bool			PendingTasks();
		void			WaitTasks();

		const CBillboarder		*GetBillboarder(CParticleRenderer_Billboard::EBillboardMode mode);

	public: // should be private
		TUnlockedCallbackChain<void(const SBillboard_ExecPage&)>	m_ExecTasks_Page;
		TUnlockedCallbackChain<void(const CBillboard_CPU&)>			m_ExecTasks_DuringPages;
		TUnlockedCallbackChain<void(const CBillboard_CPU&)>			m_ExecTasks_AfterPages;

	private:
		void					Exec_TriggerFinish(const CBillboard_CPU &self);

	private:
		bool					m_ThreadedTasks;
		bool					m_IndicesPerParticleDiffers;
		bool					m_NeedSort;
		bool					m_SoftAnimationBlending;

		TArray<const SBillboard_DrawRequest*>	m_DrawRequests;

		Threads::CEvent			m_CompletionEvent;
		TAtomic<bool>			m_Completed;

		const CBillboarder		*m_BBForIndexPattern;

		hh_u32					m_VPP4_DrawRequestCount;
		hh_u32					m_VPP4_ParticleCount;

		TArray< TStridedMemoryView<const CFloat3> >	m_PositionsCollection;
		hh_u32					m_TotalParticleCount;
		hh_u32					m_TotalVertexCount;
		hh_u32					m_TotalIndexCount;

		TStaticArray<CBillboarder*, CParticleRenderer_Billboard::__MaxBillboardModes>	m_Billboarders;
	};

	//----------------------------------------------------------------------------
	//
	//	Billboard Exec
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CBillboard_Exec_PositionsNormals
	{
	public:
		CFloat4x4							m_BillboardingMatrix;
		float								m_GlobalScale;
		TStridedMemoryView<CFloat3, 0x10>	m_Positions;
		TStridedMemoryView<CFloat3, 0x10>	m_Normals;
		TStridedMemoryView<CFloat4, 0x10>	m_Tangents;

		CBillboard_Exec_PositionsNormals() : m_GlobalScale(1.f) { }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SBillboard_ExecPage &batch);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CBillboard_Exec_Colors
	{
	public:
		TStridedMemoryView<CFloat4>			m_Colors;
		TStridedMemoryView<CUbyte4>			m_ColorU32s;

		CBillboard_Exec_Colors() { }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SBillboard_ExecPage &batch);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CBillboard_Exec_Texcoords
	{
	public:
		bool								m_FillTexcoord01;
		TStridedMemoryView<CFloat2>			m_Texcoords;
		TStridedMemoryView<CWord2>			m_TexcoordS16s;
		// SoftAnimationBlending: (Texcoords + Texcoord2s + AtlasIds)
		TStridedMemoryView<CFloat2>			m_Texcoord2s;
		TStridedMemoryView<CWord2>			m_Texcoord2S16s;
		TMemoryView<float>					m_AtlasIds;

		CBillboard_Exec_Texcoords() : m_FillTexcoord01(false) {}
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SBillboard_ExecPage &batch);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CBillboard_Exec_AlphaCursor
	{
	public:
		TMemoryView<float>			m_AlphaCursors;

		CBillboard_Exec_AlphaCursor() {}
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SBillboard_ExecPage &batch);
	};

	//----------------------------------------------------------------------------

	class CBillboard_Exec_Indices;

	typedef FastDelegate<void (const CBillboard_CPU &, const CBillboard_Exec_Indices&)>			CbBillboard_Exec_Indices_PostSort;

	class	PK_BILLBOARDER_EXPORT CBillboard_Exec_Indices
	{
	public:
		hh_i32								m_OverrideSort; // -1: bb.NeedSort() // 0 : force no sort // 1 : force sort
		CFloat3								m_SortDirection;
		SIndexStream						m_IndexStream;
		TStridedMemoryView<CFloat3, 0x10>	m_Positions;

		CbBillboard_Exec_Indices_PostSort	m_PostSortCb;

		CBillboard_Exec_Indices() : m_OverrideSort(-1) {}
		void	Clear() { m_OverrideSort = -1; m_IndexStream.Clear(); m_Positions.Clear(); m_PostSortCb = CbBillboard_Exec_Indices_PostSort(); }

		// bb must have been Prepare(), and m_OverrideSort specified IFN
		hh_u32		WorkingBufferSize(CBillboard_CPU &bb) const;

		bool		SetupedAndAddExec(CBillboard_CPU &bb, void *workingBuffer, hh_u32 workingBufferSize);

	private:
		void				*m_WorkingBuffer;
		hh_u32				m_WorkingBufferSize;
		bool				m_GenToWb;

		friend class CBillboard_CPU;
		void				operator()(const SBillboard_ExecPage &bb);
		void				operator()(const CBillboard_CPU &bb);
	};

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
