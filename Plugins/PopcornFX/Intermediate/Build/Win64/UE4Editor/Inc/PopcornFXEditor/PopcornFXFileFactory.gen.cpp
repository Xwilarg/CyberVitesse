// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/PopcornFXFileFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXFileFactory() {}
// Cross Module References
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXFileFactory_NoRegister();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXFileFactory();
	UNREALED_API UClass* Z_Construct_UClass_UFactory();
	UPackage* Z_Construct_UPackage__Script_PopcornFXEditor();
// End Cross Module References
	void UPopcornFXFileFactory::StaticRegisterNativesUPopcornFXFileFactory()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXFileFactory_NoRegister()
	{
		return UPopcornFXFileFactory::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXFileFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UFactory,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFXEditor,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Object" },
				{ "IncludePath", "PopcornFXFileFactory.h" },
				{ "ModuleRelativePath", "Private/PopcornFXFileFactory.h" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXFileFactory>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXFileFactory::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXFileFactory, 1372790536);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXFileFactory(Z_Construct_UClass_UPopcornFXFileFactory, &UPopcornFXFileFactory::StaticClass, TEXT("/Script/PopcornFXEditor"), TEXT("UPopcornFXFileFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXFileFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
