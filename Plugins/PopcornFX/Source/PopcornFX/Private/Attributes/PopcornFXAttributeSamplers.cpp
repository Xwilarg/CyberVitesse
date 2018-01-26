//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "PopcornFXSDK.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXAttributeSamplerActor.h"
#include "PopcornFXAttributeList.h"
#include "PopcornFXAttributeSampler.h"
#include "PopcornFXAttributeSamplerShape.h"
#include "PopcornFXAttributeSamplerSkinnedMesh.h"
#include "PopcornFXAttributeSamplerImage.h"
#include "PopcornFXAttributeSamplerText.h"
#include "PopcornFXAttributeSamplerCurve.h"
#include "PopcornFXAttributeSamplerCurveDynamic.h"
#include "Assets/PopcornFXMesh.h"
#include "Internal/ResourceHandlerImage_UE.h"

#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#if WITH_EDITOR
#	include "Editor.h"
#	include "Engine/Selection.h"
#endif

#include "PopcornFXSDK.h"
#include <hh_imaging/include/im_image.h>
#include <hh_particles/include/ps_descriptor.h>
#include <hh_particles/include/ps_samplers_classes.h>
#include <hh_kernel/include/kr_containers_array.h>
#include <hh_kernel/include/kr_containers_onstack.h>
#include <hh_kernel/include/kr_refcounted_buffer.h>
#include <hh_maths/include/hh_maths_interpolable.h>

#include <hh_particles/include/ps_font_metrics.h>

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXAttributeSampler"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXAttributeSampler, Log, All);

//----------------------------------------------------------------------------
//
// APopcornFXAttributeSamplerActor
//
//----------------------------------------------------------------------------

APopcornFXAttributeSamplerActor::APopcornFXAttributeSamplerActor(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, Sampler(null)
	, m_SamplerComponentType(EPopcornFXAttributeSamplerComponentType::Shape)
{
#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (!IsRunningCommandlet() && SpriteComponent != null)
	{
		struct FConstructorStatics
		{
			FName ID_Effects;
			FText NAME_Effects;
			FConstructorStatics()
				: ID_Effects(TEXT("Effects")) // do not change, recognize by the engine
				, NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects")) // do not change, recognize by the engine
			{}
		};
		static FConstructorStatics ConstructorStatics;
		// SpriteComponent->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
		SpriteComponent->RelativeScale3D = FVector(1.5f);
		SpriteComponent->bHiddenInGame = true;
		SpriteComponent->bIsScreenSizeScaled = true;
		SpriteComponent->SpriteInfo.Category=ConstructorStatics.ID_Effects;
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Effects;
		SpriteComponent->bReceivesDecals = false;
		// SpriteComponent->SetupAttachment() // later, when sampler will be ready
	}
#endif

	SetFlags(RF_Transactional);
}

//----------------------------------------------------------------------------

void	APopcornFXAttributeSamplerActor::_CtorRootSamplerComponent(const FObjectInitializer &PCIP, EPopcornFXAttributeSamplerComponentType::Type samplerType)
{
	m_SamplerComponentType = samplerType;
	m_IsValidSpecializedActor = true;

	UClass		*samplerClass = UPopcornFXAttributeSampler::SamplerComponentClass(m_SamplerComponentType);
	Sampler = static_cast<UPopcornFXAttributeSampler*>(
		PCIP.CreateDefaultSubobject(
			this, TEXT("Sampler"),
			UPopcornFXAttributeSampler::StaticClass(), samplerClass, true, false, false));

	RootComponent = Sampler;

#if WITH_EDITORONLY_DATA
	if (!IsRunningCommandlet() && SpriteComponent != null)
		SpriteComponent->SetupAttachment(RootComponent);
#endif
}

//----------------------------------------------------------------------------

APopcornFXAttributeSamplerShapeActor::APopcornFXAttributeSamplerShapeActor(const FObjectInitializer &PCIP)
:	Super(PCIP)
{
	_CtorRootSamplerComponent(PCIP, EPopcornFXAttributeSamplerComponentType::Shape);
}

APopcornFXAttributeSamplerSkinnedMeshActor::APopcornFXAttributeSamplerSkinnedMeshActor(const FObjectInitializer &PCIP)
: Super(PCIP)
{
	_CtorRootSamplerComponent(PCIP, EPopcornFXAttributeSamplerComponentType::SkinnedMesh);
}

APopcornFXAttributeSamplerImageActor::APopcornFXAttributeSamplerImageActor(const FObjectInitializer &PCIP)
: Super(PCIP)
{
	_CtorRootSamplerComponent(PCIP, EPopcornFXAttributeSamplerComponentType::Image);
}

APopcornFXAttributeSamplerCurveActor::APopcornFXAttributeSamplerCurveActor(const FObjectInitializer &PCIP)
: Super(PCIP)
{
	_CtorRootSamplerComponent(PCIP, EPopcornFXAttributeSamplerComponentType::Curve);
}

APopcornFXAttributeSamplerTextActor::APopcornFXAttributeSamplerTextActor(const FObjectInitializer &PCIP)
: Super(PCIP)
{
	_CtorRootSamplerComponent(PCIP, EPopcornFXAttributeSamplerComponentType::Text);
}

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

#if WITH_EDITORONLY_DATA
void		APopcornFXAttributeSamplerActor::ReloadSprite()
{
	if (IsRunningCommandlet())
		return;
	if (!HH_VERIFY(SpriteComponent != null))
		return;

	FString		spritePath;
	if (m_IsValidSpecializedActor)
	{
		FString				spriteName;
		switch (m_SamplerComponentType)
		{
		case	EPopcornFXAttributeSamplerComponentType::Shape:
		{
			const UPopcornFXAttributeSamplerShape	*shape = Cast<UPopcornFXAttributeSamplerShape>(Sampler);
			if (shape != null)
				spriteName = ResolveAttribSamplerShapeNodeName(shape->ShapeType);
			else
				spriteName = "CParticleSamplerShape";
			break;
		}
		case	EPopcornFXAttributeSamplerComponentType::SkinnedMesh:
			spriteName = "CParticleSamplerSkinnedMesh";
			break;
		case	EPopcornFXAttributeSamplerComponentType::Image:
			spriteName = "CParticleSamplerTexture";
			break;
		case EPopcornFXAttributeSamplerComponentType::Curve:
			spriteName = "CParticleSamplerCurve";
			break;
		case	EPopcornFXAttributeSamplerComponentType::Text:
			spriteName = "CParticleSamplerText";
			break;
		default:
			spriteName = "CParticleSamplerShape";
			break;
		}
		spritePath = "/PopcornFX/SlateBrushes/" + spriteName + "_med." + spriteName + "_med";
	}
	else
	{
		// Baad, should be replaced by the actual Actor !
		spritePath = "/PopcornFX/SlateBrushes/BadIcon.BadIcon";
	}

	if (!spritePath.IsEmpty())
	{
		SpriteComponent->SetSprite(LoadObject<UTexture2D>(null, *spritePath));
		SpriteComponent->MarkForNeededEndOfFrameRecreate();
	}

	if (RootComponent != null && SpriteComponent->GetAttachmentRoot() != RootComponent)
	{
		SpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);
	}
}
#endif // WITH_EDITORONLY_DATA

//----------------------------------------------------------------------------

void	APopcornFXAttributeSamplerActor::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	const UProperty	*propertyThatChanged = propertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(APopcornFXAttributeSamplerActor, m_SamplerComponentType) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(APopcornFXAttributeSamplerActor, Sampler))
		{
			ReloadSprite();
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

#if WITH_EDITORONLY_DATA
void	APopcornFXAttributeSamplerActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
	ReloadSprite();
}
#endif

//----------------------------------------------------------------------------

void		APopcornFXAttributeSamplerActor::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	if (!m_IsValidSpecializedActor) // else, the saved Sprite should already be the right one
	{
		SpriteComponent->ConditionalPostLoad();
		Sampler->ConditionalPostLoad();
		ReloadSprite();
	}
#endif // WITH_EDITOR
}

//----------------------------------------------------------------------------

void		APopcornFXAttributeSamplerActor::PostActorCreated()
{
	Super::PostActorCreated();
#if WITH_EDITOR
	ReloadSprite();
#endif // WITH_EDITOR
}

//----------------------------------------------------------------------------
//
// UPopcornFXAttributeSampler
//
//----------------------------------------------------------------------------

// static 
UClass		*UPopcornFXAttributeSampler::SamplerComponentClass(EPopcornFXAttributeSamplerComponentType::Type type)
{
	switch (type)
	{
	case	EPopcornFXAttributeSamplerComponentType::Shape:
		return UPopcornFXAttributeSamplerShape::StaticClass();
	case	EPopcornFXAttributeSamplerComponentType::SkinnedMesh:
		return UPopcornFXAttributeSamplerSkinnedMesh::StaticClass();
	case	EPopcornFXAttributeSamplerComponentType::Image:
		return UPopcornFXAttributeSamplerImage::StaticClass();
	case	EPopcornFXAttributeSamplerComponentType::Curve:
		return UPopcornFXAttributeSamplerCurve::StaticClass();
	case	EPopcornFXAttributeSamplerComponentType::Text:
		return UPopcornFXAttributeSamplerText::StaticClass();
	}
	// dummy attribute sampler
	return UPopcornFXAttributeSampler::StaticClass();
}

