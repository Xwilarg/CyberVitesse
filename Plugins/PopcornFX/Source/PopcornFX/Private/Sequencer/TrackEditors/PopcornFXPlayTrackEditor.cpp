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

#include "PopcornFXPlayTrackEditor.h"

#include "PopcornFXPlugin.h"

#include "SequencerUtilities.h"
#include "ISectionLayoutBuilder.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "PopcornFXEmitter.h"
#include "PopcornFXEmitterComponent.h"

#include "Sequencer/Sections/PopcornFXPlayKeys.h"
#include "Sequencer/Tracks/PopcornFXPlayTrack.h"
#include "EditorStyleSet.h"

#include "CommonMovieSceneTools.h"

#include "Sections/MovieSceneParticleSection.h"
#include "EnumKeyArea.h"
#include "SequencerSectionPainter.h"

#define LOCTEXT_NAMESPACE "PopcornFXPlayTrackEditor"

//----------------------------------------------------------------------------
//
//	Play Track Section Interface (handles rendering of play sections)
//
//----------------------------------------------------------------------------

extern UPopcornFXEmitterComponent	*TryGetEmitterComponent(UObject *object);

//----------------------------------------------------------------------------

FPopcornFXPlaySection::FPopcornFXPlaySection(UMovieSceneSection &inSection, TSharedRef<ISequencer> inOwningSequencer)
:	m_Section(inSection)
,	m_OwningSequencerPtr(inOwningSequencer)
{
	m_PlayStateKeyEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPopcornFXPlayStateKey"));
	checkf(m_PlayStateKeyEnum != null, TEXT("FPopcornFXPlaySection could not find the EPopcornFXPlayStateKey UEnum by name."))

	m_StartKeyBrush = FEditorStyle::GetBrush("PopcornFXSequencer.StartEmitter");
	m_StopKeyBrush = FEditorStyle::GetBrush("PopcornFXSequencer.StopEmitter");
}

//----------------------------------------------------------------------------

int32	FPopcornFXPlaySection::OnPaintSection(FSequencerSectionPainter &inPainter) const
{
	return inPainter.LayerId + 1;
}

//----------------------------------------------------------------------------

void	FPopcornFXPlaySection::GenerateSectionLayout(class ISectionLayoutBuilder &layoutBuilder) const
{
	UMovieSceneParticleSection	*playSection = Cast<UMovieSceneParticleSection>(&m_Section);

	layoutBuilder.SetSectionAsKeyArea(MakeShareable(new FPopcornFXPlayStateKeyArea(playSection->GetParticleCurve(), playSection, m_PlayStateKeyEnum)));
}

//----------------------------------------------------------------------------

const FSlateBrush	*FPopcornFXPlaySection::GetKeyBrush(FKeyHandle keyHandle) const
{
	UMovieSceneParticleSection	*playSection = Cast<UMovieSceneParticleSection>(&m_Section);

	if (playSection == null)
		return null;
	FIntegralKey	key = playSection->GetParticleCurve().GetKey(keyHandle);
	switch (key.Value)
 	{
		case	EPopcornFXPlayStateKey::Start:
			return m_StartKeyBrush;
		case	EPopcornFXPlayStateKey::Stop:
			return m_StopKeyBrush;
		default:
			return null;
	}
}

#if (ENGINE_MINOR_VERSION <= 16)

FText	FPopcornFXPlaySection::GetDisplayName() const
{
	return LOCTEXT("TrackName", "Start/Stop/Toggle Emitter");
}

#endif // (ENGINE_MINOR_VERSION <= 16)

//----------------------------------------------------------------------------
//
//	Play Track Editor
//
//----------------------------------------------------------------------------

FPopcornFXPlayTrackEditor::FPopcornFXPlayTrackEditor(TSharedRef<ISequencer> InSequencer)
:	FMovieSceneTrackEditor(InSequencer)
{
}

//----------------------------------------------------------------------------

TSharedRef<ISequencerTrackEditor>	FPopcornFXPlayTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> owningSequencer)
{
	return MakeShareable(new FPopcornFXPlayTrackEditor(owningSequencer));
}

//----------------------------------------------------------------------------

