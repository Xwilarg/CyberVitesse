#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2009/05/17 14:48 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_MESH_INL__
#define __GE_MESH_INL__

#ifndef	__GE_MESH_H__
#	error	ge_mesh.inl must not be included manually, include ge_mesh.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Implementations:
//
//----------------------------------------------------------------------------

template<CVStreamSemanticDictionnary::EDefaultOrdinals _Ordinal>
HH_FORCEINLINE bool	CVertexStreamDescriptor::Has() const
{
	HH_STATIC_ASSERT(_Ordinal != CVStreamSemanticDictionnary::Ordinal_INVALID && _Ordinal < CVStreamSemanticDictionnary::__MaxDefaultOrdinals);
	return (m_PackedFlags & (1 << _Ordinal)) != 0;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE bool	CVertexStreamDescriptor::Has(hh_u32 ordinal) const
{
	if (ordinal < CVStreamSemanticDictionnary::__MaxDefaultOrdinals)
	{
		HH_ASSERT(ordinal > CVStreamSemanticDictionnary::Ordinal_INVALID);
		return (m_PackedFlags & (1 << ordinal)) != 0;
	}
	return m_VertexDeclaration.FindAbstractStreamInternalIndex(ordinal).Valid();
}

//----------------------------------------------------------------------------

template<CVStreamSemanticDictionnary::EDefaultOrdinals _Ordinal>
HH_INLINE CGuid		CVertexStreamDescriptor::FindDefaultStreamInternalIndex() const
{
	HH_STATIC_ASSERT(_Ordinal != CVStreamSemanticDictionnary::Ordinal_INVALID && _Ordinal < CVStreamSemanticDictionnary::__MaxDefaultOrdinals && (1 << _Ordinal) <= InternalMask_Ordinals);
	static const hh_u32	ordMask = 1 << _Ordinal;
	if ((m_PackedFlags & ordMask) == 0)
	{
		HH_PARANOID_ASSERT(!m_VertexDeclaration.FindAbstractStreamInternalIndex(_Ordinal).Valid());
		return CGuid::INVALID;
	}

	// the number of bits set before the bit numbered 'ordinal' tells us how many streams are before 'ordinal'.
	// ex: 'ordinal' == 5, we mask out the 4 previous bits (we know bit 0 won't be set)
	// if there are two bits set in these 4 previous bits, we know we have two other streams before 'ordinal'.
	// therefore, the index of 'ordinal' is 2

	static const hh_u32	prevStreamsMask = InternalMask_Ordinals & (ordMask - 1);
	return IntegerTools::CountSetBits(m_PackedFlags & prevStreamsMask);	// index of the 'ordinal' stream.
}

//----------------------------------------------------------------------------

HH_INLINE SVStreamCode	CVertexStreamDescriptor::AbstractElementDeclaration(hh_u32 ordinal) const
{
	const CGuid	id = FindAbstractStreamInternalIndex(ordinal);
	if (!id.Valid())
		return SVStreamCode();
	return AbstractElementDeclarationFromInternalIndex(id);
}

//----------------------------------------------------------------------------

HH_INLINE SVStreamCode	CVertexStreamDescriptor::AbstractElementDeclarationFromInternalIndex(CGuid index) const
{
	HH_ASSERT(index.Valid());
	return m_VertexDeclaration.StreamCodes()[index];
}

//----------------------------------------------------------------------------
//
//	CMeshVStream
//
//----------------------------------------------------------------------------

HH_INLINE CGuid	CMeshVStream::AllocateAdditionalDataForWriting(hh_u32 neededVertexCount)
{
	HH_ASSERT(neededVertexCount > 0);
	const hh_u32	writeStart = m_Descriptor.m_VertexCount;
	const hh_u32	requiredVertexCount = writeStart + neededVertexCount;
	if (!_GrowIFN(requiredVertexCount))
		return CGuid::INVALID;
	m_Descriptor.m_VertexCount = requiredVertexCount;
	return writeStart;
}

//----------------------------------------------------------------------------

HH_INLINE TStridedMemoryView<const CFloat3>	CMeshVStream::Positions() const
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Position>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<const CFloat3>(*this, id);
	return TStridedMemoryView<const CFloat3>();
}

HH_INLINE TStridedMemoryView<CFloat3>		CMeshVStream::Positions()
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Position>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<CFloat3>(*this, id);
	return TStridedMemoryView<CFloat3>();
}

