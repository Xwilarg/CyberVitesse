// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Nodes/PopcornFXNode_DynamicField.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXNode_DynamicField() {}
// Cross Module References
	POPCORNFXEDITOR_API UEnum* Z_Construct_UEnum_PopcornFXEditor_EPopcornFXPinFieldType();
	UPackage* Z_Construct_UPackage__Script_PopcornFXEditor();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_DynamicField_NoRegister();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_DynamicField();
	BLUEPRINTGRAPH_API UClass* Z_Construct_UClass_UK2Node();
// End Cross Module References
	static UEnum* EPopcornFXPinFieldType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFXEditor_EPopcornFXPinFieldType, Z_Construct_UPackage__Script_PopcornFXEditor(), TEXT("EPopcornFXPinFieldType"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXPinFieldType(EPopcornFXPinFieldType_StaticEnum, TEXT("/Script/PopcornFXEditor"), TEXT("EPopcornFXPinFieldType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFXEditor_EPopcornFXPinFieldType_CRC() { return 3990536013U; }
	UEnum* Z_Construct_UEnum_PopcornFXEditor_EPopcornFXPinFieldType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFXEditor();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXPinFieldType"), 0, Get_Z_Construct_UEnum_PopcornFXEditor_EPopcornFXPinFieldType_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXPinFieldType::Float", (int64)EPopcornFXPinFieldType::Float },
				{ "EPopcornFXPinFieldType::Float2", (int64)EPopcornFXPinFieldType::Float2 },
				{ "EPopcornFXPinFieldType::Float3", (int64)EPopcornFXPinFieldType::Float3 },
				{ "EPopcornFXPinFieldType::Float4", (int64)EPopcornFXPinFieldType::Float4 },
				{ "EPopcornFXPinFieldType::Int", (int64)EPopcornFXPinFieldType::Int },
				{ "EPopcornFXPinFieldType::Int2", (int64)EPopcornFXPinFieldType::Int2 },
				{ "EPopcornFXPinFieldType::Int3", (int64)EPopcornFXPinFieldType::Int3 },
				{ "EPopcornFXPinFieldType::Int4", (int64)EPopcornFXPinFieldType::Int4 },
				{ "EPopcornFXPinFieldType::Vector2D", (int64)EPopcornFXPinFieldType::Vector2D },
				{ "EPopcornFXPinFieldType::Vector", (int64)EPopcornFXPinFieldType::Vector },
				{ "EPopcornFXPinFieldType::LinearColor", (int64)EPopcornFXPinFieldType::LinearColor },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "LinearColor.DisplayName", "LinearColor (Float4)" },
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_DynamicField.h" },
				{ "Vector.DisplayName", "Vector (Float3)" },
				{ "Vector2D.DisplayName", "Vector2D (Float2)" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFXEditor,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXPinFieldType",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXPinFieldType::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXNode_DynamicField::StaticRegisterNativesUPopcornFXNode_DynamicField()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXNode_DynamicField_NoRegister()
	{
		return UPopcornFXNode_DynamicField::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXNode_DynamicField()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UK2Node,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFXEditor,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Nodes/PopcornFXNode_DynamicField.h" },
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_DynamicField.h" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_ParticleFieldType_MetaData[] = {
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_DynamicField.h" },
			};
#endif
			static const UE4CodeGen_Private::FUInt32PropertyParams NewProp_m_ParticleFieldType = { UE4CodeGen_Private::EPropertyClass::UInt32, "m_ParticleFieldType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000020000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXNode_DynamicField, m_ParticleFieldType_DEPRECATED), METADATA_PARAMS(NewProp_m_ParticleFieldType_MetaData, ARRAY_COUNT(NewProp_m_ParticleFieldType_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_BreakValue_MetaData[] = {
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_DynamicField.h" },
			};
#endif
			auto NewProp_m_BreakValue_SetBit = [](void* Obj){ ((UPopcornFXNode_DynamicField*)Obj)->m_BreakValue_DEPRECATED = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_m_BreakValue = { UE4CodeGen_Private::EPropertyClass::Bool, "m_BreakValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000020000000, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXNode_DynamicField), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_m_BreakValue_SetBit)>::SetBit, METADATA_PARAMS(NewProp_m_BreakValue_MetaData, ARRAY_COUNT(NewProp_m_BreakValue_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_m_PinType_MetaData[] = {
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_DynamicField.h" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_m_PinType = { UE4CodeGen_Private::EPropertyClass::Byte, "m_PinType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000000000, 1, nullptr, STRUCT_OFFSET(UPopcornFXNode_DynamicField, m_PinType), Z_Construct_UEnum_PopcornFXEditor_EPopcornFXPinFieldType, METADATA_PARAMS(NewProp_m_PinType_MetaData, ARRAY_COUNT(NewProp_m_PinType_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_ParticleFieldType,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_BreakValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_m_PinType,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXNode_DynamicField>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXNode_DynamicField::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00080081u,
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
	IMPLEMENT_CLASS(UPopcornFXNode_DynamicField, 401414519);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXNode_DynamicField(Z_Construct_UClass_UPopcornFXNode_DynamicField, &UPopcornFXNode_DynamicField::StaticClass, TEXT("/Script/PopcornFXEditor"), TEXT("UPopcornFXNode_DynamicField"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXNode_DynamicField);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
