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

#ifndef __KR_CONTAINERS_ARRAY_H__
#define __KR_CONTAINERS_ARRAY_H__

#include "hh_kernel/include/kr_memoryviews.h"
#include "hh_kernel/include/kr_containers_internals.h"
#include "hh_kernel/include/kr_containers_static.h"
#include "hh_kernel/include/kr_mem.h"
#include "hh_kernel/include/kr_refptr.h"
#include "hh_kernel/include/hh_kernel_config.h"

// perfs
#define	HH_ITERATORS_USE_POINTERS_FOR_CONTIGUOUS_IT
//#define	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM	// smaller sizeof(TArray<...>), but a bit slower. not sure it still works if disabled

__PK_API_BEGIN
//----------------------------------------------------------------------------

extern HH_KERNEL_EXPORT bool	g_KRContainersConfig_MonitorTArrays;
extern HH_KERNEL_EXPORT bool	g_KRContainersConfig_MonitorTHashMaps;
extern HH_KERNEL_EXPORT bool	g_KRContainersConfig_MonitorTFastHashMaps;

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CContainersMonitorTArrays
{
private:
	static hh_u64		m_CopyConstructs;
	static hh_u64		m_CopyConstructBytes;
	static hh_u64		m_CopyAssigns;
	static hh_u64		m_CopyAssignBytes;
	static hh_u64		m_AllocatedBytes;
	static hh_u64		m_ReleasedBytes;

public:
	static void	Reset();

	static void	RegisterNewCopyConstructor(hh_u32 rawCopiedDataSize, hh_u32 dataStride);
	static void	RegisterNewCopyAssignment(hh_u32 rawOriginalDataSize, hh_u32 rawCopiedDataSize, hh_u32 dataStride);
	static void	RegisterNewMemoryMovement(hh_ireg memoryOffset);

	// accessors:

	hh_u64	CopyConstructs() const { return m_CopyConstructs; }
	hh_u64	CopyConstructBytes() const { return m_CopyConstructBytes; }
	hh_u64	CopyAssigns() const { return m_CopyAssigns; }
	hh_u64	CopyAssignBytes() const { return m_CopyAssignBytes; }
	hh_u64	AllocatedBytes() const { return m_AllocatedBytes; }
	hh_u64	ReleasedBytes() const { return m_ReleasedBytes; }
};

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(String);
HH_FORWARD_DECLARE(FastString);

//----------------------------------------------------------------------------
//
//	TArrays
//
//----------------------------------------------------------------------------

template<	hh_u32	_Alignment = 0,
			hh_i32	_InitialSize = 8,
			hh_i32	_GrowConstant = 8,
			hh_i32	_GrowMultiplier = 0,
			hh_i32	_GrowDivider = 2>
class	TArrayStaticController
{
public:
	HH_INLINE static hh_u32	GrowSize(hh_u32 oldSize)
	{
		if (oldSize == 0)
			return _InitialSize;
		return oldSize + _GrowConstant + (oldSize * _GrowMultiplier) + (oldSize / _GrowDivider);
	}
	HH_FORCEINLINE static hh_u32	Alignment() { return _Alignment; }
};

//----------------------------------------------------------------------------

typedef TArrayStaticController<16>						TArrayAligned16;
typedef TArrayStaticController<64>						TArrayAligned64;
typedef TArrayStaticController<128>						TArrayAligned128;
typedef TArrayStaticController<Memory::CacheLineSize>	TArrayCacheAligned;

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller, typename _HasTrivialRelocate = TTypeTraits::HasTrivialRelocate<_Type> >
class	TArray_BaseContainerImpl : public _Controller
{
public:
	typedef	TArray_BaseContainerImpl<_Type, _Controller, _HasTrivialRelocate>	SelfType;
	typedef	_HasTrivialRelocate													HasTrivialRelocate;

private:
	_Type						*m_Data;
	hh_u32						m_Count;
#ifndef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
	hh_u32						m_MaxCount;
#endif

protected:
	HH_FORCEINLINE _Type		*_Data() { return m_Data; }
	HH_FORCEINLINE const _Type	*_Data() const { return m_Data; }
	HH_FORCEINLINE hh_u32		_Count() const { return m_Count; }
	HH_FORCEINLINE void			_SetCount(hh_u32 count) { m_Count = count; }

#ifndef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
	HH_FORCEINLINE hh_u32		_MaxCount() const { return m_MaxCount; }
	HH_FORCEINLINE void			_SetMaxCount(hh_u32 newSize) { m_MaxCount = newSize; }
#else
	HH_FORCEINLINE hh_u32		_MaxCount() const { /*if (m_Data == null) return 0;*/ hh_u32 maxCount = Mem::Size(m_Data); HH_PARANOID_ASSERT(maxCount % sizeof(_Type) == 0); return maxCount / sizeof(_Type); }
	HH_FORCEINLINE void			_SetMaxCount(hh_u32 newSize) {}
#endif

