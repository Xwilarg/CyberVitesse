#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2015/04/19 15:00 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HH_PARTICLES_SAMPLER_CPU_H__
#define __HH_PARTICLES_SAMPLER_CPU_H__

#include "hh_particles/include/ps_samplers.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	CPU samplers
//
//----------------------------------------------------------------------------

class	CParticleUpdater_CPU;
struct	SParticleImplementationCPU;
class	CParticleContextScene;
class	CParticleContextInstance;
class	CParticleRandomContext;
class	CCompilerIRExternals;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleSamplerCPU : public CRefCountedObject
{
protected:
	PCParticleSampler					m_HboParent;
	const CParticleUpdater_CPU			*m_Updater;

	static CParticleSamplerDescriptor	*_GetSamplerDescIFP(const CParticleSampler *hboParent, const CParticleContextScene *pCtxN);

	template<typename _TypeDesc>
	const _TypeDesc						*_ResolveSamplerDescriptor(const CParticleContextScene *pCtxN) const
	{
		CParticleSamplerDescriptor	*desc = _GetSamplerDescIFP(m_HboParent.Get(), pCtxN);
		if (HH_PREDICT_LIKELY(desc != null) && HH_VERIFY(desc->SamplerTypeID() == _TypeDesc::SamplerTypeID()))
			return checked_cast<_TypeDesc*>(desc);
		return checked_cast<const _TypeDesc*>(m_HboParent->GetSamplerDefaultDescriptor().Get());
	}

public:
	CParticleSamplerCPU(const CParticleUpdater_CPU *updater, const PCParticleSampler &hboParent);

	virtual bool						SetupAfterImplementation(const SParticleImplementationCPU &cpuImplem) = 0;	// TODO: rename
	virtual void						BindToEvaluator(CCompilerIRExternals &externals) const = 0;
	virtual const char					*ClassName() const = 0;

	const CParticleSampler				*HboParent() const { return m_HboParent.Get(); }
	const CParticleUpdater_CPU			*ParentUpdater() const { return m_Updater; }
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerCPU);

//----------------------------------------------------------------------------

typedef PParticleSamplerCPU	CbSamplerRemapperCPU(const CParticleUpdater_CPU *updater, const CParticleSampler *hboSamp, const SParticleDeclaration &decl);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HH_PARTICLES_SAMPLER_CPU_H__
