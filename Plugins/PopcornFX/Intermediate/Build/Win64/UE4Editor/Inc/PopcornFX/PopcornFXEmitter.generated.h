// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPopcornFXEmitterComponent;
struct FVector;
#ifdef POPCORNFX_PopcornFXEmitter_generated_h
#error "PopcornFXEmitter.generated.h already included, missing '#pragma once' in PopcornFXEmitter.h"
#endif
#define POPCORNFX_PopcornFXEmitter_generated_h

#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_40_DELEGATE \
struct _Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms \
{ \
	UPopcornFXEmitterComponent* EmitterComponent; \
	FVector Location; \
	FVector Rotation; \
}; \
static inline void FPopcornFXEmitterTerminateSignature_DelegateWrapper(const FMulticastScriptDelegate& PopcornFXEmitterTerminateSignature, UPopcornFXEmitterComponent* EmitterComponent, FVector Location, FVector Rotation) \
{ \
	_Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms Parms; \
	Parms.EmitterComponent=EmitterComponent; \
	Parms.Location=Location; \
	Parms.Rotation=Rotation; \
	PopcornFXEmitterTerminateSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_33_DELEGATE \
struct _Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms \
{ \
	UPopcornFXEmitterComponent* EmitterComponent; \
	FVector Location; \
	FVector Rotation; \
}; \
static inline void FPopcornFXEmitterStopSignature_DelegateWrapper(const FMulticastScriptDelegate& PopcornFXEmitterStopSignature, UPopcornFXEmitterComponent* EmitterComponent, FVector Location, FVector Rotation) \
{ \
	_Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms Parms; \
	Parms.EmitterComponent=EmitterComponent; \
	Parms.Location=Location; \
	Parms.Rotation=Rotation; \
	PopcornFXEmitterStopSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_25_DELEGATE \
struct _Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms \
{ \
	UPopcornFXEmitterComponent* EmitterComponent; \
	FVector Location; \
	FVector Rotation; \
}; \
static inline void FPopcornFXEmitterStartSignature_DelegateWrapper(const FMulticastScriptDelegate& PopcornFXEmitterStartSignature, UPopcornFXEmitterComponent* EmitterComponent, FVector Location, FVector Rotation) \
{ \
	_Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms Parms; \
	Parms.EmitterComponent=EmitterComponent; \
	Parms.Location=Location; \
	Parms.Rotation=Rotation; \
	PopcornFXEmitterStartSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnInterpToggle) \
	{ \
		P_GET_UBOOL(Z_Param_bEnable); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnInterpToggle(Z_Param_bEnable); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnInterpToggle) \
	{ \
		P_GET_UBOOL(Z_Param_bEnable); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnInterpToggle(Z_Param_bEnable); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAPopcornFXEmitter(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_APopcornFXEmitter(); \
public: \
	DECLARE_CLASS(APopcornFXEmitter, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(APopcornFXEmitter) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_INCLASS \
private: \
	static void StaticRegisterNativesAPopcornFXEmitter(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_APopcornFXEmitter(); \
public: \
	DECLARE_CLASS(APopcornFXEmitter, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(APopcornFXEmitter) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API APopcornFXEmitter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(APopcornFXEmitter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APopcornFXEmitter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APopcornFXEmitter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APopcornFXEmitter(APopcornFXEmitter&&); \
	NO_API APopcornFXEmitter(const APopcornFXEmitter&); \
public:


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API APopcornFXEmitter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APopcornFXEmitter(APopcornFXEmitter&&); \
	NO_API APopcornFXEmitter(const APopcornFXEmitter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APopcornFXEmitter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APopcornFXEmitter); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(APopcornFXEmitter)


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_PRIVATE_PROPERTY_OFFSET
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_42_PROLOG
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_RPC_WRAPPERS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_INCLASS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_RPC_WRAPPERS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_INCLASS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h_45_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXEmitter."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXEmitter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
