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

#include "Nodes/PopcornFXNode_DynamicField.h"
#include "PopcornFXNode_SetAttribute.generated.h"

UCLASS(MinimalApi)
class UPopcornFXNode_SetAttribute : public UPopcornFXNode_DynamicField
{
	GENERATED_UCLASS_BODY()
private:
	virtual bool		SetupNativeFunctionCall(UK2Node_CallFunction *functionCall) override;
	virtual UClass		*GetSelfPinClass() const override;
};
