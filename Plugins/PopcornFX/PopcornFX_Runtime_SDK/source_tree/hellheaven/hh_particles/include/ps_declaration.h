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

#ifndef	__PS_DECLARATION_H__
#define	__PS_DECLARATION_H__

#include <hh_kernel/include/kr_base_types.h>
#include <hh_kernel/include/kr_string_id.h>

#include "hh_particles/include/ps_simcache.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
// TODO: move all these implementations to an isolated .cpp file

class	CParticleSampler;
class	CParticleSpatialDescriptor;
class	CActionFactory;
class	CParticlePageView;
//HH_FORWARD_DECLARE(SimulationCache);	// forward declaring doesn't work

//----------------------------------------------------------------------------

struct	HH_PARTICLES_EXPORT SFieldInitializer
{
	union
	{
		hh_u8				m_RawDefaultValueStorageU8[16];
		hh_u16				m_RawDefaultValueStorageU16[8];
		hh_u32				m_RawDefaultValueStorageU32[4];
		hh_i32				m_RawDefaultValueStorageI32[4];
		float				m_RawDefaultValueStorageF32[4];
		hh_u64				m_RawDefaultValueStorageU64[2];
	};

	SFieldInitializer()
	{
		m_RawDefaultValueStorageU64[0] = 0;
		m_RawDefaultValueStorageU64[1] = 0;
	}

	bool	Parse(const char *initializer, EBaseTypeID fieldType);
};

//----------------------------------------------------------------------------

struct	HH_PARTICLES_EXPORT SParticleDeclaration : public CNonCopyable
{
	struct	SField
	{
		enum	EFlags
		{
			UsedInEvolve				= 0x1,							// set when the field is read from by any of the particle evolvers.
			UsedInRender				= 0x2,							// set when the field is read from by any of the particle renderers.
			Used						= UsedInEvolve | UsedInRender,
			ModifiedInSpawn				= 0x4,							// set when the field is written to in the spawn stage.
			ModifiedInEvolve			= 0x8,							// set when the field is written to in the evolve stage.
			Modified					= ModifiedInSpawn | ModifiedInEvolve,
			Virtual						= 0x10,							// no real storage, only a single value is used for the whole stream

			NeedsSimCacheLerp			= 0x20,

			NeedsLocalToWorldTranslate	= 0x1000,
			NeedsLocalToWorldRotate		= 0x2000,
			NeedsLocalToWorldTransform	= NeedsLocalToWorldTranslate | NeedsLocalToWorldRotate,

			Private						= 0x10000,
		};

		CStringId			m_NameGUID;
		CStringId			m_FullNameGUID;	// in case of parent fields, will contain "parent.Color"
		EBaseTypeID			m_Type;
		hh_u32				m_StorageSize;
		hh_u32				m_Flags;
		SFieldInitializer	m_Initializer;

		static const SField	Invalid;

//		SField();
		SField(CStringId name, EBaseTypeID type, hh_u32 storageSize, hh_u32 flags);
		~SField();

		HH_FORCEINLINE bool	Valid() const { EBaseTypeID type = TBaseTypeID<void>::TypeID; return m_Type != type; }

		bool				IsUsed() const { return (m_Flags & Used) != 0; }
		bool				IsModified() const { return (m_Flags & Modified) != 0; }
		bool				IsSpawnConstant() const { return (m_Flags & (ModifiedInSpawn | ModifiedInEvolve | NeedsLocalToWorldTransform)) == 0;}
		bool				IsEvolveConstant() const { return (m_Flags & (ModifiedInSpawn | ModifiedInEvolve)) < ModifiedInEvolve;}
		bool				IsVirtual() const { return (m_Flags & Virtual) != 0; }
		bool				SimCacheLerp() const { return (m_Flags & NeedsSimCacheLerp) != 0; }
		hh_u32				TransformFilter() const
		{
			HH_STATIC_ASSERT(NeedsLocalToWorldTranslate == 0x1000 && NeedsLocalToWorldRotate == 0x2000 && NeedsLocalToWorldTransform == 0x3000);
			return (m_Flags & NeedsLocalToWorldTransform) >> 12;
		}

		bool				operator == (const SField &other) const;
		bool				operator != (const SField &other) const { return !(*this == other); }
	};

	struct	SAttrib
	{
		enum	EFlags
		{
			Used		= 1 << 0,
		};

		CStringId				m_NameGUID;
		CStringId				m_FullNameGUID;
		EBaseTypeID				m_Type;
		hh_u32					m_Flags;

		static const SAttrib	Invalid;

		SAttrib(CStringId name, EBaseTypeID type);
		~SAttrib();

		bool	operator == (const SAttrib &other) const;
		bool	operator != (const SAttrib &other) const { return !(*this == other); }
	};

	struct	SEvent
	{
		enum	EFlags
		{
			UsedInSpawn						= 1 << 0,
			UsedInEvolve					= 1 << 1,
			Used							= UsedInSpawn | UsedInEvolve,
			NeedAutoForwardParentFields		= 1 << 2,
			BroadcastSpawnerIDs				= 1 << 3,
		};

