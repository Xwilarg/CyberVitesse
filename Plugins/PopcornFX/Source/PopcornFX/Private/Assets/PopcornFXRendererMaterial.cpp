//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXRendererMaterial.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXEffect.h"
#include "PopcornFXTextureAtlas.h"
#include "PopcornFXCustomVersion.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"

#include "PopcornFXSDK.h"
#include <hh_base_object/include/hbo_handler.h>
#include <hh_particles/include/ps_renderers_classes.h>
#include <hh_particles/include/ps_scene_actions.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXRendererMaterial, Log, All);

//----------------------------------------------------------------------------

namespace
{
	enum ERMID
	{
		RMID_Billboard = 0,
		RMID_Ribbon,
		RMID_Decal,
		RMID_Mesh,
		_RMID_Count,
	};

#if WITH_EDITOR
	void		_SetStaticSwtich(FStaticParameterSet &params, FName name, bool value)
	{
		for (int32 i = 0; i < params.StaticSwitchParameters.Num(); ++i)
		{
			FStaticSwitchParameter		&param  = params.StaticSwitchParameters[i];
			if (param.ParameterName == name)
			{
				param.Value = value;
				param.bOverride = true;
				// should be uniq !?
				return;
			}
		}
		new (params.StaticSwitchParameters) FStaticSwitchParameter(name, value, true, FGuid()); // FGuid ???
	}

	void		_SetStaticSwtiches_Common(UMaterialInstanceConstant *material, FPopcornFXSubRendererMaterial &mat)
	{
		FStaticParameterSet			params = material->GetStaticParameters();

		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_SOFT_PARTICLES(), mat.SoftnessDistance > 0.f);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_SOFT_ANIM(), mat.SoftAnimBlending != 0);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_HAS_NORMAL_TEXTURE(), mat.TextureNormal != null);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_HAS_ALPHA_REMAPPER(), mat.TextureAlphaRemapper != null);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_NO_ALPHA(), mat.NoAlpha != 0);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_CAST_SHADOW(), mat.CastShadow != 0);
		HH_ASSERT(mat.m_RMId >= 0 && mat.m_RMId < _RMID_Count);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_BILLBOARD(), mat.m_RMId == RMID_Billboard);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_RIBBON(), mat.m_RMId == RMID_Ribbon);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_DECAL(), mat.m_RMId == RMID_Decal);
		_SetStaticSwtich(params, FPopcornFXPlugin::Name_POPCORNFX_IS_MESH(), mat.m_RMId == RMID_Mesh);

		material->UpdateStaticPermutation(params);
	}
#endif

	struct FRendererMaterialsFuncs
	{
		typedef bool(*CbCompat)(const FPopcornFXSubRendererMaterial *mat, UMaterialInterface *material);

		CbCompat		m_CompatFunc;

#if WITH_EDITOR
		typedef bool(*CbSetup)(UPopcornFXRendererMaterial *self, const PopcornFX::CBaseObject *rendererBase);
		typedef bool(*CbParams)(UPopcornFXRendererMaterial *self, UMaterialInstanceDynamic *material, uint32 subMaterialIndex);
		typedef bool(*CbConstParams)(UPopcornFXRendererMaterial *self, UMaterialInstanceConstant *material, uint32 subMaterialIndex);
		CbSetup			m_SetupFunc;
		CbParams		m_ParamsFunc;
		CbConstParams	m_ParamsConstFunc;
#endif // WITH_EDITOR
	};

#if WITH_EDITOR
	bool		RM_Setup_Billboard(UPopcornFXRendererMaterial *self, const PopcornFX::CBaseObject *rendererBase)
	{
		const PopcornFX::CParticleRenderer_Billboard	*renderer = PopcornFX::HBO::Cast<const PopcornFX::CParticleRenderer_Billboard>(rendererBase);
		if (renderer == null)
			return false;
		self->SubMaterials.SetNum(1);
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[0];
		mat.MaterialType = kBillboard_Material_ToUE[renderer->BillboardingMaterial()];
		mat.TextureDiffuse = LoadTexturePk(renderer->Diffuse());
		mat.TextureNormal = LoadTexturePk(renderer->Normal());
		mat.TextureAlphaRemapper = LoadTexturePk(renderer->AlphaRemapper());
		mat.TextureAtlas = LoadAtlasPk(renderer->AtlasDefinition());
		mat.NoAlpha = (renderer->BillboardingMaterial() == PopcornFX::CParticleRenderer_Billboard::Additive_NoAlpha);
		mat.SoftAnimBlending = renderer->SoftAnimationBlending();
		mat.CastShadow = renderer->CastShadow();
		mat.UserData = renderer->UserData().Data();
		mat.SoftnessDistance = renderer->IsSoft() ? renderer->SoftnessDistance() : 0.f;
		mat.DrawOrder = renderer->DrawOrder();

		return true;
	}

	bool		RM_Params_Billboard(UPopcornFXRendererMaterial *self, UMaterialInstanceDynamic *material, uint32 subMaterialIndex)
	{
		check(subMaterialIndex < uint32(self->SubMaterials.Num()));
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[subMaterialIndex];
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_DiffuseTexture(), mat.TextureDiffuse);
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_NormalTexture(), mat.TextureNormal);
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_AlphaRemapper(), mat.TextureAlphaRemapper);
		material->SetScalarParameterValue(FPopcornFXPlugin::Name_SoftnessDistance(), mat.SoftnessDistance * FPopcornFXPlugin::GlobalScale());
		return true;
	}

	bool		RM_ParamsConst_Billboard(UPopcornFXRendererMaterial *self, UMaterialInstanceConstant *material, uint32 subMaterialIndex)
	{
		check(subMaterialIndex < uint32(self->SubMaterials.Num()));
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[subMaterialIndex];

		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_DiffuseTexture(), mat.TextureDiffuse);
		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_NormalTexture(), mat.TextureNormal);
		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_AlphaRemapper(), mat.TextureAlphaRemapper);
		material->SetScalarParameterValueEditorOnly(FPopcornFXPlugin::Name_SoftnessDistance(), mat.SoftnessDistance * FPopcornFXPlugin::GlobalScale());

		_SetStaticSwtiches_Common(material, mat);

		// !!
		material->PostEditChange();
		return true;
	}

	bool		RM_Params_Decal(UPopcornFXRendererMaterial *self, UMaterialInstanceDynamic *material, uint32 subMaterialIndex)
	{
		check(subMaterialIndex < uint32(self->SubMaterials.Num()));
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[subMaterialIndex];

		material->SetTextureParameterValue(FPopcornFXPlugin::Name_DiffuseTexture(), mat.TextureDiffuse);
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_NormalTexture(), mat.TextureNormal);
		material->SetVectorParameterValue(FPopcornFXPlugin::Name_DecalColor(), FLinearColor::White);
		return true;
	}

	bool		RM_ParamsConst_Decal(UPopcornFXRendererMaterial *self, UMaterialInstanceConstant *material, uint32 subMaterialIndex)
	{
		check(subMaterialIndex < uint32(self->SubMaterials.Num()));
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[subMaterialIndex];

		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_DiffuseTexture(), mat.TextureDiffuse);
		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_NormalTexture(), mat.TextureNormal);
		material->SetVectorParameterValueEditorOnly(FPopcornFXPlugin::Name_DecalColor(), FLinearColor::White);

		_SetStaticSwtiches_Common(material, mat);

		// !!
		material->PostEditChange();
		return true;
	}
