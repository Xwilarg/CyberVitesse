//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "ParticleDrawer_Sound.h"

#include "PopcornFXPlugin.h"
#include "PopcornFXStats.h"
#include "PopcornFXSceneComponent.h"
#include "Internal/ParticleScene.h"
#include "PopcornFXHelper.h"

#include "ActiveSound.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundBase.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_renderers_classes.h>
#include <hh_kernel/include/kr_containers_onstack.h>

using PopcornFX::CParticleRenderer_Sound;

namespace
{

	bool		_EnableAudio(UWorld *world)
	{
		if (GEngine == null || !GEngine->UseSound())
			return false;
		if (world == null)
			return false;
		if (world->GetAudioDevice() == null)
			return false;
		if (world->WorldType != EWorldType::Game &&
			world->WorldType != EWorldType::PIE &&
			world->WorldType != EWorldType::Editor)
			return false;
		return true;
	}

} // namespace


//----------------------------------------------------------------------------

// static
PopcornFX::CParticleDrawer_Std		*CParticleDrawer_Sound::BuildRemapper(CParticleScene *scene, PopcornFX::CParticleRenderMedium *renderMedium, PopcornFX::CParticleRenderManager_Std *renderManager)
{
	//HH_ASSERT(IsInGameThread()); // could be lazy load in worker threads (during update)
	CParticleDrawer_Sound		*newDrawer = HH_NEW(CParticleDrawer_Sound);
	HH_ASSERT(newDrawer != null);
	if (newDrawer != null && newDrawer->Setup(scene, renderMedium))
		return newDrawer;
	HH_DELETE(newDrawer);
	newDrawer = null;
	return newDrawer;
}

//----------------------------------------------------------------------------

CParticleDrawer_Sound::CParticleDrawer_Sound()
	: m_SoundPoolCollection(null)
{
}

//----------------------------------------------------------------------------

CParticleDrawer_Sound::~CParticleDrawer_Sound()
{
	HH_DELETE(m_SoundPoolCollection);
}

//----------------------------------------------------------------------------

bool	CParticleDrawer_Sound::CanRenderInView(CRendererSubView &view) const
{
	return view.Pass() == CRendererSubView::UpdatePass_PostUpdate;
}

//----------------------------------------------------------------------------

bool	CParticleDrawer_Sound::Setup(CParticleScene *scene, PopcornFX::CParticleRenderMedium *renderMedium)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleDrawer_Sound::Setup", POPCORNFX_UE_PROFILER_COLOR);

	//HH_ASSERT(IsInGameThread()); // could be lazy load in worker threads (during update)

	HH_ASSERT(scene != null && renderMedium != null);
	const CParticleRenderer_Sound		*rendererDesc = PopcornFX::HBO::Cast<const CParticleRenderer_Sound>(renderMedium->MediumsAndRenderers().First().m_Renderer.Get());
	HH_ASSERT(rendererDesc != null);
	if (rendererDesc == null)
		return false;

	m_RenderMedium = renderMedium;
	m_Scene = scene;

	const UPopcornFXSceneComponent	*sceneComponent = scene->SceneComponent();
	if (!HH_VERIFY(sceneComponent != null))
		return false;

	UWorld		*world = sceneComponent->GetWorld();
	if (!_EnableAudio(world))
		return false;

	HH_ASSERT(m_SoundPoolCollection == null);
	m_SoundPoolCollection = HH_NEW(CSoundDescriptorPoolCollection());

	const uint32		soundCount = rendererDesc->Sounds().Count();;
	if (!m_SoundPoolCollection->m_Pools.Resize(soundCount))
		return false;
	for (hh_u32 i = 0; i < soundCount; i++)
	{
		PopcornFX::CParticleRenderer_Sound_Descriptor	*soundDesc = rendererDesc->Sounds()[i].Get();
		if (!HH_VERIFY_MESSAGE(soundDesc != null, "Corrupted base object: %s", rendererDesc->RawName().Data()))
			continue;
		m_SoundPoolCollection->m_Pools[i].Setup(m_SoundPoolCollection, rendererDesc, soundDesc, world);
	}
	return true;
}

