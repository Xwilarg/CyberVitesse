#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2009/04/07 4:14 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_PRIMITIVES_H__
#define __PK_MATHS_PRIMITIVES_H__

#include "hh_maths/include/hh_maths.h"
#include "hh_maths/include/hh_maths_matrix.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimensions>
struct	TPrimitiveHitReport
{
	TVector<_Type, _Dimensions>	point;
	TVector<_Type, _Dimensions>	normal;
	_Type						t;

	TPrimitiveHitReport()
	:	t(TNumericTraits<float>::kPositiveInfinity)
	{
		HH_ONLY_IN_DEBUG(point = CFloat3(TNumericTraits<float>::NaN()));	// fill with garbage in debug... the user should take care of initialising this
		HH_ONLY_IN_DEBUG(normal = CFloat3(TNumericTraits<float>::NaN()));
	}
};

//----------------------------------------------------------------------------

typedef	TPrimitiveHitReport<float, 3>	TPrimitiveHitReport3D;

//----------------------------------------------------------------------------
//
//	Rays
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimensions>
class	TPrimitiveRay
{
public:
	typedef TPrimitiveRay<_Type, _Dimensions>	SelfType;
	typedef TVector<_Type, _Dimensions>			VectorType;
private:
	VectorType	m_Origin;
	VectorType	m_Direction;
	_Type		m_Length;
public:
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	INVALID;

	HH_INLINE TPrimitiveRay()
	:	m_Length(TNumericTraits<_Type>::kNegativeInfinity)	// don't initialize the other members, only m_Length is required to be invalid for 'Valid()' to return false
	{
		HH_ONLY_IN_DEBUG(m_Origin = CFloat3(TNumericTraits<float>::NaN()));	// fill with garbage in debug... the user should take care of initialising this
		HH_ONLY_IN_DEBUG(m_Direction = CFloat3(TNumericTraits<float>::NaN()));
	}

	TPrimitiveRay(const VectorType &origin, const VectorType &direction)
	:	m_Origin(origin)
	,	m_Direction(direction)
	,	m_Length(TNumericTraits<_Type>::kPositiveInfinity)
	{
		HH_ASSERT(m_Direction.IsNormalized());
	}

	TPrimitiveRay(const VectorType &origin, const VectorType &direction, _Type length)
	:	m_Origin(origin)
	,	m_Direction(direction)
	,	m_Length(length)
	{
		HH_ASSERT(m_Direction.IsNormalized());
	}

	~TPrimitiveRay() {}

	static SelfType	FromStartAndEnd(const VectorType &start, const VectorType &end)
	{
		const CFloat3	origin = start;
		CFloat3			direction = end - start;
		const float		length = direction.Length();

		if (length > 1.0e-10f)
			direction *= 1.0f / length;
		else
			direction = VectorType::XAXIS;
		return SelfType(origin, direction, length);
	}

	HH_FORCEINLINE bool				Valid() const { return m_Length >= 0.0f; }
	HH_FORCEINLINE const VectorType	&Origin() const { return m_Origin; }
	HH_FORCEINLINE VectorType		&Origin() { return m_Origin; }
	HH_FORCEINLINE const VectorType	&Direction() const { return m_Direction; }
	HH_FORCEINLINE bool				Infinite() const { return m_Length == TNumericTraits<_Type>::kPositiveInfinity; }
	HH_FORCEINLINE _Type			Length() const { return m_Length; }
	HH_FORCEINLINE void				SetLength(_Type length) { m_Length = length; }
	void							SlideRay(_Type slideBy) { m_Origin += m_Direction * slideBy; }

