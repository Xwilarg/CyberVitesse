//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXSceneComponent.h"

#include "PopcornFXPlugin.h"
#include "Render/RendererSubView.h"
#include "PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"
#include "World/PopcornFXWaitForSceneActor.h"
#include "PopcornFXSceneActor.h"
#include "PopcornFXStats.h"

#include "Engine/CollisionProfile.h"

#include "PopcornFXSDK.h"
#include <pk_drawers/include/dr_scene.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXSceneComponent, Log, All);

//----------------------------------------------------------------------------
//
// UPopcornFXSceneComponent
//
//----------------------------------------------------------------------------

UPopcornFXSceneComponent::UPopcornFXSceneComponent(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, m_ParticleScene(null)
{
	const float			extent = 500;
	FixedRelativeBoundingBox = FBox(FVector(-extent), FVector(extent));

	PrimaryComponentTick.bCanEverTick = true;

	// Attr sampler skeletal mesh is executed:
	// - TG_PrePhysics for SkeletalMeshComponents
	// - TG_EndPhysics for DestructibleComponents

	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;

	PrimaryComponentTick.bRunOnAnyThread = false;
	// Needs thread register to PopcornFX and more safety on Emitters
	//PrimaryComponentTick.bRunOnAnyThread = true;

	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	bTickInEditor = true;
	//MaxTimeBeforeForceUpdateTransform = 5.0f;
	bAutoActivate = true;

	// If we are selectable, selecting a particle will select us (the Scene) instead of the Emitter
	bSelectable = false;

	bAllowConcurrentTick = true;

	bGenerateOverlapEvents = false;
	bReceivesDecals = false;

	bCastVolumetricTranslucentShadow = true;
	//bCastVolumetricTranslucentShadow = false;

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	bEnableUpdate = true;
	bEnableRender = true;

	HeavyDebugMode = EPopcornFXHeavyDebugMode::None;
	bRender_FreezeBillboardingMatrix = false;
	bRender_OverrideColorByDrawCall = false;
	Render_OverrideDebugColorCount = 6;
	Render_OverrideAllMaterial = null;

	struct FConstructorStatics
	{
		FName		DefaultSceneName;
		FConstructorStatics()
			: DefaultSceneName(TEXT("PopcornFX_DefaultScene"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	SceneName = ConstructorStatics.DefaultSceneName;

	//DefaultCollisionFilter = CreateDefaultSubobject<FPopcornFXCollisionFilter>(TEXT("DefaultCollisionFilter"));

	SetFlags(RF_Transactional); // Allow this actor's deletion
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::OnUnregister()
{
	if (m_OnSettingsChangedHandle.IsValid())
	{
		FPopcornFXPlugin::Get().m_OnSettingsChanged.Remove(m_OnSettingsChangedHandle);
		m_OnSettingsChangedHandle.Reset();
	}

	// Called each time a property is modified !
	// ! So, do not delete here !
	//CParticleScene::SafeDelete(m_ParticleScene);
	//m_ParticleScene = null;

	Super::OnUnregister();
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::BeginDestroy()
{
	CParticleScene::SafeDelete(m_ParticleScene);
	m_ParticleScene = null;
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::PostLoad()
{
	ResolveSettings();
	Super::PostLoad();
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

void	UPopcornFXSceneComponent::PostEditChangeProperty(FPropertyChangedEvent &propertyChangedEvent)
{
	static FName		SimulationSettingsOverrideName(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSceneComponent, SimulationSettingsOverride));
	static FName		RenderSettingsOverrideName(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSceneComponent, RenderSettingsOverride));
	if (propertyChangedEvent.MemberProperty != null)
	{
		const FName		memberName = propertyChangedEvent.MemberProperty->GetFName();
		if (memberName == SimulationSettingsOverrideName ||
			memberName == RenderSettingsOverrideName)
			ResolveSettings();
	}
	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::MirrorGameWorldProperties(const UPopcornFXSceneComponent *other)
{
	HH_ASSERT(other != null);
	HH_ASSERT(other->GetWorld() != null);
	HH_ASSERT(other->GetWorld()->IsGameWorld() || other->GetWorld()->WorldType == EWorldType::Editor);

	SceneName = other->SceneName;
	bEnableUpdate = other->bEnableUpdate;
	bEnableRender = other->bEnableRender;
	BoundingBoxMode = other->BoundingBoxMode;
	FixedRelativeBoundingBox = other->FixedRelativeBoundingBox;
	SimulationSettingsOverride = other->SimulationSettingsOverride;
	RenderSettingsOverride = other->RenderSettingsOverride;

	//HeavyDebugMode = other->HeavyDebugMode;
	//bDebugFreezeBillboardingMatrix = other->bDebugFreezeBillboardingMatrix;

}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::OnRegister()
{
	Super::OnRegister();

	SetTickGroup(m_ResolvedSimulationSettings.SceneUpdateTickGroup.GetValue());

	if (m_ParticleScene == null && !IsTemplate() && !IsRunningCommandlet())
	{
		m_ParticleScene = CParticleScene::CreateNew(this);
	}
	if (m_ParticleScene != null)
	{
		HH_ASSERT(m_ParticleScene->SceneComponent() == this);
		if (m_ResolvedSimulationSettings.bEnableEventListeners)
			m_ParticleScene->RegisterFieldListeners();
	}

#if WITH_EDITOR
	// Pull all Emitters that have been registered before us
	APopcornFXWaitForScene::NotifySceneLoaded(this);
#endif
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXSceneComponent::TickComponent", POPCORNFX_UE_PROFILER_COLOR);
	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_PopcornFXUpdateTime);
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_ParticleScene == null || bEnableUpdate == 0 || deltaTime == 0)
		return;

	if (!m_OnSettingsChangedHandle.IsValid())
	{
		m_OnSettingsChangedHandle = FPopcornFXPlugin::Get().m_OnSettingsChanged.AddUObject(this, &UPopcornFXSceneComponent::_OnSettingsChanged);
	}

	HH_ASSERT(m_ParticleScene->SceneComponent() == this);

	m_ParticleScene->StartUpdate(deltaTime, tickType);

	FBoxSphereBounds			bounds;

	if (BoundingBoxMode == EPopcornFXSceneBBMode::Dynamic)
	{
		bounds = m_ParticleScene->Bounds();
	}
	else if (BoundingBoxMode == EPopcornFXSceneBBMode::DynamicPlusFixed)
	{
		bounds = Union(FBoxSphereBounds(FixedRelativeBoundingBox.ShiftBy(GetComponentLocation())), m_ParticleScene->Bounds());
	}
	else if (BoundingBoxMode == EPopcornFXSceneBBMode::Fixed)
		bounds = FixedRelativeBoundingBox.ShiftBy(GetComponentLocation());

	if (bounds.SphereRadius <= 0.001f)
	{
		FBoxSphereBounds		selfLittleBounds(GetComponentLocation(), FVector(100.f), 100.f);
		bounds = Union(bounds, selfLittleBounds);
	}

	if (bounds.Origin != Bounds.Origin || bounds.SphereRadius != Bounds.SphereRadius)
	{
		Bounds = bounds;
		UpdateBounds();
		UpdateComponentToWorld();
	}

	HH_FIXME("MarkRenderDynamicDataDirty only if needed");
	MarkRenderDynamicDataDirty();

	if (m_ParticleScene->PostUpdate_ShouldMarkRenderStateDirty())
		MarkRenderStateDirty();

	//UpdateComponentToWorld();
	//MarkRenderStateDirty();
	//Super::MarkRenderDynamicDataDirty();
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::SendRenderDynamicData_Concurrent()
{
	SCOPE_CYCLE_COUNTER(STAT_PopcornFX_SRDDTime);

	Super::SendRenderDynamicData_Concurrent();

	if (m_ParticleScene == null)
		return;
	m_ParticleScene->SendRenderDynamicData_Concurrent();
}

void	UPopcornFXSceneComponent::CreateRenderState_Concurrent()
{
	Super::CreateRenderState_Concurrent();

	SendRenderDynamicData_Concurrent();
}

//----------------------------------------------------------------------------

FBoxSphereBounds	UPopcornFXSceneComponent::CalcBounds(const FTransform & LocalToWorld) const
{
	return Bounds;
	//FBox SimBounds;
	//SimBounds.Min = FVector(-HALF_WORLD_MAX, -HALF_WORLD_MAX, -HALF_WORLD_MAX);
	//SimBounds.Max = FVector(+HALF_WORLD_MAX, +HALF_WORLD_MAX, +HALF_WORLD_MAX);
	//return FBoxSphereBounds(SimBounds);
}

//----------------------------------------------------------------------------

FPrimitiveSceneProxy	*UPopcornFXSceneComponent::CreateSceneProxy()
{
	FPopcornFXSceneProxy *sceneProxy = new FPopcornFXSceneProxy(this);
	return sceneProxy;
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 15)
void	UPopcornFXSceneComponent::GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const
{
	Super::GetUsedMaterials(OutMaterials, bGetDebugMaterials);
	if (m_ParticleScene == null)
		return;
	m_ParticleScene->GetUsedMaterials(OutMaterials, bGetDebugMaterials);
}
#endif // (ENGINE_MINOR_VERSION >= 15)

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::ResolveSettings()
{
	SimulationSettingsOverride.ResolveSettingsTo(m_ResolvedSimulationSettings);
	RenderSettingsOverride.ResolveSettingsTo(m_ResolvedRenderSettings);
}


//----------------------------------------------------------------------------
//
// Blueprint functions
//
//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::Clear()
{
	if (!HH_VERIFY(m_ParticleScene != null))
		return;
	m_ParticleScene->Clear();
}

//----------------------------------------------------------------------------

bool	UPopcornFXSceneComponent::RegisterEventListener(FPopcornFXRaiseEventSignature Delegate, FName EventName)
{
	if (!HH_VERIFY(m_ParticleScene != null))
		return false;
	if (!m_ResolvedSimulationSettings.bEnableEventListeners)
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Register Event Listener: bEnableEventListeners is set to false"));
		return false;
	}
	const PopcornFX::CStringId	eventNameId = PopcornFX::CStringId(TCHAR_TO_ANSI(*EventName.ToString()));
	if (eventNameId.Empty())
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Register Event Listener: Empty EventName"));
		return false;
	}
	if (!Delegate.IsBound())
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Register Event Listener: Invalid delegate"));
		return false;
	}
	return m_ParticleScene->RegisterEventListener(eventNameId, Delegate);
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::UnregisterEventListener(FPopcornFXRaiseEventSignature Delegate, FName EventName)
{
	if (!HH_VERIFY(m_ParticleScene != null))
		return;
	const PopcornFX::CStringId	eventNameId = PopcornFX::CStringId(TCHAR_TO_ANSI(*EventName.ToString()));
	if (eventNameId.Empty())
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Unregister Event Listener: Empty EventName"));
		return;
	}
	if (!Delegate.IsBound())
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Unregister Event Listener: Invalid delegate"));
		return;
	}
	m_ParticleScene->UnregisterEventListener(eventNameId, Delegate);
}

//----------------------------------------------------------------------------

bool	UPopcornFXSceneComponent::GetFieldValue(const FName &fieldName, void *outValue, EPopcornFXParticleFieldType::Type expectedFieldType) const
{
	if (!HH_VERIFY(m_ParticleScene != null))
		return false;
	if (!m_ResolvedSimulationSettings.bEnableEventListeners)
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Get Particle Field: bEnableEventListeners is set to false"));
		return false;
	}
	if (fieldName.IsNone() || !fieldName.IsValid())
	{
		UE_LOG(LogPopcornFXSceneComponent, Warning, TEXT("Get Particle Field: Invalid FieldName"));
		return false;
	}
	return m_ParticleScene->GetFieldValue(fieldName, outValue, expectedFieldType);
}

//----------------------------------------------------------------------------

void	UPopcornFXSceneComponent::SetAudioSamplingInterface(IPopcornFXFillAudioBuffers *fillAudioBuffers)
{
	HH_ASSERT(IsInGameThread());
	if (!HH_VERIFY(m_ParticleScene != null))
		return;
	m_ParticleScene->m_FillAudioBuffers = fillAudioBuffers;
}

//----------------------------------------------------------------------------
