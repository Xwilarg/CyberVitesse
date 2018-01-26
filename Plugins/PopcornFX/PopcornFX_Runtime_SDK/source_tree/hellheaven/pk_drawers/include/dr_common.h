#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/01/08 11:34 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include <hh_kernel/include/kr_memoryviews.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace Drawers
{
	struct	SIndexStream
	{
		void			*m_Indices;
		hh_u32			m_Count;
		bool			m_Large; // true: u32, false: u16

		SIndexStream() : m_Indices(null), m_Count(0), m_Large(false) { }
		SIndexStream(void *indices, hh_u32 count, bool large) : m_Indices(indices), m_Count(count), m_Large(large) { }

		bool			Setup(void *indices, hh_u32 count, bool large) { m_Indices = indices; m_Count = count; m_Large = large; return m_Indices != null && m_Count > 0; }
		void			Clear() { m_Indices = null; m_Count = 0; m_Large = false; }
		bool			Empty() const { return m_Indices == null || m_Count == 0; }
		bool			LargeIndices() const { return m_Large; }
		bool			Large() const { return LargeIndices(); }
		hh_u32			Stride() const { return LargeIndices() ? sizeof(hh_u32) : sizeof(hh_u16); }
		hh_u32			Count() const { return m_Count; }
		hh_u32			CoveredBytes() const { return Count() * Stride(); }
		void			*RawData() const { return m_Indices; }

		template <typename _IndexType>
		HH_FORCEINLINE _IndexType		*Data(hh_u32 start = 0) const
		{
			HH_ASSERT(sizeof(_IndexType) == Stride());
			HH_ASSERT(start < Count());
			return reinterpret_cast<_IndexType*>(m_Indices) + start;
		}

		template <typename _IndexType>
		HH_FORCEINLINE TMemoryView<_IndexType>		View() const { return TMemoryView<_IndexType>(Data<_IndexType>(), m_Count); }
		template <typename _IndexType>
		HH_FORCEINLINE TMemoryView<_IndexType>		View(hh_u32 start, hh_u32 count) const
		{
			HH_ASSERT(start + count <= Count());
			return TMemoryView<_IndexType>(Data<_IndexType>(start), count);
		}

		HH_FORCEINLINE hh_u32			Get(hh_u32 index) const
		{
			HH_ASSERT(index < m_Count);
			if (m_Large)
				return reinterpret_cast<const hh_u32*>(m_Indices)[index];
			else
				return reinterpret_cast<const hh_u16*>(m_Indices)[index];
		}
	};

	void		UniformScaleStream(float scale, const TStridedMemoryView<CFloat3, 0x10> &stream);

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
