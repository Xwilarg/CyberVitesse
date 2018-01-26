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

#ifndef	__HH_COMPILER_MSVC_H__
#define	__HH_COMPILER_MSVC_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_compiler_msvc.h must not be included outside hellheaven.h
#endif
#ifndef	__HH_COMPILER_H__
#	error	hh_compiler_msvc.h must not be included outside hellheaven.h
#endif
#ifndef	HH_COMPILER_MSVC
#	error	wrong compiler
#endif

//----------------------------------------------------------------------------
//
//	MICROSOFT VISUAL STUDIO - VC++
//
//----------------------------------------------------------------------------

#define	HH_COMPILER_VERSION			_MSC_VER
#define	HH_COMPILER_VERSION_STRING	STRINGIFY(_MSC_VER)
#define	HH_COMPILER_NAME			"Microsoft Visual C++"

#ifdef _CHAR_UNSIGNED
#	define	PK_CHAR_UNSIGNED	1
#else
#	define	PK_CHAR_UNSIGNED	0
#endif

// disable all the deprecated warnings we don't want...
// FIXME: perhaps we shouldn't do this, and work around it another way, some SDK users might want them active in their builds
#ifndef	_CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef STRSAFE_NO_DEPRECATE
#	define STRSAFE_NO_DEPRECATE
#endif

#ifdef	_CPPRTTI		// has RTTI enabled in the projects settings
#	define	PK_HAS_RTTI			1
#else
#	define	PK_HAS_RTTI			0
#endif

#ifdef	_CPPUNWIND		// has exception handling enabled in the projects settings
#	define	PK_HAS_EXCEPTIONS	1
#else
#	define	PK_HAS_EXCEPTIONS	0
#endif

#if	defined(_M_AMD64) || defined(_M_X64) || defined(_WIN64)
#	define	HH_CPU_ARCH			HH_CPU_X86
#	if	defined(HH_DURANGO)
#		define	HH_CPU_MODEL	HH_CPU_JAGUAR	// Xbox One
#	else
#		define	HH_CPU_MODEL	HH_CPU_X64
#	endif
#elif	defined(_M_IX86) || defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
#	define	HH_CPU_ARCH			HH_CPU_X86
#	define	HH_CPU_MODEL		HH_CPU_X86
#elif	defined(_M_IA64) || defined(_IA64_)
//#	define	HH_CPU_MODEL		HH_CPU_IA64
#	error	unsupported architecture
#elif	(_XBOX_VER >= 200)
#	define	HH_CPU_ARCH			HH_CPU_PPC
#	define	HH_CPU_MODEL		HH_CPU_XENON	// Xbox 360
#elif	defined(_M_PPC) || defined(POWERPC)
#	define	HH_CPU_ARCH			HH_CPU_PPC
#	define	HH_CPU_MODEL		HH_CPU_PPC
#elif	defined(_M_ARM)
#	define	HH_CPU_ARCH			HH_CPU_ARM
#	define	HH_CPU_MODEL		HH_CPU_ARM
#else
#	error	unknown CPU architecture !
#endif

// standard asm syntax
#define	HH_ASSEMBLY_FORMAT_VC	HH_CPU_MODEL

#define	HH_API_EXPORT	__declspec(dllexport)
#define	HH_API_IMPORT	__declspec(dllimport)

#if	defined(HH_WINDOWS) && (HH_CPU_MODEL == HH_CPU_X86)
#	define	HH_HAS_STDCALL
#endif

#if	defined(HH_X360) || (HH_CPU_ARCH == HH_CPU_ARM)
#	define	PK_HAS_ALLOCA		0
#else
#	define	PK_HAS_ALLOCA		1
#	define PK_ALLOCA(__size)	alloca(__size)
#endif

#ifndef	PK_HAS_TLS	// we can override this and force it off
#	define	PK_HAS_TLS			1
#endif

#if defined( _UNICODE ) || defined( UNICODE )
#	define	HH_UNICODE
#endif

