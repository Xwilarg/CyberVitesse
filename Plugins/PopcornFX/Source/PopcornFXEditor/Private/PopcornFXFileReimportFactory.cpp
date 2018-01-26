//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXFileReimportFactory.h"

#include "PopcornFXFileFactory.h"
#include "Assets/PopcornFXFile.h"

#include "EditorReimportHandler.h"
#include "AssetRegistryModule.h"
#include "EditorFramework/AssetImportData.h"
#include "UObject/UnrealType.h"

#include "PopcornFXSDK.h"

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXFileReimportFactory, Log, All);

//----------------------------------------------------------------------------

UPopcornFXFileReimportFactory::UPopcornFXFileReimportFactory(const class FObjectInitializer& PCIP)
:	Super(PCIP)
{
	SupportedClass = UPopcornFXFile::StaticClass();
	bCreateNew = false;
}

//----------------------------------------------------------------------------

bool	UPopcornFXFileReimportFactory::CanReimport(UObject *obj, TArray<FString> &outFilenames)
{
	UPopcornFXFile		*file = Cast<UPopcornFXFile>(obj);
	if (file != null && HH_VERIFY(file->IsFileValid()))
	{
		const FString	fileSourcePath = file->FileSourcePath();
		// Avoid adding an empty file source path
		if (HH_VERIFY(!fileSourcePath.IsEmpty()))
		{
#if (ENGINE_MINOR_VERSION >= 9)
			if (HH_VERIFY(file->AssetImportData != null) && fileSourcePath != file->AssetImportData->GetFirstFilename())
			{
				file->AssetImportData->UpdateFilenameOnly(fileSourcePath);
			}
			outFilenames.Add(UAssetImportData::ResolveImportFilename(fileSourcePath, file->GetOutermost()));
#else
			// Auto reimport not supported
			outFilenames.Add(FReimportManager::ResolveImportFilename(fileSourcePath, file));
#endif
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

void	UPopcornFXFileReimportFactory::SetReimportPaths(UObject *obj, const TArray<FString> &newReimportPaths)
{
	if (!HH_VERIFY(newReimportPaths.Num() == 1))
		return;
	UPopcornFXFile	*file = Cast<UPopcornFXFile>(obj);
	if (HH_VERIFY(file != NULL && file->IsFileValid()))
	{
		file->SetFileSourcePath(newReimportPaths[0]);
#if (ENGINE_MINOR_VERSION >= 9)
		if (HH_VERIFY(file->AssetImportData != null))
			file->AssetImportData->UpdateFilenameOnly(newReimportPaths[0]);
#else
		// Auto reimport not supported
#endif
	}
}

//----------------------------------------------------------------------------

EReimportResult::Type	UPopcornFXFileReimportFactory::Reimport(UObject *obj)
{
	UPopcornFXFile	*file = Cast<UPopcornFXFile>(obj);
	if (!HH_VERIFY(file != null))
	{
		UE_LOG(LogPopcornFXFileReimportFactory, Error, TEXT("Cannot reimport: Invalid object"));
		return EReimportResult::Failed;
	}
	if (!HH_VERIFY(file->IsFileValid()))
	{
		UE_LOG(LogPopcornFXFileReimportFactory, Error, TEXT("Cannot reimport: Invalid PopcornFXFile '%s'"), *file->GetPathName());
		return EReimportResult::Failed;
	}

	const FString	fileSourcePath = file->FileSourcePath();
	if (!HH_VERIFY(!fileSourcePath.IsEmpty()))
	{
		UE_LOG(LogPopcornFXFileReimportFactory, Error, TEXT("Cannot reimport: Empty file source path '%s'"), *file->GetPathName());
		return EReimportResult::Failed;
	}
	//file->PreEditChange();

	if (!file->ImportFile(fileSourcePath))
	{
		UE_LOG(LogPopcornFXFileReimportFactory, Error, TEXT("Reimport failed '%s'"), *file->GetPathName());
		return EReimportResult::Failed;
	}

#if (ENGINE_MINOR_VERSION >= 9)
	if (HH_VERIFY(file->AssetImportData != null))
		file->AssetImportData->Update(fileSourcePath);
#else
	// Auto reimport not supported
#endif

	if (file->GetOuter())
		file->GetOuter()->MarkPackageDirty();
	else
		file->MarkPackageDirty();

	// Register any kind of asset, pkat, effect, ..
	//if (!file->IsInAssetRegistry())
	//FAssetRegistryModule::AssetCreated(file);

	file->PostEditChange();

#if WITH_EDITOR
	// massive ugly notify all object that reference this file
	// but works fine
	{
		TArray<FReferencerInformation> extRefs;
		file->RetrieveReferencers(null, &extRefs);
		for (int32 i = 0; i < extRefs.Num(); ++i)
		{
			FReferencerInformation		&ref = extRefs[i];
			if (HH_VERIFY(ref.Referencer != null))
			{
				for (int32 ptyi = 0; ptyi < ref.ReferencingProperties.Num(); ++ptyi)
				{
					// we need to notify templates too, attributes must always be up to date
					//if (ref.Referencer->IsTemplate() || ref.Referencer->IsPendingKill())
					if (ref.Referencer->IsPendingKill())
						continue;

					FPropertyChangedEvent	ptyChangedEvent(const_cast<UProperty*>(ref.ReferencingProperties[ptyi]));
					ref.Referencer->PostEditChangeProperty(ptyChangedEvent);
				}
			}
		}
	}
#endif // WITH_EDITOR

	return EReimportResult::Succeeded;
}

//----------------------------------------------------------------------------

int32	UPopcornFXFileReimportFactory::GetPriority() const
{
	return ImportPriority;
}

//----------------------------------------------------------------------------
