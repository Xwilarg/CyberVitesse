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

#include "Misc/Guid.h"

// Global custom serialization version
struct POPCORNFX_API FPopcornFXCustomVersion
{
	enum Type
	{
		// Before any version changes were made in the plugin
		BeforeCustomVersionWasAdded = 0,

		VertexInstanceBBAdded,

		// New non-Transcient Material Instances Constant
		// With Static Switches
		RendererMaterials_AddMaterialInstanceConstant,

		// EPopcornFXMaterialType:: ... _NoAlpha are now Removed/Deprecated
		// Now uses Static Switch POPCORNFX_IS_NO_ALPHA
		RendererMaterials_RemoveNoAlphaType,

		// New default Materials with StaticSwitches
		RendererMaterials_NewDefaultMaterials,

		// Default Materials moved to the PopcornFX Config
		RendererMaterials_DefaultMaterialsInConfig,

		// Texture Atlases own class
		RendererMaterials_AddTextureAtlasClass,

		// Changed booleans to an enum
		AttributeSamplers_ChangeTransforms,

		// UPopcornFXNode_DynamicField replace FieldType By PinType
		Node_ReplaceFieldTypeBySimplerPinType,

		// -----<new versions can be added above this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	// The GUID for this custom version number
	const static FGuid GUID;

private:
	FPopcornFXCustomVersion() {}
};
