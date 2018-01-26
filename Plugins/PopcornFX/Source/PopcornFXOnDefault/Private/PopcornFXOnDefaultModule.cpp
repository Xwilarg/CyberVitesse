//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXOnDefaultModule.h"

#include "IPluginManager.h"

#include "Runtime/Launch/Resources/Version.h"

/**
	Since 4.13, PopcornFXEditor is not loaded automatically in Standalone Game
	But Standalone Game needs to PopcornFXEditor stuff (Kismet nodes, SetAttribute, GetAttributes ...)

	This module is meant to workaround this, and manually load the PopcornFXEditor module if in Editor mode,
	during the "Default" LoadingPhase.
	PopcornFX module cannot do that because loaded too early ("PostConfigInit" LoadingPhase, because of vertex factories)
	And I could not find a sutable delegate where PopcornFX module could hook himself to load PopcornFXEditor.
*/

IMPLEMENT_MODULE(FPopcornFXOnDefaultModule, PopcornFXOnDefault)

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXOnDefault, Log, All);

void	FPopcornFXOnDefaultModule::StartupModule()
{
#if WITH_EDITOR
	FModuleManager		&ModuleManager = FModuleManager::Get();
	{
		const FName			ModuleName = TEXT("PopcornFXEditor");
		if (!ModuleManager.IsModuleLoaded(ModuleName))
		{
#if (ENGINE_MINOR_VERSION >= 17)
			if (ModuleManager.LoadModule(ModuleName) == nullptr)	// ptr not valid
#else
			if (!ModuleManager.LoadModule(ModuleName).IsValid())	// shared ptr
#endif
			{
				UE_LOG(LogPopcornFXOnDefault, Error, TEXT("Failed to load PopcornFXEditor module, might crash later"));
			}
		}
	}
#endif // WITH_EDITOR
}

void	FPopcornFXOnDefaultModule::ShutdownModule()
{
#if WITH_EDITOR
#endif // WITH_EDITOR
}
