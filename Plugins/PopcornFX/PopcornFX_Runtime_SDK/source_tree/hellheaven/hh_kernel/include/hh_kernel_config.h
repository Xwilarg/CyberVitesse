#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2004/11/30 22:59 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_KERNEL_CONFIG_H__
#define	__HH_KERNEL_CONFIG_H__

#include <hh_config.h>

//	this is the main hh_kernel compile time configuration file.

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	KR_MEM CONFIGURATION
//	Also see pv_mem.h for more in-depth control
//
//----------------------------------------------------------------------------

//	enables kr_mem detailed realtime memory dumps, module mem usage/flow stats, etc...

#ifndef	KR_MEM_DEBUG
#	if	defined(HH_DEBUG) && !defined(HH_X360) && !defined(HH_PS3)	//debug
#		define	KR_MEM_DEBUG	1
#	else
#		define	KR_MEM_DEBUG	0
#	endif
#endif

#ifndef	KR_MEM_NO_DEFAULT_ALLOCATORS
#	define	KR_MEM_NO_DEFAULT_ALLOCATORS	0
#endif

#ifndef	PK_MEM_PROFILE_ALLOCATIONS
#	if	defined(HH_DEBUG)
#		define	PK_MEM_PROFILE_ALLOCATIONS	1
#	else
#		define	PK_MEM_PROFILE_ALLOCATIONS	0
#	endif
#endif

// Default Allocator Stat Nodes and Debugs (using stdlib)
#ifndef KR_MEM_DEFAULT_ALLOCATOR_DEBUG
#	define	KR_MEM_DEFAULT_ALLOCATOR_DEBUG		1
#endif

// Patch PK_MEM_DEFAULT_ALLOCATOR_DEBUG when must not be enabled anyway
#if (KR_MEM_DEBUG == 0) || (KR_MEM_NO_DEFAULT_ALLOCATORS != 0) || defined(HH_RETAIL)
#	undef	KR_MEM_DEFAULT_ALLOCATOR_DEBUG
#	define	KR_MEM_DEFAULT_ALLOCATOR_DEBUG		0
#endif

//----------------------------------------------------------------------------
//
// kr_containers
//
//----------------------------------------------------------------------------

#ifndef	HH_ITERATORS_DEBUG_CHECK
#	if	defined(HH_DEBUG)
#		define	HH_ITERATORS_DEBUG_CHECK	1
#	else
#		define	HH_ITERATORS_DEBUG_CHECK	0
#	endif
#endif

#ifndef	HH_MONITOR_CONTAINERS
#	if	defined(HH_DEBUG) && defined(HH_DESKTOP)
#		define	HH_MONITOR_CONTAINERS		1
#	else
#		define	HH_MONITOR_CONTAINERS		0
#	endif
#endif

//----------------------------------------------------------------------------
// kr_refptr config:

#ifndef	KR_REFPTR_UBER_DEBUG
#	define	KR_REFPTR_UBER_DEBUG					0
#endif

#ifndef	KR_REFPTR_UBER_DEBUG_ALL_REFPOINTERS
#	define	KR_REFPTR_UBER_DEBUG_ALL_REFPOINTERS	0
#endif

//----------------------------------------------------------------------------
// kr_threads config

#if	!defined(KR_THREADS_ENABLE_DEBUG_NAMES)
#	if !defined(HH_RETAIL)
#		define	KR_THREADS_ENABLE_DEBUG_NAMES	1
#	else
#		define	KR_THREADS_ENABLE_DEBUG_NAMES	0
#	endif
#endif

#if	defined(HH_PS3)
#	ifndef	HH_CRITICAL_SECTION_PS3_USE_LWMUTEX
#		define	HH_CRITICAL_SECTION_PS3_USE_LWMUTEX		1
#	endif
#endif

#if	defined(HH_PSP2)
#	ifndef	HH_CRITICAL_SECTION_PSP2_USE_LWMUTEX
#		define	HH_CRITICAL_SECTION_PSP2_USE_LWMUTEX	1
#	endif
#endif

// Thread-Local-Storage
#ifndef	KR_THREADS_TLS_DISABLED
#	define	KR_THREADS_TLS_DISABLED				0
#endif

// TLS controls for windows XP compatibility
#if	!defined(KR_THREADS_TLS_DYNAMIC_DETECTION)
#	define	KR_THREADS_TLS_DYNAMIC_DETECTION	1	// 1 = detect OS version and choose the safest TLS implementation
#endif

