//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXWaitForSceneActor.h"

#include "PopcornFXSceneComponent.h"
#include "PopcornFXSceneActor.h"
#include "PopcornFXEmitterComponent.h"

#include "EngineUtils.h"

#include "PopcornFXSDK.h"

//static
APopcornFXWaitForScene	*APopcornFXWaitForScene::GetIFP(UWorld *world, FName sceneName)
{
	for (TActorIterator<APopcornFXWaitForScene> it(world); it; ++it)
	{
		if (it->SceneName == sceneName)
			return *it;
	}
	return null;
}

//static
APopcornFXWaitForScene	*APopcornFXWaitForScene::GetOrCreate(UWorld *world, FName sceneName)
{
	APopcornFXWaitForScene	*actor = GetIFP(world, sceneName);
	if (actor != null)
		return actor;

	FActorSpawnParameters		params;
	params.ObjectFlags = RF_Public | RF_Transient; // | RF_Transactional;

	// ! Do not force the name !
	// Or UE will magicaly just return the last deleted Actor with the same name !
	//params.Name = FName(TEXT("PopcornFX_WaitForScene"));

	actor = world->SpawnActor<APopcornFXWaitForScene>(APopcornFXWaitForScene::StaticClass(), params);
	if (!HH_VERIFY(actor != null))
		return null;
	actor->SceneName = sceneName;
	return actor;
}

//static
void	APopcornFXWaitForScene::NotifySceneLoaded(APopcornFXSceneActor *sceneActor)
{
	APopcornFXWaitForScene	*actor = GetIFP(sceneActor->GetWorld(), sceneActor->GetSceneName());
	if (actor != null)
		return;
	actor->_NotifySceneLoaded(sceneActor);
}

//static
void	APopcornFXWaitForScene::NotifySceneLoaded(UPopcornFXSceneComponent *sceneComp)
{
	APopcornFXWaitForScene	*actor = GetIFP(sceneComp->GetWorld(), sceneComp->SceneName);
	if (actor == null)
		return;
	APopcornFXSceneActor	*sceneActor = Cast<APopcornFXSceneActor>(sceneComp->GetOuter());
	if (!HH_VERIFY(sceneActor != null))
		return;
	actor->_NotifySceneLoaded(sceneActor);
}

APopcornFXWaitForScene::APopcornFXWaitForScene(const class FObjectInitializer& ObjectInitializer)
:	Super(ObjectInitializer)
{
}

void	APopcornFXWaitForScene::_NotifySceneLoaded(APopcornFXSceneActor *scene)
{
	HH_ASSERT(scene->GetSceneName() == SceneName);

	OnSceneLoadedDelegates.Broadcast(scene);

	OnSceneLoadedDelegates.Clear();

	// self destroy !
	ConditionalBeginDestroy();
}
