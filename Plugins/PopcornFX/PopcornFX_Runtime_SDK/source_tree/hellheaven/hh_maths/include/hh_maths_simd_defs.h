#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2009/03/24 16:58 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_SIMD_DEFS_H__
#define __PK_MATHS_SIMD_DEFS_H__

#include "hh_maths/include/hh_maths.h"

#if		defined(HH_WINDOWS)
#	include <intrin.h>
#elif	defined(HH_MACOSX)
#	if defined(__ppc__)
#		define PPC_INTRINSICS
#	endif
#elif	defined(HH_PS3)
#	if	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#		include <ppu_intrinsics.h>
#	else	// (HH_CPU_MODEL == HH_CPU_CELL_SPU)
#		include <spu_intrinsics.h>
#	endif
#elif	defined(__ARM_NEON)
#	include <arm_neon.h>
#elif	defined(__ALTIVEC__)
#else
//need something here ?
#endif

//----------------------------------------------------------------------------

#define HH_SIMD_NONE		1
#define HH_SIMD_SSE			2
#define HH_SIMD_XBOX		3
#define HH_SIMD_PPC			4
#define HH_SIMD_CELL_PPU	5
#define HH_SIMD_CELL_SPU	6
#define HH_SIMD_NEON		7

#ifdef HH_SIMD
#	error	HH_SIMD should not be defined manually...
#endif

//----------------------------------------------------------------------------
//
//	native vector structures/classes common declaration helpers.
//
//	note:	as we may include multiple SIMD versions inside the same build (at least SIMD_None and any other)
//			we need to define these all the time.
//
//	the naming pattern is:
//		SIMD::Scalar:	HH_SIMD_NATIVE_SCAREG_DECL__ ## HH_SIMD
//		SIMD::Vector:	HH_SIMD_NATIVE_VECREG_DECL__ ## HH_SIMD
//		SIMD::Matrix:	HH_SIMD_NATIVE_MATREG_DECL__ ## HH_SIMD
//
//----------------------------------------------------------------------------

#if 0
// PPC
HH_STATIC_ASSERT(HH_SIMD_PPC == 4);
#define	HH_SIMD_NATIVE_VECREG_DECL__4		HH_ALIGN(0x10)
#define	HH_SIMD_NATIVE_SCAREG_DECL__4
#define	HH_SIMD_NATIVE_MATREG_DECL__4		HH_ALIGN(0x10)
#endif

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

#if	defined(HH_ASSEMBLY_FORMAT_VC) && (HH_CPU_MODEL == HH_CPU_X86)	// inline assembly doesn't work on x64 in visual studio...
#	define	INLINE_INTEL_ASSEMBLY_AVAILABLE		1
#else
#	define	INLINE_INTEL_ASSEMBLY_AVAILABLE		0
#endif

//----------------------------------------------------------------------------

#if		defined(HH_NO_SIMD)// || defined(__GNUC__) || defined(__GNUG__) || defined(__ICC)
#	define HH_SIMD		HH_SIMD_NONE
#	include	"hh_maths/include/simd/none/simd_defs.h"
#elif	(HH_CPU_ARCH == HH_CPU_X86)	// X86
#	define HH_SIMD		HH_SIMD_SSE
#	if	(HH_CPU_MODEL == HH_CPU_JAGUAR)	// We're compiling for PS4 / XBoxOne
#		if	!defined(HH_SIMD_SSE_COMPILABLE_VERSION)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX
#		endif
#	endif
#	include	"hh_maths/include/simd/sse/simd_defs.h"
#elif	(HH_CPU_ARCH == HH_CPU_PPC)	// PPC
#	if	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#		define HH_SIMD		HH_SIMD_CELL_PPU
#		include	"hh_maths/include/simd/cell/ppu/simd_defs.h"
#	elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
#		define HH_SIMD		HH_SIMD_CELL_SPU
#		include	"hh_maths/include/simd/cell/spu/simd_defs.h"
#	elif	(HH_CPU_MODEL == HH_CPU_XENON)
#		pragma	warning(disable:4627)		// buggy warning on vc2008/X360 when using __declspec(passinreg) : "warning C4627: '0#': skipped when looking for precompiled header use"
#		define	HH_SIMD		HH_SIMD_XBOX
#		include	"hh_maths/include/simd/xbox/simd_defs.h"
#	else	// Generic PPC
#		define HH_SIMD		HH_SIMD_PPC
#		include	"hh_maths/include/simd/ppc/simd_defs.h"
#	endif
#elif	(HH_CPU_ARCH == HH_CPU_ARM)
#	if defined(__ARM_NEON) && (__ARM_NEON != 0)
#		define HH_SIMD		HH_SIMD_NEON
#		include	"hh_maths/include/simd/neon/simd_defs.h"
#	else	// Generic non-NEON ARM
#		define HH_SIMD		HH_SIMD_NONE
#		include	"hh_maths/include/simd/none/simd_defs.h"
#	endif
#else
#	error unsupported CPU architecture
#endif

//----------------------------------------------------------------------------

#if		(HH_SIMD == HH_SIMD_NONE)
#	define	__HH_SIMD_NAMESPACE		SIMD_None
#elif	(HH_SIMD == HH_SIMD_XBOX)
#	define	__HH_SIMD_NAMESPACE		SIMD_Xbox
#elif	(HH_SIMD == HH_SIMD_PPC)
#	define	__HH_SIMD_NAMESPACE		SIMD_PPC
#elif	(HH_SIMD == HH_SIMD_SSE)
#	define	__HH_SIMD_NAMESPACE		SIMD_SSE
#elif	(HH_SIMD == HH_SIMD_CELL_PPU) || (HH_SIMD == HH_SIMD_CELL_SPU)
#	define	__HH_SIMD_NAMESPACE		SIMD_Cell
#elif	(HH_SIMD == HH_SIMD_NEON)
#	define	__HH_SIMD_NAMESPACE		SIMD_Neon
#else
#	error	Unknown SIMD type
#endif


__PK_API_BEGIN
//----------------------------------------------------------------------------

// forward declare namesapce
namespace __HH_SIMD_NAMESPACE
{
} // namespace __HH_SIMD_NAMESPACE

//----------------------------------------------------------------------------

// define the default SIMD namespace:

namespace SIMD
{
	using namespace	__HH_SIMD_NAMESPACE;
} // namespace SIMD

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_SIMD_DEFS_H__
