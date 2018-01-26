#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2010/03/16 9:00 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_VECTOR_INL__
#define __PK_MATHS_VECTOR_INL__

#ifndef	__PK_MATHS_VECTOR_H__
#	error	hh_maths_vector.inl must not be included manually, include hh_maths_vector.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Ctors <1>
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 1>::TVector()
{
	HH_ONLY_IN_DEBUG(
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[0])
	);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 1>::TVector(const _V &other)
:	_V(other)
{
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 1>::TVector(_Type value)
{
	this->m_Data[0] = value;
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
HH_FORCEINLINE TVector<_Type, 1>::TVector(const TVector<_Type2, 1> &copyFrom)
{
	this->m_Data[0] = (_Type)copyFrom.Axis(0);
}

//----------------------------------------------------------------------------
//
//	Ctors <2>
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 2>::TVector()
{
	HH_ONLY_IN_DEBUG(
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[0]);
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[1]);
	);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 2>::TVector(const _V &other)
:	_V(other)
{
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 2>::TVector(_Type value)
{
	this->m_Data[0] = value;
	this->m_Data[1] = value;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 2>::TVector(_Type v0, _Type v1)
{
	this->m_Data[0] = v0;
	this->m_Data[1] = v1;
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
HH_FORCEINLINE TVector<_Type, 2>::TVector(const TVector<_Type2, 2> &copyFrom)
{
	this->m_Data[0] = (_Type)copyFrom.Axis(0);
	this->m_Data[1] = (_Type)copyFrom.Axis(1);
}

//----------------------------------------------------------------------------
//
//	Ctors <3>
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 3>::TVector()
{
	HH_ONLY_IN_DEBUG(
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[0]);
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[1]);
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[2]);
	);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 3>::TVector(const _V &other)
:	_V(other)
{
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 3>::TVector(_Type value)
{
	this->m_Data[0] = value;
	this->m_Data[1] = value;
	this->m_Data[2] = value;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 3>::TVector(_Type v0, _Type v1, _Type v2)
{
	this->m_Data[0] = v0;
	this->m_Data[1] = v1;
	this->m_Data[2] = v2;
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
HH_FORCEINLINE TVector<_Type, 3>::TVector(const TVector<_Type2, 3> &copyFrom)
{
	this->m_Data[0] = (_Type)copyFrom.Axis(0);
	this->m_Data[1] = (_Type)copyFrom.Axis(1);
	this->m_Data[2] = (_Type)copyFrom.Axis(2);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 3>::TVector(const TVector<_Type, 3 - 1> &vec, _Type v)
{
	this->m_Data[0] = vec.Axis(0);
	this->m_Data[1] = vec.Axis(1);
	this->m_Data[2] = v;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 3>::TVector(_Type v0, const TVector<_Type, 3 - 1> &vec)
{
	this->m_Data[0] = v0;
	this->m_Data[1] = vec.Axis(0);
	this->m_Data[2] = vec.Axis(1);
}

//----------------------------------------------------------------------------
//
//	Ctors <4>
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector()
{
	HH_ONLY_IN_DEBUG(
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[0]);
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[1]);
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[2]);
		TNumericTraits<_Type>::SetInvalidDebug(this->m_Data[3]);
	);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(const _V &other)
:	_V(other)
{
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(_Type value)
{
	this->m_Data[0] = value;
	this->m_Data[1] = value;
	this->m_Data[2] = value;
	this->m_Data[3] = value;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(_Type v0, _Type v1, _Type v2, _Type v3)
{
	HH_STATIC_ASSERT_CUSTOM_COUNT(4, 4, Initializers);
	this->m_Data[0] = v0;
	this->m_Data[1] = v1;
	this->m_Data[2] = v2;
	this->m_Data[3] = v3;
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
HH_FORCEINLINE TVector<_Type, 4>::TVector(const TVector<_Type2, 4> &copyFrom)
{
	this->m_Data[0] = (_Type)copyFrom.Axis(0);
	this->m_Data[1] = (_Type)copyFrom.Axis(1);
	this->m_Data[2] = (_Type)copyFrom.Axis(2);
	this->m_Data[3] = (_Type)copyFrom.Axis(3);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(const TVector<_Type, 4 - 1> &vec, _Type v)
{
	this->m_Data[0] = vec.Axis(0);
	this->m_Data[1] = vec.Axis(1);
	this->m_Data[2] = vec.Axis(2);
	this->m_Data[3] = v;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(_Type v0, const TVector<_Type, 4 - 1> &vec)
{
	this->m_Data[0] = v0;
	this->m_Data[1] = vec.Axis(0);
	this->m_Data[2] = vec.Axis(1);
	this->m_Data[3] = vec.Axis(2);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(const TVector<_Type, 2> &vec, _Type v2, _Type v3)
{
	this->m_Data[0] = vec.Axis(0);
	this->m_Data[1] = vec.Axis(1);
	this->m_Data[2] = v2;
	this->m_Data[3] = v3;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(_Type v0, _Type v1, const TVector<_Type, 2> &vec)
{
	this->m_Data[0] = v0;
	this->m_Data[1] = v1;
	this->m_Data[2] = vec.Axis(0);
	this->m_Data[3] = vec.Axis(1);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(const TVector<_Type, 2> &vec0, const TVector<_Type, 2> &vec1)
{
	this->m_Data[0] = vec0.Axis(0);
	this->m_Data[1] = vec0.Axis(1);
	this->m_Data[2] = vec1.Axis(0);
	this->m_Data[3] = vec1.Axis(1);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TVector<_Type, 4>::TVector(_Type v0, const TVector<_Type, 2> &vec, _Type v3)
{
	this->m_Data[0] = v0;
	this->m_Data[1] = vec.Axis(0);
	this->m_Data[2] = vec.Axis(1);
	this->m_Data[3] = v3;
}

//----------------------------------------------------------------------------
//
//	operators
//
//----------------------------------------------------------------------------

template<typename _Type1, hh_u32 _Dimension1, typename _Type2>
struct	TVectorPromotableOpImpl
{
	// scalars
	typedef TVectorBase<_Type1, _Dimension1>						_SrcVector;
	typedef typename TNumericPromoter<_Type1, _Type2>::PromotedType	_PromotedScalar;
	typedef TVector<_PromotedScalar, _Dimension1>					_PromotedVector;

#define	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(__which) \
	static HH_FORCEINLINE _PromotedVector	__which (const _SrcVector &op0, _Type2 const &op1) \
	{ \
		_PromotedVector	output; \
		for (hh_u32 i = 0; i < _Dimension1; i++) \
		{ \
			output.Axis(i) = op0.Axis(i) GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which) op1; \
		} \
		return output; \
	} \

	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Add);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Sub);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Mul);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Div);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Mod);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Or);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(And);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Xor);

	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Lsr);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Lsl);

#undef	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP
};

//----------------------------------------------------------------------------

template<typename _Type1, hh_u32 _Dimension1, typename _Type2, hh_u32 _Dimension2>
struct	TVectorPromotableOpImpl<_Type1, _Dimension1, TVectorImpl<_Type2, _Dimension2> >
{
	// vectors
	HH_STATIC_ASSERT(_Dimension1 == _Dimension2);
	typedef TVectorBase<_Type1, _Dimension1>						_SrcVector;
	typedef typename TNumericPromoter<_Type1, _Type2>::PromotedType	_PromotedScalar;
	typedef TVector<_PromotedScalar, _Dimension1>					_PromotedVector;

#define	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(__which) \
	static HH_FORCEINLINE _PromotedVector	__which (const _SrcVector &op0, const TVectorImpl<_Type2, _Dimension1> &op1) \
	{ \
		_PromotedVector	output; \
		for (hh_u32 i = 0; i < _Dimension1; i++) \
		{ \
			output.Axis(i) = op0.Axis(i) GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which) op1.Axis(i); \
		} \
		return output; \
	} \

	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Add);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Sub);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Mul);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Div);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Mod);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Or);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(And);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Xor);

	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Lsr);
	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP(Lsl);

