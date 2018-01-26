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

class	CRibbonBillboarder;
struct	SParticleStreamDef;
class	CParticleStorage;
class	CParticleStreamAbstract;

namespace Drawers
{

	//----------------------------------------------------------------------------
	//
	//	SBillboard_DrawRequest:
	//		correctly cache CParticleRenderer_Ribbon particles fields and properties
	//
	//----------------------------------------------------------------------------

	struct	PK_BILLBOARDER_EXPORT SRibbon_DrawRequest : public SBase_DrawRequest
	{
		CParticleRenderer_Ribbon::EBillboardMode	m_Mode;
		CParticleRenderer_Ribbon::EQuality			m_Quality;

		PCTextureAtlasData	m_Atlas;

		CFloat4			m_DefaultColor; // if !m_ColorStreamId.Valid();
		float			m_DefaultWidth; // if !m_WidthStreamId.Valid();
		float			m_DefaultTextureID; // if !m_TextureIDStreamId.Valid();
		float			m_DefaultTextureU; // if !m_TextureUStreamId.Valid();

		float			m_NormalsBendingFactor;

		bool			m_ColorFloat3 : 1;
		bool			m_NeedSort : 1;
		bool			m_RotateTexture : 1;
		bool			m_FlipU : 1;
		bool			m_FlipV : 1;
		//bool			m_ColorFloat3; // @TODO

		CGuid			m_ColorStreamId;
		CGuid			m_WidthStreamId;
		CGuid			m_AxisStreamId;
		CGuid			m_TextureIDStreamId;
		CGuid			m_TextureUStreamId;
		CGuid			m_AlphaCursorStreamId;

		CGuid			m_SelfIDStreamId;
		CGuid			m_ParentIDStreamId;
		CGuid			m_SpawnerIDStreamId;

		hh_u8			_paddto16[16];

		SRibbon_DrawRequest()
		:	m_Mode(CParticleRenderer_Ribbon::__MaxBillboardModes)
		{
		}

		bool					Setup(const CParticleStreamToRender *streamToRender, const CParticleRenderer_Ribbon &rendererDesc); // return false if stream is empty or setup failed
		void					Clear();

		bool					Valid() const { return SBase_DrawRequest::Valid(); }
		bool					Empty() const { return SBase_DrawRequest::Empty(); }

		const CParticleRenderer_Ribbon		*UnsafeRenderer() const { return static_cast<const CParticleRenderer_Ribbon*>(m_UnsafeRenderer.Get()); }

	private:
		bool					_Setup(const CParticleRenderer_Ribbon &rendererDesc);
	};

	//----------------------------------------------------------------------------
	//
	//	CBillboard_DrawRequests
	//
	//----------------------------------------------------------------------------

	typedef Drawers::TDrawRequests< TDrawRequests_DefaultPolicy<CParticleRenderer_Ribbon, SRibbon_DrawRequest> >		CRibbon_DrawRequests;

	//----------------------------------------------------------------------------
	//
	//	misc helper functions
	//
	//----------------------------------------------------------------------------

	// return a valid Billboarding mode
	CParticleRenderer_Ribbon::EBillboardMode		ResolveRibbonBillboarder(const SParticleStreamDef &streamDef, CGuid &outAxisStreamId, const CParticleRenderer_Ribbon &rendererDesc);

	// must be delete with HH_DELETE
	CRibbonBillboarder								*NewRibbonBillboarder(CParticleRenderer_Ribbon::EBillboardMode mode);

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
