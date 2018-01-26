#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2012/04/25 15:27 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ARRAY_SLOT_INL__
#define __KR_CONTAINERS_ARRAY_SLOT_INL__

#ifndef	__KR_CONTAINERS_ARRAY_H__
#	error	kr_containers_array_slot.inl must not be included manually, include kr_containers_array_slot.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>	struct	TSlotArrayInvalidExtractor<_Type*>
{
	static HH_FORCEINLINE _Type				*Invalid() { return null; }
	static HH_FORCEINLINE void				Invalidate(_Type *&object) { object = null; }
	static HH_FORCEINLINE bool				IsValid(_Type *object) { return object != null; }
};

template<typename _Type>	struct	TSlotArrayInvalidExtractor<TRefPtr<_Type> >
{
	static HH_FORCEINLINE TRefPtr<_Type>	Invalid() { return null; }
	static HH_FORCEINLINE void				Invalidate(TRefPtr<_Type> &object) { object = null; }
	static HH_FORCEINLINE bool				IsValid(TRefPtr<_Type> object) { return object != null; }
};

template<typename _Type>	struct	TSlotArrayInvalidExtractor<TWeakPtr<_Type> >
{
	static HH_FORCEINLINE TRefPtr<_Type>	Invalid() { return null; }
	static HH_FORCEINLINE void				Invalidate(TRefPtr<_Type> &object) { object = null; }
	static HH_FORCEINLINE bool				IsValid(TRefPtr<_Type> object) { return object != null; }
};

template<>					struct	TSlotArrayInvalidExtractor<CGuid>
{
	static HH_FORCEINLINE CGuid				Invalid() { return CGuid::INVALID; }
	static HH_FORCEINLINE void				Invalidate(CGuid &object) { object = CGuid::INVALID; }
	static HH_FORCEINLINE bool				IsValid(CGuid object) { return object.Valid(); }
};

template<>					struct	TSlotArrayInvalidExtractor<CString>
{
	static HH_FORCEINLINE CString			Invalid() { return null; }
	static HH_FORCEINLINE void				Invalidate(CString &object) { object = null; }
	static HH_FORCEINLINE bool				IsValid(const CString &object) { return !object.Empty(); }
};

//----------------------------------------------------------------------------

// Iterator:
template<typename _Type, typename _Storage, typename _VType>
class TSlotArray_BaseIterator
{
public:
	typedef	TSlotArray_BaseIterator<_Type, _Storage, _VType>	SelfType;
private:
	_VType				*m_Ptr;
	hh_u32				m_Id;
	hh_u32				m_SlotCount;

	bool				_IsSlotValid() const
	{
		return m_Id < m_SlotCount && TSlotArrayInvalidExtractor<_Type>::IsValid(m_Ptr[m_Id]);
	}

	HH_FORCEINLINE void	_AssertIsValid() const
	{
		HH_PARANOID_ASSERT_MESSAGE(m_Ptr != null, "uninitialized TSlotArray iterator");
		HH_ASSERT(_IsSlotValid());
	}

	void				_AssertIsValidInit() const
	{
		HH_ASSERT(m_Id == m_SlotCount || _IsSlotValid());
	}

	friend class	TSlotArray_Base<_Type, _Storage>;
	TSlotArray_BaseIterator(_VType *ptr, hh_u32 id, hh_u32 slotCount)
	:	m_Ptr(ptr)
	,	m_Id(id)
	,	m_SlotCount(slotCount)
	{
		_AssertIsValidInit();
	}

public:
	TSlotArray_BaseIterator()	// should we allow construction of empty iterators?
	:	m_Ptr(null)
	,	m_Id(0)
	,	m_SlotCount(0)
	{
	}

	template<typename _Type2>
	HH_INLINE TSlotArray_BaseIterator(const TSlotArray_BaseIterator<_Type, _Storage, _Type2> &other)
	:	m_Ptr(other.m_Ptr)
	,	m_Id(other.m_Id)
	,	m_SlotCount(other.m_SlotCount)
	{
		_AssertIsValidInit();
	}


//	template<typename _Type2>	// dirty trick to avoid exposing the raw construction to the outside
//	TSlotArray_BaseIterator(TIteratorBuildProxy<_Type2> ptrProxy, hh_u32 id, hh_u32 slotCount) : m_Ptr(ptrProxy.m_Ptr), m_Id(id), m_SlotCount(slotCount) { _AssertIsValidInit(); }

	SelfType	&operator = (const SelfType &other)
	{
		m_Ptr = other.m_Ptr;
		m_Id = other.m_Id;
		m_SlotCount = other.m_SlotCount;
		_AssertIsValidInit();
		return *this;
	}

	HH_FORCEINLINE hh_u32	Slot() const
	{
		_AssertIsValid();
		return m_Id;
	}

