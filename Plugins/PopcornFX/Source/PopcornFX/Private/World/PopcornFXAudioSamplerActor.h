//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"
#include "PopcornFXAudioSampling.h"

#include "GameFramework/Actor.h"

#include "PopcornFXAudioSamplerActor.generated.h"

UCLASS(Blueprintable)
class POPCORNFX_API	APopcornFXAudioSamplerActor : public AActor, public FPopcornFXFillAudioBuffers
{
	GENERATED_UCLASS_BODY()
public:
	/** PopcornFXScene name */
	UPROPERTY(EditAnywhere, Category="PopcornFX")
	FName				SceneName;

	/** Spectrum data to refresh each frame */
	UPROPERTY(Transient, BlueprintReadWrite, Category="PopcornFX")
	TArray<float>	m_SpectrumData;

	/** Waveform data to refresh each frame*/
	UPROPERTY(Transient, BlueprintReadWrite, Category="PopcornFX")
	TArray<float>	m_WaveformData;
public:
	virtual const float	*GetRawSpectrumBuffer(const FName &ChannelName, uint32 &OutBufferSize) override;
	virtual const float	*GetRawWaveformBuffer(const FName &ChannelName, uint32 &OutBufferSize) override;

	virtual void		BeginPlay() override;
};
