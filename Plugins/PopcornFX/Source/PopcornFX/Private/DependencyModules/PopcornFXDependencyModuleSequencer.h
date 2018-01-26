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

#if WITH_EDITOR

#include "PopcornFXDependencyModule.h"

class POPCORNFX_API FPopcornFXDependencyModuleSequencer : public FPopcornFXDependencyModule
{
public:
	FPopcornFXDependencyModuleSequencer();

	virtual void	Load() override;
	virtual void	Unload() override;
private:
	FDelegateHandle		AttributeTrackCreateEditorHandle;
	FDelegateHandle		PlayTrackCreateEditorHandle;
};

#endif // WITH_EDITOR
