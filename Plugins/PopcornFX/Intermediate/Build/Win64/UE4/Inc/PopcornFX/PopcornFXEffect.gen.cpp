// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Assets/PopcornFXEffect.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXEffect() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEffect_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEffect();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXFile();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXRendererMaterial_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeList_NoRegister();
// End Cross Module References
	void UPopcornFXEffect::StaticRegisterNativesUPopcornFXEffect()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXEffect_NoRegister()
	{
		return UPopcornFXEffect::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXEffect()
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
				{ "BlueprintType", "true" },
				{ "IncludePath", "Assets/PopcornFXEffect.h" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
				{ "ToolTip", "PopcornFX Effect Asset imported from a .pkfx file." },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EditorLoopDelay_MetaData[] = {
				{ "ClampMax", "20.0" },
				{ "ClampMin", "0.0" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_EditorLoopDelay = { UE4CodeGen_Private::EPropertyClass::Float, "EditorLoopDelay", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXEffect, EditorLoopDelay), METADATA_PARAMS(NewProp_EditorLoopDelay_MetaData, ARRAY_COUNT(NewProp_EditorLoopDelay_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EditorLoopEmitter_MetaData[] = {
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
			};
#endif
			auto NewProp_EditorLoopEmitter_SetBit = [](void* Obj){ ((UPopcornFXEffect*)Obj)->EditorLoopEmitter = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_EditorLoopEmitter = { UE4CodeGen_Private::EPropertyClass::Bool, "EditorLoopEmitter", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800000000, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXEffect), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_EditorLoopEmitter_SetBit)>::SetBit, METADATA_PARAMS(NewProp_EditorLoopEmitter_MetaData, ARRAY_COUNT(NewProp_EditorLoopEmitter_MetaData)) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DecalsRendererMaterials_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterials" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_DecalsRendererMaterials = { UE4CodeGen_Private::EPropertyClass::Array, "DecalsRendererMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010008000020009, 1, nullptr, STRUCT_OFFSET(UPopcornFXEffect, DecalsRendererMaterials), METADATA_PARAMS(NewProp_DecalsRendererMaterials_MetaData, ARRAY_COUNT(NewProp_DecalsRendererMaterials_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DecalsRendererMaterials_Inner_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterials" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_DecalsRendererMaterials_Inner = { UE4CodeGen_Private::EPropertyClass::Object, "DecalsRendererMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x00020000000a0008, 1, nullptr, 0, Z_Construct_UClass_UPopcornFXRendererMaterial_NoRegister, METADATA_PARAMS(NewProp_DecalsRendererMaterials_Inner_MetaData, ARRAY_COUNT(NewProp_DecalsRendererMaterials_Inner_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParticleRendererMaterials_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterials" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ParticleRendererMaterials = { UE4CodeGen_Private::EPropertyClass::Array, "ParticleRendererMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010008000020009, 1, nullptr, STRUCT_OFFSET(UPopcornFXEffect, ParticleRendererMaterials), METADATA_PARAMS(NewProp_ParticleRendererMaterials_MetaData, ARRAY_COUNT(NewProp_ParticleRendererMaterials_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParticleRendererMaterials_Inner_MetaData[] = {
				{ "Category", "PopcornFX RendererMaterials" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ParticleRendererMaterials_Inner = { UE4CodeGen_Private::EPropertyClass::Object, "ParticleRendererMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x00020000000a0008, 1, nullptr, 0, Z_Construct_UClass_UPopcornFXRendererMaterial_NoRegister, METADATA_PARAMS(NewProp_ParticleRendererMaterials_Inner_MetaData, ARRAY_COUNT(NewProp_ParticleRendererMaterials_Inner_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DefaultAttributeList_MetaData[] = {
				{ "Category", "PopcornFX Attributes" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Assets/PopcornFXEffect.h" },
				{ "ToolTip", "if this is private, \"in an extrnal package\" error will happen !\nBUT USE GetDefaultAttributeList() !" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_DefaultAttributeList = { UE4CodeGen_Private::EPropertyClass::Object, "DefaultAttributeList", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120000000a0009, 1, nullptr, STRUCT_OFFSET(UPopcornFXEffect, DefaultAttributeList), Z_Construct_UClass_UPopcornFXAttributeList_NoRegister, METADATA_PARAMS(NewProp_DefaultAttributeList_MetaData, ARRAY_COUNT(NewProp_DefaultAttributeList_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EditorLoopDelay,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EditorLoopEmitter,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DecalsRendererMaterials,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DecalsRendererMaterials_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ParticleRendererMaterials,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ParticleRendererMaterials_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DefaultAttributeList,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXEffect>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXEffect::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00880080u,
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
	IMPLEMENT_CLASS(UPopcornFXEffect, 180558649);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXEffect(Z_Construct_UClass_UPopcornFXEffect, &UPopcornFXEffect::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXEffect"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXEffect);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
