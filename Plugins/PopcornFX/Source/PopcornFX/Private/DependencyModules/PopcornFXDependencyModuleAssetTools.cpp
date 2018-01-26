//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#if WITH_EDITOR
#include "PopcornFXDependencyModuleAssetTools.h"

#include "PopcornFXPlugin.h"
#include "Assets/PopcornFXEffect.h"
#include "Assets/PopcornFXTextureAtlas.h"

#include "AssetToolsModule.h"
#include "Editor/PopcornFXTypeActions.h"
#include "Editor/PopcornFXStyle.h"

#include "PopcornFXSDK.h"


#define LOCTEXT_NAMESPACE "PopcornFXAssetTools"

//----------------------------------------------------------------------------

FPopcornFXDependencyModuleAssetTools::FPopcornFXDependencyModuleAssetTools()
{
}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModuleAssetTools::Load()
{
	if (m_Loaded)
		return;
	if (!HH_VERIFY(FModuleManager::Get().IsModuleLoaded("AssetTools")))
		return;

	IAssetTools	&assetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	m_PopcornFXAssetCategoryBit = assetTools.RegisterAdvancedAssetCategory(FName(TEXT("PopcornFX")), LOCTEXT("PopcornFXAssetCategory", "PopcornFX"));

	FColor			pkColor = FPopcornFXPlugin::Color_PopcornFX().Quantize();

	m_EffectActions = MakeShareable(new FPopcornFXFileTypeActions(m_PopcornFXAssetCategoryBit));
	m_EffectActions->m_SupportedClass = UPopcornFXEffect::StaticClass();
	m_EffectActions->m_Name = NSLOCTEXT("AssetTypeActions", "PopcornFXEffectName", "PopcornFX Effect");
	m_EffectActions->m_Color = pkColor;
	assetTools.RegisterAssetTypeActions(m_EffectActions.ToSharedRef());

	m_TextureAtlasActions = MakeShareable(new FPopcornFXFileTypeActions(m_PopcornFXAssetCategoryBit));
	m_TextureAtlasActions->m_SupportedClass = UPopcornFXTextureAtlas::StaticClass();
	m_TextureAtlasActions->m_Name = NSLOCTEXT("AssetTypeActions", "PopcornFXTextureAtlasName", "PopcornFX Texture Atlas");
	m_TextureAtlasActions->m_Color = pkColor;
	assetTools.RegisterAssetTypeActions(m_TextureAtlasActions.ToSharedRef());

	m_FileActions = MakeShareable(new FPopcornFXFileTypeActions(m_PopcornFXAssetCategoryBit));
	m_FileActions->m_SupportedClass = UPopcornFXFile::StaticClass();
	m_FileActions->m_Name = NSLOCTEXT("AssetTypeActions", "PopcornFXFileName", "PopcornFX File");
	m_FileActions->m_Color = pkColor;
	assetTools.RegisterAssetTypeActions(m_FileActions.ToSharedRef());

	m_Loaded = true;
	FPopcornFXStyle::Initialize();
}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModuleAssetTools::Unload()
{
	if (!m_Loaded)
		return;
	m_Loaded = false;

	if (/*HH_VERIFY*/(FModuleManager::Get().IsModuleLoaded("AssetTools"))) // @FIXME: we should be called when the plugin is still loaded
	{
		IAssetTools	&assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		if (m_EffectActions.IsValid())
		{
			assetTools.UnregisterAssetTypeActions(m_EffectActions.ToSharedRef());
			m_EffectActions = null;
		}
		if (m_TextureAtlasActions.IsValid())
		{
			assetTools.UnregisterAssetTypeActions(m_TextureAtlasActions.ToSharedRef());
			m_TextureAtlasActions = null;
		}
		if (m_FileActions.IsValid())
		{
			assetTools.UnregisterAssetTypeActions(m_FileActions.ToSharedRef());
			m_FileActions = null;
		}
	}
	FPopcornFXStyle::Shutdown();
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
