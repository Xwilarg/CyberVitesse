//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#if WITH_EDITOR

#include "PopcornFXAttributeTrackEditor.h"

#include "PopcornFXPlugin.h"

#include "SequencerUtilities.h"
#include "MovieSceneParameterSection.h"

#include "PopcornFXEmitter.h"
#include "PopcornFXEmitterComponent.h"
#include "Attributes/PopcornFXAttributeList.h"
#include "PopcornFXAttributeFunctions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "Sequencer/Sections/PopcornFXAttributeSection.h"
#include "Sequencer/Tracks/PopcornFXAttributeTrack.h"

#define LOCTEXT_NAMESPACE "PopcornFXAttributeTrackEditor"

static FName	trackName("PopcornFXAttribute");

//----------------------------------------------------------------------------

FPopcornFXAttributeTrackEditor::FPopcornFXAttributeTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

//----------------------------------------------------------------------------

TSharedRef<ISequencerTrackEditor>	FPopcornFXAttributeTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> owningSequencer)
{
	return MakeShareable(new FPopcornFXAttributeTrackEditor(owningSequencer));
}

//----------------------------------------------------------------------------

TSharedRef<ISequencerSection>	FPopcornFXAttributeTrackEditor::MakeSectionInterface(UMovieSceneSection &sectionObject, UMovieSceneTrack &track, FGuid objectBinding)
{
	UMovieSceneParameterSection	*parameterSection = Cast<UMovieSceneParameterSection>(&sectionObject);
	checkf(parameterSection != nullptr, TEXT("Unsupported section type."));

	return MakeShareable(new FPopcornFXAttributeSection(*parameterSection, FText::FromName(parameterSection->GetFName())));
}

//----------------------------------------------------------------------------

TSharedPtr<SWidget>	FPopcornFXAttributeTrackEditor::BuildOutlinerEditWidget(const FGuid &objectBinding, UMovieSceneTrack *track, const FBuildEditWidgetParams &params)
{
	UPopcornFXAttributeTrack	*attributeTrack = Cast<UPopcornFXAttributeTrack>(track);
	
	// Create a container edit box
	return FSequencerUtilities::MakeAddButton(LOCTEXT("AttributeText", "Attribute"),
		FOnGetContent::CreateSP(this, &FPopcornFXAttributeTrackEditor::OnGetAddParameterMenuContent, objectBinding, attributeTrack),
		params.NodeIsHovered);
}

//----------------------------------------------------------------------------

void	FPopcornFXAttributeTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder &menuBuilder, const FGuid &objectBinding, const UClass *objectClass)
{
	if (objectClass->IsChildOf(APopcornFXEmitter::StaticClass()))
	{
		const TSharedPtr<ISequencer>	parentSequencer = GetSequencer();

		menuBuilder.AddMenuEntry(
			LOCTEXT( "AddAttributeTrack", "Effect Attributes Track" ),
			LOCTEXT( "AddAttributeTrackTooltip", "Adds a track for controlling attribute values." ),
			FSlateIcon(),
			FUIAction
			(
				FExecuteAction::CreateSP(this, &FPopcornFXAttributeTrackEditor::AddAttributeTrack, objectBinding),
				FCanExecuteAction::CreateSP(this, &FPopcornFXAttributeTrackEditor::CanAddAttributeTrack, objectBinding)
			));
	}
}

//----------------------------------------------------------------------------

bool	FPopcornFXAttributeTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> type) const
{
	return type == UPopcornFXAttributeTrack::StaticClass();
}

//----------------------------------------------------------------------------

TSharedRef<SWidget>	FPopcornFXAttributeTrackEditor::OnGetAddParameterMenuContent(FGuid objectBinding, UPopcornFXAttributeTrack *attributeTrack)
{
	FMenuBuilder	addParameterMenuBuilder(true, null);

	TSharedPtr<ISequencer>	sequencerPtr = GetSequencer();
	APopcornFXEmitter		*emitter = sequencerPtr.IsValid() ? Cast<APopcornFXEmitter>(sequencerPtr->FindSpawnedObjectOrTemplate(objectBinding)) : null;

	if (emitter == null ||
		emitter->PopcornFXEmitterComponent == null ||
		emitter->GetEffect() == null)
		return addParameterMenuBuilder.MakeWidget();
	UPopcornFXAttributeList	*attributeList = emitter->PopcornFXEmitterComponent->GetAttributeList();
	HH_ASSERT(attributeList != null);

	if (!attributeList->Valid() ||
		!attributeList->IsUpToDate(emitter->GetEffect()))
		return addParameterMenuBuilder.MakeWidget();

	const hh_u32	attrCount = attributeList->AttributeCount();
	for (hh_u32 iAttr = 0; iAttr < attrCount; ++iAttr)
	{
		const FPopcornFXAttributeDesc	*desc = attributeList->GetAttributeDesc(iAttr);

		if (!HH_VERIFY(desc != null))
			continue;
		const FName		attrName = desc->m_AttributeFName;
		const hh_u32	attrType = desc->m_AttributeType;
		FUIAction		attrAction;
		bool			supportedType = true;
		switch (attrType)
		{
			case	PopcornFX::EBaseTypeID::BaseType_Float:
				attrAction.ExecuteAction = FExecuteAction::CreateSP(this, &FPopcornFXAttributeTrackEditor::AddScalarAttribute, objectBinding, attributeTrack, attrName, iAttr);
				break;
			case	PopcornFX::EBaseTypeID::BaseType_Float3:
				attrAction.ExecuteAction = FExecuteAction::CreateSP(this, &FPopcornFXAttributeTrackEditor::AddVectorAttribute, objectBinding, attributeTrack, attrName, iAttr);
				break;
			case	PopcornFX::EBaseTypeID::BaseType_Float4:
				attrAction.ExecuteAction = FExecuteAction::CreateSP(this, &FPopcornFXAttributeTrackEditor::AddColorAttribute, objectBinding, attributeTrack, attrName, iAttr);
				break;
			default: // Every other types are not supported right now
				supportedType = false;
				break;
		}
		if (supportedType)
			addParameterMenuBuilder.AddMenuEntry(FText::FromName(attrName), FText(), FSlateIcon(), attrAction);
	}
	return addParameterMenuBuilder.MakeWidget();
}

