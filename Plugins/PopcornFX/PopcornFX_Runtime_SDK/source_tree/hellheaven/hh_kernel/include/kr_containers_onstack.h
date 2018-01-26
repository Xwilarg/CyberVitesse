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

#ifndef __KR_CONTAINERS_ONSTACK_H__
#define __KR_CONTAINERS_ONSTACK_H__

#include "hh_kernel/include/kr_memoryviews.h"
#include "hh_kernel/include/kr_containers_internals.h"

#if	(HH_CPU_MODEL != HH_CPU_CELL_SPU)
#	include "hh_kernel/include/kr_mem.h"	// needed for HH_MALLOC/HH_FREE in safe stack containers :/
#endif

#ifdef	HH_PS3
#	include <alloca.h>
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Stack-allocated containers
//
//----------------------------------------------------------------------------

#if	defined(HH_DESKTOP)
#	define	HH_STACKALLOC_MAX_SIZE						1024 * 256	// 256 Kb
#else
#	define	HH_STACKALLOC_MAX_SIZE						1024 * 32	// 32 Kb
#endif

// FIXME: we could remove all __size args in the STACKFREE macros, by using Mem::OnStack, but we should check the actual perfs of a call to Mem::OnStack()

#if	(PK_HAS_ALLOCA != 0)

#if	(HH_CPU_MODEL == HH_CPU_CELL_SPU)

#define	HH_STACKALLOC_IFP(__size)						PK_ALLOCA(__size)
#define	HH_STACKFREE_IFP(__ptr, __size)					do { } while (0)
#define	HH_STACKALIGNEDALLOC_IFP(__size, __al)			PopcornFX::Mem::Align<__al>(PK_ALLOCA((__size) + ((__al) - 1)))
#define	HH_STACKALIGNEDFREE_IFP(__ptr, __size, __al)	do { } while (0)

#define	HH_STACKALLOC_SAFE(__size)						PK_ALLOCA(__size)
#define	HH_STACKFREE_SAFE(__ptr)						do { } while (0)
#define	HH_STACKALIGNEDALLOC_SAFE(__size, __al)			PopcornFX::Mem::Align<__al>(PK_ALLOCA((__size) + ((__al) - 1)))
#define	HH_STACKALIGNEDFREE_SAFE(__ptr, __al)			do { } while (0)

#else

#define	HH_STACKALLOC_IFP(__size)						(((__size) > HH_STACKALLOC_MAX_SIZE) ? HH_MALLOC(__size) : PK_ALLOCA(__size))
#define	HH_STACKFREE_IFP(__ptr, __size)					do { if ((__size) > HH_STACKALLOC_MAX_SIZE) { void *localPtr = __ptr; HH_FREE(localPtr); } } while (0)
#define	HH_STACKALIGNEDALLOC_IFP(__size, __al)			(((__size) + ((__al) - 1) > HH_STACKALLOC_MAX_SIZE) ? HH_MALLOC_ALIGNED(__size, __al) : PopcornFX::Mem::Align<__al>(PK_ALLOCA((__size) + ((__al) - 1))))
#define	HH_STACKALIGNEDFREE_IFP(__ptr, __size, __al)	do { if ((__size) + ((__al) - 1) > HH_STACKALLOC_MAX_SIZE) { void *localPtr = __ptr; HH_FREE(localPtr); } } while (0)

#define	HH_STACKALLOC_SAFE(__size)						(((__size) > PopcornFX::Mem::StackSpaceLeftAtSP(null)) ? HH_MALLOC(__size) : PK_ALLOCA(__size))
#define	HH_STACKFREE_SAFE(__ptr)						do { if (!PopcornFX::Mem::OnStack(__ptr)) { void *localPtr = __ptr; HH_FREE(localPtr); } } while (0)
#define	HH_STACKALIGNEDALLOC_SAFE(__size, __al)			(((__size) + ((__al) - 1) > PopcornFX::Mem::StackSpaceLeftAtSP(null)) ? HH_MALLOC_ALIGNED(__size, __al) : PopcornFX::Mem::Align<__al>(PK_ALLOCA((__size) + ((__al) - 1))))
#define	HH_STACKALIGNEDFREE_SAFE(__ptr, __al)			do { if (!Mem::OnStack(__ptr)) { void *localPtr = __ptr; HH_FREE(localPtr); } } while (0)

#endif

#else

// can't use alloca. use standard heap allocation (ouch)

