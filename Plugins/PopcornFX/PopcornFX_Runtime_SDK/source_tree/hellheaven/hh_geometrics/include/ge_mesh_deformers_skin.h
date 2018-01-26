#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2004/04/06 7:13 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__GE_MESH_DEFORMERS_SKIN_H__
#define	__GE_MESH_DEFORMERS_SKIN_H__

#include <hh_kernel/include/kr_threads.h>	// for 'CThreadManager::MaxThreadCount'
#include <hh_kernel/include/kr_thread_pool.h>
#include <hh_kernel/include/kr_delegates.h>
#include <hh_kernel/include/kr_callbacks.h>
#include <hh_geometrics/include/ge_mesh_deformers.h>
#include <hh_geometrics/include/ge_skeleton.h>

#if	!defined(PK_GEOMETRICS_COLLECT_SKINNING_STATS)
#	if	!defined(HH_RETAIL) && (HH_GEOMETRICS_BUILD_MESH_DEFORMERS != 0)
#		define	PK_GEOMETRICS_COLLECT_SKINNING_STATS	1
#	else
#		define	PK_GEOMETRICS_COLLECT_SKINNING_STATS	0
#	endif
#elif (HH_GEOMETRICS_BUILD_MESH_DEFORMERS == 0)
#	undef	PK_GEOMETRICS_COLLECT_SKINNING_STATS
#	define	PK_GEOMETRICS_COLLECT_SKINNING_STATS	0
#endif

#if	(HH_GEOMETRICS_BUILD_MESH_DEFORMERS != 0)

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if (PK_GEOMETRICS_COLLECT_SKINNING_STATS != 0)

struct	HH_GEOMETRICS_EXPORT SSkinningStatsReport
{
	hh_u32					m_MeshesSkinned;

	hh_u32					m_SkinnedVerticesCount_P;
	hh_u32					m_SkinnedVerticesCount_PN;
	hh_u32					m_SkinnedVerticesCount_PNT;
	hh_u32					m_SkinnedVerticesCount_Misc;

	hh_u32					m_SkinnedVerticesCycles_P;
	hh_u32					m_SkinnedVerticesCycles_PN;
	hh_u32					m_SkinnedVerticesCycles_PNT;
	hh_u32					m_SkinnedVerticesCycles_Misc;

	hh_u32					m_NormalizedVectorsCount;
	hh_u32					m_NormalizedVectorsCycles;

	TStaticArray<hh_u32, 5>	m_SkinnedVerticesPerLevel;	// [0] == 1 w/v, [1] == 2 w/v, ... [4] == more than 4 w/v

	hh_u8					m_Padding[Memory::CacheLineSize - ((11*sizeof(hh_u32) + sizeof(TStaticArray<hh_u32, 5>)) % Memory::CacheLineSize)];

	SSkinningStatsReport();

	void					Clear();
	SSkinningStatsReport	&operator += (const SSkinningStatsReport &other);
};

HH_STATIC_ASSERT(sizeof(SSkinningStatsReport) % Memory::CacheLineSize == 0);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CSkinningStats
{
protected:
	static SSkinningStatsReport			m_ThreadArray[CThreadManager::MaxThreadCount];	// reset on Reset()
public:
	static SSkinningStatsReport			&ThreadLocalStats(CThreadID threadId);
	static void							ComputeGlobalStats(SSkinningStatsReport &outReport);
	static void							Reset();
};

//----------------------------------------------------------------------------
#else // (PK_GEOMETRICS_COLLECT_SKINNING_STATS != 0)
// dummy implem when disabled

class	HH_GEOMETRICS_EXPORT CSkinningStats
{
public:
	static void							Reset() {}
};

