//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXAudioSampling.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXStats.h"
#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------

FPopcornFXFillAudioBuffers::FPopcornFXFillAudioBuffers()
{

}

//----------------------------------------------------------------------------

FPopcornFXFillAudioBuffers::~FPopcornFXFillAudioBuffers()
{
	for (hh_i32 iAudioPyramid = 0; iAudioPyramid < m_AudioSpectrumPyramids.Num(); ++iAudioPyramid)
		CleanAudioPyramid(m_AudioSpectrumPyramids[iAudioPyramid]);
	for (hh_i32 iAudioPyramid = 0; iAudioPyramid < m_AudioWaveformPyramids.Num(); ++iAudioPyramid)
		CleanAudioPyramid(m_AudioWaveformPyramids[iAudioPyramid]);
	m_AudioSpectrumPyramids.Empty();
	m_AudioWaveformPyramids.Empty();
}

//----------------------------------------------------------------------------

void	FPopcornFXFillAudioBuffers::RegisterAudioChannel(FName ChannelName)
{
	if (!ChannelName.IsValid())
		return;
	m_AudioChannels.AddUnique(ChannelName);
}

//----------------------------------------------------------------------------

void	FPopcornFXFillAudioBuffers::UnregisterAudioChannel(FName ChannelName)
{
	if (!ChannelName.IsValid())
		return;
	m_AudioChannels.Remove(ChannelName);
}

//----------------------------------------------------------------------------

void	FPopcornFXFillAudioBuffers::CleanAudioPyramid(SAudioPyramid &audioPyramid)
{
	TArray<float*>		&pyramid = audioPyramid.m_ConvolutionPyramid;
	const hh_u32		pyramidSize = pyramid.Num();
	for (hh_u32 iLevel = 0; iLevel < pyramidSize; ++iLevel)
	{
		if (pyramid[iLevel] == null)
			break;
		HH_FREE(pyramid[iLevel]);
		pyramid[iLevel] = null;
	}
	pyramid.Empty();

	audioPyramid.m_AudioSampleCount = 0;
	audioPyramid.m_Valid = false;
}

//----------------------------------------------------------------------------
//
//					Game thread, before/after PKFX simulation
//
//----------------------------------------------------------------------------

void	FPopcornFXFillAudioBuffers::BuildAudioPyramid(SAudioPyramid &audioPyramid, const float * const rawBuffer, uint32 bufferSize)
{
	HH_NAMEDSCOPEDPROFILE_C("FPopcornFXFillAudioBuffers::BuildAudioPyramid", POPCORNFX_UE_PROFILER_COLOR);

	TArray<float*>	&pyramid = audioPyramid.m_ConvolutionPyramid;
	if (pyramid.Num() == 0 || audioPyramid.m_AudioSampleCount != bufferSize)
	{
		CleanAudioPyramid(audioPyramid);

		const hh_u32	pyramidSize = PopcornFX::IntegerTools::Log2(bufferSize) + 1;
		pyramid.SetNum(pyramidSize);

		hh_u32	sampleCount = bufferSize;
		for (hh_u32 iLevel = 0; iLevel < pyramidSize; ++iLevel)
		{
			HH_ASSERT(sampleCount != 0);

			// Allocate border bytes to ensure filtering doesn't overflow
			pyramid[iLevel] = static_cast<float*>(HH_CALLOC_ALIGNED((2 + sampleCount + 2) * sizeof(float), 0x10));
			if (!HH_VERIFY(pyramid[iLevel]))
			{
				CleanAudioPyramid(audioPyramid);
				return;
			}
			sampleCount >>= 1;
		}
		audioPyramid.m_AudioSampleCount = bufferSize;
	}

	float	*highResAudioData = pyramid[0];
	HH_ASSERT(highResAudioData != null);

	PopcornFX::Mem::Copy(highResAudioData + 2, rawBuffer, sizeof(*highResAudioData) * bufferSize);

	// Compute convolution pyramid
	hh_u32			sampleCount = bufferSize;
	const hh_u32	pyramidSize = pyramid.Num();
	for (hh_u32 iLevel = 0; iLevel < pyramidSize; ++iLevel)
	{
		HH_ASSERT(sampleCount > 0);

		float	* __restrict dstRawAudioData = 2 + pyramid[iLevel];
		if (iLevel > 0)
		{
			const float	* __restrict srcRawAudioData = 2 + pyramid[iLevel - 1];

			for (hh_u32 iSample = 0; iSample < sampleCount; ++iSample)
				dstRawAudioData[iSample] = 0.5f * (srcRawAudioData[iSample * 2 + 0] + srcRawAudioData[iSample * 2 + 1]);
		}

		const float	firstValue = dstRawAudioData[0];
		const float	lastValue = dstRawAudioData[bufferSize - 1];

		// Fill border with same values
		dstRawAudioData[-1] = firstValue;
		dstRawAudioData[-2] = firstValue;
		dstRawAudioData[sampleCount + 0] = lastValue;
		dstRawAudioData[sampleCount + 1] = lastValue;

		sampleCount >>= 1;
	}
	audioPyramid.m_Valid = true;
}

