#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2008/10/04 22:53 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_BASE_TYPES_H__
#define __KR_BASE_TYPES_H__

#include "hh_maths/include/hh_maths_vector.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	ACHTUNG! unless you know exactly what you're doing, do _NOT_ change anything in this file.
//
//----------------------------------------------------------------------------
// this enum is ordered by type 'quality'. the order will be used during type promotion (mostly in kr_compiler)

enum	EBaseTypeID
{
	BaseType_Evolved = -1,	// this must not change, some unsigned hacks use -1 (see below)
	BaseType_Void = 0,
	BaseType_Bool,
	BaseType_U8,
	BaseType_UByte2,
	BaseType_UByte3,
	BaseType_UByte4,
	BaseType_I8,
	BaseType_Byte2,
	BaseType_Byte3,
	BaseType_Byte4,
	BaseType_U16,
	BaseType_UWord2,
	BaseType_UWord3,
	BaseType_UWord4,
	BaseType_I16,
	BaseType_Word2,
	BaseType_Word3,
	BaseType_Word4,
	BaseType_U32,
	BaseType_UInt2,
	BaseType_UInt3,
	BaseType_UInt4,
	BaseType_I32,
	BaseType_Int2,
	BaseType_Int3,
	BaseType_Int4,
	BaseType_U64,
	BaseType_I64,
	BaseType_Float,
	BaseType_Float2,
	BaseType_Float3,
	BaseType_Float4,
	BaseType_Double,
	BaseType_Quaternion,

	__MaxBaseTypes
};

//----------------------------------------------------------------------------

template<typename _Type>
struct	TBaseTypeID;

//----------------------------------------------------------------------------