#endif // WITH_EDITOR

	bool		RM_Compat_Billboard(const FPopcornFXSubRendererMaterial *mat, UMaterialInterface *materialInterface)
	{
		bool		isCompatible = true;
		UMaterial	*material = materialInterface->GetMaterial();

		isCompatible &= material != null;
		if (isCompatible)
		{
			const bool	isLit = mat->MaterialType == EPopcornFXMaterialType::Billboard_AlphaBlend_Lit ||
				mat->MaterialType == EPopcornFXMaterialType::Billboard_AlphaBlendAdditive_Lit;
			const bool	isMaterialLit = material->GetShadingModel() != EMaterialShadingModel::MSM_Unlit;

			const bool	isAdditive = mat->MaterialType == EPopcornFXMaterialType::Billboard_Additive;
			const bool	isMaterialAdditive = material->GetBlendMode() == EBlendMode::BLEND_Additive;

			const bool	isTranslucent = mat->MaterialType != EPopcornFXMaterialType::Billboard_Additive;
			const bool	isMaterialTranslucent = material->GetBlendMode() == EBlendMode::BLEND_Translucent;

			//isCompatible &= isLit == isMaterialLit;
			isCompatible &= material->MaterialDomain == EMaterialDomain::MD_Surface;
			//isCompatible &= isMaterialAdditive == isAdditive;
			//isCompatible &= isMaterialTranslucent == isTranslucent;
			isCompatible &= material->bUsedWithParticleSprites;
		}
		return isCompatible;
	}

#if WITH_EDITOR
	bool		RM_Setup_Ribbon(UPopcornFXRendererMaterial *self, const PopcornFX::CBaseObject *rendererBase)
	{
		const PopcornFX::CParticleRenderer_Ribbon	*renderer = PopcornFX::HBO::Cast<const PopcornFX::CParticleRenderer_Ribbon>(rendererBase);
		if (renderer == null)
			return false;
		self->SubMaterials.SetNum(1);
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[0];
		mat.MaterialType = kBillboard_Material_ToUE[renderer->BillboardingMaterial()];
		mat.TextureDiffuse = LoadTexturePk(renderer->Diffuse());
		mat.TextureNormal = LoadTexturePk(renderer->Normal());
		mat.TextureAlphaRemapper = LoadTexturePk(renderer->AlphaRemapper());
		mat.TextureAtlas = LoadAtlasPk(renderer->AtlasDefinition());
		mat.NoAlpha = (renderer->BillboardingMaterial() == PopcornFX::CParticleRenderer_Ribbon::Additive_NoAlpha);
		mat.SoftAnimBlending = false;
		mat.CastShadow = renderer->CastShadow();
		mat.UserData = renderer->UserData().Data();
		mat.SoftnessDistance = renderer->IsSoft() ? renderer->SoftnessDistance() : 0.f;
		mat.DrawOrder = renderer->DrawOrder();
		return true;
	}
#endif // WITH_EDITOR

#if WITH_EDITOR
	bool		RM_Setup_Decal(UPopcornFXRendererMaterial *self, const PopcornFX::CBaseObject *rendererBase)
	{
		const PopcornFX::CActionFactoryDecalSpawner	*decalFactory = PopcornFX::HBO::Cast<const PopcornFX::CActionFactoryDecalSpawner>(rendererBase);
		if (decalFactory == null)
			return false;
		self->SubMaterials.SetNum(1);
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[0];
		mat.MaterialType = kDecal_Material_ToUE[decalFactory->Material()];
		mat.TextureDiffuse = LoadTexturePk(decalFactory->Diffuse());
		mat.TextureNormal = LoadTexturePk(decalFactory->Normal());
		mat.NoAlpha = false;
		mat.SoftAnimBlending = false;
		mat.CastShadow = false;
		mat.UserData = FString();
		//mat.UserData = decalFactory->UserData().Data();
		return true;
	}
