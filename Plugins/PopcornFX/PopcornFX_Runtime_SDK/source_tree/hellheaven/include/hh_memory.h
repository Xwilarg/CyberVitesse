#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2014/09/07 18:19 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_MEMORY_H__
#define	__HH_MEMORY_H__

#include "hh_maths/include/hh_numeric_tools.h"

#if defined(HH_MACOSX) || defined(HH_IOS)

// MacOSX
#	if		defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#		if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 101200	// >= Mac OSX 10.12
#			define	PK_APPLE_USE_STD_ATOMIC_FENCE		1
#		endif

// iOS
#	elif	defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#		if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 100000	// >= iOS 10.0
#			define	PK_APPLE_USE_STD_ATOMIC_FENCE		1
#		endif

// tvOS
#	elif	defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#		if __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ >= 100000	// >= iOS 10.0
#			define	PK_APPLE_USE_STD_ATOMIC_FENCE		1
#		endif

#	else
#		error Could not find MacOSX/iOS versions
#	endif

#	ifdef PK_APPLE_USE_STD_ATOMIC_FENCE
#		include <atomic>
#	else
#		include <libkern/OSAtomic.h>
#	endif

#endif // APPLE

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Misc utility functions (alignment, raw pointer advance, etc...)
//
//----------------------------------------------------------------------------

namespace	Mem
{

	template<hh_ureg _Alignment, typename _Type>
	HH_FORCEINLINE bool		IsAligned(_Type ptr)
	{
		HH_STATIC_ASSERT(TIntegerTemplateTools::IsPowerOfTwo<_Alignment>::True);
		return ((hh_ureg)ptr & (_Alignment - 1)) == 0;
	}

	template<typename _Type>
	HH_FORCEINLINE _Type	Alignment(_Type bits)
	{
		return 1 + ((bits ^ (bits - 1)) >> 1);	// (01101011010000 ^ 1101011001111) --> 00000000011111 --> 00000000001111 --> 000000000010000 \o/
	}

	template<typename _Type>
	HH_FORCEINLINE hh_ureg	Alignment(_Type *ptr)
	{
		HH_STATIC_ASSERT(sizeof(_Type*) == sizeof(hh_ureg));
		hh_ureg	bits = (hh_ureg)ptr;
		return 1 + ((bits ^ (bits - 1)) >> 1);
	}

	template<hh_ureg _Alignment, typename _Type>
	HH_FORCEINLINE _Type	Align(_Type ptr)
	{
		HH_STATIC_ASSERT(TIntegerTemplateTools::IsPowerOfTwo<_Alignment>::True);
		return (_Type)(((hh_ureg)ptr + (_Alignment - 1)) & ~(hh_ureg(_Alignment) - 1));
	}

	template<typename _Type>
	HH_FORCEINLINE _Type	Align(_Type ptr, hh_u32 alignment)
	{
#if 1
		HH_ASSERT(IntegerTools::IsPowerOfTwo(alignment));
		return (_Type)(((hh_ureg)ptr + (alignment - 1)) & ~(hh_ureg(alignment) - 1));
#else
		hh_ureg	uPtr = (hh_ureg)ptr + (alignment - 1);
		hh_ureg	uSpill = uPtr % alignment;
		return (_Type)(uPtr - uSpill);
#endif
	}

	template<typename _Type0, typename _Type1>
	HH_FORCEINLINE bool		IsAliased(const _Type0 *ptr0, hh_u32 count0, const _Type1 *ptr1, hh_u32 count1)
	{
		const hh_u8	*s0 = reinterpret_cast<const hh_u8*>(ptr0);
		const hh_u8	*s1 = reinterpret_cast<const hh_u8*>(ptr1);
		const hh_u8	*e0 = reinterpret_cast<const hh_u8*>(ptr0 + count0);
		const hh_u8	*e1 = reinterpret_cast<const hh_u8*>(ptr1 + count1);
		//return s1 < e0 && s0 < e1;

		// could use branchless version:
#if 1
		return ((s1 - e0) & (s0 - e1)) < 0;
#else
		return !!(hh_ireg((s1 - e0) & (s0 - e1)) >> (8 * sizeof(hh_ireg) - 1));
#endif
	}

