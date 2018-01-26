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

#ifndef __KR_MEMORYVIEWS_H__
#define __KR_MEMORYVIEWS_H__

#include "hh_kernel/include/kr_iterators.h"
#include "hh_kernel/include/kr_mem_utils.h"	// for Mem::SwapEndian*() functions

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Memory Views
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TMemoryView
{
public:
	typedef	_Type													ValueType;
	typedef typename TType_BuildConst<_Type>::Type					ValueTypeConst;
	typedef typename TType_RemoveConstness<_Type>::Type				ValueTypeNonConst;
	typedef	typename TType_TransferConstness<_Type, hh_u8>::Type*	_RawDataPtr;
	typedef TMemoryView<_Type>										SelfType;

	typedef	TContiguousBaseIterator<ValueType>						Iterator;
	typedef	TContiguousBaseIterator<ValueTypeConst>					ConstIterator;

protected:
	_RawDataPtr		m_Data;
	hh_u32			m_Count;

	void			_AssertAccessible(hh_u32 index) const { HH_ASSERT(m_Data != null); HH_ASSERT(index < m_Count); }

public:
	TMemoryView() : m_Data(null), m_Count(0) {}
	explicit TMemoryView(_Type &data) : m_Data(reinterpret_cast<_RawDataPtr>(&data)), m_Count(1) {}
	template<hh_u32 _Count>
	TMemoryView(_Type (&data)[_Count]) : m_Data(reinterpret_cast<_RawDataPtr>(&data[0])), m_Count(_Count) {}
	TMemoryView(_Type *data, hh_u32 count) : m_Data(reinterpret_cast<_RawDataPtr>(data)), m_Count(count) {}

	TMemoryView(const TMemoryView<ValueTypeNonConst> &copyFrom)
	:	m_Data(reinterpret_cast<_RawDataPtr>(static_cast<_Type*>(copyFrom.Data())))
	,	m_Count(copyFrom.Count())
	{
	}

	template<typename _TypeCopy>
	explicit TMemoryView(const TMemoryView<_TypeCopy> &copyFrom)
	:	m_Data(reinterpret_cast<_RawDataPtr>(copyFrom.Data()))
	,	m_Count(copyFrom.Count())
	{
		HH_STATIC_ASSERT_CUSTOM(sizeof(_Type) == sizeof(_TypeCopy), "Incompatible_Footprints___Destination_Overspans_Source");
	}

	~TMemoryView() {}

	HH_INLINE SelfType	Slice(hh_u32 begin) const
	{
		HH_ASSERT(begin <= m_Count);
		return Slice(begin, m_Count - begin);
	}
	HH_INLINE SelfType	Slice(hh_u32 begin, hh_u32 count) const
	{
		HH_ASSERT(begin + count <= m_Count);
		return SelfType(reinterpret_cast<_Type*>(m_Data + sizeof(_Type) * begin), count);
	}
	HH_INLINE SelfType	SliceIFP(hh_u32 begin) const { if (!Empty()) return Slice(begin); return SelfType(); }
	HH_INLINE SelfType	SliceIFP(hh_u32 begin, hh_u32 count) const { if (!Empty()) return Slice(begin, count); return SelfType(); }

	template<typename _Type2>
	static HH_INLINE SelfType	Reinterpret(const TMemoryView<_Type2> &otherView)
	{
		HH_STATIC_ASSERT_CUSTOM(sizeof(_Type) == sizeof(_Type2), "Incompatible_Footprints");
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherView.Data())), otherView.Count());
	}
	template<typename _Type2>
	static HH_INLINE SelfType	ReinterpretSlice(const TMemoryView<_Type2> &otherView, hh_u32 begin, hh_u32 count)
	{
		HH_STATIC_ASSERT_CUSTOM(sizeof(_Type) == sizeof(_Type2), "Incompatible_Footprints");
		HH_ASSERT(begin + count <= otherView.Count());
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherView.Data() + begin)), count);
	}

	HH_FORCEINLINE void			Clear() { m_Data = null; m_Count = 0; }
	HH_FORCEINLINE bool			Empty() const { return m_Data == null || m_Count == 0; }
	HH_FORCEINLINE hh_u32		Count() const { return m_Count; }
	HH_FORCEINLINE hh_u32		ElementSizeInBytes() const { return sizeof(_Type); }
	HH_FORCEINLINE hh_u32		ElementFootprintInBytes() const { return sizeof(_Type); }
	HH_FORCEINLINE hh_u32		CoveredBytes() const { return m_Count * ElementSizeInBytes(); }
	HH_FORCEINLINE _Type		*Data() const { return reinterpret_cast<_Type*>(m_Data); }
	HH_FORCEINLINE _Type		*DataEnd() const { return reinterpret_cast<_Type*>(m_Data + CoveredBytes()); }

	HH_FORCEINLINE _Type		&operator [] (hh_u32 index) const
	{
		HH_ONLY_IF_ASSERTS(_AssertAccessible(index));
		return *reinterpret_cast<_Type*>(m_Data + sizeof(_Type) * index);
	}
	HH_FORCEINLINE void			operator += (hh_u32 index)
	{
		if (!Empty())
		{
			HH_ASSERT(m_Data != null || m_Count == 0);
			HH_ASSERT(index <= m_Count);
			m_Data += sizeof(_Type) * index;
			m_Count -= index;
		}
	}
	HH_FORCEINLINE SelfType		operator + (hh_u32 index) const
	{
		SelfType	other = *this;
		other += index;
		return other;
	}

	HH_FORCEINLINE _Type		&First() const { return operator [] (0); }
	HH_FORCEINLINE _Type		&Last() const { return operator [] (m_Count - 1); }

	HH_FORCEINLINE Iterator		Begin() const { return Iterator((_Type*)m_Data); }
	HH_FORCEINLINE Iterator		End() const { return Iterator((_Type*)(m_Data + m_Count * sizeof(_Type))); }

	HH_FORCEINLINE hh_u32		Stride() const { return sizeof(_Type); }
};

