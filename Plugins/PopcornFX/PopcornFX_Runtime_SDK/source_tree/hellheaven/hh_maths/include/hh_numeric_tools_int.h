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

#ifndef __PK_NUMERIC_TOOLS_INT_H__
#define __PK_NUMERIC_TOOLS_INT_H__

#include "hh_maths/include/hh_numeric_traits_int.h"

#if	defined(HH_PS3)
#	if	HH_CPU_MODEL == HH_CPU_CELL_SPU
#		include <spu_intrinsics.h>
#	else
#		include <ppu_intrinsics.h>
#	endif
#elif	defined(HH_ORBIS)
#elif	defined(HH_PSP2)
#elif	defined(HH_ANDROID)
#elif	defined(HH_X360)
#	include <PPCIntrinsics.h>
#elif	defined(HH_MACOSX) || defined(HH_LINUX) || defined(HH_IOS)
// ???
#elif	defined(HH_NX)
#else
#	include <intrin.h>
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	static-template integer tools
//
//----------------------------------------------------------------------------

namespace	TIntegerTemplateTools
{
	template<hh_u32 _Value>	struct					IsZero				: FalseType {};
	template<>				struct					IsZero<0>			: TrueType {};
	template<hh_u32 _Value>	struct					IsNonZero			: TTypeBoolConstant<!IsZero<_Value>::True> {};
	template<hh_u32 _Value>	struct					IsPowerOfTwo		: TTypeBoolConstant<IsNonZero<_Value>::True && IsZero<_Value & (_Value - 1)>::True> {};
	template<hh_u32 _Align, hh_u32 _Value>	struct	IsAligned			: TTypeBoolConstant<IsZero<_Value & (_Align - 1)>::True> { HH_STATIC_ASSERT(IsPowerOfTwo<_Align>::True); };
	template<hh_u32 _Value>	struct					IsAligned16			: IsAligned<0x10, _Value> {};

	template<hh_u32 _Align, hh_u32 _Value>	struct	Align				{ HH_STATIC_ASSERT(IsPowerOfTwo<_Align>::True); static const hh_u32	Value = (_Value + (_Align - 1)) & ~(_Align - 1); };

	// Logk<x,b>
	template<hh_u32 _Value, hh_u32 _Base> struct	LogkImpl			{ static const hh_i32	Value = 1 + LogkImpl<(_Value / _Base), _Base>::Value; };
	template<hh_u32 _Base> struct					LogkImpl<1, _Base>	{ static const hh_i32	Value = 0; };
	template<hh_u32 _Base> struct					LogkImpl<0, _Base>	{ static const hh_i32	Value = -1; };	// avoid infinite recursion above, even if undefined

	template<hh_u32 _Value, hh_u32 _Base> struct	Logk				: private LogkImpl<_Value, _Base>	{ static const hh_u32	Value = LogkImpl<_Value, _Base>::Value; };
	template<hh_u32 _Base> struct					Logk<0, _Base>											{ static const hh_u32	Value = 0; };

	// Log2<x>
	template<hh_u32 _Value> struct					Log2				: public Logk<_Value, 2> {};
//	template<hh_u32 _Value> struct					Log2<_Value>		: public Logk<_Value & 0xFFFFFFFFULL, 2> {};	// prevent stupid sign-extension

	// Log10<x>
	template<hh_u32 _Value> struct					Log10				: public Logk<_Value, 10> {};
//	template<hh_u32 _Value> struct					Log10<_Value>		: public Logk<_Value & 0xFFFFFFFFULL, 10> {};	// prevent stupid sign-extension

