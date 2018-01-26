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

#if WITH_EDITOR

#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkit.h"

class UPopcornFXEffect;

/**
* Sequencer public interface
*/
class POPCORNFX_API IPopcornFXEffectEditor : public FAssetEditorToolkit
{
public:
};

#endif // WITH_EDITOR

