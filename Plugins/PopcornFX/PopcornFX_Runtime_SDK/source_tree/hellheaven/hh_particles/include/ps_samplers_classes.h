#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2010/05/26 15:25 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PS_PARTICLES_SAMPLERS_H__
#define __PS_PARTICLES_SAMPLERS_H__

#include "hh_particles/include/ps_samplers.h"
#include "hh_particles/include/ps_config.h"

#include <hh_kernel/include/kr_threads.h>
#include <hh_geometrics/include/ge_shapes_hbo.h>
#include <hh_engine_utils/include/eu_samplers.h>
#include <hh_engine_utils/include/eu_samplers_curve.h>
#include <hh_imaging/include/im_image.h>
#include <hh_imaging/include/im_samplers.h>
#include <hh_geometrics/include/ge_collidable_object.h>		// Colliders / STraceFilter (for shape intersections in CParticleSamplerShape)
#include <hh_kernel/include/kr_rtti.h>
#include <hh_kernel/include/kr_resources.h>	// for TResourcePtr<>
#include <hh_compiler/include/cp_config.h>	// PK_COMPILER_BUILD_COMPILER

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Curves
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_CURVE != 0)

class	CParticleSamplerDescriptor_Curve : public CParticleSamplerDescriptor
{
public:
	static hh_u32	SamplerTypeID() { static hh_u32 id = _GetNextSamplerTypeID(); return id; }
	CParticleSamplerDescriptor_Curve(const CCurveSamplerInterface *curve0 = null, const CCurveSamplerInterface *curve1 = null, const CCurveSamplerInterface *CDF = null) : CParticleSamplerDescriptor(SamplerTypeID()), m_Curve0(curve0), m_Curve1(curve1), m_CDF(CDF) {}

	const CCurveSamplerInterface	*m_Curve0;
	const CCurveSamplerInterface	*m_Curve1;
	const CCurveSamplerInterface	*m_CDF;
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDescriptor_Curve);

//----------------------------------------------------------------------------

class	CParticleSamplerCPU_Curve;
class	CParticleSamplerCPU_DoubleCurve;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerCurve), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerCurve>
{
public:
	enum	ESamplingQuality
	{
		Quality_Exact = 0,
		Quality_High,
		Quality_Medium,
		Quality_Low,
	};
private:
	typedef TArray<float, TArrayAligned16>	TAlignedFloatArray;

	HBO_FIELD(hh_u32,						ValueType);		// Do not use 'ValueType()' and 'SetValueType()' use 'Order()' and 'SetOrder()' from now on.
	HBO_FIELD_ABSTRACT(hh_u32,				Interpolator);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	Times);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatValues);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatTangents);
	HBO_FIELD_ABSTRACT(CFloat4,				MinLimits);
	HBO_FIELD_ABSTRACT(CFloat4,				MaxLimits);
	HBO_FIELD(hh_u32,						Quality);
	HBO_FIELD(bool,							IsProbabilityCurve);

	bool									_OnValueTypeModified();
	bool									_OnCurveInterpolatorModified(CCurveDescriptor &curve);
	bool									_OnLimitsModified(CCurveDescriptor &curve);
	bool									_OnCurveInterpolatorModified();
	bool									_OnDataModified();
	bool									_OnLimitsModified();
	bool									_OnIsProbabilityCurveModified();

protected:
	CCurveDescriptor						m_Curve0;
	CCurveDescriptor						*m_CDF;
	CFloat4									m_DomainIntegral;
	CParticleSamplerCPU_Curve				*m_CpuSamplerForStaticEval;
	bool									m_IsUselessCDF;

	virtual void							SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;

	virtual bool							VirtualLink() override;
	virtual void							VirtualUnlink() override;
	bool									_ConstructCDF();
	void									_RefreshCDF();

	void									_OnOrderChanged(CCurveDescriptor *curve);
	const CParticleSamplerDescriptor_Curve	*_GetDefaultCurveDescriptorAndCreateIFN(bool forceRebuild = false);

