//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXFunctions.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXSceneActor.h"
#include "PopcornFXEmitterComponent.h"
#include "PopcornFXSceneComponent.h"
#include "Assets/PopcornFXEffect.h"
#include "PopcornFXTypes.h"

#include "Internal/ResourceHandlerImage_UE.h"

#include "Engine/Engine.h"

#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXFunctions"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXFunctions, Log, All);

//----------------------------------------------------------------------------

UPopcornFXFunctions::UPopcornFXFunctions(class FObjectInitializer const &pcip)
	: Super(pcip)
{
}

//----------------------------------------------------------------------------

const FString	&UPopcornFXFunctions::PopcornFXPluginVersion()
{
	return FPopcornFXPlugin::PluginVersionString();
}

//----------------------------------------------------------------------------

const FString	&UPopcornFXFunctions::PopcornFXRuntimeVersion()
{
	return FPopcornFXPlugin::PopornFXRuntimeVersionString();
}

//----------------------------------------------------------------------------

float	UPopcornFXFunctions::PopcornFXGlobalScale()
{
	return FPopcornFXPlugin::GlobalScale();
}

//----------------------------------------------------------------------------

float	UPopcornFXFunctions::PopcornFXGlobalScaleInv()
{
	return FPopcornFXPlugin::GlobalScaleRcp();
}

//----------------------------------------------------------------------------

int32	UPopcornFXFunctions::PopcornFXTotalParticleCount()
{
	return FPopcornFXPlugin::TotalParticleCount();
}

//----------------------------------------------------------------------------

void	UPopcornFXFunctions::NotifySettingChanged()
{
	FPopcornFXPlugin::Get().HandleSettingsModified();
}

//----------------------------------------------------------------------------

UPopcornFXEmitterComponent*	UPopcornFXFunctions::SpawnEmitterAtLocation(UObject* WorldContextObject, class UPopcornFXEffect* Effect, FName SceneName, FVector Location, FRotator Rotation, bool bStartEmitter, bool bAutoDestroy)
{
	if (Effect == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter At Location: No effect specified"));
		return null;
	}

#if (ENGINE_MINOR_VERSION >= 17)
	UWorld	*world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
#else
	UWorld	*world = GEngine->GetWorldFromContextObject(WorldContextObject);
#endif

	if (world == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter At Location: Couldn't get the world from '%s'"), *WorldContextObject->GetName());
		return null;
	}
	UPopcornFXEmitterComponent	*emitter = UPopcornFXEmitterComponent::CreateStandaloneEmitterComponent(Effect, null, world, null, bAutoDestroy);
	if (emitter == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter At Location: Failed to create Effect '%s'"), *Effect->GetName());
		return null;
	}
	emitter->bEnableUpdates = true;
	emitter->bAutoActivate = true;
	emitter->SceneName = SceneName;
	if (!emitter->ResolveScene(true))
	{
		emitter->DestroyComponent();
		return null;
	}
	emitter->SetWorldLocation(Location);
	emitter->SetWorldRotation(Rotation);
	emitter->SetEffect(Effect, bStartEmitter);
	return emitter;
}

//----------------------------------------------------------------------------

UPopcornFXEmitterComponent *UPopcornFXFunctions::SpawnEmitterAttached(class UPopcornFXEffect* Effect, class USceneComponent* AttachToComponent, FName SceneName, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bStartEmitter, bool bAutoDestroy)
{
	if (AttachToComponent == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter Attached: No AttachToComponent specified"));
		return null;
	}
	if (Effect == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter Attached: No effect specified"));
		return null;
	}
	UWorld	*world = AttachToComponent->GetWorld();
	if (world == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter Attached: Couldn't get the world from '%s'"), *AttachToComponent->GetName());
		return null;
	}
	UPopcornFXEmitterComponent	*emitter = UPopcornFXEmitterComponent::CreateStandaloneEmitterComponent(Effect, null, world, null, bAutoDestroy);
	if (emitter == null)
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Spawn Emitter Attached: Failed to create Effect '%s'"), *Effect->GetName());
		return null;
	}
	emitter->bEnableUpdates = true;
	emitter->bAutoActivate = true;
	emitter->SceneName = SceneName;
	if (!emitter->ResolveScene(true))
	{
		emitter->DestroyComponent();
		return null;
	}
#if (ENGINE_MINOR_VERSION >= 12)
	emitter->AttachToComponent(AttachToComponent, FAttachmentTransformRules::KeepRelativeTransform, AttachPointName);
