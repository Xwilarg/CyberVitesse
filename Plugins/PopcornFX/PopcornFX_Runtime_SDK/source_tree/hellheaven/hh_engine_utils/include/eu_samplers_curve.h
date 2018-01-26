#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2008/09/11 15:59 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __EN_SAMPLERS_CURVE_H__
#define __EN_SAMPLERS_CURVE_H__

#include "hh_kernel/include/kr_callbacks.h"
#include "hh_base_object/include/hbo_object.h"
#include "hh_maths/include/hh_maths_interpolable.h"
#include "hh_geometrics/include/ge_coordinate_frame.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>
class	TInterpolableVectorArrayView;

//----------------------------------------------------------------------------
//
//	curve interface
//
//----------------------------------------------------------------------------

class	CCurveSamplerInterface
{
protected:
	CFloat2				m_ParametricDomain;

public:
	CCurveSamplerInterface() : m_ParametricDomain(0,1) {}
	virtual ~CCurveSamplerInterface() {}

	const CFloat2		&ParametricDomain() const { return m_ParametricDomain; }

	// The input 'cursors' can lay outside the parametric domain.
	// The implementation should take care of clamping the cursors if necessary.

	virtual bool		Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat1> &outputSampledValues) const = 0;
	virtual bool		Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat2> &outputSampledValues) const = 0;
	virtual bool		Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat3> &outputSampledValues) const = 0;
	virtual bool		Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat4> &outputSampledValues) const = 0;

	virtual bool		Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat1> &outputSampledValues) const = 0;
	virtual bool		Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat2> &outputSampledValues) const = 0;
	virtual bool		Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat3> &outputSampledValues) const = 0;
	virtual bool		Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat4> &outputSampledValues) const = 0;

	// Integrates the curve between 't0' and 't1', t0 and t1 are ordered in increasing order, but may wrap.
	// These should correctly handle wrapping values of t0 and t1
	// ex: Integrate(0.2, 0.6, outVal), providing the curve is in the [0,1] range, will
	//     - integrate in the [0.2, 06] range, and set outVal to that result.
	// ex: Integrate(0.6, 0.2, outVal), providing the curve is in the [0,1] range, will
	//     - integrate in the [0.6, 1] range
	//     - integrate in the [0, 0.2] range
	//     - set outVal to the sum of the two integrations

	virtual bool		Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat1> &outputSampledValues) const = 0;
	virtual bool		Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat2> &outputSampledValues) const = 0;
	virtual bool		Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat3> &outputSampledValues) const = 0;
	virtual bool		Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat4> &outputSampledValues) const = 0;
};

//----------------------------------------------------------------------------
//
//	curve storage
//
//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT CCurveDescriptor : public CCurveSamplerInterface
{
public:
//	CInterpolableVectorArray::EInterpolator		m_Interpolator;
	hh_u32										m_Interpolator;
	hh_u32										m_Order;
	TArray<float, TArrayAligned16>				m_Times;
	TArray<float, TArrayAligned16>				m_FloatValues;
	TArray<float, TArrayAligned16>				m_FloatTangents;
	CFloat4										m_MinEvalLimits;
	CFloat4										m_MaxEvalLimits;

	TCallbackChain<void(CCurveDescriptor*)>		m_OnOrderChange;

	void										_OnTimeListChanged();

protected:
	bool										m_NeedsEvalClamp;

	template<typename _Type>
	bool	AreArraysCoherent() const
	{
		HH_STATIC_ASSERT(_Type::Dimension > 0);
		const hh_u32	rawVCount = m_FloatValues.Count();//RawValueArray<_Type>().Count();
		const hh_u32	rawTCount = m_FloatTangents.Count();
		const hh_u32	vCount = rawVCount / _Type::Dimension;
		const hh_u32	tCount = rawTCount / _Type::Dimension;
		return (m_Times.Count() == vCount) && (tCount == 0 || tCount == 2 * vCount) && (vCount * _Type::Dimension == rawVCount) && (tCount * _Type::Dimension == rawTCount);
	}

public:
	CCurveDescriptor();

	const CFloat4		&MinLimits() const { return m_MinEvalLimits; }
	const CFloat4		&MaxLimits() const { return m_MaxEvalLimits; }
	void				SetEvalLimits(const CFloat4 &minLimit, const CFloat4 &maxLimit);
	void				SetParametricDomain(const CFloat2 &pDomain);

	bool				Resize(hh_u32 count);
	bool				Decimate(float threshold);
	void				ShiftToOrder(hh_u32 order);
	hh_u32				Order() const { HH_ASSERT(m_Order >= 1 && m_Order <= 4); return m_Order; }

	template<typename _Type>
	void	BuildRawArrayView(TInterpolableVectorArrayView<const _Type> &outView) const
	{
		HH_ASSERT(AreArraysCoherent<_Type>());
		outView.SetSource(m_Times, m_FloatValues, m_FloatTangents, (const CInterpolableVectorArray::EInterpolator*)&m_Interpolator, *reinterpret_cast<const _Type*>(&m_MinEvalLimits), *reinterpret_cast<const _Type*>(&m_MaxEvalLimits));
	}

	template<typename _Type>
	void	BuildRawArrayView(TInterpolableVectorArrayView<_Type> &outView)
	{
		HH_ASSERT(AreArraysCoherent<_Type>());
		outView.SetSource(m_Times, m_FloatValues, m_FloatTangents, (CInterpolableVectorArray::EInterpolator*)&m_Interpolator, *reinterpret_cast<const _Type*>(&m_MinEvalLimits), *reinterpret_cast<const _Type*>(&m_MaxEvalLimits));
	}

	template<typename _Type>
	bool	IsViewSane(const TInterpolableVectorArrayView<_Type> &amISane) const
	{
		TInterpolableVectorArrayView<const _Type> other;
		BuildRawArrayView<const _Type>(other);
		return other == amISane;
	}

#if	(HH_MATHS_INTERPOLATORS_ALLOW_QUATERNIONS != 0)
	bool			_Sample_Impl(const TMemoryView<const float> &cursors, const TStridedMemoryView<CQuaternion> &outputSampledValues) const;
#endif

	// virtual overrides of 'CCurveSamplerInterface'
	virtual bool	Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat1> &outputSampledValues) const override;
	virtual bool	Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat2> &outputSampledValues) const override;
	virtual bool	Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat3> &outputSampledValues) const override;
	virtual bool	Sample(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat4> &outputSampledValues) const override;

	virtual bool	Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat1> &outputSampledValues) const override;
	virtual bool	Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat2> &outputSampledValues) const override;
	virtual bool	Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat3> &outputSampledValues) const override;
	virtual bool	Derive(const TMemoryView<const float> &cursors, const TStridedMemoryView<CFloat4> &outputSampledValues) const override;

	virtual bool	Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat1> &outputSampledValues) const override;
	virtual bool	Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat2> &outputSampledValues) const override;
	virtual bool	Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat3> &outputSampledValues) const override;
	virtual bool	Integrate(const TMemoryView<const float> &t0, const TMemoryView<const float> &t1, const TMemoryView<CFloat4> &outputSampledValues) const override;
	// end virtual overrides

	bool			IsCoherent() const;
	void			MakeTangentsCoherent(); // remove or resize tangents
};

