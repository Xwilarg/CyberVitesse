#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2003/07/01 3:22 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_DEFS_H__
#define	__HH_DEFS_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_defs.h must not be included outside hellheaven.h
#endif

//----------------------------------------------------------------------------
__PK_API_END	// temporarily leave the PopcornFX namespace

#if	(PK_HAS_EXCEPTIONS == 0)
#	ifdef	_HAS_EXCEPTIONS
#		undef	_HAS_EXCEPTIONS
#	endif
#	define	_HAS_EXCEPTIONS	0

#	if !defined(HH_LINUX) && !defined(HH_MACOSX) && !defined(HH_ANDROID) && !defined(HH_IOS) && !defined(HH_NX)
#		include <xstddef>
#	else
#		include <stddef.h> // dont cstddef: avoid dependency on c++ stdlib here
#	endif
#endif	// (PK_HAS_EXCEPTIONS == 0)


#include "hellheaven_api.h"
#include "hh_assert.h"

// platform intrinsics:
#if	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#	include <ppu_intrinsics.h>
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
#	include <spu_intrinsics.h>
#elif	(HH_CPU_MODEL == HH_CPU_XENON)
#	include <PPCIntrinsics.h>
#elif	(HH_CPU_MODEL == HH_CPU_JAGUAR)
#	include <immintrin.h>	// SSE/AVX
#elif	(HH_CPU_ARCH == HH_CPU_X86)	// Generic X86
#	if	!defined(HH_COMPILER_GCC) || defined(__SSE__)
#		include <emmintrin.h>	// minimal SSE build version for popcorn is SSE2 since 2013
//#		include <xmmintrin.h>
#	endif
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// check our types have the expected size:

HH_STATIC_ASSERT(sizeof(hh_u8) == 1);
HH_STATIC_ASSERT(sizeof(hh_u16) == 2);
HH_STATIC_ASSERT(sizeof(hh_u32) == 4);
HH_STATIC_ASSERT(sizeof(hh_u64) == 8);
HH_STATIC_ASSERT(sizeof(hh_ureg) == sizeof(void*));

HH_STATIC_ASSERT(sizeof(hh_i8) == 1);
HH_STATIC_ASSERT(sizeof(hh_i16) == 2);
HH_STATIC_ASSERT(sizeof(hh_i32) == 4);
HH_STATIC_ASSERT(sizeof(hh_i64) == 8);
HH_STATIC_ASSERT(sizeof(hh_ireg) == sizeof(void*));

HH_STATIC_ASSERT(sizeof(float) == 4);
HH_STATIC_ASSERT(sizeof(double) == 8);

//----------------------------------------------------------------------------

#if		defined(HH_DEBUG)
#	define HH_ONLY_IN_DEBUG(__expr)			__expr
#	define HH_ONLY_IN_RELEASE(__expr)		NOTHING
#	define HH_ONLY_IN_RETAIL(__expr)		NOTHING
#	define HH_ONLY_IN_NDEBUG(__expr)		NOTHING		// in NOT debug
#	define HH_ONLY_IN_NRETAIL(__expr)		__expr		// in NOT retail
#elif	defined(HH_RETAIL)
#	define HH_ONLY_IN_DEBUG(__expr)			NOTHING
#	define HH_ONLY_IN_RELEASE(__expr)		NOTHING
#	define HH_ONLY_IN_RETAIL(__expr)		__expr
#	define HH_ONLY_IN_NDEBUG(__expr)		__expr		// in NOT debug
#	define HH_ONLY_IN_NRETAIL(__expr)		NOTHING		// in NOT retail
#else	// HH_RELEASE
#	define HH_ONLY_IN_DEBUG(__expr)			NOTHING
#	define HH_ONLY_IN_RELEASE(__expr)		__expr
#	define HH_ONLY_IN_RETAIL(__expr)		NOTHING
#	define HH_ONLY_IN_NDEBUG(__expr)		__expr		// in NOT debug
#	define HH_ONLY_IN_NRETAIL(__expr)		__expr		// in NOT retail
#endif

//----------------------------------------------------------------------------

#define	HH_ARRAY_COUNT(__arr)				(sizeof(__arr) / sizeof((__arr)[0]))

#if	defined(HH_COMPILER_GCC) || /* GCC and Clang */ \
	(defined(HH_COMPILER_MSVC) && (HH_COMPILER_VERSION >= 1911)) /* msvc 15 (visual studio 2017) */
#	define	HH_MEMBER_OFFSET(__type, __member)	__builtin_offsetof(__type, __member)
#else
#	define	HH_MEMBER_OFFSET(__type, __member)	(hh_ureg(&(((__type*)0xB00B500)->__member)) - hh_ureg(0xB00B500))	// some compilers issue a null ptr deref warning if we use '0' as the base address
#endif

//----------------------------------------------------------------------------

#if	(PK_HAS_AUTO != 0)

