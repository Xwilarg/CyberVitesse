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

#include "K2Node_CallFunction.h"
#include "PopcornFXTypes.h"

#include "PopcornFXNode_DynamicField.generated.h"

UENUM(BlueprintType)
namespace	EPopcornFXPinFieldType
{
	enum	Type
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Vector2D UMETA(DisplayName="Vector2D (Float2)"),
		Vector UMETA(DisplayName="Vector (Float3)"),
		LinearColor UMETA(DisplayName="LinearColor (Float4)"),
	};
}

namespace	EPopcornFXPinFieldType
{
	uint32		RequiredPinNum(Type type);
	bool		CanBeGlobalScaled(Type type);
	bool		GetGraphPinsType(Type type, FEdGraphPinType &outPinType);

	// Removes leading "EPopcornFXPinFieldType::" if nessecary for Pin value
	FString		GetPinValueName(EPopcornFXPinFieldType::Type value);
}

UCLASS(Abstract, MinimalApi)
class UPopcornFXNode_DynamicField : public UK2Node
{
	GENERATED_UCLASS_BODY()

private:
	virtual void	Serialize(FArchive& Ar) override;
	virtual void	PostLoad() override;

	virtual void	AllocateDefaultPins() override;
	virtual void	PostReconstructNode() override;
	virtual FText	GetNodeTitle(ENodeTitleType::Type titleType) const override { return m_NodeTitle; }
	virtual FText	GetTooltipText() const override { return m_NodeTooltip; }
	virtual void	PinDefaultValueChanged(UEdGraphPin *pin) override;

#if (ENGINE_MINOR_VERSION >= 13)
	virtual FSlateIcon	GetIconAndTint(FLinearColor& OutColor) const override;
#else
	virtual FName		GetPaletteIcon(FLinearColor& OutColor) const override;
#endif

	virtual void	GetMenuActions(FBlueprintActionDatabaseRegistrar &actionRegistrar) const override;
	virtual FText	GetMenuCategory() const override { return m_MenuCategory; }
	virtual bool	NodeCausesStructuralBlueprintChange() const { return true; }
	virtual void	ExpandNode(class FKismetCompilerContext &compilerContext, UEdGraph *sourceGraph) override;

private:
	bool			MovePinByName(class FKismetCompilerContext &compilerContext, UK2Node_CallFunction *nativeFunctionCall, const FString &name, bool required);
	void			BreakAndHidePinByName(const FString &name);

protected:
	EPopcornFXPinFieldType::Type		PinType() const { return m_PinType; }
	virtual bool	SetupNativeFunctionCall(UK2Node_CallFunction *functionCall) { check(false); return false; }
	virtual UClass	*GetSelfPinClass() const { check(false); return nullptr; }

protected:
	TEnumAsByte<EEdGraphPinDirection>	m_ValueDirection;

	FText		m_NodeTitle;
	FText		m_NodeTooltip;
	FText		m_MenuCategory;
	FString		m_ValuePrefix;

	TArray<TPair<FString, FString> >	m_CustomParameters;

private:
	UPROPERTY()
	TEnumAsByte<EPopcornFXPinFieldType::Type>	m_PinType;

	UPROPERTY()
	uint32			m_BreakValue_DEPRECATED : 1;

	UPROPERTY()
	uint32			m_ParticleFieldType_DEPRECATED;
};
