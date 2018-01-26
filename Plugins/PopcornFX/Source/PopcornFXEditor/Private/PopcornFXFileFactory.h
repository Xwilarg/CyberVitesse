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

#include "Factories/Factory.h"

#include "PopcornFXFileFactory.generated.h"

UCLASS(MinimalAPI, HideCategories=Object)
class UPopcornFXFileFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
public:

	void		CleanUp() override;
	void		PostInitProperties() override;

	bool		DoesSupportClass(UClass * inClass) override;
	UClass		*ResolveSupportedClass() override;
	UObject		*FactoryCreateBinary(UClass *inClass, UObject *inParent, FName inName, EObjectFlags flags, UObject *context, const TCHAR *type, const uint8 *&buffer, const uint8 *bufferEnd, FFeedbackContext *warn, bool &bOutOperationCanceled) override;
};
