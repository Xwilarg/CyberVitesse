//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXCustomVersion.h"

#include "Serialization/CustomVersion.h"

const FGuid FPopcornFXCustomVersion::GUID(0xf3a955bf, 0x66d1877f, 0xa2949658, 0x4363aff4);

// Register the custom version with core
FCustomVersionRegistration	GRegisterPopcornFXCustomVersion(FPopcornFXCustomVersion::GUID, FPopcornFXCustomVersion::LatestVersion, TEXT("PopcornFXVer"));
