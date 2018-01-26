// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXSceneComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXSceneComponent() {}
// Cross Module References
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXRaiseEventSignature__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXHeavyDebugMode();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXSceneBBMode();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXFieldListener();
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSceneComponent_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXSceneComponent();
	ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXSceneComponent_Clear();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXSceneComponent_RegisterEventListener();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXSceneComponent_UnregisterEventListener();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXRenderSettings();
	POPCORNFX_API UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXSimulationSettings();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FBox();
// End Cross Module References
	UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXRaiseEventSignature__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_PopcornFX, "PopcornFXRaiseEventSignature__DelegateSignature", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00120000, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	static UEnum* EPopcornFXHeavyDebugMode_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXHeavyDebugMode, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXHeavyDebugMode"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXHeavyDebugMode(EPopcornFXHeavyDebugMode_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXHeavyDebugMode"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXHeavyDebugMode_CRC() { return 384180650U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXHeavyDebugMode()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXHeavyDebugMode"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXHeavyDebugMode_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXHeavyDebugMode::None", (int64)EPopcornFXHeavyDebugMode::None },
				{ "EPopcornFXHeavyDebugMode::ParticlePoints", (int64)EPopcornFXHeavyDebugMode::ParticlePoints },
				{ "EPopcornFXHeavyDebugMode::MediumsBBox", (int64)EPopcornFXHeavyDebugMode::MediumsBBox },
				{ "EPopcornFXHeavyDebugMode::PagesBBox", (int64)EPopcornFXHeavyDebugMode::PagesBBox },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "MediumsBBox.ToolTip", "Debug Mediums BBox" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "None.ToolTip", "No debug" },
				{ "PagesBBox.ToolTip", "Debug Particle Pages bounding boxes, colored by mediums" },
				{ "ParticlePoints.ToolTip", "Debug Particles as points, colored by mediums" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXHeavyDebugMode",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXHeavyDebugMode::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* EPopcornFXSceneBBMode_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXSceneBBMode, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXSceneBBMode"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXSceneBBMode(EPopcornFXSceneBBMode_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXSceneBBMode"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXSceneBBMode_CRC() { return 1652820916U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXSceneBBMode()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXSceneBBMode"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXSceneBBMode_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXSceneBBMode::Dynamic", (int64)EPopcornFXSceneBBMode::Dynamic },
				{ "EPopcornFXSceneBBMode::DynamicPlusFixed", (int64)EPopcornFXSceneBBMode::DynamicPlusFixed },
				{ "EPopcornFXSceneBBMode::Fixed", (int64)EPopcornFXSceneBBMode::Fixed },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXSceneBBMode",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXSceneBBMode::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FPopcornFXFieldListener::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern POPCORNFX_API uint32 Get_Z_Construct_UScriptStruct_FPopcornFXFieldListener_CRC();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPopcornFXFieldListener, Z_Construct_UPackage__Script_PopcornFX(), TEXT("PopcornFXFieldListener"), sizeof(FPopcornFXFieldListener), Get_Z_Construct_UScriptStruct_FPopcornFXFieldListener_CRC());
	}
	return Singleton;
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPopcornFXFieldListener(FPopcornFXFieldListener::StaticStruct, TEXT("/Script/PopcornFX"), TEXT("PopcornFXFieldListener"), false, nullptr, nullptr);
static struct FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXFieldListener
{
	FScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXFieldListener()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("PopcornFXFieldListener")),new UScriptStruct::TCppStructOps<FPopcornFXFieldListener>);
	}
} ScriptStruct_PopcornFX_StaticRegisterNativesFPopcornFXFieldListener;
	UScriptStruct* Z_Construct_UScriptStruct_FPopcornFXFieldListener()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPopcornFXFieldListener_CRC();
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("PopcornFXFieldListener"), sizeof(FPopcornFXFieldListener), Get_Z_Construct_UScriptStruct_FPopcornFXFieldListener_CRC(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
			};
