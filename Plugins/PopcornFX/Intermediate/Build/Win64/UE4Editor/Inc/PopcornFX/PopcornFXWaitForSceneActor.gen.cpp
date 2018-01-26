// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/World/PopcornFXWaitForSceneActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXWaitForSceneActor() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXWaitForScene_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXWaitForScene();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	void APopcornFXWaitForScene::StaticRegisterNativesAPopcornFXWaitForScene()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXWaitForScene_NoRegister()
	{
		return APopcornFXWaitForScene::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXWaitForScene()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_AActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Input Collision Replication Materials" },
				{ "IncludePath", "World/PopcornFXWaitForSceneActor.h" },
				{ "ModuleRelativePath", "Private/World/PopcornFXWaitForSceneActor.h" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXWaitForScene>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXWaitForScene::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00900080u,
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
	IMPLEMENT_CLASS(APopcornFXWaitForScene, 2778785513);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXWaitForScene(Z_Construct_UClass_APopcornFXWaitForScene, &APopcornFXWaitForScene::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXWaitForScene"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXWaitForScene);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
