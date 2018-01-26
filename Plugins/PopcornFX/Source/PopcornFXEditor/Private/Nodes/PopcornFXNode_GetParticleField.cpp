//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXNode_GetParticleField.h"

#include "EdGraphSchema_K2.h"

#include "PopcornFXSDK.h"
#include "PopcornFXFunctions.h"
#include "Nodes/PopcornFXNode_GetParticleField.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXSceneComponent.h"

#define LOCTEXT_NAMESPACE "PopcornFXNode_GetParticleField"

UPopcornFXNode_GetParticleField::UPopcornFXNode_GetParticleField(const FObjectInitializer &objectInitializer)
:	Super(objectInitializer)
{
	m_NodeTitle = LOCTEXT("GetParticleField", "Get Particle Field");
	m_NodeTooltip = LOCTEXT("GetParticleFieldNodeTooltip", "Retrieves a field from the particle that triggered the event");
	m_MenuCategory = LOCTEXT("MenuCategory", "PopcornFX|Events");
	m_ValuePrefix = TEXT("Out");

	const UEdGraphSchema_K2	*schema = GetDefault<UEdGraphSchema_K2>();
	HH_ASSERT(schema != null);
	m_CustomParameters.Add(TPair<FString, FString>(TPairInitializer<FString, FString>("FieldName", schema->PC_Name)));
}

//---------------------------------------------------------------------------

UClass	*UPopcornFXNode_GetParticleField::GetSelfPinClass() const
{
	return UPopcornFXSceneComponent::StaticClass();
}

//----------------------------------------------------------------------------

bool	UPopcornFXNode_GetParticleField::IsCompatibleWithGraph(UEdGraph const *graph) const
{
	const UEdGraphSchema_K2	*schema = Cast<UEdGraphSchema_K2>(graph->GetSchema());
	const bool				isConstructionScript = schema != null ? schema->IsConstructionScript(graph) : false;

	return !isConstructionScript && Super::IsCompatibleWithGraph(graph);
}

//----------------------------------------------------------------------------

bool		UPopcornFXNode_GetParticleField::SetupNativeFunctionCall(UK2Node_CallFunction *functionCall)
{
	FName			functionName;
	switch (PinType())
	{
	case	EPopcornFXPinFieldType::Float:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsFloat);
		break;
	case	EPopcornFXPinFieldType::Float2:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsFloat2);
		break;
	case	EPopcornFXPinFieldType::Float3:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsFloat3);
		break;
	case	EPopcornFXPinFieldType::Float4:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsFloat4);
		break;

	case	EPopcornFXPinFieldType::Int:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsInt);
		break;
	case	EPopcornFXPinFieldType::Int2:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsInt2);
		break;
	case	EPopcornFXPinFieldType::Int3:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsInt3);
		break;
	case	EPopcornFXPinFieldType::Int4:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsInt4);
		break;

	case	EPopcornFXPinFieldType::Vector2D:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsVector2D);
		break;
	case	EPopcornFXPinFieldType::Vector:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsVector);
		break;
	case	EPopcornFXPinFieldType::LinearColor:
		functionName = GET_FUNCTION_NAME_CHECKED(UPopcornFXFunctions, GetParticleFieldAsLinearColor);
		break;
	}
	if (!functionName.IsValid() || functionName.IsNone())
		return false;
	UFunction	*function = UPopcornFXFunctions::StaticClass()->FindFunctionByName(functionName);
	if (!HH_VERIFY(function != null))
		return false;
	functionCall->SetFromFunction(function);
	//functionCall->FunctionReference.SetExternalMember(functionName, UPopcornFXFunctions::StaticClass());
	functionCall->AllocateDefaultPins();
	return true;
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