//----------------------------------------------------------------------------
//
//	Strided Memory Views (common base)
//
//----------------------------------------------------------------------------

struct			SStridedMemoryViewRawStorage
{
	hh_u8		*m_RawDataPtr;
	hh_u32		m_Count;
	hh_u32		m_Stride;
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TStridedMemoryViewBase
{
public:
	typedef	_Type													ValueType;
	typedef typename TType_BuildConst<_Type>::Type					ValueTypeConst;
	typedef typename TType_RemoveConstness<_Type>::Type				ValueTypeNonConst;
	typedef	typename TType_TransferConstness<_Type, hh_u8>::Type*	_RawDataPtr;

	typedef	TStridedBaseIterator<ValueType>							Iterator;
	typedef	TStridedBaseIterator<ValueTypeConst>					ConstIterator;

protected:

	SStridedMemoryViewRawStorage	m_Storage;

	HH_FORCEINLINE _RawDataPtr		_InternalData() const { return (_RawDataPtr)m_Storage.m_RawDataPtr; }
	void							_AssertAccessible(hh_u32 index) const { HH_ASSERT(m_Storage.m_RawDataPtr != null); HH_ASSERT(index < m_Storage.m_Count); }

public:
	TStridedMemoryViewBase()
	{
		m_Storage.m_RawDataPtr = null;
		m_Storage.m_Count = 0;
		m_Storage.m_Stride = 0;
	}
	TStridedMemoryViewBase(_Type *data, hh_u32 count, hh_u32 stride)
	{
		_RawDataPtr	ptr = reinterpret_cast<_RawDataPtr>(data);
		m_Storage.m_RawDataPtr = const_cast<hh_u8*>(ptr);	// necessary evil.
		m_Storage.m_Count = count;
		m_Storage.m_Stride = stride;
	}

	HH_FORCEINLINE void			Clear() { m_Storage.m_RawDataPtr = null; m_Storage.m_Stride = 0; m_Storage.m_Count = 0; }
	HH_FORCEINLINE bool			Empty() const { return m_Storage.m_RawDataPtr == null || m_Storage.m_Count == 0; }
	HH_FORCEINLINE hh_u32		Count() const { return m_Storage.m_Count; }
	HH_FORCEINLINE hh_u32		ElementSizeInBytes() const { return sizeof(_Type); }
	HH_FORCEINLINE hh_u32		Stride() const { return m_Storage.m_Stride; }
	HH_FORCEINLINE hh_u32		CoveredBytes() const { return (Stride() != 0 ? m_Storage.m_Count * Stride(): ElementSizeInBytes()); }
	HH_FORCEINLINE _Type		*Data() const { return reinterpret_cast<_Type*>(_InternalData()); }
	HH_FORCEINLINE _Type		*DataEnd() const { return reinterpret_cast<_Type*>(_InternalData() + CoveredBytes()); }
	HH_FORCEINLINE bool			Virtual() const { return Stride() == 0 /*&& Count() > 1*/; }

	HH_FORCEINLINE _Type		& operator [] (hh_u32 index) const
	{
		HH_ONLY_IF_ASSERTS(_AssertAccessible(index));
		HH_ASSUME(m_Storage.m_Stride >= sizeof(_Type) || m_Storage.m_Stride == 0);
		return *reinterpret_cast<_Type*>(_InternalData() + m_Storage.m_Stride * index);
	}
	HH_FORCEINLINE void			operator += (hh_u32 index)
	{
		if (!Empty())
		{
			HH_ASSERT(_InternalData() != null || m_Storage.m_Count == 0);
			HH_ASSERT(index <= m_Storage.m_Count);
			HH_ASSUME(m_Storage.m_Stride >= sizeof(_Type) || m_Storage.m_Stride == 0);
			m_Storage.m_RawDataPtr += m_Storage.m_Stride * index;
			m_Storage.m_Count -= index;
		}
	}

	HH_FORCEINLINE _Type		&First() const { return operator [] (0); }
	HH_FORCEINLINE _Type		&Last() const { return operator [] (m_Storage.m_Count - 1); }

	HH_FORCEINLINE Iterator		Begin() const { return Iterator((_Type*)m_Storage.m_RawDataPtr, m_Storage.m_Stride); }
	HH_FORCEINLINE Iterator		End() const { return Iterator((_Type*)(m_Storage.m_RawDataPtr + m_Storage.m_Count * m_Storage.m_Stride), m_Storage.m_Stride); }
};

//----------------------------------------------------------------------------
//
//	Strided Memory Views With Static Footprint
//
//	ex: sizeof(_Type) == 12, footprint == 16, stride == 24, count = 3:
//
//	############____........############____........############____........
//	[data      ]pad][other ]
//	[element 0     ]........[element 1     ]........[element 2     ]........
//	0          12  16      24          36  40       48         60  64      72
//
//----------------------------------------------------------------------------

template<typename _Type, hh_i32 _FootprintInBytes = -1>
class	TStridedMemoryView : public TStridedMemoryViewBase<_Type>
{
public:
	typedef TStridedMemoryViewBase<_Type>					BaseType;
	typedef TStridedMemoryView<_Type, _FootprintInBytes>	SelfType;
	typedef typename BaseType::ValueTypeConst				ValueTypeConst;
	typedef typename BaseType::ValueTypeNonConst			ValueTypeNonConst;
	static const hh_u32										ValueFootprintInBytes = _FootprintInBytes < 0 ? sizeof(_Type) : hh_u32(_FootprintInBytes);
	typedef	typename BaseType::_RawDataPtr					_RawDataPtr;

