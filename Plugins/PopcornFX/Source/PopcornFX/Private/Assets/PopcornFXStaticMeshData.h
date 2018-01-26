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

#include "PopcornFXMinimal.h"

#include "Engine/AssetUserData.h"

#include "PopcornFXStaticMeshData.generated.h"

class UPopcornFXMesh;

/** UserData proxy to the PopcornFXMesh Acceleration Structures. */
UCLASS(MinimalAPI)
class UPopcornFXStaticMeshData : public UAssetUserData
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(Category="PopcornFX Mesh", VisibleAnywhere, Instanced)
	UPopcornFXMesh			*PopcornFXMesh;
};
