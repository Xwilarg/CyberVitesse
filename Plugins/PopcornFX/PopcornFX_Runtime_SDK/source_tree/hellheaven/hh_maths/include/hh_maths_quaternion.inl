#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2010/03/16 17:41 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_QUATERNION_INL__
#define __PK_MATHS_QUATERNION_INL__

#ifndef	__PK_MATHS_QUATERNION_H__
#	error	hh_maths_quaternion.inl must not be included manually, include hh_maths_quaternion.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>::TQuaternion()
{
	HH_ONLY_IN_DEBUG(
		TNumericTraits<_Type>::SetInvalidDebug(m_Imag[0]);
		TNumericTraits<_Type>::SetInvalidDebug(m_Imag[1]);
		TNumericTraits<_Type>::SetInvalidDebug(m_Imag[2]);
		TNumericTraits<_Type>::SetInvalidDebug(m_Real);
	);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>::TQuaternion(_Type inx, _Type iny, _Type inz, _Type inw)
{
	m_Imag[0] = inx;
	m_Imag[1] = iny;
	m_Imag[2] = inz;
	m_Real    = inw;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>::TQuaternion(AxisVectorType vec, _Type real)
{
	*reinterpret_cast<AxisVectorType*>(m_Imag) = vec;
	m_Real = real;
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _OtherType>
HH_FORCEINLINE TQuaternion<_Type>::TQuaternion(const TQuaternion<_OtherType> &quat)
{
	*reinterpret_cast<AxisVectorType*>(m_Imag) = quat.ImagComponent();
	m_Real = quat.RealComponent();
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator - ()
{
	m_Imag[0] = -m_Imag[0];
	m_Imag[1] = -m_Imag[1];
	m_Imag[2] = -m_Imag[2];
	m_Real    = -m_Real;
	return *this;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator += (const SelfType &quat)
{
	m_Imag[0] += quat.m_Imag[0];
	m_Imag[1] += quat.m_Imag[1];
	m_Imag[2] += quat.m_Imag[2];
	m_Real    += quat.m_Real;
	return *this;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator -= (const SelfType &quat)
{
	m_Imag[0] -= quat.m_Imag[0];
	m_Imag[1] -= quat.m_Imag[1];
	m_Imag[2] -= quat.m_Imag[2];
	m_Real    -= quat.m_Real;
	return *this;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator *= (_Type scalar)
{
	m_Imag[0] *= scalar;
	m_Imag[1] *= scalar;
	m_Imag[2] *= scalar;
	m_Real    *= scalar;
	return *this;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator *= (const SelfType &quat)
{
	Mult(quat, *this);
	return *this;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator /= (_Type scalar)
{
	const _Type	rcp = HHRcp(scalar);
	m_Imag[0] *= rcp;
	m_Imag[1] *= rcp;
	m_Imag[2] *= rcp;
	m_Real    *= rcp;
	return *this;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE const TQuaternion<_Type>		&TQuaternion<_Type>::operator /= (const SelfType &quat)
{
	operator *= (quat.Inverse());
	return *this;
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>			TQuaternion<_Type>::operator + (const SelfType &quat) const
{
	return SelfType(m_Imag[0] + quat.m_Imag[0],
					m_Imag[1] + quat.m_Imag[1],
					m_Imag[2] + quat.m_Imag[2],
					m_Real    + quat.m_Real);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>			TQuaternion<_Type>::operator - (const SelfType &quat) const
{
	return SelfType(m_Imag[0] - quat.m_Imag[0],
					m_Imag[1] - quat.m_Imag[1],
					m_Imag[2] - quat.m_Imag[2],
					m_Real    - quat.m_Real);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>			TQuaternion<_Type>::operator * (const SelfType &quat) const
{
	SelfType	tmp;
	Mult(quat, tmp);
	return tmp;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>			TQuaternion<_Type>::operator * (_Type scalar) const
{
	return SelfType(m_Imag[0] * scalar,
					m_Imag[1] * scalar,
					m_Imag[2] * scalar,
					m_Real    * scalar);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>			TQuaternion<_Type>::operator / (const SelfType &quat) const
{
	SelfType tmp = *this;
	tmp /= quat;
	return tmp;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>			TQuaternion<_Type>::operator / (_Type scalar) const
{
	const _Type	rcp = HHRcp(scalar);
	return SelfType(m_Imag[0] * rcp,
					m_Imag[1] * rcp,
					m_Imag[2] * rcp,
					m_Real    * rcp);
}

//----------------------------------------------------------------------------
//
//	Compare ops
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE bool					TQuaternion<_Type>::operator == (const SelfType &quat) const
{
	return	m_Imag[0] == quat.m_Imag[0] &&
			m_Imag[1] == quat.m_Imag[1] &&
			m_Imag[2] == quat.m_Imag[2] &&
			m_Real    == quat.m_Real;
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE _Type				TQuaternion<_Type>::Dot(const SelfType &otherQuat) const
{
	return	m_Imag[0] * otherQuat.m_Imag[0] +
			m_Imag[1] * otherQuat.m_Imag[1] +
			m_Imag[2] * otherQuat.m_Imag[2] +
			m_Real    * otherQuat.m_Real;
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>	TQuaternion<_Type>::Conjugate() const
{
	return SelfType(-m_Imag[0],
					-m_Imag[1],
					-m_Imag[2],
					 m_Real);
}

//----------------------------------------------------------------------------

// this is the "standard" way to multiply quaternions. it's taking 16 muls and 12 add/sub. however, we could change this to 12 mults/20 add/subs or so.
// see if it becomes problematic. if it does, see if a batched mult version wouldn't be more efficient with this classic version
template<typename _Type>
HH_INLINE void	TQuaternion<_Type>::Mult(const TQuaternion<_Type> &quat, TQuaternion<_Type> &output) const
{
#if 1
//	const _Type	xv = w() * quat.x() + x() * quat.w() + y() * quat.z() - z() * quat.y();
//	const _Type	yv = w() * quat.y() - x() * quat.z() + y() * quat.w() + z() * quat.x();
//	const _Type	zv = w() * quat.z() + x() * quat.y() - y() * quat.x() + z() * quat.w();
//	const _Type	wv = w() * quat.w() - x() * quat.x() - y() * quat.y() - z() * quat.z();

	const _Type	xv = x() * quat.w() + w() * quat.x() + y() * quat.z() - z() * quat.y();
	const _Type	yv = y() * quat.w() + w() * quat.y() + z() * quat.x() - x() * quat.z();
	const _Type	zv = z() * quat.w() + w() * quat.z() + x() * quat.y() - y() * quat.x();
	const _Type	wv = w() * quat.w() - x() * quat.x() - y() * quat.y() - z() * quat.z();

	output.x() = xv;
	output.y() = yv;
	output.z() = zv;
	output.w() = wv;
#else
	// this one is less scalar-intensive, but less vectorization-friendly
	const _Type	c0 = x() * quat.x() + z() * quat.y();
	const _Type	a = (z() - y()) * (quat.y() - quat.z()) - c0;
	const _Type	b = (w() + x()) * (quat.w() + quat.x()) - c0;
	const _Type	c = w() * quat.w() - y() * quat.z();

	const _Type	c1 = x() * quat.y() + z() * quat.x();
	const _Type	d = (w() - x()) * (quat.y() + quat.z()) + c1;
	const _Type	e = (z() + y()) * (quat.w() - quat.x()) + c1;
	const _Type	f = y() * quat.w() - w() * quat.z();

	output.x() = b - c;
	output.y() = d + f;
	output.z() = e - f;
	output.w() = a + c;
#endif
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::Inverse() const
{
	const _Type	iqdot = HHRcp(LengthSquared());
	return SelfType(m_Imag[0] * -iqdot,
					m_Imag[1] * -iqdot,
					m_Imag[2] * -iqdot,
					m_Real    *  iqdot);
}

//----------------------------------------------------------------------------

template<typename _Type>
bool	TQuaternion<_Type>::IsFinite() const	// returns true if not (IsNan() || IsInfinite())
{
	return	TNumericTraits<_Type>::IsFinite(m_Imag[0]) &&
			TNumericTraits<_Type>::IsFinite(m_Imag[1]) &&
			TNumericTraits<_Type>::IsFinite(m_Imag[2]) &&
			TNumericTraits<_Type>::IsFinite(m_Real);
}

//----------------------------------------------------------------------------

template<typename _Type>
bool	TQuaternion<_Type>::IsNan() const
{
	return	TNumericTraits<_Type>::IsNan(m_Imag[0]) ||
			TNumericTraits<_Type>::IsNan(m_Imag[1]) ||
			TNumericTraits<_Type>::IsNan(m_Imag[2]) ||
			TNumericTraits<_Type>::IsNan(m_Real);
}

//----------------------------------------------------------------------------

template<typename _Type>
bool	TQuaternion<_Type>::IsInfinite() const
{
	return	TNumericTraits<_Type>::IsInfinite(m_Imag[0]) ||
			TNumericTraits<_Type>::IsInfinite(m_Imag[1]) ||
			TNumericTraits<_Type>::IsInfinite(m_Imag[2]) ||
			TNumericTraits<_Type>::IsInfinite(m_Real);
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::FromAxisAngle(const TVector<_Type, 3> &axis, _Type angle)
{
	const _Type	x = angle * 0.5f;
	const _Type	sintheta = sinf(x);
	const _Type	costheta = cosf(x);
	return TQuaternion<_Type>(axis.Normalized() * sintheta, costheta);
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::FromUnitAxisAngle(const TVector<_Type, 3> &axis, _Type angle)
{
	const _Type	x = angle * 0.5f;
	const _Type	sintheta = sinf(x);
	const _Type	costheta = cosf(x);
	return TQuaternion<_Type>(axis * sintheta, costheta);
}

//----------------------------------------------------------------------------

template<typename _Type>
template<typename _Type2>
TVector<_Type2, 3>	TQuaternion<_Type>::RotateVector(const TVector<_Type2, 3> &v) const
{
	const _Type				r = m_Real;
	const AxisVectorType	&u = *reinterpret_cast<const AxisVectorType*>(m_Imag);

#if 1	// two cross products, but less muls -> faster in scalar mode
	const TVector<_Type2, 3>	uv = u.Cross(v);
	const TVector<_Type2, 3>	uuv = u.Cross(uv);
	return uv * (r + r) + (v + (uuv + uuv));	// v + uv * (2 * m_Real) + uuv * 2
#else
	const TVector<_Type2, 3>	uv = u.Cross(v);
	const _Type					r2 = r * r;
	const _Type					u2 = u.Dot(v);
	return uv * (r + r) + u * (u2 + u2) + v * (r2 + r2 - 1.0f);
#endif
}

//----------------------------------------------------------------------------

template<typename _Type>
void	TQuaternion<_Type>::RotateAroundAxis(const AxisVectorType &axis, _Type angle)
{
	const SelfType	aa = FromAxisAngle(axis, angle);
	aa.Mult(*this, *this);
}

//----------------------------------------------------------------------------

template<typename _Type>
void	TQuaternion<_Type>::RotateAroundUnitAxis(const AxisVectorType &axis, _Type angle)
{
	const SelfType	aa = FromUnitAxisAngle(axis, angle);
	aa.Mult(*this, *this);
}

//----------------------------------------------------------------------------
// we will flip ourself if we are at more than 180 degrees with otherQuat

template<typename _Type>
void	TQuaternion<_Type>::AlignWith(const SelfType &otherQuat)
{
	// we need to flip ourself if:
	//	(this + other).Length < (this - other).Length
	//	(this + other).Length - (this - other).Length < 0
	//	sqr(x + ox) + sqr(y + oy) + sqr(z + oz) + sqr(w + ow) - sqr(x - ox) - sqr(y - oy) - sqr(z - oz) - sqr(w - ow) < 0
	//	sqr(x + ox) - sqr(x - ox) + sqr(y + oy) - sqr(y - oy) + sqr(z + oz) - sqr(z - oz) + sqr(w + ow) - sqr(w - ow) < 0)
	//		(x + ox)^2 - (x - ox)^2
	//		x^2 + 2*x*ox + ox^2 - (x^2 - 2*x*ox + ox^2)
	//		(x^2 - x^2) + (2*x*ox + 2*x*ox) + (ox^2 - ox^2)
	//		4*x*ox
	//	4*x*ox + 4*y*oy + 4*z*oz + 4*w*ow < 0
	//	4*(x*ox + y*oy + z*oz + w*ow) < 0
	//	x*ox + y*oy + z*oz + w*ow < 0
	//	x.Dot(ox) < 0	<--- !

	if (Dot(otherQuat) < 0)
	{
		operator - ();
	}
}

//----------------------------------------------------------------------------

template<typename _Type>
_Type	TQuaternion<_Type>::ErrorBetween(const SelfType &q0, const SelfType &q1)
{
	const _Type	costheta = q0.Dot(q1);
	return (HHAbs(costheta) < 0.9999999f) ? acosf(costheta) * (1.0f / TNumericConstants<float>::Pi()) : 0.0f;
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::Slerp(const SelfType &q0, const SelfType &q1, float frac)
{
	// Get cosine of angle between quats.
	const _Type	costheta = q0.Dot(q1);
	if (costheta < 1.0f)
	{
		const _Type	theta = acosf(costheta);
		const _Type	sininv = HHRcp(sinf(theta));
//		const _Type	sininv = HHRSqrt(1.0f - costheta * costheta);
		const _Type	scale0 = sinf((1.0f - frac) * theta) * sininv;
		const _Type	scale1 = sinf(frac * theta) * sininv;
		return TQuaternion<_Type>(scale0 * q0 + q1 * scale1);
	}
	else
		return q0;
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::SlerpSafe(const SelfType &q0, const SelfType &q1, float frac)
{
	SelfType	aligned = q0;
	aligned.AlignWith(q1);
	return Slerp(aligned, q1, frac);
}

//----------------------------------------------------------------------------
#if 0
template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::Squad(const SelfType &prev, const SelfType &q0, const SelfType &q1, const SelfType &next, float frac)
{
	const SelfType	tmp0 = SlerpSafe(prev, next, frac);
	const SelfType	tmp1 = SlerpSafe(q0, q1, frac);
	return Slerp(tmp0, tmp1, 2.0f * frac * (1.0f - frac));
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::Log() const
{
	const _Type	theta = acosf(HHMin(m_Real, 1.0f));
	if (theta == 0)
		return SelfType(m_Imag[0], m_Imag[1], m_Imag[2], 0);

	const _Type	sintheta = sinf(theta);
	_Type	k;
	if (HHAbs(sintheta) < 1.0f && HHAbs(theta) >= TNumericTraits<_Type>::Max() * HHAbs(sintheta))
		k = 0;
	else
		k = theta / sintheta;

	return SelfType(m_Imag[0] * k, m_Imag[1] * k, m_Imag[2] * k, 0.0f);
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::Exp() const
{
	const _Type	theta = reinterpret_cast<const AxisVectorType*>(m_Imag)->Length();
	const _Type	sintheta = sinf(theta);

	_Type k;
	if (HHAbs(theta) < 1 && HHAbs(sintheta) >= TNumericTraits<_Type>::Max() * HHAbs(theta))
		k = 0;
	else
		k = sintheta / theta;

	const _Type	costheta = cosf(theta);
	return SelfType(m_Imag[0] * k, m_Imag[1] * k, m_Imag[2] * k, costheta);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE TQuaternion<_Type>	TQuaternion<_Type>::_Intermediate(const SelfType &q0, const SelfType &q1, const SelfType &q2)
{
	const SelfType	q1inv = q1.Inverse();
	const SelfType	c1 = q1inv * q2;
	const SelfType	c2 = q1inv * q0;
	const SelfType	c3 = ((_Type)(-0.25)) * (c2.Log() + c1.Log());
	const SelfType	qa = q1 * c3.Exp();
	return qa.Normalized();
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::SplineLerp(const SelfType &prev, const SelfType &q0, const SelfType &q1, const SelfType &next, float frac)
{
	const SelfType	qa = _Intermediate(prev, q0, q1);
	const SelfType	qb = _Intermediate(q0, q1, next);
	return Squad(q0, qa, qb, q1, frac);
}
#endif
//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::Nlerp(const SelfType &q0, const SelfType &q1, float frac)
{
	SelfType	result(q0 + frac * (q1 - q0));
	result.Normalize();
	return result;
}

//----------------------------------------------------------------------------

template<typename _Type>
TQuaternion<_Type>	TQuaternion<_Type>::NlerpSafe(const SelfType &q0, const SelfType &q1, float frac)
{
	SelfType	aligned = q0;
	aligned.AlignWith(q1);
	return Nlerp(aligned, q1, frac);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_QUATERNION_INL__
