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

#if WITH_EDITOR

#include "Runtime/Launch/Resources/Version.h"
#include "MovieSceneTrackEditor.h"

/**
 * Track editor for effect attributes.
 */
class FPopcornFXPlayTrackEditor : public FMovieSceneTrackEditor
{
public:
	FPopcornFXPlayTrackEditor(TSharedRef<ISequencer> InSequencer);
	virtual ~FPopcornFXPlayTrackEditor() { }

	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:
	// ISequencerTrackEditor interface
	virtual void							BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding, const UClass* ObjectClass) override;
	virtual TSharedRef<ISequencerSection>	MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	virtual bool							SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;

	void									AddStateKey(const FGuid objectGuid);
#if (ENGINE_MINOR_VERSION >= 17)
	FKeyPropertyResult						AddStateKeyInternal(float keyTime, UObject *object);
#else
	bool									AddStateKeyInternal(float keyTime, UObject *object);
#endif	// (ENGINE_MINOR_VERSION >= 17)
};


/**
*	Handles rendering of popcornfx play/stop sections.
*/
class	FPopcornFXPlaySection
	: public ISequencerSection
	, public TSharedFromThis<FPopcornFXPlaySection>
{
public:
	FPopcornFXPlaySection(UMovieSceneSection &inSection, TSharedRef<ISequencer> inOwningSequencer);
	~FPopcornFXPlaySection() { }

	// ISequencerSection interface
	virtual UMovieSceneSection			*GetSectionObject() override { return &m_Section; }
	virtual float						GetSectionHeight() const override { return 20.0f; }
	virtual void						GenerateSectionLayout(class ISectionLayoutBuilder& LayoutBuilder) const override;
	virtual int32						OnPaintSection(FSequencerSectionPainter& InPainter) const override;
	virtual const FSlateBrush			*GetKeyBrush(FKeyHandle KeyHandle) const override;
	virtual bool						SectionIsResizable() const override { return false; }

#if (ENGINE_MINOR_VERSION <= 16)
	virtual FText							GetDisplayName() const override;
	virtual FText							GetSectionTitle() const override { return FText::GetEmpty(); }
#endif	// (ENGINE_MINOR_VERSION <= 16)
private:
	UMovieSceneSection		&m_Section;
	TWeakPtr<ISequencer>	m_OwningSequencerPtr;

	const UEnum				*m_PlayStateKeyEnum;

	const FSlateBrush		*m_StartKeyBrush;
	const FSlateBrush		*m_StopKeyBrush;
};

#endif // WITH_EDITOR