#endif // WITH_EDITOR

	bool		RM_Compat_Decal(const FPopcornFXSubRendererMaterial *mat, UMaterialInterface *materialInterface)
	{
		bool		isCompatible = true;
		UMaterial	*material = materialInterface->GetMaterial();

		isCompatible &= material != null;
		if (isCompatible)
		{
			const bool	isMaterialDecal = material->MaterialDomain == EMaterialDomain::MD_DeferredDecal;

			isCompatible &= isMaterialDecal;
		}
		return isCompatible;
	}

#if WITH_EDITOR
	bool		RM_Setup_Mesh(UPopcornFXRendererMaterial *self, const PopcornFX::CBaseObject *rendererBase)
	{
		const PopcornFX::CParticleRenderer_Mesh	*renderer = PopcornFX::HBO::Cast<const PopcornFX::CParticleRenderer_Mesh>(rendererBase);
		if (renderer == null)
			return false;

		const hh_u32				meshCount = renderer->Meshes().Count();
		self->SubMaterials.SetNum(meshCount);
		for (hh_u32 meshi = 0; meshi < renderer->Meshes().Count(); ++meshi)
		{
			const PopcornFX::CParticleRenderer_Mesh_Descriptor	*desc = renderer->Meshes()[meshi];
			check(desc != null);
			FPopcornFXSubRendererMaterial			&mat = self->SubMaterials[meshi];

			mat.MaterialType = kMesh_Material_ToUE[desc->Material()];
			mat.NoAlpha = (desc->Material() == PopcornFX::CParticleRenderer_Mesh_Descriptor::Additive_NoAlpha);
			mat.SoftAnimBlending = false;
			mat.CastShadow = desc->CastShadow();
			mat.TextureDiffuse = LoadTexturePk(desc->Diffuse());
			mat.TextureNormal = LoadTexturePk(desc->Normal());
			mat.TextureSpecular = LoadTexturePk(desc->Specular());
			//mat.TextureAlphaRemapper = LoadTexturePk(desc->AlphaRemapper());
			mat.StaticMesh = LoadMeshPk(desc->Mesh());
			mat.StaticMeshLOD = desc->MeshLOD();
			mat.SubMeshId = desc->SubMeshId();
			mat.UserData = renderer->UserData().Data();
			mat.SoftnessDistance = 0.f;
			mat.DrawOrder = renderer->DrawOrder();
		}
		return true;
	}

	bool		RM_Params_Mesh(UPopcornFXRendererMaterial *self, UMaterialInstanceDynamic *material, uint32 subMaterialIndex)
	{
		check(subMaterialIndex < uint32(self->SubMaterials.Num()));
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[subMaterialIndex];
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_DiffuseTexture(), mat.TextureDiffuse);
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_NormalTexture(), mat.TextureNormal);
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_SpecularTexture(), mat.TextureSpecular);
		material->SetTextureParameterValue(FPopcornFXPlugin::Name_AlphaRemapper(), mat.TextureAlphaRemapper);
		material->SetScalarParameterValue(FPopcornFXPlugin::Name_SoftnessDistance(), mat.SoftnessDistance * FPopcornFXPlugin::GlobalScale());
		return true;
	}

	bool		RM_ParamsConst_Mesh(UPopcornFXRendererMaterial *self, UMaterialInstanceConstant *material, uint32 subMaterialIndex)
	{
		check(subMaterialIndex < uint32(self->SubMaterials.Num()));
		FPopcornFXSubRendererMaterial		&mat = self->SubMaterials[subMaterialIndex];

		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_DiffuseTexture(), mat.TextureDiffuse);
		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_NormalTexture(), mat.TextureNormal);
		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_SpecularTexture(), mat.TextureSpecular);
		material->SetTextureParameterValueEditorOnly(FPopcornFXPlugin::Name_AlphaRemapper(), mat.TextureAlphaRemapper);
		material->SetScalarParameterValueEditorOnly(FPopcornFXPlugin::Name_SoftnessDistance(), mat.SoftnessDistance * FPopcornFXPlugin::GlobalScale());

		_SetStaticSwtiches_Common(material, mat);

		// !!
		material->PostEditChange();
		return true;
	}

#endif // WITH_EDITOR

	bool		RM_Compat_Mesh(const FPopcornFXSubRendererMaterial *mat, UMaterialInterface *materialInterface)
	{
		UMaterial		*material = materialInterface->GetMaterial();
		return material == null ? false : material->bUsedWithMeshParticles;
		//return material->CheckMaterialUsage(EMaterialUsage::MATUSAGE_MeshParticles, true);
	}

#if WITH_EDITOR
#	define RENDERMATCALLBACKS(__setup, __params, __compat)	{ &RM_Compat_ ## __compat, &RM_Setup_ ## __setup, &RM_Params_ ## __params, &RM_ParamsConst_ ## __params }
#else
#	define RENDERMATCALLBACKS(__setup, __params, __compat)	{ &RM_Compat_ ## __compat }
#endif

	const FRendererMaterialsFuncs		kRendererMaterialFuncs[] =
	{
		RENDERMATCALLBACKS(Billboard, Billboard, Billboard), // RMID_Billboard
		RENDERMATCALLBACKS(Ribbon, Billboard, Billboard), // RMID_Ribbon
		RENDERMATCALLBACKS(Decal, Decal, Decal), // RMID_Decal
		RENDERMATCALLBACKS(Mesh, Mesh, Mesh), // RMID_Mesh

		// ONLY APPEND NEW RENDERERS (or it will fuck ids)
	};

	HH_STATIC_ASSERT(_RMID_Count == HH_ARRAY_COUNT(kRendererMaterialFuncs));
}

