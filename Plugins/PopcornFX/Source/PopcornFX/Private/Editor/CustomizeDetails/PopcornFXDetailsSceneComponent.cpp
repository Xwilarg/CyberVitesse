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
#include "PopcornFXDetailsSceneComponent.h"

#include "PopcornFXSceneComponent.h"
#include "PopcornFXSceneActor.h"
#include "Internal/ParticleScene.h"
#include "PopcornFXSDK.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailsView.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXDetailsSceneComponent, Log, All);

#define LOCTEXT_NAMESPACE "PopcornFXDetailsSceneComponent"

//----------------------------------------------------------------------------

FPopcornFXDetailsSceneComponent::FPopcornFXDetailsSceneComponent()
:	m_DetailLayout(null)
{

}

//----------------------------------------------------------------------------

TSharedRef<IDetailCustomization>	FPopcornFXDetailsSceneComponent::MakeInstance()
{
	return MakeShareable(new FPopcornFXDetailsSceneComponent);
}

//----------------------------------------------------------------------------

FReply		FPopcornFXDetailsSceneComponent::OnClear()
{
	if (!HH_VERIFY(m_DetailLayout != null))
		return FReply::Unhandled();

#if (ENGINE_MINOR_VERSION >= 18)
	const TArray< TWeakObjectPtr<UObject> > &objects = m_DetailLayout->GetDetailsView()->GetSelectedObjects();
#else
	const TArray< TWeakObjectPtr<UObject> > &objects = m_DetailLayout->GetDetailsView().GetSelectedObjects();
#endif	// (ENGINE_MINOR_VERSION >= 18)

	for (int32 obji = 0; obji < objects.Num(); ++obji)
	{
		if (objects[obji].IsValid())
		{
			UPopcornFXSceneComponent* sceneComponent = Cast<UPopcornFXSceneComponent>(objects[obji].Get());
			if (sceneComponent == null)
			{
				APopcornFXSceneActor* actor = Cast<APopcornFXSceneActor>(objects[obji].Get());
				if (actor != null)
					sceneComponent = actor->PopcornFXSceneComponent;
			}
			if (sceneComponent != null && sceneComponent->ParticleScene() != null)
				sceneComponent->ParticleScene()->Clear();
		}
	}
	return FReply::Handled();
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsSceneComponent::RebuildDetails()
{
	if (!HH_VERIFY(m_DetailLayout != null))
		return;
	m_DetailLayout->ForceRefreshDetails();
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsSceneComponent::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	m_DetailLayout = &DetailLayout;

	//const TArray< TWeakObjectPtr<UObject> > &objects = m_DetailLayout->GetDetailsView().GetSelectedObjects();

	//DetailLayout.HideProperty("AttributeList");

	IDetailCategoryBuilder	&sceneCategory = DetailLayout.EditCategory("PopcornFX Scene");
	sceneCategory.AddCustomRow(LOCTEXT("Editor Actions", "Editor Actions"), false)
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("SceneActions", "Scene Actions"))
		]
	.ValueContent()
		.MinDesiredWidth(125.0f * 3.0f)
		.MaxDesiredWidth(125.0f * 4.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[SNew(SButton).Text(LOCTEXT("Clear", "Clear")).OnClicked(this, &FPopcornFXDetailsSceneComponent::OnClear)]
		];

	{
		TSharedRef<IPropertyHandle>	simulationSettings = DetailLayout.GetProperty("SimulationSettingsOverride");
		if (!HH_VERIFY(simulationSettings->IsValidHandle()))
			return;
		TSharedPtr<IPropertyHandle>	enableEventListeners = simulationSettings->GetChildHandle(GET_MEMBER_NAME_STRING_CHECKED(FPopcornFXSimulationSettings, bEnableEventListeners));
		if (!HH_VERIFY(enableEventListeners.IsValid() && enableEventListeners->IsValidHandle()))
			return;

		bool	value;
		enableEventListeners->GetValue(value);
		if (!value)
			DetailLayout.HideProperty("FieldListeners");
		enableEventListeners->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsSceneComponent::RebuildDetails));
	}
	{
		TSharedRef<IPropertyHandle>	bboxMode = DetailLayout.GetProperty("BoundingBoxMode");
		if (!HH_VERIFY(bboxMode->IsValidHandle()))
			return;
		uint8	value;

		bboxMode->GetValue(value);
		switch (value)
		{
		case	EPopcornFXSceneBBMode::Dynamic:
			DetailLayout.HideProperty("FixedRelativeBoundingBox");
			break;
		default:
			break;
		}
		bboxMode->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPopcornFXDetailsSceneComponent::RebuildDetails));
	}

#if (PK_HAS_GPU == 0)
	{
		DetailLayout.HideProperty("SimulationSettingsOverride.bOverride_PreferedSimulation");
		DetailLayout.HideProperty("SimulationSettingsOverride.PreferedSimulation");
		DetailLayout.HideProperty("RenderSettingsOverride.bOverride_PreferedBillboarding");
		DetailLayout.HideProperty("RenderSettingsOverride.PreferedBillboarding");
	}
#endif // (PK_HAS_GPU == 0)
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