	// returns a value with all bits set. usage : AllSet<_Type>::Value
	template<typename _Type> struct					AllSet { HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True); static const _Type Value = _Type(~_Type(0)); };

	// power of two
	template<typename _Type, _Type _Value, hh_u32 _It, hh_u32 _UnprocessedBits>
	struct	NextOrEqualPowerOfTwo_Calc
	{
		static const _Type	Value = NextOrEqualPowerOfTwo_Calc<_Type, _Value | (_Value >> _It), _It + _It, _UnprocessedBits - _It>::Value;
	};
	template<typename _Type, _Type _Value, hh_u32 _It>
	struct	NextOrEqualPowerOfTwo_Calc<_Type, _Value, _It, 0>
	{
		static const _Type	Value = _Value;
	};
	template<typename _Type, _Type _Value>
	struct	NextOrEqualPowerOfTwo_Impl
	{
		static const _Type	Value = NextOrEqualPowerOfTwo_Calc<_Type, _Value - 1, 1, sizeof(_Type) * 8 - 1>::Value + 1;
	};

	template<hh_u32 _Value>	struct	NextOrEqualPowerOfTwo : NextOrEqualPowerOfTwo_Impl<hh_u32, _Value> {};
	template<hh_u32 _Value>	struct	NextPowerOfTwo : NextOrEqualPowerOfTwo_Impl<hh_u32, _Value + 1> {};

	// Max<a,b>
	template<hh_u32 _Value0,
			 hh_u32 _Value1,
			 hh_u32 _Value2 = _Value1,
			 hh_u32 _Value3 = _Value2,
			 hh_u32 _Value4 = _Value3,
			 hh_u32 _Value5 = _Value4>
	struct	Max
	{
#if 1
		// this one has less recursions on the compiler-side
		static const hh_u32	Value = Max<Max<Max<_Value0, _Value1>::Value,
											Max<_Value2, _Value3>::Value>::Value,
											Max<_Value4, _Value5>::Value>::Value;
#else
		static const hh_u32	Value = Max<Max<_Value0, _Value1, _Value2, _Value3, _Value4>::Value, _Value5>::Value;
#endif
	};

	template<hh_u32 _Value0, hh_u32 _Value1> struct	Max<_Value0, _Value1, _Value1, _Value1, _Value1, _Value1> { static const hh_u32	Value = _Value0 > _Value1 ? _Value0 : _Value1; };
	template<hh_u32 _Value0, hh_u32 _Value1, hh_u32 _Value2> struct	Max<_Value0, _Value1, _Value2, _Value2, _Value2, _Value2> { static const hh_u32	Value = Max<Max<_Value0, _Value1>::Value, _Value2>::Value; };
	template<hh_u32 _Value0, hh_u32 _Value1, hh_u32 _Value2, hh_u32 _Value3> struct	Max<_Value0, _Value1, _Value2, _Value3, _Value3, _Value3> { static const hh_u32	Value = Max<Max<_Value0, _Value1>::Value, Max<_Value2, _Value3>::Value>::Value; };
	template<hh_u32 _Value0, hh_u32 _Value1, hh_u32 _Value2, hh_u32 _Value3, hh_u32 _Value4> struct	Max<_Value0, _Value1, _Value2, _Value3, _Value4, _Value4> { static const hh_u32	Value = Max<Max<Max<_Value0, _Value1>::Value, Max<_Value2, _Value3>::Value>::Value, _Value4>::Value; };

	// Min<a,b>
	template<hh_u32 _Value0,
			 hh_u32 _Value1,
			 hh_u32 _Value2 = _Value1,
			 hh_u32 _Value3 = _Value2,
			 hh_u32 _Value4 = _Value3,
			 hh_u32 _Value5 = _Value4>
	struct	Min
	{
#if 1
		// this one has less recursions on the compiler-side
		static const hh_u32	Value = Min<Min<Min<_Value0, _Value1>::Value,
											Min<_Value2, _Value3>::Value>::Value,
											Min<_Value4, _Value5>::Value>::Value;
#else
		static const hh_u32	Value = Min<Min<_Value0, _Value1, _Value2, _Value3, _Value4>::Value, _Value5>::Value;
#endif
	};

	template<hh_u32 _Value0, hh_u32 _Value1>
	struct	Min<_Value0, _Value1, _Value1, _Value1, _Value1, _Value1> { static const hh_u32	Value = _Value0 < _Value1 ? _Value0 : _Value1; };
	template<hh_u32 _Value0, hh_u32 _Value1, hh_u32 _Value2>
	struct	Min<_Value0, _Value1, _Value2, _Value2, _Value2, _Value2> { static const hh_u32	Value = Min<Min<_Value0, _Value1>::Value, _Value2>::Value; };
	template<hh_u32 _Value0, hh_u32 _Value1, hh_u32 _Value2, hh_u32 _Value3>
	struct	Min<_Value0, _Value1, _Value2, _Value3, _Value3, _Value3> { static const hh_u32	Value = Min<Min<_Value0, _Value1>::Value, Min<_Value2, _Value3>::Value>::Value; };
	template<hh_u32 _Value0, hh_u32 _Value1, hh_u32 _Value2, hh_u32 _Value3, hh_u32 _Value4>
	struct	Min<_Value0, _Value1, _Value2, _Value3, _Value4, _Value4> { static const hh_u32	Value = Min<Min<Min<_Value0, _Value1>::Value, Min<_Value2, _Value3>::Value>::Value, _Value4>::Value; };

	// Sqrt<x>
	template<hh_u32 _Value,
			 hh_u32 _BoundLo = 0,
			 hh_u32 _BoundHi = (_Value <= 1) ? _Value : Min<_Value / 2, 0xFFFF>::Value,
			 bool _Converged = (_BoundHi - _BoundLo <= 1)>
	struct	Sqrt
	{
	private:	// Binary search
		static const hh_u32	_Midpoint = (_BoundLo + _BoundHi) / 2;
		static const bool	_IsInLowPart = ((_Midpoint * _Midpoint) > _Value) ? true : false;
	public:
		static const hh_u32	Value = Sqrt<_Value, _IsInLowPart ? _BoundLo : _Midpoint,
												 _IsInLowPart ? _Midpoint : _BoundHi>::Value;
	};

	template<hh_u32 _Value, hh_u32 _BoundLo, hh_u32 _BoundHi>
	struct	Sqrt<_Value, _BoundLo, _BoundHi, true>
	{
		static const hh_u32	Value = (_BoundHi * _BoundHi > _Value) ? _BoundLo : _BoundHi;
	};
};

