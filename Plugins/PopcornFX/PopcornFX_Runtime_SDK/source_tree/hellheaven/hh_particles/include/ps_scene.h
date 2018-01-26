#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2012/12/05 01:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_SCENE_H__
#define	__PS_SCENE_H__

#include <hh_geometrics/include/ge_collidable_object.h>
#include <hh_kernel/include/kr_string_id.h>
// PActionParameters cannot be forward declared
#include <hh_engine_utils/include/kr_actions_parameters.h>

#include "hh_particles/include/ps_config.h"
#include "hh_particles/include/ps_declaration.h"	// for SParticleDeclaration::SEvent

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticleMediumCollection;
class	CParticlePageView;
class	CActionFactoryDecalSpawner;
class	CActionFactory;
class	CActionExecutor;
class	CParticleSamplerCPU_Shape;
class	CParticleEvaluationContext;
namespace	Threads
{
	struct	SThreadContext;
}

//----------------------------------------------------------------------------
//
//	scene-unique objects
//	(typically only one instance per scene)
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT IParticleScene
{
public:
	virtual ~IParticleScene() {}

	//----------------------------------------------------------------------------
	//
	//	Collision interface
	//
	//----------------------------------------------------------------------------

	virtual void	RayTracePacket(	const Colliders::STraceFilter	&traceFilter,
									const Colliders::SRayPacket		&packet,
									const Colliders::STracePacket	&results);

	virtual void	RayTracePacketTemporal(	const Colliders::STraceFilter	&traceFilter,
											const Colliders::SRayPacket		&packet,
											const Colliders::STracePacket	&results);	// by default, calls RayTracePacket()

	// allows you to expose custom parameters based on what the collidable object is.
	// ie : you want effects instantiated due to a scene collision to "stick" to dynamic objects, using a localspace evolver.
	// the action parameter will be reused between calls within the same batch.
	// only the ref to the 'parameters' refptr is const, you can modify/add parameters to these if you want.
	virtual void	InstantiatePostCollideAction(	void					*contactObject,
													void					*contactSurface,
													const CActionFactory	*collideTrigger,
													CActionExecutor			*whereToRun,
													const PActionParameters	&parameters,
													Threads::SThreadContext	&threadCtx,
													float					timeSinceEndOfFrame,
													float					hitFractionInFrame,
													bool					isSticky);	// FIXME: these might break down when using sub-evolves.

#if	(HH_PARTICLES_HAS_OVERLAP_SCENE != 0)
	virtual void	OverlapScene(	const TStridedMemoryView<const CFloat3>	&start,
									const TStridedMemoryView<const CFloat3>	&end,
									const TStridedMemoryView<const float>	&radius,
									const TStridedMemoryView<hh_u32>		&output);
#endif

	// called by physics/collision evolver
	virtual void	ResolveContactMaterials(const TMemoryView<void * const>					&contactObjects,
											const TMemoryView<void * const>					&contactSurfaces,
											const TMemoryView<Colliders::SSurfaceProperties>&outSurfaceProperties) const;

	// called by physics/collision evolver
	virtual void	GetDynamicStates(	const TMemoryView<void * const>						&contactObjects,
										const TMemoryView<CollidableObject::SDynamicState>	&outStates) const;

	// only used by two-way collisions
	virtual void	AddGlobalImpulses(	const TMemoryView<void * const>			&contactObjects,
										const TStridedMemoryView<const CFloat3>	&globalImpulses,
										const TStridedMemoryView<const CFloat3>	&globalImpactLocations);

	// Optional, you don't have to override this one (see the .chm SDK documentation)

	struct	SCollisionDescriptor
	{
		TStridedMemoryView<const float>		m_Dts;					// R
		TStridedMemoryView<const float>		m_Masses;				// R
		TStridedMemoryView<const float>		m_BounceRestitutions;	// R
		TStridedMemoryView<const float>		m_ContactFrictions;		// R
		TStridedMemoryView<const float>		m_SweepRadii;			// R
		TStridedMemoryView<hh_i32>			m_CollisionCounts;		// R/W
		TStridedMemoryView<CFloat4>			m_Positions;			// R/W
		TStridedMemoryView<CFloat4>			m_Velocities;			// R/W
		TStridedMemoryView<const CFloat4>	m_PrevPositions;		// R

		bool								m_DieOnContact;
		bool								m_WeightBounceWithNdotV;
		bool								m_IgnoreSurfaceProperties;
		float								m_BounceRestitution;
		float								m_BounceOffset;
		hh_u32								m_ContactFrictionModel;
		float								m_ContactFriction;
		hh_u32								m_BouncesBeforeDeath;
		hh_u32								m_MaxIterations;
		bool								m_StopIfFinalIterationHits;
		hh_u32								m_WorldInteractionMode;
		hh_u32								m_RestitutionCombineMode;
		hh_u32								m_FrictionCombineMode;
		float								m_DefaultMass;
		float								m_DefaultSweepRadiusFactor;
		hh_u32								m_FilterID;
		bool								m_SphereSweep;
		bool								m_TemporalTracing;
		const CParticleSamplerCPU_Shape		*m_Collider;
		const SParticleDeclaration::SEvent	*m_EventOnCollide;
		bool								m_UsePostContactVel;
		hh_u32								m_EventTransformsMode;
		bool								m_EventIsSticky;

		// will be used for sub integration when a collision occurs, on the post-collision portion
		CFloat3								m_EstimationAccel;
		CFloat3								m_EstimationVelField;
		float								m_EstimationDrag;
		TStridedMemoryView<const CFloat3>	m_EstimationForces;				// R
		TStridedMemoryView<const CFloat3>	m_EstimationAccels;				// R
		TStridedMemoryView<const CFloat3>	m_EstimationMediaVelocities;	// R

		SCollisionDescriptor();
	};

	virtual void	Collide(const CParticleEvaluationContext &context, const SCollisionDescriptor &desc);

	//----------------------------------------------------------------------------
	//
	//	Environment interface
	//
	//----------------------------------------------------------------------------

	enum	EFieldType
	{
		Field_VelocityAndDensity = 0,	//xyz = velocity, w = density
		Field_Gravitational,
		Field_Magnetic,
		__MaxFieldTypes
	};

	virtual bool	HasEnvironmentField(EFieldType field) const;

	virtual void	BatchedSampleField(	EFieldType								field,
										const TStridedMemoryView<const CFloat3>	&positions,
										const TStridedMemoryView<CFloat4>		&outField,
										hh_u32									filterID) const;

	virtual void	BatchedSampleField(	EFieldType								field,
										const TStridedMemoryView<const CFloat3>	&positions,
										const TStridedMemoryView<CFloat4>		&outField,
										const TStridedMemoryView<CFloat4>		&outFieldSpatialDerivatives,
										hh_u32									filterID) const;

	//----------------------------------------------------------------------------
	//
	//	Audio interface
	//
	//----------------------------------------------------------------------------

	// global audio spectrum functions
	virtual	TMemoryView<const float * const>	GetAudioSpectrum(CStringId channelGroup, hh_u32 &outBaseCount) const;
	virtual	TMemoryView<const float * const>	GetAudioWaveform(CStringId channelGroup, hh_u32 &outBaseCount) const;

	// sound playing functions
	struct	SSoundDescriptor
	{
		CStringId		m_ChannelGroup;
		const char		*m_Path;
		const char		*m_EventStart;
		const char		*m_EventStop;
		CFloat3			m_WorldPosition;
		float			m_Volume;
		float			m_StartTimeOffsetInSeconds;
		float			m_PlayTimeInSeconds;
		hh_u32			m_UserData;

		SSoundDescriptor() : m_Path(null), m_EventStart(null), m_EventStop(null) {}
	};

	// StartSound returns the 'sound' instance. if StartSound returns null, the sound action is Terminated immediately
	virtual void						*StartSound(const SSoundDescriptor &descriptor);

	// UpdateSound should return false when the sound has finished playing.
	virtual bool						UpdateSound(void *sound, const CFloat3 &worldPosition, float volume);

	// StopSound is called when the sound is no longer needed.
	virtual void						StopSound(void *sound);

	//----------------------------------------------------------------------------
	//
	//	Decal interface
	//
	//----------------------------------------------------------------------------

	struct	SDecalDescriptor
	{
		CFloat4			m_BoxConstant_A16;	// will be aligned to SIMD-friendly address (16-bytes)
		CFloat3			m_BoxScale_A16;		// will be aligned to SIMD-friendly address (16-bytes)
		float			m_FaceCullLimit;

		enum	EMaterialType
		{
			Material_AlphaTest = 0,
			Material_Blended,

			__MaxMaterials,
		};

		union
		{
			const CActionFactoryDecalSpawner	*m_ActionFactory;
			hh_u64								_padding_32_64;
		};

		EMaterialType	Material() const;
		CStringId		UserMaterial() const;
		CStringId		Diffuse() const;
		CStringId		NMap() const;

		SDecalDescriptor() { HH_ASSERT(Mem::IsAligned<0x10>(this)); }
	};

#ifndef	HH_COMPILER_GCC
	HH_STATIC_ASSERT( 0 == HH_MEMBER_OFFSET(SDecalDescriptor, m_BoxConstant_A16));
	HH_STATIC_ASSERT(16 == HH_MEMBER_OFFSET(SDecalDescriptor, m_BoxScale_A16));
#endif

	virtual void						SpawnDecals(const SDecalDescriptor				&descriptor,
													const TMemoryView<void * const>		&contactObjects,
													const TMemoryView<void * const>		&contactSurfaces,
													const TMemoryView<const CFloat4>	&contactPositions,
													const TMemoryView<const CFloat4>	&contactNormals,
													const TMemoryView<const float>		&sizes,
													const TMemoryView<const CFloat4>	&colors);

	//----------------------------------------------------------------------------
	//
	//	Event interface
	//
	//----------------------------------------------------------------------------

	struct	SEventDescriptor
	{
		CStringId	m_EventName;
	};

	// 'mediumCollection' will never be null (you can assert this)
	// 'pageView' can be null:
	//	- if 'pageView' is null, the event does not come from a particle, but from a layer script.
	//		'indices' contain the SpawnerID, use mediumCollection->ResolveSpawnerFromSpawnerID(indices[i]) to retreive the initial spawner.
	//	- if 'pageView' is not null, the event comes from a particle.
	//		if 'indices' is empty, every particle in 'pageView' raised the event.
	//		if 'indices' is not empty, it contains the indices of all the particles in 'pageView' that raised the event.

	virtual void						RaiseEvent(	const SEventDescriptor			&descriptor,
													const CParticleMediumCollection	*mediumCollection,
													const CParticlePageView			*pageView,
													TMemoryView<const hh_u32>		indices);

	//----------------------------------------------------------------------------
	//
	//	Debug interface
	//
	//----------------------------------------------------------------------------

	virtual void						DebugAssert(const CParticleEvaluationContext		&context,
													const TStridedMemoryView<const hh_i32>	&predicates);

};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_SCENE_H__

