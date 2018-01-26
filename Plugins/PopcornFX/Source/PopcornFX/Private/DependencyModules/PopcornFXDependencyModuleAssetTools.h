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

#if WITH_EDITOR

#include "PopcornFXDependencyModule.h"
#include "AssetTypeCategories.h"

class	FPopcornFXFileTypeActions;

class POPCORNFX_API FPopcornFXDependencyModuleAssetTools : public FPopcornFXDependencyModule
{
public:
	FPopcornFXDependencyModuleAssetTools();

	virtual void	Load() override;
	virtual void	Unload() override;
protected:
	EAssetTypeCategories::Type				m_PopcornFXAssetCategoryBit;
	TSharedPtr<FPopcornFXFileTypeActions>	m_EffectActions;
	TSharedPtr<FPopcornFXFileTypeActions>	m_TextureAtlasActions;
	TSharedPtr<FPopcornFXFileTypeActions>	m_FileActions;
};

#endif // WITH_EDITOR
