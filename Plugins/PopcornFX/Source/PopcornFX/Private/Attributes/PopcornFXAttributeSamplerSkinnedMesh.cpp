//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXAttributeSamplerSkinnedMesh.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXStats.h"
#include "PopcornFXAttributeList.h"
#include "PopcornFXAttributeSampler.h"
#include "PopcornFXCustomVersion.h"

#include "Assets/ClothingAsset.h"

#include "Components/SkinnedMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#if (ENGINE_MINOR_VERSION >= 18)
// Don't include the DestructibleComponent implementation header,
// relies on ApexDestruction plugin which isn't loaded when PopcornFX runtime
// module is loaded. Include the interface instead.
// #	include "DestructibleComponent.h"
#	include "DestructibleInterface.h"
#else
#	include "Components/DestructibleComponent.h"
#endif	// (ENGINE_MINOR_VERSION >= 18)

#include "Engine/SkeletalMesh.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_samplers_classes.h>
#include <hh_geometrics/include/ge_mesh_resource.h>
#include <hh_geometrics/include/ge_mesh.h>
#include <hh_geometrics/include/ge_mesh_sampler_accel.h>
#include <hh_geometrics/include/ge_mesh_deformers_skin.h>
#include <hh_kernel/include/kr_profiler.h>
#include <hh_kernel/include/kr_containers_onstack.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXAttributeSamplerSkinned, Log, All);

#define	POPCORNFX_MAX_ANIM_IDLE_TIME	1.0f

struct FPopcornFXClothSection
{
	hh_u32							m_BaseVertexOffset;
	hh_u32							m_VertexCount;
	hh_u32							m_ClothDataIndex;
	PopcornFX::TArray<hh_u32>		m_Indices;
};

//----------------------------------------------------------------------------

struct FAttributeSamplerSkinnedMeshData
{
	bool	m_ShouldUpdateTransforms = false;
	bool	m_BoneVisibilityChanged = false;

	float	m_AccumulatedDts = 0.0f;

	PopcornFX::PMeshNew								m_Mesh;
	PopcornFX::PParticleSamplerDescriptor_Shape		m_Desc;

	PopcornFX::TArray<CFloat4, PopcornFX::TArrayAligned16>	m_DstPositions;
	PopcornFX::TArray<CFloat4, PopcornFX::TArrayAligned16>	m_DstNormals;
	PopcornFX::TArray<CFloat4, PopcornFX::TArrayAligned16>	m_DstTangents;

	PopcornFX::TArray<CFloat4, PopcornFX::TArrayAligned16>	m_OldPositions;
	PopcornFX::TArray<CFloat4, PopcornFX::TArrayAligned16>	m_DstVelocities;

	PopcornFX::TArray<float, PopcornFX::TArrayAligned16>	m_BoneWeights;
	PopcornFX::TArray<hh_u8, PopcornFX::TArrayAligned16>	m_BoneIndices;

	PopcornFX::TArray<CFloat4x4, PopcornFX::TArrayAligned16>	m_BoneInverseMatrices;

	PopcornFX::TArray<FPopcornFXClothSection>	m_ClothSections;
	TMap<int32, FClothSimulData>				m_ClothSimDataCopy;
	FMatrix										m_InverseTransforms;

	PopcornFX::TBaseSkinningStreamsProxy<hh_u8>	m_SkinningStreamsProxy;
	PopcornFX::SSkinContext						m_SkinContext;
	PopcornFX::CSkinAsyncContext				m_AsyncSkinContext;
	PopcornFX::CSkeletonView					*m_SkeletonView = null;

	PopcornFX::SSamplerSourceOverride				m_Override;

	PopcornFX::CDiscreteProbabilityFunction1D_O1::SWorkingBuffers	m_OverrideSurfaceSamplingWorkingBuffers;
	PopcornFX::CMeshSurfaceSamplerStructuresRandom					m_OverrideSurfaceSamplingAccelStructs;

	TWeakObjectPtr<USkinnedMeshComponent>		m_CurrentSkinnedMeshComponent = null;
};

//----------------------------------------------------------------------------