//----------------------------------------------------------------------------

void	CParticleDrawer_Sound::HarvestParticleRadiusVisibilityCoefficients(const PopcornFX::CParticleRenderer *rendererDesc, float &outMaxScale, float &outMaxConstant) const
{
}

//----------------------------------------------------------------------------

bool	CParticleDrawer_Sound::AreRenderersCompatible(const PopcornFX::CParticleRenderer *rendererA, const PopcornFX::CParticleRenderer *rendererB)
{
	//HH_ASSERT(IsInGameThread()); // could be lazy load in worker threads (during update)

	HH_ASSERT(rendererA != null);
	// we MUST return false,
	// because we only use SelfID, which is uniq only inside ParticleMediums
	// @FIXME: use SelfID+"MediumID?" to make AreRenderersCompatible return rendererA->CompatibleWith(rendererB))
	return false; 
}

//----------------------------------------------------------------------------

void	CParticleDrawer_Sound::StartRender(CRendererSubView &view, const CFloat3 &geometricMean, const TMemoryView<const PopcornFX::CParticleRenderMedium::SMediumRendererAssoc> &drawRequests)
{
	HH_NAMEDSCOPEDPROFILE_C("CParticleDrawer_Sound::StartRender", POPCORNFX_UE_PROFILER_COLOR);
	if (!CParticleDrawer_Sound::CanRenderInView(view))
		return;

	HH_ASSERT(view.Pass() == CRendererSubView::UpdatePass_PostUpdate);
	HH_ASSERT(IsInGameThread());

	const UPopcornFXSceneComponent	*sceneComponent = m_Scene->SceneComponent();
	if (!HH_VERIFY(sceneComponent != null))
		return;
	UWorld		*world = sceneComponent->GetWorld();
	if (!HH_VERIFY(_EnableAudio(world))) // why this changed since Setup ?
		return;
	if (!HH_VERIFY(m_SoundPoolCollection != null)) // why this changed since Setup ?
		return;

	HH_ASSERT(world->WorldType == EWorldType::PIE ||
		world->WorldType == EWorldType::Game ||
		world->WorldType == EWorldType::Editor);

	m_SoundPoolCollection->BeginInsert(world);

	m_DrawRequests.Clear();
	for (hh_u32 dri = 0; dri < drawRequests.Count(); ++dri)
		m_DrawRequests.LockAndLoad(drawRequests[dri]);
	if (m_DrawRequests.Count() == 0)
	{
		m_SoundPoolCollection->EndInsert(world);
		return;
	}

	const hh_u32	soundPoolCount = m_SoundPoolCollection->m_Pools.Count();
	const hh_u32	maxSoundPoolID = soundPoolCount - 1;
	const float		maxSoundPoolsFp = maxSoundPoolID;

	const float		globalScale = view.GlobalScale();

	HH_STACKMEMORYVIEW(float, soundDescVolumes, soundPoolCount);

	for (hh_u32 dri = 0, drCount = m_DrawRequests.Count(); dri < drCount; ++dri)
	{
		const PopcornFX::Drawers::SSound_DrawRequest	&dr = m_DrawRequests[dri];
		HH_ASSERT(dr.Valid());
		HH_ASSERT(dr.ParticleCount() > 0);

		const float		baseVolume = dr.m_Renderer->Volume();
		const float		baseRadius = dr.m_Renderer->Radius();
		const float		baseDopplerLevel = dr.m_Renderer->DopplerLevel();
		const float		isBlended = dr.m_Renderer->SoftSoundBlending();

		HH_ASSERT(dr.m_Renderer->Sounds().Count() == soundPoolCount); // All dr should be compatible, so same Sounds() count
		for (hh_u32 s = 0; s < soundPoolCount; s++)
		{
			const PopcornFX::CParticleRenderer_Sound_Descriptor		*soundDesc = dr.m_Renderer->Sounds()[s].Get();
			soundDescVolumes[s] = /*HH_VERIFY*/(soundDesc != null) ? soundDesc->Volume() : 0.0f;
		}

		const PopcornFX::CParticleStreamToRender_MainMemory		*lockedStream = dr.StreamToRender_MainMemory();
		if (!HH_VERIFY(lockedStream != null))
			continue;
		const hh_u32			pageCount = lockedStream->PageCount();

		for (hh_u32 pagei = 0; pagei < pageCount; ++pagei)
		{
			const PopcornFX::CParticlePageToRender_MainMemory	*page = lockedStream->Page(pagei);
			HH_ASSERT(page != null && !page->Empty());

			const hh_u32	particleCount = page->ParticleCount();

			PopcornFX::TStridedMemoryView<const float>		lifeRatios = page->StreamForReading<float>(dr.m_LifeRatioStreamId);
			PopcornFX::TStridedMemoryView<const float>		invLives = page->StreamForReading<float>(dr.m_InvLifeStreamId);
			PopcornFX::TStridedMemoryView<const hh_i32>		selfIDs = page->StreamForReading<hh_i32>(dr.m_SelfIDStreamId);
			PopcornFX::TStridedMemoryView<const CFloat3>	positions = page->StreamForReading<CFloat3>(dr.m_PositionStreamId);
			PopcornFX::TStridedMemoryView<const CFloat3>	velocities = dr.m_VelocityStreamId.Valid() ? page->StreamForReading<CFloat3>(dr.m_VelocityStreamId) : PopcornFX::TStridedMemoryView<const CFloat3>(&CFloat4::ZERO.xyz(), positions.Count(), 0);
			PopcornFX::TStridedMemoryView<const float>		soundIDs = dr.m_SoundIDStreamId.Valid() ? page->StreamForReading<float>(dr.m_SoundIDStreamId) : PopcornFX::TStridedMemoryView<const float>(&CFloat4::ZERO.x(), positions.Count(), 0);
			PopcornFX::TStridedMemoryView<const float>		volumes = dr.m_VolumeStreamId.Valid() ? page->StreamForReading<float>(dr.m_VolumeStreamId) : PopcornFX::TStridedMemoryView<const float>(&CFloat4::ONE.x(), positions.Count(), 0);
			PopcornFX::TStridedMemoryView<const float>		radii = dr.m_RadiusStreamId.Valid() ? page->StreamForReading<float>(dr.m_RadiusStreamId) : PopcornFX::TStridedMemoryView<const float>(&CFloat4::ONE.x(), positions.Count(), 0);

			const hh_u32	posCount = positions.Count();
			HH_ASSERT(
				posCount == particleCount &&
				posCount == lifeRatios.Count() &&
				posCount == invLives.Count() &&
				posCount == selfIDs.Count() &&
				posCount == velocities.Count() &&
				posCount == soundIDs.Count() &&
				posCount == volumes.Count() &&
				posCount == radii.Count());

			for (hh_u32 i = 0; i < posCount; ++i)
			{
				const float		volume = baseVolume * volumes[i];

				// Prefer volume to 0, rather than Stop. Stop cracks.
				// Leave that decision to InsertSoundIFP anyway
//				if (volume <= 1.0e-6f)
//					continue;

				const FVector	pos = ToUE(positions[i] * globalScale);
				const float		radius = baseRadius * radii[i] * globalScale;

				const bool		audible = world->GetAudioDevice()->LocationIsAudible(pos, radius + radius * 0.5f);
				const float		soundId = PopcornFX::HHMin(soundIDs[i], maxSoundPoolsFp);
				const float		soundIdFrac = PopcornFX::HHFrac(soundId) * isBlended;
				const hh_u32	soundId0 = hh_u32(soundId);
				const hh_u32	soundId1 = PopcornFX::HHMin(soundId0 + 1, maxSoundPoolID);

				SSoundInsertDesc	desc;
				desc.m_SelfID = hh_u32(selfIDs[i]);
				desc.m_Position = pos;
				desc.m_Velocity = ToUE(velocities[i] * globalScale);
				desc.m_Radius = radius;
				desc.m_DopplerLevel = baseDopplerLevel;
				desc.m_Age = lifeRatios[i] / invLives[i];
				desc.m_Audible = audible;

				desc.m_Volume = volume * soundDescVolumes[soundId0] * (1.0f - soundIdFrac);
				m_SoundPoolCollection->m_Pools[soundId0].InsertSoundIFP(desc);

				if (!isBlended)
					continue;

				// Leave that decision to InsertSoundIFP
//				if (soundIdFrac <= 1.0e-3f)
//					continue;
				desc.m_Volume = volume * soundDescVolumes[soundId1] * soundIdFrac;
				m_SoundPoolCollection->m_Pools[soundId1].InsertSoundIFP(desc);
			}
		}
	}

	m_SoundPoolCollection->EndInsert(world);

	m_DrawRequests.ClearAndUnlock();
}