// Thread pool debugger
#if	!defined(KR_THREADPOOL_PRETTYDEBUG)
#	ifdef HH_RETAIL
#		define KR_THREADPOOL_PRETTYDEBUG		0
#	else
#		define KR_THREADPOOL_PRETTYDEBUG		1
#	endif
#endif

// Thread pool : build Popcorn's internal threadpool
#if	!defined(PK_THREADPOOL_HAS_INTERNAL_IMPLEM)
#	define	PK_THREADPOOL_HAS_INTERNAL_IMPLEM	1
#endif

// Enable some Waits asserts after N milliseconds
#if	!defined(PK_THREADS_WAITS_ASSERTS)
#	ifndef HH_RETAIL
#		define	PK_THREADS_WAITS_ASSERTS		1
#	else
#		define	PK_THREADS_WAITS_ASSERTS		0
#	endif
#endif

// default to 10 seconds
#if	defined(PK_THREADS_WAITS_ASSERTS) && !defined(PK_THREADS_WAITS_ASSERTS_MS)
#	define	PK_THREADS_WAITS_ASSERTS_MS			(5 * 1000)
#endif

#if (PK_THREADS_WAITS_ASSERTS != 0) && (KR_THREADPOOL_PRETTYDEBUG == 0)
#	error config error: PK_THREADS_WAITS_ASSERTS needs KR_THREADPOOL_PRETTYDEBUG
#endif

#if	!defined(PK_THREADS_HAS_NAMED_SYNCPRIMS)
#	if 0 // consoles have only limited named resources, could/will crash insufficient resources
#		define	PK_THREADS_HAS_NAMED_SYNCPRIMS	1
#	else
#		define	PK_THREADS_HAS_NAMED_SYNCPRIMS	0
#	endif
#endif

//----------------------------------------------------------------------------
// reporting

#if	!defined(HH_LOG_REPORTS_ENABLED)
#	if	!defined(HH_RETAIL)
#		define	HH_LOG_REPORTS_ENABLED		1
#	else
#		define	HH_LOG_REPORTS_ENABLED		0
#	endif
#endif

#if	!defined(HH_PERF_HOTSPOTS_ENABLED)
#	if	!defined(HH_RETAIL)
#		define	HH_PERF_HOTSPOTS_ENABLED	1
#	else
#		define	HH_PERF_HOTSPOTS_ENABLED	0
#	endif
#endif

//----------------------------------------------------------------------------
// caps

#if	!defined(KR_CAPS_DETECT_CPU)
#	define	KR_CAPS_DETECT_CPU				1
#endif

#if	!defined(KR_CAPS_DETECT_MEM)
#	define	KR_CAPS_DETECT_MEM				1
#endif

#if	defined(HH_ANDROID)
#	if	!defined(KR_USE_NDK_CPU_CAPS_FILES)
#		define	KR_USE_NDK_CPU_CAPS_FILES		0
#	endif
#endif

//----------------------------------------------------------------------------
// compression

#if	!defined(KR_COMPRESSION_BUILD_FASTLZ)
#	define	KR_COMPRESSION_BUILD_FASTLZ		1
#endif

#if	!defined(KR_COMPRESSION_BUILD_ZLIB)
#	define	KR_COMPRESSION_BUILD_ZLIB		0
#endif

#if	!defined(KR_COMPRESSION_BUILD_BZIP2)
#	define	KR_COMPRESSION_BUILD_BZIP2		0
#endif

//----------------------------------------------------------------------------
// callstack

#ifndef	HH_KERNEL_BUILD_CALLSTACKS
#	if	defined(HH_RETAIL) || \
		defined(HH_PS3) || \
		defined(HH_X360) || \
		defined(HH_PSP2) || \
		defined(HH_ORBIS) || \
		defined(HH_DURANGO) || \
		defined(HH_IOS) || \
		defined(HH_ANDROID)
#		define	HH_KERNEL_BUILD_CALLSTACKS	0	// by default, disable callstacks for consoles and retail builds
#	else
#		define	HH_KERNEL_BUILD_CALLSTACKS	1	// enable for everything else
#	endif
#endif

// not properly implemented or buggee
// DURANGO: missing kernel32 functions
// IOS: clang segfaults
#if		defined(HH_DURANGO) || defined(HH_IOS)
#	undef HH_KERNEL_BUILD_CALLSTACKS
#	define HH_KERNEL_BUILD_CALLSTACKS		0
#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif
