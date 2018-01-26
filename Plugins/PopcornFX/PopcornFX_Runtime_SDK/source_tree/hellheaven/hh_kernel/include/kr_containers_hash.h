#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2007/12/28 19:19 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_HASH_H__
#define __KR_CONTAINERS_HASH_H__

#include "hh_kernel/include/kr_containers.h"
#include "hh_kernel/include/kr_string.h"
#include "hh_kernel/include/kr_hash.h"
#include "hh_kernel/include/kr_containers_list.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
// hash maps

template<typename _Type, bool _IsConst> class THashMapIterator;

template<typename _Type, typename _KeyType, typename _HashType = TTypeHasher<_KeyType> >
class	THashMap/* : public CNonCopyable*/
{
private:
	typedef THashMap<_Type, _KeyType, _HashType>	SelfType;

	friend class THashMapIterator<SelfType, true>;
	friend class THashMapIterator<SelfType, false>;	// berk

	template<typename _Type2, typename _KeyType2>
	class	THashMapElement : public TIntrusiveLinkedListHook<THashMapElement<_Type2, _KeyType2> >
	{
	public:
		typedef _Type2		Type;
		typedef _KeyType2	KeyType;
	private:
		typedef	THashMapElement<_Type2, _KeyType2>	SelfType;
		_KeyType2	m_Key;
		hh_u32		m_Hash;
	protected:
	public:
		_Type2		m_Value;
		THashMapElement() { HH_ASSERT_NOT_REACHED(); }
		template<typename _VirtualType>
		THashMapElement(const _VirtualType &key, hh_u32 hash, const _Type2 &object) : m_Key(key), m_Hash(hash), m_Value(object) {}
		template<typename _VirtualType>
		THashMapElement(const _VirtualType &key, hh_u32 hash, const _Type2 *object) : m_Key(key), m_Hash(hash), m_Value(*object) {}
		~THashMapElement() {}

		template<typename _VirtualType>
		SelfType	*FindNext(const _VirtualType &key, hh_u32 hash)
		{
			SelfType	*cur = this;
			while (cur != null)
			{
				if (cur->m_Hash == hash && cur->m_Key == key)
					break;
				cur = cur->Next();
			}
			return cur;
		}

		SelfType	*Next() { return TIntrusiveLinkedListHook<THashMapElement<_Type2, _KeyType2> >::Next(); }

		const _KeyType2	&Key() const { return m_Key; }
	};
	typedef	THashMapElement<_Type, _KeyType>	CellType;
	typedef	TRefPtr<CellType>					PCellType;

	hh_u32		m_Size;
	PCellType	*m_Cells;
	hh_u32		m_FirstUsedCell;
	hh_u32		m_ElementCount;

public:
	typedef THashMapIterator<SelfType, false>	Iterator;
	typedef THashMapIterator<SelfType, true>	ConstIterator;

protected:
	void	_Copy(const SelfType &other)
	{
		Resize(other.m_Size);

		ConstIterator	it = other.Begin();
		ConstIterator	end = other.End();
		while (it < end)
		{
			Insert(it.Key(), *it);
			++it;
		}
	}

public:
	THashMap() : m_Size(0), m_Cells(null), m_FirstUsedCell(0), m_ElementCount(0) { Resize(512); }

	explicit THashMap(hh_u32 size)
	:	m_Size(size)
	,	m_Cells(null)
	,	m_FirstUsedCell(0)
	,	m_ElementCount(0)
	{
		if (m_Size != 0)
		{
			if (!IntegerTools::IsPowerOfTwo(m_Size))
			{
				HH_ASSERT_NOT_REACHED_MESSAGE("invalid TFastHashMapSize: %d, sizes must be powers of two... growing to %d\n" COMMA m_Size COMMA IntegerTools::NextPowerOfTwo(m_Size));
				m_Size = IntegerTools::NextPowerOfTwo(m_Size);
			}
			m_Cells = HH_TALLOC<PCellType>(m_Size);	// UNSAFE! but calloc equivalent to TRefPtr's default constructor...
			if (m_Cells != null)
			{
				Mem::Clear(m_Cells, m_Size * sizeof(*m_Cells));
				m_FirstUsedCell = m_Size;
			}
			else
				m_Size = 0;
		}
	}

	explicit THashMap(const SelfType &other) : m_Cells(null), m_Size(0), m_FirstUsedCell(0), m_ElementCount(0)
	{
		_Copy(other);
	}

	~THashMap()
	{
		Clear();
		if (m_Cells != null)
		{
			HH_FREE(m_Cells);
		}
	}

	void	operator = (const SelfType &other)
	{
		Clear();	// not especially optimal
		_Copy(other);
	}

	void	Clear()
	{
		m_ElementCount = 0;
		m_FirstUsedCell = m_Size;
		for (hh_u32 i = 0; i < m_Size; i++)
		{
			if (m_Cells[i] != null)
			{
				m_Cells[i]->Exterminate();	// make sure the whole branch is destroyed
			}
			m_Cells[i] = null;	// let refpointers take care of destruction...
		}
	}