//----------------------------------------------------------------------------
//
//	non static-template tools
//
//----------------------------------------------------------------------------

// template<typename _Type>
namespace	IntegerTools
{
	// log2(0)	-#INF
	// log2(1)	0	2^0
	// log2(2)	1	2^1
	// log2(4)	2	2^2
	// log2(8)	3	2^3
	// ...
	// returns the index of the highest bit set, (1 << Log2(v)) is the nearest power of two lower than or equal to 'v'

	template<typename _Type> static hh_u32	Log2(_Type v);
	template<typename _Type> static hh_u32	Log2Nz(_Type v);
	static hh_u32							Log2(hh_u32 v);
	static hh_u32							Log2Nz(hh_u32 v);

#if	defined(HH_CPU_64BITS)
	static hh_u64							Log2(hh_u64 v);
	static hh_u64							Log2Nz(hh_u64 v);
#endif

	static hh_u32							Log10(hh_u32 v);
	static hh_u64							Log10(hh_u64 v);

	static hh_u32							Log10FromLog2_32(hh_u32 l2);
	static hh_u64							Log10FromLog2_64(hh_u32 l2);

	template<typename _Type> static bool	IsPowerOfTwoNz(_Type value);
	template<typename _Type> static bool	IsPowerOfTwo(_Type value);

	//----------------------------------------------------------------------------
	// NextOrEqualPowerOfTwo				NextPowerOfTwo
	// 0 -> 0		5 -> 8		...			0 -> 1		5 -> 8		...
	// 1 -> 1		6 -> 8					1 -> 2		6 -> 8
	// 2 -> 2		7 -> 8					2 -> 4		7 -> 8
	// 3 -> 4		8 -> 8					3 -> 4		8 -> 16
	// 4 -> 4		9 -> 16					4 -> 8		9 -> 16
	//----------------------------------------------------------------------------

	template<typename _Type> static _Type	NextOrEqualPowerOfTwo(_Type value);
	template<typename _Type> static _Type	NextPowerOfTwo(_Type value);

	//static _Type	NearestGreaterPowerOf2(_Type value) { return 0; }
	//...

	// 'from' and 'to' are the (inclusive) bit-indices of the first and last bit to extract.
	// the result is shifted right so that the 'from' bit becomes bit 0 of the result.
	template<typename _Type> static _Type	ExtractBits(_Type v, hh_u32 from, hh_u32 to);

	template<typename _Type> static _Type	CountSetBits(_Type value);
	static hh_u32							CountSetBits(hh_u32 value);

	static hh_u32							CountTrailingZeroes(hh_u32 value);
	static hh_u32							CountTrailingZeroes(hh_u64 value);
	static hh_u32							CountLeadingZeroes(hh_u32 value);
	static hh_u32							CountLeadingZeroes(hh_u64 value);
	static hh_u32							CountTrailingOnes(hh_u32 value);
	static hh_u32							CountTrailingOnes(hh_u64 value);
	static hh_u32							CountLeadingOnes(hh_u32 value);
	static hh_u32							CountLeadingOnes(hh_u64 value);

	static hh_u8							ReverseBytes(hh_u8 value);	// 0xAB -> 0xAB
	static hh_u16							ReverseBytes(hh_u16 value);	// 0xAABB -> 0xBBAA
	static hh_u32							ReverseBytes(hh_u32 value);	// 0xAABBCCDD -> 0xDDCCBBAA
	static hh_u64							ReverseBytes(hh_u64 value);	// 0xAABBCCDDEEFFGGHH -> 0xHHGGFFEEDDCCBBAA
	// for a 128bit-wide version, see the simd_vector headers

	static hh_u8							LoadEndianSwapped(const hh_u8 *src);	// 0xAB -> 0xAB
	static hh_u16							LoadEndianSwapped(const hh_u16 *src);	// 0xAABB -> 0xBBAA
	static hh_u32							LoadEndianSwapped(const hh_u32 *src);	// 0xAABBCCDD -> 0xDDCCBBAA
	static hh_u64							LoadEndianSwapped(const hh_u64 *src);	// 0xAABBCCDDEEFFGGHH -> 0xHHGGFFEEDDCCBBAA
	// for a 128bit-wide version, see the simd_vector headers