	HH_STATIC_ASSERT(ValueFootprintInBytes >= sizeof(_Type));
	HH_STATIC_ASSERT(sizeof(BaseType) == sizeof(SStridedMemoryViewRawStorage));

protected:
public:
	TStridedMemoryView()
	{}
	explicit TStridedMemoryView(_Type &data)
	:	BaseType(&data, 1, sizeof(_Type))
	{}
	template<hh_u32 _Count>
	TStridedMemoryView(_Type (&data)[_Count])
	:	BaseType(&data[0], _Count, sizeof(_Type))
	{}
	TStridedMemoryView(_Type *data, hh_u32 count, hh_u32 stride = sizeof(_Type))
	:	BaseType(data, count, stride)
	{
		HH_ASSERT(stride >= ValueFootprintInBytes || stride == 0);
	}

	// build from a memview
	TStridedMemoryView(const TMemoryView<ValueTypeConst> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), sizeof(_Type))
	{}
	TStridedMemoryView(const TMemoryView<ValueTypeNonConst> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), sizeof(_Type))
	{}
	template<typename _TypeCopy>
	explicit TStridedMemoryView(const TMemoryView<_TypeCopy> &copyFrom)
	:	BaseType(reinterpret_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), sizeof(_TypeCopy))
	{
		HH_STATIC_ASSERT_CUSTOM(ValueFootprintInBytes <= sizeof(_TypeCopy), "Incompatible_Footprints___Destination_Overspans_Source");
	}

	// build from a strided memview
	template<hh_i32 _FootprintInBytesCopy>
	TStridedMemoryView(const TStridedMemoryView<ValueTypeNonConst, _FootprintInBytesCopy> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), copyFrom.Stride())
	{
		HH_STATIC_ASSERT_CUSTOM(ValueFootprintInBytes <= (TStridedMemoryView<ValueTypeNonConst, _FootprintInBytesCopy>::ValueFootprintInBytes), "Incompatible_Footprints___Destination_Overspans_Source");
	}
	template<typename _TypeCopy, hh_i32 _FootprintInBytesCopy>
	explicit TStridedMemoryView(const TStridedMemoryView<_TypeCopy, _FootprintInBytesCopy> &copyFrom)
	:	BaseType(reinterpret_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), copyFrom.Stride())
	{
		HH_STATIC_ASSERT_CUSTOM(ValueFootprintInBytes <= (TStridedMemoryView<_TypeCopy, _FootprintInBytesCopy>::ValueFootprintInBytes), "Incompatible_Footprints___Destination_Overspans_Source");
	}

	~TStridedMemoryView() {}

	HH_INLINE SelfType	Slice(hh_u32 begin) const
	{
		return Slice(begin, BaseType::m_Storage.m_Count);
	}
	HH_INLINE SelfType	Slice(hh_u32 begin, hh_u32 count) const
	{
		HH_ASSERT(begin + count <= BaseType::m_Storage.m_Count);
		return SelfType(reinterpret_cast<_Type*>(BaseType::_InternalData() + BaseType::m_Storage.m_Stride * begin), count, BaseType::m_Storage.m_Stride);
	}
	HH_INLINE SelfType	SliceIFP(hh_u32 begin) const { if (!BaseType::Empty()) return Slice(begin); return SelfType(); }
	HH_INLINE SelfType	SliceIFP(hh_u32 begin, hh_u32 count) const { if (!BaseType::Empty()) return Slice(begin, count); return SelfType(); }

	template<typename _Type2, hh_i32 _FootprintInBytes2>
	static HH_INLINE SelfType	Reinterpret(const TStridedMemoryView<_Type2, _FootprintInBytes2> &otherView, hh_u32 baseOffsetInBytes = 0)
	{
		//HH_STATIC_ASSERT_CUSTOM(ValueFootprintInBytes <= (TStridedMemoryView<_Type2, _FootprintInBytes2>::ValueFootprintInBytes), "Incompatible_Footprints___Destination_Overspans_Source");
		HH_ASSERT(baseOffsetInBytes + ValueFootprintInBytes <= otherView.Stride() || otherView.Stride() == 0);
		_Type2	*otherViewPtr = reinterpret_cast<_Type2*>(reinterpret_cast<typename BaseType::_RawDataPtr>(otherView.Data()));
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherViewPtr) + baseOffsetInBytes),
						otherView.Count(),
						otherView.Stride());
	}
	template<hh_u32 _BaseOffsetInBytes, typename _Type2, hh_i32 _FootprintInBytes2>
	static HH_INLINE SelfType	Reinterpret(const TStridedMemoryView<_Type2, _FootprintInBytes2> &otherView)
	{
		//HH_STATIC_ASSERT_CUSTOM(_BaseOffsetInBytes + ValueFootprintInBytes <= (TStridedMemoryView<_Type2, _FootprintInBytes2>::ValueFootprintInBytes), "Incompatible_Footprints___Destination_Overspans_Source");
		HH_ASSERT(_BaseOffsetInBytes + ValueFootprintInBytes <= otherView.Stride() || otherView.Stride() == 0);
		_Type2	*otherViewPtr = otherView.Data();
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherViewPtr) + _BaseOffsetInBytes),
						otherView.Count(),
						otherView.Stride());
	}
	template<typename _Type2, hh_i32 _FootprintInBytes2>
	static HH_INLINE SelfType	ReinterpretSlice(const TStridedMemoryView<_Type2, _FootprintInBytes2> &otherView, hh_u32 baseOffsetInBytes, hh_u32 begin, hh_u32 count)
	{
		//HH_STATIC_ASSERT_CUSTOM(ValueFootprintInBytes <= (TStridedMemoryView<_Type2, _FootprintInBytes2>::ValueFootprintInBytes), "Incompatible_Footprints___Destination_Overspans_Source");
		HH_ASSERT(baseOffsetInBytes + ValueFootprintInBytes <= otherView.Stride() || otherView.Stride() == 0);
		HH_ASSERT(begin + count <= otherView.Count());
		_Type2	*otherViewPtr = reinterpret_cast<_Type2*>(reinterpret_cast<typename BaseType::_RawDataPtr>(otherView.Data()) + begin * otherView.Stride());
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherViewPtr) + baseOffsetInBytes),
						count,
						otherView.Stride());
	}

	// helper to roll back to a standard memview. returns an empty view if the conversion failed (ex: non-contiguous view)
	TMemoryView<_Type>			ToMemoryViewIFP() const
	{
		// Sometimes there are virtual stridedmemviews (stride = 0) but with a single element.
		// These can be safely converted to a single element memview
		if (Contiguous() | (BaseType::m_Storage.m_Count == 1))
		{
			return TMemoryView<_Type>(reinterpret_cast<_Type*>(BaseType::m_Storage.m_RawDataPtr), BaseType::m_Storage.m_Count);
		}
		return TMemoryView<_Type>();
	}

	HH_FORCEINLINE hh_u32		ElementFootprintInBytes() const { return ValueFootprintInBytes; }
	bool						ContiguousFootprint() const { return BaseType::m_Storage.m_Stride == ValueFootprintInBytes; }
	bool						Contiguous() const { return ContiguousFootprint() && ValueFootprintInBytes == sizeof(_Type); }

	HH_FORCEINLINE SelfType		operator + (hh_u32 index) const
	{
		SelfType	other = *this;
		other += index;
		return other;
	}
};

