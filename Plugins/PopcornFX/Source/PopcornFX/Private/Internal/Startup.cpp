//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "Startup.h"

#include "FileSystemController_UE.h"
#include "ResourceHandlerMesh_UE.h"
#include "ResourceHandlerImage_UE.h"

#include "GenericPlatform/GenericPlatformMemory.h"
#include "HAL/PlatformAffinity.h"

#include "PopcornFXSDK.h"

#include <hh_version.h>
#include <hh_kernel/include/kr_init.h>
#include <hh_base_object/include/hb_init.h>
#include <hh_engine_utils/include/eu_init.h>
#include <hh_compiler/include/cp_init.h>
#include <hh_imaging/include/im_init.h>
#include <hh_geometrics/include/ge_init.h>
#include <hh_geometrics/include/ge_coordinate_frame.h>
#include <hh_geometrics/include/ge_billboards.h>
#include <hh_particles/include/ps_init.h>
#include <pk_drawers/include/dr_init.h>
#include <hh_kernel/include/kr_static_config_flags.h>

// g_ModuleConfigMem
#include <hh_engine_utils/include/kr_config_base.h>

#include <hh_kernel/include/kr_mem_stats.h>

#include <hh_kernel/include/kr_buffer.h>
#include <hh_kernel/include/kr_assert_internals.h>

#include <hh_kernel/include/kr_thread_pool_default.h>

#include <hh_kernel/include/kr_plugins.h>

#include <hh_toolkit/include/hh_toolkit_version.h>

#define HH_ASSERT_DISPLAY_HBO_SCOPE		1

#if (HH_ASSERT_DISPLAY_HBO_SCOPE != 0)
#	include <hh_base_object/include/hbo_scope_details.h>
#	include <hh_particles/include/ps_debug.h>
#endif

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXStartup, Log, All);
#define LOCTEXT_NAMESPACE "PopcornFXStartup"

//----------------------------------------------------------------------------

HH_LOG_MODULE_DECLARE();

//----------------------------------------------------------------------------

#ifdef	HH_PLUGINS_STATIC

HH_PLUGIN_DECLARE(CCompilerBackendCPU_Emulation);

#if (PK_GPU_D3D11 == 1)
HH_PLUGIN_DECLARE(CCompilerBackendGPU_D3D11);
#endif

#	ifdef	HH_DEBUG
#		define	HH_PLUGIN_POSTFIX_BUILD		"_D"
#	else
#		define	HH_PLUGIN_POSTFIX_BUILD		""
#	endif
#	if	defined(HH_WINDOWS)
#		define	HH_PLUGIN_POSTFIX_EXT		".dll"
#	else
#		define	HH_PLUGIN_POSTFIX_EXT		""
#	endif
#endif

//----------------------------------------------------------------------------

void	AddDefaultGlobalListenersOverride(void *userHandle);

namespace
{
#if	(HH_ASSERTS_IN_DEBUG != 0) || (HH_ASSERTS_IN_RELEASE != 0)

	PopcornFX::TAtomic<hh_u32>		g_GlobalAssertId = 0;
	void	PrettyFormatAssert_Unsafe(char *buffer, hh_u32 bufferLen, PK_ASSERT_CATCHER_PARAMETERS)
	{
		using namespace PopcornFX;

		const CThreadID		threadId = CCurrentThread::ThreadID();
		const hh_u32		globalAssertId = g_GlobalAssertId++;
		const char			*baseObject = null;
#	if (HH_ASSERT_DISPLAY_HBO_SCOPE != 0)
		CString				_baseObjectCache;
		if (HBOScope::MainContext() != null)
		{
			TMemoryView<const HBOScope::SNodeRecord>	stack = HBOScope::MainContext()->CurrentStackNowAndHere();
			if (!stack.Empty())
			{
				BuildParticleObjectTreeViewName(_baseObjectCache, stack.Last().m_BaseObjectPath);
				baseObject = _baseObjectCache.Data();
			}
		}
#	endif
		// pointer compare ok (compiler probably removed duplicated static strings, else we dont care)
		const bool		pexp = (expanded != failed);
		const bool		pmsg = (message != failed && message != expanded);
		const bool		pbo = (baseObject != null);
		SNativeStringUtils::SPrintf(
			buffer, bufferLen,
			"!! PopcornFX Assertion failed !!"
			"\nFile       : %s(%d)"
			"\nFunction   : %s(...)"
			"%s%s"// Message
			"\nCondition  : %s"
			"%s%s" // Expanded
			"%s%s" // BaseObject
			"\nThreadID   : %d"
			"\nAssertNum  : %d"
			"\n"
			, file, line
			, function
			, (pmsg	? "\nMessage    : " : ""), (pmsg ? message : "")
			, failed
			, (pexp	? "\nExpanded   : " : ""), (pexp ? expanded : "")
			, (pbo	? "\nBaseObject : " : ""), (pbo ? baseObject : "")
			, hh_u32(threadId)
			, globalAssertId
			);
	}

