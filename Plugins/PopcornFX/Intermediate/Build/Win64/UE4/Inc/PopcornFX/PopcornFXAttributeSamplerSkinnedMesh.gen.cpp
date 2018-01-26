// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/PopcornFXAttributeSamplerSkinnedMesh.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePopcornFXAttributeSamplerSkinnedMesh() {}
// Cross Module References
	POPCORNFX_API UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXSkinnedTransforms();
	UPackage* Z_Construct_UPackage__Script_PopcornFX();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerSkinnedMesh_NoRegister();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerSkinnedMesh();
	POPCORNFX_API UClass* Z_Construct_UClass_UPopcornFXAttributeSampler();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
// End Cross Module References
	static UEnum* EPopcornFXSkinnedTransforms_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_PopcornFX_EPopcornFXSkinnedTransforms, Z_Construct_UPackage__Script_PopcornFX(), TEXT("EPopcornFXSkinnedTransforms"));
		}
		return Singleton;
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EPopcornFXSkinnedTransforms(EPopcornFXSkinnedTransforms_StaticEnum, TEXT("/Script/PopcornFX"), TEXT("EPopcornFXSkinnedTransforms"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_PopcornFX_EPopcornFXSkinnedTransforms_CRC() { return 1124392094U; }
	UEnum* Z_Construct_UEnum_PopcornFX_EPopcornFXSkinnedTransforms()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_PopcornFX();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EPopcornFXSkinnedTransforms"), 0, Get_Z_Construct_UEnum_PopcornFX_EPopcornFXSkinnedTransforms_CRC(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EPopcornFXSkinnedTransforms::SkinnedComponentRelativeTr", (int64)EPopcornFXSkinnedTransforms::SkinnedComponentRelativeTr },
				{ "EPopcornFXSkinnedTransforms::SkinnedComponentWorldTr", (int64)EPopcornFXSkinnedTransforms::SkinnedComponentWorldTr },
				{ "EPopcornFXSkinnedTransforms::AttrSamplerRelativeTr", (int64)EPopcornFXSkinnedTransforms::AttrSamplerRelativeTr },
				{ "EPopcornFXSkinnedTransforms::AttrSamplerWorldTr", (int64)EPopcornFXSkinnedTransforms::AttrSamplerWorldTr },
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "AttrSamplerRelativeTr.ToolTip", "Use Attribute Sampler Actor local transforms relative to it's parent actor" },
				{ "AttrSamplerWorldTr.ToolTip", "Use Attribute Sampler Actor world transforms" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "SkinnedComponentRelativeTr.ToolTip", "Use Skinned Mesh Actor local transforms relative to it's parent actor" },
				{ "SkinnedComponentWorldTr.ToolTip", "Use Skinned Mesh Actor world transforms" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_PopcornFX,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				"EPopcornFXSkinnedTransforms",
				RF_Public|RF_Transient|RF_MarkAsNative,
				nullptr,
				(uint8)UEnum::ECppForm::Namespaced,
				"EPopcornFXSkinnedTransforms::Type",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void UPopcornFXAttributeSamplerSkinnedMesh::StaticRegisterNativesUPopcornFXAttributeSamplerSkinnedMesh()
	{
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerSkinnedMesh_NoRegister()
	{
		return UPopcornFXAttributeSamplerSkinnedMesh::StaticClass();
	}
	UClass* Z_Construct_UClass_UPopcornFXAttributeSamplerSkinnedMesh()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UPopcornFXAttributeSampler,
				(UObject* (*)())Z_Construct_UPackage__Script_PopcornFX,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "BlueprintSpawnableComponent", "" },
				{ "ClassGroupNames", "PopcornFX" },
				{ "HideCategories", "Trigger PhysicsVolume" },
				{ "IncludePath", "PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Can override an Attribute Sampler **Shape** in a **USkeletalMesh**." },
			};
#endif
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEditorBuildInitialPose_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
			};