	HH_FORCEINLINE bool		IsAliased_RawBytePointers(const void *ptr0, hh_u32 byteCount0, const void *ptr1, hh_u32 byteCount1)
	{
		return IsAliased(reinterpret_cast<const hh_u8*>(ptr0), byteCount0, reinterpret_cast<const hh_u8*>(ptr1), byteCount1);
	}

	template<typename _Type>
	HH_FORCEINLINE _Type	AdvanceRawPointer(_Type ptr, int advanceInBytes) { return (_Type)((hh_u8*)ptr + advanceInBytes); }

	template<typename _Type>
	HH_FORCEINLINE _Type	*_RefToPtr(_Type &object)
	{
		return &object;
	};

	// That's legit (HH_PLACEMENT_NEW(..., _Type(...)) with _Type=SomeType*), just have to be careful
//	template<typename _Type>
//	_Type					*_RefToPtr(_Type *&object)
//	{
//		HH_STATIC_ASSERT_NOT_REACHED_CUSTOM("Expecting a reference, got a pointer");
//		return object;
//	};

}

//----------------------------------------------------------------------------

#define	HH_ASSERT_ALIGNED4(__memPtr)	HH_ASSERT_MESSAGE(PopcornFX::Mem::IsAligned<0x04>((__memPtr)), "Unaligned memory! %p should be 4-bytes aligned." COMMA (void*)hh_ureg(__memPtr))
#define	HH_ASSERT_ALIGNED8(__memPtr)	HH_ASSERT_MESSAGE(PopcornFX::Mem::IsAligned<0x08>((__memPtr)), "Unaligned memory! %p should be 8-bytes aligned." COMMA (void*)hh_ureg(__memPtr))
#define	HH_ASSERT_ALIGNED16(__memPtr)	HH_ASSERT_MESSAGE(PopcornFX::Mem::IsAligned<0x10>((__memPtr)), "Unaligned memory! %p should be 16-bytes aligned." COMMA (void*)hh_ureg(__memPtr))
#define	HH_ASSERT_ALIGNED32(__memPtr)	HH_ASSERT_MESSAGE(PopcornFX::Mem::IsAligned<0x20>((__memPtr)), "Unaligned memory! %p should be 32-bytes aligned." COMMA (void*)hh_ureg(__memPtr))
#define	HH_ASSERT_ALIGNED64(__memPtr)	HH_ASSERT_MESSAGE(PopcornFX::Mem::IsAligned<0x40>((__memPtr)), "Unaligned memory! %p should be 64-bytes aligned." COMMA (void*)hh_ureg(__memPtr))
#define	HH_ASSERT_ALIGNED128(__memPtr)	HH_ASSERT_MESSAGE(PopcornFX::Mem::IsAligned<0x80>((__memPtr)), "Unaligned memory! %p should be 128-bytes aligned." COMMA (void*)hh_ureg(__memPtr))

//----------------------------------------------------------------------------
//
//	Memory barriers
//
//----------------------------------------------------------------------------

#if	defined(HH_IOS) || defined(HH_MACOSX)
//----------------------------------------------------------------------------
//
//	iOs / MacOsX
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		__asm__ __volatile__("" : : : "memory");	// assume we're using a GCC
	}

	HH_FORCEINLINE void	FullBarrier()
	{
#	ifdef PK_APPLE_USE_STD_ATOMIC_FENCE
		std::atomic_thread_fence(std::memory_order_acq_rel);
#	else
		OSMemoryBarrier();
#	endif
	}
}

