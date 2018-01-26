#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/10/30 19:06 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __EU_SAMPLERS_H__
#define __EU_SAMPLERS_H__

#if	(PK_HAS_DETERMINISM != 0)
namespace	PopcornFX
{
	class	CAbstractRandomGenerator;
	typedef	CAbstractRandomGenerator	CAbstractGenerator;
}
#else
namespace	PopcornFX
{
	class	CAbstractRandomGenerator;
	class	CRandomGenerator;
	typedef	CRandomGenerator		CAbstractGenerator;
}
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// we might want a structure specific to volume sampling, and another one specific to surface sampling (with extra surface streams and stuff?)

class	HH_ENGINEUTILS_EXPORT CSampleDataStream
{
public:
	enum	EMode	// bad! that's specific to gf_mesh's sampling implementation!
	{
		NaiveRandom = 0,
		UniformDistribution
		//Regular = 0,
		//Random,
	};

	enum	EStream
	{
		Stream_Positions = 0,
		Stream_Normals,
		Stream_Tangents,
		Stream_Texcoords,
		Stream_Colors,
		Stream_Velocities,

		__MaxStreams
	};

	enum
	{
		StreamFlagsMask_Positions	= 1 << Stream_Positions,
		StreamFlagsMask_Normals		= 1 << Stream_Normals,
		StreamFlagsMask_Tangents	= 1 << Stream_Tangents,
		StreamFlagsMask_Texcoords	= 1 << Stream_Texcoords,
		StreamFlagsMask_Colors		= 1 << Stream_Colors,
		StreamFlagsMask_Velocities	= 1 << Stream_Velocities,
	};

private:
	static const hh_u32			StreamFlagsMask = 0xFF;
	HH_STATIC_ASSERT((1 << (__MaxStreams - 1)) < StreamFlagsMask);

	hh_u32								m_Flags;
	hh_u32								m_ElementCount;
	TStridedMemoryView<CFloat3>			m_OutputPositions;
	TStridedMemoryView<CFloat3>			m_OutputNormals;
	TStridedMemoryView<CFloat4>			m_OutputTangents;
	TStridedMemoryView<CFloat2>			m_OutputTexcoords;
	TStridedMemoryView<CFloat4>			m_OutputColors;
	TStridedMemoryView<CFloat3>			m_OutputVelocities;

	TStridedMemoryView<const CFloat3>	m_InputParametricCoords;
	TStridedMemoryView<CFloat3>			m_OutputParametricCoords;

	CAbstractRandomGenerator			*m_Sampler;

public:
	CSampleDataStream(CAbstractRandomGenerator *sampler = null, EMode samplingMode = NaiveRandom);

	void								MakeViewFromDataStream(const CSampleDataStream &other, hh_u32 firstIndex, hh_u32 count);
	void								MakeViewFromDataStream_KeepOwnSampler(const CSampleDataStream &other, hh_u32 firstIndex, hh_u32 count);

	EMode								m_SamplingMode;
	CGuid								m_SrcTexcoordsChannelKey;	// CGuid::INVALID for default. in case it's a mesh, will contain the index of the vertex stream to sample.
	CGuid								m_SrcColorsChannelKey;		// CGuid::INVALID for default. in case it's a mesh, will contain the index of the vertex stream to sample.

	// FIXME: optimize this by making these private and forcing users to use setters that will keep track of the number of active streams,
	// the element count, etc..., and will be able to do basic safety checks
	// this will allow us to perform some useful optimizations later. (if we only have positions, we could use the cheap strided memview permutator
	// in place of the expensive generic data-stream permutator)

	HH_FORCEINLINE hh_u32				StreamFlags() const { return m_Flags & StreamFlagsMask; }
	HH_FORCEINLINE hh_u32				ElementCount() const { return m_ElementCount; }
	template<EStream _Which>
	HH_FORCEINLINE bool					HasStream() const { return (m_Flags & (1 << _Which)) != 0; }
	HH_FORCEINLINE bool					HasStream(EStream which) const { return (m_Flags & (1 << which)) != 0; }

	HH_FORCEINLINE const TStridedMemoryView<CFloat3>		&OutputPositions() const { return m_OutputPositions; }
	HH_FORCEINLINE const TStridedMemoryView<CFloat3>		&OutputNormals() const { return m_OutputNormals; }
	HH_FORCEINLINE const TStridedMemoryView<CFloat4>		&OutputTangents() const { return m_OutputTangents; }
	HH_FORCEINLINE const TStridedMemoryView<CFloat2>		&OutputTexcoords() const { return m_OutputTexcoords; }
	HH_FORCEINLINE const TStridedMemoryView<CFloat4>		&OutputColors() const { return m_OutputColors; }
	HH_FORCEINLINE const TStridedMemoryView<CFloat3>		&OutputVelocities() const { return m_OutputVelocities; }

	HH_FORCEINLINE const TStridedMemoryView<const CFloat3>	&InputParametricCoords() const { return m_InputParametricCoords; }
	HH_FORCEINLINE const TStridedMemoryView<CFloat3>		&OutputParametricCoords() const { return m_OutputParametricCoords; }

	void								SetOutputPositions(const TStridedMemoryView<CFloat3> &stream);
	void								SetOutputNormals(const TStridedMemoryView<CFloat3> &stream);
	void								SetOutputTangents(const TStridedMemoryView<CFloat4> &stream);
	void								SetOutputTexcoords(const TStridedMemoryView<CFloat2> &stream);
	void								SetOutputColors(const TStridedMemoryView<CFloat4> &stream);
	void								SetOutputVelocities(const TStridedMemoryView<CFloat3> &stream);

	void								SetOutputStreamGeneric(const SStridedMemoryViewRawStorage &genericStream, EStream streamId);

	void								SetInputParametricCoords(const TStridedMemoryView<const CFloat3> &stream);
	void								SetOutputParametricCoords(const TStridedMemoryView<CFloat3> &stream);

	void								ClearOutputStreams(hh_u32 streamMasks) const;

	CAbstractRandomGenerator			*Sampler() const { HH_ASSERT(m_Sampler != null); return m_Sampler; }
	void								SetSampler(CAbstractGenerator *sampler);	// if null, will use the global sampler 'Random::DefaultGenerator()'
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __EU_SAMPLERS_H__