//----------------------------------------------------------------------------

void	CParticleDrawer_Sound::KickRender(CRendererSubView &view)
{
}

//----------------------------------------------------------------------------

void	CParticleDrawer_Sound::Clear()
{
	if (m_SoundPoolCollection != null)
		m_SoundPoolCollection->Clear();
}

//----------------------------------------------------------------------------

void	CParticleDrawer_Sound::Update()
{
}

//----------------------------------------------------------------------------
//
//	Sound pools
//
//----------------------------------------------------------------------------

CSoundDescriptor::~CSoundDescriptor()
{
	Clear();
}

//----------------------------------------------------------------------------

void	CSoundDescriptor::Clear()
{
	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp != null)
	{
		comp->ConditionalBeginDestroy();
		m_AudioComponent = null;
	}
}

//----------------------------------------------------------------------------

UAudioComponent		*CSoundDescriptor::GetAudioComponentIFP() const
{
	UAudioComponent		*comp = m_AudioComponent.Get();
	if (comp != null)
	{
		if (!comp->IsValidLowLevel()) // TWeakObjectPtr should have checked that
		{
			comp = null;
			m_AudioComponent = null;
		}
		else if (!comp->IsRegistered())
		{
			// respawn
			comp->ConditionalBeginDestroy();
			comp = null;
			m_AudioComponent = null;
		}
	}
	return comp;
}

