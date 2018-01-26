// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FVector;
#ifdef POPCORNFX_PopcornFXAttributeSamplerShape_generated_h
#error "PopcornFXAttributeSamplerShape.generated.h already included, missing '#pragma once' in PopcornFXAttributeSamplerShape.h"
#endif
#define POPCORNFX_PopcornFXAttributeSamplerShape_generated_h

#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSetHeight) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_height); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetHeight(Z_Param_height); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetInnerRadius) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_innerRadius); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetInnerRadius(Z_Param_innerRadius); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetBoxDimension) \
	{ \
		P_GET_STRUCT(FVector,Z_Param_boxDimensions); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetBoxDimension(Z_Param_boxDimensions); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetWeight) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_height); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetWeight(Z_Param_height); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetRadius) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_radius); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetRadius(Z_Param_radius); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSetHeight) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_height); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetHeight(Z_Param_height); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetInnerRadius) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_innerRadius); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetInnerRadius(Z_Param_innerRadius); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetBoxDimension) \
	{ \
		P_GET_STRUCT(FVector,Z_Param_boxDimensions); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetBoxDimension(Z_Param_boxDimensions); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetWeight) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_height); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetWeight(Z_Param_height); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetRadius) \
	{ \
		P_GET_PROPERTY(UFloatProperty,Z_Param_radius); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetRadius(Z_Param_radius); \
		P_NATIVE_END; \
	}


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerShape(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerShape(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerShape, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerShape) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerShape(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerShape(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerShape, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerShape) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerShape(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerShape) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerShape); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerShape); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerShape(UPopcornFXAttributeSamplerShape&&); \
	NO_API UPopcornFXAttributeSamplerShape(const UPopcornFXAttributeSamplerShape&); \
public:


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerShape(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerShape(UPopcornFXAttributeSamplerShape&&); \
	NO_API UPopcornFXAttributeSamplerShape(const UPopcornFXAttributeSamplerShape&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerShape); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerShape); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerShape)


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_PRIVATE_PROPERTY_OFFSET
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_40_PROLOG
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_RPC_WRAPPERS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_INCLASS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_RPC_WRAPPERS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_INCLASS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h_43_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeSamplerShape."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerShape_h


#define FOREACH_ENUM_EPOPCORNFXSHAPECOLLECTIONSAMPLINGHEURISTIC(op) \
	op(EPopcornFXShapeCollectionSamplingHeuristic::NoWeight) \
	op(EPopcornFXShapeCollectionSamplingHeuristic::WeightWithVolume) 
PRAGMA_ENABLE_DEPRECATION_WARNINGS