//----------------------------------------------------------------------------
//
// FPopcornFXSubRendererMaterial
//
//----------------------------------------------------------------------------

FPopcornFXSubRendererMaterial::FPopcornFXSubRendererMaterial()
	: Material(null)
	, MaterialType()
	, TextureDiffuse(null)
	, TextureNormal(null)
	, TextureSpecular(null)
	, TextureAlphaRemapper(null)
	, TextureAtlas(null)
	, NoAlpha(false)
	, SoftAnimBlending(false)
	, CastShadow(false)
	, UserData()
	, SoftnessDistance(0)
	, DrawOrder(0)
	, StaticMesh(null)
	, StaticMeshLOD(0)
	, SubMeshId(-1)
	, m_RMId(-1)
	, MaterialInstance(null)
	//, MaterialInstanceCacheForRenderThread(null)
	//, MaterialInstanceCache_FrameNumber(0)
{

}

//----------------------------------------------------------------------------
#if WITH_EDITOR

bool	FPopcornFXSubRendererMaterial::_ResetDefaultMaterial_NoReload()
{
	Material = FindDefaultMaterial();
	return Material != null;
}

//----------------------------------------------------------------------------

namespace Test
{
	template <typename _TObj>
	_TObj		*__DuplicateObjectToPath(_TObj *source, const FString &dstPath)
	{
		if (source == null)
			return null;
		_TObj			*dupObj = null;
		UObject			*outer = null;
		FString			resPath = dstPath;
		if (ResolveName(outer, resPath, true, true))
			dupObj = DuplicateObject(source, outer, *resPath);
		if (dupObj != null)
		{
			dupObj->MarkPackageDirty();
			if (dupObj->GetOuter())
				dupObj->GetOuter()->MarkPackageDirty();
		}
		return dupObj;
	}
}
using namespace Test;

//----------------------------------------------------------------------------

FString		FPopcornFXSubRendererMaterial::GetDefaultMaterialFileName_FallBack() const
{
	const UEnum		*matTypeEnum = ::FindObject<UEnum>((UObject*)ANY_PACKAGE, TEXT("EPopcornFXMaterialType"));
	check(matTypeEnum != null);
#if (ENGINE_MINOR_VERSION >= 16)
	return matTypeEnum->GetNameStringByIndex(MaterialType);
#else
	return matTypeEnum->GetEnumName(MaterialType);
#endif
}

//----------------------------------------------------------------------------

UMaterialInterface		*FPopcornFXSubRendererMaterial::FindDefaultMaterial() const
{
	if (!HH_VERIFY(uint32(m_RMId) < HH_ARRAY_COUNT(kRendererMaterialFuncs)))
		return null;
	UMaterialInterface		*defaultMaterial = FPopcornFXPlugin::Get().Settings()->GetConfigDefaultMaterial(MaterialType.GetValue());
	return defaultMaterial;
}

#endif // WITH_EDITOR

//----------------------------------------------------------------------------

bool	FPopcornFXSubRendererMaterial::CanBeMergeWith(const FPopcornFXSubRendererMaterial &other) const
{
	return
		m_RMId == other.m_RMId &&
		MaterialType == other.MaterialType &&
		TextureDiffuse == other.TextureDiffuse &&
		TextureNormal == other.TextureNormal &&
		TextureSpecular == other.TextureSpecular &&
		TextureAlphaRemapper == other.TextureAlphaRemapper &&
		TextureAtlas == other.TextureAtlas &&
		NoAlpha == other.NoAlpha &&
		SoftAnimBlending == other.SoftAnimBlending &&
		CastShadow == other.CastShadow &&
		UserData == other.UserData &&
		SoftnessDistance == other.SoftnessDistance &&
		DrawOrder == other.DrawOrder &&
		StaticMesh == other.StaticMesh &&
		StaticMeshLOD == other.StaticMeshLOD &&
		SubMeshId == other.SubMeshId;
}

//----------------------------------------------------------------------------

void	FPopcornFXSubRendererMaterial::ConditionalPostLoadAll()
{
	if (Material != null)
		Material->ConditionalPostLoad();
	if (TextureDiffuse != null)
		TextureDiffuse->ConditionalPostLoad();
	if (TextureNormal != null)
		TextureNormal->ConditionalPostLoad();
	if (TextureSpecular != null)
		TextureSpecular->ConditionalPostLoad();
	if (TextureAlphaRemapper != null)
		TextureAlphaRemapper->ConditionalPostLoad();
	if (TextureAtlas != null)
		TextureAtlas->ConditionalPostLoad();
	if (StaticMesh != null)
		StaticMesh->ConditionalPostLoad();
	if (MaterialInstance != null)
		MaterialInstance->ConditionalPostLoad();
}

//----------------------------------------------------------------------------
//
// UPopcornFXRendererMaterial
//
//----------------------------------------------------------------------------

UPopcornFXRendererMaterial::UPopcornFXRendererMaterial(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
}

void	UPopcornFXRendererMaterial::BeginDestroy()
{
	Super::BeginDestroy();
	m_DetachFence.BeginFence();
}

bool	UPopcornFXRendererMaterial::IsReadyForFinishDestroy()
{
	return Super::IsReadyForFinishDestroy() && m_DetachFence.IsFenceComplete();
}

void	UPopcornFXRendererMaterial::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FPopcornFXCustomVersion::GUID);
}