UPopcornFXAttributeSamplerSkinnedMesh::UPopcornFXAttributeSamplerSkinnedMesh(const FObjectInitializer &PCIP)
	: Super(PCIP)
	, m_LastFrameUpdate(0)
{
	bAutoActivate = true;

	// Default skinned mesh only builds positions
	bPauseSkinning = false;
	bSkinPositions = true;
	bSkinNormals = false;
	bSkinTangents = false;
	bBuildColors = false;
	bBuildUVs = false;
	bComputeVelocities = false;
	bBuildClothData = false;

	// We don't know the target yet, TickGroup will be set at ResolveSkinnedMeshComponent
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	// UPopcornFXAttributeSampler override:
	m_SamplerType = EPopcornFXAttributeSamplerType::Shape;

	TargetActor = null;
	bUseSkinnedMeshTransforms_DEPRECATED = false;
	bUseRelativeTransform_DEPRECATED = true;
	Transforms = EPopcornFXSkinnedTransforms::AttrSamplerRelativeTr;
	bApplyScale = false;
#if WITH_EDITORONLY_DATA
	bEditorBuildInitialPose = false;
#endif // WITH_EDITORONLY_DATA

	m_Data = new FAttributeSamplerSkinnedMeshData();
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::BeginDestroy()
{
	Super::BeginDestroy();
	if (m_Data != null)
	{
		Clear();
		delete m_Data;
		m_Data = null;
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::Clear()
{
	HH_ASSERT(m_Data != null);

	HH_DELETE(m_Data->m_SkeletonView);
	m_Data->m_SkeletonView = null;

	m_Data->m_DstPositions.Clear();
	m_Data->m_DstNormals.Clear();
	m_Data->m_DstTangents.Clear();
	m_Data->m_DstVelocities.Clear();
	m_Data->m_OldPositions.Clear();
	m_Data->m_BoneIndices.Clear();
	m_Data->m_BoneWeights.Clear();
	m_Data->m_BoneInverseMatrices.Clear();

	m_Data->m_ClothSections.Clear();
	m_Data->m_ClothSimDataCopy.Empty();

	m_Data->m_ShouldUpdateTransforms = false;
	m_Data->m_BoneVisibilityChanged = false;

	m_Data->m_AccumulatedDts = 0.0f;

	m_Data->m_SkinContext.m_SrcPositions = TStridedMemoryView<const CFloat3>();
	m_Data->m_SkinContext.m_SrcNormals = TStridedMemoryView<const CFloat3>();
	m_Data->m_SkinContext.m_SrcTangents = TStridedMemoryView<const CFloat3>();

	m_Data->m_SkinContext.m_DstPositions = TStridedMemoryView<CFloat3>();
	m_Data->m_SkinContext.m_DstNormals = TStridedMemoryView<CFloat3>();
	m_Data->m_SkinContext.m_DstTangents = TStridedMemoryView<CFloat4>();

	m_Data->m_Override.m_PositionsOverride = TStridedMemoryView<const CFloat3>();
	m_Data->m_Override.m_NormalsOverride = TStridedMemoryView<const CFloat3>();
	m_Data->m_Override.m_TangentsOverride = TStridedMemoryView<const CFloat4>();

	if (m_Data->m_CurrentSkinnedMeshComponent != null)
	{
		RemoveTickPrerequisiteComponent(m_Data->m_CurrentSkinnedMeshComponent.Get());
		m_Data->m_CurrentSkinnedMeshComponent = null;
	}
	PrimaryComponentTick.RemovePrerequisite(GetWorld(), GetWorld()->EndPhysicsTickFunction);
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::Skin_PreProcess(uint32 vertexStart, uint32 vertexCount, const PopcornFX::SSkinContext &ctx)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::Skin_PreProcess", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(m_Data != null);
	HH_ASSERT(vertexStart + vertexCount <= m_Data->m_DstPositions.Count());
	HH_ASSERT(m_Data->m_DstPositions.Count() == m_Data->m_OldPositions.Count());
	HH_ASSERT(m_Data->m_DstPositions.Count() == m_Data->m_DstVelocities.Count());

	PopcornFX::TStridedMemoryView<const CFloat3>	src = ctx.m_DstPositions.Slice(vertexStart, vertexCount);

	PopcornFX::TStridedMemoryView<CFloat3>	dst = PopcornFX::TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_Data->m_OldPositions.RawDataPointer()), m_Data->m_OldPositions.Count(), 16).Slice(vertexStart, vertexCount);

	HH_ASSERT(src.Stride() == 0x10 && dst.Stride() == 0x10);
	PopcornFX::Mem::Copy(dst.Data(), src.Data(), dst.Count() * dst.Stride());
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::Skin_PostProcess(uint32 vertexStart, uint32 vertexCount, const PopcornFX::SSkinContext &ctx)
{
	HH_ASSERT(m_Data != null);
	HH_ASSERT(m_Data->m_Mesh != null);

	if (bBuildClothData && !m_Data->m_ClothSections.Empty())
		FetchClothData(vertexStart, vertexCount);
	if (!bComputeVelocities)
		return;
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::Skin_PostProcess", POPCORNFX_UE_PROFILER_COLOR);

	PopcornFX::TStridedMemoryView<const CFloat3>	posCur = ctx.m_DstPositions.Slice(vertexStart, vertexCount);
	PopcornFX::TStridedMemoryView<const CFloat3>	posOld = PopcornFX::TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_Data->m_OldPositions.RawDataPointer()), m_Data->m_OldPositions.Count(), 16).Slice(vertexStart, vertexCount);
	PopcornFX::TStridedMemoryView<CFloat3>			vel = PopcornFX::TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_Data->m_DstVelocities.RawDataPointer()), m_Data->m_DstVelocities.Count(), 16).Slice(vertexStart, vertexCount);

	if (vel.Empty())
		return;
	if (m_Data->m_AccumulatedDts >= POPCORNFX_MAX_ANIM_IDLE_TIME)
	{
		PopcornFX::Mem::Clear(vel.Data(), vel.CoveredBytes());
		return;
	}
	HH_ASSERT(posCur.Stride() == 0x10);
	HH_ASSERT(posOld.Stride() == 0x10);
	HH_ASSERT(vel.Stride() == 0x10);

	CFloat3			* __restrict dstVelPtr = vel.Data();
	const CFloat3	*curPosPtr = posCur.Data();
	const CFloat3	*oldPosPtr = posOld.Data();

	HH_ASSERT(PopcornFX::Mem::IsAligned<0x10>(dstVelPtr));
	HH_ASSERT(PopcornFX::Mem::IsAligned<0x10>(curPosPtr));
	HH_ASSERT(PopcornFX::Mem::IsAligned<0x10>(oldPosPtr));

	const float				invDt = 1.0f / m_Data->m_AccumulatedDts;
	const VectorRegister	iDt = MakeVectorRegister(invDt, invDt, invDt, invDt);

	for (hh_u32 iVertex = 0; iVertex < vertexCount; ++iVertex)
	{
		const VectorRegister	curPos = VectorLoad(curPosPtr);
		const VectorRegister	oldPos = VectorLoad(oldPosPtr);
		const VectorRegister	v = VectorMultiply(VectorSubtract(curPos, oldPos), iDt);

		VectorStore(v, dstVelPtr);

		dstVelPtr = PopcornFX::Mem::AdvanceRawPointer(dstVelPtr, 0x10);
		curPosPtr = PopcornFX::Mem::AdvanceRawPointer(curPosPtr, 0x10);
		oldPosPtr = PopcornFX::Mem::AdvanceRawPointer(oldPosPtr, 0x10);
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::Skin_Finish(const PopcornFX::SSkinContext &ctx)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::Skin_Finish", POPCORNFX_UE_PROFILER_FAST_COLOR);

	HH_ASSERT(m_Data != null);
	HH_ASSERT(m_Data->m_Mesh != null);

	// Rebuild sampling structures if bone visibility array has changed
	if (!m_Data->m_BoneVisibilityChanged)
		return;
	if (!HH_VERIFY(bSkinPositions))
	{
		// @TODO : warn the user that distribution will be invalid if he only checked bSkinNormals or bSkinTangents
		// and he either is sampling a destructible mesh or the target skinned mesh component got one of its bone hidden/unhidden
		return;
	}
	if (!HH_VERIFY(m_Data->m_OverrideSurfaceSamplingAccelStructs.Build(m_Data->m_Mesh->TriangleBatch().m_IStream,
		m_Data->m_SkinContext.m_DstPositions,
		&m_Data->m_OverrideSurfaceSamplingWorkingBuffers)))
	{
		// @TODO : warn the user that it couldn't rebuild accel strucs
	}
}

//----------------------------------------------------------------------------