	void	SetDirection(const VectorType &direction)
	{
		m_Direction = direction;
		m_Direction.Normalize();
	}
	void	SetDirectionAndLength(const VectorType &direction, _Type length)
	{
		m_Direction = direction;
		m_Direction.Normalize();
	}
	void	SetMoveVec(const VectorType &move)
	{
		m_Length = move.Length();
		if (m_Length > 1.0e-10f)
			m_Direction = move * (1.0f / m_Length);
		else
		{
			m_Direction = VectorType::ZERO;
			m_Direction.Axis(0) = 1.0f;
		}
	}
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
HH_ALIGN(0x10) const TPrimitiveRay<_Type, _Dimension>	TPrimitiveRay<_Type, _Dimension>::ZERO =
TPrimitiveRay<_Type, _Dimension>(TVector<_Type, _Dimension>(0), TVector<_Type, _Dimension>(1,0,0), TNumericTraits<_Type>::Zero());

template<typename _Type, hh_u32 _Dimension>
HH_ALIGN(0x10) const TPrimitiveRay<_Type, _Dimension>	TPrimitiveRay<_Type, _Dimension>::INVALID = TPrimitiveRay<_Type, _Dimension>();

//----------------------------------------------------------------------------

typedef TPrimitiveRay<float, 3>	CRay;

//----------------------------------------------------------------------------
//
//	Spheres
//
//----------------------------------------------------------------------------

template<typename _Type>	// FIXME: take this into account to ignore the center and speed-up some computations (in particular, the trace)
class	TPrimitiveSpherical
{
public:
	typedef TVector<_Type, 3>			VectorType;
	typedef TPrimitiveSpherical<_Type>	SelfType;
private:
	VectorType							m_Center;
	_Type								m_Radius;
	_Type								m_SquaredRadius;

public:
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	kINFINITE;

	TPrimitiveSpherical() : m_Center(0), m_Radius(0), m_SquaredRadius(0) {}
	TPrimitiveSpherical(_Type radius) : m_Center(0), m_Radius(radius), m_SquaredRadius(radius * radius) {}
	TPrimitiveSpherical(const VectorType &center, _Type radius) : m_Center(center), m_Radius(radius), m_SquaredRadius(radius * radius) {}
	~TPrimitiveSpherical() {}

	// accessors
	void							SetRadius(_Type radius) { m_Radius = radius; m_SquaredRadius = radius * radius; }
	HH_FORCEINLINE _Type			Radius() const { return m_Radius; }
	HH_FORCEINLINE _Type			RadiusSquared() const { return m_SquaredRadius; }

	void							SetCenter(const VectorType &center) { m_Center = center; }
	void							SetCenter(const VectorType *center) { m_Center = *center; }
	void							SetCenter(_Type broadcastCenter) { m_Center = VectorType(broadcastCenter); }
	void							SetCenter(_Type x, _Type y, _Type z) { m_Center = VectorType(x, y, z); }
	HH_FORCEINLINE const VectorType	&Center() const { return m_Center; }
	HH_FORCEINLINE VectorType		&Center() { return m_Center; }

	// WARNING: don't use unless you know what you are doing...
	void							_SetSquaredRadiusUnsafe(_Type squaredRadius) { m_SquaredRadius = squaredRadius; }
	void							_ValidateUnsafeSquaredRadius() { m_Radius = HHSqrt(m_SquaredRadius); }

	void	Add(const VectorType &point)
	{
		const VectorType	extent = m_Center - point;
		const _Type			lenSq = extent.LengthSquared();
		if (lenSq > m_SquaredRadius)
		{
			const _Type	len = HHSqrt(lenSq);
			const _Type	newRadius = 0.5f * (len + m_Radius);
			SetRadius(newRadius);
			m_Center = point + extent * (newRadius / len);
		}
	}

	void	Add(const SelfType &spheroid) { HH_ASSERT_NOT_REACHED(); }	// FIXME: implement

//	static SelfType	Merge(const SelfType &a, const SelfType &b) { HH_ASSERT_NOT_IMPLEMENTED(); return a; }	// FIXME: implement
};

//----------------------------------------------------------------------------

template<typename _Type>
HH_ALIGN(0x10) const TPrimitiveSpherical<_Type>	TPrimitiveSpherical<_Type>::ZERO = TPrimitiveSpherical<_Type>(TNumericTraits<_Type>::Zero());

template<typename _Type>
HH_ALIGN(0x10) const TPrimitiveSpherical<_Type>	TPrimitiveSpherical<_Type>::kINFINITE = TPrimitiveSpherical<_Type>(TNumericTraits<_Type>::Infinity());

//----------------------------------------------------------------------------

typedef TPrimitiveSpherical<float>	CSphere;

//----------------------------------------------------------------------------
//
//	Point cloud tools
//
//----------------------------------------------------------------------------

template<typename _Type>
struct	TPointCloudTools
{
	static void	MergeMinMax(const TStridedMemoryView<const _Type> &pointList, _Type &outMin, _Type &outMax)
	{
		typename TStridedMemoryView<const _Type>::ConstIterator	it = pointList.Begin();
		typename TStridedMemoryView<const _Type>::ConstIterator	end = pointList.End();

		for (; it < end; ++it)
		{
			const _Type	&p = *it;
			outMin = HHMin(outMin, p);
			outMax = HHMax(outMax, p);
		}
	}
};

//----------------------------------------------------------------------------

template<>
struct	TPointCloudTools<CFloat3>
{
	static void				MergeMinMax(const TStridedMemoryView<const CFloat3> &pointList, CFloat3 &outMin, CFloat3 &outMax);
};

//----------------------------------------------------------------------------
//
//	AABBs
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimensions>
class	TPrimitiveAABB;

template<typename _Type, hh_u32 _Dimensions>
void	PKSwapEndianInPlace(TPrimitiveAABB<_Type, _Dimensions> &object);

//----------------------------------------------------------------------------
//
//	AABB class
//
//----------------------------------------------------------------------------

namespace	Internal
{
	template<hh_u32 _Count>
	struct	SCornerExtractor
	{
		template<typename SelfType, typename VectorType>
		static void	RecursiveExtractCorners(const SelfType &box, VectorType *output)
		{
			HH_STATIC_ASSERT(_Count != 0);
			SCornerExtractor<_Count - 1>::RecursiveExtractCorners(box, output);
			output[_Count - 1] = box.template ExtractCorner<_Count - 1>();
		}
	};

