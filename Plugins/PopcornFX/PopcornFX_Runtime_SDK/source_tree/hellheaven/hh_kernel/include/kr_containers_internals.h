#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/10/23 12:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_INTERNALS_H__
#define __KR_CONTAINERS_INTERNALS_H__

#include "hh_kernel/include/kr_mem_basics.h"
#include "hh_kernel/include/kr_mem_utils.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Internal
{
	template<typename _Type, bool _HasTrivialRelocate = TTypeTraits::HasTrivialRelocate<_Type>::True>
	struct	SRelocationHelper
	{
		static HH_FORCEINLINE void	Relocate(_Type *dst, const _Type *src, hh_u32 count)
		{
			// Help compilers that do not have a good-enough memcpy intrinsic, or for whom memcpy is not an actual intrinsic
			// TODO: actually detect these with #ifdef ?
			if (sizeof(_Type) > 64)	// for more than 16 32-bits block copies, use memcpy
			{
				// explicit C-casts to void* : silence compiler warnings. We know we're potentially copying vtable pointers.
				// non-relocatable objects should specialize the 'TTypeTraits::HasTrivialRelocate<>' template with 'TTypeTraits::FalseType'
				memcpy((void*)dst, (const void*)src, count * sizeof(_Type));
			}
			else
			{
				hh_u8		*rdst = reinterpret_cast<hh_u8*>(dst);
				const hh_u8	*rsrc = reinterpret_cast<const hh_u8*>(src);
				for (hh_i32 i = 0, stop = count * sizeof(_Type); i < stop; i++)	// should be unrolled and optimized away for small types
					rdst[i] = rsrc[i];
			}
		}

		static HH_FORCEINLINE void	Relocate_OverlappedCompact(_Type *dst, const _Type *src, hh_u32 count)
		{
			HH_ASSERT(dst < src);	// otherwise, should call Relocate_OverlappedExpand
			HH_ASSERT(hh_ireg(src) - hh_ireg(dst) >= hh_ireg(sizeof(_Type)) && count != 0);	// invalid to overlap more than the sizeof type (ctor would overlap old type before dtoring)

			Mem::Copy_Overlapped(dst, src, count * sizeof(_Type));
		}

		static HH_FORCEINLINE void	Relocate_OverlappedExpand(_Type *dst, const _Type *src, hh_u32 count)
		{
			HH_ASSERT(src < dst);	// otherwise, should call Relocate_OverlappedExpand
			HH_ASSERT(hh_ireg(dst) - hh_ireg(src) >= hh_ireg(sizeof(_Type)) && count != 0);	// invalid to overlap more than the sizeof type (ctor would overlap old type before dtoring)

			Mem::Copy_Overlapped(dst, src, count * sizeof(_Type));
		}
	};

	template<typename _Type>
	struct	SRelocationHelper<_Type, false>
	{
		// We only have very few non-relocatable objects (see 'TTypeTraits::HasTrivialRelocate<>' specializations)

		static HH_FORCEINLINE void	Relocate(_Type *dst, const _Type *src, hh_u32 count)
		{
			for (hh_u32 i = 0; i < count; ++i)
			{
				Mem::Construct(dst[i], src[i]);
				Mem::Destruct(src[i]);
			}
		}

		static HH_FORCEINLINE void	Relocate_OverlappedCompact(_Type *dst, const _Type *src, hh_u32 count)
		{
			HH_ASSERT(dst < src);	// otherwise, should call Relocate_OverlappedExpand
			HH_ASSERT(hh_ireg(src) - hh_ireg(dst) >= hh_ireg(sizeof(_Type)) && count != 0);	// invalid to overlap more than the sizeof type (ctor would overlap old type before dtoring)

			// target is before source : move forward
			Relocate(dst, src, count);
		}

		static HH_FORCEINLINE void	Relocate_OverlappedExpand(_Type *dst, const _Type *src, hh_u32 count)
		{
			HH_ASSERT(src < dst);	// otherwise, should call Relocate_OverlappedExpand
			HH_ASSERT(hh_ireg(dst) - hh_ireg(src) >= hh_ireg(sizeof(_Type)) && count != 0);	// invalid to overlap more than the sizeof type (ctor would overlap old type before dtoring)

			// target is after source : move backward
			for (hh_i32 i = count; --i > 0; )
			{
				Mem::Construct(dst[i], src[i]);
				Mem::Destruct(src[i]);
			}
		}
	};

	//----------------------------------------------------------------------------

	template<typename _Type, bool _HasNonTrivialCtor = TTypeTraits::HasTrivialConstructor<_Type>::False>
	struct	TRangeConstructor
	{
		static HH_FORCEINLINE void	ConstructDefault(_Type *, hh_u32, hh_u32) {}
	};

	template<typename _Type>
	struct	TRangeConstructor<_Type, true>
	{
		static HH_INLINE void	ConstructDefault(_Type *data, hh_u32 start, hh_u32 end)
		{
			if (end > start)
			{
				hh_i32	i = start - end;
				_Type	*p = data + end;
				while (i < 0)
				{
					Mem::Construct(p[i]);
					i++;
				}
			}
		}
	};

	//----------------------------------------------------------------------------

	template<typename _Type, bool _HasNonTrivialDtor = TTypeTraits::HasTrivialDestructor<_Type>::False>
	struct	TRangeDestructor
	{
		static HH_FORCEINLINE void	Destroy(_Type *data, hh_u32 start, hh_u32 end)
		{
			HH_ASSERT(start <= end && (data != null || start == end));
			(void)data; (void)start; (void)end;	// avoid unused parameter warnings under GCC
		}
	};

	template<typename _Type>
	struct	TRangeDestructor<_Type, true>
	{
		static HH_INLINE void	Destroy(_Type *data, hh_u32 start, hh_u32 end)
		{
			HH_ASSERT(start <= end && (data != null || start == end));
			hh_i32	i = start - end;
			_Type	*p = data + end;
			while (i < 0)
			{
				Mem::Destruct(p[i]);
				i++;
			}
		}
	};

	//----------------------------------------------------------------------------
	
	template<typename _Type, bool _HasNonTrivialDtor = TTypeTraits::HasTrivialDestructor<_Type>::False>
	struct	TRangeResizer
	{
		static HH_FORCEINLINE void	Resize(_Type *, hh_u32, hh_u32) {}
	};

	template<typename _Type>
	struct	TRangeResizer<_Type, true>
	{
		static void	Resize(_Type *data, hh_u32 oldCount, hh_u32 newCount)
		{
			if (oldCount < newCount)	// construct
			{
				hh_i32	i = oldCount - newCount;
				_Type	*p = data + newCount;
				while (i < 0)
				{
					Mem::Construct(p[i]);
					i++;
				}
			}
			else	// destruct
			{
				hh_i32	i = newCount - oldCount;
				_Type	*p = data + oldCount;
				while (i < 0)
				{
					Mem::Destruct(p[i]);
					i++;
				}
			}
		}
	};

	//----------------------------------------------------------------------------

	template<typename _TypeDst, typename _TypeSrc, bool _CanRawCopy = TTypeTraits::HasTrivialConstructor<_TypeDst>::True && TTypeTraits::HasTrivialCopy<_TypeDst>::True && sizeof(_TypeSrc) == sizeof(_TypeDst)>
	struct	TRangeCopier
	{
		static HH_INLINE void	Copy(_TypeDst *dst, const TMemoryView<_TypeSrc> &view, hh_u32 start, hh_u32 end)
		{
			HH_ASSERT(start <= end && end <= view.Count());
			HH_ASSERT(dst != null);
			HH_ASSERT(end - start == 0 || !Mem::Overlaps(dst, (end - start) * sizeof(_TypeDst), &view[start], (end - start) * sizeof(_TypeSrc)));	// Can't overlap

			for (hh_u32 i = start; i < end; i++)
			{
				Mem::Construct<_TypeDst>(dst[i], view[i]);
			}
		}

		static HH_INLINE void	Copy(_TypeDst *dst, const TStridedMemoryViewBase<_TypeSrc> &view, hh_u32 start, hh_u32 end)
		{
			HH_ASSERT(start <= end && end <= view.Count());
			HH_ASSERT(dst != null);
			HH_ASSERT(end - start == 0 || !Mem::Overlaps(dst, (end - start) * sizeof(_TypeDst), &view[start], (end - start) * view.Stride()));	// Can't overlap

			for (hh_u32 i = start; i < end; i++)
			{
				Mem::Construct<_TypeDst>(dst[i], view[i]);
			}
		}
	};

	template<typename _TypeDst, typename _TypeSrc>
	struct	TRangeCopier<_TypeDst, _TypeSrc, true>
	{
		static HH_INLINE void	Copy(_TypeDst *dst, const TMemoryView<_TypeSrc> &view, hh_u32 start, hh_u32 end)
		{
			HH_STATIC_ASSERT(sizeof(_TypeDst) == sizeof(_TypeSrc));	// enforced by the _CanRawCopy template argument
			HH_ASSERT(start <= end && end <= view.Count());
			HH_ASSERT(dst != null);
			HH_ASSERT(!Mem::Overlaps(dst, (end - start) * sizeof(_TypeDst), &view[start], (end - start) * sizeof(_TypeSrc)));	// Can't overlap

			memcpy(dst + start, &(view[0]), (end - start) * sizeof(*dst));
		}

		static HH_INLINE void	Copy(_TypeDst *dst, const TStridedMemoryViewBase<_TypeSrc> &view, hh_u32 start, hh_u32 end)
		{
			HH_STATIC_ASSERT(sizeof(_TypeDst) == sizeof(_TypeSrc));	// enforced by the _CanRawCopy template argument
			HH_ASSERT(start <= end && end <= view.Count());
			HH_ASSERT(dst != null);
			HH_ASSERT(!Mem::Overlaps(dst, (end - start) * sizeof(_TypeDst), &view[start], (end - start) * view.Stride()));	// Can't overlap

			for (hh_u32 i = start; i < end; i++)
			{
				Mem::Construct<_TypeDst>(dst[i], view[i]);
			}
		}
	};

}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_INTERNALS_H__
