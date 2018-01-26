#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2010/11/11 12:40 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_NUMERIC_TOOLS_FLOAT_INL__
#define __PK_NUMERIC_TOOLS_FLOAT_INL__

#ifndef	__PK_NUMERIC_TOOLS_FLOAT_H__
#	error	hh_numeric_tools_float.inl must not be included manually, include hh_numeric_tools_float.h instead
#endif

// NOTE: in this file, we switch between various implementations at compile time,
// and we assume that if HH_WIN32 is defined, we're building with MSVC, and its intrinsics.
// we probably should make better checks... and switch based on what all the used intrinsics depend on: the compiler

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Internal helpers
//
//----------------------------------------------------------------------------

namespace	Internal
{
	// default: _Ulps > 0
	template<typename _Type, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _Ulps, bool _Negative = (_Ulps < 0), bool _Zero = (_Ulps == 0)>
	struct	TUlpsHelper
	{
		HH_STATIC_ASSERT(_Negative == false);
		HH_STATIC_ASSERT(_Zero == false);
		HH_STATIC_ASSERT(_Ulps > 0);

		static _Type	Ulps(_Type value)
		{
			typename TNumericTraits<_Type>::SelfFpTypeBits				absValue = FpToBits(value) & ~TNumericTraits<_Type>::FpSignMask;
			static const typename TNumericTraits<_Type>::SelfFpTypeBits	kThreshold = TNumericTraits<_Type>::FpBits_PositiveInf - _Ulps;
			if (absValue > kThreshold)
				return TNumericTraits<_Type>::Zero();
			return FpFromBits<_Type>(absValue + _Ulps) - FpFromBits<_Type>(absValue);	// LHS !!!
		}
	};

	// _Ulps == 0
	template<typename _Type, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _Ulps>
	struct	TUlpsHelper<_Type, _Ulps, false, true>
	{
		HH_STATIC_ASSERT(_Ulps == 0);
		static _Type	Ulps(_Type value)
		{
			return TNumericTraits<_Type>::Zero();
		}
	};

	// _Ulps < 0
	template<typename _Type, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _Ulps>
	struct	TUlpsHelper<_Type, _Ulps, true, false>
	{
		HH_STATIC_ASSERT(_Ulps < 0);
		static _Type	Ulps(_Type value)
		{
			typename TNumericTraits<_Type>::SelfFpTypeBits				absValue = FpToBits(value) & ~TNumericTraits<_Type>::FpSignMask;
			static const typename TNumericTraits<_Type>::SelfFpTypeBits	kThreshold = -_Ulps;
			if (absValue < kThreshold)
				return TNumericTraits<_Type>::Zero();
			return FpFromBits<_Type>(absValue + _Ulps) - FpFromBits<_Type>(absValue);	// LHS !!!
		}
	};
}

//----------------------------------------------------------------------------
//
//	TFpTools
//
//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Operator, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _UlpsUpper, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _UlpsLower>
struct	TFpTools<_Type>::TOp<_Operator, _UlpsUpper, _UlpsLower, false>
{
	static bool	Eval(_Type u, _Type v)
	{
		// grab the floating point numbers in a two-complement ulp form, and subtract them from each other to get their delta in ulps
		typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	ulpDelta = FpBitsTo2C(v) - FpBitsTo2C(u);
		if (ulpDelta > -_UlpsLower && ulpDelta < _UlpsUpper)
			ulpDelta = 0;
		return _Operator::Eval(0, ulpDelta);
	}
};

template<typename _Type>
template<typename _Operator, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _UlpsUpper, typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _UlpsLower>
struct	TFpTools<_Type>::TOp<_Operator, _UlpsUpper, _UlpsLower, true>
{
	static bool	Eval(_Type u, _Type v)
	{
		// grab the floating point numbers in a two-complement ulp form, and subtract them from each other to get their delta in ulps
		typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	ulpDelta = FpBitsTo2C(v) - FpBitsTo2C(u);
		return ((ulpDelta < -_UlpsLower || ulpDelta > _UlpsUpper) && _Operator::Eval(0, ulpDelta));
	}
};

//----------------------------------------------------------------------------

template<typename _Type>
template<typename TNumericTraits<_Type>::SelfFpTypeBitsSigned _Ulps>
HH_FORCEINLINE _Type	TFpTools<_Type>::Ulps(_Type value)
{
	return Internal::TUlpsHelper<_Type, _Ulps>::Ulps(value);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_NUMERIC_TOOLS_FLOAT_INL__