	template<>
	struct	SCornerExtractor<1>
	{
		template<typename SelfType, typename VectorType>
		static void	RecursiveExtractCorners(const SelfType &box, VectorType *output)
		{
			output[0] = box.template ExtractCorner<0>();
		}
	};
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimensions>
class	TPrimitiveAABB
{
public:
	typedef TPrimitiveAABB<_Type, _Dimensions>			SelfType;
	typedef TVector<_Type, _Dimensions>					VectorType;
	static const hh_u32									Dimensions = _Dimensions;

private:
	VectorType		m_Limits[2];

	friend void		PKSwapEndianInPlace<_Type, _Dimensions>(TPrimitiveAABB<_Type, _Dimensions> &object);

	VectorType		_SpecializedExtent() const { return m_Limits[1] - m_Limits[0]; }

	static _Type	_HalfAreaFromExtent(const TVector<_Type, 2> &extent)
	{
		return extent.x() * extent.y();	// we assume a rect is two-sided. otherwise, this will be the full area
	}
	static _Type	_HalfAreaFromExtent(const TVector<_Type, 3> &extent)
	{
		return extent.x() * extent.y() + extent.y() * extent.z() + extent.z() * extent.x();
	}

	friend void		PKSwapEndianInPlace<_Type, _Dimensions>(TPrimitiveAABB<_Type, _Dimensions> &object);

	template <typename _VecType>
	HH_FORCEINLINE static _VecType	_VMin(const _VecType &v0, const _VecType &v1) { return HHMin(v0, v1); }
	template <typename _VecType>
	HH_FORCEINLINE static _VecType	_VMax(const _VecType &v0, const _VecType &v1) { return HHMax(v0, v1); }

	_Type							_AxialExtent(hh_u32 axis) const { HH_ASSERT(axis <= _Dimensions); return m_Limits[1].Axis(axis) - m_Limits[0].Axis(axis); }

public:
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	kINFINITE;
	HH_ALIGN(0x10) static const SelfType	DEGENERATED;
//	HH_ALIGN(0x10) static const SelfType	DEGENERATED_INFINITE;
	HH_ALIGN(0x10) static const SelfType	NORMALIZED_01;
	HH_ALIGN(0x10) static const SelfType	NORMALIZED_M1P1;

	TPrimitiveAABB() {}
	TPrimitiveAABB(const VectorType &minMax) { m_Limits[0] = minMax; m_Limits[1] = minMax; }
	TPrimitiveAABB(const VectorType &min, const VectorType &max) { SetMinMax(min, max); }
	template<typename _Type2>
	TPrimitiveAABB(const TPrimitiveAABB<_Type2, _Dimensions> &other) { SetMinMax(other.Min(), other.Max()); }
	~TPrimitiveAABB() {}

	template<typename _Type2>
	static SelfType		FromRadius(_Type2 radius) { return SelfType(VectorType(-radius), VectorType(radius)); }
	template<typename _Type2>
	static SelfType		FromRadius(const TVector<_Type2, _Dimensions> &radiusVec) { return SelfType(-radiusVec, radiusVec); }

	static SelfType		FromMinMax(const VectorType &min, const VectorType &max) { return SelfType(min, max); }
	static SelfType		FromMinMaxUnchecked(const VectorType &min, const VectorType &max) { SelfType box; return box.SetMinMaxUnchecked(min, max); }

