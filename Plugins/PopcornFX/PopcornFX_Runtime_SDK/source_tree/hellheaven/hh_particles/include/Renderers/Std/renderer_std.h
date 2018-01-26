#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2013/02/12 07:49 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_RENDERER_STD_H__
#define	__PS_RENDERER_STD_H__

#include "hh_particles/include/ps_renderers.h"
#include "hh_particles/include/ps_object_remapper.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticleRenderManager_Std;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleDrawer_Std : public CParticleDrawer
{
private:
public:
	virtual void	OnParentDescriptorChanged(const CParticleRenderer *parent) override {}	// in practise, only called by the popcorn editor or when changing renderer's properties in-code
};

//----------------------------------------------------------------------------

typedef CParticleDrawer_Std	*CbDrawerRemapperStd(CParticleRenderMedium *rMedium, CParticleRenderManager_Std *manager);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleRenderManager_Std : public CParticleRenderManager
{
public:
	typedef	TObjectMapping<TRemappableObject<CParticleRenderer>, CbDrawerRemapperStd>	TDrawerMapping;

private:
	TDrawerMapping					*m_DrawerMapping;

	bool							_Setup();

private:
	CParticleRenderManager_Std(const char *className);

public:
	virtual ~CParticleRenderManager_Std();

	static CParticleRenderManager_Std	*New(const char *className = null);

	const TDrawerMapping				*DrawerMapping() const { return m_DrawerMapping; }
	TDrawerMapping						*DrawerMapping() { return m_DrawerMapping; }	// FIXME: ideally, don't expose this

	virtual void						Update(float dt) override;
	virtual CParticleDrawer_Std			*NewParticleDrawer(CParticleRenderMedium *owner) override;	// covariant
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_RENDERER_STD_H__

