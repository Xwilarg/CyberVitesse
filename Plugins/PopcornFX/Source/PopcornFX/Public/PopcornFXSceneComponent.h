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
#include "PopcornFXSettings.h"
#include "PopcornFXTypes.h"

#include "Components/PrimitiveComponent.h"

#include "PopcornFXSceneComponent.generated.h"

class	CParticleScene;
class	FPopcornFXSceneProxy;

UENUM()
namespace EPopcornFXSceneBBMode
{
	enum	Type
	{
		Dynamic,
		DynamicPlusFixed,
		Fixed,
	};
}

UENUM()
namespace EPopcornFXHeavyDebugMode
{
	enum	Type
	{
		/** No debug */
		None = 0,
		/** Debug Particles as points, colored by mediums */
		ParticlePoints,
		/** Debug Mediums BBox */
		MediumsBBox,
		/** Debug Particle Pages bounding boxes, colored by mediums */
		PagesBBox,
	};
}

USTRUCT()
struct	FPopcornFXFieldListener
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, Category="PopcornFX FieldListener")
	FName	FieldName;

	UPROPERTY(EditAnywhere, Category="PopcornFX FieldListener")
	TEnumAsByte<EPopcornFXParticleFieldType::Type>	FieldType;
};

DECLARE_DYNAMIC_DELEGATE(FPopcornFXRaiseEventSignature);

/** Handles all the PopcornFX Particle Simulation and Rendering context.
* (All PopcornFX Emitters will actually ask a PopcornFXSceneComponent to spawn Particles)
*/
UCLASS(HideCategories=(Input, Collision, Replication, Materials), EditInlineNew, ClassGroup=PopcornFX)
class POPCORNFX_API UPopcornFXSceneComponent : public UPrimitiveComponent
{
	GENERATED_UCLASS_BODY()

	/** The SceneName used to indentify this PopcornFXScene
	* PopcornFX Emitters will look for this SceneName when searching a PopcornFXScene to Spawn into.
	*/
	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	FName									SceneName;

	/** Enable PopcornFX Updates for this scene. */
	UPROPERTY(Category="PopcornFX Scene", Interp, EditAnywhere)
	uint32									bEnableUpdate : 1;

	/** Enable PopcornFX Render for this scene. */
	UPROPERTY(Category="PopcornFX Scene", Interp, EditAnywhere)
	uint32									bEnableRender : 1;

	/**	Bounding Box Computing Mode. */
	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	TEnumAsByte<EPopcornFXSceneBBMode::Type>	BoundingBoxMode;

	/**	Fixed relative bounding box used if BoundingBoxMode is DynamicPlusFixed or Fixed. */
	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	FBox									FixedRelativeBoundingBox;

	/** Override PopcornFX Config's Simulation Settings. */
	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	FPopcornFXSimulationSettings			SimulationSettingsOverride;

	/** Override PopcornFX Config's Render Settings. */
	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	FPopcornFXRenderSettings				RenderSettingsOverride;

	/** Global array of fields that the scene will try to recover when an event is broadcasted */
	UPROPERTY(Category="PopcornFX Scene", EditAnywhere)
	TArray<FPopcornFXFieldListener>			FieldListeners;

	/** DEBUG: Draw some heavy debug. */
	UPROPERTY(Category="PopcornFX SceneDebug", EditAnywhere)
	TEnumAsByte<EPopcornFXHeavyDebugMode::Type>		HeavyDebugMode;

	/** DEBUG: Freezes the main billboarding matrix. */
	UPROPERTY(Category="PopcornFX SceneDebug", EditAnywhere)
	uint32									bRender_FreezeBillboardingMatrix : 1;

	/** DEBUG: Overrides Particle Color by a per draw call color. */
	UPROPERTY(Category="PopcornFX SceneDebug", EditAnywhere)
	uint32									bRender_OverrideColorByDrawCall : 1;

	/** DEBUG: The number of colors used for Override Color debugs. */
	UPROPERTY(Category="PopcornFX SceneDebug", EditAnywhere)
	int32									Render_OverrideDebugColorCount;