// Simpler solution to VectorTransformVector (assumes W == 1)
VectorRegister	_TransformVector(VectorRegister &v, const FMatrix *m)
{
	const VectorRegister	*M = (const VectorRegister*)m;
	VectorRegister			x, y, z;

	x = VectorReplicate(v, 0);
	y = VectorReplicate(v, 1);
	z = VectorReplicate(v, 2);

	x = VectorMultiply(x, M[0]);
	y = VectorMultiply(y, M[1]);
	z = VectorMultiply(z, M[2]);

	x = VectorAdd(x, y);
	z = VectorAdd(z, M[3]);
	x = VectorAdd(x, z);

	return x;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::FetchClothData(uint32 vertexStart, uint32 vertexCount)
{
	// Done during skinning job's PostProcess callback
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::FetchClothData", POPCORNFX_UE_PROFILER_COLOR);

	const TMap<int32, FClothSimulData>	&clothData = m_Data->m_ClothSimDataCopy;
	if (clothData.Num() == 0)
		return;

	const float			invScale = FPopcornFXPlugin::GlobalScaleRcp();
	const FMatrix		localM = m_Data->m_InverseTransforms * invScale;
	for (hh_u32 iSection = 0; iSection < m_Data->m_ClothSections.Count(); ++iSection)
	{
		const FPopcornFXClothSection	&section = m_Data->m_ClothSections[iSection];

		const hh_u32	baseVertexOffset = section.m_BaseVertexOffset;
		if (baseVertexOffset > vertexStart + vertexCount ||
			baseVertexOffset + section.m_VertexCount <= vertexStart)
			continue;
		if (!HH_VERIFY(clothData.Contains(section.m_ClothDataIndex)))
			continue;
		{
			HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::FetchClothData::Section", POPCORNFX_UE_PROFILER_COLOR);

			const FClothSimulData	&data = clothData[section.m_ClothDataIndex];
			HH_ASSERT(data.Positions.Num() > 0);
			HH_ASSERT(data.Positions.Num() == data.Normals.Num());
			HH_ASSERT(section.m_VertexCount == section.m_Indices.Count());

			const hh_u32	realVertexStart = PopcornFX::HHMax(vertexStart, baseVertexOffset);
			const hh_u32	realVertexEnd = PopcornFX::HHMin(baseVertexOffset + section.m_VertexCount, vertexStart + vertexCount);
			const hh_u32	realVertexCount = realVertexEnd - realVertexStart;
			const hh_u32	indicesStart = realVertexStart - baseVertexOffset;

			const FVector4	*srcPositions = data.Positions.GetData();
			const FVector4	*srcNormals = data.Normals.GetData();
			const hh_u32	*srcIndices = section.m_Indices.RawDataPointer() + indicesStart;

			CFloat4		_dummyNormal[1];
			CFloat4		*dstPositions = m_Data->m_DstPositions.RawDataPointer() + realVertexStart;
			CFloat4		*dstNormals = bSkinNormals ? m_Data->m_DstPositions.RawDataPointer() + realVertexStart : _dummyNormal;

			const hh_u32	dstStride = bSkinNormals ? 0x10 : 0;

			for (hh_u32 iVertex = 0; iVertex < realVertexCount; ++iVertex)
			{
				const hh_u32	simIndex = srcIndices[iVertex];

				VectorRegister			srcPos = VectorLoad(srcPositions + simIndex);
				const VectorRegister	srcNormal = VectorLoad(srcNormals + simIndex);

				srcPos = _TransformVector(srcPos, &localM);

				VectorStore(srcPos, dstPositions);
				VectorStore(srcNormal, dstNormals);

				dstPositions = PopcornFX::Mem::AdvanceRawPointer(dstPositions, 0x10);
				dstNormals = PopcornFX::Mem::AdvanceRawPointer(dstNormals, dstStride);
			}
		}
	}
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

void	UPopcornFXAttributeSamplerSkinnedMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const UProperty	*propertyThatChanged = PropertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bEditorBuildInitialPose) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, TargetActor) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, SkinnedMeshComponentName))
		{
			bEditorBuildInitialPose = false;
			BuildInitialPose();
		}
		else if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bSkinPositions) ||
				 propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bSkinNormals) ||
				 propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bSkinTangents) ||
				 propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bBuildColors) ||
				 propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bBuildUVs) ||
				 propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bBuildClothData))
		{
			if (!bSkinPositions)
				bComputeVelocities = false;
			BuildInitialPose();
		}
		else if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXAttributeSamplerSkinnedMesh, bComputeVelocities))
		{
			if (bComputeVelocities)
				bSkinPositions = true;
			BuildInitialPose();
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

PopcornFX::CParticleSamplerDescriptor	*UPopcornFXAttributeSamplerSkinnedMesh::_AttribSampler_SetupSamplerDescriptor(FPopcornFXSamplerDesc &desc, const PopcornFX::CParticleSamplerDescriptor *defaultShapeDesc)
{
	check(m_Data != null);

	bPauseSkinning = false;
	if (m_Data->m_Mesh == null)
	{
		if (!BuildInitialPose())
			return null;
	}

	if (m_Data->m_Desc == null)
		m_Data->m_Desc = HH_NEW(PopcornFX::CParticleSamplerDescriptor_Shape());
	PopcornFX::CParticleSamplerDescriptor_Shape	*shapeDesc = m_Data->m_Desc.Get();
	if (!HH_VERIFY(shapeDesc != null))
		return null;
	PopcornFX::CShapeDescriptor_Mesh	*descMesh = HH_NEW(PopcornFX::CShapeDescriptor_Mesh());
	if (!HH_VERIFY(descMesh != null))
		return null;
	descMesh->SetSamplingStructs(&m_Data->m_OverrideSurfaceSamplingAccelStructs, null);
	descMesh->SetMesh(m_Data->m_Mesh, &(m_Data->m_Override));

	shapeDesc->m_Shape = descMesh;
	shapeDesc->m_WorldTr_Current = reinterpret_cast<const CFloat4x4*>(&(m_WorldTr_Current));
	shapeDesc->m_WorldTr_Previous = reinterpret_cast<CFloat4x4*>(&(m_WorldTr_Previous));
	shapeDesc->m_Angular_Velocity = reinterpret_cast<const CFloat3*>(&(m_Angular_Velocity));
	shapeDesc->m_Linear_Velocity = reinterpret_cast<CFloat3*>(&(m_Linear_Velocity));

	UpdateTransforms();

	desc.m_NeedUpdate = true;

	return shapeDesc;
}

//----------------------------------------------------------------------------

USkinnedMeshComponent	*UPopcornFXAttributeSamplerSkinnedMesh::ResolveSkinnedMeshComponent(AActor *fallbackActor, bool enableLogOnError)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::ResolveSkinnedMeshComponent", POPCORNFX_UE_PROFILER_FAST_COLOR);
	if (!HH_VERIFY(fallbackActor != null))
		return null;
	const AActor			*parent = TargetActor == null ? fallbackActor : TargetActor;
	USkinnedMeshComponent	*skinnedMesh = null;
	if (SkinnedMeshComponentName != NAME_None)
	{
		UObjectPropertyBase	*prop = FindField<UObjectPropertyBase>(parent->GetClass(), SkinnedMeshComponentName);
		if (prop != null)
			skinnedMesh = Cast<USkinnedMeshComponent>(prop->GetObjectPropertyValue_InContainer(parent));
	}
	else
	{
		skinnedMesh = Cast<USkinnedMeshComponent>(parent->GetRootComponent());
	}
	if (skinnedMesh == null)
	{
		if (enableLogOnError)
		{
			// always log, must have a USkinnedMeshComponent or useless
			UE_LOG(LogPopcornFXAttributeSamplerSkinned, Warning,
				TEXT("Could not find component 'USkinnedMeshComponent %s.%s' for UPopcornFXAttributeSamplerSkinnedMesh '%s'"),
				*parent->GetName(), (SkinnedMeshComponentName != NAME_None ? *SkinnedMeshComponentName.ToString() : TEXT("RootComponent")),
				*GetFullName());
		}
		return null;
	}
	return skinnedMesh;
}

