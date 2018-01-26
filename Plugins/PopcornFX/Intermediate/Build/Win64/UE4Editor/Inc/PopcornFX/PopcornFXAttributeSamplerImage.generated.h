// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UTexture2D;
#ifdef POPCORNFX_PopcornFXAttributeSamplerImage_generated_h
#error "PopcornFXAttributeSamplerImage.generated.h already included, missing '#pragma once' in PopcornFXAttributeSamplerImage.h"
#endif
#define POPCORNFX_PopcornFXAttributeSamplerImage_generated_h

#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSetTexture) \
	{ \
		P_GET_OBJECT(UTexture2D,Z_Param_InTexture); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetTexture(Z_Param_InTexture); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSetTexture) \
	{ \
		P_GET_OBJECT(UTexture2D,Z_Param_InTexture); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetTexture(Z_Param_InTexture); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerImage(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerImage(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerImage, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerImage) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerImage(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerImage(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerImage, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerImage) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerImage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerImage) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerImage); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerImage); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerImage(UPopcornFXAttributeSamplerImage&&); \
	NO_API UPopcornFXAttributeSamplerImage(const UPopcornFXAttributeSamplerImage&); \
public:


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerImage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerImage(UPopcornFXAttributeSamplerImage&&); \
	NO_API UPopcornFXAttributeSamplerImage(const UPopcornFXAttributeSamplerImage&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerImage); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerImage); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerImage)


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_PRIVATE_PROPERTY_OFFSET
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_30_PROLOG
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_RPC_WRAPPERS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_INCLASS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_RPC_WRAPPERS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_INCLASS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h_33_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeSamplerImage."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerImage_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
