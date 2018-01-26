#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2010/09/19 20:23 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_NUMERIC_TOOLS_INT_INL__
#define __PK_NUMERIC_TOOLS_INT_INL__

#ifndef	__PK_NUMERIC_TOOLS_INT_H__
#	error	hh_numeric_tools_int.inl must not be included manually, include hh_numeric_tools_int.h instead
#endif

// NOTE: in this file, we switch between various implementations at compile time,
// and we assume that if HH_WIN32 is defined, we're building with MSVC, and its intrinsics.
// we probably should make better checks... and switch based on what all the used intrinsics depend on: the compiler

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	IntegerTools
{
	namespace	Internal
	{
		extern const hh_u32		kPowersOf10_32[10];
		extern const hh_u64		kPowersOf10_64[20];

		template<typename _Type, bool _Signed = TTypeTraits::IsSigned<_Type>::True>
		struct	SLog2Specializer
		{
			HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
			static hh_u32	Log2(_Type v)
			{
				hh_u32	r = 0;
				while (v >>= 1)
				{
					r++;
				}
				return r;
			}
		};

		template<typename _Type>
		struct	SLog2Specializer<_Type, true>
		{
			HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
			static hh_u32	Log2(_Type v)
			{
				hh_u32	r = 0;
				typename TNumericTraits<_Type>::UnsignedType	uv = (v < 0) ? -v : v;
				while (uv >>= 1)
				{
					r++;
				}
				return r;
			}
		};
	}
}

