#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2012/12/06 12:03 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HH_PARTICLES_KERNEL_CPU_H__
#define __HH_PARTICLES_KERNEL_CPU_H__

#include "hh_particles/include/ps_evolvers.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	CPU kernels
//
//----------------------------------------------------------------------------

class	CParticleUpdater_CPU;
class	CParticleEvaluationContext;
class	CParticleRandomContext;
struct	SParticleImplementationCPU;
class	CParticleStream_MainMemory;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleKernelCPU : public CRefCountedObject
{
public:
};
HH_DECLARE_REFPTRCLASS(ParticleKernelCPU);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleKernelCPU_Evolver : public CParticleKernelCPU
{
protected:
	PCParticleEvolver					m_HboParent;
	const CParticleUpdater_CPU			*m_Updater;

public:
	CParticleKernelCPU_Evolver(const CParticleUpdater_CPU *updater, const PCParticleEvolver &hboParent);

	virtual bool						SetupAfterImplementation(const SParticleImplementationCPU &cpuImplem) = 0;	// TODO: rename
	virtual void						SetupStream(CParticleEvaluationContext &context, CParticleRandomContext &randContext) const {}
	virtual void						Run(CParticleEvaluationContext &context, CParticleRandomContext &randContext, const TStridedMemoryView<const float> &dt) const = 0;
	virtual const char					*ClassName() const = 0;

	const CParticleUpdater_CPU			*ParentUpdater() const { return m_Updater; }
	const CParticleEvolver				*HboParent() const { return m_HboParent.Get(); }
};
HH_DECLARE_REFPTRCLASS(ParticleKernelCPU_Evolver);

//----------------------------------------------------------------------------

typedef bool	CbEvolverRemapperCPU(const CParticleUpdater_CPU *updater, const CParticleEvolver *hboEv, const SParticleDeclaration &decl, TArray<PParticleKernelCPU_Evolver> &outList);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HH_PARTICLES_KERNEL_CPU_H__
