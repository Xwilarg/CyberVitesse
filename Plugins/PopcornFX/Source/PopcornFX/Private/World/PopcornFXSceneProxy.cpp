//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXSceneProxy.h"

#include "PopcornFXSceneComponent.h"
#include "Render/RendererSubView.h"
#include "Render/PopcornFXVertexFactoryShaderParameters.h"
#include "Internal/ParticleScene.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_mediums.h>

//----------------------------------------------------------------------------

FPopcornFXSceneProxy::FPopcornFXSceneProxy(UPopcornFXSceneComponent *component)
:	FPrimitiveSceneProxy(component)
{
	m_SceneComponent = component;

	bVerifyUsedMaterials = true;
}

//----------------------------------------------------------------------------

FPopcornFXSceneProxy::~FPopcornFXSceneProxy()
{
}

void	FPopcornFXSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	CParticleScene		*particleScene = ParticleSceneToRender();
	if (particleScene == null)
		return;
	particleScene->GetDynamicMeshElements(this, Views, ViewFamily, VisibilityMap, Collector);
}

//----------------------------------------------------------------------------

void	FPopcornFXSceneProxy::GatherSimpleLights(const FSceneViewFamily& ViewFamily, FSimpleLightArray& OutParticleLights) const
{
	const CParticleScene		*particleScene = ParticleSceneToRender();
	if (particleScene == null)
		return;
	particleScene->GatherSimpleLights(ViewFamily, OutParticleLights);
}

//----------------------------------------------------------------------------

uint32	FPopcornFXSceneProxy::GetMemoryFootprint() const
{
	return sizeof(*this) + FPrimitiveSceneProxy::GetAllocatedSize();
}

//----------------------------------------------------------------------------

#if (ENGINE_MINOR_VERSION >= 11)
FPrimitiveViewRelevance	FPopcornFXSceneProxy::GetViewRelevance(const FSceneView* view) const
#else
FPrimitiveViewRelevance	FPopcornFXSceneProxy::GetViewRelevance(const FSceneView* view)
#endif
{
	FPrimitiveViewRelevance	viewRelevance;

	// The primitive has one or more distortion elements.
	viewRelevance.bDistortionRelevance = true;

	// The primitive is drawn.
	viewRelevance.bDrawRelevance = IsShown(view) && view->Family->EngineShowFlags.Particles;

	// The primitive's dynamic elements are rendered for the view.
	viewRelevance.bDynamicRelevance = true;

	// The primitive is drawn only in the editor and composited onto the scene after post processing using no depth testing.
	//viewRelevance.bEditorNoDepthTestPrimitiveRelevance

	// The primitive is drawn only in the editor and composited onto the scene after post processing.
	//viewRelevance.bEditorPrimitiveRelevance

	// The primitive should have GatherSimpleLights called on the proxy when gathering simple lights.
	viewRelevance.bHasSimpleLights = true;

	// Whether this primitive view relevance has been initialized this frame.
	//viewRelevance.bInitializedThisFrame = true;

	// The primitive has one or more masked elements.
	//viewRelevance.bMaskedRelevance

	// The primitive has one or more elements that have normal translucency.
	viewRelevance.bNormalTranslucencyRelevance = true;

	// The primitive has one or more opaque or masked elements.
	viewRelevance.bOpaqueRelevance = true;

	// The primitive should render to the custom depth pass.
	viewRelevance.bRenderCustomDepth = ShouldRenderCustomDepth();

	// The primitive should render to the main depth pass.
	viewRelevance.bRenderInMainPass = true;

	// The primitive has one or more elements that have SeparateTranslucency.
	viewRelevance.bSeparateTranslucencyRelevance = true;

	// The primitive is casting a shadow.
	viewRelevance.bShadowRelevance = true;// IsShadowCast(view);

	// The primitive's static elements are rendered for the view.
	//viewRelevance.bStaticRelevance

	return viewRelevance;
}

//----------------------------------------------------------------------------

void	FPopcornFXSceneProxy::GetLightRelevance(const FLightSceneProxy * LightSceneProxy, bool & bDynamic, bool & bRelevant, bool & bLightMapped, bool & bShadowMapped) const
{
	bDynamic = true;
	bRelevant = true;
	bLightMapped = false;
	bShadowMapped = false;
}

//----------------------------------------------------------------------------
