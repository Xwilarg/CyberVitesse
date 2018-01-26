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
#include <hh_particles/include/Renderers/Std/renderer_std.h>
#include <pk_drawers/include/dr_sound.h>

class	UAudioComponent;
class	FAudioDevice;
struct	FActiveSound;
struct	FListener;
class	USoundBase;
class	UWorld;

FWD_PK_API_BEGIN
class	CRendererSubView;
class	CParticleDrawer;
class	CParticleRenderMedium;
class	CParticleRenderer;
class	CParticleRenderManager_Std;
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

using PopcornFX::CRendererSubView;

class	CParticleScene;
class	CSoundDescriptorPoolCollection;
class	CSoundDescriptorPool;


class	CParticleDrawer_Sound : public PopcornFX::CParticleDrawer_Std
{
public:
	static PopcornFX::CParticleDrawer_Std		*BuildRemapper(CParticleScene *scene, PopcornFX::CParticleRenderMedium *renderMedium, PopcornFX::CParticleRenderManager_Std *renderManager);

	CParticleDrawer_Sound();
	~CParticleDrawer_Sound();

	bool					Setup(CParticleScene *scene, PopcornFX::CParticleRenderMedium *renderMedium);

	virtual void			OnParentDescriptorChanged(const PopcornFX::CParticleRenderer *parent) override { }
	virtual void			Clear() override;
	virtual void			Update() override;
	virtual void			HarvestParticleRadiusVisibilityCoefficients(const PopcornFX::CParticleRenderer *rendererDesc, float &outMaxScale, float &outMaxConstant) const override;
	virtual bool			AreRenderersCompatible(const PopcornFX::CParticleRenderer *rendererA, const PopcornFX::CParticleRenderer *rendererB) override;
	virtual bool			CanRenderInView(CRendererSubView &view) const override;
	virtual void			PreRender(CRendererSubView &view) override { }
	virtual void			StartRender(CRendererSubView &view, const CFloat3 &geometricMean, const TMemoryView<const PopcornFX::CParticleRenderMedium::SMediumRendererAssoc> &drawRequests) override;
	virtual void			KickRender(CRendererSubView &view) override;

public:
	PopcornFX::CParticleRenderMedium		*m_RenderMedium = null;
	CParticleScene							*m_Scene = null;

	CSoundDescriptorPoolCollection			*m_SoundPoolCollection = null;

	PopcornFX::Drawers::CSound_DrawRequests	m_DrawRequests;
};

struct	SSoundInsertDesc
{
	// in UE space
	hh_u32		m_SelfID;
	float		m_Age;
	FVector		m_Position;
	FVector		m_Velocity;
	float		m_Radius;
	float		m_DopplerLevel;
	float		m_Volume;
	bool		m_Audible;
};

class	CSoundDescriptor
{
public:
	struct	SUpdateCtx
	{
		uint32			m_CurrentUpdateId;
		UWorld			*m_World;
		USoundBase		*m_Sound;
		SUpdateCtx(uint32 frameId, UWorld *world, USoundBase *sound)
		:	m_CurrentUpdateId(frameId)
		,	m_World(world)
		,	m_Sound(sound)
		{
		}
	};

	CSoundDescriptor() { }
	~CSoundDescriptor();

	void		Clear();

	bool		UsedThisUpdate(uint32 currentUpdateId) const { return currentUpdateId == m_UsedUpdateId; }
	uint32		LastUsedUpdateId() const { return m_UsedUpdateId; }
	hh_u32		SelfID() const { return m_SelfID; }
	FVector		LastPosition() const { return m_LastPosition; }
	bool		Playing() const;

	void		Update(SUpdateCtx &updCtx, const SSoundInsertDesc &insertDesc);
	void		Spawn(SUpdateCtx &updCtx, const SSoundInsertDesc &insertDesc);
	void		Unuse(SUpdateCtx &updCtx);

	void		Stop();

private:
	UAudioComponent		*GetAudioComponentIFP() const;
	UAudioComponent		*GetOrCreateAudioComponent(SUpdateCtx &updCtx);

private:
	hh_u32		m_SelfID = 0;
	uint32		m_UsedUpdateId = 0;
	FVector		m_LastPosition;
	mutable TWeakObjectPtr<UAudioComponent>		m_AudioComponent;
};

class	CSoundDescriptorPoolCollection;

class	CSoundDescriptorPool
{
public:
	CSoundDescriptorPool() {}

	bool			Setup(CSoundDescriptorPoolCollection *poolCollection, const PopcornFX::CParticleRenderer_Sound *rendererDesc, PopcornFX::CParticleRenderer_Sound_Descriptor *soundDesc, UWorld *world);
	void			Clear();

	USoundBase		*GetOrLoadSound();

	void			BeginInsert(UWorld *world);
	void			InsertSoundIFP(const SSoundInsertDesc &insertDesc);
	void			EndInsert(UWorld *world);

private:
	CSoundDescriptorPoolCollection		*m_PoolCollection = null;

	hh_u32		m_SoundsPlaying = 0;
	hh_u32		m_LastUpdatedSlotCount = 0;

	//USoundBase	*m_SoundBase = null;
	FString		m_SoundPath;

	double		m_MaxDeltaPlaying = 0.0;

	PopcornFX::TArray<CSoundDescriptor>		m_Slots;
	PopcornFX::TArray<SSoundInsertDesc>		m_ToSpawn;
};

class	CSoundDescriptorPoolCollection
{
public:
	PopcornFX::TArray<CSoundDescriptorPool>		m_Pools;

public:
	CSoundDescriptorPoolCollection() { }

	void	Clear();

	uint32	CurrentUpdateId() const { return m_CurrentUpdateId; }
	float	CurrentUpdateDt() const { return m_DeltaTime; }
	UWorld	*World() const { return m_World; }

	void	BeginInsert(UWorld *world);
	void	EndInsert(UWorld *world);

private:
	uint32		m_CurrentUpdateId = 0;
	UWorld		*m_World = null;
	double		m_LastTime = 0;
	float		m_DeltaTime = 0;
	
};
