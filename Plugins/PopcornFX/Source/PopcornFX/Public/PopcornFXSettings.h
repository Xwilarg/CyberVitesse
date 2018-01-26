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

#include "PopcornFXPublic.h"

#include "Engine/EngineBaseTypes.h"

#include "BackwardCompat/PopcornFXDefaultMaterialsSettings.h"

#include "PopcornFXSettings.generated.h"

class UMaterialInterface;

/** Where to Simulate/Update PopcornFX Particles */
UENUM()
namespace EPopcornFXConfigPreferedSimulation
{
	enum	Type
	{
		/** Simulate Particles on CPU. */ 
		CPU UMETA(DisplayName="CPU only"),

		/** EXPERIMENTAL. Does not work, yet. */
		GPU UMETA(DisplayName="GPU when possible"),
	};
}

/** Where to Billboard PopcornFX Particles */
UENUM()
namespace EPopcornFXConfigPreferedBillboarding
{
	enum	Type
	{
		/** All CPU particles billboarded on CPU, keep GPU ones on GPU */
		MaxCPU UMETA(DisplayName="Maximum CPU"),
		/** Same as MaxCPU, but if some CPU and GPU particles can be batched together, Batch them and Billboard on GPU. */
		Auto UMETA(DisplayName="Auto"),
		/** Try to always billboard on GPU, even CPU particles (but, some billboarding features can still have to run on CPU) */
		MaxGPU UMETA(DisplayName="Maximum GPU"),
	};
}

/** How to Sort PopcornFX Particles Draw Calls */
UENUM()
namespace EPopcornFXDrawCallSortMethod
{
	enum	Type
	{
		/** Do NOT sort draw calls.
		*/
		None,

		/** Sort Draw Calls by their Bounding Boxes.
		*
		* - Pro: no performance overhead
		* - Con: still have sorting issues
		*/
		PerDrawCalls,

		/** Experimental ! work in progress !
		* Slice Draw Calls, then sort those slices.
		*
		* - Pro: fixes more sorting issues
		* - Con: performance overhead: sorts ALL particles (even additive ones)
		* - Con: performance overhead: increase the number of draw calls
		*/
		PerSlicedDrawCalls,

		/** Experimental ! work in progress !
		* Enables localazied Particle Pages.
		* Then sort draw call per pages.
		*
		* - Pro: fixes more sorting issues
		* - Con: performance overhead: sorts ALL particles (even additive ones)
		* - Con: performance overhead: increase the number of draw calls
		*/
		PerPageDrawCalls,

	};
}

/** How to early Cull PopcornFX Particles for Rendering. */
UENUM()
namespace EPopcornFXRenderCullMethod
{
	enum	Type
	{
		/** Do NOT Cull Particle to Render. */
		None,

		/** Can cull entire Particle Draw Requests if BBox are out of Views's frustrums. */
		DrawRequestsAgainstFrustrum,

		/** Cull entire Particle Pages. if BBox are out of Views's frustrums. */
		// CullPagesAgainstFrustrum,
	};
}

/** PopcornFX Localized page mode */
UENUM()
namespace EPopcornFXLocalizedPagesMode
{
	enum	Type
	{
		/** Disabled */
		Disable,

		/** Enabled, Default will be off, needs per-layer explicit enabling. */
		EnableDefaultsToOff,

		/** Enabled, Default will be on, unless per-layer explicit disabling. */
		EnableDefaultsToOn,
	};
}

/** Object Types to include/exclude in PopcornFX collision queries. */
USTRUCT()
struct FPopcornFXCollisionFilter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	int32				ObjectTypesToQuery;

	FPopcornFXCollisionFilter();
};

