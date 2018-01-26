// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Private/Nodes/PopcornFXNode_SetAttribute.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXNode_SetAttribute() {}
// Cross Module References
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_SetAttribute_NoRegister();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_SetAttribute();
	POPCORNFXEDITOR_API UClass* Z_Construct_UClass_UPopcornFXNode_DynamicField();
	UPackage* Z_Construct_UPackage__Script_PopcornFXEditor();
// End Cross Module References
	void UPopcornFXNode_SetAttribute::StaticRegisterNativesUPopcornFXNode_SetAttribute()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXNode_SetAttribute_NoRegister()
	{
		return UPopcornFXNode_SetAttribute::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXNode_SetAttribute()
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
				{ "IncludePath", "Nodes/PopcornFXNode_SetAttribute.h" },
				{ "ModuleRelativePath", "Private/Nodes/PopcornFXNode_SetAttribute.h" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXNode_SetAttribute>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXNode_SetAttribute::StaticClass,
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
	IMPLEMENT_CLASS(UPopcornFXNode_SetAttribute, 2131410288);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXNode_SetAttribute(Z_Construct_UClass_UPopcornFXNode_SetAttribute, &UPopcornFXNode_SetAttribute::StaticClass, TEXT("/Script/PopcornFXEditor"), TEXT("UPopcornFXNode_SetAttribute"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXNode_SetAttribute);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
