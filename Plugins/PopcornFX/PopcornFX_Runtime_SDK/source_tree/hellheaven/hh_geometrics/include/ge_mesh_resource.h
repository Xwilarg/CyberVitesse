#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2009/09/14 8:56 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_MESH_RESOURCE_H__
#define __GE_MESH_RESOURCE_H__

#include "hh_geometrics/include/ge_mesh.h"
#include "hh_geometrics/include/ge_colliders.h"
#include "hh_geometrics/include/ge_skeleton.h"
#include "hh_geometrics/include/ge_collidable_object.h"
#include "hh_geometrics/include/ge_coordinate_frame.h"
#include "hh_kernel/include/kr_resources.h"
#include "hh_kernel/include/kr_callbacks.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CMeshCodecProgressionNotifier;
class	CMeshProjection;
HH_FORWARD_DECLARE(MeshNew);
//HH_FORWARD_DECLARE(MeshImporterConfig);
struct	SMeshImportSettings;			// include ge_mesh_plugins.h
HH_FORWARD_DECLARE(MeshImportOut);		// include ge_mesh_plugins.h
class	CSampleDataStream;

namespace	MeshUtils
{
	class	CTetraVertexWeighted;
}

//----------------------------------------------------------------------------

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	class	CMeshSurfaceSamplerStructuresRandom;
	struct	SMeshUV2PCBuildConfig;
	class	CMeshSurfaceSamplerStructuresFromUV;
#endif

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)
	class	CMeshVolumeSamplerStructuresRandom;
	struct	STetrahedralMesh;
#endif

#if	(HH_GEOMETRICS_BUILD_KDTREE != 0)
	class	CMeshKdTree;
	struct	SMeshKdTreeBuildConfig;
#	if	(HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER != 0)
		class	CMeshKdTreeDebugger;
#	endif
#endif

//----------------------------------------------------------------------------

struct	SSamplerSourceOverride
{
	TStridedMemoryView<const CFloat3>	m_PositionsOverride;
	TStridedMemoryView<const CFloat3>	m_NormalsOverride;
	TStridedMemoryView<const CFloat4>	m_TangentsOverride;
	TStridedMemoryView<const CFloat3>	m_VelocitiesOverride;

	bool	Empty() const
	{
		return	m_PositionsOverride.Empty() &
				m_NormalsOverride.Empty() &
				m_TangentsOverride.Empty() &
				m_VelocitiesOverride.Empty();
	}
};

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CMeshNew : public CRefCountedObject
{
private:
//	friend class						CResourceMeshFileSerializer_V01;

	CMeshTriangleBatch					m_TriangleBatch;
	CString								m_Name;

	float								m_Surface;
	float								m_Volume;

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	Threads::CCriticalSection			m_SurfaceSamplingAccelStructsLock;
	CMeshSurfaceSamplerStructuresRandom	*m_SurfaceSamplingAccelStructs;

	Threads::CCriticalSection			m_SurfaceSamplingAccelStructsUVLock;
	CMeshSurfaceSamplerStructuresFromUV	*m_SurfaceSamplingAccelStructsUV;
#endif

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)
public:
	struct	SWeightedVertex
	{
		hh_u32	m_Indices[3];
		float	m_Weights[3];
	};
private:
	void								*m_TetrahedralIndices;	// uses m_IStream.IndexByteWidth()
	hh_u32								m_TetrahedralIndicesCount;

	CFloat3								*m_TetrahedralOtherPositions;
	CFloat3								*m_TetrahedralOtherNormals;
	CFloat3								*m_TetrahedralOtherVelocities;
	CFloat4								*m_TetrahedralOtherColors;
	hh_u32								m_TetrahedralOtherCount;

	SWeightedVertex						*m_TetrahedralWeightedVerts;
	hh_u32								m_TetrahedralWeightedVertsCount;

	Threads::CCriticalSection			m_VolumeSamplingAccelStructsLock;
	CMeshVolumeSamplerStructuresRandom	*m_VolumeSamplingAccelStructs;
#endif

#if	(HH_GEOMETRICS_BUILD_KDTREE != 0)
#	if	(HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER != 0)
	CMeshKdTreeDebugger					*m_KdTreeDebugger;
#	endif
protected:
	CMeshKdTree							*m_KdTree;
private:
#endif

