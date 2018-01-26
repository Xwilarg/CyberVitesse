//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXNode_DynamicField.h"

#include "EdGraphSchema_K2.h"
#include "KismetCompiler.h"
#include "ScopedTransaction.h"
#include "GraphEditorSettings.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"

#include "PopcornFXSDK.h"
#include "PopcornFXCustomVersion.h"
#include "PopcornFXAttributeFunctions.h"
#include "Nodes/PopcornFXNode_DynamicField.h"
#include "PopcornFXEmitterComponent.h"

#define LOCTEXT_NAMESPACE "PopcornFXNode_DynamicField"

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXNode, Log, All);


//----------------------------------------------------------------------------

UPopcornFXNode_DynamicField::UPopcornFXNode_DynamicField(const FObjectInitializer &objectInitializer)
:	Super(objectInitializer)
,	m_ValueDirection(EGPD_Output)
,	m_PinType(EPopcornFXPinFieldType::Float)
,	m_BreakValue_DEPRECATED(false)
,	m_ParticleFieldType_DEPRECATED(uint32(-1)) // old default value is same as new default m_PinType(Float)
{
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	const UEdGraphSchema_K2		*graphSchema = GetDefault<UEdGraphSchema_K2>();
	check(graphSchema != null);

	// Create the execution pins
	CreatePin(EGPD_Input, graphSchema->PC_Exec, TEXT(""), null, false, false, graphSchema->PN_Execute);
	CreatePin(EGPD_Output, graphSchema->PC_Exec, TEXT(""), null, false, false, graphSchema->PN_Then);

	// Create the input pins
	CreatePin(EGPD_Input, graphSchema->PC_Object, TEXT(""), GetSelfPinClass(), false, false, TEXT("InSelf"));
	const hh_u32	paramCount = m_CustomParameters.Num();
	for (hh_u32 iParam = 0; iParam < paramCount; ++iParam)
		CreatePin(EGPD_Input, m_CustomParameters[iParam].Value, TEXT(""), null, false, false, m_CustomParameters[iParam].Key);

	// Create the return value pin
	CreatePin(EGPD_Output, graphSchema->PC_Boolean, TEXT(""), null, false, false, graphSchema->PN_ReturnValue);

	{
		UEdGraphPin		*pinTypePin = CreatePin(EGPD_Input, graphSchema->PC_Byte, TEXT(""), FindObject<UEnum>(ANY_PACKAGE, TEXT("EPopcornFXPinFieldType"), true), false, false, TEXT("PinType"));
		HH_ASSERT(pinTypePin != null);
		pinTypePin->bNotConnectable = true;

		const FString	&newValue = EPopcornFXPinFieldType::GetPinValueName(m_PinType);
		// graphSchema->TrySetDefaultValue(*pinTypePin, newValue); // !!! Will call ReconstructNode() !! infinite recurse !
		pinTypePin->DefaultValue = newValue;
	}

	const EEdGraphPinDirection	pinDirection = m_ValueDirection.GetValue();
	const uint32				pinCount = EPopcornFXPinFieldType::RequiredPinNum(m_PinType);
	FEdGraphPinType				pinsType;
	EPopcornFXPinFieldType::GetGraphPinsType(m_PinType, pinsType);

	if (pinCount <= 1)
	{
		CreatePin(pinDirection, pinsType, m_ValuePrefix + "Value");
	}
	else
	{
		CreatePin(pinDirection, pinsType, m_ValuePrefix + "ValueX");
		//if (pinCount >= 2)
		CreatePin(pinDirection, pinsType, m_ValuePrefix + "ValueY");
		if (pinCount >= 3)
			CreatePin(pinDirection, pinsType, m_ValuePrefix + "ValueZ");
		if (pinCount >= 4)
			CreatePin(pinDirection, pinsType, m_ValuePrefix + "ValueW");
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FPopcornFXCustomVersion::GUID);

	if (Ar.IsLoading())
	{
		//const int32		version = GetLinkerCustomVersion(FPopcornFXCustomVersion::GUID);
		//if (version < FPopcornFXCustomVersion::Node_ReplaceFieldTypeBySimplerPinType)
		if (m_ParticleFieldType_DEPRECATED != uint32(-1))
		{
			check(m_ParticleFieldType_DEPRECATED <= EPopcornFXParticleFieldType::Int4);
			EPopcornFXParticleFieldType::Type	fieldType = (EPopcornFXParticleFieldType::Type)m_ParticleFieldType_DEPRECATED;
			switch (fieldType)
			{
			case EPopcornFXParticleFieldType::Float:
				m_PinType = EPopcornFXPinFieldType::Float;
				break;
			case EPopcornFXParticleFieldType::Float2:
				if (m_BreakValue_DEPRECATED)
					m_PinType = EPopcornFXPinFieldType::Float2;
				else
					m_PinType = EPopcornFXPinFieldType::Vector2D;
				break;
			case EPopcornFXParticleFieldType::Float3:
				if (m_BreakValue_DEPRECATED)
					m_PinType = EPopcornFXPinFieldType::Float3;
				else
					m_PinType = EPopcornFXPinFieldType::Vector;
				break;
			case EPopcornFXParticleFieldType::Float4:
				if (m_BreakValue_DEPRECATED)
					m_PinType = EPopcornFXPinFieldType::Float4;
				else
					m_PinType = EPopcornFXPinFieldType::LinearColor;
				break;
			case EPopcornFXParticleFieldType::Int:
				m_PinType = EPopcornFXPinFieldType::Int;
				break;
			case EPopcornFXParticleFieldType::Int2:
				m_PinType = EPopcornFXPinFieldType::Int2;
				break;
			case EPopcornFXParticleFieldType::Int3:
				m_PinType = EPopcornFXPinFieldType::Int3;
				break;
			case EPopcornFXParticleFieldType::Int4:
				m_PinType = EPopcornFXPinFieldType::Int4;
				break;
			}
			UE_LOG(LogPopcornFXNode, Log, TEXT("%s: Upgraded BP Node %s from FieldType %s %d to %d"), *GetFullName(), *m_NodeTitle.ToString(), m_BreakValue_DEPRECATED ? TEXT("breaked") : TEXT("merged"), int32(m_ParticleFieldType_DEPRECATED), int32(m_PinType));

			m_ParticleFieldType_DEPRECATED = uint32(-1);
		}
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::PostLoad()
{
	Super::PostLoad();
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::PostReconstructNode()
{
	Super::PostReconstructNode();

	// Break and Hide unwanted pins

	const uint32				pinCount = EPopcornFXPinFieldType::RequiredPinNum(m_PinType);
	if (pinCount <= 1)
	{
		BreakAndHidePinByName(m_ValuePrefix + "ValueX");
		BreakAndHidePinByName(m_ValuePrefix + "ValueY");
		BreakAndHidePinByName(m_ValuePrefix + "ValueZ");
		BreakAndHidePinByName(m_ValuePrefix + "ValueW");
	}
	else
	{
		BreakAndHidePinByName(m_ValuePrefix + "Value");
		//if (pinCount < 1) // false
		//	BreakAndHidePinByName(m_ValuePrefix + "ValueX");
		if (pinCount < 2)
			BreakAndHidePinByName(m_ValuePrefix + "ValueY");
		if (pinCount < 3)
			BreakAndHidePinByName(m_ValuePrefix + "ValueZ");
		if (pinCount < 4)
			BreakAndHidePinByName(m_ValuePrefix + "ValueW");
	}

	// InApplyGlobalScale can exists in m_CustomParameters
	if (!EPopcornFXPinFieldType::CanBeGlobalScaled(m_PinType))
		BreakAndHidePinByName("InApplyGlobalScale");
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 13)
FSlateIcon	UPopcornFXNode_DynamicField::GetIconAndTint(FLinearColor& OutColor) const
#else
FName		UPopcornFXNode_DynamicField::GetPaletteIcon(FLinearColor& OutColor) const
#endif
{
	const UGraphEditorSettings	*editorSettings = GetDefault<UGraphEditorSettings>();
	HH_ASSERT(editorSettings != null);
	OutColor = editorSettings->FunctionCallNodeTitleColor;
#if (ENGINE_MINOR_VERSION >= 13)
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
#else
	return TEXT("Kismet.AllClasses.FunctionIcon");
#endif
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::PinDefaultValueChanged(UEdGraphPin *pin)
{
	if (pin != FindPinChecked("PinType"))
		return;

	if (!HH_VERIFY(!pin->DefaultValue.IsEmpty()))
		return;

	UEnum		*pinTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPopcornFXPinFieldType"), true);
	check(pinTypeEnum);
	int32		rawValue = pinTypeEnum->GetValueByName(FName(*pin->DefaultValue)); // GetValueByName is fine with or without "EPopcornFXPinFieldType::"...
	if (!HH_VERIFY(rawValue != INDEX_NONE))
	{
		rawValue = EPopcornFXPinFieldType::Float;
	}
	EPopcornFXPinFieldType::Type	newValue = (EPopcornFXPinFieldType::Type)rawValue;

	if (newValue == m_PinType)
		return; // no need to ReconstructNode()

	m_PinType = newValue;
	ReconstructNode();
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::GetMenuActions(FBlueprintActionDatabaseRegistrar &actionRegistrar) const
{
	if (m_NodeTitle.IsEmpty() || m_NodeTooltip.IsEmpty() || m_MenuCategory.IsEmpty())
		return;
	const UClass	*actionKey = GetClass();
	if (actionRegistrar.IsOpenForRegistration(actionKey))
	{
		UBlueprintNodeSpawner	*nodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		HH_ASSERT(nodeSpawner != null);
		actionRegistrar.AddBlueprintAction(actionKey, nodeSpawner);
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::ExpandNode(class FKismetCompilerContext &compilerContext, UEdGraph *sourceGraph)
{
	Super::ExpandNode(compilerContext, sourceGraph);

	// Force rebuild output category
	UEdGraphPin		*pinTypePin = FindPinChecked("PinType");
	HH_ASSERT(pinTypePin != null);
	if (pinTypePin->DefaultValue.IsEmpty())
	{
		compilerContext.MessageLog.Error(*LOCTEXT("PopcornFXExpandNodeError", "UPopcornFXNode_FieldDynamic: Pin Type not specified. @@").ToString(), this);
		BreakAllNodeLinks();
		return;
	}
	HH_ONLY_IF_ASSERTS({
			const FString	&value = EPopcornFXPinFieldType::GetPinValueName(m_PinType);
			HH_ASSERT(pinTypePin->DefaultValue == value);
	});

	const UEdGraphSchema_K2	*schema = GetDefault<UEdGraphSchema_K2>();
	check(schema);

	UK2Node_CallFunction	*nativeFunctionCall = compilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, sourceGraph);
	if (!SetupNativeFunctionCall(nativeFunctionCall))
	{
		compilerContext.MessageLog.Error(*LOCTEXT("PopcornFXExpandNodeError", "UPopcornFXNode_FieldDynamic: Failed to recover native function call. @@").ToString(), this);
		BreakAllNodeLinks();
		return;
	}

	bool	success = true;

	success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, schema->PN_Execute, true));
	success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, schema->PN_Then, true));
	success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, "InSelf", true));

	const hh_u32	paramCount = m_CustomParameters.Num();
	for (hh_u32 iParam = 0; iParam < paramCount; ++iParam)
	{
		// m_CustomParameters are optional
		/*success &= */MovePinByName(compilerContext, nativeFunctionCall, m_CustomParameters[iParam].Key, false);
	}

	const uint32	pinCount = EPopcornFXPinFieldType::RequiredPinNum(m_PinType);

	if (pinCount <= 1)
	{
		success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, m_ValuePrefix + "Value", true));
	}
	else
	{
		success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, m_ValuePrefix + "ValueX", true));
		//if (pinCount >= 2)
		success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, m_ValuePrefix + "ValueY", true));
		if (pinCount >= 3)
			success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, m_ValuePrefix + "ValueZ", true));
		if (pinCount >= 4)
			success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, m_ValuePrefix + "ValueW", true));
	}

	success &= HH_VERIFY(MovePinByName(compilerContext, nativeFunctionCall, schema->PN_ReturnValue, true));

	if (!success)
	{
		compilerContext.MessageLog.Error(*LOCTEXT("PopcornFXExpandNodeError", "UPopcornFXNode_FieldDynamic: Internal connection error - @@").ToString(), this);
	}

	BreakAllNodeLinks();
}

