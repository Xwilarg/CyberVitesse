// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPopcornFXEmitterComponent;
struct FLinearColor;
struct FVector;
struct FVector2D;
#ifdef POPCORNFX_PopcornFXAttributeFunctions_generated_h
#error "PopcornFXAttributeFunctions.generated.h already included, missing '#pragma once' in PopcornFXAttributeFunctions.h"
#endif
#define POPCORNFX_PopcornFXAttributeFunctions_generated_h

#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueZ); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ,Z_Param_InValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsLinearColor) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT_REF(FLinearColor,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsLinearColor(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueW); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsLinearColor) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT(FLinearColor,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueZ); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueW); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ,Z_Param_InValueW,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsVector) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsVector(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsVector) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT(FVector,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsVector(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueZ); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsVector2D) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT_REF(FVector2D,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsVector2D(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsVector2D) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT(FVector2D,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsVector2D(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueY); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execResetToDefaultValue) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::ResetToDefaultValue(Z_Param_InSelf,Z_Param_InAttributeIndex); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execFindAttributeIndex) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_InAttributeName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=UPopcornFXAttributeFunctions::FindAttributeIndex(Z_Param_InSelf,Z_Param_InAttributeName); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueZ); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueW); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ,Z_Param_InValueW); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueZ); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValueY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsInt) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UIntProperty,Z_Param_Out_OutValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsInt(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsInt) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InValue); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsInt(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsLinearColor) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT_REF(FLinearColor,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsLinearColor(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueW); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_Out_OutValueW,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsLinearColor) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT(FLinearColor,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat4) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueZ); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueW); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat4(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ,Z_Param_InValueW,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsVector) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsVector(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueZ); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_Out_OutValueZ,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsVector) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT(FVector,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsVector(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat3) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueY); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueZ); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat3(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InValueZ,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsVector2D) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT_REF(FVector2D,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsVector2D(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueX); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValueY); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValueX,Z_Param_Out_OutValueY,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsVector2D) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_STRUCT(FVector2D,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsVector2D(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat2) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueX); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValueY); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat2(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValueX,Z_Param_InValueY,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetAttributeAsFloat) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY_REF(UFloatProperty,Z_Param_Out_OutValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::GetAttributeAsFloat(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_Out_OutValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execSetAttributeAsFloat) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_InValue); \
		P_GET_UBOOL(Z_Param_InApplyGlobalScale); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::SetAttributeAsFloat(Z_Param_InSelf,Z_Param_InAttributeIndex,Z_Param_InValue,Z_Param_InApplyGlobalScale); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execResetToDefaultValue) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UIntProperty,Z_Param_InAttributeIndex); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UPopcornFXAttributeFunctions::ResetToDefaultValue(Z_Param_InSelf,Z_Param_InAttributeIndex); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execFindAttributeIndex) \
	{ \
		P_GET_OBJECT(UPopcornFXEmitterComponent,Z_Param_InSelf); \
		P_GET_PROPERTY(UNameProperty,Z_Param_InAttributeName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(int32*)Z_Param__Result=UPopcornFXAttributeFunctions::FindAttributeIndex(Z_Param_InSelf,Z_Param_InAttributeName); \
		P_NATIVE_END; \
	}


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeFunctions(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeFunctions(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeFunctions, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeFunctions) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_INCLASS \
private: \
	static void StaticRegisterNativesUPopcornFXAttributeFunctions(); \
	friend POPCORNFX_API class UClass* Z_Construct_UClass_UPopcornFXAttributeFunctions(); \
public: \
	DECLARE_CLASS(UPopcornFXAttributeFunctions, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/PopcornFX"), NO_API) \
	DECLARE_SERIALIZER(UPopcornFXAttributeFunctions) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeFunctions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeFunctions) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeFunctions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeFunctions); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeFunctions(UPopcornFXAttributeFunctions&&); \
	NO_API UPopcornFXAttributeFunctions(const UPopcornFXAttributeFunctions&); \
public:


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPopcornFXAttributeFunctions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPopcornFXAttributeFunctions(UPopcornFXAttributeFunctions&&); \
	NO_API UPopcornFXAttributeFunctions(const UPopcornFXAttributeFunctions&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPopcornFXAttributeFunctions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPopcornFXAttributeFunctions); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPopcornFXAttributeFunctions)


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_PRIVATE_PROPERTY_OFFSET
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_20_PROLOG
#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_RPC_WRAPPERS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_INCLASS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_PRIVATE_PROPERTY_OFFSET \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_INCLASS_NO_PURE_DECLS \
	CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h_23_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PopcornFXAttributeFunctions."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CyberVitesse_Plugins_PopcornFX_Source_PopcornFX_Public_PopcornFXAttributeFunctions_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