#if (HH_GEOMETRICS_BUILD_MESHPROJECTION != 0)
	Threads::CCriticalSection			m_MeshProjectionLock;
	mutable CMeshProjection				*m_MeshProjection;	// mutable : we'll lazy-build this as needed when calling the MeshProjection() accessor method.
#endif

protected:
	CAABB								m_BBox;

	Threads::CCriticalSection			m_RuntimeStructsSetupLock;

	void								_FreeAllResources();

public:
	CMeshNew();
	CMeshNew(const CMeshTriangleBatch &other);
	~CMeshNew();

	bool								DeepCopy(const CMeshNew &other);

	CMeshTriangleBatch					&TriangleBatch() { return m_TriangleBatch; }
	const CMeshTriangleBatch			&TriangleBatch() const { return m_TriangleBatch; }

	hh_u32								TetrahedralIndicesCount() const { return m_TetrahedralIndicesCount; }
	hh_u32								TetrahedralOtherCount() const { return m_TetrahedralOtherCount; }
	hh_u32								TetrahedralWeightedVertsCount() const { return m_TetrahedralWeightedVertsCount; }

	void								*TetrahedralIndices() const { return m_TetrahedralIndices; }
	SWeightedVertex						*TetrahedralWeightedVerts() const { return m_TetrahedralWeightedVerts; }

	void								SetTetrahedralIndices(const void *indices, hh_u32 count);
	void								SetTetrahedralIndices_StealOwnership(void *indices, hh_u32 count);
	void								SetTetrahedralWeightedVertex(const TMemoryView<const SWeightedVertex> &vertices);
	void								SetTetrahedralWeightedVertex_StealOwnership(SWeightedVertex *vertices, hh_u32 count);

	TCallbackChainUnordered<void()>		m_OnMeshDataChanged;

	const CString						&Name() const { return m_Name; }
	void								SetName(const CString &name) { m_Name = name; }

	const CAABB							&BBox() const { return m_BBox; }
	void								RebuildBBox();
	void								UnsafeSetBBox(const CAABB &bbox) { m_BBox = bbox; }
#if	(HH_GEOMETRICS_BUILD_KDTREE != 0)
	void								SetKdTree_GiveOwnership(CMeshKdTree *kdTree);	// once called, the CMeshNew instance will own the KdTree, and call HH_DELETE() on it to release it when appropriate.
	bool								BuildKdTree(const SMeshKdTreeBuildConfig &buildConfig, bool forceRebuild);	// returns true if the mesh still has a kdtree after the method returns
#endif

	virtual void						SetupRuntimeStructsIFN(bool setupSamplingStruct = false);
	void								SetupSamplingStructsIFN(bool forceRebuild = false);

	bool								BuildTangentsIFN();			// only builds the tangents if the vertex buffer has no tangents

	void								RebuildSurfaceAndVolume();
	void								SetSurfaceAndVolume(float surface, float volume);
	float								RawVolume() const { return m_Volume; }
	float								RawSurface() const { return m_Surface; }
	float								Volume() const { return HHMax(0.0f, m_Volume); }
	float								Surface() const { return HHMax(0.0f, m_Surface); }

	hh_u32								GetSurfaceSamplingMask() const;
	hh_u32								GetVolumeSamplingMask() const;

	void								SampleVertex(const CSampleDataStream &samplePacket, const SSamplerSourceOverride *sourceOverride = null);

	void								ConvertToCoordinateSystem(ECoordinateFrame from, ECoordinateFrame to);

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	bool								SampleSurface(const CSampleDataStream &samplePacket, const SSamplerSourceOverride *sourceOverride = null);
	void								SampleSurface(const CMeshSurfaceSamplerStructuresRandom &samplingStructs, const CSampleDataStream &samplePacket, const SSamplerSourceOverride *sourceOverride = null);
	bool								SetupSurfaceSamplingAccelStructs(CGuid densityColorStreamId, hh_u32 densityColorChannelId, CMeshSurfaceSamplerStructuresRandom &outSurfSampler);
	void								SetupDefaultSurfaceSamplingAccelStructsIFN(bool forceRebuild);
	void								StealDefaultSurfaceSamplingAccelStructs(CMeshSurfaceSamplerStructuresRandom *accelStructs);
	CMeshSurfaceSamplerStructuresRandom	*DefaultSurfaceSamplingAccelStructs() const { return m_SurfaceSamplingAccelStructs; }

	bool								SampleSurfaceParametricCoordsFromUV(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const CFloat2> &uv/*, const SSamplerSourceOverride *sourceOverride = null*/);
	void								SampleSurfaceParametricCoordsFromUV(const CMeshSurfaceSamplerStructuresFromUV &samplingStructs, const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const CFloat2> &uv/*, const SSamplerSourceOverride *sourceOverride = null*/);
	bool								SetupSurfaceSamplingAccelStructsUV(const SMeshUV2PCBuildConfig &buildConfig, CMeshSurfaceSamplerStructuresFromUV &outSurfUVSampler);
	void								SetupDefaultSurfaceSamplingAccelStructsUVIFN(const SMeshUV2PCBuildConfig &buildConfig, bool forceRebuild);
	void								StealDefaultSurfaceSamplingAccelStructsUV(CMeshSurfaceSamplerStructuresFromUV *accelStructs);
	CMeshSurfaceSamplerStructuresFromUV	*DefaultSurfaceSamplingAccelStructsUV() const { return m_SurfaceSamplingAccelStructsUV; }
