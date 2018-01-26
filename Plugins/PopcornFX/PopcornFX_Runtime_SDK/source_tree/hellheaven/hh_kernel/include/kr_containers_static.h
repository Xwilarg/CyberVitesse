#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/10/23 12:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_STATIC_H__
#define __KR_CONTAINERS_STATIC_H__

#include "hh_maths/include/hh_numeric_tools_int.h"
#include "hh_kernel/include/kr_memoryviews.h"
#include "hh_kernel/include/kr_containers_internals.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Pairs
//
//----------------------------------------------------------------------------

template<typename _Type0, typename _Type1>
class	TPair
{
private:
	typedef TPair<_Type0, _Type1>	SelfType;

	_Type0	m_First;
	_Type1	m_Second;

protected:
public:
	TPair() {}
	TPair(const _Type0 &first, const _Type1 &second) : m_First(first), m_Second(second) {}
//	TPair(const SelfType &copyFrom) : m_First(copyFrom.m_First), m_Second(copyFrom.m_Second) {}
	~TPair() {}

	const _Type0	&First() const { return m_First; }
	_Type0			&First() { return m_First; }
	const _Type1	&Second() const { return m_Second; }
	_Type1			&Second() { return m_Second; }

	const _Type0	&Key() const { return m_First; }
	_Type0			&Key() { return m_First; }
	const _Type1	&Value() const { return m_Second; }
	_Type1			&Value() { return m_Second; }

	bool	operator == (const SelfType &compareWith) const { return compareWith.m_First == m_First && compareWith.m_Second == m_Second; }
	bool	operator != (const SelfType &compareWith) const { return compareWith.m_First != m_First || compareWith.m_Second != m_Second; }
};

//----------------------------------------------------------------------------

namespace TTypeTraits
{
	template<typename _Type0, typename _Type1> struct	HasTrivialConstructor<TPair<_Type0, _Type1> >
	:	TTypeBoolConstant<HasTrivialConstructor<_Type0>::True &&
						  HasTrivialConstructor<_Type1>::True>
	{};

	template<typename _Type0, typename _Type1> struct	HasTrivialDestructor<TPair<_Type0, _Type1> >
	:	TTypeBoolConstant<HasTrivialDestructor<_Type0>::True &&
						  HasTrivialDestructor<_Type1>::True>
	{};
};

//----------------------------------------------------------------------------
//
//	Tuples
//
//----------------------------------------------------------------------------

template<typename _Type0, typename _Type1, typename _Type2>
class	TTuple
{
private:
	typedef TTuple<_Type0, _Type1, _Type2>	SelfType;

	_Type0	m_First;
	_Type1	m_Second;
	_Type2	m_Third;

protected:
public:
	TTuple() {}
	TTuple(const _Type0 &first, const _Type1 &second, const _Type2 &third) : m_First(first), m_Second(second), m_Third(third) {}
	~TTuple() {}

	const _Type0	&First() const { return m_First; }
	_Type0			&First() { return m_First; }
	const _Type1	&Second() const { return m_Second; }
	_Type1			&Second() { return m_Second; }
	const _Type2	&Third() const { return m_Third; }
	_Type2			&Third() { return m_Third; }

	const _Type0	&Key() const { return m_First; }
	_Type0			&Key() { return m_First; }
	const _Type1	&Value1() const { return m_Second; }
	_Type1			&Value1() { return m_Second; }
	const _Type2	&Value2() const { return m_Third; }
	_Type2			&Value2() { return m_Third; }

	bool	operator == (const SelfType &compareWith) const { return compareWith.m_First == m_First && compareWith.m_Second == m_Second && compareWith.m_Third == m_Third; }
	bool	operator != (const SelfType &compareWith) const { return compareWith.m_First != m_First || compareWith.m_Second != m_Second || compareWith.m_Third != m_Third; }
};

//----------------------------------------------------------------------------

namespace TTypeTraits
{
	template<typename _Type0, typename _Type1, typename _Type2> struct	HasTrivialConstructor<TTuple<_Type0, _Type1, _Type2> >
	:	TTypeBoolConstant<HasTrivialConstructor<_Type0>::True &&
						  HasTrivialConstructor<_Type1>::True &&
						  HasTrivialConstructor<_Type2>::True>
	{};

