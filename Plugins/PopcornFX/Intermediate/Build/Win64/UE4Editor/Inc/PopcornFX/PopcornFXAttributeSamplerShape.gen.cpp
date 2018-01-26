// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerShape.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerShape() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXShapeCollectionSamplingHeuristic();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerShape_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerShape();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetBoxDimension();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetHeight();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetInnerRadius();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetRadius();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetWeight();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXAttributeSamplerActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType();
// End Cross Module References
	static UEnum* EPopcornFXShapeCollectionSamplingHeuristic_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXShapeCollectionSamplingHeuristic, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXShapeCollectionSamplingHeuristic"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXShapeCollectionSamplingHeuristic(EPopcornFXShapeCollectionSamplingHeuristic_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXShapeCollectionSamplingHeuristic"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXShapeCollectionSamplingHeuristic_CRC() { return 2981509492U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXShapeCollectionSamplingHeuristic()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXShapeCollectionSamplingHeuristic"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXShapeCollectionSamplingHeuristic_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXShapeCollectionSamplingHeuristic::NoWeight", (int64)EPopcornFXShapeCollectionSamplingHeuristic::NoWeight },
				{ "EPopcornFXShapeCollectionSamplingHeuristic::WeightWithVolume", (int64)EPopcornFXShapeCollectionSamplingHeuristic::WeightWithVolume },
				{ "EPopcornFXShapeCollectionSamplingHeuristic::WeightWithSurface", (int64)EPopcornFXShapeCollectionSamplingHeuristic::WeightWithSurface },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "hh_geometrics/include/ge_shapes.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXShapeCollectionSamplingHeuristic",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXShapeCollectionSamplingHeuristic::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXAttributeSamplerShape::StaticRegisterNativesUPopcornFXAttributeSamplerShape()
	{
		UClass* Class = UPopcornFXAttributeSamplerShape::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SetBoxDimension", (Native)&UPopcornFXAttributeSamplerShape::execSetBoxDimension },
			{ "SetHeight", (Native)&UPopcornFXAttributeSamplerShape::execSetHeight },
			{ "SetInnerRadius", (Native)&UPopcornFXAttributeSamplerShape::execSetInnerRadius },
			{ "SetRadius", (Native)&UPopcornFXAttributeSamplerShape::execSetRadius },
			{ "SetWeight", (Native)&UPopcornFXAttributeSamplerShape::execSetWeight },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetBoxDimension()
	{
		struct PopcornFXAttributeSamplerShape_eventSetBoxDimension_Parms
		{
			FVector boxDimensions;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_boxDimensions = { UE4CodeGen_Private::EPropertyClass::Struct, "boxDimensions", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerShape_eventSetBoxDimension_Parms, boxDimensions), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_boxDimensions,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerShape, "SetBoxDimension", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04820401, sizeof(PopcornFXAttributeSamplerShape_eventSetBoxDimension_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetHeight()
	{
		struct PopcornFXAttributeSamplerShape_eventSetHeight_Parms
		{
			float height;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_height = { UE4CodeGen_Private::EPropertyClass::Float, "height", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerShape_eventSetHeight_Parms, height), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_height,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerShape, "SetHeight", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerShape_eventSetHeight_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetInnerRadius()
	{
		struct PopcornFXAttributeSamplerShape_eventSetInnerRadius_Parms
		{
			float innerRadius;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_innerRadius = { UE4CodeGen_Private::EPropertyClass::Float, "innerRadius", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerShape_eventSetInnerRadius_Parms, innerRadius), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_innerRadius,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerShape, "SetInnerRadius", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerShape_eventSetInnerRadius_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetRadius()
	{
		struct PopcornFXAttributeSamplerShape_eventSetRadius_Parms
		{
			float radius;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_radius = { UE4CodeGen_Private::EPropertyClass::Float, "radius", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerShape_eventSetRadius_Parms, radius), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_radius,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerShape, "SetRadius", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerShape_eventSetRadius_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetWeight()
	{
		struct PopcornFXAttributeSamplerShape_eventSetWeight_Parms
		{
			float height;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_height = { UE4CodeGen_Private::EPropertyClass::Float, "height", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerShape_eventSetWeight_Parms, height), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_height,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerShape, "SetWeight", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerShape_eventSetWeight_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerShape_NoRegister()
	{
		return UPopcornFXAttributeSamplerShape::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerShape()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXAttributeSampler,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetBoxDimension, "SetBoxDimension" }, // 4022035495
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetHeight, "SetHeight" }, // 2022052682
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetInnerRadius, "SetInnerRadius" }, // 85978308
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetRadius, "SetRadius" }, // 2929655815
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerShape_SetWeight, "SetWeight" }, // 2982431306
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSamplerShape.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Shape** by a **UStaticMesh**." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bUseRelativeTransform_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "Relative Transforms will be used if activated.\nEnable if sampled in SpawnerScript's Eval(), so shape will be sampled locally to the Emitter.\nDisable if sampled in SpawnerScript's **Post**Eval(), so the shape will be sampled world space." },
			};
#endif
			auto NewProp_bUseRelativeTransform_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerShape*)Obj)->bUseRelativeTransform = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUseRelativeTransform = { UE4CodeGen_Private::EPropertyClass::Bool, "bUseRelativeTransform", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerShape), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bUseRelativeTransform_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bUseRelativeTransform_MetaData, ARRAY_COUNT(NewProp_bUseRelativeTransform_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Shapes_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "Collection sub-Shapes" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Shapes = { UE4CodeGen_Private::EPropertyClass::Array, "Shapes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, Shapes), METADATA_PARAMS(NewProp_Shapes_MetaData, ARRAY_COUNT(NewProp_Shapes_MetaData)) };
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Shapes_Inner = { UE4CodeGen_Private::EPropertyClass::Object, "Shapes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UClass_APopcornFXAttributeSamplerActor_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMeshSubIndex_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_StaticMeshSubIndex = { UE4CodeGen_Private::EPropertyClass::Int, "StaticMeshSubIndex", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, StaticMeshSubIndex), METADATA_PARAMS(NewProp_StaticMeshSubIndex_MetaData, ARRAY_COUNT(NewProp_StaticMeshSubIndex_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMesh_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_StaticMesh = { UE4CodeGen_Private::EPropertyClass::Object, "StaticMesh", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, StaticMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(NewProp_StaticMesh_MetaData, ARRAY_COUNT(NewProp_StaticMesh_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CollectionUseShapeWeights_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "Distribute sampling by the sub-Shapes own Weights (in addition to CollectionSamplingHeuristic)" },
			};
#endif
			auto NewProp_CollectionUseShapeWeights_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerShape*)Obj)->CollectionUseShapeWeights = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_CollectionUseShapeWeights = { UE4CodeGen_Private::EPropertyClass::Bool, "CollectionUseShapeWeights", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerShape), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_CollectionUseShapeWeights_SetBit)>::SetBit, METADATA_PARAMS(NewProp_CollectionUseShapeWeights_MetaData, ARRAY_COUNT(NewProp_CollectionUseShapeWeights_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CollectionSamplingHeuristic_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "Distribute sampling by the given CollectionSamplingHeuristic of sub-Shapes" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_CollectionSamplingHeuristic = { UE4CodeGen_Private::EPropertyClass::Byte, "CollectionSamplingHeuristic", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, CollectionSamplingHeuristic), Z_Construct_UEnum_PopcornFX_EPopcornFXShapeCollectionSamplingHeuristic, METADATA_PARAMS(NewProp_CollectionSamplingHeuristic_MetaData, ARRAY_COUNT(NewProp_CollectionSamplingHeuristic_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Height_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Height = { UE4CodeGen_Private::EPropertyClass::Float, "Height", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, Height), METADATA_PARAMS(NewProp_Height_MetaData, ARRAY_COUNT(NewProp_Height_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_InnerRadius_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_InnerRadius = { UE4CodeGen_Private::EPropertyClass::Float, "InnerRadius", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, InnerRadius), METADATA_PARAMS(NewProp_InnerRadius_MetaData, ARRAY_COUNT(NewProp_InnerRadius_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Radius_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Radius = { UE4CodeGen_Private::EPropertyClass::Float, "Radius", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, Radius), METADATA_PARAMS(NewProp_Radius_MetaData, ARRAY_COUNT(NewProp_Radius_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BoxDimension_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_BoxDimension = { UE4CodeGen_Private::EPropertyClass::Struct, "BoxDimension", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, BoxDimension), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(NewProp_BoxDimension_MetaData, ARRAY_COUNT(NewProp_BoxDimension_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Weight_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
				{ "ToolTip", "Weights sampling distribution when is a sub-Shape of a Shape Collection\n(if CollectionUseShapeWeights is enabled in the Shape Collection)." },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Weight = { UE4CodeGen_Private::EPropertyClass::Float, "Weight", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, Weight), METADATA_PARAMS(NewProp_Weight_MetaData, ARRAY_COUNT(NewProp_Weight_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ShapeType_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerShape.h" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_ShapeType = { UE4CodeGen_Private::EPropertyClass::Byte, "ShapeType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerShape, ShapeType), Z_Construct_UEnum_PopcornFX_EPopcornFXAttribSamplerShapeType, METADATA_PARAMS(NewProp_ShapeType_MetaData, ARRAY_COUNT(NewProp_ShapeType_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bUseRelativeTransform,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Shapes,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Shapes_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMeshSubIndex,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_StaticMesh,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CollectionUseShapeWeights,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CollectionSamplingHeuristic,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Height,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InnerRadius,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Radius,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BoxDimension,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Weight,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ShapeType,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSamplerShape>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSamplerShape::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSamplerShape, 995576980);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSamplerShape(Z_Construct_UClass_UPopcornFXAttributeSamplerShape, &UPopcornFXAttributeSamplerShape::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSamplerShape"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSamplerShape);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