public:
	CParticleSamplerCurve();
	~CParticleSamplerCurve();

	HH_FORCEINLINE hh_u32					Order() const { return ValueType(); }	// TODO: make an upgrader and rename the 'ValueType' HBO field.
	HH_FORCEINLINE bool						SetOrder(hh_u32 order) { return SetValueType(order); }	// TODO: make an upgrader and rename the 'ValueType' HBO field.

	void									SetData(const TMemoryView<const float> &times,		// count = n
													const TMemoryView<const float> &values,		// count = n * order
													const TMemoryView<const float> &tangents);	// count = n * order * 2 OR empty if linear curve

	const CCurveDescriptor					&Curve0Sampler() const { return m_Curve0; }
	CCurveDescriptor						&Curve0Sampler() { return m_Curve0; }

	const CCurveDescriptor					*CurveCDFSampler() const { return m_CDF; }
	CCurveDescriptor						*CurveCDFSampler() { return m_CDF; }

	const CFloat4							&DomainIntegral() const { return m_DomainIntegral; }

#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool							DefineCommonType(CCompilerContext &context);
	virtual void							DefineType(CCompilerContext &context) const override;
	virtual void							DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

	hh_u32									FullTypeDimension() const;
	EBaseTypeID								FullType() const;

	mutable TCallbackChainUnordered<void(void)>	m_OnDataModified;
	mutable TCallbackChainUnordered<void(void)>	m_OnLimitsModified;
	mutable TCallbackChainUnordered<void(void)>	m_OnIsProbabilityCurveModified;

	static CStringView						GetSamplerTypeName(EBaseTypeID sampleType, bool canChangeAtRuntime, bool needsAccessToInstance, bool isProbabilityCurve);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerCurve);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerDoubleCurve), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerDoubleCurve>
{
private:
	typedef TArray<float, TArrayAligned16>	TAlignedFloatArray;

	HBO_FIELD(hh_u32,						ValueType);		// Do not use 'ValueType()' and 'SetValueType()' use 'Order()' and 'SetOrder()' from now on.
	HBO_FIELD_ABSTRACT(hh_u32,				Interpolator);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	Times);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatValues);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatTangents);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	Times1);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatValues1);
	HBO_FIELD_ABSTRACT(TAlignedFloatArray,	FloatTangents1);

	HBO_FIELD_ABSTRACT(CFloat4,				MinLimits);
	HBO_FIELD_ABSTRACT(CFloat4,				MaxLimits);

	bool									_OnValueTypeModified();
	bool									_OnCurveInterpolatorModified(CCurveDescriptor &curve);
	bool									_OnCurveInterpolatorModified();
	bool									_OnDataModified();
	bool									_OnData1Modified();
	bool									_OnLimitsModified();

protected:
	CCurveDescriptor						m_Curve0;
	CCurveDescriptor						m_Curve1;
	CFloat4									m_DomainIntegral;
	CFloat4									m_DomainIntegral1;
	CParticleSamplerCPU_DoubleCurve			*m_CpuSamplerForStaticEval;

	virtual bool							VirtualLink() override;
	virtual void							VirtualUnlink() override;

	void									_OnOrderChanged(CCurveDescriptor *curve);
	const CParticleSamplerDescriptor_Curve	*_GetDefaultCurveDescriptorAndCreateIFN(bool forceRebuild = false);

public:
	CParticleSamplerDoubleCurve();
	~CParticleSamplerDoubleCurve();

	HH_FORCEINLINE hh_u32					Order() const { return ValueType(); }	// TODO: make an upgrader and rename the 'ValueType' HBO field.
	HH_FORCEINLINE bool						SetOrder(hh_u32 order) { return SetValueType(order); }	// TODO: make an upgrader and rename the 'ValueType' HBO field.

	void									SetData(const TMemoryView<const float> &times,		// count = n
													const TMemoryView<const float> &values,		// count = n * order
													const TMemoryView<const float> &tangents);	// count = n * order * 2 OR empty if linear curve
	void									SetData1(	const TMemoryView<const float> &times,		// count = n
														const TMemoryView<const float> &values,		// count = n * order
														const TMemoryView<const float> &tangents);	// count = n * order * 2 OR empty if linear curve

	const CCurveDescriptor					&Curve0Sampler() const { return m_Curve0; }
	CCurveDescriptor						&Curve0Sampler() { return m_Curve0; }
	const CCurveDescriptor					&Curve1Sampler() const { return m_Curve1; }
	CCurveDescriptor						&Curve1Sampler() { return m_Curve1; }

	const CFloat4							&DomainIntegral() const { return m_DomainIntegral; }
	const CFloat4							&DomainIntegral1() const { return m_DomainIntegral1; }

