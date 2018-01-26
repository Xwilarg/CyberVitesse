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
#include "PopcornFXSDK.h"

#include "RenderCommandFence.h"

#include "PopcornFXRendererMaterial.generated.h"

FWD_PK_API_BEGIN
class	CParticleRenderer_Billboard;
class	CParticleRenderer_Ribbon;
class	CParticleRenderer_Mesh;
class	CParticleRenderer_Light;
class	CActionFactoryDecalSpawner;
class	CParticleRenderer;
namespace HBO
{
	class	CHandler;
}
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class	UMaterialInterface;
class	UMaterialInstanceConstant;
class	UMaterialInstanceDynamic;
class	UStaticMesh;
class	UTexture;
class	UTexture2D;

class	UPopcornFXEffect;
class	UPopcornFXTextureAtlas;

#define EXEC_X_POPCORNFX_MATERIAL_TYPE()							\
		X_POPCORNFX_MATERIAL_TYPE(Billboard_Additive)				\
		X_POPCORNFX_MATERIAL_TYPE(Billboard_AlphaBlend)				\
		X_POPCORNFX_MATERIAL_TYPE(Billboard_AlphaBlend_Lit)			\
		X_POPCORNFX_MATERIAL_TYPE(Billboard_AlphaBlendAdditive)		\
		X_POPCORNFX_MATERIAL_TYPE(Billboard_AlphaBlendAdditive_Lit)	\
		X_POPCORNFX_MATERIAL_TYPE(Billboard_Distortion)				\
		X_POPCORNFX_MATERIAL_TYPE(Decal_AlphaTest)					\
		X_POPCORNFX_MATERIAL_TYPE(Decal_Blended)					\
		X_POPCORNFX_MATERIAL_TYPE(Mesh_Additive)					\
		X_POPCORNFX_MATERIAL_TYPE(Mesh_Distortion)					\
		X_POPCORNFX_MATERIAL_TYPE(Mesh_Solid)						\
		X_POPCORNFX_MATERIAL_TYPE(Mesh_Solid_Lit)

UENUM()
namespace EPopcornFXMaterialType
{
	enum	Type
	{
		Billboard_Additive,
		Billboard_AlphaBlend,
		Billboard_AlphaBlend_Lit,
		Billboard_AlphaBlendAdditive,
		Billboard_AlphaBlendAdditive_Lit,
		Billboard_Distortion,

		Decal_AlphaTest,
		Decal_Blended,

		Mesh_Additive,
		Mesh_Distortion,
		Mesh_Solid,
		Mesh_Solid_Lit,

		// Put them in the end so that the material remap can work..
		Billboard_Additive_NoAlpha,
		Mesh_Additive_NoAlpha,

		__Max UMETA(Hidden)
	};
}

extern const EPopcornFXMaterialType::Type		kBillboard_Material_ToUE[10];
extern const EPopcornFXMaterialType::Type		kRibbon_Material_ToUE[10];
extern const EPopcornFXMaterialType::Type		kDecal_Material_ToUE[2];
extern const EPopcornFXMaterialType::Type		kMesh_Material_ToUE[5];

USTRUCT(BlueprintType)
struct FPopcornFXSubRendererMaterial
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category="PopcornFX RendererMaterial", EditAnywhere, BlueprintReadOnly, meta=(DisplayThumbnail="true"))
	UMaterialInterface			*Material;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	TEnumAsByte<EPopcornFXMaterialType::Type>	MaterialType;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	UTexture2D					*TextureDiffuse;
	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	UTexture2D					*TextureNormal;
	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	UTexture2D					*TextureSpecular;
	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	UTexture2D					*TextureAlphaRemapper;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	UPopcornFXTextureAtlas		*TextureAtlas;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	uint32						NoAlpha : 1;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	uint32						SoftAnimBlending : 1;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	uint32						CastShadow : 1;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	FString						UserData;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	float						SoftnessDistance;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	int32						DrawOrder;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	UStaticMesh					*StaticMesh;
	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	uint32						StaticMeshLOD;
	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	int32						SubMeshId;

	UPROPERTY()
	int32						m_RMId;

	UPROPERTY(Instanced)
	UMaterialInstanceConstant	*MaterialInstance;

	FPopcornFXSubRendererMaterial();

#if WITH_EDITOR
	bool				_ResetDefaultMaterial_NoReload();
	UMaterialInterface	*FindDefaultMaterial() const;