	HH_FORCEINLINE const VectorType	&Min() const { return m_Limits[0]; }
	HH_FORCEINLINE const VectorType	&Max() const { return m_Limits[1]; }
	HH_FORCEINLINE VectorType		&Min() { return m_Limits[0]; }
	HH_FORCEINLINE VectorType		&Max() { return m_Limits[1]; }

	_Type				Width()  const { return _AxialExtent(0); }
	_Type				Height() const { return _AxialExtent(1); }
	_Type				Depth() const {	HH_STATIC_ASSERT(_Dimensions > 2); return _AxialExtent(2); }

	bool				Empty() const { return _SpecializedExtent().AxialProduct() == 0; }
	bool				Valid() const { return All(m_Limits[0] <= m_Limits[1]); }
	bool				IsFinite() const { return m_Limits[0].IsFinite() && m_Limits[1].IsFinite(); }

	VectorType			Extent() const { return _SpecializedExtent(); };
	VectorType			Center() const { return (m_Limits[0] + m_Limits[1]) * 0.5f; }
	_Type				Area() const { return 2.0f * HalfArea(); }
	_Type				HalfArea() const { return _HalfAreaFromExtent(_SpecializedExtent()); }
	_Type				Volume() const
	{
		HH_STATIC_ASSERT_CUSTOM(_Dimensions >= 3, "AABB_Does_Not_Have_Enough_Dimensions_To_Have_A_Meaningful_Volume_Method");
		return _SpecializedExtent().AxialProduct();
	}

	void				Add(const VectorType &point) { m_Limits[0] = _VMin(m_Limits[0], point); m_Limits[1] = _VMax(m_Limits[1], point); }
	void				Add(const SelfType &box) { m_Limits[0] = _VMin(m_Limits[0], box.m_Limits[0]); m_Limits[1] = _VMax(m_Limits[1], box.m_Limits[1]); }
	void				Add(const TStridedMemoryView<const VectorType> &pointList)
	{
		if (!pointList.Empty())
			TPointCloudTools<VectorType>::MergeMinMax(pointList, m_Limits[0], m_Limits[1]);
	}

	static HH_INLINE SelfType	Merge(const SelfType &a, const SelfType &b) { SelfType out(a); out.Add(b); return out; }	// don't merge these two in a single template
	static HH_INLINE SelfType	Merge(const SelfType &a, const VectorType &b) { SelfType out(a); out.Add(b); return out; }
	static HH_INLINE SelfType	Merge(const VectorType &a, const SelfType &b) { return Merge(b, a); }

	void		MoveTo(const VectorType &pt) { m_Limits[1] = pt + _SpecializedExtent(); m_Limits[0] = pt; }
	template<typename _FactorType>
	void		ScaleFromCenter(_FactorType factor)
	{
		VectorType			halfExtent = _SpecializedExtent() * 0.5f;
		const VectorType	center = m_Limits[0] + halfExtent;
		halfExtent *= factor;
		m_Limits[0] = center - halfExtent;
		m_Limits[1] = center + halfExtent;
	}
	template<typename _FactorType>
	SelfType	ScaledFromCenter(_FactorType factor) const
	{
		SelfType	scaled(*this);
		scaled.ScaleFromCenter(factor);
		return scaled;
	}

	SelfType	&SetMin(const VectorType &min) { m_Limits[0] = min; return *this; }
	SelfType	&SetMax(const VectorType &max) { m_Limits[1] = max; return *this; }
	SelfType	&SetMinMax(const VectorType &minMax) { m_Limits[0] = minMax; m_Limits[1] = minMax; return *this; }
	SelfType	&SetMinMax(const VectorType &min, const VectorType &max) { m_Limits[0] = min; m_Limits[1] = max; HH_ASSERT_MESSAGE(Valid(), "Invalid min/max supplied to SetMinMax()"); return *this; }
	SelfType	&SetMinMaxUnchecked(const VectorType &min, const VectorType &max) { m_Limits[0] = min; m_Limits[1] = max; return *this; }
	SelfType	&SetMinMaxUnordered(const VectorType &min, const VectorType &max) { m_Limits[0] = _VMin(min, max); m_Limits[1] = _VMax(min, max); return *this; }

