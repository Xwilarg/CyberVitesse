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
#include <hh_kernel/include/kr_containers_static.h>
#include <hh_kernel/include/kr_workingbuffer.h>
#include <hh_kernel/include/kr_callbacks.h>
#include <hh_particles/include/ps_stream.h>

#include "pk_drawers/include/dr_common.h"
#include "pk_drawers/include/dr_tasks.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CRibbonBillboarder;

namespace Drawers
{
	struct	SSubView;
	struct	SIndexStream;

	class	CRibbon_CPU;
	struct	SRibbon_DrawRequest;
	struct	SRibbon_CPU_DrawRequest;

	struct	PK_BILLBOARDER_EXPORT SRibbon_ExecBatch
	{
		const SRibbon_CPU_DrawRequest		*m_DrawRequest;
		// offsets inside draw request
		hh_u32			m_ParticleOffset;
		hh_u32			m_VertexOffset;
		hh_u32			m_IndexOffset;
		hh_u32			m_ParticleCount;
		hh_u32			m_VertexCount;
		hh_u32			m_IndexCount;

		// return m_DrawRequest->offset + offset
		hh_u32			FullParticleOffset() const;
		hh_u32			FullVertexOffset() const;
		hh_u32			FullIndexOffset() const;
	};

	//----------------------------------------------------------------------------

	namespace Task
	{
		class	PK_BILLBOARDER_EXPORT CRibbon_CPU_ExecCb : public Task::CBase
		{
		public:
			FastDelegate<void(const SRibbon_ExecBatch&)>	m_Cb;
			SRibbon_ExecBatch								m_Batch;

			virtual bool			_VirtualLaunch() override;
			//virtual void			_OnRefptrDestruct() override
			//{
			//	m_Batch.
			//}
		};
		HH_DECLARE_REFPTRCLASS(Ribbon_CPU_ExecCb);

		class	PK_BILLBOARDER_EXPORT CRibbon_CPU_ExecSortIndices: public Task::CBase
		{
		public:
			const CRibbon_CPU		*m_Self;
			virtual bool			_VirtualLaunch() override;
		};
		HH_DECLARE_REFPTRCLASS(Ribbon_CPU_ExecSortIndices);

	} // namespace Task

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
