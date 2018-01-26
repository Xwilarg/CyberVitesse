// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXSimulationCache.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXSimulationCache() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSimulationCache_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSimulationCache();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXFile();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	void UPopcornFXSimulationCache::StaticRegisterNativesUPopcornFXSimulationCache()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXSimulationCache_NoRegister()
	{
		return UPopcornFXSimulationCache::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXSimulationCache()
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
				{ "IncludePath", "Assets/PopcornFXSimulationCache.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXSimulationCache.h" },
				{ "ToolTip", "PopcornFX Simulation Cache asset imported from a .pksc file." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXSimulationCache>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXSimulationCache::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00880080u,
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
	IMPLEMENT_CLASS(UPopcornFXSimulationCache, 1319149909);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXSimulationCache(Z_Construct_UClass_UPopcornFXSimulationCache, &UPopcornFXSimulationCache::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXSimulationCache"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXSimulationCache);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
