//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXAssetBroker.h"

#include "PopcornFXEmitterComponent.h"
#include "Assets/PopcornFXEffect.h"

#if WITH_EDITOR

UClass		*FPopcornFXEmitterComponentAssetBroker::ComponentStaticClass()
{
	return UPopcornFXEmitterComponent::StaticClass();
}

UClass		*FPopcornFXEmitterComponentAssetBroker::GetSupportedAssetClass()
{
	return UPopcornFXEffect::StaticClass();
}

bool		FPopcornFXEmitterComponentAssetBroker::AssignAssetToComponent(UActorComponent* InComponent, UObject* InAsset)
{
	UPopcornFXEmitterComponent* comp = Cast<UPopcornFXEmitterComponent>(InComponent);
	if (comp != null)
	{
		UPopcornFXEffect	*asset = Cast<UPopcornFXEffect>(InAsset);
		if ((asset != nullptr) || (asset == nullptr))
		{
			comp->Effect = asset;
			return true;
		}
	}
	return false;
}

UObject		*FPopcornFXEmitterComponentAssetBroker::GetAssetFromComponent(UActorComponent* InComponent)
{
	UPopcornFXEmitterComponent* comp = Cast<UPopcornFXEmitterComponent>(InComponent);
	if (comp != null)
	{
		if ((comp->Effect != nullptr) && (comp->Effect->IsAsset()))
		{
			return comp->Effect;
		}
	}
	return nullptr;
}

#endif // WITH_EDITOR