#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool							DefineCommonType(CCompilerContext &context);
	virtual void							DefineType(CCompilerContext &context) const override;
	virtual void							DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

	hh_u32									FullTypeDimension() const;
	EBaseTypeID								FullType() const;

	mutable TCallbackChainUnordered<void(void)>	m_OnDataModified;
	mutable TCallbackChainUnordered<void(void)>	m_OnLimitsModified;

	static CStringView						GetDoubleSamplerTypeName(EBaseTypeID sampleType, bool canChangeAtRuntime, bool needsAccessToInstance);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDoubleCurve);

#endif

//----------------------------------------------------------------------------
//
//	Shapes
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_SHAPE != 0)

class	CParticleSamplerDescriptor_Shape : public CParticleSamplerDescriptor
{
public:
	static hh_u32	SamplerTypeID() { static hh_u32 id = _GetNextSamplerTypeID(); return id; }

	CParticleSamplerDescriptor_Shape(const CShapeDescriptor *desc = null)
	:	CParticleSamplerDescriptor(SamplerTypeID())
	,	m_Shape(desc)
	,	m_WorldTr_Current(null)
	,	m_WorldTr_Previous(null)
	,	m_Angular_Velocity(null)
	,	m_Linear_Velocity(null)
	{
	}

	PCShapeDescriptor	m_Shape;
	const CFloat4x4		*m_WorldTr_Current;
	const CFloat4x4		*m_WorldTr_Previous;
	const CFloat3		*m_Angular_Velocity;
	const CFloat3		*m_Linear_Velocity;
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDescriptor_Shape);

//----------------------------------------------------------------------------

class	CParticleSamplerCPU_Shape;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerShape), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerShape>
{
private:
	HBO_FIELD(HBO::CShapeDescriptorBase*,	Shape);
	HBO_FIELD(hh_u32,						SampleDimensionality);
	HBO_FIELD(bool,							TransformTranslate);
	HBO_FIELD(bool,							TransformRotate);

	HBO_FIELD(hh_u32,						FixedLocationsCount);
	HBO_FIELD(hh_u32,						FixedLocationsSeed);

	bool									_OnFixedLocationsModified();
	bool									_OnShapeDescriptorModified();

public:
	class	CFixedLocationsContainer
	{
	public:
		struct	SField
		{
			SStridedMemoryViewRawStorage	m_MemViewStorage;
			EBaseTypeID						m_Type;

			bool			Empty() const { return m_MemViewStorage.m_RawDataPtr == null; }
		};

		hh_u32													m_StreamSize;
		hh_u32													m_ActiveMask;
		TStaticArray<SField, CSampleDataStream::__MaxStreams>	m_Fields;

		CFixedLocationsContainer();
		~CFixedLocationsContainer();

		hh_u32				Count() const { return m_StreamSize; }
		void				Clean();
		bool				Empty() const;
		void				RebuildMask();
		bool				Setup(hh_u32 mask, hh_u32 count);
	};

protected:
	bool											m_UsingFixedLocations;
	CFixedLocationsContainer						m_FixedLocations;
	hh_u32											m_FixedLocations_CurrentSeed;
	CParticleSamplerCPU_Shape						*m_CpuSamplerForStaticEval;

	virtual	bool									VirtualLink() override;
	virtual void									VirtualUnlink() override;
			bool									_RebuildFixedLocationsIFN();

			const CParticleSamplerDescriptor_Shape	*_GetDefaultShapeDescriptorAndCreateIFN(bool forceRebuild = false);

public:
	CParticleSamplerShape();
	~CParticleSamplerShape();

			hh_u32							GetSamplingStreamMask() const;

	virtual void							SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool							DefineCommonType(CCompilerContext &context);
	virtual	void							DefineType(CCompilerContext &context) const override;
	virtual	void							DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

