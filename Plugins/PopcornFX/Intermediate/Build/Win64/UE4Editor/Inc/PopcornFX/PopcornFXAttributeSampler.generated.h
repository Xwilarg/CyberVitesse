// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef POPCORNFX_PopcornFXAttributeSampler_generated_h
#error "PopcornFXAttributeSampler.generated.h already included, missing '#pragma once' in PopcornFXAttributeSampler.h"
#endif
#define POPCORNFX_PopcornFXAttributeSampler_generated_h

#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_RPC_WRAPPERS
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_RPC_WRAPPERS_NO_PURE_DECLS
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSampler(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSampler(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSampler, USceneComponent, COMPILED_IN_FLAGS(CLASS_Abstract), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSampler) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSampler(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSampler(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSampler, USceneComponent, COMPILED_IN_FLAGS(CLASS_Abstract), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSampler) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSampler(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSampler) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSampler); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSampler); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSampler(UPopcornFXAttributeSampler&&); \
	NO_API UPopcornFXAttributeSampler(const UPopcornFXAttributeSampler&); \
public:


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSampler(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSampler(UPopcornFXAttributeSampler&&); \
	NO_API UPopcornFXAttributeSampler(const UPopcornFXAttributeSampler&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSampler); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSampler); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSampler)


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_PRIVATE_PROPERTY_OFFSET
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_72_PROLOG
#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_RPC_WRAPPERS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_INCLASS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_PRIVATE_PROPERTY_OFFSET \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_RPC_WRAPPERS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_INCLASS_NO_PURE_DECLS \
	depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h_75_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeSampler."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID depot_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSampler_h


#define FOREACH_ENUM_EPOPCORNFXATTRIBSAMPLERSHAPETYPE(op) \
	op(EPopcornFXAttribSamplerShapeType::Box) \
	op(EPopcornFXAttribSamplerShapeType::Sphere) \
	op(EPopcornFXAttribSamplerShapeType::ComplexEllipsoid) \
	op(EPopcornFXAttribSamplerShapeType::Cylinder) \
	op(EPopcornFXAttribSamplerShapeType::Capsule) \
	op(EPopcornFXAttribSamplerShapeType::Cone) \
	op(EPopcornFXAttribSamplerShapeType::Plane) \
	op(EPopcornFXAttribSamplerShapeType::Mesh) 
#define FOREACH_ENUM_EPOPCORNFXATTRIBUTESAMPLERTYPE(op) \
	op(EPopcornFXAttributeSamplerType::None) \
	op(EPopcornFXAttributeSamplerType::Shape) \
	op(EPopcornFXAttributeSamplerType::Image) \
	op(EPopcornFXAttributeSamplerType::Curve) 
#define FOREACH_ENUM_EPOPCORNFXATTRIBUTESAMPLERCOMPONENTTYPE(op) \
	op(EPopcornFXAttributeSamplerComponentType::Shape) \
	op(EPopcornFXAttributeSamplerComponentType::SkinnedMesh) \
	op(EPopcornFXAttributeSamplerComponentType::Image) \
	op(EPopcornFXAttributeSamplerComponentType::Curve) 
PRAGMA_ENABLE_DEPRECATION_WARNINGS
