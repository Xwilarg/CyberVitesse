#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2013/08/23 10:20 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_ACTIONS_H__
#define	__PS_ACTIONS_H__

#include <hh_engine_utils/include/kr_actions.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ActionInstanceParticle);
class	CParticleMediumCollection;

//----------------------------------------------------------------------------

struct	SSpawnTransformsPack
{
	// All matrices below MUST BE ORTHONORMAL (no scale, and the 3 axes perpendicular to each other)
	const CFloat4x4		*m_WorldTr_Current;			// current frame transforms of the effect
	const CFloat4x4		*m_WorldTr_Previous;		// previous frame transforms of the effect
	const CFloat3		*m_WorldVel_Current;		// current frame velocity of the effect
	const CFloat3		*m_WorldVel_Previous;		// previous frame velocity of the effect

	// These are for static effects. No need to fill this if you provide the float4x4 transforms above
	CFloat3				m_WorldPosition_Fixed;
	CQuaternion			m_WorldOrientation_Fixed;

	SSpawnTransformsPack()
	:	m_WorldTr_Current(null)
	,	m_WorldTr_Previous(null)
	,	m_WorldVel_Current(null)
	,	m_WorldVel_Previous(null)
	,	m_WorldPosition_Fixed(0.0f)
	,	m_WorldOrientation_Fixed(CQuaternion::IDENTITY)
	{ }
};
HH_REGISTER_RTTI(SSpawnTransformsPack);

//----------------------------------------------------------------------------
//
//	generic particle-system action, from which all particle-triggered
//	actions are derived. Extends the base action classes
//	by providing batched-spawns
//
//----------------------------------------------------------------------------

class	HH_ENGINE_EXPORT HBO_CLASS(CActionFactoryParticle), public CActionFactory
{
private:
protected:
	virtual PActionInstance				VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const override;

public:
	CActionFactoryParticle();
	virtual ~CActionFactoryParticle();

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactoryParticle);

//----------------------------------------------------------------------------

class	CActionInstanceParticle : public CActionInstance
{
protected:
	CActionInstanceParticle(const CActionFactoryParticle *parentFactory, const PActionParameters &parameters);

	CParticleMediumCollection	*m_MediumCollection;

public:
	virtual ~CActionInstanceParticle();
	virtual void				Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) override;
	virtual bool				Setup(Threads::SThreadContext *threadContext, const PActionParameters &parameters);

	HH_ACTIONS_DECLARE_INSTANCE(Particle);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_SCENE_ACTIONS_H__

