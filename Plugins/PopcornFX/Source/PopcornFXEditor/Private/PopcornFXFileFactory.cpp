//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXFileFactory.h"

#include "Assets/PopcornFXEffect.h"
#include "Assets/PopcornFXTextureAtlas.h"
#include "Assets/PopcornFXSimulationCache.h"

#include "ObjectTools.h"
#include "PackageTools.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "EditorFramework/AssetImportData.h"
#include "Editor.h"

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXFileFactory, Log, All);


//----------------------------------------------------------------------------

UPopcornFXFileFactory::UPopcornFXFileFactory(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SupportedClass = UPopcornFXFile::StaticClass();
	Formats.Add(TEXT("pkfx;PopcornFX Effect"));
	// Formats.Add(TEXT("pkmm;PopcornFX mesh"));
	Formats.Add(TEXT("pkat;PopcornFX Texture Atlas"));
	Formats.Add(TEXT("pksc;PopcornFX Simulation Cache"));

	bCreateNew = false;
	bText = false;
	bEditorImport = true;
}

//----------------------------------------------------------------------------

void	UPopcornFXFileFactory::CleanUp()
{
	// Cleanup the factory before further use
	Super::CleanUp();
}

//----------------------------------------------------------------------------

void	UPopcornFXFileFactory::PostInitProperties()
{
	Super::PostInitProperties();
	bEditorImport = true;
	bText = false;
}

//----------------------------------------------------------------------------

bool	UPopcornFXFileFactory::DoesSupportClass(UClass * inClass)
{
	return inClass == UPopcornFXFile::StaticClass() || inClass->IsChildOf(UPopcornFXFile::StaticClass());
}

//----------------------------------------------------------------------------

UClass	*UPopcornFXFileFactory::ResolveSupportedClass()
{
	return SupportedClass;
}

//----------------------------------------------------------------------------

UObject	*UPopcornFXFileFactory::FactoryCreateBinary(UClass *inClass, UObject *inParent, FName inName, EObjectFlags flags, UObject *context, const TCHAR *type, const uint8 *&buffer, const uint8 *bufferEnd, FFeedbackContext *warn, bool &bOutOperationCanceled)
{
	FEditorDelegates::OnAssetPreImport.Broadcast(this, inClass, inParent, inName, type);

	FString				effectName = ObjectTools::SanitizeObjectName(inName.ToString());
	FString				usableResourcePath = UFactory::GetCurrentFilename();

	UClass				*buildClass = null;
	const FString		sourceExt = type;
	if (sourceExt == TEXT("pkfx"))
	{
		buildClass = UPopcornFXEffect::StaticClass();
	}
	else if (sourceExt == TEXT("pkat"))
	{
		buildClass = UPopcornFXTextureAtlas::StaticClass();
	}
	else if (sourceExt == TEXT("pksc"))
	{
		buildClass = UPopcornFXSimulationCache::StaticClass();
	}
	else
	{
		buildClass = UPopcornFXFile::StaticClass();
	}

	UPopcornFXFile	*newFile = Cast<UPopcornFXFile>(CreateOrOverwriteAsset(buildClass, inParent, inName, flags | RF_Public));
	if (!HH_VERIFY(newFile != NULL))
	{
		FEditorDelegates::OnAssetPostImport.Broadcast(this, null);
		return null;
	}

	if (newFile->ImportFile(*usableResourcePath) &&
		HH_VERIFY(newFile->IsFileValid()) &&
		HH_VERIFY(newFile->PkExtension() == sourceExt))
	{
		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(newFile);
#if (ENGINE_MINOR_VERSION >= 9)
		if (HH_VERIFY(newFile->AssetImportData != null))
			newFile->AssetImportData->Update(newFile->FileSourcePath());
#else
		// Auto reimport not supported
#endif
	}
	else
	{
		UE_LOG(LogPopcornFXFileFactory, Error, TEXT("File creation failed '%s' in '%s'"), *newFile->GetPathName(), *inParent->GetPathName());
		bOutOperationCanceled = true;
		newFile->ConditionalBeginDestroy();
		newFile->GetOuter()->ConditionalBeginDestroy();
		newFile = null;
	}
	FEditorDelegates::OnAssetPostImport.Broadcast(this, newFile);

	return newFile;
}

//----------------------------------------------------------------------------
