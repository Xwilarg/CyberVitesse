#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2012/08/20 03:23 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__GE_PROBABILITY_FUNCTION_1D_H__
#define	__GE_PROBABILITY_FUNCTION_1D_H__

#include <hh_kernel/include/kr_delegates.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Binary-Search PDF : O(log2(N))
//
//----------------------------------------------------------------------------

struct	SImportanceFunctor
{
	virtual ~SImportanceFunctor() {}
	virtual float	operator () () = 0;
};

struct	SIndexRemapper
{
	virtual ~SIndexRemapper() {}
	virtual void	operator () (const TPair<hh_u32, float> *srcIndices, hh_u32 *dstIndices, hh_u32 count) = 0;
};

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CDiscreteProbabilityFunction1D
{
protected:
	TArray<float, TArrayAligned16>							m_CDF;
	TArray<hh_u32, TArrayAligned16>							m_IndexLookup;
	float													m_TotalDensity;

public:
	CDiscreteProbabilityFunction1D();

	void													Clean();
	bool													Setup(	hh_u32				elementCount,
																	SIndexRemapper		&indexRemapper,
																	SImportanceFunctor	&nextElementProbability,
																	float				indexRemapDiscrepancyThreshold = 0.01f,
																	float				minProbabilityThreshold = 1.0e-10f);

	HH_FORCEINLINE const TArray<float, TArrayAligned16>		&CDF() const { return m_CDF; }
	HH_FORCEINLINE const TArray<hh_u32, TArrayAligned16>	&IndexLookup() const { return m_IndexLookup; }	// TODO: Remove this in v1.10
	HH_FORCEINLINE float									TotalDensity() const { return m_TotalDensity; }
};

//----------------------------------------------------------------------------
//
//	Direct-indexing PDF : O(1)
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CDiscreteProbabilityFunction1D_O1
{
public:
	struct	SSlot
	{
		float		m_Proba;
		hh_u32		m_Other;
	};

	// You can keep these around to avoid allocating/freeing memory over and over
	// if you need to recompute the PDF multiple times
	struct	SWorkingBuffers
	{
		TArray<hh_u8, TArrayAligned16>	m_BufferA;
		TArray<hh_u8, TArrayAligned16>	m_BufferB;
	};

protected:
	TArray<SSlot, TArrayAligned16>							m_Samplee;
	float													m_TotalDensity;
	float													m_SampleeCount[8];

public:
	CDiscreteProbabilityFunction1D_O1();

	void													Clean();
	bool													Setup(hh_u32 elementCount, FastDelegate<float()> nextElementProbability, SWorkingBuffers *workingBuffers = null);
	bool													Setup(hh_u32 elementCount, FastDelegate<float(hh_u32 startOffset, const TMemoryView<float> &outDensities)> nextElementProbability, SWorkingBuffers *workingBuffers = null);
	void													SetupAndSteal(TArray<SSlot, TArrayAligned16> &other, float totalDensity);

	HH_FORCEINLINE float									TotalDensity() const { return m_TotalDensity; }
	HH_FORCEINLINE bool										Empty() const { return m_Samplee.Empty(); }

	HH_FORCEINLINE TMemoryView<const SSlot>					Samplee() const { return m_Samplee; }
	HH_FORCEINLINE const float								*SampleeCountX4() const { return m_SampleeCount; }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __GE_PROBABILITY_FUNCTION_1D_H__