UPopcornFXAttributeSampler::UPopcornFXAttributeSampler(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, m_SamplerType(EPopcornFXAttributeSamplerType::None)
{
	SetFlags(RF_Transactional);
}

//----------------------------------------------------------------------------
//
// UPopcornFXAttributeSamplerShape
//
//----------------------------------------------------------------------------

namespace
{
	using PopcornFX::CShapeDescriptor;
	using PopcornFX::PShapeDescriptor;
	using PopcornFX::PCShapeDescriptor;

	struct SNewShapeParams
	{
		const UPopcornFXAttributeSamplerShape	*self;
	};

	struct SUpdateShapeParams
	{
		const UPopcornFXAttributeSamplerShape	*self;
		CShapeDescriptor						*shape;
	};

	typedef CShapeDescriptor*(*CbNewDescriptor)(const SNewShapeParams &params);
	typedef void(*CbUpdateShapeDescriptor)(const SUpdateShapeParams &params);

	// NULL
	template <typename _Descriptor>
	CShapeDescriptor	*_NewDescriptor(const SNewShapeParams &params)
	{
		return null;
	}

	// NULL
	template <typename _Descriptor>
	void				_UpdateShapeDescriptor(const SUpdateShapeParams &params)
	{
	}

	// BOX
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Box>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape	*self = params.self;
		//const PopcornFX::CShapeDescriptor_Box	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Box*>(params.defaultShape);

		HH_ASSERT(self->BoxDimension.GetMin() >= 0.0f);
		const FVector	safeBoxDimension = self->BoxDimension.GetClampedToSize(0.0f, self->BoxDimension.GetMax());
		const CFloat3	dim = ToPk(safeBoxDimension) * FPopcornFXPlugin::GlobalScaleRcp();
		return HH_NEW(PopcornFX::CShapeDescriptor_Box(dim));
	}

	// BOX
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Box>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape	*self = params.self;
		PopcornFX::CShapeDescriptor_Box			*shape = static_cast<PopcornFX::CShapeDescriptor_Box*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Box	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Box*>(params.defaultShape);

		HH_ASSERT(self->BoxDimension.GetMin() >= 0.0f);
		const FVector	safeBoxDimension = self->BoxDimension.GetClampedToSize(0.0f, self->BoxDimension.GetMax());
		const CFloat3	dim = ToPk(safeBoxDimension) * FPopcornFXPlugin::GlobalScaleRcp();

		shape->SetDimensions(dim);
	}

	// SPHERE
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Sphere>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		//const PopcornFX::CShapeDescriptor_Sphere	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Sphere*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;

		return HH_NEW(PopcornFX::CShapeDescriptor_Sphere(radius, innerRadius));
	}

	// SPHERE
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Sphere>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		PopcornFX::CShapeDescriptor_Sphere			*shape = static_cast<PopcornFX::CShapeDescriptor_Sphere*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Sphere	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Sphere*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;

		shape->SetInnerRadius(innerRadius);
		shape->SetRadius(radius);
	}

	// ELLIPSOID
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_ComplexEllipsoid>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape				*self = params.self;
		//const PopcornFX::CShapeDescriptor_ComplexEllipsoid	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_ComplexEllipsoid*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;

		return HH_NEW(PopcornFX::CShapeDescriptor_ComplexEllipsoid(radius, innerRadius));
	}

	// ELLIPSOID
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_ComplexEllipsoid>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape				*self = params.self;
		PopcornFX::CShapeDescriptor_ComplexEllipsoid		*shape = static_cast<PopcornFX::CShapeDescriptor_ComplexEllipsoid*>(params.shape);
		//const PopcornFX::CShapeDescriptor_ComplexEllipsoid	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_ComplexEllipsoid*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;

		shape->SetInnerRadius(innerRadius);
		shape->SetRadius(radius);
	}

	// CYLINDRE
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Cylinder>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		//const PopcornFX::CShapeDescriptor_Cylinder	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Cylinder*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);
		HH_ASSERT(self->Height >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;
		const float			height = FGenericPlatformMath::Max(self->Height, 0.0f) * invGlobalScale;

		return HH_NEW(PopcornFX::CShapeDescriptor_Cylinder(radius, height, innerRadius));
	}

	// CYLINDRE
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Cylinder>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		PopcornFX::CShapeDescriptor_Cylinder		*shape = static_cast<PopcornFX::CShapeDescriptor_Cylinder*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Cylinder	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Cylinder*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);
		HH_ASSERT(self->Height >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;
		const float			height = FGenericPlatformMath::Max(self->Height, 0.0f) * invGlobalScale;

		shape->SetInnerRadius(innerRadius);
		shape->SetRadius(radius);
		shape->SetHeight(height);
	}

	// CAPSULE
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Capsule>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		//const PopcornFX::CShapeDescriptor_Capsule	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Capsule*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);
		HH_ASSERT(self->Height >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;
		const float			height = FGenericPlatformMath::Max(self->Height, 0.0f) * invGlobalScale;

		return HH_NEW(PopcornFX::CShapeDescriptor_Capsule(radius, height, innerRadius));
	}

	// CAPSULE
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Capsule>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		PopcornFX::CShapeDescriptor_Capsule			*shape = static_cast<PopcornFX::CShapeDescriptor_Capsule*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Capsule	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Capsule*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->InnerRadius >= 0.0f);
		HH_ASSERT(self->Height >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			innerRadius = FGenericPlatformMath::Max(self->InnerRadius, 0.0f) * invGlobalScale;
		const float			height = FGenericPlatformMath::Max(self->Height, 0.0f) * invGlobalScale;

		shape->SetInnerRadius(innerRadius);
		shape->SetRadius(radius);
		shape->SetHeight(height);
	}

	// CONE
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Cone>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape	*self = params.self;
		//const PopcornFX::CShapeDescriptor_Cone	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Cone*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->Height >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			height = FGenericPlatformMath::Max(self->Height, 0.0f) * invGlobalScale;

		return HH_NEW(PopcornFX::CShapeDescriptor_Cone(radius, height));
	}

	// CONE
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Cone>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape	*self = params.self;
		PopcornFX::CShapeDescriptor_Cone		*shape = static_cast<PopcornFX::CShapeDescriptor_Cone*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Cone	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Cone*>(params.defaultShape);

		HH_ASSERT(self->Radius >= 0.0f);
		HH_ASSERT(self->Height >= 0.0f);

		const float			invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		const float			radius = FGenericPlatformMath::Max(self->Radius, 0.0f) * invGlobalScale;
		const float			height = FGenericPlatformMath::Max(self->Height, 0.0f) * invGlobalScale;

		shape->SetRadius(radius);
		shape->SetHeight(height);
	}

	// PLANE
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Plane>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape	*self = params.self;
		//const PopcornFX::CShapeDescriptor_Plane	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Plane*>(params.defaultShape);

		return HH_NEW(PopcornFX::CShapeDescriptor_Plane());
	}

	// PLANE
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Plane>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		PopcornFX::CShapeDescriptor_Plane			*shape = static_cast<PopcornFX::CShapeDescriptor_Plane*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Plane		*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Plane*>(params.defaultShape);
	}

	// MESH
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Mesh>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape	*self = params.self;
		//const PopcornFX::CShapeDescriptor_Mesh	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Mesh*>(params.defaultShape);

		if (self->StaticMesh == null)
			return null;

		HH_FIXME("use the ResourceManager with TResourcePtr !!");
		UPopcornFXMesh		*pkMesh = UPopcornFXMesh::FindStaticMesh(self->StaticMesh);
		if (pkMesh == null)
			return null;

		PopcornFX::PResourceMesh	meshRes = pkMesh->LoadResourceMeshIFN(true);
		if (meshRes == null)
			return null;

		const hh_u32				batchCount = meshRes->StaticBatchList().Count();
		if (!HH_VERIFY(batchCount != 0))
			return null;

		const hh_u32							batchi = PopcornFX::HHMin(hh_u32(PopcornFX::HHMax(self->StaticMeshSubIndex, 0)), batchCount - 1);
		const PopcornFX::PResourceMeshBatch		&batch = meshRes->StaticBatchList()[batchi];
		if (!HH_VERIFY(batch != null))
			return null;
		if (!HH_VERIFY(batch->RawMesh() != null))
			return null;

		PopcornFX::CSampleDataStream::EMode	samplingMode = PopcornFX::CSampleDataStream::UniformDistribution;
		// don't care Uniform is fine...
		//if (defaultShape != null && defaultShape->ShapeType() == PopcornFX::CShapeDescriptor::ShapeMesh)
		//	samplingMode = static_cast<const PopcornFX::CShapeDescriptor_Mesh*>(defaultShape)->SamplingMode();
		return HH_NEW(PopcornFX::CShapeDescriptor_Mesh(batch->RawMesh(), CFloat3::ONE, samplingMode));
	}

	// MESH
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Mesh>(const SUpdateShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape		*self = params.self;
		PopcornFX::CShapeDescriptor_Mesh			*shape = static_cast<PopcornFX::CShapeDescriptor_Mesh*>(params.shape);
		//const PopcornFX::CShapeDescriptor_Mesh		*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Mesh*>(params.defaultShape);

		//HH_TODO("Update mesh ?");
	}

	// COLLECTION
	template <>
	CShapeDescriptor	*_NewDescriptor<PopcornFX::CShapeDescriptor_Collection>(const SNewShapeParams &params)
	{
		const UPopcornFXAttributeSamplerShape			*self = params.self;
		//const PopcornFX::CShapeDescriptor_Collection	*defaultShape = static_cast<const PopcornFX::CShapeDescriptor_Collection*>(params.defaultShape);

		PopcornFX::CShapeDescriptor_Collection	*shapeCollection = HH_NEW(PopcornFX::CShapeDescriptor_Collection());
		if (shapeCollection == null)
			return null;
		if (self->Shapes.Num() == 0) // not an error
			return shapeCollection;

		HH_STATIC_ASSERT(EPopcornFXShapeCollectionSamplingHeuristic::NoWeight			== (hh_u32)PopcornFX::CShapeDescriptor_Collection::NoWeight);
		HH_STATIC_ASSERT(EPopcornFXShapeCollectionSamplingHeuristic::WeightWithVolume	== (hh_u32)PopcornFX::CShapeDescriptor_Collection::WeightWithVolume);
		HH_STATIC_ASSERT(EPopcornFXShapeCollectionSamplingHeuristic::WeightWithSurface	== (hh_u32)PopcornFX::CShapeDescriptor_Collection::WeightWithSurface);
		HH_STATIC_ASSERT(3 == PopcornFX::CShapeDescriptor_Collection::__MaxSamplingHeuristics);
		shapeCollection->m_SamplingHeuristic = static_cast<PopcornFX::CShapeDescriptor_Collection::ESamplingHeuristic>(self->CollectionSamplingHeuristic.GetValue());
		shapeCollection->m_UseSubShapeWeights = (self->CollectionUseShapeWeights != 0);
		shapeCollection->m_PermutateMultiSamples = false;

		const float	invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
		for (auto shapeIt = self->Shapes.CreateConstIterator(); shapeIt; ++shapeIt)
		{
			const APopcornFXAttributeSamplerActor	*attrSampler = *shapeIt;

			if (attrSampler == null)
				continue;
			UPopcornFXAttributeSamplerShape	*shape = Cast<UPopcornFXAttributeSamplerShape>(attrSampler->Sampler);
			if (shape == null || shape->IsCollection())
				continue;

			// Meshes associated with shape collections won't be setup by their default shape
			if (!shape->IsValid())
			{
				HH_ONLY_IF_ASSERTS(bool shapeInitOk = )shape->InitShape(null);
				HH_ASSERT(shapeInitOk);
			}

			PopcornFX::CShapeDescriptor	*desc = shape->GetShapeDescriptor();
			if (!HH_VERIFY(desc != null) ||
				!HH_VERIFY(shapeCollection->AddSubShape(desc)))
				continue;
			// Don't transform the world rotation
			const FVector	transformedLocation = self->GetComponentTransform().InverseTransformPosition(shape->GetComponentLocation());
			FMatrix			localMatrix = FTransform(shape->GetComponentRotation(), transformedLocation, FVector::ZeroVector).ToMatrixNoScale();

			// This first matrix is likely to be incorrect on editor load
			// it will be patched at PreUpdate
			localMatrix.M[3][0] *= invGlobalScale;
			localMatrix.M[3][1] *= invGlobalScale;
			localMatrix.M[3][2] *= invGlobalScale;
			desc->m_LocalTransforms = ToPk(localMatrix);
		}
		return shapeCollection;
	}

	// COLLECTION
	template <>
	void				_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Collection>(const SUpdateShapeParams &params)
	{
		//PopcornFX::CShapeDescriptor_Collection	*collection = static_cast<PopcornFX::CShapeDescriptor_Collection*>(shape);
	}

	CbNewDescriptor const		kCbNewDescriptors[] = {
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Box>,				//Box = 0,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Sphere>,			//Sphere,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_ComplexEllipsoid>,	//ComplexEllipsoid,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Cylinder>,			//Cylinder,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Capsule>,			//Capsule,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Cone>,				//Cone,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Plane>,				//Plane,
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Mesh>,				//Mesh,
		//null,		// Spline
		&_NewDescriptor<PopcornFX::CShapeDescriptor_Collection>,		//Collection
	};

	CbUpdateShapeDescriptor const	kCbUpdateShapeDescriptors[] = {
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Box>,				//Box = 0,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Sphere>,			//Sphere,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_ComplexEllipsoid>,	//ComplexEllipsoid,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Cylinder>,			//Cylinder,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Capsule>,			//Capsule,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Cone>,				//Cone,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Plane>,				//Plane,
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Mesh>,				//Mesh,
		//null,		// Spline
		&_UpdateShapeDescriptor<PopcornFX::CShapeDescriptor_Collection>,		//Collection
	};

	HH_STATIC_ASSERT(HH_ARRAY_COUNT(kCbNewDescriptors) == EPopcornFXAttribSamplerShapeType_Max);
	HH_STATIC_ASSERT(HH_ARRAY_COUNT(kCbUpdateShapeDescriptors) == EPopcornFXAttribSamplerShapeType_Max);
}