#endif
			auto NewProp_bEditorBuildInitialPose_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bEditorBuildInitialPose = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEditorBuildInitialPose = { UE4CodeGen_Private::EPropertyClass::Bool, "bEditorBuildInitialPose", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000800000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bEditorBuildInitialPose_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bEditorBuildInitialPose_MetaData, ARRAY_COUNT(NewProp_bEditorBuildInitialPose_MetaData)) };
#endif // WITH_EDITORONLY_DATA
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bUseSkinnedMeshTransforms_MetaData[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
			};
#endif
			auto NewProp_bUseSkinnedMeshTransforms_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bUseSkinnedMeshTransforms_DEPRECATED = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUseSkinnedMeshTransforms = { UE4CodeGen_Private::EPropertyClass::Bool, "bUseSkinnedMeshTransforms", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000020000000, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bUseSkinnedMeshTransforms_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bUseSkinnedMeshTransforms_MetaData, ARRAY_COUNT(NewProp_bUseSkinnedMeshTransforms_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bUseRelativeTransform_MetaData[] = {
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
			};
#endif
			auto NewProp_bUseRelativeTransform_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bUseRelativeTransform_DEPRECATED = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUseRelativeTransform = { UE4CodeGen_Private::EPropertyClass::Bool, "bUseRelativeTransform", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000020000000, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bUseRelativeTransform_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bUseRelativeTransform_MetaData, ARRAY_COUNT(NewProp_bUseRelativeTransform_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bApplyScale_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to use scaled transforms" },
			};
#endif
			auto NewProp_bApplyScale_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bApplyScale = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bApplyScale = { UE4CodeGen_Private::EPropertyClass::Bool, "bApplyScale", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bApplyScale_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bApplyScale_MetaData, ARRAY_COUNT(NewProp_bApplyScale_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Transforms_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Determines what transforms will be used for this attribute sampler" },
			};
#endif
			static const UE4CodeGen_Private::FBytePropertyParams NewProp_Transforms = { UE4CodeGen_Private::EPropertyClass::Byte, "Transforms", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerSkinnedMesh, Transforms), Z_Construct_UEnum_PopcornFX_EPopcornFXSkinnedTransforms, METADATA_PARAMS(NewProp_Transforms_MetaData, ARRAY_COUNT(NewProp_Transforms_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bBuildClothData_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to use the simulated cloth positions/normals (tangents unavailable)." },
			};
#endif
			auto NewProp_bBuildClothData_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bBuildClothData = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bBuildClothData = { UE4CodeGen_Private::EPropertyClass::Bool, "bBuildClothData", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bBuildClothData_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bBuildClothData_MetaData, ARRAY_COUNT(NewProp_bBuildClothData_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bComputeVelocities_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to access this skinned mesh's Velocities" },
			};
#endif
			auto NewProp_bComputeVelocities_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bComputeVelocities = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bComputeVelocities = { UE4CodeGen_Private::EPropertyClass::Bool, "bComputeVelocities", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bComputeVelocities_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bComputeVelocities_MetaData, ARRAY_COUNT(NewProp_bComputeVelocities_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bBuildUVs_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to access this skinned mesh's UVs" },
			};
#endif
			auto NewProp_bBuildUVs_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bBuildUVs = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bBuildUVs = { UE4CodeGen_Private::EPropertyClass::Bool, "bBuildUVs", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bBuildUVs_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bBuildUVs_MetaData, ARRAY_COUNT(NewProp_bBuildUVs_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bBuildColors_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to access this skinned mesh's Colors" },
			};
#endif
			auto NewProp_bBuildColors_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bBuildColors = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bBuildColors = { UE4CodeGen_Private::EPropertyClass::Bool, "bBuildColors", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bBuildColors_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bBuildColors_MetaData, ARRAY_COUNT(NewProp_bBuildColors_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bSkinTangents_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to access this skinned mesh's Tangents" },
			};
#endif
			auto NewProp_bSkinTangents_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bSkinTangents = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSkinTangents = { UE4CodeGen_Private::EPropertyClass::Bool, "bSkinTangents", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bSkinTangents_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bSkinTangents_MetaData, ARRAY_COUNT(NewProp_bSkinTangents_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bSkinNormals_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to access this skinned mesh's Normals" },
			};
#endif
			auto NewProp_bSkinNormals_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bSkinNormals = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSkinNormals = { UE4CodeGen_Private::EPropertyClass::Bool, "bSkinNormals", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bSkinNormals_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bSkinNormals_MetaData, ARRAY_COUNT(NewProp_bSkinNormals_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bSkinPositions_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to access this skinned mesh's Positions" },
			};
#endif
			auto NewProp_bSkinPositions_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bSkinPositions = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSkinPositions = { UE4CodeGen_Private::EPropertyClass::Bool, "bSkinPositions", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000001, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bSkinPositions_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bSkinPositions_MetaData, ARRAY_COUNT(NewProp_bSkinPositions_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bPauseSkinning_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Enable this if you want to pause the CPU Skinning" },
			};
#endif
			auto NewProp_bPauseSkinning_SetBit = [](void* Obj){ ((UPopcornFXAttributeSamplerSkinnedMesh*)Obj)->bPauseSkinning = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bPauseSkinning = { UE4CodeGen_Private::EPropertyClass::Bool, "bPauseSkinning", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, sizeof(uint8), UE4CodeGen_Private::ENativeBool::NotNative, sizeof(UPopcornFXAttributeSamplerSkinnedMesh), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_bPauseSkinning_SetBit)>::SetBit, METADATA_PARAMS(NewProp_bPauseSkinning_MetaData, ARRAY_COUNT(NewProp_bPauseSkinning_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SkinnedMeshComponentName_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Use this property to specify the target skinned component name:\n- If TargetActor is specified, looks for a skinned mesh component that has this name in TargetActor\n- If TargetActor is specified, but no skinned mesh component has this name, fallbacks to TargetActor's RootComponent\n- If TargetActor isn't specified, looks for a skinned mesh component that has this name in this actor\n- If TargetActor isn't specified, but skinned mesh component has this name, fallbacks to this actor's RootComponent" },
			};
#endif
			static const UE4CodeGen_Private::FNamePropertyParams NewProp_SkinnedMeshComponentName = { UE4CodeGen_Private::EPropertyClass::Name, "SkinnedMeshComponentName", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerSkinnedMesh, SkinnedMeshComponentName), METADATA_PARAMS(NewProp_SkinnedMeshComponentName_MetaData, ARRAY_COUNT(NewProp_SkinnedMeshComponentName_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TargetActor_MetaData[] = {
				{ "Category", "PopcornFX AttributeSampler" },
				{ "ModuleRelativePath", "Public/PopcornFXAttributeSamplerSkinnedMesh.h" },
				{ "ToolTip", "Specifies which actors contains the target SkinnedMeshComponent" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TargetActor = { UE4CodeGen_Private::EPropertyClass::Object, "TargetActor", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000005, 1, nullptr, STRUCT_OFFSET(UPopcornFXAttributeSamplerSkinnedMesh, TargetActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(NewProp_TargetActor_MetaData, ARRAY_COUNT(NewProp_TargetActor_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
#if WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bEditorBuildInitialPose,
#endif // WITH_EDITORONLY_DATA
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bUseSkinnedMeshTransforms,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bUseRelativeTransform,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bApplyScale,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_Transforms,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bBuildClothData,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bComputeVelocities,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bBuildUVs,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bBuildColors,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bSkinTangents,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bSkinNormals,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bSkinPositions,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_bPauseSkinning,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_SkinnedMeshComponentName,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_TargetActor,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UPopcornFXAttributeSamplerSkinnedMesh>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UPopcornFXAttributeSamplerSkinnedMesh::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00B01080u,
				nullptr, 0,
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
	IMPLEMENT_CLASS(UPopcornFXAttributeSamplerSkinnedMesh, 1915877249);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPopcornFXAttributeSamplerSkinnedMesh(Z_Construct_UClass_UPopcornFXAttributeSamplerSkinnedMesh, &UPopcornFXAttributeSamplerSkinnedMesh::StaticClass, TEXT("/Script/PopcornFX"), TEXT("UPopcornFXAttributeSamplerSkinnedMesh"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPopcornFXAttributeSamplerSkinnedMesh);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
