#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2012/04/25 16:35 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ARRAY_CHUNKED_INL__
#define __KR_CONTAINERS_ARRAY_CHUNKED_INL__

#ifndef	__KR_CONTAINERS_ARRAY_H__
#	error	kr_containers_array_slot.inl must not be included manually, include kr_containers_array_slot.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	TChunkedArray
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE void		TChunkedArray<_Type, _ChunkSize, _Controller>::_RemapFromVirtualIndex(CGuid index, hh_u32 &chunkId, hh_u32 &subOffset)
{
	HH_ASSERT(index.Valid());
	subOffset = index & _ChunkMask;
	chunkId = index >> _ChunkShift;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE CGuid	TChunkedArray<_Type, _ChunkSize, _Controller>::_RemapToVirtualIndex(hh_u32 chunkId, hh_u32 subOffset)
{
	HH_ASSERT(subOffset < _ChunkSize);
	return ((chunkId << _ChunkShift) + subOffset);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE _Type	*TChunkedArray<_Type, _ChunkSize, _Controller>::_DerefVirtualIndex(CGuid index) const
{
	hh_u32	chunkId;
	hh_u32	subOffset;
	_RemapFromVirtualIndex(index, chunkId, subOffset);
	_Type	*chunk = m_Chunks[chunkId];
	return chunk + subOffset;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
_Type		*TChunkedArray<_Type, _ChunkSize, _Controller>::_RawAllocNewChunk()
{
	_Type	*newChunk = HH_TALLOC<_Type>(_ChunkSize, _Controller::Alignment());
	if (newChunk == null)
	{
		HH_ASSERT_NOT_REACHED();
		return null;
	}

	if (m_ChunksCount >= m_ChunksCapacity)
	{
		const hh_u32	newChunkListSize = m_ChunksCount + _ChunkSize;
		_Type			**newChunkList = reinterpret_cast<_Type**>(HH_MALLOC_ALIGNED(newChunkListSize * sizeof(_Type*), _Controller::Alignment()));
		if (newChunkList == null)
		{
			HH_ASSERT_NOT_REACHED();
			HH_FREE(newChunk);
			return null;
		}
		if (m_Chunks != null)
			Mem::Copy(newChunkList, m_Chunks, m_ChunksCount * sizeof(_Type*));

		_Type	**oldChunkList = m_Chunks;
		m_Chunks = newChunkList;
		if (m_PrevChunks != null)
			HH_FREE(m_PrevChunks);
		m_PrevChunks = oldChunkList;
		m_ChunksCapacity = newChunkListSize;
	}

	m_Chunks[m_ChunksCount++] = newChunk;
	return newChunk;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
TChunkedArray<_Type, _ChunkSize, _Controller>::TChunkedArray()
:	m_Chunks(null)
,	m_PrevChunks(null)
,	m_ChunksCount(0)
,	m_ChunksCapacity(0)
{
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
TChunkedArray<_Type, _ChunkSize, _Controller>::~TChunkedArray()
{
	Clean();
	if (m_Chunks != null)
		HH_FREE(m_Chunks);
	if (m_PrevChunks != null)
		HH_FREE(m_PrevChunks);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE const _Type	&TChunkedArray<_Type, _ChunkSize, _Controller>::operator [] (CGuid slot) const
{
	return *_DerefVirtualIndex(slot);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE _Type		&TChunkedArray<_Type, _ChunkSize, _Controller>::operator [] (CGuid slot)
{
	return *_DerefVirtualIndex(slot);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
bool		TChunkedArray<_Type, _ChunkSize, _Controller>::Resize(hh_u32 newSize)
{
	const hh_u32	targetChunkCount = (newSize + _ChunkSize - 1) / _ChunkSize;	// round-up
	while (m_ChunksCount < targetChunkCount)
	{
		_Type	*newChunk = _RawAllocNewChunk();
		if (newChunk == null)
			return false;

		for (hh_u32 i = 0; i < _ChunkSize; i++)
		{
			Mem::Construct(newChunk[i]);
		}
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
template<typename _Type2>
HH_INLINE CGuid		TChunkedArray<_Type, _ChunkSize, _Controller>::IndexOf(_Type2 const &what) const
{
	for (hh_u32 c = 0; c < m_ChunksCount; c++)
	{
		const _Type	*chunk = m_Chunks[c];
		for (hh_u32 i = 0; i < _ChunkSize; i++)
		{
			if (chunk[i] == what)
				return _RemapToVirtualIndex(c, i);
		}
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
template<typename _Type2>
HH_INLINE bool		TChunkedArray<_Type, _ChunkSize, _Controller>::Contains(_Type2 const &what) const
{
	return IndexOf(what).Valid();
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
void	TChunkedArray<_Type, _ChunkSize, _Controller>::Clean()
{
	if (m_Chunks != null)
	{
		for (hh_u32 i = 0; i < m_ChunksCount; i++)
		{
			_Type	*chunk = m_Chunks[i];
			HH_ASSERT(chunk != null);
			Internal::TRangeDestructor<_Type>::Destroy(chunk, 0, _ChunkSize);
			Mem::Free(chunk);
		}
		HH_FREE(m_Chunks);
		m_Chunks = null;
		m_ChunksCapacity = 0;
		m_ChunksCount = 0;

		if (m_PrevChunks != null)
			HH_FREE(m_PrevChunks);
		m_PrevChunks = null;
	}
	HH_ASSERT(m_ChunksCount == 0);
}

//----------------------------------------------------------------------------
//
//	TChunkedSlotArray
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
bool		TChunkedSlotArray<_Type, _ChunkSize, _Controller>::_AllocFreeSlotIFP(hh_u32 &chunkId, hh_u32 &subOffset)
{
	HH_ASSERT(m_FreeSlots.Empty());

	// no space left, we need to allocate a new chunk
	_Type	*newChunk = ParentType::_RawAllocNewChunk();
	if (newChunk == null)
		return false;

	chunkId = ParentType::m_ChunksCount - 1;
	subOffset = 0;

	if (!m_FreeSlots.Resize(m_FreeSlots.Count() + _ChunkSize - 1))
	{
		HH_ASSERT_NOT_REACHED();
		// undo insertion
		HH_ASSERT(ParentType::m_Chunks[chunkId] == newChunk);
		ParentType::m_Chunks[chunkId] = null;
		--ParentType::m_ChunksCount;
		HH_FREE(newChunk);
		return false;
	}

	ParentType::m_Chunks[chunkId] = newChunk;
	for (hh_u32 i = 0; i < _ChunkSize; i++)
	{
		Mem::Construct(newChunk[i], TSlotArrayInvalidExtractor<_Type>::Invalid());
	}

	// we're going to PopBack() free chunk IDs, we want the successive inserts to use the lower slots,
	// so order the free slots with the lower IDs towards the end:
	const hh_u32	firstSlot = ParentType::_RemapToVirtualIndex(chunkId, 0);
	const hh_u32	lastSlot = firstSlot + _ChunkSize - 1;

	hh_u32	*slots = &m_FreeSlots.Last() + firstSlot + 1;
	for (hh_u32 i = firstSlot; ++i <= lastSlot; )
	{
		(slots - i)[0] = i;
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
TChunkedSlotArray<_Type, _ChunkSize, _Controller>::TChunkedSlotArray()
:	m_UsedSlots(0)
{
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
TChunkedSlotArray<_Type, _ChunkSize, _Controller>::~TChunkedSlotArray()
{
	Clean();
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE const _Type	&TChunkedSlotArray<_Type, _ChunkSize, _Controller>::operator [] (CGuid slot) const
{
	return *ParentType::_DerefVirtualIndex(slot);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
HH_FORCEINLINE _Type		&TChunkedSlotArray<_Type, _ChunkSize, _Controller>::operator [] (CGuid slot)
{
	return *ParentType::_DerefVirtualIndex(slot);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
CGuid		TChunkedSlotArray<_Type, _ChunkSize, _Controller>::Insert(const _Type &element)
{
	hh_u32	chunkId;
	hh_u32	subOffset;
	if (!m_FreeSlots.Empty())
	{
		ParentType::_RemapFromVirtualIndex(m_FreeSlots.PopBack(), chunkId, subOffset);
	}
	else if (!_AllocFreeSlotIFP(chunkId, subOffset))
	{
		return CGuid::INVALID;
	}

	_Type	*slot = ParentType::m_Chunks[chunkId] + subOffset;
	*slot = element;
	m_UsedSlots++;
	return ParentType::_RemapToVirtualIndex(chunkId, subOffset);
}

//----------------------------------------------------------------------------

#if	(PK_HAS_RVALUE_REF != 0)
template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
CGuid		TChunkedSlotArray<_Type, _ChunkSize, _Controller>::Insert(_Type &&element)
{
	hh_u32	chunkId;
	hh_u32	subOffset;
	if (!m_FreeSlots.Empty())
	{
		ParentType::_RemapFromVirtualIndex(m_FreeSlots.PopBack(), chunkId, subOffset);
	}
	else if (!_AllocFreeSlotIFP(chunkId, subOffset))
	{
		return CGuid::INVALID;
	}

	_Type	*slot = ParentType::m_Chunks[chunkId] + subOffset;
	*slot = element;
	m_UsedSlots++;
	return ParentType::_RemapToVirtualIndex(chunkId, subOffset);
}
#endif

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
void		TChunkedSlotArray<_Type, _ChunkSize, _Controller>::Remove(CGuid slot)
{
	HH_ASSERT(slot.Valid() && !m_FreeSlots.Contains(slot));
	m_FreeSlots.PushBack(slot);
	_Type	*data = ParentType::_DerefVirtualIndex(slot);
	TSlotArrayInvalidExtractor<_Type>::Invalidate(*data);
	m_UsedSlots--;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
template<typename _Type2>
HH_INLINE CGuid		TChunkedSlotArray<_Type, _ChunkSize, _Controller>::IndexOf(_Type2 const &what) const
{
	hh_u32	activeSlotsScanned = 0;
	for (hh_u32 c = 0, stop = ParentType::m_ChunksCount; c < stop; c++)
	{
		const _Type	*chunk = ParentType::m_Chunks[c];
		for (hh_u32 i = 0; i < _ChunkSize; i++)
		{
			const _Type	&element = chunk[i];
			if (TSlotArrayInvalidExtractor<_Type>::IsValid(element))
			{
				if (element == what)
					return ParentType::_RemapToVirtualIndex(c, i);
				if (++activeSlotsScanned >= m_UsedSlots)
					return CGuid::INVALID;
			}
		}
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
template<typename _Type2>
HH_INLINE bool		TChunkedSlotArray<_Type, _ChunkSize, _Controller>::Contains(_Type2 const &what) const
{
	return IndexOf(what).Valid();
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize, typename _Controller>
void	TChunkedSlotArray<_Type, _ChunkSize, _Controller>::Clear()
{
	if (Empty())	// already cleared, quick exit
		return;

	HH_ASSERT(m_FreeSlots.Count() + UsedCount() == Count());
	for (hh_u32 i = 0, stop = ParentType::m_ChunksCount; i < stop; i++)
	{
		_Type	*chunk = ParentType::m_Chunks[i];
		HH_ASSERT(chunk != null);
		for (hh_u32 j = 0; j < _ChunkSize; j++)
		{
			if (TSlotArrayInvalidExtractor<_Type>::IsValid(chunk[j]))
			{
				TSlotArrayInvalidExtractor<_Type>::Invalidate(chunk[j]);
				HH_ASSERT(!TSlotArrayInvalidExtractor<_Type>::IsValid(chunk[j]));
				const CGuid	slot = ParentType::_RemapToVirtualIndex(i, j);
				HH_ASSERT(slot.Valid() && !m_FreeSlots.Contains(slot));
				m_FreeSlots.PushBack(slot);
			}
		}
	}
	m_UsedSlots = 0;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ARRAY_CHUNKED_INL__
