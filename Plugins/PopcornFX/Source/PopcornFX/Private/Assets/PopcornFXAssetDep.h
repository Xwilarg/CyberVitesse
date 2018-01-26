//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"

#include "PopcornFXAssetDep.generated.h"

class	UPopcornFXFile;

UENUM()
namespace EPopcornFXAssetDepType
{
	enum Type
	{
		None,
		Effect,			// path to an effect
		Texture,		// path to a texture : display image browser and preview
		TextureAtlas,	// path to a texture atlas
		Font,			// path to a font
		Mesh,			// path to a mesh
		Video,			// path to a video
		Sound,			// path to an audio source
		SimCache,		// path to a simulation cache
		File			// path to a random file
	};
}
namespace EPopcornFXAssetDepType
{
	enum
	{
		_Count = File + 1
	};
}

EPopcornFXAssetDepType::Type		PkToAssetDepType(uint32 pkFieldType);
UClass								*AssetDepTypeToClass(EPopcornFXAssetDepType::Type type, bool forCreation = false);
bool								AssetDepClassIsCompatible(EPopcornFXAssetDepType::Type type, UClass *uclass);

USTRUCT()
struct FPopcornFXFieldPath
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY()
	FString			BaseObjectName;

	UPROPERTY()
	FString			FieldName;

	bool			Match(const FString &baseObjectName, const FString &fieldName) const
	{
		return BaseObjectName == baseObjectName && FieldName == fieldName;
	}
};

/** Describe an Asset needed by a PopcornFX Asset. */
UCLASS(MinimalAPI, EditInlineNew)
class UPopcornFXAssetDep : public UObject
{
	GENERATED_UCLASS_BODY()

public:

#if WITH_EDITORONLY_DATA
	// can be empty if source path unchanged // use GetSourcePath()
	UPROPERTY(Category="PopcornFX AssetDependencies", VisibleAnywhere)
	FString			SourcePath;

	UPROPERTY(Category="PopcornFX AssetDependencies", VisibleAnywhere)
	FString			ImportPath;

	UPROPERTY(Category="PopcornFX AssetDependencies", VisibleAnywhere)
	TEnumAsByte<EPopcornFXAssetDepType::Type>	Type;
#endif // WITH_EDITORONLY_DATA

	UPROPERTY(Category="PopcornFX AssetDependencies", EditAnywhere, meta=(DisplayThumbnail="true"))
	UObject			*Asset; // keep in game build for referencing ?

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TArray<FPopcornFXFieldPath>		m_Patches;
#endif // WITH_EDITORONLY_DATA

#if WITH_EDITOR

	bool			IsCompatibleClass(UClass *uclass) const;

	bool			Setup(UPopcornFXFile *file, const FString &sourcePathOrNot, const FString &importPath, EPopcornFXAssetDepType::Type type);

	bool			SetAsset(UPopcornFXFile *file, UObject *asset);

	const FString	&GetSourcePath() const { return SourcePath.IsEmpty() ? ImportPath : SourcePath; }
	FString			GetDefaultAssetPath() const;
	UObject			*FindDefaultAsset() const;

	// Import default one IFN, then set to default.
	bool			ImportIFNAndResetDefaultAsset(UPopcornFXFile *file, bool triggerOnAssetDepChanged);

	// Reimport default one, then set to default
	bool			ReimportAndResetDefaultAsset(UPopcornFXFile *file, bool triggerOnAssetDepChanged);

	void			GetAssetPkPath(PopcornFX::CString &outputPkPath) const;

	void			ClearPatches();
	void			AddFieldToPatch(const char *baseObjectName, const char *fieldName);
	void			PatchFields(UPopcornFXFile *file, bool writeFile = true) const;

	UPopcornFXFile	*ParentPopcornFXFile() const;

	bool			Match(const FString &importPath, EPopcornFXAssetDepType::Type type) { return ImportPath == importPath && Type == type; }

	static bool		Conflicts(const FString &importPath, EPopcornFXAssetDepType::Type type, const TArray<UPopcornFXAssetDep*> &otherAssetDeps);

	static void		RenameIfCollision(FString &inOutImportPath, EPopcornFXAssetDepType::Type type, const TArray<UPopcornFXAssetDep*> &otherAssetDeps);

	// overrides UObject
	virtual void	PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void	PostEditUndo() override;

#endif // WITH_EDITOR

};
