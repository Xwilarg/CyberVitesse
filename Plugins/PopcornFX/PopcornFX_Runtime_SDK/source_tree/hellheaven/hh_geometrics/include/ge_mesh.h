#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/08/01 07:34 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_MESH_H__
#define __GE_MESH_H__

#include "hh_maths/include/hh_maths_primitives.h"
#include "hh_kernel/include/kr_file.h"
#include "hh_kernel/include/kr_containers.h"
#include "hh_geometrics/include/ge_config.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CVStreamSemanticDictionnary
{
public:
	enum	EDefaultOrdinals
	{
		Ordinal_INVALID		= 0,
		Ordinal_Position	= 1,
		Ordinal_Normal		= 2,
		Ordinal_Tangent		= 3,
		Ordinal_Color		= 4,
		Ordinal_Texcoord	= 5,

		__MaxDefaultOrdinals
	};

	static const CString	&NameFromOrdinal(CGuid ordinal);
	static CGuid			NameToOrdinal(const CString &name);
	static CGuid			NameToOrdinalAndRegisterIFN(const CString &name);

	// helpers
	static CGuid			UvStreamToOrdinal(hh_u32 uvChannelId);			// "UvStream%d"
	static CGuid			ColorStreamToOrdinal(hh_u32 colorChannelId);	// "ColorStream%d"
};

//----------------------------------------------------------------------------
//
//	Vertex element declarator
//
//	the lower 8 bits are used to store packed information such as the element type, number of components, and alignment/packing flags
//	the upper 24 bits are used to store a name ordinal that can be used to retreive a string naming the stream (ex: "Position")
//	SN?TTTCC
//
//----------------------------------------------------------------------------

struct	HH_GEOMETRICS_EXPORT SVStreamCode
{
	enum	EType
	{
		Type_U8		= 0,
		Type_I8		= 1,
		Type_U16 	= 2,
		Type_I16 	= 3,
		Type_U32 	= 4,
		Type_I32 	= 5,
		Type_F16 	= 6,
		Type_F32 	= 7,

		__MaxTypes
	};

	enum	EElementType
	{
		Element_UByte1 = 0,	Element_UByte2,		Element_UByte3, 	Element_UByte4,		// U8
		Element_Byte1,		Element_Byte2,		Element_Byte3,		Element_Byte4,		// I8
		Element_UWord1,		Element_UWord2,		Element_UWord3,		Element_UWord4,		// U16
		Element_Word1,		Element_Word2,		Element_Word3,		Element_Word4,		// I16
		Element_UInt1,		Element_UInt2,		Element_UInt3,		Element_UInt4,		// U32
		Element_Int1,		Element_Int2,		Element_Int3,		Element_Int4,		// I32
		Element_Half1,		Element_Half2,		Element_Half3,		Element_Half4,		// F16
		Element_Float1,		Element_Float2,		Element_Float3,		Element_Float4,		// F32

		__MaxElementTypes
	};
	static hh_u32	ElementTypeComponentCount(EElementType eType) { return 1 + (eType & 3); }

	HH_STATIC_ASSERT(__MaxElementTypes == 4 * __MaxTypes);


	static const hh_u32	Mask_Components = 0x3;			// 2 bits -> 0-3 -> up to 4 dimensions
	static const hh_u32	Mask_Type = 0x7;				// 3 bits -> 0-7 -> 8 types: u8 i8 u16 i16 u32 i32 f16 f32
	static const hh_u32	Mask_NormalizedValue = 0x40;	// if set, the data represents normalized values between 0 and 1
	static const hh_u32	Mask_SIMDStride = 0x80;			// if set, the stride between each element will be 0x10 bytes
	static const hh_u32	Mask_NameOrdinal = 0xFFFFFF00;	// 24 bits -> 9-32

	static const hh_u32	Mask_SpecialFlags = Mask_NormalizedValue | Mask_SIMDStride;

	static const hh_u32	Shift_Type = 2;
	static const hh_u32	Shift_NameOrd = 8;

	static const hh_u32	Mask_ElementTypeLookupExtractor = Mask_Components | (Mask_Type << Shift_Type);

	static const hh_u8	ElementSizes[__MaxElementTypes];					// in bytes
	static const hh_u8	ElementMemoryFootprintWithSIMD[__MaxElementTypes];	// in bytes

	HH_STATIC_ASSERT(Mask_ElementTypeLookupExtractor == 0x1F);	// if this changes, we'll need to change the two lookups above
	HH_STATIC_ASSERT(__MaxTypes <= Mask_Type + 1);

	hh_u32	m_Code;