//----------------------------------------------------------------------------

bool	UPopcornFXNode_DynamicField::MovePinByName(class FKismetCompilerContext &compilerContext, UK2Node_CallFunction *nativeFunctionCall, const FString &name, bool required)
{
	UEdGraphPin		*outPin = nativeFunctionCall->FindPin(name);
	HH_ASSERT(outPin != null || !required);
	if (outPin == null)
		return false;

	UEdGraphPin		*inPin = FindPin(name);
	HH_ASSERT((inPin != null && !inPin->bHidden) || !required);
	if (inPin == null || inPin->bHidden)
		return false;

	bool	success = compilerContext.MovePinLinksToIntermediate(*inPin, *outPin).CanSafeConnect();
	HH_ASSERT(success || !required);
	return success;
}

//----------------------------------------------------------------------------

void	UPopcornFXNode_DynamicField::BreakAndHidePinByName(const FString &name)
{
	UEdGraphPin		*valuePin = FindPin(name);
	if (valuePin != null)
	{
		valuePin->BreakAllPinLinks();
		valuePin->bHidden = true;
	}
}

//----------------------------------------------------------------------------

namespace	EPopcornFXPinFieldType
{

	uint32		RequiredPinNum(Type type)
	{
		switch (type)
		{
		case Float:
		case Int:
		case Vector2D:
		case Vector:
		case LinearColor:
			return 1;
		case Float2:
		case Int2:
			return 2;
		case Float3:
		case Int3:
			return 3;
		case Float4:
		case Int4:
			return 4;
		}
		checkNoEntry();
		return 0;
	}