// MSVC++ memory leaks tracker

#if	defined(HH_DEBUG) && !defined(_XBOX_ONE) && !defined(_DURANGO)
	__PK_API_END	// temporarily leave the PopcornFX namespace
#	include "crtdbg.h"
	__PK_API_BEGIN
#	define	HH_MEMORY_DEBUG	do { \
								_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF); \
								_CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE); \
								_CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR); \
							} while (0)
#else
#	define	HH_MEMORY_DEBUG
#endif

//
//	some informations on the inlining syntax, from MSDN:
//
//	The insertion (called inline expansion or inlining) occurs only if the compiler's
//	cost/benefit analysis show it to be profitable.
//	Inline expansion alleviates the function-call overhead at the potential cost of larger code size.
//	The __forceinline keyword overrides the cost/benefit analysis and relies on
//	the judgment of the programmer instead. Exercise caution when using __forceinline.
//	Indiscriminate use of __forceinline can result in larger code with only marginal
//	performance gains or, in some cases, even performance losses
//	(due to increased paging of a larger executable, for example).
//
//	The compiler treats the inline expansion options and keywords as suggestions.
//	There is no guarantee that functions will be inlined. You cannot force the compiler to inline
//	a particular function, even with the __forceinline keyword.
//

#define	HH_INLINE					__inline

//	HH_FORCE_INLINE is defined in hellheaven_config.h
#if	(_MSC_VER >= 1300)
#	if defined(HH_FORCE_INLINING)
#		undef	HH_FORCEINLINE
#		define	HH_FORCEINLINE		__forceinline
#	endif
#	define	HH_NOINLINE				__declspec(noinline)
#else
#	define	HH_FORCEINLINE			__inline
#	define	HH_NOINLINE
#endif


#define	HH_HAS_ASSUME
#define	HH_ASSUME(__expr)				__assume(__expr)
#ifdef	HH_DEBUG
#	define	HH_ASSUME_NOT_REACHED()		HH_ASSERT_NOT_REACHED()
#else
#	define	HH_ASSUME_NOT_REACHED()		__assume(false)
#endif

#define	HH_MAY_ALIAS
#define	HH_NOALIAS					__declspec(noalias)
//#define	HH_RESTRICT					__declspec(restrict)
//#define	HH_RESTRICT					__restrict
#define	HH_NOINLINE					__declspec(noinline)
#define	HH_FASTCALL					__fastcall
#define	HH_PREDICT_LIKELY(__cond)	(__cond)
#define	HH_PREDICT_UNLIKELY(__cond)	(__cond)
#define	HH_PURE
#define	HH_NOVTABLE					__declspec(novtable)

#define	__HH_FUNCSIG__				__FUNCSIG__

#define	HH_ALIGN(__a)			__declspec(align(__a))
#define	HH_ALIGNOF(__t)			__alignof(__t)

#ifndef	HH_DEPRECATED
#	if _MSC_VER >= 1300
#		define HH_DEPRECATED(__msg)	__declspec(deprecated(__msg))
#	else
#		define HH_DEPRECATED(...)
#	endif
#endif

#define	HH_MAIN_STARTUP_ALIGNER

#if	(_MSC_VER >= 1700)	// vs2012
#	define PK_WARN_UNUSED_RETURN	_Check_return_	// Only used when building with the /analyze compiler flag.
#endif

//#define	PK_UNUSED(__arg)			__pragma(warning(suppress: 4100)) __arg
#define	PK_UNUSED(__arg)			__arg

#define	HH_BREAKPOINT()				__debugbreak()	// __asm int 3

#if	(_MSC_VER < 1400)	// versions below VC2005 do not support the override specifier
#	define	override
#endif
// note: >= 1400 handles 'sealed' and 'abstract' too

//----------------------------------------------------------------------------