			bool							UsingFixedLocations() const { return m_UsingFixedLocations; }
			const CFixedLocationsContainer	&FixedLocations() const { return m_FixedLocations; }

	static CStringView						GetSamplerTypeName(char (&dstBuffer)[32], bool usesFixedLocations, bool canChangeAtRuntime, bool needsAccessToInstance, hh_u32 sampleDimensionality);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerShape);

#endif

//----------------------------------------------------------------------------
//
//	AnimTrack
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_ANIMTRACK != 0)

class	CParticleSamplerCPU_AnimTrack;

//----------------------------------------------------------------------------

class	CParticleSamplerDescriptor_AnimTrack : public CParticleSamplerDescriptor
{
public:
	struct	SPathDefinition
	{
		PCSamplerCurve	m_Position;
		PCSamplerCurve	m_Rotation;
		PCSamplerCurve	m_Scale;

		SPathDefinition(const CSamplerCurve *position = null, const CSamplerCurve *rotation = null, const CSamplerCurve *scale = null);
	};

	static hh_u32	SamplerTypeID() { static hh_u32 id = _GetNextSamplerTypeID(); return id; }
	CParticleSamplerDescriptor_AnimTrack();

	TArray<SPathDefinition>			m_Tracks;
	const CFloat4x4					*m_TrackTransforms;

	TMemoryView<const SPathDefinition>	AnimTracks() const { return m_Tracks; }
	const CFloat4x4						&TrackTransforms() const { HH_ASSERT(m_TrackTransforms != null); return *m_TrackTransforms; }

	bool						_Setup(const PAnimationClip &resource);	// returns false on allocation failure
	bool						_LoadTracksFromClip(const CAnimationClip *animClip);
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDescriptor_AnimTrack);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerAnimTrack), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerAnimTrack>, public CBasicTransformsStorageHelper
{
public:
private:
	HBO_FIELD(CString,						AnimResource);
	HBO_FIELD(hh_u32,						AnimTrackIndex);
	HBO_FIELD(CParticleSamplerShape*,		Sampler);

	HBO_FIELD(bool,							TransformTranslate);
	HBO_FIELD(bool,							TransformRotate);
	HBO_FIELD(bool,							TransformScale);

	HBO_FIELD_ABSTRACT(CFloat3,				Position);
	HBO_FIELD_ABSTRACT(CFloat3,				EulerOrientation);
	HBO_FIELD(CFloat3,						Scale);

	bool									_OnAnimResourceModified();
	bool									_OnSamplerModified();
	bool									_OnTransformsPositionModified();
	bool									_OnTransformsOrientationModified();
	bool									_OnTransformsScaleModified();

protected:
	PAnimationClip							m_AnimClipResource;
	CFloat4x4								m_TrackTransforms;

	CParticleSamplerCPU_AnimTrack			*m_CpuSamplerForStaticEval;

	void									_RebuildMatrices();

	virtual void							RebuildTransforms(CFloat4x4 &outMatrix) override; // CBasicTransformsStorageHelper

protected:
	virtual	bool							VirtualLink() override;
	virtual void							VirtualUnlink() override;

	const CParticleSamplerDescriptor_AnimTrack	*_GetDefaultAnimTrackDescriptorAndCreateIFN(bool forceRebuild = false);

public:
	CParticleSamplerAnimTrack();
	~CParticleSamplerAnimTrack();

			hh_u32							GetSamplingStreamMask(bool &usesFixedLocations) const;

	virtual void							SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool							DefineCommonType(CCompilerContext &context);
	virtual	void							DefineType(CCompilerContext &context) const override;
	virtual	void							DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

	static CStringView						GetSamplerTypeName(char (&dstBuffer)[32], hh_u32 shapeStreamMask, bool usesFixedLocations, bool canChangeAtRuntime, bool needsAccessToInstance, hh_u32 sampleDimensionality);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerAnimTrack);

#endif

//----------------------------------------------------------------------------
//
//	Textures
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_TEXTURE != 0)

