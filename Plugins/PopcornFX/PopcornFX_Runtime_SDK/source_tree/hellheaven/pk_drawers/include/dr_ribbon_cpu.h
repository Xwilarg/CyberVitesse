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
#include <hh_particles/include/ps_renderers_classes.h>
#include <hh_geometrics/include/ge_billboards_ribbon.h>

#include "pk_drawers/include/dr_common.h"
#include "pk_drawers/include/dr_ribbon.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace Drawers
{
	struct	SRibbon_ExecBatch;
	struct	SRibbon_CPU_DrawRequest;
	class	CRibbon_CPU;
	class	CScene;

	//----------------------------------------------------------------------------
	//
	//	IRibbonSorter: Ribbon sorter interface
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT IRibbonSorter
	{
	public:
		// output
		hh_u64															m_OutSort_RibbonIdMask;
		TStridedMemoryView< const hh_u64 >								m_OutSort_RibbonIds;
		TStridedMemoryView< const CRibbonBillboarder::SCentersIndex >	m_OutSort_Indices;

		virtual ~IRibbonSorter() { }
		// returns the task to wait for (must NOT be added to the pool)
		virtual PAsynchronousJob		SpawnSort(CScene &scene, const CRibbon_CPU &bb, const SRibbon_CPU_DrawRequest &dr) = 0;
		virtual void					ImmediateSort(const CRibbon_CPU &bb, const SRibbon_CPU_DrawRequest &dr) = 0;
	};

	//----------------------------------------------------------------------------
	//
	//	CRibbonSorter: Ribbon sorter standart implementation
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbonSorter : public IRibbonSorter
	{
	private:
		CRibbon_ThreadSort				m_RibbonSorter;
	public:
		virtual ~CRibbonSorter() { }
		virtual PAsynchronousJob		SpawnSort(CScene &scene, const CRibbon_CPU &bb, const SRibbon_CPU_DrawRequest &dr) override;
		virtual void					ImmediateSort(const CRibbon_CPU &bb, const SRibbon_CPU_DrawRequest &dr) override;
	private:
		bool	_Setup(const CRibbon_CPU &bb, const SRibbon_CPU_DrawRequest &dr);
	};

	//----------------------------------------------------------------------------
	//
	//	SRibbon_CPU_DrawRequest: Augmented draw request cache for CPU billboarding
	//
	//----------------------------------------------------------------------------

	struct	PK_BILLBOARDER_EXPORT SRibbon_CPU_PageCache
	{
		TStridedMemoryView<const hh_u32>	m_SpawnerIDs;
		TStridedMemoryView<const hh_u32>	m_ParentIDs;
		TStridedMemoryView<const hh_u32>	m_SelfIDs;

		TStridedMemoryView<const CFloat3>	m_Positions;
		TStridedMemoryView<const float>		m_Widths;
		TStridedMemoryView<const CFloat4>	m_Colors;
		TStridedMemoryView<const float>		m_TextureUs;
		TStridedMemoryView<const float>		m_TextureIDs;
		TStridedMemoryView<const CFloat3>	m_Axis;
		TStridedMemoryView<const float>		m_AlphaCursors;

		void	Setup(const SRibbon_DrawRequest &dr, const CParticlePageToRender_MainMemory &page);
	};

	//----------------------------------------------------------------------------

	struct	PK_BILLBOARDER_EXPORT SRibbon_CPU_DrawRequest
	{
		const SRibbon_DrawRequest					*m_DrawRequest;
		CRibbonBillboarder							*m_Billboarder;
		IRibbonSorter								*m_RibbonSorter;
		hh_u32										m_ParticleOffset;
		hh_u32										m_VertexOffset;
		hh_u32										m_IndexOffset;
		TMemoryView<const SRibbon_CPU_PageCache>	m_PageCaches;

		SRibbon_CPU_DrawRequest()
		:	m_Billboarder(null)
		,	m_RibbonSorter(null)
		{
		}

		SRibbon_CPU_DrawRequest(const SRibbon_CPU_DrawRequest &other)
		{
			Mem::Copy(this, &other, sizeof(*this));
			m_Billboarder = null;
			m_RibbonSorter = null;
		}

		~SRibbon_CPU_DrawRequest()
		{
			if (m_Billboarder != null)
				HH_DELETE(m_Billboarder);
			if (m_RibbonSorter != null)
				HH_DELETE(m_RibbonSorter);
		}
	};

	//----------------------------------------------------------------------------
	//
	// CRibbon_CPU: CPU tasks billboards ribbons
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_CPU
	{
	public:
		// A single CRibbon_CPU must only contain compatible Renderer/DrawRequests
		static bool		AreBillboardingCompatible(const CParticleRenderer_Ribbon *rendererA, const CParticleRenderer_Ribbon *rendererB HH_ONLY_IF_ASSERTS(COMMA bool asserts = false));

		CRibbon_CPU();
		~CRibbon_CPU();

		void			Clean(); // Clear and release resources
		void			Clear();
		void			ClearExecTasks();

		// Prepare for draw requests
		// (will clear ExecTasks and flags)
		bool			Prepare(const TMemoryView<const SRibbon_DrawRequest> &drawRequets);

		template <typename _TRibbonSorter>
		void			SetRibbonSorter() { m_NewRibbonSorter = &_NewRibbonSorter<_TRibbonSorter>; }

		// Setup tasks callbacks
		template <typename _TObj> void AddExecBatch(_TObj *functor) { m_ExecTasks_Batch += FastDelegate<void(const SRibbon_ExecBatch&)>(functor, &_TObj::operator ()); }
		template <typename _TObj> void AddExecFull(_TObj *functor) { m_ExecTasks_Full.PushBack(FastDelegate<void(const SRibbon_ExecBatch&)>(functor, &_TObj::operator ())); }
		template <typename _TObj> void AddExecSortIndices(_TObj *functor) { m_ExecTask_SortIndices += FastDelegate<void(const CRibbon_CPU&)>(functor, &_TObj::operator ()); }

		const TMemoryView<const SRibbon_DrawRequest>	&DrawRequests() const { return m_DrawRequests; }
		bool			Empty() const { return m_TotalParticleCount == 0; }
		hh_u32			TotalParticleCount() const { return m_TotalParticleCount; }
		hh_u32			TotalVertexCount() const { return m_TotalVertexCount; }
		hh_u32			TotalIndexCount() const { return m_TotalIndexCount; }

		CParticleRenderer_Ribbon::EQuality	Quality() const { return m_Quality; }
		bool								NeedSort() const { return m_NeedSort; }

		void			LaunchTasks(CScene &scene, bool kickJobs = true);

		bool			PendingTasks();
		void			WaitTasks();

	public: // should be private
		TUnlockedCallbackChain<void(const SRibbon_ExecBatch&)>		m_ExecTasks_Batch;
		TArray<FastDelegate<void(const SRibbon_ExecBatch&)> >		m_ExecTasks_Full;
		TUnlockedCallbackChain<void(const CRibbon_CPU&)>			m_ExecTask_SortIndices;

	private:
		void						Exec_BatchCallbacks(const SRibbon_ExecBatch &batch) { m_ExecTasks_Batch(batch); }
		void						Exec_TriggerFinish(const CRibbon_CPU &self);

		template <typename _Type>
		static IRibbonSorter		*_NewRibbonSorter() { return HH_NEW(_Type); }

		const CRibbonBillboarder	*GetTemplateBillboarder(CParticleRenderer_Ribbon::EBillboardMode billboardMode);

	private:
		bool				m_ThreadedTasks;

		IRibbonSorter		*(*m_NewRibbonSorter)();

		TMemoryView<const SRibbon_DrawRequest>	m_DrawRequests;
		TArray<SRibbon_CPU_DrawRequest>			m_RibbonDrawRequests;
		TArray<SRibbon_CPU_PageCache>			m_PageCaches;
		hh_u32									m_LastDrCount;

		CParticleRenderer_Ribbon::EQuality		m_Quality;
		bool									m_NeedSort;

		Threads::CEvent					m_CompletionEvent;
		TAtomic<bool>					m_Completed;

		hh_u32							m_TotalParticleCount;
		hh_u32							m_TotalVertexCount;
		hh_u32							m_TotalIndexCount;
	};

	//----------------------------------------------------------------------------
	//
	// Ribbon Exec
	//
	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_Exec_Positions
	{
	public:
		CFloat4x4							m_BillboardingMatrix;
		float								m_GlobalScale;
		TStridedMemoryView<CFloat3, 0x10>	m_Positions;
		TStridedMemoryView<CFloat3, 0x10>	m_Normals;
		TStridedMemoryView<CFloat4, 0x10>	m_Tangents;
		TStridedMemoryView<CFloat2>			m_UVFactors;
		TStridedMemoryView<CFloat4>			m_UVFactors4;

		CRibbon_Exec_Positions() : m_GlobalScale(1.f) { }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SRibbon_ExecBatch &ribbonData);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_Exec_Colors
	{
	public:
		TStridedMemoryView<CFloat4>		m_Colors;

		CRibbon_Exec_Colors() { }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SRibbon_ExecBatch &ribbonData);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_Exec_Texcoords
	{
	public:
		TStridedMemoryView<CFloat2>		m_Texcoords;
		bool							m_ForUVFactor;

		CRibbon_Exec_Texcoords() : m_ForUVFactor(false) {}
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SRibbon_ExecBatch &ribbonData);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_Exec_AlphaCursor
	{
	public:
		TMemoryView<float>			m_AlphaCursors;

		CRibbon_Exec_AlphaCursor() {}
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SRibbon_ExecBatch &ribbonData);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_Exec_UVRemap
	{
	public:
		TStridedMemoryView<CFloat4>		m_UVRemap;

		CRibbon_Exec_UVRemap() { }
		void	Clear() { Mem::Reinit(*this); }
		void	operator()(const SRibbon_ExecBatch &ribbonData);
	};

	//----------------------------------------------------------------------------

	class	PK_BILLBOARDER_EXPORT CRibbon_Exec_FillSortIndices
	{
	public:
		CFloat3								m_SortDirection;
		SIndexStream						m_IndexStream;
		TStridedMemoryView<CFloat3, 0x10>	m_Positions;

		CRibbon_Exec_FillSortIndices() {}
		void	Clear() { m_IndexStream.Clear(); m_Positions.Clear(); }
		bool	SetupedAndAddExec(CRibbon_CPU &bb, hh_i32 overrideNeedSort = -1);

	private:
		bool				m_GenToWb;
		CWorkingBuffer		m_WorkingBuffer;

		friend class CRibbon_CPU;
		void				operator()(const SRibbon_ExecBatch &self);
		void				operator()(const CRibbon_CPU &self);
	};

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
