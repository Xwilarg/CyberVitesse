//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "DrawCalls_Light.h"

#include "World/PopcornFXSceneProxy.h"
#include "Internal/ParticleScene.h"
#include "Render/MyRHIStuff.h"
#include "Render/RenderManager.h"

#include "PopcornFXSDK.h"
#include <hh_particles/include/Storage/MainMemory/storage_ram.h>

//#if (PK_GPU_D3D11 == 1)
//#	include <hh_particles/include/Storage/D3D11/storage_d3d11.h>
//#endif

//----------------------------------------------------------------------------

void		ParticleDrawCall_Light_BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Light &renderer)
{
	flags.Set(SDrawerFlags::SupportGPUParticles, false);
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

bool		ParticleDrawCall_GatherLights(CParticleRenderManager *renderManager, PopcornFX::CRendererSubView &views, const PopcornFX::Drawers::CLight_DrawRequests &drawRequests)
{
	CParticleScene		*const scene = views.SceneProxy()->ParticleSceneToRender();

	HH_ASSERT(scene != null);
	HH_ASSERT(scene->SceneComponent() != null);

	scene->m_LightPositions.Clear();
	scene->m_LightDatas.Clear();

	if (drawRequests.Empty())
		return true;

	PopcornFX::TArray<FSimpleLightPerViewEntry>		&lightPositions = scene->m_LightPositions;
	PopcornFX::TArray<FSimpleLightEntry>			&lightDatas = scene->m_LightDatas;
	HH_ASSERT(lightPositions.Count() == lightDatas.Count());

	static const float		kMinLightSize = 1e-3f;
	const float				globalScale = views.GlobalScale();

	// Compute the total particle count
	const hh_u32		totalParticleCount = drawRequests.TotalParticleCount();

	if (!lightPositions.Reserve(totalParticleCount) ||
		!lightDatas.Reserve(totalParticleCount))
	{
		HH_ASSERT_NOT_REACHED();
		return false;
	}

	const bool		forceLitTranslucentGeometry = renderManager->m_RenderSettings.bForceLightsLitTranslucent;

	hh_u32		globalOffset = 0;

	for (hh_u32 dri = 0; dri < drawRequests.Count(); ++dri)
	{
		const PopcornFX::Drawers::SLight_DrawRequest	&drawRequest = drawRequests[dri];
		HH_ASSERT(drawRequest.ParticleCount() > 0);

		if (drawRequest.StorageClass() != PopcornFX::CParticleStorageManager_MainMemory::DefaultStorageClass())
		{
			HH_ASSERT_NOT_REACHED();
			continue;
		}

		HH_TODO("renderManager->CullTest");

		const PopcornFX::CParticleStreamToRender_MainMemory	*lockedStream = drawRequest.StreamToRender_MainMemory();
		if (!HH_VERIFY(lockedStream != null)) // Light particles shouldn't handle GPU streams for now
			continue;

		const float		colorFactor = drawRequest.m_IntensityMultiplier;
		const float		sizeFactor = drawRequest.m_RadiusMultiplier * globalScale;
		const float		exponent = drawRequest.m_AttenuationSteepness * drawRequest.m_AttenuationSteepnessRange.y();
		const bool		litTranslucentGeometry = forceLitTranslucentGeometry | drawRequest.m_LitTranslucency;

		const hh_u32	pageCount = lockedStream->PageCount();
		const hh_u32	drStart = globalOffset;

		globalOffset += drawRequest.ParticleCount();
		hh_u32					offset = drStart;
		for (hh_u32 pagei = 0; pagei < pageCount; ++pagei)
		{
			const PopcornFX::CParticlePageToRender_MainMemory		*page = lockedStream->Page(pagei);
			HH_ASSERT(page != null);
			const hh_u32			pcount = page == null ? 0 : page->ParticleCount();
			if (pcount == 0)
				continue;

			TStridedMemoryView<const CFloat3>	positions = page->StreamForReading<CFloat3>(drawRequest.m_PositionStreamId);
			HH_ASSERT(positions.Count() == pcount);
			TStridedMemoryView<const float>		sizes;
			HH_ALIGN(0x10) float				defaultSize = drawRequest.m_ConstantRadius;
			if (drawRequest.m_SizeStreamId.Valid())
				sizes = page->StreamForReading<float>(drawRequest.m_SizeStreamId);
			else
				sizes = TStridedMemoryView<const float>(&defaultSize, pcount, 0);

			// Color Alpha is ignored, use CFloat3
			TStridedMemoryView<const CFloat3>	colors(&CFloat3::ONE, pcount, 0);
			if (drawRequest.m_ColorStreamId.Valid())
			{
				if (drawRequest.m_ColorFloat3)
					colors = page->StreamForReading<CFloat3>(drawRequest.m_ColorStreamId);
				else
					colors = TStridedMemoryView<const CFloat3>::Reinterpret(page->StreamForReading<CFloat4>(drawRequest.m_ColorStreamId));
			}
			HH_ASSERT(!colors.Empty()); // DrawRequest should have checked that !

			for (hh_u32 parti = 0; parti < pcount; ++parti)
			{
				const float					radius = sizes[parti] * sizeFactor;
				if (radius < kMinLightSize)
					continue;
				PopcornFX::CGuid			lposi = lightPositions.PushBack();
				FSimpleLightPerViewEntry	&lightpos = lightPositions[lposi];
				lightpos.Position = ToUE(positions[parti] * globalScale);

				PopcornFX::CGuid			ldatai = lightDatas.PushBack();
				FSimpleLightEntry			&lightdata = lightDatas[ldatai];
				lightdata.Color = ToUE(colors[parti] * colorFactor);
				lightdata.Radius = radius;

				// Set the exponent to 0 if we want to enable inverse squared falloff
				// Note:
				//		- Radius will now represent the falloff's clamped area
				//		- Color will now represent the physical light's lumens
				lightdata.Exponent = exponent;

#if (ENGINE_MINOR_VERSION >= 16)
				// [QUOTE Engine/Private/Particles/ParticleModules.cpp line 3033]
				// Particle lights don't affect volumetric fog by default, since they are likely to be moving fast and volumetric fog relies on a strong temporal filter
				// [/QUOTE]
				lightdata.VolumetricScatteringIntensity = 0.0f;
#endif

				// We could add a boolean in the light renderer
				lightdata.bAffectTranslucency = litTranslucentGeometry;
			}
			offset += pcount;
		}
		HH_ASSERT(offset == drStart + drawRequest.ParticleCount());
	}

	return true;
}

//----------------------------------------------------------------------------
