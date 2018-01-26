//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXSettingsEditor.h"

#include "PopcornFXPlugin.h"

#if WITH_EDITOR
#	include "XmlFile.h"
#	include "PopcornFXCustomVersion.h"
#	include "Assets/PopcornFXFile.h"
#	include "Settings/EditorLoadingSavingSettings.h"
#	include "Misc/MessageDialog.h"
#	include "UObject/UnrealType.h"
#endif

#define LOCTEXT_NAMESPACE "PopcornFXEditorSettings"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXEditorSettings, Log, All);

//----------------------------------------------------------------------------

UPopcornFXSettingsEditor::UPopcornFXSettingsEditor(const FObjectInitializer& PCIP)
	: Super(PCIP)
#if WITH_EDITORONLY_DATA
	, ImportSourcePack("")
	, bSourcePackFound(false)
	, bAddSourcePackToMonitoredDirectories(false)
	, AssetDependenciesAutoImport(EPopcornFXAssetDependenciesAutoImport::Always)
	, bAlwaysRenderAttributeSamplerShapes(false)
#endif // WITH_EDITORONLY_DATA
{
	static const FString		kDefaultIncludes[] = {
		"*.pkfx", "*.pkat",
		"*.fbx",
		"*.dds", "*.png", "*.jpg", "*.jpeg", "*.tga", "*.exr", "*.hdr", "*.tif",
		"*.mp3", "*.wav", "*.ogg",
		"*.pksc",
	};
	static uint32				kDefaultIncludesCount = sizeof(kDefaultIncludes) / sizeof(*kDefaultIncludes);
	static const FString		kDefaultExcludes[] = {
		"Editor/*",
	};
	static uint32				kDefaultExcludesCount = sizeof(kDefaultExcludes) / sizeof(*kDefaultExcludes);
#if WITH_EDITORONLY_DATA
	AutoReimportMirrorPackWildcards.SetNum(kDefaultIncludesCount + kDefaultExcludesCount);
	uint32		configwci = 0;
	for (uint32 i = 0; i < kDefaultIncludesCount; ++i, ++configwci)
	{
		AutoReimportMirrorPackWildcards[configwci].Wildcard = kDefaultIncludes[i];
		AutoReimportMirrorPackWildcards[configwci].bInclude = true;
	}
	for (uint32 i = 0; i < kDefaultExcludesCount; ++i, ++configwci)
	{
		AutoReimportMirrorPackWildcards[configwci].Wildcard = kDefaultExcludes[i];
		AutoReimportMirrorPackWildcards[configwci].bInclude = false;
	}
#endif // WITH_EDITORONLY_DATA
}

#if WITH_EDITOR

//----------------------------------------------------------------------------

void	UPopcornFXSettingsEditor::PostLoad()
{
	Super::PostLoad();

	// /!\ not actualy called when GetDefault<UPopcornFXSettingsEditor> is used
	UpdateSourcePack();
}

//----------------------------------------------------------------------------

void	UPopcornFXSettingsEditor::PreEditChange(UProperty *propertyAboutToChange)
{
	Super::PreEditChange(propertyAboutToChange);

	if (propertyAboutToChange != null)
	{
		if (propertyAboutToChange->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSettingsEditor, ImportSourcePack))
		{
			SourcePackRootDir_ToRemove = SourcePackRootDir;
		}
		else if (propertyAboutToChange->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSettingsEditor, bAddSourcePackToMonitoredDirectories))
		{
			SourcePackRootDir_ToRemove = SourcePackRootDir;
		}
	}
}

//----------------------------------------------------------------------------

void	UPopcornFXSettingsEditor::PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent)
{
	const UProperty	*propertyThatChanged = propertyChangedEvent.Property;
	if (propertyThatChanged != null)
	{
		if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSettingsEditor, ImportSourcePack))
		{
			UpdateSourcePack();
		}
		else if (propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSettingsEditor, bAddSourcePackToMonitoredDirectories)
#if WITH_EDITORONLY_DATA
				|| propertyThatChanged->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UPopcornFXSettingsEditor, AutoReimportMirrorPackWildcards)
#endif // WITH_EDITORONLY_DATA
				)
		{
			ForceUpdateAutoReimportSettings();
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}

//----------------------------------------------------------------------------

namespace
{

