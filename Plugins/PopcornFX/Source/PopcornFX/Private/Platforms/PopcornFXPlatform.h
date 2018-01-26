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

#include "Platform.h"

FWD_PK_API_BEGIN
class CImage;
FWD_PK_API_END

class UTexture;

PopcornFX::CImage			*PopcornFXPlatform_NewImageFromTexture(class UTexture *texture);
extern PopcornFX::CImage	*_CreateFallbackImage();

#if PLATFORM_PS4
#	define PKFX_COMMON_NewImageFromTexture		0
#elif PLATFORM_XBOXONE
#	define PKFX_COMMON_NewImageFromTexture		0
#else
#	define PKFX_COMMON_NewImageFromTexture		1
#endif