//----------------------------------------------------------------------------

bool	FPopcornFXAttributeTrackEditor::CanAddAttributeTrack(FGuid objectBinding)
{
	return GetSequencer()->GetFocusedMovieSceneSequence()->GetMovieScene()->FindTrack(UPopcornFXAttributeTrack::StaticClass(), objectBinding, trackName) == null;
}

//----------------------------------------------------------------------------

void	FPopcornFXAttributeTrackEditor::AddAttributeTrack(FGuid objectBinding)
{
	FindOrCreateTrackForObject(objectBinding, UPopcornFXAttributeTrack::StaticClass(), trackName, true);
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

//----------------------------------------------------------------------------

void	FPopcornFXAttributeTrackEditor::AddScalarAttribute(FGuid objectBinding, UPopcornFXAttributeTrack *attributeTrack, FName attrName, hh_u32 attrIndex)
{
	const FScopedTransaction	tr(LOCTEXT("AddFloatAttribute", "Add float attribute"));

	const float	keyTime = GetTimeForKey();
	for (TWeakObjectPtr<> object : GetSequencer()->FindObjectsInCurrentSequence(objectBinding))
	{
		APopcornFXEmitter	*emitter = Cast<APopcornFXEmitter>(object.Get());
		if (emitter == null ||
			emitter->PopcornFXEmitterComponent == null ||
			emitter->GetEffect() == null)
			continue;

		float		defaultValue;
		if (!UPopcornFXAttributeFunctions::GetAttributeAsFloat(emitter->PopcornFXEmitterComponent, attrIndex, defaultValue, false))
			continue;

		attributeTrack->Modify();
		attributeTrack->AddScalarAttributeKey(attrName, keyTime, defaultValue);
	}
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

//----------------------------------------------------------------------------

void	FPopcornFXAttributeTrackEditor::AddVectorAttribute(FGuid objectBinding, UPopcornFXAttributeTrack *attributeTrack, FName attrName, hh_u32 attrIndex)
{
	const FScopedTransaction	tr(LOCTEXT("AddVectorAttribute", "Add vector attribute"));

	const float	keyTime = GetTimeForKey();
	for (TWeakObjectPtr<> object : GetSequencer()->FindObjectsInCurrentSequence(objectBinding))
	{
		APopcornFXEmitter	*emitter = Cast<APopcornFXEmitter>(object.Get());
		if (emitter == null ||
			emitter->PopcornFXEmitterComponent == null ||
			emitter->GetEffect() == null)
			continue;

		FVector		defaultValue;
		if (!UPopcornFXAttributeFunctions::GetAttributeAsVector(emitter->PopcornFXEmitterComponent, attrIndex, defaultValue, false))
			continue;

		attributeTrack->Modify();
		attributeTrack->AddVectorAttributeKey(attrName, keyTime, defaultValue);
	}
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

//----------------------------------------------------------------------------

void	FPopcornFXAttributeTrackEditor::AddColorAttribute(FGuid objectBinding, UPopcornFXAttributeTrack *attributeTrack, FName attrName, hh_u32 attrIndex)
{
	const FScopedTransaction	tr(LOCTEXT("AddVectorAttribute", "Add vector attribute"));

	const float	keyTime = GetTimeForKey();
	for (TWeakObjectPtr<> object : GetSequencer()->FindObjectsInCurrentSequence(objectBinding))
	{
		APopcornFXEmitter	*emitter = Cast<APopcornFXEmitter>(object.Get());
		if (emitter == null ||
			emitter->PopcornFXEmitterComponent == null ||
			emitter->GetEffect() == null)
			continue;

		FLinearColor		defaultValue;
		if (!UPopcornFXAttributeFunctions::GetAttributeAsLinearColor(emitter->PopcornFXEmitterComponent, attrIndex, defaultValue, false))
			continue;

		attributeTrack->Modify();
		attributeTrack->AddColorAttributeKey(attrName, keyTime, defaultValue);
	}
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

//----------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE

#endif // WITH_EDITOR