//----------------------------------------------------------------------------
//
//	HBO curve
//
//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CSamplerCurve), public CBaseObject, public CCurveDescriptor
{
private:
	typedef TArray<float, TArrayAligned16>	TAlignedFloatArray;

	HBO_FIELD(CString,						BindingSemantic);

	HBO_FIELD_ABSTRACT(hh_u32,				ValueType/*, CCurveDescriptor::m_Order, private, private*/);
	HBO_FIELD_ABSTRACT(hh_u32,				Interpolator/*, CCurveDescriptor::m_Interpolator*/);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	Times/*, CCurveDescriptor::m_Times, public, public*/);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatValues/*, CCurveDescriptor::m_FloatValues, public, public*/);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatTangents/*, CCurveDescriptor::m_FloatTangents, public, public*/);
	HBO_FIELD_ABSTRACT(CFloat4,				MinLimits/*, CCurveDescriptor::m_MinEvalLimits*/);
	HBO_FIELD_ABSTRACT(CFloat4,				MaxLimits/*, CCurveDescriptor::m_MaxEvalLimits*/);

	bool					_OnTimesModified();
	bool					_OnLimitsModified();
	bool					_OnValueTypeModified();

protected:
	virtual bool			VirtualLink() override;

public:
	CSamplerCurve();
	~CSamplerCurve() {}

	void					SetOrder(hh_u32 order) { HH_ASSERT(order >= 1 && order <= 4); SetValueType(order); }

	void					SetData(const TMemoryView<const float> &times,		// count = n
									const TMemoryView<const float> &values,		// count = n * order
									const TMemoryView<const float> &tangents);	// count = n * order * 2 OR empty if linear curve

#if	(HH_MATHS_INTERPOLATORS_ALLOW_QUATERNIONS != 0)
	bool	_OLD_Eval(const TMemoryView<const float> &cursors, const TStridedMemoryView<CQuaternion> &outputSampledValues) const { return CCurveDescriptor::_Sample_Impl(cursors, outputSampledValues); }
#endif

	HBO_CLASS_DECLARATION();
};

HH_DECLARE_REFPTRCLASS(SamplerCurve);

//----------------------------------------------------------------------------
//
//	anim tracks
//
//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CAnimationTrack), public CBaseObject
{
private:
	HBO_FIELD(CString,					TrackName);
	HBO_FIELD(TArray<CSamplerCurve*>,	Channels);
	HBO_FIELD(hh_u32,					CoordinateFrame);

	bool			OnChannelsModified();
	void			_ConvertCoordinates(ECoordinateFrame from, ECoordinateFrame to);

protected:
	virtual bool	VirtualLink() override;

public:
	CAnimationTrack();
	~CAnimationTrack() {}

	void			ConvertToCoordinateSystem(ECoordinateFrame to);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(AnimationTrack);

//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CAnimationClip), public CBaseObject
{
private:
	HBO_FIELD(TArray<CAnimationTrack*>,	EntityStreams);
	HBO_FIELD(float,					LengthInSeconds);
	HBO_FIELD(float,					PlaybackSpeed);
protected:
public:
	CAnimationClip();
	~CAnimationClip() {}

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(AnimationClip);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __EN_SAMPLERS_CURVE_H__
