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
#include "PopcornFXDetailsEmitterComponent.h"

#include "Assets/PopcornFXEffect.h"
#include "PopcornFXEmitterComponent.h"
#include "Editor/EditorHelpers.h"
#include "PopcornFXSDK.h"

#include "DetailLayoutBuilder.h"
#include "IDetailsView.h"
#include "DetailCategoryBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"
#include "SVectorInputBox.h"
#include "EditorReimportHandler.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"

#include "SAssetDropTarget.h"

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXDetailsEmitterComponent"

//----------------------------------------------------------------------------

FPopcornFXDetailsEmitterComponent::FPopcornFXDetailsEmitterComponent()
{

}

//----------------------------------------------------------------------------

TSharedRef<IDetailCustomization>	FPopcornFXDetailsEmitterComponent::MakeInstance()
{
	return MakeShareable(new FPopcornFXDetailsEmitterComponent);
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsEmitterComponent::GatherEmitters(TArray<UPopcornFXEmitterComponent*> &outComponents) const
{
	const TArray< TWeakObjectPtr<UObject> >		&objects = m_SelectedObjectsList;
	for (int32 obji = 0; obji < objects.Num(); ++obji)
	{
		if (objects[obji].IsValid())
		{
			UPopcornFXEmitterComponent* emitterComponent = Cast<UPopcornFXEmitterComponent>(objects[obji].Get());
			if (emitterComponent == null)
			{
				APopcornFXEmitter* actor = Cast<APopcornFXEmitter>(objects[obji].Get());
				if (actor != null)
					emitterComponent = actor->PopcornFXEmitterComponent;
			}
			if (emitterComponent != null)
				outComponents.Add(emitterComponent);
		}
	}
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsEmitterComponent::GatherEffects(TArray<UPopcornFXEffect*> &outEffects)
{
	const TArray< TWeakObjectPtr<UObject> >		&objects = m_SelectedObjectsList;
	for (int32 obji = 0; obji < objects.Num(); ++obji)
	{
		if (objects[obji].IsValid())
		{
			UPopcornFXEmitterComponent	*emitterComponent = Cast<UPopcornFXEmitterComponent>(objects[obji].Get());
			if (emitterComponent == null)
			{
				APopcornFXEmitter		*actor = Cast<APopcornFXEmitter>(objects[obji].Get());
				if (actor != null)
					emitterComponent = actor->PopcornFXEmitterComponent;
			}
			if (emitterComponent != null && emitterComponent->Effect != null)
				outEffects.AddUnique(emitterComponent->Effect);
		}
	}
}

//----------------------------------------------------------------------------

FReply		FPopcornFXDetailsEmitterComponent::OnStartEmitter()
{
	TArray<UPopcornFXEmitterComponent*> emitters;
	GatherEmitters(emitters);
	for (int32 i = 0; i < emitters.Num(); ++i)
		if (emitters[i]->IsRegistered())
			emitters[i]->StartEmitter();
	return FReply::Handled();
}

//----------------------------------------------------------------------------

FReply		FPopcornFXDetailsEmitterComponent::OnStopEmitter()
{
	TArray<UPopcornFXEmitterComponent*> emitters;
	GatherEmitters(emitters);
	for (int32 i = 0; i < emitters.Num(); ++i)
		if (emitters[i]->IsRegistered())
			emitters[i]->StopEmitter();
	return FReply::Handled();
}

//----------------------------------------------------------------------------

FReply		FPopcornFXDetailsEmitterComponent::OnRestartEmitter()
{
	TArray<UPopcornFXEmitterComponent*> emitters;
	GatherEmitters(emitters);
	for (int32 i = 0; i < emitters.Num(); ++i)
		if (emitters[i]->IsRegistered())
			emitters[i]->RestartEmitter();
	return FReply::Handled();
}

//----------------------------------------------------------------------------

bool	FPopcornFXDetailsEmitterComponent::IsStartEnabled() const
{
	TArray<UPopcornFXEmitterComponent*> emitters;
	GatherEmitters(emitters);
	for (int32 i = 0; i < emitters.Num(); ++i)
		if (!emitters[i]->IsEmitterStarted())
			return true;
	return false;
}

//----------------------------------------------------------------------------

bool	FPopcornFXDetailsEmitterComponent::IsStopEnabled() const
{
	TArray<UPopcornFXEmitterComponent*> emitters;
	GatherEmitters(emitters);
	for (int32 i = 0; i < emitters.Num(); ++i)
		if (emitters[i]->IsEmitterEmitting())
			return true;
	return false;
}

//----------------------------------------------------------------------------

FReply	FPopcornFXDetailsEmitterComponent::OnReloadEffect()
{
	TArray<UPopcornFXEffect*>		effects;
	GatherEffects(effects);
	for (int32 i = 0; i < effects.Num(); ++i)
	{
		effects[i]->ReloadFile();
	}
	return FReply::Handled();
}

//----------------------------------------------------------------------------

FReply	FPopcornFXDetailsEmitterComponent::OnReimportEffect()
{
	TArray<UPopcornFXEffect*>		effects;
	GatherEffects(effects);
	for (int32 i = 0; i < effects.Num(); ++i)
	{
		FReimportManager::Instance()->Reimport(effects[i]);
	}
	return FReply::Handled();
}

//----------------------------------------------------------------------------

namespace
{
	static bool		IsValid(const TSharedPtr<IPropertyHandle> &ptr)
	{
		return ptr.IsValid() && ptr->IsValidHandle();
	}
}

void	FPopcornFXDetailsEmitterComponent::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	m_DetailLayout = &DetailLayout;
#if (ENGINE_MINOR_VERSION >= 18)
	m_SelectedObjectsList = m_DetailLayout->GetDetailsView()->GetSelectedObjects();
#else
	m_SelectedObjectsList = m_DetailLayout->GetDetailsView().GetSelectedObjects();
#endif	// (ENGINE_MINOR_VERSION >= 18)
	m_PropertyUtilities = DetailLayout.GetPropertyUtilities();

	const bool						bForAdvanced = false;
	IDetailCategoryBuilder			&fxPropertiesCategory = DetailLayout.EditCategory("PopcornFX Emitter");
	IDetailCategoryBuilder			&fxEditorCategory = fxPropertiesCategory;// DetailLayout.EditCategory("PopcornFX Editor Actions");

	// We hide the property from the editor
	//DetailLayout.HideProperty("Materials");
	//DetailLayout.HideProperty("AttributeList");

	m_EffectPty = DetailLayout.GetProperty(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEmitterComponent, Effect));

	//m_AttributesPty = DetailLayout.GetProperty(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEmitterComponent, AttributeList));
	if (//HH_VERIFY(IsValidHandle(m_AttributesPty)) &&
		HH_VERIFY(IsValidHandle(m_EffectPty)))
	{
		m_EffectPty->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsEmitterComponent::ReloadAttributes));
	}

	// Add some buttons
	fxEditorCategory.AddCustomRow(LOCTEXT("Emitter Actions", "Emitter Actions"), false)
	.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EmitterActions", "Emitter Actions"))
		]
	.ValueContent()
		.MinDesiredWidth(125.0f * 3.0f)
		.MaxDesiredWidth(125.0f * 4.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
				[SNew(SButton)
				.Text(LOCTEXT("Start", "Start"))
				.ToolTipText(LOCTEXT("Start_ToolTip", "Starts the emitter. Available if the emitter is not \"IsEmitterStarted\"."))
				.OnClicked(this, &FPopcornFXDetailsEmitterComponent::OnStartEmitter)
				.IsEnabled(this, &FPopcornFXDetailsEmitterComponent::IsStartEnabled)]
			+ SHorizontalBox::Slot()
				[SNew(SButton)
				.Text(LOCTEXT("Stop", "Stop"))
				.ToolTipText(LOCTEXT("Stop_ToolTip", "Stops the emitter. Available if the emitter is \"IsEmitterEmitting\"."))
				.OnClicked(this, &FPopcornFXDetailsEmitterComponent::OnStopEmitter)
				.IsEnabled(this, &FPopcornFXDetailsEmitterComponent::IsStopEnabled)
				]
			+ SHorizontalBox::Slot()
				[SNew(SButton)
				.Text(LOCTEXT("Restart", "Restart"))
				.ToolTipText(LOCTEXT("Restart_ToolTip", "Terminates then Starts the emitter."))
				.OnClicked(this, &FPopcornFXDetailsEmitterComponent::OnRestartEmitter)]
		]
	;
	// Add some buttons
	fxEditorCategory.AddCustomRow(LOCTEXT("Effect Actions", "Effect Actions"), false)
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EffectActions", "Effect Actions"))
		]
	.ValueContent()
		.MinDesiredWidth(125.0f * 3.0f)
		.MaxDesiredWidth(125.0f * 4.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[SNew(SButton)
			.Text(LOCTEXT("Reload", "Reload")).OnClicked(this, &FPopcornFXDetailsEmitterComponent::OnReloadEffect)
			.ToolTipText(LOCTEXT("Reload_ToolTip", "Reloads the PopcornFXEffect, will kill all particles related to this effect."))]
			+ SHorizontalBox::Slot()
			[SNew(SButton)
			.Text(LOCTEXT("Reimport", "Reimport")).OnClicked(this, &FPopcornFXDetailsEmitterComponent::OnReimportEffect)
			.ToolTipText(LOCTEXT("Reimport_ToolTip", "Reimports the PopcornFXEffect."))]
		]
	;

	//IDetailCategoryBuilder	&fxAttributeCategory = DetailLayout.EditCategory("PopcornFX Attributes");