	SelfType	&SetupFromHalfExtent(const VectorType &halfExtent)
	{
		m_Limits[0] = -halfExtent;
		m_Limits[1] = halfExtent;
		HH_ASSERT_MESSAGE(Valid(), "Invalid halfExtent supplied to SetupFromHalfExtent() all axes should be positive...");
		return *this;
	}
	SelfType	&SetupFromHalfExtent(const _Type halfExtent)
	{
		HH_ASSERT(halfExtent >= 0);
		m_Limits[0] = VectorType(-halfExtent);
		m_Limits[1] = VectorType(halfExtent);
		return *this;
	}
	SelfType	&SetupFromPointCloud(const TStridedMemoryView<const VectorType> &points)
	{
		if (!points.Empty())
		{
			Degenerate();
			TPointCloudTools<VectorType>::MergeMinMax(points, m_Limits[0], m_Limits[1]);
		}
		else
		{
			m_Limits[0] = VectorType::ZERO;
			m_Limits[1] = VectorType::ZERO;
		}
		return *this;
	}
	SelfType	&SetupFromPointCloud(const TStridedMemoryView<const VectorType> &points, const CFloat4x4 &matrix)
	{
		// cheap approximation : avoid transforming every single point, just transform the final local box of the point-cloud.
		SetupFromOBB(SelfType().SetupFromPointCloud(points), matrix);
		return *this;
	}
	SelfType	&SetupFromOBB(const SelfType &box, const CFloat4x4 &matrix)
	{
		enum { kCornerCount = 1 << _Dimensions };
		VectorType	corners[kCornerCount];
		Internal::SCornerExtractor<kCornerCount>::RecursiveExtractCorners(box, corners);

		Degenerate();
		for (hh_u32 i = 0; i < kCornerCount; i++)
		{
			Add(matrix.TransformVector(corners[i]));
		}
		return *this;
	}

	// translates the box
	SelfType	&operator += (_Type translation) { m_Limits[0] += translation; m_Limits[1] += translation; return *this; }
	SelfType	&operator -= (_Type translation) { m_Limits[0] -= translation; m_Limits[1] -= translation; return *this; }
	SelfType	&operator += (const VectorType &translation) { m_Limits[0] += translation; m_Limits[1] += translation; return *this; }
	SelfType	&operator -= (const VectorType &translation) { m_Limits[0] -= translation; m_Limits[1] -= translation; return *this; }

	// scales the box
	SelfType	&operator *= (_Type scale) { m_Limits[0] *= scale; m_Limits[1] *= scale; if (scale < 0) { HHSwap(m_Limits[0], m_Limits[1]); } return *this; }
	SelfType	&operator *= (const VectorType &scale) { VectorType mm[2]; mm[0] = m_Limits[0] * scale; mm[1] = m_Limits[1] * scale; SetMinMax(_VMin(mm[0], mm[1]), _VMax(mm[0], mm[1])); return *this; }

	bool		operator == (const SelfType &other) const { return m_Limits[0] == other.m_Limits[0] && m_Limits[1] == other.m_Limits[1]; }
	bool		operator != (const SelfType &other) const { return !operator == (other); }

	HH_INLINE SelfType	&Degenerate()
	{
		m_Limits[0] = TNumericTraits<VectorType>::Max();
		m_Limits[1] = TNumericTraits<VectorType>::Min();
		return *this;
	}

	bool	Overlaps(const SelfType &box) const
	{
		return	All((m_Limits[0] < box.m_Limits[1]) &
					(m_Limits[1] > box.m_Limits[0]));
	}
	bool	Overlaps(const VectorType &point) const
	{
		return	All((m_Limits[0] <= point) &
					(m_Limits[1] >  point));
	}

	bool	Contains(const SelfType &box) const
	{
		return	All((m_Limits[0] <= box.m_Limits[0]) &
					(m_Limits[1] >= box.m_Limits[1]));
	}
	bool	Contains(const VectorType &point) const
	{
		return Overlaps(point);
	}

	// bit0: x, bit1: y, bit2: z, etc..
	// bit cleared: min, bit set: max
	template<hh_u32 _CornerMask>
	VectorType	ExtractCorner() const
	{
		VectorType	out;
		for (hh_u32 i = 0; i < _Dimensions; i++)
		{
			out.Axis(i) = m_Limits[((_CornerMask) >> i) & 1].Axis(i);
		}
		return out;
	}

