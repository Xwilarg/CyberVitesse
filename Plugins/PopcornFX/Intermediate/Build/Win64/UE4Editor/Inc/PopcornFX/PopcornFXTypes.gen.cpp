// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXTypes.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXTypes() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
// End Cross Module References
	static UEnum* EPopcornFXParticleFieldType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXParticleFieldType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXParticleFieldType(EPopcornFXParticleFieldType_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXParticleFieldType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType_CRC() { return 1961680952U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXParticleFieldType"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXParticleFieldType::Float", (int64)EPopcornFXParticleFieldType::Float },
				{ "EPopcornFXParticleFieldType::Float2", (int64)EPopcornFXParticleFieldType::Float2 },
				{ "EPopcornFXParticleFieldType::Float3", (int64)EPopcornFXParticleFieldType::Float3 },
				{ "EPopcornFXParticleFieldType::Float4", (int64)EPopcornFXParticleFieldType::Float4 },
				{ "EPopcornFXParticleFieldType::Int", (int64)EPopcornFXParticleFieldType::Int },
				{ "EPopcornFXParticleFieldType::Int2", (int64)EPopcornFXParticleFieldType::Int2 },
				{ "EPopcornFXParticleFieldType::Int3", (int64)EPopcornFXParticleFieldType::Int3 },
				{ "EPopcornFXParticleFieldType::Int4", (int64)EPopcornFXParticleFieldType::Int4 },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXTypes.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXParticleFieldType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXParticleFieldType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
