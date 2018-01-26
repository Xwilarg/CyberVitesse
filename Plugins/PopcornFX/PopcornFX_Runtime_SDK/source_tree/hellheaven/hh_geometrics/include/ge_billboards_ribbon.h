#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2013/03/29 08:24 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_BILLBOARDS_RIBBON_H__
#define __GE_BILLBOARDS_RIBBON_H__

#include "hh_maths/include/hh_maths_primitives.h"
#include "hh_geometrics/include/ge_config.h"

#include "hh_kernel/include/kr_parallel_sort.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CRibbonBillboarder
{
public:
	struct SCentersIndex
	{
		hh_u16			m_PageIndex;
		hh_u16			m_PartIndex;
	};

	struct	SAlignmentContext
	{
		hh_u64								m_RibbonsIdsMask;
		TStridedMemoryView<const hh_u64>	m_RibbonsIds;

		TStridedMemoryView<CFloat3>			m_OutPositions;

		TStridedMemoryView<CFloat3>			m_OutNormals; // {normX, normY, normZ, undef}
		TStridedMemoryView<CFloat4>			m_OutTangents; // {tangX, tangY, tangZ, 1.0f}

		TStridedMemoryView<CFloat2>			m_OutUVFactors;  // either CFloat2
		TStridedMemoryView<CFloat4>			m_OutUVFactors4; // or CFloat4

		TStridedMemoryView<const SCentersIndex>							m_Indices;
		TStridedMemoryView<const TStridedMemoryView<const CFloat3> >	m_Centers;
		TStridedMemoryView<const TStridedMemoryView<const float> >		m_Widths;
		TStridedMemoryView<const TStridedMemoryView<const CFloat3> >	m_Axis;		// normal of the ribbon
		TStridedMemoryView<const TStridedMemoryView<const float> >		m_AlphaCursor;
		float															m_NormalsBendingFactor;

		SAlignmentContext() : m_NormalsBendingFactor(0) {}
	};

	CRibbonBillboarder(hh_u32 vCount, hh_u32 iCount)
	:	m_NeedsUFlipping(false)
	,	m_NeedsVFlipping(false)
	,	m_NeedsRotateTexture(false)
	,	m_BillboardVertexCount(vCount)
	,	m_BillboardIndexCount(iCount)
	{
	}
	virtual ~CRibbonBillboarder() {}

	HH_FORCEINLINE hh_u32	BillboardVertexCount() const { return m_BillboardVertexCount; }
	HH_FORCEINLINE hh_u32	BillboardIndexCount() const { return m_BillboardIndexCount; }

	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context) const = 0;
	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context, hh_u32 particleStart, hh_u32 particleCount) const = 0;

	virtual void	FillIndexPattern(const TMemoryView<hh_u32> &pattern) const = 0;
	virtual void	FillTriangleIndices(hh_u32 *indices, hh_u32 billboardCount, hh_u32 firstIndex) const = 0;
	virtual void	FillTriangleIndices(hh_u16 *indices, hh_u32 billboardCount, hh_u16 firstIndex) const = 0;

	template<typename _InputType, typename _OutputType>
	struct TFillData
	{
		TStridedMemoryView<_OutputType>									m_Dst;
		TStridedMemoryView<const SCentersIndex>							m_Indices;
		TStridedMemoryView<const TStridedMemoryView<const _InputType> >	m_Src;

		TFillData(	const TStridedMemoryView<_OutputType>									&_dst,
					const TStridedMemoryView<const SCentersIndex>							&_indices,
					const TStridedMemoryView<const TStridedMemoryView<const _InputType> >	&_src)
		:	m_Dst(_dst)
		,	m_Indices(_indices)
		,	m_Src(_src)
		{
		}
	};

	virtual void	FillColors(const TFillData<CFloat4, CFloat4> &datas, const CFloat4 &colorScale = CFloat4::ONE) const = 0;

	virtual void	FillUVRemap(const TStridedMemoryView<CFloat4> &uvScalesAndOffsets) const = 0;
	virtual void	FillUVRemapFromU(	const TStridedMemoryView<CFloat4> &uvScalesAndOffsets,
										const TStridedMemoryView<const SCentersIndex> &indices,
										const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs) const = 0;
	virtual void	FillUVRemapFromAtlas(	const TStridedMemoryView<CFloat4> &uvScalesAndOffsets,
											const TStridedMemoryView<const SCentersIndex> &indices,
											const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
											const TRectangleMapper<float> &rectMapper) const = 0;
	virtual void	FillUVRemapFromUAndAtlas(	const TStridedMemoryView<CFloat4> &uvScalesAndOffsets,
												const TStridedMemoryView<const SCentersIndex> &indices,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
												const TRectangleMapper<float> &rectMapper) const = 0;

	// Fill texture without any flip or rotate (UVFactors wont work either)
	virtual void	FillTexcoordsForUVFactors(const TStridedMemoryView<CFloat2> &texcoords) const = 0;

	virtual void	FillTexcoords(const TStridedMemoryView<CFloat2> &texcoords) const = 0;
	virtual void	FillTexcoordsFromU(	const TStridedMemoryView<CFloat2> &texcoords,
										const TStridedMemoryView<const SCentersIndex> &indices,
										const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs) const = 0;
	virtual void	FillTexcoordsFromAtlas(	const TStridedMemoryView<CFloat2> &texcoords,
											const TStridedMemoryView<const SCentersIndex> &indices,
											const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
											const TRectangleMapper<float> &rectMapper) const = 0;
	virtual void	FillTexcoordsFromUAndAtlas(	const TStridedMemoryView<CFloat2> &texcoords,
												const TStridedMemoryView<const SCentersIndex> &indices,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
												const TRectangleMapper<float> &rectMapper) const = 0;

	virtual void	FillAlphaCursors(const TFillData<float, float> &datas) const = 0;

	bool					m_NeedsUFlipping;
	bool					m_NeedsVFlipping;
	// only affects FillTexcoords* (expect FillTexcoordsForUVFactors)
	bool					m_NeedsRotateTexture;