	bool						_ReallocBuffer(hh_u32 newSize);
	bool						_Grow(hh_u32 wishedSize);
	bool						_RawResize(hh_u32 newSize);	// all the elements that will be removed should have been destroyed beforehand
	void						_Clean();	// frees the buffer

public:
	TArray_BaseContainerImpl();
	~TArray_BaseContainerImpl();

	void						Swap(SelfType &swapWith);
};

//----------------------------------------------------------------------------

template<typename _Type, typename _BaseImpl>
class	TArray_Base : public _BaseImpl
{
public:
	typedef TArray_Base<_Type, _BaseImpl>			SelfType;
	typedef _Type									ValueType;
//	typedef typename TType_BuildConst<_Type>::Type	ValueTypeConst;
	typedef _Type const								ValueTypeConst;
	typedef _BaseImpl								BaseImpl;

	template<typename _Type2, typename _BaseImpl2>
	friend class	TArray_Base;

#ifdef	HH_ITERATORS_USE_POINTERS_FOR_CONTIGUOUS_IT
	typedef	ValueType								*UncheckedIterator;
	typedef	ValueType const							*UncheckedConstIterator;
#else
	typedef	TContiguousBaseIterator<ValueType>		UncheckedIterator;
	typedef	TContiguousBaseIterator<ValueTypeConst>	UncheckedConstIterator;
#endif

private:
	typedef Internal::SRelocationHelper<_Type, BaseImpl::HasTrivialRelocate::True>	Relocator;

#if	(HH_ITERATORS_DEBUG_CHECK != 0)

private:
	template<typename _Type1, typename _ContainerType>
	class	TDebugContiguousBaseIterator : public TContiguousBaseIterator<_Type1>
	{
	private:
		typedef	typename TType_RemoveConstness<_Type1>::Type			_UnconstType1;
		typedef	const _UnconstType1										_ConstType1;

		typedef TDebugContiguousBaseIterator<_Type1, _ContainerType>		SelfType;
		typedef TDebugContiguousBaseIterator<_UnconstType1, _ContainerType>	SelfUnconstType;
		typedef TDebugContiguousBaseIterator<_ConstType1, _ContainerType>	SelfConstType;
		typedef TContiguousBaseIterator<_Type1>								RealType;

		const _ContainerType	*m_Owner;

		void	_CheckBase() const { HH_ASSERT(m_Owner != null && RealType::m_Ptr >= m_Owner->_RawBegin() && RealType::m_Ptr <= m_Owner->_RawEnd()); }
		void	_Check() const { _CheckBase(); HH_ASSERT(RealType::m_Ptr != null); }
		void	_CheckDeref() const { _Check(); HH_ASSERT_MESSAGE(RealType::m_Ptr != m_Owner->_RawEnd(), "invalid operation: trying to dereference Iterator::End()"); }
	public:
		HH_FORCEINLINE TDebugContiguousBaseIterator() {}
		template<typename _Type2>
		HH_FORCEINLINE TDebugContiguousBaseIterator(const TDebugContiguousBaseIterator<_Type2, _ContainerType> &other) : RealType(other), m_Owner(other._INTERNAL_Owner()) {}
		HH_FORCEINLINE TDebugContiguousBaseIterator(_Type1 *ptr, const _ContainerType *owner) : RealType(ptr), m_Owner(owner) { _CheckBase(); }
		const _ContainerType	*_INTERNAL_Owner() const { return m_Owner; }	// thanks GCC