// basic types
typedef	unsigned __int64	hh_u64;
typedef	unsigned __int32	hh_u32;
typedef	unsigned __int16	hh_u16;
typedef	unsigned __int8		hh_u8;
typedef	signed __int64		hh_i64;
typedef	signed __int32		hh_i32;
typedef	signed __int16		hh_i16;
typedef	signed __int8		hh_i8;

#define	HH_W64	__w64

//----------------------------------------------------------------------------
// unwanted level 4 warnings to disable.
// warning C4201 : nonstandard extension used: nameless struct/union
// warning C4239 : nonstandard extension used: 'extern' before template explicit instantiation
// warning C4480 : nonstandard extension used: specifying underlying type for enum ''
// warning C4481 : nonstandard extension used: override specifier 'override'
// warning C4244 : conversion from 'int' to 'short', possible loss of data
// warning C4127 : Conditional expression is constant
// warning C4152 : nonstandard extension, function/data pointer conversion in expression
// warning C4231 : nonstandard extension used: 'identifier' before template explicit instantiation

// warning C4251 : needs to have dll-interface to be used by clients of ''
// warning C4275 : non dll-interface struct/class '' used as base for dll-interface struct/class ''
// warning C4355 : 'this' : used in base member initializer list

#pragma warning(disable : 4201 4239 4480 4481 4244 4127 4152)
#pragma warning(disable : 4251 4275 4355)	// dll-export interfaces
#if	(_MSC_VER <= 1700)	// <= vs2012
#	pragma warning(disable : 4231)
#endif

#if	(_MSC_VER >= 1900)	// since vs2015
#	pragma warning(disable : 4091)	// 'typedef ': ignored on left of '' when no variable is declared (thrown by third party code with 'typedef enum {}')
#endif

#if	!defined(HH_WARNINGS_PARANOID)
	// warning C4100 : '%s' unreferenced formal parameter
	// warning C4706 : assignment within conditional expression
	// warning C4701 : local variable '%s' may be used without having been initialized (usually, we wouldn't want that warning disabled, but the uninitialized variables detection in level 4 warnings is just plain dumb...)
	// warning C4710 : inline function not expanded
	// warning C4714 : function marked as __forceinline not inlined
	// warning C4206 : nonstandard extension used : translation unit is empty
	// warning C4057 : '%s' : '%t' differs in indirection to slightly different base types from '%t'
	// warning C4054 : 'type cast' : from function pointer '%t' to data pointer '%t'
	// warning C4055 : 'type cast' : from data pointer '%t' to function pointer '%t'

#	pragma warning(disable : 4100 4706 4701 4710 4714 4206 4057 4054 4055)

	// X64 compatibility warnings:
#	pragma warning(disable : 4312) // 'type cast' : conversion from '%t' to '%p' of greater size
#endif

//----------------------------------------------------------------------------
// enable off by default warnings:

// C4263 (level 4)	'function': member function does not override any base class virtual member function
// C4289 (level 4)	nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
// C4062 (level 4)	enumerator 'identifier' in a switch of enum 'enumeration' is not handled
//// C4557 (level 3)	'__assume' contains effect 'effect'
//// C4574 (level 4)	'identifier' is defined to be '0': did you mean to use '#if identifier'?
// C4668 (level 4)	'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
// C4928 (level 1)	illegal copy-initialization; more than one user-defined conversion has been implicitly applied
#pragma warning(default : 4263 4289 4062 /*4557*/ /*4574*/ 4668 4928)

//----------------------------------------------------------------------------
// warnings we want to treat as errors:

// warning C4002: too many actual parameters for macro '%s'
// warning C4003: not enough actual parameters for macro '%s'

#pragma warning(error : 4002 4003)

#if	(_MSC_VER >= 1500)	// if greater or equal to Visual Studio 2008
#	define	HH_HAS_VARIADIC_MACROS
#endif

// C++0x features

