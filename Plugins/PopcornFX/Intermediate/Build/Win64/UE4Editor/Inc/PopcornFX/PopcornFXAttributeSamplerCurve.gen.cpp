// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerCurve.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerCurve() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurve_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurve();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurve_SetCurve();
	ENGINE_API UClass* Z_Construct_UClass_UCurveBase_NoRegister();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurve_SetCurveDimension();
	ENGINE_API UClass* Z_Construct_UClass_UCurveLinearColor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UCurveVector_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
// End Cross Module References
	static UEnum* EAttributeSamplerCurveDimension_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EAttributeSamplerCurveDimension"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EAttributeSamplerCurveDimension(EAttributeSamplerCurveDimension_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EAttributeSamplerCurveDimension"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension_CRC() { return 3601926292U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EAttributeSamplerCurveDimension"), 0, Get_Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EAttributeSamplerCurveDimension::Float1", (int64)EAttributeSamplerCurveDimension::Float1 },
				{ "EAttributeSamplerCurveDimension::Float2", (int64)EAttributeSamplerCurveDimension::Float2 },
				{ "EAttributeSamplerCurveDimension::Float3", (int64)EAttributeSamplerCurveDimension::Float3 },
				{ "EAttributeSamplerCurveDimension::Float4", (int64)EAttributeSamplerCurveDimension::Float4 },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EAttributeSamplerCurveDimension",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EAttributeSamplerCurveDimension::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXAttributeSamplerCurve::StaticRegisterNativesUPopcornFXAttributeSamplerCurve()
	{
		UClass* Class = UPopcornFXAttributeSamplerCurve::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SetCurve", (Native)&UPopcornFXAttributeSamplerCurve::execSetCurve },
			{ "SetCurveDimension", (Native)&UPopcornFXAttributeSamplerCurve::execSetCurveDimension },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurve_SetCurve()
	{
		struct PopcornFXAttributeSamplerCurve_eventSetCurve_Parms
		{
			UCurveBase* InCurve;
			bool InIsSecondCurve;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurve_eventSetCurve_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurve_eventSetCurve_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			auto NewProp_InIsSecondCurve_SetBit = [](void* Obj){ ((PopcornFXAttributeSamplerCurve_eventSetCurve_Parms*)Obj)->InIsSecondCurve = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_InIsSecondCurve = { UE4CodeGen_Private::EPropertyClass::Bool, "InIsSecondCurve", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXAttributeSamplerCurve_eventSetCurve_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_InIsSecondCurve_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InCurve = { UE4CodeGen_Private::EPropertyClass::Object, "InCurve", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurve_eventSetCurve_Parms, InCurve), Z_Construct_UClass_UCurveBase_NoRegister, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InIsSecondCurve,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InCurve,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Set the UCurve to be sampled.\nMust match the current dimension.\n@return true if curve is up." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurve, "SetCurve", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerCurve_eventSetCurve_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerCurve_SetCurveDimension()
	{
		struct PopcornFXAttributeSamplerCurve_eventSetCurveDimension_Parms
		{
			TEnumAsByte<EAttributeSamplerCurveDimension::Type> InCurveDimension;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_InCurveDimension = { UE4CodeGen_Private::EPropertyClass::Byte, "InCurveDimension", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerCurve_eventSetCurveDimension_Parms, InCurveDimension), Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InCurveDimension,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Changes the Curve Dimension, will clear the current Curve if dimension changes" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerCurve, "SetCurveDimension", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerCurve_eventSetCurveDimension_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurve_NoRegister()
	{
		return UPopcornFXAttributeSamplerCurve::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerCurve()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXAttributeSampler,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurve_SetCurve, "SetCurve" }, // 2506490847
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerCurve_SetCurveDimension, "SetCurveDimension" }, // 94765264
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSamplerCurve.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Curve** by a **UCurve...**." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SecondCurve4D_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Second 4 Dimensions UCurve when IsDoubleCurve" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SecondCurve4D = { UE4CodeGen_Private::EPropertyClass::Object, "SecondCurve4D", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, SecondCurve4D), Z_Construct_UClass_UCurveLinearColor_NoRegister, METADATA_PARAMS(NewProp_SecondCurve4D_MetaData, ARRAY_COUNT(NewProp_SecondCurve4D_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Curve4D_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "The 4 Dimensions UCurve to be sampled" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Curve4D = { UE4CodeGen_Private::EPropertyClass::Object, "Curve4D", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, Curve4D), Z_Construct_UClass_UCurveLinearColor_NoRegister, METADATA_PARAMS(NewProp_Curve4D_MetaData, ARRAY_COUNT(NewProp_Curve4D_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SecondCurve3D_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Second 3 Dimensions UCurve when IsDoubleCurve" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SecondCurve3D = { UE4CodeGen_Private::EPropertyClass::Object, "SecondCurve3D", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, SecondCurve3D), Z_Construct_UClass_UCurveVector_NoRegister, METADATA_PARAMS(NewProp_SecondCurve3D_MetaData, ARRAY_COUNT(NewProp_SecondCurve3D_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Curve3D_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "The 3 Dimensions UCurve to be sampled" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Curve3D = { UE4CodeGen_Private::EPropertyClass::Object, "Curve3D", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, Curve3D), Z_Construct_UClass_UCurveVector_NoRegister, METADATA_PARAMS(NewProp_Curve3D_MetaData, ARRAY_COUNT(NewProp_Curve3D_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SecondCurve1D_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Second 1 Dimension UCurve when IsDoubleCurve" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SecondCurve1D = { UE4CodeGen_Private::EPropertyClass::Object, "SecondCurve1D", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, SecondCurve1D), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(NewProp_SecondCurve1D_MetaData, ARRAY_COUNT(NewProp_SecondCurve1D_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Curve1D_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "The 1 Dimension UCurve to be sampled" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Curve1D = { UE4CodeGen_Private::EPropertyClass::Object, "Curve1D", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, Curve1D), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(NewProp_Curve1D_MetaData, ARRAY_COUNT(NewProp_Curve1D_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsDoubleCurve_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Enables DoubleCurve sampling" },
			};
#endif
			auto NewProp_bIsDoubleCurve_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerCurve*)Obj)->bIsDoubleCurve = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsDoubleCurve = { UE4CodeGen_Private::EPropertyClass::Bool, "bIsDoubleCurve", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerCurve), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bIsDoubleCurve_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bIsDoubleCurve_MetaData, ARRAY_COUNT(NewProp_bIsDoubleCurve_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurveDimension_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerCurve.h" },
				{ "ToolTip", "Curve dimension" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_CurveDimension = { UE4CodeGen_Private::EPropertyClass::Byte, "CurveDimension", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerCurve, CurveDimension), Z_Construct_UEnum_PopcornFX_EAttributeSamplerCurveDimension, METADATA_PARAMS(NewProp_CurveDimension_MetaData, ARRAY_COUNT(NewProp_CurveDimension_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SecondCurve4D,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Curve4D,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SecondCurve3D,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Curve3D,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SecondCurve1D,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Curve1D,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bIsDoubleCurve,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CurveDimension,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSamplerCurve>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSamplerCurve::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSamplerCurve, 3085669641);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSamplerCurve(Z_Construct_UClass_UPopcornFXAttributeSamplerCurve, &UPopcornFXAttributeSamplerCurve::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSamplerCurve"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSamplerCurve);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
