#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2006/11/16 3:35 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_STATIC_ASSERT_H__
#define	__HH_STATIC_ASSERT_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

#define	HH_STATIC_ASSERT_CUSTOM__AWAKE(__condition, __message)	_HH_STATIC_ASSERT_BUILD_CUSTOM(__condition, GLUE(__, __message), __COUNTER__)

// special static asserts for function-scoped asserts
#define	HH_STATIC_ASSERT_BUILD_CUSTOM_FN_MESSAGE(__message)		GLUE(ASSERT_FAILED__, __message)
#define	HH_STATIC_ASSERT_CUSTOM_FN__AWAKE(condition, __message)	typedef int	HH_STATIC_ASSERT_BUILD_CUSTOM_FN_MESSAGE(__message)[2]; typedef int HH_STATIC_ASSERT_BUILD_CUSTOM_FN_MESSAGE(__message)[(condition) ? 2 : -1]
#define	HH_STATIC_ASSERT_FN__AWAKE(__condition)					HH_STATIC_ASSERT_CUSTOM_FN__AWAKE(__condition, __FAILED__)

// static asserts implementation:
template<hh_u32 _mescouilles>	struct	SStaticAssertTester {};

#define	_HH_STATIC_ASSERT_CUSTOM_MESSAGE(__message, __counter)		STATIC_ASSERT_ ## __counter ## __message ## __
#define	_HH_STATIC_ASSERT_BUILD_CUSTOM(__condition, __message, __counter) \
template<bool Failed>	struct	_HH_STATIC_ASSERT_CUSTOM_MESSAGE(__message, __counter); \
template<>				struct	_HH_STATIC_ASSERT_CUSTOM_MESSAGE(__message, __counter)<false> {}; \
typedef SStaticAssertTester<sizeof(_HH_STATIC_ASSERT_CUSTOM_MESSAGE(__message, __counter)<!(__condition)>)> GLUE(_MesCouilles, __counter)

//----------------------------------------------------------------------------