#if 0
	TSharedPtr<IPropertyHandle>		catAttributeListProperty;
	catAttributeListProperty = m_DetailLayout->GetProperty(
		GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEmitterComponent, AttributeList));

	TSharedPtr<IPropertyHandle>		attributeListProperty;
	if (IsValid(catAttributeListProperty))
	{
		uint32			num = 0;
		catAttributeListProperty->GetNumChildren(num);
		if (num == 1)
			attributeListProperty = catAttributeListProperty->GetChildHandle(0);
	}

	TSharedPtr<IPropertyHandle>		attributesProperty;
	TSharedPtr<IPropertyHandle>		attribSamplersProperty;
	if (IsValid(attributeListProperty))
	{
		attributesProperty = attributeListProperty->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeList, m_Attributes));
		attribSamplersProperty = attributeListProperty->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeList, m_AttribSamplers));
	}
#elif 0
	const TArray< TWeakObjectPtr<UObject> >& objects = m_DetailLayout->GetDetailsView().GetSelectedObjects();
	if (objects.Num() == 1)
	{
		const uint32		obji = 0;
		UPopcornFXEmitterComponent* emitterComponent = Cast<UPopcornFXEmitterComponent>(objects[obji].Get());
		if (emitterComponent == null)
		{
			APopcornFXEmitter* actor = Cast<APopcornFXEmitter>(objects[obji].Get());
			if (actor != null)
				emitterComponent = actor->PopcornFXEmitterComponent;
		}

		if (emitterComponent != null &&
			emitterComponent->AttributeList != null &&
			emitterComponent->Effect != null &&
			emitterComponent->Effect->AttributeList() != null)
		{
			const UPopcornFXEffect						*effect = emitterComponent->Effect;
			PopcornFX::PCParticleAttributeList			attribList = effect->AttributeListIFP();
			const TArray<UPopcornFXAttribute*>			&attributes = emitterComponent->AttributeList->Attributes();
			const TArray<UPopcornFXAttribSampler*>		&samplers = emitterComponent->AttributeList->Samplers();

			for (int32 attri = 0; attri < attributes.Num(); ++attri)
			{
				if (!HH_VERIFY(attributes[attri] != null))
					continue;
				TSharedPtr<FPopcornFXSlateAttribBuilder>	attribBuilder = FPopcornFXSlateAttribBuilder::CreateNew(emitterComponent, attributes[attri], false, true);
				if (attribBuilder.IsValid())
				{
					attribBuilder->AddGroupTo(fxAttributeCategory);
					attribBuilder->Finish();
				}
			}

			TSharedPtr<IPropertyHandle>		attributeListProperty;

			attributeListProperty = m_DetailLayout->GetProperty(
				GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEmitterComponent, AttributeList));
				//UPopcornFXAttributeList::StaticClass(),
				//GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXEmitterComponent, AttributeList));

			TSharedPtr<IPropertyHandle>		samplerListProperty;
			if (HH_VERIFY(attributeListProperty.IsValid() && attributeListProperty->IsValidHandle()))
			{
				samplerListProperty = attributeListProperty->GetChildHandle(
					GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeList, m_AttribSamplers));
			}

			if (HH_VERIFY(samplerListProperty.IsValid() && samplerListProperty->IsValidHandle()))
			{
				samplerListProperty->MarkHiddenByCustomization();
				if (false)
				for (hh_u32 sampleri = 0; sampleri < attribList->UniqueSamplerList().Count(); sampleri++)
				{
					if (samplers[sampleri] == null ||
						!samplers[sampleri]->Valid())  // could happen if sampler type is Unknown
						continue;
					TSharedPtr<IPropertyHandle>		samplerProperty = samplerListProperty->GetChildHandle(sampleri);
					if (HH_VERIFY(samplerProperty.IsValid() && samplerProperty->IsValidHandle()))
					{
						//TSharedPtr<IPropertyHandle>		samplerPty = samplerProperty->GetChildHandle(0);
						//if (HH_VERIFY(samplerPty.IsValid() && samplerPty->IsValidHandle()))
						{
							//const FString		&name = samplers[sampleri]->GetSamplerName();
							//FName				fname = samplers[sampleri]->GetSamplerFName();
							//IDetailGroup		&attrGroup = fxAttribSamplerCategory.AddGroup(fname, name, false);
							//attrGroup.AddPropertyRow(samplerPty.ToSharedRef())
							samplerProperty->MarkHiddenByCustomization();
							fxAttribSamplerCategory.AddProperty(samplerProperty.ToSharedRef(), EPropertyLocation::Common)
								.DisplayName("KKuette");
							
							//attrGroup.AddPropertyRow(samplerPty.ToSharedRef());
							//attrGroup.HeaderRow()
							//	.NameContent()
							//	[
							//		SNew(SHorizontalBox)
							//	]
							//.ValueContent()
							//	[
							//		samplerPty->CreatePropertyValueWidget()
							//	];
						}
					}

					//TSharedPtr<FPopcornFXSlateAttribSamplerBuilder>	attribSamplerBuilder = FPopcornFXSlateAttribSamplerBuilder::CreateNew(emitterComponent, samplers[sampleri], false, true);
					//if (attribSamplerBuilder.IsValid())
					//{
					//	attribSamplerBuilder->AddGroupTo(fxAttribSamplerCategory);
					//	attribSamplerBuilder->Finish();
					//}
				}
			}
		}
	}
#endif
}

//----------------------------------------------------------------------------

TOptional<float>	FPopcornFXDetailsEmitterComponent::OnGetValue(float field) const
{
	return field;
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsEmitterComponent::OnSetValue(float newValue, ETextCommit::Type commitInfo, float *field)
{
	*field = newValue;
}

//----------------------------------------------------------------------------

int32	FPopcornFXDetailsEmitterComponent::GetVectorDimension(int32 field) const
{
	return field;
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsEmitterComponent::ReloadAttributes()
{
	if (m_PropertyUtilities.IsValid())
		m_DetailLayout->ForceRefreshDetails();
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
