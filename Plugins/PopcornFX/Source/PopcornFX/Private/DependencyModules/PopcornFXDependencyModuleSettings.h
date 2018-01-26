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

#include "PopcornFXDependencyModule.h"

class POPCORNFX_API FPopcornFXDependencyModuleSettings : public FPopcornFXDependencyModule
{
public:
	FPopcornFXDependencyModuleSettings();

	virtual void	Load() override;
	virtual void	Unload() override;
};
