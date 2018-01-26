//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXEffect.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXCustomVersion.h"
#include "Attributes/PopcornFXAttributeList.h"
#include "Internal/ParticleScene.h"
#include "Internal/DependencyHelper.h"
#include "Internal/FileSystemController_UE.h"
#include "Assets/PopcornFXRendererMaterial.h"

#include "Editor/EditorHelpers.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_descriptor.h>
#include <hh_particles/include/ps_renderers_classes.h>
#include <hh_particles/include/ps_scene_actions.h>
#include <hh_base_object/include/hbo_helpers.h>
#include <hh_base_object/include/hbo_details.h> // GatherChilds

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXEffect, Log, All);

//----------------------------------------------------------------------------
//
// UPopcornFXEffect
//
//----------------------------------------------------------------------------

UPopcornFXEffect::UPopcornFXEffect(const FObjectInitializer& PCIP)
	: Super(PCIP)
#if WITH_EDITORONLY_DATA
	, EditorLoopEmitter(false)
	, EditorLoopDelay(2.0f)
#endif // WITH_EDITORONLY_DATA
	, m_Loaded(false)
{
	DefaultAttributeList = CreateDefaultSubobject<UPopcornFXAttributeList>(TEXT("DefaultAttributeList"));
}

//----------------------------------------------------------------------------

bool	UPopcornFXEffect::IsLoadCompleted() const
{
	return HasAnyFlags(RF_LoadCompleted) && DefaultAttributeList != null && DefaultAttributeList->HasAnyFlags(RF_LoadCompleted);
}

//----------------------------------------------------------------------------

