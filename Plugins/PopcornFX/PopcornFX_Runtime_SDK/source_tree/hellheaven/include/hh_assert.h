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

#ifndef	__HH_ASSERT_H__
#define	__HH_ASSERT_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if	(HH_ANONYMOUS_ASSERTS != 0)
#	define	HH_ASSERT_CULL_STRING_IFN(__s)	"<anonymous>"	// will allow the linker to collapse all redundant occurences to a single symbol
#else
#	define	HH_ASSERT_CULL_STRING_IFN(__s)	__s
#endif

// asleep asserts
#define HH_ONLY_IF_ASSERTS__LETHARGIC(__expr)
#define HH_ONLY_IF_NO_ASSERTS__LETHARGIC(__expr)			__expr
//#define	HH_ASSERT__LETHARGIC(condition)						do { (void)sizeof(condition); } while (0)
//#define	HH_ASSERT_MESSAGE__LETHARGIC(condition, message)	do { (void)sizeof(condition); } while (0)	// message may contains commas, don't do anything with it. it whouldn't cause any problems

// active asserts
#define HH_ONLY_IF_ASSERTS__AWAKE(__expr)					__expr
#define HH_ONLY_IF_NO_ASSERTS__AWAKE(__expr)

#ifndef	HH_HAS_VARIADIC_MACROS
#	error compiler without variadic support macros are not supported anymore
#endif