/** Overridable PopcornFX Simulation Settings. */
USTRUCT(BlueprintType)
struct FPopcornFXSimulationSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_PreferedSimulation:1;

	/** Where to Simulate/Update PopcornFX Particles.
	* (Shown only if GPU build)
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_PreferedSimulation"))
	TEnumAsByte<EPopcornFXConfigPreferedSimulation::Type> PreferedSimulation; // /!\ Hidden if no PK_HAS_GPU

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_DecalsMaxLifeSpan:1;

	/** Maximum life span for decals spawned by PopconFX Particles. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_DecalsMaxLifeSpan"))
	float DecalsMaxLifeSpan;

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_bEnablePhysicalMaterials:1;

	/** Particle Collisions takes Physical Materials into account. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_bEnablePhysicalMaterials"))
	uint32 bEnablePhysicalMaterials : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_bEnableEventListeners:1;

	/** Can broadcasted particle events trigger delegates */
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_bEnableEventListeners"))
	uint32 bEnableEventListeners : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_DefaultCollisionFilter:1;

	/** Default Collision Filter for PopcornFX Particle Collision queries. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_DefaultCollisionFilter"))
	FPopcornFXCollisionFilter DefaultCollisionFilter;

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_CollisionFilters:1;

	/** Collision Filters indexed by CParticleEvolverPhysics's FilterIDs. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_CollisionFilters"))
	TArray<FPopcornFXCollisionFilter> CollisionFilters;

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_LocalizedPagesMode : 1;

	/** Experimental ! work in progress ! Will enable Particle Page localization. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_LocalizedPagesMode"))
	TEnumAsByte<EPopcornFXLocalizedPagesMode::Type> LocalizedPagesMode;

	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings")
	uint32 bOverride_SceneUpdateTickGroup : 1;

	/** Tick Group for the PopcornFX Scene Update.
	(Change this at your own risk.)
	(SceneUpdateTickGroup is taken into account at Component OnRegister.)
	- You should benchmark !
	- Scene with Particles with collision/intersect could have bad performance if "During Physics".
	- But Scene with Particles **without** collision/intersect could take advantage to run in parallel in "During Physics".
	- Ticking Later ("Post Update Work") can fix particle lag, scene view lag etc, but might affect performance: you should benchmark !
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Simulation Settings", meta=(EditCondition="bOverride_SceneUpdateTickGroup"))
	TEnumAsByte<ETickingGroup> SceneUpdateTickGroup;

	FPopcornFXSimulationSettings();

	void		ResolveSettingsTo(FPopcornFXSimulationSettings &outSettings) const;
};


/** Overridable PopcornFX Render Settings */
USTRUCT(BlueprintType)
struct FPopcornFXRenderSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_PreferedBillboarding:1;

	/** Where to Billboard PopcornFX Particles.
	* (Shown only if GPU build)
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_PreferedBillboarding"))
	TEnumAsByte<EPopcornFXConfigPreferedBillboarding::Type> PreferedBillboarding;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_DrawCallSortMethod:1;

	/** How to Sort batches of Particles for Renderering. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_DrawCallSortMethod"))
	TEnumAsByte<EPopcornFXDrawCallSortMethod::Type> DrawCallSortMethod;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_RenderCullMethod:1;

	/** How to early Cull particles for Renderering. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_RenderCullMethod"))
	TEnumAsByte<EPopcornFXRenderCullMethod::Type> RenderCullMethod;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_FarCullDistance:1;

	/** Cull draw batches farther than FarCullDistance. (0 means disabled).
	* If RenderCullMethod is None or DrawRequestsAgainstFrustrum, culls Draw Requests.
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_FarCullDistance"))
	float FarCullDistance;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_bEnableEarlyFrameRelease:1;

	/** Tries to early release RenderThread's particle data copy.
	* Can reduce a bit Tick time and memory usage.
	* But could have shadow flickering issues, and other unforseen side effects.
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_bEnableEarlyFrameRelease"))
	uint32 bEnableEarlyFrameRelease : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_bEnableBillboardShadows:1;

	/** Enable Shadow Casting for Lit or Solid Billboard Particles. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_bEnableBillboardShadows"))
	uint32 bEnableBillboardShadows : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_bEnableRibbonShadows:1;

	/** Enable Shadow Casting for Lit or Solid Ribbon Particles. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_bEnableRibbonShadows"))
	uint32 bEnableRibbonShadows : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_bEnableMeshShadows:1;

	/** Enable Shadow Casting for Lit or Solid Mesh Particles. */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_bEnableMeshShadows"))
	uint32 bEnableMeshShadows : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_bForceLightsLitTranslucent:1;

	/** Force Particle Lights to Lit Translucent Geometry (including other Lit Particles) */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_bForceLightsLitTranslucent"))
	uint32 bForceLightsLitTranslucent : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_MeshParticlesLODBias:1;

	/** LOD Bias for Particle Renderer Mesh */
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_MeshParticlesLODBias"))
	int32 MeshParticlesLODBias;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_Shadows_ExtendCullBBoxes:1;

	/** Extend Culling Bounding Boxes for all shadows casting Particles.
	* (valid if Culling is enabled and Enable*Shadows is/are enabled)
	*
	* Can fix SOME missing shadows.
	* Can be seen as a "Cast Shadow Radius":
	* Shadow casting particles are culled if out of view and farther than Shadows_ExtendCullBBoxes cm.
	*
	* The higher the Extent is, the more particles will be Billboarded, it can impact performances.
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_Shadows_ExtendCullBBoxes"))
	float Shadows_ExtendCullBBoxes;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_bShadows_DisableCull :1;

	/** Force disable Culling at Render for all shadow casting Particles.
	* (valid if Culling is enabled and Enable*Shadows is/are enabled)
	* Will fix ALL missing shadows.
	* If true, ALL shadow casting particles will be Billboarded, performances will be impacted.
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(EditCondition="bOverride_bShadows_DisableCull"))
	uint32 bShadows_DisableCull : 1;

	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings")
	uint32 bOverride_SlicedDraw_PreferedSliceDepth : 1;

	/**
	*/
	UPROPERTY(EditAnywhere, Category="PopcornFX Render Settings", meta=(ClampMin="1", ClampMax="100000", UIMin="1", UIMax="100000", EditCondition="bOverride_SlicedDraw_PreferedSliceDepth"))
	float SlicedDraw_PreferedSliceDepth;

	FPopcornFXRenderSettings();

	void		ResolveSettingsTo(FPopcornFXRenderSettings &outSettings) const;
};