		HH_FORCEINLINE SelfType	&operator ++ () { RealType::operator ++ (); _Check(); return *this; }
		HH_FORCEINLINE SelfType	&operator -- () { RealType::operator -- (); _Check(); return *this; }
		HH_FORCEINLINE SelfType	operator ++ (int) { _Check(); SelfType oldValue = *this; RealType::m_Ptr++; _Check(); return oldValue; }
		HH_FORCEINLINE SelfType	operator -- (int) { _Check(); SelfType oldValue = *this; RealType::m_Ptr--; _Check(); return oldValue; }
		HH_FORCEINLINE SelfType	operator + (int count) const { return SelfType(RealType::m_Ptr + count, m_Owner); }
		HH_FORCEINLINE SelfType	operator - (int count) const { return SelfType(RealType::m_Ptr - count, m_Owner); }
		HH_FORCEINLINE SelfType	&operator += (int count) { RealType::operator += (count); _Check(); return *this; }
		HH_FORCEINLINE SelfType	&operator -= (int count) { RealType::operator -= (count); _Check(); return *this; }
		HH_FORCEINLINE int		operator - (SelfType &other) const { _CheckBase(); other._CheckBase(); HH_ASSERT_MESSAGE(m_Owner == other.m_Owner, "unrelated iterators."); return RealType::operator - (other); }

		HH_FORCEINLINE _Type1	&operator * () const { _CheckDeref(); return RealType::operator * (); }
		HH_FORCEINLINE _Type1	&operator [] (int index) const { _CheckDeref(); return RealType::operator [] (index); }
		HH_FORCEINLINE _Type1	*operator -> () const { _CheckDeref(); return RealType::operator -> (); }

		HH_FORCEINLINE bool		operator == (const SelfUnconstType &other) const { _CheckBase(); return RealType::operator == (other); }
		HH_FORCEINLINE bool		operator != (const SelfUnconstType &other) const { _CheckBase(); return RealType::operator != (other); }
		HH_FORCEINLINE bool		operator > (const SelfUnconstType &other) const { _CheckBase(); return RealType::operator > (other); }
		HH_FORCEINLINE bool		operator < (const SelfUnconstType &other) const { _CheckBase(); return RealType::operator < (other); }
		HH_FORCEINLINE bool		operator >= (const SelfUnconstType &other) const { _CheckBase(); return RealType::operator >= (other); }
		HH_FORCEINLINE bool		operator <= (const SelfUnconstType &other) const { _CheckBase(); return RealType::operator <= (other); }

		HH_FORCEINLINE bool		operator == (const SelfConstType &other) const { _CheckBase(); return RealType::operator == (other); }
		HH_FORCEINLINE bool		operator != (const SelfConstType &other) const { _CheckBase(); return RealType::operator != (other); }
		HH_FORCEINLINE bool		operator > (const SelfConstType &other) const { _CheckBase(); return RealType::operator > (other); }
		HH_FORCEINLINE bool		operator < (const SelfConstType &other) const { _CheckBase(); return RealType::operator < (other); }
		HH_FORCEINLINE bool		operator >= (const SelfConstType &other) const { _CheckBase(); return RealType::operator >= (other); }
		HH_FORCEINLINE bool		operator <= (const SelfConstType &other) const { _CheckBase(); return RealType::operator <= (other); }
	};

	// we need these for fast access to _RawBegin() and _RawEnd(), to avoid constructing temporary Iterators inside the checking code
	friend class	TDebugContiguousBaseIterator<ValueType, SelfType>;
	friend class	TDebugContiguousBaseIterator<ValueTypeConst, SelfType>;

public:
	typedef	TDebugContiguousBaseIterator<ValueType, SelfType>		Iterator;
	typedef	TDebugContiguousBaseIterator<ValueTypeConst, SelfType>	ConstIterator;

#else

public:
	typedef	UncheckedIterator			Iterator;
	typedef	UncheckedConstIterator		ConstIterator;
#endif

private:
	bool								_GrowIFN();
	bool								_GrowIFN(hh_u32 count);
	void								_DestroyRange(hh_u32 start, hh_u32 end);

