// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef POPCORNFX_PopcornFXAttributeSamplerText_generated_h
#error "PopcornFXAttributeSamplerText.generated.h already included, missing '#pragma once' in PopcornFXAttributeSamplerText.h"
#endif
#define POPCORNFX_PopcornFXAttributeSamplerText_generated_h

#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSetText) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_InText); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetText(Z_Param_InText); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSetText) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_InText); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetText(Z_Param_InText); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerText(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerText(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerText, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerText) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerText(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerText(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerText, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerText) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerText(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerText) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerText); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerText); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerText(UPopcornFXAttributeSamplerText&&); \
	NO_API UPopcornFXAttributeSamplerText(const UPopcornFXAttributeSamplerText&); \
public:


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerText(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerText(UPopcornFXAttributeSamplerText&&); \
	NO_API UPopcornFXAttributeSamplerText(const UPopcornFXAttributeSamplerText&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerText); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerText); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerText)


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_PRIVATE_PROPERTY_OFFSET
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_21_PROLOG
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_RPC_WRAPPERS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_INCLASS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_INCLASS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h_24_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeSamplerText."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerText_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
