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
#include "PopcornFXDetailsSettings.h"

#include "PopcornFXSettings.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"

//----------------------------------------------------------------------------

FPopcornFXDetailsSettings::FPopcornFXDetailsSettings()
{

}

//----------------------------------------------------------------------------

TSharedRef<IDetailCustomization>	FPopcornFXDetailsSettings::MakeInstance()
{
	return MakeShareable(new FPopcornFXDetailsSettings);
}

//----------------------------------------------------------------------------

void	FPopcornFXDetailsSettings::CustomizeDetails(IDetailLayoutBuilder &detailLayout)
{
#if (PK_HAS_GPU == 0)
	detailLayout.HideProperty("SimulationSettings.bOverride_PreferedSimulation");
	detailLayout.HideProperty("SimulationSettings.PreferedSimulation");
	detailLayout.HideProperty("RenderSettings.bOverride_PreferedBillboarding");
	detailLayout.HideProperty("RenderSettings.PreferedBillboarding");
#endif // (PK_HAS_GPU == 0)
}

//----------------------------------------------------------------------------

#endif // WITH_EDITOR
