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

#include "PopcornFXMinimal.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_delegates.h>

FWD_PK_API_BEGIN
namespace HBO
{
	class CFieldDefinition;
}
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

namespace PopcornFX
{
	typedef FastDelegate<void(const PopcornFX::CString &value, const PopcornFX::CBaseObject &hbo, hh_u32 fieldIndex)> CbDependency;
	void			GatherDependencies(const PopcornFX::CString &path, const CbDependency &cb);
} // namespace PopcornFX

#endif // WITH_EDITOR
