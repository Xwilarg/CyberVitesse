//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXAssetDep.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXFile.h"
#include "PopcornFXEffect.h"
#include "PopcornFXTextureAtlas.h"
#include "PopcornFXSimulationCache.h"
#include "Internal/FileSystemController_UE.h"

#include "Sound/SoundWave.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#if WITH_EDITOR
#	include "AssetRegistryModule.h"
#	include "Editor/EditorHelpers.h"
#endif // WITH_EDITOR

#include "PopcornFXSDK.h"
#include <hh_base_object/include/hbo_helpers.h>

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXAssetDep, Log, All);

//----------------------------------------------------------------------------

namespace
{
	const EPopcornFXAssetDepType::Type		kPkToAssetDepType[] = {

		EPopcornFXAssetDepType::None,//Caracs_None = 0,
		// Resrouces/Paths:
		EPopcornFXAssetDepType::Effect,//Caracs_ResourceEffect,				// path to an effect
		EPopcornFXAssetDepType::Texture,//Caracs_ResourceTexture,			// path to a texture : display image browser and preview
		EPopcornFXAssetDepType::TextureAtlas,//Caracs_ResourceTextureAtlas,	// path to a texture atlas
		EPopcornFXAssetDepType::Font,//Caracs_ResourceFont,					// path to a font
		EPopcornFXAssetDepType::Mesh,//Caracs_ResourceMesh,					// path to a mesh
		EPopcornFXAssetDepType::None,//Caracs_ResourceAlembic,				// path to an alembic
		EPopcornFXAssetDepType::Video,//Caracs_ResourceVideo,				// path to a video
		EPopcornFXAssetDepType::Sound,//Caracs_ResourceSound,				// path to an audio source
		EPopcornFXAssetDepType::SimCache,//Caracs_ResourceSimCache			// path to a simulation cache
		EPopcornFXAssetDepType::File,//Caracs_Path,							// path to a random file
		EPopcornFXAssetDepType::File,//Caracs_ExternalLink,					// can reference an external HBO, in a different file
		// Others
		EPopcornFXAssetDepType::None,//Caracs_Color,						// RGB or RGBA color
		EPopcornFXAssetDepType::None,//Caracs_Hex,							// should be displayed as Hexadecimal
		EPopcornFXAssetDepType::None,//Caracs_Slider,						// should be displayed as a slider
		EPopcornFXAssetDepType::None,//Caracs_TextBuffer,					// not just a string, should be editable with a larger text editor (ex: script)
	};

	HH_STATIC_ASSERT(HH_ARRAY_COUNT(kPkToAssetDepType) == PopcornFX::HBO::Caracs_TextBuffer + 1);

	//
	// UE naming convention: https://wiki.unrealengine.com/Assets_Naming_Convention
	//
	const TCHAR		*kAssetDepTypePrefix[] = {
		null,					//None,
		TEXT("Pkfx_"),			//Effect,
		TEXT("T_"),				//Texture,
		TEXT("Pkat_"),			//TextureAtlas,
		null/* TEXT("Font_") not supported yet*/,	//Font,
		TEXT("SM_"),			//Mesh,
		TEXT("Video_"),			//Video,
		TEXT("SW_"),			//Sound,
		TEXT("Pksc_"),			//SimCache
		null,					//File
	};
	HH_STATIC_ASSERT(HH_ARRAY_COUNT(kAssetDepTypePrefix) == EPopcornFXAssetDepType::_Count);

}

//----------------------------------------------------------------------------

EPopcornFXAssetDepType::Type		PkToAssetDepType(uint32 pkFieldType)
{
	if (HH_VERIFY(pkFieldType < HH_ARRAY_COUNT(kPkToAssetDepType)))
		return kPkToAssetDepType[pkFieldType];
	return EPopcornFXAssetDepType::None;
}

//----------------------------------------------------------------------------

