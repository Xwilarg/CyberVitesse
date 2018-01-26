//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXNode_GetAttribute.h"

#include "EdGraphSchema_K2.h"

#include "PopcornFXSDK.h"
#include "PopcornFXAttributeFunctions.h"
#include "Nodes/PopcornFXNode_GetAttribute.h"
#include "PopcornFXEmitterComponent.h"

#define LOCTEXT_NAMESPACE "PopcornFXNode_GetAttribute"

//----------------------------------------------------------------------------

UPopcornFXNode_GetAttribute::UPopcornFXNode_GetAttribute(const FObjectInitializer &objectInitializer)
:	Super(objectInitializer)
{
	m_NodeTitle = LOCTEXT("GetAttribute", "Get Attribute");
	m_NodeTooltip = LOCTEXT("GetAttributeNodeTooltip", "Gets a specific attribute value");
	m_MenuCategory = LOCTEXT("MenuCategory", "PopcornFX|Attributes");
	m_ValuePrefix = TEXT("Out");

	const UEdGraphSchema_K2	*schema = GetDefault<UEdGraphSchema_K2>();
	HH_ASSERT(schema != null);
	m_CustomParameters.Add(TPair<FString, FString>(TPairInitializer<FString, FString>("InAttributeIndex", schema->PC_Int)));
	m_CustomParameters.Add(TPair<FString, FString>(TPairInitializer<FString, FString>("InApplyGlobalScale", schema->PC_Boolean)));
}

//---------------------------------------------------------------------------

UClass	*UPopcornFXNode_GetAttribute::GetSelfPinClass() const
{
	return UPopcornFXEmitterComponent::StaticClass();
}

//----------------------------------------------------------------------------

bool		UPopcornFXNode_GetAttribute::SetupNativeFunctionCall(UK2Node_CallFunction *functionCall)
{
	FName			functionName;
	switch (PinType())
	{
	case	EPopcornFXPinFieldType::Float:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsFloat);
		break;
	case	EPopcornFXPinFieldType::Float2:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsFloat2);
		break;
	case	EPopcornFXPinFieldType::Float3:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsFloat3);
		break;
	case	EPopcornFXPinFieldType::Float4:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsFloat4);
		break;

	case	EPopcornFXPinFieldType::Int:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsInt);
		break;
	case	EPopcornFXPinFieldType::Int2:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsInt2);
		break;
	case	EPopcornFXPinFieldType::Int3:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsInt3);
		break;
	case	EPopcornFXPinFieldType::Int4:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsInt4);
		break;

	case	EPopcornFXPinFieldType::Vector2D:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsVector2D);
		break;
	case	EPopcornFXPinFieldType::Vector:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsVector);
		break;
	case	EPopcornFXPinFieldType::LinearColor:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXAttributeFunctions, GetAttributeAsLinearColor);
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
