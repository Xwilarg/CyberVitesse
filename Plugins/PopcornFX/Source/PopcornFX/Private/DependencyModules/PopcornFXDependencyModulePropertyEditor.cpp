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
#include "PopcornFXDependencyModulePropertyEditor.h"

#include "Editor/CustomizeDetails/PopcornFXDetailsSceneComponent.h"
#include "Editor/CustomizeDetails/PopcornFXDetailsEmitterComponent.h"
#include "Editor/CustomizeDetails/PopcornFXDetailsAttributeList.h"
#include "Editor/CustomizeDetails/PopcornFXDetailsAttributeSamplerShape.h"
#include "Editor/CustomizeDetails/PopcornFXDetailsAttributeSamplerCurve.h"
#include "Editor/CustomizeDetails/PopcornFXDetailsAttributeSamplerActor.h"
#include "Editor/CustomizeDetails/PopcornFXDetailsSettings.h"

#include "Editor/PropertyCustomization/PopcornFXCustomizationAssetDep.h"
#include "Editor/PropertyCustomization/PopcornFXCustomizationRendererMaterial.h"
#include "Editor/PropertyCustomization/PopcornFXCustomizationSubRendererMaterial.h"
#include "Editor/PropertyCustomization/PopcornFXCustomizationCollisionFilter.h"

#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"

#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------

FPopcornFXDependencyModulePropertyEditor::FPopcornFXDependencyModulePropertyEditor()
{

}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModulePropertyEditor::Load()
{
	if (m_Loaded)
		return;

	if (!HH_VERIFY(FModuleManager::Get().IsModuleLoaded("PropertyEditor")))
		return;
	FPropertyEditorModule	&propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Register the custom editor
	propertyModule.RegisterCustomClassLayout("PopcornFXEmitterComponent", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsEmitterComponent::MakeInstance));
	propertyModule.RegisterCustomClassLayout("PopcornFXSceneComponent", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsSceneComponent::MakeInstance));
	propertyModule.RegisterCustomClassLayout("PopcornFXAttributeList", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsAttributeList::MakeInstance));
	propertyModule.RegisterCustomClassLayout("PopcornFXAttributeSamplerShape", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsAttributeSamplerShape::MakeInstance));
	propertyModule.RegisterCustomClassLayout("PopcornFXAttributeSamplerCurve", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsAttributeSamplerCurve::MakeInstance));
	propertyModule.RegisterCustomClassLayout("PopcornFXAttributeSamplerActor", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsAttributeSamplerActor::MakeInstance));
	propertyModule.RegisterCustomClassLayout("PopcornFXSettings", FOnGetDetailCustomizationInstance::CreateStatic(&FPopcornFXDetailsSettings::MakeInstance));

	propertyModule.RegisterCustomPropertyTypeLayout("PopcornFXAssetDep", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPopcornFXCustomizationAssetDep::MakeInstance));
	propertyModule.RegisterCustomPropertyTypeLayout("PopcornFXRendererMaterial", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPopcornFXCustomizationRendererMaterial::MakeInstance));
	propertyModule.RegisterCustomPropertyTypeLayout("PopcornFXSubRendererMaterial", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPopcornFXCustomizationSubRendererMaterial::MakeInstance));
	propertyModule.RegisterCustomPropertyTypeLayout("PopcornFXCollisionFilter", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPopcornFXCustomizationCollisionFilter::MakeInstance));

	m_Loaded = true;
}

//----------------------------------------------------------------------------

void	FPopcornFXDependencyModulePropertyEditor::Unload()
{
	if (!m_Loaded)
		return;
	m_Loaded = false;

	if (!/*HH_VERIFY*/(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))) // @FIXME: we should be called when the plugin is still loaded
		return;

	FPropertyEditorModule&	propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Unregister the custom editor
	propertyModule.UnregisterCustomClassLayout("PopcornFXEmitterComponent");
	propertyModule.UnregisterCustomClassLayout("PopcornFXSceneComponent");
	propertyModule.UnregisterCustomClassLayout("PopcornFXAttributeList");
	propertyModule.UnregisterCustomClassLayout("PopcornFXAttributeSamplerShape");
	propertyModule.UnregisterCustomClassLayout("PopcornFXAttributeSamplerCurve");
	propertyModule.UnregisterCustomClassLayout("PopcornFXAttributeSamplerActor");
	// propertyModule.UnregisterCustomClassLayout("PopcornFXAttribSamplerShape");

	propertyModule.UnregisterCustomPropertyTypeLayout("PopcornFXAssetDep");
	propertyModule.UnregisterCustomPropertyTypeLayout("PopcornFXRendererMaterial");
	propertyModule.UnregisterCustomPropertyTypeLayout("PopcornFXSubRendererMaterial");
}

//----------------------------------------------------------------------------

#endif // WITH_EDITOR
