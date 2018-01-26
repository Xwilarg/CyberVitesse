#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2013/07/31 11:14 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_WORKINGBUFFER_H__
#define	__KR_WORKINGBUFFER_H__

#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_mem.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<hh_u32 _Alignment = 0, hh_u32 _ShiftGrow = 2>
struct	TWorkingBufferController
{
	HH_INLINE static bool			DoClean(hh_u32 askedToClearCount)
	{
		return askedToClearCount > 1000; // 16 seconds at 60 fps
	}
	HH_INLINE static hh_u32			GrowSize(hh_u32 newSize)
	{
		return newSize + (newSize >> _ShiftGrow);
	}
	HH_FORCEINLINE static hh_u32	Alignment() { return _Alignment; }
};

//----------------------------------------------------------------------------

template<typename _Controller>
class	TWorkingBuffer : public CNonCopyable
{
private:
	// not thread-safe !
	// mutable only for const Get()
	mutable void		*m_Buffer;
	mutable hh_u32		m_BufferSize;
	mutable hh_u32		m_AskedToClearCount;

public:
	// Scoped allocation and deletion

	TWorkingBuffer()
	:	m_Buffer(null)
	,	m_BufferSize(0)
	,	m_AskedToClearCount(0)
	{
	}

	~TWorkingBuffer()
	{
		if (m_Buffer)
			HH_FREE(m_Buffer);
	}

	HH_INLINE hh_u32			Capacity() const { return m_BufferSize; }

	HH_INLINE void				Clear() const
	{
		if (_Controller::DoClean(++m_AskedToClearCount))
			Clean();
	}

	HH_INLINE void				Clean() const
	{
		HH_FREE(m_Buffer);
		m_Buffer = null;
		m_BufferSize = 0;
		m_AskedToClearCount = 0;
	}

	HH_INLINE void				*Get() const
	{
		return m_Buffer;
	}

	template<typename _Type>
	HH_INLINE _Type				*GetAs() const
	{
		return reinterpret_cast<_Type*>(m_Buffer);
	}

	HH_INLINE void				*Get(hh_u32 sizeInBytes) const
	{
		if (sizeInBytes == 0)
		{
			Clear();
			return null;
		}
		m_AskedToClearCount = 0;
		if (sizeInBytes > m_BufferSize)
		{
			m_BufferSize = _Controller::GrowSize(sizeInBytes);
			m_Buffer = HH_REALLOC_ALIGNED(m_Buffer, m_BufferSize, _Controller::Alignment());
		}
		return m_Buffer;
	}

	template<typename _Type>
	HH_INLINE _Type				*GetSome(hh_u32 elementCount) const
	{
		return reinterpret_cast<_Type*>(Get(elementCount * sizeof(_Type)));
	}

	template<typename _Type>
	HH_INLINE TStridedMemoryView<_Type>	GetView(hh_u32 elementCount, hh_u32 stride) const
	{
		HH_ASSERT(sizeof(_Type) <= stride);
		return TStridedMemoryView<_Type>(reinterpret_cast<_Type*>(Get(elementCount * stride)), elementCount, stride);
	}

	template<typename _Type>
	HH_INLINE TMemoryView<_Type>		GetView(hh_u32 elementCount) const
	{
		return TMemoryView<_Type>(reinterpret_cast<_Type*>(Get(elementCount * sizeof(_Type))), elementCount);
	}

};

typedef TWorkingBuffer<TWorkingBufferController<16, 2> >	CWorkingBuffer;

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_WORKINGBUFFER_H__
