#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2010/05/27 16:10 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_RENDERERS_CLASSES_H__
#define	__PS_RENDERERS_CLASSES_H__

#include "hh_particles/include/ps_renderers.h"

#include <hh_kernel/include/kr_resources.h>				// for TResourcePtr<>
#include <hh_geometrics/include/ge_rectangle_list.h>	// used for texture atlases

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SParticleDeclaration;
class	CRectangleList;

//----------------------------------------------------------------------------

class	CTextureAtlasData : public CRefCountedObject
{
public:
	CRectangleList					m_RectangleList;
	TArray<TRectangleMapper<hh_u16>::TCorners, TArrayAligned16>	m_CornersU16;	// 4 * 2 * 2 = 16 bytes
	TArray<TRectangleMapper<hh_i16>::TCorners, TArrayAligned16>	m_CornersS16;	// 4 * 2 * 2 = 16 bytes
};
HH_DECLARE_REFPTRCLASS(TextureAtlasData);

//----------------------------------------------------------------------------
//
//	CParticleRenderer_List : Allows to have multiple renderers
//
//----------------------------------------------------------------------------

class	HBO_CLASS(CParticleRenderer_List), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_List>
{
private:
	HBO_FIELD(TArray<CParticleRenderer*>, Renderers);

	bool							_OnRenderersModified();

public:
	CParticleRenderer_List();
	~CParticleRenderer_List();

	virtual void					AddToList(TArray<CParticleRenderer*> &renderers, CParticleBuildReport &outReport) override;

	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual bool					CompatibleWith(const CParticleRenderer *other) const override;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_List);

//----------------------------------------------------------------------------
//
//	CParticleRenderer_Null : empty renderer, used to disable rendering.
//
//----------------------------------------------------------------------------

class	HBO_CLASS(CParticleRenderer_Null), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_Null>
{
public:
	CParticleRenderer_Null();
	~CParticleRenderer_Null();

	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual bool					CompatibleWith(const CParticleRenderer *other) const override;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Null);

//----------------------------------------------------------------------------
//
//	CParticleRenderer_Billboard
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_RENDERER_BILLBOARD != 0)

class	HBO_CLASS(CParticleRenderer_Billboard), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_Billboard>
{
public: // identifiers
	enum	EBillboardMaterial
	{
		// Custom = 0,				// REMOVED v1.9.0
		Additive = 0,				// -> dont need sort ...
		Additive_NoAlpha,
		Additive_Soft,
		AlphaBlend,					// -> need sorting ...
		AlphaBlend_Soft,
		AlphaBlend_SoftLighting,
		AlphaBlend_Lighting,
		AlphaBlend_Distortion,
		AlphaBlend_AdditiveSoft,
		AlphaBlend_AdditiveSoftLighting,
		__MaxMaterials
	};

	enum	EBillboardMode
	{
		ScreenPoint = 0,
		ScreenAlignedQuad,
		ViewposAlignedQuad,
		ScreenAlignedTriangle,
		VelocityAxisAligned,
		VelocityCapsuleAlign,
		VelocitySpheroidalAlign,
		PlanarAlignedQuad,
		__MaxBillboardModes
	};

private: // var
	// material
	HBO_FIELD(hh_u32,				BillboardingMaterial);
	HBO_FIELD(CString,				Diffuse);				// texture
	HBO_FIELD(CString,				Normal);				// normals for lighting
	HBO_FIELD(CString,				AtlasDefinition);
	HBO_FIELD(CString,				AlphaRemapper);
//	HBO_FIELD(TArray<hh_u32>,		SelectedTextureIds);
	HBO_FIELD(bool,					SoftAnimationBlending);
	HBO_FIELD(float,				SoftnessDistance);

	// geometry
	HBO_FIELD(EBillboardMode,		BillboardMode);
	HBO_FIELD(bool,					VFlipUVs);
	HBO_FIELD(float,				ConstantRadius);
	HBO_FIELD(float,				AspectRatio);