void	UPopcornFXRendererMaterial::PostLoad()
{
	Super::PostLoad();

	const int32		version = GetLinkerCustomVersion(FPopcornFXCustomVersion::GUID);

#if WITH_EDITOR
	bool			reloadAllInstances = false;

	if (version < FPopcornFXCustomVersion::RendererMaterials_AddMaterialInstanceConstant)
	{
		reloadAllInstances = true;
	}

	if (version < FPopcornFXCustomVersion::RendererMaterials_RemoveNoAlphaType)
	{
		for (int32 subMati = 0; subMati < SubMaterials.Num(); ++subMati)
		{
			FPopcornFXSubRendererMaterial		&subMat = SubMaterials[subMati];
			if (subMat.MaterialType == EPopcornFXMaterialType::Billboard_Additive_NoAlpha)
			{
				subMat.MaterialType = EPopcornFXMaterialType::Billboard_Additive;
				subMat.NoAlpha = true;
				reloadAllInstances = true;
			}
			else if (subMat.MaterialType == EPopcornFXMaterialType::Mesh_Additive_NoAlpha)
			{
				subMat.MaterialType = EPopcornFXMaterialType::Mesh_Additive;
				subMat.NoAlpha = true;
				reloadAllInstances = true;
			}
		}
	}

	//if (version < FPopcornFXCustomVersion::RendererMaterials_NewDefaultMaterials)
	if (version < FPopcornFXCustomVersion::RendererMaterials_DefaultMaterialsInConfig)
	{
		const FString			pluginBase = "/PopcornFX/";
		for (int32 subMati = 0; subMati < SubMaterials.Num(); ++subMati)
		{
			FPopcornFXSubRendererMaterial		&subMat = SubMaterials[subMati];
			bool								reloadDefault = false;
			subMat.ConditionalPostLoadAll();
			if (subMat.Material != null)
			{
				FString		oldMatPath = subMat.Material->GetPathName();
				if (oldMatPath.StartsWith(pluginBase))
					reloadDefault = true;
			}
			else
				reloadDefault = true;

			if (reloadDefault)
			{
				subMat._ResetDefaultMaterial_NoReload();
				reloadAllInstances = true;
			}
		}
	}

	//if (version < FPopcornFXCustomVersion::RendererMaterials_AddTextureAtlasClass)
	//{
	//	// we need to unref ALL texture atlases, or will fail at reimport !
	//	for (int32 subi = 0; subi < SubMaterials.Num(); ++subi)
	//	{
	//		FPopcornFXSubRendererMaterial		&subMat = SubMaterials[subi];
	//		if (subMat.TextureAtlas != null &&
	//			subMat.TextureAtlas->GetClass() == UPopcornFXFile::StaticClass())
	//		{
	//			subMat.TextureAtlas = null;
	//		}
	//	}
	//}

	if (reloadAllInstances)
	{
		for (int32 subMati = 0; subMati < SubMaterials.Num(); ++subMati)
		{
			FPopcornFXSubRendererMaterial		&subMat = SubMaterials[subMati];
			subMat.ConditionalPostLoadAll();
		}
		ReloadInstances();
	}

#else
	// NO WITH_EDITOR
	if (version < FPopcornFXCustomVersion::LatestVersion)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("UPopcornFXRendererMaterial '%s' is outdated (%d), please re-save or re-cook asset"), *GetPathName(), version);
	}
#endif

}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 13)
void	UPopcornFXRendererMaterial::PreSave(const class ITargetPlatform* TargetPlatform)
#else
void	UPopcornFXRendererMaterial::PreSave()
#endif
{
	// Flush rendering commands to ensure the rendering thread do not touch us
	FlushRenderingCommands();

#if (ENGINE_MINOR_VERSION >= 13)
	Super::PreSave(TargetPlatform);
#else
	Super::PreSave();
#endif
}

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::PreEditChange(UProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);

	// Flush rendering commands to ensure the rendering thread do not touch us
	// UActorComponent already does that ? we are not UActorComponent !?
	FlushRenderingCommands();
}

//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::PostEditChangeChainProperty(struct FPropertyChangedChainEvent & propertyChangedEvent)
{
	static FName		SubMaterialsName(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXRendererMaterial, SubMaterials));
	static FName		MaterialInstanceName(GET_MEMBER_NAME_STRING_CHECKED(FPopcornFXSubRendererMaterial, MaterialInstance));

	if (propertyChangedEvent.Property != null &&
		propertyChangedEvent.Property->GetFName() != MaterialInstanceName)
	{
		int32		matIndex = propertyChangedEvent.GetArrayIndex(GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXRendererMaterial, SubMaterials));
		if (matIndex >= 0)
		{
			TriggerParticleRenderersModification();
		}
		else if (propertyChangedEvent.PropertyChain.GetActiveMemberNode() != null)
		{
			UProperty		*member =  propertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue();
			if (member != null && member->GetFName() == SubMaterialsName)
			{
				TriggerParticleRenderersModification();
			}
		}
	}
	Super::PostEditChangeChainProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::PostEditUndo()
{
	TriggerParticleRenderersModification();
	Super::PostEditUndo();
}