template<typename _Type>	struct	TBaseTypeID					{ static const EBaseTypeID TypeID = BaseType_Evolved; };
template<>					struct	TBaseTypeID<void> 			{ static const EBaseTypeID TypeID = BaseType_Void; };
template<>					struct	TBaseTypeID<bool> 			{ static const EBaseTypeID TypeID = BaseType_Bool; };
template<>					struct	TBaseTypeID<hh_u8>			{ static const EBaseTypeID TypeID = BaseType_U8; };
template<>					struct	TBaseTypeID<CUbyte1> 		{ static const EBaseTypeID TypeID = BaseType_U8; };		// hum
template<>					struct	TBaseTypeID<CUbyte2> 		{ static const EBaseTypeID TypeID = BaseType_UByte2; };
template<>					struct	TBaseTypeID<CUbyte3> 		{ static const EBaseTypeID TypeID = BaseType_UByte3; };
template<>					struct	TBaseTypeID<CUbyte4> 		{ static const EBaseTypeID TypeID = BaseType_UByte4; };
template<>					struct	TBaseTypeID<hh_i8>			{ static const EBaseTypeID TypeID = BaseType_I8; };
template<>					struct	TBaseTypeID<CByte1> 		{ static const EBaseTypeID TypeID = BaseType_I8; };		// hum
template<>					struct	TBaseTypeID<CByte2> 		{ static const EBaseTypeID TypeID = BaseType_Byte2; };
template<>					struct	TBaseTypeID<CByte3> 		{ static const EBaseTypeID TypeID = BaseType_Byte3; };
template<>					struct	TBaseTypeID<CByte4> 		{ static const EBaseTypeID TypeID = BaseType_Byte4; };
template<>					struct	TBaseTypeID<hh_u16> 		{ static const EBaseTypeID TypeID = BaseType_U16; };
template<>					struct	TBaseTypeID<CUword1> 		{ static const EBaseTypeID TypeID = BaseType_U16; };	// hum
template<>					struct	TBaseTypeID<CUword2> 		{ static const EBaseTypeID TypeID = BaseType_UWord2; };
template<>					struct	TBaseTypeID<CUword3> 		{ static const EBaseTypeID TypeID = BaseType_UWord3; };
template<>					struct	TBaseTypeID<CUword4> 		{ static const EBaseTypeID TypeID = BaseType_UWord4; };
template<>					struct	TBaseTypeID<hh_i16> 		{ static const EBaseTypeID TypeID = BaseType_I16; };
template<>					struct	TBaseTypeID<CWord1> 		{ static const EBaseTypeID TypeID = BaseType_I16; };	// hum
template<>					struct	TBaseTypeID<CWord2> 		{ static const EBaseTypeID TypeID = BaseType_Word2; };
template<>					struct	TBaseTypeID<CWord3> 		{ static const EBaseTypeID TypeID = BaseType_Word3; };
template<>					struct	TBaseTypeID<CWord4> 		{ static const EBaseTypeID TypeID = BaseType_Word4; };
template<>					struct	TBaseTypeID<hh_u32> 		{ static const EBaseTypeID TypeID = BaseType_U32; };
template<>					struct	TBaseTypeID<CUint1> 		{ static const EBaseTypeID TypeID = BaseType_U32; };	// hum
template<>					struct	TBaseTypeID<CUint2> 		{ static const EBaseTypeID TypeID = BaseType_UInt2; };
template<>					struct	TBaseTypeID<CUint3> 		{ static const EBaseTypeID TypeID = BaseType_UInt3; };
template<>					struct	TBaseTypeID<CUint4> 		{ static const EBaseTypeID TypeID = BaseType_UInt4; };
template<>					struct	TBaseTypeID<hh_i32> 		{ static const EBaseTypeID TypeID = BaseType_I32; };
template<>					struct	TBaseTypeID<CInt1> 			{ static const EBaseTypeID TypeID = BaseType_I32; };	// hum
template<>					struct	TBaseTypeID<CInt2> 			{ static const EBaseTypeID TypeID = BaseType_Int2; };
template<>					struct	TBaseTypeID<CInt3> 			{ static const EBaseTypeID TypeID = BaseType_Int3; };
template<>					struct	TBaseTypeID<CInt4> 			{ static const EBaseTypeID TypeID = BaseType_Int4; };
template<>					struct	TBaseTypeID<hh_u64> 		{ static const EBaseTypeID TypeID = BaseType_U64; };
template<>					struct	TBaseTypeID<hh_i64> 		{ static const EBaseTypeID TypeID = BaseType_I64; };
template<>					struct	TBaseTypeID<float>			{ static const EBaseTypeID TypeID = BaseType_Float; };
template<>					struct	TBaseTypeID<CFloat1>		{ static const EBaseTypeID TypeID = BaseType_Float; };	// hum
template<>					struct	TBaseTypeID<CFloat2>		{ static const EBaseTypeID TypeID = BaseType_Float2; };
template<>					struct	TBaseTypeID<CFloat3>		{ static const EBaseTypeID TypeID = BaseType_Float3; };
template<>					struct	TBaseTypeID<CFloat4>		{ static const EBaseTypeID TypeID = BaseType_Float4; };
template<>					struct	TBaseTypeID<double>			{ static const EBaseTypeID TypeID = BaseType_Double; };
template<>					struct	TBaseTypeID<CQuaternion>	{ static const EBaseTypeID TypeID = BaseType_Quaternion; };

template<typename _Type>	struct	TBaseTypeID<_Type const>	: public TBaseTypeID<_Type> {};
template<typename _Type>	struct	TBaseTypeID<_Type volatile>	: public TBaseTypeID<_Type> {};

//----------------------------------------------------------------------------

template<typename _Type>	struct	IsBaseType : TTypeBoolConstant<TBaseTypeID<_Type>::TypeID >= 0> {};
template<typename _Type>	struct	IsBaseType<_Type const> : IsBaseType<_Type> {};	// needed if we want const pointers to work

//----------------------------------------------------------------------------