	enum	EFlags
	{
		SIMD_Friendly = Mask_SIMDStride,
		Normalized = Mask_NormalizedValue,
	};

	SVStreamCode() : m_Code(CVStreamSemanticDictionnary::Ordinal_INVALID << Shift_NameOrd) {}
	SVStreamCode(hh_u32 nameOrdinal, EType type, hh_u32 components, hh_u32 specialFlags = 0)
	:	m_Code((nameOrdinal << Shift_NameOrd) | (type << Shift_Type) | (components - 1) | (specialFlags & Mask_SpecialFlags))
	{
		HH_ASSERT(type >= 0 && type < __MaxTypes && components <= 4);
		HH_ASSERT((specialFlags & ~Mask_SpecialFlags) == 0);	// invalid flags
	}
	SVStreamCode(hh_u32 nameOrdinal, EElementType elementType, hh_u32 specialFlags = 0)
	:	m_Code((nameOrdinal << Shift_NameOrd) | elementType | (specialFlags & Mask_SpecialFlags))
	{
		HH_ASSERT(elementType >= 0 && elementType < __MaxElementTypes);
		HH_ASSERT((specialFlags & ~Mask_SpecialFlags) == 0);	// invalid flags
	}

	HH_FORCEINLINE hh_u32		Components() const { return (m_Code & Mask_Components) + 1; }
	HH_FORCEINLINE EType		Type() const { return (EType)((m_Code >> Shift_Type) & Mask_Type); }
	HH_FORCEINLINE EElementType	ElementType() const { return (EElementType)(m_Code & Mask_ElementTypeLookupExtractor); }
	HH_FORCEINLINE hh_u32		SpecialFlags() const { return m_Code & Mask_SpecialFlags; }
	HH_FORCEINLINE bool			HasStride16() const { return (m_Code & Mask_SIMDStride) != 0; }
	HH_FORCEINLINE bool			RepresentsNormalizedData() const { return (m_Code & Mask_NormalizedValue) != 0; }
	HH_FORCEINLINE hh_u32		NameOrdinal() const { return m_Code >> Shift_NameOrd; }
	HH_INLINE hh_u32			ElementSizeInBytes() const { return ElementSizes[ElementType()]; }
	HH_INLINE hh_u32			ElementFootprintInBytes() const { hh_u32 l = ElementType(); return HasStride16() ? ElementMemoryFootprintWithSIMD[l] : ElementSizes[l]; }

	void						SetStride16(bool active) { if (active) m_Code |= Mask_SIMDStride; else m_Code &= ~Mask_SIMDStride; }
	void						SetRepresentsNormalizedData(bool active) { if (active) m_Code |= Mask_NormalizedValue; else m_Code &= ~Mask_NormalizedValue; }
	void						SetNameOrdinal(hh_u32 ordinal) { m_Code = (m_Code & ~Mask_NameOrdinal) | (ordinal << Shift_NameOrd); }
	void						SetElementType(EElementType elementType) { m_Code = (m_Code & ~Mask_ElementTypeLookupExtractor) | elementType; }

	bool						Empty() const { return NameOrdinal() == CVStreamSemanticDictionnary::Ordinal_INVALID; }
	const CString				&GetSemanticName() const { return CVStreamSemanticDictionnary::NameFromOrdinal(NameOrdinal()); }
	HH_FORCEINLINE bool			operator == (const SVStreamCode &other) const { return m_Code == other.m_Code; }
	HH_FORCEINLINE bool			operator != (const SVStreamCode &other) const { return m_Code != other.m_Code; }
	HH_FORCEINLINE bool			operator > (const SVStreamCode &other) const { return m_Code > other.m_Code; }
	HH_FORCEINLINE bool			operator >= (const SVStreamCode &other) const { return m_Code >= other.m_Code; }
	HH_FORCEINLINE bool			operator < (const SVStreamCode &other) const { return m_Code < other.m_Code; }
	HH_FORCEINLINE bool			operator <= (const SVStreamCode &other) const { return m_Code <= other.m_Code; }
};

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT SVertexDeclaration
{
public:
	typedef	TSemiDynamicArray<SVStreamCode, 6>	TStreamCodesList;
private:
	TStreamCodesList	m_StreamCodes;	// eats up (6 + 1) * 4 = 28 bytes
	bool				_AreStreamsValid() const;
	bool				_InternalEqualTo(const SVertexDeclaration &other) const;

public:
	SVertexDeclaration() {}
	SVertexDeclaration(const TMemoryView<const SVStreamCode> &streamList);

	HH_FORCEINLINE const TStreamCodesList	&StreamCodes() const { return m_StreamCodes; }
	HH_FORCEINLINE TStreamCodesList			&StreamCodes() { return m_StreamCodes; }	// FIXME: REMOVE THIS! totally unsafe. external code shouldn't have direct access to these

	HH_FORCEINLINE hh_u32					ElementCount() const { return m_StreamCodes.Count(); }
	HH_FORCEINLINE void						Clear() { m_StreamCodes.Clear(); }
	HH_FORCEINLINE bool						Empty() const { return m_StreamCodes.Empty(); }
	HH_FORCEINLINE bool						operator == (const SVertexDeclaration &other) const { return this == &other || _InternalEqualTo(other); }
	HH_FORCEINLINE bool						operator != (const SVertexDeclaration &other) const { return !operator == (other); }

	bool	Combine(const SVertexDeclaration &other);			// will return false if there is are mismatching doublons
	bool	AddStreamCodeIFN(const SVStreamCode &streamCode);	// will return false if the stream code is already here, but doesn't match the one passed as input

	bool	AreStreamsMissingFromDeclaration(const SVertexDeclaration &otherDecl) const;
	bool	GetStreamsMissingFromDeclaration(const SVertexDeclaration &otherDecl, SVertexDeclaration &outMissingStreams) const;
	void	GetCommonDeclaration(const SVertexDeclaration &otherDecl, SVertexDeclaration &outCommonDecl) const;
	void	GetCombinedDeclaration(const SVertexDeclaration &otherDecl, SVertexDeclaration &outCommonDecl) const;

	CGuid	FindAbstractStreamInternalIndex(hh_u32 ordinal) const;
};