//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::TriggerParticleRenderersModification()
{
	UPopcornFXEffect		*effect = ParentEffect.Get();
	if (!HH_VERIFY(effect != null))
		return;

	const PopcornFX::PCBaseObjectFile	bofile = FPopcornFXPlugin::Get().FindPkFile(effect);
	if (bofile == null) // no loaded, do nothing
		return;

	const PopcornFX::TArray<PopcornFX::PBaseObject>		&objects = bofile->ObjectList();
	if (objects.Empty())
		return;

	for (int32 rendereri = 0; rendereri < BaseObjectNames.Num(); ++rendereri)
	{
		const PopcornFX::CString	boName = TCHAR_TO_ANSI(*BaseObjectNames[rendereri]);
		for (hh_u32 obji = 0; obji < objects.Count(); ++obji)
		{
			const PopcornFX::CBaseObject	*bo = objects[obji].Get();
			if (bo != null && boName == bo->Name())
			{
				bo->TriggerModificationOfLinkedFieldsInReferencingObjects();
				break;
			}
		}
	}

	ReloadInstances();
}

bool	UPopcornFXRendererMaterial::_Setup(UPopcornFXEffect *parentEffect, const PopcornFX::CBaseObject *bo)
{
	ParentEffect = parentEffect;

	BaseObjectNames.Empty(BaseObjectNames.Num());
#if WITH_EDITOR
	BaseObjectFriendlyNames.Empty(BaseObjectFriendlyNames.Num());
#endif
	_AddBaseObject(bo);

	int32		rmId = -1;
	for (hh_u32 rmi = 0; rmi < HH_ARRAY_COUNT(kRendererMaterialFuncs); ++rmi)
	{
		if (kRendererMaterialFuncs[rmi].m_SetupFunc(this, bo))
		{
			rmId = rmi;
			for (int32 mati = 0; mati < SubMaterials.Num(); ++mati)
				SubMaterials[mati].m_RMId = rmId;
			break;
		}
	}

	if (rmId < 0)
	{
		SubMaterials.Empty();
		return false;
	}

	for (int32 mati = 0; mati < SubMaterials.Num(); ++mati)
		if (SubMaterials[mati].Material == null)
			SubMaterials[mati]._ResetDefaultMaterial_NoReload();

	// ReloadInstances();
	return true;
}

//----------------------------------------------------------------------------

namespace
{
	const PopcornFX::CBaseObject		*_FirstParent(const PopcornFX::CBaseObject *bo)
	{
		for (hh_u32 i = 0; i < bo->ReferencingBlocks().Count(); ++i)
		{
			if (bo->ReferencingBlocks()[i].First() != null)
				return bo->ReferencingBlocks()[i].First().Get();
		}
		return null;
	}