	template<typename _Type0, typename _Type1, typename _Type2> struct	HasTrivialDestructor<TTuple<_Type0, _Type1, _Type2> >
	:	TTypeBoolConstant<HasTrivialDestructor<_Type0>::True &&
						  HasTrivialDestructor<_Type1>::True &&
						  HasTrivialDestructor<_Type2>::True>
	{};
};

//----------------------------------------------------------------------------
//
//	Static arrays
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Count>
class	TStaticArray
{
private:
	typedef TStaticArray<_Type, _Count>				SelfType;
//	typedef typename TType_BuildConst<_Type>::Type	_TypeConst;
	typedef _Type const								_TypeConst;

	// Alignment to fix misaligned pointer access undefined behavior possibly inside _Type
	// (keep it enum for visual studio natvis)
	enum { kAlignment = HH_ALIGNOF(_Type) };
	static const hh_ureg		_kAlignment = kAlignment; // (and static const for gdb)

	hh_u8	_m_Data[_Count * sizeof(_Type) + kAlignment - 1];

	HH_FORCEINLINE _TypeConst	*_Data() const { return reinterpret_cast<_TypeConst*>(Mem::Align<kAlignment>(_m_Data)); }
	HH_FORCEINLINE _Type		*_Data() { return reinterpret_cast<_Type*>(Mem::Align<kAlignment>(_m_Data)); }

public:
	TStaticArray()
	{
		Internal::TRangeConstructor<_Type>::ConstructDefault(_Data(), 0, _Count);
	}
	TStaticArray(_TypeConst &value)
	{
		for (hh_u32 i = 0; i < _Count; i++)
			Mem::Construct(_Data()[i], value);
	}
	TStaticArray(const SelfType &copyFrom)
	{
		// !! Mem Aligment !!
		Internal::TRangeCopier<_Type, _TypeConst>::Copy(_Data(), TMemoryView<_TypeConst>(copyFrom), 0, _Count);
	}
	~TStaticArray()
	{
		Internal::TRangeDestructor<_Type>::Destroy(_Data(), 0, _Count);
	}

	const SelfType			&operator = (const SelfType &copyFrom)
	{
		Internal::TRangeDestructor<_Type>::Destroy(_Data(), 0, _Count);
		// !! Mem Aligment !!
		Internal::TRangeCopier<_Type, _TypeConst>::Copy(_Data(), TMemoryView<_TypeConst>(copyFrom), 0, _Count);
		return *this;
	}

	// conversions to memory views
	HH_FORCEINLINE operator	TMemoryView<_Type> () { return TMemoryView<_Type>(_Data(), _Count); }
	HH_FORCEINLINE operator	TMemoryView<_TypeConst> () const { return TMemoryView<_TypeConst>(_Data(), _Count); }
	HH_FORCEINLINE operator	TStridedMemoryView<_Type> () { return TStridedMemoryView<_Type>(_Data(), _Count, sizeof(_Type)); }
	HH_FORCEINLINE operator	TStridedMemoryView<_TypeConst> () const { return TStridedMemoryView<_TypeConst>(_Data(), _Count, sizeof(_Type)); }

	template<typename _Type2>
	HH_INLINE operator TMemoryView<_Type2> () { return TMemoryView<_Type2>(_Data(), _Count); }
	template<typename _Type2>
	HH_INLINE operator TMemoryView<_Type2 const> () const { return TMemoryView<_Type2 const>(_Data(), _Count); }

	HH_FORCEINLINE _TypeConst	&operator [] (hh_u32 index) const { HH_ASSERT(index < _Count); return _Data()[index]; }
	HH_FORCEINLINE _Type		&operator [] (hh_u32 index) { HH_ASSERT(index < _Count); return _Data()[index]; }

	HH_FORCEINLINE _TypeConst	&First() const { return _Data()[0]; }
	HH_FORCEINLINE _Type		&First() { return _Data()[0]; }
	HH_FORCEINLINE _TypeConst	&Last() const { return _Data()[_Count - 1]; }
	HH_FORCEINLINE _Type		&Last() { return _Data()[_Count - 1]; }

	HH_FORCEINLINE _TypeConst	*Begin() const { return _Data(); }
	HH_FORCEINLINE _Type		*Begin() { return _Data(); }
	HH_FORCEINLINE _TypeConst	*End() const { return _Data() + _Count; }
	HH_FORCEINLINE _Type		*End() { return _Data() + _Count; }

