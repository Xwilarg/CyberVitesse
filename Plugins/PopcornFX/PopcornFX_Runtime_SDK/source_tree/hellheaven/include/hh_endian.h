#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/10/09 12:28 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_ENDIAN_H__
#define	__HH_ENDIAN_H__

#include "hh_maths/include/hh_numeric_tools.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if	!defined(HH_LITTLE_ENDIAN) && !defined(HH_BIG_ENDIAN)
#	if	defined(__BYTE_ORDER__)
#		if		defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#			define	HH_LITTLE_ENDIAN
#		elif	defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#			define	HH_BIG_ENDIAN
#		else	// probably __ORDER_PDP_ENDIAN__
#			error	Unsupported __BYTE_ORDER__ value
#		endif
#	elif	(HH_CPU_ARCH == HH_CPU_X86) || defined(__LITTLE_ENDIAN__)
#		define	HH_LITTLE_ENDIAN
#	elif	(HH_CPU_ARCH == HH_CPU_PPC) || defined(__BIG_ENDIAN__) || defined(_M_PPCBE) || defined(__SPU__)
#		define	HH_BIG_ENDIAN
#	elif	(HH_CPU_ARCH == HH_CPU_ARM)
//#		define	HH_BIG_ENDIAN
#		define	HH_LITTLE_ENDIAN
#	else
#		error	Could not determine target platform endianness
#	endif
#endif

//----------------------------------------------------------------------------

// something else thanks to GCC for witch multichars character constants are deprecated... :|
// MCCC like Multi-Characters Character Constant
// FIXME: care about endianness?
/* #define	HH_MCCC2(c0, c1)		((c0) | ((c1) << 8)) */
/* #define	HH_MCCC4(c0, c1, c2, c3)	((c0) | ((c1) << 8) | ((c2) << 16) | ((c3) << 24)) */

#if	defined(HH_LITTLE_ENDIAN)
// something else thanks to GCC for which multichars character constants are deprecated...
#	define	HH_PACK_16(a, b)					(((a) & 0xFF) | (((b) & 0xFF) << 8))
#	define	HH_PACK_32(a, b, c, d)				(HH_PACK_16(a, b) | (HH_PACK_16(c, d) << 16))
#	define	HH_PACK_64(a, b, c, d, e, f, g, h)	(((hh_i64)HH_PACK_32(a, b, c, d)) | (((hh_i64)HH_PACK_32(e, f, g, h)) << 32))

#	define	HH_UNPACK_16_0(packed)				((packed) & 0xFF)
#	define	HH_UNPACK_16_1(packed)				(((packed) >> 8) & 0xFF)

#else	/* defined(HH_BIG_ENDIAN) */

#	define	HH_PACK_16(a, b)					(((a) << 8) | (b))
#	define	HH_PACK_32(a, b, c, d)				((HH_PACK_16(a, b) << 16) | HH_PACK_16(c, d))
#	define	HH_PACK_64(a, b, c, d, e, f, g, h)	((((hh_i64)HH_PACK_32(a, b, c, d)) << 32) | ((hh_i64)HH_PACK_32(e, f, g, h)))

#	define	HH_UNPACK_16_0(packed)				((packed >> 8) & 0xFF)
#	define	HH_UNPACK_16_1(packed)				(packed & 0xFF)

#endif

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _TypeSizeInBytes = sizeof(_Type)>
struct	TEndianSwapper
{
//	HH_FORCEINLINE static _Type	Swap(const _Type &v);
//	HH_FORCEINLINE static void	SwapInPlace(_Type &v);
//	HH_FORCEINLINE static _Type	LoadAndSwap(const _Type *ptr);
//	HH_FORCEINLINE static void	SwapAndStore(const _Type &v, void *ptr);
};

template<typename _Type>
struct	TEndianSwapper<_Type, 1>
{
	HH_FORCEINLINE static _Type	Swap(const _Type &v) { return v; }
	HH_FORCEINLINE static void	SwapInPlace(const _Type &v) { }
	HH_FORCEINLINE static _Type	LoadAndSwap(const void *ptr) { return *static_cast<const hh_u8 *>(ptr); }
	HH_FORCEINLINE static void	SwapAndStore(const _Type &v, void *ptr) { *static_cast<hh_u8 *>(ptr) = reinterpret_cast<const hh_u8 &>(v); }
};

template<typename _Type>
struct	TEndianSwapper<_Type, 2>
{
	HH_FORCEINLINE static _Type	Swap(const _Type &v) { const hh_u16 vs = IntegerTools::ReverseBytes(reinterpret_cast<const hh_u16 &>(v)); return reinterpret_cast<const _Type &>(vs); }
	HH_FORCEINLINE static void	SwapInPlace(_Type &v) { hh_u16 *__st = reinterpret_cast<hh_u16*>(&v); *__st = IntegerTools::ReverseBytes(*reinterpret_cast<const hh_u16*>(__st)); }
	HH_FORCEINLINE static _Type	LoadAndSwap(const void *ptr) { const hh_u16 vs = IntegerTools::LoadEndianSwapped(static_cast<const hh_u16 *>(ptr)); return reinterpret_cast<const _Type &>(vs); }
	HH_FORCEINLINE static void	SwapAndStore(const _Type &v, void *ptr) { IntegerTools::StoreEndianSwapped(reinterpret_cast<const hh_u16 &>(v), static_cast<hh_u16 *>(ptr)); }
};

