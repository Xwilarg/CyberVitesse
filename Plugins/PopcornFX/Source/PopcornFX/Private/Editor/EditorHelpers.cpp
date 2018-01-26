//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#if WITH_EDITOR
#include "EditorHelpers.h"

#include "PopcornFXSDK.h"

#include "AssetToolsModule.h"
#include "NameTypes.h"
#include "FileHelpers.h"
#include "AssetRegistryModule.h"
#include "IAssetTools.h"
#include "Factories/Factory.h"
#include "Editor.h"

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "PopcornFXEditorHelpers"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXEditorHelpers, Log, All);

//----------------------------------------------------------------------------

// Source/Editor/UnrealEd/Private/ObjectTools.cpp 2682
FString				SanitizeObjectPath(const FString &path)
{
	FString SanitizedName;
	FString InvalidChars = INVALID_OBJECTNAME_CHARACTERS;

	// See if the name contains invalid characters.
	FString Char;
	for (int32 CharIdx = 0; CharIdx < path.Len(); ++CharIdx)
	{
		Char = path.Mid(CharIdx, 1);

		if (Char == TEXT("\\"))
		{
			SanitizedName += TEXT("/");
		}
		else if (Char != TEXT("/") && InvalidChars.Contains(*Char))
		{
			SanitizedName += TEXT("_");
		}
		else
		{
			SanitizedName += Char;
		}
	}

	FPaths::RemoveDuplicateSlashes(SanitizedName);

	return SanitizedName;
}

//----------------------------------------------------------------------------

bool			HelperImportFile(const FString &src, const FString& dst, UClass *uclass)
{
	FString			dstDir, dstName, dstExt;
	FPaths::Split(dst, dstDir, dstName, dstExt);

	UObject	*importedObject = null;

	// If the src & dst have the same name, no conflict
	FString	tmp, srcName;
	FPaths::Split(src, tmp, srcName, tmp);
	if (dstName == srcName)
	{
		TArray<FString>	srcFiles;
		srcFiles.Add(src);

		FAssetToolsModule	&assetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		TArray<UObject*>	importedObjects = assetToolsModule.Get().ImportAssets(srcFiles, dstDir);

		HH_ASSERT(importedObjects.Num() <= 1);
		if (importedObjects.Num() == 0 || !HH_VERIFY(importedObjects[0] != null))
		{
			UE_LOG(LogPopcornFXEditorHelpers, Error, TEXT("Could not import '%s' to '%s' in '%s'"), *src, *dstName, *dstDir);
			return false;
		}
		importedObject = importedObjects[0];
	}
	else
	{
		FString			dstPkg = dstDir / dstName;
		HH_ASSERT(SanitizeObjectPath(dstPkg) == dstPkg);

		UPackage		*pkg = CreatePackage(NULL, *dstPkg);
		if (!HH_VERIFY(pkg != null))
		{
			UE_LOG(LogPopcornFXEditorHelpers, Error, TEXT("Could not import '%s': could not create package '%s'"), *src, *dstDir);
			return false;
		}
		bool	bImportWasCancelled = false;

		importedObject = UFactory::StaticImportObject(uclass, pkg, FName(*dstName), RF_Public | RF_Standalone, bImportWasCancelled, *src);
		if (bImportWasCancelled || !HH_VERIFY(importedObject != null))
		{
			UE_LOG(LogPopcornFXEditorHelpers, Error, TEXT("Could not import '%s' to '%s' in '%s'"), *src, *dstName, *dstDir);
			return false;
		}
		FAssetRegistryModule::AssetCreated(importedObject);
		GEditor->BroadcastObjectReimported(importedObject);
	}

	HH_ASSERT(importedObject != null);

	{
		// Hack to force Load and PostLoad assets
		// Or we get later crash in
		// Source\Runtime\Engine\Private\ContentStreaming.cpp 275
		// check(Texture && Texture->Resource);
		auto linker = importedObject->GetLinker();
		if (linker && !importedObject->HasAnyFlags(RF_LoadCompleted))
		{
			//CLog::Log(HH_INFO, "FileSystem FindUObject: Preloading '%s'", TCHAR_TO_ANSI(*p));
			importedObject->SetFlags(RF_NeedLoad | RF_NeedPostLoad);
			linker->Preload(importedObject);
		}
		importedObject->ConditionalPostLoad();
	}


	UE_LOG(LogPopcornFXEditorHelpers, Log, TEXT("Import succeeded: '%s' as '%s'"), *src, *(importedObject->GetPathName()));
	return true;
}

//----------------------------------------------------------------------------

void	ForceSetPackageDiry(UObject *object)
{
	if (object == null || !object->IsValidLowLevel())
		return;
	UPackage		*package = object->GetOutermost();
	if (package == null)
		return;
	if (package->IsDirty())
		return;
	package->SetDirtyFlag(true);
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE

//----------------------------------------------------------------------------
#endif // WITH_EDITOR
