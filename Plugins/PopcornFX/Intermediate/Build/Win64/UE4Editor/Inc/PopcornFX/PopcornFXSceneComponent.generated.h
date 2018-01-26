// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef POPCORNFX_PopcornFXSceneComponent_generated_h
#error "PopcornFXSceneComponent.generated.h already included, missing '#pragma once' in PopcornFXSceneComponent.h"
#endif
#define POPCORNFX_PopcornFXSceneComponent_generated_h

#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_53_GENERATED_BODY \
	friend POPCORNFX_API class UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXFieldListener(); \
	POPCORNFX_API static class UScriptStruct* StaticStruct();


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_62_DELEGATE \
static inline void FPopcornFXRaiseEventSignature_DelegateWrapper(const FScriptDelegate& PopcornFXRaiseEventSignature) \
{ \
	PopcornFXRaiseEventSignature.ProcessDelegate<UObject>(NULL); \
}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execUnregisterEventListener) \
	{ \
		P_GET_PROPERTY(UDelegateProperty,Z_Param_Delegate); \
		P_GET_PROPERTY(UNameProperty,Z_Param_EventName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->UnregisterEventListener(FPopcornFXRaiseEventSignature(Z_Param_Delegate),Z_Param_EventName); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterEventListener) \
	{ \
		P_GET_PROPERTY(UDelegateProperty,Z_Param_Delegate); \
		P_GET_PROPERTY(UNameProperty,Z_Param_EventName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->RegisterEventListener(FPopcornFXRaiseEventSignature(Z_Param_Delegate),Z_Param_EventName); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execClear) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->Clear(); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execUnregisterEventListener) \
	{ \
		P_GET_PROPERTY(UDelegateProperty,Z_Param_Delegate); \
		P_GET_PROPERTY(UNameProperty,Z_Param_EventName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->UnregisterEventListener(FPopcornFXRaiseEventSignature(Z_Param_Delegate),Z_Param_EventName); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execRegisterEventListener) \
	{ \
		P_GET_PROPERTY(UDelegateProperty,Z_Param_Delegate); \
		P_GET_PROPERTY(UNameProperty,Z_Param_EventName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->RegisterEventListener(FPopcornFXRaiseEventSignature(Z_Param_Delegate),Z_Param_EventName); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execClear) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->Clear(); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXSceneComponent(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXSceneComponent(); \
public: \
	DECLARE_CLASS(UPopcornFXSceneComponent, UPrimitiveComponent, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXSceneComponent) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXSceneComponent(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXSceneComponent(); \
public: \
	DECLARE_CLASS(UPopcornFXSceneComponent, UPrimitiveComponent, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXSceneComponent) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXSceneComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXSceneComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXSceneComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXSceneComponent); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXSceneComponent(UPopcornFXSceneComponent&&); \
	NO_API UPopcornFXSceneComponent(const UPopcornFXSceneComponent&); \
public:


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXSceneComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXSceneComponent(UPopcornFXSceneComponent&&); \
	NO_API UPopcornFXSceneComponent(const UPopcornFXSceneComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXSceneComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXSceneComponent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXSceneComponent)


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_PRIVATE_PROPERTY_OFFSET
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_67_PROLOG
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_RPC_WRAPPERS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_INCLASS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_RPC_WRAPPERS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_INCLASS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h_70_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXSceneComponent."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXSceneComponent_h


#define FOREACH_ENUM_EPOPCORNFXHEAVYDEBUGMODE(op) \
	op(EPopcornFXHeavyDebugMode::None) \
	op(EPopcornFXHeavyDebugMode::ParticlePoints) \
	op(EPopcornFXHeavyDebugMode::MediumsBBox) 
#define FOREACH_ENUM_EPOPCORNFXSCENEBBMODE(op) \
	op(EPopcornFXSceneBBMode::Dynamic) \
	op(EPopcornFXSceneBBMode::DynamicPlusFixed) 
PRAGMA_ENABLE_DEPRECATION_WARNINGS