	VectorType	ExtractCorner(hh_u32 corner) const
	{
		VectorType	out;
		for (hh_u32 i = 0; i < _Dimensions; i++)
		{
			out.Axis(i) = m_Limits[corner & 1].Axis(i);
			corner >>= 1;
		}
		return out;
	}
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimensions>
HH_ALIGN(0x10) const TPrimitiveAABB<_Type, _Dimensions>	TPrimitiveAABB<_Type, _Dimensions>::ZERO =
TPrimitiveAABB<_Type, _Dimensions>(TVector<_Type, _Dimensions>(0));

template<typename _Type, hh_u32 _Dimensions>
HH_ALIGN(0x10) const TPrimitiveAABB<_Type, _Dimensions>	TPrimitiveAABB<_Type, _Dimensions>::kINFINITE =
TPrimitiveAABB<_Type, _Dimensions>(TVector<_Type, _Dimensions>(TNumericTraits<_Type>::NegativeInfinity()), TVector<_Type, _Dimensions>(TNumericTraits<_Type>::PositiveInfinity()));

template<typename _Type, hh_u32 _Dimensions>
HH_ALIGN(0x10) const TPrimitiveAABB<_Type, _Dimensions>	TPrimitiveAABB<_Type, _Dimensions>::DEGENERATED =
TPrimitiveAABB<_Type, _Dimensions>().Degenerate();

/*template<typename _Type, hh_u32 _Dimensions>
HH_ALIGN(0x10) const TPrimitiveAABB<_Type, _Dimensions>	TPrimitiveAABB<_Type, _Dimensions>::DEGENERATED_INFINITE =
TPrimitiveAABB<_Type, _Dimensions>().DegenerateInfinite();*/

template<typename _Type, hh_u32 _Dimensions>
HH_ALIGN(0x10) const TPrimitiveAABB<_Type, _Dimensions>	TPrimitiveAABB<_Type, _Dimensions>::NORMALIZED_01 =
TPrimitiveAABB<_Type, _Dimensions>(TVector<_Type, _Dimensions>(0), TVector<_Type, _Dimensions>(1));

template<typename _Type, hh_u32 _Dimensions>
HH_ALIGN(0x10) const TPrimitiveAABB<_Type, _Dimensions>	TPrimitiveAABB<_Type, _Dimensions>::NORMALIZED_M1P1 =
TPrimitiveAABB<_Type, _Dimensions>(TVector<_Type, _Dimensions>(-1), TVector<_Type, _Dimensions>(1));

//----------------------------------------------------------------------------

template<typename _Type1, hh_u32 _Dimensions1, typename _Type2>
struct	TAABBPromotableOpImpl
{
	// operands: scalars
	typedef TPrimitiveAABB<_Type1, _Dimensions1>						_SrcBox;
	typedef typename TNumericPromoter<_Type1, _Type2>::PromotedType		_PromotedScalar;
	typedef TPrimitiveAABB<_PromotedScalar, _Dimensions1>				_PromotedBox;

	static _PromotedBox	Scaled(const _SrcBox &box, const _Type2 &scale)
	{
		_PromotedBox	out = box;
		out *= scale;
		return out;
	}
	static _PromotedBox	Translated(const _SrcBox &box, const _Type2 &offset)
	{
		_PromotedBox	out = box;
		out += offset;
		return out;
	}
};

//----------------------------------------------------------------------------

template<typename _Type1, hh_u32 _Dimensions1, typename _Type2, hh_u32 _Dimensions2>
struct	TAABBPromotableOpImpl<_Type1, _Dimensions1, TVector<_Type2, _Dimensions2> >
{
	// operands: vectors
	HH_STATIC_ASSERT(_Dimensions1 == _Dimensions2);
	typedef TPrimitiveAABB<_Type1, _Dimensions1>					_SrcBox;
	typedef typename TNumericPromoter<_Type1, _Type2>::PromotedType	_PromotedScalar;
	typedef TPrimitiveAABB<_PromotedScalar, _Dimensions1>			_PromotedBox;

