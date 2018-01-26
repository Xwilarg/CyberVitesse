#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2013/06/25 17:10 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_COMPILER_CLANG_H__
#define	__HH_COMPILER_CLANG_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_compiler_clang.h must not be included outside hellheaven.h
#endif
#ifndef	__HH_COMPILER_H__
#	error	hh_compiler_clang.h must not be included outside hellheaven.h
#endif
#ifndef	HH_COMPILER_CLANG
#	error	wrong compiler
#endif

//----------------------------------------------------------------------------

#define	HH_COMPILER_NAME			"clang"

// too verbose on clang: comment to re-enable compile messages
#define HH_MESSAGE(__msg)

#ifndef HH_MESSAGE
#	if 1
//#	define	HH_MESSAGE(__msg)	_Pragma(STRINGIFY(message(__msg)))	// dammit clang... what's the point of making a pragma MESSAGE a goddamn WARNING ?!
#	else
#		define	HH_MESSAGE(__msg)	\
	/*_Pragma("clang diagnostic push");*/					 \
	/*_Pragma("clang diagnostic ignored \"-fshow-name\"");*/			\
	/*_Pragma("clang diagnostic ignored \"-fshow-source-location\"");*/ \
	/*_Pragma("clang diagnostic ignored \"-fdiagnostics-show-category=note\"");*/ \
	_Pragma(STRINGIFY(message(__msg)));									\
	/*_Pragma("clang diagnostic pop")*/
#	endif
#endif

#ifndef	PK_HAS_ALLOCA
#	define	PK_HAS_ALLOCA		1
#	define	PK_ALLOCA(__size)	alloca(__size)
#endif

//#define	HH_MAY_ALIAS	__attribute__((__may_alias__))
//#if	defined(__psp2__) && defined(HH_DEBUG) && defined(HH_FORCE_INLINING)
//#	undef	HH_FORCE_INLINING
//#endif

#define	CLANG_FULL_VERSION		HH_COMPILER_MAKE_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)

#if	!defined(__has_cpp_attribute)
#	define	__has_cpp_attribute(__name)	0
#endif

#ifndef	PK_UNUSED
#	if __has_cpp_attribute(maybe_unused) && (__cplusplus >= 201406L)	// NOTE: In theory, standard says it should be 201402L, but clang maps to C++14, C++17 gets >= 201406L
#		define PK_UNUSED(__arg)		__arg [[maybe_unused]]
#	elif __has_cpp_attribute(gnu::unused)
#		define PK_UNUSED(__arg)		__arg [[gnu::unused]]
#	else
#		define PK_UNUSED(__arg)		__arg __attribute__((__unused__))
#	endif
#endif

#include "hh_compiler_gcc.h"

// Reset GCC-detected flags, we'll use CLang's own detection stuff here
#undef	PK_HAS_RTTI
#undef	HH_HAS_STATIC_ASSERT
#undef	HH_HAS_NULLPTR
#undef	PK_HAS_AUTO
#undef	PK_HAS_DECLTYPE
#undef	PK_DECLTYPE
#undef	PK_HAS_RVALUE_REF
#undef	PK_HAS_EXPLICIT_CONVERSIONS
#undef	PK_HAS_DEF_DEL_FUNCTIONS

#ifdef	__RTTI			// #if	__option(rtti)
#	define	PK_HAS_RTTI			1
#else
#	define	PK_HAS_RTTI			0
#endif

#if	__has_feature(__cxx_static_assert__)
#	define	HH_HAS_STATIC_ASSERT
#endif

#if	__has_feature(__cxx_nullptr__)
#	define	HH_HAS_NULLPTR			nullptr
#endif

#if	__has_feature(cxx_override_control)
#	if	defined(PK_GCC_DEFINED_OVERRIDE)
#		undef	override			// hh_compiler_gcc.h already redefined this. undef it.
#	endif
#endif

#ifndef	PK_HAS_AUTO
#	if __has_feature(__cxx_auto_type__) && __has_feature(__cxx_decltype__)
#		define	PK_HAS_AUTO		1
#	else
#		define	PK_HAS_AUTO		0
#	endif
#endif

#ifndef	PK_HAS_DECLTYPE
#	if __has_feature(__cxx_decltype__)
#		define	PK_HAS_DECLTYPE		1
#		define	PK_DECLTYPE(__expr)			decltype((__expr))
#	else
#		define	PK_HAS_DECLTYPE		0
#		define	PK_DECLTYPE(__expr)			typeof(__expr)
#	endif
#endif

#ifndef	PK_HAS_RVALUE_REF
#	if __has_feature(__cxx_rvalue_references__)
#		define	PK_HAS_RVALUE_REF	1
#	else
#		define	PK_HAS_RVALUE_REF	0
#	endif
#endif

#ifndef	PK_HAS_EXPLICIT_CONVERSIONS
#	if	__has_feature(__cxx_explicit_conversions__)
#		define	PK_HAS_EXPLICIT_CONVERSIONS	1
#	else
#		define	PK_HAS_EXPLICIT_CONVERSIONS	0
#	endif
#endif

// C++11 Defaulted Deleted functions
#ifndef	PK_HAS_DEF_DEL_FUNCTIONS
#	if __has_feature(cxx_defaulted_functions) && __has_feature(cxx_deleted_functions)
#		define	PK_HAS_DEF_DEL_FUNCTIONS	1
#	else
#		define	PK_HAS_DEF_DEL_FUNCTIONS	0
#	endif
#endif

#pragma clang diagnostic ignored "-Wchar-subscripts"

//----------------------------------------------------------------------------
#endif	// __HH_COMPILER_SNC_H__
