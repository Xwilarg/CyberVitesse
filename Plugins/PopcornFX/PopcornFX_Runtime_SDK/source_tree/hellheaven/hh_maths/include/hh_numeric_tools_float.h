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

#ifndef __PK_NUMERIC_TOOLS_FLOAT_H__
#define __PK_NUMERIC_TOOLS_FLOAT_H__

#include "hh_maths/include/hh_numeric_traits_float.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	NOTE: Old, avoid using those, will codegen to potentially horrible LHS-y code
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE typename TNumericTraits<_Type>::SelfFpTypeBits		*FpBits(_Type &fp)
{
	return reinterpret_cast<typename TNumericTraits<_Type>::SelfFpTypeBits*>(&fp);
}

template<typename _Type>
HH_FORCEINLINE typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	*FpBitsSigned(_Type &fp)
{
	return reinterpret_cast<typename TNumericTraits<_Type>::SelfFpTypeBitsSigned*>(&fp);
}

template<typename _Type>
HH_FORCEINLINE typename TNumericTraits<_Type>::SelfFpTypeBits		FpToBits(_Type fp)
{
	return *reinterpret_cast<typename TNumericTraits<_Type>::SelfFpTypeBits*>(&fp);
//	return bit_cast<typename TNumericTraits<_Type>::SelfFpTypeBits>(fp);
}

template<typename _Type>
HH_FORCEINLINE _Type												FpFromBits(typename TNumericTraits<_Type>::SelfFpTypeBits bits)
{
	return *reinterpret_cast<_Type*>(&bits);
//	return bit_cast<_Type>(bits);
}

template<typename _Type>
HH_FORCEINLINE void													FpFromBits(_Type &fp, typename TNumericTraits<_Type>::SelfFpTypeBits bits)
{
	fp = *reinterpret_cast<_Type*>(&bits);
//	fp = bit_cast<_Type>(bits);
}

template<typename _Type>	// converts float bits to two complement representation
HH_FORCEINLINE typename TNumericTraits<_Type>::SelfFpTypeBits		FpBitsTo2C(_Type &fpval)
{
	typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	bits = bit_cast<typename TNumericTraits<_Type>::SelfFpTypeBits>(fpval);
	if (bits < 0)
		bits = TNumericTraits<_Type>::FpSignMask - bits;
	return bits;
}

//----------------------------------------------------------------------------
//
//	Floating-point specific tools
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TFpTools
{
public:
	HH_STATIC_ASSERT(TTypeTraits::IsFloat<_Type>::True);

	typedef typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	FpTypeBitsSigned;
	typedef typename TNumericTraits<_Type>::SelfFpTypeBits			FpTypeBitsUnsigned;
	template<typename _Operator, FpTypeBitsSigned _UlpsUpper, FpTypeBitsSigned _UlpsLower, bool _OptimizeForNoEquality = false>
	struct	TOp
	{
		static bool	Eval(_Type u, _Type v);
	};

	template<FpTypeBitsSigned _UlpsUpper, FpTypeBitsSigned _UlpsLower>
	static bool		UlpsGreater(_Type u, _Type v) { return TOp<TTypeOpGreater<_Type>, _UlpsUpper, _UlpsLower, true>::Eval(u, v); }
	template<FpTypeBitsSigned _UlpsUpper, FpTypeBitsSigned _UlpsLower>
	static bool		UlpsLower(_Type u, _Type v) { return TOp<TTypeOpLower<_Type>, _UlpsUpper, _UlpsLower, true>::Eval(u, v); }
	template<FpTypeBitsSigned _UlpsUpper, FpTypeBitsSigned _UlpsLower>
	static bool		UlpsGreaterOrEqual(_Type u, _Type v) { return !UlpsLower<_UlpsUpper, _UlpsLower>(u, v); }
	template<FpTypeBitsSigned _UlpsUpper, FpTypeBitsSigned _UlpsLower>
	static bool		UlpsLowerOrEqual(_Type u, _Type v) { return !UlpsGreater<_UlpsUpper, _UlpsLower>(u, v); }

	template<FpTypeBitsSigned _UlpsUpper, FpTypeBitsSigned _UlpsLower>
	static bool		UlpsEqual(_Type u, _Type v)
	{
		FpTypeBitsSigned	ulpDelta = FpBitsTo2C(v) - FpBitsTo2C(u);
		return (ulpDelta >= -_UlpsLower && ulpDelta <= _UlpsUpper);
	}

	template<FpTypeBitsSigned _Ulps>
	static bool		UlpsGreater(_Type u, _Type v) { return UlpsGreater<_Ulps, _Ulps>(u, v); }
	template<FpTypeBitsSigned _Ulps>
	static bool		UlpsLower(_Type u, _Type v) { return UlpsLower<_Ulps, _Ulps>(u, v); }
	template<FpTypeBitsSigned _Ulps>
	static bool		UlpsGreaterOrEqual(_Type u, _Type v) { return UlpsGreaterOrEqual<_Ulps, _Ulps>(u, v); }
	template<FpTypeBitsSigned _Ulps>
	static bool		UlpsLowerOrEqual(_Type u, _Type v) { return UlpsLowerOrEqual<_Ulps, _Ulps>(u, v); }
	template<FpTypeBitsSigned _Ulps>
	static bool		UlpsEqual(_Type u, _Type v) { return UlpsEqual<_Ulps, _Ulps>(u, v); }

	template<FpTypeBitsSigned _Ulps>
	static _Type	Ulps(_Type value);

	HH_FORCEINLINE static _Type	UlpsUpperError(_Type value) { return Ulps<_Type, +1>(value); }
	HH_FORCEINLINE static _Type	UlpsLowerError(_Type value) { return Ulps<_Type, -1>(value); }
};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_maths/include/hh_numeric_tools_float.inl"

#endif // __PK_NUMERIC_TOOLS_FLOAT_H__