	volatile bool							g_Asserting = false;
	PopcornFX::Threads::CCriticalSection	g_AssertLock;

	PopcornFX::Assert::EResult	AssertImpl(PK_ASSERT_CATCHER_PARAMETERS)
	{
		using namespace PopcornFX;

		HH_CRITICAL_SECTION_SCOPEDLOCK(g_AssertLock);

		if (g_Asserting) // assert recursion ! break now
		{
			UE_LOG(LogPopcornFXStartup, Warning, TEXT("!! PopcornFX Assertion Recursion !! %s(%d): '%s' '%s'"), file, line, failed, message);
#if defined(HH_DEBUG)
			HH_BREAKPOINT();
#endif
			return Assert::Result_Skip;
		}
		g_Asserting = true;

		char			_buffer[2048];
		PrettyFormatAssert_Unsafe(_buffer, sizeof(_buffer), PK_ASSERT_CATCHER_ARGUMENTS);
		const char		*perttyMessage = _buffer;
		UE_LOG(LogPopcornFXStartup, Warning, TEXT("%s"), ANSI_TO_TCHAR(perttyMessage));

		Assert::EResult		result = Assert::Result_Skip;

		// FIXME: on macosx, message box is queued on the main thread, we cannot do that ! need to workaround eg: spawn /usr/bin/osascript ?

#if PLATFORM_DESKTOP && (PLATFORM_WINDOWS || PLATFORM_LINUX)
		{
			EAppReturnType::Type	msgBoxRes;
			{
				// UE4 handles all WinProc messages then wait updates tasks before execute them >>> DEAD LOCK
				// GPumpingMessagesOutsideOfMainLoop = true should enqueue messages
				TGuardValue<bool>	pumpMessageGuard(GPumpingMessagesOutsideOfMainLoop, true);
				msgBoxRes = FPlatformMisc::MessageBoxExt(EAppMsgType::CancelRetryContinue, ANSI_TO_TCHAR(perttyMessage), TEXT("PopcornFX Assertion failed"));
			}
			switch (msgBoxRes)
			{
			case EAppReturnType::Cancel:
				result = Assert::Result_Break;
				break;
			case EAppReturnType::Retry:
				result = Assert::Result_Skip;
				break;
			case EAppReturnType::Continue:
				result = Assert::Result_Ignore;
				break;
			default:
				break;
			}
		}

#else // No msgbox avail

#	if defined(HH_DEBUG)
		if (FPlatformMisc::IsDebuggerPresent())
		{
			//
			// !! PopcornFX Assertion Failed !! (and no proper message box system)
			//
			// Look at the Log
			//
			HH_BREAKPOINT();

			// Ignore subsequent calls of this assert
			result = Assert::Result_Ignore;
		}
		else
		{
			// Just skip (dont break) (but still log all asserts)
			result = Assert::Result_Skip;
		}
#	else
		// Ignore subsequent calls of this asserts
		result = Assert::Result_Ignore;
#	endif

#endif

		{
			const TCHAR		*msg = null;
			switch (result)
			{
			case Assert::Result_Break:	msg = TEXT("! About to BREAK on this assert !"); break;
			case Assert::Result_Ignore:	msg = TEXT("Will ignore all subsequent calls of this assert"); break;
			case Assert::Result_Skip:	msg = null; break;
			}
			if (msg != null)
				UE_LOG(LogPopcornFXStartup, Warning, TEXT("Assert action: %s"), msg);
		}

		g_Asserting = false;

		return result;
	}

#endif // (HH_ASSERTS_IN_DEBUG != 0) || (HH_ASSERTS_IN_RELEASE != 0)


	bool			StartupPlugins()
	{
		using namespace PopcornFX;

		bool		success = true;

#ifdef	HH_PLUGINS_STATIC
		{
			const char	*backendPath = "Plugins/CBCPU_Emulation_Win32" HH_PLUGIN_POSTFIX_BUILD HH_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendCPU_Emulation();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#if (PK_GPU_D3D11 == 1)
		{
			const char	*backendPath = "Plugins/CBGPU_D3D11_Win32" HH_PLUGIN_POSTFIX_BUILD HH_PLUGIN_POSTFIX_EXT;
			IPluginModule	*backend = StartupPlugin_CCompilerBackendGPU_D3D11();
			success &= (backend != null && CPluginManager::PluginRegister(backend, true, backendPath));
		}
#endif
		return success;
#else
		TODO
#endif
	}

	void			ShutdownPlugins()
	{
		using namespace PopcornFX;

#ifdef	HH_PLUGINS_STATIC
#if (PK_GPU_D3D11 == 1)
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendGPU_D3D11();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendGPU_D3D11();
		}
#endif
		{
			IPluginModule	*backend = GetPlugin_CCompilerBackendCPU_Emulation();
			(backend != null && CPluginManager::PluginRelease(backend));
			ShutdownPlugin_CCompilerBackendCPU_Emulation();
		}
#else
		TODO
#endif
	}

