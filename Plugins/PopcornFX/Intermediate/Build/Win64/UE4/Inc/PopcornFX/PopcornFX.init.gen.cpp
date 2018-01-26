// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFX_init() {}
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature();
	POPCORNFX_API UFunction* Z_Construct_UDelegateFunction_PopcornFX_PopcornFXRaiseEventSignature__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_PopcornFX()
	{
		static UPackage* ReturnPackage = nullptr;
		if (!ReturnPackage)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStartSignature__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterStopSignature__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_PopcornFX_PopcornFXEmitterTerminateSignature__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_PopcornFX_PopcornFXRaiseEventSignature__DelegateSignature,
			};
			static const UE4CodeGen_Private::FPackageParams PackageParams = {
				"/Script/PopcornFX",
				PKG_CompiledIn | 0x00000000,
				0x6965A7FE,
				0xA349C7CA,
				SingletonFuncArray, ARRAY_COUNT(SingletonFuncArray),
				METADATA_PARAMS(nullptr, 0)
			};
			UE4CodeGen_Private::ConstructUPackage(ReturnPackage, PackageParams);
		}
		return ReturnPackage;
	}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