	HH_FORCEINLINE ValueType			*_RawBegin() { return BaseImpl::_Data(); }
	HH_FORCEINLINE ValueType			*_RawEnd() { return BaseImpl::_Data() + BaseImpl::_Count(); }
	HH_FORCEINLINE ValueType const		*_RawBegin() const { return BaseImpl::_Data(); }
	HH_FORCEINLINE ValueType const		*_RawEnd() const { return BaseImpl::_Data() + BaseImpl::_Count(); }

protected:
	TArray_Base() {}
	explicit TArray_Base(hh_u32 size);
	TArray_Base(const SelfType &copyFrom);
	TArray_Base(const TMemoryView<const _Type> &copyFrom);
	TArray_Base(const TStridedMemoryViewBase<const _Type> &copyFrom);

	template<typename _Type2, template<typename> class _MemView>
	void								_Copy(const _MemView<_Type2> &copyFrom);

public:
	~TArray_Base();

	// conversions to memory views
	operator TMemoryView<ValueTypeConst> () const;
	operator TMemoryView<ValueType> ();

	operator TStridedMemoryView<ValueTypeConst> () const;
	operator TStridedMemoryView<ValueType> ();

	void	operator = (const SelfType &copyFrom);

	template<typename _OtherImpl>
	void	operator = (const TArray_Base<_Type, _OtherImpl> &copyFrom);

	template<typename _OtherImpl>
	bool	operator == (const TArray_Base<_Type, _OtherImpl> &testWith) const;

	template<typename _OtherImpl>
	bool	operator != (const TArray_Base<_Type, _OtherImpl> &testWith) const;

#if	(HH_ITERATORS_DEBUG_CHECK == 0)
	HH_FORCEINLINE Iterator			Begin() { return _RawBegin(); }
	HH_FORCEINLINE Iterator			End() { return _RawEnd(); }
	HH_FORCEINLINE ConstIterator	Begin() const { return _RawBegin(); }
	HH_FORCEINLINE ConstIterator	End() const { return _RawEnd(); }
#else
	HH_FORCEINLINE Iterator			Begin() { return Iterator(_RawBegin(), this); }
	HH_FORCEINLINE Iterator			End() { return Iterator(_RawEnd(), this); }
	HH_FORCEINLINE ConstIterator	Begin() const { return ConstIterator(_RawBegin(), this); }
	HH_FORCEINLINE ConstIterator	End() const { return ConstIterator(_RawEnd(), this); }
#endif

	HH_FORCEINLINE const _Type		&operator [] (hh_u32 pos) const { HH_ASSERT(pos < BaseImpl::_Count()); return BaseImpl::_Data()[pos]; }
	HH_FORCEINLINE _Type			&operator [] (hh_u32 pos) { HH_ASSERT(pos < BaseImpl::_Count()); return BaseImpl::_Data()[pos]; }

	HH_FORCEINLINE const _Type		&First() const { HH_ASSERT(BaseImpl::_Count() > 0); return BaseImpl::_Data()[0]; }
	HH_FORCEINLINE _Type			&First() { HH_ASSERT(BaseImpl::_Count() > 0); return BaseImpl::_Data()[0]; }
	HH_FORCEINLINE const _Type		&Last() const { HH_ASSERT(BaseImpl::_Count() > 0); return BaseImpl::_Data()[BaseImpl::_Count() - 1]; }
	HH_FORCEINLINE _Type			&Last() { HH_ASSERT(BaseImpl::_Count() > 0); return BaseImpl::_Data()[BaseImpl::_Count() - 1]; }

	TMemoryView<ValueTypeConst>		View() const { return TMemoryView<ValueTypeConst>(BaseImpl::_Data(), BaseImpl::_Count()); }
	TMemoryView<_Type>				ViewForWriting() { return TMemoryView<_Type>(BaseImpl::_Data(), BaseImpl::_Count()); }

	HH_FORCEINLINE ValueTypeConst	*RawDataPointer() const { return BaseImpl::_Data(); }
	HH_FORCEINLINE _Type			*RawDataPointer() { return BaseImpl::_Data(); }

	bool							Reserve(hh_u32 totalCapacity);
	bool							GrowedReserve(hh_u32 totalCapacity); // Reserve(_Controller::GrowSize(totalCapacity));
	bool							Resize(hh_u32 newSize);
	bool							HardResize(hh_u32 newSize);

