// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXTextureAtlas.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXTextureAtlas() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXTextureAtlas_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXTextureAtlas();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXFile();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	void UPopcornFXTextureAtlas::StaticRegisterNativesUPopcornFXTextureAtlas()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXTextureAtlas_NoRegister()
	{
		return UPopcornFXTextureAtlas::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXTextureAtlas()
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
				{ "IncludePath", "Assets/PopcornFXTextureAtlas.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXTextureAtlas.h" },
				{ "ToolTip", "PopcornFX Texture Atlas asset imported from a .pkat file." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXTextureAtlas>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXTextureAtlas::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXTextureAtlas, 1456445943);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXTextureAtlas(Z_Construct_UClass_UPopcornFXTextureAtlas, &UPopcornFXTextureAtlas::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXTextureAtlas"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXTextureAtlas);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