#define	HH_STACKALLOC_IFP(__size)						HH_MALLOC(__size)
#define	HH_STACKFREE_IFP(__ptr, __size)					do { void *localPtr = __ptr; HH_FREE(localPtr); } while (0)
#define	HH_STACKALIGNEDALLOC_IFP(__size, __al)			HH_MALLOC_ALIGNED(__size, __al)
#define	HH_STACKALIGNEDFREE_IFP(__ptr, __size, __al)	do { void *localPtr = __ptr; HH_FREE(localPtr); } while (0)

#define	HH_STACKALLOC_SAFE(__size)						HH_MALLOC(__size)
#define	HH_STACKFREE_SAFE(__ptr)						do { void *localPtr = __ptr; HH_FREE(localPtr); } while (0)
#define	HH_STACKALIGNEDALLOC_SAFE(__size, __al)			HH_MALLOC_ALIGNED(__size, __al)
#define	HH_STACKALIGNEDFREE_SAFE(__ptr, __al)			do { void *localPtr = __ptr; HH_FREE(localPtr); } while (0)

#endif

//----------------------------------------------------------------------------

template<typename _Type>
class TStackMemoryView : public TMemoryView<_Type>
{
private:
	HH_ONLY_IF_ASSERTS(hh_u32 m_SizeChecker);
public:
	explicit TStackMemoryView(_Type *data, hh_u32 count);
	~TStackMemoryView();
};

//----------------------------------------------------------------------------

#define	HH_STACKMEMORYVIEW(__type, __name, __count)					PopcornFX::TStackMemoryView<__type >	__name((__type*)HH_STACKALLOC_IFP(sizeof(__type) * (__count)), (__count));
#define	HH_STACKALIGNEDMEMORYVIEW(__type, __name, __count, __al)	PopcornFX::TStackMemoryView<__type >	__name((__type*)HH_STACKALIGNEDALLOC_IFP(sizeof(__type) * (__count), (__al)), (__count));

//----------------------------------------------------------------------------

template<typename _Type>
class TStridedStackMemoryView : public TStridedMemoryView<_Type>
{
private:
	HH_ONLY_IF_ASSERTS(hh_u32 m_SizeChecker);

public:
	explicit TStridedStackMemoryView(_Type *data, hh_u32 count, hh_u32 stride = sizeof(_Type));
	~TStridedStackMemoryView();
};

//----------------------------------------------------------------------------

template<typename _Type>
class TStridedStackMemoryView_Unchecked : public TStridedMemoryView<_Type>
{
private:
	HH_ONLY_IF_ASSERTS(hh_u32 m_SizeChecker);

public:
	explicit TStridedStackMemoryView_Unchecked(_Type *data, hh_u32 count, hh_u32 stride = sizeof(_Type));
	~TStridedStackMemoryView_Unchecked();
};

//----------------------------------------------------------------------------

template<typename _Type>
class TStridedStackMemoryView_Safe : public TStridedMemoryView<_Type>
{
private:
	HH_ONLY_IF_ASSERTS(hh_u32 m_SizeChecker);

public:
	explicit TStridedStackMemoryView_Safe(_Type *data, hh_u32 count, hh_u32 stride = sizeof(_Type));
	~TStridedStackMemoryView_Safe();
};

//----------------------------------------------------------------------------

#define	HH_STACKSTRIDEDMEMORYVIEW(__type, __name, __count)							PopcornFX::TStridedStackMemoryView<__type>		__name((__type*)HH_STACKALLOC_IFP(sizeof(__type) * (__count)), (__count));
#define	HH_STACKSTRIDEDMEMORYVIEW_SAFE(__type, __name, __count)						PopcornFX::TStridedStackMemoryView_Safe<__type>	__name((__type*)HH_STACKALLOC_SAFE(sizeof(__type) * (__count)), (__count));

#define	HH_STACKALIGNEDSTRIDEDMEMORYVIEW(__type, __name, __count, __al)				PopcornFX::TStridedStackMemoryView<__type>		__name((__type*)HH_STACKALIGNEDALLOC_IFP(sizeof(__type) * (__count), (__al)), (__count));
#define	HH_STACKALIGNEDSTRIDEDMEMORYVIEW_SAFE(__type, __name, __count, __al)		PopcornFX::TStridedStackMemoryView_Safe<__type>	__name((__type*)HH_STACKALIGNEDALLOC_SAFE(sizeof(__type) * (__count), (__al)), (__count));

//----------------------------------------------------------------------------