	PopcornFX::Threads::CAbstractPool			*_CreateThreadPool_UE_Auto()
	{
		const hh_u64			ueImportantAffinity = FPlatformAffinity::GetMainGameMask() | FPlatformAffinity::GetRenderingThreadMask() | FPlatformAffinity::GetRHIThreadMask();
		const hh_u64			ueTasksAffinity = FPlatformAffinity::GetPoolThreadMask() | FPlatformAffinity::GetTaskGraphThreadMask();

		const bool				ueHasAffinities = (ueImportantAffinity != ueTasksAffinity);

		const PopcornFX::CGenericAffinityMask		fullAffinityMask = PopcornFX::CPU::Caps().ProcessAffinity(); // masks all available logical cores
		const hh_u32								coreCount = fullAffinityMask.NumBitsSet();

		PopcornFX::CGenericAffinityMask				allWorkersAffinityMask;
		hh_u32										workerCount;
		if (ueHasAffinities)
		{
			allWorkersAffinityMask.SetAffinityBlock64(0, ueTasksAffinity);
			allWorkersAffinityMask &= fullAffinityMask;

			workerCount = allWorkersAffinityMask.NumBitsSet();
			HH_RELEASE_ASSERT_MESSAGE(workerCount > 1, "Worker Thread affinity might not be right");
		}
		else
		{
			allWorkersAffinityMask = fullAffinityMask;
			// let 2 core unused for main thread, and render thread
			const hh_u32			dontUseCoreCount = 2;
			const hh_u32			minWorkerCount = 1;
			workerCount = PopcornFX::HHMax(dontUseCoreCount + minWorkerCount, coreCount) - dontUseCoreCount;
		}

		PopcornFX::CWorkerThreadPool	*pool = HH_NEW(PopcornFX::CWorkerThreadPool);
		check(pool != null);

		//const PopcornFX::CThreadManager::EPriority	mainThreadPrio = PopcornFX::CThreadManager::Priority_Normal;
		const PopcornFX::CThreadManager::EPriority		workerThreadPrio = PopcornFX::CThreadManager::Priority_High;

		bool			success = true;
		for (hh_u32 i = 0; i < workerCount; ++i)
		{
			success &= HH_VERIFY(pool->AddWorker(workerThreadPrio, &allWorkersAffinityMask) != null);
		}

		// Let ue do this:
		//PopcornFX::CCurrentThread::SetPriority(mainThreadPrio);
		//PopcornFX::CCurrentThread::SetAffinity(mainThreadAffinityMask);

		if (!HH_VERIFY(success))
		{
			UE_LOG(LogPopcornFXStartup, Fatal, TEXT("Failed to Startup PopcornFX Thread Pool"));
			HH_SAFE_DELETE(pool);
			return null;
		}
		pool->StartWorkers();

		UE_LOG(LogPopcornFXStartup, Log, TEXT("PopcornFX Worker Thread Pool created: %d threads, affinity %llx"), workerCount, (unsigned long long)(allWorkersAffinityMask.AffinityBlock64(0)));
		return pool;
	}
	void		_DestroyThreadPool(PopcornFX::Threads::CAbstractPool *pool)
	{
		HH_DELETE(pool);
	}

	void		PopcornFX_AllocFailed(bool *outRetryAlloc, hh_u32 size, hh_u32 alignment)
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("PopcornFX went out of memory for %d bytes !\n"), size);
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("PopcornFX memory stats in bytes: \n"));
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("    footprint:       %d (overhead: %d)\n"), hh_u32(PopcornFX::CMemStats::m_RealFootprint), hh_u32(PopcornFX::CMemStats::m_RealFootprint) - hh_u32(PopcornFX::CMemStats::m_Footprint));
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("    total allocated: %d\n"), hh_u32(PopcornFX::CMemStats::m_RealAllocatedBytes));
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("    total freed:     %d\n"), hh_u32(PopcornFX::CMemStats::m_RealFreedBytes));

		FGenericPlatformMemory::OnOutOfMemory(size, alignment);
	}

	// UE's Malloc will correctly setup system heap size and all (PS4)
	void		*PopcornFX_Alloc(size_t size)
	{
		return FMemory::Malloc(size, DEFAULT_ALIGNMENT); // popcornfx takes care of alignment already
	}

	void		*PopcornFX_Realloc(void *ptr, size_t size)
	{
		return FMemory::Realloc(ptr, size, DEFAULT_ALIGNMENT);
	}

	void		PopcornFX_Free(void *ptr)
	{
		return FMemory::Free(ptr);
	}

} // namespace

