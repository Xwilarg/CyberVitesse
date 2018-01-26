// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerActor() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	ENGINE_API UClass* Z_Construct_UClass_UBillboardComponent_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerShapeActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerShapeActor();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerSkinnedMeshActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerSkinnedMeshActor();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerImageActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerImageActor();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerCurveActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerCurveActor();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerTextActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerTextActor();
// End Cross Module References
	void APopcornFXAttributeSamplerActor::StaticRegisterNativesAPopcornFXAttributeSamplerActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerActor_NoRegister()
	{
		return APopcornFXAttributeSamplerActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerActor()
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
				{ "IncludePath", "PopcornFXAttributeSamplerActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
				{ "ToolTip", "Abstract Actor containing a PopcornFXAttributeSampler Component" },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SpriteComponent_MetaData[] = {
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SpriteComponent = { UE4CodeGen_Private::EPropertyClass::Object, "SpriteComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800080008, 1, nullptr, STRUCT_OFFSET(APopcornFXAttributeSamplerActor, SpriteComponent), Z_Construct_UClass_UBillboardComponent_NoRegister, METADATA_PARAMS(NewProp_SpriteComponent_MetaData, ARRAY_COUNT(NewProp_SpriteComponent_MetaData)) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Sampler_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Sampler = { UE4CodeGen_Private::EPropertyClass::Object, "Sampler", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120000000a001d, 1, nullptr, STRUCT_OFFSET(APopcornFXAttributeSamplerActor, Sampler), Z_Construct_UClass_UPopcornFXAttributeSampler_NoRegister, METADATA_PARAMS(NewProp_Sampler_MetaData, ARRAY_COUNT(NewProp_Sampler_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SpriteComponent,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Sampler,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAttributeSamplerActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAttributeSamplerActor::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00900081u,
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
	IMPLEMENT_CLASS(APopcornFXAttributeSamplerActor, 1506352671);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAttributeSamplerActor(Z_Construct_UClass_APopcornFXAttributeSamplerActor, &APopcornFXAttributeSamplerActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAttributeSamplerActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAttributeSamplerActor);
	void APopcornFXAttributeSamplerShapeActor::StaticRegisterNativesAPopcornFXAttributeSamplerShapeActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerShapeActor_NoRegister()
	{
		return APopcornFXAttributeSamplerShapeActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerShapeActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_APopcornFXAttributeSamplerActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "PopcornFXAttributeSamplerActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Shape** by a **UStaticMesh**." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAttributeSamplerShapeActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAttributeSamplerShapeActor::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXAttributeSamplerShapeActor, 345974778);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAttributeSamplerShapeActor(Z_Construct_UClass_APopcornFXAttributeSamplerShapeActor, &APopcornFXAttributeSamplerShapeActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAttributeSamplerShapeActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAttributeSamplerShapeActor);
	void APopcornFXAttributeSamplerSkinnedMeshActor::StaticRegisterNativesAPopcornFXAttributeSamplerSkinnedMeshActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerSkinnedMeshActor_NoRegister()
	{
		return APopcornFXAttributeSamplerSkinnedMeshActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerSkinnedMeshActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_APopcornFXAttributeSamplerActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "PopcornFXAttributeSamplerActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Shape** in a **USkeletalMesh**." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAttributeSamplerSkinnedMeshActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAttributeSamplerSkinnedMeshActor::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXAttributeSamplerSkinnedMeshActor, 546344770);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAttributeSamplerSkinnedMeshActor(Z_Construct_UClass_APopcornFXAttributeSamplerSkinnedMeshActor, &APopcornFXAttributeSamplerSkinnedMeshActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAttributeSamplerSkinnedMeshActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAttributeSamplerSkinnedMeshActor);
	void APopcornFXAttributeSamplerImageActor::StaticRegisterNativesAPopcornFXAttributeSamplerImageActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerImageActor_NoRegister()
	{
		return APopcornFXAttributeSamplerImageActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerImageActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_APopcornFXAttributeSamplerActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "PopcornFXAttributeSamplerActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Image** by a **UTexture**." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAttributeSamplerImageActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAttributeSamplerImageActor::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXAttributeSamplerImageActor, 4231484758);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAttributeSamplerImageActor(Z_Construct_UClass_APopcornFXAttributeSamplerImageActor, &APopcornFXAttributeSamplerImageActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAttributeSamplerImageActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAttributeSamplerImageActor);
	void APopcornFXAttributeSamplerCurveActor::StaticRegisterNativesAPopcornFXAttributeSamplerCurveActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerCurveActor_NoRegister()
	{
		return APopcornFXAttributeSamplerCurveActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerCurveActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_APopcornFXAttributeSamplerActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "PopcornFXAttributeSamplerActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Curve** by a **UCurve...**." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAttributeSamplerCurveActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAttributeSamplerCurveActor::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXAttributeSamplerCurveActor, 2357546183);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAttributeSamplerCurveActor(Z_Construct_UClass_APopcornFXAttributeSamplerCurveActor, &APopcornFXAttributeSamplerCurveActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAttributeSamplerCurveActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAttributeSamplerCurveActor);
	void APopcornFXAttributeSamplerTextActor::StaticRegisterNativesAPopcornFXAttributeSamplerTextActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerTextActor_NoRegister()
	{
		return APopcornFXAttributeSamplerTextActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAttributeSamplerTextActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_APopcornFXAttributeSamplerActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "PopcornFXAttributeSamplerActor.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerActor.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Text** by a **FString**." },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAttributeSamplerTextActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAttributeSamplerTextActor::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXAttributeSamplerTextActor, 604080828);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAttributeSamplerTextActor(Z_Construct_UClass_APopcornFXAttributeSamplerTextActor, &APopcornFXAttributeSamplerTextActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAttributeSamplerTextActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAttributeSamplerTextActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
