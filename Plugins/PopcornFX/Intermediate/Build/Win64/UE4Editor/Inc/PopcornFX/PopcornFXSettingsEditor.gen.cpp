// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/PopcornFXSettingsEditor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXSettingsEditor() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDependenciesAutoImport();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FMyAutoReimportWildcard();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSettingsEditor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSettingsEditor();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
// End Cross Module References
	static UEnum* EPopcornFXAssetDependenciesAutoImport_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDependenciesAutoImport, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXAssetDependenciesAutoImport"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXAssetDependenciesAutoImport(EPopcornFXAssetDependenciesAutoImport_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXAssetDependenciesAutoImport"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDependenciesAutoImport_CRC() { return 1937920779U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDependenciesAutoImport()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXAssetDependenciesAutoImport"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDependenciesAutoImport_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXAssetDependenciesAutoImport::Never", (int64)EPopcornFXAssetDependenciesAutoImport::Never },
				{ "EPopcornFXAssetDependenciesAutoImport::Ask", (int64)EPopcornFXAssetDependenciesAutoImport::Ask },
				{ "EPopcornFXAssetDependenciesAutoImport::Always", (int64)EPopcornFXAssetDependenciesAutoImport::Always },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "Always.DisplayName", "Always Import" },
				{ "Always.ToolTip", "Always import missing dependencies, without asking." },
				{ "Ask.DisplayName", "Ask to Import" },
				{ "Ask.ToolTip", "Ask before importing missing dependencies." },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "Never.DisplayName", "Never Import" },
				{ "Never.ToolTip", "Never try to import missing dependencies." },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXAssetDependenciesAutoImport",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXAssetDependenciesAutoImport::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FMyAutoReimportWildcard::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FMyAutoReimportWildcard_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FMyAutoReimportWildcard, Z_Construct_UPackage__Script_PopcornFX(), TEXT("MyAutoReimportWildcard"), sizeof(FMyAutoReimportWildcard), Get_Z_Construct_UScriptStruct_FMyAutoReimportWildcard_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FMyAutoReimportWildcard(FMyAutoReimportWildcard::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("MyAutoReimportWildcard"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFMyAutoReimportWildcard
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFMyAutoReimportWildcard()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("MyAutoReimportWildcard")),new UScriptStruct::TCppStructOps<FMyAutoReimportWildcard>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFMyAutoReimportWildcard;
	UScriptStruct* Z_Construct_UScriptStruct_FMyAutoReimportWildcard()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FMyAutoReimportWildcard_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("MyAutoReimportWildcard"), sizeof(FMyAutoReimportWildcard), Get_Z_Construct_UScriptStruct_FMyAutoReimportWildcard_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Define our own FAutoReimportWildcard class because this class definition is contained in the UnrealEd module,\nnot available in non-editor builds (Build fails with Clang on ORBIS)\nUltimately, we should place the UPopcornFXSettingsEditor class in the PopcornFXEditor module instead of the runtime one." },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMyAutoReimportWildcard>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bInclude_MetaData[] = {
				{ "Category", "AutoReimport" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "When true, files that match this wildcard will be included (if it doesn't fail any other filters), when false, matches will be excluded from the reimporter" },
			};