	HBO_FIELD(CString,				PositionField);
	HBO_FIELD(CString,				SizeField);
	HBO_FIELD(CString,				ColorField);
	HBO_FIELD(CString,				TextureIDField);
	HBO_FIELD(CString,				AlphaCursorField);
	HBO_FIELD(CString,				RotationField);
	HBO_FIELD(CString,				AxisField);
	HBO_FIELD(CString,				Axis2Field);
	HBO_FIELD(float,				AxisScale);

	// lighting
	HBO_FIELD(bool,					CastShadow);
	HBO_FIELD(float,				NormalsBendingFactor);
	HBO_FIELD(float,				NormalsWrapFactor);
	HBO_FIELD(float,				LightExponent);
	HBO_FIELD(CString,				LightName);
	HBO_FIELD(CFloat3,				LightScale);
	HBO_FIELD(CFloat3,				AmbientLight);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_BILLBOARD_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_BILLBOARD_H
#endif

	// callbacks
	bool							_OnNonDeclarationRebuildingPropertyModified();
	bool							_OnDeclarationRebuildingPropertyModified();
	bool							_OnGenericPropertyModified();
	bool							_OnAspectRatioModified();
	bool							_OnAtlasDefinitionModified();
	bool							_OnConstantRadiusModified();

	bool							_OnPositionFieldModified();
	bool							_OnSizeFieldModified();
	bool							_OnColorFieldModified();
	bool							_OnTextureIDFieldModified();
	bool							_OnAlphaCursorFieldModified();
	bool							_OnRotationFieldModified();
	bool							_OnAxis0FieldModified();
	bool							_OnAxis1FieldModified();

	EFieldVisibility				_DiffuseVisibility() const;
	EFieldVisibility				_NormalVisibility() const;
	EFieldVisibility				_AlphaRemapperVisibility() const;
	EFieldVisibility				_RotationVisibility() const;
	EFieldVisibility				_AlphaRemmaperVisibility() const;
	EFieldVisibility				_Axis0Visibility() const;
	EFieldVisibility				_Axis1Visibility() const;
	EFieldVisibility				_LightFieldsVisiblity() const;
	EFieldVisibility				_SoftnessVisibility() const;
	EFieldVisibility				_ConstantLengthVisibility() const;	// TODO: remove this

	CFloat2							m_ScaleXY;
	CStringId						m_PositionFieldID;
	CStringId						m_SizeFieldID;
	CStringId						m_ColorFieldID;
	CStringId						m_TextureIDFieldID;
	CStringId						m_AlphaCursorFieldID;
	CStringId						m_RotationFieldID;
	CStringId						m_Axis0FieldID;
	CStringId						m_Axis1FieldID;

protected:
	float							m_ConstantRadiusA16Storage[4];

	// called when the class is linked with the particle engine
	virtual bool					VirtualLink() override;
	bool							_ReloadAtlasDefinition(const CString &path);
	void							_OnRectListReloaded(CRectangleList *rectList);

public:	// temporarily public, TODO: clean this
	TResourcePtr<CRectangleList>	m_RectangleList;
	TArray<TRectangleMapper<hh_u16>::TCorners, TArrayAligned16>	m_CornersU16;	// 4 * 2 * 2 = 16 bytes
	TArray<TRectangleMapper<hh_i16>::TCorners, TArrayAligned16>	m_CornersS16;	// 4 * 2 * 2 = 16 bytes

	PCTextureAtlasData				m_SharedAtlasData;

	bool							HasAtlas() const { return m_RectangleList != null && !m_RectangleList->m_CornersFp32.Empty(); }

public:
	CParticleRenderer_Billboard();
	~CParticleRenderer_Billboard();

	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	// used to group batches with the same materials
	virtual bool					CompatibleWith(const CParticleRenderer *other) const override;

	// methods called by the render object

	HH_INLINE bool					_NeedsIndices() const { return m_BillboardMode != ScreenPoint; }
	HH_INLINE bool					_NeedsBackToFrontSorting() const { return IsSorted(); }

	HH_FORCEINLINE const CFloat2	&ScaleXY() const { return m_ScaleXY; }
	HH_FORCEINLINE const float		&ConstantRadiusA16() const { return *Mem::Align<0x10>(m_ConstantRadiusA16Storage); }

