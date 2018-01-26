// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Nodes/PopcornFXNode_GetAttribute.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXNode_GetAttribute() {}
// Cross Module References
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_GetAttribute_NoRegister();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_GetAttribute();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_DynamicField();
	UPackage* Z_Construct_UPackage__Script_PopcornFXEditor();
// End Cross Module References
	void UPopcornFXNode_GetAttribute::StaticRegisterNativesUPopcornFXNode_GetAttribute()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXNode_GetAttribute_NoRegister()
	{
		return UPopcornFXNode_GetAttribute::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXNode_GetAttribute()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXNode_DynamicField,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFXEditor,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "Nodes/PopcornFXNode_GetAttribute.h" },
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_GetAttribute.h" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXNode_GetAttribute>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXNode_GetAttribute::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00080080u,
				nullptr, 0,
				nullptr, 0,
				nullptr,
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPopcornFXNode_GetAttribute, 782152108);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXNode_GetAttribute(Z_Construct_UClass_UPopcornFXNode_GetAttribute, &UPopcornFXNode_GetAttribute::StaticClass, TEXT("/Script/PopcornFXEditor"), TEXT("UPopcornFXNode_GetAttribute"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXNode_GetAttribute);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
