// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Sequencer/Templates/PopcornFXAttributeSectionTemplate.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSectionTemplate() {}
// Cross Module References
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	MOVIESCENETRACKS_API UScriptStruct* Z_Construct_UScriptStruct_FMovieSceneParameterSectionTemplate();
// End Cross Module References
class UScriptStruct* FPopcornFXAttributeSectionTemplate::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXAttributeSectionTemplate"), sizeof(FPopcornFXAttributeSectionTemplate), Get_Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXAttributeSectionTemplate(FPopcornFXAttributeSectionTemplate::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXAttributeSectionTemplate"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXAttributeSectionTemplate
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXAttributeSectionTemplate()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXAttributeSectionTemplate")),new UScriptStruct::TCppStructOps<FPopcornFXAttributeSectionTemplate>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXAttributeSectionTemplate;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXAttributeSectionTemplate"), sizeof(FPopcornFXAttributeSectionTemplate), Get_Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Private/Sequencer/Templates/PopcornFXAttributeSectionTemplate.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXAttributeSectionTemplate>(); };
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				Z_Construct_UScriptStruct_FMovieSceneParameterSectionTemplate,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXAttributeSectionTemplate",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000005),
				sizeof(FPopcornFXAttributeSectionTemplate),
				alignof(FPopcornFXAttributeSectionTemplate),
				nullptr, 0,
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXAttributeSectionTemplate_CRC() { return 4256779425U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