//----------------------------------------------------------------------------
//
//	Strided Memory Views With Dynamic Footprint
//
//	ex: sizeof(_Type) == 12, footprint == 16, stride == 24, count = 3:
//
//	############____........############____........############____........
//	[data      ]pad][other ]
//	[element 0     ]........[element 1     ]........[element 2     ]........
//	0          12  16      24          36  40       48         60  64      72
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TStridedMemoryViewWithFootprint : public TStridedMemoryViewBase<_Type>
{
public:
	typedef TStridedMemoryViewBase<_Type>				BaseType;
	typedef TStridedMemoryViewWithFootprint<_Type>		SelfType;
	typedef typename BaseType::ValueTypeConst			ValueTypeConst;
	typedef typename BaseType::ValueTypeNonConst		ValueTypeNonConst;
	typedef	typename BaseType::_RawDataPtr				_RawDataPtr;

	HH_STATIC_ASSERT(sizeof(BaseType) == sizeof(SStridedMemoryViewRawStorage));

protected:
	hh_u32				m_ElementFootprintInBytes;

public:
	TStridedMemoryViewWithFootprint()
	:	m_ElementFootprintInBytes(sizeof(_Type))
	{}
	explicit TStridedMemoryViewWithFootprint(_Type &data)
	:	BaseType(&data, 1, sizeof(_Type))
	,	m_ElementFootprintInBytes(sizeof(_Type))
	{}
	template<hh_u32 _Count>
	TStridedMemoryViewWithFootprint(_Type (&data)[_Count])
	:	BaseType(&data[0], _Count, sizeof(_Type))
	,	m_ElementFootprintInBytes(sizeof(_Type))
	{}
	TStridedMemoryViewWithFootprint(_Type *data, hh_u32 count, hh_u32 stride = sizeof(_Type), hh_u32 footprint = sizeof(_Type))
	:	BaseType(data, count, stride)
	,	m_ElementFootprintInBytes(footprint)
	{
		HH_ASSERT(footprint <= stride || stride == 0);
	}

	// build from a memview
	TStridedMemoryViewWithFootprint(const TMemoryView<ValueTypeConst> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), sizeof(_Type))
	,	m_ElementFootprintInBytes(sizeof(_Type))
	{}
	TStridedMemoryViewWithFootprint(const TMemoryView<ValueTypeNonConst> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), sizeof(_Type))
	,	m_ElementFootprintInBytes(sizeof(_Type))
	{}
	template<typename _TypeCopy>
	explicit TStridedMemoryViewWithFootprint(const TMemoryView<_TypeCopy> &copyFrom)
	:	BaseType(reinterpret_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), sizeof(_TypeCopy))
	,	m_ElementFootprintInBytes(sizeof(_Type))
	{
		HH_STATIC_ASSERT_CUSTOM(sizeof(_Type) <= sizeof(_TypeCopy), "Incompatible_Footprints___Destination_Overspans_Source");
	}

	// build from a strided memview
	template<hh_i32 _FootprintInBytesCopy>
	TStridedMemoryViewWithFootprint(const TStridedMemoryView<ValueTypeNonConst, _FootprintInBytesCopy> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), copyFrom.Stride())
	,	m_ElementFootprintInBytes(copyFrom.ElementFootprintInBytes())
	{
		HH_ASSERT(sizeof(_Type) <= m_ElementFootprintInBytes);
	}
	template<typename _TypeCopy, hh_i32 _FootprintInBytesCopy>
	explicit TStridedMemoryViewWithFootprint(const TStridedMemoryView<_TypeCopy, _FootprintInBytesCopy> &copyFrom)
	:	BaseType(reinterpret_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), copyFrom.Stride())
	,	m_ElementFootprintInBytes(copyFrom.ElementFootprintInBytes())
	{
		HH_ASSERT(sizeof(_Type) <= m_ElementFootprintInBytes);
	}

	// build from a strided memview with footprint
	TStridedMemoryViewWithFootprint(const TStridedMemoryViewWithFootprint<ValueTypeNonConst> &copyFrom)
	:	BaseType(static_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), copyFrom.Stride())
	,	m_ElementFootprintInBytes(copyFrom.ElementFootprintInBytes())
	{
		HH_ASSERT(sizeof(_Type) <= m_ElementFootprintInBytes);
	}
	template<typename _TypeCopy>
	explicit TStridedMemoryViewWithFootprint(const TStridedMemoryViewWithFootprint<_TypeCopy> &copyFrom)
	:	BaseType(reinterpret_cast<_Type*>(copyFrom.Data()), copyFrom.Count(), copyFrom.Stride())
	,	m_ElementFootprintInBytes(copyFrom.ElementFootprintInBytes())
	{
		HH_ASSERT(sizeof(_Type) <= m_ElementFootprintInBytes);
	}

	~TStridedMemoryViewWithFootprint() {}

	HH_INLINE SelfType	Slice(hh_u32 begin) const
	{
		return Slice(begin, BaseType::m_Storage.m_Count);
	}
	HH_INLINE SelfType	Slice(hh_u32 begin, hh_u32 count) const
	{
		HH_ASSERT(begin + count <= BaseType::m_Storage.m_Count);
		return SelfType(reinterpret_cast<_Type*>(BaseType::_InternalData() + BaseType::m_Storage.m_Stride * begin), count, BaseType::m_Storage.m_Stride, m_ElementFootprintInBytes);
	}
	HH_INLINE SelfType	SliceIFP(hh_u32 begin) const { if (!BaseType::Empty()) return Slice(begin); return SelfType(); }
	HH_INLINE SelfType	SliceIFP(hh_u32 begin, hh_u32 count) const { if (!BaseType::Empty()) return Slice(begin, count); return SelfType(); }

	template<typename _Type2>
	static HH_INLINE SelfType	ReinterpretSlice(const TStridedMemoryViewWithFootprint<_Type2> &otherView, hh_u32 baseOffsetInBytes, hh_u32 begin, hh_u32 count)
	{
		HH_ASSERT(baseOffsetInBytes + sizeof(_Type) <= otherView.Stride());
		HH_ASSERT(begin + count <= otherView.Count());
		_Type2	*otherViewPtr = reinterpret_cast<_Type2*>(reinterpret_cast<typename BaseType::_RawDataPtr>(otherView.Data()) + begin * otherView.Stride());
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherViewPtr) + baseOffsetInBytes),
						count,
						otherView.Stride(),
						otherView.ElementFootprintInBytes());
	}
	template<hh_u32 _BaseOffsetInBytes, typename _Type2>
	static HH_INLINE SelfType	ReinterpretSlice(const TStridedMemoryViewWithFootprint<_Type2> &otherView)
	{
		HH_ASSERT(_BaseOffsetInBytes + sizeof(_Type) <= otherView.Stride());
		_Type2	*otherViewPtr = otherView.Data();
		return SelfType(reinterpret_cast<_Type*>(reinterpret_cast<typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type *>(otherViewPtr) + _BaseOffsetInBytes),
						otherView.Count(),
						otherView.Stride(),
						otherView.ElementFootprintInBytes());
	}

	// helper to roll back to a standard memview. returns an empty view if the conversion failed (ex: non-contiguous view)
	TMemoryView<_Type>			ToMemoryViewIFP() const
	{
		// Sometimes there are virtual stridedmemviews (stride = 0) but with a single element.
		// These can be safely converted to a single element memview
		if (Contiguous() | (BaseType::m_Storage.m_Count == 1))
		{
			return TMemoryView<_Type>(reinterpret_cast<_Type*>(BaseType::m_Storage.m_RawDataPtr), BaseType::m_Storage.m_Count);
		}
		return TMemoryView<_Type>();
	}

	HH_FORCEINLINE hh_u32		ElementFootprintInBytes() const { return m_ElementFootprintInBytes; }
	bool						ContiguousFootprint() const { return BaseType::m_Storage.m_Stride == m_ElementFootprintInBytes; }
	bool						Contiguous() const { return ContiguousFootprint() && m_ElementFootprintInBytes == sizeof(_Type); }

	HH_FORCEINLINE SelfType		operator + (hh_u32 index) const
	{
		SelfType	other = *this;
		other += index;
		return other;
	}
};