//----------------------------------------------------------------------------
//
//	Log2
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE hh_u32	IntegerTools::Log2(_Type v)
{
	return Internal::SLog2Specializer<_Type>::Log2(v);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE hh_u32	IntegerTools::Log2Nz(_Type v)
{
	HH_PARANOID_ASSERT(v != 0);
	HH_ASSUME(v != 0);
	return Internal::SLog2Specializer<_Type>::Log2(v);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::Log2(hh_u32 v)
{
#if defined(HH_COMPILER_MSVC) && (HH_CPU_MODEL == HH_CPU_JAGUAR)

	return (v == 0) + 31 - __lzcnt(v);

#elif	defined(HH_ASSEMBLY_FORMAT_VC) && (HH_CPU_MODEL == HH_CPU_X86)	// inline assembly doesn't work on x64 in visual studio...

	__asm
	{
		mov	eax, v	// bsr doesn't modify eax if the arg == 0
		bsr	eax, eax
	}

#elif	defined(HH_WINDOWS)

	hh_u32	l2;
	hh_u32	r = _BitScanReverse((DWORD*)&l2, v);
	return l2 & ~(r - 1);

#elif	defined(HH_X360)

	// TODO: use the proper intrinsic
	return Internal::SLog2Specializer<hh_u32>::Log2(v);

#elif	defined(HH_COMPILER_INTEL)	// linux/macos/ICC

	if (v != 0)
		v = _bit_scan_reverse(v);
	return v;

#elif	defined(HH_COMPILER_GCC)

#	if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	// computes : 31 - clz(v) - ((v == 0) ? -1 : 0)
#		if 1
			vec_char16	vv = (vec_char16)spu_promote(v, 0);
			vec_char16	zMask = si_ceqi(vv, 0);	// -1 if 0, 0 otherwise
			vec_char16	lZeroes = si_clz(vv);
			return spu_extract((vec_uint4)si_sf(zMask, si_sfi(lZeroes, 31)), 0);	// 31 - x - zMask
#		else
			// computes : ((v != 0) ? -1 : 0) - (clz(v) - 32)
			vec_char16	vv = (vec_char16)spu_promote(v, 0);
			vec_char16	zMask = si_clgti(vv, 0);	// -1 if != 0, 0 otherwise
			vec_char16	lZeroes = si_clz(vv);
			return spu_extract((vec_uint4)si_sf(si_ai(lZeroes, -32), zMask), 0);		// 31 - x - zMask
#		endif
#	elif (HH_CPU_MODEL == HH_CPU_JAGUAR) //&& defined(HH_ASSEMBLY_FORMAT_AT_T)
		return (v == 0) + 31 - __builtin_clz(v);
//		hh_u32	lg2;
//		__asm__("xor %0, %0\n"
//				"cmp $1, %1\n"
//				"adc $31, %0\n"
//				"lzcntl %1, %1\n"
//				"sub %1, %0\n"
//				: "=r"(lg2), "+r"(value) : : "cc");
//		return lg2;
#	elif (HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		__asm__("bsr %0,%0" : "+r"(v) : : "cc");
		return v;
#	else
		if (v != 0)
			v = 31 - __builtin_clz(v);
		return v;
#	endif

#elif	defined(HH_COMPILER_GHS)

	if (v != 0)
		return 31 - __cntlzw(v);
	return v;

#else
#	error	unsupported
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::Log2Nz(hh_u32 v)
{
	HH_PARANOID_ASSERT(v != 0);
	HH_ASSUME(v != 0);

#if defined(HH_COMPILER_MSVC) && (HH_CPU_MODEL == HH_CPU_JAGUAR)

	return 31 - __lzcnt(v);

#elif	defined(HH_ASSEMBLY_FORMAT_VC) && (HH_CPU_MODEL == HH_CPU_X86)	// inline assembly doesn't work on x64 in visual studio...

	__asm
	{
		bsr	eax, v
	}

#elif	defined(HH_WINDOWS)

	hh_u32	l2;
	_BitScanReverse((DWORD*)&l2, v);
	return l2;

#elif	defined(HH_COMPILER_INTEL)	// linux/macos/ICC

	return _bit_scan_reverse(v);

#elif	defined(HH_COMPILER_GCC)

#	if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
		return 31 - spu_extract(spu_cntlz(spu_promote(v, 0)), 0);
#	elif (HH_CPU_MODEL == HH_CPU_JAGUAR) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		return 31 - __builtin_clz(v);
//			hh_u32	lg2;
//			__asm__("mov $31, %0\n"
//					"lzcntl %1, %1\n"
//					"sub %1, %0\n"
//					: "=&q"(lg2) : "r"(value - 1) : "cc");
//			return lg2;
#	elif (HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		__asm__("bsr %0,%0" : "+r"(v) : : "cc");
		return v;
#	else
		return 31 - __builtin_clz(v);
#	endif

#elif	defined(HH_COMPILER_GHS)

	return 31 - __cntlzw(v);

#else
	return Internal::SLog2Specializer<hh_u32>::Log2(v);
#endif
}

//----------------------------------------------------------------------------

#if	defined(HH_CPU_64BITS)

HH_FORCEINLINE hh_u64	IntegerTools::Log2(hh_u64 v)
{
#if	defined(HH_WINDOWS)

	hh_u32	l2;
	hh_u32	r = _BitScanReverse64((DWORD*)&l2, v);
	return l2 & ~(r - 1);

#elif	defined(HH_COMPILER_INTEL)	// linux/macos/ICC

	return _bit_scan_reverse64(v);	// TODO: check this intrinsic's behavior when v == 0

#elif	defined(HH_COMPILER_GCC)

#	if (HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		__asm__("bsr %0,%0" : "+r"(v) : : "cc");
		return v;
#	else
		if (v != 0)
			v = 63 - __builtin_clzll(v);
		return v;
#	endif

#else
#	error	unsupported
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u64	IntegerTools::Log2Nz(hh_u64 v)
{
	HH_PARANOID_ASSERT(v != 0);
	HH_ASSUME(v != 0);

#if	defined(HH_WINDOWS)

	hh_u32	l2;
	_BitScanReverse64((DWORD*)&l2, v);
	return l2;

#elif	defined(HH_COMPILER_INTEL)	// linux/macos/ICC

	return _bit_scan_reverse64(v);	// TODO: check this intrinsic's behavior when v == 0

#elif	defined(HH_COMPILER_GCC)

#	if (HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		__asm__("bsr %0,%0" : "+r"(v) : : "cc");
		return v;
#	else
		return 63 - __builtin_clzll(v);
#	endif

#else
#	error	unsupported
#endif
}
#endif

//----------------------------------------------------------------------------
//
//	base-10 logarithms
//
//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::Log10(hh_u32 v)
{
	hh_u32	t = (Log2(v) + 1) * 1233 >> 12;
	return t - (v < Internal::kPowersOf10_32[t]);
}

HH_FORCEINLINE hh_u64	IntegerTools::Log10(hh_u64 v)
{
	hh_u64	t = (Log2(v) + 1) * 1233 >> 12;
	return t - (v < Internal::kPowersOf10_64[t]);
}

HH_FORCEINLINE hh_u32	IntegerTools::Log10FromLog2_32(hh_u32 l2)
{
	HH_ASSERT(l2 <= 31);
	hh_u32	t = (l2 + 1) * 1233 >> 12;
	return t - ((1UL << l2) < Internal::kPowersOf10_32[t]);
}

HH_FORCEINLINE hh_u64	IntegerTools::Log10FromLog2_64(hh_u32 l2)
{
	HH_ASSERT(l2 <= 63);
	hh_u64	t = (l2 + 1) * 1233 >> 12;
	return t - ((1ULL << l2) < Internal::kPowersOf10_64[t]);
}

//----------------------------------------------------------------------------
//
//	Is power of two
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE bool	IntegerTools::IsPowerOfTwoNz(_Type value)
{
	HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
	return (value & (value - 1)) == 0;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE bool	IntegerTools::IsPowerOfTwo(_Type value)
{
	HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
	return value != 0 && IsPowerOfTwoNz(value);
}


//----------------------------------------------------------------------------
//
//	Next powers of two
//
//----------------------------------------------------------------------------

#define	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO

//----------------------------------------------------------------------------

namespace	IntegerTools {

template<typename _Type>
HH_FORCEINLINE _Type	NextOrEqualPowerOfTwo(_Type value)
{
	HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
	value--;
	for (hh_u32 i = 1; i < sizeof(_Type) * 8; i += i)
	{
		value |= value >> i;
	}
	value++;
#ifndef	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO
	value += (value == 0);
#endif
	return value;
}

//----------------------------------------------------------------------------

template<>
HH_FORCEINLINE hh_u32	NextOrEqualPowerOfTwo(hh_u32 value)
{
#if	defined(HH_WINDOWS)

#	ifdef	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO
		// returns 0 when given 0 :  [0=>0][1=>1][2=>2]
#		if	defined(HH_ASSEMBLY_FORMAT_VC) && (HH_CPU_MODEL == HH_CPU_X86)	// inline assembly doesn't work on x64 in visual studio...
			__asm
			{
				mov		edx, value
				xor		eax, eax
				test	edx, edx
				lea		ecx, [eax-1]
				lea		edx, [edx-1]
				setnz	al
				bsr		ecx, edx
				add		eax, eax
				rol		eax, cl
			}
#		elif	(HH_CPU_MODEL == HH_CPU_JAGUAR)
			return _rotr((value != 0), __lzcnt(value - 1));
#		else
			bool	isNZ = value != 0;
			hh_u32	l2;
			if (!_BitScanReverse((DWORD*)&l2, value - 1))
				l2 = hh_u32(-1);
			return hh_u32(isNZ) << ++l2;
#		endif
#	else
		// returns 1 when given 0 : [0=>1][1=>1][2=>2]
		hh_u32	l2;
		if (!_BitScanReverse((DWORD*)&l2, value - 1))
			l2 = hh_u32(-1);
		return 1 << ++l2;
#	endif

#elif	defined(HH_COMPILER_GCC)

#	if	(HH_CPU_MODEL == HH_CPU_JAGUAR) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		// Jaguar handles lzcnt instruction -> use that.
#		ifdef	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO
			// returns 0 when given 0 :  [0=>0][1=>1][2=>2]
			// (isZero ? 0 : 1) <<< clz(value - 1)
			hh_u32	b;
			__asm__("xor %0, %0\n"
					"lzcntl %1, %%ecx\n"
					"setnz %b0\n"
					"rorl %%cl, %0\n"
					: "=&q"(b) : "r"(value - 1) : "cc", "ecx");
			return b;
#		else
			// returns 1 when given 0 : [0=>1][1=>1][2=>2]
			hh_u32	s = __builtin_clz(value-1);
			hh_u32	b = 1;
			__asm__("rorl %%cl, %0" : "+r"(b) : "c"(s) : "cc", "cl");
			return b;
#		endif
#	elif	0 // (HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		// FIXME: flutted-up
		// We can't be sure generic X86 handles lzcnt : use bsr instead
#		ifdef	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO
			// returns 0 when given 0 :  [0=>0][1=>1][2=>2]
			// (isZero ? 0 : 1) <<< clz(value - 1)
			hh_u32	b;
			__asm__("xor %0, %0\n"
					"lea -1(%0), %%ecx\n"
					"test %1, %1\n"
					"lea -1(%1), %1\n"
					"setnz %b0\n"
					"bsr %1, %%ecx\n"
					"addl %0, %0\n"
					"roll %%cl, %0\n"
					: "=&q"(b) :"r"(value) : "cc", "ecx", "1");
			return b;
#		else
			// returns 1 when given 0 : [0=>1][1=>1][2=>2]
			hh_u32	v = -1;
			__asm__("bsr %1,%0" : "+r"(v) : "r"(value - 1) : "cc");	// 9.0, [0=>1][1=>1][2=>1]
			return 1 << (++v);
#		endif
#	else
		// other arch : hope __builtin_clz() will actually map to a real intrinsic and not a function call to sone default implem...
#		ifdef	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO
			bool	isNZ = value != 0;
			hh_u32	lz = 0;
			if (value != 1)
				lz = (32 - __builtin_clz(value-1));
			return isNZ << lz;
#		else
			hh_u32	lz = 0;
			if (value != 1)
				lz = (32 - __builtin_clz(value-1));
			return 1 << lz;
//			return (0x80000000 >> (__builtin_clz(value-1)-1)); ?
#		endif
#	endif

#else

	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value++;
#	ifndef	PK_MATHS_NEQPOT_ZERO_WHEN_ZERO
		value += (value == 0);
#	endif
	return value;

#endif
}

//----------------------------------------------------------------------------

template<typename _Type>
_Type	NextPowerOfTwo(_Type value)
{
	HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
	return NextOrEqualPowerOfTwo(value + 1);
}

} // namespace	IntegerTools

//----------------------------------------------------------------------------
//_Type	IntegerTools::NearestGreaterPowerOf2(_Type value) { return 0; }
//...

//----------------------------------------------------------------------------
//
//	bit utilities
//
//----------------------------------------------------------------------------

template<typename _Type>
_Type	IntegerTools::ExtractBits(_Type v, hh_u32 from, hh_u32 to)
{
	HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
	return (((typename TNumericTraits<_Type>::UnsignedType)v) >> from) & ((TNumericTraits<typename TNumericTraits<_Type>::UnsignedType>::One() << (to - from + 1)) - 1);
}

//----------------------------------------------------------------------------

template<typename _Type>
_Type	IntegerTools::CountSetBits(_Type value)
{
	HH_STATIC_ASSERT(TTypeTraits::IsInteger<_Type>::True);
#if 1
	typename TNumericTraits<_Type>::UnsignedType	uv = (typename TNumericTraits<_Type>::UnsignedType)value;
	hh_u32	count = 0;
	while (uv != 0)
	{
		count += uv & 1;
		uv >>= 1;
	}
	return count;
#else
	value = value - ((value >> 1) & ((_Type)(AllSet() / 3)));
	value = (value & (_Type)AllSet() / 15 * 3) + ((value >> 2) & ((_Type)AllSet() / 15 * 3));
	value = (value + (value >> 4)) & ((_Type)AllSet() / 255 * 15);
	return (_Type)(value * ((_Type)AllSet() / 255)) >> ((sizeof(value) - 1) * 8);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountSetBits(hh_u32 value)
{
	// could use popcnt on platforms supporting it
#if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)

	// 2 EVN, 1 ODD, latency = 8
	vec_uint4	v = spu_promote(value, 0);	// should map to no-op
	vec_uchar16	popcnt8 = spu_cntb((vec_char16)v);										// EVN, latency = 4
	vec_uint4	popcnt32 = (vec_uint4)spu_sumb(popcnt8, (vec_uchar16)spu_splats(0));	// EVN, latency = 4 + ODD, latency = 4
	return spu_extract(popcnt32, 0);		// should map to no-op

#elif (HH_CPU_MODEL == HH_CPU_JAGUAR)

	// Jaguar handles popcnt
	return _mm_popcnt_u32(value);

#else
	// Generic arch
#if 1
	value = (value & 0x55555555) + ((value >> 1) & 0x55555555);
	value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
	value = (value & 0x0F0F0F0F) + ((value >> 4) & 0x0F0F0F0F);
	value = (value + (value >> 8));
	value = (value + (value >> 16)) & 0x000000FF;
	return value;
#else
	value = value - ((value >> 1) & 0x55555555);
	value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
	return ((value + ((value >> 4) & 0xF0F0F0F)) * 0x1010101) >> 24;

//	hh_u32 tmp;
//	tmp = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);
//	return ((tmp + (tmp >> 3)) & 030707070707) % 63;
#endif
#endif
}

//----------------------------------------------------------------------------

#if (HH_CPU_MODEL == HH_CPU_JAGUAR)

//HH_FORCEINLINE hh_u64	IntegerTools::CountSetBits(hh_u64 value)
//{
//	return _mm_popcnt_u64(value);
//}

#endif

//----------------------------------------------------------------------------
//
//	leading/trailing bits pos computation
//	VisualStudio
//	FIXME: it might be better to use __bsf and __bsr when building on ICC
//
//----------------------------------------------------------------------------
#if	defined(HH_WINDOWS)

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingZeroes(hh_u32 value)
{
	hh_u32	pos;
	if (_BitScanForward((DWORD*)&pos, value) == 0)
		return 32;
	return pos;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingZeroes(hh_u64 value)
{
#if	defined(HH_CPU_64BITS)
	hh_u32	pos;
	if (_BitScanForward64((DWORD*)&pos, value) == 0)
		return 64;
	return pos;
#else
	LARGE_INTEGER	v64e;
	v64e.QuadPart = value;
	hh_u32	pos;
	if (_BitScanForward((DWORD*)&pos, v64e.LowPart) != 0)
		return pos;
	if (_BitScanForward((DWORD*)&pos, v64e.HighPart) == 0)
		return 64;
	return pos + 32;
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingZeroes(hh_u32 value)
{
	hh_u32	pos;
	if (_BitScanReverse((DWORD*)&pos, value) == 0)
		return 32;
	return 31 - pos;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingZeroes(hh_u64 value)
{
#if	defined(HH_CPU_64BITS)
	hh_u32	pos;
	if (_BitScanReverse64((DWORD*)&pos, value) == 0)
		return 64;
	return 63 - pos;
#else
	LARGE_INTEGER	v64e;
	v64e.QuadPart = value;
	hh_u32	pos;
	if (_BitScanReverse((DWORD*)&pos, v64e.HighPart) != 0)
		return 31 - pos;
	if (_BitScanReverse((DWORD*)&pos, v64e.LowPart) == 0)
		return 64;
	return 63 - pos;
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingOnes(hh_u32 value)
{
	hh_u32	pos;
	if (_BitScanForward((DWORD*)&pos, ~value) == 0)
		return 32;
	return pos;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingOnes(hh_u64 value)
{
#if	defined(HH_CPU_64BITS)
	hh_u32	pos;
	if (_BitScanForward64((DWORD*)&pos, ~value) == 0)
		return 64;
	return pos;
#else
	LARGE_INTEGER	v64e;
	v64e.QuadPart = ~value;
	hh_u32	pos;
	if (_BitScanForward((DWORD*)&pos, v64e.LowPart) != 0)
		return pos;
	if (_BitScanForward((DWORD*)&pos, v64e.HighPart) == 0)
		return 64;
	return pos + 32;
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingOnes(hh_u32 value)
{
	hh_u32	pos;
	if (_BitScanReverse((DWORD*)&pos, ~value) == 0)
		return 32;
	return 31 - pos;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingOnes(hh_u64 value)
{
	hh_u32	pos;
#if	defined(HH_CPU_64BITS)
	if (_BitScanReverse64((DWORD*)&pos, ~value) == 0)
		return 64;
	pos = 63 - pos;
#else
	LARGE_INTEGER	v64e;
	v64e.QuadPart = ~value;
	if (_BitScanReverse((DWORD*)&pos, v64e.HighPart) != 0)
		return 31 - pos;
	if (_BitScanReverse((DWORD*)&pos, v64e.LowPart) == 0)
		return 64;
	pos = 63 - pos;
#endif
	return pos;
}

#else	// !HH_WINDOWS
//----------------------------------------------------------------------------
//
//	leading/trailing bits pos computation
//	GCC/ICC
//
//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingZeroes(hh_u32 value)
{
#if	defined(HH_COMPILER_INTEL)
	if (value == 0)
		return 32;
	return _bit_scan_forward(value);
#elif	defined(HH_COMPILER_GCC) /*&& !defined(HH_COMPILER_SNC)*/
#	if (HH_CPU_MODEL == HH_CPU_JAGUAR)
		// Jaguar supports tzcnt instruction
		return __builtin_ctz(value);
#	elif (HH_CPU_MODEL == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
		hh_u32	rv = 32;
		__asm__("bsf %1,%0" : "+r"(rv) : "r"(value) : "cc");
		return rv;
#	else
		return __builtin_ctz(value);
#	endif
#else
	hh_u32 pos = 32;
	while (value != 0)
	{
		value <<= 1;
		pos--;
	}
	return pos;
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingZeroes(hh_u64 value)
{
#if	defined(HH_COMPILER_GCC) && !defined(HH_COMPILER_SNC)
	return __builtin_ctzll(value);
#else
	// horrible naive implem
	hh_u32		pos = 64;
	while (value != 0)
	{
		value <<= 1;
		pos--;
	}
	return pos;
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingZeroes(hh_u32 value)
{
#if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	return spu_extract(spu_cntlz(spu_promote(value, 0)), 0);
#else
#	if	defined(HH_COMPILER_INTEL)
		if (value == 0)
			return 32;
		return 31 - _bit_scan_reverse(value);
#	elif	defined(HH_COMPILER_GCC)
//#		if	HH_CPU_MODEL == HH_CPU_CELL_SPU
//		value = spu_extract(spu_cntlz(spu_promote(value, 0)), 0);
//#		else
		return value == 0 ? 32 : __builtin_clz(value);
//#		endif
#	elif	defined(HH_X360) && defined(HH_COMPILER_MSVC)
		if (value == 0)
			return 32;
		return _CountLeadingZeros(value);
#	elif	defined(HH_COMPILER_GHS)
		if (value == 0)
			return 32;
		return __cntlzw(value);
#	else
		hh_u32 pos = 32;
		while (value != 0)
		{
			value >>= 1;
			pos--;
		}
		return pos;
#	endif
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingZeroes(hh_u64 value)
{
#if	defined(HH_X360) && defined(HH_COMPILER_MSVC)
	return _CountLeadingZeros64(value);
#else
	hh_u32 pos = 64;
	while (value != 0)
	{
		value >>= 1;
		pos--;
	}
	return pos;
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingOnes(hh_u32 value)
{
	return CountTrailingZeroes(~value);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingOnes(hh_u32 value)
{
	return CountLeadingZeroes(~value);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountTrailingOnes(hh_u64 value)
{
	return CountTrailingZeroes(~value);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::CountLeadingOnes(hh_u64 value)
{
	return CountLeadingZeroes(~value);
}

#endif

//----------------------------------------------------------------------------
//
//	byte swapping / endianness change
//
//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u8		IntegerTools::ReverseBytes(hh_u8 value)
{
	return value;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u16	IntegerTools::ReverseBytes(hh_u16 value)
{
#if	defined(HH_WINDOWS)
	//return _byteswap_ushort(value);	// sloppy intrinsic, don't use
	return _rotl16(value, 8);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __lhbrx(&value);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	return spu_extract((vec_ushort8)si_rothi((qword)spu_promote(value, 0), 8), 0);
#elif	defined(HH_X360)
	return _byteswap_ushort(value);
#elif	(HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
	__asm__("rol $8, %0" : "+r"(value) : : "cc");
	return value;
#elif	defined(__arch__swab16)
	return __arch__swab16(value);
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	return hh_u16(__builtin_rev(value) >> 16);
#elif	defined(HH_COMPILER_GCC)
	return hh_u16(__builtin_bswap32(value) >> 16);
#else
	return RotateBitsLeft<8>(value);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::ReverseBytes(hh_u32 value)
{
#if	defined(HH_WINDOWS) || defined(HH_X360)
	return _byteswap_ulong(value);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __lwbrx(&value);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	qword		vq = (qword)spu_promote(value, 0);
	qword		sh = (qword)(vec_uint4){ 0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C };
	return spu_extract((vec_uint4)si_shufb(vq, vq, sh), 0);
#elif	(HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
	__asm__("bswap %0" : "+r"(value));
	return value;
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	return __builtin_rev(value);
#elif	defined(HH_COMPILER_GCC)
	return __builtin_bswap32(value);
#else
	return (value << 24) | ((value << 8) & 0x00FF0000) | ((value >> 8) & 0x0000FF00) | (value >> 24);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u64	IntegerTools::ReverseBytes(hh_u64 value)
{
#if	defined(HH_WINDOWS) || defined(HH_X360)
	return _byteswap_uint64(value);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __ldbrx(&value);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	qword		vq = (qword)spu_promote(value, 0);
	qword		sh = (qword)(vec_uint4){ 0x07060504, 0x03020100, 0x0F0E0D0C, 0x0B0A0908 };
	return spu_extract((vector unsigned long long)si_shufb(vq, vq, sh), 0);
#elif	(HH_CPU_ARCH == HH_CPU_X86) && defined(HH_ASSEMBLY_FORMAT_AT_T)
	__asm__("bswap %0" : "+r"(value));
	return value;
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	hh_u64	a = __builtin_rev(hh_u32(value));
	hh_u64	b = __builtin_rev(hh_u32(value >> 32));
	return b | (a << 32);
//#elif	defined(HH_COMPILER_GCC)
//	return __builtin_bswap64(value);
#elif	defined(HH_COMPILER_GCC)
	hh_u64	a = __builtin_bswap32(hh_u32(value));
	hh_u64	b = __builtin_bswap32(hh_u32(value >> 32));
	return b | (a << 32);
#else
	return (value >> 56) |
			((value & 0x00FF000000000000ULL) >> 40) |
			((value & 0x0000FF0000000000ULL) >> 24) |
			((value & 0x000000FF00000000ULL) >> 8) |
			((value & 0x00000000FF000000ULL) << 8) |
			((value & 0x0000000000FF0000ULL) << 24) |
			((value & 0x000000000000FF00ULL) << 40) |
			(value << 56);
#endif
}

//----------------------------------------------------------------------------
//
//	byte swapping / endianness change : Load from memory
//
//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u8	IntegerTools::LoadEndianSwapped(const hh_u8 *src)	// 0xAB -> 0xAB
{
	return *src;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u16	IntegerTools::LoadEndianSwapped(const hh_u16 *src)	// 0xAABB -> 0xBBAA
{
#if	defined(HH_X360)
	return __loadshortbytereverse(0, src);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __lhbrx(const_cast<hh_u16*>(src));	// doesn't modify memory, yet espects a non-const pointer... great.
#else
	return ReverseBytes(*src);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::LoadEndianSwapped(const hh_u32 *src)	// 0xAABBCCDD -> 0xDDCCBBAA
{
#if	defined(HH_X360)
	return __loadwordbytereverse(0, src);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __lwbrx(const_cast<hh_u32*>(src));	// doesn't modify memory, yet espects a non-const pointer... great.
#else
	return ReverseBytes(*src);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u64	IntegerTools::LoadEndianSwapped(const hh_u64 *src)	// 0xAABBCCDDEEFFGGHH -> 0xHHGGFFEEDDCCBBAA
{
#if	defined(HH_X360)
	return __loaddoublewordbytereverse(0, src);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __ldbrx(const_cast<hh_u64*>(src));	// doesn't modify memory, yet espects a non-const pointer... great.
#else
	return ReverseBytes(*src);
#endif
}

//----------------------------------------------------------------------------
//
//	byte swapping / endianness change : Store to memory
//
//----------------------------------------------------------------------------

HH_FORCEINLINE void	IntegerTools::StoreEndianSwapped(const hh_u8 value, hh_u8 *dst)	// 0xAB -> 0xAB
{
	*dst = value;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	IntegerTools::StoreEndianSwapped(const hh_u16 value, hh_u16 *dst)	// 0xAABB -> 0xBBAA
{
#if	defined(HH_X360)
	__storeshortbytereverse(value, 0, dst);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	__sthbrx(const_cast<hh_u16*>(dst), value);	// doesn't modify memory, yet espects a non-const pointer... great.
#else
	*dst = ReverseBytes(value);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	IntegerTools::StoreEndianSwapped(const hh_u32 value, hh_u32 *dst)	// 0xAABBCCDD -> 0xDDCCBBAA
{
#if	defined(HH_X360)
	__storewordbytereverse(value, 0, dst);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	__stwbrx(const_cast<hh_u32*>(dst), value);	// doesn't modify memory, yet espects a non-const pointer... great.
#else
	*dst = ReverseBytes(value);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	IntegerTools::StoreEndianSwapped(const hh_u64 value, hh_u64 *dst)	// 0xAABBCCDDEEFFGGHH -> 0xHHGGFFEEDDCCBBAA
{
#if	defined(HH_X360)
	__storedoublewordbytereverse(value, 0, dst);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	__stdbrx(const_cast<hh_u64*>(dst), value);	// doesn't modify memory, yet espects a non-const pointer... great.
#else
	*dst = ReverseBytes(value);
#endif
}

//----------------------------------------------------------------------------
//
//	bit rotations
//	8 bits
//
//----------------------------------------------------------------------------

// clang 3.6.0 has a regression, it fails to compile template parameters passed to inline asm
#if defined(HH_COMPILER_CLANG) && (CLANG_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(3, 6, 0) && CLANG_FULL_VERSION < HH_COMPILER_MAKE_VERSION(3, 7, 1))
#	define	HH_NO_ASM_TPL_PARAM		1
#else
#	define	HH_NO_ASM_TPL_PARAM		0
#endif

//----------------------------------------------------------------------------

template<hh_u32 _N>
HH_FORCEINLINE hh_u8		IntegerTools::RotateBitsLeft(hh_u8 value)
{
	enum { _N_WRAPPED = _N & 7, _RN_WRAPPED = (8 - _N) & 7 };
#if	defined(HH_WINDOWS)
	return _rotl8(value, _N_WRAPPED);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86) && (HH_NO_ASM_TPL_PARAM == 0)
	__asm__("rol %1, %0" : "+r"(value) : "I"(_N_WRAPPED) : "cc");
	return value;
//#elif	(HH_CPU_ARCH == HH_CPU_ARM) && !defined(HH_COMPILER_SNC)
//	return _crol_(value, _N_WRAPPED);
#else
	return (value << _N_WRAPPED) | (value >> _RN_WRAPPED);
#endif
}

//----------------------------------------------------------------------------

template<hh_u32 _N>
HH_FORCEINLINE hh_u8		IntegerTools::RotateBitsRight(hh_u8 value)
{
	enum { _N_WRAPPED = _N & 7, _RN_WRAPPED = (8 - _N) & 7 };
#if	defined(HH_WINDOWS)
	return _rotr8(value, _N_WRAPPED);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86) && (HH_NO_ASM_TPL_PARAM == 0)
	__asm__("ror %1, %0" : "+r"(value) : "I"(_N_WRAPPED) : "cc");
	return value;
//#elif	(HH_CPU_ARCH == HH_CPU_ARM) && !defined(HH_COMPILER_SNC)
//	return _cror_(value, _N_WRAPPED);
#else
	return (value >> _N_WRAPPED) | (value << _RN_WRAPPED);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u8		IntegerTools::RotateBitsLeft(hh_u8 value, hh_u32 n)
{
#if	defined(HH_WINDOWS)
	return _rotl8(value, n);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_MODEL == HH_CPU_X86 || HH_CPU_MODEL == HH_CPU_JAGUAR)
	__asm__("rol %%cl, %0" : "+r"(value) : "c"(n) : "cc");
	return value;
//#elif	(HH_CPU_ARCH == HH_CPU_ARM) && !defined(HH_COMPILER_SNC)
//	return _crol_(value, n);
#else
	const hh_u32	nWrapped = n & 7;
	return (value << nWrapped) | (value >> (8 - nWrapped));
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u8		IntegerTools::RotateBitsRight(hh_u8 value, hh_u32 n)
{
#if	defined(HH_WINDOWS)
	return _rotr8(value, n);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_MODEL == HH_CPU_X86 || HH_CPU_MODEL == HH_CPU_JAGUAR)
	__asm__("ror %%cl, %0" : "+r"(value) : "c"(n) : "cc");
	return value;
//#elif	(HH_CPU_ARCH == HH_CPU_ARM) && !defined(HH_COMPILER_SNC)
//	return _cror_(value, n);
#else
	const hh_u32	nWrapped = n & 7;
	return (value >> nWrapped) | (value << (8 - nWrapped));
#endif
}

//----------------------------------------------------------------------------
//
//	bit rotations
//	16 bits
//
//----------------------------------------------------------------------------

template<hh_u32 _N>
HH_FORCEINLINE hh_u16	IntegerTools::RotateBitsLeft(hh_u16 value)
{
	enum { _N_WRAPPED = _N & 15, _RN_WRAPPED = (16 - _N) & 15 };
#if	defined(HH_WINDOWS)
	return _rotl16(value, _N_WRAPPED);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86) && (HH_NO_ASM_TPL_PARAM == 0)
	__asm__("rol %1, %0" : "+r"(value) : "I"(_N_WRAPPED) : "cc");
	return value;
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	return spu_extract((vec_ushort8)si_rothi((qword)spu_promote(value, 0), _N_WRAPPED), 0);
#else
	return (value << _N_WRAPPED) | (value >> _RN_WRAPPED);
#endif
}

//----------------------------------------------------------------------------

template<hh_u32 _N>
HH_FORCEINLINE hh_u16	IntegerTools::RotateBitsRight(hh_u16 value)
{
	enum { _N_WRAPPED = _N & 15, _RN_WRAPPED = (16 - _N) & 15 };
#if	defined(HH_WINDOWS)
	return _rotr16(value, _N_WRAPPED);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86) && (HH_NO_ASM_TPL_PARAM == 0)
	__asm__("ror %1, %0" : "+r"(value) : "I"(_N_WRAPPED) : "cc");
	return value;
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	return spu_extract((vec_ushort8)si_rothi((qword)spu_promote(value, 0), _RN_WRAPPED), 0);
#else
	return (value >> _N_WRAPPED) | (value << _RN_WRAPPED);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u16	IntegerTools::RotateBitsLeft(hh_u16 value, hh_u32 n)
{
#if	defined(HH_WINDOWS)
	return _rotl16(value, n);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_MODEL == HH_CPU_X86 || HH_CPU_MODEL == HH_CPU_JAGUAR)
	__asm__("rol %%cl, %0" : "+r"(value) : "c"(n) : "cc");
	return value;
//#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
//	return spu_extract((vec_ushort8)si_rothi((qword)spu_promote(value, 0), n), 0);	// 'n' must be an integer literal
#else
	const hh_u32	nWrapped = n & 15;
	return (value << nWrapped) | (value >> (16 - nWrapped));
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u16	IntegerTools::RotateBitsRight(hh_u16 value, hh_u32 n)
{
#if	defined(HH_WINDOWS)
	return _rotr16(value, n);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_MODEL == HH_CPU_X86 || HH_CPU_MODEL == HH_CPU_JAGUAR)
	__asm__("ror %%cl, %0" : "+r"(value) : "c"(n) : "cc");
	return value;
//#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
//	return spu_extract((vec_ushort8)si_rothi((qword)spu_promote(value, 0), 16 - n), 0);	// 'n' must be an integer literal
#else
	const hh_u32	nWrapped = n & 15;
	return (value >> nWrapped) | (value << (16 - nWrapped));
#endif
}

//----------------------------------------------------------------------------
//
//	bit rotations
//	32 bits
//
//----------------------------------------------------------------------------

template<hh_u32 _N>
HH_FORCEINLINE hh_u32	IntegerTools::RotateBitsLeft(hh_u32 value)
{
	enum { _N_WRAPPED = _N & 31, _RN_WRAPPED = (32 - _N) & 31 };
#if	defined(HH_WINDOWS) || defined(HH_X360)	// will map to rlwinm on X360
	return _rotl(value, _N_WRAPPED);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86) && (HH_NO_ASM_TPL_PARAM == 0)
	__asm__("roll %1, %0" : "+r"(value) : "I"(_N_WRAPPED) : "cc");
	return value;
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __rlwinm(value, _N_WRAPPED, 0, 31);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	return spu_extract((vec_uint4)si_roti((qword)spu_promote(value, 0), _N_WRAPPED), 0);
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	return __builtin_rol(value, _N_WRAPPED);
//#elif	(HH_CPU_ARCH == HH_CPU_ARM)
//	return _irol_(value, _N_WRAPPED);
#else
	return (value << _N_WRAPPED) | (value >> _RN_WRAPPED);	// impressive... visual studio knows how to convert this to a 'rol' or 'ror' instruction, depending on 'n'...
#endif
}

//----------------------------------------------------------------------------

template<hh_u32 _N>
HH_FORCEINLINE hh_u32	IntegerTools::RotateBitsRight(hh_u32 value)
{
	enum { _N_WRAPPED = _N & 31, _RN_WRAPPED = (32 - _N) & 31 };
#if	defined(HH_WINDOWS) || defined(HH_X360)	// will map to rlwinm on X360
	return _rotr(value, _N_WRAPPED);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86) && (HH_NO_ASM_TPL_PARAM == 0)
	__asm__("rorl %1, %0" : "+r"(value) : "I"(_N_WRAPPED) : "cc");
	return value;
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __rlwinm(value, _RN_WRAPPED, 0, 31);
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	return spu_extract((vec_uint4)si_roti((qword)spu_promote(value, 0), _RN_WRAPPED), 0);
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	return __builtin_ror(value, _N_WRAPPED);
//#elif	(HH_CPU_ARCH == HH_CPU_ARM)
//	return _iror_(value, _N_WRAPPED);
#else
	return (value >> _N_WRAPPED) | (value << _RN_WRAPPED);	// impressive... visual studio knows how to convert this to a 'rol' or 'ror' instruction, depending on 'n'...
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::RotateBitsLeft(hh_u32 value, hh_u32 n)
{
#if	defined(HH_WINDOWS) || defined(HH_X360)	// will map to rlwnm on X360 (warning: microcoded)
	return _rotl(value, n);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86)
	__asm__("roll %%cl, %0" : "+r"(value) : "c"(n) : "cc");
	return value;
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __rlwnm(value, n, 0, 31);	// warning: microcoded
//#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
//	return spu_extract((vec_uint4)si_roti((qword)spu_promote(value, 0), n), 0);	// 'n' must be an integer literal
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	return __builtin_rol(value, n);
//#elif	(HH_CPU_ARCH == HH_CPU_ARM)
//	return _irol_(value, n);
#else
	const hh_u32	nWrapped = n & 31;
	return (value << nWrapped) | (value >> (32 - nWrapped));	// impressive... visual studio knows how to convert this to a 'rol' or 'ror' instruction, depending on 'n'...
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_u32	IntegerTools::RotateBitsRight(hh_u32 value, hh_u32 n)
{
#if	defined(HH_WINDOWS) || defined(HH_X360)	// will map to rlwnm on X360 (warning: microcoded)
	return _rotr(value, n);
#elif	defined(HH_COMPILER_GCC) && (HH_CPU_ARCH == HH_CPU_X86)
	__asm__("rorl %%cl, %0" : "+r"(value) : "c"(n) : "cc");
	return value;
#elif	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
	return __rlwnm(value, 32 - n, 0, 31);	// warning: microcoded
//#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
//	return spu_extract((vec_uint4)si_roti((qword)spu_promote(value, 0), 32 - n), 0);	// 'n' must be an integer literal
#elif	(HH_CPU_ARCH == HH_CPU_ARM) && defined(HH_COMPILER_SNC)
	return __builtin_ror(value, n);
//#elif	(HH_CPU_ARCH == HH_CPU_ARM)
//	return _iror_(value, n);
#else
	const hh_u32	nWrapped = n & 31;
	return (value >> nWrapped) | (value << (32 - nWrapped));	// impressive... visual studio knows how to convert this to a 'rol' or 'ror' instruction, depending on 'n'...
#endif
}

//----------------------------------------------------------------------------
//
//	X86-specific
//
//----------------------------------------------------------------------------

#if	(HH_CPU_ARCH == HH_CPU_X86)

	//	WARNING: use only if CPU features >= SSE4.2
	HH_FORCEINLINE hh_u32	IntegerTools::POPCNT32(hh_u32 value)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u32	r = 0;
		__asm__("popcnt %1, %0" : "=r"(r) : "r"(value));
		return r;
#else
		return __popcnt(value);
#endif
	}

	// should be safe everywhere
	HH_FORCEINLINE hh_u32	IntegerTools::BTComplement(hh_u32 value, hh_u32 i)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u32	r = 0;
		__asm__("btc %1, %0" : "=r"(r) : "r"(i), "0"(value) : "cc");
		return r;
#else
		_bittestandcomplement((LONG*)&value, i);
		return value;
#endif
	}

	HH_FORCEINLINE hh_u32	IntegerTools::BTSet(hh_u32 value, hh_u32 i)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u32	r = 0;
		__asm__("bts %1, %0" : "=r"(r) : "r"(i), "0"(value) : "cc");
		return r;
#else
		_bittestandset((LONG*)&value, i);
		return value;
#endif
	}

	HH_FORCEINLINE hh_u32	IntegerTools::BTReset(hh_u32 value, hh_u32 i)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u32	r = 0;
		__asm__("btr %1, %0" : "=r"(r) : "r"(i), "0"(value) : "cc");
		return r;
#else
		_bittestandreset((LONG*)&value, i);
		return value;
#endif
	}

#if	(HH_CPU_MODEL == HH_CPU_X64)
	HH_FORCEINLINE hh_u64	IntegerTools::BTComplement(hh_u64 value, hh_u32 i)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u64	r = 0;
		__asm__("btc %1, %0" : "=r"(r) : "r"(i), "0"(value) : "cc");
		return r;
#else
		_bittestandcomplement64((__int64*)&value, i);
		return value;
#endif
	}

	HH_FORCEINLINE hh_u64	IntegerTools::BTSet(hh_u64 value, hh_u32 i)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u64	r = 0;
		__asm__("bts %1, %0" : "=r"(r) : "r"(i), "0"(value) : "cc");
		return r;
#else
		_bittestandset64((__int64*)&value, i);
		return value;
#endif
	}

	HH_FORCEINLINE hh_u64	IntegerTools::BTReset(hh_u64 value, hh_u32 i)
	{
#if	defined(HH_COMPILER_GCC)
		hh_u64	r = 0;
		__asm__("btr %1, %0" : "=r"(r) : "r"(i), "0"(value) : "cc");
		return r;
#else
		_bittestandreset64((__int64*)&value, i);
		return value;
#endif
	}
#endif

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_NUMERIC_TOOLS_INT_INL__