	static _PromotedBox	Scaled(const _SrcBox &box, const TVector<_Type2, _Dimensions2> &scale)
	{
		_PromotedBox	out = box;
		out *= scale;
		return out;
	}
	static _PromotedBox	Translated(const _SrcBox &box, const TVector<_Type2, _Dimensions2> &offset)
	{
		_PromotedBox	out = box;
		out += offset;
		return out;
	}
};

//----------------------------------------------------------------------------
// global operator * for boxes/rects: Scale
// box * scale
template<typename _Type, hh_u32 _Dimensions, typename _Type2>
HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::_PromotedBox	operator * (const TPrimitiveAABB<_Type, _Dimensions> &box, const _Type2 &scale)
{
	return TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::Scaled(box, scale);
}
// scale * box
template<typename _Type, hh_u32 _Dimensions, typename _Type2>
HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::_PromotedBox	operator * (const _Type2 &scale, const TPrimitiveAABB<_Type, _Dimensions> &box)
{
	return TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::Scaled(box, scale);
}
// template<typename _Type, hh_u32 _Dimensions, typename _Type2>
// HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, TVector<_Type2, _Dimensions>>::_PromotedBox	operator * (const TVector<_Type2, _Dimensions> &scale, const TPrimitiveAABB<_Type, _Dimensions> &box)
// {
// 	return TAABBPromotableOpImpl<_Type, _Dimensions, TVector<_Type2, _Dimensions>>::Scaled(box, scale);
// }

// global operator + for boxes/rects: Translate
// box + offset
template<typename _Type, hh_u32 _Dimensions, typename _Type2>
HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::_PromotedBox	operator + (const TPrimitiveAABB<_Type, _Dimensions> &box, const _Type2 &offset)
{
	return TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::Translated(box, offset);
}
// // scale + box
// template<typename _Type, hh_u32 _Dimensions, typename _Type2>
// HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::_PromotedBox	operator + (const _Type2 &offset, const TPrimitiveAABB<_Type, _Dimensions> &box)
// {
// 	return TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::Translated(box, offset);
// }

// global operator - for boxes/rects: Translate
// box - offset
template<typename _Type, hh_u32 _Dimensions, typename _Type2>
HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::_PromotedBox	operator - (const TPrimitiveAABB<_Type, _Dimensions> &box, const _Type2 &offset)
{
	return TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::Translated(box, -offset);
}
//// scale - box -> box + (scale - box.Center())
//template<typename _Type, hh_u32 _Dimensions, typename _Type2>
//HH_INLINE typename TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::_PromotedBox	operator - (const _Type2 &offset, const TPrimitiveAABB<_Type, _Dimensions> &box)
//{
//	return TAABBPromotableOpImpl<_Type, _Dimensions, _Type2>::Translated(box, offset - box.Center());
//}

//----------------------------------------------------------------------------

typedef TPrimitiveAABB<float, 2>	CRect;
typedef TPrimitiveAABB<hh_i32, 2>	CIRect;
typedef TPrimitiveAABB<hh_u32, 2>	CURect;
typedef TPrimitiveAABB<float, 3>	CAABB;

//----------------------------------------------------------------------------

HH_STATIC_ASSERT_CUSTOM(sizeof(CRect) == 4 * 2 * 2,			"Unexpected_Size_For_Class_CRect");
HH_STATIC_ASSERT_CUSTOM(sizeof(CIRect) == 4 * 2 * 2,		"Unexpected_Size_For_Class_CIRect");
HH_STATIC_ASSERT_CUSTOM(sizeof(CAABB) == 4 * 3 * 2,			"Unexpected_Size_For_Class_CAABB");

//----------------------------------------------------------------------------

//template<typename _Type, hh_u32 _Dimensions>
//TPrimitiveAABB<_Type, _Dimensions>	PKSwapEndian(const TPrimitiveAABB<_Type, _Dimensions> &object)
//{
//	TPrimitiveAABB<_Type, _Dimensions>	out = object;
//	PKSwapEndianInPlace(out);
//	return out;
//}

namespace	Internal	// kr_memoryviews.h
{
	template<typename _Type, hh_u32 _Dimensions>
	struct	TStreamEndianSwapperHelpers<TPrimitiveAABB<_Type, _Dimensions> >
	{
		HH_STATIC_ASSERT(sizeof(TPrimitiveAABB<_Type, _Dimensions>) == sizeof(_Type) * 2 * _Dimensions);
		static void	SwapInPlace(TPrimitiveAABB<_Type, _Dimensions> *data, hh_u32 count) { TNativeSwapper<sizeof(_Type)>::SwapInPlace(data, count * 2 * _Dimensions); }
	};
}

template<typename _Type, hh_u32 _Dimensions>
void	PKSwapEndianInPlace(TPrimitiveAABB<_Type, _Dimensions> &object)
{
	PKSwapEndianInPlace(object.m_Limits[0]);
	PKSwapEndianInPlace(object.m_Limits[1]);
}

//----------------------------------------------------------------------------

template<typename _Type>
struct	TRectangleMapper
{
	struct	TCorners
	{
		enum
		{
			CornerMinMax = 0,
			CornerMaxMax = 1,
			CornerMaxMin = 2,
			CornerMinMin = 3,
		};

