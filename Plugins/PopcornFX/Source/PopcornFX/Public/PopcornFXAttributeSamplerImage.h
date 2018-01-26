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

#include "PopcornFXPublic.h"

#include "PopcornFXAttributeSampler.h"

#include "PopcornFXAttributeSamplerImage.generated.h"

FWD_PK_API_BEGIN
class	CParticleSamplerDescriptor_Image;
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

// this forward declaration is here to help the UE Header Parser which seems to crash because of FWD_PK_API_...
class	UPopcornFXAttributeSamplerImage;

struct	FAttributeSamplerImageData;

/** Can override an Attribute Sampler **Image** by a **UTexture**. */
UCLASS(EditInlineNew, meta=(BlueprintSpawnableComponent), ClassGroup=PopcornFX)
class POPCORNFX_API UPopcornFXAttributeSamplerImage : public UPopcornFXAttributeSampler
{
	GENERATED_UCLASS_BODY()

public:
	/** Enable to allow PopcornFX to convert the texture at Runtime, if
	* the texture is not in a format directly samplable by PopcornFX.
	* /!\ It can take a significant amount of time to convert.
	*/
	UPROPERTY(Category = "PopcornFX AttributeSampler", EditAnywhere)
	uint32	bAllowTextureConvertionAtRuntime : 1;

	UFUNCTION(BlueprintCallable, Category="PopcornFX|AttributeSampler")
	void	SetTexture(class UTexture2D *InTexture);

	/** The texture to be sampled */
	UPROPERTY(Category="PopcornFX AttributeSampler", BlueprintReadOnly, EditAnywhere)
	class UTexture2D	*Texture;

	// overrides
	void			BeginDestroy() override;
#if WITH_EDITOR
	void			PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif // WITH_EDITOR

	// PopcornFX Internal
	virtual PopcornFX::CParticleSamplerDescriptor	*_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultImageDesc) override;

private:
	bool			RebuildImageSampler(const PopcornFX::CParticleSamplerDescriptor_Image *desc);
	bool			_RebuildImageSampler(const PopcornFX::CParticleSamplerDescriptor_Image *desc);

	FAttributeSamplerImageData	*m_Data;


};
