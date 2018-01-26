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

#include "Render/PopcornFXVertexFactory.h"
#include "Render/RendererSubView.h"
#include "PopcornFXSceneComponent.h"

#include "PrimitiveSceneProxy.h"

class	FParticleAttributes;

class	FPopcornFXSceneProxy : public FPrimitiveSceneProxy
{
public:
	FPopcornFXSceneProxy(UPopcornFXSceneComponent *component);
	~FPopcornFXSceneProxy();

	virtual void					GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;

	virtual void					GatherSimpleLights(const FSceneViewFamily& ViewFamily, FSimpleLightArray& OutParticleLights) const override;
	virtual uint32					GetMemoryFootprint() const;

#if (ENGINE_MINOR_VERSION >= 11)
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
#else
	virtual FPrimitiveViewRelevance	GetViewRelevance(const FSceneView* view) override;
#endif

	virtual void					GetLightRelevance(const FLightSceneProxy * LightSceneProxy, bool & bDynamic, bool & bRelevant, bool & bLightMapped, bool & bShadowMapped) const override;

	virtual const FBoxSphereBounds	&GetBounds() const { check(m_SceneComponent != null); return m_SceneComponent->Bounds; }

	CParticleScene					*ParticleScene() { return m_SceneComponent != null ? m_SceneComponent->ParticleScene() : null; }
	CParticleScene					*ParticleSceneToRender() { return m_SceneComponent != null ? m_SceneComponent->ParticleSceneToRender() : null; }
	CParticleScene					*ParticleSceneToRender() const { return m_SceneComponent != null ? m_SceneComponent->ParticleSceneToRender() : null; }

private:
	UPopcornFXSceneComponent		*m_SceneComponent;

};