	bool		CanBeGlobalScaled(Type type)
	{
		switch (type)
		{
		case Float:
		case Float2:
		case Float3:
		case Float4:
		case Vector2D:
		case Vector:
			return true;
		case Int:
		case Int2:
		case Int3:
		case Int4:
		case LinearColor:
			return false;
		}
		checkNoEntry()
		return false;
	}

#if (ENGINE_MINOR_VERSION >= 9)
#	define _GET_BASE_STRUCT(__type)		TBaseStructure<__type>::Get()
#else
#	define _GET_BASE_STRUCT(__type)		FindObjectChecked<UScriptStruct>(FindObjectChecked<UPackage>(nullptr, TEXT("/Script/CoreUObject")), TEXT(#__type))
#endif

	bool		GetGraphPinsType(Type type, FEdGraphPinType &outPinType)
	{
		const UEdGraphSchema_K2	*schema = GetDefault<UEdGraphSchema_K2>();
		check(schema);
		switch (type)
		{
		case Float:
		case Float2:
		case Float3:
		case Float4:
			outPinType.PinCategory = schema->PC_Float;
			return true;
		case Int:
		case Int2:
		case Int3:
		case Int4:
			outPinType.PinCategory = schema->PC_Int;
			return true;
		case Vector2D:
			outPinType.PinCategory = schema->PC_Struct;
			outPinType.PinSubCategoryObject = _GET_BASE_STRUCT(FVector2D);
			return true;
		case Vector:
			outPinType.PinCategory = schema->PC_Struct;
			outPinType.PinSubCategoryObject = _GET_BASE_STRUCT(FVector);
			return true;
		case LinearColor:
			outPinType.PinCategory = schema->PC_Struct;
			outPinType.PinSubCategoryObject = _GET_BASE_STRUCT(FLinearColor);
			return true;
		}
		checkNoEntry()
		return false;
	}

#undef _GET_BASE_STRUCT

	// Removes leading "EPopcornFXPinFieldType::" if nessecary for Pin value
	FString		GetPinValueName(Type value)
	{
		static const TCHAR	kEnumName[] = TEXT("EPopcornFXPinFieldType");
		static const TCHAR	kEnumNameDoubleColon[] = TEXT("EPopcornFXPinFieldType::");
		UEnum		*pinTypeEnum = FindObject<UEnum>(ANY_PACKAGE, kEnumName, true);
		check(pinTypeEnum);
		FString		name = pinTypeEnum->GetNameByValue(value).ToString();
		if (name.Len() > HH_ARRAY_COUNT(kEnumNameDoubleColon) && name.StartsWith(kEnumNameDoubleColon))
			name = name.RightChop(HH_ARRAY_COUNT(kEnumNameDoubleColon) - 1);
		return name;
	}
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
