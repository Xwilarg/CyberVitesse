//----------------------------------------------------------------------------
//
//	Global PopcornFX-SDK configuration header (RELEASE)
//	include hierarchy:
//
//	-> hellheaven/include/hellheaven.h
//	->-> hellheaven/include/hh_config.h
//
//----------------------------------------------------------------------------

#define		KR_CALLSTACKS_USE_BFD			0

#define		PK_HAS_DETERMINISM												0

//#define		HH_PARTICLES_BUILD_DEBUG_HISTORY							1
//#define		HH_PARTICLES_ALLOCATOR_PAGESIZE_MIN							0x00000010
//#define		HH_PARTICLES_ALLOCATOR_PAGESIZE_MAX							0x01000000

//#define		HH_PARTICLES_UPDATER_USE_D3D11								1
//#define		HH_PARTICLES_BUILD_EVOLVER_SCRIPT							1
//#define		HH_PARTICLES_BUILD_EVOLVER_FIELD							1
//#define		HH_PARTICLES_BUILD_EVOLVER_ROTATION							1
//#define		HH_PARTICLES_BUILD_EVOLVER_DAMPER							1
//#define		HH_PARTICLES_BUILD_EVOLVER_PHYSICS							1
//#define		HH_PARTICLES_BUILD_EVOLVER_COLLISIONS						1
//#define		HH_PARTICLES_BUILD_EVOLVER_SPAWNER							1
//#define		HH_PARTICLES_BUILD_EVOLVER_FLOCKING							1
//#define		HH_PARTICLES_BUILD_EVOLVER_CONTAINMENT						1
//#define		HH_PARTICLES_BUILD_EVOLVER_FLIPBOOK							1
//#define		HH_PARTICLES_BUILD_SAMPLER_CURVE							1
//#define		HH_PARTICLES_BUILD_SAMPLER_SHAPE							1
//#define		HH_PARTICLES_BUILD_SAMPLER_TEXTURE							1
//#define		HH_PARTICLES_BUILD_SAMPLER_TURBULENCE						1
//#define		HH_PARTICLES_DEBUG_CHECK_STREAMS_FOR_INF_OR_NAN				1
//#define		HH_PARTICLES_USE_CORRECT_MAX_CONSTANT_RADIUS_COMPUTATION	1
//#define		HH_GEOMETRICS_BUILD_MESH_DEFORMERS							1
//#define		HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE					1
//#define		HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME						1
//#define		HH_GEOMETRICS_BUILD_MESH_WRITERS							1
//#define		HH_GEOMETRICS_BUILD_MESH_INSPECTOR							1
//#define		HH_GEOMETRICS_BUILD_BILLBOARDS								1
//#define		HH_GEOMETRICS_BUILD_KDTREE_COMPILER							1
//#define		HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER							1
//#define		HH_GEOMETRICS_BUILD_KDTREE									1
//#define		HH_COMPILER_ENABLE_TYPE_CHECKS_FOR_CPP_TO_SCRIPT_CALLS		1
//#define		HH_BASE_OBJECT_STRIP_METADATA								1
//#define		HH_BASE_OBJECT_BUILD_SERIALIZER_TEXT						1
//#define		HH_BASE_OBJECT_BUILD_SERIALIZER_BINARY						1
//#define		HH_BASE_OBJECT_HANDLE_UNKNOWN								1
//#define		HH_IMAGING_BUILD_DOWNSAMPLERS								1
//#define		KR_COMPRESSION_BUILD_FASTLZ									1
//#define		KR_COMPRESSION_BUILD_ZLIB									1
//#define		KR_COMPRESSION_BUILD_BZIP2									1
//#define		KR_CAPS_DETECT_CPU											1
//#define		KR_CAPS_DETECT_MEM											1
//#define		KR_MEM_DEBUG												0
//#define		KR_THREADS_TLS_DISABLED										0
//#define		KR_THREADS_TLS_DYNAMIC_DETECTION							1
//#define		HH_ITERATORS_DEBUG_CHECK									0
//#define		HH_MONITOR_CONTAINERS										0
//#define		KR_REFPTR_UBER_DEBUG										0
//#define		KR_REFPTR_UBER_DEBUG_ALL_REFPOINTERS						0
//#define		HH_KERNEL_BUILD_CALLSTACKS									1
//#define		HH_ASSERTS_IN_DEBUG											0
//#define		HH_ASSERTS_IN_RELEASE										1
//#define		HH_ANONYMOUS_ASSERTS										0
#define			PK_CBEM_ENABLE_DEEP_PROFILES								0
#define			KR_USE_NDK_CPU_CAPS_FILES								1

// X360
#if (((defined(_XBOX_VER) && _XBOX_VER >= 200)) || defined(XBOX) || defined(_XBOX))
//#	define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_SPAWNER			256U
//#	define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_EVOLVENEWBORNS	128U
//#	define	HH_CODE_DUPLICATION_LIMITER									2

// PS3
#elif	defined(__CELLOS_LV2__)
//#	define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_SPAWNER			256U
//#	define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_EVOLVENEWBORNS	128U
//#	define	HH_CODE_DUPLICATION_LIMITER									2

#	define	HH_CRITICAL_SECTION_PS3_USE_LWMUTEX							1

// PC
#else
//#	define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_SPAWNER			1024U
//#	define	HH_PARTICLES_STACK_PROCESSING_GRANULARITY_EVOLVENEWBORNS	256U
//#	define	HH_CODE_DUPLICATION_LIMITER									0


//#	define	HH_ASSERTS_LEVEL_PARANOID	// uncomment to enable hardcore asserts (serious perf hits)
#endif

//#define	HH_NO_SIMD

// Linux:
//   Since UE4 4.14, Game builds links with UE's own stdlib
//   Disable PopcornFX use of stdlib to avoid having to build PopcornFX with UE4's stdlib
// Android: just dont compile with stdlib stuff (like UE)
#if (defined(LINUX) || defined(_LINUX) || defined(__LINUX__) || defined(__linux__)) || \
	(defined(__ANDROID__))
#	define	KR_MEM_DEFAULT_ALLOCATOR_DEBUG		0
#endif