//----------------------------------------------------------------------------

struct	SVertexElementStreamDescriptor
{
	hh_u32	m_OffsetInBytes;
	hh_u32	m_StrideInBytes;
};

//----------------------------------------------------------------------------


class	HH_GEOMETRICS_EXPORT CVertexStreamDescriptor
{
private:
public:
	static const hh_u32			InternalFlag_Interleaved	= 0x00000001;
	static const hh_u32			InternalMask_Ordinals		= 0xFFFFFFFE & ((1 << CVStreamSemanticDictionnary::__MaxDefaultOrdinals) - 1);
	HH_STATIC_ASSERT(CVStreamSemanticDictionnary::__MaxDefaultOrdinals < 32 && (((1 << CVStreamSemanticDictionnary::__MaxDefaultOrdinals) - 1) & ~InternalFlag_Interleaved) <= InternalMask_Ordinals);
	HH_STATIC_ASSERT(CVStreamSemanticDictionnary::Ordinal_INVALID == 0);	// we rely on this to store the interleaved flag in bit 0

	CVertexStreamDescriptor() : m_MaxVertexCount(0), m_VertexCount(0), m_VertexSize(0), m_PackedFlags(0) {}
	~CVertexStreamDescriptor () {}

	SVertexDeclaration			m_VertexDeclaration;
	hh_u32						m_MaxVertexCount;
	hh_u32						m_VertexCount;
	hh_u32						m_VertexSize;	// if interleaved, contains the real vertex size. if streamed, contains the total VB size
	hh_u32						m_PackedFlags;

	HH_FORCEINLINE bool			Empty() const { return m_VertexCount == 0; }
	HH_FORCEINLINE bool			Interleaved() const { return (m_PackedFlags & InternalFlag_Interleaved) != 0; }
	HH_FORCEINLINE hh_u32		StreamSize() const { return Interleaved() ? m_VertexSize * m_MaxVertexCount : m_VertexSize; }

	void						Setup(const SVertexDeclaration &vDecl, bool interleaved, hh_u32 maxCount);
	void						Setup(const TMemoryView<const SVStreamCode> &vDecl, bool interleaved, hh_u32 maxCount);
	void						Setup_MaxCount(hh_u32 maxCount);
	void						Setup_Interleaved(bool interleaved);

	bool						HasDefaultStreamsCombination(hh_u32 combinedDefaultStreams) const { HH_ASSERT((combinedDefaultStreams & InternalMask_Ordinals) == combinedDefaultStreams); return (m_PackedFlags & (combinedDefaultStreams & InternalMask_Ordinals)) == combinedDefaultStreams; }

	template<CVStreamSemanticDictionnary::EDefaultOrdinals _Ordinal> bool	Has() const;
	bool						Has(hh_u32 ordinal) const;

	template<CVStreamSemanticDictionnary::EDefaultOrdinals _Ordinal>
	CGuid						FindDefaultStreamInternalIndex() const;
	CGuid						FindAbstractStreamInternalIndex(hh_u32 ordinal) const;

