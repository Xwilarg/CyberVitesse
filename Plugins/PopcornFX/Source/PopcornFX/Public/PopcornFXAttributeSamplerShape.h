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

#include "PopcornFXAttributeSampler.h"

#include "PopcornFXAttributeSamplerShape.generated.h"

struct FAttributeSamplerShapeData;

FWD_PK_API_BEGIN
class	CShapeDescriptor;
class	CParticleSamplerDescriptor_Shape;
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

UENUM()
namespace EPopcornFXShapeCollectionSamplingHeuristic
{
	// hh_geometrics/include/ge_shapes.h
	enum	Type
	{
		NoWeight = 0,
		WeightWithVolume,
		WeightWithSurface,
	};
}

/** Can override an Attribute Sampler **Shape** by a **UStaticMesh**. */
UCLASS(EditInlineNew, meta=(BlueprintSpawnableComponent), ClassGroup=PopcornFX)
class POPCORNFX_API UPopcornFXAttributeSamplerShape : public UPopcornFXAttributeSampler
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(Category="PopcornFX AttributeSampler", BlueprintCallable)
	void					SetRadius(float radius);

	UFUNCTION(Category = "PopcornFX AttributeSampler", BlueprintCallable)
	void					SetWeight(float height);

	UFUNCTION(Category = "PopcornFX AttributeSampler", BlueprintCallable)
	void					SetBoxDimension(FVector boxDimensions);

	UFUNCTION(Category = "PopcornFX AttributeSampler", BlueprintCallable)
	void					SetInnerRadius(float innerRadius);

	UFUNCTION(Category = "PopcornFX AttributeSampler", BlueprintCallable)
	void					SetHeight(float height);
public:
	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	TEnumAsByte<EPopcornFXAttribSamplerShapeType::Type>		ShapeType;

	/** Weights sampling distribution when is a sub-Shape of a Shape Collection
	* (if CollectionUseShapeWeights is enabled in the Shape Collection).
	*/
	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere, BlueprintReadOnly)
	float					Weight;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere, BlueprintReadOnly)
	FVector					BoxDimension;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere, BlueprintReadOnly)
	float					Radius;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere, BlueprintReadOnly)
	float					InnerRadius;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere, BlueprintReadOnly)
	float					Height;

	/** Distribute sampling by the given CollectionSamplingHeuristic of sub-Shapes */
	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	TEnumAsByte<EPopcornFXShapeCollectionSamplingHeuristic::Type>		CollectionSamplingHeuristic;

	/** Distribute sampling by the sub-Shapes own Weights (in addition to CollectionSamplingHeuristic) */
	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	uint32					CollectionUseShapeWeights : 1;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	class UStaticMesh		*StaticMesh;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	int32					StaticMeshSubIndex;

	/** Collection sub-Shapes */
	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	TArray<class APopcornFXAttributeSamplerActor*>	Shapes;

	/** Relative Transforms will be used if activated.
	* Enable if sampled in SpawnerScript's Eval(), so shape will be sampled locally to the Emitter.
	* Disable if sampled in SpawnerScript's **Post**Eval(), so the shape will be sampled world space.
	*/
	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	uint32					bUseRelativeTransform : 1;

public:
	// overrides
	virtual void			BeginDestroy() override;
#if WITH_EDITOR
	void					PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
	void					TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) override;
#endif // WITH_EDITOR

	inline bool						IsCollection() const { return ShapeType == EPopcornFXAttribSamplerShapeType::Collection; }
	inline bool						IsValid() const;
	bool							InitShape(const PopcornFX::CShapeDescriptor *defaultShapeDesc);
	PopcornFX::CShapeDescriptor		*GetShapeDescriptor() const;

#if WITH_EDITOR
	void							RenderShapeIFP(bool isSelected) const;
#endif
private:
	bool											CanUpdateShapeProperties();
	void											UpdateShapeProperties();

	// PopcornFX Internal
	virtual PopcornFX::CParticleSamplerDescriptor	*_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultDesc) override;
	virtual void									_AttribSampler_PreUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType) override;

#if WITH_EDITOR
	virtual void									_AttribSampler_IndirectSelectedThisTick() override { m_IndirectSelectedThisTick = true; }
#endif
private:
	FAttributeSamplerShapeData	*m_Data;
	uint64						m_LastFrameUpdate;
	FMatrix						m_WorldTr_Current;
	FMatrix						m_WorldTr_Previous;
	FVector						m_Angular_Velocity;
	FVector						m_Linear_Velocity;

#if WITH_EDITOR
	bool						m_IndirectSelectedThisTick;
#endif // WITH_EDITOR
};