#undef	HH_MATHS_VECTOR_BUILD_PROMOTABLE_OP
};

//----------------------------------------------------------------------------

template<typename _Type1, hh_u32 _Dimension1, typename _Type2, hh_u32 _Dimension2>
struct	TVectorPromotableOpImpl<_Type1, _Dimension1, TVector<_Type2, _Dimension2> > : public TVectorPromotableOpImpl<_Type1, _Dimension1, TVectorImpl<_Type2, _Dimension2> >
{
};

//----------------------------------------------------------------------------
//
//	Vector compares
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskEqual(_Type scalar) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] == scalar) ? 0xFFFFFFFF : 0x00000000;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskEqual(const TVectorImpl<_Type2, _Dimension> &vec) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] == vec.Axis(i)) ? 0xFFFFFFFF : 0x00000000;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskNotEqual(_Type scalar) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] == scalar) ? 0x00000000 : 0xFFFFFFFF;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskNotEqual(const TVectorImpl<_Type2, _Dimension> &vec) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] == vec.Axis(i)) ? 0x00000000 : 0xFFFFFFFF;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskGreater(const TVectorImpl<_Type2, _Dimension> &vec) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] > vec.Axis(i)) ? 0xFFFFFFFF : 0x00000000;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskGreaterOrEqual(const TVectorImpl<_Type2, _Dimension> &vec) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] >= vec.Axis(i)) ? 0xFFFFFFFF : 0x00000000;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskLower(const TVectorImpl<_Type2, _Dimension> &vec) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] < vec.Axis(i)) ? 0xFFFFFFFF : 0x00000000;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE typename TVectorImpl<_Type, _Dimension>::MaskType	TVectorImpl<_Type, _Dimension>::MaskLowerOrEqual(const TVectorImpl<_Type2, _Dimension> &vec) const
{
	typename TVectorImpl<_Type, _Dimension>::MaskType	mask;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		mask.Axis(i) = (this->m_Data[i] <= vec.Axis(i)) ? 0xFFFFFFFF : 0x00000000;	// might not produce very efficient code... check this out
	}
	return mask;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
