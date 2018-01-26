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

#include <hh_kernel/include/kr_containers_array.h>
#include <hh_kernel/include/kr_containers_array_wrap.h>
#include <hh_particles/include/ps_mediums.h>
#include <hh_particles/include/ps_stream_to_render.h>
#include <hh_particles/include/ps_renderers.h>
#include <hh_kernel/include/kr_log.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

#ifdef HH_ASSERT_AND_LOG
#	error factorize me
#endif

#define	HH_ASSERT_AND_LOG(__condition, ...)		do {			\
		if (!(__condition))										\
		{														\
			CLog::Log(HH_ERROR, __VA_ARGS__);					\
			HH_ASSERT_MESSAGE(__condition, __VA_ARGS__);		\
		}														\
	} while (0)

//----------------------------------------------------------------------------

class	CParticleStorage;
class	CParticleStreamAbstract;
class	CParticleStream_MainMemory;
typedef CParticleStream_MainMemory	CParticleStream;	// To avoid breaking everyone's code...

//----------------------------------------------------------------------------

namespace Drawers
{
	struct	PK_BILLBOARDER_EXPORT SBase_DrawRequest
	{
	public:
		PCParticleRenderer		m_UnsafeRenderer; // unsafe if used on other threads

		// Common PositionStreamId (Setuped by specialized draw requests)
		CGuid					m_PositionStreamId;

		SBase_DrawRequest()
		:	m_ParticleCount(0)
		,	m_StreamToRender(null)
		{ }

		// HH_ONLY_IF_ASSERTS(~SBase_DrawRequest() { /*HH_ASSERT(m_StreamToRender == null);*/ })

		bool					Valid() const { return m_StreamToRender != null && m_PositionStreamId.Valid(); }
		bool					Empty() const { return !Valid() || m_StreamToRender == null || m_ParticleCount == 0; }
		hh_u32					ParticleCount() const { return m_ParticleCount; }

		CStringId									StorageClass() const { HH_ASSERT(!Empty()); return m_StreamToRender->StorageClass(); }
		const CParticleStreamToRender				&StreamToRender() const { HH_ASSERT(m_StreamToRender != null); return *m_StreamToRender; }
		const CParticleStreamToRender_MainMemory	*StreamToRender_MainMemory() const;

	protected:
		bool					SetupStream(const CParticleStreamToRender *streamToRender);
		void					ClearStream();

	private:
		hh_u32					m_ParticleCount;
		union
		{
			const CParticleStreamToRender		*m_StreamToRender;
			hh_u64								_paddptr_m_StreamToRender;
		};
	};

	//----------------------------------------------------------------------------

	template<typename _Policy>
	class	PK_BILLBOARDER_EXPORT TDrawRequests : public _Policy
	{
	public:
		typedef typename _Policy::CMyParticleRenderer		CMyParticleRenderer;
		typedef typename _Policy::CMyDrawRequest			CMyDrawRequest;
	protected:
		using _Policy::_Clear;
		using _Policy::_Clean;
		using _Policy::_ClearAndUnlock;
		using _Policy::_LockAndLoad;

	public:
		TDrawRequests() : m_TotalParticleCount(0) {}
		~TDrawRequests() { HH_ASSERT(m_TotalParticleCount == 0); }

		void			ClearAndUnlock()
		{
			_Policy::_Clear();
			for (hh_u32 dri = 0; dri < m_DrawRequests.Count(); ++dri)
				_Policy::_ClearAndUnlock(m_DrawRequests[dri]);
			m_DrawRequests.Clear();
			m_TotalParticleCount = 0;
			m_StreamToRenderContainer.Clear();
		}

		void			Clear() { ClearAndUnlock(); }
		void			Clean()
		{
			Clear();
			for (hh_u32 dri = 0; dri < m_DrawRequests.Count(); ++dri)
				_Policy::_Clean(m_DrawRequests[dri]);
		}

		bool			Reserve(hh_u32 drawRequestCount) { return m_DrawRequests.Reserve(drawRequestCount); }

