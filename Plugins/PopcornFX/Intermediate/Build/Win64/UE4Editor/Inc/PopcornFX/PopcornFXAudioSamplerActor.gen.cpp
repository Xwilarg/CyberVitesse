// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/World/PopcornFXAudioSamplerActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAudioSamplerActor() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAudioSamplerActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAudioSamplerActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	void APopcornFXAudioSamplerActor::StaticRegisterNativesAPopcornFXAudioSamplerActor()
	{
	}
	UClass* Z_Construct_UClass_APopcornFXAudioSamplerActor_NoRegister()
	{
		return APopcornFXAudioSamplerActor::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXAudioSamplerActor()
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
				{ "BlueprintType", "true" },
				{ "IncludePath", "World/PopcornFXAudioSamplerActor.h" },
				{ "IsBlueprintBase", "true" },
				{ "ModuleRelativePath", "Private/World/PopcornFXAudioSamplerActor.h" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_WaveformData_MetaData[] = {
				{ "Category", "PopcornFX" },
				{ "ModuleRelativePath", "Private/World/PopcornFXAudioSamplerActor.h" },
				{ "ToolTip", "Waveform data to refresh each frame" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_WaveformData = { UE4CodeGen_Private::EPropertyClass::Array, "m_WaveformData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000002004, 1, nullptr, STRUCT_OFFSET(APopcornFXAudioSamplerActor, m_WaveformData), METADATA_PARAMS(NewProp_m_WaveformData_MetaData, ARRAY_COUNT(NewProp_m_WaveformData_MetaData)) };
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_m_WaveformData_Inner = { UE4CodeGen_Private::EPropertyClass::Float, "m_WaveformData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_SpectrumData_MetaData[] = {
				{ "Category", "PopcornFX" },
				{ "ModuleRelativePath", "Private/World/PopcornFXAudioSamplerActor.h" },
				{ "ToolTip", "Spectrum data to refresh each frame" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_SpectrumData = { UE4CodeGen_Private::EPropertyClass::Array, "m_SpectrumData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000002004, 1, nullptr, STRUCT_OFFSET(APopcornFXAudioSamplerActor, m_SpectrumData), METADATA_PARAMS(NewProp_m_SpectrumData_MetaData, ARRAY_COUNT(NewProp_m_SpectrumData_MetaData)) };
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_m_SpectrumData_Inner = { UE4CodeGen_Private::EPropertyClass::Float, "m_SpectrumData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneName_MetaData[] = {
				{ "Category", "PopcornFX" },
				{ "ModuleRelativePath", "Private/World/PopcornFXAudioSamplerActor.h" },
				{ "ToolTip", "PopcornFXScene name" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_SceneName = { UE4CodeGen_Private::EPropertyClass::Name, "SceneName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(APopcornFXAudioSamplerActor, SceneName), METADATA_PARAMS(NewProp_SceneName_MetaData, ARRAY_COUNT(NewProp_SceneName_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_WaveformData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_WaveformData_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_SpectrumData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_SpectrumData_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SceneName,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXAudioSamplerActor>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXAudioSamplerActor::StaticClass,
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
	IMPLEMENT_CLASS(APopcornFXAudioSamplerActor, 3552700154);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXAudioSamplerActor(Z_Construct_UClass_APopcornFXAudioSamplerActor, &APopcornFXAudioSamplerActor::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXAudioSamplerActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXAudioSamplerActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
