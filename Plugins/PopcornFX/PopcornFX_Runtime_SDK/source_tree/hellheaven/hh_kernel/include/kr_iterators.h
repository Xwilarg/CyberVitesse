#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2010/04/10 10:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_ITERATORS_H__
#define __KR_ITERATORS_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Iterators
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TContiguousBaseIterator
{
protected:
	template<typename _Type2> friend class	TContiguousBaseIterator;

	typedef	typename TType_RemoveConstness<_Type>::Type	_UnconstType;
	typedef	const _UnconstType							_ConstType;

	typedef TContiguousBaseIterator<_Type>				SelfType;
	typedef TContiguousBaseIterator<_UnconstType>		SelfUnconstType;
	typedef TContiguousBaseIterator<_ConstType>			SelfConstType;

	_Type	*m_Ptr;

public:
	HH_FORCEINLINE TContiguousBaseIterator() : m_Ptr(null) {}
	template<typename _Type2>
	HH_FORCEINLINE TContiguousBaseIterator(const TContiguousBaseIterator<_Type2> &other) : m_Ptr(other.m_Ptr) {}
	HH_FORCEINLINE explicit TContiguousBaseIterator(_Type *ptr) : m_Ptr(ptr) {}

	HH_FORCEINLINE SelfType	&operator ++ () { m_Ptr++; return *this; }
	HH_FORCEINLINE SelfType	&operator -- () { m_Ptr--; return *this; }
	HH_FORCEINLINE SelfType	operator ++ (int) { SelfType oldValue = *this; m_Ptr++; return oldValue; }
	HH_FORCEINLINE SelfType	operator -- (int) { SelfType oldValue = *this; m_Ptr--; return oldValue; }
	HH_FORCEINLINE SelfType	operator + (int count) const { return SelfType(m_Ptr + count); }
	HH_FORCEINLINE SelfType	operator - (int count) const { return SelfType(m_Ptr - count); }
	HH_FORCEINLINE SelfType	&operator += (int count) { m_Ptr += count; return *this; }
	HH_FORCEINLINE SelfType	&operator -= (int count) { m_Ptr -= count; return *this; }
	HH_FORCEINLINE int		operator - (SelfType &other) const { return m_Ptr - other.m_Ptr; }

	HH_FORCEINLINE _Type	&operator * () const { return *m_Ptr; }
	HH_FORCEINLINE _Type	&operator [] (int index) const { return m_Ptr[index]; }
	HH_FORCEINLINE _Type	*operator -> () const { return m_Ptr; }

	HH_FORCEINLINE bool		operator == (const SelfUnconstType &other) const { return m_Ptr == other.m_Ptr; }
	HH_FORCEINLINE bool		operator != (const SelfUnconstType &other) const { return m_Ptr != other.m_Ptr; }
	HH_FORCEINLINE bool		operator > (const SelfUnconstType &other) const { return m_Ptr > other.m_Ptr; }
	HH_FORCEINLINE bool		operator < (const SelfUnconstType &other) const { return m_Ptr < other.m_Ptr; }
	HH_FORCEINLINE bool		operator >= (const SelfUnconstType &other) const { return m_Ptr >= other.m_Ptr; }
	HH_FORCEINLINE bool		operator <= (const SelfUnconstType &other) const { return m_Ptr <= other.m_Ptr; }

	HH_FORCEINLINE bool		operator == (const SelfConstType &other) const { return m_Ptr == other.m_Ptr; }
	HH_FORCEINLINE bool		operator != (const SelfConstType &other) const { return m_Ptr != other.m_Ptr; }
	HH_FORCEINLINE bool		operator > (const SelfConstType &other) const { return m_Ptr > other.m_Ptr; }
	HH_FORCEINLINE bool		operator < (const SelfConstType &other) const { return m_Ptr < other.m_Ptr; }
	HH_FORCEINLINE bool		operator >= (const SelfConstType &other) const { return m_Ptr >= other.m_Ptr; }
	HH_FORCEINLINE bool		operator <= (const SelfConstType &other) const { return m_Ptr <= other.m_Ptr; }
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TStridedBaseIterator
{
protected:
	template<typename _Type2> friend class	TStridedBaseIterator;

	typedef	typename TType_RemoveConstness<_Type>::Type	_UnconstType;
	typedef	const _UnconstType							_ConstType;

	typedef TStridedBaseIterator<_Type>					SelfType;
	typedef TStridedBaseIterator<_UnconstType>			SelfUnconstType;
	typedef TStridedBaseIterator<_ConstType>			SelfConstType;

	typedef	typename TType_TransferConstness<_Type, hh_u8>::Type*	_RawDataPtr;

	_Type	*m_Ptr;
	hh_u32	m_StrideInBytes;

public:
	HH_FORCEINLINE TStridedBaseIterator() : m_Ptr(null), m_StrideInBytes(sizeof(_Type)) {}
	template<typename _Type2>
	HH_FORCEINLINE TStridedBaseIterator(const TStridedBaseIterator<_Type2> &other) : m_Ptr(other.m_Ptr), m_StrideInBytes(other.m_StrideInBytes) {}
	HH_FORCEINLINE TStridedBaseIterator(_Type *ptr, hh_u32 stride) : m_Ptr(ptr), m_StrideInBytes(stride) { /*HH_ASSERT(m_StrideInBytes > 0);*/ }

	bool					HasInfiniteIterationCycle() const { return m_StrideInBytes == 0; }
	bool					Safe() const { return !HasInfiniteIterationCycle(); }

	HH_FORCEINLINE SelfType	&operator ++ () { m_Ptr = reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) + m_StrideInBytes); return *this; }
	HH_FORCEINLINE SelfType	&operator -- () { m_Ptr = reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) - m_StrideInBytes); return *this; }
	HH_FORCEINLINE SelfType	operator ++ (int) { SelfType oldValue = *this; m_Ptr = reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) + m_StrideInBytes); return oldValue; }
	HH_FORCEINLINE SelfType	operator -- (int) { SelfType oldValue = *this; m_Ptr = reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) - m_StrideInBytes); return oldValue; }
	HH_FORCEINLINE SelfType	operator + (int count) const { return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) + count * m_StrideInBytes), sizeof(_Type)); }
	HH_FORCEINLINE SelfType	operator - (int count) const { return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) - count * m_StrideInBytes), sizeof(_Type)); }
	HH_FORCEINLINE SelfType	&operator += (int count) { m_Ptr = reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) + count * m_StrideInBytes); return *this; }
	HH_FORCEINLINE SelfType	&operator -= (int count) { m_Ptr = reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) - count * m_StrideInBytes); return *this; }
	HH_FORCEINLINE int		operator - (const SelfType &other) const { HH_ASSERT(m_StrideInBytes > 0); hh_ireg byteOffset = reinterpret_cast<_RawDataPtr>(m_Ptr) - reinterpret_cast<_RawDataPtr>(other.m_Ptr); hh_ireg offset = byteOffset / m_StrideInBytes; HH_ASSERT(offset * (hh_i32)m_StrideInBytes == byteOffset); return offset; }

	HH_FORCEINLINE _Type	&operator * () const { return *m_Ptr; }
	HH_FORCEINLINE _Type	&operator [] (int index) const { return *reinterpret_cast<_Type*>(reinterpret_cast<_RawDataPtr>(m_Ptr) + index * m_StrideInBytes); }
	HH_FORCEINLINE _Type	*operator -> () const { return m_Ptr; }

	HH_FORCEINLINE bool		operator == (const SelfUnconstType &other) const { return m_Ptr == other.m_Ptr; }
	HH_FORCEINLINE bool		operator != (const SelfUnconstType &other) const { return m_Ptr != other.m_Ptr; }
	HH_FORCEINLINE bool		operator > (const SelfUnconstType &other) const { return m_Ptr > other.m_Ptr; }
	HH_FORCEINLINE bool		operator < (const SelfUnconstType &other) const { return m_Ptr < other.m_Ptr; }
	HH_FORCEINLINE bool		operator >= (const SelfUnconstType &other) const { return m_Ptr >= other.m_Ptr; }
	HH_FORCEINLINE bool		operator <= (const SelfUnconstType &other) const { return m_Ptr <= other.m_Ptr; }

	HH_FORCEINLINE bool		operator == (const SelfConstType &other) const { return m_Ptr == other.m_Ptr; }
	HH_FORCEINLINE bool		operator != (const SelfConstType &other) const { return m_Ptr != other.m_Ptr; }
	HH_FORCEINLINE bool		operator > (const SelfConstType &other) const { return m_Ptr > other.m_Ptr; }
	HH_FORCEINLINE bool		operator < (const SelfConstType &other) const { return m_Ptr < other.m_Ptr; }
	HH_FORCEINLINE bool		operator >= (const SelfConstType &other) const { return m_Ptr >= other.m_Ptr; }
	HH_FORCEINLINE bool		operator <= (const SelfConstType &other) const { return m_Ptr <= other.m_Ptr; }
};

//----------------------------------------------------------------------------

#define	HH_FOREACH_IT(__it, __begin, __end)		for (PK_DECLTYPE(__begin) __it = __begin, _itEnd = __end; __it != _itEnd; ++__it)
#define	HH_RFOREACH_IT(__it, __begin, __end)	for (PK_DECLTYPE(__begin) _itBegin = __end, _itEnd = __begin, __it = _itBegin; __it-- != _itEnd; )
#define	HH_FOREACH(__it, __container)			HH_FOREACH_IT(__it, __container.Begin(), __container.End())
#define	HH_RFOREACH(__it, __container)			HH_RFOREACH_IT(__it, __container.Begin(), __container.End())

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_ITERATORS_H__