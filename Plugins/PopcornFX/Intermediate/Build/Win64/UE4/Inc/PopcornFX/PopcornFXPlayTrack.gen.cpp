// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Sequencer/Tracks/PopcornFXPlayTrack.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXPlayTrack() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXPlayStateKey();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXPlayTrack_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXPlayTrack();
	MOVIESCENE_API UClass* Z_Construct_UClass_UMovieSceneNameableTrack();
	MOVIESCENE_API UClass* Z_Construct_UClass_UMovieSceneSection_NoRegister();
// End Cross Module References
	static UEnum* EPopcornFXPlayStateKey_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXPlayStateKey, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXPlayStateKey"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXPlayStateKey(EPopcornFXPlayStateKey_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXPlayStateKey"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXPlayStateKey_CRC() { return 1030447312U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXPlayStateKey()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXPlayStateKey"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXPlayStateKey_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXPlayStateKey::Start", (int64)EPopcornFXPlayStateKey::Start },
				{ "EPopcornFXPlayStateKey::Stop", (int64)EPopcornFXPlayStateKey::Stop },
				{ "EPopcornFXPlayStateKey::Toggle", (int64)EPopcornFXPlayStateKey::Toggle },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/Sequencer/Tracks/PopcornFXPlayTrack.h" },
				{ "ToolTip", "Defines the types of state keys." },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXPlayStateKey",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXPlayStateKey::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXPlayTrack::StaticRegisterNativesUPopcornFXPlayTrack()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXPlayTrack_NoRegister()
	{
		return UPopcornFXPlayTrack::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXPlayTrack()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UMovieSceneNameableTrack,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Sequencer/Tracks/PopcornFXPlayTrack.h" },
				{ "ModuleRelativePath", "Private/Sequencer/Tracks/PopcornFXPlayTrack.h" },
				{ "ToolTip", "Handles Start/Stop of emitters." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Sections_MetaData[] = {
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Private/Sequencer/Tracks/PopcornFXPlayTrack.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Sections = { UE4CodeGen_Private::EPropertyClass::Array, "Sections", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040008000000008, 1, nullptr, STRUCT_OFFSET(UPopcornFXPlayTrack, Sections), METADATA_PARAMS(NewProp_Sections_MetaData, ARRAY_COUNT(NewProp_Sections_MetaData)) };
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Sections_Inner = { UE4CodeGen_Private::EPropertyClass::Object, "Sections", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000080008, 1, nullptr, 0, Z_Construct_UClass_UMovieSceneSection_NoRegister, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Sections,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Sections_Inner,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXPlayTrack>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXPlayTrack::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00A80080u,
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
	IMPLEMENT_CLASS(UPopcornFXPlayTrack, 624512384);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXPlayTrack(Z_Construct_UClass_UPopcornFXPlayTrack, &UPopcornFXPlayTrack::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXPlayTrack"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXPlayTrack);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