	SVStreamCode				AbstractElementDeclarationFromInternalIndex(CGuid index) const;
	SVStreamCode				AbstractElementDeclaration(hh_u32 ordinal) const;

	void						AbstractElementStreamDescriptorFromInternalIndex(CGuid index, SVertexElementStreamDescriptor &outDesc) const;


	void	Swap(CVertexStreamDescriptor &other)
	{
		HHSwap(m_VertexDeclaration, other.m_VertexDeclaration);
		HHSwap(m_MaxVertexCount, other.m_MaxVertexCount);
		HHSwap(m_VertexCount, other.m_VertexCount);
		HHSwap(m_PackedFlags, other.m_PackedFlags);
		HHSwap(m_VertexSize, other.m_VertexSize);
	}
};

HH_FORCEINLINE void	HHSwap(CVertexStreamDescriptor &vDesc0, CVertexStreamDescriptor &vDesc1)
{
	vDesc0.Swap(vDesc1);
}

//----------------------------------------------------------------------------
//
//	Main vertex stream class
//
//	we need to store an array of vertex declarations.
//	however, for quick search inside it, we'll also store an u32 containing a bitfield
//	with one bit for each predefined element in 'EDefaultOrdinals',
//	allowing us to quickly know if one of these commonly-used vertex elements is present.
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CMeshVStream
{
private:
	static const hh_u32								InternalFlag_Locked = 0x00000001;

	void											*m_Stream16AndFlags;
	CVertexStreamDescriptor							m_Descriptor;

	template<typename _Type>
	static TStridedMemoryView<_Type>				_AbstractStreamFromInternalIndexImpl(typename TTypeSwitcher_IfTrue<const CMeshVStream, CMeshVStream, TTypeTraits::IsConst<_Type>::True>::Type &vStream, CGuid index);
	template<bool _Interleaved, typename _Type>
	static _Type									*_SkipStream(typename TTypeSwitcher_IfTrue<const CMeshVStream, CMeshVStream, TTypeTraits::IsConst<_Type>::True>::Type &vStream, hh_u32 elementId, _Type *buffer);
	template<bool _SrcInterleaved, bool _DstInterleaved>
	void											_CopyVertexRange(const CMeshVStream &src, hh_u32 srcIndex, CMeshVStream &dst, hh_u32 dstIndex, hh_u32 vertexCount);

protected:
	bool											_RawStreamResize(hh_u32 newVertexCount);	// only touches m_MaxVertexCount, the caller should take care of setting m_VertexCount manually
	bool											_GrowIFN(hh_u32 newVertexCount);			// calls _RawStreamResize, the same holds form m_MaxVertexCount and m_VertexCount

public:
	CMeshVStream();
	CMeshVStream(const CMeshVStream &source);
	~CMeshVStream();
	CMeshVStream									&operator = (const CMeshVStream &source);

	HH_FORCEINLINE hh_u32							VertexCount() const { return m_Descriptor.m_VertexCount; }
	HH_FORCEINLINE hh_u32							MaxVertexCount() const { return m_Descriptor.m_MaxVertexCount; }
	HH_FORCEINLINE hh_u32							VertexSize() const { return m_Descriptor.m_VertexSize; }
	HH_FORCEINLINE hh_u32							StreamSize() const { return Interleaved() ? m_Descriptor.m_VertexSize * m_Descriptor.m_MaxVertexCount : m_Descriptor.m_VertexSize; }

	HH_FORCEINLINE const hh_u8						*RawStream() const { return reinterpret_cast<const hh_u8*>(reinterpret_cast<hh_ureg>(m_Stream16AndFlags) & ~static_cast<hh_ureg>(0xF)); }
	HH_FORCEINLINE hh_u8							*RawStreamForWriting() { return reinterpret_cast<hh_u8*>(reinterpret_cast<hh_ureg>(m_Stream16AndFlags) & ~static_cast<hh_ureg>(0xF)); }
	HH_FORCEINLINE const CVertexStreamDescriptor	&VertexStreamDescriptor() const { return m_Descriptor; }
	HH_FORCEINLINE const SVertexDeclaration			&VertexDeclaration() const { return m_Descriptor.m_VertexDeclaration; }


	HH_FORCEINLINE bool								Interleaved() const { return m_Descriptor.Interleaved(); }
	HH_FORCEINLINE bool								Empty() const { HH_ASSERT(RawStream() != null || m_Descriptor.m_VertexCount == 0); return m_Descriptor.m_VertexCount == 0; }

	// be careful with Reformat(), it costs quite a lot, and performs a full re-copy of the vertex stream into a new buffer
	bool											Reformat(const SVertexDeclaration &vertexDeclaration, bool interleaved);	// the order is not guaranteed to be kept: vertex elements might get reordered...
	bool											Reformat(const SVertexDeclaration &vertexDeclaration) { return Reformat(vertexDeclaration, Interleaved()); }
	bool											Reformat(const TMemoryView<const SVStreamCode> &vertexDeclaration, bool interleaved);	// the order is not guaranteed to be kept: vertex elements might get reordered...
	bool											Reformat(const TMemoryView<const SVStreamCode> &vertexDeclaration) { return Reformat(vertexDeclaration, Interleaved()); }
	bool											ReformatStreamAndAddIFN(const SVStreamCode &vertexElement);
	bool											ReformatStreamAndAddIFN(const TMemoryView<const SVStreamCode> &vertexElements);
	bool											ReformatLayout(bool interleaved);	// FIXME: enum?
	bool											ReformatFromCommonDeclaration(const CMeshVStream &stream0, const CMeshVStream &stream1, bool interleaved = false);
	bool											ReformatFromCombinedDeclaration(const CMeshVStream &stream0, const CMeshVStream &stream1, bool interleaved = false);

	bool											Reserve(hh_u32 newVertexCount);	// only touches m_MaxVertexCount, the caller should take care of setting m_VertexCount manually
	HH_INLINE bool									Resize(hh_u32 newVertexCount) { if (_GrowIFN(newVertexCount)) { m_Descriptor.m_VertexCount = newVertexCount; return true; } return false; }
	bool											ExactResize(hh_u32 newVertexCount);
	HH_FORCEINLINE bool								Condense() { return ExactResize(m_Descriptor.m_VertexCount); }
	HH_FORCEINLINE void								Clear() { m_Descriptor.m_VertexCount = 0; }
	HH_FORCEINLINE void								Clean() { ExactResize(0); }	// frees all memory
	HH_INLINE CGuid									AllocateAdditionalDataForWriting(hh_u32 neededVertexCount);

	CGuid											Merge(const CMeshVStream &mergeWith);
	CGuid											MergeAndTransformVertices(const CMeshVStream &mergeWith, const CFloat4x4 &transforms);
	void											TransformVertices(hh_u32 firstVertexToTransform, hh_u32 numberOfVerticesToTransform, const CFloat4x4 &transforms);

	// begin wrappers
	template<CVStreamSemanticDictionnary::EDefaultOrdinals _Ordinal>
	HH_FORCEINLINE bool								Has() const { return m_Descriptor.Has<_Ordinal>(); }
	HH_FORCEINLINE bool								Has(hh_u32 ordinal) const { return m_Descriptor.Has(ordinal); }
	// end wrappers

	TStridedMemoryView<const CFloat3>				Positions() const;
	TStridedMemoryView<CFloat3>						Positions();
	TStridedMemoryView<const CFloat3>				Normals() const;
	TStridedMemoryView<CFloat3>						Normals();
	TStridedMemoryView<const CFloat4>				Tangents() const;
	TStridedMemoryView<CFloat4>						Tangents();
	TStridedMemoryView<const CFloat4>				Colors() const;
	TStridedMemoryView<CFloat4>						Colors();
	TStridedMemoryView<const CFloat2>				Texcoords() const;
	TStridedMemoryView<CFloat2>						Texcoords();

	// begin wrappers
	template<CVStreamSemanticDictionnary::EDefaultOrdinals _Ordinal>
	HH_FORCEINLINE CGuid							FindDefaultStreamInternalIndex() const { return m_Descriptor.FindDefaultStreamInternalIndex<_Ordinal>(); }
	HH_FORCEINLINE CGuid							FindAbstractStreamInternalIndex(hh_u32 ordinal) const { return m_Descriptor.FindAbstractStreamInternalIndex(ordinal); }

	SVStreamCode									AbstractElementDeclarationFromInternalIndex(CGuid index) const { return m_Descriptor.AbstractElementDeclarationFromInternalIndex(index); }
	HH_INLINE SVStreamCode							AbstractElementDeclaration(hh_u32 ordinal) const { return m_Descriptor.AbstractElementDeclaration(ordinal); }
	// end wrappers

	const void										*AbstractUntypedStreamFromInternalIndex(CGuid index, hh_u32 &outStride, hh_u32 &outCount) const;
	void											*AbstractUntypedStreamFromInternalIndex(CGuid index, hh_u32 &outStride, hh_u32 &outCount);
	template<typename _Type>
	HH_FORCEINLINE TStridedMemoryView<const _Type>	AbstractStreamFromInternalIndex(CGuid index) const { return _AbstractStreamFromInternalIndexImpl<const _Type>(*this, index); }
	template<typename _Type>
	HH_FORCEINLINE TStridedMemoryView<_Type>		AbstractStreamFromInternalIndex(CGuid index) { return _AbstractStreamFromInternalIndexImpl<_Type>(*this, index); }
	template<typename _Type>
	HH_INLINE TStridedMemoryView<const _Type>		AbstractStream(hh_u32 ordinal) const;
	template<typename _Type>
	HH_INLINE TStridedMemoryView<_Type>				AbstractStream(hh_u32 ordinal);

	void											ImportPositions(const TStridedMemoryView<const CFloat3> &srcPositions) { ImportAbstractStream(CVStreamSemanticDictionnary::Ordinal_Position, srcPositions); }
	void											ImportNormals(const TStridedMemoryView<const CFloat3> &srcNormals) { ImportAbstractStream(CVStreamSemanticDictionnary::Ordinal_Normal, srcNormals); }
	void											ImportTangents(const TStridedMemoryView<const CFloat4> &srcTangents) { ImportAbstractStream(CVStreamSemanticDictionnary::Ordinal_Tangent, srcTangents); }
	void											ImportColors(const TStridedMemoryView<const CFloat4> &srcColors) { ImportAbstractStream(CVStreamSemanticDictionnary::Ordinal_Color, srcColors); }
	void											ImportTexcoords(const TStridedMemoryView<const CFloat2> &srcTexcoords) { ImportAbstractStream(CVStreamSemanticDictionnary::Ordinal_Texcoord, srcTexcoords); }

	void											ImportAbstractStreamFromInternalIndexAndRawData(CGuid index, const void *src, hh_u32 stride, hh_u32 elementSizeInBytes, hh_u32 elementCount);	// expects 'index' to be valid
	template<typename _Type>
	void											ImportAbstractStreamFromInternalIndex(CGuid index, const TStridedMemoryView<_Type> &src);	// expects 'index' to be valid
	template<typename _Type>
	void											ImportAbstractStream(hh_u32 ordinal, const TStridedMemoryView<_Type> &src);	// expects 'ordinal' to be valid

	void											CopyVertexRange(hh_u32 indexFrom, hh_u32 indexTo, hh_u32 vertexCount);
	void											CopyVertexRange(const CMeshVStream &fromOther, hh_u32 indexFrom, hh_u32 indexTo, hh_u32 vertexCount);

	void											Lock() { HH_ASSERT(!Locked()); m_Stream16AndFlags = reinterpret_cast<void*>(reinterpret_cast<hh_ureg>(m_Stream16AndFlags) | static_cast<hh_ureg>(InternalFlag_Locked)); }
	void											Unlock() { HH_ASSERT(Locked()); m_Stream16AndFlags = reinterpret_cast<void*>(reinterpret_cast<hh_ureg>(m_Stream16AndFlags) & ~static_cast<hh_ureg>(InternalFlag_Locked)); }
	bool											Locked() const { return (reinterpret_cast<hh_ureg>(m_Stream16AndFlags) & static_cast<hh_ureg>(InternalFlag_Locked)) != 0; }

	// unsafe accessors...
	// BE CAREFUL IF YOU USE THESE! they expose some of the internal workings of the vertex stream class, might change, and are totally unsafe if you don't know what you're doing!
	void											SetVertexCount_Unsafe(hh_u32 newVertexCount) { m_Descriptor.m_VertexCount = newVertexCount; m_Descriptor.m_MaxVertexCount = newVertexCount; }
	void											SetAbstractStream_Unsafe(void *stream)
	{
		HH_ASSERT_ALIGNED16(stream);
		m_Stream16AndFlags = reinterpret_cast<void*>((reinterpret_cast<hh_ureg>(m_Stream16AndFlags) & 0xF) | reinterpret_cast<hh_ureg>(stream));
	}

	void											Swap(CMeshVStream &other)
	{
		HHSwap(m_Stream16AndFlags, other.m_Stream16AndFlags);
		HHSwap(m_Descriptor, other.m_Descriptor);
	}
};