#else
	emitter->AttachTo(AttachToComponent, AttachPointName);
#endif
	if (LocationType == EAttachLocation::KeepWorldPosition)
		emitter->SetWorldLocationAndRotation(Location, Rotation);
	else
		emitter->SetRelativeLocationAndRotation(Location, Rotation);
	emitter->SetEffect(Effect, bStartEmitter);
	return emitter;
}

//----------------------------------------------------------------------------

void		UPopcornFXFunctions::NotifyObjectChanged(class UObject *object)
{
	FPopcornFXPlugin::Get().NotifyObjectChanged(object);
}

//----------------------------------------------------------------------------

//static
bool		UPopcornFXFunctions::IsTextureCPUInUse(const FString &virtualPath)
{
	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImage>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	CResourceHandlerImage_UE		*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE*>(ihandler);
	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);
	return handler->IsUsed(virtPath, false);
}

//static
bool		UPopcornFXFunctions::IsTextureGPUInUse(const FString &virtualPath)
{
#if (PK_GPU_D3D11 != 0)
	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImageGPU_D3D11>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	CResourceHandlerImage_UE_D3D11	*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE_D3D11*>(ihandler);
	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);
	return handler->IsUsed(virtPath, false);
#else
	return false;
#endif
}

//----------------------------------------------------------------------------


//static
bool		UPopcornFXFunctions::RegisterVirtualTextureOverride_CPU_FloatR(const FString &virtualPath, int32 width, int32 height, const TArray<float> &pixels)
{
	if (!HH_VERIFY(pixels.Num() > 0))
		return false;

	if (!HH_VERIFY(width > 0) || !HH_VERIFY(height > 0))
		return false;

	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImage>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	CResourceHandlerImage_UE		*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE*>(ihandler);

	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);
	const PopcornFX::CUint2			size(width, height);

	HH_ASSERT(pixels.GetTypeSize() == sizeof(float));
	return handler->RegisterVirtualTexture(
		virtPath, false,
		PopcornFX::CImage::Format_Fp32Lum,
		size,
		pixels.GetData(),
		pixels.Num() * pixels.GetTypeSize()
	);
}

//static
bool		UPopcornFXFunctions::RegisterVirtualTextureOverride_CPU_FloatRGBA(const FString &virtualPath, int32 width, int32 height, const TArray<FLinearColor> &pixels)
{
	if (!HH_VERIFY(pixels.Num() > 0))
		return false;

	if (!HH_VERIFY(width > 0) || !HH_VERIFY(height > 0))
		return false;

	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImage>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	CResourceHandlerImage_UE		*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE*>(ihandler);

	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);
	const PopcornFX::CUint2			size(width, height);
	HH_ASSERT(pixels.GetTypeSize() == sizeof(FLinearColor));
	return handler->RegisterVirtualTexture(
		virtPath, false,
		PopcornFX::CImage::Format_Fp32RGBA,
		size,
		pixels.GetData(),
		pixels.Num() * pixels.GetTypeSize()
	);
}

//static
bool		UPopcornFXFunctions::UnregisterVirtualTextureOverride_CPU(const FString &virtualPath)
{
	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImage>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);
	CResourceHandlerImage_UE		*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE*>(ihandler);
	return handler->UnregisterVirtualTexture(virtPath, false);
}

//----------------------------------------------------------------------------

// static
bool		UPopcornFXFunctions::RegisterVirtualTextureOverride_GPU(const FString &virtualPath, UTexture *texture)
{
#if (PK_GPU_D3D11 != 0)
	if (!HH_VERIFY(texture != null))
		return false;

	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImageGPU_D3D11>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	CResourceHandlerImage_UE_D3D11	*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE_D3D11*>(ihandler);

	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);

	return handler->RegisterVirtualTexture(
		virtPath, false,
		texture);
#else
	return false;
#endif
}