	hh_u32	Size() const { return m_Size; }
	hh_u32	Count() const { return m_ElementCount; }
	bool	Empty() const { return m_ElementCount == 0; }

	bool	Resize(hh_u32 newSize)
	{
		if (newSize == 0)
		{
			Clear();
			if (m_Cells != null)
			{
				HH_FREE(m_Cells);
			}
		}

		SelfType	newHashMap(newSize);

		if (m_Size != 0)	// we already have something here... we need to rehash everything and re-insert all elements
		{
			for (hh_u32 i = 0; i < m_Size; i++)
			{
				CellType	*cell = m_Cells[i].Get();
				while (cell != null)
				{
					newHashMap.Insert(cell->Key(), cell->m_Value);
					cell = reinterpret_cast<CellType*>(cell->Next());
				}
			}
		}

		Swap(newHashMap);
		return true;
	}

	Iterator	Begin()
	{
		return Iterator(m_Cells, (m_FirstUsedCell != m_Size) ? m_Cells[m_FirstUsedCell].Get() : null, m_Size, m_FirstUsedCell, 0);
	}
	Iterator	End()
	{
		return Iterator(m_Cells, null, m_Size, m_Size, m_ElementCount);
	}

	ConstIterator	Begin() const
	{
		return ConstIterator(m_Cells, (m_FirstUsedCell != m_Size) ? m_Cells[m_FirstUsedCell].Get() : null, m_Size, m_FirstUsedCell, 0);
	}
	ConstIterator	End() const
	{
		return ConstIterator(m_Cells, null, m_Size, m_Size, m_ElementCount);
	}

	// we should be careful here, _VirtualType and _KeyType should hash to the same thing...
	template<typename _VirtualType>
	HH_FORCEINLINE hh_u32	Hash(const _VirtualType &key) const { return _HashType::Hash(key); }

	template<typename _VirtualType>
	bool	Contains(const _VirtualType &key) const { return Find(key) != null; }

	template<typename _VirtualType>
	const _Type	*Find(const _VirtualType &key) const
	{
		HH_ASSERT(IntegerTools::IsPowerOfTwo(m_Size));
		const hh_u32	hash = _HashType::Hash(key);
		CellType		*cell = m_Cells[hash & (m_Size - 1)].Get();
		if (cell != null)
		{
			const CellType	*found = cell->FindNext(key, hash);
			if (found != null)
				return &(found->m_Value);
		}
		return null;
	}
	template<typename _VirtualType>
	_Type	*Find(const _VirtualType &key)
	{
		HH_ASSERT(IntegerTools::IsPowerOfTwo(m_Size));
		const hh_u32	hash = _HashType::Hash(key);
		CellType		*cell = m_Cells[hash & (m_Size - 1)].Get();
		if (cell != null)
		{
			CellType	*found = cell->FindNext(key, hash);
			if (found != null)
				return &(found->m_Value);
		}
		return null;
	}

	// FIXME: how could we implement a const version?
	template<typename _VirtualType>
	_Type	*Insert(const _VirtualType &key, const _Type &objectToAdd) { return Insert(key, &objectToAdd); }
	template<typename _VirtualType>
	_Type	*Insert(const _VirtualType &key, const _Type *objectToAdd)
	{
		HH_ASSERT(objectToAdd != null);
		HH_ASSERT(Find(key) == null);	// this should only be called when we're sure the element isn't already here...

		const hh_u32	hash = _HashType::Hash(key);
		PCellType		cellToInsert = HH_NEW(CellType(key, hash, objectToAdd));
		if (cellToInsert == null)
			return null;

		HH_ASSERT(IntegerTools::IsPowerOfTwo(m_Size));
		const hh_u32	id = hash & (m_Size - 1);
		if (id < m_FirstUsedCell)
			m_FirstUsedCell = id;

		if (m_Cells[id] != null)
		{
			cellToInsert->InsertBefore(m_Cells[id]);
		}

		m_Cells[id] = cellToInsert;
		m_ElementCount++;
		return &(cellToInsert->m_Value);
	}

	template<typename _VirtualType>
	_Type	*InsertIFN(const _VirtualType &key, const _Type &objectToAdd) { return InsertIFN(key, &objectToAdd); }
	template<typename _VirtualType>
	_Type	*InsertIFN(const _VirtualType &key, const _Type *objectToAdd)
	{
		HH_ASSERT(objectToAdd != null);
		HH_ASSERT(IntegerTools::IsPowerOfTwo(m_Size));
		const hh_u32	hash = _HashType::Hash(key);
		const hh_u32	id = hash & (m_Size - 1);
		if (m_Cells[id] != null)
		{
			HH_ASSERT(m_FirstUsedCell <= id);
			CellType	*element = m_Cells[id]->FindNext(key, hash);
			if (element != null)
			{
				return &(element->m_Value);
			}
		}

		PCellType	cellToInsert = HH_NEW(CellType(key, hash, objectToAdd));
		if (cellToInsert == null)
			return null;

		if (id < m_FirstUsedCell)
			m_FirstUsedCell = id;
		if (m_Cells[id] != null)
			cellToInsert->InsertBefore(m_Cells[id]);
		m_Cells[id] = cellToInsert;
		m_ElementCount++;
		return &(cellToInsert->m_Value);
	}

