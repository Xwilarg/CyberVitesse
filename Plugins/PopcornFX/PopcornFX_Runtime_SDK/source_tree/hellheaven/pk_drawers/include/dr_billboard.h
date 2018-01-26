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

	//----------------------------------------------------------------------------
	//
	//	SBillboard_DrawRequest:
	//		correctly cache CParticleRenderer_Billboard particles fields and properties
	//
	//----------------------------------------------------------------------------

	struct	PK_BILLBOARDER_EXPORT SBillboard_DrawRequest : public SBase_DrawRequest
	{
		CParticleRenderer_Billboard::EBillboardMode		m_Mode;

		PCTextureAtlasData		m_Atlas;

		float			m_ConstantRadius;
		float			m_AspectRatio;
		float			m_NormalsBendingFactor;
		float			m_AxisScale;

		bool			m_ColorFloat3 : 1;
		bool			m_SizeFloat2 : 1;
		bool			m_NeedSort : 1;
		bool			m_VFlipUVs : 1;
		bool			m_SoftAnimationBlending : 1;

		CGuid			m_SizeStreamId;
		CGuid			m_ColorStreamId;
		CGuid			m_TextureIDStreamId;
		CGuid			m_Axis0StreamId;
		CGuid			m_Axis1StreamId;
		CGuid			m_RotationStreamId;
		CGuid			m_AlphaCursorStreamId;

		SBillboard_DrawRequest()
		:	m_Mode(CParticleRenderer_Billboard::__MaxBillboardModes)
		{ }

		void					Clear();
		bool					Setup(const CParticleStreamToRender *streamToRender, const CParticleRenderer_Billboard &rendererDesc); // return false if stream is empty or setup failed

		bool					Valid() const { return SBase_DrawRequest::Valid(); }
		bool					Empty() const { return SBase_DrawRequest::Empty(); }

		const CParticleRenderer_Billboard		*UnsafeRenderer() const { return static_cast<const CParticleRenderer_Billboard*>(m_UnsafeRenderer.Get()); }

		// returns true if the billboarder should use the m_RotationStreamId if it exists
		bool					CouldUseRotation() const
		{
			HH_ASSERT(Valid());
			return m_Mode == CParticleRenderer_Billboard::ScreenAlignedQuad ||
				m_Mode == CParticleRenderer_Billboard::ViewposAlignedQuad ||
				m_Mode == CParticleRenderer_Billboard::PlanarAlignedQuad;
		}
		// returns true if the billboarder should use m_AxisScale
		bool					NeedAxisScale() const
		{
			HH_ASSERT(Valid());
			return m_Mode == CParticleRenderer_Billboard::VelocityAxisAligned ||
				m_Mode == CParticleRenderer_Billboard::VelocityCapsuleAlign ||
				m_Mode == CParticleRenderer_Billboard::VelocitySpheroidalAlign ||
				m_Mode == CParticleRenderer_Billboard::PlanarAlignedQuad;
		}
		// returns true if the billboarder should use m_AspectRatio
		bool					NeedAspectRatio() const
		{
			HH_ASSERT(Valid());
			return m_Mode == CParticleRenderer_Billboard::ScreenAlignedQuad;
		}

	private:
		bool					_Setup(const CParticleRenderer_Billboard &rendererDesc);
	};

	//----------------------------------------------------------------------------
	//
	//	CBillboard_DrawRequests
	//
	//----------------------------------------------------------------------------

	typedef Drawers::TDrawRequests< TDrawRequests_DefaultPolicy<CParticleRenderer_Billboard, SBillboard_DrawRequest> >	CBillboard_DrawRequests;

	//----------------------------------------------------------------------------
	//
	//	misc helper functions
	//
	//----------------------------------------------------------------------------

	// return a valid Billboarding mode with compatible outAxisField0 and outAxisField1 (!Valid() if not used)
	CParticleRenderer_Billboard::EBillboardMode		ResolveBillboarder(const SParticleStreamDef &streamDef, CGuid &outAxis0StreamId, CGuid &outAxis1StreamId, const CParticleRenderer_Billboard &rendererDesc);
	// HH_NEW'ed billboarder
	CBillboarder									*NewBillboardBillboarder(CParticleRenderer_Billboard::EBillboardMode mode);

	//----------------------------------------------------------------------------
	//
	//	"Standard" Billboarders Helpers
	//
	//----------------------------------------------------------------------------

	// VPP = Vertices Per Particles, the number of vertices generated per particles by the standart CBillboarder
	// IPP = Indices Per Particles
	enum EBillboardBBModeMask
	{
		BillboardBBModeMask_VPP0 = // DEPRECATED ones
			(1U << PopcornFX::CParticleRenderer_Billboard::ScreenPoint) |
			(1U << PopcornFX::CParticleRenderer_Billboard::ScreenAlignedTriangle),
		BillboardBBModeMask_VPP4 =
			(1U << PopcornFX::CParticleRenderer_Billboard::ScreenAlignedQuad) |
			(1U << PopcornFX::CParticleRenderer_Billboard::ViewposAlignedQuad) |
			(1U << PopcornFX::CParticleRenderer_Billboard::VelocityAxisAligned) |
			(1U << PopcornFX::CParticleRenderer_Billboard::VelocitySpheroidalAlign) |
			(1U << PopcornFX::CParticleRenderer_Billboard::PlanarAlignedQuad),
		BillboardBBModeMask_VPP6 =
			(1U << PopcornFX::CParticleRenderer_Billboard::VelocityCapsuleAlign)
	};

	HH_STATIC_ASSERT(
		(BillboardBBModeMask_VPP0 | BillboardBBModeMask_VPP4 | BillboardBBModeMask_VPP6) ==
			((1U << PopcornFX::CParticleRenderer_Billboard::__MaxBillboardModes) - 1)
	);

	inline bool				BillboardBBModeMaskIsUniqueVPP(hh_u32 bbModeMask)
	{
		HH_ASSERT(bbModeMask != 0);
		return (
			((bbModeMask & ~BillboardBBModeMask_VPP4) == 0) |
			((bbModeMask & ~BillboardBBModeMask_VPP0) == 0) |
			((bbModeMask & ~BillboardBBModeMask_VPP6) == 0)
		);
	}

	inline hh_u32			BBModeVPP(PopcornFX::CParticleRenderer_Billboard::EBillboardMode mode)
	{
		const hh_u32		m = (1U << mode);
		if (m & BillboardBBModeMask_VPP0)
			return 0;
		else if (m & BillboardBBModeMask_VPP4)
			return 4;
		else if (m & BillboardBBModeMask_VPP6)
			return 6;
		HH_ASSERT_NOT_REACHED();
		return 0;
	}

	inline hh_u32			BBModeIPP(PopcornFX::CParticleRenderer_Billboard::EBillboardMode mode)
	{
		const hh_u32		m = (1U << mode);
		if (m & BillboardBBModeMask_VPP0)
			return 0;
		else if (m & BillboardBBModeMask_VPP4)
			return 6;
		else if (m & BillboardBBModeMask_VPP6)
			return 12;
		HH_ASSERT_NOT_REACHED();
		return 0;
	}

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