template<typename _TypeA, typename _TypeB>
HH_FORCEINLINE auto	HHMin(const _TypeA &a, const _TypeB &b) -> decltype((b < a) ? b : a) { return ((b < a) ? b : a); }

template<typename _TypeA, typename _TypeB, typename _TypeC>
HH_FORCEINLINE auto	HHMin(const _TypeA &a, const _TypeB &b, const _TypeC &c) -> decltype(HHMin(HHMin(a, b), c)) { return HHMin(HHMin(a, b), c); }

template<typename _TypeA, typename _TypeB, typename _TypeC, typename _TypeD>
HH_FORCEINLINE auto	HHMin(const _TypeA &a, const _TypeB &b, const _TypeC &c, const _TypeD &d) -> decltype(HHMin(HHMin(a, b), HHMin(c, d))) { return HHMin(HHMin(a, b), HHMin(c, d)); }

//----------------------------------------------------------------------------

template<typename _TypeA, typename _TypeB>
HH_FORCEINLINE auto	HHMax(const _TypeA &a, const _TypeB &b) -> decltype((a > b) ? a : b) { return ((a > b) ? a : b); }

template<typename _TypeA, typename _TypeB, typename _TypeC>
HH_FORCEINLINE auto	HHMax(const _TypeA &a, const _TypeB &b, const _TypeC &c) -> decltype(HHMax(HHMax(a, b), c)) { return HHMax(HHMax(a, b), c); }

template<typename _TypeA, typename _TypeB, typename _TypeC, typename _TypeD>
HH_FORCEINLINE auto	HHMax(const _TypeA &a, const _TypeB &b, const _TypeC &c, const _TypeD &d) -> decltype(HHMax(HHMax(a, b), HHMax(c, d))) { return HHMax(HHMax(a, b), HHMax(c, d)); }


//----------------------------------------------------------------------------
#else

template<typename _TypeA, typename _TypeB>
HH_FORCEINLINE _TypeA	HHMin(const _TypeA a, const _TypeB b) { return ((b < a) ? b : a); }

template<typename _TypeA, typename _TypeB, typename _TypeC>
HH_FORCEINLINE _TypeA	HHMin(const _TypeA a, const _TypeB b, const _TypeC c) { return HHMin(HHMin(a, b), c); }

template<typename _TypeA, typename _TypeB, typename _TypeC, typename _TypeD>
HH_FORCEINLINE _TypeA	HHMin(const _TypeA a, const _TypeB b, const _TypeC c, const _TypeD d) { return HHMin(HHMin(a, b), HHMin(c, d)); }

// required headers not there yet

//template<typename _TypeA, typename _TypeB>
//TTypePromoter<_TypeA, _TypeB>::PromotedType
//	HHMin(const _TypeA a, const _TypeB b) { return ((b < a) ? b : a); }
//
//template<typename _TypeA, typename _TypeB, typename _TypeC>
//TTypePromoter<TTypePromoter<_TypeA, _TypeB>::PromotedType, _TypeC>::PromotedType
//	HHMin(const _TypeA a, const _TypeB b, const _TypeC c) { return HHMin(HHMin(a, b), c); }
//
//template<typename _TypeA, typename _TypeB, typename _TypeC, typename _TypeD>
//TTypePromoter<TTypePromoter<_TypeA, _TypeB>::PromotedType, TTypePromoter<_TypeC, _TypeD>::PromotedType>::PromotedType
//	HHMin(const _TypeA a, const _TypeB b, const _TypeC c, const _TypeD d) { return HHMin(HHMin(a, b), HHMin(c, d)); }

//----------------------------------------------------------------------------

template<typename _TypeA, typename _TypeB>
HH_FORCEINLINE _TypeA	HHMax(const _TypeA a, const _TypeB b) { return ((a > b) ? a : b); }

template<typename _TypeA, typename _TypeB, typename _TypeC>
HH_FORCEINLINE _TypeA	HHMax(const _TypeA a, const _TypeB b, const _TypeC c) { return HHMax(HHMax(a, b), c); }

template<typename _TypeA, typename _TypeB, typename _TypeC, typename _TypeD>
HH_FORCEINLINE _TypeA	HHMax(const _TypeA a, const _TypeB b, const _TypeC c, const _TypeD d) { return HHMax(HHMax(a, b), HHMax(c, d)); }

// required headers not there yet

//template<typename _TypeA, typename _TypeB>
//TTypePromoter<_TypeA, _TypeB>::PromotedType
//	HHMax(const _TypeA a, const _TypeB b) { return ((a > b) ? a : b); }
//
//template<typename _TypeA, typename _TypeB, typename _TypeC>
//TTypePromoter<TTypePromoter<_TypeA, _TypeB>::PromotedType, _TypeC>::PromotedType
//	HHMax(const _TypeA a, const _TypeB b, const _TypeC c) { return HHMax(HHMax(a, b), c); }
//
//template<typename _TypeA, typename _TypeB, typename _TypeC, typename _TypeD>
//TTypePromoter<TTypePromoter<_TypeA, _TypeB>::PromotedType, TTypePromoter<_TypeC, _TypeD>::PromotedType>::PromotedType
//	HHMax(const _TypeA a, const _TypeB b, const _TypeC c, const _TypeD d) { return HHMax(HHMax(a, b), HHMax(c, d)); }

