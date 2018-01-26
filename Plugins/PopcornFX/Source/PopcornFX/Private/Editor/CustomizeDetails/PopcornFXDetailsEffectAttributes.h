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

#include "Runtime/Launch/Resources/Version.h"
#include "IDetailCustomization.h"

class FPopcornFXDetailsEffectAttributes : public IDetailCustomization
{
public:
	virtual void	CustomizeDetails(class IDetailLayoutBuilder& DetailBuilder) override;
};

#endif // WITH_EDITOR
