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

#include "Render/DrawCall.h"

#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------

struct FPopcornFXRenderSettings;

//----------------------------------------------------------------------------

struct SDrawCallSliceHelper
{
	const void		*m_Indices;
	hh_u32			m_IndexCount;
	hh_u32			m_PrimitiveSize;
	bool			m_LargeIndices;

	hh_u32			m_VPP4_ParticleCount;

	CFloat3			m_ViewAxis;
	CFloat3			m_ViewPos;

	hh_u32			m_TotalVertexCount;

	TMemoryView< const TStridedMemoryView<const CFloat3> >	m_PositionCollection;

	const FPopcornFXRenderSettings		*m_RenderSettings;

	PopcornFX::TArray<SDrawCallSlice>	*m_OutDrawCallSlices = null;

	float					m_OutMaxSliceTreeDepth;

	bool					GenSlices();

	HH_NOINLINE float		GetDepth(hh_u32 _index) const
	{
		//const hh_u32	_index = primIndex * 3;
		HH_ASSERT(_index < m_IndexCount);
		//const hh_u32	index = (m_IndexCount - _index) - 1; // Reverse "Back to Front" to "Front to Back"
		const hh_u32	index = _index;
		hh_u32			vertexi;
		if (m_LargeIndices)
			vertexi = reinterpret_cast<const hh_u32*>(m_Indices)[index];
		else
			vertexi = reinterpret_cast<const hh_u16*>(m_Indices)[index];
		HH_ASSERT(vertexi < m_TotalVertexCount);
		const CFloat3		&pos = FindPositionFromVertexi(vertexi);
		//const float			depth = m_ViewAxis.Dot(pos - m_ViewPos);
		const float			depth = m_ViewAxis.Dot(pos - m_ViewPos);
		return depth;
	}

	const CFloat3			&FindPositionFromVertexi(hh_u32 vertexi) const
	{
		hh_u32			particlei;
		if (vertexi < m_VPP4_ParticleCount * 4)
			particlei = vertexi / 4;
		else
			particlei = m_VPP4_ParticleCount + (vertexi - (m_VPP4_ParticleCount * 4)) / 6;
		return FindPosition(particlei);
	}

	const CFloat3			&FindPosition(hh_u32 particlei) const
	{
		for (hh_u32 colli = 0; colli < m_PositionCollection.Count(); ++colli)
		{
			const TStridedMemoryView<const CFloat3>		&positions = m_PositionCollection[colli];
			if (particlei < positions.Count())
				return positions[particlei];
			HH_ASSERT(particlei >= positions.Count());
			particlei -= positions.Count();
		}
		HH_ASSERT_NOT_REACHED();
		return CFloat3::ZERO;
	}


};

//----------------------------------------------------------------------------
