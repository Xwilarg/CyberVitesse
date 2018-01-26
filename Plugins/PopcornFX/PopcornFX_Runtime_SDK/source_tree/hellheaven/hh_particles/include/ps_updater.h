#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2012/12/05 02:51 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_UPDATER_H__
#define	__PS_UPDATER_H__

#include <hh_kernel/include/kr_callbacks.h>
#include <hh_kernel/include/kr_string_id.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticleMediumCollection;
class	CParticleUpdateManager;
class	CParticleStorageManager;
class	CParticleStorage;
class	CParticleMedium;
class	CParticleState;
struct	SParticleDeclaration;
class	CParticleEvaluationContext;
class	CParticleRandomContext;

//----------------------------------------------------------------------------
//
//	Particle Updater
//
//	Takes care to update a whole particle storage by running the appropriate particle kernels
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleUpdater : public CNonCopyable
{
protected:
	CParticleUpdateManager	*m_Manager;

	CParticleUpdater(CParticleUpdateManager *manager);
public:
	virtual ~CParticleUpdater() {}

	virtual void			Clear() = 0;
	virtual void			Clean() = 0;

	virtual void			PreUpdate(float dt, CParticleStorage *storage) { }
	virtual void			UpdateLaunchAsync(float dt, CParticleStorage *storage) = 0;

	virtual void			SetupStream(CParticleEvaluationContext &context, CParticleRandomContext &randContext) const = 0;

	CParticleUpdateManager	*Manager() const { return m_Manager; }

	TCallbackChainUnordered<void(CParticleUpdater *, CParticleStorage *)>	m_PreUpdateHook;
	TCallbackChainUnordered<void(CParticleUpdater *, CParticleStorage *)>	m_PostUpdateHook;
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleUpdateManager : public CNonCopyable
{
private:
	CStringId						m_UpdateClass;

public:
	CParticleUpdateManager(const CStringId &updateClass);
	virtual ~CParticleUpdateManager();

	const CStringId					&UpdateClass() const { return m_UpdateClass; }

	virtual void					SetOwner(CParticleMediumCollection *owner) = 0;
	virtual void					Clear() = 0;
	virtual void					Clean() = 0;

	virtual void					Update(float dt) = 0;
	virtual void					PostUpdate() = 0;
	virtual void					UpdateFence() = 0;			// Waits for all the update tasks from all updaters to complete
	virtual bool					UpdatePending() = 0;		// Like UpdateFence(), but non-blocking. returns false if all update tasks have completed, true if there are some left to process.
	virtual CParticleUpdater		*NewUpdater(CParticleMedium *owner, CParticleState *state, const SParticleDeclaration &decl, bool evolveOnDeath) = 0; // covariant
	virtual CParticleStorageManager	*StorageManager() const = 0; // covariant

	static bool						InstallGlobalRemapper(const FastDelegate<void(CParticleUpdateManager *)> &hookRemapper);
	static bool						UninstallGlobalRemapper(const FastDelegate<void(CParticleUpdateManager *)> &hookRemapper);

	void							HookRemappers();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_UPDATER_H__
