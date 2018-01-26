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

#ifndef	__HH_COMPILER_GCC_H__
#define	__HH_COMPILER_GCC_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_compiler_gcc.h must not be included outside hellheaven.h
#endif
#ifndef	__HH_COMPILER_H__
#	error	hh_compiler_gcc.h must not be included outside hellheaven.h
#endif
#ifndef	HH_COMPILER_GCC
#	error	wrong compiler
#endif

//----------------------------------------------------------------------------
//
//	GNU C COMPILER - GCC
//
//----------------------------------------------------------------------------

#define	GCC_FULL_VERSION			HH_COMPILER_MAKE_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)

#ifndef	HH_COMPILER_VERSION
#	define	HH_COMPILER_VERSION		(__GNUC__ * 1000 + __GNUC_MINOR__)
#endif
#ifndef	HH_COMPILER_VERSION_STRING
#	define	HH_COMPILER_VERSION_STRING	STRINGIFY(__GNUC__) "." STRINGIFY(__GNUC_MINOR__) "." STRINGIFY(__GNUC_PATCHLEVEL__)
#endif

#ifndef	HH_COMPILER_NAME		// might be set by hh_compiler_snc.h
#	define	HH_COMPILER_NAME		"GCC"
#endif

#ifdef __CHAR_UNSIGNED__
#	define	PK_CHAR_UNSIGNED	1
#else
#	define	PK_CHAR_UNSIGNED	0
#endif

#ifdef	__GXX_RTTI		// has RTTI enabled in the projects settings
#	define	PK_HAS_RTTI			1
#else
#	define	PK_HAS_RTTI			0
#endif

#ifdef	__EXCEPTIONS	// has exception handling enabled in the projects settings
#	define	PK_HAS_EXCEPTIONS	1
#else
#	define	PK_HAS_EXCEPTIONS	0
#endif

#if	defined(__ORBIS__)
#	define	HH_CPU_ARCH			HH_CPU_X86
#	define	HH_CPU_MODEL		HH_CPU_JAGUAR
#elif	defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
#	define	HH_CPU_ARCH			HH_CPU_X86
#	define	HH_CPU_MODEL		HH_CPU_X86
#elif	defined(__amd64__) || defined(__x86_64__)
#	define	HH_CPU_ARCH			HH_CPU_X86
#	define	HH_CPU_MODEL		HH_CPU_X64
#elif	defined(__ia64__)
//#	define	HH_CPU_MODEL		HH_CPU_IA64
#	error	unsupported architecture
#elif	defined(__PPU__)
#	define	HH_CPU_ARCH			HH_CPU_PPC
#	define	HH_CPU_MODEL		HH_CPU_CELL_PPU
#elif	defined(__SPU__)
#	define	HH_CPU_ARCH			HH_CPU_PPC
#	define	HH_CPU_MODEL		HH_CPU_CELL_SPU
#elif	defined(__ppc__)	// ???
#	define	HH_CPU_ARCH			HH_CPU_PPC
#	define	HH_CPU_MODEL		HH_CPU_PPC
#elif	defined(__arm__) || defined(__aarch64__) || defined(__arm64) || defined(__ARM_ARCH)
#	define	HH_CPU_ARCH			HH_CPU_ARM
#	if defined(__arm64) || defined(__aarch64__) || (defined(__ARM_64BIT_STATE) && (__ARM_64BIT_STATE != 0))
#		define	HH_CPU_MODEL	HH_CPU_ARM64
#	else
#		define	HH_CPU_MODEL	HH_CPU_ARM
#	endif
#else
#	error	unknown CPU architecture
#endif

// AT&T inline asm syntax
#define	HH_ASSEMBLY_FORMAT_AT_T	HH_CPU_MODEL

#define	HH_API_EXPORT	__attribute__((__visibility__("default")))
#define	HH_API_IMPORT	__attribute__((__visibility__("default")))

#ifndef	PK_HAS_ALLOCA
#	define	PK_HAS_ALLOCA		1
/*#	define	PK_ALLOCA(s)	__builtin_alloca(((s) + 7) & ~7) */
#	define	PK_ALLOCA(s)		__builtin_alloca(s)
#endif