struct FAttributeSamplerShapeData
{
	PopcornFX::PParticleSamplerDescriptor_Shape		m_Desc;
	PopcornFX::PShapeDescriptor						m_Shape;
};

UPopcornFXAttributeSamplerShape::UPopcornFXAttributeSamplerShape(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, m_LastFrameUpdate(0)
{
	bAutoActivate = true;

#if WITH_EDITOR
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	m_IndirectSelectedThisTick = false;
#endif // WITH_EDITOR

	// UPopcornFXAttributeSampler override:
	m_SamplerType = EPopcornFXAttributeSamplerType::Shape;

	ShapeType = EPopcornFXAttribSamplerShapeType::Sphere;
	BoxDimension = FVector(100.f);
	Radius = 100.f;
	InnerRadius = 0.f;
	Height = 100.f;
	StaticMesh = null;
	StaticMeshSubIndex = 0;
	Weight = 1.0f;

	CollectionSamplingHeuristic = EPopcornFXShapeCollectionSamplingHeuristic::NoWeight;
	CollectionUseShapeWeights = 1;

	bUseRelativeTransform = true;

	m_Data = new FAttributeSamplerShapeData();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::BeginDestroy()
{
	Super::BeginDestroy();
	if (m_Data != null)
	{
		delete m_Data;
		m_Data = null;
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::SetRadius(float radius)
{
	Radius = radius;
	UpdateShapeProperties();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::SetWeight(float height)
{
	Height = height;
	UpdateShapeProperties();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::SetBoxDimension(FVector boxDimensions)
{
	BoxDimension = boxDimensions;
	UpdateShapeProperties();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::SetInnerRadius(float innerRadius)
{
	InnerRadius = innerRadius;
	UpdateShapeProperties();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::SetHeight(float height)
{
	Height = height;
	UpdateShapeProperties();
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerShape::CanUpdateShapeProperties()
{
	HH_ASSERT(m_Data != null);
	if (m_Data->m_Shape == null)
		return false;
	if (!HH_VERIFY(m_Data->m_Desc != null) ||
		!HH_VERIFY(m_Data->m_Desc->m_Shape != null) ||
		!HH_VERIFY(m_Data->m_Desc->m_Shape == m_Data->m_Shape) ||
		!HH_VERIFY(m_Data->m_Shape->ShapeType() == ToPkShapeType(ShapeType)))
	{
		// well, if it should not happen, better clear everything now
		m_Data->m_Shape = null;
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::UpdateShapeProperties()
{
	if (!CanUpdateShapeProperties())
		return;
	HH_ASSERT(m_Data->m_Shape->ShapeType() == ToPkShapeType(ShapeType));
	(*kCbUpdateShapeDescriptors[ShapeType.GetValue()])(SUpdateShapeParams{ this, m_Data->m_Shape.Get() });
	m_Data->m_Shape->m_Weight = Weight;
}

//----------------------------------------------------------------------------

#if WITH_EDITOR

// source_tree/hellheaven_tools/hh_components/HH-NativeEditor/src/NEdModuleConfig.cpp
// Shapes_BaseColor
static const FColor		kSamplerShapesDebugColor = FLinearColor(0.1f, 0.3f, 0.15f, 1.f).ToFColor(false);
static const FColor		kSamplerShapesDebugColorSelected = FLinearColor(0.2f, 0.5f, 0.75f, 1.f).ToFColor(false);
static const int32		kSamplerShapesDebugSegmentCount = 32;

void	UPopcornFXAttributeSamplerShape::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	UProperty	*propertyThatChanged = propertyChangedEvent.MemberProperty;
	if (propertyThatChanged != null)
	{
		// Don't update shapes because parent class properties were modified
		if (propertyThatChanged->GetOwnerClass() != UPopcornFXAttributeSamplerShape::StaticClass())
			return;
		if (!CanUpdateShapeProperties())
			return;
		const FString	propertyName = propertyThatChanged->GetName();
		if (propertyName == TEXT("Weight") ||
			propertyName == TEXT("BoxDimension") ||
			propertyName == TEXT("Radius") ||
			propertyName == TEXT("InnerRadius") ||
			propertyName == TEXT("Height") ||
			propertyName == TEXT("CollectionSamplingHeuristic") ||
			propertyName == TEXT("CollectionUseShapeWeights"))
			//(propertyName == TEXT("Shapes") && propertyChangedEvent.ChangeType != EPropertyChangeType::ArrayAdd)
		{
			HH_ASSERT(m_Data->m_Shape->ShapeType() == ToPkShapeType(ShapeType));
			(*kCbUpdateShapeDescriptors[ShapeType.GetValue()])(SUpdateShapeParams{this, m_Data->m_Shape.Get()});
			m_Data->m_Shape->m_Weight = Weight;
		}
		else
		{
			// invalidate shape
			m_Data->m_Desc = null;
			m_Data->m_Shape = null;
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);

}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction *thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	HH_ASSERT(GetWorld() != null);
	if (GetWorld()->IsGameWorld())
		return;

	bool				render = FPopcornFXPlugin::Get().SettingsEditor()->bAlwaysRenderAttributeSamplerShapes;

	const USelection	*selectedAssets = GEditor->GetSelectedActors();
	HH_ASSERT(selectedAssets != null);
	bool				isSelected = selectedAssets->IsSelected(GetOwner());
	if (isSelected || m_IndirectSelectedThisTick)
	{
		render = true;
		isSelected = true;
		m_IndirectSelectedThisTick = false;
	}

	if (!render)
		return;

	// Use the TickComponent to render the geometry, because _AttribSampler_PreUpdate()
	// isn't called on attr samplers that are not directly referenced (ie shape collection referencing other attr sampler shapes)
	// So they wont be rendered. Attr sampler collections manually rendering their subshapes isn't an option (might induce multiple draws for a single attr sampler)
	RenderShapeIFP(isSelected);
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::RenderShapeIFP(bool isSelected) const
{
	UWorld			*world = GetWorld();
	const FColor	debugColor = isSelected ? kSamplerShapesDebugColorSelected : kSamplerShapesDebugColor;
	const FVector	location = GetComponentTransform().GetLocation();
	const float		height = FGenericPlatformMath::Max(Height, 0.0f);
	const float		radius = FGenericPlatformMath::Max(Radius, 0.0f);
	const FVector	upVector = GetUpVector();
	switch (ShapeType)
	{
		case	EPopcornFXAttribSamplerShapeType::Box:
			DrawDebugBox(world, location, BoxDimension.GetClampedToSize(0.0f, BoxDimension.GetMax()) * 0.5f, GetComponentTransform().GetRotation(), debugColor);
			break;
		case	EPopcornFXAttribSamplerShapeType::Sphere:
			DrawDebugSphere(world, location, radius, kSamplerShapesDebugSegmentCount, debugColor);
			break;
		//case	EPopcornFXAttribSamplerShapeType::ComplexEllipsoid:
		case	EPopcornFXAttribSamplerShapeType::Cylinder:
			DrawDebugCylinder(GetWorld(), location - upVector * height * 0.5f, location + upVector * height * 0.5f, radius, kSamplerShapesDebugSegmentCount, debugColor);
			break;
		case	EPopcornFXAttribSamplerShapeType::Capsule:
			DrawDebugCapsule(world, location, (height / 2.0f) + radius, radius, GetComponentTransform().GetRotation(), debugColor);
			break;
		case	EPopcornFXAttribSamplerShapeType::Cone:
		{
			const float		sideLen = FGenericPlatformMath::Sqrt(height * height + radius * radius);
			const float		angle = FGenericPlatformMath::Atan(radius / height);
			DrawDebugCone(world, location + upVector * height, -upVector, sideLen, angle, angle, kSamplerShapesDebugSegmentCount, debugColor);
			break;
		}
		//case	EPopcornFXAttribSamplerShapeType::Spline:
		case	EPopcornFXAttribSamplerShapeType::Collection:
		{
			if (isSelected)
			{
				const hh_u32	subShapeCount = Shapes.Num();
				for (hh_u32 iSubShape = 0; iSubShape < subShapeCount; ++iSubShape)
				{
					const APopcornFXAttributeSamplerActor	*subShapeActor = Shapes[iSubShape];
					if (subShapeActor != null && subShapeActor->Sampler != null)
						subShapeActor->Sampler->_AttribSampler_IndirectSelectedThisTick();
				}
			}
			// if not select, the sub shape will render themselves
		}
		//case	EPopcornFXAttribSamplerShapeType::Sweep:
		default:
		{
			break;
		}
	}
}

#endif // WITH_EDITOR

//----------------------------------------------------------------------------

PopcornFX::CParticleSamplerDescriptor	*UPopcornFXAttributeSamplerShape::_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultDesc)
{
	check(m_Data != null);

	if (defaultDesc == null)
		return null;
	const PopcornFX::CParticleSamplerDescriptor_Shape	*defaultShapeDesc = static_cast<const PopcornFX::CParticleSamplerDescriptor_Shape*>(defaultDesc);
	const PopcornFX::CShapeDescriptor					*defaultShape = defaultShapeDesc->m_Shape.Get();
	if (defaultShape == null)
		return null;
	if (!HH_VERIFY(InitShape(defaultShape)))
		return null;
	desc.m_NeedUpdate = true;
	_AttribSampler_PreUpdate(null, 0.f, LEVELTICK_PauseTick);
	return m_Data->m_Desc.Get();
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerShape::InitShape(const PopcornFX::CShapeDescriptor *defaultShapeDesc)
{
	bool	needsInit = false;
	if (m_Data->m_Desc == null)
	{
		m_Data->m_Desc = HH_NEW(PopcornFX::CParticleSamplerDescriptor_Shape());
		needsInit = true;
	}
	PopcornFX::CParticleSamplerDescriptor_Shape	*shapeDesc = m_Data->m_Desc.Get();
	if (!HH_VERIFY(shapeDesc != null))
		return false;
	if (needsInit)
	{
		shapeDesc->m_WorldTr_Current = reinterpret_cast<const CFloat4x4*>(&(m_WorldTr_Current));
		shapeDesc->m_WorldTr_Previous = reinterpret_cast<CFloat4x4*>(&(m_WorldTr_Previous));
		shapeDesc->m_Angular_Velocity = reinterpret_cast<const CFloat3*>(&(m_Angular_Velocity));
		shapeDesc->m_Linear_Velocity = reinterpret_cast<CFloat3*>(&(m_Linear_Velocity));
	}

	if (m_Data->m_Shape != null)
	{
		if (HH_VERIFY(shapeDesc->m_Shape == m_Data->m_Shape) &&
			HH_VERIFY(m_Data->m_Shape->ShapeType() == ToPkShapeType(ShapeType)))
		{
			(*kCbUpdateShapeDescriptors[ShapeType])(SUpdateShapeParams{this, m_Data->m_Shape.Get()}); // Simply update the already existing shape
		}
		else
			m_Data->m_Shape = null; // something wrong happened, rebuild the shape
	}

	if (m_Data->m_Shape == null)
	{
		shapeDesc->m_Shape = null;
		m_Data->m_Shape = (*kCbNewDescriptors[ShapeType.GetValue()])(SNewShapeParams{this});
		if (!HH_VERIFY(m_Data->m_Shape != null))
			return false;
		m_Data->m_Shape->m_Weight = Weight;
		shapeDesc->m_Shape = m_Data->m_Shape;
	}
	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerShape::IsValid() const
{
	return m_Data != null && m_Data->m_Shape != null;
}

//----------------------------------------------------------------------------

PopcornFX::CShapeDescriptor	*UPopcornFXAttributeSamplerShape::GetShapeDescriptor() const
{
	HH_ASSERT(m_Data != null);
	return m_Data->m_Shape.Get();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerShape::_AttribSampler_PreUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerShape::_AttribSampler_PreUpdate", POPCORNFX_UE_PROFILER_COLOR);
	check(m_Data != null);

	//HH_ASSERT(m_LastFrameUpdate != GFrameCounter);
	if (m_LastFrameUpdate != GFrameCounter)
	{
		m_WorldTr_Previous = m_WorldTr_Current;
		m_LastFrameUpdate = GFrameCounter;
	}

	if (bUseRelativeTransform)
	{
		m_WorldTr_Current = GetRelativeTransform().ToMatrixNoScale();
	}
	else
	{
		m_WorldTr_Current = GetComponentTransform().ToMatrixNoScale();
	}

	const float	invGlobalScale = FPopcornFXPlugin::GlobalScaleRcp();
	if (IsCollection() && IsValid())
	{
		HH_ASSERT(m_Data->m_Desc != null);
		HH_ASSERT(m_Data->m_Desc->m_Shape != null);
		HH_ASSERT(m_Data->m_Desc->m_Shape.Get() == m_Data->m_Shape.Get());

		PopcornFX::CShapeDescriptor_Collection			*shape = static_cast<PopcornFX::CShapeDescriptor_Collection*>(m_Data->m_Shape.Get());
		PopcornFX::TMemoryView<const PCShapeDescriptor>	subShapes = shape->SubShapes();

		bool			collectionIsDirty = false;
		hh_u32			iSubShape = 0;
		const hh_u32	shapeCount = Shapes.Num();
		const hh_u32	subShapeCount = subShapes.Count();
		for (hh_u32 iShape = 0; iShape < shapeCount && iSubShape < subShapeCount; ++iShape)
		{
			const APopcornFXAttributeSamplerActor	*attr = Shapes[iShape];

			if (attr == null)
				continue;
			const UPopcornFXAttributeSamplerShape	*shapeComp = Cast<UPopcornFXAttributeSamplerShape>(attr->Sampler);
			if (shapeComp == null || shapeComp->IsCollection())
				continue;
			HH_TODO("Remove this Gore non-const cast when the popcornfx runtime is modified the proper way ")
			CShapeDescriptor	*desc = const_cast<CShapeDescriptor*>(subShapes[iSubShape].Get());
			HH_ASSERT(desc != null);
			if (desc != shapeComp->GetShapeDescriptor())
			{
				collectionIsDirty = true;
				break;
			}

			// Don't transform the world rotation
			const FVector	transformedLocation = GetComponentTransform().InverseTransformPositionNoScale(shapeComp->GetComponentLocation());
			FMatrix			localMatrix = FTransform(shapeComp->GetComponentRotation(), transformedLocation, FVector::ZeroVector).ToMatrixNoScale();

			localMatrix.M[3][0] *= invGlobalScale;
			localMatrix.M[3][1] *= invGlobalScale;
			localMatrix.M[3][2] *= invGlobalScale;
			desc->m_LocalTransforms = ToPk(localMatrix);
			++iSubShape;
		}
		if (collectionIsDirty)
			m_Data->m_Shape = null;
	}
	m_WorldTr_Current.M[3][0] *= invGlobalScale;
	m_WorldTr_Current.M[3][1] *= invGlobalScale;
	m_WorldTr_Current.M[3][2] *= invGlobalScale;

	HH_TODO("attribute angular linear velocity");
	m_Angular_Velocity = FVector(0);
	m_Linear_Velocity = ComponentVelocity;
}

//----------------------------------------------------------------------------
//
// UPopcornFXAttributeSamplerCurve
//
//----------------------------------------------------------------------------

#define CURVE_MINIMUM_DELTA	0.01f

//----------------------------------------------------------------------------

struct FAttributeSamplerCurveData
{
	PopcornFX::PParticleSamplerDescriptor_Curve	m_Desc;
	PopcornFX::PSamplerCurve					m_Curve0;
	PopcornFX::PSamplerCurve					m_Curve1;

	bool	m_NeedsReload;
};

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerCurve::SetCurveDimension(TEnumAsByte<EAttributeSamplerCurveDimension::Type> InCurveDimension)
{
	if (CurveDimension == InCurveDimension)
		return;
	SecondCurve1D = null;
	//SecondCurve2D = null;
	SecondCurve3D = null;
	SecondCurve4D = null;
	Curve1D = null;
	//Curve2D = null;
	Curve3D = null;
	Curve4D = null;
	CurveDimension = InCurveDimension;
	m_Data->m_NeedsReload = true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurve::SetCurve(class UCurveBase *InCurve, bool InIsSecondCurve)
{
	if (!HH_VERIFY(InCurve != null))
		return false;
	if (InIsSecondCurve)
	{
		SecondCurve1D = null;
		//SecondCurve2D = null;
		SecondCurve3D = null;
		SecondCurve4D = null;
	}
	else
	{
		Curve1D = null;
		//Curve2D = null;
		Curve3D = null;
		Curve4D = null;
	}
	bool			ok = false;
	switch (CurveDimension)
	{
	case	EAttributeSamplerCurveDimension::Float1:
		if (InIsSecondCurve)
			ok = ((SecondCurve1D = Cast<UCurveFloat>(InCurve)) != null);
		else
			ok = ((Curve1D = Cast<UCurveFloat>(InCurve)) != null);
		break;
	case	EAttributeSamplerCurveDimension::Float2:
		HH_ASSERT_NOT_REACHED();
		ok = false;
		break;
	case	EAttributeSamplerCurveDimension::Float3:
		if (InIsSecondCurve)
			ok = ((SecondCurve3D = Cast<UCurveVector>(InCurve)) != null);
		else
			ok = ((Curve3D = Cast<UCurveVector>(InCurve)) != null);
		break;
	case	EAttributeSamplerCurveDimension::Float4:
		if (InIsSecondCurve)
			ok = ((SecondCurve4D = Cast<UCurveLinearColor>(InCurve)) != null);
		else
			ok = ((Curve4D = Cast<UCurveLinearColor>(InCurve)) != null);
		break;
	}
	m_Data->m_NeedsReload = true;
	return ok;
}

//----------------------------------------------------------------------------

UPopcornFXAttributeSamplerCurve::UPopcornFXAttributeSamplerCurve(const FObjectInitializer &PCIP)
: Super(PCIP)
{
	bAutoActivate = true;
	bIsDoubleCurve = false;
	CurveDimension = EAttributeSamplerCurveDimension::Float1;

	// UPopcornFXAttributeSampler override:
	m_SamplerType = EPopcornFXAttributeSamplerType::Curve;

	Curve1D = null;
	Curve3D = null;
	Curve4D = null;

	m_Data = new FAttributeSamplerCurveData();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerCurve::BeginDestroy()
{
	if (m_Data != null)
	{
		if (m_Data->m_Desc != null)
		{
			m_Data->m_Desc->m_Curve0 = null;
			m_Data->m_Desc->m_Curve1 = null;
		}
		delete m_Data;
		m_Data = null;
	}
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

void	UPopcornFXAttributeSamplerCurve::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	UProperty	*propertyThatChanged = propertyChangedEvent.Property;

	if (propertyThatChanged != null)
	{
		// Always rebuild for now
		m_Data->m_NeedsReload = true;
	}
	Super::PostEditChangeProperty(propertyChangedEvent);
}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerCurve::FetchCurveData(const FRichCurve *curve, PopcornFX::CSamplerCurve *curveSampler, uint32 axis)
{
	const PopcornFX::TArray<float, PopcornFX::TArrayAligned16>	&times = curveSampler->m_Times;
	PopcornFX::TArray<float, PopcornFX::TArrayAligned16>		&values = curveSampler->m_FloatValues;
	PopcornFX::TArray<float, PopcornFX::TArrayAligned16>		&tangents = curveSampler->m_FloatTangents;
	TArray<FRichCurveKey>::TConstIterator						keyIt = curve->GetKeyIterator();

	bool			isLinear = false;
	float			previousTangent = 0.0f;
	float			previousValue = keyIt ? (*keyIt).Value - CURVE_MINIMUM_DELTA : 0.0f;
	const hh_u32	keyCount = times.Count();
	for (hh_u32 i = 0; i < keyCount; ++i)
	{
		const int32	index = i * CurveDimension;
		const int32	tangentIndex = index * 2 + axis;
		const float	delta = i > 0 ? times[i] - times[i - 1] : 0.0f;

		if (keyIt && times[i] == (*keyIt).Time)
		{
			const FRichCurveKey	&key = *keyIt++;

			values[index + axis] = key.Value;
			if (key.InterpMode == ERichCurveInterpMode::RCIM_Linear)
			{
				const float	nextValue = i + 1 < keyCount ? curve->Eval(times[i + 1]) : key.Value;
				const float	arriveTangent = key.Value - previousValue;
				const float	leaveTangent = nextValue - key.Value;

				tangents[tangentIndex] = isLinear ? arriveTangent : key.ArriveTangent * delta;
				tangents[tangentIndex + CurveDimension] = leaveTangent;
				previousTangent = leaveTangent;
				previousValue = key.Value;
			}
			else
			{
				const float	nextSampledValue = curve->Eval(times[i] + CURVE_MINIMUM_DELTA);
				const float	prevSampledValue = curve->Eval(times[i] - CURVE_MINIMUM_DELTA);
				const float	nextTime = i + 1 < times.Count() ? times[i + 1] : times[i] + CURVE_MINIMUM_DELTA;
				const float	nextDelta = nextTime - times[i];

				tangents[tangentIndex] = isLinear ? previousTangent : ((nextSampledValue - prevSampledValue) / (2.0f * CURVE_MINIMUM_DELTA)) * delta;
				tangents[tangentIndex + CurveDimension] = key.LeaveTangent * nextDelta;
			}
			isLinear = key.InterpMode == ERichCurveInterpMode::RCIM_Linear;
		}
		else
		{
			const float	value = curve->Eval(times[i]);

			// We do have to eval the key value because each axis can have independant
			// key count and times..
			values[index + axis] = value;
			if (isLinear)
			{
				const float	nextValue = i + 1 < keyCount ? curve->Eval(times[i + 1]) : value;
				const float	arriveTangent = value - previousValue;
				const float	leaveTangent = nextValue - value;

				tangents[tangentIndex] = arriveTangent;
				tangents[tangentIndex + CurveDimension] = leaveTangent;
				previousTangent = leaveTangent;
				previousValue = value;
			}
			else
			{
				const float	nextSampledValue = curve->Eval(times[i] + CURVE_MINIMUM_DELTA);
				const float	prevSampledValue = curve->Eval(times[i] - CURVE_MINIMUM_DELTA);
				const float	nextTime = i + 1 < times.Count() ? times[i + 1] : times[i] + CURVE_MINIMUM_DELTA;
				const float	nextDelta = nextTime - times[i];

				tangents[tangentIndex] = ((nextSampledValue - prevSampledValue) / (2.0f * CURVE_MINIMUM_DELTA)) * delta;
				tangents[tangentIndex + CurveDimension] = ((nextSampledValue - prevSampledValue) / (2.0f * CURVE_MINIMUM_DELTA)) * nextDelta;
			}
		}
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerCurve::GetAssociatedCurves(UCurveBase *&curve0, UCurveBase *&curve1)
{
	switch (CurveDimension)
	{
		case	EAttributeSamplerCurveDimension::Float1:
		{
			curve0 = Curve1D;
			curve1 = SecondCurve1D;
			break;
		}
		case	EAttributeSamplerCurveDimension::Float2:
		{
			HH_ASSERT_NOT_REACHED();
			break;
		}
		case	EAttributeSamplerCurveDimension::Float3:
		{
			curve0 = Curve3D;
			curve1 = SecondCurve3D;
			break;
		}
		case	EAttributeSamplerCurveDimension::Float4:
		{
			curve0 = Curve4D;
			curve1 = SecondCurve4D;
			break;
		}
	}
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurve::SetupCurve(PopcornFX::CSamplerCurve *curveSampler, UCurveBase *curve)
{
	static const float	kMaximumKey = 1.0f - CURVE_MINIMUM_DELTA;
	static const float	kMinimumKey = CURVE_MINIMUM_DELTA;

	TArray<FRichCurveEditInfo>	curves = curve->GetCurves();
	if (!HH_VERIFY(curves.Num() == CurveDimension))
		return false;

	curveSampler->SetOrder(CurveDimension);
	curveSampler->m_Interpolator = PopcornFX::CInterpolableVectorArray::Interpolator_Hermite;

	TArray<float>	collectedTimes;
	// Fetch every independant keys
	for (int32 i = 0; i < CurveDimension; ++i)
	{
		const FRichCurve	*fcurve = curves[i].CurveToEdit;
		HH_ASSERT(fcurve != null);
		for (auto keyIt = fcurve->GetKeyIterator(); keyIt; ++keyIt)
			collectedTimes.AddUnique((*keyIt).Time);
	}
	collectedTimes.RemoveAll([](const float& time) { return time > 1.0f; });
	collectedTimes.Sort();

	// Not enough keys: return (should handle this ? init curve at 0 ?)
	if (collectedTimes.Num() <= 0)
		return false;
	if (collectedTimes.Last() < kMaximumKey)
		collectedTimes.Add(1.0f);
	if (collectedTimes[0] > kMinimumKey)
		collectedTimes.Insert(0.0f, 0);

	// Resize the curve if necessary
	const hh_u32	keyCount = collectedTimes.Num();
	if (curveSampler->m_Times.Count() != keyCount)
	{
		if (!HH_VERIFY(curveSampler->Resize(keyCount)))
			return false;
	}
	PopcornFX::TArray<float, PopcornFX::TArrayAligned16>	&times = curveSampler->m_Times;
	HH_ASSERT(times.Count() == collectedTimes.Num());
	HH_ASSERT(times.RawDataPointer() != null);
	HH_ASSERT(collectedTimes.GetData() != null);
	memcpy(times.RawDataPointer(), collectedTimes.GetData(), keyCount * sizeof(float));

	float	minValues[4] = { 0 };
	float	maxValues[4] = { 0 };
	for (int32 i = 0; i < CurveDimension; ++i)
	{
		const FRichCurve	*fcurve = curves[i].CurveToEdit;
		HH_ASSERT(fcurve != null);

		fcurve->GetValueRange(minValues[i], maxValues[i]);
		FetchCurveData(fcurve, curveSampler, i);
	}
	curveSampler->m_MinEvalLimits = CFloat4(minValues[0], minValues[1], minValues[2], minValues[3]);
	curveSampler->m_MaxEvalLimits = CFloat4(maxValues[0], maxValues[1], maxValues[2], maxValues[3]);
	curveSampler->m_Interpolator = PopcornFX::CInterpolableVectorArray::Interpolator_Hermite;
	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurve::RebuildCurvesData()
{
	UCurveBase	*curve0 = null;
	UCurveBase	*curve1 = null;

	GetAssociatedCurves(curve0, curve1);
	if (curve0 == null || (bIsDoubleCurve && curve1 == null))
		return false;
	bool	success = true;

	success &= SetupCurve(m_Data->m_Curve0.Get(), curve0);
	success &= !bIsDoubleCurve || SetupCurve(m_Data->m_Curve1.Get(), curve1);
	return success;
}

//----------------------------------------------------------------------------

PopcornFX::CParticleSamplerDescriptor	*UPopcornFXAttributeSamplerCurve::_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultCurveDesc)
{
HH_TODO("Determine when this should be set to true : has the curve asset changed ?")
	m_Data->m_NeedsReload = true;
	if (m_Data->m_NeedsReload)
	{
		const PopcornFX::CParticleSamplerDescriptor_Curve	*pDesc = static_cast<const PopcornFX::CParticleSamplerDescriptor_Curve*>(defaultCurveDesc);

		const bool	defaultIsDoubleCurve = pDesc != null && pDesc->m_Curve1 != null;
		if (defaultIsDoubleCurve != bIsDoubleCurve)
			return null;
		if (m_Data->m_Curve0 == null)
			m_Data->m_Curve0 = HH_NEW(PopcornFX::CSamplerCurve());
		if (bIsDoubleCurve)
		{
			if (m_Data->m_Curve1 == null)
				m_Data->m_Curve1 = HH_NEW(PopcornFX::CSamplerCurve());
		}
		else if (m_Data->m_Curve1 != null)
			m_Data->m_Curve1 = null;
		if (!RebuildCurvesData())
			return null;
		m_Data->m_NeedsReload = false;
		if (m_Data->m_Desc == null)
			m_Data->m_Desc = HH_NEW(PopcornFX::CParticleSamplerDescriptor_Curve(m_Data->m_Curve0.Get(), m_Data->m_Curve1.Get()));
		HH_ASSERT(m_Data->m_Desc->m_Curve0 == m_Data->m_Curve0.Get());
		HH_ASSERT(m_Data->m_Desc->m_Curve1 == m_Data->m_Curve1.Get());
	}
	return m_Data->m_Desc.Get();
}

//----------------------------------------------------------------------------
//
// UPopcornFXAttributeSamplerCurveDynamic
//
//----------------------------------------------------------------------------

struct FAttributeSamplerCurveDynamicData
{
	PopcornFX::PParticleSamplerDescriptor_Curve	m_Desc;
	PopcornFX::PSamplerCurve	m_Curve0;

	PopcornFX::TArray<float>	m_Times;
	PopcornFX::TArray<float>	m_Tangents;
	PopcornFX::TArray<float>	m_Values;
	bool						m_DirtyValues;
};

//----------------------------------------------------------------------------

UPopcornFXAttributeSamplerCurveDynamic::UPopcornFXAttributeSamplerCurveDynamic(const FObjectInitializer &PCIP)
:	Super(PCIP)
{
	bAutoActivate = true;

	// UPopcornFXAttributeSampler override:
	m_SamplerType = EPopcornFXAttributeSamplerType::Curve;

	m_Data = new FAttributeSamplerCurveDynamicData();

	CurveDimension = EAttributeSamplerCurveDimension::Float1;
	CurveInterpolator = ECurveDynamicInterpolator::Linear;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerCurveDynamic::BeginDestroy()
{
	if (m_Data != null)
	{
		if (m_Data->m_Desc != null)
		{
			m_Data->m_Desc->m_Curve0 = null;
			m_Data->m_Desc->m_Curve1 = null;
		}
		delete m_Data;
		m_Data = null;
	}
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::CreateCurveIFN()
{
	HH_ASSERT(m_Data != null);
	if (m_Data->m_Curve0 == null)
	{
		m_Data->m_Curve0 = HH_NEW(PopcornFX::CSamplerCurve());
		if (HH_VERIFY(m_Data->m_Curve0 != null))
		{
			m_Data->m_DirtyValues = true;
			m_Data->m_Curve0->SetOrder((hh_u32)CurveDimension);
			m_Data->m_Curve0->m_Interpolator = CurveInterpolator == ECurveDynamicInterpolator::Linear ?
				PopcornFX::CInterpolableVectorArray::Interpolator_Linear :
				PopcornFX::CInterpolableVectorArray::Interpolator_Hermite;

			//m_Data->m_Curve0->SetEvalLimits(CFloat4(0.0f), CFloat4(1.0f));
		}
	}
	return m_Data->m_Curve0 != null;
}

//----------------------------------------------------------------------------

template <class _Type>
bool	UPopcornFXAttributeSamplerCurveDynamic::SetValuesGeneric(const TArray<_Type> &values)
{
	const hh_u32	valueCount = values.Num();
	if (valueCount < 2)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetValues, Values should contain at least 2 items."));
		return false;
	}
	if ((m_Data->m_Values.Count() / (hh_u32)CurveDimension) != values.Num())
	{
		if (!HH_VERIFY(m_Data->m_Values.Resize(valueCount * (hh_u32)CurveDimension)))
			return false;
	}
	PopcornFX::Mem::Copy(m_Data->m_Values.RawDataPointer(), values.GetData(), sizeof(float) * (hh_u32)CurveDimension * valueCount);
	m_Data->m_DirtyValues = true;
	return true;
}

//----------------------------------------------------------------------------

template <class _Type>
bool	UPopcornFXAttributeSamplerCurveDynamic::SetTangentsGeneric(const TArray<_Type> &arriveTangents, const TArray<_Type> &leaveTangents)
{
	if (arriveTangents.Num() != leaveTangents.Num())
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetTangents: ArriveTangents count differs from LeaveTangents"));
		return false;
	}
	const hh_u32	tangentCount = arriveTangents.Num();
	if (tangentCount < 2)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetValues, Values should contain at least 2 items."));
		return false;
	}
	const hh_u32	dim = (hh_u32)CurveDimension;
	if ((m_Data->m_Tangents.Count() / dim / 2) != tangentCount)
	{
		if (!HH_VERIFY(m_Data->m_Tangents.Resize(tangentCount * dim * 2)))
			return false;
	}
	const float	*prevTangents = reinterpret_cast<const float*>(arriveTangents.GetData());
	const float	*nextTangents = reinterpret_cast<const float*>(leaveTangents.GetData());
	float		*dstTangents = m_Data->m_Tangents.RawDataPointer();

	for (hh_u32 iTangent = 0; iTangent < tangentCount; ++iTangent)
	{
		for (hh_u32 iDim = 0; iDim < dim; ++iDim)
			*dstTangents++ = *prevTangents++;
		for (hh_u32 iDim = 0; iDim < dim; ++iDim)
			*dstTangents++ = *nextTangents++;
	}
	m_Data->m_DirtyValues = true;
	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetTimes(const TArray<float> &Times)
{
	HH_ASSERT(m_Data != null);

	HH_ONLY_IF_ASSERTS(
		for (hh_i32 iTime = 0; iTime < Times.Num(); ++iTime)
			HH_ASSERT(Times[iTime] >= 0.0f && Times[iTime] <= 1.0f);
	);

	if (Times.Num() < 2)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetTimes: Times should contain at least 2 items."));
		return false;
	}
	const hh_u32	timesCount = Times.Num();
	if (HH_VERIFY(m_Data->m_Times.Resize(timesCount)))
	{
		m_Data->m_DirtyValues = true;
		PopcornFX::Mem::Copy(m_Data->m_Times.RawDataPointer(), Times.GetData(), sizeof(float) * timesCount);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetValues1D(const TArray<float> &Values)
{
	HH_ASSERT(m_Data != null);
	if (CurveDimension != EAttributeSamplerCurveDimension::Float1)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetValues1D: Curve doesn't have Float1 dimension"));
		return false;
	}
	return SetValuesGeneric<float>(Values);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetValues3D(const TArray<FVector> &Values)
{
	HH_ASSERT(m_Data != null);
	if (CurveDimension != EAttributeSamplerCurveDimension::Float3)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetValues3D: Curve doesn't have Float3 dimension"));
		return false;
	}
	return SetValuesGeneric<FVector>(Values);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetValues4D(const TArray<FLinearColor> &Values)
{
	HH_ASSERT(m_Data != null);
	if (CurveDimension != EAttributeSamplerCurveDimension::Float4)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetValues4D: Curve doesn't have Float4 dimension"));
		return false;
	}
	return SetValuesGeneric<FLinearColor>(Values);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetTangents1D(const TArray<float> &ArriveTangents, const TArray<float> &LeaveTangents)
{
	HH_ASSERT(m_Data != null);
	if (CurveInterpolator != ECurveDynamicInterpolator::Spline)
		return true; // No need to set tangents
	if (CurveDimension != EAttributeSamplerCurveDimension::Float1)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetTangents1D: Curve doesn't have Float1 dimension"));
		return false;
	}
	return SetTangentsGeneric<float>(ArriveTangents, LeaveTangents);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetTangents3D(const TArray<FVector> &ArriveTangents, const TArray<FVector> &LeaveTangents)
{
	HH_ASSERT(m_Data != null);
	if (CurveInterpolator != ECurveDynamicInterpolator::Spline)
		return true; // No need to set tangents
	if (CurveDimension != EAttributeSamplerCurveDimension::Float3)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetTangents3D: Curve doesn't have Float3 dimension"));
		return false;
	}
	return SetTangentsGeneric<FVector>(ArriveTangents, LeaveTangents);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerCurveDynamic::SetTangents4D(const TArray<FLinearColor> &ArriveTangents, const TArray<FLinearColor> &LeaveTangents)
{
	HH_ASSERT(m_Data != null);
	if (CurveInterpolator != ECurveDynamicInterpolator::Spline)
		return true; // No need to set tangents
	if (CurveDimension != EAttributeSamplerCurveDimension::Float4)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't SetTangents4D: Curve doesn't have Float4 dimension"));
		return false;
	}
	return SetTangentsGeneric<FLinearColor>(ArriveTangents, LeaveTangents);
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerCurveDynamic::_AttribSampler_PreUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType)
{
	HH_ASSERT(m_Data != null);

	PopcornFX::PSamplerCurve	curve = m_Data->m_Curve0;
	if (curve == null || !m_Data->m_DirtyValues)
		return;
	if (m_Data->m_Values.Empty())
		return;

	m_Data->m_DirtyValues = false;

	const bool	hasTimes = !m_Data->m_Times.Empty();
	const bool	hasTangents = !m_Data->m_Tangents.Empty();
	if (!hasTimes)
	{
		// Generate the times based on the value count (minus one: the first value is at time 0.0f, and the last is at time 1.0f)
		const hh_u32	timesCount = m_Data->m_Values.Count() / (hh_u32)CurveDimension;
		if (!HH_VERIFY(m_Data->m_Times.Resize(timesCount)))
			return;
		
		float		*times = m_Data->m_Times.RawDataPointer();
		const float	step = 1.0f / (timesCount - 1);
		for (hh_u32 iTime = 0; iTime < timesCount; ++iTime)
			*times++ = step * iTime;
	}

	if (!CreateCurveIFN())
		return;
	const hh_u32	valueCount = m_Data->m_Times.Count();
	if (!HH_VERIFY(curve->Resize(valueCount)))
		return;

	if ((hasTangents && m_Data->m_Tangents.Count() / 2 != m_Data->m_Values.Count()) ||
		(hasTimes && m_Data->m_Times.Count() != m_Data->m_Values.Count() / (hh_u32)CurveDimension))
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Tangents/Times count differs from values. Make sure to set the same number of values/tangents/times"));
		return;
	}
	PopcornFX::Mem::Copy(curve->m_Times.RawDataPointer(), m_Data->m_Times.RawDataPointer(), sizeof(float) * valueCount);
	PopcornFX::Mem::Copy(curve->m_FloatValues.RawDataPointer(), m_Data->m_Values.RawDataPointer(), sizeof(float) * (hh_u32)CurveDimension * valueCount);

	if (CurveInterpolator == ECurveDynamicInterpolator::Spline)
	{
		const hh_u32	tangentsByteCount = sizeof(float) * (hh_u32)CurveDimension * 2 * valueCount;

		if (hasTangents)
			PopcornFX::Mem::Copy(curve->m_FloatTangents.RawDataPointer(), m_Data->m_Tangents.RawDataPointer(), tangentsByteCount);
		else
			PopcornFX::Mem::Clear(curve->m_FloatTangents.RawDataPointer(), tangentsByteCount);
	}
}

//----------------------------------------------------------------------------

PopcornFX::CParticleSamplerDescriptor	*UPopcornFXAttributeSamplerCurveDynamic::_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultCurveDesc)
{
	HH_ASSERT(m_Data != null);
	const PopcornFX::CParticleSamplerDescriptor_Curve	*pDesc = static_cast<const PopcornFX::CParticleSamplerDescriptor_Curve*>(defaultCurveDesc);

	if (pDesc == null)
		return null;
	const PopcornFX::CCurveDescriptor	*defaultCurve = static_cast<const PopcornFX::CCurveDescriptor*>(pDesc->m_Curve0);
	if (defaultCurve == null)
		return null;
	const bool	defaultIsDoubleCurve = pDesc != null && pDesc->m_Curve1 != null;
	if (defaultIsDoubleCurve)
	{
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("Couldn't setup UPopcornFXAttributeSamplerCurveDynamic: Source curve is DoubleCurve, not supported by dynamic curve attr sampler."));
		return null;
	}
	if (!CreateCurveIFN())
		return null;
	if (m_Data->m_Desc == null)
		m_Data->m_Desc = HH_NEW(PopcornFX::CParticleSamplerDescriptor_Curve(m_Data->m_Curve0.Get(), null));
	HH_ASSERT(m_Data->m_Desc->m_Curve0 == m_Data->m_Curve0.Get());
	desc.m_NeedUpdate = true;
	return m_Data->m_Desc.Get();
}

//----------------------------------------------------------------------------
//
// UPopcornFXAttributeSamplerText
//
//----------------------------------------------------------------------------

struct FAttributeSamplerTextData
{
	PopcornFX::PParticleSamplerDescriptor_Text	m_Desc;

	bool	m_NeedsReload;
};

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerText::SetText(FString InText)
{
	m_Data->m_NeedsReload = true;
	Text = InText;
}

//----------------------------------------------------------------------------

UPopcornFXAttributeSamplerText::UPopcornFXAttributeSamplerText(const FObjectInitializer &PCIP)
: Super(PCIP)
{
	bAutoActivate = true;

	Text = "";
	// UPopcornFXAttributeSampler override:
	m_SamplerType = EPopcornFXAttributeSamplerType::Text;

	m_Data = new FAttributeSamplerTextData();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerText::BeginDestroy()
{
	if (m_Data != null)
	{
		delete m_Data;
		m_Data = null;
	}
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------

#if WITH_EDITOR

void	UPopcornFXAttributeSamplerText::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	const UProperty	*propertyThatChanged = propertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerText, Text))
			m_Data->m_NeedsReload = true;
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}

#endif // WITH_EDITOR

//----------------------------------------------------------------------------

PopcornFX::CParticleSamplerDescriptor	*UPopcornFXAttributeSamplerText::_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultTextDesc)
{
	if (m_Data->m_Desc == null)
	{
		m_Data->m_Desc = HH_NEW(PopcornFX::CParticleSamplerDescriptor_Text());
		if (!HH_VERIFY(m_Data->m_Desc != null))
			return null;
		m_Data->m_NeedsReload = true;
	}
	if (m_Data->m_NeedsReload)
	{
		// @TODO kerning
		PopcornFX::CFontMetrics		*fontMetrics = null;
		bool						useKerning = false;
		if (!HH_VERIFY(m_Data->m_Desc->_Setup(TCHAR_TO_ANSI(*Text), fontMetrics, useKerning)))
			return null;
		m_Data->m_NeedsReload = false;
	}
	return m_Data->m_Desc.Get();
}

//----------------------------------------------------------------------------
//
// UPopcornFXAttributeSamplerImage
//
//----------------------------------------------------------------------------

struct FAttributeSamplerImageData
{
	PopcornFX::PParticleSamplerDescriptor_Image		m_Desc;
	PopcornFX::CImageSampler						*m_ImageSampler = null;
	PopcornFX::CImageSamplerInterface::EFilterType	m_ImageSamplerFilter;
	PopcornFX::TResourcePtr<PopcornFX::CImage>		m_TextureResource;

	bool		m_ReloadTexture = false;

	void		Clear()
	{
		m_Desc = null;
		m_TextureResource.Clear();
	}

	FAttributeSamplerImageData()
	{ }

	~FAttributeSamplerImageData()
	{
		if (m_ImageSampler != null)
		{
			HH_DELETE(m_ImageSampler);
			m_ImageSampler = null;
		}
	}

};

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerImage::SetTexture(class UTexture2D *InTexture)
{
	Texture = InTexture;
	m_Data->m_ReloadTexture = true;
}

//----------------------------------------------------------------------------

UPopcornFXAttributeSamplerImage::UPopcornFXAttributeSamplerImage(const FObjectInitializer &PCIP)
	: Super(PCIP)
{
	bAutoActivate = true;

	bAllowTextureConvertionAtRuntime = false;

	Texture = null;
	// UPopcornFXAttributeSampler override:
	m_SamplerType = EPopcornFXAttributeSamplerType::Image;

	m_Data = new FAttributeSamplerImageData();
	check(m_Data != null);
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerImage::BeginDestroy()
{
	if (m_Data != null)
	{
		delete m_Data;
		m_Data = null;
	}
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------

#if WITH_EDITOR

void	UPopcornFXAttributeSamplerImage::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	const UProperty	*propertyThatChanged = propertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		const FString	propertyName = propertyThatChanged->GetName();

		if (propertyName == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerImage, Texture) ||
			propertyName == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerImage, bAllowTextureConvertionAtRuntime))
		{
			m_Data->m_ReloadTexture = true;
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}

#endif // WITH_EDITOR

//----------------------------------------------------------------------------

PopcornFX::CParticleSamplerDescriptor	*UPopcornFXAttributeSamplerImage::_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultImageDesc)
{
	const PopcornFX::CParticleSamplerDescriptor_Image	*imageDesc = static_cast<const PopcornFX::CParticleSamplerDescriptor_Image*>(defaultImageDesc);
	if (imageDesc == null)
		return null;
	if (!/*HH_VERIFY*/(RebuildImageSampler(imageDesc)))
	{
		const FString	imageName = Texture != null ? Texture->GetName() : FString(TEXT("null"));
		UE_LOG(LogPopcornFXAttributeSampler, Warning, TEXT("AttrSamplerImage: Failed to setup texture '%s' in '%s'"), *imageName, *GetPathName());
		return null;
	}
	return m_Data->m_Desc.Get();
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerImage::RebuildImageSampler(const PopcornFX::CParticleSamplerDescriptor_Image *desc)
{
	if (!_RebuildImageSampler(desc))
	{
		m_Data->Clear();
		return false;
	}
	return true;
}

bool	UPopcornFXAttributeSamplerImage::_RebuildImageSampler(const PopcornFX::CParticleSamplerDescriptor_Image	*desc)
{
	if (Texture == null)
		return false;

	bool		samplerChanged = false;

	if (m_Data->m_ImageSampler == null ||
		m_Data->m_ImageSamplerFilter != desc->m_DefaultFilter)
	{
		if (m_Data->m_ImageSampler != null)
		{
			HH_DELETE(m_Data->m_ImageSampler);
			m_Data->m_ImageSampler = null;
		}

		m_Data->m_ImageSamplerFilter = desc->m_DefaultFilter;
		if (desc->m_DefaultFilter == PopcornFX::CImageSamplerInterface::FilterType_Point)
			m_Data->m_ImageSampler = HH_NEW(PopcornFX::CImageSampler);
		else if (desc->m_DefaultFilter == PopcornFX::CImageSamplerInterface::FilterType_Linear)
			m_Data->m_ImageSampler = HH_NEW(PopcornFX::CImageSamplerBilinear);
		else
		{
			HH_ASSERT_NOT_REACHED();
			return false;
		}
		samplerChanged = true;
	}

	if (m_Data->m_ReloadTexture ||
		m_Data->m_TextureResource == null)
	{
		m_Data->m_ReloadTexture = false;

		const PopcornFX::CString	fullPath = TCHAR_TO_ANSI(*Texture->GetPathName());
		const bool					reloadImage = false;
		m_Data->m_TextureResource = PopcornFX::Resource::DefaultManager()->Load<PopcornFX::CImage>(fullPath, true, reloadImage);
		if (m_Data->m_TextureResource == null)
			return false;
		if (!HH_VERIFY(!m_Data->m_TextureResource->m_Frames.Empty()) ||
			!HH_VERIFY(!m_Data->m_TextureResource->m_Frames[0].m_Mipmaps.Empty()))
			return false;

		PopcornFX::CImageSurface	surface(m_Data->m_TextureResource->m_Frames[0].m_Mipmaps[0], m_Data->m_TextureResource->m_Format);
		HH_ASSERT(m_Data->m_ImageSampler != null); // see above
		if (!/*HH_VERIFY*/(m_Data->m_ImageSampler->SetupFromSurface(surface)))
		{
			if (bAllowTextureConvertionAtRuntime)
			{
				const PopcornFX::CImage::EFormat		dstFormat = PopcornFX::CImage::Format_BGRA8;
				UE_LOG(LogPopcornFXAttributeSampler, Log,
					TEXT("AttrSamplerImage: texture '%s' format %s not supported for sampling, converting to %s (because AllowTextureConvertionAtRuntime) in '%s'"),
					*Texture->GetName(),
					ANSI_TO_TCHAR(PopcornFX::CImage::GetFormatName(m_Data->m_TextureResource->m_Format)),
					ANSI_TO_TCHAR(PopcornFX::CImage::GetFormatName(dstFormat)),
					*GetPathName());

				PopcornFX::CImageSurface	newSurface;
				newSurface.m_Format = dstFormat;
				if (!newSurface.CopyAndConvertIFN(surface))
				{
					UE_LOG(LogPopcornFXAttributeSampler, Warning,
						TEXT("AttrSamplerImage: could not convert texture '%s' from %s to %s in %s"),
						*Texture->GetName(),
						ANSI_TO_TCHAR(PopcornFX::CImage::GetFormatName(m_Data->m_TextureResource->m_Format)),
						ANSI_TO_TCHAR(PopcornFX::CImage::GetFormatName(dstFormat)),
						*GetPathName());
					return false;
				}
				if (!HH_VERIFY(m_Data->m_ImageSampler->SetupFromSurface(newSurface)))
				{
					return false;
				}
			}
			else
			{
				UE_LOG(LogPopcornFXAttributeSampler, Warning,
					TEXT("AttrSamplerImage: texture '%s' format %s not supported for sampling (and AllowTextureConvertionAtRuntime not enabled) in %s"),
					*Texture->GetName(),
					ANSI_TO_TCHAR(PopcornFX::CImage::GetFormatName(m_Data->m_TextureResource->m_Format)),
					*GetPathName());
				return false;
			}
		}
	}

	if (m_Data->m_Desc == null)
	{
		m_Data->m_Desc = HH_NEW(PopcornFX::CParticleSamplerDescriptor_Image(m_Data->m_ImageSampler));
		if (!HH_VERIFY(m_Data->m_Desc != null))
			return false;
	}
	else if (samplerChanged)
	{
		m_Data->m_Desc->m_Sampler = m_Data->m_ImageSampler;
	}
	m_Data->m_Desc->m_DefaultFilter = desc->m_DefaultFilter;
	m_Data->m_Desc->m_DefaultTCMode = desc->m_DefaultTCMode;
	HH_ASSERT(m_Data->m_Desc->m_Sampler == m_Data->m_ImageSampler);

	return true;
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
