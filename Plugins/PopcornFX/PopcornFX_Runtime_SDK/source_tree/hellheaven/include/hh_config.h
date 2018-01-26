#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2004/10/16 22:01 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HELLHEAVEN_CONFIG_H__
#define	__HELLHEAVEN_CONFIG_H__

//----------------------------------------------------------------------------
// Detection of the build configuration we're in:
//	- PK_DEBUG
//	- PK_RELEASE
//	- PK_RETAIL

#if	defined(PK_RETAIL) || defined(HH_RETAIL)
	// was manually set to retail
#	if	defined(PK_RELEASE) || defined(PK_DEBUG) || \
		defined(HH_RELEASE) || defined(HH_DEBUG)
#		error	Bad PopcornFX configuration: multiple build configurations defined (retail&(release|debug))
#	endif
#	undef	PK_RETAIL
#	undef	HH_RETAIL
#	define	PK_RETAIL
#elif	defined(PK_RELEASE) || defined(HH_RELEASE)
	// was manually set to release
#	if	defined(PK_RETAIL) || defined(PK_DEBUG) || \
		defined(HH_RETAIL) || defined(HH_DEBUG)
#		error	Bad PopcornFX configuration: multiple build configurations defined (release&(retail|debug))
#	endif
#	undef	PK_RELEASE
#	undef	HH_RELEASE
#	define	PK_RELEASE
#elif	defined(PK_DEBUG) || defined(HH_DEBUG)
	// was manually set to debug
#	if	defined(PK_RETAIL) || defined(PK_RELEASE) || \
		defined(HH_RETAIL) || defined(HH_RELEASE)
#		error	Bad PopcornFX configuration: multiple build configurations defined (debug&(retail|release))
#	endif
#	undef	PK_DEBUG
#	undef	HH_DEBUG
#	define	PK_DEBUG
#else
	// autodetect
#	if	defined(_DEBUG) || defined(NN_SDK_BUILD_DEBUG)
#		define	PK_DEBUG
#	else
#		define	PK_RELEASE
#	endif
#endif

//----------------------------------------------------------------------------
// retro-compatibility with oldstyle defines: HH_*

#if	defined(PK_RETAIL)
#	define	HH_RETAIL
#elif	defined(PK_RELEASE)
#	define	HH_RELEASE
#elif	defined(PK_DEBUG)
#	define	HH_DEBUG
#else
#	error Bad PopcornFX configuration: could not detect build configuration
#endif

//----------------------------------------------------------------------------
// external global build-config:

#if		defined(PK_DEBUG)
#	include	"hh_sdk_config_d.h"
#elif	defined(PK_RELEASE)
#	include	"hh_sdk_config_r.h"
#elif	defined(PK_RETAIL)
#	include	"hh_sdk_config_s.h"
#else
#	error
#endif

//----------------------------------------------------------------------------
//	comment to disable forced inlining of all 'HH_FORCEINLINE' functions, and morph them into regular HH_INLINE functions...

#define	HH_FORCE_INLINING

//----------------------------------------------------------------------------
//	uncomment to enable extreme compiler warnings

//#define	HH_WARNINGS_PARANOID

//----------------------------------------------------------------------------
// asserts

#ifndef	HH_ASSERTS_IN_RELEASE
#	if	defined(PK_DEBUG) || defined(PK_RELEASE)
#		define	HH_ASSERTS_IN_RELEASE	1	// release asserts are active in debug and in release, not in retail
#	else
#		define	HH_ASSERTS_IN_RELEASE	0
#	endif
#endif

#ifndef	HH_ASSERTS_IN_DEBUG
#	if	defined(PK_DEBUG)
#		define	HH_ASSERTS_IN_DEBUG		1	// standard asserts are active in debug only
#	else
#		define	HH_ASSERTS_IN_DEBUG		0
#	endif
#endif

#ifndef	HH_ANONYMOUS_ASSERTS
#	define	HH_ANONYMOUS_ASSERTS		0
#endif

//----------------------------------------------------------------------------

#ifndef	HH_CODE_DUPLICATION_LIMITER
#	define	HH_CODE_DUPLICATION_LIMITER		0	// no limitations
#endif

//----------------------------------------------------------------------------

#endif
