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

#ifndef	__GE_COLLIDERS_H__
#define	__GE_COLLIDERS_H__

#include "hh_maths/include/hh_maths_primitives.h"
#include "hh_geometrics/include/ge_config.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
// forward declares

template<class _VectorType> class	TPrimitivesPlanar;
typedef TPrimitivesPlanar<CFloat3>	CPlane;

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CFastRay
{
private:
	CFloat4		m_Origin;
	CFloat4		m_Direction;
	CFloat4		m_InverseDirection;
	float		m_Length;
	hh_u32		m_Octant;

	// 111 -> mmm, 010 -> pmp, 000 -> ppp, ...
	HH_FORCEINLINE hh_u32	_BuildOctant(const CFloat3 &direction)
	{
		const hh_u32	*fpBits = (const hh_u32*)&direction;
		return (((fpBits[0] >> 29) & 0x4) | ((fpBits[1] >> 30) & 0x2) | (fpBits[2] >> 31));
	}

public:
	CFastRay(const CRay &fromRay)
	:	m_Origin(fromRay.Origin().xyz1())
	,	m_Direction(fromRay.Direction().xyz1())
	,	m_Length(fromRay.Length())
	,	m_Octant(_BuildOctant(fromRay.Direction()))
	{
		m_InverseDirection = 1.0f / m_Direction;	// we don't care about divides by zero, we will get infinities, that's what we actually want.
		//m_Direction.w() = 0.0f;
	}

	HH_FORCEINLINE const CFloat4	&Origin() const { return m_Origin; }
	HH_FORCEINLINE const CFloat4	&Direction() const { return m_Direction; }
	HH_FORCEINLINE const CFloat4	&InverseDirection() const { return m_InverseDirection; }
	HH_FORCEINLINE float			Length() const { return m_Length; }
	HH_FORCEINLINE hh_u32			Octant() const { return m_Octant; }

	void							SetLength(float newLength) { m_Length = newLength; }
	void							SlideRay(float slideBy) { m_Origin.xyz() = (m_Origin + m_Direction * slideBy).xyz(); }
};

//----------------------------------------------------------------------------

namespace	Colliders
{
	struct	STriangleRef : public CNonCopyable
	{
		const CFloat3	&Vert0;
		const CFloat3	&Vert1;
		const CFloat3	&Vert2;
		STriangleRef(const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2) : Vert0(v0), Vert1(v1), Vert2(v2) {}
	};

#if 0
	HH_GEOMETRICS_EXPORT bool	BoxOverlapsTriangle(const CFloat3 &boxCenter, const CFloat3 &boxHalfExtent, const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2);
	HH_FORCEINLINE bool			BoxOverlapsTriangle(const CFloat3 &boxCenter, const CFloat3 &boxHalfExtent, const STriangleRef &triangle) { return BoxOverlapsTriangle(boxCenter, boxHalfExtent, triangle.Vert0, triangle.Vert1, triangle.Vert2); }
	HH_FORCEINLINE bool			BoxOverlapsTriangle(const CAABB &box, const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2) { return BoxOverlapsTriangle(box.Center(), 0.5f * box.Extent(), v0, v1, v2); }
	HH_FORCEINLINE bool			BoxOverlapsTriangle(const CAABB &box, const STriangleRef &triangle) { return BoxOverlapsTriangle(box, triangle.Vert0, triangle.Vert1, triangle.Vert2); }
#endif

	//HH_GEOMETRICS_EXPORT bool	BoxOverlapsBox(const CAABB &box0, const CFloat3x3 &tr0, const CAABB &box1, const CFloat3x3 &tr1);
	//HH_GEOMETRICS_EXPORT bool	BoxSweepBox(const CAABB &box0, const CFloat3x3 &tr0, const CAABB &box1, const CFloat3x3 &tr1, const CFloat3 &sweepDir, float sweepLength, TPrimitiveHitReport3D &report);

	struct	HH_GEOMETRICS_EXPORT SRayPacket
	{
		CAABB						m_RayBounds;
		TMemoryView<const CFloat4>	m_RayOrigins_Aligned16;
		TMemoryView<const CFloat4>	m_RayDirectionsAndLengths_Aligned16;
		TMemoryView<const float>	m_RaySweepRadii_Aligned16;