#endif

	bool				CanBeMergeWith(const FPopcornFXSubRendererMaterial &other) const;

	inline bool			RenderThread_SameMaterial_Billboard(const FPopcornFXSubRendererMaterial &other) const
	{
		return
			Material == other.Material &&
			TextureDiffuse == other.TextureDiffuse &&
			TextureNormal == other.TextureNormal &&
			TextureSpecular == other.TextureSpecular &&
			TextureAlphaRemapper == other.TextureAlphaRemapper &&
			TextureAtlas == other.TextureAtlas &&
			NoAlpha == other.NoAlpha &&
			SoftAnimBlending == other.SoftAnimBlending &&
			// UserData == other.UserData && // not used
			SoftnessDistance == other.SoftnessDistance &&
			DrawOrder == other.DrawOrder &&
			CastShadow == other.CastShadow
			;
	}

	inline bool			RenderThread_SameMaterial_ParamsOnly_Mesh(const FPopcornFXSubRendererMaterial &other) const
	{
		// caller should have check that beforehand
		HH_ASSERT(StaticMesh == other.StaticMesh);
		HH_ASSERT(StaticMeshLOD == other.StaticMeshLOD);
		HH_ASSERT(SubMeshId == other.SubMeshId);
		//HH_ASSERT(Material == other.Material);
		return
			//Material == other.Material && // ! ParamsOnly !
			TextureDiffuse == other.TextureDiffuse &&
			TextureNormal == other.TextureNormal &&
			TextureSpecular == other.TextureSpecular &&
			TextureAlphaRemapper == other.TextureAlphaRemapper &&
			TextureAtlas == other.TextureAtlas && // not a parameter, yet ?
			NoAlpha == other.NoAlpha &&
			SoftAnimBlending == other.SoftAnimBlending &&
			// UserData == other.UserData && // not used
			SoftnessDistance == other.SoftnessDistance &&
			DrawOrder == other.DrawOrder &&
			CastShadow == other.CastShadow
			// StaticMesh == other.StaticMesh && // not material parameters
			// SubMeshId == other.SubMeshId;
			;
	}

	void				ConditionalPostLoadAll();

private:
#if WITH_EDITOR
	FString				GetDefaultMaterialFileName_FallBack() const;
#endif

};

/** Save the Materials needed to Render the PopcornFX Particles of a PopcornFXEffect.
* A PopcornFXRendererMaterial instance match a specific set of PopcornFX CParticleRenderer_*
* batched together for Rendering.
*/
UCLASS(MinimalAPI)
class UPopcornFXRendererMaterial : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
	TWeakObjectPtr<UPopcornFXEffect>			ParentEffect;

	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	TArray<FString>								BaseObjectNames;

	UPROPERTY(Category="PopcornFX RendererMaterial", EditAnywhere, EditFixedSize, NoClear)
	TArray<FPopcornFXSubRendererMaterial>		SubMaterials;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category="PopcornFX RendererMaterial", VisibleAnywhere)
	TArray<FString>								BaseObjectFriendlyNames;
#endif

	virtual void		BeginDestroy() override;
	virtual bool		IsReadyForFinishDestroy() override;

	virtual void		Serialize(FArchive& Ar) override;
	virtual void		PostLoad() override;

#if WITH_EDITOR
	// overrides UObject
	virtual void		PreEditChange(UProperty* PropertyThatWillChange) override;
	virtual void		PostEditChangeChainProperty(struct FPropertyChangedChainEvent & PropertyChangedEvent) override;
	virtual void		PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void		PostEditUndo();
#endif // WITH_EDITOR
#if (ENGINE_MINOR_VERSION >= 13)
	virtual void		PreSave(const class ITargetPlatform* TargetPlatform) override;
#else
	virtual void		PreSave() override;
#endif

#if WITH_EDITOR
	void				TriggerParticleRenderersModification();
	bool				Setup(UPopcornFXEffect *parentEffect, const PopcornFX::CActionFactoryDecalSpawner *decalFactory);
	bool				Setup(UPopcornFXEffect *parentEffect, const PopcornFX::CParticleRenderer *renderer);
	bool				Add(UPopcornFXEffect *parentEffect, const PopcornFX::CParticleRenderer *renderer);

	void				ReloadInstances();
#endif // WITH_EDITOR

	bool				Contains(const FString &particleRendererName);

	bool				CanBeMergeWith(const UPopcornFXRendererMaterial *other) const;

	FPopcornFXSubRendererMaterial		*GetSubMaterial(uint32 index) { if (index >= uint32(SubMaterials.Num())) return null; return &(SubMaterials[index]); }
	const FPopcornFXSubRendererMaterial	*GetSubMaterial(uint32 index) const { if (index >= uint32(SubMaterials.Num())) return null; return &(SubMaterials[index]); }
	uint32								SubMaterialCount() const { return SubMaterials.Num(); }

	static bool								RendererClassHasMaterial(const PopcornFX::HBO::CHandler *handler);
	static UPopcornFXRendererMaterial		*FindMe(const PopcornFX::CParticleRenderer *renderer);
	static UPopcornFXRendererMaterial		*FindMe(const PopcornFX::CActionFactoryDecalSpawner *decalFactory);

	UMaterialInstanceDynamic				*CreateInstance(uint32 subMatId);
	//UMaterialInstanceDynamic				*GetInstance(uint32 subMatId, bool forRenderThread);
	UMaterialInstanceConstant				*GetInstance(uint32 subMatId, bool forRenderThread) const;

private:
#if WITH_EDITOR
	bool				_ReloadInstance(uint32 subMatId);
	bool				_Setup(UPopcornFXEffect *parentEffect, const PopcornFX::CBaseObject *bo);
	void				_AddBaseObject(const PopcornFX::CBaseObject *bo);
#endif

	FRenderCommandFence		m_DetachFence;
};

// Helpers

UTexture2D					*LoadTexturePk(const PopcornFX::CString &pkPath);
UPopcornFXTextureAtlas		*LoadAtlasPk(const PopcornFX::CString &pkPath);
UStaticMesh					*LoadMeshPk(const PopcornFX::CString &pkPath);
void						SetMaterialTextureParameter(UMaterialInstanceDynamic *mat, FName textureName, const PopcornFX::CString &pkTexturePath);

