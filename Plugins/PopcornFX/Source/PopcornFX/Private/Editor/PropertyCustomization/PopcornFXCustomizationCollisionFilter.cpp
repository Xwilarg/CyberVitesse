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
#include "PopcornFXCustomizationCollisionFilter.h"

#include "PopcornFXSceneComponent.h"
#include "Editor/EditorHelpers.h"
#include "PopcornFXSDK.h"

#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetThumbnail.h"
#include "Engine/CollisionProfile.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"

#define LOCTEXT_NAMESPACE "PopcornFXCustomizationCollisionFilter"

// static
TSharedRef<IPropertyTypeCustomization> FPopcornFXCustomizationCollisionFilter::MakeInstance()
{
	return MakeShareable(new FPopcornFXCustomizationCollisionFilter);
}

FPopcornFXCustomizationCollisionFilter::FPopcornFXCustomizationCollisionFilter()
{
}

void	FPopcornFXCustomizationCollisionFilter::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	int32				filterIndex = PropertyHandle->GetIndexInArray();

	m_ObjectTypesToQuery = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(FPopcornFXCollisionFilter, ObjectTypesToQuery));
	HH_VERIFY(IsValidHandle(m_ObjectTypesToQuery));

	FText		headerName;
	FText		headerTooltip;
	if (filterIndex != INDEX_NONE)
	{
		headerName = FText::FromString(FString::Printf(TEXT("[%d]"), filterIndex));
		headerTooltip = LOCTEXT("FilterID_Collision_preset", "The CParticleEvolverPhysics [FilterID] Collision Preset");
	}
	else
	{
		headerName = LOCTEXT("Default_Collision_preset", "Default Collision Preset");
		headerTooltip = headerName;
	}

	HeaderRow
		.NameContent()
		.MinDesiredWidth(125.f * 3.f)
		.MaxDesiredWidth(125.f * 4.f)
		[
			SNew(STextBlock)
			.Text(headerName)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.ToolTipText(headerTooltip)
		];
}


void	FPopcornFXCustomizationCollisionFilter::CustomizeChildren(
	TSharedRef<class IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& StructBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	m_CollisionProfile = UCollisionProfile::Get();
	if (!HH_VERIFY(m_CollisionProfile != null))
		return;
	int32			channel = 0;
	FName			channelName;
	FName			end = TEXT("");
	const hh_u32	maxChannels = 32; // ECC_MAX

	while ((channelName = m_CollisionProfile->ReturnChannelNameFromContainerIndex(channel)) != end && channel < maxChannels)
	{
		FString				channelStr = channelName.ToString();
		if (channelStr.IsEmpty())
		{
			++channel;
			continue;
		}
#if (ENGINE_MINOR_VERSION >= 17)
		FDetailWidgetRow	&row = StructBuilder.AddCustomRow(FText::FromString(channelStr));
#else
		FDetailWidgetRow	&row = StructBuilder.AddChildContent(FText::FromString(channelStr));
#endif

		row.NameContent()
			[
				SNew(STextBlock)
				.Text(FText::FromString(channelStr))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			];

		row.ValueContent()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &Self::OnCheckStateChanged, channel)
				.IsChecked(this, &Self::IsChecked, channel)
			];

		++channel;
	}
}

// Gore #ifdefs..
ECheckBoxState	FPopcornFXCustomizationCollisionFilter::IsChecked(int32 channel) const
{
	int32			value;
	if (!IsValidHandle(m_ObjectTypesToQuery) || m_ObjectTypesToQuery->GetValue(value) != FPropertyAccess::Success)
		return ECheckBoxState::Undetermined;
	if ((value & ECC_TO_BITFIELD(channel)) != 0)
		return ECheckBoxState::Checked;
	return ECheckBoxState::Unchecked;
}

void	FPopcornFXCustomizationCollisionFilter::OnCheckStateChanged(ECheckBoxState checked, int32 channel)
{
	int32		value;
	if (!IsValidHandle(m_ObjectTypesToQuery) || m_ObjectTypesToQuery->GetValue(value) != FPropertyAccess::Success)
		return;

	if (checked == ECheckBoxState::Unchecked)
	{
		value &= ~(ECC_TO_BITFIELD(channel));
	}
	else if (checked == ECheckBoxState::Checked)
	{
		value |= (ECC_TO_BITFIELD(channel));
	}
	else
		return;

	m_ObjectTypesToQuery->SetValue(value);
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