class	CParticleSamplerDescriptor_Image : public CParticleSamplerDescriptor
{
public:
	static hh_u32	SamplerTypeID() { static hh_u32 id = _GetNextSamplerTypeID(); return id; }
	CParticleSamplerDescriptor_Image(const CImageSamplerInterface *desc = null)
	:	CParticleSamplerDescriptor(SamplerTypeID())
	,	m_Sampler(desc)
	,	m_DefaultFilter(CImageSamplerInterface::FilterType_Point)
	,	m_DefaultTCMode(CImageSamplerInterface::TexcoordMode_Wrap)
	{}

	const CImageSamplerInterface			*m_Sampler;
	CImageSamplerInterface::EFilterType		m_DefaultFilter;
	CImageSamplerInterface::ETexcoordMode	m_DefaultTCMode;
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDescriptor_Image);

//----------------------------------------------------------------------------

class	CRectangleList;
class	CParticleSamplerCPU_Texture;

// Image CPU Container, should move to CPU Sampler stuff only
HH_FORWARD_DECLARE(ImageSamplerContainer);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerTexture), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerTexture>
{
public:
	enum	ESampleGammaSpace
	{
		SampleGammaLinear = 0,
		SampleGammaSRGB,
		SampleGammaLinearToSRGB,
		SampleGammaSRGBToLinear,
		__MaxSampleGammaSpaces
	};
	enum	ESampleMode
	{
		SampleModeRegular = 0x1,
		SampleModeDensity = 0x2,
		SampleModeBoth = SampleModeRegular | SampleModeDensity,
	};
	enum	EDensitySrc
	{
		DensitySrc_Red,
		DensitySrc_Green,
		DensitySrc_Blue,
		DensitySrc_Alpha,
		DensitySrc_RGBA_Avg,
		__MaxDensitySrc
	};

private:
//	HBO_FIELD(CString,				OverrideName);
	HBO_FIELD(CString,				TextureResource);
	HBO_FIELD(CString,				AtlasDefinition);
	HBO_FIELD(hh_u32,				ScriptOutputType);
	HBO_FIELD(hh_u32,				DefaultFilter);
	HBO_FIELD(hh_u32,				DefaultWrap);
	HBO_FIELD(bool,					SampleRawValues);
	HBO_FIELD(hh_u32,				Sampler);
	HBO_FIELD(hh_u32,				SampleGammaSpace);
	HBO_FIELD(float,				DensityPower);
	HBO_FIELD(hh_u32,				DensitySrc);
	HBO_FIELD(CFloat4,				DensityRGBAWeights);

	bool							_OnTextureResourceModified(const CString &oldValue);
	bool							_OnAtlasDefinitionModified();
	bool							_OnDeclarationModified();
	bool							_OnSamplerModified();
	bool							_OnAddressingModified();
	bool							_OnScriptOutputTypeModified();
	bool							_InvalidateImageResource();
	EFieldVisibility				_Regular_FieldsVisibility() const;
	EFieldVisibility				_PDF_FieldsVisibility() const;
	EFieldVisibility				_PDF_RGBAWeightsVisibility() const;
	EFieldVisibility				_Gamma_FieldsVisibility() const;
	void							_OnRectListReloaded(CRectangleList *rectList);
	void							_OnImageResourceReloaded(CImage *image);

protected:
	TResourcePtr<CImage>			m_Resource;
	TResourcePtr<CRectangleList>	m_RectangleList;

	CImageSurface					m_Surface;
	PImageSamplerContainer			m_SamplerContainer;

	bool							m_IsTextureResourcePowerOfTwo;
	bool							m_IsOldTextureResourcePowerOfTwo;

	CParticleSamplerCPU_Texture		*m_CpuSamplerForStaticEval;

	virtual bool					VirtualLink() override;
	virtual void					VirtualUnlink() override;

	bool							_LoadImageResource(bool forceReload);
	bool							_IsTextureResourcePowerOfTwo() const;
	CImage::EFormat					_GetInternalImageFormatSrc(CImage::EFormat srcFormat) const;
	CImage::EFormat					_GetInternalImageFormatDst(CImage::EFormat srcFormat) const;
	bool							_ReloadAtlasDefinition(const CString &path, bool reloadImageIFN);
	hh_u32							_GetSampleDimension() const;

	const CParticleSamplerDescriptor_Image	*_GetDefaultImageDescriptorAndCreateIFN(bool forceRebuild = false);

public:
	CParticleSamplerTexture();
	~CParticleSamplerTexture();

	virtual void						SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool						DefineCommonType(CCompilerContext &context);
	virtual	void						DefineType(CCompilerContext &context) const override;
	virtual	void						DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

			CImageSurface				&Surface() { return m_Surface; }
	const	CImageSurface				&Surface() const { return m_Surface; }

	void								SetSurface(const CImageSurface &surface);

	const TResourcePtr<CImage>			&Resource() const { return m_Resource; }
	const TResourcePtr<CRectangleList>	&RectangleList() const { return m_RectangleList; }

	const PImageSamplerContainer		&SamplerContainer() const { return m_SamplerContainer; }

	static CStringView					GetSamplerTypeName(char (&dstBuffer)[32], hh_u32 inputDimensionality, hh_u32 outputDimensionality, hh_u32 samplerMode, bool atlasSampling, bool canChangeAtRuntime, bool needsAccessToInstance);

	mutable TCallbackChainUnordered<void(const CBaseObject*)>		m_TextureReloaded;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerTexture);

