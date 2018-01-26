#pragma once

//----------------------------------------------------------------------------
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
#include "pk_drawers/include/dr_draw_requests.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBillboarder;

namespace Drawers
{
	struct	SSubView;
	struct	SIndexStream;

	class	CCopyStream;
	struct	SBase_DrawRequest;

	struct	PK_BILLBOARDER_EXPORT SCopyStream_ExecPage
	{
		const SBase_DrawRequest					*m_DrawRequest;
		const CCopyStream						*m_BB;
		hh_u32									m_DrawRequestIndex; // in CCopyStream
		const CParticlePageToRender_MainMemory	*m_Page;
		hh_u32									m_ParticleOffset;
	};

	namespace Task
	{
		enum
		{
			kCopyStream_ParticlePagePerTask = 8
		};

		class	PK_BILLBOARDER_EXPORT CCopyStream_PagesBatch : public Task::CBase
		{
		public:
			TStaticCountedArray<const CParticlePageToRender_MainMemory*, kCopyStream_ParticlePagePerTask>	m_Pages;
			const CCopyStream			*m_Self;
			SCopyStream_ExecPage		m_StartBatch;
			hh_u32						m_BatchPEnd;
			virtual bool				_VirtualLaunch() override;
			virtual void				_OnRefptrDestruct() override
			{
				// release the ref to the page -> otherwise, the page will never be returned to the free-pool.
				for (hh_u32 i = 0; i < m_Pages.Count(); i++)
					m_Pages[i] = null;
				CAsynchronousPooledJob::_OnRefptrDestruct();
			}
		};
		HH_DECLARE_REFPTRCLASS(CopyStream_PagesBatch);

		class	PK_BILLBOARDER_EXPORT CCopyStream_ExecCb : public Task::CBase
		{
		public:
			const CCopyStream										*m_Self;
			const TUnlockedCallbackChain<void(const CCopyStream&)>	*m_Cb;
			virtual bool		_VirtualLaunch() override;
		};
		HH_DECLARE_REFPTRCLASS(CopyStream_ExecCb);

	}

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