	static void								StoreEndianSwapped(const hh_u8 value, hh_u8 *dst);		// 0xAB -> 0xAB
	static void								StoreEndianSwapped(const hh_u16 value, hh_u16 *dst);	// 0xAABB -> 0xBBAA
	static void								StoreEndianSwapped(const hh_u32 value, hh_u32 *dst);	// 0xAABBCCDD -> 0xDDCCBBAA
	static void								StoreEndianSwapped(const hh_u64 value, hh_u64 *dst);	// 0xAABBCCDDEEFFGGHH -> 0xHHGGFFEEDDCCBBAA
	// for a 128bit-wide version, see the simd_vector headers

	// bit rotations
	template<hh_u32 _N> static hh_u8		RotateBitsLeft(hh_u8 value);
	template<hh_u32 _N> static hh_u8		RotateBitsRight(hh_u8 value);
	static hh_u8							RotateBitsLeft(hh_u8 value, hh_u32 n);
	static hh_u8							RotateBitsRight(hh_u8 value, hh_u32 n);

	template<hh_u32 _N> static hh_u16		RotateBitsLeft(hh_u16 value);
	template<hh_u32 _N> static hh_u16		RotateBitsRight(hh_u16 value);
	static hh_u16							RotateBitsLeft(hh_u16 value, hh_u32 n);
	static hh_u16							RotateBitsRight(hh_u16 value, hh_u32 n);

	template<hh_u32 _N> static hh_u32		RotateBitsLeft(hh_u32 value);
	template<hh_u32 _N> static hh_u32		RotateBitsRight(hh_u32 value);
	static hh_u32							RotateBitsLeft(hh_u32 value, hh_u32 n);
	static hh_u32							RotateBitsRight(hh_u32 value, hh_u32 n);

#if	(HH_CPU_ARCH == HH_CPU_X86)
	// use only if >= SSE4.2
	static hh_u32							POPCNT32(hh_u32 value);

	// should be safe everywhere
	static hh_u32							BTComplement(hh_u32 value, hh_u32 i);
	static hh_u32							BTSet(hh_u32 value, hh_u32 i);
	static hh_u32							BTReset(hh_u32 value, hh_u32 i);

#if	(HH_CPU_MODEL == HH_CPU_X64)
	static hh_u64							BTComplement(hh_u64 value, hh_u32 i);
	static hh_u64							BTSet(hh_u64 value, hh_u32 i);
	static hh_u64							BTReset(hh_u64 value, hh_u32 i);
#endif

#endif

}

//----------------------------------------------------------------------------
//
//	templated bit-extraction
//	_From:	first bit to extract (inclusive)
//	_To:	last bit to extract (inclusive)
//
//	this matches the norm used by Intel in their tech docs.
//
//----------------------------------------------------------------------------

template<hh_u32 _From, hh_u32 _To = _From>
struct	TBitExtractor
{
	template<typename _Type>
	HH_FORCEINLINE static typename TNumericTraits<_Type>::UnsignedType	ExtractBits(_Type v)
	{
		HH_STATIC_ASSERT(_To < 8 * sizeof(_Type));
		HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
		enum { kExtractMask = (((typename TNumericTraits<_Type>::UnsignedType)(1)) << (_To - _From + 1)) - 1 };
		return (((typename TNumericTraits<_Type>::UnsignedType)v) >> _From) & kExtractMask;
	}
};

//----------------------------------------------------------------------------
//
//	integer bit-shift helper: accepts negative shift values, and converts them to the correct positive shift direction
//
//----------------------------------------------------------------------------

template<hh_i32 _ShiftCount, bool _NegativeShift = (_ShiftCount < 0)>
struct	TIntegerShifter
{
	template<typename _IType>
	HH_FORCEINLINE static _IType	ShiftLeft(_IType v) { return v << _ShiftCount; }
	template<typename _IType>
	HH_FORCEINLINE static _IType	ShiftRight(_IType v) { return v >> _ShiftCount; }
};

//----------------------------------------------------------------------------

template<hh_i32 _ShiftCount>
struct	TIntegerShifter<_ShiftCount, true>
{
	template<typename _IType>
	HH_FORCEINLINE static _IType	ShiftLeft(_IType v) { return TIntegerShifter<-_ShiftCount>::ShiftRight(v); }
	template<typename _IType>
	HH_FORCEINLINE static _IType	ShiftRight(_IType v) { return TIntegerShifter<-_ShiftCount>::ShiftLeft(v); }
};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_maths/include/hh_numeric_tools_int.inl"

#endif // __PK_NUMERIC_TOOLS_INT_H__
