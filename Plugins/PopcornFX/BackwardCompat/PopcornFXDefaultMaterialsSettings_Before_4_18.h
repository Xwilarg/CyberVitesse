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

#include "Runtime/Launch/Resources/Version.h"

#if (ENGINE_MINOR_VERSION >= 18)
#	error This file shouldn't be present in UE4 >= 18
#endif	// (ENGINE_MINOR_VERSION >= 18)

#include "Misc/StringAssetReference.h"

// Note: Code specific to keep 4.18 compatible with previous versions
// To remove once we drop <= UE4.17

#include "PopcornFXDefaultMaterialsSettings.generated.h"

USTRUCT()
struct FPopcornFXDefaultMaterialsSettings
{
	GENERATED_USTRUCT_BODY()

	FPopcornFXDefaultMaterialsSettings() { }

	/** Additive Billboard Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Billboard_Additive;
	
	/** AlphaBlend Billboard Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Billboard_AlphaBlend;
	
	/** Lit AlphaBlend Billboard Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Billboard_AlphaBlend_Lit;
	
	/** AlphaBlend Additive Billboard Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Billboard_AlphaBlendAdditive;
	
	/** Lit AlphaBlend Additive Billboard Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Billboard_AlphaBlendAdditive_Lit;
	
	/** Distortion Billboard Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Billboard_Distortion;
	
	/** AlphaTest Decal Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Decal_AlphaTest;
	
	/** Blended Decal Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Decal_Blended;
	
	/** Additive Mesh Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Mesh_Additive;
	
	/** Distortion Mesh Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Mesh_Distortion;
	
	/** Solid Mesh Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Mesh_Solid;
	
	/** Lit Solid Mesh Default Material. */
	UPROPERTY(Config, EditAnywhere, Category="Default Materials", meta=(AllowedClasses="MaterialInterface"))
	FStringAssetReference			Material_Mesh_Solid_Lit;
};
