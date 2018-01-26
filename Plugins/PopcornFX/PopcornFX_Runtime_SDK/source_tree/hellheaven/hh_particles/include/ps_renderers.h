#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2004/03/25 10:47 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_RENDERERS_H__
#define	__PS_RENDERERS_H__

#include <hh_base_object/include/hbo_object.h>

#include "hh_particles/include/ps_system.h"
#include "hh_particles/include/ps_object_remapper.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticlePage;
class	CParticleUpdater_CPU;	// for 'CParticleRenderer::BuildRuntimeKernels'
HH_FORWARD_DECLARE(ParticleKernelCPU_Evolver);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleRenderer), public CBaseObject, public TRemappableObject<CParticleRenderer>
{
private:
	HBO_FIELD(bool,					VisibleInEditor);	// dummy field used only for correct display editor-side, don't use outside the editor
	HBO_FIELD(hh_i32,				DrawOrder);
	HBO_FIELD(CString,				UserData);
	HBO_FIELD(CString,				CustomName);

	bool							_OnDrawOrderModified();
	bool							_OnCustomNameModified();
	CBaseObject::EFieldVisibility	_VisibleInEditorVisibility() const;

protected:
	CParticleRenderer(CGuid remappableObjectClassID = CGuid::INVALID);

	virtual void					VirtualUnlink() override;
	virtual bool					VirtualLink() override;

public:
	~CParticleRenderer();

	virtual void					AddToList(TArray<CParticleRenderer*> &renderers, CParticleBuildReport &outReport) { renderers.PushBack(this); }
	virtual void					SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const { HH_ASSERT_NOT_REACHED_MESSAGE("unspecialized CParticleRenderer called..."); }
	virtual void					SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const { HH_ASSERT_NOT_REACHED_MESSAGE("unspecialized CParticleRenderer called..."); };
	virtual bool					BuildRuntimeKernels(const CParticleUpdater_CPU *updater, const SParticleDeclaration &decl, TArray<PParticleKernelCPU_Evolver> &outList) const { return true; }

	// double check final declaration
	virtual void					CheckParticleDeclaration(const SParticleDeclaration &decl, CParticleBuildReport &outReport) const { }
	virtual bool					CompatibleWith(const CParticleRenderer *other) const;

	mutable TCallbackChainUnordered<void(const CParticleRenderer*)>	m_OnDestroy;
	mutable TCallbackChainUnordered<void(const CParticleRenderer*)>	m_OnModified;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleRenderer);

//----------------------------------------------------------------------------
//
//	Particle Drawer
//
//	Takes care to draw particle streams
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleDrawer : public CRefCountedObject
{
public:
	virtual ~CParticleDrawer() {}

	enum	EDebugRenderMode
	{
		DebugRender_Unfocused,
		DebugRender_Selected,
	};

	virtual void	Clear() = 0;
	virtual void	Update() {}
	virtual bool	CanRenderInView(CRendererSubView &view) const = 0;

	// called on all drawers before the real render calls are made
	virtual void	PreRender(CRendererSubView &view) {}
	// called on all drawers with the list of mediums/renderers to draw
	virtual void	StartRender(CRendererSubView &view, const CFloat3 &geometricMean, const TMemoryView<const CParticleRenderMedium::SMediumRendererAssoc> &drawRequests) = 0;
	// called on all drawers after the main render pass has been made. you can wait for VB-fill tasks to complete, call unlocks on the VBs here, and do the actual draw-call.
	virtual void	KickRender(CRendererSubView &view) = 0;

	// only used by the popcorn editor to draw a small subset of particles with debug visualization.
	virtual void	EditorRender(CRendererSubView &view, const CParticlePage *page, const TMemoryView<const hh_u32> &sparseIndices, EDebugRenderMode mode, const CParticleRenderer *renderer) {}

	// maxVisibleRadius = max(realRadius * maxScale, maxConstant)
	virtual void	HarvestParticleRadiusVisibilityCoefficients(const CParticleRenderer *rendererDesc, float &outMaxScale, float &outMaxConstant) const = 0;
	virtual bool	AreRenderersCompatible(const CParticleRenderer *rendererA, const CParticleRenderer *rendererB) = 0;
	virtual void	OnParentDescriptorChanged(const CParticleRenderer *parent) = 0;	// in practise, only called by the popcorn editor or when changing renderer's properties in-code

	// must be a combination of (1 << CParticleDescriptor::StorageLocation_CPU) and (1 << CParticleDescriptor::StorageLocation_GPU)
	// any bit set in 'outPrefferedLocations' must also be set in 'outSupportedLocations'
	// returning false means the drawer has no preference (runtime won't take drawer location preference into account when
	// deciding where to run the sim, and assume the drawer will deal with it)
	// The default implem below says it only supports CPU, to avoid breaking any existing integration.
	// If you explicitely want your drawers to support GPU particle buffers, override this method.
	virtual bool	GetStoragePrefs(hh_u32 &outSupportedLocations, hh_u32 &outPrefferedLocations) { outSupportedLocations = (1 << CParticleDescriptor::StorageLocation_CPU); outPrefferedLocations = outSupportedLocations; return true; }

	TCallbackChainUnordered<void(CParticleUpdater *, CParticleStorage *)>	m_PreRenderHook;
	TCallbackChainUnordered<void(CParticleUpdater *, CParticleStorage *)>	m_PostRenderHook;
};
HH_DECLARE_REFPTRCLASS(ParticleDrawer);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleRenderManager : public CNonCopyable
{
private:
	CString						m_RenderClass;
public:
	CParticleRenderManager(const char *renderClass);
	virtual ~CParticleRenderManager();

	const CString				&RenderClass() const { return m_RenderClass; }

	virtual void				Update(float dt) = 0;
	virtual CParticleDrawer		*NewParticleDrawer(CParticleRenderMedium *owner) = 0;

	static bool					InstallGlobalRemapper(const FastDelegate<void(CParticleRenderManager *)> &hookRemapper);
	static bool					UninstallGlobalRemapper(const FastDelegate<void(CParticleRenderManager *)> &hookRemapper);

	void						HookRemappers();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_RENDERERS_H__


