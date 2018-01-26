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

#include "PopcornFXPublic.h"

#include "GameFramework/Actor.h"

#include "PopcornFXSceneActor.generated.h"

class CParticleScene;

/** Actor container for a PopcornFXSceneComponent. */
UCLASS(HideCategories=(Input, Collision, Replication, Materials))
class POPCORNFX_API APopcornFXSceneActor : public AActor
{
	GENERATED_UCLASS_BODY()

	/** The PopcornFX Scene Component */
	UPROPERTY(Category="PopcornFX Scene", VisibleAnywhere, Instanced, BlueprintReadOnly)
	class UPopcornFXSceneComponent		*PopcornFXSceneComponent;

	CParticleScene		*ParticleScene();

	FName				GetSceneName() const;
};
