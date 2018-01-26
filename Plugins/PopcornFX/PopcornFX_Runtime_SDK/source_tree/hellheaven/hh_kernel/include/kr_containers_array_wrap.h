#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/09/14 11:44 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ARRAY_WRAP_H__
#define __KR_CONTAINERS_ARRAY_WRAP_H__

#include "hh_kernel/include/kr_memoryviews.h"
#include "hh_kernel/include/kr_containers_internals.h"
#include "hh_kernel/include/kr_containers_static.h"
#include "hh_kernel/include/kr_mem.h"
#include "hh_kernel/include/kr_refptr.h"
#include "hh_kernel/include/hh_kernel_config.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	TWrapArrayStackReuse
//
//----------------------------------------------------------------------------

template <typename _Type>
struct TReusePolicy_None
{
	static void		Aquire(_Type &type) {}
	static void		Release(_Type &type) {}
};

template <typename _Type>
struct TReusePolicy_Clear
{
	static void		Aquire(_Type &type) {}
	static void		Release(_Type &type) { type.Clear(); }
};

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy = TReusePolicy_Clear>
class TWrapArrayStackReuse
{
public:
	typedef TWrapArrayStackReuse<_ArrayType, _ReusePolicy>		SelfType;
	typedef _ArrayType						ArrayType;
	typedef typename ArrayType::ValueType	ValueType;

	typedef	_ReusePolicy<ValueType>	CReusePolicy;

	TWrapArrayStackReuse() : m_UsedCount(0) {}
	~TWrapArrayStackReuse() { }

	ArrayType const		&RawArray() const { return m_Array; }

	void				Clear();
	void				Clean();

	bool				Empty() const { return m_UsedCount == 0; }
	hh_u32				Count() const { return m_UsedCount; }
	bool				Reserve(hh_u32 count);
	bool				Resize(hh_u32 count);
	TMemoryView<ValueType const>	View() const { return m_Array.View().Slice(0, m_UsedCount); }
	TMemoryView<ValueType>			ViewForWriting() { return m_Array.ViewForWriting().Slice(0, m_UsedCount); }

	HH_FORCEINLINE const ValueType		&operator [] (hh_u32 index) const { HH_ASSERT(index < m_UsedCount); return m_Array[index]; }
	HH_FORCEINLINE ValueType			&operator [] (hh_u32 index) { HH_ASSERT(index < m_UsedCount); return m_Array[index]; }

	CGuid				PushBack();
	ValueType			*PushBackReserved();
	void				PopBackAndDiscard();


protected:
	hh_u32				m_UsedCount;
	ArrayType			m_Array;
};

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
void	TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::Clear()
{
	for (hh_u32 i = 0; i < m_UsedCount; ++i)
		CReusePolicy::Release(m_Array[i]);
	m_UsedCount = 0;
}

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
void	TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::Clean()
{
	for (hh_u32 i = 0; i < m_UsedCount; ++i)
		CReusePolicy::Release(m_Array[i]);
	m_UsedCount = 0;
	m_Array.Clean();
}

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
bool	TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::Reserve(hh_u32 count)
{
	if (count <= m_Array.Count())
		return true;
	return m_Array.Resize(count);
}

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
bool	TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::Resize(hh_u32 count)
{
	if (!Reserve(count))
		return false;
	const hh_u32	oldCount = m_UsedCount;
	m_UsedCount = count;
	for (hh_u32 i = m_UsedCount; i < oldCount; ++i)
		CReusePolicy::Release(m_Array[i]);
	for (hh_u32 i = oldCount; i < m_UsedCount; ++i)
		CReusePolicy::Aquire(m_Array[i]);
}

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
CGuid	TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::PushBack()
{
	if (!Reserve(m_UsedCount + 1))
		return CGuid::INVALID;
	const hh_u32	i = m_UsedCount;
	CReusePolicy::Aquire(m_Array[i]);
	++m_UsedCount;
	return i;
}

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
typename TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::ValueType	*TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::PushBackReserved()
{
	//HH_ASSERT_MESSAGE(m_Array.Capacity() >= m_UsedCount + 1, "Should have been reserved !");
	HH_ASSERT_MESSAGE(m_Array.Count() >= m_UsedCount + 1, "Should have been reserved !");
	const CGuid		id = PushBack();
	HH_ASSERT(id.Valid());
	return &(m_Array[id]);
}

//----------------------------------------------------------------------------

template<typename _ArrayType, template <typename> class _ReusePolicy>
void	TWrapArrayStackReuse<_ArrayType, _ReusePolicy>::PopBackAndDiscard()
{
	if (!HH_VERIFY(m_UsedCount > 0))
		return;
	CReusePolicy::Release(m_Array[m_UsedCount - 1]);
	--m_UsedCount;
	return;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ARRAY_WRAP_H__