template<typename _Type>
struct	TEndianSwapper<_Type, 4>
{
	HH_FORCEINLINE static _Type	Swap(const _Type &v) { const hh_u32 vs = IntegerTools::ReverseBytes(reinterpret_cast<const hh_u32 &>(v)); return reinterpret_cast<const _Type &>(vs); }
	HH_FORCEINLINE static void	SwapInPlace(_Type &v) { hh_u32 *__st = reinterpret_cast<hh_u32*>(&v); *__st = IntegerTools::ReverseBytes(*reinterpret_cast<const hh_u32*>(__st)); }
	HH_FORCEINLINE static _Type	LoadAndSwap(const void *ptr) { const hh_u32 vs = IntegerTools::LoadEndianSwapped(static_cast<const hh_u32 *>(ptr)); return reinterpret_cast<const _Type &>(vs); }
	HH_FORCEINLINE static void	SwapAndStore(const _Type &v, void *ptr) { IntegerTools::StoreEndianSwapped(reinterpret_cast<const hh_u32 &>(v), static_cast<hh_u32 *>(ptr)); }
};

template<typename _Type>
struct	TEndianSwapper<_Type, 8>
{
	HH_FORCEINLINE static _Type	Swap(const _Type &v) { const hh_u64 vs = IntegerTools::ReverseBytes(reinterpret_cast<const hh_u64 &>(v)); return reinterpret_cast<const _Type &>(vs); }
	HH_FORCEINLINE static void	SwapInPlace(_Type &v) { hh_u64 *__st = reinterpret_cast<hh_u64*>(&v); *__st = IntegerTools::ReverseBytes(*reinterpret_cast<const hh_u64*>(__st)); }
	HH_FORCEINLINE static _Type	LoadAndSwap(const void *ptr) { const hh_u64 vs = IntegerTools::LoadEndianSwapped(static_cast<const hh_u64 *>(ptr)); return reinterpret_cast<const _Type &>(vs); }
	HH_FORCEINLINE static void	SwapAndStore(const _Type &v, void *ptr) { IntegerTools::StoreEndianSwapped(reinterpret_cast<const hh_u64 &>(v), static_cast<hh_u64 *>(ptr)); }
};

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE _Type	PKSwapEndian(const _Type &v) { return TEndianSwapper<_Type>::Swap(v); }
template<typename _Type>
HH_FORCEINLINE void		PKSwapEndianInPlace(_Type &v) { TEndianSwapper<_Type>::SwapInPlace(v); }
template<typename _Type>
HH_FORCEINLINE _Type	PKLoadAndSwapEndian(const void *src) { return TEndianSwapper<_Type>::LoadAndSwap(src); }
template<typename _Type>
HH_FORCEINLINE void		PKSwapEndianAndStore(const _Type &v, void *dst) { TEndianSwapper<_Type>::SwapAndStore(v, dst); }

//----------------------------------------------------------------------------

#if	defined(HH_LITTLE_ENDIAN)

template<typename _Type>
HH_FORCEINLINE _Type	PKSwapLittleEndian(const _Type &v) { return v; }
template<typename _Type>
HH_FORCEINLINE _Type	PKSwapBigEndian(const _Type &v) { return PKSwapEndian(v); }

template<typename _Type>
HH_FORCEINLINE void		PKSwapLittleEndianInPlace(_Type &v) {}
template<typename _Type>
HH_FORCEINLINE void		PKSwapBigEndianInPlace(_Type &v) { PKSwapEndianInPlace(v); }

template<typename _Type>
HH_FORCEINLINE _Type	PKLoadAndSwapLittleEndian(const void *srcPtr)
{
	// FIXME ?
	HH_ALIGN(0x10) hh_u8 storage[sizeof(_Type)];
	memcpy(storage, srcPtr, sizeof(_Type));
	return *reinterpret_cast<const _Type*>(storage);
}
template<typename _Type>
HH_FORCEINLINE _Type	PKLoadAndSwapBigEndian(const void *srcPtr) { return PKLoadAndSwapEndian<_Type>(srcPtr); }

template<typename _Type>
HH_FORCEINLINE void		PKSwapLittleEndianAndStore(const _Type &v, void *dstPtr)
{
	memcpy(dstPtr, &v, sizeof(_Type));
	//*static_cast<_Type*>(dstPtr) = v;
}
template<typename _Type>
HH_FORCEINLINE void		PKSwapBigEndianAndStore(const _Type &v, void *dstPtr) { PKSwapEndianAndStore<_Type>(v, dstPtr); }

#else
// big-endian

template<typename _Type>
HH_FORCEINLINE _Type	PKSwapLittleEndian(const _Type &v) { return PKSwapEndian(v); }
template<typename _Type>
HH_FORCEINLINE _Type	PKSwapBigEndian(const _Type &v) { return v; }

template<typename _Type>
HH_FORCEINLINE void		PKSwapLittleEndianInPlace(_Type &v) { PKSwapEndianInPlace(v); }
template<typename _Type>
HH_FORCEINLINE void		PKSwapBigEndianInPlace(_Type &v) {}

template<typename _Type>
HH_FORCEINLINE _Type	PKLoadAndSwapLittleEndian(const void *srcPtr) { return PKLoadAndSwapEndian<_Type>(srcPtr); }
template<typename _Type>
HH_FORCEINLINE _Type	PKLoadAndSwapBigEndian(const void *srcPtr) { return *static_cast<const _Type*>(srcPtr); }

template<typename _Type>
HH_FORCEINLINE void		PKSwapLittleEndianAndStore(const _Type &v, void *dstPtr) { PKSwapEndianAndStore<_Type>(v, dstPtr); }
template<typename _Type>
HH_FORCEINLINE void		PKSwapBigEndianAndStore(const _Type &v, void *dstPtr) { *static_cast<_Type*>(dstPtr) = v; }

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HH_ENDIAN_H__
