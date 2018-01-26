#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/01/08 11:34 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include <hh_particles/include/ps_renderers_classes.h>

#include "pk_drawers/include/dr_draw_requests.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CBillboarder;
struct	SParticleStreamDef;
class	CParticleStorage;
class	CParticleStreamAbstract;

namespace Drawers
{
	struct	PK_BILLBOARDER_EXPORT SSound_DrawRequest : public SBase_DrawRequest
	{
		const CParticleRenderer_Sound		*m_Renderer; // unsafe if used on other threads

		CGuid					m_LifeRatioStreamId;
		CGuid					m_InvLifeStreamId;
		CGuid					m_VelocityStreamId;
		CGuid					m_VolumeStreamId;
		CGuid					m_SoundIDStreamId;
		CGuid					m_RadiusStreamId;
		CGuid					m_SelfIDStreamId;

		SSound_DrawRequest()
		:	m_Renderer(null)
		{ }

		bool					Setup(const CParticleStreamToRender *streamToRender, const CParticleRenderer_Sound &rendererDesc); // return false if stream is empty or setup failed
		void					Clear();

		bool					Valid() const { return SBase_DrawRequest::Valid(); }
		bool					Empty() const { return SBase_DrawRequest::Empty(); }

	private:
		bool					_Setup(const CParticleRenderer_Sound &rendererDesc);
	};

	typedef Drawers::TDrawRequests< TDrawRequests_DefaultPolicy<CParticleRenderer_Sound, SSound_DrawRequest> >	CSound_DrawRequests;

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
