#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2007/11/14 17:28 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_NUMERIC_TRAITS_FLOAT_H__
#define __PK_NUMERIC_TRAITS_FLOAT_H__

#include "hh_maths/include/hh_numeric_tools_float.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Generic IEEE-754-style fp constants.
//	variable number of mantissa and exponent bits, assumes 1-bit sign,
//	and the following, packed layout:
//
//	bit pattern
//	 S E M
//	NB   0
//	with NB = E + M + 1
//
//----------------------------------------------------------------------------

template<typename _MaskType, hh_u32 _MantissaBitCount, hh_u32 _ExponentBitCount>
struct	TAbstractIEEE754_FpConstants
{
	typedef typename TNumericTraits<_MaskType>::UnsignedType	SelfFpTypeBits;
	typedef typename TNumericTraits<_MaskType>::SignedType		SelfFpTypeBitsSigned;

	enum
	{
		FpMantissaBitCount	= _MantissaBitCount,
		FpExponentBitCount	= _ExponentBitCount,
		FpExponentShift		= FpMantissaBitCount,
		FpSignShift			= FpMantissaBitCount + FpExponentBitCount,
	};

	enum
#if	!defined(HH_COMPILER_GCC) || (GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4, 4, 0))
		: _MaskType
#endif
	{
		FpMantissaMask	= ((_MaskType)1 << FpMantissaBitCount) - 1,							// 0x007FFFFF | 0x000FFFFFFFFFFFFFULL
		FpExponentMask	= (((_MaskType)1 << FpExponentBitCount) - 1) << FpExponentShift,	// 0x7F800000 | 0x7FF0000000000000ULL
		FpSignMask		= ((_MaskType)1 << FpSignShift),									// 0x80000000 | 0x8000000000000000ULL
		FpPositiveExp	= ((_MaskType)1 << (FpExponentBitCount - 1)) - 1,					// 0x0000007F | 0x00000000000003FFULL

		FpBits_One			= FpPositiveExp << FpExponentShift,
		FpBits_PositiveInf	= FpExponentMask,
		FpBits_NegativeInf	= FpExponentMask | FpSignMask,
		FpBits_FirstQNaN	= (_MaskType)FpBits_PositiveInf + 1,
	};
};

//----------------------------------------------------------------------------

template<typename _Type, typename _FpLayout>
struct	TAbstractFloatingPointRepresentation : public _FpLayout
{
	typedef	_Type		SelfType;

	static const _Type	kPositiveInfinity;
	static const _Type	kNegativeInfinity;
	static const _Type	kNaN;
	static const _Type	kMax;
	static const _Type	kMin;
	static const _Type	kSmallest;
	static const _Type	kSmallestDenormal;

	static SelfType		PositiveInfinity() { return FpFromBits<SelfType>(_FpLayout::FpBits_PositiveInf); }	// LHS ! use kPositiveInfinity if possible (watchout for static initialization order if you do)
	static SelfType		NegativeInfinity() { return FpFromBits<SelfType>(_FpLayout::FpBits_NegativeInf); }	// LHS ! use kNegativeInfinity if possible (watchout for static initialization order if you do)
	static SelfType		Infinity() { return PositiveInfinity(); }			// alias for 'PositiveInfinity()'. should we keep this?
	static SelfType		NaN() { return FpFromBits<SelfType>(_FpLayout::FpBits_FirstQNaN); }
	static SelfType		InvalidDebugInitializer() { return FpFromBits<SelfType>(_FpLayout::FpBits_FirstQNaN); }

	static SelfType		Max() { return FpFromBits<_Type>(_FpLayout::FpBits_PositiveInf - 1); }								// largest representable value before switching to infinity (~ 3.4028235e+038)
	static SelfType		Min() { return FpFromBits<_Type>((_FpLayout::FpBits_PositiveInf - 1) | _FpLayout::FpSignMask); }	// largest representable negative value before switching to -infinity (~ -3.4028235e+038)
	static SelfType		Smallest() { return FpFromBits<_Type>((~(static_cast<typename _FpLayout::SelfFpTypeBits>(0)) & ~(_FpLayout::FpExponentMask | _FpLayout::FpSignMask)) + 1); }					// smallest representable value before switching to denormals (~ 1.1754944e-038)
	static SelfType		SmallestDenormal() { return FpFromBits<_Type>(1); }	// smallest representable value before switching to 0.0f (~ 1.4012985e-045)

	static void			SetPositiveInfinity(SelfType &dst) { *FpBits(dst) = _FpLayout::FpBits_PositiveInf; }
	static void			SetNegativeInfinity(SelfType &dst) { *FpBits(dst) = _FpLayout::FpBits_NegativeInf; }
	static void			SetNaN(SelfType &dst) { *FpBits(dst) = _FpLayout::FpBits_FirstQNaN; }
	static void			SetInvalidDebug(SelfType &dst) { *FpBits(dst) = _FpLayout::FpBits_FirstQNaN; }

