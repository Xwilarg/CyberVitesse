#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2015/03/30 05:56 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_UPDATER_AUTO_H__
#define	__PS_UPDATER_AUTO_H__

#include "hh_particles/include/ps_config.h"
#include "hh_particles/include/ps_updater.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

//namespace	CPU			{ class	CParticleUpdateManager; }
class	CParticleUpdateManager_CPU;

#if	(HH_PARTICLES_UPDATER_USE_D3D11 != 0)
//namespace	GPU_D3D11	{ class	CParticleUpdateManager; }
class	CParticleUpdateManager_D3D11;
#endif

//----------------------------------------------------------------------------

class	CParticleUpdateManager_Auto : public CParticleUpdateManager
{
public:
	enum	ESimLocation
	{
		SimLocation_Auto = 0,
		SimLocation_CPU,
		SimLocation_GPU,
	};
private:
	ESimLocation						m_PrefferedSimLocation;
	CParticleUpdateManager_CPU			*m_Updater_CPU;
#if	(HH_PARTICLES_UPDATER_USE_D3D11 != 0)
	CParticleUpdateManager_D3D11		*m_Updater_D3D11;
#endif

	bool								_Setup();
	hh_u32								_PreferredSimLocation(const CParticleMedium *owner, const SParticleDeclaration &decl);

protected:
	CParticleUpdateManager_Auto(const CStringId &className);

public:
	~CParticleUpdateManager_Auto();

	virtual void						SetOwner(CParticleMediumCollection *owner) override;
	virtual void						Clear() override;
	virtual void						Clean() override;

	virtual void						Update(float dt) override;
	virtual void						PostUpdate() override;
	virtual void						UpdateFence() override;
	virtual bool						UpdatePending() override;
	virtual CParticleUpdater			*NewUpdater(CParticleMedium *owner, CParticleState *state, const SParticleDeclaration &decl, bool evolveOnDeath) override;
	virtual CParticleStorageManager		*StorageManager() const override;

	static CParticleUpdateManager_Auto	*New();
	static CStringId					DefaultUpdateClass();

	void								SetPrefferedSimLocation(ESimLocation location);
	CParticleUpdateManager_CPU			*GetUpdater_CPU() const { return m_Updater_CPU; }
#if	(HH_PARTICLES_UPDATER_USE_D3D11 != 0)
	CParticleUpdateManager_D3D11		*GetUpdater_D3D11() const { return m_Updater_D3D11; }
#endif
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_UPDATER_AUTO_H__

