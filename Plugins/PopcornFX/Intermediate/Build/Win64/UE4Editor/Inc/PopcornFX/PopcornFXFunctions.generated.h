// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPopcornFXSceneComponent;
struct FLinearColor;
struct FVector;
struct FVector2D;
class UTexture;
class UObject;
class UPopcornFXEffect;
class USceneComponent;
struct FRotator;
class UPopcornFXEmitterComponent;
#ifdef POPCORNFX_PopcornFXFunctions_generated_h
#error "PopcornFXFunctions.generated.h already included, missing '#pragma once' in PopcornFXFunctions.h"
#endif
#define POPCORNFX_PopcornFXFunctions_generated_h

#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt4) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt4(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt3) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt3(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt2) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt2(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsLinearColor) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_STRUCT_REF(FLinearColor,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsLinearColor(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat4) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat4(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsVector) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsVector(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat3) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat3(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsVector2D) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_STRUCT_REF(FVector2D,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsVector2D(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat2) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat2(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUnregisterVirtualTextureOverride_GPU) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::UnregisterVirtualTextureOverride_GPU(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterVirtualTextureOverride_GPU) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_GET_OBJECT(UTexture,Z_Param_texture); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::RegisterVirtualTextureOverride_GPU(Z_Param_virtualPath,Z_Param_texture); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUnregisterVirtualTextureOverride_CPU) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::UnregisterVirtualTextureOverride_CPU(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterVirtualTextureOverride_CPU_FloatRGBA) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_GET_PROPERTY(UIntProperty,Z_Param_width); \
		P_GET_PROPERTY(UIntProperty,Z_Param_height); \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_pixels); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::RegisterVirtualTextureOverride_CPU_FloatRGBA(Z_Param_virtualPath,Z_Param_width,Z_Param_height,Z_Param_Out_pixels); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterVirtualTextureOverride_CPU_FloatR) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_GET_PROPERTY(UIntProperty,Z_Param_width); \
		P_GET_PROPERTY(UIntProperty,Z_Param_height); \
		P_GET_TARRAY_REF(float,Z_Param_Out_pixels); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::RegisterVirtualTextureOverride_CPU_FloatR(Z_Param_virtualPath,Z_Param_width,Z_Param_height,Z_Param_Out_pixels); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsTextureGPUInUse) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::IsTextureGPUInUse(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsTextureCPUInUse) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::IsTextureCPUInUse(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execNotifyObjectChanged) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_object); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UPopcornFXFunctions::NotifyObjectChanged(Z_Param_object); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSpawnEmitterAttached) \
	{ \
		P_GET_OBJECT(UPopcornFXEffect,Z_Param_Effect); \
		P_GET_OBJECT(USceneComponent,Z_Param_AttachToComponent); \
		P_GET_PROPERTY(UNameProperty,Z_Param_SceneName); \
		P_GET_PROPERTY(UNameProperty,Z_Param_AttachPointName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_PROPERTY(UByteProperty,Z_Param_LocationType); \
		P_GET_UBOOL(Z_Param_bStartEmitter); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=UPopcornFXFunctions::SpawnEmitterAttached(Z_Param_Effect,Z_Param_AttachToComponent,Z_Param_SceneName,Z_Param_AttachPointName,Z_Param_Location,Z_Param_Rotation,EAttachLocation::Type(Z_Param_LocationType),Z_Param_bStartEmitter,Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSpawnEmitterAtLocation) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UPopcornFXEffect,Z_Param_Effect); \
		P_GET_PROPERTY(UNameProperty,Z_Param_SceneName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_UBOOL(Z_Param_bStartEmitter); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=UPopcornFXFunctions::SpawnEmitterAtLocation(Z_Param_WorldContextObject,Z_Param_Effect,Z_Param_SceneName,Z_Param_Location,Z_Param_Rotation,Z_Param_bStartEmitter,Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execNotifySettingChanged) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->NotifySettingChanged(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXTotalParticleCount) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=UPopcornFXFunctions::PopcornFXTotalParticleCount(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXGlobalScaleInv) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=UPopcornFXFunctions::PopcornFXGlobalScaleInv(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXGlobalScale) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=UPopcornFXFunctions::PopcornFXGlobalScale(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXRuntimeVersion) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(FString*)Z_Param__Result=UPopcornFXFunctions::PopcornFXRuntimeVersion(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXPluginVersion) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(FString*)Z_Param__Result=UPopcornFXFunctions::PopcornFXPluginVersion(); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt4) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt4(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt3) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt3(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt2) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt2(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsInt) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsInt(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsLinearColor) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_STRUCT_REF(FLinearColor,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsLinearColor(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat4) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat4(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsVector) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsVector(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat3) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat3(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsVector2D) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_STRUCT_REF(FVector2D,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsVector2D(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat2) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat2(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetParticleFieldAsFloat) \
	{ \
		P_GET_OBJECT(UPopcornFXSceneComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_FieldName); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::GetParticleFieldAsFloat(Z_Param_InSelf,Z_Param_FieldName,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUnregisterVirtualTextureOverride_GPU) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::UnregisterVirtualTextureOverride_GPU(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterVirtualTextureOverride_GPU) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_GET_OBJECT(UTexture,Z_Param_texture); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::RegisterVirtualTextureOverride_GPU(Z_Param_virtualPath,Z_Param_texture); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execUnregisterVirtualTextureOverride_CPU) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::UnregisterVirtualTextureOverride_CPU(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterVirtualTextureOverride_CPU_FloatRGBA) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_GET_PROPERTY(UIntProperty,Z_Param_width); \
		P_GET_PROPERTY(UIntProperty,Z_Param_height); \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_pixels); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::RegisterVirtualTextureOverride_CPU_FloatRGBA(Z_Param_virtualPath,Z_Param_width,Z_Param_height,Z_Param_Out_pixels); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterVirtualTextureOverride_CPU_FloatR) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_GET_PROPERTY(UIntProperty,Z_Param_width); \
		P_GET_PROPERTY(UIntProperty,Z_Param_height); \
		P_GET_TARRAY_REF(float,Z_Param_Out_pixels); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::RegisterVirtualTextureOverride_CPU_FloatR(Z_Param_virtualPath,Z_Param_width,Z_Param_height,Z_Param_Out_pixels); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsTextureGPUInUse) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::IsTextureGPUInUse(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsTextureCPUInUse) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_virtualPath); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXFunctions::IsTextureCPUInUse(Z_Param_virtualPath); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execNotifyObjectChanged) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_object); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UPopcornFXFunctions::NotifyObjectChanged(Z_Param_object); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSpawnEmitterAttached) \
	{ \
		P_GET_OBJECT(UPopcornFXEffect,Z_Param_Effect); \
		P_GET_OBJECT(USceneComponent,Z_Param_AttachToComponent); \
		P_GET_PROPERTY(UNameProperty,Z_Param_SceneName); \
		P_GET_PROPERTY(UNameProperty,Z_Param_AttachPointName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_PROPERTY(UByteProperty,Z_Param_LocationType); \
		P_GET_UBOOL(Z_Param_bStartEmitter); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=UPopcornFXFunctions::SpawnEmitterAttached(Z_Param_Effect,Z_Param_AttachToComponent,Z_Param_SceneName,Z_Param_AttachPointName,Z_Param_Location,Z_Param_Rotation,EAttachLocation::Type(Z_Param_LocationType),Z_Param_bStartEmitter,Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSpawnEmitterAtLocation) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UPopcornFXEffect,Z_Param_Effect); \
		P_GET_PROPERTY(UNameProperty,Z_Param_SceneName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_UBOOL(Z_Param_bStartEmitter); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=UPopcornFXFunctions::SpawnEmitterAtLocation(Z_Param_WorldContextObject,Z_Param_Effect,Z_Param_SceneName,Z_Param_Location,Z_Param_Rotation,Z_Param_bStartEmitter,Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execNotifySettingChanged) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->NotifySettingChanged(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXTotalParticleCount) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=UPopcornFXFunctions::PopcornFXTotalParticleCount(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXGlobalScaleInv) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=UPopcornFXFunctions::PopcornFXGlobalScaleInv(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXGlobalScale) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=UPopcornFXFunctions::PopcornFXGlobalScale(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXRuntimeVersion) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(FString*)Z_Param__Result=UPopcornFXFunctions::PopcornFXRuntimeVersion(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execPopcornFXPluginVersion) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(FString*)Z_Param__Result=UPopcornFXFunctions::PopcornFXPluginVersion(); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXFunctions(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXFunctions(); \
public: \
	DECLARE_CLASS(UPopcornFXFunctions, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXFunctions) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXFunctions(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXFunctions(); \
public: \
	DECLARE_CLASS(UPopcornFXFunctions, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXFunctions) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXFunctions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXFunctions) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXFunctions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXFunctions); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXFunctions(UPopcornFXFunctions&&); \
	NO_API UPopcornFXFunctions(const UPopcornFXFunctions&); \
public:


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXFunctions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXFunctions(UPopcornFXFunctions&&); \
	NO_API UPopcornFXFunctions(const UPopcornFXFunctions&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXFunctions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXFunctions); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXFunctions)


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_PRIVATE_PROPERTY_OFFSET
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_20_PROLOG
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_RPC_WRAPPERS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_INCLASS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_INCLASS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h_23_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXFunctions."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXFunctions_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
