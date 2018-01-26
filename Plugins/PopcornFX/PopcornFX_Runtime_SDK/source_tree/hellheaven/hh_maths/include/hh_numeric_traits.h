#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2007/11/14 17:28 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_NUMERIC_TRAITS_H__
#define __PK_NUMERIC_TRAITS_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>
struct	CGenericNumericTraits
{
	HH_STATIC_ASSERT_CUSTOM(TTypeTraits::IsNumeric<_Type>::True, "Non_Numeric_Type");

	typedef	_Type	SelfType;
	typedef _Type	SignedType;
	typedef _Type	UnsignedType;
};

template<typename _Type>
class	TNumericTraits
{
	HH_STATIC_ASSERT_NOT_REACHED_CUSTOM("Unspecialized numeric traits");
};

template<typename _Type>
class	TNumericConstants
{
	HH_STATIC_ASSERT_NOT_REACHED_CUSTOM("Unspecialized numeric constants");
};

template<typename _Type>
class	TNumericTraits<const _Type> : public TNumericTraits<_Type> {};

template<typename _Type>
class	TNumericTraits<volatile _Type> : public TNumericTraits<_Type> {};

template<typename _Type>
class	TNumericConstants<const _Type> : public TNumericConstants<_Type> {};

template<typename _Type>
class	TNumericConstants<volatile _Type> : public TNumericConstants<_Type> {};

template<hh_u32 _Quality>
struct	TNumericTypeQuality { static const hh_u32	Quality = _Quality; };
template<hh_u32 _Quality>	const hh_u32	TNumericTypeQuality<_Quality>::Quality;

//----------------------------------------------------------------------------
//
//	Numeric ops
//
//----------------------------------------------------------------------------

#define	HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(__opname, __operator) \
template<typename _Type0, typename _Type1 = _Type0> \
struct	TTypeOp ## __opname \
{ \
	static bool	Eval(_Type0 u, _Type1 v) \
	{ \
		return u __operator v; \
	} \
}

HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(Equal, == );
HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(NotEqual, != );
HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(Greater, > );
HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(Lower, < );
HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(GreaterOrEqual, >= );
HH_NUMERIC_TRAITS_BUILD_NUMERIC_OP(LowerOrEqual, <= );

//----------------------------------------------------------------------------
//
//	Cast helpers
//
//----------------------------------------------------------------------------

template<typename _TypeA, typename _TypeB>
struct	TNumericPromoter
{
	// pre-conditions
	HH_STATIC_ASSERT_CUSTOM(TTypeTraits::IsNumeric<_TypeA>::True && TTypeTraits::IsNumeric<_TypeB>::True, "Invalid_Use_Of__TNumericPromoter____Template_Expects_Numeric_Types");
public:
	typedef _TypeA	FirstType;
	typedef _TypeB	SecondType;

	typedef typename TTypeSwitcher_IfTrue<_TypeA, _TypeB, TNumericTraits<_TypeA>::Quality >= TNumericTraits<_TypeB>::Quality>::Type	PromotedType;
};

//----------------------------------------------------------------------------

template<typename _TypeA, typename _TypeB, bool _AreBothNumeric = TTypeTraits::IsNumeric<_TypeA>::True && TTypeTraits::IsNumeric<_TypeB>::True>
struct	TTypePromoter : TNumericPromoter<_TypeA, _TypeB>
{
};

//----------------------------------------------------------------------------

template<typename _TypeA, typename _TypeB>
struct	TTypePromoter<_TypeA, _TypeB, false> : TNumericPromoter<_TypeA, _TypeB>
{
};

//----------------------------------------------------------------------------
//
//	mask helpers, will only work for native types
//
//----------------------------------------------------------------------------

template<typename _Type, bool _IsFloat = TTypeTraits::IsFloat<_Type>::True >
struct	TIntegerFootprint
{
	typedef typename TNumericTraits<_Type>::UnsignedType		UnsignedType;
	typedef typename TNumericTraits<_Type>::SignedType			SignedType;
};

template<typename _Type>
struct	TIntegerFootprint<_Type, true>
{
	typedef typename TNumericTraits<_Type>::SelfFpTypeBits			UnsignedType;
	typedef typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	SignedType;
};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_maths/include/hh_numeric_traits_int.h"
#include "hh_maths/include/hh_numeric_traits_float.h"

#endif // __PK_NUMERIC_TRAITS_H__
