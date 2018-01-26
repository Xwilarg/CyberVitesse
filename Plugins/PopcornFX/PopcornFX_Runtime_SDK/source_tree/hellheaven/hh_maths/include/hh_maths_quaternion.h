#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2008/04/01 9:11 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_QUATERNION_H__
#define __PK_MATHS_QUATERNION_H__

#ifndef __PK_MATHS_H__
#	error	hh_maths_Quaternion.h cannot be included without hh_maths.h
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// forward-declares

template<typename _Type, hh_u32 _Dimension>
class	TVector;
template<typename _Type, hh_u32 _AxisCount, hh_u32 _AxisDimensionCount>
class	TMatrix;

//----------------------------------------------------------------------------

template<typename _Type> class	TQuaternion;
template<typename _Type> void	PKSwapEndianInPlace(TQuaternion<_Type> &object);

//----------------------------------------------------------------------------

template<typename _Type>
class	TQuaternion
{
public:
	typedef TQuaternion<_Type>	SelfType;
	typedef	TVector<_Type, 3>	AxisVectorType;
	typedef _Type				ScalarType;
	static const hh_u32			Dimension = 4;

private:
	_Type						m_Imag[3];
	_Type						m_Real;

	friend void	PKSwapEndianInPlace<>(TQuaternion<_Type> &object);

	TQuaternion(_Type x, _Type y, _Type z, _Type w);

public:
	HH_ALIGN(0x10) static const SelfType	IDENTITY;
	TQuaternion();
	TQuaternion(AxisVectorType vec, _Type real);
	template<typename _OtherType>
	TQuaternion(const TQuaternion<_OtherType> &quat);

	// accessors

	HH_FORCEINLINE const AxisVectorType	&ImagComponent() const { return *reinterpret_cast<const AxisVectorType*>(m_Imag); }
	HH_FORCEINLINE const _Type			&RealComponent() const { return m_Real; }
	HH_FORCEINLINE AxisVectorType		&ImagComponent() { return *reinterpret_cast<AxisVectorType*>(m_Imag); }
	HH_FORCEINLINE _Type				&RealComponent() { return m_Real; }

	HH_FORCEINLINE _Type				x() const { return m_Imag[0]; }
	HH_FORCEINLINE _Type				&x() { return m_Imag[0]; }
	HH_FORCEINLINE _Type				y() const { return m_Imag[1]; }
	HH_FORCEINLINE _Type				&y() { return m_Imag[1]; }
	HH_FORCEINLINE _Type				z() const { return m_Imag[2]; }
	HH_FORCEINLINE _Type				&z() { return m_Imag[2]; }
	HH_FORCEINLINE _Type				w() const { return m_Real; }
	HH_FORCEINLINE _Type				&w() { return m_Real; }

	// operators

	const SelfType						&operator - ();
	const SelfType						&operator += (const SelfType &quat);
	const SelfType						&operator -= (const SelfType &quat);
	const SelfType						&operator *= (_Type scalar);
	const SelfType						&operator *= (const SelfType &quat);
	const SelfType						&operator /= (_Type scalar);
	const SelfType						&operator /= (const SelfType &quat);

	SelfType							operator + (const SelfType &quat) const;
	SelfType							operator - (const SelfType &quat) const;
	SelfType							operator * (const SelfType &quat) const;
	SelfType							operator * (_Type scalar) const;
	SelfType							operator / (const SelfType &quat) const;
	SelfType							operator / (_Type scalar) const;

	bool								operator == (const SelfType &quat) const;
	HH_FORCEINLINE bool					operator != (const SelfType &quat) const { return !operator == (quat); }

	// functions

	HH_INLINE void						Mult(const SelfType &quat, SelfType &output) const;