	HH_FORCEINLINE SelfType	&operator ++ ()
	{
		_AssertIsValid();
		m_Id++;
		while (m_Id < m_SlotCount)	// Begin().m_Id == m_SlotCount
		{
			if (TSlotArrayInvalidExtractor<_Type>::IsValid(m_Ptr[m_Id]))
				break;
			m_Id++;
		}
		return *this;
	}

	HH_FORCEINLINE SelfType	&operator -- ()
	{
		_AssertIsValid();
		HH_ASSERT(m_Id > 0);	// can't -- the Begin() iterator
		// assert also if we don't find any valid slots before ourself, that would mean we are the Begin() iterator.
		m_Id--;
		while (m_Id > 0)	// Begin().m_Id == 0
		{
			if (TSlotArrayInvalidExtractor<_Type>::IsValid(m_Ptr[m_Id]))
				break;
			m_Id--;
		}
		HH_ASSERT(TSlotArrayInvalidExtractor<_Type>::IsValid(m_Ptr[m_Id]));	// we --'ed past the End() iterator
		return *this;
	}

	HH_FORCEINLINE SelfType	operator ++ (int) { SelfType oldValue = *this; this->operator++(); return oldValue; }
	HH_FORCEINLINE SelfType	operator -- (int) { SelfType oldValue = *this; this->operator--(); return oldValue; }
	HH_FORCEINLINE SelfType	&operator += (hh_u32 count) { while (count--) operator ++ (); return *this; }
	HH_FORCEINLINE SelfType	&operator -= (hh_u32 count) { while (count--) operator -- (); return *this; }

	HH_FORCEINLINE SelfType	operator + (hh_u32 count) const { SelfType s(*this); s += count; return s; }
	HH_FORCEINLINE SelfType	operator - (hh_u32 count) const { SelfType s(*this); s -= count; return s; }

	HH_FORCEINLINE _VType	&operator * () const { _AssertIsValid(); return m_Ptr[m_Id]; }
	HH_FORCEINLINE _VType	*operator -> () const { _AssertIsValid(); return &m_Ptr[m_Id]; }

