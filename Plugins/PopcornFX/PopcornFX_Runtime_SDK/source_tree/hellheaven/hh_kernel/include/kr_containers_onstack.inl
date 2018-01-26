#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2009/12/19 16:34 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ONSTACK_INL__
#define __KR_CONTAINERS_ONSTACK_INL__

#ifndef	__KR_CONTAINERS_ONSTACK_H__
#	error	kr_containers_onstack.inl must not be included manually, include kr_containers_onstack.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	stack memory views
//
//----------------------------------------------------------------------------

template<typename _Type>
TStackMemoryView<_Type>::TStackMemoryView(_Type *data, hh_u32 count)
:	TMemoryView<_Type>(data, count)
	HH_ONLY_IF_ASSERTS(COMMA m_SizeChecker(sizeof(_Type) * count))
{
	for (hh_u32 i = 0; i < count; i++)
	{
		Mem::Construct(data[i]);
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
TStackMemoryView<_Type>::~TStackMemoryView()
{
	HH_ASSERT(m_SizeChecker == TMemoryView<_Type>::CoveredBytes());
	for (hh_u32 i = 0; i < TMemoryView<_Type>::m_Count; i++)
	{
		Mem::Destruct(TMemoryView<_Type>::m_Data[i]);
	}
	HH_STACKFREE_IFP(TMemoryView<_Type>::Data(), TMemoryView<_Type>::CoveredBytes());
}

//----------------------------------------------------------------------------
//
//	stack strided memory views
//
//----------------------------------------------------------------------------

template<typename _Type>
TStridedStackMemoryView<_Type>::TStridedStackMemoryView(_Type *data, hh_u32 count, hh_u32 stride /*= sizeof(_Type)*/)
:	TStridedMemoryView<_Type>(data, count, stride)
	HH_ONLY_IF_ASSERTS(COMMA m_SizeChecker(stride * count))
{
	while (count--)
	{
		Mem::Construct(*data);
		Mem::AdvanceRawPointer(data, stride);
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedStackMemoryView<_Type>::~TStridedStackMemoryView()
{
	HH_ASSERT(m_SizeChecker == TStridedMemoryView<_Type>::CoveredBytes());
	_Type	*data = reinterpret_cast<_Type*>(TStridedMemoryView<_Type>::m_Storage.m_RawDataPtr);
	hh_u32	count = TStridedMemoryView<_Type>::m_Storage.m_Count;
	while (count--)
	{
		Mem::Destruct(*data);
		Mem::AdvanceRawPointer(data, TStridedMemoryView<_Type>::m_Storage.m_Stride);
	}
	HH_STACKFREE_IFP(TStridedMemoryView<_Type>::Data(), TStridedMemoryView<_Type>::CoveredBytes());
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedStackMemoryView_Unchecked<_Type>::TStridedStackMemoryView_Unchecked(_Type *data, hh_u32 count, hh_u32 stride /*= sizeof(_Type)*/)
:	TStridedMemoryView<_Type>(data, count, stride)
	HH_ONLY_IF_ASSERTS(COMMA m_SizeChecker(stride * count))
{
	while (count--)
	{
		Mem::Construct(*data);
		Mem::AdvanceRawPointer(data, stride);
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedStackMemoryView_Unchecked<_Type>::~TStridedStackMemoryView_Unchecked()
{
	HH_ONLY_IF_ASSERTS(hh_u32 size = TStridedMemoryView<_Type>::CoveredBytes(); HH_ASSERT(m_SizeChecker == size));
	_Type	*data = reinterpret_cast<_Type*>(TStridedMemoryView<_Type>::m_Storage.m_RawDataPtr);
	hh_u32	count = TStridedMemoryView<_Type>::m_Storage.m_Count;
	while (count--)
	{
		Mem::Destruct(*data);
		Mem::AdvanceRawPointer(data, TStridedMemoryView<_Type>::m_Storage.m_Stride);
	}
	//HH_STACKFREE(TStridedMemoryView<_Type>::Data());
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedStackMemoryView_Safe<_Type>::TStridedStackMemoryView_Safe(_Type *data, hh_u32 count, hh_u32 stride /*= sizeof(_Type)*/)
:	TStridedMemoryView<_Type>(data, count, stride)
	HH_ONLY_IF_ASSERTS(COMMA m_SizeChecker(stride * count))
{
	while (count--)
	{
		Mem::Construct(*data);
		Mem::AdvanceRawPointer(data, stride);
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedStackMemoryView_Safe<_Type>::~TStridedStackMemoryView_Safe()
{
	HH_ONLY_IF_ASSERTS(hh_u32 size = TStridedMemoryView<_Type>::CoveredBytes(); HH_ASSERT(m_SizeChecker == size));
	_Type	*data = reinterpret_cast<_Type*>(TStridedMemoryView<_Type>::m_Storage.m_RawDataPtr);
	hh_u32	count = TStridedMemoryView<_Type>::m_Storage.m_Count;
	while (count--)
	{
		Mem::Destruct(*data);
		Mem::AdvanceRawPointer(data, TStridedMemoryView<_Type>::m_Storage.m_Stride);
	}
	HH_STACKFREE_SAFE(TStridedMemoryView<_Type>::Data());
}

//----------------------------------------------------------------------------
//
//	stack fixed arrays
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TStackFixedArray<_Type>::TStackFixedArray(_Type *data, hh_u32 capacity)
:	m_Data(data)
,	m_Capacity(capacity)
,	m_Count(0)
{
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TStackFixedArray<_Type>::~TStackFixedArray()
{
	if (m_Data != null)
	{
		for (hh_u32 i = 0; i != m_Count; i++)
		{
			Mem::Destruct(m_Data[i]);
		}
		HH_STACKFREE_IFP(m_Data, CoveredBytes());
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE void		TStackFixedArray<_Type>::Resize(hh_u32 count)
{
	HH_ASSERT(count <= m_Capacity);
	if (count < m_Count)
	{
		for (hh_u32 i = count; i != m_Count; i++)	// destroy elements from  'count' to 'm_Count'
		{
			Mem::Destruct(m_Data[i]);
		}
	}
	else
	{
		for (hh_u32 i = m_Count; i != count; i++)	// destroy elements from  'm_Count' to 'count'
		{
			Mem::Construct(m_Data[i]);
		}
	}
	m_Count = count;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE CGuid	TStackFixedArray<_Type>::PushBack(const _Type &element)
{
	HH_ASSERT(m_Count < m_Capacity);
	const hh_u32	insertionIndex = m_Count++;
	Mem::Construct(m_Data[insertionIndex], element);
	return insertionIndex;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE _Type	TStackFixedArray<_Type>::PopBack()
{
	HH_ASSERT(m_Count > 0);
	const hh_u32	index = --m_Count;
	const _Type		poped(m_Data[index]);
	Mem::Destruct(m_Data[index]);
	return poped;
}

//----------------------------------------------------------------------------

template<typename _Type>
void	TStackFixedArray<_Type>::Remove(hh_u32 index)
{
	HH_ASSERT(index < m_Count);
	Mem::Destruct(m_Data[index]);
	--m_Count;
	if (index != m_Count)
	{
		// don't use 'm_Data[index] = m_Data[m_Count], we don't want to trigger an uncontrolled destructor/constructor chain...'
		_Type		*dst = m_Data + index;
		const _Type	*src = m_Data + m_Count;
		if (TTypeTraits::HasTrivialRelocate<_Type>::True)
		{
			if (sizeof(_Type) >= 64)
			{
				Internal::SRelocationHelper<_Type>::Relocate(dst, src, 1);
			}
			else
			{
				hh_u8		*rdst = reinterpret_cast<hh_u8*>(dst);
				const hh_u8	*rsrc = reinterpret_cast<const hh_u8*>(src);
				for (hh_u32 i = 0; i < sizeof(_Type); i++)	// should be optimized away for small types
					*rdst++ = *rsrc++;
			}
		}
		else
		{
			*dst = *src;	// will invoke copy operator for non-trivially relocatable types
		}
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Type2>
CGuid	TStackFixedArray<_Type>::IndexOf(_Type2 const &element)
{
	for (hh_u32 i = 0; i < Count(); ++i)
	{
		if ((*this)[i] == element)
			return i;
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ONSTACK_INL__