	template<typename _VirtualType>
	bool	Remove(const _VirtualType &key)
	{
		HH_ASSERT(IntegerTools::IsPowerOfTwo(m_Size));
		const hh_u32	hash = _HashType::Hash(key);
		const hh_u32	index = hash & (m_Size - 1);
		CellType		*cell = m_Cells[index].Get();
		if (cell != null)
		{
			cell = cell->FindNext(key, hash);
			if (cell != null)
			{
				{
					PCellType	nextCell = cell->Next();
					cell->Unlink();
					if (m_Cells[index] == cell)
						m_Cells[index] = nextCell;
				}

				HH_ASSERT(index >= m_FirstUsedCell && index < m_Size);
				if (index == m_FirstUsedCell)
				{
					// if 'm_FirstUsedCell' points to the cell where we removed this entry, and if we removed the last entry in this cell
					if (m_Cells[index] == null)
					{
						// go forward in the hashmap to find the first occupied cell
						hh_u32	id = index;
						while (++id != m_Size)
						{
							if (m_Cells[id] != null)
							{
								break;
							}
						}
						m_FirstUsedCell = id;
					}
				}

				HH_ASSERT(m_ElementCount > 0);
				m_ElementCount--;
				return true;
			}
		}
		return false;
	}

	void	PopulateArray(TArray<TPair<_KeyType, _Type> > &outputArray)
	{
		outputArray.Clear();	// just make sure it's empty...
		for (hh_u32 i = 0; i < m_Size; i++)
		{
			CellType	*cell = m_Cells[i].Get();
			while (cell != null)
			{
				outputArray.PushBack(TPair<_KeyType, _Type>(cell->Key(), cell->m_Value));
				cell = reinterpret_cast<CellType*>(cell->Next());
			}
		}
	}

	void	PopulateRefArray(TArray<TPair<_KeyType, _Type*> > &outputArray) const
	{
		outputArray.Clear();	// just make sure it's empty...
		for (hh_u32 i = 0; i < m_Size; i++)
		{
			CellType	*cell = m_Cells[i].Get();
			while (cell != null)
			{
				outputArray.PushBack(TPair<_KeyType, _Type*>(cell->Key(), &(cell->m_Value)));
				cell = reinterpret_cast<CellType*>(cell->Next());
			}
		}
	}

	void	ListElements(void (*callback)(void *arg, _Type &object, const _KeyType &key), void *callbackArg) const
	{
		for (hh_u32 i = 0; i < m_Size; i++)
		{
			CellType	*cell = m_Cells[i].Get();
			while (cell != null)
			{
				CellType	*nextCell = reinterpret_cast<CellType*>(cell->Next());
				callback(callbackArg, cell->m_Value, cell->Key());
				cell = nextCell;
			}
		}
	}

	void	Swap(SelfType &swapWith)
	{
		HH_ASSERT(this != &swapWith);
		HHSwap(m_Size, swapWith.m_Size);
		HHSwap(m_Cells, swapWith.m_Cells);
		HHSwap(m_FirstUsedCell, swapWith.m_FirstUsedCell);
		HHSwap(m_ElementCount, swapWith.m_ElementCount);
	}

	// element access
	template<typename _VirtualType>
	_Type	*operator[] (const _VirtualType &key)
	{
		return Find(key);
	}

	template<typename _VirtualType>
	const _Type	*operator[] (const _VirtualType &key) const
	{
		return Find(key);
	}
};

//----------------------------------------------------------------------------

template<typename _Type, typename _KeyType, typename _HashType>
void	HHSwap(THashMap<_Type, _KeyType, _HashType> &map0, THashMap<_Type, _KeyType, _HashType> &map1)
{
	map0.Swap(map1);
}

//----------------------------------------------------------------------------
//
//	Iterator
//
//----------------------------------------------------------------------------

template<typename _Type, bool _IsConstIterator>
class	THashMapIterator
{
protected:
	typedef THashMapIterator<_Type, _IsConstIterator>	SelfType;
	typedef typename _Type::CellType					CellType;
public:
	typedef typename TType_MakeConstIf<typename CellType::Type, _IsConstIterator>::Type		Type;
	typedef typename TType_MakeConstIf<typename _Type::PCellType, _IsConstIterator>::Type	PCellType;
	typedef typename CellType::KeyType														KeyType;

protected:
	friend class THashMapIterator<_Type, !_IsConstIterator>;

	hh_u32		m_Idx;
	PCellType	*m_Cells;
	CellType	*m_Cell;
	hh_u32		m_Size;
	hh_u32		m_VirtualIdx;

	HH_FORCEINLINE bool	_GetNextRow()
	{
		while (m_Cell == null)
		{
			m_Idx++;
			if (m_Idx >= m_Size)
			{
				m_Idx = m_Size;
				return false;
			}
			m_Cell = m_Cells[m_Idx].Get();
		}
		return true;
	}

