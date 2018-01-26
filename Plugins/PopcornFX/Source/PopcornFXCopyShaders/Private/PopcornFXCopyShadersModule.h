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

#include "ModuleManager.h"

/**
	Compares modification date of PopcornFX usf shaders in Engine and the Plugin, and Copy the last modified one into the other.
*/
class POPCORNFXCOPYSHADERS_API FPopcornFXCopyShadersModule : public IModuleInterface
{
public:
	virtual void	StartupModule() override;
	virtual void	ShutdownModule() override;
private:
	void			UpdateShaders();
};
