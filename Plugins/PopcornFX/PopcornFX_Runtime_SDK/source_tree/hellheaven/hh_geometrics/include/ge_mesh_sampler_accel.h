#pragma once
//----------------------------------------------------------------------------
// Created on Mon 2015/08/26 14:47 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_MESH_SAMPLER_ACCEL_H__
#define __GE_MESH_SAMPLER_ACCEL_H__

#include "hh_geometrics/include/ge_mesh_resource.h"
#include "hh_geometrics/include/ge_probability_function_1d.h"	// for CMeshSurfaceSamplerStructuresRandom

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Sampling accel : Surface sampling
//
//----------------------------------------------------------------------------

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
class	CMeshSurfaceSamplerStructuresRandom
{
private:
	CDiscreteProbabilityFunction1D_O1		m_PDF;	// Probability Density Function with O(1) eval

public:
	bool									Build(	CMeshIStream										&istream,
													const TStridedMemoryView<const CFloat3>				&positions,
													CDiscreteProbabilityFunction1D_O1::SWorkingBuffers	*workingBuffers = null);

	bool									Build(	CMeshIStream										&istream,
													const TStridedMemoryView<const CFloat3>				&positions,
													const TStridedMemoryView<const float>				&densities,
													CDiscreteProbabilityFunction1D_O1::SWorkingBuffers	*workingBuffers = null);

	void									BuildAndSteal(TArray<CDiscreteProbabilityFunction1D_O1::SSlot, TArrayAligned16> &pdf, float totalDensity);

	const CDiscreteProbabilityFunction1D_O1	&ProbaFunc() const { return m_PDF; }
};
#endif	// (HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)

//----------------------------------------------------------------------------
//
//	Sampling accel : Volume sampling
//
//----------------------------------------------------------------------------

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)
class	CMeshVolumeSamplerStructuresRandom
{
private:
	CDiscreteProbabilityFunction1D_O1		m_PDF;	// Probability Density Function with O(1) eval
	const void								*m_TetrahedralIndices;

public:
	CMeshVolumeSamplerStructuresRandom();

	bool									Build(	CMeshIStream										&istream,
													const TStridedMemoryView<const CFloat3>				&positions,
													const CFloat3										*otherPositions,
													const void											*tetraIndices,
													hh_u32												tetraIndexCount,
													CDiscreteProbabilityFunction1D_O1::SWorkingBuffers	*workingBuffers = null);

	bool									Build(	CMeshIStream										&istream,
													const TStridedMemoryView<const CFloat3>				&positions,
													const TStridedMemoryView<const float>				&densities,
													const CFloat3										*otherPositions,
													const float											*otherDensities,
													const void											*tetraIndices,
													hh_u32												tetraIndexCount,
													CDiscreteProbabilityFunction1D_O1::SWorkingBuffers	*workingBuffers = null);

	void									BuildAndSteal(TArray<CDiscreteProbabilityFunction1D_O1::SSlot, TArrayAligned16> &pdf, float totalDensity);

	const void								*TetrahedralIndices() const { return m_TetrahedralIndices; };
	const CDiscreteProbabilityFunction1D_O1	&ProbaFunc() const { return m_PDF; }
};
#endif	// (HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)

//----------------------------------------------------------------------------
//
//	Sampling accel : UV to PCoords
//
//----------------------------------------------------------------------------

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)

struct	SMeshUV2PCBuildConfig
{
	hh_u32		m_MaxDepth;
	hh_u32		m_MaxTrianglesPerLeaf;

	SMeshUV2PCBuildConfig() : m_MaxDepth(10), m_MaxTrianglesPerLeaf(4) {}
};

//----------------------------------------------------------------------------

class	CMeshSurfaceSamplerStructuresFromUV
{
private:
	CMeshVStream	*m_VStream;
	CMeshIStream	*m_IStream;
	TArray<hh_u32>	m_Array;

	CRect			m_UVBounds;

public:
	CMeshSurfaceSamplerStructuresFromUV();
	~CMeshSurfaceSamplerStructuresFromUV();

	bool						Build(const SMeshUV2PCBuildConfig &buildConfig, CMeshVStream &vstream, CMeshIStream &istream);
	void						BuildAndSteal(TArray<hh_u32> &nodes, const CRect &bounds, CMeshVStream &vstream, CMeshIStream &istream);

	void						Condense();

	const CRect					&UVBounds() const { return m_UVBounds; }
	TMemoryView<const CFloat2>	UVs() const { if (!HH_VERIFY(m_VStream != null)) return TMemoryView<const CFloat2>(); return m_VStream->Texcoords().ToMemoryViewIFP(); }
	const CMeshIStream			*IndexStream() const { return m_IStream; }
	TMemoryView<const hh_u32>	Array() const { return m_Array; }
};
#endif	// (HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)

//----------------------------------------------------------------------------
__PK_API_END

//----------------------------------------------------------------------------
#endif // __GE_MESH_SAMPLER_ACCEL_H__