private:
	hh_u32					m_BillboardVertexCount;
	hh_u32					m_BillboardIndexCount;
};

//----------------------------------------------------------------------------

class	CRibbonBillboarder_WideLinesQuad : public CRibbonBillboarder
{
public:
	CRibbonBillboarder_WideLinesQuad() : CRibbonBillboarder(4, 6) { }
	~CRibbonBillboarder_WideLinesQuad() { }

	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context) const override;
	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context, hh_u32 particleStart, hh_u32 particleCount) const override;

	virtual void	FillIndexPattern(const TMemoryView<hh_u32> &pattern) const override;
	virtual void	FillTriangleIndices(hh_u32 *indices, hh_u32 billboardCount, hh_u32 firstIndex) const override;
	virtual void	FillTriangleIndices(hh_u16 *indices, hh_u32 billboardCount, hh_u16 firstIndex) const override;
	virtual void	FillColors(const TFillData<CFloat4, CFloat4> &datas, const CFloat4 &colorScale = CFloat4::ONE) const override;

	virtual void	FillUVRemap(const TStridedMemoryView<CFloat4> &uvScalesAndOffsets) const override;
	virtual void	FillUVRemapFromU(	const TStridedMemoryView<CFloat4> &uvScalesAndOffsets,
										const TStridedMemoryView<const SCentersIndex> &indices,
										const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs) const	override;
	virtual void	FillUVRemapFromAtlas(	const TStridedMemoryView<CFloat4> &uvScalesAndOffsets,
											const TStridedMemoryView<const SCentersIndex> &indices,
											const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
											const TRectangleMapper<float> &rectMapper) const override;
	virtual void	FillUVRemapFromUAndAtlas(	const TStridedMemoryView<CFloat4> &uvScalesAndOffsets,
												const TStridedMemoryView<const SCentersIndex> &indices,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
												const TRectangleMapper<float> &rectMapper) const override;

	virtual void	FillTexcoordsForUVFactors(const TStridedMemoryView<CFloat2> &texcoords) const override;

	virtual void	FillTexcoords(const TStridedMemoryView<CFloat2> &texcoords) const override;
	virtual void	FillTexcoordsFromU(	const TStridedMemoryView<CFloat2> &texcoords,
										const TStridedMemoryView<const SCentersIndex> &indices,
										const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs) const override;
	virtual void	FillTexcoordsFromAtlas(	const TStridedMemoryView<CFloat2> &texcoords,
											const TStridedMemoryView<const SCentersIndex> &indices,
											const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
											const TRectangleMapper<float> &rectMapper) const override;
	virtual void	FillTexcoordsFromUAndAtlas(	const TStridedMemoryView<CFloat2> &texcoords,
												const TStridedMemoryView<const SCentersIndex> &indices,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texUs,
												const TStridedMemoryView<const TStridedMemoryView<const float> > &texIds,
												const TRectangleMapper<float> &rectMapper) const override;

	virtual void	FillAlphaCursors(const TFillData<float, float> &datas) const override;
};

//----------------------------------------------------------------------------

