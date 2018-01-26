// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXFile.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXFile() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXFile_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXFile();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAssetDep_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UAssetImportData_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UAssetUserData_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UInterface_AssetUserData_NoRegister();
// End Cross Module References
	void UPopcornFXFile::StaticRegisterNativesUPopcornFXFile()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXFile_NoRegister()
	{
		return UPopcornFXFile::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXFile()
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
				{ "IncludePath", "Assets/PopcornFXFile.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
				{ "ToolTip", "Base of any PopcornFX Asset imported a from .pk* file." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetDependencies_MetaData[] = {
				{ "Category", "PopcornFX AssetDependencies" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_AssetDependencies = { UE4CodeGen_Private::EPropertyClass::Array, "AssetDependencies", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020088000020009, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, AssetDependencies), METADATA_PARAMS(NewProp_AssetDependencies_MetaData, ARRAY_COUNT(NewProp_AssetDependencies_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetDependencies_Inner_MetaData[] = {
				{ "Category", "PopcornFX AssetDependencies" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_AssetDependencies_Inner = { UE4CodeGen_Private::EPropertyClass::Object, "AssetDependencies", RF_Public|RF_Transient|RF_MarkAsNative, 0x00020000000a0008, 1, nullptr, 0, Z_Construct_UClass_UPopcornFXAssetDep_NoRegister, METADATA_PARAMS(NewProp_AssetDependencies_Inner_MetaData, ARRAY_COUNT(NewProp_AssetDependencies_Inner_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_LastCachedCacherVersion_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt64PropertyParams NewProp_m_LastCachedCacherVersion = { UE4CodeGen_Private::EPropertyClass::UInt64, "m_LastCachedCacherVersion", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, m_LastCachedCacherVersion), METADATA_PARAMS(NewProp_m_LastCachedCacherVersion_MetaData, ARRAY_COUNT(NewProp_m_LastCachedCacherVersion_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_FileVersionId_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_m_FileVersionId = { UE4CodeGen_Private::EPropertyClass::UInt32, "m_FileVersionId", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, m_FileVersionId), METADATA_PARAMS(NewProp_m_FileVersionId_MetaData, ARRAY_COUNT(NewProp_m_FileVersionId_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_FileData_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
				{ "ToolTip", "The file content" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_FileData = { UE4CodeGen_Private::EPropertyClass::Array, "m_FileData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, m_FileData), METADATA_PARAMS(NewProp_m_FileData_MetaData, ARRAY_COUNT(NewProp_m_FileData_MetaData)) };
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_m_FileData_Inner = { UE4CodeGen_Private::EPropertyClass::Byte, "m_FileData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_IsBaseObject_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			auto NewProp_m_IsBaseObject_SetBit = [](void* Obj){ ((UPopcornFXFile*)Obj)->m_IsBaseObject = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_m_IsBaseObject = { UE4CodeGen_Private::EPropertyClass::Bool, "m_IsBaseObject", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080000000000, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXFile), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_m_IsBaseObject_SetBit)>::SetBit, METADATA_PARAMS(NewProp_m_IsBaseObject_MetaData, ARRAY_COUNT(NewProp_m_IsBaseObject_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_FileExtension_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
				{ "ToolTip", "Original extension of the file" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_m_FileExtension = { UE4CodeGen_Private::EPropertyClass::Str, "m_FileExtension", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, m_FileExtension), METADATA_PARAMS(NewProp_m_FileExtension_MetaData, ARRAY_COUNT(NewProp_m_FileExtension_MetaData)) };
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetImportData_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_AssetImportData = { UE4CodeGen_Private::EPropertyClass::Object, "AssetImportData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, AssetImportData), Z_Construct_UClass_UAssetImportData_NoRegister, METADATA_PARAMS(NewProp_AssetImportData_MetaData, ARRAY_COUNT(NewProp_AssetImportData_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FileSourceVirtualPathIsNotVirtual_MetaData[] = {
				{ "Category", "Source" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_FileSourceVirtualPathIsNotVirtual = { UE4CodeGen_Private::EPropertyClass::UInt32, "FileSourceVirtualPathIsNotVirtual", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080800020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, FileSourceVirtualPathIsNotVirtual), METADATA_PARAMS(NewProp_FileSourceVirtualPathIsNotVirtual_MetaData, ARRAY_COUNT(NewProp_FileSourceVirtualPathIsNotVirtual_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FileSourceVirtualPath_MetaData[] = {
				{ "Category", "Source" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
				{ "ToolTip", "Source virtual path of the imported file" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_FileSourceVirtualPath = { UE4CodeGen_Private::EPropertyClass::Str, "FileSourceVirtualPath", RF_Public|RF_Transient|RF_MarkAsNative, 0x0020080800020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, FileSourceVirtualPath), METADATA_PARAMS(NewProp_FileSourceVirtualPath_MetaData, ARRAY_COUNT(NewProp_FileSourceVirtualPath_MetaData)) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AssetUserDatas_MetaData[] = {
				{ "Category", "UserDatas" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
				{ "ToolTip", "Array of user data stored with the asset" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_AssetUserDatas = { UE4CodeGen_Private::EPropertyClass::Array, "m_AssetUserDatas", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010048000000009, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, m_AssetUserDatas), METADATA_PARAMS(NewProp_m_AssetUserDatas_MetaData, ARRAY_COUNT(NewProp_m_AssetUserDatas_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AssetUserDatas_Inner_MetaData[] = {
				{ "Category", "UserDatas" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
				{ "ToolTip", "Array of user data stored with the asset" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_m_AssetUserDatas_Inner = { UE4CodeGen_Private::EPropertyClass::Object, "m_AssetUserDatas", RF_Public|RF_Transient|RF_MarkAsNative, 0x0002000000080008, 1, nullptr, 0, Z_Construct_UClass_UAssetUserData_NoRegister, METADATA_PARAMS(NewProp_m_AssetUserDatas_Inner_MetaData, ARRAY_COUNT(NewProp_m_AssetUserDatas_Inner_MetaData)) };
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ThumbnailImage_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXFile.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ThumbnailImage = { UE4CodeGen_Private::EPropertyClass::Object, "ThumbnailImage", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXFile, ThumbnailImage), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_ThumbnailImage_MetaData, ARRAY_COUNT(NewProp_ThumbnailImage_MetaData)) };
#endif // WITH_EDITORONLY_DATA
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AssetDependencies,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AssetDependencies_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_LastCachedCacherVersion,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_FileVersionId,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_FileData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_FileData_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_IsBaseObject,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_FileExtension,
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AssetImportData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FileSourceVirtualPathIsNotVirtual,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FileSourceVirtualPath,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AssetUserDatas,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AssetUserDatas_Inner,
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ThumbnailImage,
#endif // WITH_EDITORONLY_DATA
			};
			static const UE4CodeGen_Private::FImplementedInterfaceParams InterfaceParams[] = {
				{ Z_Construct_UClass_UInterface_AssetUserData_NoRegister, (int32)VTABLE_OFFSET(UPopcornFXFile, IInterface_AssetUserData), false },
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXFile>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXFile::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00900080u,
				nullptr, 0,
				PropPointers, ARRAY_COUNT(PropPointers),
				nullptr,
				&StaticCppClassTypeInfo,
				InterfaceParams, ARRAY_COUNT(InterfaceParams),
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPopcornFXFile, 2640826367);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXFile(Z_Construct_UClass_UPopcornFXFile, &UPopcornFXFile::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXFile"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXFile);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
