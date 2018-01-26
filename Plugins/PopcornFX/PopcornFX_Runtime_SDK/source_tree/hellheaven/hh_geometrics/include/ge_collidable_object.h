#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2009/09/14 8:56 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__GE_COLLIDABLE_OBJECT_H__
#define	__GE_COLLIDABLE_OBJECT_H__

#include "hh_geometrics/include/ge_colliders.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Colliders
{
	struct	STraceFilter
	{
		hh_u32	m_FilterFlags;

		STraceFilter(hh_u32 flags = 0) : m_FilterFlags(flags) {}
	};

	//----------------------------------------------------------------------------

	enum	ECombineMode
	{
		Combine_Average = 0,
		Combine_Multiply,
		Combine_Min,
		Combine_Max,

		__MaxCombineModes
	};

	//----------------------------------------------------------------------------

	struct	SSurfaceProperties
	{
		float			m_Restitution;
		float			m_StaticFriction;
		float			m_DynamicFriction;	// NOTE: must be <= m_StaticFriction
		CGuid			m_SurfaceType;
		ECombineMode	m_RestitutionCombineMode;
		ECombineMode	m_FrictionCombineMode;

		SSurfaceProperties()
		:	m_Restitution(1)
		,	m_StaticFriction(0)
		,	m_DynamicFriction(0)
		,	m_SurfaceType(CGuid::INVALID)
		,	m_RestitutionCombineMode(Combine_Multiply)
		,	m_FrictionCombineMode(Combine_Average)
		{
		}
	};
	HH_STATIC_ASSERT(sizeof(SSurfaceProperties) == 0x18);

}	// namespace Colliders

//----------------------------------------------------------------------------

namespace	CollidableObject
{

	struct	SDynamicState
	{
		CFloat4x4				m_GlobalTransforms;
		CFloat3					m_LinearVelocity;	// undefined if Static()
		CFloat3					m_AngularVelocity;	// undefined if Static()
		CFloat3					m_CenterOfMass;		// undefined if Static()
		float					m_InvMass;
		CFloat3x3				m_InvInertiaTensor;	// undefined if Static(), A value of 0 in an element is interpreted as infinite inertia along that axis.

		SDynamicState()
		:	m_GlobalTransforms(CFloat4x4::IDENTITY)
		,	m_LinearVelocity(0)
		,	m_AngularVelocity(0)
		,	m_CenterOfMass(0)
		,	m_InvMass(0)
		,	m_InvInertiaTensor(CFloat3x3::IDENTITY)
		{
		}

		bool					Static() const { return m_InvMass == 0; }
		void					Clear()
		{
			m_GlobalTransforms = CFloat4x4::IDENTITY;
			m_LinearVelocity = CFloat3::ZERO;
			m_AngularVelocity = CFloat3::ZERO;
			m_CenterOfMass = CFloat3::ZERO;
			m_InvMass = 0;
			m_InvInertiaTensor = CFloat3x3::IDENTITY;
		}

		const CFloat3			&LinearVelocity() const { HH_ASSERT(!Static()); return m_LinearVelocity; }
		const CFloat3			&AngularVelocity() const { HH_ASSERT(!Static()); return m_AngularVelocity; }
		const CFloat3			&CenterOfMass() const { HH_ASSERT(!Static()); return m_CenterOfMass; }
		const CFloat3x3			&InvInertiaTensor() const { HH_ASSERT(!Static()); return m_InvInertiaTensor; }
		float					InvMass() const { return m_InvMass; }
	};

	extern void					*DEFAULT;	// default object, used for convenience / default surface parameters.

}	// namespace CollidableObject

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __GE_COLLIDABLE_OBJECT_H__

