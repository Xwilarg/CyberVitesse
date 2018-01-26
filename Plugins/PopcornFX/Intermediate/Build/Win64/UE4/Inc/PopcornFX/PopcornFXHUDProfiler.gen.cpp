// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/HUD/PopcornFXHUDProfiler.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXHUDProfiler() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXHUDProfiler_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXHUDProfiler();
	ENGINE_API UClass* Z_Construct_UClass_AHUD();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	void APopcornFXHUDProfiler::StaticRegisterNativesAPopcornFXHUDProfiler()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXHUDProfiler_NoRegister()
	{
		return APopcornFXHUDProfiler::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXHUDProfiler()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_AHUD,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Rendering Actor Input Replication" },
				{ "IncludePath", "HUD/PopcornFXHUDProfiler.h" },
				{ "ModuleRelativePath", "Private/HUD/PopcornFXHUDProfiler.h" },
				{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXHUDProfiler>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXHUDProfiler::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x0088028Cu,
				nullptr, 0,
				nullptr, 0,
				"Game",
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(APopcornFXHUDProfiler, 1153943020);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXHUDProfiler(Z_Construct_UClass_APopcornFXHUDProfiler, &APopcornFXHUDProfiler::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXHUDProfiler"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXHUDProfiler);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
