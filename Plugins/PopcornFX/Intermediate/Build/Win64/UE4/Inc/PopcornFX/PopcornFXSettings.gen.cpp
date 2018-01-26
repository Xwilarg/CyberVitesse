// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXSettings.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXSettings() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXLocalizedPagesMode();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXRenderCullMethod();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXDrawCallSortMethod();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedBillboarding();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedSimulation();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXRenderSettings();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSimulationSettings();
	ENGINE_API UEnum* Z_Construct_UEnum_Engine_ETickingGroup();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXCollisionFilter();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSettings_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSettings();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings();
// End Cross Module References
	static UEnum* EPopcornFXLocalizedPagesMode_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXLocalizedPagesMode, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXLocalizedPagesMode"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXLocalizedPagesMode(EPopcornFXLocalizedPagesMode_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXLocalizedPagesMode"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXLocalizedPagesMode_CRC() { return 2342335635U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXLocalizedPagesMode()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXLocalizedPagesMode"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXLocalizedPagesMode_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXLocalizedPagesMode::Disable", (int64)EPopcornFXLocalizedPagesMode::Disable },
				{ "EPopcornFXLocalizedPagesMode::EnableDefaultsToOff", (int64)EPopcornFXLocalizedPagesMode::EnableDefaultsToOff },
				{ "EPopcornFXLocalizedPagesMode::EnableDefaultsToOn", (int64)EPopcornFXLocalizedPagesMode::EnableDefaultsToOn },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "Disable.ToolTip", "Disabled" },
				{ "EnableDefaultsToOff.ToolTip", "Enabled, Default will be off, needs per-layer explicit enabling." },
				{ "EnableDefaultsToOn.ToolTip", "Enabled, Default will be on, unless per-layer explicit disabling." },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "PopcornFX Localized page mode" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXLocalizedPagesMode",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXLocalizedPagesMode::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXRenderCullMethod_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXRenderCullMethod, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXRenderCullMethod"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXRenderCullMethod(EPopcornFXRenderCullMethod_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXRenderCullMethod"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXRenderCullMethod_CRC() { return 2399749338U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXRenderCullMethod()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXRenderCullMethod"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXRenderCullMethod_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXRenderCullMethod::None", (int64)EPopcornFXRenderCullMethod::None },
				{ "EPopcornFXRenderCullMethod::DrawRequestsAgainstFrustrum", (int64)EPopcornFXRenderCullMethod::DrawRequestsAgainstFrustrum },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "DrawRequestsAgainstFrustrum.ToolTip", "Can cull entire Particle Draw Requests if BBox are out of Views's frustrums." },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "None.ToolTip", "Do NOT Cull Particle to Render." },
				{ "ToolTip", "How to early Cull PopcornFX Particles for Rendering." },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXRenderCullMethod",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXRenderCullMethod::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXDrawCallSortMethod_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXDrawCallSortMethod, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXDrawCallSortMethod"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXDrawCallSortMethod(EPopcornFXDrawCallSortMethod_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXDrawCallSortMethod"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXDrawCallSortMethod_CRC() { return 2178451999U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXDrawCallSortMethod()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXDrawCallSortMethod"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXDrawCallSortMethod_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXDrawCallSortMethod::None", (int64)EPopcornFXDrawCallSortMethod::None },
				{ "EPopcornFXDrawCallSortMethod::PerDrawCalls", (int64)EPopcornFXDrawCallSortMethod::PerDrawCalls },
				{ "EPopcornFXDrawCallSortMethod::PerSlicedDrawCalls", (int64)EPopcornFXDrawCallSortMethod::PerSlicedDrawCalls },
				{ "EPopcornFXDrawCallSortMethod::PerPageDrawCalls", (int64)EPopcornFXDrawCallSortMethod::PerPageDrawCalls },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "None.ToolTip", "Do NOT sort draw calls." },
				{ "PerDrawCalls.ToolTip", "Sort Draw Calls by their Bounding Boxes.\n\n- Pro: no performance overhead\n- Con: still have sorting issues" },
				{ "PerPageDrawCalls.ToolTip", "Experimental ! work in progress !\nEnables localazied Particle Pages.\nThen sort draw call per pages.\n\n- Pro: fixes more sorting issues\n- Con: performance overhead: sorts ALL particles (even additive ones)\n- Con: performance overhead: increase the number of draw calls" },
				{ "PerSlicedDrawCalls.ToolTip", "Experimental ! work in progress !\nSlice Draw Calls, then sort those slices.\n\n- Pro: fixes more sorting issues\n- Con: performance overhead: sorts ALL particles (even additive ones)\n- Con: performance overhead: increase the number of draw calls" },
				{ "ToolTip", "How to Sort PopcornFX Particles Draw Calls" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXDrawCallSortMethod",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXDrawCallSortMethod::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXConfigPreferedBillboarding_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedBillboarding, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXConfigPreferedBillboarding"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXConfigPreferedBillboarding(EPopcornFXConfigPreferedBillboarding_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXConfigPreferedBillboarding"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedBillboarding_CRC() { return 1219842470U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedBillboarding()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXConfigPreferedBillboarding"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedBillboarding_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXConfigPreferedBillboarding::MaxCPU", (int64)EPopcornFXConfigPreferedBillboarding::MaxCPU },
				{ "EPopcornFXConfigPreferedBillboarding::Auto", (int64)EPopcornFXConfigPreferedBillboarding::Auto },
				{ "EPopcornFXConfigPreferedBillboarding::MaxGPU", (int64)EPopcornFXConfigPreferedBillboarding::MaxGPU },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "Auto.DisplayName", "Auto" },
				{ "Auto.ToolTip", "Same as MaxCPU, but if some CPU and GPU particles can be batched together, Batch them and Billboard on GPU." },
				{ "MaxCPU.DisplayName", "Maximum CPU" },
				{ "MaxCPU.ToolTip", "All CPU particles billboarded on CPU, keep GPU ones on GPU" },
				{ "MaxGPU.DisplayName", "Maximum GPU" },
				{ "MaxGPU.ToolTip", "Try to always billboard on GPU, even CPU particles (but, some billboarding features can still have to run on CPU)" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Where to Billboard PopcornFX Particles" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXConfigPreferedBillboarding",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXConfigPreferedBillboarding::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXConfigPreferedSimulation_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedSimulation, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXConfigPreferedSimulation"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXConfigPreferedSimulation(EPopcornFXConfigPreferedSimulation_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXConfigPreferedSimulation"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedSimulation_CRC() { return 4291878046U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedSimulation()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXConfigPreferedSimulation"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedSimulation_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXConfigPreferedSimulation::CPU", (int64)EPopcornFXConfigPreferedSimulation::CPU },
				{ "EPopcornFXConfigPreferedSimulation::GPU", (int64)EPopcornFXConfigPreferedSimulation::GPU },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "CPU.DisplayName", "CPU only" },
				{ "CPU.ToolTip", "Simulate Particles on CPU." },
				{ "GPU.DisplayName", "GPU when possible" },
				{ "GPU.ToolTip", "EXPERIMENTAL. Does not work, yet." },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Where to Simulate/Update PopcornFX Particles" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXConfigPreferedSimulation",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXConfigPreferedSimulation::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FPopcornFXRenderSettings::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXRenderSettings_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXRenderSettings, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXRenderSettings"), sizeof(FPopcornFXRenderSettings), Get_Z_Construct_UScriptStruct_FPopcornFXRenderSettings_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXRenderSettings(FPopcornFXRenderSettings::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXRenderSettings"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXRenderSettings
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXRenderSettings()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXRenderSettings")),new UScriptStruct::TCppStructOps<FPopcornFXRenderSettings>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXRenderSettings;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXRenderSettings()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXRenderSettings_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXRenderSettings"), sizeof(FPopcornFXRenderSettings), Get_Z_Construct_UScriptStruct_FPopcornFXRenderSettings_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Overridable PopcornFX Render Settings" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXRenderSettings>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SlicedDraw_PreferedSliceDepth_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ClampMax", "100000" },
				{ "ClampMin", "1" },
				{ "EditCondition", "bOverride_SlicedDraw_PreferedSliceDepth" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "UIMax", "100000" },
				{ "UIMin", "1" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SlicedDraw_PreferedSliceDepth = { UE4CodeGen_Private::EPropertyClass::Float, "SlicedDraw_PreferedSliceDepth", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, SlicedDraw_PreferedSliceDepth), METADATA_PARAMS(NewProp_SlicedDraw_PreferedSliceDepth_MetaData, ARRAY_COUNT(NewProp_SlicedDraw_PreferedSliceDepth_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_SlicedDraw_PreferedSliceDepth_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_SlicedDraw_PreferedSliceDepth_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_SlicedDraw_PreferedSliceDepth = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_SlicedDraw_PreferedSliceDepth = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_SlicedDraw_PreferedSliceDepth", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_SlicedDraw_PreferedSliceDepth_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_SlicedDraw_PreferedSliceDepth_MetaData, ARRAY_COUNT(NewProp_bOverride_SlicedDraw_PreferedSliceDepth_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bShadows_DisableCull_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_bShadows_DisableCull" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Force disable Culling at Render for all shadow casting Particles.\n(valid if Culling is enabled and Enable*Shadows is/are enabled)\nWill fix ALL missing shadows.\nIf true, ALL shadow casting particles will be Billboarded, performances will be impacted." },
			};
#endif
			auto NewProp_bShadows_DisableCull_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bShadows_DisableCull = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bShadows_DisableCull = { UE4CodeGen_Private::EPropertyClass::Bool, "bShadows_DisableCull", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bShadows_DisableCull_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bShadows_DisableCull_MetaData, ARRAY_COUNT(NewProp_bShadows_DisableCull_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bShadows_DisableCull_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bShadows_DisableCull_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_bShadows_DisableCull = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bShadows_DisableCull = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bShadows_DisableCull", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bShadows_DisableCull_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bShadows_DisableCull_MetaData, ARRAY_COUNT(NewProp_bOverride_bShadows_DisableCull_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Shadows_ExtendCullBBoxes_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_Shadows_ExtendCullBBoxes" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Extend Culling Bounding Boxes for all shadows casting Particles.\n(valid if Culling is enabled and Enable*Shadows is/are enabled)\n\nCan fix SOME missing shadows.\nCan be seen as a \"Cast Shadow Radius\":\nShadow casting particles are culled if out of view and farther than Shadows_ExtendCullBBoxes cm.\n\nThe higher the Extent is, the more particles will be Billboarded, it can impact performances." },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Shadows_ExtendCullBBoxes = { UE4CodeGen_Private::EPropertyClass::Float, "Shadows_ExtendCullBBoxes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, Shadows_ExtendCullBBoxes), METADATA_PARAMS(NewProp_Shadows_ExtendCullBBoxes_MetaData, ARRAY_COUNT(NewProp_Shadows_ExtendCullBBoxes_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_Shadows_ExtendCullBBoxes_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_Shadows_ExtendCullBBoxes_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_Shadows_ExtendCullBBoxes = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_Shadows_ExtendCullBBoxes = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_Shadows_ExtendCullBBoxes", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_Shadows_ExtendCullBBoxes_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_Shadows_ExtendCullBBoxes_MetaData, ARRAY_COUNT(NewProp_bOverride_Shadows_ExtendCullBBoxes_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MeshParticlesLODBias_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_MeshParticlesLODBias" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "LOD Bias for Particle Renderer Mesh" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_MeshParticlesLODBias = { UE4CodeGen_Private::EPropertyClass::Int, "MeshParticlesLODBias", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, MeshParticlesLODBias), METADATA_PARAMS(NewProp_MeshParticlesLODBias_MetaData, ARRAY_COUNT(NewProp_MeshParticlesLODBias_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_MeshParticlesLODBias_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_MeshParticlesLODBias_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_MeshParticlesLODBias = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_MeshParticlesLODBias = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_MeshParticlesLODBias", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_MeshParticlesLODBias_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_MeshParticlesLODBias_MetaData, ARRAY_COUNT(NewProp_bOverride_MeshParticlesLODBias_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bForceLightsLitTranslucent_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_bForceLightsLitTranslucent" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Force Particle Lights to Lit Translucent Geometry (including other Lit Particles)" },
			};
#endif
			auto NewProp_bForceLightsLitTranslucent_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bForceLightsLitTranslucent = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bForceLightsLitTranslucent = { UE4CodeGen_Private::EPropertyClass::Bool, "bForceLightsLitTranslucent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bForceLightsLitTranslucent_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bForceLightsLitTranslucent_MetaData, ARRAY_COUNT(NewProp_bForceLightsLitTranslucent_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bForceLightsLitTranslucent_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bForceLightsLitTranslucent_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_bForceLightsLitTranslucent = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bForceLightsLitTranslucent = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bForceLightsLitTranslucent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bForceLightsLitTranslucent_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bForceLightsLitTranslucent_MetaData, ARRAY_COUNT(NewProp_bOverride_bForceLightsLitTranslucent_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableMeshShadows_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_bEnableMeshShadows" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Enable Shadow Casting for Lit or Solid Mesh Particles." },
			};
#endif
			auto NewProp_bEnableMeshShadows_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bEnableMeshShadows = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableMeshShadows = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableMeshShadows", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableMeshShadows_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableMeshShadows_MetaData, ARRAY_COUNT(NewProp_bEnableMeshShadows_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bEnableMeshShadows_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bEnableMeshShadows_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_bEnableMeshShadows = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bEnableMeshShadows = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bEnableMeshShadows", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bEnableMeshShadows_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bEnableMeshShadows_MetaData, ARRAY_COUNT(NewProp_bOverride_bEnableMeshShadows_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableRibbonShadows_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_bEnableRibbonShadows" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Enable Shadow Casting for Lit or Solid Ribbon Particles." },
			};
#endif
			auto NewProp_bEnableRibbonShadows_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bEnableRibbonShadows = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableRibbonShadows = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableRibbonShadows", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableRibbonShadows_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableRibbonShadows_MetaData, ARRAY_COUNT(NewProp_bEnableRibbonShadows_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bEnableRibbonShadows_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bEnableRibbonShadows_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_bEnableRibbonShadows = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bEnableRibbonShadows = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bEnableRibbonShadows", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bEnableRibbonShadows_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bEnableRibbonShadows_MetaData, ARRAY_COUNT(NewProp_bOverride_bEnableRibbonShadows_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableBillboardShadows_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_bEnableBillboardShadows" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Enable Shadow Casting for Lit or Solid Billboard Particles." },
			};
#endif
			auto NewProp_bEnableBillboardShadows_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bEnableBillboardShadows = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableBillboardShadows = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableBillboardShadows", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableBillboardShadows_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableBillboardShadows_MetaData, ARRAY_COUNT(NewProp_bEnableBillboardShadows_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bEnableBillboardShadows_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bEnableBillboardShadows_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_bEnableBillboardShadows = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bEnableBillboardShadows = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bEnableBillboardShadows", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bEnableBillboardShadows_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bEnableBillboardShadows_MetaData, ARRAY_COUNT(NewProp_bOverride_bEnableBillboardShadows_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableEarlyFrameRelease_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_bEnableEarlyFrameRelease" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Tries to early release RenderThread's particle data copy.\nCan reduce a bit Tick time and memory usage.\nBut could have shadow flickering issues, and other unforseen side effects." },
			};
#endif
			auto NewProp_bEnableEarlyFrameRelease_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bEnableEarlyFrameRelease = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableEarlyFrameRelease = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableEarlyFrameRelease", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableEarlyFrameRelease_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableEarlyFrameRelease_MetaData, ARRAY_COUNT(NewProp_bEnableEarlyFrameRelease_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bEnableEarlyFrameRelease_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bEnableEarlyFrameRelease_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_bEnableEarlyFrameRelease = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bEnableEarlyFrameRelease = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bEnableEarlyFrameRelease", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bEnableEarlyFrameRelease_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bEnableEarlyFrameRelease_MetaData, ARRAY_COUNT(NewProp_bOverride_bEnableEarlyFrameRelease_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FarCullDistance_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_FarCullDistance" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Cull draw batches farther than FarCullDistance. (0 means disabled).\nIf RenderCullMethod is None or DrawRequestsAgainstFrustrum, culls Draw Requests." },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FarCullDistance = { UE4CodeGen_Private::EPropertyClass::Float, "FarCullDistance", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, FarCullDistance), METADATA_PARAMS(NewProp_FarCullDistance_MetaData, ARRAY_COUNT(NewProp_FarCullDistance_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_FarCullDistance_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_FarCullDistance_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_FarCullDistance = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_FarCullDistance = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_FarCullDistance", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_FarCullDistance_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_FarCullDistance_MetaData, ARRAY_COUNT(NewProp_bOverride_FarCullDistance_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RenderCullMethod_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_RenderCullMethod" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "How to early Cull particles for Renderering." },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_RenderCullMethod = { UE4CodeGen_Private::EPropertyClass::Byte, "RenderCullMethod", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, RenderCullMethod), Z_Construct_UEnum_PopcornFX_EPopcornFXRenderCullMethod, METADATA_PARAMS(NewProp_RenderCullMethod_MetaData, ARRAY_COUNT(NewProp_RenderCullMethod_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_RenderCullMethod_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_RenderCullMethod_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_RenderCullMethod = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_RenderCullMethod = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_RenderCullMethod", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_RenderCullMethod_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_RenderCullMethod_MetaData, ARRAY_COUNT(NewProp_bOverride_RenderCullMethod_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DrawCallSortMethod_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_DrawCallSortMethod" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "How to Sort batches of Particles for Renderering." },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_DrawCallSortMethod = { UE4CodeGen_Private::EPropertyClass::Byte, "DrawCallSortMethod", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, DrawCallSortMethod), Z_Construct_UEnum_PopcornFX_EPopcornFXDrawCallSortMethod, METADATA_PARAMS(NewProp_DrawCallSortMethod_MetaData, ARRAY_COUNT(NewProp_DrawCallSortMethod_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_DrawCallSortMethod_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_DrawCallSortMethod_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_DrawCallSortMethod = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_DrawCallSortMethod = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_DrawCallSortMethod", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_DrawCallSortMethod_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_DrawCallSortMethod_MetaData, ARRAY_COUNT(NewProp_bOverride_DrawCallSortMethod_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PreferedBillboarding_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "EditCondition", "bOverride_PreferedBillboarding" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Where to Billboard PopcornFX Particles.\n(Shown only if GPU build)" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_PreferedBillboarding = { UE4CodeGen_Private::EPropertyClass::Byte, "PreferedBillboarding", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXRenderSettings, PreferedBillboarding), Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedBillboarding, METADATA_PARAMS(NewProp_PreferedBillboarding_MetaData, ARRAY_COUNT(NewProp_PreferedBillboarding_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_PreferedBillboarding_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_PreferedBillboarding_SetBit = [](void* Obj){ ((FPopcornFXRenderSettings*)Obj)->bOverride_PreferedBillboarding = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_PreferedBillboarding = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_PreferedBillboarding", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXRenderSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_PreferedBillboarding_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_PreferedBillboarding_MetaData, ARRAY_COUNT(NewProp_bOverride_PreferedBillboarding_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SlicedDraw_PreferedSliceDepth,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_SlicedDraw_PreferedSliceDepth,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bShadows_DisableCull,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bShadows_DisableCull,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Shadows_ExtendCullBBoxes,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_Shadows_ExtendCullBBoxes,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_MeshParticlesLODBias,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_MeshParticlesLODBias,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bForceLightsLitTranslucent,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bForceLightsLitTranslucent,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableMeshShadows,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bEnableMeshShadows,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableRibbonShadows,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bEnableRibbonShadows,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableBillboardShadows,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bEnableBillboardShadows,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableEarlyFrameRelease,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bEnableEarlyFrameRelease,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FarCullDistance,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_FarCullDistance,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_RenderCullMethod,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_RenderCullMethod,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DrawCallSortMethod,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_DrawCallSortMethod,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_PreferedBillboarding,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_PreferedBillboarding,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXRenderSettings",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXRenderSettings),
				alignof(FPopcornFXRenderSettings),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXRenderSettings_CRC() { return 4060754921U; }
class UScriptStruct* FPopcornFXSimulationSettings::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSimulationSettings_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXSimulationSettings, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXSimulationSettings"), sizeof(FPopcornFXSimulationSettings), Get_Z_Construct_UScriptStruct_FPopcornFXSimulationSettings_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXSimulationSettings(FPopcornFXSimulationSettings::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXSimulationSettings"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSimulationSettings
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSimulationSettings()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXSimulationSettings")),new UScriptStruct::TCppStructOps<FPopcornFXSimulationSettings>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXSimulationSettings;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSimulationSettings()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSimulationSettings_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXSimulationSettings"), sizeof(FPopcornFXSimulationSettings), Get_Z_Construct_UScriptStruct_FPopcornFXSimulationSettings_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Overridable PopcornFX Simulation Settings." },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXSimulationSettings>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneUpdateTickGroup_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_SceneUpdateTickGroup" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Tick Group for the PopcornFX Scene Update.\n      (Change this at your own risk.)\n      (SceneUpdateTickGroup is taken into account at Component OnRegister.)\n      - You should benchmark !\n      - Scene with Particles with collision/intersect could have bad performance if \"During Physics\".\n      - But Scene with Particles **without** collision/intersect could take advantage to run in parallel in \"During Physics\".\n      - Ticking Later (\"Post Update Work\") can fix particle lag, scene view lag etc, but might affect performance: you should benchmark !" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_SceneUpdateTickGroup = { UE4CodeGen_Private::EPropertyClass::Byte, "SceneUpdateTickGroup", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSimulationSettings, SceneUpdateTickGroup), Z_Construct_UEnum_Engine_ETickingGroup, METADATA_PARAMS(NewProp_SceneUpdateTickGroup_MetaData, ARRAY_COUNT(NewProp_SceneUpdateTickGroup_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_SceneUpdateTickGroup_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_SceneUpdateTickGroup_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_SceneUpdateTickGroup = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_SceneUpdateTickGroup = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_SceneUpdateTickGroup", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_SceneUpdateTickGroup_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_SceneUpdateTickGroup_MetaData, ARRAY_COUNT(NewProp_bOverride_SceneUpdateTickGroup_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LocalizedPagesMode_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_LocalizedPagesMode" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Experimental ! work in progress ! Will enable Particle Page localization." },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_LocalizedPagesMode = { UE4CodeGen_Private::EPropertyClass::Byte, "LocalizedPagesMode", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSimulationSettings, LocalizedPagesMode), Z_Construct_UEnum_PopcornFX_EPopcornFXLocalizedPagesMode, METADATA_PARAMS(NewProp_LocalizedPagesMode_MetaData, ARRAY_COUNT(NewProp_LocalizedPagesMode_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_LocalizedPagesMode_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_LocalizedPagesMode_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_LocalizedPagesMode = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_LocalizedPagesMode = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_LocalizedPagesMode", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_LocalizedPagesMode_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_LocalizedPagesMode_MetaData, ARRAY_COUNT(NewProp_bOverride_LocalizedPagesMode_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CollisionFilters_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_CollisionFilters" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Collision Filters indexed by CParticleEvolverPhysics's FilterIDs." },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_CollisionFilters = { UE4CodeGen_Private::EPropertyClass::Array, "CollisionFilters", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSimulationSettings, CollisionFilters), METADATA_PARAMS(NewProp_CollisionFilters_MetaData, ARRAY_COUNT(NewProp_CollisionFilters_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_CollisionFilters_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "CollisionFilters", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FPopcornFXCollisionFilter, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_CollisionFilters_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_CollisionFilters_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_CollisionFilters = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_CollisionFilters = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_CollisionFilters", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_CollisionFilters_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_CollisionFilters_MetaData, ARRAY_COUNT(NewProp_bOverride_CollisionFilters_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DefaultCollisionFilter_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_DefaultCollisionFilter" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Default Collision Filter for PopcornFX Particle Collision queries." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_DefaultCollisionFilter = { UE4CodeGen_Private::EPropertyClass::Struct, "DefaultCollisionFilter", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSimulationSettings, DefaultCollisionFilter), Z_Construct_UScriptStruct_FPopcornFXCollisionFilter, METADATA_PARAMS(NewProp_DefaultCollisionFilter_MetaData, ARRAY_COUNT(NewProp_DefaultCollisionFilter_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_DefaultCollisionFilter_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_DefaultCollisionFilter_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_DefaultCollisionFilter = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_DefaultCollisionFilter = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_DefaultCollisionFilter", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_DefaultCollisionFilter_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_DefaultCollisionFilter_MetaData, ARRAY_COUNT(NewProp_bOverride_DefaultCollisionFilter_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableEventListeners_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_bEnableEventListeners" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Can broadcasted particle events trigger delegates" },
			};
#endif
			auto NewProp_bEnableEventListeners_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bEnableEventListeners = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableEventListeners = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableEventListeners", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableEventListeners_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableEventListeners_MetaData, ARRAY_COUNT(NewProp_bEnableEventListeners_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bEnableEventListeners_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bEnableEventListeners_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_bEnableEventListeners = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bEnableEventListeners = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bEnableEventListeners", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bEnableEventListeners_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bEnableEventListeners_MetaData, ARRAY_COUNT(NewProp_bOverride_bEnableEventListeners_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnablePhysicalMaterials_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_bEnablePhysicalMaterials" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Particle Collisions takes Physical Materials into account." },
			};
#endif
			auto NewProp_bEnablePhysicalMaterials_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bEnablePhysicalMaterials = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnablePhysicalMaterials = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnablePhysicalMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnablePhysicalMaterials_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnablePhysicalMaterials_MetaData, ARRAY_COUNT(NewProp_bEnablePhysicalMaterials_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_bEnablePhysicalMaterials_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_bEnablePhysicalMaterials_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_bEnablePhysicalMaterials = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_bEnablePhysicalMaterials = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_bEnablePhysicalMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_bEnablePhysicalMaterials_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_bEnablePhysicalMaterials_MetaData, ARRAY_COUNT(NewProp_bOverride_bEnablePhysicalMaterials_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DecalsMaxLifeSpan_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_DecalsMaxLifeSpan" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Maximum life span for decals spawned by PopconFX Particles." },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_DecalsMaxLifeSpan = { UE4CodeGen_Private::EPropertyClass::Float, "DecalsMaxLifeSpan", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSimulationSettings, DecalsMaxLifeSpan), METADATA_PARAMS(NewProp_DecalsMaxLifeSpan_MetaData, ARRAY_COUNT(NewProp_DecalsMaxLifeSpan_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_DecalsMaxLifeSpan_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "/!\\ Hidden if no PK_HAS_GPU" },
			};
#endif
			auto NewProp_bOverride_DecalsMaxLifeSpan_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_DecalsMaxLifeSpan = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_DecalsMaxLifeSpan = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_DecalsMaxLifeSpan", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_DecalsMaxLifeSpan_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_DecalsMaxLifeSpan_MetaData, ARRAY_COUNT(NewProp_bOverride_DecalsMaxLifeSpan_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PreferedSimulation_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "EditCondition", "bOverride_PreferedSimulation" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Where to Simulate/Update PopcornFX Particles.\n(Shown only if GPU build)" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_PreferedSimulation = { UE4CodeGen_Private::EPropertyClass::Byte, "PreferedSimulation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXSimulationSettings, PreferedSimulation), Z_Construct_UEnum_PopcornFX_EPopcornFXConfigPreferedSimulation, METADATA_PARAMS(NewProp_PreferedSimulation_MetaData, ARRAY_COUNT(NewProp_PreferedSimulation_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bOverride_PreferedSimulation_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			auto NewProp_bOverride_PreferedSimulation_SetBit = [](void* Obj){ ((FPopcornFXSimulationSettings*)Obj)->bOverride_PreferedSimulation = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bOverride_PreferedSimulation = { UE4CodeGen_Private::EPropertyClass::Bool, "bOverride_PreferedSimulation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(FPopcornFXSimulationSettings), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bOverride_PreferedSimulation_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bOverride_PreferedSimulation_MetaData, ARRAY_COUNT(NewProp_bOverride_PreferedSimulation_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SceneUpdateTickGroup,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_SceneUpdateTickGroup,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LocalizedPagesMode,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_LocalizedPagesMode,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CollisionFilters,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_CollisionFilters_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_CollisionFilters,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DefaultCollisionFilter,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_DefaultCollisionFilter,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableEventListeners,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bEnableEventListeners,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnablePhysicalMaterials,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_bEnablePhysicalMaterials,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DecalsMaxLifeSpan,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_DecalsMaxLifeSpan,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_PreferedSimulation,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bOverride_PreferedSimulation,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXSimulationSettings",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXSimulationSettings),
				alignof(FPopcornFXSimulationSettings),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXSimulationSettings_CRC() { return 3343206002U; }
class UScriptStruct* FPopcornFXCollisionFilter::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXCollisionFilter_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXCollisionFilter, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXCollisionFilter"), sizeof(FPopcornFXCollisionFilter), Get_Z_Construct_UScriptStruct_FPopcornFXCollisionFilter_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXCollisionFilter(FPopcornFXCollisionFilter::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXCollisionFilter"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXCollisionFilter
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXCollisionFilter()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXCollisionFilter")),new UScriptStruct::TCppStructOps<FPopcornFXCollisionFilter>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXCollisionFilter;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXCollisionFilter()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXCollisionFilter_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXCollisionFilter"), sizeof(FPopcornFXCollisionFilter), Get_Z_Construct_UScriptStruct_FPopcornFXCollisionFilter_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Object Types to include/exclude in PopcornFX collision queries." },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXCollisionFilter>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ObjectTypesToQuery_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_ObjectTypesToQuery = { UE4CodeGen_Private::EPropertyClass::Int, "ObjectTypesToQuery", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXCollisionFilter, ObjectTypesToQuery), METADATA_PARAMS(NewProp_ObjectTypesToQuery_MetaData, ARRAY_COUNT(NewProp_ObjectTypesToQuery_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ObjectTypesToQuery,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXCollisionFilter",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXCollisionFilter),
				alignof(FPopcornFXCollisionFilter),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXCollisionFilter_CRC() { return 3708130099U; }
	void UPopcornFXSettings::StaticRegisterNativesUPopcornFXSettings()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXSettings_NoRegister()
	{
		return UPopcornFXSettings::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXSettings()
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
				{ "IncludePath", "PopcornFXSettings.h" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "PopcornFX Plugin Config" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ProfilerHUD_SubDescriptorsPerObject_MetaData[] = {
				{ "Category", "PopcornFX HUD" },
				{ "ClampMax", "100" },
				{ "ClampMin", "0" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "ProfilerHUD: Number of sub-descriptors per BaseObject to display" },
				{ "UIMax", "100" },
				{ "UIMin", "0" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_ProfilerHUD_SubDescriptorsPerObject = { UE4CodeGen_Private::EPropertyClass::Int, "ProfilerHUD_SubDescriptorsPerObject", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, ProfilerHUD_SubDescriptorsPerObject), METADATA_PARAMS(NewProp_ProfilerHUD_SubDescriptorsPerObject_MetaData, ARRAY_COUNT(NewProp_ProfilerHUD_SubDescriptorsPerObject_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ProfilerHUD_BaseObjectsPerFile_MetaData[] = {
				{ "Category", "PopcornFX HUD" },
				{ "ClampMax", "100" },
				{ "ClampMin", "0" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "ProfilerHUD: Number of BaseObjects to display per PopcornFX File to display" },
				{ "UIMax", "100" },
				{ "UIMin", "0" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_ProfilerHUD_BaseObjectsPerFile = { UE4CodeGen_Private::EPropertyClass::Int, "ProfilerHUD_BaseObjectsPerFile", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, ProfilerHUD_BaseObjectsPerFile), METADATA_PARAMS(NewProp_ProfilerHUD_BaseObjectsPerFile_MetaData, ARRAY_COUNT(NewProp_ProfilerHUD_BaseObjectsPerFile_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HUD_HideNodesBelowPercent_MetaData[] = {
				{ "Category", "PopcornFX HUD" },
				{ "ClampMax", "100" },
				{ "ClampMin", "0" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "HUDs: Hide stat nodes below this percentage" },
				{ "UIMax", "100" },
				{ "UIMin", "0" },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_HUD_HideNodesBelowPercent = { UE4CodeGen_Private::EPropertyClass::Int, "HUD_HideNodesBelowPercent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, HUD_HideNodesBelowPercent), METADATA_PARAMS(NewProp_HUD_HideNodesBelowPercent_MetaData, ARRAY_COUNT(NewProp_HUD_HideNodesBelowPercent_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HUD_DisplayScreenRatio_MetaData[] = {
				{ "Category", "PopcornFX HUD" },
				{ "ClampMax", "1" },
				{ "ClampMin", "0" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "HUDs: Ratio of the Screen used to display HUDs" },
				{ "UIMax", "1" },
				{ "UIMin", "0" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_HUD_DisplayScreenRatio = { UE4CodeGen_Private::EPropertyClass::Float, "HUD_DisplayScreenRatio", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, HUD_DisplayScreenRatio), METADATA_PARAMS(NewProp_HUD_DisplayScreenRatio_MetaData, ARRAY_COUNT(NewProp_HUD_DisplayScreenRatio_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DefaultMaterials_MetaData[] = {
				{ "Category", "PopcornFX Materials" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "List of default materials for the project." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_DefaultMaterials = { UE4CodeGen_Private::EPropertyClass::Struct, "DefaultMaterials", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, DefaultMaterials), Z_Construct_UScriptStruct_FPopcornFXDefaultMaterialsSettings, METADATA_PARAMS(NewProp_DefaultMaterials_MetaData, ARRAY_COUNT(NewProp_DefaultMaterials_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RenderSettings_MetaData[] = {
				{ "Category", "PopcornFX Render Settings" },
				{ "FullyExpand", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "PopcornFX Render Settings." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_RenderSettings = { UE4CodeGen_Private::EPropertyClass::Struct, "RenderSettings", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, RenderSettings), Z_Construct_UScriptStruct_FPopcornFXRenderSettings, METADATA_PARAMS(NewProp_RenderSettings_MetaData, ARRAY_COUNT(NewProp_RenderSettings_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SimulationSettings_MetaData[] = {
				{ "Category", "PopcornFX Simulation Settings" },
				{ "FullyExpand", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "PopcornFX Simulation Settings." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_SimulationSettings = { UE4CodeGen_Private::EPropertyClass::Struct, "SimulationSettings", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, SimulationSettings), Z_Construct_UScriptStruct_FPopcornFXSimulationSettings, METADATA_PARAMS(NewProp_SimulationSettings_MetaData, ARRAY_COUNT(NewProp_SimulationSettings_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_GlobalScale_MetaData[] = {
				{ "Category", "Pack" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "PopcornFX Particles to UE4 Global Scale. ie: GlobalScale = 100 means 1 unit in PopcornFX will become 100 unit in Unreal Engine" },
			};
#endif
			static const UE4CodeGen_Private::FFloatPropertyParams NewProp_GlobalScale = { UE4CodeGen_Private::EPropertyClass::Float, "GlobalScale", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, GlobalScale), METADATA_PARAMS(NewProp_GlobalScale_MetaData, ARRAY_COUNT(NewProp_GlobalScale_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PackMountPoint_MetaData[] = {
				{ "Category", "Pack" },
				{ "ModuleRelativePath", "Public/PopcornFXSettings.h" },
				{ "ToolTip", "Needs Restart Editor !\nThe Content root directory of the PopcornFX Pack.\nAll Assets used by PopcornFX must be under this PackMountPoint." },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_PackMountPoint = { UE4CodeGen_Private::EPropertyClass::Str, "PackMountPoint", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000004001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSettings, PackMountPoint), METADATA_PARAMS(NewProp_PackMountPoint_MetaData, ARRAY_COUNT(NewProp_PackMountPoint_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ProfilerHUD_SubDescriptorsPerObject,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ProfilerHUD_BaseObjectsPerFile,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_HUD_HideNodesBelowPercent,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_HUD_DisplayScreenRatio,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_DefaultMaterials,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_RenderSettings,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SimulationSettings,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_GlobalScale,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_PackMountPoint,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXSettings>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXSettings::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00080086u,
				nullptr, 0,
				PropPointers, ARRAY_COUNT(PropPointers),
				"Engine",
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPopcornFXSettings, 2721870699);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXSettings(Z_Construct_UClass_UPopcornFXSettings, &UPopcornFXSettings::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXSettings"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXSettings);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