//----------------------------------------------------------------------------

static CFloat3	ToPk(const FPackedNormal &packedNormal)
{
	FVector	unpackedVector;
	VectorRegister	normal = packedNormal.GetVectorRegister();
	VectorStoreFloat3(normal, &unpackedVector);
	return ToPk(unpackedVector);
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FPopcornFXCustomVersion::GUID);

	if (Ar.IsLoading())
	{
		// Fix old bool parameters to enum
		const int32		version = GetLinkerCustomVersion(FPopcornFXCustomVersion::GUID);
		if (version < FPopcornFXCustomVersion::AttributeSamplers_ChangeTransforms)
		{
			if (bUseSkinnedMeshTransforms_DEPRECATED)
			{
				if (bUseRelativeTransform_DEPRECATED)
					Transforms = EPopcornFXSkinnedTransforms::SkinnedComponentRelativeTr;
				else
					Transforms = EPopcornFXSkinnedTransforms::SkinnedComponentWorldTr;
			}
			else
			{
				if (bUseRelativeTransform_DEPRECATED)
					Transforms = EPopcornFXSkinnedTransforms::AttrSamplerRelativeTr;
				else
					Transforms = EPopcornFXSkinnedTransforms::AttrSamplerWorldTr;
			}
			UE_LOG(LogPopcornFXAttributeSamplerSkinned, Log, TEXT("Upgraded from UseSkinnedMeshTransforms:%d UseRelativeTransform:%d to Transforms:%d"), bUseSkinnedMeshTransforms_DEPRECATED, bUseRelativeTransform_DEPRECATED, int(Transforms));
		}
	}
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerSkinnedMesh::SetComponentTickingGroup(USkinnedMeshComponent *skinnedMesh)
{
	if (Cast<USkeletalMeshComponent>(skinnedMesh) != null)
	{
		// Bone transforms are executed in the PrePhysics tick group,
		// So we set skinned mesh attr sampler components to be ticked in the same ticking group,
		// And this component will have the target skinned mesh component as prerequisite
		PrimaryComponentTick.TickGroup = TG_PrePhysics;
		AddTickPrerequisiteComponent(skinnedMesh);
	}
#if (ENGINE_MINOR_VERSION >= 18)
	else if (Cast<IDestructibleInterface>(skinnedMesh) != null)
#else
	else if (Cast<UDestructibleComponent>(skinnedMesh) != null)
#endif	// (ENGINE_MINOR_VERSION >= 18)
	{
		// Transforms are updated in TG_EndPhysics after APEX update for Destructible components
		PrimaryComponentTick.TickGroup = TG_EndPhysics;
		PrimaryComponentTick.AddPrerequisite(GetWorld(), GetWorld()->EndPhysicsTickFunction);
	}
	else
	{
		// We don't handle PoseableMesh, yet ?
		HH_ASSERT_NOT_REACHED();
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------

namespace
{
	const TArray<int32>		&GetMasterBoneMap(const USkinnedMeshComponent *skinnedMesh)
	{
		return skinnedMesh->GetMasterBoneMap();
	}

	using	PK_FSkelMeshSection = FSkelMeshSection;

	const TArray<PK_FSkelMeshSection>		&GetSections(const FStaticLODModel &lodmodel)
	{
		return lodmodel.Sections;
	}

	const TArray<FTransform>	&GetComponentSpaceTransforms(const USkinnedMeshComponent *comp)
	{
		return comp->GetComponentSpaceTransforms();
	}

} // namespace

  //----------------------------------------------------------------------------

class	FGPUVertex
{
public:
	FGPUVertex(const FStaticLODModel &lodModel, hh_u32 vertexIndex, bool hasExtraBoneInfluences)
		: m_LODModel(lodModel)
		, m_VertexIndex(vertexIndex)
		, m_HasExtraBoneInfluences(hasExtraBoneInfluences)
		, m_VertexPtr(lodModel.VertexBufferGPUSkin.GetVertexPtr(vertexIndex))
	{
		if (m_HasExtraBoneInfluences)
			m_SkinWeightPtr_ExtraInfluences = m_LODModel.SkinWeightVertexBuffer.GetSkinWeightPtr<true>(m_VertexIndex);
		else
			m_SkinWeightPtr_NoExtraInfluences = m_LODModel.SkinWeightVertexBuffer.GetSkinWeightPtr<false>(m_VertexIndex);
	}

	HH_FORCEINLINE CFloat3	Position() const
	{
		return ToPk(m_LODModel.VertexBufferGPUSkin.GetVertexPositionFast(m_VertexPtr));
	}

	HH_FORCEINLINE CFloat3	Normal() const
	{
		return ToPk(m_VertexPtr->TangentZ);
	}

	HH_FORCEINLINE CFloat3	Tangent() const
	{
		return ToPk(m_VertexPtr->TangentX);
	}

	HH_FORCEINLINE const CFloat2	Texcoords(hh_u32 UVIndex) const
	{
		return ToPk(m_LODModel.VertexBufferGPUSkin.GetVertexUVFast(m_VertexIndex, UVIndex));
	}

	HH_FORCEINLINE CFloat4	Color() const
	{
		return ToPk(m_LODModel.ColorVertexBuffer.VertexColor(m_VertexIndex));
	}

	HH_FORCEINLINE hh_u8	BoneIndex(const hh_u32 boneIndex) const
	{
		if (m_HasExtraBoneInfluences)
			return m_SkinWeightPtr_ExtraInfluences->InfluenceBones[boneIndex];
		else
			return m_SkinWeightPtr_NoExtraInfluences->InfluenceBones[boneIndex];
	}

	HH_FORCEINLINE float	BoneWeight(const hh_u32 boneIndex) const
	{
		if (m_HasExtraBoneInfluences)
			return static_cast<float>(m_SkinWeightPtr_ExtraInfluences->InfluenceWeights[boneIndex]);
		else
			return static_cast<float>(m_SkinWeightPtr_NoExtraInfluences->InfluenceWeights[boneIndex]);
	}

private:
	const FStaticLODModel			&m_LODModel;
	const hh_u32					m_VertexIndex;
	const bool						m_HasExtraBoneInfluences;

	const TGPUSkinVertexBase		*m_VertexPtr = null;
	union
	{
		const TSkinWeightInfo<false>	*m_SkinWeightPtr_NoExtraInfluences;
		const TSkinWeightInfo<true>		*m_SkinWeightPtr_ExtraInfluences;
	};
};

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerSkinnedMesh::BuildInitialPose()
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::BuildInitialPos", POPCORNFX_UE_PROFILER_COLOR);

	check(m_Data != null);
	Clear();

	HH_TODO("Big endian");
#if !PLATFORM_LITTLE_ENDIAN
	HH_ASSERT_NOT_REACHED();
	return false;
#endif

	USkinnedMeshComponent	*skinnedMesh = ResolveSkinnedMeshComponent(GetOwner(), true);
	if (skinnedMesh == null || skinnedMesh->SkeletalMesh == null)
		return false;

	const USkeletalMesh			*skelMesh = skinnedMesh->SkeletalMesh;
	const FSkeletalMeshResource	*skelMeshRes = skinnedMesh->GetSkeletalMeshResource();
	if (!HH_VERIFY(skelMeshRes != null && skelMeshRes->LODModels.Num() > 0))
		return false;
	const FStaticLODModel	&lodModel = skelMeshRes->LODModels[0];

	PopcornFX::PMeshNew		meshNew = HH_NEW(PopcornFX::CMeshNew);
	if (!HH_VERIFY(meshNew != null))
		return false;
	PopcornFX::CMeshTriangleBatch	&triBatch = meshNew->TriangleBatch();
	PopcornFX::CMeshVStream			&vstream = triBatch.m_VStream;
	PopcornFX::CMeshIStream			&istream = triBatch.m_IStream;

	//
	// Indices
	//

	TArray<uint32>	srcIndices;
	lodModel.MultiSizeIndexContainer.GetIndexBuffer(srcIndices);
	if (!HH_VERIFY(srcIndices.Num() > 0))
		return false;

	HH_TODO("Does the Index stream have to have U32 indices ???");
	istream.SetPrimitiveType(PopcornFX::CMeshIStream::Triangles);
	istream.Reformat(PopcornFX::CMeshIStream::U32Indices);

	const hh_u32	totalIndexCount = hh_u32(srcIndices.Num());
	if (!HH_VERIFY(istream.Resize(totalIndexCount)))
		return false;
	HH_ASSERT(istream.IndexByteWidth() == PopcornFX::CMeshIStream::U32Indices);
	hh_u32	*dstIndices = reinterpret_cast<hh_u32*>(istream.RawStreamForWriting());

	PopcornFX::Mem::Copy(dstIndices, &(srcIndices[0]), totalIndexCount * sizeof(hh_u32));

	if (!HH_VERIFY(totalIndexCount > 0))
		return false;

	//
	// Vertices
	//

	const hh_u32	totalVertexCount = lodModel.NumVertices;
	if (!HH_VERIFY(totalVertexCount > 0))
		return false;

	const hh_u32	uvCount = lodModel.NumTexCoords;

	PopcornFX::SVertexDeclaration	decl;

	const bool	skinPositions = bSkinPositions;
	const bool	skinNormals = bSkinNormals;
	const bool	skinTangents = bSkinTangents;
	const bool	buildColors = bBuildColors;
	bool		hasColors = false;
	const bool	buildUVs = bBuildUVs;
	const bool	useSkinning = skinPositions || skinNormals || skinTangents;

	if (skinPositions)
	{
		decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
			PopcornFX::CVStreamSemanticDictionnary::Ordinal_Position,
			PopcornFX::SVStreamCode::Element_Float3,
			PopcornFX::SVStreamCode::SIMD_Friendly
			));
		if (!HH_VERIFY(m_Data->m_DstPositions.Resize(totalVertexCount)))
			return false;
		m_Data->m_SkinContext.m_DstPositions = PopcornFX::TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_Data->m_DstPositions.RawDataPointer()), m_Data->m_DstPositions.Count(), 16);
		m_Data->m_Override.m_PositionsOverride = m_Data->m_SkinContext.m_DstPositions;
	}
	if (skinNormals)
	{
		decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
			PopcornFX::CVStreamSemanticDictionnary::Ordinal_Normal,
			PopcornFX::SVStreamCode::Element_Float3,
			PopcornFX::SVStreamCode::SIMD_Friendly | PopcornFX::SVStreamCode::Normalized
			));
		if (!HH_VERIFY(m_Data->m_DstNormals.Resize(totalVertexCount)))
			return false;
		m_Data->m_SkinContext.m_DstNormals = PopcornFX::TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_Data->m_DstNormals.RawDataPointer()), m_Data->m_DstNormals.Count(), 16);
		m_Data->m_Override.m_NormalsOverride = m_Data->m_SkinContext.m_DstNormals;
	}
	if (skinTangents)
	{
		decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
			PopcornFX::CVStreamSemanticDictionnary::Ordinal_Tangent,
			PopcornFX::SVStreamCode::Element_Float4,
			PopcornFX::SVStreamCode::SIMD_Friendly | PopcornFX::SVStreamCode::Normalized
			));
		if (!HH_VERIFY(m_Data->m_DstTangents.Resize(totalVertexCount)))
			return false;
		m_Data->m_SkinContext.m_DstTangents = PopcornFX::TStridedMemoryView<CFloat4>(reinterpret_cast<CFloat4*>(m_Data->m_DstTangents.RawDataPointer()), m_Data->m_DstTangents.Count(), 16);
		m_Data->m_Override.m_TangentsOverride = m_Data->m_DstTangents;
	}
	if (buildUVs)
	{
		for (hh_u32 iUV = 0; iUV < uvCount; ++iUV)
		{
			decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
				PopcornFX::CVStreamSemanticDictionnary::UvStreamToOrdinal(iUV),
				PopcornFX::SVStreamCode::Element_Float2
				));
		}
	}
	if (buildColors)
	{
		if (lodModel.ColorVertexBuffer.GetNumVertices() == totalVertexCount)
		{
			hasColors = true;
			decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
				PopcornFX::CVStreamSemanticDictionnary::Ordinal_Color,
				PopcornFX::SVStreamCode::Element_Float4,
				PopcornFX::SVStreamCode::SIMD_Friendly
				));
		}
		else
		{
			// @TODO : WARN the user that the skeletal mesh doesn't have color streams available
		}
	}
	if (bComputeVelocities)
	{
		// There should be at least position skinning
		HH_ASSERT(skinPositions && useSkinning);
		if (!HH_VERIFY(m_Data->m_DstVelocities.Resize(totalVertexCount)) ||
			!HH_VERIFY(m_Data->m_OldPositions.Resize(totalVertexCount)))
			return false;
		// No need to clear the oldPositions
		PopcornFX::Mem::Clear(m_Data->m_DstVelocities.RawDataPointer(), sizeof(CFloat4) * totalVertexCount);

		m_Data->m_SkinContext.m_CustomProcess_PreSkin = PopcornFX::SSkinContext::CbCustomProcess(this, &UPopcornFXAttributeSamplerSkinnedMesh::Skin_PreProcess);
		m_Data->m_Override.m_VelocitiesOverride = PopcornFX::TStridedMemoryView<CFloat3>(reinterpret_cast<CFloat3*>(m_Data->m_DstVelocities.RawDataPointer()), m_Data->m_DstVelocities.Count(), 16);
	}
	m_Data->m_SkinContext.m_CustomProcess_PostSkin = PopcornFX::SSkinContext::CbCustomProcess(this, &UPopcornFXAttributeSamplerSkinnedMesh::Skin_PostProcess);
	m_Data->m_SkinContext.m_CustomFinish = PopcornFX::SSkinContext::CbCustomFinish(this, &UPopcornFXAttributeSamplerSkinnedMesh::Skin_Finish);

	HH_VERIFY(vstream.Reformat(decl));
	if (!HH_VERIFY(vstream.Resize(totalVertexCount)))
		return false;

	const TStridedMemoryView<CFloat3>	srcPositionsView = vstream.Positions();
	const TStridedMemoryView<CFloat3>	srcNormalsView = vstream.Normals();
	const TStridedMemoryView<CFloat3>	srcTangentsView = TStridedMemoryView<CFloat3>::Reinterpret(vstream.Tangents());
	const TStridedMemoryView<CFloat4>	srcColorsView = vstream.Colors();

	HH_STACKSTRIDEDMEMORYVIEW(PopcornFX::TStridedMemoryView<CFloat2>, srcUVsView, uvCount);
	for (hh_u32 iUV = 0; iUV < uvCount; ++iUV)
		srcUVsView[iUV] = vstream.AbstractStream<CFloat2>(PopcornFX::CVStreamSemanticDictionnary::UvStreamToOrdinal(iUV));

	m_Data->m_SkinContext.m_SrcPositions = srcPositionsView;
	m_Data->m_SkinContext.m_SrcNormals = srcNormalsView;
	m_Data->m_SkinContext.m_SrcTangents = srcTangentsView;

	// No need to create bone weights/indices if we don't skin anything..
	hh_u32			maxInfluenceCount = 0;
	const hh_u32	sectionCount = GetSections(lodModel).Num();
	if (useSkinning)
	{
		for (hh_u32 iSection = 0; iSection < sectionCount; ++iSection)
		{
			const PK_FSkelMeshSection	&section = GetSections(lodModel)[iSection];
			HH_ASSERT(section.MaxBoneInfluences > 0);
			if (section.MaxBoneInfluences > static_cast<int32>(maxInfluenceCount))
				maxInfluenceCount = section.MaxBoneInfluences;
		}

		const hh_u32	totalDataCount = maxInfluenceCount * totalVertexCount;
		// Allocate necessary space for bone weights/indices
		if (!HH_VERIFY(totalDataCount > 0))
			return false;
		if (!HH_VERIFY(m_Data->m_BoneWeights.Resize(totalDataCount)) ||
			!HH_VERIFY(m_Data->m_BoneIndices.Resize(totalDataCount)))
			return false;
		HH_ASSERT(m_Data->m_BoneWeights.Count() == m_Data->m_BoneIndices.Count());
		HH_ASSERT(!m_Data->m_BoneWeights.Empty());
	}

	float	* __restrict boneWeights = m_Data->m_BoneWeights.RawDataPointer();
	hh_u8	* __restrict boneIndices = m_Data->m_BoneIndices.RawDataPointer();

	PopcornFX::Mem::Clear(boneWeights, sizeof(float) * maxInfluenceCount * totalVertexCount);
	PopcornFX::Mem::Clear(boneIndices, sizeof(hh_u8) * maxInfluenceCount * totalVertexCount);
	const USkinnedMeshComponent	*masterPoseComponent = skinnedMesh->MasterPoseComponent.Get();
	const bool					hasMasterPoseComponent = masterPoseComponent != null;
	if (hasMasterPoseComponent)
		HH_ASSERT(GetMasterBoneMap(skinnedMesh).Num() == skinnedMesh->SkeletalMesh->RefSkeleton.GetNum());

	HH_ONLY_IF_ASSERTS(hh_u32 totalVertices = 0);

	const float		scale = FPopcornFXPlugin::GlobalScale();
	const float		invScale = FPopcornFXPlugin::GlobalScaleRcp();
	const float		eSq = KINDA_SMALL_NUMBER * KINDA_SMALL_NUMBER;

	// Pack the vertices bone weights/indices from sparse sections
	for (hh_u32 iSection = 0; iSection < sectionCount; ++iSection)
	{
		const PK_FSkelMeshSection	&section = GetSections(lodModel)[iSection];
		const bool					hasExtraBoneInfluences = section.HasExtraBoneInfluences();
		HH_ONLY_IF_ASSERTS(const hh_u32 numSectionVertices = section.GetNumVertices());

		if (section.bDisabled)
		{
			HH_ONLY_IF_ASSERTS(totalVertices += numSectionVertices);
			continue;
		}

		const hh_u32	sectionOffset = section.BaseVertexIndex;
		const hh_u32	numVertices = section.GetNumVertices();
		const hh_u32	sectionInfluenceCount = section.MaxBoneInfluences;

		FPopcornFXClothSection		clothSection;
		const bool					validClothSection = bBuildClothData && section.HasClothingData() && useSkinning;

		bool						buildClothIndices = false;
		TMemoryView<const FVector>	clothVertices;
		if (validClothSection)
		{
			const hh_i32	clothingAssetIndex = skelMesh->GetClothingAssetIndex(section.ClothingData.AssetGuid);
			if (HH_VERIFY(clothingAssetIndex != INDEX_NONE))
			{
				UClothingAsset	*clothAsset = Cast<UClothingAsset>(skelMesh->MeshClothingAssets[clothingAssetIndex]);

				if (clothAsset != null && clothAsset->LodData.Num() > 0)
				{
					const FClothLODData				&lodData = clothAsset->LodData[0];
					const FClothPhysicalMeshData	&physMeshData = lodData.PhysicalMeshData;

					clothVertices = TMemoryView<const FVector>(physMeshData.Vertices.GetData(), physMeshData.Vertices.Num());

					if (HH_VERIFY(clothSection.m_Indices.Resize(numVertices)))
					{
						buildClothIndices = true;
						clothSection.m_BaseVertexOffset = section.BaseVertexIndex;
						clothSection.m_ClothDataIndex = clothingAssetIndex;
						clothSection.m_VertexCount = numVertices;
					}
				}
				else
				{
					UE_LOG(LogPopcornFXAttributeSamplerSkinned, Warning, TEXT("Couldn't build cloth data for asset '%s', section %d: No cloth LOD data available"), *skelMesh->GetName(), iSection);
				}
			}
		}

		for (hh_u32 iVertex = 0; iVertex < numVertices; ++iVertex)
		{
			const hh_u32	offsetNoInfluences = sectionOffset + iVertex;
			HH_ASSERT(offsetNoInfluences < totalVertexCount);

			const FGPUVertex	vertex(lodModel, offsetNoInfluences, hasExtraBoneInfluences);

			if (hasColors)
				srcColorsView[offsetNoInfluences] = vertex.Color();
			if (buildUVs)
			{
				for (hh_u32 iUV = 0; iUV < uvCount; ++iUV)
					srcUVsView[iUV][offsetNoInfluences] = vertex.Texcoords(iUV);
			}
			if (!useSkinning)
				continue;
			if (skinPositions)
			{
				srcPositionsView[offsetNoInfluences] = vertex.Position() * invScale;
				m_Data->m_DstPositions[offsetNoInfluences] = CFloat4(srcPositionsView[offsetNoInfluences], 0.0f);
			}
			if (skinNormals)
			{
				srcNormalsView[offsetNoInfluences] = vertex.Normal();
				m_Data->m_DstNormals[offsetNoInfluences] = CFloat4(srcNormalsView[offsetNoInfluences], 0.0f);
			}
			if (skinTangents)
			{
				srcTangentsView[offsetNoInfluences] = vertex.Tangent();
				m_Data->m_DstTangents[offsetNoInfluences] = CFloat4(srcTangentsView[offsetNoInfluences], 0.0f);
			}
			if (buildClothIndices)
			{
				// Expensive step, but necessary to have less runtime overhead
				hh_i32			index = INDEX_NONE;
				const hh_u32	clothVertexCount = clothVertices.Count();
				const FVector	pos = ToUE(srcPositionsView[offsetNoInfluences]) * scale;
				for (hh_u32 iClothVertex = 0; iClothVertex < clothVertexCount; ++iClothVertex)
				{
					if ((clothVertices[iClothVertex] - pos).SizeSquared() <= eSq)
					{
						index = iClothVertex;
						break;
					}
				}
				if ((buildClothIndices = index != INDEX_NONE))
					clothSection.m_Indices[iVertex] = index;
				else
				{
					UE_LOG(LogPopcornFXAttributeSamplerSkinned, Warning, TEXT("Couldn't build cloth LUT for asset '%s', section %d: Legacy APEX cloth assets not supported"), *skelMesh->GetName(), iSection);
				}
			}
			if (validClothSection) // Avoid skinning streams altogether
				continue;
			const hh_u32	offsetInfluences = offsetNoInfluences * maxInfluenceCount;
			HH_ASSERT(offsetInfluences + maxInfluenceCount <= m_Data->m_BoneWeights.Count());

			for (hh_u32 iInfluence = 0; iInfluence < sectionInfluenceCount; ++iInfluence)
			{
				boneWeights[offsetInfluences + iInfluence] = vertex.BoneWeight(iInfluence) / 255.0f;
				HH_ASSERT(boneWeights[offsetInfluences + iInfluence] >= 0.0f && boneWeights[offsetInfluences + iInfluence] <= 1.0f);

				const hh_u8		localBoneIndex = section.BoneMap[vertex.BoneIndex(iInfluence)];
				const hh_i32	masterBoneMapIndex = hasMasterPoseComponent ? GetMasterBoneMap(skinnedMesh)[localBoneIndex] : 0;
				HH_ASSERT(masterBoneMapIndex >= 0x0 && masterBoneMapIndex <= 0xFF);

				boneIndices[offsetInfluences + iInfluence] = hasMasterPoseComponent ? masterBoneMapIndex : localBoneIndex;

				// We can stop copying data, weights are sorted
				if (boneWeights[offsetInfluences + iInfluence] == 0.0f)
					break;
			}
		}
		if (buildClothIndices)
		{
			HH_PARANOID_ASSERT(validClothSection);
			m_Data->m_ClothSections.PushBack(clothSection);
		}

		HH_ONLY_IF_ASSERTS(totalVertices += numSectionVertices);
	}
	HH_ASSERT(totalVertices == totalVertexCount);

	// No need to build sampling structs as we override them
	meshNew->SetupRuntimeStructsIFN(false);

	if (!HH_VERIFY(m_Data->m_OverrideSurfaceSamplingAccelStructs.Build(istream,
		srcPositionsView,
		&m_Data->m_OverrideSurfaceSamplingWorkingBuffers)))
		return false;

	if (useSkinning)
	{
		HH_ASSERT(m_Data->m_SkeletonView == null);
		if (!HH_VERIFY(m_Data->m_SkinningStreamsProxy.Setup(lodModel.NumVertices, m_Data->m_BoneWeights, m_Data->m_BoneIndices)))
			return false;
		m_Data->m_SkinContext.m_SkinningStreams = &m_Data->m_SkinningStreamsProxy;

		const hh_u32	boneCount = lodModel.RequiredBones.Num();
		if (!HH_VERIFY(boneCount > 0))
			return false;
		if (!HH_VERIFY(m_Data->m_BoneInverseMatrices.Resize(boneCount)))
			return false;
		m_Data->m_SkeletonView = HH_NEW(PopcornFX::CSkeletonView(boneCount, null, m_Data->m_BoneInverseMatrices.RawDataPointer()));
		if (!HH_VERIFY(m_Data->m_SkeletonView != null))
			return false;

		// If everything is OK, we assign this component as dependency
		if (!HH_VERIFY(SetComponentTickingGroup(skinnedMesh)))
			return false;
		m_Data->m_CurrentSkinnedMeshComponent = skinnedMesh;
	}
	m_Data->m_Mesh = meshNew;

	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAttributeSamplerSkinnedMesh::UpdateSkinning()
{
	HH_ASSERT(IsInGameThread());
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::UpdateSkinning", POPCORNFX_UE_PROFILER_COLOR);

	HH_TODO("Don't skin several times the same skinned mesh component");

	// Do we have to resolve the skeletal mesh each frame ?
	USkinnedMeshComponent	*skinnedMesh = m_Data->m_CurrentSkinnedMeshComponent.Get();
	if (!HH_VERIFY(skinnedMesh != null))
		return false;

	// Don't even start the skinning if the skeleton view is invalid
	if (!HH_VERIFY(m_Data->m_SkeletonView != null))
		return false;
	const hh_u32	boneCount = m_Data->m_BoneInverseMatrices.Count();
	if (!HH_VERIFY(boneCount > 0))
		return false;
	const FVector	invScale(FPopcornFXPlugin::GlobalScaleRcp());

	const USkinnedMeshComponent	*baseComponent = skinnedMesh->MasterPoseComponent.IsValid() ? skinnedMesh->MasterPoseComponent.Get() : skinnedMesh;
	if (!HH_VERIFY(boneCount == GetComponentSpaceTransforms(baseComponent).Num()) ||
		!HH_VERIFY(boneCount == skinnedMesh->SkeletalMesh->RefBasesInvMatrix.Num()))
		return false;

	const USkeletalMesh			*mesh = skinnedMesh->SkeletalMesh;
	const TArray<uint8>			&boneVisibilityStates = skinnedMesh->BoneVisibilityStates;
	const TArray<FTransform>	&spaceBases = GetComponentSpaceTransforms(baseComponent);
	HH_ASSERT(boneVisibilityStates.Num() == boneCount);
	m_Data->m_BoneVisibilityChanged = false;
	for (hh_u32 iBone = 0; iBone < boneCount; ++iBone)
	{
		// Consider only purely visible vertices (hierarchy of bones is respected)
		const bool	isBoneVisible = boneVisibilityStates[iBone] == EBoneVisibilityStatus::BVS_Visible;
		const bool	wasBoneVisible = m_Data->m_BoneInverseMatrices[iBone] != CFloat4x4::ZERO;
		if (isBoneVisible)
		{
			FMatrix	matrix = mesh->RefBasesInvMatrix[iBone] * spaceBases[iBone].ToMatrixWithScale();

			matrix.ScaleTranslation(invScale);
			m_Data->m_BoneInverseMatrices[iBone] = ToPk(matrix);
		}
		else
			m_Data->m_BoneInverseMatrices[iBone] = CFloat4x4::ZERO;
		m_Data->m_BoneVisibilityChanged |= isBoneVisible != wasBoneVisible;
	}

	USkeletalMeshComponent	*skelMesh = Cast<USkeletalMeshComponent>(skinnedMesh);
	if (skelMesh != null &&
		!skelMesh->bDisableClothSimulation)
	{
		if (bBuildClothData &&
			!m_Data->m_ClothSections.Empty())
		{
			SCOPE_CYCLE_COUNTER(STAT_PopcornFX_FetchClothData); // Time cloth data copy

			const TMap<int32, FClothSimulData>	&simData = skelMesh->GetCurrentClothingData_GameThread();
			const FMatrix						&inverseTr = skelMesh->GetComponentToWorld().Inverse().ToMatrixWithScale();

			m_Data->m_InverseTransforms = inverseTr;
			m_Data->m_ClothSimDataCopy = simData;
		}
	}

	// Launch skinning tasks
	PopcornFX::CSkeletalSkinnerSimple::AsyncSkinStart(m_Data->m_AsyncSkinContext, *m_Data->m_SkeletonView, m_Data->m_SkinContext);

	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::TickComponent", POPCORNFX_UE_PROFILER_COLOR);

	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	HH_ASSERT(m_Data != null);
	HH_ASSERT(m_Data->m_Mesh != null);
	m_Data->m_ClothSimDataCopy.Empty();

	// Don't skin anything if we don't have any skinned mesh component assigned
	const USkinnedMeshComponent	*skinnedMesh = m_Data->m_CurrentSkinnedMeshComponent.Get();
	if (skinnedMesh == null)
	{
		HH_ASSERT(!m_Data->m_ShouldUpdateTransforms);
		return;
	}
	HH_ASSERT(bSkinPositions || bSkinNormals || bSkinTangents);
	bool	shouldUpdateTransforms = (skinnedMesh->bRecentlyRendered || skinnedMesh->MeshComponentUpdateFlag == EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones) && !bPauseSkinning;

	// We don't want to interpolate more than POPCORNFX_MAX_ANIM_IDLE_TIME of inactive animation.
	if (m_Data->m_AccumulatedDts < POPCORNFX_MAX_ANIM_IDLE_TIME)
		m_Data->m_AccumulatedDts += GetWorld()->DeltaTimeSeconds;
	if (shouldUpdateTransforms)
	{
		// This means that no emitter is attached to this attr sampler, automatically pause skinning
		if (m_Data->m_AsyncSkinContext.m_SkinMergeJob != null)
		{
			HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::TickComponent AsyncSkinWait", POPCORNFX_UE_PROFILER_COLOR);

			// @TODO : Warn the user that he'll have to unpause the skinner when rehooking an effect to this attr sampler
			PopcornFX::CSkeletalSkinnerSimple::AsyncSkinWait(m_Data->m_AsyncSkinContext, null);
			bPauseSkinning = true;
			return;
		}
		shouldUpdateTransforms = UpdateSkinning();
	}
	else if (m_Data->m_ShouldUpdateTransforms)
	{
		// Clear velocities
		PopcornFX::Mem::Clear(m_Data->m_DstVelocities.RawDataPointer(), sizeof(CFloat4) * m_Data->m_DstVelocities.Count());
	}
	m_Data->m_ShouldUpdateTransforms = shouldUpdateTransforms;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::UpdateTransforms()
{
	m_WorldTr_Previous = m_WorldTr_Current;

	switch (Transforms)
	{
		case	EPopcornFXSkinnedTransforms::SkinnedComponentRelativeTr:
			if (m_Data->m_CurrentSkinnedMeshComponent.IsValid())
			{
				if (bApplyScale)
					m_WorldTr_Current = m_Data->m_CurrentSkinnedMeshComponent->GetRelativeTransform().ToMatrixWithScale();
				else
					m_WorldTr_Current = m_Data->m_CurrentSkinnedMeshComponent->GetRelativeTransform().ToMatrixNoScale();
			}
			break;
		case	EPopcornFXSkinnedTransforms::SkinnedComponentWorldTr:
			if (m_Data->m_CurrentSkinnedMeshComponent.IsValid())
			{
				if (bApplyScale)
					m_WorldTr_Current = m_Data->m_CurrentSkinnedMeshComponent->GetComponentTransform().ToMatrixWithScale();
				else
					m_WorldTr_Current = m_Data->m_CurrentSkinnedMeshComponent->GetComponentTransform().ToMatrixNoScale();
			}
			break;
		case	EPopcornFXSkinnedTransforms::AttrSamplerRelativeTr:
			if (bApplyScale)
				m_WorldTr_Current = GetRelativeTransform().ToMatrixWithScale();
			else
				m_WorldTr_Current = GetRelativeTransform().ToMatrixNoScale();
			break;
		case	EPopcornFXSkinnedTransforms::AttrSamplerWorldTr:
			if (bApplyScale)
				m_WorldTr_Current = GetComponentTransform().ToMatrixWithScale();
			else
				m_WorldTr_Current = GetComponentTransform().ToMatrixNoScale();
			break;
		default:
			HH_ASSERT_NOT_REACHED();
			break;
	}

	const float		pkScaleRcp = FPopcornFXPlugin::GlobalScaleRcp();
	m_WorldTr_Current.M[3][0] *= pkScaleRcp;
	m_WorldTr_Current.M[3][1] *= pkScaleRcp;
	m_WorldTr_Current.M[3][2] *= pkScaleRcp;

	m_Angular_Velocity = FVector(0);
	m_Linear_Velocity = ComponentVelocity;
}

//----------------------------------------------------------------------------

void	UPopcornFXAttributeSamplerSkinnedMesh::_AttribSampler_PreUpdate(CParticleScene *scene, float deltaTime, enum ELevelTick tickType)
{
	HH_NAMEDSCOPEDPROFILE_C("UPopcornFXAttributeSamplerSkinnedMesh::_AttribSampler_PreUpdate", POPCORNFX_UE_PROFILER_COLOR);

	if (m_LastFrameUpdate == GFrameCounter)
		return;
	m_LastFrameUpdate = GFrameCounter;

	UpdateTransforms();

	if (m_Data->m_CurrentSkinnedMeshComponent.IsValid() &&
		m_Data->m_ShouldUpdateTransforms)
	{
		HH_ASSERT(bSkinPositions || bSkinNormals || bSkinTangents);
		SCOPE_CYCLE_COUNTER(STAT_PopcornFX_SkinningWaitTime);

		PopcornFX::CSkeletalSkinnerSimple::AsyncSkinWait(m_Data->m_AsyncSkinContext, null);

		if (!bPauseSkinning)
			m_Data->m_AccumulatedDts = 0.0f;
	}
}

//----------------------------------------------------------------------------
