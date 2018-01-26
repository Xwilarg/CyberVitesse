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

#include <hh_kernel/include/kr_threads_job_pool.h>
#include <hh_geometrics/include/ge_billboards_ribbon.h>

#include "pk_drawers/include/dr_billboard_cpu_tasks.h"
#include "pk_drawers/include/dr_ribbon_cpu_tasks.h"
#include "pk_drawers/include/dr_mesh_cpu_tasks.h"
#include "pk_drawers/include/dr_copystream_tasks.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace Drawers
{
	class	PK_BILLBOARDER_EXPORT CScene
	{
	public:
		TAsynchronousJobPool<Task::CEmpty, 16>							m_JobPool_Empty;
		TAsynchronousJobPool<Task::CExecCallback, 16>					m_JobPool_ExecCallback;

		TAsynchronousJobPool<Task::CBillboard_CPU_PagesBatch, 64>		m_JobPool_Billboard_CPU_PagesBatch;
		TAsynchronousJobPool<Task::CBillboard_CPU_ExecCb, 16>			m_JobPool_Billboard_CPU_ExecCb;

		TAsynchronousJobPool<Task::CRibbon_CPU_ExecCb, 64>				m_JobPool_Ribbon_CPU_ExecCb;
		TAsynchronousJobPool<Task::CRibbon_CPU_ExecSortIndices, 16>		m_JobPool_Ribbon_CPU_ExecSortIndices;
		CRibbon_ThreadSort::CJobPool									m_JobPool_Ribbon_ThreadSort;

		TAsynchronousJobPool<Task::CMesh_CPU_ExecCb, 16>				m_JobPool_Mesh_CPU_ExecCb;

		TAsynchronousJobPool<Task::CCopyStream_PagesBatch, 64>			m_JobPool_CopyStream_PagesBatch;
		TAsynchronousJobPool<Task::CCopyStream_ExecCb, 16>				m_JobPool_CopyStream_ExecCb;

		CScene();
		~CScene();
	};

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
