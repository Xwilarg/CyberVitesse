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

class	FPopcornFXPlugin;

/** Base class for every PopcornFX dependency modules */
class FPopcornFXDependencyModule
{
public:
	FPopcornFXDependencyModule();
	virtual ~FPopcornFXDependencyModule();

	inline bool			Loaded() const { return m_Loaded; }
	virtual void		Load() = 0;
	virtual void		Unload() = 0;

protected:
	bool				m_Loaded;
	FPopcornFXPlugin	*m_Plugin;
};