template<typename _Type>
class	TStackFixedArray
{
public:
	typedef	_Type										ValueType;
	//typedef typename TType_BuildConst<_Type>::Type		ValueTypeConst;
	typedef TStackFixedArray<_Type>						SelfType;

	typedef	TContiguousBaseIterator<ValueType>			Iterator;
 	typedef	TContiguousBaseIterator<ValueType const>	ConstIterator;

private:
	_Type		*m_Data;
	hh_u32		m_Capacity;
	hh_u32		m_Count;

	void		_AssertAccessible(hh_u32 index) const { HH_ASSERT(m_Data != null); HH_ASSERT(index < m_Count); }
public:
	explicit TStackFixedArray(_Type *data, hh_u32 capacity);
	~TStackFixedArray();

	// conversions to memory views
	HH_FORCEINLINE operator	TMemoryView<_Type> () { return TMemoryView<_Type>(&m_Data[0], m_Count); }
	HH_FORCEINLINE operator	TMemoryView<_Type const> () const { return TMemoryView<_Type const>(&m_Data[0], m_Count); }
	HH_FORCEINLINE operator	TStridedMemoryView<_Type> () { return TStridedMemoryView<_Type>(&m_Data[0], m_Count, sizeof(_Type)); }
	HH_FORCEINLINE operator	TStridedMemoryView<_Type const> () const { return TStridedMemoryView<_Type const>(&m_Data[0], m_Count, sizeof(_Type)); }

	HH_FORCEINLINE void				Clear() { /*m_Count = 0;*/Resize(0); }	// the compiler should be able to optimize this away for trivial ctors/dtors
	void							Resize(hh_u32 count);

	HH_FORCEINLINE bool				Empty() const { return m_Count == 0; }
	HH_FORCEINLINE hh_u32			Capacity() const { return m_Capacity; }
	HH_FORCEINLINE hh_u32			Count() const { return m_Count; }
	HH_FORCEINLINE bool				Full() const { return m_Count == m_Capacity; }

	HH_FORCEINLINE hh_u32			CoveredBytes() const { return m_Capacity * sizeof(_Type); }
	HH_FORCEINLINE _Type			*Data() { return m_Data; }
	HH_FORCEINLINE const _Type		*Data() const { return m_Data; }
	HH_FORCEINLINE _Type			&operator [] (hh_u32 index) const { HH_ONLY_IF_ASSERTS(_AssertAccessible(index)); return m_Data[index]; }
	HH_FORCEINLINE _Type			*operator + (hh_u32 index) const /* warning: unsafe! be careful with operator precedence! */ { return m_Data + index; }

	TMemoryView<ValueType const>	View() const { return TMemoryView<ValueType const>(m_Data, m_Count); }
	TMemoryView<ValueType>			ViewForWriting() { return TMemoryView<ValueType>(m_Data, m_Count); }

	HH_FORCEINLINE _Type			&First() const { return operator [] (0); }
	HH_FORCEINLINE _Type			&Last() const { return operator [] (m_Count - 1); }

	CGuid							PushBack(const _Type &element);
	_Type							PopBack();
	HH_FORCEINLINE CGuid			Push(const _Type &element) { return PushBack(element); }
	HH_FORCEINLINE _Type			Pop() { return PopBack(); }

	void							Remove(hh_u32 index);

	HH_FORCEINLINE Iterator			Begin() { return Iterator(m_Data); }
	HH_FORCEINLINE Iterator			End() { return Iterator(m_Data + m_Count); }
	HH_FORCEINLINE ConstIterator	Begin() const { return ConstIterator(m_Data); }
	HH_FORCEINLINE ConstIterator	End() const { return ConstIterator(m_Data + m_Count); }

	template <typename _Type2>
	CGuid							IndexOf(_Type2 const &element);
	template <typename _Type2>
	bool							Contains(_Type2 const &element) { return IndexOf(element).Valid(); }
};

//----------------------------------------------------------------------------

#define	HH_STACKFIXEDARRAY(__type, __name, __capacity)					PopcornFX::TStackFixedArray<__type>	__name((__type*)HH_STACKALLOC_IFP(sizeof(__type) * (__capacity)), (__capacity));
#define	HH_STACKALIGNEDFIXEDARRAY(__type, __name, __capacity, __al)		PopcornFX::TStackFixedArray<__type>	__name((__type*)HH_STACKALIGNEDALLOC_IFP(sizeof(__type) * (__capacity), (__al)), (__capacity));

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_containers_onstack.inl"

#endif // __KR_CONTAINERS_ONSTACK_H__

