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
#include "PopcornFXPlayTrack.generated.h"

/**
* Defines the types of state keys.
*/
UENUM()
namespace	EPopcornFXPlayStateKey
{
	enum	Type
	{
		Start = 0,
		Stop = 1,
		Toggle = 2,
	};
}

/**
 * Handles Start/Stop of emitters.
 */
UCLASS(MinimalAPI)
class UPopcornFXPlayTrack : public UMovieSceneNameableTrack
{
	GENERATED_UCLASS_BODY()
public:
	// UMovieSceneTrack interface
	virtual UMovieSceneSection			*CreateNewSection() override;
	FMovieSceneEvalTemplatePtr			CreateTemplateForSection(const UMovieSceneSection &inSection) const override;

	virtual void	RemoveAllAnimationData() override { Sections.Empty(); }
	virtual bool	HasSection(const UMovieSceneSection &section) const override { return Sections.Contains(&section); }
	virtual void	AddSection(UMovieSceneSection &section) override { Sections.Add(&section); }
	virtual void	RemoveSection(UMovieSceneSection &section) override { Sections.Remove(&section); }
	virtual bool	IsEmpty() const override { return Sections.Num() == 0; }

	void			AddNewSection(float sectionTime);

	virtual TRange<float>						GetSectionBoundaries() const override;
	virtual const TArray<UMovieSceneSection*>	&GetAllSections() const override { return Sections; }

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
#endif
private:
	UPROPERTY()
	TArray<UMovieSceneSection*>	Sections;
};
