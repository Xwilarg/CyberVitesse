// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXStaticMeshData.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXStaticMeshData() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXStaticMeshData_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXStaticMeshData();
	ENGINE_API UClass* Z_Construct_UClass_UAssetUserData();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXMesh_NoRegister();
// End Cross Module References
	void UPopcornFXStaticMeshData::StaticRegisterNativesUPopcornFXStaticMeshData()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXStaticMeshData_NoRegister()
	{
		return UPopcornFXStaticMeshData::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXStaticMeshData()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UAssetUserData,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Assets/PopcornFXStaticMeshData.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXStaticMeshData.h" },
				{ "ToolTip", "UserData proxy to the PopcornFXMesh Acceleration Structures." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PopcornFXMesh_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXStaticMeshData.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_PopcornFXMesh = { UE4CodeGen_Private::EPropertyClass::Object, "PopcornFXMesh", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120000000a0009, 1, nullptr, STRUCT_OFFSET(UPopcornFXStaticMeshData, PopcornFXMesh), Z_Construct_UClass_UPopcornFXMesh_NoRegister, METADATA_PARAMS(NewProp_PopcornFXMesh_MetaData, ARRAY_COUNT(NewProp_PopcornFXMesh_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_PopcornFXMesh,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXStaticMeshData>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXStaticMeshData::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00A81080u,
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
	IMPLEMENT_CLASS(UPopcornFXStaticMeshData, 3240079689);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXStaticMeshData(Z_Construct_UClass_UPopcornFXStaticMeshData, &UPopcornFXStaticMeshData::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXStaticMeshData"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXStaticMeshData);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
