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
	struct	PK_BILLBOARDER_EXPORT SLight_DrawRequest : public SBase_DrawRequest
	{
		CParticleRenderer_Light::EBillboardMaterial		m_BillboardingMaterial;

		float				m_ConstantRadius;	// if (!=0), will be used instead of the particle radius
		float				m_IntensityMultiplier;
		float				m_RadiusMultiplier;
		float				m_RadiusFactorFromIntensity;
		CFloat2				m_AttenuationSteepnessRange;
		float				m_AttenuationSteepness;
		bool				m_LitTranslucency;
		
		bool				m_ColorFloat3 : 1;

		CGuid				m_SizeStreamId;
		CGuid				m_ColorStreamId;

		SLight_DrawRequest() { }

		void					Clear();
		bool					Setup(const CParticleStreamToRender *streamToRender, const CParticleRenderer_Light &rendererDesc); // return false if stream is empty or setup failed

		bool					Valid() const { return SBase_DrawRequest::Valid(); }
		bool					Empty() const { return SBase_DrawRequest::Empty(); }

		const CParticleRenderer_Light		*UnsafeRenderer() const { return static_cast<const CParticleRenderer_Light*>(m_UnsafeRenderer.Get()); }

	private:
		bool					_Setup(const CParticleRenderer_Light &rendererDesc);
	};

	typedef Drawers::TDrawRequests< TDrawRequests_DefaultPolicy<CParticleRenderer_Light, SLight_DrawRequest> >	CLight_DrawRequests;

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
