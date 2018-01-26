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

#include "Render/RendererSubView.h"

#include "PopcornFXSDK.h"

FWD_PK_API_BEGIN
class	CParticleMediumCollection;
FWD_PK_API_END

class UMaterialInterface;

#if	!(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#	define	POPCORNFX_RENDER_DEBUG			1
#else
#	define	POPCORNFX_RENDER_DEBUG			0
#endif

class CParticleRenderManagerBase : public PopcornFX::CRefCountedObject
{
public:
	virtual ~CParticleRenderManagerBase() {}
	virtual void		Clean() = 0;
	virtual void		Clear() = 0;
	virtual void		InstallToMediumCollection(PopcornFX::CParticleMediumCollection &collection) = 0;

	virtual void		GameThread_PreUpdate(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection) = 0;
	virtual void		GameThread_EndUpdate(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection) = 0;
	virtual void		ConcurrentThread_SendRenderDynamicData(const CParticleScene *scene, const PopcornFX::CParticleMediumCollection &collection) = 0;

	virtual void		RenderThread_BuildAndRenderNewFrame(PopcornFX::CRendererSubView &views) = 0;
	virtual void		RenderThread_RenderLastFrame(PopcornFX::CRendererSubView &views) = 0;

	// WeakPtr because Materials can be deleted between
	// the collect and GetUsedMaterials
	::TArray<TWeakObjectPtr<UMaterialInterface> >	m_LastCollectedUsedMaterials;
	bool											m_LastCollectedMaterialsChanged = false;
	bool											m_NeedsBuildNewFrameToRender = false;
};
HH_DECLARE_REFPTRCLASS(ParticleRenderManagerBase);

PParticleRenderManagerBase		NewParticleRenderManager();
