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

#include "PopcornFXMinimal.h"
#include "PopcornFXAttributeSampler.h"

#include "SharedPointer.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_descriptor.h>
#include <hh_particles/include/ps_samplers.h>

#include "PopcornFXAttributeList.generated.h"

class	AActor;

FWD_PK_API_BEGIN
struct	SAttributesContainer;
struct	SAttributesContainer_SAttrib;
class	CParticleSampler;
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class	UPopcornFXEffect;
class	UPopcornFXAttribSamplerInterface;
class	UPopcornFXEmitterComponent;

EPopcornFXAttributeSamplerType::Type		ResolveAttribSamplerType(const PopcornFX::CParticleSampler *sampler);
EPopcornFXAttributeSamplerType::Type		ResolveAttribSamplerType(const PopcornFX::CParticleSamplerDescriptor &desc);

EPopcornFXAttribSamplerShapeType::Type		ResolveAttribSamplerShapeType(const PopcornFX::CParticleSampler *sampler);

// PopcornFX::CShapeDescriptor::EShapeType
hh_u32										ToPkShapeType(EPopcornFXAttribSamplerShapeType::Type ueShapeType);

const char			*ResolveAttribSamplerNodeName(const PopcornFX::CParticleSampler *sampler, EPopcornFXAttributeSamplerType::Type samplerType);
const char			*ResolveAttribSamplerShapeNodeName(EPopcornFXAttribSamplerShapeType::Type shapeType);

USTRUCT()
struct FPopcornFXAttributeDesc
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY()
	FName					m_AttributeFName;

	UPROPERTY()
	uint32					m_AttributeType;

	FPopcornFXAttributeDesc() : m_AttributeFName(), m_AttributeType(~0U) {}

	bool					Valid() const { return m_AttributeFName.IsValid() && !m_AttributeFName.IsNone(); }
	FName					AttributeFName() const { return m_AttributeFName; }
	FString					AttributeName() const { return (Valid() ? m_AttributeFName.ToString() : FString()); }
	bool					ValidAttributeType() const { return m_AttributeType != ~0U; }
	uint32					AttributeBaseTypeID() const { HH_VERIFY(ValidAttributeType());  return m_AttributeType; }

	void		Reset(const PopcornFX::CParticleAttributeDeclaration *decl);
	void		Reset()
	{
		m_AttributeFName = FName();
		m_AttributeType = ~0U;
	}

	bool		NameMatch(const FPopcornFXAttributeDesc &other) const
	{
		return Valid() && m_AttributeFName == other.m_AttributeFName;
	}

	bool		ExactMatch(const FPopcornFXAttributeDesc &other) const
	{
		return Valid() && m_AttributeFName == other.m_AttributeFName && m_AttributeType == other.m_AttributeType;
	}
};

USTRUCT()
struct FPopcornFXSamplerDesc
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(Category="PopcornFX AttributeSampler", VisibleAnywhere)
	FName						m_SamplerFName;

	UPROPERTY(Category="PopcornFX AttributeSampler", VisibleAnywhere)
	TEnumAsByte<EPopcornFXAttributeSamplerType::Type>	m_SamplerType;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	AActor						*m_AttributeSamplerActor;

	UPROPERTY(Category="PopcornFX AttributeSampler", EditAnywhere)
	FName						m_AttributeSamplerComponentProperty;

	PopcornFX::PParticleSamplerDescriptor		m_SamplerDescriptor;

	bool					m_NeedUpdate;

	FPopcornFXSamplerDesc() : m_SamplerFName(), m_SamplerType(EPopcornFXAttributeSamplerType::None), m_AttributeSamplerActor(null), m_NeedUpdate(false) {}

	bool		Valid() const { return m_SamplerFName.IsValid() && !m_SamplerFName.IsNone(); }
	FName		SamplerFName() const { return m_SamplerFName; }
	FString		SamplerName() const { return (Valid() ? m_SamplerFName.ToString() : FString()); }
	EPopcornFXAttributeSamplerType::Type	SamplerType() const { return m_SamplerType; }

	UPopcornFXAttributeSampler		*ResolveAttributeSampler(AActor *fallbackActor, UObject *enableLogForOwner) const;

	void		Reset(const PopcornFX::CParticleSampler *sampler);
	void		Reset()
	{
		m_SamplerFName = FName();
		m_SamplerType = EPopcornFXAttributeSamplerType::None;
	}

	void		CopyValuesFrom(const FPopcornFXSamplerDesc &other)
	{
		m_AttributeSamplerActor = other.m_AttributeSamplerActor;
		m_AttributeSamplerComponentProperty = other.m_AttributeSamplerComponentProperty;
	}

	void		SwapValuesWith(FPopcornFXSamplerDesc &other)
	{
		Swap(m_AttributeSamplerActor, other.m_AttributeSamplerActor);
		Swap(m_AttributeSamplerComponentProperty, other.m_AttributeSamplerComponentProperty);
	}

	void		ResetValue() { m_AttributeSamplerActor = null; m_AttributeSamplerComponentProperty = FName(); }
	bool		ValueIsEmpty() const { return m_AttributeSamplerActor == null && m_AttributeSamplerComponentProperty.IsNone(); }

	bool		NameMatch(const FPopcornFXSamplerDesc &other) const
	{
		return Valid() && m_SamplerFName == other.m_SamplerFName;
	}

	bool		ExactMatch(const FPopcornFXSamplerDesc &other) const
	{
		return Valid() && m_SamplerFName == other.m_SamplerFName && m_SamplerType == other.m_SamplerType;
	}
};

