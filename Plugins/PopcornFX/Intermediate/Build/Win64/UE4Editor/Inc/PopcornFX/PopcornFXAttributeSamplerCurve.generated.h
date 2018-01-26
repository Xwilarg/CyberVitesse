// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UCurveBase;
#ifdef POPCORNFX_PopcornFXAttributeSamplerCurve_generated_h
#error "PopcornFXAttributeSamplerCurve.generated.h already included, missing '#pragma once' in PopcornFXAttributeSamplerCurve.h"
#endif
#define POPCORNFX_PopcornFXAttributeSamplerCurve_generated_h

#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSetCurve) \
	{ \
		P_GET_OBJECT(UCurveBase,Z_Param_InCurve); \
		P_GET_UBOOL(Z_Param_InIsSecondCurve); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetCurve(Z_Param_InCurve,Z_Param_InIsSecondCurve); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetCurveDimension) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_InCurveDimension); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetCurveDimension(EAttributeSamplerCurveDimension::Type(Z_Param_InCurveDimension)); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSetCurve) \
	{ \
		P_GET_OBJECT(UCurveBase,Z_Param_InCurve); \
		P_GET_UBOOL(Z_Param_InIsSecondCurve); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetCurve(Z_Param_InCurve,Z_Param_InIsSecondCurve); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetCurveDimension) \
	{ \
		P_GET_PROPERTY(UByteProperty,Z_Param_InCurveDimension); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->SetCurveDimension(EAttributeSamplerCurveDimension::Type(Z_Param_InCurveDimension)); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerCurve(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurve(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerCurve, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerCurve) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerCurve(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurve(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerCurve, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerCurve) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerCurve(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerCurve) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerCurve); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerCurve); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerCurve(UPopcornFXAttributeSamplerCurve&&); \
	NO_API UPopcornFXAttributeSamplerCurve(const UPopcornFXAttributeSamplerCurve&); \
public:


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerCurve(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerCurve(UPopcornFXAttributeSamplerCurve&&); \
	NO_API UPopcornFXAttributeSamplerCurve(const UPopcornFXAttributeSamplerCurve&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerCurve); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerCurve); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerCurve)


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_PRIVATE_PROPERTY_OFFSET
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_43_PROLOG
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_RPC_WRAPPERS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_INCLASS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_RPC_WRAPPERS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_INCLASS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h_46_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeSamplerCurve."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurve_h


#define FOREACH_ENUM_EATTRIBUTESAMPLERCURVEDIMENSION(op) \
	op(EAttributeSamplerCurveDimension::Float1) \
	op(EAttributeSamplerCurveDimension::Float2) \
	op(EAttributeSamplerCurveDimension::Float3) 
PRAGMA_ENABLE_DEPRECATION_WARNINGS
