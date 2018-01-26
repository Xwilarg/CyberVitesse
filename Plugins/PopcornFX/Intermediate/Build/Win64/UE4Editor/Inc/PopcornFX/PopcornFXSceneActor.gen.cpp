// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXSceneActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXSceneActor() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXSceneActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXSceneActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSceneComponent_NoRegister();
// End Cross Module References
	void APopcornFXSceneActor::StaticRegisterNativesAPopcornFXSceneActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXSceneActor_NoRegister()
	{
		return APopcornFXSceneActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXSceneActor()
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
				{ "IncludePath", "PopcornFXSceneActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneActor.h" },
				{ "ToolTip", "Actor container for a PopcornFXSceneComponent." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PopcornFXSceneComponent_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneActor.h" },
				{ "ToolTip", "The PopcornFX Scene Component" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_PopcornFXSceneComponent = { UE4CodeGen_Private::EPropertyClass::Object, "PopcornFXSceneComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120000000a001d, 1, nullptr, STRUCT_OFFSET(APopcornFXSceneActor, PopcornFXSceneComponent), Z_Construct_UClass_UPopcornFXSceneComponent_NoRegister, METADATA_PARAMS(NewProp_PopcornFXSceneComponent_MetaData, ARRAY_COUNT(NewProp_PopcornFXSceneComponent_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_PopcornFXSceneComponent,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXSceneActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXSceneActor::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00900080u,
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
	IMPLEMENT_CLASS(APopcornFXSceneActor, 185919989);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXSceneActor(Z_Construct_UClass_APopcornFXSceneActor, &APopcornFXSceneActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXSceneActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXSceneActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
