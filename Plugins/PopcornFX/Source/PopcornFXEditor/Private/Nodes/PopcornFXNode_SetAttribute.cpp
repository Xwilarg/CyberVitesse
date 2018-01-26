//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXNode_SetAttribute.h"

#include "EdGraphSchema_K2.h"
#include "KismetCompiler.h"

#include "PopcornFXSDK.h"

#include "PopcornFXAttributeFunctions.h"
#include "Nodes/PopcornFXNode_SetAttribute.h"
#include "PopcornFXEmitterComponent.h"

#define LOCTEXT_NAMESPACE "PopcornFXNode_SetAttribute"

//----------------------------------------------------------------------------

UPopcornFXNode_SetAttribute::UPopcornFXNode_SetAttribute(const FObjectInitializer &objectInitializer)
:	Super(objectInitializer)
{
	m_NodeTitle = LOCTEXT("SetAttribute", "Set Attribute");
	m_NodeTooltip = LOCTEXT("SetAttributeNodeTooltip", "Sets a specific attribute value");
	m_MenuCategory = LOCTEXT("MenuCategory", "PopcornFX|Attributes");
	m_ValuePrefix = TEXT("In");
	m_ValueDirection = EGPD_Input;

	const UEdGraphSchema_K2		*schema = GetDefault<UEdGraphSchema_K2>();
	HH_ASSERT(schema != null);
	m_CustomParameters.Add(TPair<FString, FString>(TPairInitializer<FString, FString>("InAttributeIndex", schema->PC_Int)));
	m_CustomParameters.Add(TPair<FString, FString>(TPairInitializer<FString, FString>("InApplyGlobalScale", schema->PC_Boolean)));
}

//---------------------------------------------------------------------------

UClass	*UPopcornFXNode_SetAttribute::GetSelfPinClass() const
{
	return UPopcornFXEmitterComponent::StaticClass();
}

//----------------------------------------------------------------------------

bool		UPopcornFXNode_SetAttribute::SetupNativeFunctionCall(UK2Node_CallFunction *functionCall)
{
	FName			functionName;
	switch (PinType())
	{
	case	EPopcornFXPinFieldType::Float:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsFloat);
		break;
	case	EPopcornFXPinFieldType::Float2:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsFloat2);
		break;
	case	EPopcornFXPinFieldType::Float3:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsFloat3);
		break;
	case	EPopcornFXPinFieldType::Float4:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsFloat4);
		break;

	case	EPopcornFXPinFieldType::Int:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsInt);
		break;
	case	EPopcornFXPinFieldType::Int2:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsInt2);
		break;
	case	EPopcornFXPinFieldType::Int3:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsInt3);
		break;
	case	EPopcornFXPinFieldType::Int4:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsInt4);
		break;

	case	EPopcornFXPinFieldType::Vector2D:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsVector2D);
		break;
	case	EPopcornFXPinFieldType::Vector:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsVector);
		break;
	case	EPopcornFXPinFieldType::LinearColor:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, SetAttributeAsLinearColor);
		break;
	}
	if (!functionName.IsValid() || functionName.IsNone())
		return false;
	UFunction	*function = UPopcornFXAttributeFunctions::StaticClass()->FindFunctionByName(functionName);
	if (!HH_VERIFY(function != null))
		return false;
	functionCall->SetFromFunction(function);
	//functionCall->FunctionReference.SetExternalMember(functionName, UPopcornFXAttributeFunctions::StaticClass());
	functionCall->AllocateDefaultPins();
	return true;
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