	HH_FORCEINLINE _TypeConst	*Data() const { return _Data(); }
	HH_FORCEINLINE _Type		*Data() { return _Data(); }

	HH_FORCEINLINE hh_u32		Count() const { return _Count; }
	HH_FORCEINLINE hh_u32		CoveredBytes() const { return _Count * sizeof(_Type); }
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _MaxCount>
class	TStaticCountedArray
{
private:
	typedef TStaticCountedArray<_Type, _MaxCount>		SelfType;
	typedef _Type const		_TypeConst;

	// Alignment to fix misaligned pointer access undefined behavior possibly inside _Type
	// (keep it enum for visual studio natvis)
	enum { kAlignment = HH_ALIGNOF(_Type) };
	static const hh_ureg		_kAlignment = kAlignment; // (and static const for gdb)

	hh_u32	m_Count;

	hh_u8	_m_Data[_MaxCount * sizeof(_Type) + kAlignment - 1];

	HH_FORCEINLINE _TypeConst	*_Data() const { return reinterpret_cast<_TypeConst*>(Mem::Align<kAlignment>(_m_Data)); }
	HH_FORCEINLINE _Type		*_Data() { return reinterpret_cast<_Type*>(Mem::Align<kAlignment>(_m_Data)); }

public:
	HH_FORCEINLINE TStaticCountedArray() : m_Count(0) {}
	TStaticCountedArray(const _Type &v0)
	{
		HH_STATIC_ASSERT(_MaxCount >= 1);
		m_Count = 1;
		Mem::Construct(_Data()[0], v0);
	}
	TStaticCountedArray(const _Type &v0, const _Type &v1)
	{
		HH_STATIC_ASSERT(_MaxCount >= 2);
		m_Count = 2;
		Mem::Construct(_Data()[0], v0);
		Mem::Construct(_Data()[1], v1);
	}
	TStaticCountedArray(const _Type &v0, const _Type &v1, const _Type &v2)
	{
		HH_STATIC_ASSERT(_MaxCount >= 3);
		m_Count = 3;
		Mem::Construct(_Data()[0], v0);
		Mem::Construct(_Data()[1], v1);
		Mem::Construct(_Data()[2], v2);
	}
	TStaticCountedArray(const _Type &v0, const _Type &v1, const _Type &v2, const _Type &v3)
	{
		HH_STATIC_ASSERT(_MaxCount >= 4);
		m_Count = 4;
		Mem::Construct(_Data()[0], v0);
		Mem::Construct(_Data()[1], v1);
		Mem::Construct(_Data()[2], v2);
		Mem::Construct(_Data()[3], v3);
	}
	TStaticCountedArray(const TMemoryView<const _Type> &values)
	{
		// !! Mem Aligment !!
		HH_ASSERT(values.Count() <= _MaxCount);
		m_Count = HHMin(values.Count(), _MaxCount);
		Internal::TRangeCopier<_Type, _TypeConst>::Copy(_Data(), values, 0, m_Count);
	}

	TStaticCountedArray(const SelfType &copyFrom)
	{
		// !! Mem Aligment !!
		m_Count = copyFrom.m_Count;
		Internal::TRangeCopier<_Type, _TypeConst>::Copy(_Data(), TMemoryView<_TypeConst>(copyFrom), 0, m_Count);
	}

	~TStaticCountedArray()
	{
		Internal::TRangeDestructor<_Type>::Destroy(_Data(), 0, m_Count);
	}

	const SelfType		&operator = (const SelfType &copyFrom)
	{
		Internal::TRangeDestructor<_Type>::Destroy(_Data(), 0, m_Count);
		// !! Mem Aligment !!
		m_Count = copyFrom.m_Count;
		Internal::TRangeCopier<_Type, _TypeConst>::Copy(_Data(), TMemoryView<_TypeConst>(copyFrom), 0, m_Count);
		return *this;
	}

