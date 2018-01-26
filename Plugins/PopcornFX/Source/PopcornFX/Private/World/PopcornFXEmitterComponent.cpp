//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXEmitterComponent.h"

#include "PopcornFXPlugin.h"
#include "Assets/PopcornFXEffect.h"
#include "PopcornFXSceneComponent.h"
#include "PopcornFXSceneActor.h"
#include "PopcornFXEmitter.h"
#include "Internal/ParticleScene.h"
#include "Attributes/PopcornFXAttributeList.h"
#include "Render/RendererSubView.h"
#include "World/PopcornFXWaitForSceneActor.h"

#include "MessageLog.h"
#include "UObjectToken.h"
#include "MapErrors.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "UObject/UObjectIterator.h"
#if WITH_EDITOR
#	include "Engine/Selection.h"
#	include "Editor.h"
#endif

#include "PopcornFXSDK.h"
#include <hh_engine_utils/include/kr_actions.h>
#include <hh_particles/include/ps_descriptor.h>
#include <hh_particles/include/ps_actions.h>

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "FPopcornFXEmitterComponent"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXEmitterComponent, Log, All);

//----------------------------------------------------------------------------

// static
UPopcornFXEmitterComponent	*UPopcornFXEmitterComponent::CreateStandaloneEmitterComponent(UPopcornFXEffect* effect, APopcornFXSceneActor *scene, UWorld* world, AActor* actor, bool bAutoDestroy)
{
	UObject							*outer = (actor ? actor : static_cast<UObject*>(world));
	HH_ASSERT(outer != null);
	UPopcornFXEmitterComponent		*psc = NewObject<UPopcornFXEmitterComponent>(outer);
	if (psc != null)
	{
		psc->bEnableUpdates = false;
		psc->bPlayOnLoad = false;

		psc->bAutoDestroy = bAutoDestroy;
		//psc->SecondsBeforeInactive = 0.0f;
		psc->bAutoActivate = false;
		if (scene != null)
		{
			psc->SceneName = scene->GetSceneName();
			psc->Scene = scene;
		}
		psc->Effect = effect;
		//psc->bOverrideLODMethod = false;

		psc->RegisterComponentWithWorld(world);
	}
	return psc;
}

//----------------------------------------------------------------------------