bool	TVectorImpl<_Type, _Dimension>::operator == (_Type scalar) const
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (this->m_Data[i] != scalar)
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
bool	TVectorImpl<_Type, _Dimension>::operator == (const TVectorImpl<_Type2, _Dimension> &vec) const
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (this->m_Data[i] != vec.m_Data[i])
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
bool	TVectorImpl<_Type, _Dimension>::operator != (_Type scalar) const
{
	return !(*this == scalar);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
bool	TVectorImpl<_Type, _Dimension>::operator != (const TVectorImpl<_Type2, _Dimension> &vec) const
{
	return !(*this == vec);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
bool	TVectorImpl<_Type, _Dimension>::Equals(const SelfType &other, _Type epsilon) const	// FIXME: won't work with unsigned types
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (HHAbs(this->m_Data[i] - other.m_Data[i]) > epsilon)
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------
//
//	3D cross-product
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2>
HH_FORCEINLINE TVector<typename TNumericPromoter<_Type, _Type2>::PromotedType, _Dimension>	TVectorImpl<_Type, _Dimension>::Cross(const TVector<_Type2, _Dimension> &v) const
{
	HH_STATIC_ASSERT(_Dimension == 3);
	typedef TVector<typename TNumericPromoter<_Type, _Type2>::PromotedType, _Dimension>	ResultType;
	return ResultType(	this->m_Data[1] * v.Axis(2) - this->m_Data[2] * v.Axis(1),
						this->m_Data[2] * v.Axis(0) - this->m_Data[0] * v.Axis(2),
						this->m_Data[0] * v.Axis(1) - this->m_Data[1] * v.Axis(0));
}

//----------------------------------------------------------------------------
//
//	4D cross-product
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
template<typename _Type2, typename _Type3>
HH_FORCEINLINE TVector<typename TNumericPromoter<_Type, typename TNumericPromoter<_Type2, _Type3>::PromotedType>::PromotedType, _Dimension>	TVectorImpl<_Type, _Dimension>::Cross(const TVector<_Type2, _Dimension> &v0, const TVector<_Type3, _Dimension> &v1) const
{
	HH_STATIC_ASSERT(_Dimension == 4);
	typedef	typename TNumericPromoter<_Type, typename TNumericPromoter<_Type2, _Type3>::PromotedType>::PromotedType	PromotedType;
	typedef TVector<PromotedType, _Dimension>	ResultType;

	const PromotedType	a = v0.Axis(0) * v1.Axis(1) - v0.Axis(1) * v1.Axis(0);
	const PromotedType	b = v0.Axis(0) * v1.Axis(2) - v0.Axis(2) * v1.Axis(0);
	const PromotedType	c = v0.Axis(0) * v1.Axis(3) - v0.Axis(3) * v1.Axis(0);
	const PromotedType	d = v0.Axis(1) * v1.Axis(2) - v0.Axis(2) * v1.Axis(1);
	const PromotedType	e = v0.Axis(1) * v1.Axis(3) - v0.Axis(3) * v1.Axis(1);
	const PromotedType	f = v0.Axis(2) * v1.Axis(3) - v0.Axis(3) * v1.Axis(2);

	return ResultType(	(this->m_Data[1] * f) - (this->m_Data[2] * e) + (this->m_Data[3] * d),
					   -(this->m_Data[0] * f) + (this->m_Data[2] * c) - (this->m_Data[3] * b),
						(this->m_Data[0] * e) - (this->m_Data[1] * c) + (this->m_Data[3] * a),
					   -(this->m_Data[0] * d) + (this->m_Data[1] * b) - (this->m_Data[2] * a));
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	TVectorImpl<_Type, _Dimension>::Lerp(const SelfType &v, float fraction) const
{
	SelfFullType	result;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		result.m_Data[i] = this->m_Data[i] + (v.m_Data[i] - this->m_Data[i]) * fraction;
	}
	return result;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
void	TVectorImpl<_Type, _Dimension>::Normalize()
{
	HH_PARANOID_ASSERT(Length() > 1.0e-10f);
	this->operator *= (HHRSqrt(LengthSquared()));
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
HH_FORCEINLINE TVector<_Type, _Dimension>	TVectorImpl<_Type, _Dimension>::Normalized() const
{
	SelfFullType output(*this);
	output.Normalize();
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
HH_FORCEINLINE bool		TVectorImpl<_Type, _Dimension>::IsNormalized(_Type tolerance /*= 1.0e-2f*/) const
{
#if 1
	return HHAbs(Length() - 1) < tolerance;
#else
	const float	kT1 = 1 - tolerance;
	const float	kT2 = 1 - (kT1 * kT1);
	return HHAbs(LengthSquared() - 1) < kT2;
#endif
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
bool	TVectorImpl<_Type, _Dimension>::IsFinite() const	// returns true if not (IsNan() || IsInfinite())
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (!TNumericTraits<_Type>::IsFinite(this->m_Data[i]))
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
bool	TVectorImpl<_Type, _Dimension>::IsNan() const
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (TNumericTraits<_Type>::IsNan(this->m_Data[i]))
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
bool	TVectorImpl<_Type, _Dimension>::IsInfinite() const
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (TNumericTraits<_Type>::IsInfinite(this->m_Data[i]))
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
_Type	TVectorImpl<_Type, _Dimension>::AxialSum() const
{
	ScalarType	output = this->m_Data[0];
	for (hh_u32 i = 1; i < _Dimension; i++)
	{
		output += this->m_Data[i];
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
_Type	TVectorImpl<_Type, _Dimension>::AxialProduct() const
{
	ScalarType	output = this->m_Data[0];
	for (hh_u32 i = 1; i < _Dimension; i++)
	{
		output *= this->m_Data[i];
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
_Type	TVectorImpl<_Type, _Dimension>::HighestComponent() const
{
	_Type	highestValue = this->m_Data[0];
	for (hh_u32 i = 1; i < _Dimension; i++)
	{
		if (this->m_Data[i] > highestValue)
			highestValue = this->m_Data[i];
	}
	return highestValue;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
_Type	TVectorImpl<_Type, _Dimension>::LowestComponent() const
{
	_Type	lowestValue = this->m_Data[0];
	for (hh_u32 i = 1; i < _Dimension; i++)
	{
		if (this->m_Data[i] < lowestValue)
			lowestValue = this->m_Data[i];
	}
	return lowestValue;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
hh_u32	TVectorImpl<_Type, _Dimension>::HighestComponentId() const
{
	hh_u32	id = 0;
	_Type	highestValue = this->m_Data[0];
	for (hh_u32 i = 1; i < _Dimension; i++)
	{
		if (this->m_Data[i] > highestValue)
		{
			id = i;
			highestValue = this->m_Data[i];
		}
	}
	return id;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
hh_u32	TVectorImpl<_Type, _Dimension>::LowestComponentId() const
{
	hh_u32	id = 0;
	_Type	lowestValue = this->m_Data[0];
	for (hh_u32 i = 1; i < _Dimension; i++)
	{
		if (this->m_Data[i] < lowestValue)
		{
			id = i;
			lowestValue = this->m_Data[i];
		}
	}
	return id;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
HH_INLINE bool	TVectorImpl<_Type, _Dimension>::IsZero(_Type tolerance /*= 1.0e-10f*/) const
{
	return LengthSquared() <= HHSquared(tolerance);
}

//----------------------------------------------------------------------------
//
//	Endian conversions
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
void	TVectorImpl<_Type, _Dimension>::ConvertLittleEndian()
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		PKSwapLittleEndianInPlace(this->m_Data[i]);
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
void	TVectorImpl<_Type, _Dimension>::ConvertBigEndian()
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		PKSwapBigEndianInPlace(this->m_Data[i]);
	}
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	TVectorImpl<_Type, _Dimension>::ConvertLittleEndian() const
{
	SelfFullType	result(*this);
	result.ConvertLittleEndian();
	return result;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	TVectorImpl<_Type, _Dimension>::ConvertBigEndian() const
{
	SelfFullType	result(*this);
	result.ConvertBigEndian();
	return result;
}


//----------------------------------------------------------------------------
//
//	Explicit instantiations:
//	unfortunately this forces us to duplicate some code,
//	and we can't static_assert() like we used to in the methods that shouldn't
//	be instantiated at a specific dimension.
//	but GCC on PS3 can't fold comdats, and without these explicit instantiations,
//	we're left with binaries of insane dimensions
//
//----------------------------------------------------------------------------

#if	(HH_MATHS_VECTOR_EXPLICIT_INSTANTIATION != 0)

extern template class	TVectorImpl<float, 4>;
extern template class	TVectorImpl<float, 3>;
extern template class	TVectorImpl<float, 2>;
extern template class	TVector<float, 4>;
extern template class	TVector<float, 3>;
extern template class	TVector<float, 2>;

//extern template class	TVectorImpl<hh_u32, 4>;
//extern template class	TVectorImpl<hh_u32, 3>;
extern template class	TVector<hh_u32, 4>;
extern template class	TVector<hh_u32, 3>;

//extern template class	TVectorImpl<hh_i32, 4>;
//extern template class	TVectorImpl<hh_i32, 3>;
extern template class	TVector<hh_i32, 4>;
extern template class	TVector<hh_i32, 3>;

#else

template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 1>	TVector<_Type, 1>::XAXIS = TVector<_Type, 1>::UnitAxis(0);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 1>	TVector<_Type, 1>::ZERO = TVector<_Type, 1>(TNumericTraits<_Type>::Zero());
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 1>	TVector<_Type, 1>::ONE = TVector<_Type, 1>(TNumericTraits<_Type>::One());


template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 2>	TVector<_Type, 2>::XAXIS = TVector<_Type, 2>::UnitAxis(0);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 2>	TVector<_Type, 2>::YAXIS = TVector<_Type, 2>::UnitAxis(1);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 2>	TVector<_Type, 2>::ZERO = TVector<_Type, 2>(TNumericTraits<_Type>::Zero());
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 2>	TVector<_Type, 2>::ONE = TVector<_Type, 2>(TNumericTraits<_Type>::One());


template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 3>	TVector<_Type, 3>::XAXIS = TVector<_Type, 3>::UnitAxis(0);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 3>	TVector<_Type, 3>::YAXIS = TVector<_Type, 3>::UnitAxis(1);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 3>	TVector<_Type, 3>::ZAXIS = TVector<_Type, 3>::UnitAxis(2);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 3>	TVector<_Type, 3>::ZERO = TVector<_Type, 3>(TNumericTraits<_Type>::Zero());
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 3>	TVector<_Type, 3>::ONE = TVector<_Type, 3>(TNumericTraits<_Type>::One());


template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 4>	TVector<_Type, 4>::XAXIS = TVector<_Type, 4>::UnitAxis(0);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 4>	TVector<_Type, 4>::YAXIS = TVector<_Type, 4>::UnitAxis(1);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 4>	TVector<_Type, 4>::ZAXIS = TVector<_Type, 4>::UnitAxis(2);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 4>	TVector<_Type, 4>::WAXIS = TVector<_Type, 4>::UnitAxis(3);
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 4>	TVector<_Type, 4>::ZERO = TVector<_Type, 4>(TNumericTraits<_Type>::Zero());
template<typename _Type>
HH_ALIGN(0x10) const TVector<_Type, 4>	TVector<_Type, 4>::ONE = TVector<_Type, 4>(TNumericTraits<_Type>::One());

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_VECTOR_INL__
