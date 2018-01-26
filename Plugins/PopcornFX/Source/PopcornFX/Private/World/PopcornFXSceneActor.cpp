//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXSceneActor.h"

#include "PopcornFXSceneComponent.h"
#include "PopcornFXSDK.h"

APopcornFXSceneActor::APopcornFXSceneActor(const class FObjectInitializer& PCIP)
:	Super(PCIP)
{
	PopcornFXSceneComponent = CreateDefaultSubobject<UPopcornFXSceneComponent>(TEXT("PopcornFXSceneComponent0"));
	RootComponent = PopcornFXSceneComponent;
	SetFlags(RF_Transactional); // Allow this actor's deletion
}

CParticleScene		*APopcornFXSceneActor::ParticleScene()
{
	if (HH_VERIFY(PopcornFXSceneComponent != null))
	{
		return PopcornFXSceneComponent->ParticleScene();
	}
	return null;
}

FName	APopcornFXSceneActor::GetSceneName() const
{
	return PopcornFXSceneComponent != null ? PopcornFXSceneComponent->SceneName : FName();
}