	void		_NotifyReimportManager()
	{
		UObject	*loadingSavingSettings = GetMutableDefault<UEditorLoadingSavingSettings>();
		for (TFieldIterator<UProperty> ptyIt(loadingSavingSettings->GetClass()); ptyIt; ++ptyIt)
		{
			UProperty	*pty = *ptyIt;
			//if (pty != null && pty->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UEditorLoadingSavingSettings, bMonitorContentDirectories))
			//{
			//	loadingSavingSettings->PostEditChangeProperty(FPropertyChangedEvent(pty));
			//	break;
			//}
			if (pty != null && pty->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UEditorLoadingSavingSettings, AutoReimportDirectorySettings))
			{
				FPropertyChangedEvent		chevent(pty);
				loadingSavingSettings->PostEditChangeProperty(chevent);
				break;
			}
		}
	}

	bool		_RemoveAutoReimport(const FString &sourceDir)
	{
		bool		hasChanged = false;
		UEditorLoadingSavingSettings	*loadSaveSettings = GetMutableDefault<UEditorLoadingSavingSettings>();
		if (!HH_VERIFY(loadSaveSettings != null))
			return hasChanged;
		for (int32 i = 0; i < loadSaveSettings->AutoReimportDirectorySettings.Num(); ++i)
		{
			const FAutoReimportDirectoryConfig		&config = loadSaveSettings->AutoReimportDirectorySettings[i];
			if (config.SourceDirectory != sourceDir)
				continue;
			loadSaveSettings->AutoReimportDirectorySettings.RemoveAt(i);
			--i;
			hasChanged = true;
		}
		return hasChanged;
	}

	struct FAutoReimportWildcardComparator
	{
		FString		Wildcard;
		bool		bInclude;

		FAutoReimportWildcardComparator(const FString &wildcard, bool include)
		:	Wildcard(wildcard), bInclude(include) { }
	};
	bool	operator == (const FAutoReimportWildcard &wd, const FAutoReimportWildcardComparator &cmp)
	{
		return wd.Wildcard == cmp.Wildcard && wd.bInclude == cmp.bInclude;
	}

} // namespace

//----------------------------------------------------------------------------

static bool		operator == (const FAutoReimportWildcard &a, const FMyAutoReimportWildcard &b)
{
	return a.Wildcard == b.Wildcard && a.bInclude == b.bInclude;
}

void	UPopcornFXSettingsEditor::_CopyWildcards(FAutoReimportDirectoryConfig &config)
{
	const hh_u32	wildcardCount = AutoReimportMirrorPackWildcards.Num();

	config.Wildcards.SetNum(wildcardCount);
	for (hh_u32 iWildcard = 0; iWildcard < wildcardCount; ++iWildcard)
	{
		const FMyAutoReimportWildcard	&myWildcard = AutoReimportMirrorPackWildcards[iWildcard];
		FAutoReimportWildcard			newWildcard;

		newWildcard.Wildcard = myWildcard.Wildcard;
		newWildcard.bInclude = myWildcard.bInclude;
		config.Wildcards[iWildcard] = newWildcard;
	}
}

bool	UPopcornFXSettingsEditor::_HasSameWildcards(const FAutoReimportDirectoryConfig &config)
{
	const hh_u32	wildcardCount = config.Wildcards.Num();
	if (wildcardCount != AutoReimportMirrorPackWildcards.Num())
		return false;
	for (hh_u32 iWildcard = 0; iWildcard < wildcardCount; ++iWildcard)
	{
		if (!(config.Wildcards[iWildcard] == AutoReimportMirrorPackWildcards[iWildcard]))
			return false;
	}
	return true;
}

void	UPopcornFXSettingsEditor::ForceUpdateAutoReimportSettings()
{
	UEditorLoadingSavingSettings	*loadSaveSettings = GetMutableDefault<UEditorLoadingSavingSettings>();
	if (!HH_VERIFY(loadSaveSettings != null))
		return;

	bool			notify = false;

	// Remove old one IFN
	if (!SourcePackRootDir_ToRemove.IsEmpty() &&
		SourcePackRootDir_ToRemove != SourcePackRootDir)
	{
		notify |= _RemoveAutoReimport(SourcePackRootDir_ToRemove);
	}
	SourcePackRootDir_ToRemove.Empty();

	const bool		autoReimport = !SourcePackRootDir.IsEmpty() && bAddSourcePackToMonitoredDirectories;

	if (!autoReimport)
	{
		// remove everything !
		notify |= _RemoveAutoReimport(SourcePackRootDir);
	}
	else
	{
		const UPopcornFXSettings	*settings = GetMutableDefault<UPopcornFXSettings>();
		check(settings != null);

		// only change if the settings needs to be changed !
		// To avoids _NotifyReimportManager if not nessecary

		const uint32		expectedCount = 1;
		uint32				foundCount = 0;
		bool				hasChanged = false;
		for (int32 i = 0; i < loadSaveSettings->AutoReimportDirectorySettings.Num(); ++i)
		{
			const FAutoReimportDirectoryConfig		&config = loadSaveSettings->AutoReimportDirectorySettings[i];
			if (config.SourceDirectory != SourcePackRootDir)
				continue;
			++foundCount;
			if (foundCount > expectedCount)
				break;
			if (config.MountPoint.IsEmpty() && config.Wildcards.Num() == 0)
				continue;
			if (config.MountPoint == settings->PackMountPoint &&
				_HasSameWildcards(config))
			{
				continue;
			}

			hasChanged = true;
			break;
		}

		if (foundCount != expectedCount)
			hasChanged = true;

		if (hasChanged)
		{
			// remove all
			notify |= _RemoveAutoReimport(SourcePackRootDir);

			const int32		diri = loadSaveSettings->AutoReimportDirectorySettings.Add(FAutoReimportDirectoryConfig());
			check(diri >= 0);
			FAutoReimportDirectoryConfig		&config = loadSaveSettings->AutoReimportDirectorySettings[diri];
			config.SourceDirectory = SourcePackRootDir;
			config.MountPoint = settings->PackMountPoint;

			_CopyWildcards(config);

			notify = true;
		}
	}

	if (notify)
		_NotifyReimportManager();
}