	const char		*_PickName(const PopcornFX::CBaseObject *bo)
	{
		const char		*name = bo->Name();
		const PopcornFX::CString		s = name;
		if (s.StartsWith(bo->Handler()->PublishedName())) // auto-generated name: ignore
			return null;
		return name;
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::_AddBaseObject(const PopcornFX::CBaseObject *bo)
{
	BaseObjectNames.Add(bo->Name());

#if WITH_EDITOR
	PopcornFX::TArray<const PopcornFX::CBaseObject *>	stack;
	FString							friendlyName = bo->Name();
	const PopcornFX::CBaseObject	*object = bo;
	while (object != null)
	{
		const PopcornFX::CBaseObject	*next = _FirstParent(object);
		if (next == null) // dont take very first node
			break;
		const char		*name = _PickName(object);
		if (name != null)
			friendlyName = name + ("/" + friendlyName);
		stack.PushBack(object);
		object = next;
		if (stack.Contains(object))
			break;
	}
	BaseObjectFriendlyNames.Add(friendlyName);
#endif
}

//----------------------------------------------------------------------------

bool	UPopcornFXRendererMaterial::Setup(UPopcornFXEffect *parentEffect, const PopcornFX::CActionFactoryDecalSpawner *decalFactory)
{
	return _Setup(parentEffect, decalFactory);
}

//----------------------------------------------------------------------------

bool	UPopcornFXRendererMaterial::Setup(UPopcornFXEffect *parentEffect, const PopcornFX::CParticleRenderer *renderer)
{
	return _Setup(parentEffect, renderer);
}

//----------------------------------------------------------------------------

bool	UPopcornFXRendererMaterial::Add(UPopcornFXEffect *parentEffect, const PopcornFX::CParticleRenderer *renderer)
{
	check(ParentEffect.Get() == parentEffect);
	_AddBaseObject(renderer);
	return true;
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

bool	UPopcornFXRendererMaterial::Contains(const FString &particleRendererName)
{
	for (int32 rendereri = 0; rendereri < BaseObjectNames.Num(); ++rendereri)
	{
		if (particleRendererName == BaseObjectNames[rendereri])
		{
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------

bool	UPopcornFXRendererMaterial::CanBeMergeWith(const UPopcornFXRendererMaterial *other) const
{
	if (this == other)
		return true;
	if (SubMaterials.Num() != other->SubMaterials.Num())
		return false;
	for (int32 mati = 0; mati < SubMaterials.Num(); ++mati)
	{
		if (!SubMaterials[mati].CanBeMergeWith(other->SubMaterials[mati]))
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

bool	UPopcornFXRendererMaterial::_ReloadInstance(uint32 subMatId)
{
	if (!HH_VERIFY(subMatId < uint32(SubMaterials.Num())))
		return false;

	FPopcornFXSubRendererMaterial	&subMat = SubMaterials[subMatId];

	if (subMat.Material == null ||
		!HH_VERIFY(uint32(subMat.m_RMId) < HH_ARRAY_COUNT(kRendererMaterialFuncs)))
	{
		subMat.MaterialInstance = null;
		return false;
	}

	FMaterialUpdateContext			materialUpdateContext;

	UMaterialInstanceConstant		*newInstance = NewObject<UMaterialInstanceConstant>(this);
	newInstance->SetFlags(RF_NeedPostLoad | RF_NeedPostLoadSubobjects | RF_Public);
	//newInstance->SetFlags(/*RF_Transactional | */RF_Public);

	newInstance->SetParentEditorOnly(subMat.Material);

	kRendererMaterialFuncs[subMat.m_RMId].m_ParamsConstFunc(this, newInstance, subMatId);

	//newInstance->ForceRecompileForRendering();
	//UMaterialInstance::AllMaterialsCacheResourceShadersForRendering();
	//newInstance->InitStaticPermutation();

	newInstance->ConditionalPostLoad();

	subMat.MaterialInstance = newInstance;

	materialUpdateContext.AddMaterialInstance(newInstance);
	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXRendererMaterial::ReloadInstances()
{
	for (int32 mati = 0; mati < SubMaterials.Num(); ++mati)
		_ReloadInstance(mati);
	PostEditChange();
}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

bool	UPopcornFXRendererMaterial::RendererClassHasMaterial(const PopcornFX::HBO::CHandler *handler)
{
	return handler == PopcornFX::CParticleRenderer_Billboard::m_Handler ||
		handler == PopcornFX::CParticleRenderer_Ribbon::m_Handler ||
		handler == PopcornFX::CParticleRenderer_Mesh::m_Handler;
}

//----------------------------------------------------------------------------

//static
UPopcornFXRendererMaterial		*UPopcornFXRendererMaterial::FindMe(const PopcornFX::CParticleRenderer *renderer)
{
	if (renderer == null)
		return null;
	HH_TODO("Use USER DATA !");

	UPopcornFXFile		*file = FPopcornFXPlugin::Get().GetPopcornFXFile(renderer->File());
	check(file != null);
	UPopcornFXEffect	*effect = Cast<UPopcornFXEffect>(file);
	if (!HH_VERIFY(effect != null))
		return null;
	FString				objPath = renderer->Name();
	for (int32 mati = 0; mati < effect->ParticleRendererMaterials.Num(); ++mati)
	{
		UPopcornFXRendererMaterial	*mat = effect->ParticleRendererMaterials[mati];
		if (mat->Contains(objPath))
			return mat;
	}
	HH_ASSERT_MESSAGE(false, "Could not find the correct material for %s", renderer->Name());
	return null;
}

//----------------------------------------------------------------------------

//static
UPopcornFXRendererMaterial		*UPopcornFXRendererMaterial::FindMe(const PopcornFX::CActionFactoryDecalSpawner *decalFactory)
{
	if (decalFactory == null)
		return null;
	UPopcornFXFile		*file = FPopcornFXPlugin::Get().GetPopcornFXFile(decalFactory->File());
	check(file != null);
	UPopcornFXEffect	*effect = Cast<UPopcornFXEffect>(file);
	if (!HH_VERIFY(effect != null))
		return null;
	FString				objPath = decalFactory->Name();
	for (int32 mati = 0; mati < effect->DecalsRendererMaterials.Num(); ++mati)
	{
		UPopcornFXRendererMaterial	*mat = effect->DecalsRendererMaterials[mati];
		if (mat->Contains(objPath))
			return mat;
	}
	HH_ASSERT_MESSAGE(false, "Could not find the correct decal renderer materials");
	return null;
}

//----------------------------------------------------------------------------

UMaterialInstanceDynamic	*UPopcornFXRendererMaterial::CreateInstance(uint32 subMatId)
{
	if (!HH_VERIFY(subMatId < uint32(SubMaterials.Num())))
		return null;

	UMaterialInstanceDynamic	*material = UMaterialInstanceDynamic::Create(SubMaterials[subMatId].MaterialInstance, this);
	if (material != null)
		material->CopyParameterOverrides(SubMaterials[subMatId].MaterialInstance);
	else
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Couldn't create decal dynamic material instance"));
	}
	return material;
}

//----------------------------------------------------------------------------

//UMaterialInstanceDynamic	*UPopcornFXRendererMaterial::GetInstance(uint32 subMatId, bool forRenderThread)
UMaterialInstanceConstant	*UPopcornFXRendererMaterial::GetInstance(uint32 subMatId, bool forRenderThread) const
{
	if (!HH_VERIFY(subMatId < uint32(SubMaterials.Num())))
		return null;
	const FPopcornFXSubRendererMaterial	&subMat = SubMaterials[subMatId];
	return subMat.MaterialInstance;
	//if (forRenderThread)
	//	return subMat.MaterialInstanceCacheForRenderThread;
	//return subMat.MaterialInstanceCache;
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// Billboards
//
//----------------------------------------------------------------------------

const EPopcornFXMaterialType::Type		kBillboard_Material_ToUE[] =
{
	EPopcornFXMaterialType::Billboard_Additive,				//Additive
	EPopcornFXMaterialType::Billboard_Additive,				//Additive_NoAlpha
	EPopcornFXMaterialType::Billboard_Additive,				//Additive_Soft
	EPopcornFXMaterialType::Billboard_AlphaBlend,			//AlphaBlend
	EPopcornFXMaterialType::Billboard_AlphaBlend,			//AlphaBlend_Soft
	EPopcornFXMaterialType::Billboard_AlphaBlend_Lit,		//AlphaBlend_SoftLighting
	EPopcornFXMaterialType::Billboard_AlphaBlend_Lit,		//AlphaBlend_Lighting
	EPopcornFXMaterialType::Billboard_Distortion,			//AlphaBlend_Distortion
	EPopcornFXMaterialType::Billboard_AlphaBlendAdditive,	//AlphaBlend_AdditiveSoft
	EPopcornFXMaterialType::Billboard_AlphaBlendAdditive_Lit,//AlphaBlend_AdditiveSoftLighting
};
HH_STATIC_ASSERT(HH_ARRAY_COUNT(kBillboard_Material_ToUE) == PopcornFX::CParticleRenderer_Billboard::__MaxMaterials);

//----------------------------------------------------------------------------
//
// Decals
//
//----------------------------------------------------------------------------

const EPopcornFXMaterialType::Type	kDecal_Material_ToUE[] =
{
	EPopcornFXMaterialType::Decal_AlphaTest,	//Material_AlphaTest
	EPopcornFXMaterialType::Decal_Blended,		//Material_Blended
};
HH_STATIC_ASSERT(HH_ARRAY_COUNT(kDecal_Material_ToUE) == PopcornFX::IParticleScene::SDecalDescriptor::__MaxMaterials);

//----------------------------------------------------------------------------
//
// Ribbons
//
//----------------------------------------------------------------------------

const EPopcornFXMaterialType::Type		kRibbon_Material_ToUE[] =
{
	EPopcornFXMaterialType::Billboard_Additive,				// Additive,
	EPopcornFXMaterialType::Billboard_Additive,				// Additive_NoAlpha,
	EPopcornFXMaterialType::Billboard_Additive,				// Additive_Soft,
	EPopcornFXMaterialType::Billboard_AlphaBlend,			// AlphaBlend,
	EPopcornFXMaterialType::Billboard_AlphaBlend,			// AlphaBlend_Soft,
	EPopcornFXMaterialType::Billboard_AlphaBlend_Lit,		// AlphaBlend_SoftLighting,
	EPopcornFXMaterialType::Billboard_AlphaBlend_Lit,		// AlphaBlend_Lighting,
	EPopcornFXMaterialType::Billboard_Distortion,			// AlphaBlend_Distortion,
	EPopcornFXMaterialType::Billboard_AlphaBlendAdditive,	// AlphaBlend_AdditiveSoft,
	EPopcornFXMaterialType::Billboard_AlphaBlendAdditive_Lit,// AlphaBlend_AdditiveSoftLighting,
};
HH_STATIC_ASSERT(HH_ARRAY_COUNT(kRibbon_Material_ToUE) == PopcornFX::CParticleRenderer_Ribbon::__MaxMaterials);

//----------------------------------------------------------------------------
//
// Mesh
//
//----------------------------------------------------------------------------

const EPopcornFXMaterialType::Type		kMesh_Material_ToUE[] =
{
	EPopcornFXMaterialType::Mesh_Additive,			// Additive = 0,
	EPopcornFXMaterialType::Mesh_Additive,			// Additive_NoAlpha,
	EPopcornFXMaterialType::Mesh_Distortion,		// Distortion,
	EPopcornFXMaterialType::Mesh_Solid,				// Solid,
	EPopcornFXMaterialType::Mesh_Solid_Lit,			// Solid_Lighting,
};
HH_STATIC_ASSERT(HH_ARRAY_COUNT(kMesh_Material_ToUE) == PopcornFX::CParticleRenderer_Mesh_Descriptor::__MaxMeshMaterials);

//----------------------------------------------------------------------------
//
// (Helper)
//
//----------------------------------------------------------------------------

UTexture2D		*LoadTexturePk(const PopcornFX::CString &pkPath)
{
	if (pkPath.Empty())
		return null;

	UObject			*obj = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(pkPath.Data(), false);
	UTexture2D		*texture = Cast<UTexture2D>(obj);
	if (obj != null && texture == null)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Asset is not a texture: '%s'"), ANSI_TO_TCHAR(pkPath.Data()));
		return null;
	}
	if (texture == null)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Could not load texture '%s'"), ANSI_TO_TCHAR(pkPath.Data()));
		return null;
	}
	return texture;
}

//----------------------------------------------------------------------------

UPopcornFXTextureAtlas		*LoadAtlasPk(const PopcornFX::CString &pkPath)
{
	if (pkPath.Empty())
		return null;

	UObject					*obj = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(pkPath.Data(), false);
	UPopcornFXTextureAtlas	*atlas = Cast<UPopcornFXTextureAtlas>(obj);
	if (obj != null && atlas == null)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Asset is not an atlas: '%s'"), ANSI_TO_TCHAR(pkPath.Data()));
		return null;
	}
	if (atlas == null)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Could not load atlas '%s'"), ANSI_TO_TCHAR(pkPath.Data()));
		return null;
	}
	return atlas;
}

//----------------------------------------------------------------------------

UStaticMesh		*LoadMeshPk(const PopcornFX::CString &pkPath)
{
	if (pkPath.Empty())
		return null;

	UObject			*obj = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(pkPath.Data(), false);
	UStaticMesh		*mesh = Cast<UStaticMesh>(obj);
	if (obj != null && mesh == null)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Asset is not a mesh: '%s'"), ANSI_TO_TCHAR(pkPath.Data()));
		return null;
	}
	if (mesh == null)
	{
		UE_LOG(LogPopcornFXRendererMaterial, Warning, TEXT("Could not load mesh '%s'"), ANSI_TO_TCHAR(pkPath.Data()));
		return null;
	}
	return mesh;
}

//----------------------------------------------------------------------------

void	SetMaterialTextureParameter(UMaterialInstanceDynamic *mat, FName textureName, const PopcornFX::CString &pkTexturePath)
{
	mat->SetTextureParameterValue(textureName, LoadTexturePk(pkTexturePath.Data()));
}

//----------------------------------------------------------------------------
