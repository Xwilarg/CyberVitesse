// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXMesh.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXMesh() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXMesh_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXMesh();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXFile();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	ENGINE_API UClass* Z_Construct_UClass_UAssetImportData_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USkeletalMesh_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
// End Cross Module References
	void UPopcornFXMesh::StaticRegisterNativesUPopcornFXMesh()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXMesh_NoRegister()
	{
		return UPopcornFXMesh::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXMesh()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXFile,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Assets/PopcornFXMesh.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
				{ "ToolTip", "Contains a Mesh's Sampling Acceleration Structures to be used by PopcornFX Particles.\nKind-of correspond to the .pkmm files." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEditorReimport_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
				{ "ToolTip", "Use StaticMesh's sections as submeshes, or merge all sections into one single mesh." },
			};
#endif
			auto NewProp_bEditorReimport_SetBit = [](void* Obj){ ((UPopcornFXMesh*)Obj)->bEditorReimport = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEditorReimport = { UE4CodeGen_Private::EPropertyClass::Bool, "bEditorReimport", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEditorReimport_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEditorReimport_MetaData, ARRAY_COUNT(NewProp_bEditorReimport_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bMergeStaticMeshSections_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
				{ "ToolTip", "Use StaticMesh's sections as submeshes, or merge all sections into one single mesh." },
			};
#endif
			auto NewProp_bMergeStaticMeshSections_SetBit = [](void* Obj){ ((UPopcornFXMesh*)Obj)->bMergeStaticMeshSections = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bMergeStaticMeshSections = { UE4CodeGen_Private::EPropertyClass::Bool, "bMergeStaticMeshSections", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bMergeStaticMeshSections_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bMergeStaticMeshSections_MetaData, ARRAY_COUNT(NewProp_bMergeStaticMeshSections_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMeshLOD_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
				{ "ToolTip", "Use StaticMesh's sections as submeshes, or merge all sections into one single mesh." },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_StaticMeshLOD = { UE4CodeGen_Private::EPropertyClass::Int, "StaticMeshLOD", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXMesh, StaticMeshLOD), METADATA_PARAMS(NewProp_StaticMeshLOD_MetaData, ARRAY_COUNT(NewProp_StaticMeshLOD_MetaData)) };
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMeshAssetImportData_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_StaticMeshAssetImportData = { UE4CodeGen_Private::EPropertyClass::Object, "StaticMeshAssetImportData", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120008000a0009, 1, nullptr, STRUCT_OFFSET(UPopcornFXMesh, StaticMeshAssetImportData), Z_Construct_UClass_UAssetImportData_NoRegister, METADATA_PARAMS(NewProp_StaticMeshAssetImportData_MetaData, ARRAY_COUNT(NewProp_StaticMeshAssetImportData_MetaData)) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SkeletalMesh_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
				{ "ToolTip", "The source skeletal mesh" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SkeletalMesh = { UE4CodeGen_Private::EPropertyClass::Object, "SkeletalMesh", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXMesh, SkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(NewProp_SkeletalMesh_MetaData, ARRAY_COUNT(NewProp_SkeletalMesh_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMesh_MetaData[] = {
				{ "Category", "PopcornFX Mesh" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXMesh.h" },
				{ "ToolTip", "The source static mesh" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_StaticMesh = { UE4CodeGen_Private::EPropertyClass::Object, "StaticMesh", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXMesh, StaticMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(NewProp_StaticMesh_MetaData, ARRAY_COUNT(NewProp_StaticMesh_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEditorReimport,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bMergeStaticMeshSections,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMeshLOD,
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMeshAssetImportData,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SkeletalMesh,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMesh,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXMesh>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXMesh::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXMesh, 4011462325);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXMesh(Z_Construct_UClass_UPopcornFXMesh, &UPopcornFXMesh::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXMesh"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXMesh);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