#elif	(HH_CPU_MODEL == HH_CPU_ESPRESSO)
//----------------------------------------------------------------------------
//
//	Wii-U
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		// do nothing
	}

	HH_FORCEINLINE void	FullBarrier()
	{
		OSMemoryBarrier();
//		__LWSYNC();
	}
}

#elif	(HH_CPU_ARCH == HH_CPU_PPC) || (HH_CPU_MODEL == HH_CPU_CELL_PPU) || (HH_CPU_MODEL == HH_CPU_XENON)
//----------------------------------------------------------------------------
//
//	PPC / GCC / Cell-PPU
//	VC++ / X360
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
#	if	defined(HH_COMPILER_GCC)
		__asm__ __volatile__("" : : : "memory");
#	endif
	}

	HH_FORCEINLINE void	FullBarrier()
	{
#if	(HH_CPU_MODEL != HH_CPU_CELL_SPU)
		__lwsync();	// ~35-50 cycles
#endif
	}
}

#elif	(HH_CPU_ARCH == HH_CPU_ARM)
//----------------------------------------------------------------------------
//
//	PlayStation-Vita / Android / iOs
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		__asm__ __volatile__("" : : : "memory");
	}

	HH_FORCEINLINE void	FullBarrier()
	{
		// Note: this instruction won't exist if compiling for ARM below v7 or v6
#	if	defined(HH_PSP2)
		__asm__ __volatile__ ("dmb" : : : "memory");
#	else
		__asm__ __volatile__ ("dmb ish" : : : "memory");
#	endif
	}
}

#elif	defined(HH_COMPILER_MSVC)
//----------------------------------------------------------------------------
//
//	PC / MSVC
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		_ReadWriteBarrier();	// compiler reorder barrier
	}

	HH_FORCEINLINE void	FullBarrier()
	{
		_ReadWriteBarrier();	// compiler reorder barrier
		_mm_mfence();
	}
}

#elif	defined(HH_COMPILER_ICC)
//----------------------------------------------------------------------------
//
//	ICC generic
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		_ReadWriteBarrier();
//		__memory_barrier();
	}

	HH_FORCEINLINE void	FullBarrier()
	{
		_mm_mfence();
//		__memory_barrier();
//		//__sync_synchronize();
	}
}

#elif	defined(HH_COMPILER_GCC)
//----------------------------------------------------------------------------
//
//	GCC generic
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		__asm__ __volatile__("" : : : "memory");
	}

	HH_FORCEINLINE void	FullBarrier()
	{
#	if	(HH_CPU_ARCH == HH_CPU_X86)	// x86 / x86-64
		__asm__ __volatile__("mfence" : : : "memory");
#	else
		__sync_synchronize();
#	endif
	}
}

#else

//----------------------------------------------------------------------------
//
//	generic / unknown arch
//
//----------------------------------------------------------------------------

namespace	Memory
{
	HH_FORCEINLINE void	CompilerBarrier()
	{
		// do nothing
	}

	HH_FORCEINLINE void	FullBarrier()
	{
		// do nothing
	}
}

#endif

//----------------------------------------------------------------------------
//
//	Prefetching helpers
//
//	These allow to handle cache-related issues. There are two main groups:
//
//	- Prefetch
//		classic cache-line prefetch. emits prefetching instructions on CPUs that support them
//		- PrefetchCacheLine	: prefetches a single cache-line
//		- Prefetch<>		: prefetches cache lines touched by a specific byte range (passed in template parameter)
//
//	- PredictablePrefetch
//		provides the same utilities as the Prefetch functions. map to No-Op on x86-family CPUs.
//		These should be used in places where memory access patterns are hardware prefetch friendly.
//		Using manual prefetches on these acccess patterns will usually lead worse performance than doing nothing and leaving the
//		hardware prefetcher predict the next accessed cache lines and automatically prefetch them.
//		Usually, this happens when reading/writing continous arrays of bytes (ie: iterating over contiguous arrays with a fixed stride).
//
//----------------------------------------------------------------------------