	_Type								Dot(const SelfType &otherQuat) const;
	SelfType							Conjugate() const;
	HH_FORCEINLINE _Type				LengthSquared() const { return Dot(*this); }
	HH_FORCEINLINE _Type				Length() const { return HHSqrt(LengthSquared()); }
	SelfType							Normalized() const { _Type normalizer = HHRSqrt(LengthSquared()); return *this * normalizer; }
	HH_FORCEINLINE const SelfType		&Normalize() { *this = Normalized(); return *this; }
	bool								IsNormalized(_Type toleranceSquared = 1.0e-4f) const { return HHAbs(LengthSquared() - 1.0f) < toleranceSquared; }
	bool								IsReal() const { return ImagComponent().LengthSquared() == 0; }
	bool								IsFinite() const;	// returns true if not (IsNan() || IsInfinite())
	bool								IsNan() const;
	bool								IsInfinite() const;

	SelfType							Inverse() const;
	const SelfType						&Invert() { *this = Inverse(); return *this; }

	_Type								Angle() const { return 2.0f * acosf(m_Real); }

	static SelfType						FromAxisAngle(const TVector<_Type, 3> &axis, _Type angle);
	static SelfType						FromUnitAxisAngle(const TVector<_Type, 3> &axis, _Type angle);

	template<typename _Type2>
	TVector<_Type2, 3>					RotateVector(const TVector<_Type2, 3> &v) const;
	void								RotateAroundAxis(const AxisVectorType &axis, _Type angle);
	void								RotateAroundUnitAxis(const AxisVectorType &axis, _Type angle);

	void								AlignWith(const SelfType &otherQuat);	// we will flip ourself if we are at more than 180 degrees with otherQuat
	static _Type						ErrorBetween(const SelfType &q0, const SelfType &q1);

	static SelfType						Slerp(const SelfType &q0, const SelfType &q1, float frac);
	static SelfType						SlerpSafe(const SelfType &q0, const SelfType &q1, float frac);
	static SelfType						Nlerp(const SelfType &q0, const SelfType &q1, float frac);
	static SelfType						NlerpSafe(const SelfType &q0, const SelfType &q1, float frac);
};

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>	operator * (_Type scalar, const TQuaternion<_Type> &quat) { return quat * scalar; }
template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>	operator / (_Type scalar, const TQuaternion<_Type> &quat) { return quat / scalar; }

template<typename _Type>
const TQuaternion<_Type> TQuaternion<_Type>::IDENTITY = TQuaternion<_Type>(0,0,0,1);

//----------------------------------------------------------------------------

typedef TQuaternion<float>	CQuaternion;
HH_STATIC_ASSERT(sizeof(CQuaternion) == 0x10);

//----------------------------------------------------------------------------

//template<typename _Type>
//TQuaternion<_Type>	PKSwapEndian(const TQuaternion<_Type> &object)
//{
//	TQuaternion<_Type>	out = object;
//	PKSwapEndianInPlace(out);
//	return out;
//}

namespace	Internal	// kr_memoryviews.h
{
	template<typename _Type>
	struct	TStreamEndianSwapperHelpers<TQuaternion<_Type> >
	{
		static void	SwapInPlace(TQuaternion<_Type> *data, hh_u32 count) { TNativeSwapper<sizeof(_Type)>::SwapInPlace(data, count * 4); }
	};
}

template<typename _Type>
void	PKSwapEndianInPlace(TQuaternion<_Type> &object)
{
	PKSwapEndianInPlace(object.m_Imag[0]);
	PKSwapEndianInPlace(object.m_Imag[1]);
	PKSwapEndianInPlace(object.m_Imag[2]);
	PKSwapEndianInPlace(object.m_Real);
}

//----------------------------------------------------------------------------

namespace	TTypeTraits
{
	template<typename _Type>	struct	IsInteger<TQuaternion<_Type> > : FalseType {};	// this is stupid... who would want integer quaternions? #triso# we should forbid it directly in the quaternion template
	template<typename _Type>	struct	IsFloat<TQuaternion<_Type> > : TrueType {};
	template<typename _Type>	struct	IsSigned<TQuaternion<_Type> > : TrueType {};
}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_maths/include/hh_maths_quaternion.inl"

#endif // __PK_MATHS_QUATERNION_H__
