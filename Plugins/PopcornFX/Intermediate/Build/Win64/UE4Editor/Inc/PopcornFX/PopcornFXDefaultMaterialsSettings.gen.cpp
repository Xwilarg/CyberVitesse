// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXDefaultMaterialsSettings() {}
// Cross Module References
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FSoftObjectPath();
// End Cross Module References
class UScriptStruct* FPopcornFXDefaultMaterialsSettings::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXDefaultMaterialsSettings"), sizeof(FPopcornFXDefaultMaterialsSettings), Get_Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXDefaultMaterialsSettings(FPopcornFXDefaultMaterialsSettings::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXDefaultMaterialsSettings"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXDefaultMaterialsSettings
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXDefaultMaterialsSettings()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXDefaultMaterialsSettings")),new UScriptStruct::TCppStructOps<FPopcornFXDefaultMaterialsSettings>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXDefaultMaterialsSettings;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXDefaultMaterialsSettings"), sizeof(FPopcornFXDefaultMaterialsSettings), Get_Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXDefaultMaterialsSettings>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Mesh_Solid_Lit_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Lit Solid Mesh Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Mesh_Solid_Lit = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Mesh_Solid_Lit", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Mesh_Solid_Lit), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Mesh_Solid_Lit_MetaData, ARRAY_COUNT(NewProp_Material_Mesh_Solid_Lit_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Mesh_Solid_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Solid Mesh Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Mesh_Solid = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Mesh_Solid", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Mesh_Solid), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Mesh_Solid_MetaData, ARRAY_COUNT(NewProp_Material_Mesh_Solid_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Mesh_Distortion_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Distortion Mesh Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Mesh_Distortion = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Mesh_Distortion", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Mesh_Distortion), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Mesh_Distortion_MetaData, ARRAY_COUNT(NewProp_Material_Mesh_Distortion_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Mesh_Additive_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Additive Mesh Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Mesh_Additive = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Mesh_Additive", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Mesh_Additive), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Mesh_Additive_MetaData, ARRAY_COUNT(NewProp_Material_Mesh_Additive_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Decal_Blended_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Blended Decal Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Decal_Blended = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Decal_Blended", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Decal_Blended), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Decal_Blended_MetaData, ARRAY_COUNT(NewProp_Material_Decal_Blended_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Decal_AlphaTest_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "AlphaTest Decal Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Decal_AlphaTest = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Decal_AlphaTest", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Decal_AlphaTest), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Decal_AlphaTest_MetaData, ARRAY_COUNT(NewProp_Material_Decal_AlphaTest_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Billboard_Distortion_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Distortion Billboard Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Billboard_Distortion = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Billboard_Distortion", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Billboard_Distortion), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Billboard_Distortion_MetaData, ARRAY_COUNT(NewProp_Material_Billboard_Distortion_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Billboard_AlphaBlendAdditive_Lit_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Lit AlphaBlend Additive Billboard Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Billboard_AlphaBlendAdditive_Lit = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Billboard_AlphaBlendAdditive_Lit", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Billboard_AlphaBlendAdditive_Lit), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Billboard_AlphaBlendAdditive_Lit_MetaData, ARRAY_COUNT(NewProp_Material_Billboard_AlphaBlendAdditive_Lit_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Billboard_AlphaBlendAdditive_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "AlphaBlend Additive Billboard Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Billboard_AlphaBlendAdditive = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Billboard_AlphaBlendAdditive", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Billboard_AlphaBlendAdditive), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Billboard_AlphaBlendAdditive_MetaData, ARRAY_COUNT(NewProp_Material_Billboard_AlphaBlendAdditive_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Billboard_AlphaBlend_Lit_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Lit AlphaBlend Billboard Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Billboard_AlphaBlend_Lit = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Billboard_AlphaBlend_Lit", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Billboard_AlphaBlend_Lit), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Billboard_AlphaBlend_Lit_MetaData, ARRAY_COUNT(NewProp_Material_Billboard_AlphaBlend_Lit_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Billboard_AlphaBlend_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "AlphaBlend Billboard Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Billboard_AlphaBlend = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Billboard_AlphaBlend", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Billboard_AlphaBlend), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Billboard_AlphaBlend_MetaData, ARRAY_COUNT(NewProp_Material_Billboard_AlphaBlend_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Material_Billboard_Additive_MetaData[] = {
				{ "AllowedClasses", "MaterialInterface" },
				{ "Category", "Default Materials" },
				{ "ModuleRelativePath", "Private/BackwardCompat/PopcornFXDefaultMaterialsSettings.h" },
				{ "ToolTip", "Additive Billboard Default Material." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Material_Billboard_Additive = { UE4CodeGen_Private::EPropertyClass::Struct, "Material_Billboard_Additive", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FPopcornFXDefaultMaterialsSettings, Material_Billboard_Additive), Z_Construct_UScriptStruct_FSoftObjectPath, METADATA_PARAMS(NewProp_Material_Billboard_Additive_MetaData, ARRAY_COUNT(NewProp_Material_Billboard_Additive_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Mesh_Solid_Lit,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Mesh_Solid,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Mesh_Distortion,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Mesh_Additive,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Decal_Blended,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Decal_AlphaTest,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Billboard_Distortion,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Billboard_AlphaBlendAdditive_Lit,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Billboard_AlphaBlendAdditive,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Billboard_AlphaBlend_Lit,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Billboard_AlphaBlend,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Material_Billboard_Additive,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXDefaultMaterialsSettings",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXDefaultMaterialsSettings),
				alignof(FPopcornFXDefaultMaterialsSettings),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings_CRC() { return 1692901858U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