template<typename _Type>	struct	TBaseTypeTraits_Size		{ static const hh_u32 Size = sizeof(_Type); };
template<>					struct	TBaseTypeTraits_Size<void>	{ static const hh_u32 Size = 0; };

//----------------------------------------------------------------------------

template<typename _Type>	struct	TBaseTypeTraits_ScalarFpPromoter			{ typedef float		PromotedType; };
template<>					struct	TBaseTypeTraits_ScalarFpPromoter<double>	{ typedef double	PromotedType; };
template<>					struct	TBaseTypeTraits_ScalarFpPromoter<void>		{ typedef void		PromotedType; };

//----------------------------------------------------------------------------

template<typename _Type>
struct	TBaseTypeTraits_FpPromoter
{
	typedef typename TBaseTypeTraits_ScalarFpPromoter<_Type>::PromotedType	FpPromotedType;
};

template<typename _Type, hh_u32 _Dimensions>
struct	TBaseTypeTraits_FpPromoter<TVector<_Type, _Dimensions> >
{
	typedef TVector<typename TBaseTypeTraits_ScalarFpPromoter<_Type>::PromotedType, _Dimensions>	FpPromotedType;
};

//----------------------------------------------------------------------------

template<typename _Type>
struct	TBaseTypeTraits_Vector
{
	typedef _Type		ScalarType;
	static const hh_u32	VecSize = 1;	// that's a scalar
};

template<typename _Type, hh_u32 _Dimensions>
struct	TBaseTypeTraits_Vector<TVector<_Type, _Dimensions> >
{
	typedef _Type		ScalarType;
	static const hh_u32 VecSize = _Dimensions;
};

template<>
struct	TBaseTypeTraits_Vector<void>
{
	typedef void		ScalarType;
	static const hh_u32	VecSize = 0;
};

//----------------------------------------------------------------------------

template<typename _Type>
struct	CBaseType : public TBaseTypeID<_Type>, public TBaseTypeTraits_Size<_Type>, public TBaseTypeTraits_Vector<_Type>, public TBaseTypeTraits_FpPromoter<_Type>
{
	// consistency check for vector types
	HH_STATIC_ASSERT_CUSTOM(TBaseTypeTraits_Size<_Type>::Size == TBaseTypeTraits_Size<typename TBaseTypeTraits_Vector<_Type>::ScalarType>::Size * TBaseTypeTraits_Vector<_Type>::VecSize, "Inconsistent_Vector_Type");
};

//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT CBaseTypeTraits
{
private:
	static const CBaseTypeTraits	m_BaseTypeTraits[__MaxBaseTypes + 1];	// +1 needed to correctly index BaseType_Evolved, that has index -1
	static const hh_u8				m_BaseMaskForVectorization[__MaxBaseTypes + 1];
public:
	EBaseTypeID		Self;
	bool			IsFp;
	bool			IsSigned;
	EBaseTypeID		FpPromotion;
	EBaseTypeID		ScalarType;
	hh_u32			VectorDimension;
	hh_u32			Size;
	const char		*Name;

	HH_FORCEINLINE static const CBaseTypeTraits	&Traits(EBaseTypeID type) { HH_ASSERT(type >= BaseType_Evolved && type < __MaxBaseTypes); return m_BaseTypeTraits[type + 1]; }

	template<typename _Type>
	HH_FORCEINLINE static const CBaseTypeTraits	&Traits() { HH_STATIC_ASSERT(IsBaseType<_Type>::True); return m_BaseTypeTraits[TBaseTypeID<_Type>::TypeID + 1]; }

	// 'newDimension' must be in the [1,4] range
	HH_FORCEINLINE static EBaseTypeID			Revectorize(EBaseTypeID type, hh_u32 newDimension) { return RevectorizeTraits(type, newDimension).Self; }
	static const CBaseTypeTraits				&RevectorizeTraits(EBaseTypeID type, hh_u32 newDimension);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_BASE_TYPES_H__