	HH_FORCEINLINE bool				Condense() { return HardResize(BaseImpl::_Count()); }

	HH_FORCEINLINE hh_u32			Count() const { return BaseImpl::_Count(); }
	HH_FORCEINLINE bool				Empty() const { return BaseImpl::_Count() == 0; }
	HH_FORCEINLINE hh_u32			ElementSizeInBytes() const { return sizeof(_Type); }
	HH_FORCEINLINE hh_u32			SizeInBytes() const { return BaseImpl::_Count() * sizeof(_Type); }
	HH_FORCEINLINE hh_u32			CoveredBytes() const { return BaseImpl::_Count() * sizeof(_Type); }	// for interface unity with memory views
	HH_FORCEINLINE hh_u32			Stride() const { return sizeof(_Type); }

	HH_FORCEINLINE hh_u32			Capacity() const { return BaseImpl::_MaxCount(); }

	template<typename _TypeCtor>
	HH_FORCEINLINE CGuid			PushFront(_TypeCtor const &element) { return Insert(element, 0); }
	template<typename _TypeCtor>
	CGuid							PushBack(_TypeCtor const &element);

#if	(PK_HAS_RVALUE_REF != 0)
	HH_FORCEINLINE CGuid			PushFront(_Type &&element) { return Insert(element, 0); }
	CGuid							PushBack(_Type &&element);
#endif

	HH_FORCEINLINE CGuid			PushFront() { return InsertDefault(0); }
	CGuid							PushBack();

	HH_DEPRECATED("deprecated v1.11.0.32246 2016/11/12, use PushBack() instead")
	HH_FORCEINLINE CGuid			Push(_Type const &element) { return PushBack(element); }
	HH_DEPRECATED("deprecated v1.11.0.32246 2016/11/12, use PushBack() instead")
	HH_FORCEINLINE CGuid			Push() /* PushBack() */ { return PushBack(); }

	// these two never fail. the array should be large enough
	template<typename _TypeCtor>
	hh_u32							PushBackUnsafe(_TypeCtor const &element);
	hh_u32							PushBackUnsafe();

	HH_FORCEINLINE _Type			Pop() /* PopBack() */ { return PopBack(); }
	HH_FORCEINLINE void				PopAndDiscard() /* PopBackAndDiscard() */ { PopBackAndDiscard(); }
	_Type							PopBack();
	void							PopBackAndDiscard();
	_Type							PopFront();
	void							PopFrontAndDiscard();

	template<typename _Type2, typename _OtherImpl>
	CGuid							Merge(const TArray_Base<_Type2, _OtherImpl> &other);

	template<typename _Type2, typename _OtherImpl>
	CGuid							operator <<= (const TArray_Base<_Type2, _OtherImpl> &other) { return Merge(other); }

	template<typename _TypeCtor>
	CGuid							Insert(_TypeCtor const &element, hh_u32 where);
#if	(PK_HAS_RVALUE_REF != 0)
//	CGuid							Insert(_Type &&element, hh_u32 where);	// Currently no need for that, not used anywhere in the codebase
#endif
	CGuid							InsertDefault(hh_u32 where);
	template<typename _TypeArray>
	CGuid							InsertArray(const _TypeArray &array, hh_u32 where);

	void							Clear();	// empties the array, but doesn't free the memory, in case you need to push some more stuff in
	void							Clean();	// frees the buffer

	// conservative, order-preserving remove :
	void							Remove_AndKeepOrder(hh_u32 index);
	void							Remove_AndKeepOrder(hh_u32 indexStart, hh_u32 indexEnd);
	Iterator						RemoveIt_AndKeepOrder(const Iterator &it);	// returns the element right after 'it' once 'it' has been removed
	void							RemoveElement_AndKeepOrder(ValueTypeConst const &element);
	void							RemoveElementFromRawPointerInArray_AndKeepOrder(ValueTypeConst *element);
	void							RemoveAllElements_AndKeepOrder(ValueTypeConst const &element);