#endif // (PK_GEOMETRICS_COLLECT_SKINNING_STATS != 0)

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CBaseSkinningStreams
{
protected:
	const float							*m_WeightsPtr;
	const void							*m_IndicesPtr;
	hh_u32								m_Count;
	hh_u32								m_IndexSizeInBytes;
	hh_u32								m_VertexCount;
	hh_u8								m_MaxInfluencesPerVertex;	// max 255 influences, this is critical, we use some stack allocs
	TMemoryView<const hh_u32>			m_InfluenceLevelStartIDs;

public:
	CBaseSkinningStreams()
	:	m_WeightsPtr(null)
	,	m_IndicesPtr(null)
	,	m_Count(0)
	,	m_IndexSizeInBytes(0)
	,	m_VertexCount(0)
	,	m_MaxInfluencesPerVertex(0)
	{
	}
	virtual ~CBaseSkinningStreams() {}

	hh_u32							IndexSize() const { return m_IndexSizeInBytes; }
	HH_FORCEINLINE hh_u32			Count() const { return m_Count; }
	HH_FORCEINLINE hh_u32			VertexCount() const { return m_VertexCount; }
	HH_FORCEINLINE hh_u32			MaxInfluencesPerVertex() const { return m_MaxInfluencesPerVertex; }

	virtual CBaseSkinningStreams	*Clone() const = 0;

	// any nonzero value for 'largestExpectedIndex' will skip scanning the input indices to find the largest index,
	// and will use that value directly to select wether it should pack indices to 8-bits or 16-bits
	static CBaseSkinningStreams		*BuildFromUnpackedStreams(	hh_u32							vertexCount,
																const TMemoryView<const float>	&weights,
																const TMemoryView<const hh_u32>	&indices,
																const TMemoryView<const hh_u32>	&influenceLevelStartIDs = TMemoryView<const hh_u32>(),
																hh_u32							largestExpectedIndex = 0);

	static CBaseSkinningStreams		*BuildFromUnpackedStreams(	hh_u32							vertexCount,
																const TMemoryView<const float>	&weights,
																const TMemoryView<const hh_u8>	&indices,	// 8-bit indices
																const TMemoryView<const hh_u32>	&influenceLevelStartIDs = TMemoryView<const hh_u32>());

	static CBaseSkinningStreams		*BuildFromUnpackedStreams(	hh_u32							vertexCount,
																const TMemoryView<const float>	&weights,
																const TMemoryView<const hh_u16>	&indices,	// 16-bit indices
																const TMemoryView<const hh_u32>	&influenceLevelStartIDs = TMemoryView<const hh_u32>());

#if	!defined(HH_RETAIL)
	// These costs a lot: has to iterate through all weights
	void							Debug_ScanWeights(hh_u32 &outUnsortedCount, hh_u32 &outUnnormalizedCount, float kNormalizationTolerance = 1.0e-2f) const;
	bool							Debug_ScanWeightsAndCheckValidity(float kNormalizationTolerance = 1.0e-2f) const { hh_u32 s = 0; hh_u32 n = 0; Debug_ScanWeights(s, n, kNormalizationTolerance); return s + n == 0; }
#endif

	const float						*WeightStream() const { return m_WeightsPtr; }
	const void						*IndexStream() const { return m_IndicesPtr; }
	template<typename _IndexType>
	const _IndexType				*IndexStream() const { HH_ASSERT(m_IndexSizeInBytes == sizeof(_IndexType)); return static_cast<const _IndexType*>(m_IndicesPtr); }
	virtual hh_u32					IndexStreamSizeInBytes() const = 0;

	TMemoryView<const hh_u32>		InfluenceLevelStartIDs() const { return m_InfluenceLevelStartIDs; }
};

//----------------------------------------------------------------------------
//
//	Helper class: skinning streams proxy to your own data,
//	setup from pointers to weights and indices, does not manage the memory,
//	you'll have to make sure the pointers remain valid for as long as the
//	proxy lives.
//
//	you can use either of those specializations:
//	- TBaseSkinningStreamsProxy<hh_u8>
//	- TBaseSkinningStreamsProxy<hh_u16>
//
//----------------------------------------------------------------------------