		CStringId				m_NameGUID;
		CStringId				m_FullNameGUID;
		const CActionFactory	*m_Action;
		hh_u32					m_Flags;

		bool					m_CanAutoForwardParentFields;
		bool					m_Broadcast;

		static const SEvent		Invalid;
		SEvent(CStringId name, CActionFactory *action, bool broadcast);
		bool	operator == (const SEvent &other) const;
		bool	operator != (const SEvent &other) const { return !(*this == other); }
	};

	enum	EOptimizationLevel
	{
		OptLevel_Debug = 0,
		OptLevel_Editor,
		OptLevel_Shipping,
	};

	enum	ESpawnerData
	{
		SpawnerData_Age				= 1 << 0,
		SpawnerData_Life			= 1 << 1,
		SpawnerData_SpawnRate		= 1 << 2,
		SpawnerData_SpawnCount		= 1 << 3,
		SpawnerData_BaseSpawnRate	= 1 << 4,
		SpawnerData_BaseSpawnCount	= 1 << 5,
		SpawnerData_FirstSpawnDelay	= 1 << 6,
		SpawnerData_Velocity		= 1 << 7,
		SpawnerData_EmittedCount	= 1 << 8,
		SpawnerData_SpawnerID		= 1 << 9,
		SpawnerData_ParentID		= 1 << 10,
	};

	TArray<SField>						m_Fields;
	TArray<SAttrib>						m_Attribs;
	TArray<SEvent>						m_Events;
	TArray<CParticleSampler*>			m_Samplers;
	TArray<SField>						m_ParentFields;
	TArray<CParticleSpatialDescriptor*>	m_SpatialLayers;

	hh_u32								m_NeededSpawnerData;	// combination of 'ESpawnerData' flags
	bool								m_NeedsInstanceData;
	bool								m_HasEvolutionSideEffects;
	bool								m_DisableSubFrameInterpolation;
	bool								m_InsideLocalSpace;
	bool								m_HasMicroUpdates;
	EOptimizationLevel					m_OptimizationLevel;	// used by scripts to know if they should optimize their build. true by default
	bool								m_IsBuilt;

	PSimulationCache					m_SimCache;	// Do we actually need this here ??

	SParticleDeclaration();
	~SParticleDeclaration();

	void							Clear();
	hh_u32							GetCompilerASTFlags() const;
	hh_u32							GetCompilerIRFlags() const;

	CGuid							FindFieldID(CStringId name) const;
	CGuid							FindAttribID(CStringId name) const;
	CGuid							FindEventID(CStringId name) const;
	CGuid							FindSamplerID(CStringId name) const;
	CGuid							FindParentFieldID(CStringId name) const;

	const SField					&FindField(CStringId name) const;
	const SAttrib					&FindAttrib(CStringId name) const;
	const SEvent					&FindEvent(CStringId name) const;
	const SField					&FindParentField(CStringId name) const;

	CGuid							AddNewField(CStringId name, EBaseTypeID type, hh_u32 storageSize, hh_u32 flags = 0);
	CGuid							AddNewAttrib(CStringId name, EBaseTypeID type);
	CGuid							AddNewEvent(CStringId name, CActionFactory *action, bool broadcast);
	CGuid							AddNewParentField(CStringId name, EBaseTypeID type, hh_u32 storageSize, hh_u32 flags = 0);

	// if the field already exists: maxes the storage size, and combines the flags. asserts if the type is different, and returns an invalid ID
	CGuid							AddFieldIFN(const SField &field);
	CGuid							AddAttribIFN(const SAttrib &attrib);
	CGuid							AddEventIFN(const SEvent &ev);
	CGuid							AddParentFieldIFN(const SField &field);

	// helpers. avoid using these, prefer the ones taking the struct directly
	CGuid							AddFieldIFN(CStringId name, EBaseTypeID type, hh_u32 storageSize, hh_u32 flags = 0) { return AddFieldIFN(SField(name, type, storageSize, flags)); }
	CGuid							AddAttribIFN(CStringId name, EBaseTypeID type) { return AddAttribIFN(SAttrib(name, type)); }
	CGuid							AddEventIFN(CStringId name, CActionFactory *action, bool broadcast) { return AddEventIFN(SEvent(name, action, broadcast)); }
	CGuid							AddParentFieldIFN(CStringId name, EBaseTypeID type, hh_u32 storageSize, hh_u32 flags = 0) { return AddParentFieldIFN(SField(name, type, storageSize, flags)); }

	bool							AreParentFieldsUsed() const;

	bool							HasSimCache() const { return m_SimCache != null; }

	void							MarkUsedFieldsByChild(const TMemoryView<const SField> &childParentFields, SField::EFlags useLocation);

	bool							FieldIsBlank(CGuid fieldId) const;
	bool							FieldIsBlank(CStringId name) const { return FieldIsBlank(FindFieldID(name)); }

	// if (field exists) return isBlank; else return or;
	bool							FieldIsBlankOr(CStringId name, bool ifNotExists) const;

	bool							operator == (const SParticleDeclaration &other) const;
	bool							operator != (const SParticleDeclaration &other) const { return !(*this == other); }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_DECLARATION_H__