//----------------------------------------------------------------------------

HH_INLINE TStridedMemoryView<const CFloat3>	CMeshVStream::Normals() const
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Normal>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<const CFloat3>(*this, id);
	return TStridedMemoryView<const CFloat3>();
}

HH_INLINE TStridedMemoryView<CFloat3>		CMeshVStream::Normals()
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Normal>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<CFloat3>(*this, id);
	return TStridedMemoryView<CFloat3>();
}

//----------------------------------------------------------------------------

HH_INLINE TStridedMemoryView<const CFloat4>	CMeshVStream::Tangents() const
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Tangent>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<const CFloat4>(*this, id);
	return TStridedMemoryView<const CFloat4>();
}

HH_INLINE TStridedMemoryView<CFloat4>		CMeshVStream::Tangents()
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Tangent>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<CFloat4>(*this, id);
	return TStridedMemoryView<CFloat4>();
}

//----------------------------------------------------------------------------

HH_INLINE TStridedMemoryView<const CFloat4>	CMeshVStream::Colors() const
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Color>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<const CFloat4>(*this, id);
	return TStridedMemoryView<const CFloat4>();
}

HH_INLINE TStridedMemoryView<CFloat4>		CMeshVStream::Colors()
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Color>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<CFloat4>(*this, id);
	return TStridedMemoryView<CFloat4>();
}

//----------------------------------------------------------------------------

HH_INLINE TStridedMemoryView<const CFloat2>	CMeshVStream::Texcoords() const
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Texcoord>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<const CFloat2>(*this, id);
	return TStridedMemoryView<const CFloat2>();
}

HH_INLINE TStridedMemoryView<CFloat2>		CMeshVStream::Texcoords()
{
	const CGuid	id = FindDefaultStreamInternalIndex<CVStreamSemanticDictionnary::Ordinal_Texcoord>();
	if (id.Valid())
		return _AbstractStreamFromInternalIndexImpl<CFloat2>(*this, id);
	return TStridedMemoryView<CFloat2>();
}

//----------------------------------------------------------------------------

class	hh_fp16;	// hh_maths_half.h

namespace	MeshInternals
{
	template<typename _Type> struct	TStreamCodeTypeFromTypename {};
	template<> struct				TStreamCodeTypeFromTypename<hh_u8>		{ static const SVStreamCode::EType Type = SVStreamCode::Type_U8; };
	template<> struct				TStreamCodeTypeFromTypename<hh_i8>		{ static const SVStreamCode::EType Type = SVStreamCode::Type_I8; };
	template<> struct				TStreamCodeTypeFromTypename<hh_u16> 	{ static const SVStreamCode::EType Type = SVStreamCode::Type_U16; };
	template<> struct				TStreamCodeTypeFromTypename<hh_i16> 	{ static const SVStreamCode::EType Type = SVStreamCode::Type_I16; };
	template<> struct				TStreamCodeTypeFromTypename<hh_u32> 	{ static const SVStreamCode::EType Type = SVStreamCode::Type_U32; };
	template<> struct				TStreamCodeTypeFromTypename<hh_i32> 	{ static const SVStreamCode::EType Type = SVStreamCode::Type_I32; };
	template<> struct				TStreamCodeTypeFromTypename<hh_fp16>	{ static const SVStreamCode::EType Type = SVStreamCode::Type_F16; };
	template<> struct				TStreamCodeTypeFromTypename<float>		{ static const SVStreamCode::EType Type = SVStreamCode::Type_F32; };