#if	HH_CPU_MODEL == HH_CPU_CELL_SPU
#	define	HH_ASSERT__AWAKE(__condition)						do { if (HH_PREDICT_UNLIKELY(!(__condition))) { HH_BREAKPOINT(); } } while(0)
#	define	HH_ASSERT_MESSAGE__AWAKE(__condition, ...)			do { if (HH_PREDICT_UNLIKELY(!(__condition))) { HH_BREAKPOINT(); } } while(0)
#	define	HH_ASSERT__LETHARGIC(__condition)					do {} while(0)	//__noop(__condition)	// could be left empty, but avoids warnings about empty control statements in single-line control-flow expressions
#	define	HH_ASSERT_MESSAGE__LETHARGIC(__condition, ...)		do {} while(0)	//__noop(__condition, __VA_ARGS__)
#elif	defined(_PREFAST_)	// static analysis is enabled
#	define	HH_ASSERT__AWAKE(__condition)						do { if (HH_PREDICT_UNLIKELY(!(__condition))) { HH_BREAKPOINT(); } } while(0)//__assume(__condition)
#	define	HH_ASSERT_MESSAGE__AWAKE(__condition, ...)			do { if (HH_PREDICT_UNLIKELY(!(__condition))) { HH_BREAKPOINT(); } } while(0)//__assume(__condition)
#	define	HH_ASSERT__LETHARGIC(__condition)					do {} while(0)
#	define	HH_ASSERT_MESSAGE__LETHARGIC(__condition, ...)		do {} while(0)
#else
#	define	HH_ASSERT__AWAKE(__condition)						do { \
																	static bool	_ignored = false; \
																	if (HH_PREDICT_UNLIKELY(!_ignored) && HH_PREDICT_UNLIKELY(!(__condition))) \
																	{ \
																		switch (PopcornFX::Assert::EvalAssert(HH_ASSERT_CULL_STRING_IFN(__FILE__), HH_ASSERT_CULL_STRING_IFN(__FUNCTION__), __LINE__, HH_ASSERT_CULL_STRING_IFN(#__condition), HH_ASSERT_CULL_STRING_IFN(STRINGIFY(__condition)), HH_ASSERT_CULL_STRING_IFN(#__condition))) \
																		{ \
																		case	PopcornFX::Assert::Result_Break:	HH_BREAKPOINT();	break; \
																		case	PopcornFX::Assert::Result_Ignore:	_ignored = true;	break; \
																		case	PopcornFX::Assert::Result_Skip: \
																		default:														break; \
																		} \
																	} \
																} while (0)
#	define	HH_ASSERT_MESSAGE__AWAKE(__condition, ...)			do { \
																	static bool	_ignored = false; \
																	if (HH_PREDICT_UNLIKELY(!_ignored) && HH_PREDICT_UNLIKELY(!(__condition))) \
																	{ \
																		switch (PopcornFX::Assert::EvalAssertFormat(HH_ASSERT_CULL_STRING_IFN(__FILE__), HH_ASSERT_CULL_STRING_IFN(__FUNCTION__), __LINE__, HH_ASSERT_CULL_STRING_IFN(#__condition), HH_ASSERT_CULL_STRING_IFN(STRINGIFY(__condition)), __VA_ARGS__)) /* NOTE: the custom assert message will still be passed as first argument here, we need to cull lower */ \
																		{ \
																		case	PopcornFX::Assert::Result_Break:	HH_BREAKPOINT();	break; \
																		case	PopcornFX::Assert::Result_Ignore:	_ignored = true;	break; \
																		case	PopcornFX::Assert::Result_Skip: \
																		default:														break; \
																		} \
																	} \
																} while (0)
#	define	HH_ASSERT__LETHARGIC(__condition)					do {} while(0)	//__noop(__condition)	// could be left empty, but avoids warnings about empty control statements in single-line control-flow expressions
#	define	HH_ASSERT_MESSAGE__LETHARGIC(__condition, ...)		do {} while(0)	//__noop(__condition, __VA_ARGS__)
#endif

//----------------------------------------------------------------------------
//	release asserts

#if	(HH_ASSERTS_IN_RELEASE == 0)
#	define	HH_RELEASE_ASSERT					HH_ASSERT__LETHARGIC
#	define	HH_RELEASE_ASSERT_MESSAGE			HH_ASSERT_MESSAGE__LETHARGIC
#	define	HH_ONLY_IF_RELEASE_ASSERTS			HH_ONLY_IF_ASSERTS__LETHARGIC
#	define	HH_ONLY_IF_NO_RELEASE_ASSERTS		HH_ONLY_IF_NO_ASSERTS__LETHARGIC
#else
#	define	HH_RELEASE_ASSERT					HH_ASSERT__AWAKE
#	define	HH_RELEASE_ASSERT_MESSAGE			HH_ASSERT_MESSAGE__AWAKE
#	define	HH_ONLY_IF_RELEASE_ASSERTS			HH_ONLY_IF_ASSERTS__AWAKE
#	define	HH_ONLY_IF_NO_RELEASE_ASSERTS		HH_ONLY_IF_NO_ASSERTS__AWAKE
#endif

//----------------------------------------------------------------------------
//	debug asserts

#if	(HH_ASSERTS_IN_DEBUG == 0)
#	define	HH_ASSERT							HH_ASSERT__LETHARGIC
#	define	HH_ASSERT_MESSAGE					HH_ASSERT_MESSAGE__LETHARGIC
#	define	HH_ONLY_IF_ASSERTS					HH_ONLY_IF_ASSERTS__LETHARGIC
#	define	HH_ONLY_IF_NO_ASSERTS				HH_ONLY_IF_NO_ASSERTS__LETHARGIC
#else
#	define	HH_ASSERT							HH_ASSERT__AWAKE
#	define	HH_ASSERT_MESSAGE					HH_ASSERT_MESSAGE__AWAKE
#	define	HH_ONLY_IF_ASSERTS					HH_ONLY_IF_ASSERTS__AWAKE
#	define	HH_ONLY_IF_NO_ASSERTS				HH_ONLY_IF_NO_ASSERTS__AWAKE
#endif

//----------------------------------------------------------------------------
//	paranoid asserts

#if	(HH_ASSERTS_IN_DEBUG == 0) || !defined(HH_ASSERTS_LEVEL_PARANOID)
#	define	HH_PARANOID_ASSERT					HH_ASSERT__LETHARGIC
#	define	HH_PARANOID_ASSERT_MESSAGE			HH_ASSERT_MESSAGE__LETHARGIC
#	define	HH_ONLY_IF_PARANOID_ASSERTS			HH_ONLY_IF_ASSERTS__LETHARGIC
#	define	HH_ONLY_IF_NO_PARANOID_ASSERTS		HH_ONLY_IF_NO_ASSERTS__LETHARGIC
#else
#	define	HH_PARANOID_ASSERT					HH_ASSERT__AWAKE
#	define	HH_PARANOID_ASSERT_MESSAGE			HH_ASSERT_MESSAGE__AWAKE
#	define	HH_ONLY_IF_PARANOID_ASSERTS			HH_ONLY_IF_ASSERTS__AWAKE
#	define	HH_ONLY_IF_NO_PARANOID_ASSERTS		HH_ONLY_IF_NO_ASSERTS__AWAKE
#endif

//----------------------------------------------------------------------------

#define	HH_EXTRACT_FIRST_VAARG(__arg, ...)				__arg

#define	HH_ASSERT_NOT_REACHED()							do { HH_ASSERT_MESSAGE(false, "Control point shouldn't be reached."); } while (0)
#define	HH_ASSERT_NOT_REACHED_MESSAGE(...)				do { HH_ASSERT_MESSAGE(false, __VA_ARGS__); } while (0)
#define	HH_ASSERT_DEPRECATED()							do { HH_TODO("Deprecated code"); HH_ASSERT_MESSAGE(false, "<Deprecated>"); } while (0)
#define	HH_ASSERT_DEPRECATED_MESSAGE(...)				do { HH_TODO("Deprecated code"); HH_ASSERT_MESSAGE(false, __VA_ARGS__); } while (0)
#define	HH_ASSERT_NOT_IMPLEMENTED()						do { HH_TODO("implement this..."); HH_ASSERT_MESSAGE(false, "<Not implemented>"); } while (0)
#define	HH_ASSERT_NOT_IMPLEMENTED_MESSAGE(...)			do { HH_TODO(HH_EXTRACT_FIRST_VAARG(__VA_ARGS__)); HH_ASSERT_MESSAGE(false, "%s", __VA_ARGS__); } while (0)

#define	HH_RELEASE_ASSERT_NOT_REACHED()					do { HH_RELEASE_ASSERT_MESSAGE(false, "Control point shouldn't be reached."); } while (0)
#define	HH_RELEASE_ASSERT_NOT_REACHED_MESSAGE(...)		do { HH_RELEASE_ASSERT_MESSAGE(false, __VA_ARGS__); } while (0)
#define	HH_RELEASE_ASSERT_DEPRECATED()					do { HH_TODO("Deprecated code"); HH_RELEASE_ASSERT_MESSAGE(false, "<Deprecated>"); } while (0)
#define	HH_RELEASE_ASSERT_DEPRECATED_MESSAGE(...)		do { HH_TODO("Deprecated code"); HH_RELEASE_ASSERT_MESSAGE(false, __VA_ARGS__); } while (0)
#define	HH_RELEASE_ASSERT_NOT_IMPLEMENTED()				do { HH_TODO("implement this..."); HH_RELEASE_ASSERT_MESSAGE(false, "<Not implemented>"); } while (0)
#define	HH_RELEASE_ASSERT_NOT_IMPLEMENTED_MESSAGE(...)	do { HH_TODO(HH_EXTRACT_FIRST_VAARG(__VA_ARGS__)); HH_RELEASE_ASSERT_MESSAGE(false, __VA_ARGS__); } while (0)

//----------------------------------------------------------------------------
//
//	HH_VERIFY(__condition)
//	HH_RELEASE_VERIFY(__condition)
//
//	HH_VERIFY_MESSAGE(__condition, __format, ...)
//	HH_RELEASE_VERIFY_MESSAGE(__condition, __format, ...)
//
//	__condition is ALWAYS evaluated
//	if asserts are enabled, HH_VERIFY asserts if !(__condition)
//
//	HH_VERIFY returns the condition value:
//
//	if (HH_VERIFY( shouldBeTrue ))
//		return OK;
//
//	if (!HH_VERIFY( shouldBeTrue ))
//		return FAILED;
//
//----------------------------------------------------------------------------

template<hh_u32 __UniqueForStatic_ignored>
HH_FORCEINLINE static bool			&_VerifyStaticBoolIgnored() { static bool _ignored = false; return _ignored; }

//
//	Some compilers dont like __asm__ in ternaries
//	Work arround with a function
//
#if	defined(HH_COMPILER_MSVC)
#	define _HH_VERIFY__BREAKPOINT()		HH_BREAKPOINT()
#else
HH_FORCEINLINE static bool				_Verify_BreakPoint() { HH_BREAKPOINT(); return true; }
#	define _HH_VERIFY__BREAKPOINT()		::PopcornFX::_Verify_BreakPoint()
#endif

#define HH_VERIFY__NO_ASSERT(__condition)					HH_PREDICT_LIKELY(!!(__condition))
#define HH_VERIFY__ASSERT(__condition)					\
	(													\
		HH_PREDICT_UNLIKELY(!(__condition)) ?			\
		(												\
			PopcornFX::Assert::VerifyAssertFalse_Breaks(PopcornFX::_VerifyStaticBoolIgnored<__COUNTER__>(), HH_ASSERT_CULL_STRING_IFN(__FILE__), HH_ASSERT_CULL_STRING_IFN(__FUNCTION__), __LINE__, HH_ASSERT_CULL_STRING_IFN(#__condition), HH_ASSERT_CULL_STRING_IFN(STRINGIFY(__condition)), HH_ASSERT_CULL_STRING_IFN(#__condition)) \
			? (_HH_VERIFY__BREAKPOINT(), false) : false	\
		)												\
		: true											\
	)

#define HH_VERIFY_MESSAGE__NO_ASSERT(__condition, ...)		HH_PREDICT_LIKELY(!!(__condition))
#define HH_VERIFY_MESSAGE__ASSERT(__condition, ...)		\
	(													\
		HH_PREDICT_UNLIKELY(!(__condition)) ?			\
		(												\
			PopcornFX::Assert::VerifyAssertFalseFormat_Breaks(PopcornFX::_VerifyStaticBoolIgnored<__COUNTER__>(), HH_ASSERT_CULL_STRING_IFN(__FILE__), HH_ASSERT_CULL_STRING_IFN(__FUNCTION__), __LINE__, HH_ASSERT_CULL_STRING_IFN(#__condition), HH_ASSERT_CULL_STRING_IFN(STRINGIFY(__condition)), __VA_ARGS__) \
			? (_HH_VERIFY__BREAKPOINT(), false) : false	\
		)												\
		: true											\
	)

//----------------------------------------------------------------------------

#if	(HH_ASSERTS_IN_RELEASE == 0)
#	define	HH_RELEASE_VERIFY				HH_VERIFY__NO_ASSERT
#	define	HH_RELEASE_VERIFY_MESSAGE		HH_VERIFY_MESSAGE__NO_ASSERT
#else
#	define	HH_RELEASE_VERIFY				HH_VERIFY__ASSERT
#	define	HH_RELEASE_VERIFY_MESSAGE		HH_VERIFY_MESSAGE__ASSERT
#endif

#if	(HH_ASSERTS_IN_DEBUG == 0)
#	define	HH_VERIFY						HH_VERIFY__NO_ASSERT
#	define	HH_VERIFY_MESSAGE				HH_VERIFY_MESSAGE__NO_ASSERT
#else
#	define	HH_VERIFY						HH_VERIFY__ASSERT
#	define	HH_VERIFY_MESSAGE				HH_VERIFY_MESSAGE__ASSERT
#endif

#if	(HH_ASSERTS_IN_DEBUG == 0) || !defined(HH_ASSERTS_LEVEL_PARANOID)
#	define	HH_PARANOID_VERIFY				HH_VERIFY__NO_ASSERT
#	define	HH_PARANOID_VERIFY_MESSAGE		HH_VERIFY_MESSAGE__NO_ASSERT
#else
#	define	HH_PARANOID_VERIFY				HH_VERIFY__ASSERT
#	define	HH_PARANOID_VERIFY_MESSAGE		HH_VERIFY_MESSAGE__ASSERT
#endif

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

#define PK_ASSERT_CATCHER_PARAMETERS		const char *file, const char *function, int line, const char *failed, const char *expanded, const char *message
#define PK_ASSERT_CATCHER_ARGUMENTS			file, function, line, failed, expanded, message

namespace	Assert
{
	enum	EResult
	{
		Result_Break	= 0,
		Result_Ignore	= 1,
		Result_Skip		= 2,
	};

	typedef Assert::EResult	(*CbCatcher)(PK_ASSERT_CATCHER_PARAMETERS);
	HH_KERNEL_EXPORT void		SetCatcher(CbCatcher catcher);
	HH_KERNEL_EXPORT CbCatcher	Catcher();

// don't check for 'HH_NO_ASSERTS' here! we want hh_kernel to be built with these exported in case we decide to disable
// asserts for hh_kernel, but still want to use them within another module without breaking runtime linking.
//#if	!defined(HH_NO_ASSERTS)

#if	HH_CPU_MODEL == HH_CPU_CELL_SPU
	HH_FORCEINLINE EResult		EvalAssert(PK_ASSERT_CATCHER_PARAMETERS) { return Result_Break; }
	HH_FORCEINLINE EResult		EvalAssertFormat(PK_ASSERT_CATCHER_PARAMETERS, ...) { return Result_Break; }
	HH_FORCEINLINE bool			VerifyAssertFalse_Breaks(bool &_ignored, PK_ASSERT_CATCHER_PARAMETERS) { return false; }
	HH_FORCEINLINE bool			VerifyAssertFalseFormat_Breaks(bool &_ignored, PK_ASSERT_CATCHER_PARAMETERS, ...) { return false; }
#else
	HH_KERNEL_EXPORT EResult	EvalAssert(PK_ASSERT_CATCHER_PARAMETERS);
	HH_KERNEL_EXPORT EResult	EvalAssertFormat(PK_ASSERT_CATCHER_PARAMETERS, ...) PK_COMPILER_ATTRIB_PRINTF(6, 7);
	HH_KERNEL_EXPORT bool		VerifyAssertFalse_Breaks(bool &_ignored, PK_ASSERT_CATCHER_PARAMETERS);
	HH_KERNEL_EXPORT bool		VerifyAssertFalseFormat_Breaks(bool &_ignored, PK_ASSERT_CATCHER_PARAMETERS, ...) PK_COMPILER_ATTRIB_PRINTF(7, 8);
#endif

//#endif // !defined(HH_NO_ASSERTS)
}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_static_assert.h"

#endif