class	CRibbonBillboarder_WideLinesQuad_Axis : public CRibbonBillboarder_WideLinesQuad
{
public:
	CRibbonBillboarder_WideLinesQuad_Axis() : CRibbonBillboarder_WideLinesQuad() { }
	~CRibbonBillboarder_WideLinesQuad_Axis() { }

	virtual void	Align(const CFloat4x4 &unusedView, const SAlignmentContext &context) const override;
	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context, hh_u32 particleStart, hh_u32 particleCount) const override;
};

//----------------------------------------------------------------------------

class	CRibbonBillboarder_WideLinesQuad_AxisSide : public CRibbonBillboarder_WideLinesQuad
{
public:
	CRibbonBillboarder_WideLinesQuad_AxisSide() : CRibbonBillboarder_WideLinesQuad() { }
	~CRibbonBillboarder_WideLinesQuad_AxisSide() { }

	virtual void	Align(const CFloat4x4 &unusedView, const SAlignmentContext &context) const override;
	virtual void	Align(const CFloat4x4 &view, const SAlignmentContext &context, hh_u32 particleStart, hh_u32 particleCount) const override;
};

//----------------------------------------------------------------------------
//
//	CRibbon_ThreadSort
//
//----------------------------------------------------------------------------

class	CRibbon_ThreadSort_Policy
{
public:
	static const hh_u64			kRibbonIdMask;

	bool			Setup(
		TAtomic<hh_u32>													*doneCount,
		hh_u32															totalCount,
		TStridedMemoryView<const hh_u64>								*outputMemoryView_RibbonId,
		TStridedMemoryView<const CRibbonBillboarder::SCentersIndex>		*outputMemoryView_Indcies,
		const TStridedMemoryView< const TStridedMemoryView<const hh_u32> >	&pagesSpawnerIDs,
		const TStridedMemoryView< const TStridedMemoryView<const hh_u32> >	&pagesParentIDs,
		const TStridedMemoryView< const TStridedMemoryView<const hh_u32> >	&pagesSelfIDs)
	{
		m_DoneCount = doneCount;
		m_TotalCount = totalCount;
		m_OutRibbonId = outputMemoryView_RibbonId;
		m_OutIndcies = outputMemoryView_Indcies;

		HH_ASSERT(pagesSpawnerIDs.Count() == pagesParentIDs.Count());
		HH_ASSERT(pagesSpawnerIDs.Count() == pagesSelfIDs.Count());
		m_SpawnerIDs = pagesSpawnerIDs;
		m_ParentIDs = pagesParentIDs;
		m_SelfIDs = pagesSelfIDs;
		return true;
	}

public: // public for internals
	struct SSortData
	{
		hh_u64								m_RawKey;
		CRibbonBillboarder::SCentersIndex	m_Index;
		HH_FORCEINLINE bool		operator < (const SSortData &other) { return m_RawKey < other.m_RawKey; }
		HH_FORCEINLINE bool		operator <= (const SSortData &other) { return m_RawKey <= other.m_RawKey; }
		HH_FORCEINLINE bool		operator == (const SSortData &other) { return m_RawKey == other.m_RawKey; }
	};
	struct	SSortKeyExtractor
	{
		typedef hh_u64						SortKeyType;
		HH_FORCEINLINE static SortKeyType	ExtractKey(const SSortData &element)
		{
			return element.m_RawKey;
		}
	};

protected:
	hh_u32		ReadyToSpawn(hh_u32 minBatchSize) { return m_TotalCount; }
	void		FillSource(hh_u32 start, hh_u32 count, TMemoryView<SSortData> &out, const TMemoryView<SSortData> &availableWorkingBufferIFN);
	void		Finished(const TMemoryView<const SSortData> &datasSorted);

	TAtomic<hh_u32>													*m_DoneCount;
	hh_u32															m_TotalCount;
	TStridedMemoryView<const hh_u64>								*m_OutRibbonId;
	TStridedMemoryView< const CRibbonBillboarder::SCentersIndex >	*m_OutIndcies;
	TStridedMemoryView< const TStridedMemoryView<const hh_u32> >	m_SpawnerIDs;
	TStridedMemoryView< const TStridedMemoryView<const hh_u32> >	m_ParentIDs;
	TStridedMemoryView< const TStridedMemoryView<const hh_u32> >	m_SelfIDs;
};

typedef TParallelSort<CRibbon_ThreadSort_Policy>		CRibbon_ThreadSort;

//----------------------------------------------------------------------------
__PK_API_END

#endif /* __GE_BILLBOARDS_RIBBON_H__ */