UPopcornFXAttributeList		*UPopcornFXEffect::GetDefaultAttributeList()
{
	LoadEffectIFN();

	if (!HH_VERIFY(DefaultAttributeList != null)) // should always be true ?
		return null;
	if (!HH_VERIFY(DefaultAttributeList->IsUpToDate(this)))
		DefaultAttributeList->SetupDefault(this);
	if (!HH_VERIFY(DefaultAttributeList->Valid()))
		return null;
	return DefaultAttributeList;
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

void	UPopcornFXEffect::PreEditChange(UProperty* propertyAboutToChange)
{
	Super::PreEditChange(propertyAboutToChange);
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::PreReimport_Clean()
{
	ParticleRendererMaterials.Empty(ParticleRendererMaterials.Num());
	DecalsRendererMaterials.Empty(DecalsRendererMaterials.Num());

	Super::PreReimport_Clean();
}

//----------------------------------------------------------------------------

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

void	UPopcornFXEffect::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

//----------------------------------------------------------------------------

PopcornFX::PCActionFactory	UPopcornFXEffect::ActionFactory()
{
	if (!LoadEffectIFN())
		return null;
	return m_ParticleEffect->OnSpawn().Get();
}

//----------------------------------------------------------------------------

PopcornFX::PCParticleAttributeList	UPopcornFXEffect::AttributeList()
{
	if (!LoadEffectIFN())
		return null;
	return m_ParticleEffect->CustomAttributes().Get();
}

//----------------------------------------------------------------------------

PopcornFX::PCActionFactory				UPopcornFXEffect::ActionFactoryIFP() const
{
	return m_ParticleEffect == null ? null : m_ParticleEffect->OnSpawn().Get();
}

//----------------------------------------------------------------------------

PopcornFX::PCParticleAttributeList		UPopcornFXEffect::AttributeListIFP() const
{
	return m_ParticleEffect == null ? null : m_ParticleEffect->CustomAttributes().Get();
}

//----------------------------------------------------------------------------

bool	UPopcornFXEffect::LoadEffectIFN()
{
	if (!m_Loaded)
		LoadEffect();
	HH_ASSERT(m_Loaded == (m_ParticleEffect != null));
	return m_Loaded;
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::PostLoad()
{
	Super::PostLoad();


	//LoadEffect(); // not now
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::ClearEffect()
{
	m_Loaded = false;
	m_ParticleEffect = null;
}

//----------------------------------------------------------------------------

bool	UPopcornFXEffect::LoadEffect()
{
	ClearEffect();

	check(IPopcornFXPlugin::IsAvailable());
	if (!HH_VERIFY(IsFileValid()))
		return false;
	m_ParticleEffect = FPopcornFXPlugin::Get().LoadPkObject<PopcornFX::CParticleEffect>(this, "Resource");
	if (m_ParticleEffect == null)
	{
		UE_LOG(LogPopcornFXEffect, Warning/*Error*/, TEXT("Could not load Effect from file: '%s' %d"), *GetPathName(), IsTemplate());
		return false;
	}

	m_Loaded = true;

	DefaultAttributeList->SetupDefault(this);
	return m_Loaded;
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::BeginDestroy()
{
	Super::BeginDestroy();
	check(m_ParticleEffect == null); // OnFileUnload() should have been called
}

//----------------------------------------------------------------------------

FString	UPopcornFXEffect::GetDesc()
{
	FString	description(TEXT("PopcornFX particle system"));
	return description;
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

namespace
{
	struct SDependencyAppend
	{
		UPopcornFXEffect					*m_Self;
		TArray<UPopcornFXAssetDep*>			*m_OldAssetDeps;
		TArray<UPopcornFXAssetDep*>			*m_AssetDeps;

		void	_CbDependency(const PopcornFX::CString &filePath, const PopcornFX::CBaseObject &hbo, hh_u32 fieldIndex)
		{
			TArray<UPopcornFXAssetDep*>		&assetDeps = *m_AssetDeps;
			TArray<UPopcornFXAssetDep*>		&oldAssetDeps = *m_OldAssetDeps;

			if (filePath.Empty())
				return;
			FString	cleanFilePath = filePath.Data();

			const PopcornFX::HBO::CFieldDefinition			&fieldDef = hbo.GetFieldDefinition(fieldIndex);
			const PopcornFX::HBO::CFieldAttributesBase		&fieldAttribs = fieldDef.GetBaseAttributes();
			const EPopcornFXAssetDepType::Type				type = PkToAssetDepType(fieldAttribs.m_Caracs);

			const bool			supported = (AssetDepTypeToClass(type) != null);
			if (!supported)
			{
				UE_LOG(LogPopcornFXEffect, Warning, TEXT("Asset is not supported (yet): '%s' (in %s)"), *cleanFilePath, *(m_Self->GetPathName()));
				return;
			}

			FString				basePath = cleanFilePath;
			if (basePath.EndsWith(TEXT(".pkmm")))
				basePath = basePath.LeftChop(5) + TEXT(".FBX");

			FString				sourcePathOrNot;
			FString				importPath;

			// Sanitize path
			// resolve Conflicts
			{
				FString		dir, name, ext;
				FPaths::Split(basePath, dir, name, ext);

				dir = SanitizeObjectPath(dir);
				name = SanitizeObjectPath(name);
				ext = SanitizeObjectPath(ext);

				importPath = dir / name + TEXT(".") + ext;
				UPopcornFXAssetDep::RenameIfCollision(importPath, type, assetDeps);

				// if the importPath differs from sourcePath, set sourcePathOrNot
				if (importPath != basePath)
					sourcePathOrNot = basePath;
				// leave sourcePathOrNot empty if not changed
			}

			for (int32 i = 0; i < oldAssetDeps.Num(); ++i)
			{
				if (oldAssetDeps[i] != null && oldAssetDeps[i]->Match(importPath, type))
				{
					// found the old one
					UPopcornFXAssetDep		*dep = oldAssetDeps[i];
					oldAssetDeps.RemoveAt(i);

					dep->ClearPatches();

					dep->AddFieldToPatch(hbo.Name(), fieldDef.m_Name);
					assetDeps.Add(dep);
					return;
				}
			}

			for (int32 i = 0; i < assetDeps.Num(); ++i)
			{
				if (HH_VERIFY(assetDeps[i] != null) && assetDeps[i]->Match(importPath, type))
				{
					// import path already exists
					assetDeps[i]->AddFieldToPatch(hbo.Name(), fieldDef.m_Name);
					return;
				}
			}

			UPopcornFXAssetDep		*dep = NewObject<UPopcornFXAssetDep>(m_Self);

			if (dep->Setup(m_Self, sourcePathOrNot, importPath, type))
			{
				dep->AddFieldToPatch(hbo.Name(), fieldDef.m_Name);
				assetDeps.Add(dep);
			}
			else
			{
				UE_LOG(LogPopcornFXEffect, Warning, TEXT("Asset Dependency cannot be resolved: '%s' in effect '%s'"), *importPath, *(m_Self->GetPathName()));
				dep->ConditionalBeginDestroy();
				dep = null;
			}
		}
	};
} // namespace

bool	UPopcornFXEffect::_ImportFile(const FString &filePath)
{
	DefaultAttributeList->Clean();
	ClearEffect();
	if (!Super::_ImportFile(filePath))
		return false;
	return true;
}

bool	UPopcornFXEffect::FinishImport()
{
	if (!LoadEffect())
		return false;

	TArray<UPopcornFXAssetDep*>		oldAssetDeps = AssetDependencies;
	AssetDependencies.Empty();

	const bool		singleWriteFile = true;

	// Re-patch assets paths from Saved overrided AssetDependencies

	SDependencyAppend		dep;
	dep.m_Self = this;
	dep.m_AssetDeps = &AssetDependencies;
	dep.m_OldAssetDeps = &oldAssetDeps;

	PopcornFX::PBaseObjectFile		bofile = m_ParticleEffect->File();
	PopcornFX::GatherDependencies(PkPath(), PopcornFX::CbDependency(&dep, &SDependencyAppend::_CbDependency));
	for (int32 assetDepi = 0; assetDepi < AssetDependencies.Num(); ++assetDepi)
	{
		if (HH_VERIFY(AssetDependencies[assetDepi] != null))
		{
			AssetDependencies[assetDepi]->PatchFields(this, !singleWriteFile);
			AssetDependencies[assetDepi]->SetFlags(RF_Transactional);
		}
	}

	if (singleWriteFile)
		bofile->Write();

	ReloadRendererMaterials();
	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::OnAssetDepChanged(UPopcornFXAssetDep *assetDep)
{
	ReloadRendererMaterials();
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::ReloadRendererMaterials()
{
	// Gather renderers

	if (!LoadEffect())
		return;

	{
		PopcornFX::TArray<PopcornFX::PCParticleRenderer>	renderers;

		PopcornFX::HBO::GatherChilds(PopcornFX::PCBaseObject(m_ParticleEffect.Get()), renderers, false);

		TArray<UPopcornFXRendererMaterial*>					oldMaterials = ParticleRendererMaterials;
		ParticleRendererMaterials.Empty(oldMaterials.Num());

		for (hh_u32 rendereri = 0; rendereri < renderers.Count(); ++rendereri)
		{
			HH_ASSERT(renderers[rendereri] != null);
			const PopcornFX::CParticleRenderer		*renderer = renderers[rendereri].Get();

			UPopcornFXRendererMaterial				*newMat = NewObject<UPopcornFXRendererMaterial>(this);
			if (!newMat->Setup(this, renderer))
			{
				newMat->ConditionalBeginDestroy();
				newMat = null;
				continue;
			}

			// search existing renderer materials
			for (int32 mati = 0; mati < ParticleRendererMaterials.Num(); ++mati)
			{
				UPopcornFXRendererMaterial			*otherMat = ParticleRendererMaterials[mati];
				if (HH_VERIFY(otherMat != null) &&
					newMat->CanBeMergeWith(otherMat))
				{
					otherMat->Add(this, renderer);
					newMat = null;
					break;
				}
			}

			if (newMat == null) // has been inserted
				continue;

			// search old renderer materials
			for (int32 oldi = 0; oldi < oldMaterials.Num(); ++oldi)
			{
				UPopcornFXRendererMaterial	*oldMat = oldMaterials[oldi];
				if (oldMat != null &&
					newMat->CanBeMergeWith(oldMat))
				{
					oldMat->Setup(this, renderer);
					oldMaterials[oldi] = null;
					ParticleRendererMaterials.Add(oldMat);
					newMat = null;
					break;
				}
			}

			if (newMat != null)
			{
				ParticleRendererMaterials.Add(newMat);
				newMat = null;
			}
		}
	}

	{
		PopcornFX::TArray<PopcornFX::PCActionFactoryDecalSpawner>	decals;

		PopcornFX::HBO::GatherChilds(PopcornFX::PCBaseObject(m_ParticleEffect.Get()), decals, false);

		TArray<UPopcornFXRendererMaterial*>			oldDecalMaterials = DecalsRendererMaterials;
		DecalsRendererMaterials.Empty(oldDecalMaterials.Num());
		for (hh_u32 decali = 0; decali < decals.Count(); ++decali)
		{
			const PopcornFX::CActionFactoryDecalSpawner		*decal = decals[decali].Get();
			const FString							decalName = decal->Name();
			UPopcornFXRendererMaterial				*decalMat = null;
			for (int32 oldi = 0; oldi < oldDecalMaterials.Num(); ++oldi)
			{
				if (oldDecalMaterials[oldi] != null &&
					oldDecalMaterials[oldi]->Contains(decalName))
				{
					decalMat = oldDecalMaterials[oldi];
					oldDecalMaterials[oldi] = null;
					if (!decalMat->Setup(this, decal))
						decalMat = null;
					break;
				}
			}
			if (decalMat == null)
			{
				decalMat = NewObject<UPopcornFXRendererMaterial>(this);
				if (!decalMat->Setup(this, decal))
					decalMat = null;
			}

			if (HH_VERIFY(decalMat != null))
				DecalsRendererMaterials.Add(decalMat);
		}
	}

	// Trigger everithing to rebuild renderer drawers

	for (int32 mati = 0; mati < ParticleRendererMaterials.Num(); ++mati)
	{
		ParticleRendererMaterials[mati]->TriggerParticleRenderersModification();
		ParticleRendererMaterials[mati]->SetFlags(RF_Transactional);
	}

	for (int32 mati = 0; mati < DecalsRendererMaterials.Num(); ++mati)
	{
		DecalsRendererMaterials[mati]->TriggerParticleRenderersModification();
		DecalsRendererMaterials[mati]->SetFlags(RF_Transactional);
	}

	MarkPackageDirty();
	if (GetOuter())
		GetOuter()->MarkPackageDirty();

	m_OnFileChanged.Broadcast(this);
}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

uint64	UPopcornFXEffect::CurrentCacherVersion() const
{
	//
	// Effect cacher version contains:
	// - PopcornFX Runtime major minor patch (NOT revid !)
	// - EPopcornFXEffectCacherVersion
	//
	// NOT revid because we don't want to cache at every PopcornFX Runtime commit !
	// Use EPopcornFXEffectCacherVersion to outdate cache when MajMinPatch did not changed
	//

	const uint16	runtimeVer = FPopcornFXPlugin::PopornFXRuntimeMajMinPatch();
	const uint32	runtimeVerShift = (sizeof(uint64) - sizeof(runtimeVer)) * 8;

	enum EPopcornFXEffectCacherVersion
	{
		FirstVersion = 0,

		// Import was missing a rewrite of the file to fix \r\n to \n
		FixCRLFtoLF,

		// Here, add versions to outdate Effect cache

		//----
		_LastVersion_PlusOne,
		_LastVersion = _LastVersion_PlusOne - 1,
	};

	uint64	cacherVersion = 0;
	cacherVersion |= (uint64(runtimeVer) << runtimeVerShift);
	cacherVersion |= _LastVersion;
	return cacherVersion;
}

//----------------------------------------------------------------------------

bool	UPopcornFXEffect::LaunchCacher()
{
	if (!LoadEffect())
		return false;

	// Cache script IR
	{
		UE_LOG(LogPopcornFXEffect, Log, TEXT("Begin script IR caching '%s'..."), *GetPathName());
		PopcornFX::PParticleEffect		effectForWritting = const_cast<PopcornFX::CParticleEffect*>(m_ParticleEffect.Get());
		effectForWritting->SerializeAllScriptCaches();
		UE_LOG(LogPopcornFXEffect, Log, TEXT("...End script IR caching '%s'"), *GetPathName());
	}

	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::OnFileUnload()
{
	ClearEffect();
	Super::OnFileUnload();
}

//----------------------------------------------------------------------------

void	UPopcornFXEffect::OnFileLoad()
{
	Super::OnFileLoad();
	LoadEffect();
}

//----------------------------------------------------------------------------
