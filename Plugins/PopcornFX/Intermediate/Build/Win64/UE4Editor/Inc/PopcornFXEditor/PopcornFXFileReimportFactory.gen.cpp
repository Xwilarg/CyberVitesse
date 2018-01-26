// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/PopcornFXFileReimportFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXFileReimportFactory() {}
// Cross Module References
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXFileReimportFactory_NoRegister();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXFileReimportFactory();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXFileFactory();
	UPackage* Z_Construct_UPackage__Script_PopcornFXEditor();
// End Cross Module References
	void UPopcornFXFileReimportFactory::StaticRegisterNativesUPopcornFXFileReimportFactory()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXFileReimportFactory_NoRegister()
	{
		return UPopcornFXFileReimportFactory::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXFileReimportFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXFileFactory,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFXEditor,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Object" },
				{ "IncludePath", "PopcornFXFileReimportFactory.h" },
				{ "ModuleRelativePath", "Private/PopcornFXFileReimportFactory.h" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXFileReimportFactory>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXFileReimportFactory::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00080080u,
				nullptr, 0,
				nullptr, 0,
				nullptr,
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPopcornFXFileReimportFactory, 897465834);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXFileReimportFactory(Z_Construct_UClass_UPopcornFXFileReimportFactory, &UPopcornFXFileReimportFactory::StaticClass, TEXT("/Script/PopcornFXEditor"), TEXT("UPopcornFXFileReimportFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXFileReimportFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