	// conversions to memory views
	HH_FORCEINLINE operator	TMemoryView<_Type> () { return TMemoryView<_Type>(_Data(), m_Count); }
	HH_FORCEINLINE operator	TMemoryView<_Type const> () const { return TMemoryView<_Type const>(_Data(), m_Count); }
	HH_FORCEINLINE operator	TStridedMemoryView<_Type> () { return TStridedMemoryView<_Type>(_Data(), m_Count, sizeof(_Type)); }
	HH_FORCEINLINE operator	TStridedMemoryView<_Type const> () const { return TStridedMemoryView<_Type const>(_Data(), m_Count, sizeof(_Type)); }

	HH_FORCEINLINE TMemoryView<_Type>		View() { return TMemoryView<_Type>(_Data(), m_Count); }
	HH_FORCEINLINE TMemoryView<_Type const>	View() const { return TMemoryView<_Type const>(_Data(), m_Count); }

	HH_FORCEINLINE hh_u32		Capacity() const { return _MaxCount; }
	HH_FORCEINLINE void			Clear() { Resize(0); }
	HH_FORCEINLINE void			Resize(hh_u32 count) { HH_ASSERT(count <= _MaxCount); Internal::TRangeResizer<_Type>::Resize(_Data(), m_Count, count); m_Count = count; }
	HH_FORCEINLINE hh_u32		Count() const { return m_Count; }
	HH_FORCEINLINE bool			Empty() const { return m_Count == 0; }
	HH_FORCEINLINE bool			Full() const { return m_Count == _MaxCount; }
	HH_FORCEINLINE hh_u32		CoveredBytes() const { return Count() * sizeof(_Type); }

	HH_FORCEINLINE CGuid		Push(const _Type &element) { HH_ASSERT(m_Count < _MaxCount); hh_u32 insertionIndex = m_Count++; Mem::Construct(_Data()[insertionIndex], element); return insertionIndex; }
	HH_FORCEINLINE CGuid		PushBack(const _Type &element) { return Push(element); }
	HH_FORCEINLINE CGuid		Push() { HH_ASSERT(m_Count < _MaxCount); hh_u32 insertionIndex = m_Count++; Mem::Construct(_Data()[insertionIndex]); return insertionIndex; }
	HH_FORCEINLINE CGuid		PushBack() { return Push(); }
	HH_FORCEINLINE _Type		Pop() { HH_ASSERT(m_Count > 0); _Type v = _Data()[--m_Count]; Mem::Destruct(_Data()[m_Count]); return v; }
	HH_FORCEINLINE void			PopAndDiscard() { HH_ASSERT(m_Count > 0); Mem::Destruct(_Data()[--m_Count]); }
	HH_FORCEINLINE _Type		PopBack() { return Pop(); }
	HH_FORCEINLINE void			PopBackAndDiscard() { PopAndDiscard(); }
	HH_FORCEINLINE void			Remove(hh_u32 id) { HH_ASSERT(id < m_Count); m_Count--; if (id != m_Count) HHSwap(_Data()[id], _Data()[m_Count]); Mem::Destruct(_Data()[m_Count]); }
	HH_FORCEINLINE _TypeConst	&operator [] (hh_u32 pos) const { HH_ASSERT(pos < m_Count); return _Data()[pos]; }
	HH_FORCEINLINE _Type		&operator [] (hh_u32 pos) { HH_ASSERT(pos < m_Count); return _Data()[pos]; }
	HH_FORCEINLINE _TypeConst	&First() const { HH_ASSERT(m_Count > 0); return _Data()[0]; }
	HH_FORCEINLINE _Type		&First() { HH_ASSERT(m_Count > 0); return _Data()[0]; }
	HH_FORCEINLINE _TypeConst	&Last() const { HH_ASSERT(m_Count > 0); return _Data()[m_Count - 1]; }
	HH_FORCEINLINE _Type		&Last() { HH_ASSERT(m_Count > 0); return _Data()[m_Count - 1]; }

	HH_FORCEINLINE _TypeConst	*Begin() const { return _Data(); }
	HH_FORCEINLINE _Type		*Begin() { return _Data(); }
	HH_FORCEINLINE _TypeConst	*End() const { return _Data() + m_Count; }
	HH_FORCEINLINE _Type		*End() { return _Data() + m_Count; }

	HH_FORCEINLINE _TypeConst	*RawDataPointer() const { return _Data(); }
	HH_FORCEINLINE _Type		*RawDataPointer() { return _Data(); }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_STATIC_H__
