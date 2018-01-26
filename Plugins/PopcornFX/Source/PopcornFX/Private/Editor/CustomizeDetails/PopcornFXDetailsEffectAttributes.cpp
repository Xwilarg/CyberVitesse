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
#include "PopcornFXDetailsEffectAttributes.h"

#include "DetailLayoutBuilder.h"

//----------------------------------------------------------------------------

void	FPopcornFXDetailsEffectAttributes::CustomizeDetails(class IDetailLayoutBuilder& DetailLayout)
{
	// calling EditCategory reorder Categories in the Editor
	DetailLayout.HideCategory("PopcornFX RendererMaterials");
	DetailLayout.HideCategory("PopcornFX AssetDependencies");
	DetailLayout.HideCategory("Source");
	DetailLayout.HideCategory("UserDatas");
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
