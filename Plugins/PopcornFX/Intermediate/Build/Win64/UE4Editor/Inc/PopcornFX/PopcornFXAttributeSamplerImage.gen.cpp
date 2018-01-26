// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerImage.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerImage() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerImage_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerImage();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerImage_SetTexture();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
// End Cross Module References
	void UPopcornFXAttributeSamplerImage::StaticRegisterNativesUPopcornFXAttributeSamplerImage()
	{
		UClass* Class = UPopcornFXAttributeSamplerImage::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SetTexture", (Native)&UPopcornFXAttributeSamplerImage::execSetTexture },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXAttributeSamplerImage_SetTexture()
	{
		struct PopcornFXAttributeSamplerImage_eventSetTexture_Parms
		{
			UTexture2D* InTexture;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InTexture = { UE4CodeGen_Private::EPropertyClass::Object, "InTexture", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXAttributeSamplerImage_eventSetTexture_Parms, InTexture), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InTexture,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerImage.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXAttributeSamplerImage, "SetTexture", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXAttributeSamplerImage_eventSetTexture_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerImage_NoRegister()
	{
		return UPopcornFXAttributeSamplerImage::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerImage()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXAttributeSampler,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXAttributeSamplerImage_SetTexture, "SetTexture" }, // 3489636787
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSamplerImage.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerImage.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Image** by a **UTexture**." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Texture_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerImage.h" },
				{ "ToolTip", "The texture to be sampled" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Texture = { UE4CodeGen_Private::EPropertyClass::Object, "Texture", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerImage, Texture), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_Texture_MetaData, ARRAY_COUNT(NewProp_Texture_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bAllowTextureConvertionAtRuntime_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerImage.h" },
				{ "ToolTip", "Enable to allow PopcornFX to convert the texture at Runtime, if\nthe texture is not in a format directly samplable by PopcornFX.\n/!\\ It can take a significant amount of time to convert." },
			};
#endif
			auto NewProp_bAllowTextureConvertionAtRuntime_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerImage*)Obj)->bAllowTextureConvertionAtRuntime = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAllowTextureConvertionAtRuntime = { UE4CodeGen_Private::EPropertyClass::Bool, "bAllowTextureConvertionAtRuntime", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerImage), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bAllowTextureConvertionAtRuntime_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bAllowTextureConvertionAtRuntime_MetaData, ARRAY_COUNT(NewProp_bAllowTextureConvertionAtRuntime_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Texture,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bAllowTextureConvertionAtRuntime,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSamplerImage>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSamplerImage::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSamplerImage, 408377581);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSamplerImage(Z_Construct_UClass_UPopcornFXAttributeSamplerImage, &UPopcornFXAttributeSamplerImage::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSamplerImage"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSamplerImage);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
