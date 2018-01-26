#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2008/05/03 12:08 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_SHAPES_HBO_H__
#define __GE_SHAPES_HBO_H__

#include "hh_geometrics/include/ge_shapes.h"
#include "hh_base_object/include/hbo_object.h"
#include "hh_engine_utils/include/eu_transforms_helper.h"
#include "hh_geometrics/include/ge_mesh_resource.h"

__PK_API_BEGIN
namespace HBO {
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT HBO_CLASS(CShapeDescriptorBase), public CBaseObject, public CBasicTransformsStorageHelper
{
private:
	HBO_FIELD(float,			Weight);
	HBO_FIELD_ABSTRACT(CFloat3,	Position/*, CBasicTransformsStorageHelper::m_Position*/);
	HBO_FIELD_ABSTRACT(CFloat3,	EulerOrientation/*, CBasicTransformsStorageHelper::m_EulerOrientation*/);
	HBO_FIELD(CString,			CustomName);

	bool						m_RebuildingTransforms;
	bool						_OnWeightModified();
	bool						_OnPositionModified();
	bool						_OnEulerOrientationModified();
	bool						_OnCustomNameModified();

protected:
	mutable ::PopcornFX::PShapeDescriptor	m_Descriptor;

	virtual bool				VirtualLink() override;
	virtual void				RebuildTransforms(CFloat4x4 &outMatrix) override;
	void						NotifyInstancesOfDescriptorModification();
	virtual void				VirtualOnShapeDescriptorPtrModified() {}

public:
	CShapeDescriptorBase();
	~CShapeDescriptorBase();

	virtual void				ScaleShape(const CFloat3 &/*scale*/) {}

	const PopcornFX::CShapeDescriptor		*Descriptor() const { return m_Descriptor.Get(); }
	virtual PopcornFX::PShapeDescriptor		AllocNewDescriptor() const;

	float						Volume() const { HH_ASSERT(m_Descriptor != null); return m_Descriptor->Volume(); }
	float						Surface() const { HH_ASSERT(m_Descriptor != null); return m_Descriptor->Surface(); }

	//static void				RayTraceShape(const CShapeDescriptor *abstractDescriptor, CEntityWithTransformsInstance *owner, const CRay &ray, CEntityTrace &result);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ShapeDescriptorBase);

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT HBO_CLASS(CShapeDescriptor), public CShapeDescriptorBase
{
private:
	// General
	HBO_FIELD(hh_i32,			ShapeType);

	// General (Box)
	HBO_FIELD(CFloat3,			BoxDimensions);

	// General (Sphere, Cylinder, Capsule, Cone, Complex Ellipsoid)
	HBO_FIELD(float,			Radius);

	// General (Cylinder, Capsule, Cone)
	HBO_FIELD(float,			Height);

	// General (Sphere, Cylinder, Capsule, Complex Ellipsoid)
	HBO_FIELD(float,			InnerRadius);	// do not add an 'InnerHeight' field. If a good reason pops up, ask me (Julien)
	HBO_FIELD(float,			NormalizedInnerRadius);

	// General (Complex Ellipsoid)
	HBO_FIELD(bool,				Hemisphere);
	HBO_FIELD(CFloat3,			NonUniformScale);

	// General (Mesh)
	HBO_FIELD(CString,			MeshResource);
	HBO_FIELD(CFloat3,			MeshScale);
	HBO_FIELD(hh_u32,			MeshSamplingMode);
	HBO_FIELD(hh_u32,			SubMeshIndex);
	HBO_FIELD(hh_u32,			DefaultUvStream);
	HBO_FIELD(hh_u32,			DefaultColorStream);
	HBO_FIELD(hh_u32,			DensityColorStream);
	HBO_FIELD(hh_u32,			DensityChannel);

	bool						_OnShapeTypeModified();
	bool						_OnBoxDimensionsModified();
	bool						_OnRadiusModified();
	bool						_OnInnerRadiusModified();
	bool						_OnNormalizedInnerRadiusModified();
	bool						_OnHeightModified();
	bool						_OnHemisphereModified();
	bool						_OnNUScaleModified();
	bool						_OnMeshScaleModified();
	bool						_OnMeshModified();
	bool						_OnMeshDefaultStreamModified();
	bool						_OnMeshSamplingModeModified();
	bool						_OnMeshDensityModified();

	EFieldVisibility			_BoxDimensions_VisibleInEditor() const;
	EFieldVisibility			_Radius_VisibleInEditor() const;
	EFieldVisibility			_InnerRadius_VisibleInEditor() const;
	EFieldVisibility			_Height_VisibleInEditor() const;
	EFieldVisibility			_Hemisphere_VisibleInEditor() const;
	EFieldVisibility			_NUScale_VisibleInEditor() const;
	EFieldVisibility			_Mesh_VisibleInEditor() const;
	EFieldVisibility			_MeshDensity_VisibleInEditor() const;

	bool						m_InsideModificationCallback;

	TResourcePtr<CResourceMesh>					m_Mesh;
#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_SURFACE != 0)
	mutable CMeshSurfaceSamplerStructuresRandom	*m_SurfaceSamplingStruct;
#endif
#if	(HH_GEOMETRICS_BUILD_MESH_SAMPLER_VOLUME != 0)
	mutable CMeshVolumeSamplerStructuresRandom	*m_VolumeSamplingStruct;
#endif

	void						_OnResourceReloaded(CResourceMesh *resource);

protected:
	virtual bool				VirtualLink() override;

	CSampleDataStream::EMode	_StreamSamplingMode() const;

	enum	EMeshSamplingDistribution
	{
		MSM_NaiveRandom = 0,
		MSM_UniformDistribution,
		MSM_WeightedDistribution,
	};

public:
	CShapeDescriptor();
	~CShapeDescriptor();

	virtual void							ScaleShape(const CFloat3 &scale) override;
	virtual PopcornFX::PShapeDescriptor		AllocNewDescriptor() const override;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor);

//----------------------------------------------------------------------------
//
//	Compound shape entities
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT HBO_CLASS(CShapeDescriptorCollection), public CShapeDescriptorBase
{
private:
	HBO_FIELD(TArray<CShapeDescriptorBase*>,	SubShapes);
	HBO_FIELD(hh_u32,							SamplingHeuristic);
	HBO_FIELD(bool,								UseSubShapeWeights);
	HBO_FIELD(bool,								PermutateMultiSamples);

	bool										_OnDescModifiedRebuild();
	bool										_OnDescModifiedNoRebuild();

protected:
	virtual bool								VirtualLink() override;

public:
	CShapeDescriptorCollection();
	~CShapeDescriptorCollection();

	virtual void								ScaleShape(const CFloat3 &scale) override;
	virtual PopcornFX::PShapeDescriptor			AllocNewDescriptor() const override;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ShapeDescriptorCollection);

//----------------------------------------------------------------------------
};
__PK_API_END

#endif // __GE_SHAPES_HBO_H__

