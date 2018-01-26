// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSampler.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSampler() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerComponentType();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
// End Cross Module References
	static UEnum* EPopcornFXAttribSamplerShapeType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXAttribSamplerShapeType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXAttribSamplerShapeType(EPopcornFXAttribSamplerShapeType_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXAttribSamplerShapeType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType_CRC() { return 1436591702U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXAttribSamplerShapeType"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXAttribSamplerShapeType::Box", (int64)EPopcornFXAttribSamplerShapeType::Box },
				{ "EPopcornFXAttribSamplerShapeType::Sphere", (int64)EPopcornFXAttribSamplerShapeType::Sphere },
				{ "EPopcornFXAttribSamplerShapeType::ComplexEllipsoid", (int64)EPopcornFXAttribSamplerShapeType::ComplexEllipsoid },
				{ "EPopcornFXAttribSamplerShapeType::Cylinder", (int64)EPopcornFXAttribSamplerShapeType::Cylinder },
				{ "EPopcornFXAttribSamplerShapeType::Capsule", (int64)EPopcornFXAttribSamplerShapeType::Capsule },
				{ "EPopcornFXAttribSamplerShapeType::Cone", (int64)EPopcornFXAttribSamplerShapeType::Cone },
				{ "EPopcornFXAttribSamplerShapeType::Plane", (int64)EPopcornFXAttribSamplerShapeType::Plane },
				{ "EPopcornFXAttribSamplerShapeType::Mesh", (int64)EPopcornFXAttribSamplerShapeType::Mesh },
				{ "EPopcornFXAttribSamplerShapeType::Collection", (int64)EPopcornFXAttribSamplerShapeType::Collection },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSampler.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXAttribSamplerShapeType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXAttribSamplerShapeType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXAttributeSamplerType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXAttributeSamplerType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXAttributeSamplerType(EPopcornFXAttributeSamplerType_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXAttributeSamplerType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType_CRC() { return 2342960580U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXAttributeSamplerType"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXAttributeSamplerType::None", (int64)EPopcornFXAttributeSamplerType::None },
				{ "EPopcornFXAttributeSamplerType::Shape", (int64)EPopcornFXAttributeSamplerType::Shape },
				{ "EPopcornFXAttributeSamplerType::Image", (int64)EPopcornFXAttributeSamplerType::Image },
				{ "EPopcornFXAttributeSamplerType::Curve", (int64)EPopcornFXAttributeSamplerType::Curve },
				{ "EPopcornFXAttributeSamplerType::Text", (int64)EPopcornFXAttributeSamplerType::Text },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSampler.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXAttributeSamplerType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXAttributeSamplerType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXAttributeSamplerComponentType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerComponentType, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXAttributeSamplerComponentType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXAttributeSamplerComponentType(EPopcornFXAttributeSamplerComponentType_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXAttributeSamplerComponentType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerComponentType_CRC() { return 3830365028U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerComponentType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXAttributeSamplerComponentType"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerComponentType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXAttributeSamplerComponentType::Shape", (int64)EPopcornFXAttributeSamplerComponentType::Shape },
				{ "EPopcornFXAttributeSamplerComponentType::SkinnedMesh", (int64)EPopcornFXAttributeSamplerComponentType::SkinnedMesh },
				{ "EPopcornFXAttributeSamplerComponentType::Image", (int64)EPopcornFXAttributeSamplerComponentType::Image },
				{ "EPopcornFXAttributeSamplerComponentType::Curve", (int64)EPopcornFXAttributeSamplerComponentType::Curve },
				{ "EPopcornFXAttributeSamplerComponentType::Text", (int64)EPopcornFXAttributeSamplerComponentType::Text },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSampler.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXAttributeSamplerComponentType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXAttributeSamplerComponentType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXAttributeSampler::StaticRegisterNativesUPopcornFXAttributeSampler()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSampler_NoRegister()
	{
		return UPopcornFXAttributeSampler::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSampler()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_USceneComponent,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSampler.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSampler.h" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSampler>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSampler::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00B01081u,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSampler, 22779095);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSampler(Z_Construct_UClass_UPopcornFXAttributeSampler, &UPopcornFXAttributeSampler::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSampler"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSampler);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
