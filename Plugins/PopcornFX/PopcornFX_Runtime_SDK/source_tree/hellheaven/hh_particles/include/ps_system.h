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

#ifndef	__PS_SYSTEM_H__
#define	__PS_SYSTEM_H__

#include "hh_particles/include/ps_declaration.h"
#include "hh_particles/include/ps_descriptor.h"
#include "hh_particles/include/ps_mediums.h"
#include "hh_particles/include/ps_scene.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SParticleStreamField;

//----------------------------------------------------------------------------
// v1.8 : this might be moved around

class	HH_PARTICLES_EXPORT CParticleManager
{
public:
	static bool											AddPhysicsSurfaceType(const CString &name, hh_u32 key);
	static bool											RemovePhysicsSurfaceType(const CString &name);
	static void											ClearPhysicsSurfaceTypes();
	static TMemoryView<const TPair<CString, hh_u32> >	PhysicsSurfaceTypes();

	static bool											AddPhysicsCollisionFilter(const CString &name, hh_u32 key);
	static bool											RemovePhysicsCollisionFilter(const CString &name);
	static void											ClearPhysicsCollisionFilters();
	static TMemoryView<const TPair<CString, hh_u32> >	PhysicsCollisionFilters();
};

//----------------------------------------------------------------------------

struct	SParticleSpawnerFields
{
	TStridedMemoryView<const float>			m_SpawnDtToEnds;			// must always be set
	TStridedMemoryView<const CFloat4>		m_SpawnTranslations;		// in worldspace
	TStridedMemoryView<const CQuaternion>	m_SpawnOrientations;		// in worldspace
	TStridedMemoryView<const CFloat3>		m_SpawnVelocities;			// in worldspace
	TStridedMemoryView<const hh_u32>		m_SpawnerIds;
	TStridedMemoryView<const hh_u32>		m_ParentIds;

	TStridedMemoryView<const float>			m_SpawnerLifeRatios;		// must always be set

	TStridedMemoryView<const float>			m_SpawnerAges;				// CAN BE EMPTY
	TStridedMemoryView<const float>			m_SpawnerLives;				// CAN BE EMPTY
	TStridedMemoryView<const float>			m_SpawnerSpawnRates;		// CAN BE EMPTY
	TStridedMemoryView<const float>			m_SpawnerSpawnCounts;		// CAN BE EMPTY
	TStridedMemoryView<const hh_u32>		m_SpawnerEmmitedCounts;		// CAN BE EMPTY
	const SAttributesContainer				*m_Attributes;

	TStridedMemoryView<const float>			m_SpawnerBaseSpawnRates;
	TStridedMemoryView<const float>			m_SpawnerBaseSpawnCounts;
	TStridedMemoryView<const float>			m_SpawnerFirstSpawnDelays;
	TStridedMemoryView<float>				m_SpawnerFluxes;			// layer scripts will read/write this
//	float									m_SpawnerInheritVel;
	hh_u32									m_ParentFieldsStartID;		// temp gore hack
	TMemoryView<const SParticleStreamField>	m_ParentFields;				// CAN BE EMPTY

	SParticleSpawnerFields(const SAttributesContainer *attributes)
	:	m_Attributes(attributes)
//	,	m_SpawnerInheritVel(0)
	,	m_ParentFieldsStartID(0)
	{
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_SYSTEM_H__

