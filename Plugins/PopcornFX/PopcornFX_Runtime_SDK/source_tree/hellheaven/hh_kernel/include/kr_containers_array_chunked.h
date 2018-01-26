#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2008/07/28 18:31 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_CHUNKED_ARRAY_H__
#define __KR_CONTAINERS_CHUNKED_ARRAY_H__

#include "hh_kernel/include/kr_containers.h"
#include "hh_kernel/include/kr_containers_array_slot.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize = 128, typename _Controller = TArrayStaticController<> >
class	TChunkedArray
{
public:
	typedef _Type					Type;
	typedef _Controller				Controller;

protected:
	typedef TChunkedArray<_Type, _ChunkSize, _Controller>	SelfType;

	HH_STATIC_ASSERT(_ChunkSize != 0 && TIntegerTemplateTools::IsPowerOfTwo<_ChunkSize>::True);

	// TODO: we could use a pool allocator to alloc each chunk... as they have strictly the same size...
	static const hh_u32				_ChunkShift = TIntegerTemplateTools::Log2<_ChunkSize>::Value;
	static const hh_u32				_ChunkMask = _ChunkSize - 1;

	HH_STATIC_ASSERT(_ChunkSize == (1U << _ChunkShift));

	_Type							**m_Chunks;
	_Type							**m_PrevChunks;
	hh_u32							m_ChunksCount;
	hh_u32							m_ChunksCapacity;

	// FIXME: atm, no iterators. try not to need them. and when we add them, clean the ones in TSlotArray too...

	static void						_RemapFromVirtualIndex(CGuid index, hh_u32 &chunkId, hh_u32 &subOffset);
	static CGuid					_RemapToVirtualIndex(hh_u32 chunkId, hh_u32 subOffset);
	_Type							*_DerefVirtualIndex(CGuid index) const;
	_Type							*_RawAllocNewChunk();

public:
	TChunkedArray();
	~TChunkedArray();

	HH_FORCEINLINE hh_u32			Count() const { return m_ChunksCount * _ChunkSize; }
	bool							Resize(hh_u32 newSize);
	void							Clean();

	HH_FORCEINLINE const _Type		&operator [] (CGuid slot) const;
	HH_FORCEINLINE _Type			&operator [] (CGuid slot);

	template<typename _Type2>
	CGuid							IndexOf(_Type2 const &what) const;
	template<typename _Type2>
	bool							Contains(_Type2 const &what) const;
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _ChunkSize = 128, typename _Controller = TArrayStaticController<> >
class	TChunkedSlotArray : public TChunkedArray<_Type, _ChunkSize, _Controller>
{
private:
	typedef TChunkedSlotArray<_Type, _ChunkSize, _Controller>	SelfType;
	typedef TChunkedArray<_Type, _ChunkSize, _Controller>		ParentType;

	hh_u32							m_UsedSlots;
	TArray<hh_u32>					m_FreeSlots;

	// FIXME: atm, no iterators. try not to need them. and when we add them, clean the ones in TSlotArray too...

protected:
	bool							_AllocFreeSlotIFP(hh_u32 &chunkId, hh_u32 &subOffset);

public:
	TChunkedSlotArray();
	~TChunkedSlotArray();

	HH_FORCEINLINE bool				Empty() const { return m_UsedSlots == 0; }
	HH_FORCEINLINE hh_u32			Count() const { return ParentType::Count(); }
	HH_FORCEINLINE hh_u32			UsedCount() const { return m_UsedSlots; }

	HH_FORCEINLINE const _Type		&operator [] (CGuid slot) const;
	HH_FORCEINLINE _Type			&operator [] (CGuid slot);

	CGuid							Insert(const _Type &element);
#if	(PK_HAS_RVALUE_REF != 0)
	CGuid							Insert(_Type &&element);
#endif

	void							Remove(CGuid slot);

	template<typename _Type2>
	CGuid							IndexOf(_Type2 const &what) const;
	template<typename _Type2>
	bool							Contains(_Type2 const &what) const;

	void							Clear();
	HH_FORCEINLINE void				Clean() { m_FreeSlots.Clean(); m_UsedSlots = 0; ParentType::Clean(); }

//	typedef	_BaseIterator<_Type>	Iterator;
//
//	/*Const*/Iterator	Begin() const { CGuid id = _FirstValidSlot(); if (id.Valid()) return Iterator(m_Data, id, _MaxCount()); return End(); }
//	/*Const*/Iterator	End() const { return Iterator(m_Data, _MaxCount(), _MaxCount()); }
//	Iterator		Begin() { CGuid id = _FirstValidSlot(); if (id.Valid()) return Iterator(m_Data, id, _MaxCount()); return End(); }
//	Iterator		End() { return Iterator(m_Data, _MaxCount(), _MaxCount()); }
};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_containers_array_chunked.inl"

#endif // __KR_CONTAINERS_CHUNKED_ARRAY_H__
