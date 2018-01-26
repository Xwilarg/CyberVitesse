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

#include "IPopcornFXEditorPlugin.h"

class	FPopcornFXDependencyModule;

class	FPopcornFXEditorPlugin : public IPopcornFXEditorPlugin
{
	TMap<FName, FPopcornFXDependencyModule*>	m_DeferredModuleActions;

public:
	FPopcornFXEditorPlugin();

	static float	s_GlobalScale;

	static inline FPopcornFXEditorPlugin& Get()
	{
		return static_cast<FPopcornFXEditorPlugin&>(FModuleManager::LoadModuleChecked<FPopcornFXEditorPlugin>("PopcornFXEditor"));
	}

	virtual void		StartupModule() override;
	virtual void		ShutdownModule() override;

private:
	void				OnModulesChangedCallback(FName moduleThatChanged, EModuleChangeReason reason);
	void				UnloadModuleActions(TMap<FName, FPopcornFXDependencyModule*> &moduleActions);

	FDelegateHandle		m_OnModulesChangedHandle;

	TSharedPtr<class IComponentAssetBroker>		m_PopcornFXAssetBroker;
};
