// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class USceneComponent;
struct FVector;
struct FRotator;
class UPopcornFXEmitterComponent;
class UPopcornFXEffect;
#ifdef POPCORNFX_PopcornFXEmitterComponent_generated_h
#error "PopcornFXEmitterComponent.generated.h already included, missing '#pragma once' in PopcornFXEmitterComponent.h"
#endif
#define POPCORNFX_PopcornFXEmitterComponent_generated_h

#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execCopyAndStartEmitterAttached) \
	{ \
		P_GET_OBJECT(USceneComponent,Z_Param_AttachToComponent); \
		P_GET_PROPERTY(UNameProperty,Z_Param_AttachPointName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_PROPERTY(UByteProperty,Z_Param_LocationType); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=this->CopyAndStartEmitterAttached(Z_Param_AttachToComponent,Z_Param_AttachPointName,Z_Param_Location,Z_Param_Rotation,EAttachLocation::Type(Z_Param_LocationType),Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCopyAndStartEmitterAtLocation) \
	{ \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=this->CopyAndStartEmitterAtLocation(Z_Param_Location,Z_Param_Rotation,Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execResetAttributesToDefault) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->ResetAttributesToDefault(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsEmitterAlive) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->IsEmitterAlive(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsEmitterStarted) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->IsEmitterStarted(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsEmitterEmitting) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->IsEmitterEmitting(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execTerminateEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->TerminateEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execToggleEmitter) \
	{ \
		P_GET_UBOOL(Z_Param_startEmitter); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->ToggleEmitter(Z_Param_startEmitter); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStopEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->StopEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRestartEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RestartEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStartEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->StartEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetEffect) \
	{ \
		P_GET_OBJECT(UPopcornFXEffect,Z_Param_InEffect); \
		P_GET_UBOOL(Z_Param_bStartEmitter); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetEffect(Z_Param_InEffect,Z_Param_bStartEmitter); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execCopyAndStartEmitterAttached) \
	{ \
		P_GET_OBJECT(USceneComponent,Z_Param_AttachToComponent); \
		P_GET_PROPERTY(UNameProperty,Z_Param_AttachPointName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_PROPERTY(UByteProperty,Z_Param_LocationType); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=this->CopyAndStartEmitterAttached(Z_Param_AttachToComponent,Z_Param_AttachPointName,Z_Param_Location,Z_Param_Rotation,EAttachLocation::Type(Z_Param_LocationType),Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCopyAndStartEmitterAtLocation) \
	{ \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_STRUCT(FRotator,Z_Param_Rotation); \
		P_GET_UBOOL(Z_Param_bAutoDestroy); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(UPopcornFXEmitterComponent**)Z_Param__Result=this->CopyAndStartEmitterAtLocation(Z_Param_Location,Z_Param_Rotation,Z_Param_bAutoDestroy); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execResetAttributesToDefault) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->ResetAttributesToDefault(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsEmitterAlive) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->IsEmitterAlive(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsEmitterStarted) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->IsEmitterStarted(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execIsEmitterEmitting) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->IsEmitterEmitting(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execTerminateEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->TerminateEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execToggleEmitter) \
	{ \
		P_GET_UBOOL(Z_Param_startEmitter); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->ToggleEmitter(Z_Param_startEmitter); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStopEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->StopEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRestartEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->RestartEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execStartEmitter) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->StartEmitter(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetEffect) \
	{ \
		P_GET_OBJECT(UPopcornFXEffect,Z_Param_InEffect); \
		P_GET_UBOOL(Z_Param_bStartEmitter); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetEffect(Z_Param_InEffect,Z_Param_bStartEmitter); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXEmitterComponent(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXEmitterComponent(); \
public: \
	DECLARE_CLASS(UPopcornFXEmitterComponent, USceneComponent, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXEmitterComponent) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXEmitterComponent(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXEmitterComponent(); \
public: \
	DECLARE_CLASS(UPopcornFXEmitterComponent, USceneComponent, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXEmitterComponent) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXEmitterComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXEmitterComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXEmitterComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXEmitterComponent); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXEmitterComponent(UPopcornFXEmitterComponent&&); \
	NO_API UPopcornFXEmitterComponent(const UPopcornFXEmitterComponent&); \
public:


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXEmitterComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXEmitterComponent(UPopcornFXEmitterComponent&&); \
	NO_API UPopcornFXEmitterComponent(const UPopcornFXEmitterComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXEmitterComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXEmitterComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXEmitterComponent)


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_PRIVATE_PROPERTY_OFFSET
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_39_PROLOG
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_RPC_WRAPPERS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_INCLASS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_RPC_WRAPPERS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_INCLASS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h_42_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXEmitterComponent."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitterComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