#if	defined(HH_MACOSX)
#	ifdef	PK_HAS_TLS	// force it off
#		undef	PK_HAS_TLS
#	endif
#	define	PK_HAS_TLS			0	// TLS not supported on GCC/MacOsX
#else
#	ifndef	PK_HAS_TLS			// we can override this and force it off
#		define	PK_HAS_TLS		1
#	endif
#endif

#if defined( _UNICODE ) || defined( UNICODE )
#	define	HH_UNICODE
#endif

#if	(HH_CPU_MODEL == HH_CPU_X86)

#	define	HH_MAIN_STARTUP_ALIGNER \
	do { \
		/* allocate 16 extra bytes on the stack. as this builtin is being used, */ \
		/* GCC won't omit the frame pointer. */ \
		(void)PK_ALLOCA(16); \
		__asm__ __volatile__ ("andl $-32, %esp"); /* align the stack to 16 bytes (& 0xffffffe0) */ \
	} while (0)

#else
#	define	HH_MAIN_STARTUP_ALIGNER
#endif

#define	HH_INLINE		inline

//	HH_FORCE_INLINE is defined in hellheaven_config.h
#if defined(HH_FORCE_INLINING)
#	define HH_FORCEINLINE	inline __attribute__((always_inline))
#else
#	define HH_FORCEINLINE	inline
#endif

#define HH_NOINLINE		__attribute__((noinline))

#if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4, 5, 0))	// >= GCC 4.5.0

#	define	HH_HAS_ASSUME
//__builtin_expect(__expr, true)	// gcc_unreachable() // __builtin_unreachable()
#	define	HH_ASSUME(__expr)				do { if (!(__expr)) __builtin_unreachable(); } while (0)
#	ifdef	HH_DEBUG
#		define	HH_ASSUME_NOT_REACHED()		HH_ASSERT_NOT_REACHED()
#	else
#		define	HH_ASSUME_NOT_REACHED()		__builtin_unreachable()
#	endif
#else
#	define	HH_ASSUME(__expr)
#	define	HH_ASSUME_NOT_REACHED()
#endif

#ifndef	HH_MAY_ALIAS
#	define	HH_MAY_ALIAS	__attribute__((__may_alias__))
#endif

/* #define	HH_NAKED		__declspec(naked) */
//#define	HH_NOALIAS
//#define	HH_RESTRICT		__restrict
#define	HH_FASTCALL

#if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(3, 0, 0))	// >= GCC 3.0.0
#	define	HH_PREDICT_LIKELY(__cond)		__builtin_expect(!!(__cond), true)
#	define	HH_PREDICT_UNLIKELY(__cond)		__builtin_expect(!!(__cond), false)
#else
#	define	HH_PREDICT_LIKELY(__cond)		(__cond)
#	define	HH_PREDICT_UNLIKELY(__cond)		(__cond)
#endif

#if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(3, 0, 1))	&& !defined(__llvm__) // >= GCC 3.0.1
#	define	HH_HAS_BUILTIN_CONSTANT
#endif

#define	HH_PURE				__attribute__((pure))
#define	HH_NOVTABLE

#define	__HH_FUNCSIG__		__PRETTY_FUNCTION__

#define	HH_ALIGN(a)			__attribute__((aligned(a)))
#define	HH_ALIGNOF(a)		__alignof__(a)
#ifndef	HH_DEPRECATED
#	define HH_DEPRECATED(__msg)	__attribute__((deprecated))
#endif

#define PK_WARN_UNUSED_RETURN	__attribute__((warn_unused_result))

#ifndef	PK_UNUSED
#	if (GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(7, 0, 0))
#		define PK_UNUSED(__arg)		__arg [[maybe_unused]]
#	else
#		define	PK_UNUSED(__arg)	__arg __attribute__((__unused__))
#	endif
#endif

// FIXME: this is the logical way but may be wrong...

