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

#ifndef	__PS_EVOLVERS_H__
#define	__PS_EVOLVERS_H__

#include "hh_particles/include/ps_object_remapper.h"
#include <hh_base_object/include/hbo_object.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticleBuildReport;
struct	SParticleDeclaration;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleEvolver), public CBaseObject, public TRemappableObject<CParticleEvolver>
{
private:
	HBO_FIELD(bool,			Active);
	HBO_FIELD(hh_u32,		UpdateRate);
	HBO_FIELD(CString,		CustomName);

	bool					_OnActivityModified();
	bool					_OnUpdateRateModified();
	bool					_OnCustomNameModified();

protected:
	CParticleEvolver(CGuid remappableObjectClassID = CGuid::INVALID);
	virtual bool			VirtualLink() override;

public:
	~CParticleEvolver() {}

	virtual void	SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const { HH_ASSERT_NOT_REACHED_MESSAGE("Unspecialized particle evolver"); };	// we can't have pure virtual functions
	virtual void	SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const { HH_ASSERT_NOT_REACHED_MESSAGE("Unspecialized particle evolver"); };	// we can't have pure virtual functions

	// double check final declaration
	virtual void	CheckParticleDeclaration(const SParticleDeclaration &decl, CParticleBuildReport &outReport) const { }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleEvolver);

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_EVOLVERS_H__