/**
* PopcornFX Plugin Config
*/
UCLASS(MinimalAPI, Config=Engine, DefaultConfig)
class UPopcornFXSettings : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	/** Needs Restart Editor !
	* The Content root directory of the PopcornFX Pack.
	* All Assets used by PopcornFX must be under this PackMountPoint.
	*/
	UPROPERTY(Config, EditAnywhere, Category=Pack)
	FString				PackMountPoint;

	/** PopcornFX Particles to UE4 Global Scale. ie: GlobalScale = 100 means 1 unit in PopcornFX will become 100 unit in Unreal Engine */
	UPROPERTY(Config, EditAnywhere, Category=Pack)
	float				GlobalScale;

	/** PopcornFX Simulation Settings. */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX Simulation Settings", meta=(FullyExpand="true"))
	FPopcornFXSimulationSettings		SimulationSettings;

	/** PopcornFX Render Settings. */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX Render Settings", meta=(FullyExpand="true"))
	FPopcornFXRenderSettings			RenderSettings;

	/** List of default materials for the project. */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX Materials")
	FPopcornFXDefaultMaterialsSettings	DefaultMaterials;

	/** HUDs: Ratio of the Screen used to display HUDs */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX HUD", meta=(ClampMin="0", ClampMax="1", UIMin="0", UIMax="1"))
	float						HUD_DisplayScreenRatio;

	/** HUDs: Hide stat nodes below this percentage */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX HUD", meta=(ClampMin="0", ClampMax="100", UIMin="0", UIMax="100"))
	int32						HUD_HideNodesBelowPercent;

	/** ProfilerHUD: Number of BaseObjects to display per PopcornFX File to display */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX HUD", meta=(ClampMin="0", ClampMax="100", UIMin="0", UIMax="100"))
	int32						ProfilerHUD_BaseObjectsPerFile;

	/** ProfilerHUD: Number of sub-descriptors per BaseObject to display */
	UPROPERTY(Config, EditAnywhere, Category="PopcornFX HUD", meta=(ClampMin="0", ClampMax="100", UIMin="0", UIMax="100"))
	int32						ProfilerHUD_SubDescriptorsPerObject;

	// overrides
	virtual void		Serialize(FArchive& Ar) override;

#if WITH_EDITOR
	virtual void		PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent) override;
#endif

	void				UpdateSourcePack();
	UMaterialInterface	*GetConfigDefaultMaterial(uint32 ePopcornFXMaterialType) const;
};
