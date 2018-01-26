// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXAssetDep.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAssetDep() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDepType();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXFieldPath();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAssetDep_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAssetDep();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
// End Cross Module References
	static UEnum* EPopcornFXAssetDepType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDepType, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXAssetDepType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXAssetDepType(EPopcornFXAssetDepType_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXAssetDepType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDepType_CRC() { return 3733342800U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDepType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXAssetDepType"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDepType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXAssetDepType::None", (int64)EPopcornFXAssetDepType::None },
				{ "EPopcornFXAssetDepType::Effect", (int64)EPopcornFXAssetDepType::Effect },
				{ "EPopcornFXAssetDepType::Texture", (int64)EPopcornFXAssetDepType::Texture },
				{ "EPopcornFXAssetDepType::TextureAtlas", (int64)EPopcornFXAssetDepType::TextureAtlas },
				{ "EPopcornFXAssetDepType::Font", (int64)EPopcornFXAssetDepType::Font },
				{ "EPopcornFXAssetDepType::Mesh", (int64)EPopcornFXAssetDepType::Mesh },
				{ "EPopcornFXAssetDepType::Video", (int64)EPopcornFXAssetDepType::Video },
				{ "EPopcornFXAssetDepType::Sound", (int64)EPopcornFXAssetDepType::Sound },
				{ "EPopcornFXAssetDepType::SimCache", (int64)EPopcornFXAssetDepType::SimCache },
				{ "EPopcornFXAssetDepType::File", (int64)EPopcornFXAssetDepType::File },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "File.ToolTip", "path to a simulation cache" },
				{ "Font.ToolTip", "path to a texture atlas" },
				{ "Mesh.ToolTip", "path to a font" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
				{ "SimCache.ToolTip", "path to an audio source" },
				{ "Sound.ToolTip", "path to a video" },
				{ "Texture.ToolTip", "path to an effect" },
				{ "TextureAtlas.ToolTip", "path to a texture : display image browser and preview" },
				{ "Video.ToolTip", "path to a mesh" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXAssetDepType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXAssetDepType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FPopcornFXFieldPath::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXFieldPath_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXFieldPath, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXFieldPath"), sizeof(FPopcornFXFieldPath), Get_Z_Construct_UScriptStruct_FPopcornFXFieldPath_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXFieldPath(FPopcornFXFieldPath::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXFieldPath"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXFieldPath
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXFieldPath()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXFieldPath")),new UScriptStruct::TCppStructOps<FPopcornFXFieldPath>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXFieldPath;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXFieldPath()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXFieldPath_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXFieldPath"), sizeof(FPopcornFXFieldPath), Get_Z_Construct_UScriptStruct_FPopcornFXFieldPath_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXFieldPath>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FieldName_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_FieldName = { UE4CodeGen_Private::EPropertyClass::Str, "FieldName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000000, 1, nullptr, STRUCT_OFFSET(FPopcornFXFieldPath, FieldName), METADATA_PARAMS(NewProp_FieldName_MetaData, ARRAY_COUNT(NewProp_FieldName_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BaseObjectName_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_BaseObjectName = { UE4CodeGen_Private::EPropertyClass::Str, "BaseObjectName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000000, 1, nullptr, STRUCT_OFFSET(FPopcornFXFieldPath, BaseObjectName), METADATA_PARAMS(NewProp_BaseObjectName_MetaData, ARRAY_COUNT(NewProp_BaseObjectName_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FieldName,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BaseObjectName,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXFieldPath",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXFieldPath),
				alignof(FPopcornFXFieldPath),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXFieldPath_CRC() { return 957486465U; }
	void UPopcornFXAssetDep::StaticRegisterNativesUPopcornFXAssetDep()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXAssetDep_NoRegister()
	{
		return UPopcornFXAssetDep::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAssetDep()
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
				{ "IncludePath", "Assets/PopcornFXAssetDep.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
				{ "ToolTip", "Describe an Asset needed by a PopcornFX Asset." },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_Patches_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_Patches = { UE4CodeGen_Private::EPropertyClass::Array, "m_Patches", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXAssetDep, m_Patches), METADATA_PARAMS(NewProp_m_Patches_MetaData, ARRAY_COUNT(NewProp_m_Patches_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_m_Patches_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "m_Patches", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000800000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FPopcornFXFieldPath, METADATA_PARAMS(nullptr, 0) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Asset_MetaData[] = {
				{ "Category", "PopcornFX AssetDependencies" },
				{ "DisplayThumbnail", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Asset = { UE4CodeGen_Private::EPropertyClass::Object, "Asset", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAssetDep, Asset), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(NewProp_Asset_MetaData, ARRAY_COUNT(NewProp_Asset_MetaData)) };
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Type_MetaData[] = {
				{ "Category", "PopcornFX AssetDependencies" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_Type = { UE4CodeGen_Private::EPropertyClass::Byte, "Type", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAssetDep, Type), Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDepType, METADATA_PARAMS(NewProp_Type_MetaData, ARRAY_COUNT(NewProp_Type_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ImportPath_MetaData[] = {
				{ "Category", "PopcornFX AssetDependencies" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_ImportPath = { UE4CodeGen_Private::EPropertyClass::Str, "ImportPath", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAssetDep, ImportPath), METADATA_PARAMS(NewProp_ImportPath_MetaData, ARRAY_COUNT(NewProp_ImportPath_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SourcePath_MetaData[] = {
				{ "Category", "PopcornFX AssetDependencies" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXAssetDep.h" },
				{ "ToolTip", "can be empty if source path unchanged  use GetSourcePath()" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_SourcePath = { UE4CodeGen_Private::EPropertyClass::Str, "SourcePath", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAssetDep, SourcePath), METADATA_PARAMS(NewProp_SourcePath_MetaData, ARRAY_COUNT(NewProp_SourcePath_MetaData)) };
#endif // WITH_EDITORONLY_DATA
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Patches,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Patches_Inner,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Asset,
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Type,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ImportPath,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SourcePath,
#endif // WITH_EDITORONLY_DATA
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAssetDep>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAssetDep::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00081080u,
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
	IMPLEMENT_CLASS(UPopcornFXAssetDep, 3060120319);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAssetDep(Z_Construct_UClass_UPopcornFXAssetDep, &UPopcornFXAssetDep::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAssetDep"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAssetDep);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
