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

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_refptr.h>
#include <hh_particles/include/ps_descriptor.h>

#include "PopcornFXEffect.generated.h"

FWD_PK_API_BEGIN
class	IParticleSceneInterface;
HH_FORWARD_DECLARE(ParticleEffect);
HH_FORWARD_DECLARE(ActionInstance);
HH_FORWARD_DECLARE(ActionFactory);
HH_FORWARD_DECLARE(ParticleAttributeList);
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class UPopcornFXAttributeList;

class UPopcornFXRendererMaterial;

/** PopcornFX Effect Asset imported from a .pkfx file. */
UCLASS(MinimalAPI, BlueprintType)
class UPopcornFXEffect : public UPopcornFXFile
{
	GENERATED_UCLASS_BODY()

public: // if this is private, "in an extrnal package" error will happen !
	// BUT USE GetDefaultAttributeList() !
	UPROPERTY(Category="PopcornFX Attributes", Instanced, VisibleAnywhere)
	UPopcornFXAttributeList					*DefaultAttributeList;

public:
	UPROPERTY(Category="PopcornFX RendererMaterials", Instanced, VisibleAnywhere)
	TArray<UPopcornFXRendererMaterial*>		ParticleRendererMaterials;

	UPROPERTY(Category="PopcornFX RendererMaterials", Instanced, VisibleAnywhere)
	TArray<UPopcornFXRendererMaterial*>		DecalsRendererMaterials;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	uint32	EditorLoopEmitter:1;

	UPROPERTY(meta=(ClampMin="0.0", ClampMax="20.0"))
	float	EditorLoopDelay;
#endif // WITH_EDITORONLY_DATA

	bool					LoadEffectIFN();
	bool					IsLoadCompleted() const;
	bool					EffectIsLoaded() const { HH_ASSERT(IsLoadCompleted()); return m_Loaded; }

	bool									IsTheDefaultAttributeList(const UPopcornFXAttributeList *list) const { return list == DefaultAttributeList; }
	UPopcornFXAttributeList					*GetDefaultAttributeList();

	PopcornFX::PCActionFactory				ActionFactory(); // LoadEffectIFN
	PopcornFX::PCParticleAttributeList		AttributeList(); // LoadEffectIFN
	PopcornFX::PCActionFactory				ActionFactoryIFP() const;
	PopcornFX::PCParticleAttributeList		AttributeListIFP() const;

	// overrides UObject
	virtual void			Serialize(FArchive& Ar) override;
	virtual void			PostLoad() override;
	virtual void			BeginDestroy() override;
	virtual FString			GetDesc() override;

#if WITH_EDITOR
	// overrides UObject
	virtual void			PreEditChange(UProperty* PropertyAboutToChange) override;
	virtual void			PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// overrides FPopcornFXFile
	virtual void			PreReimport_Clean() override;

#endif

private:
	void					ClearEffect();
	bool					LoadEffect();

protected:
	void					ReloadRendererMaterials();

#if WITH_EDITOR
	// overrides FPopcornFXFile
	virtual bool			_ImportFile(const FString &filePath) override;
	virtual bool			FinishImport() override;
	virtual void			OnAssetDepChanged(UPopcornFXAssetDep *assetDep) override;
#endif

	// overrides FPopcornFXFile
	virtual uint64			CurrentCacherVersion() const override;
	virtual bool			LaunchCacher() override;

	virtual void			OnFileUnload() override;
	virtual void			OnFileLoad() override;

private:
	PopcornFX::PCParticleEffect		m_ParticleEffect;
	bool							m_Loaded;
};