UClass		*AssetDepTypeToClass(EPopcornFXAssetDepType::Type type, bool forCreation)
{
	switch (type)
	{
	case EPopcornFXAssetDepType::None:
		return null;
		break;
	case EPopcornFXAssetDepType::Effect:
		return null; //UPopcornFXEffect::StaticClass();
		break;
	case EPopcornFXAssetDepType::Texture:
		return UTexture2D::StaticClass();
		break;
	case EPopcornFXAssetDepType::TextureAtlas:
		return UPopcornFXTextureAtlas::StaticClass();
		break;
	case EPopcornFXAssetDepType::Font:
		return null; // not supported yet
		break;
	case EPopcornFXAssetDepType::Mesh:
		return UStaticMesh::StaticClass();
		break;
	case EPopcornFXAssetDepType::Video:
		return null;// ::StaticClass();
		break;
	case EPopcornFXAssetDepType::Sound:
		if (forCreation)
			return USoundWave::StaticClass();
		return USoundBase::StaticClass();
		break;
	case EPopcornFXAssetDepType::SimCache:
		return UPopcornFXSimulationCache::StaticClass();
	case EPopcornFXAssetDepType::File:
		return UPopcornFXFile::StaticClass();
		break;
	}
	return null;
}

//----------------------------------------------------------------------------

bool		AssetDepClassIsCompatible(EPopcornFXAssetDepType::Type type, UClass *uclass)
{
	switch (type)
	{
	case EPopcornFXAssetDepType::None:
		HH_ASSERT_NOT_REACHED();
		return false;
	case EPopcornFXAssetDepType::Effect:
		return uclass->IsChildOf<UPopcornFXEffect>();
	case EPopcornFXAssetDepType::Texture:
		return uclass->IsChildOf<UTexture>();
	case EPopcornFXAssetDepType::TextureAtlas:
		return uclass->IsChildOf<UPopcornFXTextureAtlas>();
	case EPopcornFXAssetDepType::Font:
		HH_ASSERT_NOT_REACHED(); // not supported yet (should not have been called ??)
		return false;
	case EPopcornFXAssetDepType::Mesh:
		return uclass->IsChildOf<UStaticMesh>() ||
			uclass->IsChildOf<USkeletalMesh>();
	case EPopcornFXAssetDepType::Video:
		HH_ASSERT_NOT_REACHED();
		return false;
	case EPopcornFXAssetDepType::Sound:
		return uclass->IsChildOf<USoundBase>();
	case EPopcornFXAssetDepType::SimCache:
		return uclass->IsChildOf<UPopcornFXSimulationCache>();
	case EPopcornFXAssetDepType::File:
		HH_ASSERT_NOT_REACHED();
		return uclass->IsChildOf<UPopcornFXFile>();
	}
	HH_ASSERT_NOT_REACHED();
	return false;
}

//----------------------------------------------------------------------------
//
// FPopcornFXFieldPath
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// UPopcornFXAssetDep
//
//----------------------------------------------------------------------------

UPopcornFXAssetDep::UPopcornFXAssetDep(const FObjectInitializer& PCIP)
	: Super(PCIP)
#if WITH_EDITOR
	, Type(EPopcornFXAssetDepType::None)
#endif
	, Asset(null)
{
}

//----------------------------------------------------------------------------
#if WITH_EDITOR
//----------------------------------------------------------------------------