//----------------------------------------------------------------------------
//
//	endian helpers to swap a stream of data
//
//----------------------------------------------------------------------------

namespace	Internal
{
	template<hh_u32 _FootprintInBytes> struct	TNativeSwapper		: public FalseType {};
	template<> struct							TNativeSwapper<1>	: public TrueType { static void SwapInPlace(void *data, hh_u32 count) { } };
	template<> struct							TNativeSwapper<2>	: public TrueType { static void SwapInPlace(void *data, hh_u32 count) { Mem::SwapEndian16(data, count); } };
	template<> struct							TNativeSwapper<4>	: public TrueType { static void SwapInPlace(void *data, hh_u32 count) { Mem::SwapEndian32(data, count); } };
	template<> struct							TNativeSwapper<8>	: public TrueType { static void SwapInPlace(void *data, hh_u32 count) { Mem::SwapEndian64(data, count); } };

	template<typename _Type>
	struct	TStreamEndianSwapperHelpers
	{
		static void	SwapInPlace(_Type *data, hh_u32 count)
		{
			for (hh_u32 i = 0; i < count; i++)
			{
				PKSwapEndianInPlace(data[i]);
			}
		}
	};

	template<> struct	TStreamEndianSwapperHelpers<hh_u8> { static void	SwapInPlace(hh_u8 *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
	template<> struct	TStreamEndianSwapperHelpers<hh_i8> { static void	SwapInPlace(hh_i8 *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
	template<> struct	TStreamEndianSwapperHelpers<hh_u16> { static void	SwapInPlace(hh_u16 *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
	template<> struct	TStreamEndianSwapperHelpers<hh_i16> { static void	SwapInPlace(hh_i16 *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
	template<> struct	TStreamEndianSwapperHelpers<hh_u32> { static void	SwapInPlace(hh_u32 *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
	template<> struct	TStreamEndianSwapperHelpers<hh_i32> { static void	SwapInPlace(hh_i32 *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
	template<> struct	TStreamEndianSwapperHelpers<float> { static void	SwapInPlace(float *data, hh_u32 count) { TNativeSwapper<sizeof(*data)>::SwapInPlace(data, count); } };
}

//----------------------------------------------------------------------------

template<typename _Type>
void	PKSwapEndianInPlace(const TMemoryView<_Type> &object)
{
	Internal::TStreamEndianSwapperHelpers<_Type>::SwapInPlace(object.Data(), object.Count());
}

//----------------------------------------------------------------------------

template<typename _Type, hh_i32 _FootprintInBytes>
void	PKSwapEndianInPlace(const TStridedMemoryView<_Type, _FootprintInBytes> &object)
{
	if ((sizeof(_Type) == _FootprintInBytes ||		// compile-time culling : compact footprint
		_FootprintInBytes % sizeof(_Type) == 0) &&	// compile-time culling : footprint is a multiple of sizeof(_Type), we can reinterpret as a contiguous array of _Type objects and patch the count
		Internal::TNativeSwapper<sizeof(_Type)>::True &&	// we know how to endian-swap this type
		object.Stride() == _FootprintInBytes)
	{
		Internal::TStreamEndianSwapperHelpers<_Type>::SwapInPlace(object.Data(), object.Count() * (_FootprintInBytes / sizeof(_Type)));
		TMemoryView<_Type>	contiguousView = object.ToMemoryViewIFP();
		if (!contiguousView.Empty())
		{
			PKSwapEndianInPlace(contiguousView);
			return;
		}
	}

	for (hh_u32 i = 0; i < object.Count(); i++)
	{
		PKSwapEndianInPlace(object[i]);
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
void	PKSwapEndianInPlace(const TStridedMemoryViewWithFootprint<_Type> &object)
{
	TMemoryView<_Type>	contiguousView = object.ToMemoryViewIFP();
	if (!contiguousView.Empty())
	{
		PKSwapEndianInPlace(contiguousView);
		return;
	}

	for (hh_u32 i = 0; i < object.Count(); i++)
	{
		PKSwapEndianInPlace(object[i]);
	}
}

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_MEMORYVIEWS_H__
