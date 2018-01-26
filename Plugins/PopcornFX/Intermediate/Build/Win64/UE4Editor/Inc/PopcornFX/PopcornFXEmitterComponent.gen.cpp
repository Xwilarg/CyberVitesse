// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXEmitterComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXEmitterComponent() {}
// Cross Module References
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEmitterComponent();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_CopyAndStartEmitterAtLocation();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_CopyAndStartEmitterAttached();
	ENGINE_API UEnum* Z_Construct_UEnum_Engine_EAttachLocation();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterAlive();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterEmitting();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterStarted();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_ResetAttributesToDefault();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_RestartEmitter();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_SetEffect();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEffect_NoRegister();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_StartEmitter();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_StopEmitter();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_TerminateEmitter();
	POPCORNFX_API UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_ToggleEmitter();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXSceneActor_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeList_NoRegister();
// End Cross Module References
	void UPopcornFXEmitterComponent::StaticRegisterNativesUPopcornFXEmitterComponent()
	{
		UClass* Class = UPopcornFXEmitterComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CopyAndStartEmitterAtLocation", (Native)&UPopcornFXEmitterComponent::execCopyAndStartEmitterAtLocation },
			{ "CopyAndStartEmitterAttached", (Native)&UPopcornFXEmitterComponent::execCopyAndStartEmitterAttached },
			{ "IsEmitterAlive", (Native)&UPopcornFXEmitterComponent::execIsEmitterAlive },
			{ "IsEmitterEmitting", (Native)&UPopcornFXEmitterComponent::execIsEmitterEmitting },
			{ "IsEmitterStarted", (Native)&UPopcornFXEmitterComponent::execIsEmitterStarted },
			{ "ResetAttributesToDefault", (Native)&UPopcornFXEmitterComponent::execResetAttributesToDefault },
			{ "RestartEmitter", (Native)&UPopcornFXEmitterComponent::execRestartEmitter },
			{ "SetEffect", (Native)&UPopcornFXEmitterComponent::execSetEffect },
			{ "StartEmitter", (Native)&UPopcornFXEmitterComponent::execStartEmitter },
			{ "StopEmitter", (Native)&UPopcornFXEmitterComponent::execStopEmitter },
			{ "TerminateEmitter", (Native)&UPopcornFXEmitterComponent::execTerminateEmitter },
			{ "ToggleEmitter", (Native)&UPopcornFXEmitterComponent::execToggleEmitter },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_CopyAndStartEmitterAtLocation()
	{
		struct PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms
		{
			FVector Location;
			FRotator Rotation;
			bool bAutoDestroy;
			UPopcornFXEmitterComponent* ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
				{ "EditInline", "true" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Object, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000080588, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms, ReturnValue), Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister, METADATA_PARAMS(NewProp_ReturnValue_MetaData, ARRAY_COUNT(NewProp_ReturnValue_MetaData)) };
			auto NewProp_bAutoDestroy_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms*)Obj)->bAutoDestroy = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAutoDestroy = { UE4CodeGen_Private::EPropertyClass::Bool, "bAutoDestroy", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bAutoDestroy_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Rotation = { UE4CodeGen_Private::EPropertyClass::Struct, "Rotation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms, Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Location = { UE4CodeGen_Private::EPropertyClass::Struct, "Location", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bAutoDestroy,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Rotation,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Location,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "bAutoDestroy", "true" },
				{ "Category", "PopcornFX|Emitter" },
				{ "CPP_Default_bAutoDestroy", "true" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Copies himself with current attributes values then\nPlays the specified PopcornFXEffect at the given location and rotation, fire and forget.\nThe system will go away when the emitter is complete. Does not replicate.\n@param Location - location to place the emitter in world space\n@param Rotation - rotation to place the emitter in world space" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "CopyAndStartEmitterAtLocation", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04820401, sizeof(PopcornFXEmitterComponent_eventCopyAndStartEmitterAtLocation_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_CopyAndStartEmitterAttached()
	{
		struct PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms
		{
			USceneComponent* AttachToComponent;
			FName AttachPointName;
			FVector Location;
			FRotator Rotation;
			TEnumAsByte<EAttachLocation::Type> LocationType;
			bool bAutoDestroy;
			UPopcornFXEmitterComponent* ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
				{ "EditInline", "true" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Object, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000080588, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms, ReturnValue), Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister, METADATA_PARAMS(NewProp_ReturnValue_MetaData, ARRAY_COUNT(NewProp_ReturnValue_MetaData)) };
			auto NewProp_bAutoDestroy_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms*)Obj)->bAutoDestroy = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAutoDestroy = { UE4CodeGen_Private::EPropertyClass::Bool, "bAutoDestroy", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bAutoDestroy_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_LocationType = { UE4CodeGen_Private::EPropertyClass::Byte, "LocationType", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms, LocationType), Z_Construct_UEnum_Engine_EAttachLocation, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Rotation = { UE4CodeGen_Private::EPropertyClass::Struct, "Rotation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms, Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Location = { UE4CodeGen_Private::EPropertyClass::Struct, "Location", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_AttachPointName = { UE4CodeGen_Private::EPropertyClass::Name, "AttachPointName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms, AttachPointName), METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AttachToComponent_MetaData[] = {
				{ "EditInline", "true" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_AttachToComponent = { UE4CodeGen_Private::EPropertyClass::Object, "AttachToComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000080080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms, AttachToComponent), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(NewProp_AttachToComponent_MetaData, ARRAY_COUNT(NewProp_AttachToComponent_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bAutoDestroy,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_LocationType,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Rotation,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Location,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AttachPointName,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AttachToComponent,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "bAutoDestroy", "true" },
				{ "Category", "PopcornFX|Emitter" },
				{ "CPP_Default_AttachPointName", "None" },
				{ "CPP_Default_bAutoDestroy", "true" },
				{ "CPP_Default_LocationType", "KeepRelativeOffset" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Copies himself with current attributes values then\nPlays the specified PopcornFXEffect attached to and following the specified component.\nThe system will go away when the emitter is complete. Does not replicate.\n@param AttachComponent - Component to attach to.\n@param AttachPointName - Optional named point within the AttachComponent to spawn the emitter at\n@param Location - Depending on the value of Location Type this is either a relative offset from the attach component/point or an absolute world position that will be translated to a relative offset\n@param Rotation - Depending on the value of LocationType this is either a relative offset from the attach component/point or an absolute world rotation that will be translated to a realative offset\n@param LocationType - Specifies whether Location is a relative offset or an absolute world position" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "CopyAndStartEmitterAttached", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04820401, sizeof(PopcornFXEmitterComponent_eventCopyAndStartEmitterAttached_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterAlive()
	{
		struct PopcornFXEmitterComponent_eventIsEmitterAlive_Parms
		{
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventIsEmitterAlive_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventIsEmitterAlive_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Alias of IsEmitterStarted()" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "IsEmitterAlive", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x54020401, sizeof(PopcornFXEmitterComponent_eventIsEmitterAlive_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterEmitting()
	{
		struct PopcornFXEmitterComponent_eventIsEmitterEmitting_Parms
		{
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventIsEmitterEmitting_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventIsEmitterEmitting_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Get whether the emitter is still emitting particles or not" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "IsEmitterEmitting", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x54020401, sizeof(PopcornFXEmitterComponent_eventIsEmitterEmitting_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterStarted()
	{
		struct PopcornFXEmitterComponent_eventIsEmitterStarted_Parms
		{
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventIsEmitterStarted_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventIsEmitterStarted_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Get whether the emitter is still alive or not.\nAn emitter could have stop emitting particles, but can still be \"Started\" (\"Alive\") if emitted particles still needs the emitter to be alive" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "IsEmitterStarted", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x54020401, sizeof(PopcornFXEmitterComponent_eventIsEmitterStarted_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_ResetAttributesToDefault()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Attributes" },
				{ "Keywords", "popcornfx particle emitter effect system attribute" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Reset all attribute to their default values" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "ResetAttributesToDefault", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_RestartEmitter()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "TerminateEmitter() then StartEmitter()" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "RestartEmitter", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_SetEffect()
	{
		struct PopcornFXEmitterComponent_eventSetEffect_Parms
		{
			UPopcornFXEffect* InEffect;
			bool bStartEmitter;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventSetEffect_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventSetEffect_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			auto NewProp_bStartEmitter_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventSetEffect_Parms*)Obj)->bStartEmitter = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bStartEmitter = { UE4CodeGen_Private::EPropertyClass::Bool, "bStartEmitter", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventSetEffect_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bStartEmitter_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InEffect = { UE4CodeGen_Private::EPropertyClass::Object, "InEffect", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(PopcornFXEmitterComponent_eventSetEffect_Parms, InEffect), Z_Construct_UClass_UPopcornFXEffect_NoRegister, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bStartEmitter,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InEffect,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "bStartEmitter", "false" },
				{ "Category", "PopcornFX|Emitter" },
				{ "CPP_Default_bStartEmitter", "false" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Terminate the current emitter, set the effect to @InEffect, reset attributes, and StartEmitter if @bStartEmitter." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "SetEffect", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXEmitterComponent_eventSetEffect_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_StartEmitter()
	{
		struct PopcornFXEmitterComponent_eventStartEmitter_Parms
		{
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventStartEmitter_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventStartEmitter_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Starts the emitter if not yet started\n@return true if the emitter has been started" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "StartEmitter", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXEmitterComponent_eventStartEmitter_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_StopEmitter()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Stops particle emission if emitting (the emitter can still be alive after that, see \"IsEmitterStarted\")" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "StopEmitter", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_TerminateEmitter()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Stops emission and unregister particle instance (will unregister attached locations and attributes)" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "TerminateEmitter", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, 0, nullptr, 0, 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UPopcornFXEmitterComponent_ToggleEmitter()
	{
		struct PopcornFXEmitterComponent_eventToggleEmitter_Parms
		{
			bool startEmitter;
			bool ReturnValue;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_ReturnValue_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventToggleEmitter_Parms*)Obj)->ReturnValue = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue = { UE4CodeGen_Private::EPropertyClass::Bool, "ReturnValue", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000580, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventToggleEmitter_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_ReturnValue_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			auto NewProp_startEmitter_SetBit = [](void* Obj){ ((PopcornFXEmitterComponent_eventToggleEmitter_Parms*)Obj)->startEmitter = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_startEmitter = { UE4CodeGen_Private::EPropertyClass::Bool, "startEmitter", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitterComponent_eventToggleEmitter_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_startEmitter_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_ReturnValue,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_startEmitter,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "PopcornFX|Emitter" },
				{ "Keywords", "popcornfx particle emitter effect system" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "if @startEmitter StartEmitter(), else StopEmitter(), returns IsEmitterStarted()" },
				{ "UnsafeDuringActorConstruction", "true" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UPopcornFXEmitterComponent, "ToggleEmitter", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXEmitterComponent_eventToggleEmitter_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister()
	{
		return UPopcornFXEmitterComponent::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXEmitterComponent()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_USceneComponent,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_CopyAndStartEmitterAtLocation, "CopyAndStartEmitterAtLocation" }, // 3848640757
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_CopyAndStartEmitterAttached, "CopyAndStartEmitterAttached" }, // 1293688110
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterAlive, "IsEmitterAlive" }, // 3157382318
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterEmitting, "IsEmitterEmitting" }, // 4050478495
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_IsEmitterStarted, "IsEmitterStarted" }, // 764645290
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_ResetAttributesToDefault, "ResetAttributesToDefault" }, // 2201609326
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_RestartEmitter, "RestartEmitter" }, // 3290135341
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_SetEffect, "SetEffect" }, // 1357964738
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_StartEmitter, "StartEmitter" }, // 2770352824
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_StopEmitter, "StopEmitter" }, // 3123383359
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_TerminateEmitter, "TerminateEmitter" }, // 1880117796
				{ &Z_Construct_UFunction_UPopcornFXEmitterComponent_ToggleEmitter, "ToggleEmitter" }, // 1025371510
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Object LOD Physics Collision Activation Components|Activation Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXEmitterComponent.h" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Instanciate and Emits and PopcornFX Effect into a PopcornFX Scene." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmissionStops_MetaData[] = {
				{ "Category", "PopcornFX Events" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Event called when the particle emission stops" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmissionStops = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmissionStops", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, OnEmissionStops), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmissionStops_MetaData, ARRAY_COUNT(NewProp_OnEmissionStops_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmitterTerminate_MetaData[] = {
				{ "Category", "PopcornFX Events" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Event called when TerminateEmitter() is called" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmitterTerminate = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmitterTerminate", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, OnEmitterTerminate), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmitterTerminate_MetaData, ARRAY_COUNT(NewProp_OnEmitterTerminate_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmitterStop_MetaData[] = {
				{ "Category", "PopcornFX Events" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Event called when StopEmitter() is called" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmitterStop = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmitterStop", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, OnEmitterStop), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmitterStop_MetaData, ARRAY_COUNT(NewProp_OnEmitterStop_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmitterStart_MetaData[] = {
				{ "Category", "PopcornFX Events" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "Event called when StartEmitter() is called" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmitterStart = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmitterStart", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, OnEmitterStart), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmitterStart_MetaData, ARRAY_COUNT(NewProp_OnEmitterStart_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Scene_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Scene = { UE4CodeGen_Private::EPropertyClass::Object, "Scene", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000002014, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, Scene), Z_Construct_UClass_APopcornFXSceneActor_NoRegister, METADATA_PARAMS(NewProp_Scene_MetaData, ARRAY_COUNT(NewProp_Scene_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SceneName_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "The name of the PopcornFXSceneComponent.SceneName to register for update and render" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_SceneName = { UE4CodeGen_Private::EPropertyClass::Name, "SceneName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, SceneName), METADATA_PARAMS(NewProp_SceneName_MetaData, ARRAY_COUNT(NewProp_SceneName_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bAllowTeleport_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "If true, the Emitter will skip transformations when teleported.\nSo, continuous spawners will not spawn in-between teleportation.\nBut, it will break Localspace Evolvers !" },
			};
#endif
			auto NewProp_bAllowTeleport_SetBit = [](void* Obj){ ((UPopcornFXEmitterComponent*)Obj)->bAllowTeleport = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAllowTeleport = { UE4CodeGen_Private::EPropertyClass::Bool, "bAllowTeleport", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXEmitterComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bAllowTeleport_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bAllowTeleport_MetaData, ARRAY_COUNT(NewProp_bAllowTeleport_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bPlayOnLoad_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "If true, the Emitter will be automaticaly started when spawned" },
			};
#endif
			auto NewProp_bPlayOnLoad_SetBit = [](void* Obj){ ((UPopcornFXEmitterComponent*)Obj)->bPlayOnLoad = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bPlayOnLoad = { UE4CodeGen_Private::EPropertyClass::Bool, "bPlayOnLoad", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXEmitterComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bPlayOnLoad_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bPlayOnLoad_MetaData, ARRAY_COUNT(NewProp_bPlayOnLoad_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableUpdates_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "If false, the emitter will not update the emission position neither attirubtues" },
			};
#endif
			auto NewProp_bEnableUpdates_SetBit = [](void* Obj){ ((UPopcornFXEmitterComponent*)Obj)->bEnableUpdates = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableUpdates = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnableUpdates", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXEmitterComponent), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnableUpdates_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEnableUpdates_MetaData, ARRAY_COUNT(NewProp_bEnableUpdates_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Effect_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "DisplayThumbnail", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Effect = { UE4CodeGen_Private::EPropertyClass::Object, "Effect", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, Effect), Z_Construct_UClass_UPopcornFXEffect_NoRegister, METADATA_PARAMS(NewProp_Effect_MetaData, ARRAY_COUNT(NewProp_Effect_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AttributeList_MetaData[] = {
				{ "Category", "PopcornFX Attributes" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitterComponent.h" },
				{ "ToolTip", "dont access it direclty, use GetAttributeList() instead" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_AttributeList = { UE4CodeGen_Private::EPropertyClass::Object, "AttributeList", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120000000a001d, 1, nullptr, STRUCT_OFFSET(UPopcornFXEmitterComponent, AttributeList), Z_Construct_UClass_UPopcornFXAttributeList_NoRegister, METADATA_PARAMS(NewProp_AttributeList_MetaData, ARRAY_COUNT(NewProp_AttributeList_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmissionStops,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmitterTerminate,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmitterStop,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmitterStart,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Scene,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SceneName,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bAllowTeleport,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bPlayOnLoad,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnableUpdates,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Effect,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AttributeList,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXEmitterComponent>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXEmitterComponent::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXEmitterComponent, 2701739337);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXEmitterComponent(Z_Construct_UClass_UPopcornFXEmitterComponent, &UPopcornFXEmitterComponent::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXEmitterComponent"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXEmitterComponent);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
