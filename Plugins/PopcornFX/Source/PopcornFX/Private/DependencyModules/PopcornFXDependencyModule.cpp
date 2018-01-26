//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXDependencyModule.h"

#include "PopcornFXPlugin.h"

//----------------------------------------------------------------------------

FPopcornFXDependencyModule::FPopcornFXDependencyModule()
:	m_Loaded(false)
{
	m_Plugin = (FPopcornFXPlugin*)&IPopcornFXPlugin::Get();
}

//----------------------------------------------------------------------------

FPopcornFXDependencyModule::~FPopcornFXDependencyModule()
{
	HH_ASSERT(!Loaded());
	// Unload(); // vtable if fucked up here, cannot call the virtual function
}

//----------------------------------------------------------------------------