bool	PopcornFXStartup()
{
	using namespace PopcornFX;

	SDllVersion	engineVersion;
	bool		success = false;

	if (engineVersion.Major != HH_VERSION_MAJOR || engineVersion.Minor != HH_VERSION_MINOR)
	{
		UE_LOG(LogPopcornFXStartup, Error, TEXT("PopcornFX Runtime version missmatch: PopcornFX Runtime is v%d.%d, but Plugin has been build with v%d.%d headers !"), engineVersion.Major, engineVersion.Minor, HH_VERSION_MAJOR, HH_VERSION_MINOR);
		return false;
	}

	CHHKernel::Config	kernelConfiguration;

	// Setup FileSystem
	g_FsController = new CFileSystemController_UE; // no HH_NEW, not startuped yet
#if	(HH_ASSERTS_IN_DEBUG != 0) || (HH_ASSERTS_IN_RELEASE != 0)
	kernelConfiguration.m_AssertCatcher = &AssertImpl;
#endif
	kernelConfiguration.m_AddDefaultLogListeners = &AddDefaultGlobalListenersOverride;
	kernelConfiguration.m_FsController = g_FsController;

	kernelConfiguration.m_CreateThreadPool = &_CreateThreadPool_UE_Auto;
	kernelConfiguration.m_DestroyThreadPool = &_DestroyThreadPool;

	kernelConfiguration.m_DefaultAllocator_Alloc = &PopcornFX_Alloc;
	kernelConfiguration.m_DefaultAllocator_Realloc = &PopcornFX_Realloc;
	kernelConfiguration.m_DefaultAllocator_Free = &PopcornFX_Free;
	kernelConfiguration.m_DefaultAllocator_OutOfMemory = &PopcornFX_AllocFailed;

	success = true;
	success &= CHHKernel::Startup(engineVersion, kernelConfiguration);
	success &= CHHBaseObject::Startup(engineVersion, CHHBaseObject::Config());
	success &= CHHEngineUtils::Startup(engineVersion, CHHEngineUtils::Config());
	success &= CHHCompiler::Startup(engineVersion, CHHCompiler::Config());
	success &= CHHImaging::Startup(engineVersion, CHHImaging::Config());
	success &= CHHGeometrics::Startup(engineVersion, CHHGeometrics::Config());
	success &= CHHParticles::Startup(engineVersion, CHHParticles::Config());
	success &= CPKBillboarder::Startup(engineVersion, CPKBillboarder::Config());
	success &= Kernel::CheckStaticConfigFlags(Kernel::g_BaseStaticConfig, SKernelConfigFlags());
	//success &= CParticleRenderManager::InstallGlobalRemapper(FastDelegate<void(CParticleRenderManager *)>(&CParticleScene::InternalHookDrawerRemapper));
	if (!success)
	{
		UE_LOG(LogPopcornFXStartup, Error, TEXT("PopcornFX runtime failed to startup"));
		PopcornFXShutdown();
		return false;
	}

	if (!StartupPlugins())
	{
		UE_LOG(LogPopcornFXStartup, Error, TEXT("PopcornFX runtime plugins failed to startup"));
		PopcornFXShutdown();
		return false;
	}

	HH_LOG_MODULE_INIT_START;
	HH_LOG_MODULE_INIT_END;

	CResourceHandlerMesh_UE::Startup();
	CResourceHandlerImage_UE::Startup();

	//CCurrentThread::SetTargetProcessor(0);
	//CCurrentThread::SetPriority(CThreadManager::Priority_High);
	CCoordinateFrame::SetGlobalFrame(ECoordinateFrame::Frame_LeftHand_Z_Up);
	//CBillboarderConfig::SetFrontFaceOrientation(false);

	//g_ModuleConfigMem->SetEnableDebugAllocatorDump(true);

	return success;
}

//----------------------------------------------------------------------------

void	PopcornFXShutdown()
{
	using namespace PopcornFX;

	HH_LOG_MODULE_RELEASE_START;
	HH_LOG_MODULE_RELEASE_END;

	//CParticleRenderManager::UninstallGlobalRemapper(FastDelegate<void(CParticleRenderManager *)>(&CParticleScene::InternalHookDrawerRemapper));

	ShutdownPlugins();

	CPKBillboarder::Shutdown();
	CHHParticles::Shutdown();
	CHHGeometrics::Shutdown();
	CHHImaging::Shutdown();
	CHHCompiler::Shutdown();
	CHHEngineUtils::Shutdown();
	CHHBaseObject::Shutdown();

	CResourceHandlerImage_UE::Shutdown();
	CResourceHandlerMesh_UE::Shutdown();

	delete g_FsController;
	g_FsController = null;


	CHHKernel::Shutdown();
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
