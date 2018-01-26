//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#if WITH_EDITOR
#include "PopcornFXCustomizationRendererMaterial.h"

#include "Assets/PopcornFXRendererMaterial.h"
#include "Editor/EditorHelpers.h"

#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetThumbnail.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "Widgets/Input/SEditableTextBox.h"

#define LOCTEXT_NAMESPACE "PopcornFXCustomizationRendererMaterial"

// static
TSharedRef<IPropertyTypeCustomization> FPopcornFXCustomizationRendererMaterial::MakeInstance()
{
	return MakeShareable(new FPopcornFXCustomizationRendererMaterial);
}

void	FPopcornFXCustomizationRendererMaterial::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
	//TSharedRef<IPropertyHandle>		baseObjectNames = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXRendererMaterial, BaseObjectNames)).ToSharedRef();
	TSharedRef<IPropertyHandle>		baseObjectNames = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXRendererMaterial, BaseObjectFriendlyNames)).ToSharedRef();
	check(baseObjectNames->IsValidHandle());

	TSharedRef<IPropertyHandle>		materials = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXRendererMaterial, SubMaterials)).ToSharedRef();
	check(materials->IsValidHandle());

	HeaderRow.Visibility(EVisibility::Visible);

	TSharedPtr<SVerticalBox>	vboxNames;

	HeaderRow.NameContent()
		.MinDesiredWidth(125.f * 3.f)
		.MaxDesiredWidth(125.f * 4.f)
		[
			SAssignNew(vboxNames, SVerticalBox)
		]
	.ValueContent()
		.MinDesiredWidth(125.f * 3.f)
		.MaxDesiredWidth(125.f * 4.f)
		[
			materials->CreatePropertyValueWidget()
		]
	;

	static const FName	propertyFontStyle(TEXT("PropertyWindow.NormalFont"));
	FSlateFontInfo		fontStyle = FEditorStyle::GetFontStyle(propertyFontStyle);

	uint32		numBaseObjects;
	baseObjectNames->GetNumChildren(numBaseObjects);
	for (uint32 i = 0; i < numBaseObjects; ++i)
	{
		TSharedPtr<IPropertyHandle>	boNamePty = baseObjectNames->GetChildHandle(i);
		if (!boNamePty->IsValidHandle())
			return;

		// Here, do some name formatting.
		// We could have done that in UPopcornFXRendererMaterial::_AddBaseObject,
		// but it requires to reimport effects.
		FString	boName;
		boNamePty->GetValue(boName);

		if (!boName.IsEmpty())
		{
			boName.RemoveFromStart("Spawner/");
			vboxNames->AddSlot()
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(boName))
					.Font(fontStyle)
					.IsReadOnly(true)
				];
		}
	}
}

void	FPopcornFXCustomizationRendererMaterial::CustomizeChildren(
	TSharedRef<class IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& StructBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TSharedRef<IPropertyHandle>		materials = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXRendererMaterial, SubMaterials)).ToSharedRef();
	check(materials->IsValidHandle());

	uint32			materialsNum = 0;
	materials->GetNumChildren(materialsNum);

	for (uint32 materiali = 0; materiali < materialsNum; ++materiali)
	{
#if (ENGINE_MINOR_VERSION >= 17)
		StructBuilder.AddProperty(materials->GetChildHandle(materiali).ToSharedRef())
#else
		StructBuilder.AddChildProperty(materials->GetChildHandle(materiali).ToSharedRef())
#endif
			.ShouldAutoExpand(true)
#if (ENGINE_MINOR_VERSION >= 10)
			.OverrideResetToDefault(FResetToDefaultOverride::Hide(false))
#else
			.OverrideResetToDefault(false, FSimpleDelegate())
#endif
			;
	}
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
