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

#ifndef	__PS_SAMPLERS_H__
#define	__PS_SAMPLERS_H__

#include "hh_particles/include/ps_object_remapper.h"

#include <hh_kernel/include/kr_string_id.h>
#include <hh_base_object/include/hbo_object.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CCompilerContext;
class	CCompilerASTBuilder;
class	CParticleEvaluator;
class	CParticleBuildReport;
struct	SParticleDeclaration;
struct	SParticleEvaluationBuffer;

//----------------------------------------------------------------------------
//
//	Particle samplers
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleSamplerDescriptor : public CRefCountedObject
{
private:
	const hh_u32	m_SamplerTypeID;
protected:
	CParticleSamplerDescriptor(hh_u32 samplerTypeId) : m_SamplerTypeID(samplerTypeId) {}
	static hh_u32	_GetNextSamplerTypeID();
public:
	hh_u32			SamplerTypeID() const { return m_SamplerTypeID; }
};
HH_DECLARE_REFPTRCLASS(ParticleSamplerDescriptor);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSampler), public CBaseObject, public TRemappableObject<CParticleSampler>
{
private:
	HBO_FIELD(CString,					SamplerName);
	HBO_FIELD(CString,					SamplerDescription);
	HBO_FIELD(CString,					UserData);
	HBO_FIELD(bool,						AttributeSamplerAtEvolve);

	bool								_OnSamplerNameModified();
	bool								_OnAttributeSamplerAtEvolveModified();
protected:
	virtual bool						VirtualLink() override;

	CStringId							m_SamplerNameGUID;
	PParticleSamplerDescriptor			m_DefaultDescriptor;
	CGuid								m_AttributeSamplerSlot;

protected:
	CParticleSampler(CGuid remappableObjectClassID = CGuid::INVALID);

public:
	~CParticleSampler();

	CStringId							SamplerNameGUID() const { return m_SamplerNameGUID; }

	virtual void						SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const {}
	virtual void						DefineType(CCompilerContext &context) const {}
	virtual void						DeclareExternalObject(CCompilerASTBuilder &builder) const {}

	const PParticleSamplerDescriptor	&GetSamplerDefaultDescriptor() const { return m_DefaultDescriptor; }
	CGuid								AttributeSamplerSlot() const { return m_AttributeSamplerSlot; }
	void								SetAttributeSamplerSlot(CGuid slot) { m_AttributeSamplerSlot = slot; }

	static bool							IsSamplerNameValid(const CString &name);
	bool								CanChangeAtRuntime() const;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSampler);

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_SAMPLERS_H__


