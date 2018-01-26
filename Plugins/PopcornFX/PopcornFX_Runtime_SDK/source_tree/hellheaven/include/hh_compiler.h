#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2006/07/17 22:22 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_COMPILER_H__
#define	__HH_COMPILER_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_compiler.h must not be included outside hellheaven.h
#endif

//----------------------------------------------------------------------------
// these can be directly compared to HH_CPU_ARCH and HH_CPU_MODEL
#define	HH_CPU_X86			1
#define	HH_CPU_PPC			101
#define	HH_CPU_ARM			201

//----------------------------------------------------------------------------
// these can be directly compared to HH_CPU_MODEL

// X86 family
#define	HH_CPU_X64			2
#define	HH_CPU_JAGUAR		3

// PPC family
#define	HH_CPU_CELL_PPU		102
#define	HH_CPU_CELL_SPU		103
#define	HH_CPU_XENON		104
#define	HH_CPU_ESPRESSO		105

// ARM family
#define	HH_CPU_ARM64		202

//----------------------------------------------------------------------------

#if		defined(_MSC_VER)
#	define	HH_COMPILER_MSVC
#elif	defined(__ICL) || defined(__ICC) || defined(__INTEL_COMPILER)
#	define	HH_COMPILER_INTEL
#elif	defined(__clang__)
#	define	HH_COMPILER_CLANG
#	define	HH_COMPILER_GCC	// GCC-Compatible
#elif	defined(__ghs__)	// must appear _before_ __GNUC__ || __GNUG__
#	define	HH_COMPILER_GHS
#elif	defined(__SNC__)
#	define	HH_COMPILER_SNC
#	define	HH_COMPILER_GCC	// GCC-Compatible
#elif	defined(__GNUC__) || defined(__GNUG__)
#	define	HH_COMPILER_GCC
#elif	defined(__MWERKS__) || defined(__CWCC__)
#	define	HH_COMPILER_METROWERKS
//#elif	defined(__BORLANDC__)
//#	define	HH_COMPILER_BORLAND
#else
#	error	unknown compiler
#endif

//----------------------------------------------------------------------------

#if	defined(HH_COMPILER_MAKE_VERSION)
#	error should not be defined.
#endif
#define	HH_COMPILER_MAKE_VERSION(__major, __minor, __patch)	((__major) * 100000 + (__minor) * 100 + (__patch))

//----------------------------------------------------------------------------

#if		defined(HH_COMPILER_MSVC)

#	include "hh_compiler_msvc.h"

#elif defined(HH_COMPILER_INTEL)

#	include "hh_compiler_icc.h"

#elif	defined(HH_COMPILER_GHS)

#	include "hh_compiler_ghs.h"

#elif	defined(HH_COMPILER_SNC)	// must appear _before_ HH_COMPILER_GCC

#	include "hh_compiler_snc.h"

#elif	defined(HH_COMPILER_CLANG)	// must appear _before_ HH_COMPILER_GCC

#	include "hh_compiler_clang.h"

#elif	defined(HH_COMPILER_GCC)

#	include "hh_compiler_gcc.h"

#elif	defined(HH_COMPILER_METROWERKS)

#	error	Metrowerks CodeWarrior not explicitely supported, remove this message if you intend to make it work on codewarrior...

#elif	defined(__BORLANDC__)

#	error	Borland C++ not explicitely supported, remove this message if you intend to make it work on BC++...

#else

#	error	Unknown compiler

#endif

//----------------------------------------------------------------------------

#if	defined(HH_DEBUG) || !defined(HH_HAS_ASSUME)
#	define	HH_ASSUME_NOT_REACHED_E(__expr)		do { HH_ASSERT_NOT_REACHED(); __expr; } while (0)
#else
#	define	HH_ASSUME_NOT_REACHED_E(__expr)		do { HH_ASSUME_NOT_REACHED(); } while (0)
#endif

//----------------------------------------------------------------------------
//	Check if everything went right...