	/** DEBUG: Overrides all Particles materials with this one.
	MUST be compatible for Particle Sprites AND Mesh Particles !
	(StaticSwitches are not available)
	*/
	UPROPERTY(Category="PopcornFX SceneDebug", EditAnywhere)
	UMaterialInterface						*Render_OverrideAllMaterial;


	/** Clears this scene from all existing Particles.
	Warning : Every emitters registered into this scene will stop to emit, you will need to manually restart your effects */
	UFUNCTION(BlueprintCallable, Category="PopcornFX|Scene", meta=(Keywords="popcornfx scene clear"))
	void								Clear();

	/**
	* Registers an event listener.
	* @param Delegate - Delegate to call when the event is raised.
	* @param EventName - Name of the Event to listen.
	*/
	UFUNCTION(BlueprintCallable, Category="PopcornFX|Emitter", meta=(Keywords="popcornfx particle emitter effect system event"))
	bool								RegisterEventListener(FPopcornFXRaiseEventSignature Delegate, FName EventName = NAME_None);

	/**
	* Unregisters an event listener
	* @param EventName - Name of the Event to stop listening.
	*/
	UFUNCTION(BlueprintCallable, Category="PopcornFX|Emitter", meta=(Keywords="popcornfx particle emitter effect system event"))
	void								UnregisterEventListener(FPopcornFXRaiseEventSignature Delegate, FName EventName = NAME_None);

	void								SetAudioSamplingInterface(class IPopcornFXFillAudioBuffers *fillAudioBuffers);

	// overrides UObject
	virtual void						PostLoad() override;
	virtual void						PostInitProperties() override;
#if WITH_EDITOR
	virtual void						PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif // WITH_EDITOR

	// overrides UActorComponent
	virtual void						OnRegister() override;
	virtual void						OnUnregister() override;
	virtual void						BeginDestroy() override;
	virtual void						TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) override;
	virtual void						SendRenderDynamicData_Concurrent() override;
	virtual void						CreateRenderState_Concurrent() override;

	// overrides USceneComponent
	virtual FBoxSphereBounds			CalcBounds(const FTransform & LocalToWorld) const override;

	// overrides UPrimitiveComponent
	virtual FPrimitiveSceneProxy		*CreateSceneProxy() override;
#if (ENGINE_MINOR_VERSION >= 15)
	virtual void						GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials = false) const override;
#endif

	bool								GetFieldValue(const FName &fieldName, void *outValue, EPopcornFXParticleFieldType::Type expectedFieldType) const;

	/** In Game builds, needs to be manually called to reload settings. */
	void								ResolveSettings();

	/** Actual Resolved Simulation Settings. */
	inline const FPopcornFXSimulationSettings		&ResolvedSimulationSettings() const { return m_ResolvedSimulationSettings; }
	/** Actual Resolved Render Settings. */
	inline const FPopcornFXRenderSettings			&ResolvedRenderSettings() const { return m_ResolvedRenderSettings; }

	inline int32						GetCollisionObjectTypesToQuery(uint32 filterIndex) const
	{
		if (m_ResolvedSimulationSettings.CollisionFilters.IsValidIndex(filterIndex))
			return m_ResolvedSimulationSettings.CollisionFilters[filterIndex].ObjectTypesToQuery;
		return m_ResolvedSimulationSettings.DefaultCollisionFilter.ObjectTypesToQuery;
	}

	inline CParticleScene				*ParticleScene() const { return m_ParticleScene; }
	inline CParticleScene				*ParticleSceneToRender() const { return bEnableRender != 0 ? ParticleScene() : nullptr; }

#if WITH_EDITOR
	void								MirrorGameWorldProperties(const UPopcornFXSceneComponent *other);
#endif // WITH_EDITOR

private:
	void								_OnSettingsChanged() { ResolveSettings(); }

	CParticleScene						*m_ParticleScene;
	FPopcornFXSimulationSettings		m_ResolvedSimulationSettings;
	FPopcornFXRenderSettings			m_ResolvedRenderSettings;
	FDelegateHandle						m_OnSettingsChangedHandle;
};