#ifndef	HH_BREAKPOINT
#	if		(HH_CPU_ARCH == HH_CPU_X86)
#		if	(HH_CPU_MODEL == HH_CPU_JAGUAR)
#			define HH_BREAKPOINT()		__asm__ __volatile__ ("int $0x41")
#		else
#			define HH_BREAKPOINT()		__asm__ __volatile__ ("int3")
#		endif
#	elif	(HH_CPU_ARCH == HH_CPU_PPC)
#		if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
#			define HH_BREAKPOINT()		__asm__ __volatile__ ("stopd 0,1,1\n")
#		else
#			define HH_BREAKPOINT()		__asm__ __volatile__ ("tw 31,1,1")
#		endif
#	elif	(HH_CPU_ARCH == HH_CPU_ARM)
#		if (HH_CPU_MODEL == HH_CPU_ARM64)
#			define HH_BREAKPOINT()			__asm__ __volatile__ ("brk 0x2A")
#		else
#			define HH_BREAKPOINT()			__asm__ __volatile__ ("bkpt 0x2A")
#		endif
#	else
#		error	FIXME: this is probably for PPC ? if so, move the 'else' of PPC into an 'elif (HH_CPU_MODEL == HH_CPU_CELL_PPU)' and move this-one in the real 'else'
		__PK_API_END
#			include <assert.h>
		__PK_API_BEGIN
#		define	HH_BREAKPOINT()			assert(false)	// FIXME: hum
#	endif
#endif

// basic types
#if defined(__CELLOS_LV2__)

	__PK_API_END
#	include <sys/integertypes.h>
	__PK_API_BEGIN

	// uint64_t is "unsigned long" on 64bit, which makes printf format not accept %ll
//	typedef	uint64_t			hh_u64;
	typedef	unsigned long long	hh_u64;
	typedef	uint32_t			hh_u32;
	typedef	uint16_t			hh_u16;
	typedef	uint8_t				hh_u8;
//	typedef	int64_t				hh_i64;
	typedef	long long			hh_i64;
	typedef	int32_t				hh_i32;
	typedef	int16_t				hh_i16;
	typedef	int8_t				hh_i8;

#	define __cdecl
#	define __stdcall
#	define __fastcall
#	define __thiscall

#else

	__PK_API_END
#	include <stdint.h>
	__PK_API_BEGIN

	// uint64_t is "unsigned long" on 64bit, which makes printf format not accept %ll
//	typedef	uint64_t			hh_u64;
	typedef	unsigned long long	hh_u64;
	typedef	uint32_t			hh_u32;
	typedef	uint16_t			hh_u16;
	typedef	uint8_t				hh_u8;
//	typedef	int64_t				hh_i64;
	typedef	long long			hh_i64;
	typedef	int32_t				hh_i32;
	typedef	int16_t				hh_i16;
	typedef	int8_t				hh_i8;

	/*
	typedef	unsigned long long	hh_u64;
	typedef	unsigned long		hh_u32;
	typedef	unsigned short		hh_u16;
	typedef	unsigned char		hh_u8;
	typedef	signed long long	hh_i64;
	typedef	signed long			hh_i32;
	typedef	signed short		hh_i16;
	typedef	signed char			hh_i8;
	*/

#	if	(HH_CPU_ARCH == HH_CPU_ARM)	|| (HH_CPU_MODEL == HH_CPU_X64) // on ARM and x64 __cdecl is useless and compiler warns Wattribute
#		define __cdecl
#	else
#		define __cdecl		__attribute__((cdecl))
#	endif
#	define __stdcall		__attribute__((stdcall))
#	define __fastcall		__attribute__((regparm(2)))	// __attribute__((regparm(3)))
#	define __thiscall

#	if	defined(HH_WINDOWS) && (HH_CPU_MODEL == HH_CPU_X86)
#		define	HH_HAS_STDCALL
#	endif
#endif

#define	HH_W64

#if !defined(__GXX_EXPERIMENTAL_CXX0X__) || (GCC_FULL_VERSION < HH_COMPILER_MAKE_VERSION(4,7,0))
#	if	!defined(override)
#		define	PK_GCC_DEFINED_OVERRIDE
#		define	override
#	endif
#endif

#if (GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4,3,0)) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#	define	HH_HAS_NULLPTR				nullptr
#	define	HH_HAS_STATIC_ASSERT
#	if defined(__cplusplus) && __cplusplus >= 201103L
#		define	HH_NULL_AS_VARIABLE		static const decltype(HH_HAS_NULLPTR) null = HH_HAS_NULLPTR
#	else
#	endif
#endif