//----------------------------------------------------------------------------

UAudioComponent		*CSoundDescriptor::GetOrCreateAudioComponent(SUpdateCtx &updCtx)
{
	if (!HH_VERIFY(updCtx.m_World != null && updCtx.m_Sound != null))
		return null;
	UWorld				*world = updCtx.m_World;
	USoundBase			*sound = updCtx.m_Sound;
	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp != null)
	{
		HH_ASSERT(comp->GetWorld() == world);
		HH_ASSERT(comp->Sound == sound);
		return comp;
	}

	// Create one
	FAudioDevice::FCreateComponentParams	params(updCtx.m_World);
#if (ENGINE_MINOR_VERSION >= 16)
	params.bAutoDestroy = false;	// TODO: Test how this affects 4.15
#endif	// (ENGINE_MINOR_VERSION >= 16)
	params.bPlay = false;
	params.bStopWhenOwnerDestroyed = true;
	comp = FAudioDevice::CreateComponent(updCtx.m_Sound, params);
	HH_ASSERT(comp != null);
	m_AudioComponent = comp;
	return comp;
}

//----------------------------------------------------------------------------

// @TODO optiom: cache Playing ?
bool	CSoundDescriptor::Playing() const
{
	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp == null)
		return false;
	return comp->IsPlaying();
}

//----------------------------------------------------------------------------