		TVector<_Type, 2>		m_Corners[4];
	};

private:
	TMemoryView<const TCorners>	m_CornersList_Aligned16;
public:
	TRectangleMapper(const TMemoryView<const TCorners> &cornersAligned16)
	:	m_CornersList_Aligned16(cornersAligned16)
	{
		HH_ASSERT_ALIGNED16(m_CornersList_Aligned16.Data());
	}

	const TMemoryView<const TCorners>	&CornersList_Aligned16() const { return m_CornersList_Aligned16; }
};

//----------------------------------------------------------------------------
//
//	3D Disks (used by cylinders and cones)
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Axis>
class	TDisk
{
public:
	typedef TVector<_Type, 3>	VectorType;
	typedef TDisk<_Type, _Axis>	SelfType;

	static const hh_u32	_AxisX = (_Axis + 1) % 3;
	static const hh_u32	_AxisY = (_Axis + 2) % 3;
public:
	TDisk(_Type radius, _Type axialHeight = 0)
	:	m_Radius(radius)
	,	m_AxialHeight(axialHeight)
	{
		HH_STATIC_ASSERT(_Axis >= 0 && _Axis < 3);
	}
	~TDisk() {}

	_Type	m_Radius;
	_Type	m_AxialHeight;
};

//----------------------------------------------------------------------------

#if 0
typedef TDisk<float, 1>		CDisk;	// by default, aligned on the Y axis
HH_STATIC_ASSERT_CUSTOM(sizeof(CDisk) == 4 * 2, "Unexpected_Size_For_Class_CDisk");
#endif

//----------------------------------------------------------------------------
//
//	Cylinders
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Axis>
class	TCylinder
{
public:
	typedef TVector<_Type, 3>		VectorType;
	typedef TCylinder<_Type, _Axis>	SelfType;

	static const hh_u32	_AxisX = (_Axis + 1) % 3;
	static const hh_u32	_AxisY = (_Axis + 2) % 3;
protected:
public:
	TCylinder(_Type radius, _Type zMin, _Type zMax)
	:	m_ZMin(zMin)
	,	m_ZMax(zMax)
	,	m_Radius(radius)
	,	m_Sweep(TNumericConstants<_Type>::Pi() * 2)
	{
		HH_STATIC_ASSERT(_Axis >= 0 && _Axis < 3);
	}
	~TCylinder() {}

	_Type	m_ZMin;
	_Type	m_ZMax;
	_Type	m_Radius;
	_Type	m_Sweep;
};

//----------------------------------------------------------------------------

typedef TCylinder<float, 1>		CCylinder;	// by default, aligned on the Y axis
HH_STATIC_ASSERT_CUSTOM(sizeof(CCylinder) == 4 * 4, "Unexpected_Size_For_Class_CCylinder");

//----------------------------------------------------------------------------
//
//	Cones
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Axis>
class	TCone
{
public:
	typedef TVector<_Type, 3>	VectorType;
	typedef TCone<_Type, _Axis>	SelfType;

	static const hh_u32	_AxisX = (_Axis + 1) % 3;
	static const hh_u32	_AxisY = (_Axis + 2) % 3;
protected:
public:
	TCone(_Type radius, _Type height, _Type axialHeight = 0)
	:	m_Height(height)
	,	m_AxialHeight(axialHeight)
	,	m_Radius(radius)
	,	m_Sweep(TNumericConstants<_Type>::Pi() * 2)
	{
		HH_STATIC_ASSERT(_Axis >= 0 && _Axis < 3);
	}
	~TCone() {}

	_Type	m_Height;
	_Type	m_AxialHeight;
	_Type	m_Radius;
	_Type	m_Sweep;
};

//----------------------------------------------------------------------------

typedef TCone<float, 1>		CCone;	// by default, aligned on the Y axis
HH_STATIC_ASSERT_CUSTOM(sizeof(CCone) == 4 * 4, "Unexpected_Size_For_Class_CCone");

//----------------------------------------------------------------------------
//
//	Other Tools
//
//----------------------------------------------------------------------------

bool	TestCullAABB(const CAABB &aabb, float distanceSquared, const TStridedMemoryView<const CFloat3> &viewVecs, const TStridedMemoryView<const CFloat3> &viewPositions);

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PK_MATHS_PRIMITIVES_H__