#endif

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE _Type	HHAbs(const _Type v) { return v >= 0 ? v : -v; }

//----------------------------------------------------------------------------
// dummy specializations for unsigned types, to be used within other templates:
// we could use TTypeTraits::IsSigned<_T>::True to specialize this, but it's prob. not worth the hassle.

HH_FORCEINLINE hh_u8	HHAbs(const hh_u8 v) { return v; }
HH_FORCEINLINE hh_u16	HHAbs(const hh_u16 v) { return v; }
HH_FORCEINLINE hh_u32	HHAbs(const hh_u32 v) { return v; }
HH_FORCEINLINE hh_u64	HHAbs(const hh_u64 v) { return v; }

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE _Type	HHSign(_Type x) { return x < _Type(0) ? _Type(-1) : _Type(1); }

template<typename _Type>
HH_FORCEINLINE _Type	HHSquared(const _Type &value) { return value * value; }

template<typename _Type, typename _TypeMin, typename _TypeMax>
HH_FORCEINLINE _Type	HHClamp(const _Type v, const _TypeMin min, const _TypeMax max) { return (v < min) ? min : (v > max) ? max : v; }

template<typename _Type, typename _TypeFrac>
HH_FORCEINLINE _Type	HHLerp(const _Type a, const _Type b, const _TypeFrac f) { return a + f * (b - a); }

//----------------------------------------------------------------------------
//
//	platform-specific versions:
//
//----------------------------------------------------------------------------

#if		(HH_CPU_MODEL == HH_CPU_XENON)
#	include "hh_arch_xenon.h"
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#	include "hh_arch_cell-ppu.h"
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
#	include "hh_arch_cell-spu.h"
#elif	(HH_CPU_ARCH == HH_CPU_X86)
#	include "hh_arch_x86-64.h"
#elif	(HH_CPU_ARCH == HH_CPU_ARM)
#	include "hh_arch_arm.h"
#else
#	include "hh_arch_generic.h"
#endif

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE void	HHSwap(_Type &a, _Type &b)
{
	const _Type	tmp = a;
	a = b;
	b = tmp;
}

//----------------------------------------------------------------------------

#include "hh_template_metaprogramming.h"

//----------------------------------------------------------------------------
// cast helpers

template<typename _TypeDst, typename _TypeSrc>
HH_FORCEINLINE _TypeDst	checked_genericcast(const _TypeSrc ptr)
{
	HH_STATIC_ASSERT(TTypeTraits::IsPointer<_TypeDst>::True && TTypeTraits::IsPointer<_TypeSrc>::True);
#if	(PK_HAS_RTTI != 0)
	HH_ASSERT(ptr == null || dynamic_cast<_TypeDst>(ptr) != null);
#endif
	return reinterpret_cast<_TypeDst>(ptr);
}

//----------------------------------------------------------------------------

template<typename _TypeDst, typename _TypeSrc>
HH_FORCEINLINE _TypeDst	checked_cast(const _TypeSrc ptr)
{
	HH_STATIC_ASSERT(TTypeTraits::IsPointer<_TypeDst>::True && TTypeTraits::IsPointer<_TypeSrc>::True);
#if	(PK_HAS_RTTI != 0)
	HH_ASSERT(ptr == null || dynamic_cast<_TypeDst>(ptr) != null);
#endif
	return (ptr != null) ? static_cast<_TypeDst>(ptr) : null;
}

//----------------------------------------------------------------------------
// use to cast, checking that there is no loss of information

template<typename _TypeDst, typename _TypeSrc>
HH_FORCEINLINE typename TType_StripQual<_TypeDst>::Type	checked_numcast(const _TypeSrc &value)
{
	typedef typename TType_StripQual<_TypeDst>::Type	DstType;
	const DstType	result = static_cast<DstType>(value);
	HH_ASSERT(static_cast<typename TType_StripQual<_TypeSrc>::Type>(result) == value);
	return result;
}

//----------------------------------------------------------------------------
// bit-level cast (ex: replaces *((int*)&floatValue), without messing up pointer aliasing)

template<typename _TypeDst, typename _TypeSrc>
HH_FORCEINLINE typename TType_StripQual<_TypeDst>::Type		bit_cast(const _TypeSrc &value)
{
	typedef typename TType_StripQual<_TypeDst>::Type		DstType;
	typedef typename TType_StripQual<_TypeSrc>::Type		SrcType;
	HH_STATIC_ASSERT(sizeof(DstType) == sizeof(SrcType));
	union
	{
		SrcType		a;
		DstType		b;
	}	caster;
	caster.a = value;
	return caster.b;
}

//----------------------------------------------------------------------------
#endif	/* __HH_DEFS_H__ */
