//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXEditorPlugin.h"

#include "Assets/PopcornFXEffect.h"
#include "DependencyModules/PopcornFXDependencyModule.h"
#include "DependencyModules/PopcornFXDependencyModuleSettings.h"
#include "DependencyModules/PopcornFXDependencyModuleAssetTools.h"
#include "DependencyModules/PopcornFXDependencyModulePropertyEditor.h"
#include "DependencyModules/PopcornFXDependencyModuleLevelEditor.h"
#include "DependencyModules/PopcornFXDependencyModuleSequencer.h"

#include "PopcornFXFileThumbnailRenderer.h"
#include "Editor/PopcornFXAssetBroker.h"

#include "ThumbnailRendering/ThumbnailManager.h"

//----------------------------------------------------------------------------

IMPLEMENT_MODULE(FPopcornFXEditorPlugin, PopcornFXEditorPlugin)

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXEditorPlugin, Log, All);

//----------------------------------------------------------------------------

FPopcornFXEditorPlugin::FPopcornFXEditorPlugin()
{
}

//----------------------------------------------------------------------------

void	FPopcornFXEditorPlugin::StartupModule()
{
	if (m_DeferredModuleActions.Num() <= 0)
	{
		// Deferred module actions is a list of module actions that depends on a module that ain't loaded yet
		m_DeferredModuleActions.Add("PropertyEditor", new FPopcornFXDependencyModulePropertyEditor());
		m_DeferredModuleActions.Add("Settings", new FPopcornFXDependencyModuleSettings());
		m_DeferredModuleActions.Add("AssetTools", new FPopcornFXDependencyModuleAssetTools());
		m_DeferredModuleActions.Add("LevelEditor", new FPopcornFXDependencyModuleLevelEditor());
		m_DeferredModuleActions.Add("Sequencer", new FPopcornFXDependencyModuleSequencer());

		for (auto it = m_DeferredModuleActions.CreateIterator(); it; ++it)
		{
			//	FModuleManager::Get().LoadModule(it.Key());
			if (FModuleManager::Get().IsModuleLoaded(it.Key()))
			{
				it.Value()->Load();
			}
		}

		m_OnModulesChangedHandle = FModuleManager::Get().OnModulesChanged().AddRaw(this, &FPopcornFXEditorPlugin::OnModulesChangedCallback);

		UThumbnailManager::Get().RegisterCustomRenderer(UPopcornFXEffect::StaticClass(), UPopcornFXFileThumbnailRenderer::StaticClass());

		m_PopcornFXAssetBroker = MakeShareable(new FPopcornFXEmitterComponentAssetBroker);
		FComponentAssetBrokerage::RegisterBroker(m_PopcornFXAssetBroker, FPopcornFXEmitterComponentAssetBroker::ComponentStaticClass(), true, true);
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXEditorPlugin::ShutdownModule()
{
	//UThumbnailManager::Get().UnregisterCustomRenderer(UPopcornFXEffect::StaticClass());
	FModuleManager::Get().OnModulesChanged().Remove(m_OnModulesChangedHandle);
	UnloadModuleActions(m_DeferredModuleActions);

	if (UObjectInitialized())
	{
		FComponentAssetBrokerage::UnregisterBroker(m_PopcornFXAssetBroker);
		m_PopcornFXAssetBroker = null;
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXEditorPlugin::UnloadModuleActions(TMap<FName, FPopcornFXDependencyModule*> &moduleActions)
{
	if (moduleActions.Num() > 0)
	{
		for (auto it = moduleActions.CreateIterator(); it; ++it)
		{
			FName						key = it.Key();
			FPopcornFXDependencyModule	*value = it.Value();
			value->Unload();
			HH_ASSERT(!value->Loaded());
			delete value;
		}
		moduleActions.Empty();
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXEditorPlugin::OnModulesChangedCallback(FName moduleThatChanged, EModuleChangeReason reason)
{
	if (m_DeferredModuleActions.Contains(moduleThatChanged))
	{
		auto	*moduleAction = m_DeferredModuleActions[moduleThatChanged];
		if (reason == EModuleChangeReason::ModuleLoaded)
		{
			if (!moduleAction->Loaded())
				moduleAction->Load();
		}
		else if (reason == EModuleChangeReason::ModuleUnloaded)
		{
			moduleAction->Unload();
		}
	}
}

//----------------------------------------------------------------------------