	HH_FORCEINLINE CStringId		PositionFieldID() const { return m_PositionFieldID; }
	HH_FORCEINLINE CStringId		SizeFieldID() const { return m_SizeFieldID; }
	HH_FORCEINLINE CStringId		ColorFieldID() const { return m_ColorFieldID; }
	HH_FORCEINLINE CStringId		TextureIDFieldID() const { return m_TextureIDFieldID; }
	HH_FORCEINLINE CStringId		AlphaCursorFieldID() const { return m_AlphaCursorFieldID; }
	HH_FORCEINLINE CStringId		RotationFieldID() const { return m_RotationFieldID; }
	HH_FORCEINLINE CStringId		Axis0FieldID() const { return m_Axis0FieldID; }
	HH_FORCEINLINE CStringId		Axis1FieldID() const { return m_Axis1FieldID; }

	HH_INLINE bool					IsSorted() const { return IsSorted((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			IsSorted(EBillboardMaterial mat)
	{
		return mat >= AlphaBlend;
	}

	HH_INLINE bool					IsLit() const { return IsLit((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			IsLit(EBillboardMaterial mat)
	{
		return mat == AlphaBlend_SoftLighting || mat == AlphaBlend_Lighting || mat == AlphaBlend_AdditiveSoftLighting;
	}

	HH_INLINE bool					IsSoft() const { return IsSoft((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			IsSoft(EBillboardMaterial mat)
	{
		return	mat == Additive_Soft ||
				mat == AlphaBlend_Soft ||
				mat == AlphaBlend_SoftLighting ||
				mat == AlphaBlend_AdditiveSoft ||
				mat == AlphaBlend_AdditiveSoftLighting;
	}

	HH_INLINE bool					CouldIgnoreAlpha() const { return CouldIgnoreAlpha((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			CouldIgnoreAlpha(EBillboardMaterial mat)
	{
		return	mat == Additive_NoAlpha ||
				mat == AlphaBlend_Distortion;
	}

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Billboard);

//----------------------------------------------------------------------------

_PV_HH_DEFINE_ENUM_TRAITS(CParticleRenderer_Billboard::EBillboardMode);

#endif	// (HH_PARTICLES_BUILD_RENDERER_BILLBOARD != 0)

//----------------------------------------------------------------------------
//
//	CParticleRenderer_Light
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_RENDERER_LIGHT != 0)

class	HBO_CLASS(CParticleRenderer_Light), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_Light>
{
public:
	enum	EBillboardMaterial
	{
		// Custom = 0,				//  REMOVED v1.9.0
		Lighting_SplatPass = 0,
		Lighting_SplatPass_Spec,
		__MaxMaterials
	};

private:
	HBO_FIELD(hh_u32,				BillboardingMaterial);	// simple billboard mode
	HBO_FIELD(CString,				PositionField);
	HBO_FIELD(CString,				SizeField);
	HBO_FIELD(CString,				ColorField);
	HBO_FIELD(float,				ConstantRadius);	// if (!=0), will be used instead of the particle radius
	HBO_FIELD(float,				LightIntensityMultiplier);
	HBO_FIELD(float,				LightRadiusMultiplier);
	HBO_FIELD(float,				LightRadiusFactorFromIntensity);
	HBO_FIELD(CFloat2,				LightAttenuationSteepnessRange);
	HBO_FIELD(float,				LightAttenuationSteepness);
	HBO_FIELD(bool,					LitTranslucency);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_LIGHT_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_LIGHT_H
#endif

	bool							_OnGenericPropertyModified();
	bool							_OnPositionFieldModified();
	bool							_OnSizeFieldModified();
	bool							_OnColorFieldModified();

	CStringId						m_PositionFieldID;
	CStringId						m_SizeFieldID;
	CStringId						m_ColorFieldID;

protected:
	virtual bool					VirtualLink() override;

public:
	CParticleRenderer_Light();
	~CParticleRenderer_Light();

	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual bool					CompatibleWith(const CParticleRenderer *other) const override;

	HH_FORCEINLINE CStringId		PositionFieldID() const { return m_PositionFieldID; }
	HH_FORCEINLINE CStringId		SizeFieldID() const { return m_SizeFieldID; }
	HH_FORCEINLINE CStringId		ColorFieldID() const { return m_ColorFieldID; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Light);

#endif	// (HH_PARTICLES_BUILD_RENDERER_LIGHT != 0)

//----------------------------------------------------------------------------
//
//	CParticleRenderer_Mesh
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_RENDERER_MESH != 0)

class	HBO_CLASS(CParticleRenderer_Mesh_Descriptor), public CBaseObject//, public TRemappableObjectSpecialization<CParticleRenderer_Mesh_Descriptor>
{
public:
	enum	EMeshMaterial
	{
		Additive = 0,
		Additive_NoAlpha,
		Distortion,
		Solid,
		Solid_Lighting,
		__MaxMeshMaterials
	};

private:
	//General
	HBO_FIELD(CString,					Mesh);
	HBO_FIELD(hh_u32,					MeshLOD);
	HBO_FIELD(hh_i32,					SubMeshId);
	HBO_FIELD(hh_u32,					Material);
	HBO_FIELD(bool,						CastShadow);
	HBO_FIELD(TArray<CString>,			MaterialParametersFields);
	HBO_FIELD(CString,					CustomName);
	//Textures
	HBO_FIELD(CString,					Diffuse);
	HBO_FIELD(CString,					Normal);
	HBO_FIELD(CString,					Specular);

	HBO_FIELD(CFloat3,					DiffuseColor);
	HBO_FIELD(CFloat3,					AmbientColor);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_MESHDESC_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_MESHDESC_H
#endif

	TArray< TPair<CString, CStringId> >		m_MaterialParametersFieldsIDs;
	TArray< TPair<CStringId, CStringId> >	m_MaterialParametersIdFieldsIDs;

	bool								_OnPropertyModified();
	bool								_OnMaterialParametersFieldsModified();
	bool								_OnCustomNameModified();
	EFieldVisibility					_DiffuseVisibility() const;
	EFieldVisibility					_NormalVisibility() const;
	EFieldVisibility					_SpecularVisibility() const;
	EFieldVisibility					_DiffuseColorVisibility() const;
	EFieldVisibility					_AmbientColorVisibility() const;

protected:
	virtual bool						VirtualLink() override;

public:
	CParticleRenderer_Mesh_Descriptor();
	~CParticleRenderer_Mesh_Descriptor();

	// Sorted by material field name (TPair.First())
	TMemoryView<const TPair<CString, CStringId> >	MaterialParametersFieldsIDs() const { return m_MaterialParametersFieldsIDs; }
	TMemoryView<const TPair<CStringId, CStringId> >	MaterialParametersIdFieldsIDs() const { return m_MaterialParametersIdFieldsIDs; }

	HH_INLINE bool				IsSorted() const { return IsSorted((EMeshMaterial)Material()); }
	HH_INLINE static bool		IsSorted(EMeshMaterial mat) { return false; }

	HH_INLINE bool				IsLit() const { return IsLit((EMeshMaterial)Material()); }
	HH_INLINE static bool		IsLit(EMeshMaterial mat) { return mat == Solid_Lighting; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Mesh_Descriptor);

//----------------------------------------------------------------------------

class	HBO_CLASS(CParticleRenderer_Mesh), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_Mesh>
{
private:
	// Internal
	HBO_FIELD(TArray<CParticleRenderer_Mesh_Descriptor *>,	Meshes);

	// General
	HBO_FIELD(CString,					MeshIdField);				// must be of type 'float'

	// Static transformations
	HBO_FIELD(CFloat3,					Scale);
	HBO_FIELD(CFloat3,					StaticPositionOffset);
	HBO_FIELD(CFloat3,					StaticOrientationOffset);
	HBO_FIELD(CFloat3,					StaticRotationAxis);

	// Dynamic transformations
	HBO_FIELD(CString,					PositionField);
	HBO_FIELD(CString,					ScaleField);
	HBO_FIELD(CString,					EulerRotationField);
	HBO_FIELD(CString,					RotationAxisField);
	HBO_FIELD(CString,					RotationAxisAngleField);

	HBO_FIELD(CString,					PositionOffsetField);
	HBO_FIELD(CString,					ForwardAxisField);
	//HBO_FIELD(CString,				RightAxisField);
	HBO_FIELD(CString,					UpAxisField);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_MESH_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_MESH_H
#endif

	bool								_OnGenericPropertyModified();
	bool								_OnMeshesModified();
	bool								_OnMeshIdFieldModified();
	bool								_OnPositionFieldModified();
	bool								_OnScaleFieldModified();
	bool								_OnPositionOffsetFieldModified();
	bool								_OnForwardAxisFieldModified();
	bool								_OnUpAxisFieldModified();
	bool								_OnRotationAxisFieldModified();
	bool								_OnEulerRotationFieldModified();

	EFieldVisibility					_LightFieldsVisiblity() const;
	EFieldVisibility					_SoftnessVisibility() const;

	CStringId							m_MeshIdFieldID;
	// xforms fields
	CStringId							m_PositionFieldID;
	CStringId							m_ScaleFieldID;
	CStringId							m_PositionOffsetFieldID;
	CStringId							m_ForwardAxisFieldID;
	CStringId							m_UpAxisFieldID;
	CStringId							m_RotationAxisFieldID;
	CStringId							m_RotationAxisAngleFieldID;
	CStringId							m_EulerRotationFieldID;
	// prev xform fields
	CStringId							m_PrevPositionFieldID;
	CStringId							m_PrevScaleFieldID;
	CStringId							m_PrevPositionOffsetFieldID;
	CStringId							m_PrevForwardAxisFieldID;
	CStringId							m_PrevUpAxisFieldID;
	CStringId							m_PrevRotationAxisFieldID;
	CStringId							m_PrevRotationAxisAngleFieldID;
	CStringId							m_PrevEulerRotationFieldID;

protected:
	virtual bool						VirtualLink() override;

public:
	CParticleRenderer_Mesh();
	~CParticleRenderer_Mesh();

	virtual void						SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void						SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual bool						CompatibleWith(const CParticleRenderer *other) const override;
	// Copy kernel insertion
	virtual bool						BuildRuntimeKernels(const CParticleUpdater_CPU *updater, const SParticleDeclaration &decl, TArray<PParticleKernelCPU_Evolver> &outList) const override;

	HH_FORCEINLINE CStringId			MeshIdFieldID() const { return m_MeshIdFieldID; }

	HH_FORCEINLINE CStringId			PositionFieldID() const { return m_PositionFieldID; }
	HH_FORCEINLINE CStringId			ScaleFieldID() const { return m_ScaleFieldID; }
	HH_FORCEINLINE CStringId			PositionOffsetFieldID() const { return m_PositionOffsetFieldID; }
	HH_FORCEINLINE CStringId			ForwardAxisFieldID() const { return m_ForwardAxisFieldID; }
	HH_FORCEINLINE CStringId			UpAxisFieldID() const { return m_UpAxisFieldID; }
	HH_FORCEINLINE CStringId			RotationAxisFieldID() const { return m_RotationAxisFieldID; }
	HH_FORCEINLINE CStringId			RotationAxisAngleFieldID() const { return m_RotationAxisAngleFieldID; }
	HH_FORCEINLINE CStringId			EulerRotationFieldID() const { return m_EulerRotationFieldID; }

	// Only available if CHHParticlesBase::MeshRenderersHavePrevXForms() is true
	HH_FORCEINLINE CStringId			PrevPositionFieldID() const { return m_PrevPositionFieldID; }
	HH_FORCEINLINE CStringId			PrevScaleFieldID() const { return m_PrevScaleFieldID; }
	HH_FORCEINLINE CStringId			PrevPositionOffsetFieldID() const { return m_PrevPositionOffsetFieldID; }
	HH_FORCEINLINE CStringId			PrevForwardAxisFieldID() const { return m_PrevForwardAxisFieldID; }
	HH_FORCEINLINE CStringId			PrevUpAxisFieldID() const { return m_PrevUpAxisFieldID; }
	HH_FORCEINLINE CStringId			PrevRotationAxisFieldID() const { return m_PrevRotationAxisFieldID; }
	HH_FORCEINLINE CStringId			PrevRotationAxisAngleFieldID() const { return m_PrevRotationAxisAngleFieldID; }
	HH_FORCEINLINE CStringId			PrevEulerRotationFieldID() const { return m_PrevEulerRotationFieldID; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Mesh);

_PV_HH_DEFINE_ENUM_TRAITS(CParticleRenderer_Mesh_Descriptor::EMeshMaterial);

#endif	// (HH_PARTICLES_BUILD_RENDERER_MESH != 0)

//----------------------------------------------------------------------------
//
//	CParticleRenderer_Ribbon
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_RENDERER_RIBBON != 0)

class	HBO_CLASS(CParticleRenderer_Ribbon), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_Ribbon>
{
public:
	enum	EBillboardMaterial
	{
		// Custom = 0,			// REMOVED v1.9.0
		Additive = 0,			// -> dont need sort ...
		Additive_NoAlpha,
		Additive_Soft,
		AlphaBlend,				// -> need sort ...
		AlphaBlend_Soft,
		AlphaBlend_SoftLighting,
		AlphaBlend_Lighting,
		AlphaBlend_Distortion,
		AlphaBlend_AdditiveSoft,
		AlphaBlend_AdditiveSoftLighting,
		__MaxMaterials
	};

	enum	EBillboardMode
	{
		ViewposAlignedQuad = 0,
		AxisAlignedQuad,
		AxisSideAlignedQuad,
		__MaxBillboardModes
	};

	enum	EQuality
	{
		Simple = 0,
		CorrectDeformation
		//Smooth
	};

private:
	// material
	HBO_FIELD(EBillboardMaterial,	BillboardingMaterial);
	HBO_FIELD(EQuality,				Quality);
	HBO_FIELD(CString,				Diffuse);
	HBO_FIELD(CString,				Normal);
	HBO_FIELD(CString,				AtlasDefinition);
	HBO_FIELD(CString,				AlphaRemapper);
	HBO_FIELD(float,				SoftnessDistance);

	// texture
	HBO_FIELD(CString,				TextureIDField);
	HBO_FIELD(hh_u32,				TextureID);
	HBO_FIELD(CString,				TextureUField);
	HBO_FIELD(CString,				ColorField);
	HBO_FIELD(bool,					FlipU);
	HBO_FIELD(bool,					FlipV);
	HBO_FIELD(bool,					RotateTexture);
	HBO_FIELD(bool,					TextureRepeat);

	// geometry
	HBO_FIELD(EBillboardMode,		BillboardMode);
	HBO_FIELD(CString,				PositionField);
	HBO_FIELD(CString,				AlphaCursorField);
	HBO_FIELD(float,				Width);
	HBO_FIELD(CString,				WidthField);
	HBO_FIELD(CString,				AxisField);

	// lighting
	HBO_FIELD(bool,					CastShadow);
	HBO_FIELD(float,				NormalsBendingFactor);
	HBO_FIELD(float,				NormalsWrapFactor);
	HBO_FIELD(float,				LightExponent);
	HBO_FIELD(CString,				LightName);
	HBO_FIELD(CFloat3,				LightScale);
	HBO_FIELD(CFloat3,				AmbientLight);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_RIBBON_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_RIBBON_H
#endif

	EFieldVisibility				_DiffuseVisibility() const;
	EFieldVisibility				_NormalVisibility() const;
	EFieldVisibility				_AlphaRemapperVisibility() const;
	EFieldVisibility				_AxisVisibility() const;
	EFieldVisibility				_LightFieldsVisiblity() const;
	EFieldVisibility				_SoftnessVisibility() const;

	bool							_OnNonDeclarationRebuildingPropertyModified();
	bool							_OnDeclarationRebuildingPropertyModified();
	bool							_OnGenericPropertyModified();
	bool							_OnAtlasDefinitionModified();

	bool							_OnColorFieldModified();
	bool							_OnAlphaCursorFieldModified();
	bool							_OnTextureIDFieldModified();
	bool							_OnTextureUFieldModified();

	bool							_OnPositionFieldModified();
	bool							_OnWidthFieldModified();
	bool							_OnAxisFieldModified();

	CStringId						m_ColorFieldID;
	CStringId						m_TextureIDFieldID;
	CStringId						m_TextureUFieldID;

	CStringId						m_PositionFieldID;
	CStringId						m_AlphaCursorFieldID;
	CStringId						m_WidthFieldID;
	CStringId						m_AxisFieldID;

public:	// temporarily public, TODO: clean this
	TResourcePtr<CRectangleList>	m_RectangleList;
	TArray<TRectangleMapper<hh_u16>::TCorners, TArrayAligned16>	m_CornersU16;	// 4 * 2 * 2 = 16 bytes
	TArray<TRectangleMapper<hh_i16>::TCorners, TArrayAligned16>	m_CornersS16;	// 4 * 2 * 2 = 16 bytes

	PCTextureAtlasData				m_SharedAtlasData;

	bool							HasAtlas() const { return m_RectangleList != null && !m_RectangleList->m_CornersFp32.Empty(); }

protected:
	bool							_ReloadAtlasDefinition(const CString &path);

	virtual bool					VirtualLink() override;

public:
	CParticleRenderer_Ribbon();
	~CParticleRenderer_Ribbon();

	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual bool					CompatibleWith(const CParticleRenderer *other) const override;

	// methods called by the render object

	HH_INLINE bool					_NeedsBackToFrontSorting() const { return IsSorted(); }

	HH_FORCEINLINE CStringId		ColorFieldID() const { return m_ColorFieldID; }
	HH_FORCEINLINE CStringId		AlphaCursorFieldID() const { return m_AlphaCursorFieldID; }
	HH_FORCEINLINE CStringId		TextureIDFieldID() const { return m_TextureIDFieldID; }
	HH_FORCEINLINE CStringId		TextureUFieldID() const { return m_TextureUFieldID; }

	HH_FORCEINLINE CStringId		PositionFieldID() const { return m_PositionFieldID; }
	HH_FORCEINLINE CStringId		WidthFieldID() const { return m_WidthFieldID; }
	HH_FORCEINLINE CStringId		AxisFieldID() const { return m_AxisFieldID; }

	CStringId						SelfIDFieldID() const;
	CStringId						ParentIDFieldID() const;
	CStringId						SpawnerIDFieldID() const;

	HH_INLINE bool					IsSorted() const { return IsSorted((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			IsSorted(EBillboardMaterial mat)
	{
		return mat >= AlphaBlend;
	}

	HH_INLINE bool					IsLit() const { return IsLit((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			IsLit(EBillboardMaterial mat)
	{
		return mat == AlphaBlend_SoftLighting || mat == AlphaBlend_Lighting || mat == AlphaBlend_AdditiveSoftLighting;
	}

	HH_INLINE bool					IsSoft() const { return IsSoft((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			IsSoft(EBillboardMaterial mat)
	{
		return	mat == Additive_Soft ||
				mat == AlphaBlend_Soft ||
				mat == AlphaBlend_SoftLighting ||
				mat == AlphaBlend_AdditiveSoft ||
				mat == AlphaBlend_AdditiveSoftLighting;
	}

	HH_INLINE bool					CouldIgnoreAlpha() const { return CouldIgnoreAlpha((EBillboardMaterial)BillboardingMaterial()); }
	HH_INLINE static bool			CouldIgnoreAlpha(EBillboardMaterial mat)
	{
		return	mat == Additive_NoAlpha ||
				mat == AlphaBlend_Distortion;
	}

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Ribbon);

_PV_HH_DEFINE_ENUM_TRAITS(CParticleRenderer_Ribbon::EBillboardMaterial);
_PV_HH_DEFINE_ENUM_TRAITS(CParticleRenderer_Ribbon::EBillboardMode);
_PV_HH_DEFINE_ENUM_TRAITS(CParticleRenderer_Ribbon::EQuality);

#endif	// (HH_PARTICLES_BUILD_RENDERER_RIBBON != 0)

//----------------------------------------------------------------------------
//
//	CParticleRenderer_Sound
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_RENDERER_SOUND != 0)

class	HBO_CLASS(CParticleRenderer_Sound_Descriptor), public CBaseObject
{
private:
	// General
	HBO_FIELD(CString,					SoundPath);
	HBO_FIELD(CString,					EventStart);
	HBO_FIELD(CString,					EventStop);
	HBO_FIELD(CString,					UserData);
	HBO_FIELD(CString,					CustomName);
	HBO_FIELD(float,					Volume);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_SOUNDDESC_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_SOUNDDESC_H
#endif

	// Advanced
	HBO_FIELD(hh_u32,					PoolSizeHint);

	bool								_OnGenericPropertyModified();
	bool								_OnCustomNameModified();
protected:
	virtual bool						VirtualLink() override;

public:
	CParticleRenderer_Sound_Descriptor();
	~CParticleRenderer_Sound_Descriptor();

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Sound_Descriptor);

//----------------------------------------------------------------------------

class	HBO_CLASS(CParticleRenderer_Sound), public CParticleRenderer, public TRemappableObjectSpecialization<CParticleRenderer_Sound>
{
public:
	enum	EPoolSizeHint
	{
		PoolSize_Inherit = 0,	// Displayed as 'Inherit' in sound descriptors, and as 'Auto' in the base sound renderer
		PoolSize_Small,
		PoolSize_Medium,
		PoolSize_Large,
		__MaxPoolSizeHints
	};

private:
	HBO_FIELD(TArray<CParticleRenderer_Sound_Descriptor*>,	Sounds);

	// Material
	HBO_FIELD(CString,				ChannelGroup);
	HBO_FIELD(hh_u32,				PoolSizeHint);

	// Geometry
	HBO_FIELD(CString,				PositionField);
	HBO_FIELD(CString,				SoundIDField);
	HBO_FIELD(float,				Volume);
	HBO_FIELD(CString,				VolumeField);
	HBO_FIELD(float,				Radius);
	HBO_FIELD(CString,				RadiusField);
	HBO_FIELD(float,				DopplerLevel);
	HBO_FIELD(CString,				VelocityField);

	// Advanced
	HBO_FIELD(bool,					SoftSoundBlending);

#ifdef	PK_HAS_SPECIALIZATIONS
#	define	PS_RENDERERS_SPECIALIZATION_SOUND_H
#		include <pk_specializations.h>
#	undef	PS_RENDERERS_SPECIALIZATION_SOUND_H
#endif

	bool							_OnGenericPropertyModified();
	bool							_OnPositionFieldModified();
	bool							_OnSoundIDFieldModified();
	bool							_OnVolumeFieldModified();
	bool							_OnRadiusFieldModified();
	bool							_OnVelocityFieldModified();

	CStringId						m_PositionFieldID;
	CStringId						m_SoundIDFieldID;
	CStringId						m_VolumeFieldID;
	CStringId						m_RadiusFieldID;
	CStringId						m_VelocityFieldID;

protected:
	virtual bool					VirtualLink() override;

public:
	CParticleRenderer_Sound();
	~CParticleRenderer_Sound();

	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const override;
	virtual bool					CompatibleWith(const CParticleRenderer *other) const override;

	HH_FORCEINLINE CStringId		PositionFieldID() const { return m_PositionFieldID; }
	HH_FORCEINLINE CStringId		SoundIDFieldID() const { return m_SoundIDFieldID; }
	HH_FORCEINLINE CStringId		VolumeFieldID() const { return m_VolumeFieldID; }
	HH_FORCEINLINE CStringId		RadiusFieldID() const { return m_RadiusFieldID; }
	HH_FORCEINLINE CStringId		VelocityFieldID() const { return m_VelocityFieldID; }
	CStringId						SelfIDFieldID() const;
	CStringId						LifeRatioFieldID() const;
	CStringId						InvLifeFieldID() const;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer_Sound);

#endif	// (HH_PARTICLES_BUILD_RENDERER_SOUND != 0)

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_RENDERERS_CLASSES_H__
