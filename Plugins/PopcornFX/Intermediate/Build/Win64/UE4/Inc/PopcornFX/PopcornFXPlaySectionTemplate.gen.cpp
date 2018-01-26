// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Sequencer/Templates/PopcornFXPlaySectionTemplate.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXPlaySectionTemplate() {}
// Cross Module References
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	MOVIESCENE_API UScriptStruct* Z_Construct_UScriptStruct_FMovieSceneEvalTemplate();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FIntegralCurve();
// End Cross Module References
class UScriptStruct* FPopcornFXPlaySectionTemplate::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXPlaySectionTemplate"), sizeof(FPopcornFXPlaySectionTemplate), Get_Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXPlaySectionTemplate(FPopcornFXPlaySectionTemplate::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXPlaySectionTemplate"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXPlaySectionTemplate
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXPlaySectionTemplate()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXPlaySectionTemplate")),new UScriptStruct::TCppStructOps<FPopcornFXPlaySectionTemplate>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXPlaySectionTemplate;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXPlaySectionTemplate"), sizeof(FPopcornFXPlaySectionTemplate), Get_Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/Sequencer/Templates/PopcornFXPlaySectionTemplate.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXPlaySectionTemplate>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Keys_MetaData[] = {
				{ "ModuleRelativePath", "Private/Sequencer/Templates/PopcornFXPlaySectionTemplate.h" },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Keys = { UE4CodeGen_Private::EPropertyClass::Struct, "Keys", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000000, 1, nullptr, STRUCT_OFFSET(FPopcornFXPlaySectionTemplate, Keys), Z_Construct_UScriptStruct_FIntegralCurve, METADATA_PARAMS(NewProp_Keys_MetaData, ARRAY_COUNT(NewProp_Keys_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Keys,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				Z_Construct_UScriptStruct_FMovieSceneEvalTemplate,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXPlaySectionTemplate",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000005),
				sizeof(FPopcornFXPlaySectionTemplate),
				alignof(FPopcornFXPlaySectionTemplate),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXPlaySectionTemplate_CRC() { return 2986735869U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