	HH_FORCEINLINE bool	_GetPrevRow()
	{
		while (!m_Cell)
		{
			if (m_Idx == 0)
			{
				return false;
			}
			--m_Idx;
			m_Cell = m_Cells[m_Idx].Get();
		}
		return true;
	}

public:
	HH_FORCEINLINE THashMapIterator() : m_Idx(0), m_Cells(null), m_Cell(null), m_Size(0), m_VirtualIdx(0) {}
	template<bool _Constness>
	HH_FORCEINLINE THashMapIterator(const THashMapIterator<_Type, _Constness> &other) : m_Idx(other.m_Idx), m_Cells(other.m_Cells), m_Cell(other.m_Cell), m_Size(other.m_Size), m_VirtualIdx(other.m_VirtualIdx) {}
	HH_FORCEINLINE THashMapIterator(PCellType *cells, CellType *cell, hh_u32 size, hh_u32 cellIndex, hh_u32 virtualElementIndex)
	:	m_Idx(cellIndex)
	,	m_Cells(cells)
	,	m_Cell(cell)
	,	m_Size(size)
	,	m_VirtualIdx(virtualElementIndex)
	{
	}

	HH_FORCEINLINE SelfType	&operator ++ ()
	{
		HH_ASSERT(m_Cell);
		m_Cell = m_Cell->Next();
		if (!m_Cell)
		{
			if (_GetNextRow())
				m_VirtualIdx++;
		}
		else
			m_VirtualIdx++;
		return *this;
	}

	HH_FORCEINLINE SelfType	&operator -- ()
	{
		HH_ASSERT(m_VirtualIdx > 0);
		if (m_VirtualIdx > 0)
		{
			HH_ASSERT(m_Cell != null);
			m_Cell = m_Cell->Prev();
			if (m_Cell == null)
			{
				_GetPrevRow();
				HH_ASSERT(m_Cell != null);
				m_Cell = m_Cell->Last();
			}
			m_VirtualIdx--;
		}
		return *this;
	}

	HH_FORCEINLINE bool		EndOfMap() { return m_Cell == null; }

	HH_FORCEINLINE SelfType	operator ++ (int) { SelfType oldValue = *this; this->operator++(); return oldValue; }
	HH_FORCEINLINE SelfType	operator -- (int) { SelfType oldValue = *this; this->operator--(); return oldValue; }
	HH_FORCEINLINE SelfType	operator + (int count) const { SelfType s(*this); while (count--) s.operator++(); return s; }
	HH_FORCEINLINE SelfType	operator - (int count) const { SelfType s(*this); while (count--) s.operator--(); return s; }
	HH_FORCEINLINE SelfType	&operator += (int count) { while (count--) this->operator++(); return *this; }
	HH_FORCEINLINE SelfType	&operator -= (int count) { while (count--) this->operator--(); return *this; }

	HH_FORCEINLINE const KeyType	&Key() const { return m_Cell->Key(); }

	HH_FORCEINLINE int		operator - (SelfType &other) const { return m_VirtualIdx - other.m_VirtualIdx; }
	HH_FORCEINLINE Type		&operator * () const { return m_Cell->m_Value; }
	HH_FORCEINLINE Type		&operator [] (int index) const
	{
		HH_ASSERT_NOT_REACHED_MESSAGE("Do NOT use this function!!!");	// wut?
		return m_Cell->m_Value;
	}
	HH_FORCEINLINE Type		*operator -> () const { return &m_Cell->m_Value; }

	HH_FORCEINLINE bool		operator == (const SelfType &other) const { return (m_Idx == other.m_Idx) && (m_Cell == other.m_Cell) && (m_Cells == other.m_Cells) && (m_Size == other.m_Size); }
	HH_FORCEINLINE bool		operator != (const SelfType &other) const { return !(*this == other); }
	HH_FORCEINLINE bool		operator > (const SelfType &other) const { return m_VirtualIdx > other.m_VirtualIdx; }
	HH_FORCEINLINE bool		operator < (const SelfType &other) const { return m_VirtualIdx < other.m_VirtualIdx; }
	HH_FORCEINLINE bool		operator >= (const SelfType &other) const { return m_VirtualIdx >= other.m_VirtualIdx; }
	HH_FORCEINLINE bool		operator <= (const SelfType &other) const { return m_VirtualIdx <= other.m_VirtualIdx; }
};

//----------------------------------------------------------------------------
//
//	Fast hash maps
//
//----------------------------------------------------------------------------

class	CFastHashMapProbe_Linear
{
public:
	HH_FORCEINLINE static hh_u32	Next(hh_u32 slot, hh_u32 /*probeCount*/)
	{
		return slot + 1;
	}
	static const hh_u32	HugeProbeCount = 20;
};

class	CFastHashMapProbe_PseudoQuadratic
{
public:
	HH_FORCEINLINE static hh_u32	Next(hh_u32 slot, hh_u32 probeCount)
	{
		return slot + probeCount;
	}
	static const hh_u32	HugeProbeCount = 20;
};

//----------------------------------------------------------------------------

