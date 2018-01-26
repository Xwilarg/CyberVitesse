#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2011/11/09 11:34 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticleMediumCollection;
class	CParticleRenderMedium;
class	CParticleMedium;
class	CParticleRenderer_Billboard;
class	CParticleRenderer_Ribbon;
class	CParticleRenderer_Mesh;
class	CParticleRenderer_Sound;
class	CParticleRenderer_Light;
class	CParticleDrawer;

namespace Drawers
{

	//----------------------------------------------------------------------------

#define PK_EXEC_X_RENDERER_CLASSES()	\
	X_RENDERER_CLASSES(Billboard)		\
	X_RENDERER_CLASSES(Mesh)			\
	X_RENDERER_CLASSES(Ribbon)			\
	X_RENDERER_CLASSES(Light)			\
	X_RENDERER_CLASSES(Sound)

	enum ERendererClass
	{
		Renderer_Invalid = -1,
#define	X_RENDERER_CLASSES(__name)	Renderer_ ## __name,
		PK_EXEC_X_RENDERER_CLASSES()
#undef	X_RENDERER_CLASSES
		// Renderer_Billboard, Renderer_Ribbon, etc ...
	};

	ERendererClass		GetRendererClass(const CParticleRenderer &renderer);

	//----------------------------------------------------------------------------
	//
	// Helper to walk through all CParticleRenderer and their CParticleMedium
	//
	//----------------------------------------------------------------------------

	class IRenderersWalkerDelegates;

	class CRenderersWalker
	{
	public:
		CRenderersWalker()
		:	m_Enable(0)
		,	m_WalkEmptyOnes(false)
		{ }

		void		SetWalkEmptyOnes(bool walkEmptyOnes) { m_WalkEmptyOnes = walkEmptyOnes; }
		bool		WalkEmptyOnes() const { return m_WalkEmptyOnes; }
		void		Enable(ERendererClass rendererClass) { m_Enable |= (1U << rendererClass); }
		void		Disable(ERendererClass rendererClass) { m_Enable &= ~(1U << rendererClass); }
		bool		Enabled(ERendererClass rendererClass) const { return (m_Enable & (1U << rendererClass)) != 0; }

		void		Walk(const CParticleMediumCollection &collection, IRenderersWalkerDelegates &delegates);

	private:
		hh_u32		m_Enable;
		bool		m_WalkEmptyOnes : 1;
	};

	class IRenderersWalkerDelegates
	{
	public:
		virtual bool		RenderersWalk(const CParticleRenderMedium &renderMedium) { return true; }

		// About to call at most "count" additionnal RenderersWalk (can be used to Reserve memory)
		virtual bool		RenderersWalkReserveAtMost(ERendererClass rendererClass, hh_u32 count) { return true; }

		virtual void		RenderersWalk(const CParticleRenderer_Billboard &renderer, const CParticleMedium &medium, CParticleDrawer &drawer) { RenderersWalk(renderer, medium); }
		virtual void		RenderersWalk(const CParticleRenderer_Ribbon &renderer, const CParticleMedium &medium, CParticleDrawer &drawer) { RenderersWalk(renderer, medium); }
		virtual void		RenderersWalk(const CParticleRenderer_Mesh &renderer, const CParticleMedium &medium, CParticleDrawer &drawer) { RenderersWalk(renderer, medium); }
		virtual void		RenderersWalk(const CParticleRenderer_Sound &renderer, const CParticleMedium &medium, CParticleDrawer &drawer) { RenderersWalk(renderer, medium); }
		virtual void		RenderersWalk(const CParticleRenderer_Light &renderer, const CParticleMedium &medium, CParticleDrawer &drawer) { RenderersWalk(renderer, medium); }

		virtual void		RenderersWalk(const CParticleRenderer_Billboard &renderer, const CParticleMedium &medium) { }
		virtual void		RenderersWalk(const CParticleRenderer_Ribbon &renderer, const CParticleMedium &medium) { }
		virtual void		RenderersWalk(const CParticleRenderer_Mesh &renderer, const CParticleMedium &medium) { }
		virtual void		RenderersWalk(const CParticleRenderer_Sound &renderer, const CParticleMedium &medium) { }
		virtual void		RenderersWalk(const CParticleRenderer_Light &renderer, const CParticleMedium &medium) { }

	};

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
