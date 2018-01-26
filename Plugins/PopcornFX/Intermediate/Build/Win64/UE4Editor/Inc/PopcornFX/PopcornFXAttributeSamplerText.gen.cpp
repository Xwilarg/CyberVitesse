// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerText.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerText() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerText_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerText();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerText_SetText();
// End Cross Module References
	void UPopcornFXAttributeSamplerText::StaticRegisterNativesUPopcornFXAttributeSamplerText()
	{
		UClass* Class = UPopcornFXAttributeSamplerText::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SetText", (Native)&UPopcornFXAttributeSamplerText::execSetText },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerText_SetText()
	{
		struct PopcornFXAttributeSamplerText_eventSetText_Parms
		{
			FString InText;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_InText = { UE4CodeGen_Private::EPropertyClass::Str, "InText", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerText_eventSetText_Parms, InText), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InText,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerText.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerText, "SetText", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerText_eventSetText_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerText_NoRegister()
	{
		return UPopcornFXAttributeSamplerText::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerText()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXAttributeSampler,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerText_SetText, "SetText" }, // 431810776
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSamplerText.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerText.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Text** by a **FString**." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Text_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerText.h" },
				{ "ToolTip", "The Text to be sampled" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_Text = { UE4CodeGen_Private::EPropertyClass::Str, "Text", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerText, Text), METADATA_PARAMS(NewProp_Text_MetaData, ARRAY_COUNT(NewProp_Text_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Text,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSamplerText>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSamplerText::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSamplerText, 498499586);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSamplerText(Z_Construct_UClass_UPopcornFXAttributeSamplerText, &UPopcornFXAttributeSamplerText::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSamplerText"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSamplerText);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