template<typename _IndexType>
class	TBaseSkinningStreamsProxy : public CBaseSkinningStreams
{
public:
	// Setup from native index format
	bool							Setup(	hh_u32								vertexCount,
											const TMemoryView<const float>		&weights,
											const TMemoryView<const _IndexType>	&indices,
											const TMemoryView<const hh_u32>		&influenceLevelStartIDs = TMemoryView<const hh_u32>())
	{
		HH_ASSERT(!weights.Empty());
		HH_ASSERT(weights.Count() == indices.Count());

		m_WeightsPtr = weights.Data();
		m_IndicesPtr = indices.Data();
		m_Count = weights.Count();
		m_IndexSizeInBytes = sizeof(_IndexType);
		m_VertexCount = vertexCount;

		// cull dummy influences. if we have a single one, and it starts at ID = 0,
		// it's a regular rigid skinning, no need to store influences.
		if (!influenceLevelStartIDs.Empty() &&
			(influenceLevelStartIDs.Count() > 1 || influenceLevelStartIDs.First() != 0))
		{
			m_MaxInfluencesPerVertex = influenceLevelStartIDs.Count();
			m_InfluenceLevelStartIDs = influenceLevelStartIDs;
		}
		else
		{
			m_MaxInfluencesPerVertex = weights.Count() / vertexCount;
			m_InfluenceLevelStartIDs.Clear();
			HH_ASSERT(weights.Count() == vertexCount * m_MaxInfluencesPerVertex);
		}
		return true;
	}

	virtual CBaseSkinningStreams	*Clone() const override
	{
		return HH_NEW(TBaseSkinningStreamsProxy<_IndexType>(*this));
	}

	virtual hh_u32					IndexStreamSizeInBytes() const override
	{
		return Count() * sizeof(_IndexType);
	}
};

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(SkinMergeJob);

//----------------------------------------------------------------------------

struct	HH_GEOMETRICS_EXPORT SSkinContext
{
	typedef FastDelegate<void(hh_u32, hh_u32, const SSkinContext &ctx)>		CbCustomProcess;
	typedef FastDelegate<void(const SSkinContext &ctx)>						CbCustomFinish;
//	typedef TCallbackChain<void(hh_u32, hh_u32, const SSkinContext &ctx)>	CustomProcessDelegateChain;

	const CBaseSkinningStreams			*m_SkinningStreams;

	// source streams:
	TStridedMemoryView<const CFloat3>	m_SrcPositions;
	TStridedMemoryView<const CFloat3>	m_SrcNormals;
	TStridedMemoryView<const CFloat3>	m_SrcTangents;

	// destination streams:
	TStridedMemoryView<CFloat3>			m_DstPositions;
	TStridedMemoryView<CFloat3>			m_DstNormals;
	TStridedMemoryView<CFloat4>			m_DstTangents;

	CbCustomProcess						m_CustomProcess_PreSkin;
	CbCustomProcess						m_CustomProcess_PostSkin;
	CbCustomFinish						m_CustomFinish;

	SSkinContext();
	~SSkinContext();
};

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CSkinAsyncContext : public CNonCopyable
{
public:
	Threads::CEvent		m_TasksCompletionEvent;
	PSkinMergeJob		m_SkinMergeJob;

	CSkinAsyncContext();
	~CSkinAsyncContext();
};

//----------------------------------------------------------------------------

enum
{
	SkinFlags_DisableRenormalization	= 0x1,
	SkinFlags_UseMultipassSkinning		= 0x2,
};

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CSkeletalSkinnerSimple
{
public:
	static void					AsyncSkinStart(	CSkinAsyncContext		&asyncSkinContext,
												const CSkeletonView		&skeleton,			// only uses skeleton.InverseMatrices();
												const SSkinContext		&skinContext,
												Threads::CAbstractPool	*threadPool = null);

	static bool					AsyncSkinWait(	CSkinAsyncContext		&asyncSkinContext,
												CAABB					*outBBox,	// outBBox will be filled only if the function returns true, otherwise, its contents are left unchanged. if 'block' is true, the function will always return true.
												bool					block = true,
												Threads::CAbstractPool	*threadPool = null); // must be the same as AsyncSkinStart
};

//----------------------------------------------------------------------------
__PK_API_END
	
#endif // HH_GEOMETRICS_BUILD_MESH_DEFORMERS

#endif // __GE_MESH_DEFORMERS_SKIN_H__

