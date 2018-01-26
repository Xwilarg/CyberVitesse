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

#ifndef __KR_CONTAINERS_ARRAY_INL__
#define __KR_CONTAINERS_ARRAY_INL__

#ifndef	__KR_CONTAINERS_ARRAY_H__
#	error	kr_containers_array.inl must not be included manually, include kr_containers_array.h instead
#endif

#include "hh_kernel/include/kr_mem_utils.h"

// perfs
#define	HH_ITERATORS_USE_POINTERS_FOR_CONTIGUOUS_IT
//#define	TARRAYS_FETCH_MAXSIZE_FROM_KR_MEM	// smaller sizeof(TArray<...>), but a bit slower. not sure it still works if disabled

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	TArray_BaseContainerImpl
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
bool	TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::_ReallocBuffer(hh_u32 newSize)
{
	hh_u32	newSizeInBytes = newSize * sizeof(_Type);
#if	(HH_MONITOR_CONTAINERS != 0)
	if (g_KRContainersConfig_MonitorTArrays)
		CContainersMonitorTArrays::RegisterNewMemoryMovement((hh_ireg)newSizeInBytes - (hh_ireg)_MaxCount() * sizeof(_Type));
#endif
	_Type	*tmpBuffer;
	if (HasTrivialRelocate::True)
	{
		tmpBuffer = (_Type*)HH_REALLOC_ALIGNED(m_Data, newSizeInBytes, _Controller::Alignment());
	}
	else
	{
		tmpBuffer = (_Type*)HH_MALLOC_ALIGNED(newSizeInBytes, _Controller::Alignment());
	}

	if (tmpBuffer == null)
	{
		HH_ASSERT_NOT_REACHED_MESSAGE("could not grow array of TArray<%s> to %d elements (%d bytes)\n" COMMA "" COMMA newSize COMMA newSizeInBytes);
		return false;
	}

	if (!HasTrivialRelocate::True)
	{
		Internal::SRelocationHelper<_Type, HasTrivialRelocate::True>::Relocate(tmpBuffer, m_Data, m_Count);
		HH_FREE(m_Data);
	}

	m_Data = tmpBuffer;
	_SetMaxCount(newSize);
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
bool	TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::_Grow(hh_u32 wishedSize)
{
	HH_ASSERT(wishedSize > /*_Count() + */_MaxCount());	// wut?
	const hh_u32	newSize = _Controller::GrowSize(wishedSize);
	return _ReallocBuffer(newSize);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
bool	TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::_RawResize(hh_u32 newSize)	// all the elements that will be removed should have been destroyed beforehand
{
	if (newSize < _Count())
		_SetCount(newSize);	// in case the realloc fails, this will prevent us from seeing ghost elements
	if (newSize == 0)
	{
		if (m_Data != null)
			HH_FREE(m_Data);
		return true;
	}

	if (newSize == _MaxCount() || _ReallocBuffer(newSize))
	{
		_SetCount(newSize);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
void	TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::_Clean()	// frees the buffer
{
	_SetCount(0);
	if (m_Data != null)
		HH_FREE(m_Data);
	_SetMaxCount(0);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::TArray_BaseContainerImpl()
:	m_Data(null)
,	m_Count(0)
{
	_SetMaxCount(0);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::~TArray_BaseContainerImpl()
{
#if	(HH_MONITOR_CONTAINERS != 0)
	if (g_KRContainersConfig_MonitorTArrays) CContainersMonitorTArrays::RegisterNewMemoryMovement(-(hh_ireg)_MaxCount() * sizeof(_Type));
#endif
	if (m_Data != null)
		HH_FREE(m_Data);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate>
void	TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>::Swap(SelfType &swapWith)
{
	HHSwap(m_Data, swapWith.m_Data);
	HHSwap(m_Count, swapWith.m_Count);
#ifndef	TARRAYS_FETCH_MAXSIZE_FROM_KR_MEM
	HHSwap(m_MaxCount, swapWith.m_MaxCount);
#endif
}

//----------------------------------------------------------------------------
//
//	TArray_Base
//
//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE bool	TArray_Base<_Type, _BaseImpl>::_GrowIFN()
{
	const hh_u32	count = BaseImpl::_Count();
	return (count < BaseImpl::_MaxCount()) || BaseImpl::_Grow(count + 1);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE bool	TArray_Base<_Type, _BaseImpl>::_GrowIFN(hh_u32 count)
{
	const hh_u32	wishedCount = BaseImpl::_Count() + count;
	return (wishedCount <= BaseImpl::_MaxCount()) || BaseImpl::_Grow(wishedCount);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE void	TArray_Base<_Type, _BaseImpl>::_DestroyRange(hh_u32 start, hh_u32 end)
{
	Internal::TRangeDestructor<_Type>::Destroy(_BaseImpl::_Data(), start, end);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2, template<typename> class _MemView>
void	TArray_Base<_Type, _BaseImpl>::_Copy(const _MemView<_Type2> &copyFrom)
{
	if (copyFrom.Data() != _RawBegin())
	{
		_DestroyRange(0, BaseImpl::_Count());
		BaseImpl::_SetCount(0);
		if (!copyFrom.Empty())
		{
			const hh_u32	count = copyFrom.Count();
			if (count <= BaseImpl::_MaxCount() || this->_RawResize(count))
			{
				BaseImpl::_SetCount(count);
				Internal::TRangeCopier<_Type, _Type2>::Copy(BaseImpl::_Data(), copyFrom, 0, count);
			}
		}
	}
	else
	{
		HH_ASSERT(copyFrom.Data() == null);	// Otherwise, copying into itself: you should not do that
		HH_ASSERT(copyFrom.Count() == BaseImpl::_Count());
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::TArray_Base(hh_u32 size)
{
	Resize(size);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::TArray_Base(const SelfType &copyFrom)
:	_BaseImpl()
{
#if	(HH_MONITOR_CONTAINERS != 0)
	if (g_KRContainersConfig_MonitorTArrays) CContainersMonitorTArrays::RegisterNewCopyConstructor(copyFrom.Count(), copyFrom.ElementSizeInBytes());
#endif
	_Copy(TMemoryView<const _Type>(copyFrom));
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::TArray_Base(const TMemoryView<const _Type> &copyFrom)
{
	_Copy(copyFrom);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::TArray_Base(const TStridedMemoryViewBase<const _Type> &copyFrom)
{
	_Copy(copyFrom);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::~TArray_Base()
{
	if (BaseImpl::_Data() != null)
		_DestroyRange(0, BaseImpl::_Count());
}

//----------------------------------------------------------------------------
// conversions to memory views

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::operator	TMemoryView<typename TArray_Base<_Type, _BaseImpl>::ValueTypeConst> () const
{
	return TMemoryView<ValueTypeConst>(BaseImpl::_Data(), BaseImpl::_Count());
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::operator	TMemoryView<typename TArray_Base<_Type, _BaseImpl>::ValueType> ()
{
	return TMemoryView<ValueType>(BaseImpl::_Data(), BaseImpl::_Count());
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::operator	TStridedMemoryView<typename TArray_Base<_Type, _BaseImpl>::ValueTypeConst> () const
{
	return TStridedMemoryView<ValueTypeConst>(BaseImpl::_Data(), BaseImpl::_Count(), sizeof(ValueTypeConst));
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
TArray_Base<_Type, _BaseImpl>::operator	TStridedMemoryView<typename TArray_Base<_Type, _BaseImpl>::ValueType> ()
{
	return TStridedMemoryView<ValueType>(BaseImpl::_Data(), BaseImpl::_Count(), sizeof(ValueType));
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::operator = (const SelfType &copyFrom)
{
	if (&copyFrom == this)
		return;
#if	(HH_MONITOR_CONTAINERS != 0)
	if (g_KRContainersConfig_MonitorTArrays) CContainersMonitorTArrays::RegisterNewCopyAssignment(Count(), copyFrom.Count(), ElementSizeInBytes());
#endif
	_Copy(TMemoryView<const _Type>(copyFrom));
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _OtherImpl>
void	TArray_Base<_Type, _BaseImpl>::operator = (const TArray_Base<_Type, _OtherImpl> &copyFrom)
{
	if (&copyFrom == this)
		return;
#if	(HH_MONITOR_CONTAINERS != 0)
	if (g_KRContainersConfig_MonitorTArrays) CContainersMonitorTArrays::RegisterNewCopyAssignment(Count(), copyFrom.Count(), ElementSizeInBytes());
#endif
	_Copy(TMemoryView<const _Type>(copyFrom));
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _OtherImpl>
bool	TArray_Base<_Type, _BaseImpl>::operator == (const TArray_Base<_Type, _OtherImpl> &testWith) const
{
	if (BaseImpl::_Count() != testWith._Count())
		return false;
	for (hh_u32 i = 0; i < BaseImpl::_Count(); i++)
	{
		if (BaseImpl::_Data()[i] != testWith[i])
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _OtherImpl>
bool	TArray_Base<_Type, _BaseImpl>::operator != (const TArray_Base<_Type, _OtherImpl> &testWith) const
{
	return !(*this == testWith);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
bool	TArray_Base<_Type, _BaseImpl>::Reserve(hh_u32 totalCapacity)
{
	if (totalCapacity > BaseImpl::_MaxCount())
		return BaseImpl::_ReallocBuffer(totalCapacity);
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
bool	TArray_Base<_Type, _BaseImpl>::GrowedReserve(hh_u32 totalCapacity)
{
	return _GrowIFN(totalCapacity);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
bool	TArray_Base<_Type, _BaseImpl>::Resize(hh_u32 newSize)
{
	const hh_u32	oldSize = BaseImpl::_Count();
	if (newSize < oldSize)
	{
		_DestroyRange(newSize, oldSize);
	}
	else if (newSize > oldSize)
	{
		if (newSize > BaseImpl::_MaxCount())	// not enough space, grow the buffer (same scheme as a standard PushBack())
		{
			if (!BaseImpl::_Grow(newSize))
				return false;
		}
		Internal::TRangeConstructor<_Type>::ConstructDefault(BaseImpl::_Data(), oldSize, newSize);
	}
	BaseImpl::_SetCount(newSize);
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
bool	TArray_Base<_Type, _BaseImpl>::HardResize(hh_u32 newSize)
{
	const hh_u32	oldSize = BaseImpl::_Count();
	if (newSize < oldSize)
		_DestroyRange(newSize, oldSize);

	if (BaseImpl::_RawResize(newSize))
	{
		HH_ASSERT(BaseImpl::_Count() == newSize);
		Internal::TRangeConstructor<_Type>::ConstructDefault(BaseImpl::_Data(), oldSize, newSize);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _TypeCtor>
CGuid	TArray_Base<_Type, _BaseImpl>::PushBack(_TypeCtor const &element)
{
	// We cannot pushback an element of our own array. undefined behavior in case of resize.
	// Note: we could handle that by computing the ID inside the array prior to resizing, then insert
	// back BaseImpl::_Data()[id] after the resize, but we don't want to pay for the extra code and
	// checks on every single PushBack() throughout the codebase, simply to handle that edge-case,
	// that can be easily fixed in the caller's code. Fire an assert instead:
	HH_ASSERT(hh_ureg(&element + 1) <= hh_ureg(BaseImpl::_Data()) || hh_ureg(&element) >= hh_ureg(BaseImpl::_Data() + BaseImpl::_Count()));

	const hh_u32	insertionIndex = BaseImpl::_Count();
	if (HH_PREDICT_LIKELY(insertionIndex < BaseImpl::_MaxCount()))
	{
_insert:
		BaseImpl::_SetCount(insertionIndex + 1);
		Mem::Construct(BaseImpl::_Data()[insertionIndex], element);
		return insertionIndex;
	}
	if (BaseImpl::_Grow(insertionIndex + 1))
		goto _insert;
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

#if	(PK_HAS_RVALUE_REF != 0)
template<typename _Type, typename _BaseImpl>
CGuid	TArray_Base<_Type, _BaseImpl>::PushBack(_Type &&element)
{
	// We cannot pushback an element of our own array. undefined behavior in case of resize. See PushBack(_T const &)
	HH_ASSERT(hh_ureg(&element + 1) <= hh_ureg(BaseImpl::_Data()) || hh_ureg(&element) >= hh_ureg(BaseImpl::_Data() + BaseImpl::_Count()));

	const hh_u32	insertionIndex = BaseImpl::_Count();
	if (HH_PREDICT_LIKELY(insertionIndex < BaseImpl::_MaxCount()))
	{
_insert:
		BaseImpl::_SetCount(insertionIndex + 1);
		Mem::Construct(BaseImpl::_Data()[insertionIndex], element);
		return insertionIndex;
	}
	if (BaseImpl::_Grow(insertionIndex + 1))
		goto _insert;
	return CGuid::INVALID;
}
#endif	// (PK_HAS_RVALUE_REF != 0)

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
CGuid	TArray_Base<_Type, _BaseImpl>::PushBack()
{
	const hh_u32	insertionIndex = BaseImpl::_Count();
	if (HH_PREDICT_LIKELY(insertionIndex < BaseImpl::_MaxCount()))
	{
_insert:
		BaseImpl::_SetCount(insertionIndex + 1);
		Mem::Construct(BaseImpl::_Data()[insertionIndex]);
		return insertionIndex;
	}
	if (BaseImpl::_Grow(insertionIndex + 1))
		goto _insert;
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------
// these two never fail. the array should be large enough

template<typename _Type, typename _BaseImpl>
template<typename _TypeCtor>
hh_u32	TArray_Base<_Type, _BaseImpl>::PushBackUnsafe(_TypeCtor const &element)
{
	// We cannot pushback an element of our own array. undefined behavior in case of resize. See PushBack(_T const &)
	HH_ASSERT(hh_ureg(&element + 1) < hh_ureg(BaseImpl::_Data()) || hh_ureg(&element) >= hh_ureg(BaseImpl::_Data() + BaseImpl::_Count()));

	const hh_u32	insertionIndex = BaseImpl::_Count();
	HH_ASSERT(insertionIndex < BaseImpl::_MaxCount());
	BaseImpl::_SetCount(insertionIndex + 1);
	Mem::Construct(BaseImpl::_Data()[insertionIndex], element);
	return insertionIndex;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
hh_u32	TArray_Base<_Type, _BaseImpl>::PushBackUnsafe()
{
	const hh_u32	insertionIndex = BaseImpl::_Count();
	HH_ASSERT(insertionIndex < BaseImpl::_MaxCount());
	BaseImpl::_SetCount(insertionIndex + 1);
	Mem::Construct(BaseImpl::_Data()[insertionIndex]);
	return insertionIndex;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
_Type	TArray_Base<_Type, _BaseImpl>::PopBack()
{
	HH_ASSERT(BaseImpl::_Count() > 0);
	const hh_u32	nc = BaseImpl::_Count() - 1;
	const _Type		ret = BaseImpl::_Data()[nc];
	_DestroyRange(nc, nc + 1);
	BaseImpl::_SetCount(nc);
	return ret;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::PopBackAndDiscard()
{
	HH_ASSERT(BaseImpl::_Count() > 0);
	const hh_u32	nc = BaseImpl::_Count() - 1;
	_DestroyRange(nc, nc + 1);
	BaseImpl::_SetCount(nc);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
_Type	TArray_Base<_Type, _BaseImpl>::PopFront()
{
	HH_ASSERT(BaseImpl::_Count() > 0);
	const _Type	value = BaseImpl::_Data()[0];
	Remove_AndKeepOrder(0);
	return value;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::PopFrontAndDiscard()
{
	HH_ASSERT(BaseImpl::_Count() > 0);
	Remove_AndKeepOrder(0);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2, typename _OtherImpl>
CGuid	TArray_Base<_Type, _BaseImpl>::Merge(const TArray_Base<_Type2, _OtherImpl> &other)
{
	if (other.Empty())
		return CGuid::INVALID;

	const CGuid	firstElement = Count();
	if (Reserve(firstElement + other.Count()))
	{
		const _Type2	*writeFrom = other.RawDataPointer();						// other.RawDataPointer()
		_Type			*writeTo = BaseImpl::_Data() + BaseImpl::_Count();			// RawDataPointer() + Count()
		_Type			*stopAt = writeTo + other.Count();	// writeTo + other.Count()
		hh_ireg			readOffset = hh_ireg(writeFrom) - hh_ireg(writeTo);
		while (writeTo < stopAt)
		{
			Mem::Construct(writeTo[0], *reinterpret_cast<const _Type2*>(reinterpret_cast<const hh_u8*>(writeTo) + readOffset));
			writeTo++;
		}
		BaseImpl::_SetCount(BaseImpl::_Count() + other.Count());
		return firstElement;
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _TypeCtor>
CGuid	TArray_Base<_Type, _BaseImpl>::Insert(_TypeCtor const &element, hh_u32 where)
{
	// We cannot insert an element of our own array. undefined behavior in case of resize. See PushBack(_T const &)
	HH_ASSERT(hh_ureg(&element + 1) < hh_ureg(BaseImpl::_Data()) || hh_ureg(&element) >= hh_ureg(BaseImpl::_Data() + BaseImpl::_Count()));

	const hh_u32	oldCount = BaseImpl::_Count();
	HH_ASSERT(where <= oldCount);
	if (_GrowIFN())
	{
		// FIXME: eventually merge this with a potential realloc that has to copy things around anyway ?
		if (where < oldCount)
		{
			_Type		*dst = BaseImpl::_Data() + where + 1;
			const _Type	*src = dst - 1;
			Relocator::Relocate_OverlappedExpand(dst, src, oldCount - where);
		}

		Mem::Construct(BaseImpl::_Data()[where], element);
		const hh_u32	nc = BaseImpl::_Count();
		BaseImpl::_SetCount(nc + 1);
		return where;
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
CGuid	TArray_Base<_Type, _BaseImpl>::InsertDefault(hh_u32 where)
{
	const hh_u32	oldCount = BaseImpl::_Count();
	HH_ASSERT(where <= oldCount);
	if (_GrowIFN())
	{
		// FIXME: eventually merge this with a potential realloc that has to copy things around anyway ?
		if (where < oldCount)
		{
			_Type		*dst = BaseImpl::_Data() + where + 1;
			const _Type	*src = dst - 1;
			Relocator::Relocate_OverlappedExpand(dst, src, oldCount - where);
		}

		Mem::Construct(BaseImpl::_Data()[where]);
		const hh_u32	nc = BaseImpl::_Count();
		BaseImpl::_SetCount(nc + 1);
		return where;
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _TypeArray>
CGuid	TArray_Base<_Type, _BaseImpl>::InsertArray(const _TypeArray &array, hh_u32 where)
{
	HH_ASSERT(BaseImpl::_Data() == null || array.Empty() || &array[0] != BaseImpl::_Data());	// can't insert into itself !
	const hh_u32	oldCount = BaseImpl::_Count();
	const hh_u32	addCount = array.Count();
	HH_ASSERT(where <= oldCount);
	if (!_GrowIFN(addCount))
		return CGuid::INVALID;

	if (addCount != 0)
	{
		// if we're not inserting at the end, we need to expand the old array to make space for the new one
		_Type	*dst = BaseImpl::_Data() + where;
		if (where < oldCount)
		{
			const _Type	*src = dst;
			Relocator::Relocate_OverlappedExpand(dst + addCount, src, oldCount - where);
		}

		// copy the new array in place
		Internal::TRangeCopier<_Type, typename _TypeArray::ValueTypeConst>::Copy(dst, array.View(), 0, addCount);

		BaseImpl::_SetCount(oldCount + addCount);
	}
	return where;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::Clear()	// empties the array, but doesn't free the memory, in case you need to push some more stuff in
{
	_DestroyRange(0, BaseImpl::_Count());
	BaseImpl::_SetCount(0);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::Clean()	// frees the buffer
{
	_DestroyRange(0, BaseImpl::_Count());
	BaseImpl::_Clean();
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE void	TArray_Base<_Type, _BaseImpl>::Remove_AndKeepOrder(hh_u32 index)
{
	HH_ASSERT(index < BaseImpl::_Count());
	Remove_AndKeepOrder(index, index + 1);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::Remove_AndKeepOrder(hh_u32 indexStart, hh_u32 indexEnd)
{
	HH_ASSERT(indexStart < indexEnd);
	HH_ASSERT(indexEnd <= BaseImpl::_Count());
	_DestroyRange(indexStart, indexEnd);

	const hh_u32	copyRange = BaseImpl::_Count() - indexEnd;
	if (copyRange > 0)
	{
		_Type		*dst = BaseImpl::_Data() + indexStart;
		const _Type	*src = BaseImpl::_Data() + indexEnd;
		Relocator::Relocate_OverlappedCompact(dst, src, copyRange);
	}

	BaseImpl::_SetCount(indexStart + copyRange);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE typename TArray_Base<_Type, _BaseImpl>::Iterator	TArray_Base<_Type, _BaseImpl>::RemoveIt_AndKeepOrder(const Iterator &it)	// returns the element right after 'it' once 'it' has been removed
{
	ValueType	*itPtr = &(*it);
	const CGuid	i = IndexOfElementFromRawPointerInArray(itPtr);
	Remove_AndKeepOrder(i);
	HH_ASSERT(itPtr == BaseImpl::_Data() + i);	// Remove_AndKeepOrder() should not change the buffer base
#if 1
	return it;
#else
#if	(HH_ITERATORS_DEBUG_CHECK == 0)
	return Iterator(itPtr);
#else
	return Iterator(itPtr, this);
#endif
#endif
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE void	TArray_Base<_Type, _BaseImpl>::RemoveElementFromRawPointerInArray_AndKeepOrder(ValueTypeConst *element)
{
	const CGuid	index = IndexOfElementFromRawPointerInArray(element);
	if (index.Valid())
	{
		Remove_AndKeepOrder(static_cast<hh_u32>(index));
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE void	TArray_Base<_Type, _BaseImpl>::RemoveElement_AndKeepOrder(ValueTypeConst const &element)
{
	const CGuid	index = IndexOf(element);
	if (index.Valid())
	{
		Remove_AndKeepOrder(static_cast<hh_u32>(index));
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::RemoveAllElements_AndKeepOrder(ValueTypeConst const &element)
{
	// we could use a small stack-based history of slot positions to remove, and remove everything in one go.
	// ex: handle at max. 32 removals per pass, and move the contiguous memory ranges in one go.
	// we could also have a special function RemoveElementsAt_AndKeepOrder(TMemoryView<const hh_u32> &)
	// that would accept a list of slots to remove, and remove them all in one go, taking care of the memory moves...

	for (hh_u32 i = 0; i < BaseImpl::_Count(); )
	{
		if (element != BaseImpl::_Data()[i])
			i++;
		else
			Remove_AndKeepOrder(i);
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
void	TArray_Base<_Type, _BaseImpl>::Remove(hh_u32 index)
{
	const hh_u32	oldCount = BaseImpl::_Count();
	const hh_u32	newCount = oldCount - 1;
	HH_ASSERT(index < oldCount);
	_DestroyRange(index, index + 1);
	BaseImpl::_SetCount(newCount);
	if (index != newCount)
	{
		// swap with last
		_Type		*dst = BaseImpl::_Data() + index;
		const _Type	*src = BaseImpl::_Data() + BaseImpl::_Count();
		Relocator::Relocate(dst, src, 1);
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE typename TArray_Base<_Type, _BaseImpl>::Iterator	TArray_Base<_Type, _BaseImpl>::RemoveIt(const Iterator &it)	// returns the element right after 'it' once 'it' has been removed
{
	ValueType	*itPtr = &(*it);
	const CGuid	i = IndexOfElementFromRawPointerInArray(itPtr);
	Remove(i);
	HH_ASSERT(itPtr == BaseImpl::_Data() + i);	// Remove() should not change the buffer base
#if 1
	return it;
#else
#if	(HH_ITERATORS_DEBUG_CHECK == 0)
	return Iterator(itPtr);
#else
	return Iterator(itPtr, this);
#endif
#endif
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
HH_FORCEINLINE void	TArray_Base<_Type, _BaseImpl>::RemoveElementFromRawPointerInArray(ValueTypeConst *element)
{
	const CGuid	index = IndexOfElementFromRawPointerInArray(element);
	if (index.Valid())
	{
		Remove(static_cast<hh_u32>(index));
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
HH_FORCEINLINE void	TArray_Base<_Type, _BaseImpl>::RemoveElement(_Type2 const &element)
{
	const CGuid	index = IndexOf(element);
	if (index.Valid())
	{
		Remove(static_cast<hh_u32>(index));
	}
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
hh_u32	TArray_Base<_Type, _BaseImpl>::RemoveAllElements(_Type2 const &element)
{
	// see 'RemoveAllElements_AndKeepOrder' for possible improvements
	const hh_u32	oldCount = BaseImpl::_Count();
	for (hh_u32 i = 0; i < BaseImpl::_Count(); )
	{
		if (element != BaseImpl::_Data()[i])
			i++;
		else
			Remove(i);
	}
	return oldCount - BaseImpl::_Count();
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
CGuid	TArray_Base<_Type, _BaseImpl>::IndexOfElementFromRawPointerInArray(ValueTypeConst *elementInArray) const
{
	const hh_ireg	offset = elementInArray - BaseImpl::_Data();
	HH_ASSERT(offset >= 0 && offset < static_cast<hh_ireg>(BaseImpl::_Count()));
	return static_cast<CGuid>(offset);
};

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
CGuid	TArray_Base<_Type, _BaseImpl>::IndexOfFirst(_Type2 const &element) const
{
	for (hh_u32 i = 0; i < BaseImpl::_Count(); i++)
	{
		if (element == BaseImpl::_Data()[i])
			return i;
	}
	return CGuid::INVALID;
};

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
CGuid	TArray_Base<_Type, _BaseImpl>::IndexOfLast(_Type2 const &element) const
{
	for (hh_u32 i = BaseImpl::_Count(); i-- != 0; )
	{
		if (element == BaseImpl::_Data()[i])
			return i;
	}
	return CGuid::INVALID;
};

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
bool	TArray_Base<_Type, _BaseImpl>::Contains(_Type2 const &element) const
{
	return IndexOf(element).Valid();
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
typename TArray_Base<_Type, _BaseImpl>::ValueTypeConst	*TArray_Base<_Type, _BaseImpl>::Find(_Type2 const &element) const
{
	for (hh_u32 i = 0; i < BaseImpl::_Count(); i++)
	{
		if (element == BaseImpl::_Data()[i])
			return BaseImpl::_Data() + i;
	}
	return null;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
template<typename _Type2>
typename TArray_Base<_Type, _BaseImpl>::ValueType	*TArray_Base<_Type, _BaseImpl>::Find(_Type2 const &element)
{
	for (hh_u32 i = 0; i < BaseImpl::_Count(); i++)
	{
		if (element == BaseImpl::_Data()[i])
			return BaseImpl::_Data() + i;
	}
	return null;
}

//----------------------------------------------------------------------------
//
//	Internal helpers
//
//----------------------------------------------------------------------------
#if 0
namespace	Internal
{
	template<typename _Type, typename _ContainerType>
	struct	TRangeCopier<TArray_Base<_Type, _ContainerType> >
	{
		template<typename _TypeDst>
		static HH_INLINE void	Copy(_TypeDst *dst, const TArray_Base<_Type, _ContainerType> &array, hh_u32 start, hh_u32 end)
		{
			HH_ASSERT(start >= 0 && end <= array.Count());
			HH_ASSERT(dst != null);

			if (TTypeTraits::HasTrivialConstructor<_TypeDst>::False || sizeof(_TypeDst) != sizeof(_Type))
			{
				for (hh_u32 i = start; i < end; i++)
				{
					Mem::Construct<_TypeDst>(dst[i], array[i]);
				}
			}
			else
			{
				HH_STATIC_ASSERT(sizeof(_TypeDst) == sizeof(_Type));
				memcpy(dst + start, &(array[0]), (end - start) * sizeof(*dst));
			}
		}
	};

	template<typename _Type, typename _ContainerType>
	struct	TRangeCopier<TArray<_Type, _ContainerType> >
	{
		template<typename _TypeDst>
		static HH_FORCEINLINE void	Copy(_TypeDst *dst, const TArray<_Type, _ContainerType> &array, hh_u32 start, hh_u32 end)
		{
			TRangeCopier<typename TArray<_Type, _ContainerType>::BaseType>::Copy(dst, array, start, end);
		}
	};
}
#endif
//----------------------------------------------------------------------------
//
//	Semi-dynamic arrays
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
bool	TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::_ReallocBuffer(hh_u32 newSize)
{
//	HH_STATIC_ASSERT_CUSTOM(HasTrivialRelocate::True, __NOT_IMPLEMENTED_FOR_NON_RELOCABLE_TYPES__);
	if (newSize <= kRawStaticCount)
	{
		if (Allocated())
		{
			void	*data = _Data();
			HH_ASSERT((void*)m_StaticData != data);
			Internal::SRelocationHelper<_Type, HasTrivialRelocate::True>::Relocate(Mem::Align<kAlignment>(Mem::Align<kAlignment>(reinterpret_cast<_Type*>(m_StaticData))), static_cast<const _Type*>(data), _Count());
			HH_FREE(data);
			SetAllocated(false);
		}
		return true;
	}
	const hh_u32	newSizeInBytes = newSize * sizeof(_Type);

//#if	(HH_MONITOR_CONTAINERS != 0)
//	if (g_KRContainersConfig_MonitorTArrays)
//		CContainersMonitorTArrays::RegisterNewMemoryMovement((hh_ireg)newSizeInBytes - (hh_ireg)_MaxCount() * sizeof(_Type));
//#endif

	_Type	*buffer = null;
	if (Allocated())
	{
		if (newSize == 0)
		{
			void	*data = _Data();
			if (data != null)
				HH_FREE(data);
			SetAllocated(false);
			return true;
		}
		HH_ASSERT(_Data() != null);	// if we are tagged as 'Allocated()', _Data() shouldn't be null
		if (HasTrivialRelocate::True)	// we can relocate with a raw copy
		{
			buffer = (_Type*)HH_REALLOC_ALIGNED(_Data(), newSizeInBytes, _Controller::Alignment());	// unsafe for non-relocable types
		}
		else
		{
			// we can't relocate, we have to manually construct/destruct the elements
			buffer = (_Type*)HH_MALLOC_ALIGNED(newSizeInBytes, _Controller::Alignment());
			Internal::SRelocationHelper<_Type, HasTrivialRelocate::True>::Relocate(buffer, _Data(), _Count());
		}
		if (buffer == null)
			return false;
	}
	else
	{
		buffer = (_Type*)HH_MALLOC_ALIGNED(newSizeInBytes, _Controller::Alignment());
		if (buffer == null)
			return false;
		Internal::SRelocationHelper<_Type, HasTrivialRelocate::True>::Relocate(buffer, Mem::Align<kAlignment>(reinterpret_cast<const _Type*>(m_StaticData)), _Count());
	}
	SetAllocatedDataPtr(buffer);
	_SetMaxCount(newSize);
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
bool	TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::_Grow(hh_u32 wishedSize)
{
	if (wishedSize <= kRawStaticCount)
	{
		if (!Allocated())	// it can still fit inside our static array
			return true;
		// FIXME: it fits (again ?) should we free the allocated buffer and roll-back to using the static storage?
	}
	HH_ASSERT(wishedSize > _MaxCount());
	const hh_u32	newSize = _Controller::GrowSize(wishedSize);
	return _ReallocBuffer(newSize);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
bool	TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::_RawResize(hh_u32 newSize)
{
	if (newSize < _Count())
		_SetCount(newSize);	// in case the realloc fails, this will prevent us from seeing ghost elements
	if (_ReallocBuffer(newSize))
	{
		_SetCount(newSize);	// we cannot fail if we use our static array
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
void	TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::_Clean()	// frees the buffer
{
	_SetCount(0);
	if (Allocated())
	{
#if	(HH_MONITOR_CONTAINERS != 0)
		if (g_KRContainersConfig_MonitorTArrays)
			CContainersMonitorTArrays::RegisterNewMemoryMovement(-(hh_ireg)_MaxCount() * sizeof(_Type));
#endif
		void	*data = _Data();
		HH_ASSERT(data != null);
		HH_FREE(data);
		SetAllocated(false);
	}
	_SetMaxCount(0);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::TSemiDynamicArray_BaseContainerImpl()
:	m_Count(0)
{
//	HH_STATIC_ASSERT(_FixedCount * sizeof(_Type) >= sizeof(void*));
	_SetMaxCount(0);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::~TSemiDynamicArray_BaseContainerImpl()
{
	_Clean();
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate>
void	TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller, _HasTrivialRelocate>::Swap(SelfType &swapWith)
{
	// now, m_Allocated and m_StaticData overlaps !
	// we need to be careful
	// also !! mind Alignments !! of m_StaticData
	const bool			selfAllocated = Allocated();
	const bool			otherAllocated = swapWith.Allocated();
	if (selfAllocated && otherAllocated)
	{
		HHSwap(m_Count, swapWith.m_Count);
		HHSwap(m_Allocated.m_Data, swapWith.m_Allocated.m_Data);
		HHSwap(m_Allocated.m_MaxCount, swapWith.m_Allocated.m_MaxCount);
	}
	else if (selfAllocated || otherAllocated)
	{
		SelfType			* const allocated = (selfAllocated ? this : &swapWith);
		SelfType			* const staticed = (selfAllocated ? &swapWith : this);
		const SAllocated	alloc = allocated->m_Allocated; // copy here, will be erased by dstStatic
		_Type const			* const srcStatic = staticed->_Data();
		_Type				* const dstStatic = Mem::Align<kAlignment>(reinterpret_cast<_Type*>(allocated->m_StaticData));
		const hh_u32		staticCount = staticed->_Count();
		for (hh_u32 i = 0; i < staticCount; ++i)
			dstStatic[i] = srcStatic[i];
		HHSwap(m_Count, swapWith.m_Count);
		staticed->m_Allocated = alloc;
	}
	else // both static
	{
		_Type			* const src = _Data();
		_Type			* const dst = swapWith._Data();
		const hh_u32	count = HHMax(_Count(), swapWith._Count());
		HHSwap(m_Count, swapWith.m_Count);
		for (hh_u32 i = 0; i < count; i++)
		{
			HHSwap(src[i], dst[i]);
		}
	}
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ARRAY_INL__
