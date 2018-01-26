// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FLinearColor;
struct FVector;
#ifdef POPCORNFX_PopcornFXAttributeSamplerCurveDynamic_generated_h
#error "PopcornFXAttributeSamplerCurveDynamic.generated.h already included, missing '#pragma once' in PopcornFXAttributeSamplerCurveDynamic.h"
#endif
#define POPCORNFX_PopcornFXAttributeSamplerCurveDynamic_generated_h

#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSetTangents4D) \
	{ \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_ArriveTangents); \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_LeaveTangents); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTangents4D(Z_Param_Out_ArriveTangents,Z_Param_Out_LeaveTangents); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetTangents3D) \
	{ \
		P_GET_TARRAY_REF(FVector,Z_Param_Out_ArriveTangents); \
		P_GET_TARRAY_REF(FVector,Z_Param_Out_LeaveTangents); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTangents3D(Z_Param_Out_ArriveTangents,Z_Param_Out_LeaveTangents); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetTangents1D) \
	{ \
		P_GET_TARRAY_REF(float,Z_Param_Out_ArriveTangents); \
		P_GET_TARRAY_REF(float,Z_Param_Out_LeaveTangents); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTangents1D(Z_Param_Out_ArriveTangents,Z_Param_Out_LeaveTangents); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetValues4D) \
	{ \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_Values); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetValues4D(Z_Param_Out_Values); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetValues3D) \
	{ \
		P_GET_TARRAY_REF(FVector,Z_Param_Out_Values); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetValues3D(Z_Param_Out_Values); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetValues1D) \
	{ \
		P_GET_TARRAY_REF(float,Z_Param_Out_Values); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetValues1D(Z_Param_Out_Values); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetTimes) \
	{ \
		P_GET_TARRAY_REF(float,Z_Param_Out_Times); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTimes(Z_Param_Out_Times); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSetTangents4D) \
	{ \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_ArriveTangents); \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_LeaveTangents); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTangents4D(Z_Param_Out_ArriveTangents,Z_Param_Out_LeaveTangents); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetTangents3D) \
	{ \
		P_GET_TARRAY_REF(FVector,Z_Param_Out_ArriveTangents); \
		P_GET_TARRAY_REF(FVector,Z_Param_Out_LeaveTangents); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTangents3D(Z_Param_Out_ArriveTangents,Z_Param_Out_LeaveTangents); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetTangents1D) \
	{ \
		P_GET_TARRAY_REF(float,Z_Param_Out_ArriveTangents); \
		P_GET_TARRAY_REF(float,Z_Param_Out_LeaveTangents); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTangents1D(Z_Param_Out_ArriveTangents,Z_Param_Out_LeaveTangents); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetValues4D) \
	{ \
		P_GET_TARRAY_REF(FLinearColor,Z_Param_Out_Values); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetValues4D(Z_Param_Out_Values); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetValues3D) \
	{ \
		P_GET_TARRAY_REF(FVector,Z_Param_Out_Values); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetValues3D(Z_Param_Out_Values); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetValues1D) \
	{ \
		P_GET_TARRAY_REF(float,Z_Param_Out_Values); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetValues1D(Z_Param_Out_Values); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetTimes) \
	{ \
		P_GET_TARRAY_REF(float,Z_Param_Out_Times); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=this->SetTimes(Z_Param_Out_Times); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerCurveDynamic(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerCurveDynamic, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerCurveDynamic) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeSamplerCurveDynamic(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeSamplerCurveDynamic, UPopcornFXAttributeSampler, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeSamplerCurveDynamic) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerCurveDynamic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerCurveDynamic) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerCurveDynamic); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerCurveDynamic); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerCurveDynamic(UPopcornFXAttributeSamplerCurveDynamic&&); \
	NO_API UPopcornFXAttributeSamplerCurveDynamic(const UPopcornFXAttributeSamplerCurveDynamic&); \
public:


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeSamplerCurveDynamic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeSamplerCurveDynamic(UPopcornFXAttributeSamplerCurveDynamic&&); \
	NO_API UPopcornFXAttributeSamplerCurveDynamic(const UPopcornFXAttributeSamplerCurveDynamic&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeSamplerCurveDynamic); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeSamplerCurveDynamic); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeSamplerCurveDynamic)


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_PRIVATE_PROPERTY_OFFSET
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_39_PROLOG
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_RPC_WRAPPERS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_INCLASS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_RPC_WRAPPERS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_INCLASS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h_42_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeSamplerCurveDynamic."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeSamplerCurveDynamic_h


#define FOREACH_ENUM_ECURVEDYNAMICINTERPOLATOR(op) \
	op(ECurveDynamicInterpolator::Linear) 
PRAGMA_ENABLE_DEPRECATION_WARNINGS
