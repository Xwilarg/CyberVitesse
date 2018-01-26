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

#ifndef __GE_SHAPES_H__
#define __GE_SHAPES_H__

#include "hh_maths/include/hh_maths_primitives.h"
#include "hh_engine_utils/include/eu_samplers.h"
#include "hh_geometrics/include/ge_mesh.h"
#include "hh_geometrics/include/ge_collidable_object.h"		// Colliders / STraceFilter (for shape intersections)

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(MeshNew);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor : public CRefCountedObject
{
public:
	enum	EShapeType
	{
		ShapeBox = 0,
		ShapeSphere,
		ShapeComplexEllipsoid,
		ShapeCylinder,
		ShapeCapsule,
		ShapeCone,
		ShapePlane,
		ShapeMesh,
		ShapeCollection,

		__MaxShapeTypes
	};

private:
	EShapeType						m_ShapeType;

protected:
	CShapeDescriptor(EShapeType type, hh_u32 packedSamplingMasks);

	hh_u32							m_SamplingMask;

	static HH_FORCEINLINE hh_u32	_PackSamplingMasks(hh_u32 surface, hh_u32 volume) { HH_ASSERT((surface & ~0xFFFF) == 0 && (volume & ~0xFFFF) == 0); return surface | (volume << 16); }
	static HH_FORCEINLINE hh_u32	_BroadcastSamplingMask(hh_u32 surfaceAndVolume) { HH_ASSERT((surfaceAndVolume & ~0xFFFF) == 0); return surfaceAndVolume | (surfaceAndVolume << 16); }
	static void						_TransformSamplePacketIFN(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);

public:
	virtual ~CShapeDescriptor();
	HH_FORCEINLINE EShapeType		ShapeType() const { return m_ShapeType; }

	HH_FORCEINLINE hh_u32			PackedSamplingMask() const { return m_SamplingMask; }
	HH_FORCEINLINE hh_u32			SurfaceSamplingMask() const { return m_SamplingMask & 0xFFFF; }
	HH_FORCEINLINE hh_u32			VolumeSamplingMask() const { return m_SamplingMask >> 16; }

	float							m_Weight;	// sampling weights
	CFloat4x4						m_LocalTransforms;

	virtual float					Volume() const;
	virtual float					Surface() const;
	virtual void					BuildLocalBox(CAABB &outBox) const = 0;		// the local box does not take 'm_LocalTransforms' into account.
	virtual void					BuildTransformedBox(CAABB &outBox) const;	// the transformed box takes 'm_LocalTransforms' into account.

	virtual bool					SampleSurfaceParametricCoordsFromUV(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const CFloat2> &uv) const;

	virtual bool					SampleVolume(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const = 0;
	virtual bool					SampleSurface(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const = 0;
	virtual bool					SampleVertex(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const; //= 0;

	// 'outResults' will contain 0 if the shape does not contain the sample, and -1 (0xFFFFFFFF) if it does
	virtual void					Contains(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const;

	// 'outResults' will contain the signed distance field to the shape.
	// negative values are inside the shape's volume, 0 is on the shape's surface, positive values are outside the shape.
	virtual void					DistanceField(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const;

	// 'outResults' will contain CFloat4(CFloat3(vectorFromPosToProjectedPos), distanceFromPosToProjectedPosition)
	// warning: check the return value, can fail !
	virtual bool					Project(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const;

	// 'outResults' will contain ParametricCoords
	// warning: check the return value, can fail !
	virtual bool					Project(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const;

	virtual bool					Project(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const;

	virtual void					RayTracePacket(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const;
};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor);

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Box : public CShapeDescriptor
{
protected:
	CFloat3					m_Dimensions;

public:
	CShapeDescriptor_Box();
	CShapeDescriptor_Box(const CFloat3 &dimensions);

	const CFloat3			&Dimensions() const { return m_Dimensions; }
	void					SetDimensions(const CFloat3 &d) { HH_ASSERT(All(d >= 0)); m_Dimensions = d; }

	static float			Volume(const CFloat3 &dimensions);
	static float			Surface(const CFloat3 &dimensions);

	virtual float			Volume() const override;
	virtual float			Surface() const override;
	virtual void			BuildLocalBox(CAABB &outBox) const override;

	static void				BuildParametricCoordsSurface(const CShapeDescriptor_Box *self, const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_i32> &faceId, const TStridedMemoryView<const CFloat2> &uv);
	static void				BuildParametricCoordsSurfaceRegular(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_u32> &sampleId, const TStridedMemoryView<const hh_u32> &sampleCount);
	static void				BuildParametricCoordsVolume(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const CFloat3> &normalizedPos);

	static bool				SampleVolume	(const CShapeDescriptor_Box *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool				SampleSurface	(const CShapeDescriptor_Box *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool			SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool			SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void			Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void			DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void			RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;
};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Box);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Sphere : public CShapeDescriptor
{
protected:
	float					m_Radius;
	float					m_InnerRadius;

public:
	CShapeDescriptor_Sphere();
	CShapeDescriptor_Sphere(float radius, float innerRadius = 0);

	HH_FORCEINLINE float	Radius() const { return m_Radius; }
	HH_FORCEINLINE float	InnerRadius() const { return m_InnerRadius; }

	void					Reset(float r, float ir) { HH_ASSERT(ir >= 0 && ir <= r); m_Radius = r; m_InnerRadius = ir; }
	void					SetRadius(float r) { HH_ASSERT(r >= 0); m_Radius = r; }
	void					SetInnerRadius(float ir) { HH_ASSERT(ir >= 0); m_InnerRadius = ir; }

	static float			Volume(float radius);
	static float			Volume(float radius, float innerRadius);
	static float			Surface(float radius);
	static float			Surface(float radius, float innerRadius);
	virtual float			Volume() const override;
	virtual float			Surface() const override;
	virtual void			BuildLocalBox(CAABB &outBox) const override;
	virtual void			BuildTransformedBox(CAABB &outBox) const override;

	static void				BuildParametricCoords(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const CFloat2> &thetaPhi, const TStridedMemoryView<const float> &radius);
	static void				BuildParametricCoordsRegular(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_u32> &sampleId, const TStridedMemoryView<const hh_u32> &sampleCount);

	static bool				SampleVolume	(const CShapeDescriptor_Sphere *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool				SampleSurface	(const CShapeDescriptor_Sphere *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool			SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool			SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void			Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void			DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void			RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;
};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Sphere);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_ComplexEllipsoid : public CShapeDescriptor
{
protected:
	float							m_Radius;
	float							m_InnerRadius;
	float							m_CutPlane;
	CFloat3							m_NonUniformScale;

public:
	CShapeDescriptor_ComplexEllipsoid();
	CShapeDescriptor_ComplexEllipsoid(float radius, float innerRadius = 0);
	CShapeDescriptor_ComplexEllipsoid(float radius, float innerRadius, float cutPlane, const CFloat3 &nonUniformScale);

	HH_FORCEINLINE float			Radius() const { return m_Radius; }
	HH_FORCEINLINE float			InnerRadius() const { return m_InnerRadius; }
	HH_FORCEINLINE float			CutPlane() const { return m_CutPlane; }
	HH_FORCEINLINE const CFloat3	&NonUniformScale() const { return m_NonUniformScale; }

	void							Reset(float r, float ir, float cp, const CFloat3 &nuScale) { HH_ASSERT(ir >= 0 && ir <= r && cp >= 0 && cp <= 1.0f && All(nuScale > 0)); m_Radius = r; m_InnerRadius = ir; m_CutPlane = cp; if (&m_NonUniformScale != &nuScale) { m_NonUniformScale = nuScale; } }
	void							SetRadius(float r) { HH_ASSERT(r >= 0); m_Radius = r; }
	void							SetInnerRadius(float ir) { HH_ASSERT(ir >= 0); m_InnerRadius = ir; }
	void							SetCutPlane(float cp) { HH_ASSERT(cp >= 0 && cp <= 1.0f); m_CutPlane = cp; }
	void							SetNonUniformScale(const CFloat3 &nuScale) { HH_ASSERT(All(nuScale > 0)); m_NonUniformScale = nuScale; }

	static float					Volume(float radius, float cutPlane, const CFloat3 &nuScale);
	static float					Volume(float radius, float innerRadius, float cutPlane, const CFloat3 &nuScale);

	static float					Surface(float radius, float cutPlane, const CFloat3 &nuScale);
	static float					Surface(float radius, float innerRadius, float cutPlane, const CFloat3 &nuScale);

	virtual float					Volume() const override;
	virtual float					Surface() const override;
	virtual void					BuildLocalBox(CAABB &outBox) const override;

	static bool						SampleVolume	(const CShapeDescriptor_ComplexEllipsoid *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool						SampleSurface	(const CShapeDescriptor_ComplexEllipsoid *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool					SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool					SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void					Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void					DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool					Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool					Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool					Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void					RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_ComplexEllipsoid);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Cylinder : public CShapeDescriptor
{
protected:
	float	m_Radius;
	float	m_Height;
	float	m_InnerRadius;

public:
	CShapeDescriptor_Cylinder();
	CShapeDescriptor_Cylinder(float radius, float height, float innerRadius = 0);

	HH_FORCEINLINE float	Radius() const { return m_Radius; }
	HH_FORCEINLINE float	InnerRadius() const { return m_InnerRadius; }
	HH_FORCEINLINE float	Height() const { return m_Height; }

	void					Reset(float r, float ir, float h) { HH_ASSERT(ir >= 0 && ir <= r && h >= 0); m_Radius = r; m_InnerRadius = ir; m_Height = h; }
	void					SetRadius(float r) { HH_ASSERT(r >= 0); m_Radius = r; }
	void					SetInnerRadius(float ir) { HH_ASSERT(ir >= 0); m_InnerRadius = ir; }
	void					SetHeight(float h) { HH_ASSERT(h >= 0); m_Height = h; }

	static float			Volume(float radius, float height);
	static float			Volume(float radius, float innerRadius, float height);
	static float			SurfaceUncapped(float radius, float height);
	static float			SurfaceUncapped(float radius, float innerRadius, float height);
	static float			SurfaceCaps(float radius);
	static float			Surface(float radius, float height);
	static float			Surface(float radius, float innerRadius, float height);
	virtual float			Volume() const override;
	virtual float			Surface() const override;
	virtual void			BuildLocalBox(CAABB &outBox) const override;

	static void				BuildParametricCoords(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const float> &height, const TStridedMemoryView<const float> &theta, const TStridedMemoryView<const float> &radius);
	static void				BuildParametricCoordsRegular(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_u32> &sampleId, const TStridedMemoryView<const hh_u32> &sampleCount);

	static bool				SampleVolume	(const CShapeDescriptor_Cylinder *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool				SampleSurface	(const CShapeDescriptor_Cylinder *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool			SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool			SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void			Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void			DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void			RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Cylinder);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Capsule : public CShapeDescriptor
{
protected:
	float	m_Radius;
	float	m_Height;
	float	m_InnerRadius;

public:
	CShapeDescriptor_Capsule();
	CShapeDescriptor_Capsule(float radius, float height, float innerRadius = 0);

	HH_FORCEINLINE float	Radius() const { return m_Radius; }
	HH_FORCEINLINE float	InnerRadius() const { return m_InnerRadius; }
	HH_FORCEINLINE float	Height() const { return m_Height; }

	void					Reset(float r, float ir, float h) { HH_ASSERT(ir >= 0 && ir <= r && h >= 0); m_Radius = r; m_InnerRadius = ir; m_Height = h;}
	void					SetRadius(float r) { HH_ASSERT(r >= 0); m_Radius = r; }
	void					SetInnerRadius(float ir) { HH_ASSERT(ir >= 0); m_InnerRadius = ir; }
	void					SetHeight(float h) { HH_ASSERT(h >= 0); m_Height = h; }

	static float			Volume(float radius, float innerRadius, float height);
	static float			Surface(float radius, float innerRadius, float height);
	virtual float			Volume() const override;
	virtual float			Surface() const override;
	virtual void			BuildLocalBox(CAABB &outBox) const override;

	static void				BuildParametricCoords(const CShapeDescriptor_Capsule *self, const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const float> &height, const TStridedMemoryView<const float> &theta, const TStridedMemoryView<const float> &radius);

	static bool				SampleVolume	(const CShapeDescriptor_Capsule *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool				SampleSurface	(const CShapeDescriptor_Capsule *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool			SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool			SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void			Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void			DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void			RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Capsule);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Cone : public CShapeDescriptor
{
protected:
	float	m_Radius;
	float	m_Height;

public:
	CShapeDescriptor_Cone();
	CShapeDescriptor_Cone(float radius, float height);

	HH_FORCEINLINE float	Radius() const { return m_Radius; }
	HH_FORCEINLINE float	Height() const { return m_Height; }

	void					Reset(float r, float h) { HH_ASSERT(r >= 0 && h >= 0); m_Radius = r; m_Height = h; }
	void					SetRadius(float r) { HH_ASSERT(r >= 0); m_Radius = r; }
	void					SetHeight(float h) { HH_ASSERT(h >= 0); m_Height = h; }

	static float			Volume(float radius, float height);
	static float			Surface(float radius, float height);
	static float			UncappedSurface(float radius, float height);
	virtual float			Volume() const override;
	virtual float			Surface() const override;
	virtual void			BuildLocalBox(CAABB &outBox) const override;

	static void				BuildParametricCoordsSurface(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const float> &height, const TStridedMemoryView<const float> &theta);
	static void				BuildParametricCoordsSurfaceRegular(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_u32> &sampleId, const TStridedMemoryView<const hh_u32> &sampleCount);
	static void				BuildParametricCoordsVolume(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const float> &height, const TStridedMemoryView<const float> &theta, const TStridedMemoryView<const float> &radius);

	static bool				SampleVolume	(const CShapeDescriptor_Cone *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool				SampleSurface	(const CShapeDescriptor_Cone *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool			SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool			SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void			Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void			DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void			RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Cone);

//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Plane : public CShapeDescriptor
{
protected:
public:
	CShapeDescriptor_Plane() : CShapeDescriptor(ShapePlane, 0) {}

	virtual float	Volume() const override { return 0.0f; }
	virtual float	Surface() const override { return TNumericTraits<float>::kPositiveInfinity; }
	virtual void	BuildLocalBox(CAABB &outBox) const override { outBox.SetMinMax(CFloat3(TNumericTraits<float>::kNegativeInfinity, 0, TNumericTraits<float>::kNegativeInfinity), CFloat3(TNumericTraits<float>::kPositiveInfinity, 0, TNumericTraits<float>::kPositiveInfinity)); }

	virtual bool	SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override { return SampleSurface(shapeTransforms, samplePacket); }
	virtual bool	SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override
	{
		(void)samplePacket;
		HH_ASSERT_NOT_REACHED();	// a plane is infinite... how the heck are we supposed to sample it? use an inverse distribution from the origin? 1 / RandomRange<float, 0, 1>() ? add an x/y extent and sample a rectangle on the plane ?
		return false;
	}

	virtual void			Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void			DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool			Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual void			RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Plane);

//----------------------------------------------------------------------------

class	CMeshProjection;
class	CMeshSurfaceSamplerStructuresRandom;
class	CMeshVolumeSamplerStructuresRandom;
struct	SSamplerSourceOverride;

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Mesh : public CShapeDescriptor
{
protected:
	PMeshNew									m_Mesh;
	CFloat3										m_Scale;
	CSampleDataStream::EMode					m_SamplingMode;
	hh_u32										m_DefaultUvStream;
	CGuid										m_DefaultUvMeshStreamIndex;
	hh_u32										m_DefaultColorStream;
	CGuid										m_DefaultColorMeshStreamIndex;
	SSamplerSourceOverride						*m_SampleSourceOverride;
#if	!defined(HH_RETAIL)
	CString										m_DebugMeshName;
#endif

	const CMeshSurfaceSamplerStructuresRandom	*m_SurfaceSamplingStructs;
	const CMeshVolumeSamplerStructuresRandom	*m_VolumeSamplingStructs;

	void										_ResolveDefaultStreams();

public:
	CShapeDescriptor_Mesh();
	CShapeDescriptor_Mesh(	const PMeshNew				&mesh,
							const CFloat3				&scale = CFloat3::ONE,
							CSampleDataStream::EMode	samplingMode = CSampleDataStream::UniformDistribution,
							hh_u32						defaultUvStream = 0,
							hh_u32						defaultColorStream = 0);
	~CShapeDescriptor_Mesh();

	HH_FORCEINLINE const PMeshNew								&Mesh() const { return m_Mesh; }
	HH_FORCEINLINE const CFloat3								&Scale() const { return m_Scale; }
	HH_FORCEINLINE CSampleDataStream::EMode						SamplingMode() const { return m_SamplingMode; }
	HH_FORCEINLINE hh_u32										DefaultUvStream() const { return m_DefaultUvStream; }
	HH_FORCEINLINE hh_u32										DefaultColorStream() const { return m_DefaultColorStream; }
	HH_FORCEINLINE const CMeshSurfaceSamplerStructuresRandom	*SurfaceSamplingStructs() const { return m_SurfaceSamplingStructs; }
	HH_FORCEINLINE const CMeshVolumeSamplerStructuresRandom		*VolumeSamplingStructs() const { return m_VolumeSamplingStructs; }

	hh_u32						VertexCount() const;
	hh_u32						TriangleCount() const;
	hh_u32						TetraCount() const;

	void						Reset(const PMeshNew &m, const CFloat3 &scale, CSampleDataStream::EMode sm = CSampleDataStream::UniformDistribution) { SetSamplingMode(sm); SetScale(scale); SetMesh(m); }
	void						SetMesh(const PMeshNew &m, SSamplerSourceOverride *sampleSourceOverride = null);
	void						SetScale(const CFloat3 &scale) { m_Scale = scale; }
	void						SetSamplingMode(CSampleDataStream::EMode sm) { m_SamplingMode = sm; }
	void						SetDefaultStreams(hh_u32 defaultUvStream, hh_u32 defaultColorStream);
	void						SetSamplingStructs(const CMeshSurfaceSamplerStructuresRandom *surfaceStruct, const CMeshVolumeSamplerStructuresRandom *volumeStruct);
#if	!defined(HH_RETAIL)
	void						SetDebugMeshName(const CString &debugName) { m_DebugMeshName = debugName; }
#else
	void						SetDebugMeshName(const CString &debugName) {}
#endif

	static float				Volume(const PMeshNew &mesh);
	static float				Surface(const PMeshNew &mesh);
	virtual float				Volume() const override;
	virtual float				Surface() const override;
	virtual void				BuildLocalBox(CAABB &outBox) const override;
	const CMeshProjection		*MeshProjection() const;

	static void					BuildParametricCoordsVolume(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_i32> &tetraId, const TStridedMemoryView<const CFloat3> &uvw);
	static void					BuildParametricCoordsSurface(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_i32> &triangleId, const TStridedMemoryView<const CFloat2> &uv);
	static void					BuildParametricCoordsVertex(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const hh_i32> &vertexId);

	virtual bool				SampleSurfaceParametricCoordsFromUV(const TStridedMemoryView<CInt3> &outPCoords, const TStridedMemoryView<const CFloat2> &uv) const override;

	static bool					SampleVolume	(const CShapeDescriptor_Mesh *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool					SampleSurface	(const CShapeDescriptor_Mesh *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool					SampleVertex	(const CShapeDescriptor_Mesh *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);

	virtual bool				SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool				SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool				SampleVertex	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void				DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;

	virtual bool				Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual bool				Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CInt3> &outResults) const override;
	virtual bool				Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outProjAndDist, const TStridedMemoryView<CInt3> &outPCoords) const override;
	virtual void				RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Mesh);

//----------------------------------------------------------------------------
//
//	Compound shapes
//
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CShapeDescriptor_Collection : public CShapeDescriptor
{
protected:
	void									BuildShapeWeights(const TMemoryView<float> &outWeightList) const;
	static bool								GenericSample(const CShapeDescriptor_Collection *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket, bool volumeInsteadOfSurface);
	static bool								GenericSample_N(const CShapeDescriptor_Collection *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket, bool volumeInsteadOfSurface);

	TArray<PCShapeDescriptor>				m_SubShapes;

public:
	enum	ESamplingHeuristic
	{
		NoWeight = 0,
		WeightWithVolume,
		WeightWithSurface,

		__MaxSamplingHeuristics
	};

	CShapeDescriptor_Collection();

	ESamplingHeuristic						m_SamplingHeuristic;
	bool									m_UseSubShapeWeights;
	bool									m_PermutateMultiSamples;

	void									ReserveSubShapes(hh_u32 subShapeCount);
	bool									AddSubShape(const PCShapeDescriptor &shape) { return AddSubShape(shape.Get()); }
	bool									AddSubShape(const CShapeDescriptor *shape);
	TMemoryView<const PCShapeDescriptor>	SubShapes() const { return m_SubShapes; }

	virtual float							Volume() const override;
	virtual float							Surface() const override;
	virtual void							BuildLocalBox(CAABB &outBox) const override;
	virtual void							BuildTransformedBox(CAABB &outBox) const override;

	static bool								SampleVolume	(const CShapeDescriptor_Collection *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	static bool								SampleSurface	(const CShapeDescriptor_Collection *self, const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket);
	virtual bool							SampleVolume	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;
	virtual bool							SampleSurface	(const CFloat4x4 &shapeTransforms, const CSampleDataStream &samplePacket) const override;

	virtual void							Contains		(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<hh_i32> &outResults) const override;
	virtual void							DistanceField	(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<float> &outResults) const override;
	virtual bool							Project			(const CFloat4x4 &shapeTransforms, const TStridedMemoryView<const CFloat3> &pos, const TStridedMemoryView<CFloat4> &outResults) const override;
	virtual void							RayTracePacket	(const CFloat4x4 &shapeTransforms, const Colliders::SRayPacket &packet, const Colliders::STracePacket &results) const override;

};
HH_DECLARE_REFPTRCLASS(ShapeDescriptor_Collection);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __GE_SHAPES_H__