#endif

//----------------------------------------------------------------------------
//
//	Vector fields
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_TURBULENCE != 0)

class	CRotatableFastNoise3;
template<typename _NoiseBasis>
class	TVector3Noise;

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerProceduralTurbulence), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerProceduralTurbulence>
{
public:
	enum	EInterpolator
	{
		Interpolator_Linear = 0,
		Interpolator_Cubic,
		Interpolator_Quintic,
		__MaxInterpolators
	};
	enum	EKnownFields
	{
		Field_Potential = 0,
		Field_Curl,
		__MaxKnownFields
	};

private:
	// Geometry
	HBO_FIELD(float,							GlobalScale);
	HBO_FIELD(float,							Wavelength);
	HBO_FIELD(float,							Strength);
	HBO_FIELD(hh_u32,							Octaves);
	HBO_FIELD(float,							Lacunarity);
	HBO_FIELD(float,							Gain);
	HBO_FIELD(hh_u32,							Interpolator);
	HBO_FIELD(hh_u32,							DefaultSampledField);

	// Animation
	HBO_FIELD(float,							TimeScale);
	HBO_FIELD(float,							TimeBase);
	HBO_FIELD(float,							TimeRandomVariation);

	// Advanced
	HBO_FIELD(float,							FlowFactor);
	HBO_FIELD(float,							DivergenceFactor);
	HBO_FIELD(hh_u32,							InitialSeed);
	HBO_FIELD(bool,								FastFakeFlow);
	HBO_FIELD(float,							GainMultiplier);


	bool										_OnStaticNoiseParameterModified();
	bool										_OnDefaultSampledFieldModified();
	bool										_OnStrengthModified(const float &oldValue);
	bool										_OnGainMultiplierModified();
	bool										_OnInitialSeedModified();
	EFieldVisibility							_StandardFlowParameters_VisibleInEditor() const;
	void										_SetupNoiseSampler(TVector3Noise<CRotatableFastNoise3> *noise) const;

protected:
	// NOTE: This is only used by the CPU implementation.
	// ideally, we should move it inside 'CParticleSamplerCPU_Turbulence'.
	// but keeping it here saves us potentially a lot of runtime memory duplication, especially when using attribute samplers.
	mutable TStaticArray<TPair<TVector3Noise<CRotatableFastNoise3>*, hh_u32>, CThreadManager::MaxThreadCount + 1>	m_NoiseSamplers;

	virtual	bool								VirtualLink() override;

public:
	CParticleSamplerProceduralTurbulence();
	~CParticleSamplerProceduralTurbulence();

	virtual void								SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool								DefineCommonType(CCompilerContext &context);
	virtual	void								DefineType(CCompilerContext &context) const override;
	virtual	void								DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

	TVector3Noise<CRotatableFastNoise3>			*GetNoiseSamplerAndCreateIFN(CThreadID threadId) const;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerProceduralTurbulence);

#endif