	template<typename _Type>
	struct	TStreamCodeTypeHelper
	{
		static HH_FORCEINLINE bool	Equals(SVStreamCode::EType type, hh_u32 components) { return components == 1 && type == TStreamCodeTypeFromTypename<_Type>::Type; }
	};
	template<typename _Type, hh_u32 _Count>
	struct	TStreamCodeTypeHelper<TVector<_Type, _Count> >
	{
		static HH_FORCEINLINE bool	Equals(SVStreamCode::EType type, hh_u32 components) { return components == _Count && type == TStreamCodeTypeFromTypename<_Type>::Type; }
	};
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedMemoryView<_Type>	CMeshVStream::_AbstractStreamFromInternalIndexImpl(typename TTypeSwitcher_IfTrue<const CMeshVStream, CMeshVStream, TTypeTraits::IsConst<_Type>::True>::Type &vStream, CGuid index)
{
	typedef typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type	_DataBytes;
	HH_ASSERT(index.Valid() && index < vStream.m_Descriptor.m_VertexDeclaration.ElementCount());
	const hh_u32	vCount = vStream.m_Descriptor.m_VertexCount;
	if (vCount != 0)
	{
		const SVStreamCode	&vDecl = vStream.m_Descriptor.m_VertexDeclaration.StreamCodes()[index];
		if (MeshInternals::TStreamCodeTypeHelper<typename TType_RemoveConstness<_Type>::Type>::Equals(vDecl.Type(), vDecl.Components()))
		{
			_DataBytes	*streamData = reinterpret_cast<_DataBytes*>(reinterpret_cast<hh_ureg>(vStream.m_Stream16AndFlags) & ~0xF);
			HH_ASSERT(streamData != null);

			SVertexElementStreamDescriptor	desc;
			vStream.m_Descriptor.AbstractElementStreamDescriptorFromInternalIndex(index, desc);

			return TStridedMemoryView<_Type>(reinterpret_cast<_Type*>(streamData + desc.m_OffsetInBytes), vCount, desc.m_StrideInBytes);
		}
		else
			HH_ASSERT_NOT_REACHED();	// wrong format ! this stream doesn't match '_Type'
	}
	return TStridedMemoryView<_Type>();
}

//----------------------------------------------------------------------------

template<bool _Interleaved, typename _Type>
_Type		*CMeshVStream::_SkipStream(typename TTypeSwitcher_IfTrue<const CMeshVStream, CMeshVStream, TTypeTraits::IsConst<_Type>::True>::Type &vStream, hh_u32 elementId, _Type *buffer)
{
	typedef typename TTypeSwitcher_IfTrue<const hh_u8, hh_u8, TTypeTraits::IsConst<_Type>::True>::Type	_DataBytes;

	const SVertexDeclaration::TStreamCodesList	&elements = vStream.m_Descriptor.m_VertexDeclaration.StreamCodes();
	_DataBytes	*byteView = reinterpret_cast<_DataBytes*>(buffer);
	if (_Interleaved)
	{
		byteView += elements[elementId].ElementFootprintInBytes();	// jump to next dst stream
		if (elementId < elements.Count() - 1 && elements[elementId + 1].HasStride16())
			byteView = Mem::Align<0x10>(byteView);
	}
	else
		byteView += (elements[elementId].ElementFootprintInBytes() * vStream.m_Descriptor.m_MaxVertexCount + 0xF) & ~0xF;	// jump to next dst stream
	return reinterpret_cast<_Type*>(byteView);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_INLINE TStridedMemoryView<const _Type>	CMeshVStream::AbstractStream(hh_u32 ordinal) const
{
	const CGuid	id = FindAbstractStreamInternalIndex(ordinal);
	if (!id.Valid())
		return TStridedMemoryView<const _Type>();
	return AbstractStreamFromInternalIndex<_Type>(id);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_INLINE TStridedMemoryView<_Type>		CMeshVStream::AbstractStream(hh_u32 ordinal)
{
	const CGuid	id = FindAbstractStreamInternalIndex(ordinal);
	if (!id.Valid())
		return TStridedMemoryView<_Type>();
	return AbstractStreamFromInternalIndex<_Type>(id);
}

//----------------------------------------------------------------------------

template<typename _Type>
void	CMeshVStream::ImportAbstractStreamFromInternalIndex(CGuid index, const TStridedMemoryView<_Type> &src)
{
	typedef typename TType_RemoveConstness<_Type>::Type	_TypeForWriting;
	HH_ASSERT(index.Valid());
	TStridedMemoryView<_TypeForWriting>	dst = AbstractStreamFromInternalIndex<_TypeForWriting>(index);
	HH_ASSERT(dst.Count() == src.Count());
	const hh_u32		count = HHMin(dst.Count(), src.Count());	// just in case
	const SVStreamCode	&vDecl = m_Descriptor.m_VertexDeclaration.StreamCodes()[index];
	if (MeshInternals::TStreamCodeTypeHelper<typename TType_RemoveConstness<_Type>::Type>::Equals(vDecl.Type(), vDecl.Components()))
	{
		if (!Interleaved() && src.Stride() == dst.Stride())
		{
			// the stream is continuous, we can memcopy it in-place:
			Mem::Copy(dst.Data(), src.Data(), count * vDecl.ElementFootprintInBytes());
		}
		else
		{
			for (hh_u32 i = 0; i < count; i++)
			{
				dst[i] = src[i];
			}
		}
	}
	else
		HH_ASSERT_NOT_REACHED();
}

//----------------------------------------------------------------------------

template<typename _Type>
void	CMeshVStream::ImportAbstractStream(hh_u32 ordinal, const TStridedMemoryView<_Type> &src)
{
	const CGuid	id = FindAbstractStreamInternalIndex(ordinal);
	HH_ASSERT(id.Valid());
	if (id.Valid())
	{
		ImportAbstractStreamFromInternalIndex(id, src);
	}
}

//----------------------------------------------------------------------------
//
//	Index streams
//
//----------------------------------------------------------------------------

template<>
HH_FORCEINLINE const hh_u16	*CMeshIStream::Stream<hh_u16>() const
{
	HH_ASSERT(IndexByteWidth() == U16Indices);
	return static_cast<const hh_u16*>(m_Stream);
}

//----------------------------------------------------------------------------

template<>
HH_FORCEINLINE const hh_u32	*CMeshIStream::Stream<hh_u32>() const
{
	HH_ASSERT(IndexByteWidth() == U32Indices);
	return static_cast<const hh_u32*>(m_Stream);
}

//----------------------------------------------------------------------------

template<>
HH_FORCEINLINE hh_u16		*CMeshIStream::StreamForWriting<hh_u16>()
{
	HH_ASSERT(IndexByteWidth() == U16Indices);
	return static_cast<hh_u16*>(m_Stream);
}

//----------------------------------------------------------------------------

template<>
HH_FORCEINLINE hh_u32		*CMeshIStream::StreamForWriting<hh_u32>()
{
	HH_ASSERT(IndexByteWidth() == U32Indices);
	return static_cast<hh_u32*>(m_Stream);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE CGuid	CMeshIStream::MergeAndOffsetIndices(const TMemoryView<const hh_u16> &indexList, hh_u32 offset)
{
	return MergeAndOffsetIndices(&indexList[0], indexList.Count(), offset);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE CGuid	CMeshIStream::MergeAndOffsetIndices(const TMemoryView<const hh_u32> &indexList, hh_u32 offset)
{
	return MergeAndOffsetIndices(&indexList[0], indexList.Count(), offset);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE CGuid	CMeshIStream::MergeAndOffsetIndices(const hh_u16 *indexList, hh_u32 indexCount, hh_u32 offset)
{
	return MergeAndOffsetIndices(indexList, indexCount, offset, PrimitiveType());
}

//----------------------------------------------------------------------------

HH_FORCEINLINE CGuid	CMeshIStream::MergeAndOffsetIndices(const hh_u32 *indexList, hh_u32 indexCount, hh_u32 offset)
{
	return MergeAndOffsetIndices(indexList, indexCount, offset, PrimitiveType());
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	CMeshIStream::SetPrimitiveType(EPrimitive primitiveType)
{
	m_Flags = (m_Flags & (~Mask_PrimitiveType)) | (static_cast<hh_u32>(primitiveType) & Mask_PrimitiveType);
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	CMeshIStream::SetOptimizedPostTNLCacheSize(hh_u32 sizeInVertices)
{
	m_Flags &= ~(Mask_PostTNLCacheOptSize << Shift_PostTNLCacheOptSize);
	m_Flags |= HHMax(sizeInVertices, Mask_PostTNLCacheOptSize) << Shift_PostTNLCacheOptSize;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	CMeshIStream::SetLayoutOptimizedForOverdrawReduction(bool optimized)
{
	m_Flags &= ~Flag_LayoutOptimizedForOverdrawReduction;
	if (optimized)
	{
		m_Flags |= Flag_LayoutOptimizedForOverdrawReduction;
	}
}

//----------------------------------------------------------------------------

HH_FORCEINLINE void	CMeshIStream::SetIndexByteWidthUnsafe(EFormat newFormat)
{
	m_Flags &= ~Flag_U16Indices;
	if (newFormat == U16Indices)
		m_Flags |= Flag_U16Indices;
	else
		HH_ASSERT(newFormat == U32Indices);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __GE_MESH_INL__

