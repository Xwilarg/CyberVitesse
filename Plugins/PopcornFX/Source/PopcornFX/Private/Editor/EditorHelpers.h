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

#include "PropertyHandle.h"

bool				HelperImportFile(const FString &srcPhysicalFilePath, const FString& dstAssetFilePath, UClass *uclass);
inline bool			IsValidHandle(const TSharedPtr<IPropertyHandle> &pty) { return pty.IsValid() && pty->IsValidHandle(); }
FString				SanitizeObjectPath(const FString &path);
void				ForceSetPackageDiry(UObject *object);

#endif // WITH_EDITOR