//static
bool		UPopcornFXFunctions::UnregisterVirtualTextureOverride_GPU(const FString &virtualPath)
{
#if (PK_GPU_D3D11 != 0)
	PopcornFX::IResourceHandler		*ihandler = PopcornFX::Resource::DefaultManager()->FindHandler<PopcornFX::CImageGPU_D3D11>();
	if (!HH_VERIFY(ihandler != null))
		return false;
	const PopcornFX::CString		virtPath = TCHAR_TO_ANSI(*virtualPath);
	CResourceHandlerImage_UE_D3D11	*handler = PopcornFX::checked_cast<CResourceHandlerImage_UE_D3D11*>(ihandler);
	return handler->UnregisterVirtualTexture(virtPath, false);
#else
	return false;
#endif
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsFloat(const UPopcornFXSceneComponent *InSelf, FName FieldName, float &OutValue)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	return InSelf->GetFieldValue(FieldName, &OutValue, EPopcornFXParticleFieldType::Float);
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsFloat2(const UPopcornFXSceneComponent *InSelf, FName FieldName, float &OutValueX, float &OutValueY)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	HH_ALIGN(0x10) float	outValue[2];
	if (!InSelf->GetFieldValue(FieldName, outValue, EPopcornFXParticleFieldType::Float2))
		return false;
	OutValueX = outValue[0];
	OutValueY = outValue[1];
	return true;
}

bool	UPopcornFXFunctions::GetParticleFieldAsVector2D(const UPopcornFXSceneComponent *InSelf, FName FieldName, FVector2D &OutValue)
{
	return GetParticleFieldAsFloat2(InSelf, FieldName, OutValue.X, OutValue.Y);
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsFloat3(const UPopcornFXSceneComponent *InSelf, FName FieldName, float &OutValueX, float &OutValueY, float &OutValueZ)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	HH_ALIGN(0x10) float	outValue[3];
	if (!InSelf->GetFieldValue(FieldName, outValue, EPopcornFXParticleFieldType::Float3))
		return false;
	OutValueX = outValue[0];
	OutValueY = outValue[1];
	OutValueZ = outValue[2];
	return true;
}

bool	UPopcornFXFunctions::GetParticleFieldAsVector(const UPopcornFXSceneComponent *InSelf, FName FieldName, FVector &OutValue)
{
	return GetParticleFieldAsFloat3(InSelf, FieldName, OutValue.X, OutValue.Y, OutValue.Z);
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsFloat4(const UPopcornFXSceneComponent *InSelf, FName FieldName, float &OutValueX, float &OutValueY, float &OutValueZ, float &OutValueW)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	HH_ALIGN(0x10) float	outValue[4];
	if (!InSelf->GetFieldValue(FieldName, outValue, EPopcornFXParticleFieldType::Float4))
		return false;
	OutValueX = outValue[0];
	OutValueY = outValue[1];
	OutValueZ = outValue[2];
	OutValueW = outValue[3];
	return true;
}

bool	UPopcornFXFunctions::GetParticleFieldAsLinearColor(const UPopcornFXSceneComponent *InSelf, FName FieldName, FLinearColor &OutValue)
{
	return GetParticleFieldAsFloat4(InSelf, FieldName, OutValue.R, OutValue.G, OutValue.B, OutValue.A);
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsInt(const UPopcornFXSceneComponent *InSelf, FName FieldName, int32 &OutValue)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	return InSelf->GetFieldValue(FieldName, &OutValue, EPopcornFXParticleFieldType::Int);
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsInt2(const UPopcornFXSceneComponent *InSelf, FName FieldName, int32 &OutValueX, int32 &OutValueY)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	HH_ALIGN(0x10) int32	outValue[2];
	if (!InSelf->GetFieldValue(FieldName, outValue, EPopcornFXParticleFieldType::Int2))
		return false;
	OutValueX = outValue[0];
	OutValueY = outValue[1];
	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsInt3(const UPopcornFXSceneComponent *InSelf, FName FieldName, int32 &OutValueX, int32 &OutValueY, int32 &OutValueZ)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	HH_ALIGN(0x10) int32	outValue[3];
	if (!InSelf->GetFieldValue(FieldName, outValue, EPopcornFXParticleFieldType::Int3))
		return false;
	OutValueX = outValue[0];
	OutValueY = outValue[1];
	OutValueZ = outValue[2];
	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXFunctions::GetParticleFieldAsInt4(const UPopcornFXSceneComponent *InSelf, FName FieldName, int32 &OutValueX, int32 &OutValueY, int32 &OutValueZ, int32 &OutValueW)
{
	if (!HH_VERIFY(InSelf != null))
	{
		UE_LOG(LogPopcornFXFunctions, Warning, TEXT("Get Particle Field: Invalid InSelf"));
		return false;
	}
	HH_ALIGN(0x10) int32	outValue[4];
	if (!InSelf->GetFieldValue(FieldName, outValue, EPopcornFXParticleFieldType::Int4))
		return false;
	OutValueX = outValue[0];
	OutValueY = outValue[1];
	OutValueZ = outValue[2];
	OutValueW = outValue[3];
	return true;
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
