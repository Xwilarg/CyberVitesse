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
#include "pk_drawers/include/dr_billboard_cpu_tasks.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace Drawers
{
	struct	SSubView;
	struct	SIndexStream;

	class	CMesh_CPU;
	struct	SMesh_DrawRequest;

	struct	SMesh_ExecPage
	{
		const SMesh_DrawRequest							*m_DrawRequest;
		const CParticlePageToRender_MainMemory			*m_Page;
		hh_u32											m_ParticleOffset;
	};

	namespace Task
	{
		class	PK_BILLBOARDER_EXPORT CMesh_CPU_ExecCb : public Task::CBase
		{
		public:
			const CMesh_CPU											*m_Self;
			const TUnlockedCallbackChain<void(const CMesh_CPU&)>	*m_Cb;
			virtual bool		_VirtualLaunch() override;
		};
		HH_DECLARE_REFPTRCLASS(Mesh_CPU_ExecCb);

	} // namespace Task

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
