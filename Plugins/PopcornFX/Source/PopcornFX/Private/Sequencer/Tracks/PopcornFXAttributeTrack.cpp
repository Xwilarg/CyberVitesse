//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "Sequencer/Tracks/PopcornFXAttributeTrack.h"
#include "Sequencer/Templates/PopcornFXAttributeSectionTemplate.h"

#include "PopcornFXPlugin.h"

#include "MovieSceneCommonHelpers.h"

#include "Curves/RichCurve.h"


//#include "IMovieScenePlayer.h"

#define LOCTEXT_NAMESPACE "PopcornFXAttributeTrack"

UPopcornFXAttributeTrack::UPopcornFXAttributeTrack(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	TrackTint = FColor(204, 70, 221, 65);
#endif
}

FMovieSceneEvalTemplatePtr	UPopcornFXAttributeTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FPopcornFXAttributeSectionTemplate(*CastChecked<UMovieSceneParameterSection>(&InSection), *this);
}

UMovieSceneSection	*UPopcornFXAttributeTrack::CreateNewSection()
{
	return NewObject<UMovieSceneParameterSection>(this, UMovieSceneParameterSection::StaticClass(), NAME_None, RF_Transactional);
}

TRange<float>	UPopcornFXAttributeTrack::GetSectionBoundaries() const
{
	TArray<TRange<float>>	bounds;

	for (int32 SectionIndex = 0; SectionIndex < Sections.Num(); ++SectionIndex)
		bounds.Add(Sections[SectionIndex]->GetRange());
	return TRange<float>::Hull(bounds);
}

#if WITH_EDITORONLY_DATA
FText	UPopcornFXAttributeTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("DisplayName", "Effect Attributes");
}
#endif

void	UPopcornFXAttributeTrack::AddScalarAttributeKey(FName attrName, float position, float value)
{
	UMovieSceneParameterSection	*nearestSection = Cast<UMovieSceneParameterSection>(MovieSceneHelpers::FindNearestSectionAtTime(Sections, position));

	if (nearestSection == null)
	{
		nearestSection = Cast<UMovieSceneParameterSection>(CreateNewSection());
		nearestSection->SetStartTime(position);
		nearestSection->SetEndTime(position);
		Sections.Add(nearestSection);
	}

	// THIS SHOULD BE EXPOSED.. UMovieSceneParameterSection::AddScalarParameterKey()
	{
		FRichCurve* existingCurve = null;
		auto		scalarParamsAndCurves = nearestSection->GetScalarParameterNamesAndCurves();
		for (FScalarParameterNameAndCurve& scalarParameterNameAndCurve : *scalarParamsAndCurves)
		{
			if (scalarParameterNameAndCurve.ParameterName == attrName)
			{
				existingCurve = &scalarParameterNameAndCurve.ParameterCurve;
				break;
			}
		}
		if (existingCurve == null)
		{
			FScalarParameterNameAndCurve	paramNameAndCurve;

			paramNameAndCurve.ParameterName = attrName;
			paramNameAndCurve.Index = scalarParamsAndCurves->Num() + nearestSection->GetVectorParameterNamesAndCurves()->Num() - 1;

			const hh_u32	newIndex = scalarParamsAndCurves->Add(paramNameAndCurve);
			existingCurve = &(*scalarParamsAndCurves)[newIndex].ParameterCurve;
		}
		existingCurve->AddKey(position, value);
	}
	//nearestSection->AddScalarParameterKey(attrName, time, value);
}

void	UPopcornFXAttributeTrack::AddVectorAttributeKey(FName attrName, float position, FVector value)
{
	UMovieSceneParameterSection	*nearestSection = Cast<UMovieSceneParameterSection>(MovieSceneHelpers::FindNearestSectionAtTime(Sections, position));

	if (nearestSection == null)
	{
		nearestSection = Cast<UMovieSceneParameterSection>(CreateNewSection());
		nearestSection->SetStartTime(position);
		nearestSection->SetEndTime(position);
		Sections.Add(nearestSection);
	}

	// THIS SHOULD BE EXPOSED.. UMovieSceneParameterSection::AddVectorParameterKey()
	{
		FVectorParameterNameAndCurves*	existingCurves = null;
		auto							vectorParamsAndCurves = nearestSection->GetVectorParameterNamesAndCurves();
		for (FVectorParameterNameAndCurves& vectorParameterNameAndCurve : *vectorParamsAndCurves)
		{
			if (vectorParameterNameAndCurve.ParameterName == attrName)
			{
				existingCurves = &vectorParameterNameAndCurve;
				break;
			}
		}
		if (existingCurves == null)
		{
			FVectorParameterNameAndCurves	paramNameAndCurves;

			paramNameAndCurves.ParameterName = attrName;
			paramNameAndCurves.Index = vectorParamsAndCurves->Num() + vectorParamsAndCurves->Num() - 1;

			const hh_u32	newIndex = vectorParamsAndCurves->Add(paramNameAndCurves);
			existingCurves = &(*vectorParamsAndCurves)[newIndex];
		}
		existingCurves->XCurve.AddKey(position, value.X);
		existingCurves->YCurve.AddKey(position, value.Y);
		existingCurves->ZCurve.AddKey(position, value.Z);
	}
	//nearestSection->AddVectorParameterKey( ParameterName, position, Value );
}

void	UPopcornFXAttributeTrack::AddColorAttributeKey(FName attrName, float position, FLinearColor value)
{
	UMovieSceneParameterSection	*nearestSection = Cast<UMovieSceneParameterSection>(MovieSceneHelpers::FindNearestSectionAtTime(Sections, position));

	if (nearestSection == null)
	{
		nearestSection = Cast<UMovieSceneParameterSection>(CreateNewSection());
		nearestSection->SetStartTime(position);
		nearestSection->SetEndTime(position);
		Sections.Add(nearestSection);
	}

	// THIS SHOULD BE EXPOSED.. UMovieSceneParameterSection::AddVectorParameterKey()
	{
		FColorParameterNameAndCurves*	existingCurves = null;
		auto							colorParamsAndCurves = nearestSection->GetColorParameterNamesAndCurves();
		for (FColorParameterNameAndCurves& colorParameterNameAndCurve : *colorParamsAndCurves)
		{
			if (colorParameterNameAndCurve.ParameterName == attrName)
			{
				existingCurves = &colorParameterNameAndCurve;
				break;
			}
		}
		if (existingCurves == null)
		{
			FColorParameterNameAndCurves	paramNameAndCurves;

			paramNameAndCurves.ParameterName = attrName;
			paramNameAndCurves.Index = colorParamsAndCurves->Num() + colorParamsAndCurves->Num() - 1;

			const hh_u32	newIndex = colorParamsAndCurves->Add(paramNameAndCurves);
			existingCurves = &(*colorParamsAndCurves)[newIndex];
		}
		existingCurves->RedCurve.AddKey(position, value.R);
		existingCurves->GreenCurve.AddKey(position, value.G);
		existingCurves->BlueCurve.AddKey(position, value.B);
		existingCurves->AlphaCurve.AddKey(position, value.A);
	}
	//nearestSection->AddColorParameterKey(attrName, position, value);
}

#undef LOCTEXT_NAMESPACE