#endif

#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)
	bool								SampleVolume(const CSampleDataStream &samplePacket, const SSamplerSourceOverride *sourceOverride = null);
	void								SampleVolume(const CMeshVolumeSamplerStructuresRandom &samplingStructs, const CSampleDataStream &samplePacket, const SSamplerSourceOverride *sourceOverride = null);
	bool								SetupVolumeSamplingAccelStructs(CMeshVolumeSamplerStructuresRandom &outSurfSampler);
	void								SetupDefaultVolumeSamplingAccelStructsIFN(bool forceRebuild);
	void								StealDefaultVolumeSamplingAccelStructs(CMeshVolumeSamplerStructuresRandom *accelStructs);
	CMeshVolumeSamplerStructuresRandom	*DefaultVolumeSamplingAccelStructs() const { return m_VolumeSamplingAccelStructs; }

	bool								HasTetrahedralMeshing() const { return m_TetrahedralIndices != null; }
	void								GetTetrahedralMeshing(STetrahedralMesh &outMesh);
	bool								SetTetrahedralMeshing(const STetrahedralMesh &mesh);
#endif

#if	(HH_GEOMETRICS_BUILD_KDTREE != 0)
	// FIXME: make these more generic ? throw into ph_colliders?
	struct	STraceFlags
	{
		enum	EFlags
		{
			Flags_NeedSmoothNormals = 0x1,
			Flags_NeedUV = 0x2,
		};
		hh_u32	m_Flags;

		STraceFlags() : m_Flags(0) {}
	};

	struct	STraceReport
	{
		Colliders::STraceResult	m_Hit;
		CMeshNew				*m_HitMesh;
		CGuid					m_HitTriangleId;

		STraceReport() : m_HitMesh(null) {}
	};

	void						Trace(const STraceFlags &flags, const CFastRay &ray, STraceReport &result);
	void						TracePacket(const STraceFlags &flags,
											const Colliders::SRayPacket &packet,
											const Colliders::STracePacket &results,
											void *contactSurface = null,
											void *contactObject = CollidableObject::DEFAULT);

	void						TracePacket(const STraceFlags &flags,
											const Colliders::SRayPacket &packet,
											const Colliders::STracePacket &results,
											const CFloat4x4 *meshTransforms,
											const CFloat4x4 *meshInvTransforms,
											void *contactSurface = null,
											void *contactObject = CollidableObject::DEFAULT);

	const CMeshKdTree			*RawKdTree() const { return m_KdTree; }
	CMeshKdTree					*RawKdTree() { return m_KdTree; }

#if	(HH_GEOMETRICS_BUILD_KDTREE_DEBUGGER != 0)
	const CMeshKdTreeDebugger	*KdTreeDebugger() const { return m_KdTreeDebugger; }
	void						EnableKdTreeDebugger();
	void						DisableKdTreeDebugger();
#endif
#endif

#if (HH_GEOMETRICS_BUILD_MESHPROJECTION != 0)
	const CMeshProjection		*MeshProjection() const;
#endif

	Threads::CCriticalSection	&RuntimeStructsSetupLock() { return m_RuntimeStructsSetupLock; }
};

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(SparseMorph);
class	CBaseSkinningStreams;

class	HH_GEOMETRICS_EXPORT CResourceMeshBatch : public CRefCountedObject
{
private:
	TArray<PSparseMorph>	m_SparseMorphList;
protected:
	PMeshNew				m_RawMesh;
	CString					m_VirtualMaterialName;

public:
	explicit CResourceMeshBatch(PMeshNew batch);
	~CResourceMeshBatch();

