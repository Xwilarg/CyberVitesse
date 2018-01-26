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
#include <hh_kernel/include/kr_callbacks.h>
#include <hh_particles/include/ps_stream.h>

#include "pk_drawers/include/dr_common.h"
#include "pk_drawers/include/dr_tasks.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBillboarder;

namespace Drawers
{
	struct	SSubView;
	struct	SIndexStream;

	class	CBillboard_CPU;
	struct	SBillboard_DrawRequest;

	class	CScene;

	struct	PK_BILLBOARDER_EXPORT SBillboard_ExecPage
	{
		const CScene							*m_Scene;
		const SBillboard_DrawRequest			*m_DrawRequest;
		const CBillboarder						*m_Billboarder;
		const CParticlePageToRender_MainMemory	*m_Page;
		hh_u32									m_ParticleOffset;
		hh_u32									m_VertexOffset;
		hh_u32									m_IndexOffset;
	};

	namespace Task
	{
		enum
		{
			kParticlePagePerTask = 8
		};

		class	PK_BILLBOARDER_EXPORT CBillboard_CPU_PagesBatch : public Task::CBase
		{
		public:
			TStaticCountedArray<const CParticlePageToRender_MainMemory*, kParticlePagePerTask>	m_Pages;

			const CBillboard_CPU		*m_Self;
			SBillboard_ExecPage			m_StartBatch;
			hh_u32						m_BatchPEnd;
			hh_u32						m_BatchVEnd;
			hh_u32						m_BatchIEnd;

			virtual bool				_VirtualLaunch() override;
			virtual void				_OnRefptrDestruct() override
			{
				// release the ref to the page -> otherwise, the page will never be returned to the free-pool.
				for (hh_u32 i = 0; i < m_Pages.Count(); i++)
					m_Pages[i] = null;
				CAsynchronousPooledJob::_OnRefptrDestruct();
			}
		};
		HH_DECLARE_REFPTRCLASS(Billboard_CPU_PagesBatch);

		class	PK_BILLBOARDER_EXPORT CBillboard_CPU_ExecCb : public Task::CBase
		{
		public:
			const CBillboard_CPU										*m_Self;
			const TUnlockedCallbackChain<void(const CBillboard_CPU&)>	*m_Cb;
			virtual bool		_VirtualLaunch() override;
		};
		HH_DECLARE_REFPTRCLASS(Billboard_CPU_ExecCb);

	} // namespace Task

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
