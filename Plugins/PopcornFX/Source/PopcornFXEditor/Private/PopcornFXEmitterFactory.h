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

#include "ActorFactories/ActorFactory.h"
#include "PopcornFXEmitterFactory.generated.h"

UCLASS(MinimalAPI, Config=Editor)
class UPopcornFXEmitterFactory : public UActorFactory
{
	GENERATED_UCLASS_BODY()

	// override UActorFactory
	virtual bool			CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	virtual void			PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void			PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
	virtual UObject*		GetAssetFromActorInstance(AActor* ActorInstance) override;
};
