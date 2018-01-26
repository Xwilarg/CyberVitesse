#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2009/09/14 8:56 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__GE_COLLIDERS_INL__
#define	__GE_COLLIDERS_INL__

#ifndef	__GE_COLLIDERS_H__
#	error	ge_colliders.inl must not be included manually, include ge_colliders.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// FIXME: move this elsewhere

namespace	Colliders
{
	template<typename _Type>
	bool	_SolveQuadraticEquation(_Type a, _Type b, _Type c, _Type &t0, _Type &t1)
	{
		const _Type	d = b * b - 4 * a * c;
		if (d < 0)
			return false;
		const _Type	rd = sqrt(d);
		const _Type	q1 = -0.5f * (b - rd);
		const _Type	q2 = -0.5f * (b + rd);

		const _Type	invA = 1.0f / a;
		t0 = q1 * invA;
		t1 = q2 * invA;
		if (t0 > t1)
			HHSwap(t0, t1);
		return true;
	}
}

//----------------------------------------------------------------------------
// spheres

template<typename _Type>
bool	Colliders::RayTrace(const TPrimitiveSpherical<_Type> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report)
{
	TVector<_Type, 3>	offset = self.Center() - ray.Origin();
	const _Type	dist = ray.Direction().Dot(offset);
	if (dist <= 0.0f)
		return false;

	const _Type	off2 = offset.LengthSquared();
	if (off2 <= self.RadiusSquared())	// FIXME: what should we do? we're inside the sphere...
		return false;

	const _Type	tRel = self.RadiusSquared() - (off2 - dist * dist);
	if (tRel < 0.0f)
		return false;

	const _Type	t = dist - HHSqrt(tRel);
	report.t = t;
	report.point = ray.Origin() + ray.Direction() * t;
	report.normal = (report.point - self.Center()).Normalized();
	return true;
}

//----------------------------------------------------------------------------
// Disks

template<typename _Type, hh_u32 _Axis>
bool	Colliders::RayTrace(const TDisk<_Type, _Axis> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report)
{
	static const hh_u32	_AxisX = TDisk<_Type, _Axis>::_AxisX;
	static const hh_u32	_AxisY = TDisk<_Type, _Axis>::_AxisY;

	const TVector<_Type, 3>	&ray_dir = ray.Direction();
	if (ray_dir.Axis(_Axis) == 0.0f)
		return false;
	const _Type	t = (self.m_AxialHeight - ray.Origin().Axis(_Axis)) / ray_dir.Axis(_Axis);
	if (t < 0)
		return false;
	const TVector<_Type, 3>	contactPoint = ray.Origin() + ray_dir * t;
	const TVector<_Type, 2>	vec(contactPoint.Axis(_AxisX), contactPoint.Axis(_AxisY));
	if (vec.LengthSquared() >= self.m_Radius * self.m_Radius)
		return false;

	report.t = t;
	report.point = contactPoint;
	report.normal = TVector<_Type, 3>(0);
	report.normal.Axis(_Axis) = ray_dir.Axis(_Axis) < 0 ? 1 : -1;	// FIXME: just extract inverse sign
	return true;
}

//----------------------------------------------------------------------------
// Cylinders

template<typename _Type, hh_u32 _Axis>
bool	Colliders::RayTrace(const TCylinder<_Type, _Axis> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report)
{
	static const hh_u32	_AxisX = TCylinder<_Type, _Axis>::_AxisX;
	static const hh_u32	_AxisY = TCylinder<_Type, _Axis>::_AxisY;

	const TVector<_Type, 3>	&ray_dir = ray.Direction();
	const TVector<_Type, 3>	&ray_pos = ray.Origin();

	const _Type	a = HHSquared(ray_dir.Axis(_AxisX)) + HHSquared(ray_dir.Axis(_AxisY));
	const _Type	b = 2.0f * (ray_dir.Axis(_AxisX) * ray_pos.Axis(_AxisX) + ray_dir.Axis(_AxisY) * ray_pos.Axis(_AxisY));
	const _Type	c = HHSquared(ray_pos.Axis(_AxisX)) + HHSquared(ray_pos.Axis(_AxisY)) - self.m_Radius * self.m_Radius;
	_Type		t0;
	_Type		t1;
	if (!_SolveQuadraticEquation(a, b, c, t0, t1))
		return false;

	const TVector<_Type, 3>	contactPoint = ray_pos + ray_dir * t0;
	if (contactPoint.Axis(_Axis) < self.m_ZMin)
	{
		TDisk<_Type, _Axis>	cap(self.m_Radius, self.m_ZMin);
		return RayTrace(cap, ray, report);
	}
	else if (contactPoint.Axis(_Axis) > self.m_ZMax)
	{
		TDisk<_Type, _Axis>	cap(self.m_Radius, self.m_ZMax);
		return RayTrace(cap, ray, report);
	}

	report.t = t0;
	TVector<_Type, 3>	normal = contactPoint;
	normal.Axis(_Axis) = 0.0f;
	report.normal = normal.Normalized();
	report.point = contactPoint;
	return true;
}

//----------------------------------------------------------------------------
// Cones

