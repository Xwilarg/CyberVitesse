//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXSettings.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXCustomVersion.h"
#include "Assets/PopcornFXFile.h"
#include "Assets/PopcornFXRendererMaterial.h"

#if WITH_EDITOR
#	include "XmlFile.h"
#endif
#include "Engine/EngineTypes.h"
#include "Materials/MaterialInterface.h"
#include "CollisionQueryParams.h"

#define LOCTEXT_NAMESPACE "PopcornFXSettings"

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXSettings, Log, All);

//----------------------------------------------------------------------------

#define DEFAULT_MATERIAL_PATH(__name)		"/PopcornFX/Materials/M_PK_Default_" #__name ".M_PK_Default_" #__name

static const char		* const kPluginsDefaultMaterials[] = {
#define	X_POPCORNFX_MATERIAL_TYPE(__name)	DEFAULT_MATERIAL_PATH(__name),
	EXEC_X_POPCORNFX_MATERIAL_TYPE()
#undef	X_POPCORNFX_MATERIAL_TYPE
};

static const char		* const kMaterialTypeNames[] = {
#define	X_POPCORNFX_MATERIAL_TYPE(__name)	"Material_" #__name,
	EXEC_X_POPCORNFX_MATERIAL_TYPE()
#undef	X_POPCORNFX_MATERIAL_TYPE
};

UPopcornFXSettings::UPopcornFXSettings(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, PackMountPoint("/Game/")
	, GlobalScale(100.0f)
	, HUD_DisplayScreenRatio(0.1f)
	, HUD_HideNodesBelowPercent(5)
	, ProfilerHUD_BaseObjectsPerFile(3)
	, ProfilerHUD_SubDescriptorsPerObject(0)
{
	if (!IsRunningCommandlet())
	{
#if (ENGINE_MINOR_VERSION >= 18)
#	define	X_POPCORNFX_MATERIAL_TYPE(__name)	DefaultMaterials.Material_ ## __name = FSoftObjectPath(DEFAULT_MATERIAL_PATH(__name));
			EXEC_X_POPCORNFX_MATERIAL_TYPE()
#	undef	X_POPCORNFX_MATERIAL_TYPE
#else
#	define	X_POPCORNFX_MATERIAL_TYPE(__name)	DefaultMaterials.Material_ ## __name = FStringAssetReference(DEFAULT_MATERIAL_PATH(__name));
			EXEC_X_POPCORNFX_MATERIAL_TYPE()
#	undef	X_POPCORNFX_MATERIAL_TYPE
#endif	// (ENGINE_MINOR_VERSION >= 18)
	}
}

//----------------------------------------------------------------------------

#if WITH_EDITOR
void	UPopcornFXSettings::PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

	FPopcornFXPlugin::Get().HandleSettingsModified();
}
#endif // WITH_EDITOR

//----------------------------------------------------------------------------

void	UPopcornFXSettings::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FPopcornFXCustomVersion::GUID);
}

//----------------------------------------------------------------------------

UMaterialInterface	*UPopcornFXSettings::GetConfigDefaultMaterial(uint32 ePopcornFXMaterialType) const
{
	if (ePopcornFXMaterialType >= HH_ARRAY_COUNT(kPluginsDefaultMaterials))
	{
		HH_ASSERT_NOT_REACHED();
		ePopcornFXMaterialType = 0; // fall back to bb additive
	}
	const EPopcornFXMaterialType::Type	materialType = (EPopcornFXMaterialType::Type)ePopcornFXMaterialType;

	UObject			*obj = null;
	switch (materialType)
	{
#define	X_POPCORNFX_MATERIAL_TYPE(__name)	\
	case	EPopcornFXMaterialType:: __name: obj = DefaultMaterials.Material_ ## __name .TryLoad(); break;
		EXEC_X_POPCORNFX_MATERIAL_TYPE()
#undef	X_POPCORNFX_MATERIAL_TYPE
	default:
		HH_ASSERT_NOT_REACHED();
		return null;
	}
	UMaterialInterface		*mat = null;
	if (obj != null)
	{
		mat = Cast<UMaterialInterface>(obj);
		if (!HH_VERIFY(mat != null))
			UE_LOG(LogPopcornFXSettings, Warning, TEXT("PopcornFX Config %s is not a UMaterialInterface !"), kMaterialTypeNames[materialType]);
	}

	if (mat == null)
	{
		UE_LOG(LogPopcornFXSettings, Warning, TEXT("PopcornFX Config %s is invalid, falling back on PopcornFX Plugin's default one %s"), *FString(kMaterialTypeNames[materialType]), *FString(kPluginsDefaultMaterials[materialType]));
		mat = ::LoadObject<UMaterialInterface>(null, ANSI_TO_TCHAR(kPluginsDefaultMaterials[materialType]));
		// Those are 
		HH_ASSERT(mat != null);
	}
	return mat;
}

//----------------------------------------------------------------------------
//
// Settings
//
//----------------------------------------------------------------------------

FPopcornFXSimulationSettings::FPopcornFXSimulationSettings()
	: bOverride_PreferedSimulation(0)
	, PreferedSimulation(EPopcornFXConfigPreferedSimulation::CPU)
	, bOverride_DecalsMaxLifeSpan(0)
	, DecalsMaxLifeSpan(10.f)
	, bOverride_bEnablePhysicalMaterials(0)
	, bEnablePhysicalMaterials(true)
	, bOverride_bEnableEventListeners(0)
	, bEnableEventListeners(false)
	, bOverride_DefaultCollisionFilter(0)
	, DefaultCollisionFilter()
	, bOverride_CollisionFilters(0)
	, CollisionFilters()
	, bOverride_LocalizedPagesMode(0)
	, LocalizedPagesMode(EPopcornFXLocalizedPagesMode::Disable)
	, bOverride_SceneUpdateTickGroup(0)
	, SceneUpdateTickGroup(TG_PostPhysics)
{
}


