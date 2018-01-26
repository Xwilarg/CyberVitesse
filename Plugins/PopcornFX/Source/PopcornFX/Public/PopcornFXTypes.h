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

UENUM(BlueprintType)
namespace	EPopcornFXParticleFieldType
{
	enum	Type
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
	};
}
namespace	EPopcornFXParticleFieldType
{
	inline uint32		Dim(Type type) { return (uint32(type) % 4) + 1; }
}
