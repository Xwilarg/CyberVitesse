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

#include "Components/SceneComponent.h"

#include "PopcornFXAttributeSampler.generated.h"

FWD_PK_API_BEGIN
class	CParticleSamplerDescriptor;
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class	CParticleScene;
struct	FPopcornFXSamplerDesc;

UENUM()
namespace EPopcornFXAttributeSamplerComponentType
{
	enum	Type
	{
		Shape,
		SkinnedMesh,
		Image,
		Curve,
		Text,
	};
}

UENUM()
namespace EPopcornFXAttributeSamplerType
{
	enum	Type
	{
		None = 0,
		Shape,
		Image,
		Curve,
		Text,
	};
}
enum { EPopcornFXAttributeSamplerType_Max = EPopcornFXAttributeSamplerType::Text + 1 };

UENUM()
namespace EPopcornFXAttribSamplerShapeType
{
	enum	Type
	{
		Box = 0,
		Sphere,
		ComplexEllipsoid,
		Cylinder,
		Capsule,
		Cone,
		Plane,
		Mesh,
		Collection,
	};
}
enum { EPopcornFXAttribSamplerShapeType_Max = EPopcornFXAttribSamplerShapeType::Collection + 1 };

UCLASS(Abstract, EditInlineNew)
class POPCORNFX_API UPopcornFXAttributeSampler : public USceneComponent
{
	GENERATED_UCLASS_BODY()

public:
	static UClass							*SamplerComponentClass(EPopcornFXAttributeSamplerComponentType::Type type);
	EPopcornFXAttributeSamplerType::Type	SamplerType() const { return m_SamplerType; }


	// PopcornFX Internal
	virtual PopcornFX::CParticleSamplerDescriptor	*_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultDesc) { return nullptr; }
	virtual void									_AttribSampler_PreUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType) { return; }

#if WITH_EDITOR
	virtual void									_AttribSampler_IndirectSelectedThisTick() {}
#endif

protected:
	EPopcornFXAttributeSamplerType::Type	m_SamplerType;
};