	// note: not very efficient, but we probably don't care that much...
	static bool			IsInfinite(SelfType value) { return *FpBits(value) == _FpLayout::FpBits_PositiveInf || *FpBits(value) == _FpLayout::FpBits_NegativeInf; }
	static bool			IsFinite(SelfType value) { return !IsInfinite(value) && !IsNan(value); }
	static bool			IsNan(SelfType value) { typename _FpLayout::SelfFpTypeBits fpb = FpToBits(value); return (fpb & ~(typename _FpLayout::SelfFpTypeBits)_FpLayout::FpSignMask) > _FpLayout::FpBits_PositiveInf; }
	static bool			IsDenormal(SelfType value) { return value != 0 && (FpToBits(value) & _FpLayout::FpExponentMask) == 0; }
	static bool			HasPerfPenalty(SelfType value) { return IsNan(value) || IsDenormal(value); }	// IsStandard() ? IsNormal() ?

	// return (value < 0) ? -1 : 1;
	static SelfType		Sign(SelfType value) { return FpFromBits<SelfType>(_FpLayout::FpBits_One | (FpToBits(value) & _FpLayout::FpSignMask)); }	// LHS !!!
	// return (signSource < 0) ? -abs(value) : abs(value);
	static SelfType		SetSign(SelfType value, SelfType signSource) { return FpFromBits<SelfType>((FpToBits(value) & ~_FpLayout::FpSignMask) | (FpToBits(signSource) & _FpLayout::FpSignMask)); }	// LHS !!!
	// return (signSource < 0) ? -value : value;
	static SelfType		MulSign(SelfType value, SelfType signSource) { return FpFromBits<SelfType>(FpToBits(value) ^ (FpToBits(signSource) & _FpLayout::FpSignMask)); }	// LHS !!!
};

//----------------------------------------------------------------------------

template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kPositiveInfinity = PositiveInfinity();
template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kNegativeInfinity = NegativeInfinity();
template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kNaN = NaN();

template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kMax = Max();
template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kMin = Min();
template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kSmallest = Smallest();
template<typename _Type, typename _FpLayout>
const _Type		TAbstractFloatingPointRepresentation<_Type, _FpLayout>::kSmallestDenormal = SmallestDenormal();

//----------------------------------------------------------------------------
//
//	IEEE-754 32-bits floating-point numbers
//
//	bit pattern
//	 1     8               23
//	 S EEEEEEEE MMMMMMMMMMMMMMMMMMMMMMM
//	31 30    23 22                    0
//----------------------------------------------------------------------------

template<>
class	TNumericTraits<float> : public CGenericNumericTraits<float>, public TNumericTypeQuality<50>, public TAbstractFloatingPointRepresentation<float, TAbstractIEEE754_FpConstants<hh_u32, 23, 8> >
{
public:
	typedef	float		SelfType;

	static HH_FORCEINLINE SelfType	Zero() { return 0.0f; }
	static HH_FORCEINLINE SelfType	One() { return 1.0f; }
};

//----------------------------------------------------------------------------

template<>
class	TNumericConstants<float>
{
public:
	typedef	float		SelfType;

	static HH_FORCEINLINE SelfType	Pi() { return 3.1415926535898f; }
	static HH_FORCEINLINE SelfType	PiTwo() { return 6.28318530717f; }
	static HH_FORCEINLINE SelfType	PiHalf() { return 1.57079632679f; }

	static HH_FORCEINLINE SelfType	Ln2() { return 0.693147180559945f; }
	static HH_FORCEINLINE SelfType	Ln05() { return -0.693147180559945f; }
	static HH_FORCEINLINE SelfType	Sqrt2() { return 1.41421356237f; }
	static HH_FORCEINLINE SelfType	GoldenRatio() { return 1.61803398874f; }
};

//----------------------------------------------------------------------------
//
//	IEEE-754 64-bits floating-point numbers
//
//	bit pattern
//	 1     11                             52
//	 S EEEEEEEEEEE MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//	63 62       52 51                                                 0
//
//----------------------------------------------------------------------------

template<>
class	TNumericTraits<double> : public CGenericNumericTraits<double>, public TNumericTypeQuality<60>, public TAbstractFloatingPointRepresentation<double, TAbstractIEEE754_FpConstants<hh_u64, 52, 11> >
{
public:
	typedef	double		SelfType;

	static HH_FORCEINLINE SelfType	Zero() { return 0.0; }
	static HH_FORCEINLINE SelfType	One() { return 1.0; }
};

//----------------------------------------------------------------------------

template<>
class	TNumericConstants<double>
{
public:
	typedef	double	SelfType;

	static HH_FORCEINLINE SelfType	Pi() { return 3.14159265358979323846264338327950; }
	static HH_FORCEINLINE SelfType	PiTwo() { return 6.283185307179586476925286766559; }
	static HH_FORCEINLINE SelfType	PiHalf() { return 1.5707963267948966192313216916398; }

	static HH_FORCEINLINE SelfType	Ln2() { return 0.693147180559945; }
	static HH_FORCEINLINE SelfType	Ln05() { return -0.693147180559945; }
	static HH_FORCEINLINE SelfType	Sqrt2() { return 1.4142135623730950488016887242097; }
	static HH_FORCEINLINE SelfType	GoldenRatio() { return 1.6180339887498948482045868343656; }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_NUMERIC_TRAITS_FLOAT_H__