#if	(HH_CPU_ARCH == HH_CPU_PPC) || (HH_CPU_MODEL == HH_CPU_CELL_PPU) || (HH_CPU_MODEL == HH_CPU_XENON)
//----------------------------------------------------------------------------
//
//	PPC / GCC / Cell-PPU
//	VC++ / X360
//
//----------------------------------------------------------------------------

namespace	Memory
{
	enum
	{
		CacheLineSize = 128		// 128 bytes cache lines
	};

#if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)

	HH_FORCEINLINE void									PrefetchCacheLine(const void *, hh_u32 = 0) {}
	HH_FORCEINLINE void									PredictablePrefetchCacheLine(const void *, hh_u32 = 0) {}
	template<hh_u32 _ByteCount> HH_FORCEINLINE void		Prefetch(const void *ptr, hh_u32 offset = 0) {}
	template<hh_u32 _ByteCount> HH_FORCEINLINE void		PredictablePrefetch(const void *ptr, hh_u32 offset = 0) {}
	HH_FORCEINLINE void									WriteBarrier() {}

#else

#	if	(HH_CPU_MODEL == HH_CPU_CELL_PPU)
#		define	HH_PREFETCH(__ptr, __offset)	__dcbt(static_cast<const char*>(__ptr) + (__offset))
#	else
#		define	HH_PREFETCH(__ptr, __offset)	__dcbt(__offset, __ptr)
#	endif

	HH_FORCEINLINE void		PrefetchCacheLine(const void *ptr, hh_u32 offset = 0)
	{
		HH_PREFETCH(ptr, offset);
	}

	HH_FORCEINLINE void		PredictablePrefetchCacheLine(const void *ptr, hh_u32 offset = 0)
	{
		HH_PREFETCH(ptr, offset);
	}

	template<hh_u32 _ByteCount>
	HH_FORCEINLINE void		Prefetch(const void *ptr, hh_u32 offset = 0)
	{
		HH_STATIC_ASSERT(_ByteCount <= 512);		// max 512 bytes prefetches

		const char	*optr8 = static_cast<const char*>(ptr) + offset;
		HH_PREFETCH(optr8, 0);						// 128b prefetch
		// the boolean expression is known at compile time, all the following branches should be removed in optimized builds:
		if (_ByteCount > CacheLineSize*1)			// 256b prefetch
			HH_PREFETCH(optr8, CacheLineSize*1);
		if (_ByteCount > CacheLineSize*2)			// 384b prefetch
			HH_PREFETCH(optr8, CacheLineSize*2);
		if (_ByteCount > CacheLineSize*3)			// 512b prefetch
			HH_PREFETCH(optr8, CacheLineSize*3);
	}

	template<hh_u32 _ByteCount>
	HH_FORCEINLINE void		PredictablePrefetch(const void *ptr, hh_u32 offset = 0)
	{
		Prefetch<_ByteCount>(ptr, offset);
	}

	HH_FORCEINLINE void		WriteBarrier() {}

#	undef	HH_PREFETCH
#endif
}


#elif	(HH_CPU_ARCH == HH_CPU_ARM) || (HH_CPU_MODEL == HH_CPU_ESPRESSO)
//----------------------------------------------------------------------------
//
//	PlayStation-Vita / Wii-U / Android / iOs
//
//----------------------------------------------------------------------------

