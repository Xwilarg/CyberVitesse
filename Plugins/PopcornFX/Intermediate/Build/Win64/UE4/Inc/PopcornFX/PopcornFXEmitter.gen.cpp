// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXEmitter.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXEmitter() {}
// Cross Module References
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXEmitter_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_APopcornFXEmitter();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	POPCORNFX_API UFunction* Z_Construct_UFunction_APopcornFXEmitter_OnInterpToggle();
	ENGINE_API UClass* Z_Construct_UClass_UBillboardComponent_NoRegister();
// End Cross Module References
	UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature()
	{
		struct _Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms
		{
			UPopcornFXEmitterComponent* EmitterComponent;
			FVector Location;
			FVector Rotation;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Rotation = { UE4CodeGen_Private::EPropertyClass::Struct, "Rotation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms, Rotation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Location = { UE4CodeGen_Private::EPropertyClass::Struct, "Location", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EmitterComponent_MetaData[] = {
				{ "EditInline", "true" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_EmitterComponent = { UE4CodeGen_Private::EPropertyClass::Object, "EmitterComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000080080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms, EmitterComponent), Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister, METADATA_PARAMS(NewProp_EmitterComponent_MetaData, ARRAY_COUNT(NewProp_EmitterComponent_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Rotation,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Location,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EmitterComponent,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
				{ "ToolTip", "Fires when a PopcornFX Emitter is terminated\n@param EmitterComponent - The emitter component.\n@param Location - Location.\n@param Rotation - Rotation." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_PopcornFX, "PopcornFXEmitterTerminateSignature__DelegateSignature", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00130000, sizeof(_Script_PopcornFX_eventPopcornFXEmitterTerminateSignature_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature()
	{
		struct _Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms
		{
			UPopcornFXEmitterComponent* EmitterComponent;
			FVector Location;
			FVector Rotation;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Rotation = { UE4CodeGen_Private::EPropertyClass::Struct, "Rotation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms, Rotation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Location = { UE4CodeGen_Private::EPropertyClass::Struct, "Location", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EmitterComponent_MetaData[] = {
				{ "EditInline", "true" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_EmitterComponent = { UE4CodeGen_Private::EPropertyClass::Object, "EmitterComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000080080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms, EmitterComponent), Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister, METADATA_PARAMS(NewProp_EmitterComponent_MetaData, ARRAY_COUNT(NewProp_EmitterComponent_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Rotation,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Location,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EmitterComponent,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
				{ "ToolTip", "Fires when a PopcornFX Emitter is stop\nNote: could happen after Terminate\n@param EmitterComponent - The emitter component.\n@param Location - Location.\n@param Rotation - Rotation." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_PopcornFX, "PopcornFXEmitterStopSignature__DelegateSignature", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00130000, sizeof(_Script_PopcornFX_eventPopcornFXEmitterStopSignature_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature()
	{
		struct _Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms
		{
			UPopcornFXEmitterComponent* EmitterComponent;
			FVector Location;
			FVector Rotation;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Rotation = { UE4CodeGen_Private::EPropertyClass::Struct, "Rotation", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms, Rotation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FStructPropertyParams NewProp_Location = { UE4CodeGen_Private::EPropertyClass::Struct, "Location", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EmitterComponent_MetaData[] = {
				{ "EditInline", "true" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_EmitterComponent = { UE4CodeGen_Private::EPropertyClass::Object, "EmitterComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000080080, 1, nullptr, STRUCT_OFFSET(_Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms, EmitterComponent), Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister, METADATA_PARAMS(NewProp_EmitterComponent_MetaData, ARRAY_COUNT(NewProp_EmitterComponent_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Rotation,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Location,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_EmitterComponent,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
				{ "ToolTip", "Fires when a PopcornFX Emitter is started\n@param EmitterComponent - The emitter component.\n@param Location - Location.\n@param Rotation - Rotation." },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_PopcornFX, "PopcornFXEmitterStartSignature__DelegateSignature", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x00130000, sizeof(_Script_PopcornFX_eventPopcornFXEmitterStartSignature_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	void APopcornFXEmitter::StaticRegisterNativesAPopcornFXEmitter()
	{
		UClass* Class = APopcornFXEmitter::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "OnInterpToggle", (Native)&APopcornFXEmitter::execOnInterpToggle },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_APopcornFXEmitter_OnInterpToggle()
	{
		struct PopcornFXEmitter_eventOnInterpToggle_Parms
		{
			bool bEnable;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			auto NewProp_bEnable_SetBit = [](void* Obj){ ((PopcornFXEmitter_eventOnInterpToggle_Parms*)Obj)->bEnable = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnable = { UE4CodeGen_Private::EPropertyClass::Bool, "bEnable", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(PopcornFXEmitter_eventOnInterpToggle_Parms), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEnable_SetBit)>::SetBit, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEnable,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "CallInEditor", "TRUE" },
				{ "Category", "Rendering" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
				{ "ToolTip", "Matinee Toggle anim track compatibility" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_APopcornFXEmitter, "OnInterpToggle", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(PopcornFXEmitter_eventOnInterpToggle_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_APopcornFXEmitter_NoRegister()
	{
		return APopcornFXEmitter::StaticClass();
	}
	UClass* Z_Construct_UClass_APopcornFXEmitter()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_AActor,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_APopcornFXEmitter_OnInterpToggle, "OnInterpToggle" }, // 4216843633
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Input Collision Replication" },
				{ "IncludePath", "PopcornFXEmitter.h" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SpriteComponent_MetaData[] = {
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SpriteComponent = { UE4CodeGen_Private::EPropertyClass::Object, "SpriteComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800080008, 1, nullptr, STRUCT_OFFSET(APopcornFXEmitter, SpriteComponent), Z_Construct_UClass_UBillboardComponent_NoRegister, METADATA_PARAMS(NewProp_SpriteComponent_MetaData, ARRAY_COUNT(NewProp_SpriteComponent_MetaData)) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmitterTerminate_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmitterTerminate = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmitterTerminate", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(APopcornFXEmitter, OnEmitterTerminate), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmitterTerminate_MetaData, ARRAY_COUNT(NewProp_OnEmitterTerminate_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmitterStop_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmitterStop = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmitterStop", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(APopcornFXEmitter, OnEmitterStop), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmitterStop_MetaData, ARRAY_COUNT(NewProp_OnEmitterStop_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnEmitterStart_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
			};
#endif
			static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnEmitterStart = { UE4CodeGen_Private::EPropertyClass::MulticastDelegate, "OnEmitterStart", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000010080000, 1, nullptr, STRUCT_OFFSET(APopcornFXEmitter, OnEmitterStart), Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature, METADATA_PARAMS(NewProp_OnEmitterStart_MetaData, ARRAY_COUNT(NewProp_OnEmitterStart_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PopcornFXEmitterComponent_MetaData[] = {
				{ "Category", "PopcornFX Emitter" },
				{ "EditInline", "true" },
				{ "ModuleRelativePath", "Public/PopcornFXEmitter.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_PopcornFXEmitterComponent = { UE4CodeGen_Private::EPropertyClass::Object, "PopcornFXEmitterComponent", RF_Public|RF_Transient|RF_MarkAsNative, 0x00120000000a001d, 1, nullptr, STRUCT_OFFSET(APopcornFXEmitter, PopcornFXEmitterComponent), Z_Construct_UClass_UPopcornFXEmitterComponent_NoRegister, METADATA_PARAMS(NewProp_PopcornFXEmitterComponent_MetaData, ARRAY_COUNT(NewProp_PopcornFXEmitterComponent_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SpriteComponent,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmitterTerminate,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmitterStop,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_OnEmitterStart,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_PopcornFXEmitterComponent,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<APopcornFXEmitter>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&APopcornFXEmitter::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00900080u,
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
	IMPLEMENT_CLASS(APopcornFXEmitter, 1557059093);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APopcornFXEmitter(Z_Construct_UClass_APopcornFXEmitter, &APopcornFXEmitter::StaticClass, TEXT("/Script/PopcornFX"), TEXT("APopcornFXEmitter"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APopcornFXEmitter);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
