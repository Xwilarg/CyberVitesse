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

#include "PopcornFXPlugin.h"
#include "MovieSceneTrackEditor.h"

/**
 * Track editor for effect attributes.
 */
class FPopcornFXAttributeTrackEditor : public FMovieSceneTrackEditor
{
public:
	/** Constructor. */
	FPopcornFXAttributeTrackEditor(TSharedRef<ISequencer> InSequencer);
	virtual ~FPopcornFXAttributeTrackEditor() { }

	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

	// ISequencerTrackEditor interface
	virtual TSharedPtr<SWidget>				BuildOutlinerEditWidget( const FGuid &objectBinding, UMovieSceneTrack *track, const FBuildEditWidgetParams &params) override;
	virtual TSharedRef<ISequencerSection>	MakeSectionInterface(UMovieSceneSection &sectionObject, UMovieSceneTrack &track, FGuid objectBinding) override;
	virtual bool							SupportsType(TSubclassOf<UMovieSceneTrack> type) const override;

	void									BuildObjectBindingTrackMenu(FMenuBuilder &menuBuilder, const FGuid &objectBinding, const UClass *objectClass) override;
private:
	TSharedRef<SWidget>		OnGetAddParameterMenuContent(FGuid objectBinding, class UPopcornFXAttributeTrack *attributeTrack);

	bool	CanAddAttributeTrack(FGuid objectBinding);
	void	AddAttributeTrack(FGuid objectBinding);

	void	AddScalarAttribute(FGuid objectBinding,
		UPopcornFXAttributeTrack *attributeTrack,
		FName attrName,
		hh_u32 attrIndex);

	void	AddVectorAttribute(FGuid objectBinding,
		UPopcornFXAttributeTrack *attributeTrack,
		FName attrName,
		hh_u32 attrIndex);

	void	AddColorAttribute(FGuid objectBinding,
		UPopcornFXAttributeTrack *attributeTrack,
		FName attrName,
		hh_u32 attrIndex);
};

#endif // WITH_EDITOR