#define RESOLVE_SETTING(__name) do {								\
		if (bOverride_ ## __name != 0)								\
			outValues.__name = __name;								\
		else if (configValues.bOverride_ ## __name != 0)			\
			outValues.__name = configValues.__name;					\
		else														\
			outValues.__name = defaultValues.__name;				\
		outValues.bOverride_ ## __name = 1; /* just in case */		\
	} while (0)

void	FPopcornFXSimulationSettings::ResolveSettingsTo(FPopcornFXSimulationSettings &outValues) const
{
	static const FPopcornFXSimulationSettings		defaultValues;
	check(FPopcornFXPlugin::Get().Settings() != null);
	const FPopcornFXSimulationSettings				&configValues = FPopcornFXPlugin::Get().Settings()->SimulationSettings;

#if PK_HAS_GPU
	RESOLVE_SETTING(PreferedSimulation);
#endif
	RESOLVE_SETTING(DecalsMaxLifeSpan);
	RESOLVE_SETTING(bEnablePhysicalMaterials);
	RESOLVE_SETTING(bEnableEventListeners);
	RESOLVE_SETTING(DefaultCollisionFilter);
	RESOLVE_SETTING(CollisionFilters);
	RESOLVE_SETTING(LocalizedPagesMode);
	RESOLVE_SETTING(SceneUpdateTickGroup);
}

FPopcornFXRenderSettings::FPopcornFXRenderSettings()
	: bOverride_PreferedBillboarding(0)
	, PreferedBillboarding(EPopcornFXConfigPreferedBillboarding::Auto)
	, bOverride_DrawCallSortMethod(0)
	, DrawCallSortMethod(EPopcornFXDrawCallSortMethod::PerDrawCalls)
	, bOverride_RenderCullMethod(0)
	, RenderCullMethod(EPopcornFXRenderCullMethod::DrawRequestsAgainstFrustrum)
	, bOverride_FarCullDistance(0)
	, FarCullDistance(1000000.0f) // 1 km
	, bOverride_bEnableEarlyFrameRelease(0)
	, bEnableEarlyFrameRelease(true)
	, bOverride_bEnableBillboardShadows(0)
	, bEnableBillboardShadows(true)
	, bOverride_bEnableRibbonShadows(0)
	, bEnableRibbonShadows(true)
	, bOverride_bEnableMeshShadows(0)
	, bEnableMeshShadows(true)
	, bOverride_bForceLightsLitTranslucent(0)
	, bForceLightsLitTranslucent(false)
	, bOverride_MeshParticlesLODBias(0)
	, MeshParticlesLODBias(0)
	, bOverride_Shadows_ExtendCullBBoxes(0)
	, Shadows_ExtendCullBBoxes(10000.0f) // 10m seems fine
	, bOverride_bShadows_DisableCull(0)
	, bShadows_DisableCull(0)
	, bOverride_SlicedDraw_PreferedSliceDepth(0)
	, SlicedDraw_PreferedSliceDepth(500.0f) // 5m
{
}

void	FPopcornFXRenderSettings::ResolveSettingsTo(FPopcornFXRenderSettings &outValues) const
{
	static const FPopcornFXRenderSettings		defaultValues;
	check(FPopcornFXPlugin::Get().Settings() != null);
	const FPopcornFXRenderSettings				&configValues = FPopcornFXPlugin::Get().Settings()->RenderSettings;

#if PK_HAS_GPU
	RESOLVE_SETTING(PreferedBillboarding);
#endif
	RESOLVE_SETTING(DrawCallSortMethod);
	RESOLVE_SETTING(RenderCullMethod);
	RESOLVE_SETTING(FarCullDistance);
	RESOLVE_SETTING(bEnableEarlyFrameRelease);
	RESOLVE_SETTING(bEnableBillboardShadows);
	RESOLVE_SETTING(bEnableRibbonShadows);
	RESOLVE_SETTING(bEnableMeshShadows);
	RESOLVE_SETTING(bForceLightsLitTranslucent);
	RESOLVE_SETTING(MeshParticlesLODBias);
	RESOLVE_SETTING(Shadows_ExtendCullBBoxes);
	RESOLVE_SETTING(bShadows_DisableCull);
	RESOLVE_SETTING(SlicedDraw_PreferedSliceDepth);
}

#undef RESOLVE_SETTING

//----------------------------------------------------------------------------
//
// FPopcornFXCollisionFilter
//
//----------------------------------------------------------------------------

FPopcornFXCollisionFilter::FPopcornFXCollisionFilter()
	: ObjectTypesToQuery(
	ECC_TO_BITFIELD(ECC_WorldStatic) |
	ECC_TO_BITFIELD(ECC_WorldDynamic) |
	ECC_TO_BITFIELD(ECC_Pawn) |
	ECC_TO_BITFIELD(ECC_PhysicsBody) |
	ECC_TO_BITFIELD(ECC_Vehicle) |
	ECC_TO_BITFIELD(ECC_Destructible) |
	0
	)
{
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