#if	!defined(HH_CPU_ARCH) || !defined(HH_CPU_MODEL)
#	error	unsupported CPU architecture
#endif

#if	!defined(HH_ASSEMBLY_FORMAT_VC) && !defined(HH_ASSEMBLY_FORMAT_AT_T)
#	error	unsupported inline assembly format
#endif

#if	!defined(HH_API_EXPORT) || \
	!defined(HH_API_IMPORT) || \
	!defined(PK_HAS_ALLOCA) || \
	!defined(PK_HAS_TLS) || \
	!defined(PK_HAS_RTTI) || \
	!defined(PK_HAS_EXCEPTIONS) || \
	!defined(PK_CHAR_UNSIGNED) || \
	!defined(PK_HAS_AUTO) || \
	!defined(PK_HAS_DECLTYPE) || \
	!defined(PK_HAS_RVALUE_REF) || \
	!defined(PK_HAS_EXPLICIT_CONVERSIONS) || \
	!defined(PK_HAS_DEF_DEL_FUNCTIONS)
#	error	internal configuration error
#endif

//----------------------------------------------------------------------------
// check if the cpu arch and models are coherent

#if	((HH_CPU_ARCH == HH_CPU_X86) && ( \
		HH_CPU_MODEL != HH_CPU_X86 && \
		HH_CPU_MODEL != HH_CPU_X64 && \
		HH_CPU_MODEL != HH_CPU_JAGUAR)) || \
	((HH_CPU_ARCH == HH_CPU_PPC) && ( \
		HH_CPU_MODEL != HH_CPU_PPC && \
		HH_CPU_MODEL != HH_CPU_CELL_PPU && \
		HH_CPU_MODEL != HH_CPU_CELL_SPU && \
		HH_CPU_MODEL != HH_CPU_XENON && \
		HH_CPU_MODEL != HH_CPU_ESPRESSO)) || \
	((HH_CPU_ARCH == HH_CPU_ARM) && ( \
		HH_CPU_MODEL != HH_CPU_ARM && \
		HH_CPU_MODEL != HH_CPU_ARM64))
#	error	Incoherent CPU arch/model pairs
#endif

//----------------------------------------------------------------------------

// setup the cpu bitcount macro
#if	!defined(HH_CPU_32BITS) && !defined(HH_CPU_64BITS)
#	if		(HH_CPU_MODEL == HH_CPU_X86)
#		define	HH_CPU_32BITS
#	elif	(HH_CPU_MODEL == HH_CPU_X64)
#		define	HH_CPU_64BITS
//#	elif	(HH_CPU_MODEL == HH_CPU_IA64)
//#		define	HH_CPU_64BITS
#	elif	(HH_CPU_MODEL == HH_CPU_PPC)
#		define	HH_CPU_32BITS	// ???
//#	error PPC FIXME: err.. what bitcount should this be set to? O_o
#	elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU || HH_CPU_MODEL == HH_CPU_CELL_SPU)
#		define	HH_CPU_32BITS	// actually, it's 64
#	elif	(HH_CPU_MODEL == HH_CPU_XENON)
#		define	HH_CPU_32BITS
//#	elif	(HH_CPU_MODEL == HH_CPU_CORTEXA9)
//#		define	HH_CPU_64BITS
#	elif	(HH_CPU_MODEL == HH_CPU_ARM64)
#		define	HH_CPU_64BITS
#	elif	(HH_CPU_ARCH == HH_CPU_ARM)
#		define	HH_CPU_32BITS
#	elif	(HH_CPU_MODEL == HH_CPU_ESPRESSO)
#		define	HH_CPU_32BITS
#	elif	(HH_CPU_MODEL == HH_CPU_JAGUAR)
#		define	HH_CPU_64BITS
#	endif
#endif

//----------------------------------------------------------------------------

#if	defined(HH_CPU_32BITS)
#	define	HH_CPU_BITCOUNT	32
#elif	defined(HH_CPU_64BITS)
#	define	HH_CPU_BITCOUNT	64
#else
#	error	unsupported inline CPU bitcount...
#endif