//----------------------------------------------------------------------------

static const FString			kOldPopcornFXProjectFileName = "PopcornProject.xml";
static const FString			kPopcornFXProjectFileName = "PopcornProject.pkproj";

// static
FString		UPopcornFXSettingsEditor::FixAndAppendPopcornFXProjectFileName(const FString &path)
{
	// fix old path
	if (path.EndsWith(kOldPopcornFXProjectFileName))
	{
		FString			newPath = path;
		newPath.RemoveAt(newPath.Len() - kOldPopcornFXProjectFileName.Len(), kOldPopcornFXProjectFileName.Len(), false);
		newPath /= kPopcornFXProjectFileName;
		return newPath;
	}
	// append ifn
	if (!path.EndsWith(kPopcornFXProjectFileName))
		return path / kPopcornFXProjectFileName;
	return path;
}

void	UPopcornFXSettingsEditor::UpdateSourcePack()
{
	// if not in editor, do nothing, keep saved values

	SourcePackProjectFile.Empty();
	SourcePackRootDir.Empty();
	SourcePackThumbnailsDir.Empty();
	bSourcePackFound = 0;

	if (!ImportSourcePack.IsEmpty())
	{
		FString				projectFile = ImportSourcePack;
		FPaths::NormalizeFilename(projectFile);
		FPaths::RemoveDuplicateSlashes(projectFile);

		projectFile = FixAndAppendPopcornFXProjectFileName(projectFile);

		if (FPaths::IsRelative(projectFile))
		{
#if (ENGINE_MINOR_VERSION >= 18)
			projectFile = FPaths::ProjectDir() / projectFile;
#else
			projectFile = FPaths::GameDir() / projectFile;
#endif	// (ENGINE_MINOR_VERSION >= 18)
		}

		SourcePackProjectFile = projectFile;
		int32			lastSlash = projectFile.Find("/", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		if (lastSlash != INDEX_NONE)
			SourcePackRootDir = projectFile.Left(lastSlash + 1);
		SourcePackThumbnailsDir = SourcePackRootDir / "Editor/Thumbnails"; // just in case, the default one

		if (FPaths::FileExists(projectFile))
		{
			FXmlFile	xmlFile(projectFile);
			if (xmlFile.IsValid())
			{
				bSourcePackFound = 1;
				const FXmlNode	*rootNode = xmlFile.GetRootNode();
				if (HH_VERIFY(rootNode != null))
				{
					const FXmlNode	*rootFolderNode = rootNode->FindChildNode(TEXT("RootLocation"));
					if (rootFolderNode != null)
					{
						SourcePackRootDir /= rootFolderNode->GetContent();
					}
					const FXmlNode	*thumbnailsPathNode = rootNode->FindChildNode(TEXT("ThumbnailsFolderName"));
					if (thumbnailsPathNode != null)
					{
						SourcePackThumbnailsDir = SourcePackRootDir / thumbnailsPathNode->GetContent();
					}
				}
			}
		}
		else
		{
			// Log error
		}
		SourcePackProjectFile = FPaths::ConvertRelativePathToFull(SourcePackProjectFile);
		SourcePackRootDir = FPaths::ConvertRelativePathToFull(SourcePackRootDir);
		SourcePackThumbnailsDir = FPaths::ConvertRelativePathToFull(SourcePackThumbnailsDir);
	}

	// Avoid adding invalid source pack folders
	if (bSourcePackFound)
		ForceUpdateAutoReimportSettings();
}

//----------------------------------------------------------------------------

bool	UPopcornFXSettingsEditor::AskForAValidSourcePackForIFN(const FString &sourceAssetPath)
{
	if (!HH_VERIFY(!sourceAssetPath.IsEmpty()))
		return false;

	bool				isValid = ValidSourcePack();
	if (isValid)
	{
		const FString	sourceAssetPathAbs = FPaths::ConvertRelativePathToFull(sourceAssetPath);
		if (!sourceAssetPathAbs.StartsWith(SourcePackRootDir))
		{
			UE_LOG(LogPopcornFXEditorSettings, Error, TEXT("Asset outside Source PopcornFX Project: '%s'"), *sourceAssetPath);
			const FText	title = LOCTEXT("PopcornFXAssetOutsidePackTitle", "PopcornFX: Asset outside Source PopcornFX Project");
			const FText	msg = FText::Format(LOCTEXT("PopcornFXAssetOutsidePackMsg",
													"The asset is outside the Source PopcornFX Project !\n"
													"\n"
													"Asset:\n"
													"{0}\n"
													"\n"
													"Project Settings > PopcornFX Editor > Source PopcornFX Project path:\n"
													"{1}\n"
													"\n"
													"Continue anyway ?\n"), FText::FromString(sourceAssetPath), FText::FromString(SourcePackRootDir));

			return FMessageDialog::Open(EAppMsgType::YesNo, msg, &title) == EAppReturnType::Yes;
		}
		return true;
	}
	const FString		oldImportSourcePath = ImportSourcePack;

	if (!sourceAssetPath.IsEmpty())
	{
		ImportSourcePack = FPaths::ConvertRelativePathToFull(sourceAssetPath);
		HH_ASSERT(!isValid);

		// GetPath will make it loop over all parent folders to find the right one IFP
		while (!isValid)
		{
			const FString	sourcePack = ImportSourcePack;
			FString			newSourcePack = FPaths::GetPath(sourcePack); // dirname !
			// sanity checks !
			if (newSourcePack.Len() < 3 || // matches drive paths and too small to be valid
				newSourcePack.Len() >= sourcePack.Len()) // GetPath does that too
				break;
			if (!FPaths::IsRelative(newSourcePack))
			{
				FString			relativePath = newSourcePack;

#if (ENGINE_MINOR_VERSION >= 18)
				const FString	projectDir = FPaths::ProjectDir();
#else
				const FString	projectDir = FPaths::GameDir();
#endif	// (ENGINE_MINOR_VERSION)

				if (FPaths::MakePathRelativeTo(relativePath, *projectDir) &&
					relativePath.Len() > 1 && relativePath.Len() <= newSourcePack.Len()) // use it only if shorter
				{
					newSourcePack = relativePath;
				}
			}
			if (!HH_VERIFY(newSourcePack[newSourcePack.Len() - 1] != '/'))
			{
				// baaaad, GetPath should not do that ...
				break;
			}
			ImportSourcePack = newSourcePack;
			UpdateSourcePack();
			isValid = ValidSourcePack();
		}
	}

	if (isValid)
	{
		PostEditChange();

		const FText		title = LOCTEXT("PopcornFXSourcePackFoundTitle", "PopcornFX: Source PopcornFX Project found !");
		const FText		text = 
			FText::Format(
				LOCTEXT("PopcornFXSourcePackFoundMsg",
				"Source PopcornFX Project found and saved.\n\nProject Settings > PopcornFX > Source PopcornFX Project path\n\nis now {0}\n"),
				FText::FromString(ImportSourcePack));
		FMessageDialog::Open(EAppMsgType::Ok, text, &title);

		SaveConfig(); // Force save
		return true;
	}
	else
	{
		// Restore old one
		if (ImportSourcePack != oldImportSourcePath)
		{
			ImportSourcePack = oldImportSourcePath;
			UpdateSourcePack();
			PostEditChange();
		}

		UE_LOG(LogPopcornFXEditorSettings, Error, TEXT("Source PopcornFX Project NOT found: '%s'"), *ImportSourcePack);
		const FText	title = LOCTEXT("PopcornFXSourcePackNOTFoundTitle", "PopcornFX: Invalid Source PopcornFX Project path");
		const FText	msg = LOCTEXT(	"PopcornFXSourcePackNOTFoundTitleMsg",
									"Invalid Source PopcornFX Project path, and could automaticaly find one.\n\
									Please setup:\n\
									\n\
									Project Settings > PopcornFX > Source PopcornFX Project path\n\
									\n\
									Continue anyway ?\n");
		return FMessageDialog::Open(EAppMsgType::YesNo, msg, &title) == EAppReturnType::Yes;
	}
	return false;
}

//----------------------------------------------------------------------------
#endif // WITH_EDITOR

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
