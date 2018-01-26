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
#include "PropertyEditorModule.h"
#include "IDetailCustomization.h"

#if (ENGINE_MINOR_VERSION >= 15)
#	include "IDetailCustomization.h"
#endif

class FPopcornFXDetailsSettings : public IDetailCustomization
{
public:
	FPopcornFXDetailsSettings();

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization>		MakeInstance();

	virtual void		CustomizeDetails(IDetailLayoutBuilder &detailLayout) override;
};

#endif // WITH_EDITOR
