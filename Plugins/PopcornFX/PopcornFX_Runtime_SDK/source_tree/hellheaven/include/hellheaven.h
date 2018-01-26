#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/01 3:22 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HELLHEAVEN_H__
#define	__HELLHEAVEN_H__

//----------------------------------------------------------------------------
//
//	see hellheaven_config.h for compile-time engine configuration...
//
//----------------------------------------------------------------------------

#ifdef	__cplusplus
#	define	__HH_CDECL_BEGIN	extern "C" {
#	define	__HH_CDECL_END		}
#else
#	error	hellheaven.h may not be directly included in C applications anymore... maybe fix this?
#	define	__HH_CDECL_BEGIN
#	define	__HH_CDECL_END
#endif

#define	__PK_API_BEGIN	namespace PopcornFX {
#define	__PK_API_END	}

namespace PopcornFX {}
namespace HellHeaven = PopcornFX;

__PK_API_BEGIN
//----------------------------------------------------------------------------

// will tell headers they have bee included inside the hellheaven.h core
#define	__HELLHEAVEN_H_INTERNAL__

//----------------------------------------------------------------------------

#define	NOTHING
// #define	COMMA			__COMMA_EXPANDER(__COMMA)
#define	COMMA			,

#ifndef GLUE
#	define	GLUE(a, b)			__GLUE(a, b)
#	define	__GLUE(a, b)		a ## b			// don't directly use this one
#endif // !GLUE

#ifndef STRINGIFY
#	define	STRINGIFY(s)		__STRINGIFY(s)
#	define	__STRINGIFY(s)		# s				// don't directly use this one
#endif // !STRINGIFY

// don't directly use these...
// #define	__COMMA				,
// #define	__COMMA_EXPANDER(c)		__COMMA_EXPANDER2(c)
// #define	__COMMA_EXPANDER2(c)	c

//----------------------------------------------------------------------------

#ifndef	HH_LIBRARY_NAME
#	define	HH_LIBRARY_NAME	"!!! UNKNOWN HELLHEAVEN LIBRARY !!!"
#endif

//----------------------------------------------------------------------------
//	engine standard includes

// main engine compile-time configuration
#include "hh_config.h"

// sets up os detection
#include "hh_os.h"

// compiler-specific
#include "hh_compiler.h"

// os-dependent header
#if		defined(HH_WINDOWS)
#	include "hh_os_win32.h"
#elif	defined(HH_LINUX)
#	include "hh_os_linux.h"
#elif	defined(HH_MACOSX)
#	include "hh_os_macosx.h"
#elif	defined(HH_X360)
#	include "hh_os_x360.h"
#elif	defined(HH_PS3)
#	include "hh_os_cellos-lv2.h"
#elif	defined(HH_PSP2)
#	include "hh_os_psp2.h"
#elif	defined(HH_ANDROID)
#	include "hh_os_android.h"
#elif	defined(HH_IOS)
#	include "hh_os_ios.h"
#elif	defined(HH_ORBIS)
#	include "hh_os_orbis.h"
#elif	defined(HH_NX)
#	include "hh_os_nx.h"
#else
#	error	unsupported OS
#endif

// misc stuff
#include "hh_defs.h"
#include "hh_guid.h"

//----------------------------------------------------------------------------
__PK_API_END

#undef	__HELLHEAVEN_H_INTERNAL__

#endif