	// fast remove, does not guarantee order-preservation :
	void							Remove(hh_u32 index);
	Iterator						RemoveIt(const Iterator &it);	// returns the element right after 'it' once 'it' has been removed
	template<typename _Type2>
	void							RemoveElement(_Type2 const &element);
	void							RemoveElementFromRawPointerInArray(ValueTypeConst *element);
	template<typename _Type2>
	hh_u32							RemoveAllElements(_Type2 const &element);

	template<typename _Type2>
	bool							Contains(_Type2 const &element) const;
	template<typename _Type2>
	HH_FORCEINLINE CGuid			IndexOf(_Type2 const &element) const { return IndexOfFirst(element); }
	template<typename _Type2>
	CGuid							IndexOfFirst(_Type2 const &element) const;
	template<typename _Type2>
	CGuid							IndexOfLast(_Type2 const &element) const;
	CGuid							IndexOfElementFromRawPointerInArray(ValueTypeConst *elementInArray) const;

	template<typename _Type2>
	ValueTypeConst					*Find(_Type2 const &element) const;
	template<typename _Type2>
	ValueType						*Find(_Type2 const &element);
};

//----------------------------------------------------------------------------
//
//	Dynamic arrays
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Controller = TArrayStaticController<> >
class	TArray : public TArray_Base<_Type, TArray_BaseContainerImpl<_Type, _Controller> >
{
public:
	typedef	TArray<_Type, _Controller>											SelfType;
	typedef TArray_Base<_Type, TArray_BaseContainerImpl<_Type, _Controller> >	BaseType;
	TArray() {}
	explicit TArray(hh_u32 size) : BaseType(size) {}
	TArray(const SelfType &other) : BaseType(other) {}
	TArray(const TMemoryView<const _Type> &view) : BaseType(view) {}
	TArray(const TStridedMemoryView<const _Type> &view) : BaseType(view) {}

