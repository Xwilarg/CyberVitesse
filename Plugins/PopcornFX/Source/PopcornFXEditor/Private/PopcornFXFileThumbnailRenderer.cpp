//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXFileThumbnailRenderer.h"

#include "Assets/PopcornFXFile.h"

#include "Engine/Texture2D.h"

//----------------------------------------------------------------------------

UPopcornFXFileThumbnailRenderer::UPopcornFXFileThumbnailRenderer(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
}

//----------------------------------------------------------------------------

void	UPopcornFXFileThumbnailRenderer::Draw(UObject *object, int32 x, int32 y, uint32 width, uint32 height, FRenderTarget *renderTarget, FCanvas *canvas)
{
	UPopcornFXFile		*file = Cast<UPopcornFXFile>(object);
	if (file != nullptr)
	{
		UTexture2D		*texture = file->ThumbnailImage;
		if (texture != nullptr) // Avoid useless call
			Super::Draw(texture, x, y, width, height, renderTarget, canvas);
	}
}

//----------------------------------------------------------------------------