//----------------------------------------------------------------------------
//
//	Spectrum
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_SPECTRUM != 0)

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerSpectrum), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerSpectrum>
{
private:
	HBO_FIELD(CString,			ChannelGroup);
	HBO_FIELD(hh_u32,			Mode);
	HBO_FIELD(hh_u32,			Filter);
	HBO_FIELD(hh_u32,			Convolution);
	HBO_FIELD(float,			DefaultConvolutionLevel);

	virtual bool				_OnChannelGroupModified();
	virtual bool				_OnGenericAttributeModified();

protected:
	CStringId					m_ChannelGroupNameGUID;
	virtual bool				VirtualLink() override;

public:
	CParticleSamplerSpectrum();
	~CParticleSamplerSpectrum();

	enum	EMode
	{
		ModeWaveform = 0,
		ModeSpectrum,
		__MaxModes
	};

	enum	EFilter
	{
		FilterPoint = 0,
		FilterLinear,
		FilterCubic,
		__MaxFilters
	};

#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool						DefineCommonType(CCompilerContext &context);
	virtual	void						DefineType(CCompilerContext &context) const override;
	virtual	void						DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

	CStringId							ChannelGroupNameGUID() const { return m_ChannelGroupNameGUID; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerSpectrum);

#endif


//----------------------------------------------------------------------------
//
//	Text
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_SAMPLER_TEXT != 0)

class	CFontMetrics;

class	CParticleSamplerDescriptor_Text : public CParticleSamplerDescriptor
{
public:
	static hh_u32	SamplerTypeID() { static hh_u32 id = _GetNextSamplerTypeID(); return id; }
//	CParticleSamplerDescriptor_Text(const CTextSamplerInterface *desc = null) : CParticleSamplerDescriptor(SamplerTypeID()), m_Sampler(desc) {}
	CParticleSamplerDescriptor_Text();

//	const CTextSamplerInterface	*m_Sampler;
	CString						m_TextData;
	TArray<hh_i32>				m_CharLineIDs;	// count = m_TextData.Length(), lookup with charId
	TArray<float>				m_CharOffsets;	// count = m_TextData.Length(), lookup with charId
	TArray<CInt2>				m_LineOffsets;	// x = start char ID, y = char count til next \n or EOF
	float						m_LineHeight;

	void						Clear();

	bool						_Setup(const CString &text, const CFontMetrics *fontKerning, bool useKerning);	// returns false on allocation failure
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDescriptor_Text);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSamplerText), public CParticleSampler, public TRemappableObjectSpecialization<CParticleSamplerText>
{
public:
	enum	EDataSource
	{
		DataSource_Inline = 0,
		DataSource_External,
	};

private:
	HBO_FIELD(hh_u32,				DataSource);
	HBO_FIELD(CString,				InlineText);
	HBO_FIELD(CString,				ExternalResource);
	HBO_FIELD(CString,				FontFile);
	HBO_FIELD(bool,					UseKerning);
//	HBO_FIELD(CString,				LineSeparators);
//	HBO_FIELD(CString,				WordSeparators);
//	HBO_FIELD(CString,				KerningResource);

	bool							_InvalidateTextResource();
	bool							_InvalidateKerningResource();
	EFieldVisibility				_InlineTextVisibility() const;
	EFieldVisibility				_ExternalResourceVisibility() const;
	void							_OnKerningResourceReloaded(CFontMetrics *rectList);

protected:
//	TResourcePtr<CString>			m_Resource;
	TResourcePtr<CFontMetrics>		m_KerningResource;
	CString							m_TextData;

	virtual bool					VirtualLink() override;

	const CParticleSamplerDescriptor_Text	*_GetDefaultTextDescriptorAndCreateIFN(bool forceRebuild = false);

public:
	CParticleSamplerText();
	~CParticleSamplerText();

	virtual void						SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
#if	(PK_COMPILER_BUILD_COMPILER != 0)
	static	bool						DefineCommonType(CCompilerContext &context);
	virtual	void						DefineType(CCompilerContext &context) const override;
	virtual	void						DeclareExternalObject(CCompilerASTBuilder &builder) const override;
#endif	// (PK_COMPILER_BUILD_COMPILER != 0)

	const TResourcePtr<CFontMetrics>	&KerningResource() const { return m_KerningResource; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerText);

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PS_PARTICLES_SAMPLERS_H__
