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

#include "MovieSceneNameableTrack.h"
#include "PopcornFXAttributeTrack.generated.h"

/**
 * Handles manipulation of attributes in a movie scene.
 */
UCLASS(MinimalAPI)
class UPopcornFXAttributeTrack : public UMovieSceneNameableTrack
{
	GENERATED_UCLASS_BODY()
public:
	// UMovieSceneTrack interface
	virtual FMovieSceneEvalTemplatePtr	CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
	virtual UMovieSceneSection			*CreateNewSection() override;

	virtual void	RemoveAllAnimationData() override { Sections.Empty(); }
	virtual bool	HasSection(const UMovieSceneSection &section) const override { return Sections.Contains(&section); }
	virtual void	AddSection(UMovieSceneSection &section) override { Sections.Add(&section); }
	virtual void	RemoveSection(UMovieSceneSection &section) override { Sections.Remove(&section); }
	virtual bool	IsEmpty() const override { return Sections.Num() == 0; }

	virtual TRange<float>						GetSectionBoundaries() const override;
	virtual const TArray<UMovieSceneSection*>	&GetAllSections() const override { return Sections; }

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
#endif

	void	AddScalarAttributeKey(FName attrName, float position, float value);
	void	AddVectorAttributeKey(FName attrName, float position, FVector value);
	void	AddColorAttributeKey(FName attrName, float position, FLinearColor value);

private:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};