namespace	Memory
{
	enum
	{
		CacheLineSize = 32		// 32 bytes cache lines
	};

#if	(HH_CPU_MODEL == HH_CPU_ESPRESSO)
#	define	HH_PREFETCH(__ptr, __offset)	__dcbt((void*)__ptr, __offset)
#elif	defined(HH_COMPILER_SNC)
#	define	HH_PREFETCH(__ptr, __offset)	__builtin_pld(static_cast<const char*>(__ptr) + (__offset))
#elif	defined(HH_COMPILER_GCC)
#	if (HH_CPU_MODEL == HH_CPU_ARM64)
#		if (defined(__has_builtin) && __has_builtin(__builtin_prefetch))
#			define HH_PREFETCH(__ptr, __offset) __builtin_prefetch(static_cast<const char *>(__ptr) + (__offset));
#		else
#			define HH_PREFETCH(__ptr, __offset)	(static_cast<const char *>(__ptr) + __offset); // TODO: Implement memory prefetch
#		endif
#	else
#		define HH_PREFETCH(__ptr, __offset)		asm("PLD [%0, #32]"::"r" (static_cast<const char *>(__ptr) + (__offset)))
#	endif
#else
#	error	?
#	define	HH_PREFETCH(__ptr, __offset)
#endif

	HH_FORCEINLINE void		PrefetchCacheLine(const void *ptr, hh_u32 offset = 0)
	{
		HH_PREFETCH(ptr, offset);
	}

	HH_FORCEINLINE void		PredictablePrefetchCacheLine(const void *ptr, hh_u32 offset = 0)
	{
		HH_PREFETCH(ptr, offset);
	}

	template<hh_u32 _ByteCount>
	HH_FORCEINLINE void		Prefetch(const void *ptr, hh_u32 offset = 0)
	{
		HH_STATIC_ASSERT(_ByteCount <= 256);		// max 256 bytes prefetches

		const char	*optr8 = static_cast<const char*>(ptr) + offset;
		HH_PREFETCH(optr8, 0);						// 32b prefetch
		// the boolean expression is known at compile time, all the following branches should be removed in optimized builds:
		if (_ByteCount > CacheLineSize*1)			// 64b prefetch
			HH_PREFETCH(optr8, CacheLineSize*1);
		if (_ByteCount > CacheLineSize*2)			// 96b prefetch
			HH_PREFETCH(optr8, CacheLineSize*2);
		if (_ByteCount > CacheLineSize*3)			// 128b prefetch
			HH_PREFETCH(optr8, CacheLineSize*3);
		if (_ByteCount > CacheLineSize*4)			// 160b prefetch
			HH_PREFETCH(optr8, CacheLineSize*4);
		if (_ByteCount > CacheLineSize*5)			// 192b prefetch
			HH_PREFETCH(optr8, CacheLineSize*5);
		if (_ByteCount > CacheLineSize*6)			// 224b prefetch
			HH_PREFETCH(optr8, CacheLineSize*6);
		if (_ByteCount > CacheLineSize*7)			// 256b prefetch
			HH_PREFETCH(optr8, CacheLineSize*7);
	}

	template<hh_u32 _ByteCount>
	HH_FORCEINLINE void		PredictablePrefetch(const void *ptr, hh_u32 offset = 0)
	{
		Prefetch<_ByteCount>(ptr, offset);
	}

	HH_FORCEINLINE void		WriteBarrier() {}

#undef	HH_PREFETCH
}

//----------------------------------------------------------------------------

#elif		(HH_CPU_MODEL == HH_CPU_X64) || \
			((HH_CPU_MODEL == HH_CPU_X86) && ( \
										(defined(HH_COMPILER_MSVC) && defined(_M_IX86_FP)) || \
										defined(__SSE__) \
									) \
			) || \
			(defined(HH_COMPILER_GCC) && !defined(HH_COMPILER_SNC))

//----------------------------------------------------------------------------
//
//	PC / SSE
//	GCC-Generic
//
//----------------------------------------------------------------------------

namespace	Memory
{
	enum
	{
#ifndef	HH_FORCE_CACHE_LINE_SIZE
		CacheLineSize = 64		// 64 bytes cache lines
#else
		CacheLineSize = HH_FORCE_CACHE_LINE_SIZE
#endif
	};

#if	!defined(HH_COMPILER_GCC) || defined(__SSE__)
#	define	HH_PREFETCH(__ptr, __offset)	_mm_prefetch(static_cast<const char*>(__ptr) + (__offset), _MM_HINT_T0)
#else
#	define	HH_PREFETCH(__ptr, __offset)	__builtin_prefetch(static_cast<const char*>(__ptr) + (__offset), 0, 1)
#endif

