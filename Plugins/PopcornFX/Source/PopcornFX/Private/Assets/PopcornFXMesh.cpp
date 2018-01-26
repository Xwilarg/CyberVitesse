//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXMesh.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXStaticMeshData.h"
#include "PopcornFXVersionGenerated.h"
#include "Editor/EditorHelpers.h"

#include "StaticMeshResources.h"
#include "EditorFramework/AssetImportData.h"
#if WITH_EDITOR
#	include "Misc/MessageDialog.h"
#endif

#include "PopcornFXSDK.h"
#include <hh_geometrics/include/ge_mesh_resource.h>
#include <hh_geometrics/include/ge_mesh.h>
#include <hh_kernel/include/kr_containers_array.h>
#include <hh_kernel/include/kr_containers_onstack.h>

DEFINE_LOG_CATEGORY_STATIC(LogPopcornMesh, Log, All);
#define LOCTEXT_NAMESPACE "PopcornFXMesh"

#define ENABLE_SKELETALMESH			1

//----------------------------------------------------------------------------
//
// UPopcornFXMesh
//
//----------------------------------------------------------------------------

template <typename _AssetType>
static UPopcornFXMesh		*_FindOrCreateMeshData(_AssetType *asset)
{
	if (!HH_VERIFY(asset != null))
		return null;

	asset->ConditionalPostLoad();

	UPopcornFXStaticMeshData	*userData = null;
	UPopcornFXMesh				*mesh = null;

	userData = Cast<UPopcornFXStaticMeshData>(asset->GetAssetUserDataOfClass(UPopcornFXStaticMeshData::StaticClass()));
	if (userData == null)
	{
#if WITH_EDITOR
		asset->Modify(true);
		ForceSetPackageDiry(asset);
#endif
		userData = NewObject<UPopcornFXStaticMeshData>(asset);
		asset->AddAssetUserData(userData);
	}
	else
	{
		mesh = userData->PopcornFXMesh;
	}

	if (mesh == null)
	{
#if WITH_EDITOR
		asset->Modify(true);
		ForceSetPackageDiry(asset);
#endif
		mesh = NewObject<UPopcornFXMesh>(asset);
		userData->PopcornFXMesh = mesh;
	}

	mesh->ConditionalPostLoad();

	return mesh;
}

//----------------------------------------------------------------------------

//static
UPopcornFXMesh		*UPopcornFXMesh::FindStaticMesh(UStaticMesh *staticMesh)
{
	UPopcornFXMesh	*mesh = _FindOrCreateMeshData(staticMesh);
	if (mesh != null)
		mesh->SetSourceMesh(staticMesh, false);
	return mesh;
}

//----------------------------------------------------------------------------

//static
UPopcornFXMesh	*UPopcornFXMesh::FindSkeletalMesh(USkeletalMesh *skelMesh)
{
#if ENABLE_SKELETALMESH
	UPopcornFXMesh	*mesh = _FindOrCreateMeshData(skelMesh);
	if (mesh != null)
		mesh->SetSourceMesh(skelMesh, false);
	return mesh;
#else
	return null;
#endif
}

//----------------------------------------------------------------------------

UPopcornFXMesh::UPopcornFXMesh(const FObjectInitializer& PCIP)
	: Super(PCIP)
#if WITH_EDITORONLY_DATA
	, StaticMesh(null)
	, StaticMeshLOD(0)
	, bMergeStaticMeshSections(false)
	, bEditorReimport(false)
