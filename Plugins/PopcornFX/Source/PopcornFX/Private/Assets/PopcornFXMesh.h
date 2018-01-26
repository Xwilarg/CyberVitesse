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
#include "PopcornFXFile.h"

#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"

#include "PopcornFXSDK.h"
#include <hh_geometrics/include/ge_mesh_resource.h>

#include "PopcornFXMesh.generated.h"

class UStaticMesh;
class USkeletalMesh;

/** Contains a Mesh's Sampling Acceleration Structures to be used by PopcornFX Particles.
* Kind-of correspond to the .pkmm files.
*/
UCLASS(MinimalAPI)
class UPopcornFXMesh : public UPopcornFXFile
{
	GENERATED_UCLASS_BODY()

public:
	static UPopcornFXMesh		*FindStaticMesh(UStaticMesh *staticMesh);
	static UPopcornFXMesh		*FindSkeletalMesh(USkeletalMesh *skelMesh);

	/* The source static mesh */
	UPROPERTY(Category="PopcornFX Mesh", VisibleAnywhere)
	UStaticMesh*				StaticMesh;

	/* The source skeletal mesh */
	UPROPERTY(Category="PopcornFX Mesh", VisibleAnywhere)
	USkeletalMesh*				SkeletalMesh;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category="PopcornFX Mesh", VisibleAnywhere, Instanced)
	class UAssetImportData		*StaticMeshAssetImportData;
#endif // WITH_EDITORONLY_DATA

	/* Use StaticMesh's sections as submeshes, or merge all sections into one single mesh. */
	UPROPERTY(Category="PopcornFX Mesh", EditAnywhere, BlueprintReadOnly)
	int32						StaticMeshLOD;

	/* Use StaticMesh's sections as submeshes, or merge all sections into one single mesh. */
	UPROPERTY(Category="PopcornFX Mesh", EditAnywhere, BlueprintReadOnly)
	uint32						bMergeStaticMeshSections : 1;

	/* Use StaticMesh's sections as submeshes, or merge all sections into one single mesh. */
	UPROPERTY(Category="PopcornFX Mesh", EditAnywhere, BlueprintReadOnly)
	uint32						bEditorReimport : 1;

	// overrides UObject
	virtual void				PostLoad() override;
	virtual void				BeginDestroy() override;

	PopcornFX::PResourceMesh	LoadResourceMeshIFN(bool editorBuildIFN);

	// in-game force builds from meshes
	void						RebuildFromSource(UStaticMesh *staticMesh);
	void						RebuildFromSource(USkeletalMesh *skelMesh);

#if WITH_EDITOR
	virtual void				PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void				PreReimport_Clean() override;

	bool						SourceMeshChanged() const;
#endif // WITH_EDITOR

	bool						HasSourceMesh() const { return StaticMesh != null || SkeletalMesh != null; }
	void						SetSourceMesh(UStaticMesh *staticMesh, bool dirtifyBuild);
	void						SetSourceMesh(USkeletalMesh *skelMesh, bool dirtifyBuild);
	UObject						*SourceMeshObject() const { if (StaticMesh != null) return StaticMesh; return SkeletalMesh; }

protected:
#if WITH_EDITOR
	virtual bool				_ImportFile(const FString &filePath) override;
#endif
	virtual void				OnFileUnload() override;
	virtual void				OnFileLoad() override;

private:
	static PopcornFX::PResourceMesh		NewResourceMesh(UStaticMesh *staticMesh, uint32 lod, bool mergeSections);

	bool						BuildMeshFromSource();
	bool						BuildStaticMesh();
	bool						BuildSkeletalMesh();
	void						WriteMesh();

private:
	PopcornFX::PResourceMesh	m_ResourceMesh;

};