template<typename _Probe = CFastHashMapProbe_PseudoQuadratic, hh_u32 _MinimumSize = 32, hh_u32 _Alignment = 0x10>
class	TFastHashMapDefaultController
{
public:
	typedef _Probe	Probe;

	HH_FORCEINLINE static bool	NeedsGrowth(hh_u32 count, hh_u32 size)
	{
		//return 1 + count + (count >> 1) > size;
		return 1 + count + count > size;
		//return 1 + count * 3 > size;
	}
	HH_FORCEINLINE static bool	NeedsShrink(hh_u32 count, hh_u32 size)
	{
		//return count * 10 < size;
		return size > _MinimumSize && (count << 3) < size;
	}
	HH_FORCEINLINE static bool	NeedsResize(hh_u32 count, hh_u32 size)
	{
		return NeedsGrowth(count, size) || NeedsShrink(count, size);
	}
	HH_FORCEINLINE static hh_u32	Resize(hh_u32 count, hh_u32 size)
	{
		if (NeedsGrowth(count, size))
		{
			if (size >= _MinimumSize / 2)
				return size + size;
			return _MinimumSize;
		}
		HH_ASSERT(NeedsShrink(count, size));
		return size >> 1;
	}

	HH_FORCEINLINE static hh_u32	Alignment() { return _Alignment; }
};

//----------------------------------------------------------------------------

template<typename _Type,
		 typename _Hasher = TTypeHasher<_Type>,
		 typename _Controller = TFastHashMapDefaultController<> >
class	TFastHashMapTraits
{
public:
	typedef _Type		Type;
	typedef _Hasher		Hasher;
	typedef _Controller	Controller;
};

//----------------------------------------------------------------------------
// specialization for Guids

template<typename _Hasher, typename _Controller>
class	TFastHashMapTraits<CGuid, _Hasher, _Controller>
{
public:
	typedef CGuid				Type;
	typedef _Hasher				Hasher;
	typedef _Controller			Controller;

	static const CGuid::Type	Invalid = 0xFFFFFFFF;
	static const CGuid::Type	Invalid2 = Invalid - 1;	// watchout! pretty unsafe...

	HH_FORCEINLINE static bool	Valid(Type value)
	{
		return (value & 0x80000000) == 0;
	}
};

//----------------------------------------------------------------------------
// specialization for raw pointers

template<typename _PtrType, typename _Hasher, typename _Controller>
class	TFastHashMapTraits<_PtrType*, _Hasher, _Controller>
{
public:
	typedef _PtrType*			Type;
	typedef _Hasher				Hasher;
	typedef _Controller			Controller;

	// invalid pointers used are: null and 0xFFFFFFFF
	static const Type	Invalid;
	static const Type	Invalid2;	// watchout! pretty unsafe...

	HH_FORCEINLINE static bool	Valid(Type value)
	{
		return ((hh_ureg)value + 1) <= 1;	// either 0 or 1
	}
};

template<typename _PtrType, typename _Hasher, typename _Controller>
_PtrType	*const TFastHashMapTraits<_PtrType*, _Hasher, _Controller>::Invalid = null;
template<typename _PtrType, typename _Hasher, typename _Controller>
_PtrType	*const TFastHashMapTraits<_PtrType*, _Hasher, _Controller>::Invalid2 = (_PtrType)((hh_ureg)-1);	// watchout! pretty unsafe...

//----------------------------------------------------------------------------
// Specialization for TValidityTypeWrapper<>

template<typename _RawType, typename _Hasher, typename _Controller>
class	TFastHashMapTraits<TValidityTypeWrapper<_RawType>, _Hasher, _Controller> : public TValidityTypeWrapper<_RawType>
{
public:
	typedef TValidityTypeWrapper<_RawType>	Type;
	typedef _Hasher							Hasher;
	typedef _Controller						Controller;

	HH_FORCEINLINE static bool	Valid(const Type &what) { return what.Valid(); }
};

//----------------------------------------------------------------------

template<typename _Type, typename _Traits, bool _StatsEnabled>
class	TFastHashMapStats
{
protected:
	HH_FORCEINLINE void	RegisterAccess(bool hit) const {}	// in release builds, this will always be ignored, but we FORCEINLINE() to help minimize bloating in debug builds...
};

template<typename _Type, typename _Traits>
class	TFastHashMapStats<_Type, _Traits, true>
{
private:
	// we need these to be mutable to avoid breaking the upper level map's interface constness
	mutable hh_u32	m_HitCount;
	mutable hh_u32	m_MissCount;
protected:
	void	RegisterAccess(bool hit) const { if (hit) m_HitCount++; else m_MissCount++; }
public:
	TFastHashMapStats() : m_HitCount(0), m_MissCount(0) {}

	HH_FORCEINLINE hh_u32	HitCount() const { return m_HitCount; }
	HH_FORCEINLINE hh_u32	MissCount() const { return m_MissCount; }
	void					ClearStats() { m_HitCount = 0; m_MissCount = 0; }
	float					HitRatio() const { hh_u32 total = m_HitCount + m_MissCount; if (total) return m_HitCount / (float)total; return 0.0f; }
	float					MissRatio() const { hh_u32 total = m_HitCount + m_MissCount; if (total) return m_MissCount / (float)total; return 0.0f; }
};

