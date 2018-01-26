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

struct	SParticleStreamDef;
class	CParticleStorage;
class	CParticleStreamAbstract;

namespace Drawers
{

	//----------------------------------------------------------------------------
	//
	//	SMesh_DrawRequest:
	//		correctly cache CParticleRenderer_Mesh particles fields and properties
	//
	//----------------------------------------------------------------------------

	// Max mesh descriptor material parameters:
	// (could be DiffuseColor, and 7 others)
	enum { kMeshMaxMaterialParams = 8U };

	//----------------------------------------------------------------------------

	struct SMeshDescriptor_DrawRequest
	{
		struct SParam
		{
			CStringId		m_Name;
			CGuid			m_StreamId;
			EBaseTypeID		m_StreamType;
			SParam() : m_StreamType(BaseType_Void) {}
			void			Clear() { Mem::Reinit(*this); }
		};

		PCParticleRenderer_Mesh_Descriptor	m_UnsafeMeshDescriptor;

		hh_i32			m_SubMeshId;
		CFloat3			m_DiffuseColor;
		CFloat3			m_AmbientColor;

		TStaticCountedArray<SParam, kMeshMaxMaterialParams>	m_MaterialParams; // sorted by name

		SMeshDescriptor_DrawRequest() : m_SubMeshId(-1) { }
		void			Clear() { Mem::Reinit(*this); }
	};

	//----------------------------------------------------------------------------

	struct	PK_BILLBOARDER_EXPORT SMesh_DrawRequest : public SBase_DrawRequest
	{
		CFloat3			m_Scale;
		CFloat3			m_StaticPositionOffset;
		CFloat3			m_StaticOrientationOffset;
		CFloat3			m_StaticRotationAxis;

		bool			m_ScaleFloat3; // else float

		CGuid			m_MeshIdStreamId;
		CGuid			m_ScaleStreamId;
		CGuid			m_EulerRotationStreamId;
		CGuid			m_RotationAxisStreamId;
		CGuid			m_RotationAxisAngleStreamId;
		CGuid			m_PositionOffsetStreamId;
		CGuid			m_ForwardAxisStreamId;
		CGuid			m_UpAxisStreamId;

		TArray<SMeshDescriptor_DrawRequest>	m_MeshDescs;

		SMesh_DrawRequest()
		{ }

		bool					Setup(const CParticleStreamToRender *streamToRender, const CParticleRenderer_Mesh &rendererDesc);
		void					Clear();

		bool					Valid() const { return SBase_DrawRequest::Valid(); }
		bool					Empty() const { return SBase_DrawRequest::Empty(); }

		const CParticleRenderer_Mesh		*UnsafeRenderer() const { return static_cast<const CParticleRenderer_Mesh*>(m_UnsafeRenderer.Get()); }

	private:
		bool					_Setup(const CParticleRenderer_Mesh &rendererDesc);
	};

	//----------------------------------------------------------------------------
	//
	//	CMesh_DrawRequests
	//
	//----------------------------------------------------------------------------

	typedef Drawers::TDrawRequests< TDrawRequests_DefaultPolicy<CParticleRenderer_Mesh, SMesh_DrawRequest> >	CMesh_DrawRequests;

	//----------------------------------------------------------------------------
	//
	// Helpers
	//
	//----------------------------------------------------------------------------

	HH_FORCEINLINE hh_u32			MeshIdToUInt(float meshId, float meshCount)
	{
		//const hh_u32	meshIdU32 = static_cast<hh_i32>(HHAbs(meshId));	// avoid fp->u32 cast (huge perf hit), use fp->i32 cast
		//return meshIdU32 % meshCount;
		const hh_u32	meshIdU32 = PKFloatToIntTrunc(HHClamp(meshId, 0.f, meshCount - 1.f));
		return meshIdU32;
	}

	void	FetchIdsAndCount(const TMemoryView<hh_u32> &outIdsView, const TMemoryView<hh_u32> &outPerIdCountsView, const TMemoryView<const float> &inIdsView);
	void	FetchIdsAndCount(const TMemoryView<hh_u16> &outIdsView, const TMemoryView<hh_u32> &outPerIdCountsView, const TMemoryView<const float> &inIdsView);
	void	FetchIdsAndCount(const TMemoryView<hh_u8> &outIdsView, const TMemoryView<hh_u32> &outPerIdCountsView, const TMemoryView<const float> &inIdsView);

	// return valid found count
	hh_u32	FillMeshMaterialParamStreamIds(	const TMemoryView<CGuid>	&outPerMeshParamStreamId,
											const SMesh_DrawRequest		&dr,
											const CStringId				&paramName);

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