#endif
{
#if WITH_EDITORONLY_DATA
	StaticMeshAssetImportData = CreateDefaultSubobject<UAssetImportData>(TEXT("StaticMeshAssetImportData"));
#endif
	m_FileExtension = "pkmm";
	m_IsBaseObject = false;
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::PostLoad()
{
	Super::PostLoad();
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::BeginDestroy()
{
	Super::BeginDestroy();
	m_ResourceMesh = null;
}

//----------------------------------------------------------------------------

PopcornFX::PResourceMesh	UPopcornFXMesh::LoadResourceMeshIFN(bool editorBuildIFN)
{
	if (!HasSourceMesh())
		return null;

	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerMesh_UE::LoadResourceMeshIFN", POPCORNFX_UE_PROFILER_COLOR);

	if (m_ResourceMesh == null)
	{
		m_ResourceMesh = HH_NEW(PopcornFX::CResourceMesh);
		if (!m_ResourceMesh->Load(PopcornFX::CFilePackPath(FPopcornFXPlugin::Get().FilePack(), PkPath())))
		{
			HH_ASSERT_NOT_REACHED();
			m_ResourceMesh = null;
		}
	}

#if WITH_EDITOR
	if (editorBuildIFN && SourceMeshChanged())
	{
		bool		reload = false;

		if (FPopcornFXPlugin::AskBuildMeshData_YesAll())
			reload = true;
		else
		{
			FText		title = LOCTEXT("PopcornFX: Build Mesh data", "PopcornFX: Build Mesh data");
			FString		msg;
			msg += "Do you want to generate mesh data for \"" + SourceMeshObject()->GetPathName() + "\" ?";
			EAppReturnType::Type	response = FMessageDialog::Open(EAppMsgType::YesNoYesAll, FText::FromString(msg), &title);
			if (response == EAppReturnType::YesAll)
			{
				reload = true;
				FPopcornFXPlugin::SetAskBuildMeshData_YesAll(true);
			}
			else if (response == EAppReturnType::Yes)
			{
				reload = true;
			}
		}

		if (reload)
		{
			BuildMeshFromSource();
		}
	}
#endif // WITH_EDITOR

	return m_ResourceMesh.Get();
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::RebuildFromSource(UStaticMesh *staticMesh)
{
	SetSourceMesh(staticMesh, true);
	BuildMeshFromSource();
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::RebuildFromSource(USkeletalMesh *skelMesh)
{
	SetSourceMesh(skelMesh, true);
	BuildMeshFromSource();
}

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const UProperty	*propertyThatChanged = PropertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXMesh, StaticMesh) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXMesh, SkeletalMesh) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXMesh, StaticMeshLOD) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXMesh, bMergeStaticMeshSections) ||
			propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXMesh, bEditorReimport))
		{
			StaticMeshLOD = FMath::Max(StaticMeshLOD, 0);
			bEditorReimport = 0;
			if (StaticMesh != null)
				SetSourceMesh(StaticMesh, true);
			else if (SkeletalMesh != null)
				SetSourceMesh(SkeletalMesh, true);
			LoadResourceMeshIFN(true);
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::PreReimport_Clean()
{
	m_ResourceMesh = null;
	Super::PreReimport_Clean();
}

//----------------------------------------------------------------------------

bool	AssetImportDataDiffers(const UAssetImportData *a, const UAssetImportData* b)
{
#if (ENGINE_MINOR_VERSION >= 9)
	if (a->SourceFilePath_DEPRECATED != b->SourceFilePath_DEPRECATED ||
		a->SourceFileTimestamp_DEPRECATED != b->SourceFileTimestamp_DEPRECATED)
		return true;
	const int32		c = a->SourceData.SourceFiles.Num();
	if (c != b->SourceData.SourceFiles.Num())
		return true;
	for (int32 i = 0; i < c; ++i)
	{
		const auto		&sfa = a->SourceData.SourceFiles[i];
		const auto		&sfb = b->SourceData.SourceFiles[i];
		if (sfa.RelativeFilename != sfb.RelativeFilename ||
			sfa.Timestamp != sfb.Timestamp)
			return true;
		// Seems buggy, cannot rely on this.. Timestamp comparison should be enough
		// if (sfa.FileHash != sfb.FileHash)
		// 	return true;
	}
	return false;
#else
	return
		a->SourceFilePath != b->SourceFilePath ||
		a->SourceFileTimestamp != b->SourceFileTimestamp;
#endif
}

bool	UPopcornFXMesh::SourceMeshChanged() const
{
	if (StaticMesh != null && HH_VERIFY(StaticMesh->AssetImportData != null))
	{
		return AssetImportDataDiffers(StaticMeshAssetImportData, StaticMesh->AssetImportData);
	}
#if ENABLE_SKELETALMESH
	if (SkeletalMesh != null && HH_VERIFY(SkeletalMesh->AssetImportData != null))
	{
		return AssetImportDataDiffers(StaticMeshAssetImportData, SkeletalMesh->AssetImportData);
	}
#endif
	return false;
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXMesh::SetSourceMesh(UStaticMesh *staticMesh, bool dirtifyBuild)
{
	if (dirtifyBuild || staticMesh != StaticMesh || SkeletalMesh != null)
	{
#if WITH_EDITOR
#	if (ENGINE_MINOR_VERSION >= 9)
		StaticMeshAssetImportData->SourceData.SourceFiles.Empty();
#	else
		StaticMeshAssetImportData->SourceFilePath.Empty();
		StaticMeshAssetImportData->SourceFileTimestamp.Empty();
#	endif
#endif
		StaticMesh = staticMesh;
		SkeletalMesh = null;
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::SetSourceMesh(USkeletalMesh *skelMesh, bool dirtifyBuild)
{
	if (dirtifyBuild || skelMesh != SkeletalMesh || StaticMesh != null)
	{
#if WITH_EDITOR
#	if (ENGINE_MINOR_VERSION >= 9)
		StaticMeshAssetImportData->SourceData.SourceFiles.Empty();
#	else
		StaticMeshAssetImportData->SourceFilePath.Empty();
		StaticMeshAssetImportData->SourceFileTimestamp.Empty();
#	endif
#endif
		StaticMesh = null;
		SkeletalMesh = skelMesh;
	}
}

//----------------------------------------------------------------------------

bool	UPopcornFXMesh::BuildMeshFromSource()
{
	if (StaticMesh != null)
		return BuildStaticMesh();
	if (SkeletalMesh != null)
		return BuildSkeletalMesh();
	return false;
}

//----------------------------------------------------------------------------

bool	UPopcornFXMesh::BuildStaticMesh()
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerMesh_UE::BuildStaticMesh", POPCORNFX_UE_PROFILER_COLOR);

	if (m_ResourceMesh == null)
	{
		m_ResourceMesh = HH_NEW(PopcornFX::CResourceMesh);
		if (!HH_VERIFY(m_ResourceMesh != null))
			return false;
	}

	HH_ASSERT(StaticMesh != null);
	PopcornFX::PResourceMesh		newResource = NewResourceMesh(StaticMesh, uint32(FMath::Max(StaticMeshLOD, 0)), bMergeStaticMeshSections != 0);
	if (!HH_VERIFY(newResource != null))
		return false;

	m_ResourceMesh->m_OnReloading(m_ResourceMesh.Get());

	m_ResourceMesh->Swap(*newResource);

#if WITH_EDITOR
	const bool	validStaticMesh = StaticMesh->AssetImportData != null;
#	if (ENGINE_MINOR_VERSION >= 9)
	if (validStaticMesh)
		StaticMeshAssetImportData->SourceData.SourceFiles = StaticMesh->AssetImportData->SourceData.SourceFiles;
	else
		StaticMeshAssetImportData->SourceData.SourceFiles.Empty();
#	else
	StaticMeshAssetImportData->SourceFilePath = validStaticMesh ? StaticMesh->AssetImportData->SourceFilePath : TEXT("");
	StaticMeshAssetImportData->SourceFileTimestamp = validStaticMesh ? StaticMesh->AssetImportData->SourceFileTimestamp : TEXT("");
#	endif

	WriteMesh();
#endif // WITH_EDITOR

	m_ResourceMesh->m_OnReloaded(m_ResourceMesh.Get());

	return true;
}

//----------------------------------------------------------------------------

bool	UPopcornFXMesh::BuildSkeletalMesh()
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerMesh_UE::BuildSkeletalMesh", POPCORNFX_UE_PROFILER_COLOR);

	return false;
}

//----------------------------------------------------------------------------

namespace
{
	void		_FinalSetupMeshNew(const PopcornFX::PMeshNew &meshNew)
	{
		meshNew->RebuildBBox();
		meshNew->BuildTangentsIFN();
		meshNew->SetupRuntimeStructsIFN(true);
	}

} // namespace

//----------------------------------------------------------------------------

//static
PopcornFX::PResourceMesh	UPopcornFXMesh::NewResourceMesh(UStaticMesh *staticMesh, uint32 lod, bool mergeSections)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerMesh_UE::NewResourceMesh", POPCORNFX_UE_PROFILER_COLOR);

	const float						scale = FPopcornFXPlugin::GlobalScaleRcp();

	PopcornFX::PResourceMesh	resMesh = HH_NEW(PopcornFX::CResourceMesh);
	if (!HH_VERIFY(resMesh != null))
		return resMesh;
	if (staticMesh == null || !HH_VERIFY(staticMesh->RenderData != null))
		return resMesh;
	if (staticMesh->RenderData->LODResources.Num() == 0)
		return resMesh;

	lod = PopcornFX::HHMin(lod, uint32(staticMesh->RenderData->LODResources.Num()) - 1);
	const FStaticMeshLODResources		&lodRes = staticMesh->RenderData->LODResources[lod];

	if (!HH_VERIFY(lodRes.Sections.Num() != 0))
		return resMesh;

	FIndexArrayView					srcIndices = lodRes.IndexBuffer.GetArrayView();
	const hh_u32					totalIndexCount = hh_u32(srcIndices.Num());

	const hh_u32					totalVertexCount = lodRes.PositionVertexBuffer.GetNumVertices();

	const FStaticMeshVertexBuffer	&srcVertices = lodRes.VertexBuffer;
	HH_ASSERT(srcVertices.GetNumVertices() == totalVertexCount);
	const hh_u32					uvCount = srcVertices.GetNumTexCoords();

	const FPositionVertexBuffer		&srcPositions = lodRes.PositionVertexBuffer;
	const FColorVertexBuffer		&srcColors = lodRes.ColorVertexBuffer;
	const bool						hasColors = (srcColors.GetNumVertices() == totalVertexCount);

	PopcornFX::SVertexDeclaration	decl;

	decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
		PopcornFX::CVStreamSemanticDictionnary::Ordinal_Position,
		PopcornFX::SVStreamCode::Element_Float3,
		PopcornFX::SVStreamCode::SIMD_Friendly
		));
	decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
		PopcornFX::CVStreamSemanticDictionnary::Ordinal_Normal,
		PopcornFX::SVStreamCode::Element_Float3,
		PopcornFX::SVStreamCode::SIMD_Friendly
		));
	decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
		PopcornFX::CVStreamSemanticDictionnary::Ordinal_Tangent,
		PopcornFX::SVStreamCode::Element_Float4,
		PopcornFX::SVStreamCode::SIMD_Friendly
		));
	for (hh_u32 uvi = 0; uvi < uvCount; ++uvi)
	{
		decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
			PopcornFX::CVStreamSemanticDictionnary::UvStreamToOrdinal(uvi),
			PopcornFX::SVStreamCode::Element_Float2
			));
	}
	if (hasColors)
	{
		decl.AddStreamCodeIFN(PopcornFX::SVStreamCode(
			PopcornFX::CVStreamSemanticDictionnary::Ordinal_Color,
			PopcornFX::SVStreamCode::Element_Float4,
			PopcornFX::SVStreamCode::SIMD_Friendly
			));
	}

	if (mergeSections)
	{
		PopcornFX::PMeshNew				meshNew = HH_NEW(PopcornFX::CMeshNew);
		if (!HH_VERIFY(meshNew != null))
			return resMesh;

		PopcornFX::CMeshTriangleBatch	&triBatch = meshNew->TriangleBatch();
		PopcornFX::CMeshVStream			&vstream = triBatch.m_VStream;
		PopcornFX::CMeshIStream			&istream = triBatch.m_IStream;

		HH_VERIFY(vstream.Reformat(decl));

		istream.SetPrimitiveType(PopcornFX::CMeshIStream::Triangles);
		istream.Reformat(PopcornFX::CMeshIStream::U32Indices);

		if (!HH_VERIFY(istream.Resize(totalIndexCount)))
			return resMesh;
		HH_ASSERT(istream.IndexByteWidth() == PopcornFX::CMeshIStream::U32Indices);
		hh_u32			*dstIndices = reinterpret_cast<hh_u32*>(istream.RawStreamForWriting());

		if (!HH_VERIFY(vstream.Resize(totalVertexCount)))
			return resMesh;

		// no raw data access, must do one by one -_-
		for (hh_u32 i = 0; i < totalIndexCount; ++i)
		{
			dstIndices[i] = srcIndices[i];
		}

		const TStridedMemoryView<CFloat3>	dstPositions = vstream.Positions();
		HH_RELEASE_ASSERT(!dstPositions.Empty());
		const TStridedMemoryView<CFloat3>	dstNormals = vstream.Normals();
		HH_RELEASE_ASSERT(!dstNormals.Empty());
		HH_RELEASE_ASSERT(PopcornFX::Mem::IsAligned<0x10>(dstNormals.Data()) & PopcornFX::Mem::IsAligned<0x10>(dstNormals.Stride()));
		const TStridedMemoryView<CFloat4>	dstTangents = vstream.Tangents();
		HH_RELEASE_ASSERT(!dstTangents.Empty());
		HH_RELEASE_ASSERT(PopcornFX::Mem::IsAligned<0x10>(dstTangents.Data()) & PopcornFX::Mem::IsAligned<0x10>(dstTangents.Stride()));

		HH_STACKSTRIDEDMEMORYVIEW(TStridedMemoryView<CFloat2>, dstUvs, uvCount);
		for (hh_u32 uvi = 0; uvi < uvCount; ++uvi)
		{
			dstUvs[uvi] = vstream.AbstractStream<CFloat2>(PopcornFX::CVStreamSemanticDictionnary::UvStreamToOrdinal(uvi));
			HH_RELEASE_ASSERT(!dstUvs[uvi].Empty());
		}

		TStridedMemoryView<CFloat4>			dstColors;
		if (hasColors)
		{
			dstColors = vstream.Colors();
			HH_RELEASE_ASSERT(!dstColors.Empty());
		}

		for (hh_u32 i = 0; i < totalVertexCount; ++i)
		{
			const hh_u32	srci = i;
			const hh_u32	dsti = i;

			const FVector	&pos = srcPositions.VertexPosition(srci);
			dstPositions[dsti] = ToPk(pos) * scale;

			const FVector	&normal = srcVertices.VertexTangentZ(srci);
			const FVector	&tagent = srcVertices.VertexTangentX(srci);

			HH_TODO("Optim: get the TangentZ.w to put in our tangent.w");
			// Ugly recontruct binormal sign to put in tangent.w
			// New 4.12 tangent basis precision hides TangetZ.w behind more stuff
			// Should use: if (GetUseHighPrecisionTangentBasis()) { ... TangentZ.w ... } else { .... TangentZ.w .... }
			const FVector	&finalBinormal = srcVertices.VertexTangentY(srci);
			const FVector	baseBinormal = normal ^ tagent;
			const float		biNormalSign =  FVector::DotProduct(baseBinormal, finalBinormal) < 0.0 ? -1.0f : 1.0f;

			dstNormals[dsti] = ToPk(normal);
			dstTangents[dsti] = CFloat4(ToPk(tagent), biNormalSign);

			for (hh_u32 uvi = 0; uvi < uvCount; ++uvi)
			{
				const FVector2D		&uv = srcVertices.GetVertexUV(srci, uvi);
				dstUvs[uvi][dsti] = ToPk(uv);
			}

			if (!dstColors.Empty())
			{
				const FLinearColor	color = srcColors.VertexColor(srci).ReinterpretAsLinear();
				dstColors[dsti] = ToPk(color);
			}
		}

		_FinalSetupMeshNew(meshNew);

		if (!HH_VERIFY(resMesh->AddBatch("kkette", meshNew).Valid()))
			return resMesh;
	}
	else
	{
		PopcornFX::TSemiDynamicArray<hh_u32, 256>		vertexRemapArray;
		if (!HH_VERIFY(vertexRemapArray.Resize(totalVertexCount)))
			return resMesh;
		hh_u32				*vertexRemap = vertexRemapArray.RawDataPointer();

		for (hh_u32 sectioni = 0; sectioni < hh_u32(lodRes.Sections.Num()); ++sectioni)
		{
			const FStaticMeshSection		&section = lodRes.Sections[sectioni];

			PopcornFX::PMeshNew				meshNew = HH_NEW(PopcornFX::CMeshNew);
			if (!HH_VERIFY(meshNew != null))
				continue;

			PopcornFX::CMeshTriangleBatch	&triBatch = meshNew->TriangleBatch();
			PopcornFX::CMeshVStream			&vstream = triBatch.m_VStream;
			PopcornFX::CMeshIStream			&istream = triBatch.m_IStream;

			HH_VERIFY(vstream.Reformat(decl));

			istream.SetPrimitiveType(PopcornFX::CMeshIStream::Triangles);
			istream.Reformat(PopcornFX::CMeshIStream::U32Indices);

			const hh_u32			indexCount = section.NumTriangles * 3;
			const hh_u32			srcIndexStart = section.FirstIndex;
			if (!HH_VERIFY(istream.Resize(indexCount)))
				continue;
			HH_ASSERT(istream.IndexByteWidth() == PopcornFX::CMeshIStream::U32Indices);
			hh_u32					*dstIndices = reinterpret_cast<hh_u32*>(istream.RawStreamForWriting());

			if (!HH_VERIFY(vstream.Resize(totalVertexCount))) // BAAAAAA, we ll shrink later
				continue;

			const TStridedMemoryView<CFloat3>	dstPositions = vstream.Positions();
			HH_RELEASE_ASSERT(!dstPositions.Empty());
			const TStridedMemoryView<CFloat3>	dstNormals = vstream.Normals();
			HH_RELEASE_ASSERT(!dstNormals.Empty());
			HH_RELEASE_ASSERT(PopcornFX::Mem::IsAligned<0x10>(dstNormals.Data()) & PopcornFX::Mem::IsAligned<0x10>(dstNormals.Stride()));
			const TStridedMemoryView<CFloat4>	dstTangents = vstream.Tangents();
			HH_RELEASE_ASSERT(!dstTangents.Empty());
			HH_RELEASE_ASSERT(PopcornFX::Mem::IsAligned<0x10>(dstTangents.Data()) & PopcornFX::Mem::IsAligned<0x10>(dstTangents.Stride()));

			HH_STACKSTRIDEDMEMORYVIEW(TStridedMemoryView<CFloat2>, dstUvs, uvCount);
			for (hh_u32 uvi = 0; uvi < uvCount; ++uvi)
			{
				dstUvs[uvi] = vstream.AbstractStream<CFloat2>(PopcornFX::CVStreamSemanticDictionnary::UvStreamToOrdinal(uvi));
				HH_ASSERT(!dstUvs[uvi].Empty());
			}

			TStridedMemoryView<CFloat4>			dstColors;
			if (hasColors)
			{
				dstColors = vstream.Colors();
				HH_ASSERT(!dstColors.Empty());
			}

			hh_u32					dstVertexCount = 0;

			PopcornFX::Mem::Clear(vertexRemap, sizeof(*vertexRemap) * totalVertexCount);
			for (hh_u32 indexi = 0; indexi < indexCount; ++indexi)
			{
				const hh_u32	srcVertex = srcIndices[srcIndexStart + indexi];

				hh_u32			dstVertex;
				if (vertexRemap[srcVertex] == 0)
				{
					const hh_u32	srci = srcVertex;
					const hh_u32	dsti = dstVertexCount;

					const FVector	&pos = srcPositions.VertexPosition(srci);
					dstPositions[dsti] = ToPk(pos) * scale;

					const FVector	&normal = srcVertices.VertexTangentZ(srci);
					const FVector	&tagent = srcVertices.VertexTangentX(srci);

					HH_TODO("Optim: get the TangentZ.w to put in our tangent.w");
					// Ugly recontruct binormal sign to put in tangent.w
					// New 4.12 tangent basis precision hides TangetZ.w behind more stuff
					// Should use: if (GetUseHighPrecisionTangentBasis()) { ... TangentZ.w ... } else { .... TangentZ.w .... }
					const FVector	&finalBinormal = srcVertices.VertexTangentY(srci);
					const FVector	baseBinormal = normal ^ tagent;
					const float		biNormalSign =  FVector::DotProduct(baseBinormal, finalBinormal) < 0.0 ? -1.0f : 1.0f;

					dstNormals[dsti] = ToPk(normal);
					dstTangents[dsti] = CFloat4(ToPk(tagent), biNormalSign);

					for (hh_u32 uvi = 0; uvi < uvCount; ++uvi)
					{
						const FVector2D		&uv = srcVertices.GetVertexUV(srci, uvi);
						dstUvs[uvi][dsti] = ToPk(uv);
					}

					if (!dstColors.Empty())
					{
						const FLinearColor	color = srcColors.VertexColor(srci).ReinterpretAsLinear();
						dstColors[dsti] = ToPk(color);
					}

					vertexRemap[srcVertex] = dstVertexCount + 1;
					dstVertex = dstVertexCount;
					++dstVertexCount;
				}
				else
				{
					dstVertex = vertexRemap[srcVertex] - 1;
				}

				dstIndices[indexi] = dstVertex;
			}

			HH_VERIFY(vstream.ExactResize(dstVertexCount)); // shrink only

			_FinalSetupMeshNew(meshNew);

			if (!HH_VERIFY(resMesh->AddBatch("kkette", meshNew).Valid()))
				continue;
		}
	}

	return resMesh;
}

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXMesh::WriteMesh()
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerMesh_UE::WriteMesh", POPCORNFX_UE_PROFILER_COLOR);

#if WITH_EDITOR
	Modify(true);
	ForceSetPackageDiry(this);
#endif // WITH_EDITOR

	if (m_ResourceMesh != null)
	{
		PopcornFX::SMeshWriteSettings		writeSettings;
		m_ResourceMesh->WriteToFile(PkPath(), &writeSettings, FPopcornFXPlugin::Get().FilePack());
	}
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

bool	UPopcornFXMesh::_ImportFile(const FString &filePath)
{
	return false;
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXMesh::OnFileUnload()
{
}

//----------------------------------------------------------------------------

void	UPopcornFXMesh::OnFileLoad()
{
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