	template<typename _OtherController>
	void	operator = (const TArray_Base<_Type, _OtherController> &copyFrom) { this->_Copy(TMemoryView<const _Type>(copyFrom)); }
	void	operator = (const TMemoryView<typename BaseType::ValueType> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TMemoryView<typename BaseType::ValueTypeConst> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TStridedMemoryViewBase<typename BaseType::ValueType> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TStridedMemoryViewBase<typename BaseType::ValueTypeConst> &copyFrom) { this->_Copy(copyFrom); }
};

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
void	HHSwap(TArray<_Type, _Controller> &array0, TArray<_Type, _Controller> &array1)
{
	array0.Swap(array1);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
void	PKSwapEndianInPlace(TArray<_Type, _Controller> &object)
{
	PKSwapEndianInPlace(object.ViewForWriting());
}

//----------------------------------------------------------------------------
//
//	Safe dynamic arrays.
//	These assume _Type is never relocatable
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Controller = TArrayStaticController<> >
class	TArraySafe : public TArray_Base<_Type, TArray_BaseContainerImpl<_Type, _Controller, FalseType> >
{
public:
	typedef	TArraySafe<_Type, _Controller>										SelfType;
	typedef TArray_Base<_Type, TArray_BaseContainerImpl<_Type, _Controller, FalseType> >	BaseType;
	TArraySafe() {}
	explicit TArraySafe(hh_u32 size) : BaseType(size) {}
	TArraySafe(const SelfType &other) : BaseType(other) {}
	TArraySafe(const TMemoryView<const _Type> &view) : BaseType(view) {}
	TArraySafe(const TStridedMemoryView<const _Type> &view) : BaseType(view) {}

	template<typename _OtherController>
	void	operator = (const TArray_Base<_Type, _OtherController> &copyFrom) { this->_Copy(TMemoryView<const _Type>(copyFrom)); }
	void	operator = (const TMemoryView<typename BaseType::ValueType> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TMemoryView<typename BaseType::ValueTypeConst> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TStridedMemoryViewBase<typename BaseType::ValueType> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TStridedMemoryViewBase<typename BaseType::ValueTypeConst> &copyFrom) { this->_Copy(copyFrom); }
};

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
void	HHSwap(TArraySafe<_Type, _Controller> &array0, TArraySafe<_Type, _Controller> &array1)
{
	array0.Swap(array1);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
void	PKSwapEndianInPlace(TArraySafe<_Type, _Controller> &object)
{
	PKSwapEndianInPlace(object.ViewForWriting());
}

//----------------------------------------------------------------------------
//
//	Semi-dynamic arrays
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller, typename _HasTrivialRelocate = TTypeTraits::HasTrivialRelocate<_Type> >
class	TSemiDynamicArray_BaseContainerImpl
{
public:
	typedef	TSemiDynamicArray_BaseContainerImpl<_Type,
												_FixedCount,
												_Controller,
												_HasTrivialRelocate>	SelfType;
	typedef	_HasTrivialRelocate											HasTrivialRelocate;

private:
	// Alignment to fix misaligned pointer access undefined behavior possibly inside _Type
	// (keep it enum for visual studio natvis)
	enum { kAlignment = HH_ALIGNOF(_Type) };
	static const hh_ureg		_kAlignment = kAlignment; // (and static const for gdb)

	struct SAllocated
	{
		hh_u8		*m_Data;
		hh_u32		m_MaxCount;
	};

	enum
	{
		kMinSize = sizeof(SAllocated),
		kAlignedSize = _FixedCount * sizeof(_Type) + kAlignment - 1,
		kRawArraySize = kAlignedSize < kMinSize ? kMinSize : kAlignedSize,
	};
	enum // split enum or vs2015 wont compile !???
	{
		kRawStaticCount = (kRawArraySize - (kAlignment - 1)) / sizeof(_Type),
	};

	HH_STATIC_ASSERT(kRawStaticCount >= _FixedCount);

	static const hh_u32			kAllocationMask = 0x80000000U;
	hh_u32						m_Count;

	union
	{
		SAllocated		m_Allocated;
		hh_u8			m_StaticData[kRawArraySize];
	};

	HH_FORCEINLINE bool			Allocated() const { return !!(m_Count & kAllocationMask); }
	void						SetAllocated(bool allocated) { m_Count = (m_Count & ~kAllocationMask) | (allocated ? kAllocationMask : 0); }
	HH_FORCEINLINE void			SetAllocatedDataPtr(_Type *ptr) { SetAllocated(true); m_Allocated.m_Data = reinterpret_cast<hh_u8*>(ptr); }

protected:
	HH_FORCEINLINE _Type		*_Data() { return (m_Count & kAllocationMask) ? reinterpret_cast<_Type*>(m_Allocated.m_Data) : reinterpret_cast<_Type*>(Mem::Align<kAlignment>(m_StaticData)); }
	HH_FORCEINLINE const _Type	*_Data() const { return (m_Count & kAllocationMask) ? reinterpret_cast<const _Type*>(m_Allocated.m_Data) : reinterpret_cast<const _Type*>(Mem::Align<kAlignment>(m_StaticData)); }

	HH_FORCEINLINE hh_u32		_Count() const { return m_Count & ~kAllocationMask; }
	HH_FORCEINLINE void			_SetCount(hh_u32 count) { m_Count = (m_Count & kAllocationMask) | count; }

	HH_FORCEINLINE hh_u32		_MaxCount() const { if (!Allocated()) return kRawStaticCount; return m_Allocated.m_MaxCount; }
	HH_FORCEINLINE void			_SetMaxCount(hh_u32 newCapacity) { if (Allocated()) m_Allocated.m_MaxCount = newCapacity; }

	bool						_ReallocBuffer(hh_u32 newSize);
	bool						_Grow(hh_u32 wishedSize);
	bool						_RawResize(hh_u32 newSize);
	void						_Clean();	// frees the buffer

public:
	TSemiDynamicArray_BaseContainerImpl();
	~TSemiDynamicArray_BaseContainerImpl();

	void						Swap(SelfType &swapWith);
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller = TArrayStaticController<> >
class	TSemiDynamicArray : public TArray_Base<_Type, TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller> >
{
public:
	typedef	TSemiDynamicArray<_Type, _FixedCount, _Controller>	SelfType;
	typedef TArray_Base<_Type, TSemiDynamicArray_BaseContainerImpl<_Type, _FixedCount, _Controller> >		BaseType;

	TSemiDynamicArray() {}
	explicit TSemiDynamicArray(hh_u32 reserveSize) : BaseType(reserveSize) {}
	TSemiDynamicArray(const SelfType &other) : BaseType(other) {}
	TSemiDynamicArray(const TMemoryView<const _Type> &view) : BaseType(view) {}
	TSemiDynamicArray(const TStridedMemoryView<const _Type> &view) : BaseType(view) {}

	template<typename _OtherController>
	void	operator = (const TArray_Base<_Type, _OtherController> &copyFrom) { this->_Copy(TMemoryView<const _Type>(copyFrom)); }
	void	operator = (const TMemoryView<typename BaseType::ValueType> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TMemoryView<typename BaseType::ValueTypeConst> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TStridedMemoryViewBase<typename BaseType::ValueType> &copyFrom) { this->_Copy(copyFrom); }
	void	operator = (const TStridedMemoryViewBase<typename BaseType::ValueTypeConst> &copyFrom) { this->_Copy(copyFrom); }
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller>
void	HHSwap(TSemiDynamicArray<_Type, _FixedCount, _Controller> &array0, TSemiDynamicArray<_Type, _FixedCount, _Controller> &array1)
{
	array0.Swap(array1);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _FixedCount, typename _Controller>
void	PKSwapEndianInPlace(TSemiDynamicArray<_Type, _FixedCount, _Controller> &object)
{
	PKSwapEndianInPlace(object.ViewForWriting());
}

//----------------------------------------------------------------------------
//
//	Container template helpers:
//
//----------------------------------------------------------------------------

namespace	TContainerTraits
{
	template<typename _Type>						struct	IsArray : FalseType {};
	template<typename _Type, typename _Controller>	struct	IsArray<TArray<_Type, _Controller> > : TrueType {};
	template<typename _Type, typename _Controller>	struct	IsArray<TArraySafe<_Type, _Controller> > : TrueType {};

	template<typename _Type>
	struct	RemoveArray
	{
		typedef	_Type	Type;
		typedef	_Type	PureType;
	};

	template<typename _TypeToArrayify, typename _Type>
	struct	MakeArrayIfArray
	{
		typedef	_TypeToArrayify	Type;
	};

	// TArray
	template<typename _Type, typename _Controller>
	struct	RemoveArray<TArray<_Type, _Controller> >
	{
		typedef	_Type	Type;
		typedef	typename RemoveArray<_Type>::PureType	PureType;	// remove all nested arrays
	};

	template<typename _TypeToArrayify, typename _Type, typename _Controller>
	struct	MakeArrayIfArray<_TypeToArrayify, TArray<_Type, _Controller> >
	{
		typedef	TArray<_TypeToArrayify, _Controller>	Type;
	};

#if 0
	// TArraySafe
	template<typename _Type, typename _Controller>
	struct	RemoveArray<TArraySafe<_Type, _Controller> >
	{
		typedef	_Type	Type;
		typedef	typename RemoveArray<_Type>::PureType	PureType;	// remove all nested arrays
	};

	template<typename _TypeToArrayify, typename _Type, typename _Controller>
	struct	MakeArrayIfArray<_TypeToArrayify, TArraySafe<_Type, _Controller> >
	{
		typedef	TArraySafe<_TypeToArrayify, _Controller>	Type;
	};
#endif

	template<typename _Type>
	struct	RemoveContainer : public RemoveArray<_Type> {};
}

//----------------------------------------------------------------------------
//
//	Arrays compare ops with memoryviews:
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
bool	operator == (const TArray_Base<_Type, _Controller> &a, const TMemoryView<const _Type> &b)
{
	if (a.Count() != b.Count())
		return false;

	for (hh_u32 i = 0, stop = a.Count(); i < stop; i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
bool	operator != (const TArray_Base<_Type, _Controller> &a, const TMemoryView<const _Type> &b)
{
	return !(a == b);
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
bool	operator == (const TArray_Base<_Type, _Controller> &a, const TStridedMemoryViewBase<const _Type> &b)
{
	if (a.Count() != b.Count())
		return false;

	for (hh_u32 i = 0, stop = a.Count(); i < stop; i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, typename _Controller>
bool	operator != (const TArray_Base<_Type, _Controller> &a, const TStridedMemoryViewBase<const _Type> &b)
{
	return !(a == b);
}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_containers_array.inl"

#endif // __KR_CONTAINERS_ARRAY_H__