		bool			LockAndLoad(const CParticleRenderMedium::SMediumRendererAssoc &assoc)
		{
			if (assoc.m_MediumToRender != null && assoc.m_Renderer != null)
			{
				HH_ASSERT(HBO::Cast<const CMyParticleRenderer>(assoc.m_Renderer.Get()) != null);
				return LockAndLoad(*assoc.m_MediumToRender.Get(), *static_cast<const CMyParticleRenderer*>(assoc.m_Renderer.Get()));
			}
			return false;
		}

		bool			LockAndLoad(const CParticleMedium &medium, const CMyParticleRenderer &rendererDesc)
		{
			const CGuid			dri = m_DrawRequests.PushBack();
			HH_ASSERT(dri.Valid());
			if (!dri.Valid())
				return false;
			const CParticleStreamToRender	*streamToRender = m_StreamToRenderContainer.AcquireStream(*medium.ParticleStorage());
			if (streamToRender == null)
			{
				m_DrawRequests.PopBackAndDiscard();
				return false;
			}
			CMyDrawRequest		&dr = m_DrawRequests[dri];
			if (!_Policy::_LockAndLoad(dr, streamToRender, rendererDesc))
			{
				m_StreamToRenderContainer.ReleaseBack(streamToRender);
				_Policy::_ClearAndUnlock(dr);
				m_DrawRequests.PopBackAndDiscard();
				return false;
			}
			HH_ASSERT(dr.Valid());
			HH_ASSERT(dr.ParticleCount() > 0);
			m_TotalParticleCount += dr.ParticleCount();
			return true;
		}

		bool					Empty() const { return m_DrawRequests.Empty(); }
		hh_u32					Count() const { return m_DrawRequests.Count(); }
		hh_u32					TotalParticleCount() const { return m_TotalParticleCount; }
		CMyDrawRequest			&operator [] (hh_u32 index) { return m_DrawRequests[index]; }
		const CMyDrawRequest	&operator [] (hh_u32 index) const { return m_DrawRequests[index]; }
		TMemoryView<const CMyDrawRequest>	View() const { return m_DrawRequests.View(); }
		TMemoryView<CMyDrawRequest>			ViewForWriting() const { return m_DrawRequests.View(); }

		CMyDrawRequest			&Last() { HH_ASSERT(!Empty()); return m_DrawRequests.Last(); }
		const CMyDrawRequest	&Last() const { HH_ASSERT(!Empty()); return m_DrawRequests.Last(); }

		const CParticleStreamToRenderContainer		&StreamContainer() const { return m_StreamToRenderContainer; }
		CbStreamToRenderCullPage_MainMemory			&CbCullPage_MainMemory() { return m_StreamToRenderContainer.m_CbCullPage_MainMemory; }

	private:
		CParticleStreamToRenderContainer			m_StreamToRenderContainer;
		TWrapArrayStackReuse< TArray<CMyDrawRequest, TArrayAligned16> >		m_DrawRequests;
		hh_u32										m_TotalParticleCount;
	};

	//----------------------------------------------------------------------------

	template<typename _ParticleRenderer, typename _DrawRequest>
	class	PK_BILLBOARDER_EXPORT TDrawRequests_DefaultPolicy
	{
	public:
		typedef _ParticleRenderer		CMyParticleRenderer;
		typedef _DrawRequest			CMyDrawRequest;

	protected:
		void	_Clear()
		{
		}
		void	_Clean(CMyDrawRequest &dr)
		{
			dr.Clean();
		}
		void	_ClearAndUnlock(CMyDrawRequest &dr)
		{
			dr.Clear();
		}
		bool	_LockAndLoad(CMyDrawRequest &dr, const CParticleStreamToRender *streamToRender, const CMyParticleRenderer &rendererDesc)
		{
			return dr.Setup(streamToRender, rendererDesc);
		}
	};

	//----------------------------------------------------------------------------
	// helpers:

	bool			FindParticleStream(CGuid &outStreamId, const char *desc, const CStringId &nameStringId, EBaseTypeID type, bool optional, const CBaseObject *bo, const SParticleStreamDef &streamDef);
	bool			FindParticleStream2(CGuid &outStreamId, bool &isOfType1, const char *desc, const CStringId &nameStringId, EBaseTypeID type1, EBaseTypeID type2, bool optional, const CBaseObject *bo, const SParticleStreamDef &streamDef);

	const char		*BaseTypeToString(const EBaseTypeID typeId);

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