template<typename _Type, hh_u32 _Axis>
bool	Colliders::RayTrace(const TCone<_Type, _Axis> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report)
{
	static const hh_u32	_AxisX = TCone<_Type, _Axis>::_AxisX;
	static const hh_u32	_AxisY = TCone<_Type, _Axis>::_AxisY;

	if (ray.Origin().Axis(_Axis) < self.m_AxialHeight)
	{
		TDisk<_Type, _Axis>	cap(self.m_Radius, self.m_AxialHeight);
		if (RayTrace(cap, ray, report))
			return true;
	}

	const TVector<_Type, 3>	&ray_dir = ray.Direction();
	const TVector<_Type, 3>	&ray_pos = ray.Origin();

	const _Type	dx = ray_dir.Axis(_AxisX);
	const _Type	dy = ray_dir.Axis(_AxisY);
	const _Type	dz = ray_dir.Axis(_Axis);
	const _Type	ox = ray_pos.Axis(_AxisX);
	const _Type	oy = ray_pos.Axis(_AxisY);
	const _Type	oz = ray_pos.Axis(_Axis) - (self.m_AxialHeight + self.m_Height);
	const _Type	hr2 = HHSquared(self.m_Height / self.m_Radius);

	//xD2+yD2-zD2, b=2xExD+2yEyD-2zEzD, and c=xE2+yE2-zE2. -> E = eye, D = direction

	const _Type	a = hr2 * (dx * dx + dy * dy) - dz * dz;
	const _Type	b = 2.0f * (hr2 * (ox * dx + oy * dy) - oz * dz);
	const _Type	c = hr2 * (ox * ox + oy * oy) - oz * oz;
	_Type		t0;
	_Type		t1;
	if (!_SolveQuadraticEquation(a, b, c, t0, t1))
		return false;

	TVector<_Type, 3>	contactPoint = ray_pos;
	_Type				z = (oz + (self.m_AxialHeight + self.m_Height)) + dz * t0;
	if (z >= self.m_AxialHeight && z <= self.m_AxialHeight + self.m_Height)
		contactPoint += ray_dir * t0;
	else
	{
		z = (oz + (self.m_AxialHeight + self.m_Height)) + dz * t1;
		if (z >= self.m_AxialHeight && z <= self.m_AxialHeight + self.m_Height)
			contactPoint += ray_dir * t1;
		else
			return false;
	}

	report.t = t0;
	TVector<_Type, 3>	normal = contactPoint;
	normal.Axis(_Axis) = 0.0f;
	normal.Normalize();
	normal.Axis(_Axis) = self.m_Radius / self.m_Height;
	report.normal = normal.Normalized();	// can't we get away without the double normalize? (Julien): YES we can. change this.
	report.point = contactPoint;
	return true;
}

//----------------------------------------------------------------------------
// Planes: two sided

template<typename _Type, hh_u32 _Dimension>
static bool	Colliders::RayTrace(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray, TPrimitiveHitReport<_Type, _Dimension> &report)
{
	const _Type	len = -ray.Direction().Dot(self.Normal());
	if (HHAbs(len) < _Type(1.0e-6))
		return false;	// parallel to plane

	const _Type	dist = ray.Origin().Dot(self.Normal()) + self.DistanceToOrigin();
	const _Type	t = dist / len;
	if (t < 0.0f)
		return false;

	report.t = t;
	report.point = ray.Origin() + ray.Direction() * t;
	report.normal = self.Normal();
	return true;
}

//----------------------------------------------------------------------------
// returns hits behind the camera.
// use TNumericTraits<_Type>::IsFinite() on the result to check if an intersection occured.
// if you are only interested on intersections in front of the camera, checking the return value for '< 0' will also return false when no intersections occured.

template<typename _Type, hh_u32 _Dimension>
static _Type	Colliders::RayTrace(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray)
{
	const _Type	len = -ray.Direction().Dot(self.Normal());
	if (HHAbs(len) < _Type(1.0e-6))
		return TNumericTraits<_Type>::kNegativeInfinity;	// parallel to plane
	const _Type	dist = ray.Origin().Dot(self.Normal()) + self.DistanceToOrigin();
	return dist / len;
}

//----------------------------------------------------------------------------
// Planes: culled

template<typename _Type, hh_u32 _Dimension>
static bool	Colliders::RayTraceCulled(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray, TPrimitiveHitReport<_Type, _Dimension> &report)
{
	const _Type	dist = ray.Origin().Dot(self.Normal()) + self.DistanceToOrigin();
	if (dist < 0)
		return false;

	const _Type	len = -ray.Direction().Dot(self.Normal());
	if (len < _Type(1.0e-6))
		return false;	// parallel to plane

	const _Type	t = dist / len;
	report.t = t;
	report.point = ray.Origin() + ray.Direction() * t;
	report.normal = self.Normal();
	return true;
}

//----------------------------------------------------------------------------
// returns a value < 0 if no hit, otherwise, the length along the ray at which the intersection took place.

template<typename _Type, hh_u32 _Dimension>
static _Type	Colliders::RayTraceCulled(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray)
{
	const _Type	dist = ray.Origin().Dot(self.Normal()) + self.DistanceToOrigin();
	if (dist < 0)
		return -1;

	const _Type	len = -ray.Direction().Dot(self.Normal());
	if (len < _Type(1.0e-6))	// avoid 0/0
		return -1;	// parallel to plane
	return dist / len;
}

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __GE_COLLIDERS_INL__