void	CSoundDescriptor::Update(SUpdateCtx &updCtx, const SSoundInsertDesc &insertDesc)
{
	UWorld				*world = updCtx.m_World;
	USoundBase			*sound = updCtx.m_Sound;

	m_UsedUpdateId = updCtx.m_CurrentUpdateId;

	//float		startpos = insertDesc.m_StartPos;
	//startpos -= FMath::Fmod(startpos, 1.0 / 120.0);
	HH_ASSERT(m_SelfID == insertDesc.m_SelfID);

	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp == null)
	{
		// Create one
		FAudioDevice::FCreateComponentParams	params(updCtx.m_World);
#if (ENGINE_MINOR_VERSION >= 16)
		params.bAutoDestroy = false;	// TODO: Test how this affects 4.15
#endif	// (ENGINE_MINOR_VERSION >= 16)
		params.bPlay = false;
		params.bStopWhenOwnerDestroyed = true;
		params.SetLocation(insertDesc.m_Position);
		comp = FAudioDevice::CreateComponent(updCtx.m_Sound, params);
		HH_ASSERT(comp != null);
		m_AudioComponent = comp;
		comp->Play(0);//insertDesc.m_Age);
		//PopcornFX::CLog::Log(HH_INFO, "Sound Update Spawned %d %p %f", m_UsedUpdateId, comp, insertDesc.m_Age);
	}

	if (m_LastPosition != insertDesc.m_Position)
	{
		m_LastPosition = insertDesc.m_Position;
		comp->SetWorldLocation(m_LastPosition);
	}

	if (comp->VolumeMultiplier != insertDesc.m_Volume)
	{
		comp->SetVolumeMultiplier(insertDesc.m_Volume);
	}

//	comp->ComponentVelocity = insertDesc.m_Velocity; // don't really care, FAudioDevice compute it's own Pos- PrevPos

	bool	isPlaying = comp->IsPlaying();
	// @TODO if playing access FActiveSound for faster seek without full re-setup ?
	if (!isPlaying)
	{
		//PopcornFX::CLog::Log(HH_INFO, "Sound Update Start %d cp:%d %p %f", m_UsedUpdateId, comp, isPlaying, insertDesc.m_Age);
		//comp->Play(insertDesc.m_Age);
		comp->Play(0);
		//float	time = (HH_VERIFY(comp->Sound != null) ? FMath::Fmod(m_StartPos, comp->Sound->Duration) : 0);
		//comp->Play(m_StartPos);
	}
}

//----------------------------------------------------------------------------

void	CSoundDescriptor::Spawn(SUpdateCtx &updCtx, const SSoundInsertDesc &insertDesc)
{
	UWorld				*world = updCtx.m_World;
	USoundBase			*sound = updCtx.m_Sound;

	m_UsedUpdateId = updCtx.m_CurrentUpdateId;

	//float		startpos = insertDesc.m_StartPos;
	//startpos -= FMath::Fmod(startpos, 1.0 / 120.0);
	//m_LastAge = insertDesc.m_Age;
	m_SelfID = insertDesc.m_SelfID;

	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp != null)
	{
		//comp->Stop();
		comp->ConditionalBeginDestroy();
		comp = null;
		m_AudioComponent = null;
		//comp->FadeOut(0.1f, 0.f);
	}

	if (comp == null)
	{
		// Create one
		FAudioDevice::FCreateComponentParams	params(updCtx.m_World);
#if (ENGINE_MINOR_VERSION >= 16)
		params.bAutoDestroy = false;	// TODO: Test how this affects 4.15
#endif	// (ENGINE_MINOR_VERSION >= 16)
		params.bPlay = false;
		params.bStopWhenOwnerDestroyed = true;
		params.SetLocation(insertDesc.m_Position);
		comp = FAudioDevice::CreateComponent(updCtx.m_Sound, params);
		HH_ASSERT(comp != null);
		m_AudioComponent = comp;
		comp->Play(0/*m_LastAge*/);
		//PopcornFX::CLog::Log(HH_INFO, "Sound Spawn Spawned %d %p %f", m_UsedUpdateId, comp, 0.0f/*m_LastAge*/);
	}

	if (m_LastPosition != insertDesc.m_Position)
	{
		m_LastPosition = insertDesc.m_Position;
		comp->SetWorldLocation(m_LastPosition);
	}

	if (comp->VolumeMultiplier != insertDesc.m_Volume)
	{
		comp->SetVolumeMultiplier(insertDesc.m_Volume);
	}

	//	comp->ComponentVelocity = insertDesc.m_Velocity; // don't really care, FAudioDevice compute it's own Pos- PrevPos

	bool	isPlaying = comp->IsPlaying();
	// @TODO if playing access FActiveSound for faster seek without full re-setup ?
	if (!isPlaying)
	{
		//PopcornFX::CLog::Log(HH_INFO, "Sound Spawn Start cp:%d %p p:%d %f", m_UsedUpdateId, comp, isPlaying, 0.0f/*m_LastAge*/);
		comp->Play(0/*m_LastAge*/);
		//float	time = (HH_VERIFY(comp->Sound != null) ? FMath::Fmod(m_StartPos, comp->Sound->Duration) : 0);
		//comp->Play(m_StartPos);
	}
}