UPopcornFXEmitterComponent::UPopcornFXEmitterComponent(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, m_CurrentScene(null)
	, m_Scene_EmitterId(PopcornFX::CGuid::INVALID)
	, m_Started(false)
	, m_Stopped(false)
	, m_DiedThisFrame(false)
	, m_TeleportThisFrame(false)
#if WITH_EDITOR
	, m_ReplayAfterDead(false)
#endif
{
	bAutoActivate = true;
	//SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

#if (ENGINE_MINOR_VERSION >= 14)
	// bWantsBeginPlay unused
#else
	bWantsBeginPlay = true;
#endif

	bPlayOnLoad = true;
	bEnableUpdates = true;
	bAllowTeleport = false;

	{
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
	}
	Scene = null;

	AttributeList = CreateDefaultSubobject<UPopcornFXAttributeList>("AttributeList");

	//UE_LOG(LogPopcornFXEmitterComponent, Log, TEXT("EMITTERCOMP ctor attrlist %p"), AttributeList);

	AttributeList->SetFlags(RF_Transactional);

	AttributeList->CheckEmitter(this);

	bAutoDestroy = false;
	bHasAlreadyPlayOnLoad = false;

	SetFlags(RF_Transactional);
}

//----------------------------------------------------------------------------

UPopcornFXEmitterComponent::~UPopcornFXEmitterComponent()
{
	//PopcornFX::CLog::Log(HH_INFO, "UPopcornFXEmitterComponent::~UPopcornFXEmitterComponent %p List %p cont: %p", this, AttributeList, *(AttributeList->AttributesContainerIFP()));
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 14)
static const EWorldType::Type		kWorldTypeEditorPreview = EWorldType::EditorPreview;
#else
static const EWorldType::Type		kWorldTypeEditorPreview = EWorldType::Preview;
#endif

bool	UPopcornFXEmitterComponent::ResolveScene(bool warnIFN)
{
	UWorld			*world = GetWorld();

	// dont warn if cooking or no active world
	const bool		disableWarn = !warnIFN || IsRunningCommandlet() || (world == null) || (world->WorldType == EWorldType::Inactive);
	const bool		enableWarn = !disableWarn;
	if (world == null)
	{
		if (enableWarn)
			UE_LOG(LogPopcornFXEmitterComponent, Warning, TEXT("NULL World: %s"), *GetPathName());
		return false;
	}

	if (!SceneName.IsValid() || SceneName.IsNone())
		Scene = null;
	else
	{
		bool		found = false;
		if (Scene != null && (SceneName != Scene->GetSceneName()))
			Scene = null;
		if (Scene == null && world != null)
		{
			for (TActorIterator<APopcornFXSceneActor> sceneIt(world); sceneIt; ++sceneIt)
			{
				if (sceneIt->GetSceneName() == SceneName)
				{
					Scene = *sceneIt;
					found = true;
					break;
				}
			}
#if WITH_EDITOR
			if (world->WorldType == kWorldTypeEditorPreview)
				SpawnPreviewSceneIFN(world);
#endif // WITH_EDITOR
		}
		//if (!found)
		//	UE_LOG(LogPopcornFXEmitterComponent, Warning, TEXT("Emitter '%s': scene '%s' not found"), *GetFullName(), *(SceneName.ToString()));
	}

	m_CurrentScene = Scene != null ? Scene->ParticleScene() : null;

	if (m_CurrentScene == null)
	{
		if (enableWarn)
			UE_LOG(LogPopcornFXEmitterComponent, Warning, TEXT("Could not find a valid APopcornFXSceneActor with a SceneName '%s': %s"), *SceneName.ToString(), *GetPathName());
		return false;
	}

	return m_CurrentScene != null;
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

void	UPopcornFXEmitterComponent::PreEditChange(UProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	const UWorld	*world = GetWorld();
	HH_ASSERT(IsRegistered() || world == null ||
			 (world->WorldType == kWorldTypeEditorPreview || world->WorldType == EWorldType::Editor));

	CheckForDead(); // we could have not been able to do that when terminated externally

	//bool			isTemplate = IsTemplate();
	const UProperty	*propertyThatChanged = propertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		const FString	pName = propertyThatChanged->GetName();

		if (pName == TEXT("bPlayOnLoad"))
		{
			TerminateEmitter();
			if (bPlayOnLoad != 0)
				StartEmitter();
		}
		else if (pName == TEXT("Effect") || pName == TEXT("SceneName"))
		{
			const bool		wasEmitting = IsEmitterEmitting();
			TerminateEmitter();
			if (Effect == null)
				AttributeList->Clean();
			else
			{
				HH_VERIFY(AttributeList->Prepare(Effect));
				if (wasEmitting || bPlayOnLoad != 0)
					StartEmitter();
			}
		}
		else if (pName == TEXT("AttributeList"))
		{
			//AttributeList->RefreshAttributeSamplers(this);
		}
	}

	HH_VERIFY(AttributeList->Prepare(Effect)); // make sure everything is up to date, always
	AttributeList->CheckEmitter(this);

	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::SpawnPreviewSceneIFN(UWorld *world)
{
	if (Scene == null)
	{
		FActorSpawnParameters	params;

		params.ObjectFlags = RF_Public | RF_Transactional;
		// ! Do not force the name !
		// Or UE will magicaly just return the last deleted Actor with the same name !
		//params.Name = SceneName;
		params.bAllowDuringConstructionScript = true;
		Scene = world->SpawnActor<APopcornFXSceneActor>(APopcornFXSceneActor::StaticClass(), params);
		if (Scene != null && Scene->PopcornFXSceneComponent != null)
			Scene->PopcornFXSceneComponent->SceneName = SceneName;
	}

	if (!HH_VERIFY(Scene != null))
		return;
	// Mirror the "real" world scene properties if possible, gore walkthrough though
	bool					found = false;
	APopcornFXSceneActor	*refScene = null;
	for (TObjectIterator<UWorld> worldIt; worldIt && !found; ++worldIt)
	{
		UWorld	*currentWorld = *worldIt;

		if (currentWorld->IsGameWorld() || currentWorld->WorldType == EWorldType::Editor)
		{
			for (TActorIterator<APopcornFXSceneActor> sceneIt(currentWorld); sceneIt; ++sceneIt)
			{
				if (sceneIt->GetSceneName() == SceneName)
				{
					refScene = *sceneIt;
					found = true;
					break;
				}
			}
		}
	}
	if (found && refScene != null)
		Scene->PopcornFXSceneComponent->MirrorGameWorldProperties(refScene->PopcornFXSceneComponent);
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::CheckForErrors()
{
	Super::CheckForErrors();

	AActor* Owner = GetOwner();
	if (Effect != null)
	{
		if (!Effect->IsFileValid())
		{
			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("ComponentName"), FText::FromString(GetName()));
			Arguments.Add(TEXT("OwnerName"), FText::FromString(Owner->GetName()));
			Arguments.Add(TEXT("EffectName"), FText::FromString(Effect->GetName()));
			FMessageLog("MapCheck").Warning()
				->AddToken(FUObjectToken::Create(Owner))
				->AddToken(FTextToken::Create(FText::Format(LOCTEXT("MapCheck_Message_Invalid", "{EffectName} used by {ComponentName}::{OwnerName} is corrupted"), Arguments)))
				;
		}
		else if (Effect->ActionFactoryIFP() != null)
		{
			PopcornFX::PBaseObjectFile		file = Effect->ActionFactoryIFP()->File();
			if (file != null)
			{
				PopcornFX::SEngineVersion	current = FPopcornFXPlugin::PopcornFXBuildVersion();
				PopcornFX::SEngineVersion	filever = file->m_Version;
				if (filever.Empty())
					filever = PopcornFX::SEngineVersion(1, 7, 3, 0); // bo file version did not exists

				bool		outOfDate = filever.m_Major < current.m_Major;
				if (!outOfDate && filever.m_Minor < current.m_Minor)
					outOfDate = true;
				if (outOfDate)
				{
					FFormatNamedArguments Arguments;
					Arguments.Add(TEXT("EffectName"), FText::FromString(Effect->GetName()));
					Arguments.Add(TEXT("OldVersion"), FText::FromString(FString::Printf(TEXT("%d.%d.%d.%d"), filever.m_Major, filever.m_Minor, filever.m_Patch, filever.m_Revision)));
					Arguments.Add(TEXT("NewVersion"), FText::FromString(FString::Printf(TEXT("%d.%d.%d.%d"), current.m_Major, current.m_Minor, current.m_Patch, current.m_Revision)));
					FMessageLog("MapCheck").Warning()
						->AddToken(FUObjectToken::Create(Effect))
						->AddToken(FTextToken::Create(FText::Format(LOCTEXT("MapCheck_Message_Version", "is out of date ({OldVersion}) please upgrade your PopcornFX Pack to {NewVersion} then reimport the effect"), Arguments)));
				}
			}
		}
	}
}

#endif // WITH_EDITOR

const UObject	*UPopcornFXEmitterComponent::AdditionalStatObject() const
{
	return Effect;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::EmitterBeginPlayIFN()
{
	if ((!bPlayOnLoad || bHasAlreadyPlayOnLoad)
#if WITH_EDITOR
		&& !m_ReplayAfterDead
#endif
		)
		return;

#if WITH_EDITOR
	m_ReplayAfterDead = false;
#endif
	bHasAlreadyPlayOnLoad = true;

	RestartEmitter();
	return;
}

//----------------------------------------------------------------------------

UPopcornFXEmitterComponent	*UPopcornFXEmitterComponent::CopyAndStartEmitterAtLocation(
	FVector location,
	FRotator rotation,
	bool pbAutoDestroy) // shadow bAutoDestroy
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXEmitterComponent::CopyAndStartEmitterAtLocation", POPCORNFX_UE_PROFILER_COLOR);

	if (!ResolveScene(true))
		return null;

	if (Effect == null)
	{
		UE_LOG(LogScript, Warning, TEXT("UPopcornFXEmitterComponent::CopyAndStartEmitterAttached: there is no Effect: '%s'"), *GetPathName());
		return null;
	}

	UPopcornFXEmitterComponent	*psc = CreateStandaloneEmitterComponent(Effect, Scene, GetWorld(), null, pbAutoDestroy);
	if (psc != null)
	{
		psc->SetAbsolute(true, true, true);
		psc->SetWorldLocationAndRotation(location, rotation);
		psc->SetRelativeScale3D(FVector(1.f));
		psc->UpdateComponentToWorld();

		psc->AttributeList->CopyFrom(AttributeList, GetOwner());
		psc->StartEmitter();
	}
	return psc;
}

//----------------------------------------------------------------------------

UPopcornFXEmitterComponent	*UPopcornFXEmitterComponent::CopyAndStartEmitterAttached(
	class USceneComponent* attachToComponent,
	FName attachPointName,
	FVector location,
	FRotator rotation,
	EAttachLocation::Type locationType,
	bool pbAutoDestroy) // shadows bAutoDestroy
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXEmitterComponent::CopyAndStartEmitterAttached", POPCORNFX_UE_PROFILER_COLOR);

	if (!ResolveScene(true))
		return null;

	if (Effect == null)
	{
		UE_LOG(LogScript, Warning, TEXT("UPopcornFXEmitterComponent::CopyAndStartEmitterAttached: there is no Effect: '%s'"), *GetPathName());
		return null;
	}
	if (attachToComponent == null)
	{
		UE_LOG(LogScript, Warning, TEXT("UPopcornFXEmitterComponent::CopyAndStartEmitterAttached: NULL AttachComponent specified!: '%s'"), *GetPathName());
		return null;
	}

	UPopcornFXEmitterComponent	*psc = CreateStandaloneEmitterComponent(Effect, Scene, GetWorld(), null, pbAutoDestroy);
	if (psc != null)
	{
#if (ENGINE_MINOR_VERSION >= 12)
		psc->AttachToComponent(attachToComponent, FAttachmentTransformRules::KeepRelativeTransform, attachPointName);
#else
		psc->AttachTo(attachToComponent, attachPointName);
#endif
		if (locationType == EAttachLocation::KeepWorldPosition)
		{
			psc->SetWorldLocationAndRotation(location, rotation);
		}
		else
		{
			psc->SetRelativeLocationAndRotation(location, rotation);
		}
		psc->SetRelativeScale3D(FVector(1.f));
		psc->UpdateComponentToWorld();

		psc->bEnableUpdates = true;
		psc->AttributeList->CopyFrom(AttributeList, GetOwner());
		psc->StartEmitter();
	}
	return psc;
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::SetEffect(UPopcornFXEffect *effect, bool startEmitter)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXEmitterComponent::SetEffect", POPCORNFX_UE_PROFILER_COLOR);

	TerminateEmitter();
	if (effect == null)
	{
		// SetEffect(null) is OK, why return false?
		Effect = effect;
		AttributeList->Clean();
		return true;
	}
	if (effect != Effect)
		Effect = effect;
	HH_VERIFY(AttributeList->Prepare(Effect, true));
	if (startEmitter)
		StartEmitter();
	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::StartEmitter()
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXEmitterComponent::StartEmitter", POPCORNFX_UE_PROFILER_COLOR);

	// if already started do nothing
	if (m_Started)
		return true;

	if (!ResolveScene(true))
		return false;

	if (Effect == null)
	{
		UE_LOG(LogPopcornFXEmitterComponent, Warning/*Error*/, TEXT("Could not StartEmitter '%s': there is no Effect"), *GetFullName());
		return false;
	}

	CParticleScene			*particleScene = m_CurrentScene;
	HH_ASSERT(particleScene != null);

	if (Effect->ActionFactory() == null)
	{
		UE_LOG(LogPopcornFXEmitterComponent, Warning/*Error*/, TEXT("Could not StartEmitter '%s': invalid effect '%s'"), *GetFullName(), *Effect->GetPathName());
		return false;
	}

	// setup the action instance basic parameters
	if (m_ActionParametersPtr == null)
		m_ActionParametersPtr = PopcornFX::CActionParameters::Alloc().Get();

	UPopcornFXAttributeList		*attributeList = GetAttributeList();
	HH_ASSERT(attributeList != null);

	CFloat4x4		&currentTr = ToPkRef(m_CurrentWorldTransforms);
	CFloat4x4		&previousTr = ToPkRef(m_PreviousWorldTransforms);
	CFloat3			&currentVel = ToPkRef(m_CurrentWorldVelocity);
	CFloat3			&previousVel = ToPkRef(m_PreviousWorldVelocity);

	// @TODO: prepare only once m_ActionParameters
	{
		if (!HH_VERIFY(m_ActionParametersPtr != null))
		{
			UE_LOG(LogPopcornFXEmitterComponent, Warning/*Error*/, TEXT("Could not StartEmitter '%s': cannot allocate action parameters"), *GetFullName());
			return false;
		}
		PopcornFX::SSpawnTransformsPack	sptp;
		sptp.m_WorldTr_Current = &currentTr;
		sptp.m_WorldTr_Previous = &previousTr;
		sptp.m_WorldVel_Current = &currentVel;
		sptp.m_WorldVel_Previous = &previousVel;

		if (attributeList != null)
			attributeList->RefreshAttributeSamplers(this, true);

		m_ActionParametersPtr->UpdateParameterAndAddIFN<PopcornFX::CParticleMediumCollection*>(particleScene->ParticleMediumCollectionID(), particleScene->Unsafe_ParticleMediumCollection());
		m_ActionParametersPtr->UpdateParameterAndAddIFN<PopcornFX::SSpawnTransformsPack>(particleScene->SpawnTransformsID(), sptp);
		if (attributeList != null)
		{
			// truly should be SAttributesContainer*const*, by PopcornFX needs SAttributesContainer** for its RTTI
			PopcornFX::SAttributesContainer		**attributeContainer = const_cast<PopcornFX::SAttributesContainer**>(attributeList->AttributesContainer());
			HH_ASSERT(attributeContainer != null);
			m_ActionParametersPtr->UpdateParameterAndAddIFN<PopcornFX::SAttributesContainer**>(particleScene->AttributesID(), attributeContainer);
		}
	}

	currentTr = ToPk(GetComponentTransform().ToMatrixNoScale());
	currentTr.StrippedTranslations() *= FPopcornFXPlugin::GlobalScaleRcp();
	previousTr = currentTr;
	m_LastFrameUpdate = 0;

	currentVel = CFloat3::ZERO;
	previousVel = CFloat3::ZERO;

	m_ActionInstancePtr = Effect->ActionFactory()->Instantiate(m_ActionParametersPtr.Get()).Get();
	HH_ASSERT(m_ActionInstancePtr != null);
	if (m_ActionInstancePtr == null)
	{
		UE_LOG(LogPopcornFXEmitterComponent, Warning/*Error*/, TEXT("Could not StartEmitter '%s': ActionFactory()->Instantiate failed"), *GetFullName());
		return false;
	}

	m_Started = true;
	m_Stopped = false;
	m_ActionInstancePtr->m_DeathNotifier += PopcornFX::FastDelegate<void(const PopcornFX::PActionInstance &)>(this, &UPopcornFXEmitterComponent::_OnDeathNotifier);

	// actual spawn
	//if (!particleScene->AddAndRunActionInstance(m_ActionInstancePtr))
	if (!particleScene->AddActionInstance(m_ActionInstancePtr.Get()))
	{
		m_Started = false;
		m_Stopped = true;
		return false;
	}
	
	CheckForDead();

	// here m_ActionInstancePtr could be already null (ie: with a spawn burst who dies instantly)
	if (m_ActionInstancePtr != null)
	{
		// if the instance is still alive,
		// register me to the scene for update if bEnableUpdates
		if (bEnableUpdates != 0)
		{
			if (!SelfSceneRegister())
			{
				UE_LOG(LogPopcornFXEmitterComponent, Warning/*Error*/, TEXT("Could not StartEmitter '%s': failed to register in the scene (Effect updates will not work)"), *GetFullName());
				return true;
			}
		}
	}

	const FVector			rotation = GetComponentTransform().GetRotation().GetRotationAxis();
	OnEmitterStart.Broadcast(this, GetComponentTransform().GetLocation(), rotation);
	APopcornFXEmitter* actor = Cast<APopcornFXEmitter>(GetOwner());
	if (actor != null)
		actor->OnEmitterStart.Broadcast(this, GetComponentTransform().GetLocation(), rotation);

	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::RestartEmitter()
{
	TerminateEmitter();
	StartEmitter();
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::StopEmitter()
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXEmitterComponent::StopEmitter", POPCORNFX_UE_PROFILER_COLOR);

	const UWorld	*world = GetWorld();
	HH_ASSERT(IsRegistered() || world == null ||
			  (world->WorldType == kWorldTypeEditorPreview || world->WorldType == EWorldType::Editor));

	bool		sendEvent = false;
	if (m_ActionInstancePtr != null && m_Started && !m_Stopped)
	{
		m_ActionInstancePtr->Stop();
		sendEvent = true;
	}
	m_Stopped = true;
	if (m_ActionInstancePtr == null) // we could have been destroy by the stop
		m_Started = false;

	// send only if not terminated
	if (sendEvent)
	{
		const FVector			rotation = GetComponentTransform().GetRotation().GetRotationAxis();
		OnEmitterStop.Broadcast(this, GetComponentTransform().GetLocation(), rotation);
		APopcornFXEmitter* actor = Cast<APopcornFXEmitter>(GetOwner());
		if (actor != null)
			actor->OnEmitterStop.Broadcast(this, GetComponentTransform().GetLocation(), rotation);
	}
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::ToggleEmitter(bool startEmitter)
{
	if (startEmitter && !IsEmitterStarted())
	{
		StartEmitter();
	}
	else if (!startEmitter && IsEmitterStarted())
	{
		StopEmitter();
	}
	return IsEmitterStarted();
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::TerminateEmitter()
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXEmitterComponent::TerminateEmitter", POPCORNFX_UE_PROFILER_COLOR);

	//HH_ASSERT(!IsTemplate());

	bool		sendEvent = false;
	if (m_ActionInstancePtr != null)
	{
		HH_ASSERT(FPopcornFXPlugin::IsMainThread()); // cannot be called async

		AttributeList->CheckEmitter(this);

#if 0
		PopcornFX::CLog::Log(HH_INFO,
			"UPopcornFXEmitterComponent::TerminateEmitter %p List %p cont: %p **%p %s %s",
				this, AttributeList,
				*(AttributeList->AttributesContainerIFP()),
				AttributeList->AttributesContainerIFP(),
				m_ActionInstancePtr->m_ParentFactory_DebugName.Data(),
				TCHAR_TO_ANSI(*GetPathName()));
#endif

		// keep a ref here
		PopcornFX::PActionInstance		instanceForRef = m_ActionInstancePtr.Get();

		m_ActionInstancePtr->Terminate();

		// Fail safe, Terminate should have called _OnDeathNotifier
		if (!HH_VERIFY(m_DiedThisFrame) ||
			!HH_VERIFY(m_ActionInstancePtr == null))
		{
			_OnDeathNotifier(m_ActionInstancePtr.Get());
			HH_ASSERT(m_DiedThisFrame);
			HH_ASSERT(m_ActionInstancePtr == null);
		}

		// Release ref
		instanceForRef = null;

		HH_ASSERT(m_CurrentScene != null);
		HH_ASSERT(m_CurrentScene->Unsafe_ParticleMediumCollection()->Sanity_AssertNoSpawnerRefs(AttributeList->AttributesContainerIFP()));

		sendEvent = true;
		//m_ActionInstancePtr = null; // do not null that in case its called later

		// _OnDeathNotifier should have been called
	}

	CheckForDead(); // can call OnEmissionStops

	HH_ASSERT(!SelfSceneIsRegistered());
	HH_ASSERT(m_ActionInstancePtr == null);
	HH_ASSERT(!m_Started);

	// its shame we cant check that, but an OnEmissionStops could have restarted the emitter
	//HH_ASSERT(m_ActionInstancePtr == null);
	//HH_ASSERT(!m_Started);

	if (sendEvent)
	{
		const FVector			rotation = GetComponentTransform().GetRotation().GetRotationAxis();
		OnEmitterTerminate.Broadcast(this, GetComponentTransform().GetLocation(), rotation);
		APopcornFXEmitter* actor = Cast<APopcornFXEmitter>(GetOwner());
		if (actor != null)
			actor->OnEmitterTerminate.Broadcast(this, GetComponentTransform().GetLocation(), rotation);
	}
}

//----------------------------------------------------------------------------

UPopcornFXAttributeList	*UPopcornFXEmitterComponent::GetAttributeList()
{
	if (!HH_VERIFY(AttributeList != null)) // something can go wrong when deleting stuff
		return null;
	if (!HH_VERIFY(AttributeList->Prepare(Effect)))
		return null;
	AttributeList->CheckEmitter(this);
	HH_ASSERT(AttributeList->Valid() && AttributeList->IsUpToDate(Effect));
	return AttributeList;
}

//----------------------------------------------------------------------------

UPopcornFXAttributeList	*UPopcornFXEmitterComponent::GetAttributeListIFP() const
{
	if (!HH_VERIFY(AttributeList != null)) // something can go wrong when deleting stuff
		return null;
	if (!HH_VERIFY(AttributeList->Valid() && AttributeList->IsUpToDate(Effect)))
		return null;
	AttributeList->CheckEmitter(this);
	return AttributeList;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::ResetAttributesToDefault()
{
	if (!HH_VERIFY(AttributeList != null)) // somthing can go wrong when deleting stuff
		return;
	UPopcornFXAttributeList		*attributeList = GetAttributeList();
	if (attributeList != null)
		attributeList->ResetToDefaultValues(Effect);
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::IsEmitterAlive() const
{
	return m_Started;
}
//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::IsEmitterStarted() const
{
	return m_Started;
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::IsEmitterEmitting() const
{
	return m_Started && !m_Stopped;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::PostLoad()
{
	Super::PostLoad();

	//UE_LOG(LogPopcornFXEmitterComponent, Log, TEXT("EMITTERCOMP %p post load attr list %p"), this, AttributeList);

	if (Effect != null)
	{
		Effect->ConditionalPostLoad();
		check(AttributeList != null);
		AttributeList->ConditionalPostLoad();
		HH_VERIFY(AttributeList->Prepare(Effect));
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::BeginDestroy()
{
	HH_ASSERT(IsInGameThread());
	TerminateEmitter();
	//UE_LOG(LogPopcornFXEmitterComponent, Log, TEXT("EMITTERCOMP %p BeginDestroy %p"), this, AttributeList);
	//if (AttributeList != null && AttributeList->IsValidLowLevel())
	//{
	//	//UE_LOG(LogPopcornFXEmitterComponent, Log, TEXT("EMITTERCOMP %p force BeginDestroy attr list %p"), this, AttributeList);
	//	AttributeList->ConditionalBeginDestroy();
	//}
	Super::BeginDestroy();
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::OnRegister()
{
	Super::OnRegister();

	HH_ASSERT(!IsTemplate());

	// Avoids Prepare call on the attribute list before the PostEditChangeProperty is called
	// This is due to "ReRegister()" called before the PostEditChange is broadcasted.
	if (Effect == null)
		return;

	// We need to to that again (PostLoad not called when spawned on the fly)
	check(AttributeList != null);
	HH_VERIFY(AttributeList->Prepare(Effect));

	AttributeList->CheckEmitter(this);

#if WITH_EDITOR

	if (!SceneName.IsValid() || SceneName.IsNone())
		return;
	const bool		isEditor = GIsEditor;
	const bool		isPlay = GIsPlayInEditorWorld;
	// in Editor, BeginPlay is not called
	// workarround to StartEmitter *after* the Scene is Registered !
	if (isEditor && !isPlay)
	{
		if (ResolveScene(true)) // ok the Scene as already been Register
		{
			EmitterBeginPlayIFN();
		}
		else if (!m_WaitForSceneDelegateHandle.IsValid())
		{
			APopcornFXWaitForScene		*wait = APopcornFXWaitForScene::GetOrCreate(GetWorld(), SceneName);
			if (HH_VERIFY(wait != null))
				m_WaitForSceneDelegateHandle = wait->OnSceneLoadedDelegates.AddUObject(this, &UPopcornFXEmitterComponent::_OnSceneLoaded);
		}
	}
#endif
}

//----------------------------------------------------------------------------

#if WITH_EDITOR
void	UPopcornFXEmitterComponent::_OnSceneLoaded(APopcornFXSceneActor *sceneActor)
{
	EmitterBeginPlayIFN();
	HH_ASSERT(Scene == null || Scene == sceneActor);
	m_WaitForSceneDelegateHandle.Reset();
	return;
}
#endif // WITH_EDITOR

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::OnUnregister()
{
#if WITH_EDITOR
	m_ReplayAfterDead = IsEmitterEmitting();
#endif
	TerminateEmitter();

	Super::OnUnregister();

	bHasAlreadyPlayOnLoad = false;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::BeginPlay()
{
	if (!ResolveScene(true))
	{
		// ResolveScene has logged a warning ifn
	}

	Super::BeginPlay();

	// will do nothing if already starte in OnRegister
	EmitterBeginPlayIFN();
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TerminateEmitter();

	Super::EndPlay(EndPlayReason);

	bHasAlreadyPlayOnLoad = false;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::_OnDeathNotifier(const PopcornFX::PActionInstance & action)
{
	HH_ASSERT(m_ActionInstancePtr == action.Get());

	m_DiedThisFrame = true;
#if WITH_EDITOR
	m_ReplayAfterDead = (bAutoDestroy == 0) &&
		IsEmitterEmitting() &&
		action != null &&
		(action->ParentFactory()->_m_HBO_Flags & PopcornFX::HBO::Flag_BaseVirtualUnlink_NotCalled) != 0;
#endif
	if (m_ActionInstancePtr != null)
		m_ActionInstancePtr->m_DeathNotifier -= PopcornFX::FastDelegate<void(const PopcornFX::PActionInstance &)>(this, &UPopcornFXEmitterComponent::_OnDeathNotifier);

	m_ActionInstancePtr = null;
	m_Started = false;
	m_Stopped = false;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::CheckForDead()
{
	if (m_DiedThisFrame)
	{
		m_DiedThisFrame = false;

		if (SelfSceneIsRegistered())
		{
			SelfSceneUnregister();
		}

		if (bAutoDestroy != 0)
		{
			// Avoids nested DestroyComponent()
			if (IsRegistered())
				DestroyComponent();
		}
		else if (Effect != null && !Effect->HasAnyFlags(RF_BeginDestroyed))
		{
#if WITH_EDITOR
			if (m_ReplayAfterDead)
			{
				Effect->m_OnPopcornFXFileLoaded.Remove(m_OnPopcornFXFileLoadedHandle);
				m_OnPopcornFXFileLoadedHandle = Effect->m_OnPopcornFXFileLoaded.AddUObject(this, &UPopcornFXEmitterComponent::OnPopcornFXFileLoaded);
			}
			else
#endif
			{
				if (OnEmissionStops.IsBound())
				{
					const FVector			rotation = GetComponentTransform().GetRotation().GetRotationAxis();
					OnEmissionStops.Broadcast(this, GetComponentTransform().GetLocation(), rotation);
				}
			}
		}

	}
}


//----------------------------------------------------------------------------

// Compute particle system bounds
FBoxSphereBounds	UPopcornFXEmitterComponent::CalcBounds(const FTransform &localToWorld) const
{
	FBoxSphereBounds	bounds;

	// Arbitrary bounds
	bounds.Origin = localToWorld.GetLocation();
	bounds.BoxExtent = FVector(32.0f, 32.0f, 32.0f);
	bounds.SphereRadius = 64.0f;
	return bounds;
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 9)
#	if (ENGINE_MINOR_VERSION >= 12)
void	UPopcornFXEmitterComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);
#	else // if >= 9
void	UPopcornFXEmitterComponent::OnUpdateTransform(bool bSkipPhysicsMove, ETeleportType Teleport)
{
	Super::OnUpdateTransform(bSkipPhysicsMove, Teleport);
#	endif
	switch (Teleport)
	{
	case ETeleportType::None:
		break;
	case ETeleportType::TeleportPhysics:
		m_TeleportThisFrame = true;
		break;
	}

}
#endif // (ENGINE_MINOR_VERSION >= 9)

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::Scene_OnRegistered(CParticleScene *scene, uint32 selfIdInScene)
{
	HH_ASSERT(!IsTemplate());

	HH_ASSERT(scene != null);
	HH_ASSERT(m_CurrentScene == scene);
	HH_ASSERT(selfIdInScene != PopcornFX::CGuid::INVALID);

	HH_ASSERT(m_Scene_EmitterId == PopcornFX::CGuid::INVALID);
	m_Scene_EmitterId = selfIdInScene;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::Scene_OnUnregistered(CParticleScene *scene)
{
	HH_ASSERT(!IsTemplate());

	HH_ASSERT(scene != null);
	HH_ASSERT(m_CurrentScene == scene);

	HH_ASSERT(m_Scene_EmitterId != PopcornFX::CGuid::INVALID);
	m_Scene_EmitterId = PopcornFX::CGuid::INVALID;
	m_CurrentScene = null;

	// !! make sure we are unregisterd (if not, infinite recusre)
	HH_ASSERT(!SelfSceneIsRegistered());
	if (!SelfSceneIsRegistered())
		TerminateEmitter();
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::Scene_PreUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType)
{
	HH_ASSERT(!IsTemplate());

	HH_ASSERT(scene != null);
	HH_ASSERT(SelfSceneIsRegistered());
	HH_ASSERT(m_CurrentScene == scene);

	if (IsPendingKill())
		return;
	// we should be ticking only if alive (kind-of optim purpose only)
	HH_ASSERT(IsEmitterAlive());

	const float				scale = FPopcornFXPlugin::GlobalScale();
	const float				rcpscale = 1.f / scale;

	// Scene_PreUpdate is executed once per Update, so Prev Curr should always be processed
	HH_ASSERT_MESSAGE(m_LastFrameUpdate != GFrameCounter, "Should not happen, ping Jordan");
	m_LastFrameUpdate = GFrameCounter;

	CFloat4x4			&currentTr = ToPkRef(m_CurrentWorldTransforms);
	CFloat4x4			&previousTr = ToPkRef(m_PreviousWorldTransforms);
	CFloat3				&currentVel = ToPkRef(m_CurrentWorldVelocity);
	CFloat3				&previousVel = ToPkRef(m_PreviousWorldVelocity);

	const bool			teleport = bAllowTeleport && m_TeleportThisFrame;
	// Always reset m_TeleportThisFrame
	if (m_TeleportThisFrame)
		m_TeleportThisFrame = false;

	if (!teleport)
	{
		// Prev = Curr
		previousTr = currentTr;
		previousVel = currentVel;
	}

	// Update Position
	currentTr = ToPk(GetComponentTransform().ToMatrixNoScale());
	currentTr.StrippedTranslations() *= rcpscale;

	// Update Velocity
	if (!teleport && deltaTime > 0)
		currentVel = (currentTr.StrippedTranslations() - previousTr.StrippedTranslations()) / deltaTime;
	else
		currentVel = CFloat3::ZERO;

	if (teleport)
	{
		// Make previous frame same as current
		previousTr = currentTr;
		previousVel = currentVel;
	}

	if (IsValid(AttributeList))// && AttributeList->bNeedTick)
	{
		AttributeList->CheckEmitter(this);
		AttributeList->Scene_PreUpdate(scene, this, deltaTime, tickType);
	}

	//if (deltaTime > 1.0e-6f)
	//{
	//	CFloat3	delta = currentTr.StrippedTranslations() - previousTr.StrippedTranslations();
	//	currentVel = delta * PopcornFX::HHRcp(deltaTime) * rcpscale;
	//}

#if WITH_EDITOR
	// Notify samplers they are indirectly selected this frame
	if (!GetWorld()->IsGameWorld() && IsValid(AttributeList))
	{
		const USelection	*selectedAssets = GEditor->GetSelectedActors();
		HH_ASSERT(selectedAssets != null);
		bool				isSelected = selectedAssets->IsSelected(GetOwner());
		if (isSelected)
			AttributeList->AttributeSamplers_IndirectSelectedThisTick(this);
	}
#endif
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::Scene_DuringUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType)
{
	CheckForDead();
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::SelfSceneIsRegistered() const
{
	return m_Scene_EmitterId != PopcornFX::CGuid::INVALID && m_CurrentScene != nullptr;
}

//----------------------------------------------------------------------------

bool	UPopcornFXEmitterComponent::SelfSceneRegister()
{
	HH_ASSERT(!IsTemplate());

	HH_ASSERT(!SelfSceneIsRegistered());
	if (m_CurrentScene != null)
		m_CurrentScene->Emitter_Register(this);
	bool		ok = false;
	if (SelfSceneIsRegistered())
	{
		ok = true;
	}
	return ok;
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::SelfSceneUnregister()
{
	HH_ASSERT(!IsTemplate());

	HH_ASSERT(SelfSceneIsRegistered());
	if (m_CurrentScene != null)
		m_CurrentScene->Emitter_Unregister(this);
	HH_ASSERT(!SelfSceneIsRegistered());
}

//----------------------------------------------------------------------------

void	UPopcornFXEmitterComponent::OnPopcornFXFileLoaded(const UPopcornFXFile *file)
{
	HH_ASSERT(!IsTemplate());
	if (!HH_VERIFY(file == Effect))
		return;
#if WITH_EDITOR
	if (m_ReplayAfterDead)
	{
		Effect->m_OnPopcornFXFileLoaded.Remove(m_OnPopcornFXFileLoadedHandle);
		m_ReplayAfterDead = false;
		SetEffect(Effect, true);
	}
#endif
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