		SRayPacket(const TMemoryView<const CFloat4> &rayOriginsAligned16, const TMemoryView<const CFloat4> &rayDirectionsAndLengthsAligned16);
		SRayPacket(const TMemoryView<const CFloat4> &rayOriginsAligned16, const TMemoryView<const CFloat4> &rayDirectionsAndLengthsAligned16, const CAABB &bbox);
		bool		Empty() const { return m_RayOrigins_Aligned16.Empty(); }
	};

	struct	STraceResult
	{
		TPrimitiveHitReport3D	m_BasicReport;
		CFloat2					m_UV;

		STraceResult()
		{
			HH_ONLY_IN_DEBUG(m_UV = TNumericTraits<CFloat2>::NaN());	// fill with garbage in debug... the user should take care of initialising this
		}
	};

	struct	HH_GEOMETRICS_EXPORT STracePacket
	{
		hh_u32					m_PacketSize;
		float					*m_HitTimes_Aligned16;				// cannot be null
		CFloat4					*m_ContactPoints_Aligned16;			// can be null
		CFloat4					*m_ContactNormals_Aligned16;		// can be null
		CInt4					*m_ContactPCoords_Aligned16;		// can be null
		void					**m_ContactSurfaces_Aligned16;		// can be null
		void					**m_ContactObjects_Aligned16;		// can be null
#ifdef	HH_TRACE_DEBUG_ENABLED
		hh_u32					*m_DebugKeys;						// can be null
#endif

		HH_FORCEINLINE STracePacket()
		:	m_PacketSize(0)
		,	m_HitTimes_Aligned16(null)
		,	m_ContactPoints_Aligned16(null)
		,	m_ContactNormals_Aligned16(null)
		,	m_ContactPCoords_Aligned16(null)
		,	m_ContactSurfaces_Aligned16(null)
		,	m_ContactObjects_Aligned16(null)
#ifdef	HH_TRACE_DEBUG_ENABLED
		,	m_DebugKeys(null)
#endif
		{
		}

		HH_FORCEINLINE bool		Empty() const { return m_PacketSize == 0; }
		HH_FORCEINLINE hh_u32	Count() const { return m_PacketSize; }
		bool					Valid() const;
	};

	struct	HH_GEOMETRICS_EXPORT STriangleTraceAccelerator
	{
		CFloat3	planeNormal;
		CFloat3	vertex0;
		CFloat3	edge0;
		CFloat3	edge1;
		float	a;
		float	b;
		float	c;
		float	ac_bb;

		STriangleTraceAccelerator(const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2);
	};


	HH_GEOMETRICS_EXPORT bool	RayTraceTriangle(const CRay &ray, const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2, STraceResult &hit);
	HH_GEOMETRICS_EXPORT bool	RayTraceTriangle(const CRay &ray, const STriangleTraceAccelerator &triangle, STraceResult &hit);
	HH_GEOMETRICS_EXPORT bool	RayTraceCulledTriangle(const CRay &ray, const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2, STraceResult &hit);
	HH_GEOMETRICS_EXPORT bool	RayTraceCulledTriangle(const CRay &ray, const STriangleTraceAccelerator &triangle, STraceResult &hit);
#if 0
	HH_GEOMETRICS_EXPORT bool	RayTraceTriangleUV(const CRay &ray, const CFloat3 &v0, const CFloat3 &v1, const CFloat3 &v2, CFloat2 &outHitUV);
	HH_GEOMETRICS_EXPORT bool	RayTraceTriangleUV(const CRay &ray, const STriangleTraceAccelerator &triangle, CFloat2 &outHitUV);
#endif

	// bool			AOverlapsB(A, B)
	// hh_quanta	AContainsB(A, B)

#if 0
	HH_FORCEINLINE bool			AOverlapsB(const CAABB &box, const STriangleRef &triangle) { return BoxOverlapsTriangle(box, triangle); }
	HH_FORCEINLINE bool			AOverlapsB(const STriangleRef &triangle, const CAABB &box) { return BoxOverlapsTriangle(box, triangle); }
#endif