bool	UPopcornFXAssetDep::IsCompatibleClass(UClass *uclass) const
{
	if (!HH_VERIFY(uclass != null))
		return false;
	return AssetDepClassIsCompatible(Type, uclass);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAssetDep::SetAsset(UPopcornFXFile *file, UObject *asset)
{
	check(ParentPopcornFXFile() == file);

	//if (asset == Asset) // no: in case some one already set Asset
	//	return true;

	Asset = null;
	if (asset == null)
		return false;
	if (!HH_VERIFY(IsCompatibleClass(asset->GetClass())))
		return false;
	Asset = asset;
	PatchFields(file);

	file->OnAssetDepChanged(this);
	return true;
}

//----------------------------------------------------------------------------

FString		UPopcornFXAssetDep::GetDefaultAssetPath() const
{
	return FPopcornFXPlugin::Get().BuildPathFromPkPath(TCHAR_TO_ANSI(*ImportPath), true);
}

//----------------------------------------------------------------------------

UObject		*UPopcornFXAssetDep::FindDefaultAsset() const
{
	return ::LoadObject<UObject>(null, *GetDefaultAssetPath());
}

//----------------------------------------------------------------------------

bool	UPopcornFXAssetDep::ImportIFNAndResetDefaultAsset(UPopcornFXFile *file, bool triggerOnAssetDepChanged)
{
	UObject			*defaultAsset = FindDefaultAsset();
	if (defaultAsset != null)
	{
		if (HH_VERIFY(IsCompatibleClass(defaultAsset->GetClass())))
		{
			Asset = defaultAsset;
			if (triggerOnAssetDepChanged)
				file->OnAssetDepChanged(this);
			return true;
		}
		else
		{
			// UE_LOG(LogPopcornFXAssetDep, Warning, TEXT("Incompatible Asset for %s: '%s'"), *defaultAsset->GetPathName());
		}
	}
	return ReimportAndResetDefaultAsset(file, triggerOnAssetDepChanged);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAssetDep::ReimportAndResetDefaultAsset(UPopcornFXFile *file, bool triggerOnAssetDepChanged)
{
	check(ParentPopcornFXFile() == file);

	const FString	sourcePath = FPopcornFXPlugin::Get().SettingsEditor()->SourcePackRootDir / GetSourcePath();
	if (!FPaths::FileExists(sourcePath))
	{
		UE_LOG(LogPopcornFXAssetDep, Warning, TEXT("Source File '%s' not found to Import"), *sourcePath);
		return false;
	}

	const FString	dstPath = GetDefaultAssetPath();

	HelperImportFile(sourcePath, dstPath, AssetDepTypeToClass(Type, true));

	UObject		*obj = null;
	bool		compatible = false;

	obj = LoadObject<UObject>(null, *dstPath);
	compatible = (obj != null && IsCompatibleClass(obj->GetClass()));

	if (obj == null)
	{
		UE_LOG(LogPopcornFXAssetDep, Warning, TEXT("Failed to import '%s' to '%s'"), *sourcePath, *dstPath);
		return false;
	}
	if (!HH_VERIFY(compatible))
	{
		UE_LOG(LogPopcornFXAssetDep, Warning, TEXT("Imported incompatible asset '%s'"), *sourcePath);
		return false;
	}

	Asset = obj;
	PatchFields(file);

	MarkPackageDirty();

	if (triggerOnAssetDepChanged)
		file->OnAssetDepChanged(this);

	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXAssetDep::GetAssetPkPath(PopcornFX::CString &outputPkPath) const
{
	if (Asset == null)
	{
		outputPkPath = TCHAR_TO_ANSI(*ImportPath);
		return;
	}

	PopcornFX::PFilePack		pack;
	const PopcornFX::CString	assetPath = TCHAR_TO_ANSI(*Asset->GetPathName());
	const PopcornFX::CString	virtPath = PopcornFX::CFilePath::PhysicalToVirtual(assetPath, &pack);
	if (!virtPath.Empty())
	{
		HH_VERIFY(pack == FPopcornFXPlugin::Get().FilePack());
		outputPkPath = virtPath;
		return; // OK
	}

	UE_LOG(LogPopcornFXAssetDep, Warning, TEXT("Asset '%s' is outside the mounted PopcornFX Pack"), ANSI_TO_TCHAR(assetPath.Data()));
	outputPkPath = TCHAR_TO_ANSI(*ImportPath);
}

//----------------------------------------------------------------------------

bool	UPopcornFXAssetDep::Setup(UPopcornFXFile *file, const FString &sourcePathOrNot, const FString &importPath, EPopcornFXAssetDepType::Type type)
{
	check(ParentPopcornFXFile() == file);

	// Clear
	HH_ASSERT(Asset == null);
	HH_ASSERT(SourcePath.IsEmpty());
	HH_ASSERT(ImportPath.IsEmpty());
	//Type = EPopcornFXAssetDepType::None;

	UClass		*assetTypeClass = AssetDepTypeToClass(type);
	if (!HH_VERIFY(assetTypeClass != null))
		return false;

	SourcePath = sourcePathOrNot;
	ImportPath = importPath;
	Type = type;

	UObject		*asset = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(TCHAR_TO_ANSI(*ImportPath), false);
	if (asset != null)
	{
		if (HH_VERIFY(IsCompatibleClass(asset->GetClass())))
		{
			Asset = asset;
		}
		else
			UE_LOG(LogPopcornFXAssetDep, Warning, TEXT("Asset '%s' was found but has incompatible type with %d"), *ImportPath, int32(Type.GetValue()));
	}

	return true;
}

//----------------------------------------------------------------------------

void	UPopcornFXAssetDep::ClearPatches()
{
	m_Patches.Empty(m_Patches.Num());
}

//----------------------------------------------------------------------------

void	UPopcornFXAssetDep::AddFieldToPatch(const char *baseObjectName, const char *fieldName)
{
	const FString			bo = baseObjectName;
	const FString			field = fieldName;
	for (int32 fieldi = 0; fieldi < m_Patches.Num(); ++fieldi)
	{
		if (m_Patches[fieldi].Match(bo, field))
			return;
	}
	FPopcornFXFieldPath		*f = new (m_Patches)FPopcornFXFieldPath();
	check(f != null);
	f->BaseObjectName = bo;
	f->FieldName  = field;
}

//----------------------------------------------------------------------------

void	UPopcornFXAssetDep::PatchFields(UPopcornFXFile *file, bool writeFile /*= true*/) const
{
	check(ParentPopcornFXFile() == file);

	PopcornFX::PBaseObjectFile		bofile = FPopcornFXPlugin::Get().FindPkFile(file);
	const bool						ownsFile = (bofile == null);
	if (bofile == null)
		bofile = FPopcornFXPlugin::Get().LoadPkFile(file, false);

	if (!HH_VERIFY(bofile != null))
		return;

	const PopcornFX::TArray<PopcornFX::PBaseObject>		&objects = bofile->ObjectList();
	if (objects.Empty())
		return;

	PopcornFX::CString		newAssetPath;
	GetAssetPkPath(newAssetPath);
	if (!HH_VERIFY(!newAssetPath.Empty()))
		return;

	PopcornFX::PBaseObject		lastBo;
	FString						lastName;

	for (int32 patchi = 0; patchi < m_Patches.Num(); ++patchi)
	{
		PopcornFX::PBaseObject		bo;
		const FPopcornFXFieldPath	&patch = m_Patches[patchi];

		if (lastName == patch.BaseObjectName)
			bo = lastBo;
		else
		{
			const PopcornFX::CString	pkName = TCHAR_TO_ANSI(*(patch.BaseObjectName));
			for (hh_u32 obji = 0; obji < objects.Count(); ++obji)
			{
				if (objects[obji] != null && objects[obji]->Name() == pkName)
				{
					bo = objects[obji];
				}
			}
		}
		lastName = patch.BaseObjectName;
		lastBo = bo;

		if (bo != null)
		{
			PopcornFX::CGuid		fieldId = bo->GetFieldIndex(TCHAR_TO_ANSI(*(patch.FieldName)));
			if (HH_VERIFY(fieldId.Valid()))
			{
				if (bo->GetField<PopcornFX::CString>(fieldId) != newAssetPath)
					bo->SetField(fieldId, newAssetPath);
			}
		}
	}

	if (writeFile)
	{
		PopcornFX::PFilePack		pack = FPopcornFXPlugin::Get().FilePack();
		if (HH_VERIFY(pack != null))
		{
			HH_ONLY_IF_ASSERTS(bool		writeOk = )bofile->Write(PopcornFX::CFilePackPath(pack, bofile->Path()));
			HH_ASSERT(writeOk);
		}
	}

	if (ownsFile)
	{
		// if we don't have wrote the file, all modification will be lost
		HH_ASSERT(writeFile);

		bofile->Unload();
	}

	MarkPackageDirty();
	if (GetOuter())
		GetOuter()->MarkPackageDirty();

	return;
}

//----------------------------------------------------------------------------

UPopcornFXFile	*UPopcornFXAssetDep::ParentPopcornFXFile() const
{
	UPopcornFXFile		*parentFile = Cast<UPopcornFXFile>(GetOuter());
	HH_ASSERT(parentFile != null);
	return parentFile;
}

//----------------------------------------------------------------------------

bool	UPopcornFXAssetDep::Conflicts(const FString &importPath, EPopcornFXAssetDepType::Type type, const TArray<UPopcornFXAssetDep*> &otherAssetDeps)
{
	const FString			gamePath = FPopcornFXPlugin::Get().BuildPathFromPkPath(TCHAR_TO_ANSI(*importPath), true);
	if (!HH_VERIFY(!gamePath.IsEmpty()))
		return false;

	FAssetRegistryModule&	AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	FAssetData				assetData = AssetRegistryModule.Get().GetAssetByObjectPath(*gamePath);
	if (assetData.IsValid())
	{
		// Retro compat for UPopcornFXFile atlases
		if (type == EPopcornFXAssetDepType::TextureAtlas &&
			assetData.GetClass() == UPopcornFXFile::StaticClass())
			return false;

		if (AssetDepClassIsCompatible(type, assetData.GetClass()))
			return false;

		// types differs
		return true;
	}

	for (int32 i = 0; i < otherAssetDeps.Num(); ++i)
	{
		if (otherAssetDeps[i] != null && otherAssetDeps[i]->Type != type)
		{
			const FString		&other = otherAssetDeps[i]->ImportPath;
			int32				lastPoint = -1;
			if (importPath.FindLastChar('.', lastPoint))
			{
				bool		same = true;
				// strncmp(other, importPath, lastPoint);
				int32		cmpMax = FMath::Min(lastPoint, other.Len());
				for (int32 j = 0; j <= cmpMax; ++j)
				{
					if (other[j] != importPath[j])
					{
						same = false;
						break;
					}
				}
				if (same)
					return true;
			}
			else if (other == importPath)
				return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------

//static
void	UPopcornFXAssetDep::RenameIfCollision(FString &inOutImportPath, EPopcornFXAssetDepType::Type type, const TArray<UPopcornFXAssetDep*> &otherAssetDeps)
{
	if (!HH_VERIFY(!inOutImportPath.IsEmpty()))
		return;

	if (!Conflicts(inOutImportPath, type, otherAssetDeps))
		return;

	FString		dir, filename, ext;
	FPaths::Split(inOutImportPath, dir, filename, ext);

	const TCHAR		*prefix = kAssetDepTypePrefix[type];

	// try prefixing the asset file name
	if (prefix != null)
	{
		const FString		testImportPath = dir / prefix + filename + TEXT(".") + ext;
		if (!Conflicts(testImportPath, type, otherAssetDeps))
		{
			UE_LOG(LogPopcornFXAssetDep, Log, TEXT("Will import '%s' to '%s' (prefixed)"), *inOutImportPath, *testImportPath);
			inOutImportPath = testImportPath;
			return;
		}
	}

	HH_ASSERT_MESSAGE(false, "Could not find an asset of the right type, even with a prefix or suffix");

	return;
}

//----------------------------------------------------------------------------

void	UPopcornFXAssetDep::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	const UProperty	*propertyThatChanged = propertyChangedEvent.Property;
	if (propertyThatChanged != NULL)
	{
		if (propertyThatChanged->GetName() == TEXT("Asset"))
		{
			SetAsset(ParentPopcornFXFile(), Asset);
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

void	UPopcornFXAssetDep::PostEditUndo()
{
	SetAsset(ParentPopcornFXFile(), Asset);
	Super::PostEditUndo();
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