UCLASS(MinimalAPI, EditInlineNew, DefaultToInstanced)
class UPopcornFXAttributeList : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	~UPopcornFXAttributeList();

	bool			CheckDataIntegrity() const;
	bool			Valid() const;
	bool			IsUpToDate(UPopcornFXEffect *effect) const;

	bool			IsEmpty() const;
	void			Clean();

	void			SetupDefault(UPopcornFXEffect *sourceEffect, bool force = false);
	bool			Prepare(UPopcornFXEffect *effect, bool force = false);
	void			CopyFrom(const UPopcornFXAttributeList *other, AActor *patchParentActor = null);

	const UPopcornFXAttributeList		*GetDefaultAttribteList(UPopcornFXEffect *effect) const; // can be self

	void			ResetToDefaultValues(UPopcornFXEffect *effect);

	void			RefreshAttributeSamplers(UPopcornFXEmitterComponent *emitter, bool reload = false) { if (m_Samplers.Num() > 0) _RefreshAttributeSamplers(emitter, reload); }

	PopcornFX::SAttributesContainer*const*		AttributesContainer();
	PopcornFX::SAttributesContainer*const*		AttributesContainerIFP() const { return &m_AttributesContainer; }
	PopcornFX::TMemoryView<PopcornFX::SAttributesContainer_SAttrib>	AttributeRawDataAttributes();

	void			Scene_PreUpdate(CParticleScene *scene, UPopcornFXEmitterComponent *emitter, float deltaTime, enum ELevelTick tickType);
#if WITH_EDITOR
	void			AttributeSamplers_IndirectSelectedThisTick(UPopcornFXEmitterComponent *emitter) const;
#endif

	uint32			AttributeCount() const { return m_Attributes.Num(); }
	int32			FindAttributeIndex(FName fname) const;

	uint32			SamplerCount() const { return m_Samplers.Num(); }
	int32			FindSamplerIndex(FName fname) const;

	const FPopcornFXAttributeDesc						*GetAttributeDesc(uint32 attributeId) const;
	const PopcornFX::CParticleAttributeDeclaration		*GetAttributeDeclaration(UPopcornFXEffect *effect, uint32 attributeId) const;
	const FPopcornFXSamplerDesc							*GetSamplerDesc(uint32 samplerId) const;
	const PopcornFX::CParticleSampler					*GetParticleSampler(UPopcornFXEffect *effect, uint32 samplerId) const;

	uint32			FileVersionId() const { return m_FileVersionId; }

	// overrides UObject
	virtual bool	IsSafeForRootSet() const override { return false; }
	virtual void	PostLoad() override;
	virtual void	PostInitProperties() override;
	virtual void	BeginDestroy() override;
#if (ENGINE_MINOR_VERSION >= 13)
	virtual void	PreSave(const class ITargetPlatform* TargetPlatform) override;
#else
	virtual void	PreSave() override;
#endif
	virtual void	Serialize(FArchive& Ar) override;
#if WITH_EDITOR
	virtual void	PostEditUndo() override;
	virtual void	PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

	void			RestoreAttributesFromCachedRawData(const TArray<uint8> &rawData);

private:
	void			_DestroyAttributesContainer();
	void			_RefreshAttributeSamplers(UPopcornFXEmitterComponent *emitter, bool reload);

	UPROPERTY()
	UPopcornFXEffect					*m_Effect;

	UPROPERTY(Category="PopcornFX Attributes", VisibleAnywhere)
	uint32								m_FileVersionId;

	UPROPERTY(Category="PopcornFX Attributes", VisibleAnywhere)
	TArray<FPopcornFXAttributeDesc>		m_Attributes;

	UPROPERTY(Category="PopcornFX Attributes", EditAnywhere, EditFixedSize)
	TArray<FPopcornFXSamplerDesc>		m_Samplers;

public:
	UPROPERTY(Category="PopcornFX Attributes", EditAnywhere, BlueprintReadOnly, EditFixedSize)
	TArray<uint8>						m_AttributesRawData;

	const class UPopcornFXEmitterComponent	*m_Owner = null;

	void								CheckEmitter(const class UPopcornFXEmitterComponent *emitter);

private:
	PopcornFX::SAttributesContainer		*m_AttributesContainer = null;
};

inline PopcornFX::TMemoryView<PopcornFX::SAttributesContainer_SAttrib>		UPopcornFXAttributeList::AttributeRawDataAttributes()
{
	HH_ASSERT(CheckDataIntegrity());
	return PopcornFX::TMemoryView<PopcornFX::SAttributesContainer::SAttrib>(reinterpret_cast<PopcornFX::SAttributesContainer::SAttrib*>(m_AttributesRawData.GetData()), m_Attributes.Num());
}
