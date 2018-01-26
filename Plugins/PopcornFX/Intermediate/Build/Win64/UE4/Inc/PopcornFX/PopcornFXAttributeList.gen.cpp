// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Attributes/PopcornFXAttributeList.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeList() {}
// Cross Module References
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSamplerDesc();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXAttributeDesc();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeList_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeList();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEffect_NoRegister();
// End Cross Module References
class UScriptStruct* FPopcornFXSamplerDesc::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSamplerDesc_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXSamplerDesc, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXSamplerDesc"), sizeof(FPopcornFXSamplerDesc), Get_Z_Construct_UScriptStruct_FPopcornFXSamplerDesc_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXSamplerDesc(FPopcornFXSamplerDesc::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXSamplerDesc"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSamplerDesc
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSamplerDesc()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXSamplerDesc")),new UScriptStruct::TCppStructOps<FPopcornFXSamplerDesc>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSamplerDesc;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSamplerDesc()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSamplerDesc_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXSamplerDesc"), sizeof(FPopcornFXSamplerDesc), Get_Z_Construct_UScriptStruct_FPopcornFXSamplerDesc_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXSamplerDesc>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AttributeSamplerComponentProperty_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_m_AttributeSamplerComponentProperty = { UE4CodeGen_Private::EPropertyClass::Name, "m_AttributeSamplerComponentProperty", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSamplerDesc, m_AttributeSamplerComponentProperty), METADATA_PARAMS(NewProp_m_AttributeSamplerComponentProperty_MetaData, ARRAY_COUNT(NewProp_m_AttributeSamplerComponentProperty_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AttributeSamplerActor_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_m_AttributeSamplerActor = { UE4CodeGen_Private::EPropertyClass::Object, "m_AttributeSamplerActor", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSamplerDesc, m_AttributeSamplerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(NewProp_m_AttributeSamplerActor_MetaData, ARRAY_COUNT(NewProp_m_AttributeSamplerActor_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_SamplerType_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_m_SamplerType = { UE4CodeGen_Private::EPropertyClass::Byte, "m_SamplerType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSamplerDesc, m_SamplerType), Z_Construct_UEnum_PopcornFX_EPopcornFXAttributeSamplerType, METADATA_PARAMS(NewProp_m_SamplerType_MetaData, ARRAY_COUNT(NewProp_m_SamplerType_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_SamplerFName_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_m_SamplerFName = { UE4CodeGen_Private::EPropertyClass::Name, "m_SamplerFName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000020001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSamplerDesc, m_SamplerFName), METADATA_PARAMS(NewProp_m_SamplerFName_MetaData, ARRAY_COUNT(NewProp_m_SamplerFName_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AttributeSamplerComponentProperty,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AttributeSamplerActor,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_SamplerType,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_SamplerFName,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXSamplerDesc",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXSamplerDesc),
				alignof(FPopcornFXSamplerDesc),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSamplerDesc_CRC() { return 4202009416U; }
class UScriptStruct* FPopcornFXAttributeDesc::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXAttributeDesc_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXAttributeDesc, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXAttributeDesc"), sizeof(FPopcornFXAttributeDesc), Get_Z_Construct_UScriptStruct_FPopcornFXAttributeDesc_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXAttributeDesc(FPopcornFXAttributeDesc::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXAttributeDesc"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXAttributeDesc
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXAttributeDesc()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXAttributeDesc")),new UScriptStruct::TCppStructOps<FPopcornFXAttributeDesc>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXAttributeDesc;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXAttributeDesc()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXAttributeDesc_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXAttributeDesc"), sizeof(FPopcornFXAttributeDesc), Get_Z_Construct_UScriptStruct_FPopcornFXAttributeDesc_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXAttributeDesc>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AttributeType_MetaData[] = {
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_m_AttributeType = { UE4CodeGen_Private::EPropertyClass::UInt32, "m_AttributeType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000000, 1, nullptr, STRUCT_OFFSET(FPopcornFXAttributeDesc, m_AttributeType), METADATA_PARAMS(NewProp_m_AttributeType_MetaData, ARRAY_COUNT(NewProp_m_AttributeType_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AttributeFName_MetaData[] = {
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_m_AttributeFName = { UE4CodeGen_Private::EPropertyClass::Name, "m_AttributeFName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000000, 1, nullptr, STRUCT_OFFSET(FPopcornFXAttributeDesc, m_AttributeFName), METADATA_PARAMS(NewProp_m_AttributeFName_MetaData, ARRAY_COUNT(NewProp_m_AttributeFName_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AttributeType,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AttributeFName,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXAttributeDesc",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXAttributeDesc),
				alignof(FPopcornFXAttributeDesc),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXAttributeDesc_CRC() { return 1548254476U; }
	void UPopcornFXAttributeList::StaticRegisterNativesUPopcornFXAttributeList()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeList_NoRegister()
	{
		return UPopcornFXAttributeList::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeList()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UObject,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Attributes/PopcornFXAttributeList.h" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_AttributesRawData_MetaData[] = {
				{ "Category", "PopcornFX Attributes" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_AttributesRawData = { UE4CodeGen_Private::EPropertyClass::Array, "m_AttributesRawData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000055, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeList, m_AttributesRawData), METADATA_PARAMS(NewProp_m_AttributesRawData_MetaData, ARRAY_COUNT(NewProp_m_AttributesRawData_MetaData)) };
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_m_AttributesRawData_Inner = { UE4CodeGen_Private::EPropertyClass::Byte, "m_AttributesRawData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_Samplers_MetaData[] = {
				{ "Category", "PopcornFX Attributes" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_Samplers = { UE4CodeGen_Private::EPropertyClass::Array, "m_Samplers", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000000041, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeList, m_Samplers), METADATA_PARAMS(NewProp_m_Samplers_MetaData, ARRAY_COUNT(NewProp_m_Samplers_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_m_Samplers_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "m_Samplers", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FPopcornFXSamplerDesc, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_Attributes_MetaData[] = {
				{ "Category", "PopcornFX Attributes" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_m_Attributes = { UE4CodeGen_Private::EPropertyClass::Array, "m_Attributes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeList, m_Attributes), METADATA_PARAMS(NewProp_m_Attributes_MetaData, ARRAY_COUNT(NewProp_m_Attributes_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_m_Attributes_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "m_Attributes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000020000, 1, nullptr, 0, Z_Construct_UScriptStruct_FPopcornFXAttributeDesc, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_FileVersionId_MetaData[] = {
				{ "Category", "PopcornFX Attributes" },
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_m_FileVersionId = { UE4CodeGen_Private::EPropertyClass::UInt32, "m_FileVersionId", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000020001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeList, m_FileVersionId), METADATA_PARAMS(NewProp_m_FileVersionId_MetaData, ARRAY_COUNT(NewProp_m_FileVersionId_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_Effect_MetaData[] = {
				{ "ModuleRelativePath", "Private/Attributes/PopcornFXAttributeList.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_m_Effect = { UE4CodeGen_Private::EPropertyClass::Object, "m_Effect", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeList, m_Effect), Z_Construct_UClass_UPopcornFXEffect_NoRegister, METADATA_PARAMS(NewProp_m_Effect_MetaData, ARRAY_COUNT(NewProp_m_Effect_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AttributesRawData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_AttributesRawData_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Samplers,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Samplers_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Attributes,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Attributes_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_FileVersionId,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_Effect,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeList>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeList::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00281080u,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeList, 2660128495);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeList(Z_Construct_UClass_UPopcornFXAttributeList, &UPopcornFXAttributeList::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeList"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeList);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