#ifndef	PK_HAS_AUTO
#	define	PK_HAS_AUTO					0
#endif

#ifndef	PK_HAS_DECLTYPE
#	define	PK_HAS_DECLTYPE				0
#	define	PK_DECLTYPE(__expr)			typeof(__expr)
#endif

#ifndef	PK_HAS_RVALUE_REF
#	if (__GXX_EXPERIMENTAL_CXX0X__)
#		define	PK_HAS_RVALUE_REF		1
#	else
#		define	PK_HAS_RVALUE_REF		0
#	endif
#endif

#ifndef	PK_HAS_EXPLICIT_CONVERSIONS
#	if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4,5,0)) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#		define	PK_HAS_EXPLICIT_CONVERSIONS	1
#	else
#		define	PK_HAS_EXPLICIT_CONVERSIONS	0
#	endif
#endif

// C++11 Defaulted Deleted functions
#ifndef	PK_HAS_DEF_DEL_FUNCTIONS
#	if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4,4,0))
#		define	PK_HAS_DEF_DEL_FUNCTIONS	1
#	else
#		define	PK_HAS_DEF_DEL_FUNCTIONS	0
#	endif
#endif

//----------------------------------------------------------------------------

#define	HH_HAS_VARIADIC_MACROS

#define	PK_SPLIT_BASIC_BLOCK(__comment)		__asm__("#\n\t# " __comment "\n\t#\n")

#if	!defined(__noop)
#	define	__noop(...)		do { } while (0)
#endif

#if	(GCC_FULL_VERSION < HH_COMPILER_MAKE_VERSION(4,3,0))
#	if	!defined(__COUNTER__)
#		define	__COUNTER__		__LINE__
#	endif
#endif

//----------------------------------------------------------------------------

// too verbose on gcc: comment to re-enable compile messages
#define HH_MESSAGE(__msg)

#if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4,7,0))
#	ifndef	HH_MESSAGE
#		define _HH_GCC_DO_PRAGMA(x)	_Pragma (#x)
#		define HH_MESSAGE(__msg)	_HH_GCC_DO_PRAGMA( message __msg )
#	endif
#endif

#ifndef	HH_MESSAGE
#	define HH_MESSAGE(__msg)
#endif

#ifdef	HH_SHORT_COMPILER_MESSAGES
#	define	HH_TODO(__msg)			HH_MESSAGE("[L" STRINGIFY(__LINE__) "]: TODO: " __msg)
#	define	HH_FIXME(__msg)			HH_MESSAGE("[L" STRINGIFY(__LINE__) "]: FIXME: " __msg)
#	define	HH_NOTE(__msg)			HH_MESSAGE("[L" STRINGIFY(__LINE__) "]: NOTE: " __msg)
#else
#	define	HH_TODO(__msg)			HH_MESSAGE(__FILE__ "(" STRINGIFY(__LINE__) ") /!\\/!\\/!\\/!\\ TODO /!\\/!\\/!\\/!\\ " __msg)
#	define	HH_FIXME(__msg)			HH_MESSAGE(__FILE__ "(" STRINGIFY(__LINE__) ") /!\\/!\\/!\\/!\\ FIXME /!\\/!\\/!\\/!\\ " __msg)
#	define	HH_NOTE(__msg)			HH_MESSAGE(__FILE__ "(" STRINGIFY(__LINE__) ") /!\\/!\\/!\\/!\\ NOTE /!\\/!\\/!\\/!\\ " __msg)
#endif

#if	(GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4,2,1))
//#	pragma GCC diagnostic ignored "-Wno-unused-value"	// done in the project command-line
#endif

//----------------------------------------------------------------------------

#if	(!defined(__APPLE__) && defined(HH_COMPILER_CLANG) && CLANG_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(3,9,0)) || \
	( defined(__APPLE__) && defined(HH_COMPILER_CLANG) && CLANG_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(8,1,0)) // macosx's clang version 8.1.0 corresponds to linux clang 3.9.0 ...
#	pragma GCC diagnostic ignored "-Wundefined-var-template"
#endif

//----------------------------------------------------------------------------
#endif	// __HH_COMPILER_GCC_H__
