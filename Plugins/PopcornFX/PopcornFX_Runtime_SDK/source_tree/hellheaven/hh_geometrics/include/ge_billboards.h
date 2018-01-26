#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2008/03/04 8:36 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_BILLBOARDS_H__
#define __GE_BILLBOARDS_H__

#include "hh_maths/include/hh_maths_primitives.h"
#include "hh_geometrics/include/ge_config.h"

#if	(HH_GEOMETRICS_BUILD_BILLBOARDS != 0)

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	- screen aligned	[view dir]
//	- camera aligned	[view dir, view pos]
//	- axis constrained	[view dir, view pos, axis]
//	- axis stretched & constrained	[view dir, view pos, axis*length]
//	- spheroidal billboards (axis stretch & align) [view dir, view pos, axis*length]
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CBillboarderConfig
{
public:
	// default: counter clock wise
	static void			SetFrontFaceOrientation(bool clockWise);
	static bool			FrontFaceClockWise();
};

//----------------------------------------------------------------------------

class	CBillboarderPoint;
class	CBillboarderAxial;
class	CBillboarderPlane;

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CBillboarder
{
public:
	enum	ELevel
	{
		Level_C0 = 0,
		Level_C1 = 1,
		Level_C2 = 2,
	};

private:
	ELevel					m_Level;
	hh_u32					m_BillboardVertexCount;
	hh_u32					m_BillboardIndexCount;

protected:
	CBillboarder(ELevel level, hh_u32 vCount, hh_u32 iCount) : m_Level(level), m_BillboardVertexCount(vCount), m_BillboardIndexCount(iCount) {}

public:
	virtual ~CBillboarder() {}

	ELevel					Level() const { return m_Level; }

	CBillboarderPoint		*UpcastToC0_IFP();
	const CBillboarderPoint	*UpcastToC0_IFP() const;
	CBillboarderAxial		*UpcastToC1_IFP();
	const CBillboarderAxial	*UpcastToC1_IFP() const;
	CBillboarderPlane		*UpcastToC2_IFP();
	const CBillboarderPlane	*UpcastToC2_IFP() const;

	HH_FORCEINLINE hh_u32	BillboardVertexCount() const { return m_BillboardVertexCount; }
	HH_FORCEINLINE hh_u32	BillboardIndexCount() const { return m_BillboardIndexCount; }

	virtual void			FillIndexPattern(const TMemoryView<hh_u32> &pattern) const;
	virtual void			FillTriangleIndices(hh_u32 *indices, hh_u32 billboardCount, hh_u32 firstIndex) const;
	virtual void			FillTriangleIndices(hh_u16 *indices, hh_u32 billboardCount, hh_u16 firstIndex) const;

	virtual void			FillTexcoords(const TStridedMemoryView<CFloat2> &outTexcoords, bool vFlipUVs) const;
	virtual void			FillTexcoords(const TStridedMemoryView<CUword2> &outTexcoords, bool vFlipUVs) const;
	virtual void			FillTexcoords(const TStridedMemoryView<CWord2> &outTexcoords, bool vFlipUVs) const;

	virtual void			FillTexcoordsFromAtlas(	const TStridedMemoryView<CFloat2>		&outTexcoords,
													const TStridedMemoryView<const float>	&ids,
													const TRectangleMapper<float>			&rectMapper,
													bool									vFlipUVs) const;

	virtual void			FillTexcoordsFromAtlas(	const TStridedMemoryView<CUword2>		&outTexcoords,
													const TStridedMemoryView<const float>	&ids,
													const TRectangleMapper<hh_u16>			&rectMapper,
													bool									vFlipUVs) const;

	virtual void			FillTexcoordsFromAtlas(	const TStridedMemoryView<CWord2>		&outTexcoords,
													const TStridedMemoryView<const float>	&ids,
													const TRectangleMapper<hh_i16>			&rectMapper,
													bool									vFlipUVs) const;

	virtual void			FillTexcoordsFromAtlas(	const TStridedMemoryView<CUword2>		&outTexcoords,
													const TStridedMemoryView<CUword2>		&outTexcoords2,
													const TStridedMemoryView<float>			&outAtlasIDs,
													const TStridedMemoryView<const float>	&ids,
													const TRectangleMapper<hh_u16>			&rectMapper,
													bool									vFlipUVs) const;

	virtual void			FillTexcoordsFromAtlas(	const TStridedMemoryView<CWord2>		&outTexcoords,
													const TStridedMemoryView<CWord2>		&outTexcoords2,
													const TStridedMemoryView<float>			&outAtlasIDs,
													const TStridedMemoryView<const float>	&ids,
													const TRectangleMapper<hh_i16>			&rectMapper,
													bool									vFlipUVs) const;

	virtual void			FillTexcoordsFromAtlas(	const TStridedMemoryView<CFloat2>		&outTexcoords,
													const TStridedMemoryView<CFloat2>		&outTexcoords2,
													const TStridedMemoryView<float>				&outAtlasIDs,
													const TStridedMemoryView<const float>	&ids,
													const TRectangleMapper<float>			&rectMapper,
													bool									vFlipUVs) const;

	virtual void			FillAtlasIds(const TMemoryView<float> &outAtlasIDs, const TStridedMemoryView<const float> &ids) const;

	/*deprecated ?*/virtual void	FillColors(const TStridedMemoryView<CFloat4> &dstColors, const TStridedMemoryView<const CFloat4> &srcColors, const CFloat4 &colorScale) const;
	/*deprecated ?*/virtual void	FillColors(const TStridedMemoryView<CUbyte4> &dstColors, const TStridedMemoryView<const CUbyte4> &srcColors) const;

	virtual void			FillColors(const TStridedMemoryView<CFloat4> &dstColors, const TStridedMemoryView<const CFloat4> &srcColors) const;

	// CUbyte4 color swizzling
	// ARGB : defined(HH_X360)
	// RGBA : defined(HH_PS3) || defined(HH_PSP2) || defined(HH_ORBIS) || defined(HH_ANDROID) || defined(HH_IOS)
	// BGRA : others
	virtual void			FillColors(const TStridedMemoryView<CUbyte4> &dstColors, const TStridedMemoryView<const CFloat4> &srcColors) const;

	virtual void			FillAlphaCursors(const TMemoryView<float> &dstAlphaCursor, const TStridedMemoryView<const float> &srcAlphaCursors) const;

	struct	SAlignmentContext
	{
		TStridedMemoryViewWithFootprint<CFloat3>	m_OutPositions;
		TStridedMemoryViewWithFootprint<CFloat3>	m_OutNormals;		// can be empty
		TStridedMemoryView<CFloat4>					m_OutTangents;		// can be empty
		TStridedMemoryView<const CFloat3>			m_Centers;
		TStridedMemoryView<const CFloat3>			m_Axis0;			// must be non-empty for C1 and C2 billboarders
		TStridedMemoryView<const CFloat3>			m_Axis1;			// must be non-empty for C2
		TStridedMemoryView<const float>				m_Rads;
		TStridedMemoryView<const CFloat2>			m_Rads2;
		TMemoryView<const float>					m_Rotations;		// can be empty
		CFloat2										m_Aspect;			// axis scale in billboard-space
		float										m_NormalsBendingFactor;
		CFloat2										m_ConstantHalfLengthCoeff;

		SAlignmentContext() : m_Aspect(1), m_NormalsBendingFactor(0), m_ConstantHalfLengthCoeff(0.5f) {}

		void										SetConstantLengthCoeff(float lengthCoeffX, float lengthCoeffY) { m_ConstantHalfLengthCoeff = 0.5f * CFloat2(lengthCoeffX, lengthCoeffY); }
		void										SetConstantHalfLengthCoeff(float halfLengthCoeffX, float halfLengthCoeffY) { m_ConstantHalfLengthCoeff = CFloat2(halfLengthCoeffX, halfLengthCoeffY); }
	};

	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context) const = 0;
};

//----------------------------------------------------------------------------
//
//	Helpers
//
//----------------------------------------------------------------------------

class	CPackedPlane;

class	HH_GEOMETRICS_EXPORT CBillboardUtils
{
public:

	// assumes view-oriented billboards, NOT screen-aligned
	static void		FixupPositionsAndRadiiForPerspectiveCorrectBounding(const CFloat4x4 &viewTransformsInPositionsSpace,
																		const CPackedPlane &planeInPositionsSpace,
																		const TStridedMemoryView<const float> &rads,
																		const TStridedMemoryView<const CFloat3> &positions,
																		const TStridedMemoryView<float> &outRads,
																		const TStridedMemoryViewWithFootprint<CFloat3> &outPositions);

	static void		ReprojectVerticesOnNearPlaneIFN(const CFloat4x4 &viewTransformsInPositionsSpace,
													const CPackedPlane &planeInPositionsSpace,
													const TStridedMemoryViewWithFootprint<CFloat3> &positions);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif

#endif // __GE_BILLBOARDS_H__

