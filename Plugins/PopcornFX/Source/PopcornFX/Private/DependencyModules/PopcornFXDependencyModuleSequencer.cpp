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

#include "PopcornFXDependencyModuleSequencer.h"

#include "ISequencerModule.h"
#include "Sequencer/TrackEditors/PopcornFXAttributeTrackEditor.h"
#include "Sequencer/TrackEditors/PopcornFXPlayTrackEditor.h"

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXDependencyModuleSettings"

//----------------------------------------------------------------------------

FPopcornFXDependencyModuleSequencer::FPopcornFXDependencyModuleSequencer()
{
}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModuleSequencer::Load()
{
	if (m_Loaded)
		return;

	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
		return;

	ISequencerModule	&sequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");

#if (ENGINE_MINOR_VERSION <= 15)
	AttributeTrackCreateEditorHandle = sequencerModule.RegisterTrackEditor_Handle(FOnCreateTrackEditor::CreateStatic(&FPopcornFXAttributeTrackEditor::CreateTrackEditor));
	PlayTrackCreateEditorHandle = sequencerModule.RegisterTrackEditor_Handle(FOnCreateTrackEditor::CreateStatic(&FPopcornFXPlayTrackEditor::CreateTrackEditor));
#else
	AttributeTrackCreateEditorHandle = sequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FPopcornFXAttributeTrackEditor::CreateTrackEditor));
	PlayTrackCreateEditorHandle = sequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FPopcornFXPlayTrackEditor::CreateTrackEditor));
#endif	// (ENGINE_MINOR_VERSION <= 15)

	m_Loaded = true;
}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModuleSequencer::Unload()
{
	if (!m_Loaded)
		return;
	m_Loaded = false;

	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
		return;

	ISequencerModule	&sequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");

#if (ENGINE_MINOR_VERSION <= 15)
	sequencerModule.UnRegisterTrackEditor_Handle(AttributeTrackCreateEditorHandle);
	sequencerModule.UnRegisterTrackEditor_Handle(PlayTrackCreateEditorHandle);
#else
	sequencerModule.UnRegisterTrackEditor(AttributeTrackCreateEditorHandle);
	sequencerModule.UnRegisterTrackEditor(PlayTrackCreateEditorHandle);
#endif	// (ENGINE_MINOR_VERSION <= 15)
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------

#endif // WITH_EDITOR
