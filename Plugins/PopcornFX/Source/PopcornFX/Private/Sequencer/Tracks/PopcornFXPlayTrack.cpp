//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "Sequencer/Tracks/PopcornFXPlayTrack.h"
#include "PopcornFXPlugin.h"
#include "Sequencer/Templates/PopcornFXPlaySectionTemplate.h"

#include "MovieSceneCommonHelpers.h"

#define LOCTEXT_NAMESPACE "PopcornFXPlayTrack"

//----------------------------------------------------------------------------

UPopcornFXPlayTrack::UPopcornFXPlayTrack(const FObjectInitializer &objectInitializer)
	: Super(objectInitializer)
{
#if WITH_EDITORONLY_DATA
	TrackTint = FColor(255, 255, 255, 160);
#endif
}

//----------------------------------------------------------------------------

FMovieSceneEvalTemplatePtr	UPopcornFXPlayTrack::CreateTemplateForSection(const UMovieSceneSection &inSection) const
{
	return FPopcornFXPlaySectionTemplate(*CastChecked<UMovieSceneParticleSection>(&inSection));
}

//----------------------------------------------------------------------------

UMovieSceneSection	*UPopcornFXPlayTrack::CreateNewSection()
{
	return NewObject<UMovieSceneParticleSection>(this);
}

//----------------------------------------------------------------------------

TRange<float>	UPopcornFXPlayTrack::GetSectionBoundaries() const
{
	TArray<TRange<float>>	bounds;

	for (int32 SectionIndex = 0; SectionIndex < Sections.Num(); ++SectionIndex)
		bounds.Add(Sections[SectionIndex]->GetRange());
	return TRange<float>::Hull(bounds);
}

//----------------------------------------------------------------------------

void	UPopcornFXPlayTrack::AddNewSection(float sectionTime)
{
	if (MovieSceneHelpers::FindSectionAtTime(Sections, sectionTime) == null)
	{
		UMovieSceneParticleSection	*newSection = Cast<UMovieSceneParticleSection>(CreateNewSection());
		newSection->SetStartTime(sectionTime);
		newSection->SetEndTime(sectionTime);
		newSection->SetStartTime(sectionTime);
		newSection->SetEndTime(sectionTime);
		Sections.Add(newSection);
	}
}

//----------------------------------------------------------------------------

#if WITH_EDITORONLY_DATA
FText	UPopcornFXPlayTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("DisplayName", "Start/Stop/Toggle Emitter");
}
#endif

//----------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE
