#pragma once

//----------------------------------------------------------------------------
// Started on Tue 2003/10/14 22:29:24 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_INIT_H__
#define	__KR_INIT_H__

#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_profiler_details.h"
#include "hh_kernel/include/kr_scheduler.h"
#include "hh_kernel/include/kr_caps_cpu.h"
#include "hh_kernel/include/kr_caps_mem.h"

#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hellheaven_api_library.h>
#include <hh_license_keycode.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

#ifndef	HH_THREADS_DEFAULT_STACK_SIZE_KB
#	ifdef	HH_PS3
#		ifdef	HH_DEBUG
#			define	HH_THREADS_DEFAULT_STACK_SIZE_KB	128		// 128 Kb in debug : need larger stackframes
#		else
#			define	HH_THREADS_DEFAULT_STACK_SIZE_KB	64		// 64 Kb
#		endif
#	else
#		define	HH_THREADS_DEFAULT_STACK_SIZE_KB		1024	// 1 Mb
#	endif
#endif

//----------------------------------------------------------------------------

class	IFileSystemController;

class	HH_KERNEL_EXPORT CHHKernelBase
{
public:
	struct	HH_KERNEL_EXPORT Config
	{
		Assert::CbCatcher					m_AssertCatcher;

#if	(KR_MEM_NO_DEFAULT_ALLOCATORS == 0)
		// Replace system alloction function called by the default allocator
		// (keeping default allocator debugs and stats)
		Mem::FnSysMalloc					m_DefaultAllocator_Alloc;
		Mem::FnSysRealloc					m_DefaultAllocator_Realloc;
		Mem::FnSysFree						m_DefaultAllocator_Free;
		Mem::FnOutOfMemory					m_DefaultAllocator_OutOfMemory;
#endif

		// Completely overrides default allocator functions (no more stats, nor debugs)
		Mem::FnRawAlloc						m_Allocator_Alloc;
		Mem::FnRawRealloc					m_Allocator_Realloc;
		Mem::FnRawFree						m_Allocator_Free;

		Logger::FnAddListeners				m_AddDefaultLogListeners;
		IFileSystemController				*m_FsController;

		void								*m_ProfilerRecordArg;
		Profiler::FnRecordEventStart		m_ProfilerRecordEventStart;
		Profiler::FnRecordEventEnd			m_ProfilerRecordEventEnd;
		Profiler::FnRecordMemoryTransaction	m_ProfilerRecordMemoryTransaction;
		Profiler::FnRecordThreadDependency	m_ProfilerRecordThreadDependency;

		hh_u32								m_DefaultThreadStackSize;
		Scheduler::FnCreateThreadPool		m_CreateThreadPool;
		Scheduler::FnDestroyThreadPool		m_DestroyThreadPool;
		void								(*m_OnThreadStarted)();
		void								(*m_OnThreadStopped)();

		// one 32-bit mask per worker, leave to null to let popcorn schedule its workers.
		// if you want specific scheduling, each nonzero mask in the array below will create
		// a worker thread. Each active bit in the mask means the worker should have the
		// corresponding HW thread included in its final affinity mask.
		// see the documentation for more details and examples.
		const hh_u32						*m_WorkerAffinityMasks;
		hh_u32								m_WorkerAffinityMasksCount;
		CThreadManager::EPriority			m_WorkerDefaultPriority;

		CPU::CCaps							*m_CapsCPU;
		Mem::CCaps							*m_CapsMem;

		const char							*m_LicenseKey;

		void								*m_UserHandle;

		Config(const char *licenseKey = HH_LICENSE_KEY, hh_u32 threadStack = HH_THREADS_DEFAULT_STACK_SIZE_KB * 1024);
	};

protected:
	static bool			InternalStartup(const Config &config);
	static bool			InternalShutdown();

	static bool			m_Active;
public:
	static const char	*LibraryName() { return "HH-Kernel"; }
	static bool			Active() { return m_Active; }
};

//----------------------------------------------------------------------------

typedef	THHLibrary<CHHKernelBase>	CHHKernel;

// FIXME: Uncomment this for dlls, but THHLibrary<Type> will need to be instanciated inside a cpp
// template class	HH_KERNEL_EXPORT THHLibrary<CHHKernelBase>;

//----------------------------------------------------------------------------
__PK_API_END

#endif