HH_FORCEINLINE void	HHSwap(CMeshVStream &vStream0, CMeshVStream &vStream1)
{
	vStream0.Swap(vStream1);
}

//----------------------------------------------------------------------------
//
//	Index streams
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CMeshIStream
{
public:
	enum	EFormat
	{
		U16Indices = 2,
		U32Indices = 4,
	};

	enum	EPrimitive
	{
		TriangleStrips = 0,
		Triangles,
		Lines,
		__MaxPrimitiveTypes,
	};

private:
	void						*m_Stream;
	hh_u32						m_IndexCount;
	hh_u32						m_MaxIndexCount;
	hh_u32						m_Flags;

	static const hh_u32			Mask_PrimitiveType = 0x3;			// 2 bits
	static const hh_u32			Flag_U16Indices = (1 << 2);			// 1 bit
	static const hh_u32			Shift_PostTNLCacheOptSize = 3;
	static const hh_u32			Mask_PostTNLCacheOptSize = 0x3F;	// 6 bits
	static const hh_u32			Flag_LayoutOptimizedForOverdrawReduction = 1 << (Shift_PostTNLCacheOptSize + 6);

	HH_STATIC_ASSERT(((__MaxPrimitiveTypes - 1) & Mask_PrimitiveType) == (__MaxPrimitiveTypes - 1));
	HH_STATIC_ASSERT(__MaxPrimitiveTypes == 3);	// if this is changed, offset the flags above

protected:
	bool						_RawStreamResize(hh_u32 newIndexCount);	// only touches m_MaxIndexCount, the caller should take care of setting m_IndexCount manually
	bool						_GrowIFN(hh_u32 newIndexCount);			// calls _RawStreamResize, the same holds form m_MaxIndexCount and m_IndexCount

public:
	CMeshIStream() : m_Stream(null), m_IndexCount(0), m_MaxIndexCount(0), m_Flags(0) {}
	CMeshIStream(const CMeshIStream &source);
	~CMeshIStream();
	CMeshIStream				&operator = (const CMeshIStream &source);

	bool						OptimizeStorage(const CMeshVStream &vStream);	// only uses the vertex count
	bool						OptimizeStorage();	// scans the index list and takes the highest index to determine a need for a switch from u32 to u16 indices
	bool						Reformat(CMeshIStream::EFormat newFormat);
	HH_FORCEINLINE void			ResetFormat() { m_Flags = 0; }

	bool						Reserve(hh_u32 newIndexCount);	// only touches m_MaxVertexCount, the caller should take care of setting m_VertexCount manually
	HH_INLINE bool				Resize(hh_u32 newIndexCount) { if (_GrowIFN(newIndexCount)) { m_IndexCount = newIndexCount; return true; } return false; }
	bool						ExactResize(hh_u32 newIndexCount);	// only touches m_MaxVertexCount, the caller should take care of setting m_VertexCount manually
	HH_FORCEINLINE void			Clear() { m_IndexCount = 0; }
	HH_FORCEINLINE void			Clean() { ExactResize(0); }	// frees memory
	HH_FORCEINLINE bool			Empty() const { HH_ASSERT(m_Stream != null || m_IndexCount == 0); return m_IndexCount == 0; }
	HH_FORCEINLINE hh_u32		IndexCount() const { return m_IndexCount; }
	HH_FORCEINLINE hh_u32		StreamSize() const { return m_IndexCount * IndexByteWidth(); }

	template<typename _IndexType> const _IndexType	*Stream() const { HH_STATIC_ASSERT_NOT_REACHED_CUSTOM("StreamFormat has to be either hh_u16 or hh_u32"); }
	template<typename _IndexType> _IndexType		*StreamForWriting() { HH_STATIC_ASSERT_NOT_REACHED_CUSTOM("StreamFormat has to be either hh_u16 or hh_u32"); }

	HH_FORCEINLINE const void	*RawStream() const { return m_Stream; }
	HH_FORCEINLINE void			*RawStreamForWriting() { return m_Stream; }
	CGuid						AllocateAdditionalDataForWriting(hh_u32 neededIndexCount);

	HH_FORCEINLINE EFormat		IndexByteWidth() const { return (EFormat)((m_Flags & Flag_U16Indices) ? U16Indices : U32Indices); }
	HH_FORCEINLINE EPrimitive	PrimitiveType() const { return (EPrimitive)(m_Flags & Mask_PrimitiveType); }
	void						SetPrimitiveType(EPrimitive primitiveType);

	hh_u32						OptimizedPostTNLCacheSize() const { return (m_Flags >> Shift_PostTNLCacheOptSize) & Mask_PostTNLCacheOptSize; }
	void						SetOptimizedPostTNLCacheSize(hh_u32 sizeInVertices);
	bool						LayoutOptimizedForOverdrawReduction() const { return (m_Flags & Flag_LayoutOptimizedForOverdrawReduction) != 0; }
	void						SetLayoutOptimizedForOverdrawReduction(bool optimized);

	hh_u32						LargestIndex() const;
	hh_u32						SmallestIndex() const;
	void						TriangleIndexToVertexIndices(hh_u32 triangle, hh_u32 ids[3]) const;

	CGuid						Merge(const CMeshIStream &mergeWith);
	CGuid						MergeAndOffsetIndices(const CMeshIStream &mergeWith, hh_u32 offset);
	CGuid						MergeAndOffsetIndices(const hh_u16 *indexList, hh_u32 indexCount, hh_u32 offset);
	CGuid						MergeAndOffsetIndices(const hh_u32 *indexList, hh_u32 indexCount, hh_u32 offset);
	CGuid						MergeAndOffsetIndices(const hh_u16 *indexList, hh_u32 indexCount, hh_u32 offset, const EPrimitive);
	CGuid						MergeAndOffsetIndices(const hh_u32 *indexList, hh_u32 indexCount, hh_u32 offset, const EPrimitive);
	CGuid						MergeAndOffsetIndices(const TMemoryView<const hh_u16> &indexList, hh_u32 offset);
	CGuid						MergeAndOffsetIndices(const TMemoryView<const hh_u32> &indexList, hh_u32 offset);

	// set cacheSize to 0 for auto-detection
//	void						OptimizeForPostTNLCache(bool reduceOverdraw = false, hh_u32 cacheSize = 0);

	// unsafe accessors
	void						SetIndexByteWidthUnsafe(EFormat newFormat);

	void						SetAbstractStream_Unsafe(void *stream) { m_Stream = stream; }
	void						SetIndexCount_Unsafe(hh_u32 indexCount) { m_IndexCount = indexCount; m_MaxIndexCount = indexCount; }
	hh_u32						RawFormat_Unsafe() const { return m_Flags; }
	void						SetRawFormat_Unsafe(hh_u32 iFormat) { m_Flags = iFormat; }

	void						Swap(CMeshIStream &other)
	{
		HHSwap(m_Stream, other.m_Stream);
		HHSwap(m_IndexCount, other.m_IndexCount);
		HHSwap(m_MaxIndexCount, other.m_MaxIndexCount);
		HHSwap(m_Flags, other.m_Flags);
	}

	static void					FlipHandedness(void *indexData, hh_u32 indexCount, EFormat indexFormat, EPrimitive primitiveType);
	void						FlipHandedness() { FlipHandedness(m_Stream, m_IndexCount, IndexByteWidth(), PrimitiveType()); }
};

