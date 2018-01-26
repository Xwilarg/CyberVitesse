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
#include <hh_particles/include/ps_mediums.h>

class	FPopcornFXVertexFactory;
class	FPrimitiveDrawInterface;
class	FPopcornFXSceneProxy;
class	FSceneView;
class	FSceneViewFamily;
class	FMeshElementCollector;
class	CParticleScene;

FWD_PK_API_BEGIN
//----------------------------------------------------------------------------

// Fast Forwarded through PopcornFX
class	CRendererSubView
{
public:
	enum : uint32 { kMaxViews = 8 };

	enum EPass
	{
		Pass_Unknown = 0,
		UpdatePass_PostUpdate,
		RenderPass_Main,
		RenderPass_Shadow,
	};

	struct SBBView
	{
		CFloat4x4		_m_OriginalViewForCmp;
		CFloat4x4		m_BillboardingMatrix;
		hh_u32			m_ViewsMask;

		void			Setup(const CFloat4x4 &viewMatrix);
	};

	struct SSceneView
	{
		bool				m_ToRender;
		hh_u32				m_BBViewIndex;
		const FSceneView	*m_SceneView;
	};

public:
	CRendererSubView();

	bool			Setup_GetDynamicMeshElements(
		const FPopcornFXSceneProxy *sceneProxy,
		const ::TArray<const FSceneView*> &Views,
		const FSceneViewFamily &ViewFamily,
		uint32 VisibilityMap,
		FMeshElementCollector &collector);

	bool			Setup_PostUpdate();

	float						GlobalScale() const { return m_GlobalScale; }

	bool						IsRenderPass() const { return m_Pass >= RenderPass_Main; }
	bool						IsUpdatePass() const { return m_Pass < RenderPass_Main; }

	EPass						Pass() const { return m_Pass; }
	EPass						UpdatePass() const { HH_ASSERT(IsUpdatePass()); return m_Pass; }
	EPass						RenderPass() const { HH_ASSERT(IsRenderPass()); return m_Pass; }

	const FPopcornFXSceneProxy	*SceneProxy() const { return m_SceneProxy; }
	const FSceneViewFamily		*ViewFamily() const { return m_ViewFamily; }
	FMeshElementCollector		*Collector() const { return m_Collector; }
	bool						HasShadowPass() const { HH_ASSERT(IsRenderPass()); return m_HasShadowPass; }
	const TStaticCountedArray<SSceneView, kMaxViews>	&SceneViews() const { return m_SceneViews; }
	const TStaticCountedArray<SBBView, kMaxViews>		&BBViews() const { return m_BBViews; }

	SBBView						&BBView(hh_u32 i) { return m_BBViews[i]; }

private:
	float						m_GlobalScale;
	const FPopcornFXSceneProxy	*m_SceneProxy;

	const FSceneViewFamily		*m_ViewFamily;
	FMeshElementCollector		*m_Collector;

	hh_u32						m_ViewsMask;

	EPass						m_Pass;
	bool						m_HasShadowPass; // Deduced from last frame, so m_HasShadowPass is one frame late
	bool						_m_Next_HasShadowPass;

	TStaticCountedArray<SSceneView, kMaxViews>		m_SceneViews;
	TStaticCountedArray<SBBView, kMaxViews>			m_BBViews;

};

//----------------------------------------------------------------------------
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

using PopcornFX::CRendererSubView;