//----------------------------------------------------------------------

template<typename _Type,
		 typename _Traits = TFastHashMapTraits<_Type>,
		 bool _StatsEnabled = false>
class TFastHashMap : public TFastHashMapStats<_Type, _Traits, _StatsEnabled>
{
private:
	typedef TFastHashMap<_Type, _Traits, _StatsEnabled>			SelfType;
	typedef TFastHashMapStats<_Type, _Traits, _StatsEnabled>	ParentType;
	typedef _Type	ValueType;
	typedef _Traits	Traits;

	// single array
	_Type		*m_Slots;
	hh_u32		m_Size;
	hh_u32		m_Count;

	HH_ONLY_IN_DEBUG(mutable bool	m_DebugLoopBreaker;)

	HH_ONLY_IF_ASSERTS(
		hh_u32	_Recount()
		{
			hh_u32	total = 0;
			for (hh_u32 i = 0; i < m_Size; i++)
			{
				total += _Traits::Valid(m_Slots[i]) ? 1 : 0;
			}
			HH_ASSERT(m_Count == total);
			return total;
		}
	)

	bool	_UnsafeResize(hh_u32 newSize)
	{
		HH_ASSERT(newSize >= m_Count);
		HH_ASSERT(_Recount() == m_Count);

		SelfType	resized(newSize);
		if (resized.m_Size < newSize)
			return false;

		for (hh_u32 i = 0; i < m_Size; i++)
		{
			_Type	&slotToInsert = m_Slots[i];
			if (_Traits::Valid(slotToInsert) && !resized.UnsafeInsert(slotToInsert).Valid())
			{
				HH_ASSERT_NOT_REACHED_MESSAGE("TFastHash algorithm error! Failed inserting slot %d during table resize" COMMA i);
				return false;
			}
		}

		Swap(resized);
		return true;
	}

	CGuid	_FirstValidIndex() const
	{
		for (hh_u32 i = 0; i < m_Size; i++)
		{
			if (_Traits::Valid(m_Slots[i]))
				return i;
		}
		return CGuid::INVALID;
	}

	CGuid	_LastValidIndex() const
	{
		for (hh_i32 i = m_Size - 1; i >= 0; i--)
		{
			if (_Traits::Valid(m_Slots[i]))
				return i;
		}
		return CGuid::INVALID;
	}

	void	_Copy(const SelfType &other)
	{
		Resize(other.m_Size);
		ConstIterator	it = other.Begin();
		ConstIterator	end = other.End();
		while (it < end)
		{
			Insert(*it);
			++it;
		}
	}

public:
	TFastHashMap() : m_Slots(null), m_Size(0), m_Count(0) {}

	explicit TFastHashMap(hh_u32 numSlots) : m_Slots(null), m_Size(0), m_Count(0)
	{
		if (numSlots > 0)
		{
			if (!IntegerTools::IsPowerOfTwo(numSlots))
			{
				HH_ASSERT_NOT_REACHED_MESSAGE("invalid TFastHashMapSize: %d, sizes must be powers of two... growing to %d\n" COMMA numSlots COMMA IntegerTools::NextPowerOfTwo(numSlots));
				numSlots = IntegerTools::NextPowerOfTwo(numSlots);
			}
			m_Slots = (_Type*)HH_MALLOC_ALIGNED(numSlots * sizeof(_Type), _Traits::Controller::Alignment());
			if (m_Slots != null)
			{
				m_Size = numSlots;
				for (hh_u32 i = 0; i < numSlots; i++)
				{
					Mem::Construct(m_Slots[i], _Traits::Invalid);
				}
			}
		}
	}

	explicit TFastHashMap(const SelfType &other) : m_Slots(null), m_Size(0), m_Count(0)
	{
		_Copy(other);
	}

	~TFastHashMap()
	{
		if (m_Slots != null)
		{
			for (hh_u32 i = 0; i < m_Size; i++)
			{
				Mem::Destruct(m_Slots[i]);
			}
			HH_FREE(m_Slots);
		}
		else
			HH_ASSERT(m_Size == 0 && m_Count == 0);
	}

	void	operator = (const SelfType &other)
	{
		Clear();	// not especially optimal
		_Copy(other);
	}

	template<typename _KeyType>
	CGuid	IndexOf(const _KeyType &what) const
	{
		// FIXME: catch Invalid inputs?
		if (m_Size == 0)
			return CGuid::INVALID;

		HH_ASSERT(m_Slots != null);
		const hh_u32	wrapMask = m_Size - 1;
		hh_u32			slot = _Traits::Hasher::Hash(what) & wrapMask;
		hh_u32			probeCount = 0;

		while (m_Slots[slot] != what)
		{
			ParentType::RegisterAccess(false);

			HH_ONLY_IN_DEBUG(m_DebugLoopBreaker = false);
			if (!_Traits::Valid(m_Slots[slot]))
				return CGuid::INVALID;

			probeCount++;
			HH_ASSERT_MESSAGE(probeCount < m_Size && probeCount < _Traits::Controller::Probe::HugeProbeCount, "Performance warning: huge amount of probes in TFastHash<>");
			slot = _Traits::Controller::Probe::Next(slot, probeCount) & wrapMask;
			HH_ONLY_IN_DEBUG(if (m_DebugLoopBreaker == true) return CGuid::INVALID);
		}

		ParentType::RegisterAccess(true);

		if (!_Traits::Valid(m_Slots[slot]))
			return CGuid::INVALID;

		return slot;
	}