//----------------------------------------------------------------------------

HH_FORCEINLINE void	HHSwap(CMeshIStream &vStream0, CMeshIStream &vStream1)
{
	vStream0.Swap(vStream1);
}

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CMeshTriangleBatch
{
private:
protected:
public:
	CMeshVStream	m_VStream;
	CMeshIStream	m_IStream;

	CMeshTriangleBatch();
	CMeshTriangleBatch(const CMeshTriangleBatch &batch);
	CMeshTriangleBatch(CMeshIStream::EFormat indexFormat, CMeshIStream::EPrimitive primitiveType, const TMemoryView<const SVStreamCode> &vertexFormat, bool interleaved = false);
	CMeshTriangleBatch(CMeshIStream::EFormat indexFormat, CMeshIStream::EPrimitive primitiveType, const SVertexDeclaration &vertexDeclaration, bool interleaved = false);

	~CMeshTriangleBatch();

	void			operator = (const CMeshTriangleBatch &source);

	HH_INLINE bool	Empty() const { return m_VStream.Empty() || m_IStream.Empty(); }
	HH_INLINE bool	AllEmpty() const { return m_VStream.Empty() && m_IStream.Empty(); }

	void			Clear() { m_VStream.Clear(); m_IStream.Clear(); }
	void			Clean() { m_VStream.Clean(); m_IStream.Clean(); }
	bool			OptimizeIndexStorage() { return m_IStream.OptimizeStorage(m_VStream); }

	// we should be able to return both the index of the first vertex and first index, but in practise, we'll probably need the first vertex more often...
	// returns the index of the first merged vertex. if succeeded, == this->m_VStream.VertexCount() before the merge
	CGuid			Merge(const CMeshTriangleBatch &mergeWith);
	CGuid			Merge(const CMeshTriangleBatch &mergeWith, const CFloat4x4 &transforms);

	hh_u32			PrimitiveCount() const;		// if a triangle mesh, returns the number of triangles, if a line batch, the number of lines

	void			Swap(CMeshTriangleBatch &other);
};

//----------------------------------------------------------------------------

HH_FORCEINLINE void	HHSwap(CMeshTriangleBatch &batch0, CMeshTriangleBatch &batch1)
{
	batch0.Swap(batch1);
}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_geometrics/include/ge_mesh.inl"

#endif // __GE_MESH_H__