#define	HH_STATIC_ASSERT_CUSTOM_COUNT(__count, __targetCount, __customMessage) \
			HH_STATIC_ASSERT_CUSTOM(__count == __targetCount || __count < __targetCount, STRINGIFY(NotEnough ## __customMessage)); \
			HH_STATIC_ASSERT_CUSTOM(__count == __targetCount || __count > __targetCount, STRINGIFY(TooMany ## __customMessage))

//----------------------------------------------------------------------------

#ifdef	HH_HAS_STATIC_ASSERT
// C++0x feature set, has native static_assert()

//#define	HH_STATIC_ASSERT(__condition, ...)	static_assert(__condition, "static_assert failed. " __VA_ARGS__)

#define	HH_STATIC_ASSERT(__condition)					static_assert(__condition, STRINGIFY(__condition))
#define	HH_STATIC_ASSERT_CUSTOM(__condition, __msg)		static_assert(__condition, __msg)
#define	HH_STATIC_ASSERT_NOT_IMPLEMENTED()				static_assert(false, "static_assert: not implemented. ")
#define	HH_STATIC_ASSERT_NOT_IMPLEMENTED_CUSTOM(__msg)	static_assert(false, "static_assert: not implemented. " __msg)
#define	HH_STATIC_ASSERT_DEPRECATED()					static_assert(false, "static_assert: DEPRECATED.")

// we can't use static_assert for these, as they will be wrongly triggered, even when the place they're in is not instantiated
#define	HH_STATIC_ASSERT_NOT_REACHED()					//HH_STATIC_ASSERT_CUSTOM__AWAKE(false, NOT_REACHED)//static_assert(false, "static_assert: not reached. ")
#define	HH_STATIC_ASSERT_NOT_REACHED_CUSTOM(__msg)		//HH_STATIC_ASSERT_CUSTOM__AWAKE(false, NOT_REACHED)//static_assert(false, "static_assert: not reached. " __msg)

#else
// use hand-made C++ static_assert

#if defined(HH_COMPILER_MSVC)
//----------------------------------------------------------------------------
// visual studio specific implementation. produces clean and concise results

template<bool StaticAssertFailed>	struct	STATIC_ASSERTION_SUCCESS;
template<>							struct	STATIC_ASSERTION_SUCCESS<false> {};

#define	_HH_STATIC_ASSERT_BUILD(__condition, __counter) \
	typedef PopcornFX::SStaticAssertTester<sizeof(PopcornFX::STATIC_ASSERTION_SUCCESS<!(__condition)>)> GLUE(_MesCouilles, __counter)

#define	HH_STATIC_ASSERT__AWAKE(__condition)							_HH_STATIC_ASSERT_BUILD(__condition, __COUNTER__)

// static asserts are always awake
#define	__HH_STATIC_ASSERT												HH_STATIC_ASSERT__AWAKE
#define	__HH_STATIC_ASSERT_FN											HH_STATIC_ASSERT_FN__AWAKE
#define	__HH_STATIC_ASSERT_CUSTOM										HH_STATIC_ASSERT_CUSTOM__AWAKE
#define	__HH_STATIC_ASSERT_CUSTOM_FN									HH_STATIC_ASSERT_CUSTOM_FN__AWAKE
#define	__HH_STATIC_ASSERT_NOT_REACHED()								__HH_STATIC_ASSERT_CUSTOM(false, NOT_REACHED)
//#define	__HH_STATIC_ASSERT_NOT_REACHED_FN()							__HH_STATIC_ASSERT_CUSTOM_FN(false, NOT_REACHED)
#define	__HH_STATIC_ASSERT_NOT_REACHED_CUSTOM(custom_message)			__HH_STATIC_ASSERT_CUSTOM(false, custom_message)
//#define	__HH_STATIC_ASSERT_NOT_REACHED_CUSTOM_FN(custom_message)	__HH_STATIC_ASSERT_CUSTOM_FN(false, custom_message)

#elif defined(HH_COMPILER_GCC) || defined(HH_COMPILER_GHS)
//----------------------------------------------------------------------------
// GCC specific version, doesn't work everywhere.
// will produce errors in earlier GCC that do not support __COUNTER__ if two static asserts are used on the same line within struct, classes, or template function declarations,
// as __COUNTER__ will be typedef-ed in hh_compiler_gcc.h to __LINE__ if not available and not already defined.
// I don't like that. really. It's better to have static asserts when building on GCC, but still...
// can't use do { ... } while(0) construct because it will prevent static asserts at namespace or struct / class scope.

#define	HH_STATIC_ASSERT__AWAKE(e)	typedef int	GLUE(StaticAssertionFailed_At_Line, __COUNTER__)[1 - 2 * !(e)]

// static asserts are always awake
#define	__HH_STATIC_ASSERT(condition)									HH_STATIC_ASSERT__AWAKE(condition)
#define	__HH_STATIC_ASSERT_FN(condition)								HH_STATIC_ASSERT_FN__AWAKE(condition)
#define	__HH_STATIC_ASSERT_CUSTOM(condition, msg)						HH_STATIC_ASSERT__AWAKE(condition)
#define	__HH_STATIC_ASSERT_CUSTOM_FN(condition, msg)					HH_STATIC_ASSERT_FN__AWAKE(condition)

// we can't use static_assert for these, as they will be wrongly triggered, even when the place they're in is not instantiated
#if	defined(HH_COMPILER_GHS)
#	define	__HH_STATIC_ASSERT_NOT_REACHED()							__HH_STATIC_ASSERT_CUSTOM(true, NOT_REACHED)
//#	define	__HH_STATIC_ASSERT_NOT_REACHED_FN()							__HH_STATIC_ASSERT_CUSTOM_FN(true, NOT_REACHED)
#	define	__HH_STATIC_ASSERT_NOT_REACHED_CUSTOM(custom_message)		__HH_STATIC_ASSERT_CUSTOM(true, custom_message)
//#	define	__HH_STATIC_ASSERT_NOT_REACHED_CUSTOM_FN(custom_message)	__HH_STATIC_ASSERT_CUSTOM_FN(true, custom_message)
#else
#	define	__HH_STATIC_ASSERT_NOT_REACHED()							//__HH_STATIC_ASSERT_CUSTOM(false, NOT_REACHED)
//#	define	__HH_STATIC_ASSERT_NOT_REACHED_FN()							//__HH_STATIC_ASSERT_CUSTOM_FN(false, NOT_REACHED)
#	define	__HH_STATIC_ASSERT_NOT_REACHED_CUSTOM(custom_message)		//__HH_STATIC_ASSERT_CUSTOM(false, custom_message)
//#	define	__HH_STATIC_ASSERT_NOT_REACHED_CUSTOM_FN(custom_message)	//__HH_STATIC_ASSERT_CUSTOM_FN(false, custom_message)
#endif

// we don't handle multiline comments, so we have to redefine this to a simpler form
#undef	HH_STATIC_ASSERT_CUSTOM_COUNT
#define	HH_STATIC_ASSERT_CUSTOM_COUNT(__count, __targetCount, __customMessage) \
			HH_STATIC_ASSERT_CUSTOM(__count == __targetCount, STRINGIFY(InvalidNumberOf ## __customMessage))

#endif	/*compiler-specific impl*/

#define	__HH_STATIC_ASSERT_NOT_IMPLEMENTED()							__HH_STATIC_ASSERT_CUSTOM(false, NOT_IMPLEMENTED)
//#define	__HH_STATIC_ASSERT_NOT_IMPLEMENTED_FN()							__HH_STATIC_ASSERT_CUSTOM_FN(false, NOT_IMPLEMENTED)
#define	__HH_STATIC_ASSERT_NOT_IMPLEMENTED_CUSTOM(custom_message)		__HH_STATIC_ASSERT_CUSTOM(false, custom_message)
//#define	__HH_STATIC_ASSERT_NOT_IMPLEMENTED_CUSTOM_FN(custom_message)	__HH_STATIC_ASSERT_CUSTOM_FN(false, custom_message)
#define	__HH_STATIC_ASSERT_DEPRECATED()									__HH_STATIC_ASSERT_CUSTOM(false, DEPRECATED)
//#define	__HH_STATIC_ASSERT_DEPRECATED_FN()								__HH_STATIC_ASSERT_CUSTOM_FN(false, DEPRECATED)
#define	__HH_STATIC_ASSERT_DEPRECATED_CUSTOM(custom_message)			__HH_STATIC_ASSERT_CUSTOM(false, custom_message)
//#define	__HH_STATIC_ASSERT_DEPRECATED_CUSTOM_FN(custom_message)			__HH_STATIC_ASSERT_CUSTOM_FN(false, custom_message)

#define	HH_STATIC_ASSERT(__condition)					__HH_STATIC_ASSERT(__condition/*, STRINGIFY(__condition)*/)
#define	HH_STATIC_ASSERT_CUSTOM(__condition, __msg)		__HH_STATIC_ASSERT(__condition/*, __msg*/)
#define	HH_STATIC_ASSERT_NOT_REACHED()					__HH_STATIC_ASSERT_NOT_REACHED()
#define	HH_STATIC_ASSERT_NOT_REACHED_CUSTOM(__msg)		__HH_STATIC_ASSERT_NOT_REACHED()
#define	HH_STATIC_ASSERT_NOT_IMPLEMENTED()				__HH_STATIC_ASSERT_NOT_IMPLEMENTED()
#define	HH_STATIC_ASSERT_NOT_IMPLEMENTED_CUSTOM(__msg)	__HH_STATIC_ASSERT_NOT_IMPLEMENTED()
#define	HH_STATIC_ASSERT_DEPRECATED()					__HH_STATIC_ASSERT_DEPRECATED()

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif	/*__HH_STATIC_ASSERT_H__*/