#endif
			auto NewStructOpsLambda = []() -> void* { return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPopcornFXFieldListener>(); };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FieldType_MetaData[] = {
				{ "Category", "PopcornFX FieldListener" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_FieldType = { UE4CodeGen_Private::EPropertyClass::Byte, "FieldType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXFieldListener, FieldType), Z_Construct_UEnum_PopcornFX_EPopcornFXParticleFieldType, METADATA_PARAMS(NewProp_FieldType_MetaData, ARRAY_COUNT(NewProp_FieldType_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FieldName_MetaData[] = {
				{ "Category", "PopcornFX FieldListener" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_FieldName = { UE4CodeGen_Private::EPropertyClass::Name, "FieldName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(FPopcornFXFieldListener, FieldName), METADATA_PARAMS(NewProp_FieldName_MetaData, ARRAY_COUNT(NewProp_FieldName_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FieldType,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FieldName,
			};
			static const UE4CodeGen_Private::FStructParams ReturnStructParams = {
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
				nullptr,
				&UE4CodeGen_Private::TNewCppStructOpsWrapper<decltype(NewStructOpsLambda)>::NewCppStructOps,
				"PopcornFXFieldListener",
				RF_Public|RF_Transient|RF_MarkAsNative,
				EStructFlags(0x00000001),
				sizeof(FPopcornFXFieldListener),
				alignof(FPopcornFXFieldListener),
				PropPointers, ARRAY_COUNT(PropPointers),
				METADATA_PARAMS(Struct_MetaDataParams, ARRAY_COUNT(Struct_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPopcornFXFieldListener_CRC() { return 3707868201U; }
	void UPopcornFXSceneComponent::StaticRegisterNativesUPopcornFXSceneComponent()
	{
		UClass* Class = UPopcornFXSceneComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "Clear", (Native)&UPopcornFXSceneComponent::execClear },
			{ "RegisterEventListener", (Native)&UPopcornFXSceneComponent::execRegisterEventListener },
			{ "UnregisterEventListener", (Native)&UPopcornFXSceneComponent::execUnregisterEventListener },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXSceneComponent_Clear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Scene" },
				{ "Keywords", "popcornfx scene clear" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Clears this scene from all existing Particles.\n      Warning : Every emitters registered into this scene will stop to emit, you will need to manually restart your effects" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXSceneComponent, "Clear", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXSceneComponent_RegisterEventListener()
	{
		struct PopcornFXSceneComponent_eventRegisterEventListener_Parms
		{
			FScriptDelegate Delegate;
			FName EventName;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXSceneComponent_eventRegisterEventListener_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXSceneComponent_eventRegisterEventListener_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_EventName = { UE4CodeGen_Private::EPropertyClass::Name, "EventName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXSceneComponent_eventRegisterEventListener_Parms, EventName), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FDelegatePropertyParams NewProp_Delegate = { UE4CodeGen_Private::EPropertyClass::Delegate, "Delegate", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXSceneComponent_eventRegisterEventListener_Parms, Delegate), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXRaiseEventSignature__DelegateSignature, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EventName,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Delegate,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "CPP_Default_EventName", "None" },
				{ "Keywords", "popcornfx particle emitter effect system event" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Registers an event listener.\n@param Delegate - Delegate to call when the event is raised.\n@param EventName - Name of the Event to listen." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXSceneComponent, "RegisterEventListener", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXSceneComponent_eventRegisterEventListener_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXSceneComponent_UnregisterEventListener()
	{
		struct PopcornFXSceneComponent_eventUnregisterEventListener_Parms
		{
			FScriptDelegate Delegate;
			FName EventName;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_EventName = { UE4CodeGen_Private::EPropertyClass::Name, "EventName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXSceneComponent_eventUnregisterEventListener_Parms, EventName), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FDelegatePropertyParams NewProp_Delegate = { UE4CodeGen_Private::EPropertyClass::Delegate, "Delegate", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXSceneComponent_eventUnregisterEventListener_Parms, Delegate), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXRaiseEventSignature__DelegateSignature, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EventName,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Delegate,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "CPP_Default_EventName", "None" },
				{ "Keywords", "popcornfx particle emitter effect system event" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Unregisters an event listener\n@param EventName - Name of the Event to stop listening." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXSceneComponent, "UnregisterEventListener", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXSceneComponent_eventUnregisterEventListener_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXSceneComponent_NoRegister()
	{
		return UPopcornFXSceneComponent::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXSceneComponent()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPrimitiveComponent,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXSceneComponent_Clear, "Clear" }, // 4129041040
				{ &Z_Construct_UFunction_UPopcornFXSceneComponent_RegisterEventListener, "RegisterEventListener" }, // 2883172101
				{ &Z_Construct_UFunction_UPopcornFXSceneComponent_UnregisterEventListener, "UnregisterEventListener" }, // 163232192
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Input Collision Replication Materials Mobility Trigger" },
				{ "IncludePath", "PopcornFXSceneComponent.h" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Handles all the PopcornFX Particle Simulation and Rendering context.\n(All PopcornFX Emitters will actually ask a PopcornFXSceneComponent to spawn Particles)" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Render_OverrideAllMaterial_MetaData[] = {
				{ "Category", "PopcornFX SceneDebug" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "DEBUG: Overrides all Particles materials with this one.\n      MUST be compatible for Particle Sprites AND Mesh Particles !\n      (StaticSwitches are not available)" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Render_OverrideAllMaterial = { UE4CodeGen_Private::EPropertyClass::Object, "Render_OverrideAllMaterial", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, Render_OverrideAllMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(NewProp_Render_OverrideAllMaterial_MetaData, ARRAY_COUNT(NewProp_Render_OverrideAllMaterial_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Render_OverrideDebugColorCount_MetaData[] = {
				{ "Category", "PopcornFX SceneDebug" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "DEBUG: The number of colors used for Override Color debugs." },
			};
#endif
			static const UE4CodeGen_Private::FIntPropertyParams NewProp_Render_OverrideDebugColorCount = { UE4CodeGen_Private::EPropertyClass::Int, "Render_OverrideDebugColorCount", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, Render_OverrideDebugColorCount), METADATA_PARAMS(NewProp_Render_OverrideDebugColorCount_MetaData, ARRAY_COUNT(NewProp_Render_OverrideDebugColorCount_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bRender_OverrideColorByDrawCall_MetaData[] = {
				{ "Category", "PopcornFX SceneDebug" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "DEBUG: Overrides Particle Color by a per draw call color." },
			};
#endif
			auto NewProp_bRender_OverrideColorByDrawCall_SetBit = [](void* Obj){ ((UPopcornFXSceneComponent*)Obj)->bRender_OverrideColorByDrawCall = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bRender_OverrideColorByDrawCall = { UE4CodeGen_Private::EPropertyClass::Bool, "bRender_OverrideColorByDrawCall", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSceneComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bRender_OverrideColorByDrawCall_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bRender_OverrideColorByDrawCall_MetaData, ARRAY_COUNT(NewProp_bRender_OverrideColorByDrawCall_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bRender_FreezeBillboardingMatrix_MetaData[] = {
				{ "Category", "PopcornFX SceneDebug" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "DEBUG: Freezes the main billboarding matrix." },
			};
#endif
			auto NewProp_bRender_FreezeBillboardingMatrix_SetBit = [](void* Obj){ ((UPopcornFXSceneComponent*)Obj)->bRender_FreezeBillboardingMatrix = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bRender_FreezeBillboardingMatrix = { UE4CodeGen_Private::EPropertyClass::Bool, "bRender_FreezeBillboardingMatrix", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSceneComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bRender_FreezeBillboardingMatrix_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bRender_FreezeBillboardingMatrix_MetaData, ARRAY_COUNT(NewProp_bRender_FreezeBillboardingMatrix_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HeavyDebugMode_MetaData[] = {
				{ "Category", "PopcornFX SceneDebug" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "DEBUG: Draw some heavy debug." },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_HeavyDebugMode = { UE4CodeGen_Private::EPropertyClass::Byte, "HeavyDebugMode", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, HeavyDebugMode), Z_Construct_UEnum_PopcornFX_EPopcornFXHeavyDebugMode, METADATA_PARAMS(NewProp_HeavyDebugMode_MetaData, ARRAY_COUNT(NewProp_HeavyDebugMode_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FieldListeners_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Global array of fields that the scene will try to recover when an event is broadcasted" },
			};
#endif
			static const UE4CodeGen_Private::FArrayPropertyParams NewProp_FieldListeners = { UE4CodeGen_Private::EPropertyClass::Array, "FieldListeners", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, FieldListeners), METADATA_PARAMS(NewProp_FieldListeners_MetaData, ARRAY_COUNT(NewProp_FieldListeners_MetaData)) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_FieldListeners_Inner = { UE4CodeGen_Private::EPropertyClass::Struct, "FieldListeners", RF_Public|RF_Transient|RF_MarkAsNative, 0x0000000000000000, 1, nullptr, 0, Z_Construct_UScriptStruct_FPopcornFXFieldListener, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RenderSettingsOverride_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Override PopcornFX Config's Render Settings." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_RenderSettingsOverride = { UE4CodeGen_Private::EPropertyClass::Struct, "RenderSettingsOverride", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, RenderSettingsOverride), Z_Construct_UScriptStruct_FPopcornFXRenderSettings, METADATA_PARAMS(NewProp_RenderSettingsOverride_MetaData, ARRAY_COUNT(NewProp_RenderSettingsOverride_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SimulationSettingsOverride_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Override PopcornFX Config's Simulation Settings." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_SimulationSettingsOverride = { UE4CodeGen_Private::EPropertyClass::Struct, "SimulationSettingsOverride", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, SimulationSettingsOverride), Z_Construct_UScriptStruct_FPopcornFXSimulationSettings, METADATA_PARAMS(NewProp_SimulationSettingsOverride_MetaData, ARRAY_COUNT(NewProp_SimulationSettingsOverride_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FixedRelativeBoundingBox_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Fixed relative bounding box used if BoundingBoxMode is DynamicPlusFixed or Fixed." },
			};
#endif
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_FixedRelativeBoundingBox = { UE4CodeGen_Private::EPropertyClass::Struct, "FixedRelativeBoundingBox", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, FixedRelativeBoundingBox), Z_Construct_UScriptStruct_FBox, METADATA_PARAMS(NewProp_FixedRelativeBoundingBox_MetaData, ARRAY_COUNT(NewProp_FixedRelativeBoundingBox_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BoundingBoxMode_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Bounding Box Computing Mode." },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_BoundingBoxMode = { UE4CodeGen_Private::EPropertyClass::Byte, "BoundingBoxMode", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, BoundingBoxMode), Z_Construct_UEnum_PopcornFX_EPopcornFXSceneBBMode, METADATA_PARAMS(NewProp_BoundingBoxMode_MetaData, ARRAY_COUNT(NewProp_BoundingBoxMode_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableRender_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Enable PopcornFX Render for this scene." },
			};
#endif
			auto NewProp_bEnableRender_SetBit = [](void* Obj){ ((UPopcornFXSceneComponent*)Obj)->bEnableRender = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableRender = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableRender", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000200000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSceneComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableRender_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableRender_MetaData, ARRAY_COUNT(NewProp_bEnableRender_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableUpdate_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "Enable PopcornFX Updates for this scene." },
			};
#endif
			auto NewProp_bEnableUpdate_SetBit = [](void* Obj){ ((UPopcornFXSceneComponent*)Obj)->bEnableUpdate = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableUpdate = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableUpdate", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000200000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXSceneComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableUpdate_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableUpdate_MetaData, ARRAY_COUNT(NewProp_bEnableUpdate_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneName_MetaData[] = {
				{ "Category", "PopcornFX Scene" },
				{ "ModuleRelativePath", "Public/PopcornFXSceneComponent.h" },
				{ "ToolTip", "The SceneName used to indentify this PopcornFXScene\nPopcornFX Emitters will look for this SceneName when searching a PopcornFXScene to Spawn into." },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_SceneName = { UE4CodeGen_Private::EPropertyClass::Name, "SceneName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXSceneComponent, SceneName), METADATA_PARAMS(NewProp_SceneName_MetaData, ARRAY_COUNT(NewProp_SceneName_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Render_OverrideAllMaterial,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Render_OverrideDebugColorCount,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bRender_OverrideColorByDrawCall,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bRender_FreezeBillboardingMatrix,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_HeavyDebugMode,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FieldListeners,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FieldListeners_Inner,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_RenderSettingsOverride,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SimulationSettingsOverride,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_FixedRelativeBoundingBox,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_BoundingBoxMode,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableRender,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableUpdate,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SceneName,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXSceneComponent>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXSceneComponent::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXSceneComponent, 2650440081);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXSceneComponent(Z_Construct_UClass_UPopcornFXSceneComponent, &UPopcornFXSceneComponent::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXSceneComponent"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXSceneComponent);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