	template<typename _KeyType>
	HH_INLINE const _Type	&Find(const _KeyType &what) const
	{
		const CGuid	index = IndexOf<_KeyType>(what);
		if (index.Valid())
			return m_Slots[index];
		return _Traits::Invalid;
	}

	const _Type	&At(CGuid index) const { HH_ASSERT(index.Valid() && (hh_u32)index < m_Size); return m_Slots[index]; }
	_Type		&At(CGuid index) { HH_ASSERT(index.Valid() && (hh_u32)index < m_Size); return m_Slots[index]; }

	template<typename _KeyType>
	const _Type &operator [] (const _KeyType &what) const { return Find<_KeyType>(what); }

	template<typename _KeyType>
	HH_INLINE bool	Contains(const _KeyType &what) const
	{
		return IndexOf(what).Valid();
	}

	HH_INLINE CGuid	Insert(const _Type &what)
	{
		if (_Traits::Valid(what) &&
			(!_Traits::Controller::NeedsResize(m_Count, m_Size) ||
			_UnsafeResize(_Traits::Controller::Resize(m_Count, m_Size))))
		{
			return UnsafeInsert(what);
		}
		return CGuid::INVALID;
	}

	// the caller has to make sure there are enough free slots, otherwise, this Insert() will enter an infinite loop
	CGuid	UnsafeInsert(const _Type &what)
	{
		HH_ASSERT(m_Size > 0 && m_Slots != null);
		const hh_u32	wrapMask = m_Size - 1;
		hh_u32			slot = _Traits::Hasher::Hash(what) & wrapMask;
		hh_u32			probeCount = 0;

		while (_Traits::Valid(m_Slots[slot]))
		{
			HH_ONLY_IN_DEBUG(m_DebugLoopBreaker = false);
			if (m_Slots[slot] == what)
			{
				ParentType::RegisterAccess(true);
				return slot;
			}
			ParentType::RegisterAccess(false);
			probeCount++;
			HH_ASSERT_MESSAGE(probeCount < m_Size && probeCount < _Traits::Controller::Probe::HugeProbeCount, "Performance warning: huge amount of probes in TFastHash<>");
			slot = _Traits::Controller::Probe::Next(slot, probeCount) & wrapMask;
			HH_ONLY_IN_DEBUG(if (m_DebugLoopBreaker == true) return CGuid::INVALID);
		}

		ParentType::RegisterAccess(true);
		m_Slots[slot] = what;
		m_Count++;
		return slot;
	}

	template<typename _KeyType>
	HH_INLINE bool	Remove(const _KeyType &what)
	{
		if (!_Traits::Valid(what))
			return false;
		const CGuid	slot = IndexOf(what);
		if (slot.Valid())
		{
			RemoveAt(slot);
			return true;
		}
		return false;
	}

	HH_INLINE void	RemoveAt(CGuid slot)
	{
		HH_ASSERT(m_Size > 0 && m_Slots != null && slot.Valid());
		m_Slots[slot] = _Traits::Invalid2;
		m_Count--;
	}

	HH_INLINE bool	Resize(hh_u32 newSize)
	{
		if (newSize > m_Size)
		{
			return _UnsafeResize(newSize);
		}
		return true;
	}

	HH_FORCEINLINE bool		Empty() const { return m_Count == 0; }
	HH_FORCEINLINE hh_u32	Count() const { return m_Count; }
	HH_FORCEINLINE hh_u32	Size() const { return m_Size; }
	HH_FORCEINLINE hh_u32	SlotWrapMask() const { return m_Size - 1; }

	void	Clear()
	{
		if (m_Count != 0)
		{
			for (hh_u32 i = 0; i < m_Size; i++)
			{
				m_Slots[i] = _Traits::Invalid;
			}
			m_Count = 0;
		}
	}

	void	Clean()
	{
		if (m_Slots != null)
		{
			HH_FREE(m_Slots);
			m_Slots = null;
		}
		m_Count = 0;
		m_Size = 0;
	}

	void	Swap(SelfType &swapWith)
	{
		HH_ASSERT(this != &swapWith);
		HHSwap(m_Slots, swapWith.m_Slots);
		HHSwap(m_Size, swapWith.m_Size);
		HHSwap(m_Count, swapWith.m_Count);
	}

	bool	operator == (const SelfType &other)
	{
		if (Count() != other.Count())
			return false;
		if (this == &other)
			return true;

		for (hh_u32 i = 0; i < m_Size; i++)
		{
			if (_Traits::Valid(m_Slots[i]) && !other.Contains(m_Slots[i]))
				return false;
		}
		return true;
	}