//----------------------------------------------------------------------------

void	CSoundDescriptor::Unuse(SUpdateCtx &updCtx)
{
	//if (UsedThisUpdate(updCtx.m_CurrentUpdateId))
	//	return;
	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp == null)
		return;

	m_UsedUpdateId = 0;
	m_SelfID = 0;
	//m_UsedUpdateId = updCtx.m_CurrentUpdateId;
	//m_UsedUpdateId = 0;
	//m_StartPos = insertDesc.m_StartPos;

	if (comp->IsPlaying())
	{
		//PopcornFX::CLog::Log(HH_INFO, "Sound Unuse %d(last use: %d) %p %f", updCtx.m_CurrentUpdateId, m_UsedUpdateId, comp, 0.0f/*m_LastAge*/);
		comp->Stop();
	}

	//comp->Stop();
}

//----------------------------------------------------------------------------

void	CSoundDescriptor::Stop()
{
	UAudioComponent		*comp = GetAudioComponentIFP();
	if (comp == null)
		return;
	//comp->bAutoDestroy = true;
	//comp->Stop();
	comp->ConditionalBeginDestroy();
	m_AudioComponent = null;
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

void	CSoundDescriptorPool::Clear()
{
	for (hh_u32 i = 0, slotCount = m_Slots.Count(); i < slotCount; ++i)
		m_Slots[i].Clear();
	m_SoundsPlaying = 0;
}

//----------------------------------------------------------------------------

// @FIXME cache loaded sound ? or inc ref count ? but should alreay ref by effect ?

USoundBase	*CSoundDescriptorPool::GetOrLoadSound()
{
	return ::LoadObject<USoundBase>(null, *m_SoundPath);
}

//----------------------------------------------------------------------------

bool	CSoundDescriptorPool::Setup(CSoundDescriptorPoolCollection *poolCollection, const PopcornFX::CParticleRenderer_Sound *rendererDesc, PopcornFX::CParticleRenderer_Sound_Descriptor *soundDesc, UWorld *world)
{
	m_PoolCollection = poolCollection;

	static const hh_u32		kPoolSize = 10;

	m_SoundPath = FPopcornFXPlugin::Get().BuildPathFromPkPath(soundDesc->SoundPath().Data(), true);
	if (GetOrLoadSound() == null)
		return false;
	if (!m_Slots.Resize(kPoolSize))
		return false;

	return true;
}

//----------------------------------------------------------------------------

void	CSoundDescriptorPool::BeginInsert(UWorld *world)
{
	m_SoundsPlaying = 0;
}

//----------------------------------------------------------------------------

void	CSoundDescriptorPool::InsertSoundIFP(const SSoundInsertDesc &insertDesc)
{
	UWorld			*world = m_PoolCollection->World();
	const uint32	currentUpdateId = m_PoolCollection->CurrentUpdateId();

//	const float		dt = m_PoolCollection->CurrentUpdateDt();
//	const float		dt = m_PoolCollection->World()->GetAudioDevice()->GetDeviceDeltaTime();
//	const float		maxAllowedDelta = dt + 0.9e-2f;

	USoundBase		*sound = GetOrLoadSound();
	if (!HH_VERIFY(sound != null))
	{
		Clear();
		return;
	}

	CSoundDescriptor::SUpdateCtx	updCtx(currentUpdateId, world, sound);
	HH_ASSERT(m_LastUpdatedSlotCount <= m_Slots.Count());
	bool	spawnLater = true;
	if (m_SoundsPlaying < m_Slots.Count())
	{
		PopcornFX::CGuid	slot;
		const hh_u32		insertSelfID = insertDesc.m_SelfID;
		for (hh_u32 i = 0; i < m_LastUpdatedSlotCount; ++i)
		{
			const CSoundDescriptor		&sd = m_Slots[i];
			if (sd.SelfID() != insertSelfID)
				continue;
			slot = i;
			break;
		}
		if (slot.Valid())
		{
			m_SoundsPlaying++;
			m_Slots[slot].Update(updCtx, insertDesc);
			spawnLater = false;
		}
	}
	if (spawnLater)
	{
		m_ToSpawn.PushBack(insertDesc);
	}
}

//----------------------------------------------------------------------------

void	CSoundDescriptorPool::EndInsert(UWorld *world)
{
	HH_ASSERT(world == m_PoolCollection->World());

	const uint32					currentUpdateId = m_PoolCollection->CurrentUpdateId();
	USoundBase						*sound = GetOrLoadSound();
	CSoundDescriptor::SUpdateCtx	updCtx(currentUpdateId, world, sound);

	hh_u32				tospawni = 0;

	PopcornFX::CGuid	lastUsed = 0;

	HH_ASSERT(m_LastUpdatedSlotCount <= m_Slots.Count());
	hh_u32		i = 0;
	for (; i < m_LastUpdatedSlotCount; ++i) // after m_LastUpdatedSlotCount: already unused
	{
		CSoundDescriptor	&sd = m_Slots[i];
		if (sd.UsedThisUpdate(currentUpdateId))
		{
			lastUsed = i;
			continue;
		}
		if (sound != null && tospawni < m_ToSpawn.Count())
		{
			sd.Spawn(updCtx, m_ToSpawn[tospawni]);
			++m_SoundsPlaying;
			++tospawni;
			lastUsed = i;
		}
		else
		{
			sd.Unuse(updCtx);
		}
	}
	// new sounds:
	if (sound != null && tospawni < m_ToSpawn.Count())
	{
		const hh_u32	remainingToSpawnCount = m_ToSpawn.Count() - tospawni;
		const hh_u32	finalCount = i + remainingToSpawnCount;
		if (m_Slots.Count() < finalCount)
		{
			if (!HH_VERIFY(m_Slots.Resize(finalCount)))
				return;
		}
		for (; tospawni < m_ToSpawn.Count(); ++i)
		{
			CSoundDescriptor	&sd = m_Slots[i];
			sd.Spawn(updCtx, m_ToSpawn[tospawni]);
			++tospawni;
			lastUsed = i;
		}
	}

	m_LastUpdatedSlotCount = lastUsed.Valid() ? hh_u32(lastUsed) + 1U : 0U;

	INC_DWORD_STAT_BY(STAT_PopcornFX_SoundParticleCount, m_SoundsPlaying);

	m_ToSpawn.Clear();
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

void	CSoundDescriptorPoolCollection::Clear()
{
	for (hh_u32 i = 0, poolCount = m_Pools.Count(); i < poolCount; ++i)
		m_Pools[i].Clear();
}

void	CSoundDescriptorPoolCollection::BeginInsert(UWorld *world)
{
	m_World = world;
	++m_CurrentUpdateId;

	const double	currTime = FPlatformTime::Seconds();
	m_DeltaTime = currTime - m_LastTime;
	m_LastTime = currTime;

	for (hh_u32 i = 0; i < m_Pools.Count(); ++i)
	{
		m_Pools[i].BeginInsert(world);
	}
}

void	CSoundDescriptorPoolCollection::EndInsert(UWorld *world)
{
	HH_ASSERT(m_World == world);
	for (hh_u32 i = 0; i < m_Pools.Count(); ++i)
	{
		m_Pools[i].EndInsert(world);
	}
}

//----------------------------------------------------------------------------