TSharedRef<ISequencerSection>	FPopcornFXPlayTrackEditor::MakeSectionInterface(UMovieSceneSection &sectionObject, UMovieSceneTrack &track, FGuid objectBinding)
{
	check(SupportsType(sectionObject.GetOuter()->GetClass()));

	const TSharedPtr<ISequencer>	owningSequencer = GetSequencer();
	return MakeShareable(new FPopcornFXPlaySection(sectionObject, owningSequencer.ToSharedRef()));
}

//----------------------------------------------------------------------------

void	FPopcornFXPlayTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder &menuBuilder, const FGuid &objectBinding, const UClass *objectClass)
{
	if (objectClass->IsChildOf(APopcornFXEmitter::StaticClass()) ||
		objectClass->IsChildOf(UPopcornFXEmitterComponent::StaticClass()))
	{
		const TSharedPtr<ISequencer>	parentSequencer = GetSequencer();

		menuBuilder.AddMenuEntry(
			LOCTEXT("AddPlayTrack", "Emitter Play Track"),
			LOCTEXT("AddPlayTrackTooltip", "Adds a track for controlling emitter Start/Stop."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &FPopcornFXPlayTrackEditor::AddStateKey, objectBinding))
		);
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXPlayTrackEditor::AddStateKey(const FGuid objectGuid)
{
	TSharedPtr<ISequencer>	sequencerPtr = GetSequencer();
	UObject					*object = sequencerPtr.IsValid() ? sequencerPtr->FindSpawnedObjectOrTemplate(objectGuid) : null;

	if (object == null)
		return;

	AnimatablePropertyChanged(FOnKeyProperty::CreateRaw(this, &FPopcornFXPlayTrackEditor::AddStateKeyInternal, object));
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 17)

FKeyPropertyResult	FPopcornFXPlayTrackEditor::AddStateKeyInternal(float keyTime, UObject *object)
{
	FKeyPropertyResult			keyPropertyResult;
	FFindOrCreateHandleResult	handleResult = FindOrCreateHandleToObject(object);
	FGuid						objectHandle = handleResult.Handle;

	keyPropertyResult.bHandleCreated |= handleResult.bWasCreated;

	if (objectHandle.IsValid())
	{
		FFindOrCreateTrackResult	trackResult = FindOrCreateTrackForObject(objectHandle, UPopcornFXPlayTrack::StaticClass());
		UMovieSceneTrack			*track = trackResult.Track;

		keyPropertyResult.bTrackCreated |= trackResult.bWasCreated;
		if (keyPropertyResult.bTrackCreated && ensure(track != null))
		{
			UPopcornFXPlayTrack	*playTrack = Cast<UPopcornFXPlayTrack>(track);
			playTrack->AddNewSection(keyTime);
			playTrack->SetDisplayName(LOCTEXT("TrackName", "Start/Stop/Toggle Emitter"));
			keyPropertyResult.bTrackModified = true;
		}
	}
	return keyPropertyResult;
}

#else

bool	FPopcornFXPlayTrackEditor::AddStateKeyInternal(float keyTime, UObject *object)
{
	FFindOrCreateHandleResult	handleResult = FindOrCreateHandleToObject(object);
	FGuid						objectHandle = handleResult.Handle;
	
	bool	trackCreated = false;
	bool	trackModified = false;
	bool	handleCreated = handleResult.bWasCreated;

	if (objectHandle.IsValid())
	{
		FFindOrCreateTrackResult	trackResult = FindOrCreateTrackForObject(objectHandle, UPopcornFXPlayTrack::StaticClass());
		UMovieSceneTrack			*track = trackResult.Track;

		trackCreated = trackResult.bWasCreated;
		if (trackCreated && ensure(track != null))
		{
			UPopcornFXPlayTrack	*playTrack = Cast<UPopcornFXPlayTrack>(track);
			playTrack->AddNewSection(keyTime);
			playTrack->SetDisplayName(LOCTEXT("TrackName", "Start/Stop/Toggle Emitter"));
			trackModified = true;
		}
	}
	return handleCreated || trackCreated || trackModified;
}

#endif	// (ENGINE_MINOR_VERSION >= 17)

//----------------------------------------------------------------------------

bool	FPopcornFXPlayTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> type) const
{
	return type == UPopcornFXPlayTrack::StaticClass();
}

//----------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE

#endif // WITH_EDITOR