#if	!defined(HH_CPU_BITCOUNT) || (!defined(HH_CPU_32BITS) && !defined(HH_CPU_64BITS))
#	error	internal error : incomplete CPU bitcount config
#endif

//----------------------------------------------------------------------------

#if		(HH_CPU_BITCOUNT == 32)
	typedef	HH_W64 hh_i32	hh_ireg;
	typedef	HH_W64 hh_u32	hh_ureg;
#elif	(HH_CPU_BITCOUNT == 64)
	typedef	hh_i64			hh_ireg;
	typedef	hh_u64			hh_ureg;
#endif

//----------------------------------------------------------------------------

#ifdef	HH_HAS_NULLPTR
#	undef	null
#	if	defined(HH_NULL_AS_VARIABLE)
HH_NULL_AS_VARIABLE;
#	else
#		define	null	HH_HAS_NULLPTR
#	endif
#else
#	undef	null
#	define	null	NULL
#endif

//----------------------------------------------------------------------------

#ifndef PK_WARN_UNUSED_RETURN
#	define PK_WARN_UNUSED_RETURN
#endif

//----------------------------------------------------------------------------

#ifndef PK_PRETTY_FUNCTION
#	if defined(HH_COMPILER_MSVC)
#		define	PK_PRETTY_FUNCTION		__FUNCSIG__
#	elif defined(HH_COMPILER_GCC) || defined(HH_COMPILER_INTEL)	// clang will also fall here
#		define	PK_PRETTY_FUNCTION		__PRETTY_FUNCTION__
#	else
#		define	PK_PRETTY_FUNCTION		__FUNCTION__
#	endif
#endif

//----------------------------------------------------------------------------

#ifndef PK_COMPILER_ATTRIB_PRINTF
#	if defined(HH_COMPILER_GCC) && defined(__GNUC__)
#		define PK_COMPILER_ATTRIB_PRINTF(__fmtIndexPlusOne, __firstArgIndexPlusOne)		__attribute__((format(printf, __fmtIndexPlusOne,  __firstArgIndexPlusOne)))
#	else
#		define PK_COMPILER_ATTRIB_PRINTF(__fmtIndexPlusOne, __firstArgIndexPlusOne)
#	endif
#endif

//----------------------------------------------------------------------------

#ifndef PK_SANE_MEM_ACCESS
#	if defined(__has_feature)
#		if __has_feature(address_sanitizer)
#			define PK_SANE_MEM_ACCESS			1
#		endif
#	endif
#endif

#ifndef PK_SANE_MEM_ACCESS
#	define	PK_SANE_MEM_ACCESS					0
#endif

//----------------------------------------------------------------------------

#if		(HH_CPU_MODEL == HH_CPU_X86)
#	define	HH_CPU_ARCH_NAME		"x86"
#elif	(HH_CPU_MODEL == HH_CPU_X64)
#	define	HH_CPU_ARCH_NAME		"x64"
#elif	(HH_CPU_MODEL == HH_CPU_PPC)
#	define	HH_CPU_ARCH_NAME		"ppc"
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#	define	HH_CPU_ARCH_NAME		"ppu"
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
#	define	HH_CPU_ARCH_NAME		"spu"
#elif	(HH_CPU_MODEL == HH_CPU_XENON)
#	define	HH_CPU_ARCH_NAME		"xenon"
#elif	(HH_CPU_ARCH == HH_CPU_ARM)
#	define	HH_CPU_ARCH_NAME		"arm"
#elif	(HH_CPU_MODEL == HH_CPU_ESPRESSO)
#	define	HH_CPU_ARCH_NAME		"espresso"
#elif	(HH_CPU_MODEL == HH_CPU_JAGUAR)
#	define	HH_CPU_ARCH_NAME		"jaguar"
#else
#	error	Unknown CPU model
#endif

//----------------------------------------------------------------------------
#endif	/* __HH_COMPILER_H__ */