//----------------------------------------------------------------------------

void	FPopcornFXFillAudioBuffers::PreUpdate()
{
	HH_ASSERT(IsInGameThread());
	HH_ASSERT(m_AudioSpectrumPyramids.Num() == m_AudioWaveformPyramids.Num());

	const hh_u32	audioChannelCount = m_AudioChannels.Num();
	if (m_AudioSpectrumPyramids.Num() != audioChannelCount)
	{
		m_AudioSpectrumPyramids.SetNum(audioChannelCount);
		m_AudioWaveformPyramids.SetNum(audioChannelCount);
	}

	for (hh_u32 iChannel = 0; iChannel < audioChannelCount; ++iChannel)
	{
		const FName	&audioChannelName = m_AudioChannels[iChannel];

		HH_ASSERT(!audioChannelName.IsNone() && audioChannelName.IsValid());

		uint32		spectrumBufferSize = 0;
		uint32		waveformBufferSize = 0;
		const float	*rawSpectrumBuffer = null;
		const float	*rawWaveformBuffer = null;

		{
			SCOPE_CYCLE_COUNTER(STAT_PopcornFX_ComputeAudioSpectrumTime);
			rawSpectrumBuffer = GetRawSpectrumBuffer(audioChannelName, spectrumBufferSize);
		}
		{
			SCOPE_CYCLE_COUNTER(STAT_PopcornFX_ComputeAudioWaveformTime);
			rawWaveformBuffer = GetRawWaveformBuffer(audioChannelName, waveformBufferSize);
		}
		HH_ASSERT(spectrumBufferSize == waveformBufferSize ||
				  spectrumBufferSize == 0 ||
				  waveformBufferSize == 0);

		if (rawSpectrumBuffer != null && spectrumBufferSize > 0)
		{
			HH_ASSERT(PopcornFX::IntegerTools::IsPowerOfTwo(spectrumBufferSize));
			BuildAudioPyramid(m_AudioSpectrumPyramids[iChannel], rawSpectrumBuffer, spectrumBufferSize);
		}
		if (rawWaveformBuffer != null && waveformBufferSize > 0)
		{
			HH_ASSERT(PopcornFX::IntegerTools::IsPowerOfTwo(waveformBufferSize));
			BuildAudioPyramid(m_AudioWaveformPyramids[iChannel], rawWaveformBuffer, waveformBufferSize);
		}
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXFillAudioBuffers::PostUpdate()
{
	for (hh_i32 iAudioPyramid = 0; iAudioPyramid < m_AudioSpectrumPyramids.Num(); ++iAudioPyramid)
		m_AudioSpectrumPyramids[iAudioPyramid].m_Valid = false;
	for (hh_i32 iAudioPyramid = 0; iAudioPyramid < m_AudioWaveformPyramids.Num(); ++iAudioPyramid)
		m_AudioWaveformPyramids[iAudioPyramid].m_Valid = false;
}

//----------------------------------------------------------------------------
//
//					Called during PKFX simulation
//
//----------------------------------------------------------------------------

const float * const	*FPopcornFXFillAudioBuffers::AsyncGetAudioSpectrum(const FName &channelName, uint32 &outBaseCount) const
{
	HH_ASSERT(m_AudioChannels.Num() == m_AudioSpectrumPyramids.Num());
	HH_ASSERT(channelName.IsValid());

	const hh_u32	audioChannelCount = m_AudioChannels.Num();
	for (hh_u32 iChannel = 0; iChannel < audioChannelCount; ++iChannel)
	{
		if (m_AudioChannels[iChannel] != channelName)
			continue;
		const SAudioPyramid	&audioPyramid = m_AudioSpectrumPyramids[iChannel];
		if (!audioPyramid.m_Valid)
			break;

		outBaseCount = 1U << (audioPyramid.m_ConvolutionPyramid.Num() - 1);
		return audioPyramid.m_ConvolutionPyramid.GetData();
	}
	return null;
}

//----------------------------------------------------------------------------

const float * const	*FPopcornFXFillAudioBuffers::AsyncGetAudioWaveform(const FName &channelName, uint32 &outBaseCount) const
{
	HH_ASSERT(m_AudioChannels.Num() == m_AudioWaveformPyramids.Num());
	HH_ASSERT(channelName.IsValid());

	const hh_u32	audioChannelCount = m_AudioChannels.Num();
	for (hh_u32 iChannel = 0; iChannel < audioChannelCount; ++iChannel)
	{
		if (m_AudioChannels[iChannel] != channelName)
			continue;
		const SAudioPyramid	&audioPyramid = m_AudioWaveformPyramids[iChannel];
		if (!audioPyramid.m_Valid)
			break;

		outBaseCount = 1U << (audioPyramid.m_ConvolutionPyramid.Num() - 1);
		return audioPyramid.m_ConvolutionPyramid.GetData();
	}
	return null;
}

//----------------------------------------------------------------------------