	HH_FORCEINLINE void		PrefetchCacheLine(const void *ptr, hh_u32 offset = 0)
	{
		HH_PREFETCH(ptr, offset);
	}

	HH_FORCEINLINE void		PredictablePrefetchCacheLine(const void *ptr, hh_u32 offset = 0)
	{
		// let the CPU prefetch this itself
#if	(HH_CPU_ARCH != HH_CPU_X86)
		HH_PREFETCH(ptr, offset);
#endif
	}

	template<hh_u32 _ByteCount>
	HH_FORCEINLINE void		Prefetch(const void *ptr, hh_u32 offset = 0)
	{
		HH_STATIC_ASSERT(_ByteCount <= 512);			// max 512 bytes prefetches
		static const hh_u32	CacheLineSize = 64;		// assume 64 bytes cache lines

		const char	*optr8 = static_cast<const char*>(ptr) + offset;
		HH_PREFETCH(optr8, 0);		// 64b prefetch
		// the boolean expression is known at compile time, all the following branches should be removed in optimized builds:
		if (_ByteCount > CacheLineSize*1)		// 128b prefetch
			HH_PREFETCH(optr8, CacheLineSize*1);
		if (_ByteCount > CacheLineSize*2)		// 192b prefetch
			HH_PREFETCH(optr8, CacheLineSize*2);
		if (_ByteCount > CacheLineSize*3)		// 256b prefetch
			HH_PREFETCH(optr8, CacheLineSize*3);
		if (_ByteCount > CacheLineSize*4)		// 320b prefetch
			HH_PREFETCH(optr8, CacheLineSize*4);
		if (_ByteCount > CacheLineSize*5)		// 384b prefetch
			HH_PREFETCH(optr8, CacheLineSize*5);
		if (_ByteCount > CacheLineSize*6)		// 448b prefetch
			HH_PREFETCH(optr8, CacheLineSize*6);
		if (_ByteCount > CacheLineSize*7)		// 512b prefetch
			HH_PREFETCH(optr8, CacheLineSize*7);
	}

	template<hh_u32 _ByteCount>
	HH_FORCEINLINE void		PredictablePrefetch(const void *ptr, hh_u32 offset = 0)
	{
#if	(HH_CPU_ARCH != HH_CPU_X86)
		Prefetch<_ByteCount>(ptr, offset);
#endif
	}

	HH_FORCEINLINE void		WriteBarrier()
	{
#if	!defined(HH_COMPILER_GCC) || defined(__SSE__)
		_mm_sfence();
#endif
	}
#undef	HH_PREFETCH
}

//----------------------------------------------------------------------------

#else

//----------------------------------------------------------------------------
//
//	generic/unknown arch: no prefetching
//
//----------------------------------------------------------------------------

namespace	Memory
{
	enum
	{
#ifndef	HH_FORCE_CACHE_LINE_SIZE
		CacheLineSize = 64		// 64 bytes cache lines
#else
		CacheLineSize = HH_FORCE_CACHE_LINE_SIZE
#endif
	};

	HH_FORCEINLINE void									PrefetchCacheLine(const void *, hh_u32 = 0) {}
	HH_FORCEINLINE void									PredictablePrefetchCacheLine(const void *, hh_u32 = 0) {}
	template<hh_u32 _ByteCount> HH_FORCEINLINE void		Prefetch(const void *ptr, hh_u32 offset = 0) {}
	template<hh_u32 _ByteCount> HH_FORCEINLINE void		PredictablePrefetch(const void *ptr, hh_u32 offset = 0) {}
	HH_FORCEINLINE void									WriteBarrier() {}
}

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __HH_MEMORY_H__