	// computes the intersection line of two planes 'p0' and 'p1'. if the planes don't intersect, false is returned, and the output's contents are undefined
	HH_GEOMETRICS_EXPORT bool	IntersectPlanes(const CPlane &p0, const CPlane &p1, CFloat3 &intersection, CFloat3 &axis);

	// computes the intersection point of 3 planes. if the planes don't intersect, false is returned, and the output's contents are undefined
	HH_GEOMETRICS_EXPORT bool	IntersectPlanes(const CPlane &p0, const CPlane &p1, const CPlane &p2, CFloat3 &intersection);

	HH_GEOMETRICS_EXPORT bool	SphereSweepTriangle(const CSphere &sphere, const CFloat3 &sweep_vec, const CFloat3 *v0, const CFloat3 *v1, const CFloat3 *v2, STraceResult *hit, bool double_sided);

	HH_GEOMETRICS_EXPORT bool	ClipRayToBox(const CRay &ray, const CAABB &bbox, float &t0, float &t1);
	HH_GEOMETRICS_EXPORT bool	ClipRayToBox(const CFastRay &ray, const CAABB &bbox, float &t0, float &t1);
	HH_GEOMETRICS_EXPORT bool	ClipRayToBox_AssumeRayNotOnBoxEdge(const CFastRay &ray, const CAABB &bbox, float &t0, float &t1);	// a bit faster than the safe version
	HH_GEOMETRICS_EXPORT bool	ClipRayToBox_Slabs(const CFastRay &ray, const CAABB &bbox, float &t0, float &t1);	// similar to 'ClipRayToBox(const CRay &ray ...)'

	HH_GEOMETRICS_EXPORT bool	ClosestPointsBetweenRays(const CRay &ray0, const CRay &ray1, CFloat3 &pointA, CFloat3 &pointB);

	//----------------------------------------------------------------------------
	// ray tracing tools

	// AABB
	HH_GEOMETRICS_EXPORT bool	RayTrace(const CAABB &self, const TPrimitiveRay<float, 3> &ray, TPrimitiveHitReport3D &report);

	// Disks (used by cylinders and cones)
	template<typename _Type, hh_u32 _Axis>
	bool	RayTrace(const TDisk<_Type, _Axis> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report);

	// Spheres:
	template<typename _Type>
	bool	RayTrace(const TPrimitiveSpherical<_Type> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report);

	// Cylinders
	template<typename _Type, hh_u32 _Axis>
	bool	RayTrace(const TCylinder<_Type, _Axis> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report);

	// Cones
	template<typename _Type, hh_u32 _Axis>
	bool	RayTrace(const TCone<_Type, _Axis> &self, const TPrimitiveRay<_Type, 3> &ray, TPrimitiveHitReport<_Type, 3> &report);

	// Planes: two sided
	template<typename _Type, hh_u32 _Dimension>
	static bool	RayTrace(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray, TPrimitiveHitReport<_Type, _Dimension> &report);

	// returns hits behind the camera.
	// use TNumericTraits<_Type>::IsFinite() on the result to check if an intersection occured.
	// if you are only interested on intersections in front of the camera, checking the return value for '< 0' will also return false when no intersections occured.
	template<typename _Type, hh_u32 _Dimension>
	static _Type	RayTrace(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray);

	// Planes: culled
	template<typename _Type, hh_u32 _Dimension>
	static bool	RayTraceCulled(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray, TPrimitiveHitReport<_Type, _Dimension> &report);

	// returns a value < 0 if no hit, otherwise, the length along the ray at which the intersection took place.
	template<typename _Type, hh_u32 _Dimension>
	static _Type	RayTraceCulled(const TPrimitivesPlanar<TVector<_Type, _Dimension> > &self, const TPrimitiveRay<_Type, _Dimension> &ray);

}	// namespace Colliders

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_geometrics/include/ge_colliders.inl"

#endif	// __GE_COLLIDERS_H__