#endif
			auto NewProp_bInclude_SetBit = [](void* Obj){ ((FMyAutoReimportWildcard*)Obj)->bInclude = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bInclude = { UE4CodeGen_Private::EPropertyClass::Bool, "bInclude", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(FMyAutoReimportWildcard), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bInclude_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bInclude_MetaData, ARRAY_COUNT(NewProp_bInclude_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Wildcard_MetaData[] = {
				{ "Category", "AutoReimport" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "The wildcard filter as a string. Files that match this wildcard will be included/excluded according to the bInclude member" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_Wildcard = { UE4CodeGen_Private::EPropertyClass::Str, "Wildcard", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(FMyAutoReimportWildcard, Wildcard), METADATA_PARAMS(NewProp_Wildcard_MetaData, ARRAY_COUNT(NewProp_Wildcard_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bInclude,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Wildcard,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"MyAutoReimportWildcard",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FMyAutoReimportWildcard),
				alignof(FMyAutoReimportWildcard),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FMyAutoReimportWildcard_CRC() { return 3676657910U; }
	void UPopcornFXSettingsEditor::StaticRegisterNativesUPopcornFXSettingsEditor()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXSettingsEditor_NoRegister()
	{
		return UPopcornFXSettingsEditor::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXSettingsEditor()
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
				{ "IncludePath", "PopcornFXSettingsEditor.h" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Implements the settings for the PopcornFX Plugin." },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bAlwaysRenderAttributeSamplerShapes_MetaData[] = {
				{ "Category", "Debug" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Check this to always render the attribute sampler shapes debug, even when they are not selected" },
			};
#endif
			auto NewProp_bAlwaysRenderAttributeSamplerShapes_SetBit = [](void* Obj){ ((UPopcornFXSettingsEditor*)Obj)->bAlwaysRenderAttributeSamplerShapes = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAlwaysRenderAttributeSamplerShapes = { UE4CodeGen_Private::EPropertyClass::Bool, "bAlwaysRenderAttributeSamplerShapes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800004001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSettingsEditor), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bAlwaysRenderAttributeSamplerShapes_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bAlwaysRenderAttributeSamplerShapes_MetaData, ARRAY_COUNT(NewProp_bAlwaysRenderAttributeSamplerShapes_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AssetDependenciesAutoImport_MetaData[] = {
				{ "Category", "Import" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "How to import PopcornFX Asset Dependencies, if any." },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_AssetDependenciesAutoImport = { UE4CodeGen_Private::EPropertyClass::Byte, "AssetDependenciesAutoImport", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettingsEditor, AssetDependenciesAutoImport), Z_Construct_UEnum_PopcornFX_EPopcornFXAssetDependenciesAutoImport, METADATA_PARAMS(NewProp_AssetDependenciesAutoImport_MetaData, ARRAY_COUNT(NewProp_AssetDependenciesAutoImport_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AutoReimportMirrorPackWildcards_MetaData[] = {
				{ "Category", "Import" },
				{ "EditCondition", "bAutoReimportMirrorPack" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Wildcards for Auto Reimport Mirror Pack.\nWill override PackMountPoint Wildcards in \"Editor Preferences > Loading & Saving > Directories To Monitor\"." },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_AutoReimportMirrorPackWildcards = { UE4CodeGen_Private::EPropertyClass::Array, "AutoReimportMirrorPackWildcards", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettingsEditor, AutoReimportMirrorPackWildcards), METADATA_PARAMS(NewProp_AutoReimportMirrorPackWildcards_MetaData, ARRAY_COUNT(NewProp_AutoReimportMirrorPackWildcards_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_AutoReimportMirrorPackWildcards_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "AutoReimportMirrorPackWildcards", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000800004000, 1, nullptr, 0, Z_Construct_UScriptStruct_FMyAutoReimportWildcard, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bAddSourcePackToMonitoredDirectories_MetaData[] = {
				{ "Category", "Import" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Enables auto reimport/mirroring of Source PopcornFX Project assets.\n\nThis will register the Source PopcornFX Project to \"Editor Preferences > Loading & Saving > Directories To Monitor\".\n\nWill mirror all the files included in the source pack that matches wildcards options. (See bellow)\n\nUncheck \"Editor Preferences > Loading & Saving > Auto Create Assets\" if you only want to reimport already added files." },
			};
#endif
			auto NewProp_bAddSourcePackToMonitoredDirectories_SetBit = [](void* Obj){ ((UPopcornFXSettingsEditor*)Obj)->bAddSourcePackToMonitoredDirectories = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAddSourcePackToMonitoredDirectories = { UE4CodeGen_Private::EPropertyClass::Bool, "bAddSourcePackToMonitoredDirectories", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800004001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSettingsEditor), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bAddSourcePackToMonitoredDirectories_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bAddSourcePackToMonitoredDirectories_MetaData, ARRAY_COUNT(NewProp_bAddSourcePackToMonitoredDirectories_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SourcePackThumbnailsDir_MetaData[] = {
				{ "Category", "SourcePack" },
				{ "DisplayName", "Resolved Thumbnails directory" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Plugin internal: resolved thumbnails dir" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_SourcePackThumbnailsDir = { UE4CodeGen_Private::EPropertyClass::Str, "SourcePackThumbnailsDir", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800024001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettingsEditor, SourcePackThumbnailsDir), METADATA_PARAMS(NewProp_SourcePackThumbnailsDir_MetaData, ARRAY_COUNT(NewProp_SourcePackThumbnailsDir_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SourcePackRootDir_MetaData[] = {
				{ "Category", "SourcePack" },
				{ "DisplayName", "Resolved Root directory" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Plugin internal: resolved root dir" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_SourcePackRootDir = { UE4CodeGen_Private::EPropertyClass::Str, "SourcePackRootDir", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800024001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettingsEditor, SourcePackRootDir), METADATA_PARAMS(NewProp_SourcePackRootDir_MetaData, ARRAY_COUNT(NewProp_SourcePackRootDir_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SourcePackProjectFile_MetaData[] = {
				{ "Category", "SourcePack" },
				{ "DisplayName", "Resolved Project File path" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Plugin internal: resolved project file" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_SourcePackProjectFile = { UE4CodeGen_Private::EPropertyClass::Str, "SourcePackProjectFile", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800024001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettingsEditor, SourcePackProjectFile), METADATA_PARAMS(NewProp_SourcePackProjectFile_MetaData, ARRAY_COUNT(NewProp_SourcePackProjectFile_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bSourcePackFound_MetaData[] = {
				{ "Category", "SourcePack" },
				{ "DisplayName", "Is Valid" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "Plugin internal: true if the Source PopcornFX Project is valid" },
			};
#endif
			auto NewProp_bSourcePackFound_SetBit = [](void* Obj){ ((UPopcornFXSettingsEditor*)Obj)->bSourcePackFound = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSourcePackFound = { UE4CodeGen_Private::EPropertyClass::Bool, "bSourcePackFound", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800026001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSettingsEditor), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bSourcePackFound_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bSourcePackFound_MetaData, ARRAY_COUNT(NewProp_bSourcePackFound_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ImportSourcePack_MetaData[] = {
				{ "Category", "SourcePack" },
				{ "DisplayName", "Source PopcornFX Project path" },
				{ "ModuleRelativePath", "Private/PopcornFXSettingsEditor.h" },
				{ "ToolTip", "The path of the PopcornFX Editor Project to import assets from.\nEither the full project path or the project's directory.\nCan be an absolute path, or relative to the Game Directory" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_ImportSourcePack = { UE4CodeGen_Private::EPropertyClass::Str, "ImportSourcePack", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettingsEditor, ImportSourcePack), METADATA_PARAMS(NewProp_ImportSourcePack_MetaData, ARRAY_COUNT(NewProp_ImportSourcePack_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bAlwaysRenderAttributeSamplerShapes,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AssetDependenciesAutoImport,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AutoReimportMirrorPackWildcards,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AutoReimportMirrorPackWildcards_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bAddSourcePackToMonitoredDirectories,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SourcePackThumbnailsDir,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SourcePackRootDir,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SourcePackProjectFile,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bSourcePackFound,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ImportSourcePack,
			};
#endif // WITH_EDITORONLY_DATA
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXSettingsEditor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXSettingsEditor::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00080084u,
				nullptr, 0,
				IF_WITH_EDITORONLY_DATA(PropPointers, nullptr), IF_WITH_EDITORONLY_DATA(ARRAY_COUNT(PropPointers), 0),
				"Editor",
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPopcornFXSettingsEditor, 1016673498);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXSettingsEditor(Z_Construct_UClass_UPopcornFXSettingsEditor, &UPopcornFXSettingsEditor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXSettingsEditor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXSettingsEditor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
