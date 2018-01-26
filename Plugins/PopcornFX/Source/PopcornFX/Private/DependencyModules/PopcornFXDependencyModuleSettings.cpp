//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXDependencyModuleSettings.h"

#include "PopcornFXSDK.h"
#include "PopcornFXPlugin.h"
#include "PopcornFXSettingsEditor.h"
#include "PopcornFXSettings.h"

#include "ISettingsModule.h"
#include "ISettingsSection.h"

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXDependencyModuleSettings"

//----------------------------------------------------------------------------

FPopcornFXDependencyModuleSettings::FPopcornFXDependencyModuleSettings()
{

}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModuleSettings::Load()
{
	if (m_Loaded)
		return;

	if (!FModuleManager::Get().IsModuleLoaded("Settings"))
		return;
	ISettingsModule		*settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (!HH_VERIFY(settingsModule != null))
		return;

	TSharedPtr<ISettingsSection>	runtimeSection = settingsModule->RegisterSettings(
		"Project", "Plugins", "PopcornFXSettings",
		LOCTEXT("PopcornFXSettingsName", "PopcornFX"),
		LOCTEXT("PopcornFXSettingsDescription", "Configure the PopcornFX plugin settings."),
		GetMutableDefault<UPopcornFXSettings>());
	if (HH_VERIFY(runtimeSection.IsValid()))
	{
		// Does not work for DefaultConfig
		// Done in UPopcornFXSettings::PostEditChangeProperty
		//runtimeSection->OnModified().BindRaw(m_Plugin, &FPopcornFXPlugin::HandleSettingsModified);
	}

#if WITH_EDITOR
	TSharedPtr<ISettingsSection>	editorSection = settingsModule->RegisterSettings(
		"Project", "Plugins", "PopcornFXSettingsEditor",
		LOCTEXT("PopcornFXSettingsEditorName", "PopcornFX Editor"),
		LOCTEXT("PopcornFXSettingsEditorDescription", "Configure the PopcornFX plugin Editor settings."),
		GetMutableDefault<UPopcornFXSettingsEditor>());
	if (HH_VERIFY(editorSection.IsValid()))
	{
		editorSection->OnModified().BindRaw(m_Plugin, &FPopcornFXPlugin::HandleSettingsModified);
	}
#endif // WITH_EDITOR

	//m_Plugin->HandleSettingsModified(); // will be lazy load
	m_Loaded = true;
}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModuleSettings::Unload()
{
	if (!m_Loaded)
		return;
	m_Loaded = false;

	if (!FModuleManager::Get().IsModuleLoaded("Settings"))
		return;

	ISettingsModule		*settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (settingsModule != null)
	{
#if WITH_EDITOR
		settingsModule->UnregisterSettings("Project", "Plugins", "PopcornFXSettingsEditor");
#endif // WITH_EDITOR
		settingsModule->UnregisterSettings("Project", "Plugins", "PopcornFXSettings");
	}
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
