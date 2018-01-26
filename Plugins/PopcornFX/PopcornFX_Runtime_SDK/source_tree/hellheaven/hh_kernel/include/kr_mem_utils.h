#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2003/07/05 12:02 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_MEM_UTILS_H__
#define	__KR_MEM_UTILS_H__

#include "hh_kernel/include/kr_mem_basics.h"

#include <string.h>

#ifndef	PV_MODULE_NAME
#	define	PV_MODULE_NAME	""
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Mem
{
	HH_FORCEINLINE bool			Overlaps(hh_ureg a, hh_u32 abytes, hh_ureg b, hh_u32 bbytes) { return b > a ? (b - a) < abytes : (a - b) < bbytes; }
	HH_FORCEINLINE bool			Overlaps(const void *a, hh_u32 abytes, const void *b, hh_u32 bbytes) { return Overlaps(reinterpret_cast<hh_ureg>(a), abytes, reinterpret_cast<hh_ureg>(b), bbytes); }
	HH_FORCEINLINE bool			Overlaps(volatile const void *a, hh_u32 abytes, volatile const void *b, hh_u32 bbytes) { return Overlaps(reinterpret_cast<hh_ureg>(a), abytes, reinterpret_cast<hh_ureg>(b), bbytes); }
	template <typename _TypeA, typename _TypeB>
	HH_FORCEINLINE bool			OverlapsT(const _TypeA *a, hh_u32 acount, const _TypeB *b, hh_u32 bcount) { return Overlaps(a, acount * sizeof(*a), b, bcount * sizeof(*b)); }

#if	defined(HH_WINDOWS) || defined(HH_LINUX) || defined(HH_MACOSX)
	HH_FORCEINLINE void			Copy(void * __restrict dst, const void * __restrict src, hh_u32 byteCount) { HH_ASSERT(!Mem::Overlaps(dst, byteCount, src, byteCount)); memcpy(dst, src, byteCount); }
	HH_FORCEINLINE void			Copy_Uncached(volatile void * __restrict dst, const volatile void * __restrict src, hh_u32 byteCount) { HH_ASSERT(!Mem::Overlaps(dst, byteCount, src, byteCount));  memcpy((void*)dst, (void*)src, byteCount); }
	//HH_FORCEINLINE void		Copy_Overlapped(void *dst, const void *src, hh_u32 byteCount) { memmove(dst, src, byteCount); }
	HH_KERNEL_EXPORT void		Copy_Overlapped(void *dst, const void *src, hh_u32 byteCount);
	HH_FORCEINLINE void			Clear(void *ptr, hh_u32 byteCount) { memset(ptr, 0, byteCount); }		// no need for ::ZeroMemory() -> maps to memset(ptr, 0, size) anyway...
	HH_FORCEINLINE void			Clear_Uncached(volatile void *ptr, hh_u32 byteCount) { memset((void*)ptr, 0, byteCount); }
	HH_FORCEINLINE void			Fill8(void *ptr, hh_u8 value, hh_u32 byteCount) { memset(ptr, value, byteCount); }
#elif	(HH_CPU_MODEL == HH_CPU_CELL_SPU)
	HH_FORCEINLINE void			Copy(void * __restrict dst, const void * __restrict src, hh_u32 byteCount) { HH_ASSERT(!Mem::Overlaps(dst, byteCount, src, byteCount)); memcpy(dst, src, byteCount); }
	HH_FORCEINLINE void			Copy_Uncached(volatile void * __restrict dst, const volatile void * __restrict src, hh_u32 byteCount) { HH_ASSERT(!Mem::Overlaps(dst, byteCount, src, byteCount)); memcpy((void*)dst, (void*)src, byteCount); }
	HH_FORCEINLINE void			Copy_Overlapped(void *dst, const void *src, hh_u32 byteCount) { memmove(dst, src, byteCount); }
	HH_FORCEINLINE void			Clear(void *ptr, hh_u32 byteCount) { memset(ptr, 0, byteCount); }
	HH_FORCEINLINE void			Clear_Uncached(volatile void *ptr, hh_u32 byteCount) { memset((void*)ptr, 0, byteCount); }
	HH_FORCEINLINE void			Fill8(void *ptr, hh_u8 value, hh_u32 byteCount) { memset(ptr, value, byteCount); }
#else
#	if	defined(HH_X360) || defined(HH_PS3)
	HH_KERNEL_EXPORT void		Copy(void * __restrict dst, const void * __restrict src, hh_u32 byteCount);
	HH_KERNEL_EXPORT void		Copy_Uncached(volatile void * __restrict dst, const volatile void * __restrict src, hh_u32 byteCount);
#	else
	HH_FORCEINLINE void			Copy(void * __restrict dst, const void * __restrict src, hh_u32 byteCount) { memcpy(dst, src, byteCount); }
	HH_FORCEINLINE void			Copy_Uncached(volatile void * __restrict dst, const volatile void * __restrict src, hh_u32 byteCount) { memcpy((void*)dst, (const void*)src, byteCount); }
#	endif
	HH_KERNEL_EXPORT void		Copy_Overlapped(void *dst, const void *src, hh_u32 byteCount);
	HH_KERNEL_EXPORT void		Clear(void *ptr, hh_u32 byteCount);
	HH_KERNEL_EXPORT void		Clear_Uncached(volatile void *ptr, hh_u32 byteCount);
	HH_KERNEL_EXPORT void		Fill8(void *ptr, hh_u8 value, hh_u32 byteCount);
#endif

	HH_FORCEINLINE void			Copy_OverlappedIFN(void *dst, const void *src, hh_u32 byteCount) { if (Mem::Overlaps(dst, byteCount, src, byteCount)) Mem::Copy_Overlapped(dst, src, byteCount); else Mem::Copy(dst, src, byteCount); }

	template <typename _Type>
	HH_FORCEINLINE void			CopyT(_Type * __restrict dst, const _Type * __restrict src, hh_u32 count) { Mem::Copy(dst, src, count * sizeof(_Type)); }
	template <typename _Type>
	HH_FORCEINLINE void			CopyT_Overlapped(_Type * dst, const _Type * src, hh_u32 count) { Mem::Copy_Overlapped(dst, src, count * sizeof(_Type)); }
	template <typename _Type>
	HH_FORCEINLINE void			CopyT_Uncached(volatile _Type * __restrict dst, const volatile _Type * __restrict src, hh_u32 count) { Mem::Copy_Uncached(dst, src, count * sizeof(_Type)); }
	template <typename _Type>
	HH_FORCEINLINE void			CopyT_OverlappedIFN(_Type *dst, const _Type *src, hh_u32 count) { Mem::Copy_OverlappedIFN(dst, src, count * sizeof(_Type)); }

	HH_KERNEL_EXPORT void		Fill16(void *ptr, hh_u16 value, hh_u32 wordCount);
	HH_KERNEL_EXPORT void		Fill32(void *ptr, hh_u32 value, hh_u32 dwordCount);
	HH_KERNEL_EXPORT void		Fill64(void *ptr, hh_u64 value, hh_u32 qwordCount);
	//HH_KERNEL_EXPORT void		Fill128(void *ptr, SIMD::VectorArg value, hh_u32 xqwordCount);
	HH_KERNEL_EXPORT void		Fill128(void *ptr, const void *value128Ptr, hh_u32 xqwordCount);

	HH_KERNEL_EXPORT void		FillN(void *dstPtr, const void *srcValue, hh_u32 dstElementCount, hh_u32 dstStride);

	template<typename _Type>
	HH_FORCEINLINE void			Clear(_Type &object)
	{
		// always use compiler intrinsic for small enough types
		if (sizeof(_Type) < 512)
			memset(&object, 0, sizeof(object));
		else
			Clear(&object, sizeof(object));
	}

	HH_KERNEL_EXPORT void		SwapEndian16(void *ptr, hh_u32 wordCount);
	HH_KERNEL_EXPORT void		SwapEndian32(void *ptr, hh_u32 dwordCount);
	HH_KERNEL_EXPORT void		SwapEndian64(void *ptr, hh_u32 qwordCount);
};

//----------------------------------------------------------------------------

#define HH_MEM_LOOKS_VALID(__mem_ptr_should_be_legit) \
	(	*reinterpret_cast<const hh_u32*>(__mem_ptr_should_be_legit) != 0xFEEEFEEE && \
		*reinterpret_cast<const hh_u32*>(__mem_ptr_should_be_legit) != 0xBAADF00D && \
		*reinterpret_cast<const hh_u32*>(__mem_ptr_should_be_legit) != 0xDEADBEEF && \
		*reinterpret_cast<const hh_u32*>(__mem_ptr_should_be_legit) != 0xDDDDDDDD)

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_MEM_UTILS_H__
