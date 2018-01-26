// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerCurveDynamic.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerCurveDynamic() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_ECurveDynamicInterpolator();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents1D();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents3D();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents4D();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTimes();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues1D();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues3D();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues4D();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension();
// End Cross Module References
	static UEnum* ECurveDynamicInterpolator_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_ECurveDynamicInterpolator, Z_Construct_UPackage__Script_PopcornFX(), TEXT("ECurveDynamicInterpolator"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ECurveDynamicInterpolator(ECurveDynamicInterpolator_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("ECurveDynamicInterpolator"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_ECurveDynamicInterpolator_CRC() { return 615702228U; }
	UEnum* Z_Construct_UEnum_PopcornFX_ECurveDynamicInterpolator()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ECurveDynamicInterpolator"), 0, Get_Z_Construct_UEnum_PopcornFX_ECurveDynamicInterpolator_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ECurveDynamicInterpolator::Linear", (int64)ECurveDynamicInterpolator::Linear },
				{ "ECurveDynamicInterpolator::Spline", (int64)ECurveDynamicInterpolator::Spline },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"ECurveDynamicInterpolator",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"ECurveDynamicInterpolator::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXAttributeSamplerCurveDynamic::StaticRegisterNativesUPopcornFXAttributeSamplerCurveDynamic()
	{
		UClass* Class = UPopcornFXAttributeSamplerCurveDynamic::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SetTangents1D", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetTangents1D },
			{ "SetTangents3D", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetTangents3D },
			{ "SetTangents4D", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetTangents4D },
			{ "SetTimes", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetTimes },
			{ "SetValues1D", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetValues1D },
			{ "SetValues3D", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetValues3D },
			{ "SetValues4D", (Native)&UPopcornFXAttributeSamplerCurveDynamic::execSetValues4D },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents1D()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetTangents1D_Parms
		{
			TArray<float> ArriveTangents;
			TArray<float> LeaveTangents;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetTangents1D_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents1D_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LeaveTangents_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_LeaveTangents = { UE4CodeGen_Private::EPropertyClass::Array, "LeaveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents1D_Parms, LeaveTangents), METADATA_PARAMS(NewProp_LeaveTangents_MetaData, ARRAY_COUNT(NewProp_LeaveTangents_MetaData)) };
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LeaveTangents_Inner = { UE4CodeGen_Private::EPropertyClass::Float, "LeaveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ArriveTangents_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ArriveTangents = { UE4CodeGen_Private::EPropertyClass::Array, "ArriveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents1D_Parms, ArriveTangents), METADATA_PARAMS(NewProp_ArriveTangents_MetaData, ARRAY_COUNT(NewProp_ArriveTangents_MetaData)) };
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ArriveTangents_Inner = { UE4CodeGen_Private::EPropertyClass::Float, "ArriveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LeaveTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LeaveTangents_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ArriveTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ArriveTangents_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the tangents from a float array." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetTangents1D", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents1D_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents3D()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetTangents3D_Parms
		{
			TArray<FVector> ArriveTangents;
			TArray<FVector> LeaveTangents;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetTangents3D_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents3D_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LeaveTangents_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_LeaveTangents = { UE4CodeGen_Private::EPropertyClass::Array, "LeaveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents3D_Parms, LeaveTangents), METADATA_PARAMS(NewProp_LeaveTangents_MetaData, ARRAY_COUNT(NewProp_LeaveTangents_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_LeaveTangents_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "LeaveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ArriveTangents_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ArriveTangents = { UE4CodeGen_Private::EPropertyClass::Array, "ArriveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents3D_Parms, ArriveTangents), METADATA_PARAMS(NewProp_ArriveTangents_MetaData, ARRAY_COUNT(NewProp_ArriveTangents_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_ArriveTangents_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "ArriveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LeaveTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LeaveTangents_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ArriveTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ArriveTangents_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the tangents from a vector array." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetTangents3D", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents3D_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents4D()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetTangents4D_Parms
		{
			TArray<FLinearColor> ArriveTangents;
			TArray<FLinearColor> LeaveTangents;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetTangents4D_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents4D_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LeaveTangents_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_LeaveTangents = { UE4CodeGen_Private::EPropertyClass::Array, "LeaveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents4D_Parms, LeaveTangents), METADATA_PARAMS(NewProp_LeaveTangents_MetaData, ARRAY_COUNT(NewProp_LeaveTangents_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_LeaveTangents_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "LeaveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ArriveTangents_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ArriveTangents = { UE4CodeGen_Private::EPropertyClass::Array, "ArriveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents4D_Parms, ArriveTangents), METADATA_PARAMS(NewProp_ArriveTangents_MetaData, ARRAY_COUNT(NewProp_ArriveTangents_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_ArriveTangents_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "ArriveTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LeaveTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LeaveTangents_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ArriveTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ArriveTangents_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the tangents from a color array." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetTangents4D", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTangents4D_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTimes()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetTimes_Parms
		{
			TArray<float> Times;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetTimes_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTimes_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Times_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Times = { UE4CodeGen_Private::EPropertyClass::Array, "Times", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetTimes_Parms, Times), METADATA_PARAMS(NewProp_Times_MetaData, ARRAY_COUNT(NewProp_Times_MetaData)) };
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Times_Inner = { UE4CodeGen_Private::EPropertyClass::Float, "Times", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Times,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Times_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the times from a float array. Times must be in the 0-1 range.\nIf this isn't called, times will be automatically generated from the number of values in the range 0-1 (fixed steps)." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetTimes", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetTimes_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues1D()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetValues1D_Parms
		{
			TArray<float> Values;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetValues1D_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetValues1D_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Values_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Values = { UE4CodeGen_Private::EPropertyClass::Array, "Values", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetValues1D_Parms, Values), METADATA_PARAMS(NewProp_Values_MetaData, ARRAY_COUNT(NewProp_Values_MetaData)) };
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Values_Inner = { UE4CodeGen_Private::EPropertyClass::Float, "Values", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Values,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Values_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the values from a float array." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetValues1D", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetValues1D_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues3D()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetValues3D_Parms
		{
			TArray<FVector> Values;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetValues3D_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetValues3D_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Values_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Values = { UE4CodeGen_Private::EPropertyClass::Array, "Values", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetValues3D_Parms, Values), METADATA_PARAMS(NewProp_Values_MetaData, ARRAY_COUNT(NewProp_Values_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Values_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "Values", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Values,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Values_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the values from a vector array." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetValues3D", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetValues3D_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues4D()
	{
		struct PopcornFXAttributeSamplerCurveDynamic_eventSetValues4D_Parms
		{
			TArray<FLinearColor> Values;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurveDynamic_eventSetValues4D_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetValues4D_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Values_MetaData[] = {
				{ "NativeConst", "" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Values = { UE4CodeGen_Private::EPropertyClass::Array, "Values", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000008000182, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurveDynamic_eventSetValues4D_Parms, Values), METADATA_PARAMS(NewProp_Values_MetaData, ARRAY_COUNT(NewProp_Values_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Values_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "Values", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Values,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Values_Inner,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Sets the values from a color array." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, "SetValues4D", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04420401, sizeof(PopcornFXAttributeSamplerCurveDynamic_eventSetValues4D_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic_NoRegister()
	{
		return UPopcornFXAttributeSamplerCurveDynamic::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXAttributeSampler,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents1D, "SetTangents1D" }, // 1524369625
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents3D, "SetTangents3D" }, // 1919280617
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTangents4D, "SetTangents4D" }, // 2541567274
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetTimes, "SetTimes" }, // 3061263192
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues1D, "SetValues1D" }, // 136520844
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues3D, "SetValues3D" }, // 3011617432
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurveDynamic_SetValues4D, "SetValues4D" }, // 2961470663
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Curve** by a **TArray of Values**." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurveInterpolator_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Determines the curve interpolator type" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_CurveInterpolator = { UE4CodeGen_Private::EPropertyClass::Byte, "CurveInterpolator", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurveDynamic, CurveInterpolator), Z_Construct_UEnum_PopcornFX_ECurveDynamicInterpolator, METADATA_PARAMS(NewProp_CurveInterpolator_MetaData, ARRAY_COUNT(NewProp_CurveInterpolator_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurveDimension_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurveDynamic.h" },
				{ "ToolTip", "Curve dimension" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_CurveDimension = { UE4CodeGen_Private::EPropertyClass::Byte, "CurveDimension", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurveDynamic, CurveDimension), Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension, METADATA_PARAMS(NewProp_CurveDimension_MetaData, ARRAY_COUNT(NewProp_CurveDimension_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CurveInterpolator,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CurveDimension,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSamplerCurveDynamic>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSamplerCurveDynamic::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00B01080u,
				FuncInfo, ARRAY_COUNT(FuncInfo),
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSamplerCurveDynamic, 481794763);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSamplerCurveDynamic(Z_Construct_UClass_UPopcornFXAttributeSamplerCurveDynamic, &UPopcornFXAttributeSamplerCurveDynamic::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSamplerCurveDynamic"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSamplerCurveDynamic);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
