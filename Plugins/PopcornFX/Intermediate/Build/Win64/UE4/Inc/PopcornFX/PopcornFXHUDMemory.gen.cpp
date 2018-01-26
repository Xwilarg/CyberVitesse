// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/HUD/PopcornFXHUDMemory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXHUDMemory() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXHUDMemory_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXHUDMemory();
	ENGINE_API UClass* Z_Construct_UClass_AHUD();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	void APopcornFXHUDMemory::StaticRegisterNativesAPopcornFXHUDMemory()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXHUDMemory_NoRegister()
	{
		return APopcornFXHUDMemory::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXHUDMemory()
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
				{ "IncludePath", "HUD/PopcornFXHUDMemory.h" },
				{ "ModuleRelativePath", "Private/HUD/PopcornFXHUDMemory.h" },
				{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXHUDMemory>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXHUDMemory::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXHUDMemory, 3983992524);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXHUDMemory(Z_Construct_UClass_APopcornFXHUDMemory, &APopcornFXHUDMemory::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXHUDMemory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXHUDMemory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