	HH_FORCEINLINE bool		operator == (const SelfType &other) const { HH_ASSERT(m_Ptr == other.m_Ptr && m_SlotCount == other.m_SlotCount); return m_Id == other.m_Id; }
	HH_FORCEINLINE bool		operator != (const SelfType &other) const { return !(*this == other); }
	HH_FORCEINLINE bool		operator > (const SelfType &other) const { return m_Id > other.m_Id; }
	HH_FORCEINLINE bool		operator < (const SelfType &other) const { return m_Id < other.m_Id; }
	HH_FORCEINLINE bool		operator >= (const SelfType &other) const { return m_Id >= other.m_Id; }
	HH_FORCEINLINE bool		operator <= (const SelfType &other) const { return m_Id <= other.m_Id; }
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
TSlotArray_Base<_Type, _Storage>::TSlotArray_Base()
:	m_UsedSlots(0)
,	m_FirstFreeSlot(0)
{
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
hh_u32	TSlotArray_Base<_Type, _Storage>::_JumpToNextFreeSlot(hh_u32 curSlot) const
{
	const hh_u32	count = _Storage::_MaxCount();
	if (curSlot < count)
	{
		while (++curSlot < count && TSlotArrayInvalidExtractor<_Type>::IsValid(_Storage::m_Data[curSlot]))
		{
		}
	}
	return curSlot;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
CGuid	TSlotArray_Base<_Type, _Storage>::_FindFreeSlotIFN()
{
	// pretty naive... improve this
	const hh_u32	count = _Storage::_MaxCount();
	for (hh_u32 i = m_FirstFreeSlot; i < count; i++)
	{
		if (!TSlotArrayInvalidExtractor<_Type>::IsValid(_Storage::m_Data[i]))
		{
			m_FirstFreeSlot = _JumpToNextFreeSlot(m_FirstFreeSlot);	// returns 'count' if no more free slots
			return i;
		}
	}

	if (_Storage::_GrowIFP(count + 1, count))
	{
		m_FirstFreeSlot = count + 1;
		return count;
	}
	m_FirstFreeSlot = count;	// failed growth, no more free slots
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
CGuid	TSlotArray_Base<_Type, _Storage>::_FirstValidSlot() const
{
	for (hh_u32 i = 0; i < _Storage::_MaxCount(); i++)
	{
		if (TSlotArrayInvalidExtractor<_Type>::IsValid(_Storage::m_Data[i]))
			return i;
	}
	// we are empty
	HH_ASSERT(m_UsedSlots == 0);	// make sure we actually are.. just in case.
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
template<typename _Type2>
CGuid		TSlotArray_Base<_Type, _Storage>::Insert(const _Type2 &element)
{
	const CGuid	slot = _FindFreeSlotIFN();
	if (slot.Valid())
	{
		_Storage::m_Data[slot] = element;
		m_UsedSlots++;
	}
	return slot;
}

//----------------------------------------------------------------------------

#if	(PK_HAS_RVALUE_REF != 0)
template<typename _Type, typename _Storage>
CGuid		TSlotArray_Base<_Type, _Storage>::Insert(_Type &&element)
{
	const CGuid	slot = _FindFreeSlotIFN();
	if (slot.Valid())
	{
		_Storage::m_Data[slot] = element;
		m_UsedSlots++;
	}
	return slot;
}
#endif

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
HH_FORCEINLINE void		TSlotArray_Base<_Type, _Storage>::Remove(CGuid slot)
{
	HH_ASSERT(slot.Valid() && slot < _Storage::_MaxCount() && TSlotArrayInvalidExtractor<_Type>::IsValid(_Storage::m_Data[slot]));
	TSlotArrayInvalidExtractor<_Type>::Invalidate(_Storage::m_Data[slot]);
	m_UsedSlots--;

	if (slot < m_FirstFreeSlot)
	{
		m_FirstFreeSlot = slot;
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
void	TSlotArray_Base<_Type, _Storage>::Clear()
{
	const hh_u32	count = _Storage::_MaxCount();
	for (hh_u32 i = 0; i < count; i++)
	{
		if (TSlotArrayInvalidExtractor<_Type>::IsValid(_Storage::m_Data[i]))
		{
			TSlotArrayInvalidExtractor<_Type>::Invalidate(_Storage::m_Data[i]);
		}
	}
	m_FirstFreeSlot = 0;
	m_UsedSlots = 0;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
void	TSlotArray_Base<_Type, _Storage>::Clean()
{
	Clear();
	_Storage::_Clean();
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
bool	TSlotArray_Base<_Type, _Storage>::Reserve(hh_u32 totalCapacity)
{
	const hh_u32	currentCount = _Storage::_MaxCount();
	if (totalCapacity > currentCount)
	{
		return _Storage::_GrowIFP(totalCapacity, currentCount);	// should initialize all elements correctly
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
template<typename _Type2>
CGuid	TSlotArray_Base<_Type, _Storage>::IndexOf(_Type2 const &what) const
{
	const hh_u32	count = _Storage::_MaxCount();
	for (hh_u32 i = 0; i < count; i++)
	{
		//if (TSlotArrayInvalidExtractor<_Type>::IsValid(_Storage::m_Data[i]))
		{
			if (_Storage::m_Data[i] == what)
				return i;
		}
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
template<typename _Type2>
bool	TSlotArray_Base<_Type, _Storage>::Contains(_Type2 const &what) const
{
	return IndexOf(what).Valid();
}

//----------------------------------------------------------------------------
//
//	direct-index accessors
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
HH_FORCEINLINE const _Type	&TSlotArray_Base<_Type, _Storage>::operator [] (CGuid slot) const
{
	HH_ASSERT(slot.Valid() && slot < _Storage::_MaxCount());
	return _Storage::m_Data[slot];
}

template<typename _Type, typename _Storage>
HH_FORCEINLINE _Type		&TSlotArray_Base<_Type, _Storage>::operator [] (CGuid slot)
{
	HH_ASSERT(slot.Valid() && slot < _Storage::_MaxCount());
	return _Storage::m_Data[slot];
}

//----------------------------------------------------------------------------
//
//	Begin()/End() Iterators
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
typename TSlotArray_Base<_Type, _Storage>::ConstIterator	TSlotArray_Base<_Type, _Storage>::Begin() const
{
	const CGuid	id = _FirstValidSlot();
	if (id.Valid())
		return ConstIterator(_Storage::m_Data, id, _Storage::_MaxCount());
	return End();
}

template<typename _Type, typename _Storage>
typename TSlotArray_Base<_Type, _Storage>::ConstIterator	TSlotArray_Base<_Type, _Storage>::End() const
{
	return ConstIterator(_Storage::m_Data, _Storage::_MaxCount(), _Storage::_MaxCount());
}

template<typename _Type, typename _Storage>
typename TSlotArray_Base<_Type, _Storage>::Iterator		TSlotArray_Base<_Type, _Storage>::Begin()
{
	const CGuid	id = _FirstValidSlot();
	if (id.Valid())
		return Iterator(_Storage::m_Data, id, _Storage::_MaxCount());
	return End();
}

template<typename _Type, typename _Storage>
typename TSlotArray_Base<_Type, _Storage>::Iterator		TSlotArray_Base<_Type, _Storage>::End()
{
	return Iterator(_Storage::m_Data, _Storage::_MaxCount(), _Storage::_MaxCount());
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ARRAY_SLOT_INL__
