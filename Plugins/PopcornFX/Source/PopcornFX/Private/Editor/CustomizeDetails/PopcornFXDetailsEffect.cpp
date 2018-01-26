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
#include "PopcornFXDetailsEffect.h"

#include "Assets/PopcornFXEffect.h"
#include "Editor/EditorHelpers.h"
#include "Editor/PopcornFXEffectEditor.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "PopcornFXDetailsEffect"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXEffectEditor, Log, All);

using PopcornFX::CString;

//----------------------------------------------------------------------------

namespace
{
	void			DumpPropertyArray(class IDetailLayoutBuilder& DetailLayout, IDetailCategoryBuilder &cat, FName propertyPath, UClass *classOuter = null)
	{
		TSharedRef<IPropertyHandle>		pty = DetailLayout.GetProperty(propertyPath, classOuter);
		DetailLayout.HideProperty(pty);
		uint32				count = 0;
		pty->GetNumChildren(count);
		for (uint32 i = 0; i < count; ++i)
			cat.AddProperty(pty->GetChildHandle(i));
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsEffect::CustomizeDetails(class IDetailLayoutBuilder& DetailLayout)
{
	//m_DetailLayout = &DetailLayout;
	//m_PropertyUtilities = m_DetailLayout->GetPropertyUtilities();

	// calling EditCategory reorder Categories in the Editor
	IDetailCategoryBuilder		&catAssetDeps = DetailLayout.EditCategory("PopcornFX AssetDependencies");
	IDetailCategoryBuilder		&catRenderMats = DetailLayout.EditCategory("PopcornFX RendererMaterials");

	DetailLayout.HideCategory("PopcornFX Attributes");

	//TSharedRef<IPropertyHandle>		assetDependencies = DetailLayout.GetProperty(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXFile, AssetDependencies)).ToSharedRef();

	DumpPropertyArray(DetailLayout, catAssetDeps, "AssetDependencies", UPopcornFXFile::StaticClass());
	DumpPropertyArray(DetailLayout, catRenderMats, GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEffect, ParticleRendererMaterials));
	DumpPropertyArray(DetailLayout, catRenderMats, GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEffect, DecalsRendererMaterials));
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