	bool	operator != (const SelfType &other)
	{
		return !(*this == other);
	}

	template<bool _IsConstIterator>
	class	_Iterator
	{
	public:
		typedef typename TType_MakeConstIf<_Type, _IsConstIterator>::Type	Type;
		typedef _Iterator<_IsConstIterator>									SelfType;
	private:
		friend class			_Iterator<!_IsConstIterator>;

		Type					*m_Ptr;
		Type					*m_Start;
		hh_u32					m_Count;

		bool					_FromSameContainer(const SelfType &other) const { return m_Start == other.m_Start && m_Count == other.m_Count; }

	public:
		_Iterator(Type *slot, Type *start, hh_u32 count) : m_Ptr(slot), m_Start(start), m_Count(count) {}
		_Iterator(const _Iterator<false> &other) : m_Ptr(other.m_Ptr), m_Start(other.m_Start), m_Count(other.m_Count) {}	// nonConst->nonConst and nonConst->const, but not const->nonConst
//		Iterator(const SelfType &other) : m_Ptr(other.m_Ptr, other.m_Start, other.m_Count) {}
//		const Iterator	&operator = (const SelfType &other) { m_Ptr = other.m_Ptr; m_Start = other.m_Start; m_Count = other.m_Count; return *this; }

		HH_FORCEINLINE SelfType	&operator ++ () { Type *end = m_Start + m_Count; do { m_Ptr++; } while (m_Ptr < end && !_Traits::Valid(*m_Ptr)); return *this; }
		HH_FORCEINLINE SelfType	&operator -- () { do { m_Ptr--; } while (m_Ptr > m_Start && !_Traits::Valid(*m_Ptr)); return *this; }
		HH_FORCEINLINE SelfType	operator ++ (int) { SelfType oldValue = *this; operator ++ (); return oldValue; }
		HH_FORCEINLINE SelfType	operator -- (int) { SelfType oldValue = *this; operator -- (); return oldValue; }
		HH_FORCEINLINE SelfType	operator + (int count) const { SelfType	it = *this; while (count-- > 0) ++it; return it; }
		HH_FORCEINLINE SelfType	operator - (int count) const { SelfType	it = *this; while (count-- > 0) --it; return it; }
		HH_FORCEINLINE SelfType	&operator += (int count) { while (count-- > 0) operator ++ (); return *this; }
		HH_FORCEINLINE SelfType	&operator -= (int count) { while (count-- > 0) operator -- (); return *this; }
//		HH_FORCEINLINE int		operator - (SelfType &other) const { return m_Ptr - other.m_Ptr; }

		HH_FORCEINLINE Type		&operator * () const { return *m_Ptr; }
//		HH_FORCEINLINE Type		&operator [] (int index) const { return m_Ptr[index]; }
		HH_FORCEINLINE Type		*operator -> () const { return m_Ptr; }

		HH_FORCEINLINE bool		operator == (const SelfType &other) const { HH_ASSERT(_FromSameContainer(other)); return m_Ptr == other.m_Ptr; }
		HH_FORCEINLINE bool		operator != (const SelfType &other) const { HH_ASSERT(_FromSameContainer(other)); return m_Ptr != other.m_Ptr; }
		HH_FORCEINLINE bool		operator > (const SelfType &other) const { HH_ASSERT(_FromSameContainer(other)); return m_Ptr > other.m_Ptr; }
		HH_FORCEINLINE bool		operator < (const SelfType &other) const { HH_ASSERT(_FromSameContainer(other)); return m_Ptr < other.m_Ptr; }
		HH_FORCEINLINE bool		operator >= (const SelfType &other) const { HH_ASSERT(_FromSameContainer(other)); return m_Ptr >= other.m_Ptr; }
		HH_FORCEINLINE bool		operator <= (const SelfType &other) const { HH_ASSERT(_FromSameContainer(other)); return m_Ptr <= other.m_Ptr; }
	};

	typedef _Iterator<false>	Iterator;
	typedef _Iterator<true>		ConstIterator;

	Iterator	Begin()
	{
		const CGuid	id = _FirstValidIndex();
		if (!id.Valid())
			return End();
		return Iterator(m_Slots + id, m_Slots, m_Size);
	}
	Iterator	End()
	{
		return Iterator(m_Slots + m_Size, m_Slots, m_Size);
	}

	ConstIterator	Begin() const
	{
		const CGuid	id = _FirstValidIndex();
		if (!id.Valid())
			return End();
		return ConstIterator(m_Slots + id, m_Slots, m_Size);
	}
	ConstIterator	End() const
	{
		return ConstIterator(m_Slots + m_Size, m_Slots, m_Size);
	}
};

//----------------------------------------------------------------------------

template<typename _Type, typename _Traits>
void	HHSwap(TFastHashMap<_Type, _Traits> &map0, TFastHashMap<_Type, _Traits> &map1)
{
	map0.Swap(map1);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_HASH_H__