#if	(_MSC_VER >= 1600)	// if greater or equal to Visual Studio 2010
#	define	HH_HAS_NULLPTR					nullptr
#	define	HH_HAS_STATIC_ASSERT
#	ifndef	PK_HAS_AUTO
#		define	PK_HAS_AUTO					1
#	endif
/*#elif	(HH_CPU_MODEL == HH_CPU_XENON)		// VC++ compiler for XBox360 has variadic macros (NOTE: in fact, its _MSC_VER appears to be more recent, so it correctly falls into the #if above
#	define	HH_HAS_VARIADIC_MACROS*/
#	define	PK_HAS_RVALUE_REF				1	// warning ! apparently not there in vs 2010 before SP1
#	if	(_MSC_VER >= 1800)	&& !defined(HH_DURANGO) // since vs2013, but vs2015 Durango "internal error" on this !
#		define	HH_NULL_AS_VARIABLE		static const decltype(HH_HAS_NULLPTR) null = HH_HAS_NULLPTR
#	endif
#else
#	define	PK_HAS_RVALUE_REF				0
#endif

#ifndef	PK_HAS_AUTO
#	if	(_MSC_VER >= 1800)	// since vs2013
#		define	PK_HAS_AUTO					1
#	else
#		define	PK_HAS_AUTO					0
#	endif
#endif

#ifndef	PK_HAS_DECLTYPE
#	if	(_MSC_VER >= 1600)	// since vs2010
#		define	PK_HAS_DECLTYPE				1
#		define	PK_DECLTYPE(__expr)			decltype((__expr))
#	else
#		define	PK_HAS_DECLTYPE				0
#		define	PK_DECLTYPE(__expr)			typeof((__expr))
#	endif
#endif

#ifndef	PK_HAS_EXPLICIT_CONVERSIONS
#	if	(_MSC_VER >= 1800)	// since vs2013
#		define	PK_HAS_EXPLICIT_CONVERSIONS	1
#	else
#		define	PK_HAS_EXPLICIT_CONVERSIONS	0
#	endif
#endif

// C++11 Defaulted Deleted functions
#ifndef	PK_HAS_DEF_DEL_FUNCTIONS
#	if	(_MSC_VER >= 1900)	// since vs2015
#		define	PK_HAS_DEF_DEL_FUNCTIONS	1
#	else
#		define	PK_HAS_DEF_DEL_FUNCTIONS	0
#	endif
#endif

//----------------------------------------------------------------------------

#define	PK_SPLIT_BASIC_BLOCK(__comment)

// in release, force disabling of frame-pointer omission, to allow the callstacks to work correctly everywhere.
//#if	defined(HH_RELEASE) && !defined(HH_X360) && (HH_CPU_MODEL == HH_CPU_X86)
//#	pragma	optimize("y", off)
//#endif

//----------------------------------------------------------------------------

#define	HH_MESSAGE(__msg)			__pragma(message(__msg))

#ifdef	HH_SHORT_COMPILER_MESSAGES
#	define	HH_TODO(__msg)			__pragma( message("[L" STRINGIFY(__LINE__) "]: TODO: " __msg) )
#	define	HH_FIXME(__msg)			__pragma( message("[L" STRINGIFY(__LINE__) "]: FIXME: " __msg) )
#	define	HH_NOTE(__msg)			__pragma( message("[L" STRINGIFY(__LINE__) "]: NOTE: " __msg) )
#else
#	define	HH_TODO(__msg)			__pragma( message(__FILE__ "(" STRINGIFY(__LINE__) ") /!\\/!\\/!\\/!\\ TODO /!\\/!\\/!\\/!\\ " __msg) )
#	define	HH_FIXME(__msg)			__pragma( message(__FILE__ "(" STRINGIFY(__LINE__) ") /!\\/!\\/!\\/!\\ FIXME /!\\/!\\/!\\/!\\ " __msg) )
#	define	HH_NOTE(__msg)			__pragma( message(__FILE__ "(" STRINGIFY(__LINE__) ") /!\\/!\\/!\\/!\\ NOTE /!\\/!\\/!\\/!\\ " __msg) )
#endif

//----------------------------------------------------------------------------
#endif	// __HH_COMPILER_MSVC_H__
