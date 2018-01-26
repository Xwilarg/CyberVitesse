#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2011/09/05 16:56 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_CONFIG_FLAGS_H__
#define	__KR_CONFIG_FLAGS_H__

#include "hh_config.h"
#include "hh_assert.h"
#include "hh_kernel/include/hh_kernel_config.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SKernelConfigFlags
{
	const char	*m_CompilerName;				// HH_COMPILER_NAME

	bool		m_HasDebugAsserts;				// HH_ASSERTS_IN_DEBUG
	bool		m_HasReleaseAsserts;			// HH_ASSERTS_IN_RELEASE
	bool		m_HasAnonymousAsserts;			// HH_ANONYMOUS_ASSERTS

	bool		m_HasContainersMonitoring;		// HH_MONITOR_CONTAINERS
	hh_u32		m_IteratorDebugChecks;			// HH_ITERATORS_DEBUG_CHECK

	hh_u32		m_MemDebugLevel;				// KR_MEM_DEBUG
	bool		m_MemNoDefaultAllocators;		// KR_MEM_NO_DEFAULT_ALLOCATORS

	bool		m_IsDebug;						// HH_DEBUG
	bool		m_IsRetail;						// HH_RETAIL


	HH_FORCEINLINE SKernelConfigFlags()
	:	m_CompilerName(HH_COMPILER_NAME)
#ifdef	HH_ASSERTS_IN_DEBUG
	,	m_HasDebugAsserts(HH_ASSERTS_IN_DEBUG)
#else
#	error corrupt configuration headers. HH_ASSERTS_IN_DEBUG should be defined
#endif
#ifdef	HH_ASSERTS_IN_RELEASE
	,	m_HasReleaseAsserts(HH_ASSERTS_IN_RELEASE)
#else
#	error corrupt configuration headers. HH_ASSERTS_IN_RELEASE should be defined
#endif
#ifdef	HH_ANONYMOUS_ASSERTS
	,	m_HasAnonymousAsserts(HH_ANONYMOUS_ASSERTS)
#else
#	error corrupt configuration headers. HH_ANONYMOUS_ASSERTS should be defined
#endif
#ifdef	HH_MONITOR_CONTAINERS
	,	m_HasContainersMonitoring(HH_MONITOR_CONTAINERS != 0)
#else
	,	m_HasContainersMonitoring(false)
#endif
#ifdef	HH_ITERATORS_DEBUG_CHECK
	,	m_IteratorDebugChecks(HH_ITERATORS_DEBUG_CHECK)
#else
	,	m_IteratorDebugChecks(0)
#endif
#ifdef	KR_MEM_DEBUG
	,	m_MemDebugLevel(KR_MEM_DEBUG)
#else
#	error corrupt configuration headers. KR_MEM_DEBUG should be defined
#endif
#ifdef	KR_MEM_NO_DEFAULT_ALLOCATORS
	,	m_MemNoDefaultAllocators(KR_MEM_NO_DEFAULT_ALLOCATORS)
#else
	,	m_MemNoDefaultAllocators(false)
#endif
#ifdef	HH_DEBUG
	,	m_IsDebug(true)
#else
	,	m_IsDebug(false)
#endif
#ifdef	HH_RETAIL
	,	m_IsRetail(true)
#else
	,	m_IsRetail(false)
#endif
	{
	}
};

//----------------------------------------------------------------------------

namespace	Kernel
{
	bool						HH_KERNEL_EXPORT CheckStaticConfigFlags(const SKernelConfigFlags &a, const SKernelConfigFlags &b);

	extern SKernelConfigFlags	HH_KERNEL_EXPORT g_BaseStaticConfig;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_CONFIG_FLAGS_H__

