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
#include <pk_drawers/include/dr_light.h>

//----------------------------------------------------------------------------

class CParticleScene;
class CParticleRenderManager;
struct SDrawerFlags;

//----------------------------------------------------------------------------

void		ParticleDrawCall_Light_BuildDrawerFlags(SDrawerFlags &flags, CParticleRenderManager *renderManager, const PopcornFX::CParticleRenderer_Light &renderer);

//----------------------------------------------------------------------------

bool		ParticleDrawCall_GatherLights(CParticleRenderManager *renderManager, PopcornFX::CRendererSubView &views, const PopcornFX::Drawers::CLight_DrawRequests &drawRequests);

//----------------------------------------------------------------------------
