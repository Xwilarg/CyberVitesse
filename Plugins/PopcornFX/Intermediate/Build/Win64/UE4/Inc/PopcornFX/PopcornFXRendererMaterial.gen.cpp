// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXRendererMaterial.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXRendererMaterial() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXMaterialType();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInstanceConstant_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXTextureAtlas_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXRendererMaterial_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXRendererMaterial();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEffect_NoRegister();
// End Cross Module References
	static UEnum* EPopcornFXMaterialType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXMaterialType, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXMaterialType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXMaterialType(EPopcornFXMaterialType_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXMaterialType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXMaterialType_CRC() { return 2454872421U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXMaterialType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXMaterialType"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXMaterialType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXMaterialType::Billboard_Additive", (int64)EPopcornFXMaterialType::Billboard_Additive },
				{ "EPopcornFXMaterialType::Billboard_AlphaBlend", (int64)EPopcornFXMaterialType::Billboard_AlphaBlend },
				{ "EPopcornFXMaterialType::Billboard_AlphaBlend_Lit", (int64)EPopcornFXMaterialType::Billboard_AlphaBlend_Lit },
				{ "EPopcornFXMaterialType::Billboard_AlphaBlendAdditive", (int64)EPopcornFXMaterialType::Billboard_AlphaBlendAdditive },
				{ "EPopcornFXMaterialType::Billboard_AlphaBlendAdditive_Lit", (int64)EPopcornFXMaterialType::Billboard_AlphaBlendAdditive_Lit },
				{ "EPopcornFXMaterialType::Billboard_Distortion", (int64)EPopcornFXMaterialType::Billboard_Distortion },
				{ "EPopcornFXMaterialType::Decal_AlphaTest", (int64)EPopcornFXMaterialType::Decal_AlphaTest },
				{ "EPopcornFXMaterialType::Decal_Blended", (int64)EPopcornFXMaterialType::Decal_Blended },
				{ "EPopcornFXMaterialType::Mesh_Additive", (int64)EPopcornFXMaterialType::Mesh_Additive },
				{ "EPopcornFXMaterialType::Mesh_Distortion", (int64)EPopcornFXMaterialType::Mesh_Distortion },
				{ "EPopcornFXMaterialType::Mesh_Solid", (int64)EPopcornFXMaterialType::Mesh_Solid },
				{ "EPopcornFXMaterialType::Mesh_Solid_Lit", (int64)EPopcornFXMaterialType::Mesh_Solid_Lit },
				{ "EPopcornFXMaterialType::Billboard_Additive_NoAlpha", (int64)EPopcornFXMaterialType::Billboard_Additive_NoAlpha },
				{ "EPopcornFXMaterialType::Mesh_Additive_NoAlpha", (int64)EPopcornFXMaterialType::Mesh_Additive_NoAlpha },
				{ "EPopcornFXMaterialType::__Max", (int64)EPopcornFXMaterialType::__Max },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "__Max.Hidden", "" },
				{ "Billboard_Additive_NoAlpha.ToolTip", "Put them in the end so that the material remap can work.." },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXMaterialType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXMaterialType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FPopcornFXSubRendererMaterial::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXSubRendererMaterial"), sizeof(FPopcornFXSubRendererMaterial), Get_Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXSubRendererMaterial(FPopcornFXSubRendererMaterial::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXSubRendererMaterial"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSubRendererMaterial
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSubRendererMaterial()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXSubRendererMaterial")),new UScriptStruct::TCppStructOps<FPopcornFXSubRendererMaterial>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSubRendererMaterial;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXSubRendererMaterial"), sizeof(FPopcornFXSubRendererMaterial), Get_Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXSubRendererMaterial>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaterialInstance_MetaData[] = {
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_MaterialInstance = { UE4CodeGen_Private::EPropertyClass::Object, "MaterialInstance", RF_Public|RF_Transient|RF_MarkAsNative, 0x0012000000080008, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, MaterialInstance), Z_Construct_UClass_UMaterialInstanceConstant_NoRegister, METADATA_PARAMS(NewProp_MaterialInstance_MetaData, ARRAY_COUNT(NewProp_MaterialInstance_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_RMId_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_m_RMId = { UE4CodeGen_Private::EPropertyClass::Int, "m_RMId", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000000, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, m_RMId), METADATA_PARAMS(NewProp_m_RMId_MetaData, ARRAY_COUNT(NewProp_m_RMId_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SubMeshId_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_SubMeshId = { UE4CodeGen_Private::EPropertyClass::Int, "SubMeshId", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, SubMeshId), METADATA_PARAMS(NewProp_SubMeshId_MetaData, ARRAY_COUNT(NewProp_SubMeshId_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMeshLOD_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_StaticMeshLOD = { UE4CodeGen_Private::EPropertyClass::UInt32, "StaticMeshLOD", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, StaticMeshLOD), METADATA_PARAMS(NewProp_StaticMeshLOD_MetaData, ARRAY_COUNT(NewProp_StaticMeshLOD_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMesh_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_StaticMesh = { UE4CodeGen_Private::EPropertyClass::Object, "StaticMesh", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, StaticMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(NewProp_StaticMesh_MetaData, ARRAY_COUNT(NewProp_StaticMesh_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DrawOrder_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_DrawOrder = { UE4CodeGen_Private::EPropertyClass::Int, "DrawOrder", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, DrawOrder), METADATA_PARAMS(NewProp_DrawOrder_MetaData, ARRAY_COUNT(NewProp_DrawOrder_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SoftnessDistance_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SoftnessDistance = { UE4CodeGen_Private::EPropertyClass::Float, "SoftnessDistance", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, SoftnessDistance), METADATA_PARAMS(NewProp_SoftnessDistance_MetaData, ARRAY_COUNT(NewProp_SoftnessDistance_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_UserData_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_UserData = { UE4CodeGen_Private::EPropertyClass::Str, "UserData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, UserData), METADATA_PARAMS(NewProp_UserData_MetaData, ARRAY_COUNT(NewProp_UserData_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CastShadow_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			auto NewProp_CastShadow_SetBit = [](void* Obj){ ((FPopcornFXSubRendererMaterial*)Obj)->CastShadow = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_CastShadow = { UE4CodeGen_Private::EPropertyClass::Bool, "CastShadow", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSubRendererMaterial), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_CastShadow_SetBit)>::SetBit, METADATA_PARAMS(NewProp_CastShadow_MetaData, ARRAY_COUNT(NewProp_CastShadow_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SoftAnimBlending_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			auto NewProp_SoftAnimBlending_SetBit = [](void* Obj){ ((FPopcornFXSubRendererMaterial*)Obj)->SoftAnimBlending = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_SoftAnimBlending = { UE4CodeGen_Private::EPropertyClass::Bool, "SoftAnimBlending", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSubRendererMaterial), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_SoftAnimBlending_SetBit)>::SetBit, METADATA_PARAMS(NewProp_SoftAnimBlending_MetaData, ARRAY_COUNT(NewProp_SoftAnimBlending_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_NoAlpha_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			auto NewProp_NoAlpha_SetBit = [](void* Obj){ ((FPopcornFXSubRendererMaterial*)Obj)->NoAlpha = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_NoAlpha = { UE4CodeGen_Private::EPropertyClass::Bool, "NoAlpha", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSubRendererMaterial), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_NoAlpha_SetBit)>::SetBit, METADATA_PARAMS(NewProp_NoAlpha_MetaData, ARRAY_COUNT(NewProp_NoAlpha_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TextureAtlas_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TextureAtlas = { UE4CodeGen_Private::EPropertyClass::Object, "TextureAtlas", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, TextureAtlas), Z_Construct_UClass_UPopcornFXTextureAtlas_NoRegister, METADATA_PARAMS(NewProp_TextureAtlas_MetaData, ARRAY_COUNT(NewProp_TextureAtlas_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TextureAlphaRemapper_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TextureAlphaRemapper = { UE4CodeGen_Private::EPropertyClass::Object, "TextureAlphaRemapper", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, TextureAlphaRemapper), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_TextureAlphaRemapper_MetaData, ARRAY_COUNT(NewProp_TextureAlphaRemapper_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TextureSpecular_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TextureSpecular = { UE4CodeGen_Private::EPropertyClass::Object, "TextureSpecular", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, TextureSpecular), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_TextureSpecular_MetaData, ARRAY_COUNT(NewProp_TextureSpecular_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TextureNormal_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TextureNormal = { UE4CodeGen_Private::EPropertyClass::Object, "TextureNormal", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, TextureNormal), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_TextureNormal_MetaData, ARRAY_COUNT(NewProp_TextureNormal_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TextureDiffuse_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TextureDiffuse = { UE4CodeGen_Private::EPropertyClass::Object, "TextureDiffuse", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, TextureDiffuse), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_TextureDiffuse_MetaData, ARRAY_COUNT(NewProp_TextureDiffuse_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaterialType_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_MaterialType = { UE4CodeGen_Private::EPropertyClass::Byte, "MaterialType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, MaterialType), Z_Construct_UEnum_PopcornFX_EPopcornFXMaterialType, METADATA_PARAMS(NewProp_MaterialType_MetaData, ARRAY_COUNT(NewProp_MaterialType_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "DisplayThumbnail", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Material = { UE4CodeGen_Private::EPropertyClass::Object, "Material", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(FPopcornFXSubRendererMaterial, Material), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(NewProp_Material_MetaData, ARRAY_COUNT(NewProp_Material_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_MaterialInstance,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_RMId,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SubMeshId,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMeshLOD,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMesh,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DrawOrder,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SoftnessDistance,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_UserData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CastShadow,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SoftAnimBlending,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_NoAlpha,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_TextureAtlas,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_TextureAlphaRemapper,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_TextureSpecular,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_TextureNormal,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_TextureDiffuse,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_MaterialType,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXSubRendererMaterial",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000005),
				sizeof(FPopcornFXSubRendererMaterial),
				alignof(FPopcornFXSubRendererMaterial),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial_CRC() { return 2917660403U; }
	void UPopcornFXRendererMaterial::StaticRegisterNativesUPopcornFXRendererMaterial()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXRendererMaterial_NoRegister()
	{
		return UPopcornFXRendererMaterial::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXRendererMaterial()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UObject,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Assets/PopcornFXRendererMaterial.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
				{ "ToolTip", "Save the Materials needed to Render the PopcornFX Particles of a PopcornFXEffect.\nA PopcornFXRendererMaterial instance match a specific set of PopcornFX CParticleRenderer_*\nbatched together for Rendering." },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BaseObjectFriendlyNames_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_BaseObjectFriendlyNames = { UE4CodeGen_Private::EPropertyClass::Array, "BaseObjectFriendlyNames", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXRendererMaterial, BaseObjectFriendlyNames), METADATA_PARAMS(NewProp_BaseObjectFriendlyNames_MetaData, ARRAY_COUNT(NewProp_BaseObjectFriendlyNames_MetaData)) };
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_BaseObjectFriendlyNames_Inner = { UE4CodeGen_Private::EPropertyClass::Str, "BaseObjectFriendlyNames", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000800020000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SubMaterials_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_SubMaterials = { UE4CodeGen_Private::EPropertyClass::Array, "SubMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010008002000041, 1, nullptr, STRUCT_OFFSET(UPopcornFXRendererMaterial, SubMaterials), METADATA_PARAMS(NewProp_SubMaterials_MetaData, ARRAY_COUNT(NewProp_SubMaterials_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_SubMaterials_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "SubMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000008000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FPopcornFXSubRendererMaterial, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BaseObjectNames_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterial" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_BaseObjectNames = { UE4CodeGen_Private::EPropertyClass::Array, "BaseObjectNames", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXRendererMaterial, BaseObjectNames), METADATA_PARAMS(NewProp_BaseObjectNames_MetaData, ARRAY_COUNT(NewProp_BaseObjectNames_MetaData)) };
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_BaseObjectNames_Inner = { UE4CodeGen_Private::EPropertyClass::Str, "BaseObjectNames", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000020000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParentEffect_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXRendererMaterial.h" },
			};
#endif
			static const UE4CodeGen_Private::FWeakObjectPropertyParams NewProp_ParentEffect = { UE4CodeGen_Private::EPropertyClass::WeakObject, "ParentEffect", RF_Public|RF_Transient|RF_MarkAsNative, 0x0014000000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXRendererMaterial, ParentEffect), Z_Construct_UClass_UPopcornFXEffect_NoRegister, METADATA_PARAMS(NewProp_ParentEffect_MetaData, ARRAY_COUNT(NewProp_ParentEffect_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BaseObjectFriendlyNames,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BaseObjectFriendlyNames_Inner,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SubMaterials,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SubMaterials_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BaseObjectNames,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BaseObjectNames_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ParentEffect,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXRendererMaterial>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXRendererMaterial::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00880080u,
				nullptr, 0,
				PropPointers, ARRAY_COUNT(PropPointers),
				nullptr,
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPopcornFXRendererMaterial, 1017898716);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXRendererMaterial(Z_Construct_UClass_UPopcornFXRendererMaterial, &UPopcornFXRendererMaterial::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXRendererMaterial"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXRendererMaterial);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