	void	Swap(CResourceMeshBatch &other);

	CMeshNew					*RawMesh() const { return m_RawMesh.Get(); }

	const CString				&Material() const { return m_VirtualMaterialName; }
	void						SetMaterial(const CString &name) { m_VirtualMaterialName = name; }

	CGuid						AddSparseMorph(PSparseMorph sparseMorph);
	const TArray<PSparseMorph>	&SparseMorphList() const { return m_SparseMorphList; }

};
HH_DECLARE_REFPTRCLASS(ResourceMeshBatch);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CResourceMeshBatch_Skinned : public CResourceMeshBatch
{
public:
	explicit CResourceMeshBatch_Skinned(const PMeshNew &batch, const CBaseSkinningStreams *skinningStreams);
	~CResourceMeshBatch_Skinned();

	CBaseSkinningStreams		*m_OptimizedStreams;

	void	SetSkinningStreamsAndStealOwnership(CBaseSkinningStreams *streams);
	void	SetSkinningStreamsAndClone(const CBaseSkinningStreams *streams);

	void	Swap(CResourceMeshBatch_Skinned &other);
};
HH_DECLARE_REFPTRCLASS(ResourceMeshBatch_Skinned);

//----------------------------------------------------------------------------

struct	SMeshWriteSettings
{
	// Mesh Save config
	hh_u32					m_GeometryCompression; // Not Used yet
	SVStreamCode::EType		m_Positions;
	SVStreamCode::EType		m_Normals;
	SVStreamCode::EType		m_Tangents;
	SVStreamCode::EType		m_Texcoords;

	SMeshWriteSettings()
	:	m_GeometryCompression(0)
	,	m_Positions(SVStreamCode::Type_F32)
	,	m_Normals(SVStreamCode::Type_F32)
	,	m_Tangents(SVStreamCode::__MaxTypes)
	,	m_Texcoords(SVStreamCode::Type_F32)
	{
	}
};

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ResourceMesh);
class	HH_GEOMETRICS_EXPORT CResourceMesh : public CRefCountedObject
{
private:
	TArray<PResourceMeshBatch>					m_StaticBatchList;
	TArray<PResourceMeshBatch_Skinned>			m_SkinnedBatchList;
	PSkeleton									m_Skeleton;

protected:
	CString										m_Path;
	bool										m_Loaded;

	// overridable _VirtualLoad to let users to create their mesh loader
	virtual bool								_VirtualLoad(const CFilePackPath &path);	// if Loaded(), does nothing, otherwise, loads

public:
	CResourceMesh();
	virtual ~CResourceMesh();

	// hook these to be notified of hot-reloads
	TCallbackChainUnordered<void(CResourceMesh *self)>	m_OnReloading;
	TCallbackChainUnordered<void(CResourceMesh *self)>	m_OnReloaded;

	bool										Load(const CFilePackPath &path);
	bool										WriteToFile(const CString &path, const SMeshWriteSettings *writeSettings, PFilePack pack = null, ECoordinateFrame frame = CCoordinateFrame::GlobalFrame()) const;

#if	(HH_GEOMETRICS_BUILD_MESH_IMPORTERS != 0)
	static PMeshImportOut						ImportFromFile(CMeshCodecProgressionNotifier &notifier, const SMeshImportSettings *config = null);
#endif

	CGuid										AddBatch(const CString &virtualMaterial, const PMeshNew &mesh);
	CGuid										AddBatch(const CString &virtualMaterial, const PMeshNew &mesh, const CBaseSkinningStreams *skinner);
	CGuid										AddBatch(const PResourceMeshBatch &batch);
	CGuid										AddBatch(const PResourceMeshBatch_Skinned &batch);
	void										SetSkeleton(PSkeleton skeleton) { m_Skeleton = skeleton; }
	PSkeleton									Skeleton() const { return m_Skeleton; }

	const TArray<PResourceMeshBatch>			&StaticBatchList() const { return m_StaticBatchList; }
	const TArray<PResourceMeshBatch_Skinned>	&SkinnedBatchList() const { return m_SkinnedBatchList; }

	// only swaps the actual resource data, NOT the notifiers...
	void										Swap(CResourceMesh &other);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __GE_MESH_RESOURCE_H__
