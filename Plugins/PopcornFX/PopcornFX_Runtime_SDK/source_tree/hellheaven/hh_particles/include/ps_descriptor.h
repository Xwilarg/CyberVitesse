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

#ifndef	__PS_DESCRIPTOR_H__
#define	__PS_DESCRIPTOR_H__

#include <hh_base_object/include/hbo_object.h>
#include <hh_engine_utils/include/kr_actions.h>

#include "hh_particles/include/ps_declaration.h"

#include <hh_kernel/include/kr_callbacks.h>
#include <hh_kernel/include/kr_refptr.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(CompilerSyntaxNodeExpression);
HH_FORWARD_DECLARE(ParticleEffect);
HH_FORWARD_DECLARE(ParticleDescriptor);
HH_FORWARD_DECLARE(ParticleSpatialDescriptor);
HH_FORWARD_DECLARE(ParticleState);
HH_FORWARD_DECLARE(ParticleSampler);
HH_FORWARD_DECLARE(ParticleSamplerDescriptor);
HH_FORWARD_DECLARE(ParticleEvolver);
HH_FORWARD_DECLARE(ParticleRenderer);
HH_FORWARD_DECLARE(CompilerBlobCache);
HH_FORWARD_DECLARE(SimulationCache);

struct	SParticleStreamDef;
struct	SSpawnTransformsPack;

// fwd declare <hh_maths/include/hh_maths_primitives.h>
template<typename _Type, hh_u32 _Dimensions>
class	TPrimitiveAABB;
typedef TPrimitiveAABB<float, 3>	CAABB;

//----------------------------------------------------------------------------

#ifndef	PK_ENABLE_PARTICLE_REPORT
#	ifndef	HH_RETAIL
#		define PK_ENABLE_PARTICLE_REPORT		1
#	else
#		define PK_ENABLE_PARTICLE_REPORT		0
#	endif
#endif

#if	(PK_ENABLE_PARTICLE_REPORT != 0)
#	define PK_IF_PARTICLE_REPORT(...)			__VA_ARGS__
#else
#	define PK_IF_PARTICLE_REPORT(...)			do { } while (0)
#endif

//----------------------------------------------------------------------------

class	CParticleBuildReport
{
public:
	// The message is parsed by the editor and should match a specific format.
	// this might change in the future.
	// currently, it should follow the following guidelines:
	// - a message in direct relation with a specific field should identify the field by reffering to it with "@" + FieldName.
	//   example: "Field @Color has type 'float3', expecting type 'float4'"

	enum	EMessageType
	{
		Message_Info = 0,
		Message_Warning,
		Message_Error,
	};

	struct	SMessage
	{
		const CBaseObject	*m_SourceNode;
		EMessageType		m_Type;
		CString				m_Message;

		SMessage(const CBaseObject *node, EMessageType type, const CString &message)
		:	m_SourceNode(node)
		,	m_Type(type)
		,	m_Message(message)
		{
		}
	};

private:
#if	(PK_ENABLE_PARTICLE_REPORT != 0)
	TArray<SMessage>	m_MessageList;
#endif

public:
	CParticleBuildReport();
	~CParticleBuildReport();

#if	(PK_ENABLE_PARTICLE_REPORT != 0)

	void						Clear();
	void						AddMessage(const CBaseObject *node, const CString &message, EMessageType type = Message_Error);
	void						Merge(const CParticleBuildReport &other);
	void						MergeUnique(const CParticleBuildReport &other);
	CParticleBuildReport		&operator += (const CParticleBuildReport &other);
	TMemoryView<const SMessage>	MessageList() const { return m_MessageList; }

#else // no particle build report

	void						Clear() {}
	TMemoryView<const SMessage>	MessageList() const { return TMemoryView<const SMessage>(); }

#endif
};

//----------------------------------------------------------------------------
//
//	Fields
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleFieldDeclaration), public CBaseObject
{
private:
	HBO_FIELD(CString,	FieldName);
	HBO_FIELD(hh_u32,	FieldType);
	HBO_FIELD(hh_u32,	TransformFilter);
	HBO_FIELD(bool,		EnableSimCacheLerp);
	HBO_FIELD(CString,	Initializer);

	bool				_OnFieldNameModified();
	bool				_OnFieldModified();

protected:
	virtual bool		VirtualLink() override;
	CStringId			m_FieldNameID;
public:
	CParticleFieldDeclaration();
	~CParticleFieldDeclaration() {}

	enum	ETransformFilters
	{
		TransformNone = 0,
		TransformTranslate,
		TransformRotate,
		TransformFull,
		__MaxTransformFilters
	};

	CStringId			FieldNameID() const { return m_FieldNameID; }

	HBO_CLASS_DECLARATION();
};

HH_DECLARE_REFPTRCLASS(ParticleFieldDeclaration);

//----------------------------------------------------------------------------
//
//	Attributes
//
//----------------------------------------------------------------------------

struct	SAttributesContainer_SAttrib
{
	union
	{
		hh_u32	m_Data32u[4];
		hh_i32	m_Data32i[4];
		float	m_Data32f[4];
	};

	template<typename _ScalarType>
	HH_FORCEINLINE _ScalarType			*Get();
	template<typename _ScalarType>
	HH_FORCEINLINE const _ScalarType	*Get() const;
};

//----------------------------------------------------------------------------

struct	SAttributesContainer_SSampler
{
	PParticleSamplerDescriptor	m_Desc;
};

//----------------------------------------------------------------------------

struct	HH_PARTICLES_EXPORT SAttributesContainer : public CNonCopyable
{
public:
	typedef SAttributesContainer_SAttrib		SAttrib;
	typedef SAttributesContainer_SSampler		SSampler;

private:
	SAttributesContainer();
	~SAttributesContainer();

public:
	hh_u32							m_AttribCount;
	hh_u32							m_SamplerCount;
	hh_u32							m_Padding[2];

public:
	static SAttributesContainer		*Create(hh_u32 attribCount, hh_u32 samplerCount); // new
	void							Destroy(); // self delete !

	SAttributesContainer			*Copy() const;

	bool							SameSize(const SAttributesContainer *other) const { return other->m_AttribCount == m_AttribCount && other->m_SamplerCount == m_SamplerCount; }
	void							CopyTo(SAttributesContainer *destination) const; // destination must have SameSize

	bool							Empty() const { return m_AttribCount == 0 && m_SamplerCount == 0; }

	HH_FORCEINLINE const SAttrib	*AttributesBegin() const { return reinterpret_cast<const SAttrib*>(this + 1); }
	HH_FORCEINLINE SAttrib			*AttributesBegin() { return reinterpret_cast<SAttrib*>(this + 1); }
	HH_FORCEINLINE const SSampler	*SamplersBegin() const { return reinterpret_cast<const SSampler*>(AttributesBegin() + m_AttribCount); }
	HH_FORCEINLINE SSampler			*SamplersBegin() { return reinterpret_cast<SSampler*>(AttributesBegin() + m_AttribCount); }

	hh_u32							AttribCount() const { return m_AttribCount; }
	hh_u32							AttributeCount() const { return m_AttribCount; }
	hh_u32							SamplerCount() const { return m_SamplerCount; }

	TMemoryView<SAttrib>			Attributes();
	TMemoryView<const SAttrib>		Attributes() const;
	TMemoryView<SSampler>			Samplers();
	TMemoryView<const SSampler>		Samplers() const;
};

//----------------------------------------------------------------------------

HH_STATIC_ASSERT(sizeof(SAttributesContainer) == 0x10);
HH_STATIC_ASSERT(sizeof(SAttributesContainer::SAttrib) == 0x10);
HH_REGISTER_RTTI(SAttributesContainer);
//HH_REGISTER_RTTI_SINGLE(SAttributesContainer*);
HH_REGISTER_RTTI_SINGLE(SAttributesContainer**);

template<> HH_FORCEINLINE float			*SAttributesContainer_SAttrib::Get<float>()  { return m_Data32f; }
template<> HH_FORCEINLINE hh_u32		*SAttributesContainer_SAttrib::Get<hh_u32>() { return m_Data32u; }
template<> HH_FORCEINLINE hh_i32		*SAttributesContainer_SAttrib::Get<hh_i32>() { return m_Data32i; }
template<> HH_FORCEINLINE const float	*SAttributesContainer_SAttrib::Get<float>()  const { return m_Data32f; }
template<> HH_FORCEINLINE const hh_u32	*SAttributesContainer_SAttrib::Get<hh_u32>() const { return m_Data32u; }
template<> HH_FORCEINLINE const hh_i32	*SAttributesContainer_SAttrib::Get<hh_i32>() const { return m_Data32i; }

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleAttributeDeclaration), public CBaseObject
{
private:
	HBO_FIELD(CString,			AttributeName);
	HBO_FIELD(hh_u32,			AttributeType);
	HBO_FIELD(hh_u32,			AttributeScope);
	HBO_FIELD(CString,			AttributeDescription);

	HBO_FIELD_ABSTRACT(float,	DefaultValueF1);
	HBO_FIELD_ABSTRACT(CFloat2,	DefaultValueF2);
	HBO_FIELD_ABSTRACT(CFloat3,	DefaultValueF3);
	HBO_FIELD_ABSTRACT(CFloat4,	DefaultValueF4);
	HBO_FIELD_ABSTRACT(hh_i32,	DefaultValueI1);
	HBO_FIELD_ABSTRACT(CInt2,	DefaultValueI2);
	HBO_FIELD_ABSTRACT(CInt3,	DefaultValueI3);
	HBO_FIELD_ABSTRACT(CInt4,	DefaultValueI4);

	HBO_FIELD(bool,				HasMin);
	HBO_FIELD(bool,				HasMax);

	HBO_FIELD_ABSTRACT(float,	MinValueF1);
	HBO_FIELD_ABSTRACT(CFloat2,	MinValueF2);
	HBO_FIELD_ABSTRACT(CFloat3,	MinValueF3);
	HBO_FIELD_ABSTRACT(CFloat4,	MinValueF4);
	HBO_FIELD_ABSTRACT(hh_i32,	MinValueI1);
	HBO_FIELD_ABSTRACT(CInt2,	MinValueI2);
	HBO_FIELD_ABSTRACT(CInt3,	MinValueI3);
	HBO_FIELD_ABSTRACT(CInt4,	MinValueI4);

	HBO_FIELD_ABSTRACT(float,	MaxValueF1);
	HBO_FIELD_ABSTRACT(CFloat2,	MaxValueF2);
	HBO_FIELD_ABSTRACT(CFloat3,	MaxValueF3);
	HBO_FIELD_ABSTRACT(CFloat4,	MaxValueF4);
	HBO_FIELD_ABSTRACT(hh_i32,	MaxValueI1);
	HBO_FIELD_ABSTRACT(CInt2,	MaxValueI2);
	HBO_FIELD_ABSTRACT(CInt3,	MaxValueI3);
	HBO_FIELD_ABSTRACT(CInt4,	MaxValueI4);

	CFloat4						m_DefaultValueF;
	CFloat4						m_MinValueF;
	CFloat4						m_MaxValueF;

	CInt4						m_DefaultValueI;
	CInt4						m_MinValueI;
	CInt4						m_MaxValueI;

	bool						_OnFieldModified();
	EFieldVisibility			_IsF1VisibleInEditor() const;
	EFieldVisibility			_IsF2VisibleInEditor() const;
	EFieldVisibility			_IsF3VisibleInEditor() const;
	EFieldVisibility			_IsF4VisibleInEditor() const;
	EFieldVisibility			_IsI1VisibleInEditor() const;
	EFieldVisibility			_IsI2VisibleInEditor() const;
	EFieldVisibility			_IsI3VisibleInEditor() const;
	EFieldVisibility			_IsI4VisibleInEditor() const;

	EFieldVisibility			_IsMinF1VisibleInEditor() const;
	EFieldVisibility			_IsMinF2VisibleInEditor() const;
	EFieldVisibility			_IsMinF3VisibleInEditor() const;
	EFieldVisibility			_IsMinF4VisibleInEditor() const;
	EFieldVisibility			_IsMinI1VisibleInEditor() const;
	EFieldVisibility			_IsMinI2VisibleInEditor() const;
	EFieldVisibility			_IsMinI3VisibleInEditor() const;
	EFieldVisibility			_IsMinI4VisibleInEditor() const;

	EFieldVisibility			_IsMaxF1VisibleInEditor() const;
	EFieldVisibility			_IsMaxF2VisibleInEditor() const;
	EFieldVisibility			_IsMaxF3VisibleInEditor() const;
	EFieldVisibility			_IsMaxF4VisibleInEditor() const;
	EFieldVisibility			_IsMaxI1VisibleInEditor() const;
	EFieldVisibility			_IsMaxI2VisibleInEditor() const;
	EFieldVisibility			_IsMaxI3VisibleInEditor() const;
	EFieldVisibility			_IsMaxI4VisibleInEditor() const;

protected:
public:
	CParticleAttributeDeclaration();
	~CParticleAttributeDeclaration() {}

	enum	EScope
	{
		Scope_Global = 0,
		Scope_Instance,
	};

	const SAttributesContainer::SAttrib				&GetMinValue() const;
	const SAttributesContainer::SAttrib				&GetMaxValue() const;

	void											GetDefaultValue(SAttributesContainer::SAttrib &outAttrib) const;
	HH_FORCEINLINE SAttributesContainer::SAttrib	GetDefaultValue() const { SAttributesContainer::SAttrib attrib; GetDefaultValue(attrib); return attrib; }
	void											SetDefaultValue(const SAttributesContainer::SAttrib	&value);

	void											ClampToRangeIFN(SAttributesContainer::SAttrib &attrib) const;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleAttributeDeclaration);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleAttributeList), public CBaseObject
{
private:
	HBO_FIELD(TArray<CParticleAttributeDeclaration*>,	AttributeList);
	HBO_FIELD(TArray<CParticleSampler*>,				SamplerList);

	SAttributesContainer		*m_DefaultAttributes;

	bool						_OnAttributeListModified();
	bool						_OnSamplerListModified(const _TypeOfSamplerList &oldVal);

	void						_RebuildDefaultAttrs();
	void						_RebuildUniqueSamplerIDs(const _TypeOfSamplerList &oldSamplers);

protected:
	TArray<hh_u32>								m_UniqueAttributeIDs;
	TArray<CParticleAttributeDeclaration *>		m_UniqueAttributeList;
	TArray<CParticleSampler *>					m_UniqueSamplerList;

	virtual bool				VirtualLink() override;
	void						_RebuildUniqueAttribIDs();

public:
	CParticleAttributeList();
	~CParticleAttributeList();

	TCallbackChainUnordered<void(CParticleAttributeList*)>	m_OnAttributeListModified;

	SAttributesContainer		const * const * DefaultAttributes() const { return &m_DefaultAttributes; }

	// UniqueAttributeIDs()[uniqAttributeIndex] => correponding index in AttributeList
	TMemoryView<const hh_u32>	UniqueAttributeIDs() const { return m_UniqueAttributeIDs; }

	TMemoryView<CParticleAttributeDeclaration * const>			UniqueAttributeList() { return m_UniqueAttributeList.View(); }
	TMemoryView<const CParticleAttributeDeclaration * const>	UniqueAttributeList() const { return TMemoryView<const CParticleAttributeDeclaration * const>(m_UniqueAttributeList.View()); }

	TMemoryView<const CParticleSampler * const >				UniqueSamplerList() const { return TMemoryView<const CParticleSampler * const>(m_UniqueSamplerList.View()); }
	TMemoryView<CParticleSampler * const>						UniqueSamplerList() { return m_UniqueSamplerList.View(); }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleAttributeList);

//----------------------------------------------------------------------------
//
//	Parent fields
//
//----------------------------------------------------------------------------

struct	SParentField
{
	SAttributesContainer_SAttrib	m_Data;
	EBaseTypeID						m_Type;
	CStringId						m_Name;
	hh_u8							m_Padding[8];
};
HH_STATIC_ASSERT(sizeof(SParentField) % 16 == 0);

//----------------------------------------------------------------------------

struct	SParentFieldsContainer : public CNonCopyable
{
	TArray<SParentField, TArrayAligned16>	m_ParentFields;
};

HH_REGISTER_RTTI(SParentFieldsContainer);
//HH_REGISTER_RTTI_SINGLE(SParentFieldsContainer*);
HH_REGISTER_RTTI_SINGLE(SParentFieldsContainer**);

//----------------------------------------------------------------------------
//
//	Events
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleEventDeclaration), public CBaseObject
{
private:
	// General
	HBO_FIELD(CString,			EventName);
	HBO_FIELD(hh_u32,			EventMode);
	HBO_FIELD(bool,				Broadcast);
	HBO_FIELD(CActionFactory*,	EventAction);

	// Private, not serialized
	HBO_FIELD(CParticleAttributeList*,	_SrcEventAttributes);
	HBO_FIELD(CParticleAttributeList*,	_DstEventAttributes);

	bool						_OnFieldModified();
	bool						_OnNameModified();
	bool						_OnEventAttributesModified();
	CStringId					m_EventNameGUID;

protected:
	virtual bool				VirtualLink() override;
	void						_ResetSrcDstEventAttributes_NoNotify();
	void						_ComputeBroadcastMasks();

	bool						m_ShouldBroadcastSpawerID;

public:
	CParticleEventDeclaration();
	~CParticleEventDeclaration() {}

	enum	EMode
	{
		Mode_TriggerAlways = 0,
		Mode_TriggerOnce,
	};

	const CStringId				&EventNameGUID() const { return m_EventNameGUID; }

	bool						ShouldBroadcastSpawerID() const { return m_ShouldBroadcastSpawerID; }

	HBO_CLASS_DECLARATION();
};

HH_DECLARE_REFPTRCLASS(ParticleEventDeclaration);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleState), public CBaseObject
{
private:
	HBO_FIELD(CString,						StateName);
	HBO_FIELD(TArray<CParticleEvolver*>,	Evolvers);
	HBO_FIELD(float,						MicroUpdatesMinFPS);
	HBO_FIELD(hh_u32,						MaxMicroUpdatesPerFrame);

	bool									_OnNameModified();
	bool									_OnMicroUpdateModified();
	bool									_OnEvolverModified();

	float									m_MicroUpdateMaxDt;

protected:
	virtual bool							VirtualLink() override;

public:
	CParticleState();
	~CParticleState();

	void									SetupParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const;
	void									SetupAfterParticleDeclaration(SParticleDeclaration &decl, CParticleBuildReport &outReport) const;
	void									CheckParticleDeclaration(const SParticleDeclaration &decl, CParticleBuildReport &outReport) const;
	bool									CompatibleWith(const CParticleState *other) const;

	bool									MicroUpdateEnabled() const { return MaxMicroUpdatesPerFrame() != 1; }
	float									MicroUpdateMaxDt() const { return m_MicroUpdateMaxDt; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleState);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSpatialLayerList), public CBaseObject
{
private:
	HBO_FIELD(TArray<CParticleSpatialDescriptor*>,	LayerList);

	bool						_OnLayerListModified();

protected:
	virtual bool				VirtualLink() override;
	void						_RebuildUniqueAttribIDs();

public:
	CParticleSpatialLayerList();
	~CParticleSpatialLayerList();

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSpatialLayerList);

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleEvaluator);
HH_FORWARD_DECLARE(CompilerIR);

//----------------------------------------------------------------------------
//
//	Particle effect root
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleEffect), public CBaseObject
{
private:
	// Internal
	HBO_FIELD(CActionFactory*,				OnSpawn);
	HBO_FIELD(TArray<CActionFactory*>,		LayerGroups);
	HBO_FIELD(CParticleAttributeList*,		CustomAttributes);
	HBO_FIELD(CParticleSpatialLayerList*,	SpatialLayers);
	// Bounds
	HBO_FIELD(hh_i32,						BoundsMode);
	HBO_FIELD(CFloat3,						StaticBoundsMin);
	HBO_FIELD(CFloat3,						StaticBoundsMax);
	HBO_FIELD(CString,						BoundScaleAttributeX);
	HBO_FIELD(CString,						BoundScaleAttributeY);
	HBO_FIELD(CString,						BoundScaleAttributeZ);

	bool									_OnAttributesModified();
	bool									_OnSpatialLayersModified();
	bool									_OnStaticBoundsModified();
	bool									_OnBoundsScaleModified();

	EFieldVisibility						_AreBoundsVisibleInEditor() const;

protected:
	virtual bool							VirtualLink() override;

	bool									m_HasBoundsScale;
	CStringId								m_BoundsScaleAttribNameId[3];
	CGuid									m_BoundsScaleScalarSlot[3];

public:
	CParticleEffect();
	~CParticleEffect();

	enum	EBoundsMode
	{
		Bounds_None = 0,
		Bounds_ExplicitWorld,
		Bounds_ExplicitLocal,
	};

	// only triggered when 'CustomAttributes' or 'SpatialLayers' have changed.
	TCallbackChainUnordered<void(CParticleEffect*)>	m_OnEffectModified;

	// Bounds management
	bool		HasBounds() const { return BoundsMode() != Bounds_None; }
	// All 'ComputeBounds' methods return the worldspace axis-aligned Bounds of the effect
//	CAABB		ComputeBounds(const PCActionInstance &action) const;
	// This one will be faster than the one taking the generic action
	CAABB		ComputeBounds(const PCActionParameters &actionParameters) const;
	// This one will be faster than the one taking the generic action parameters
	CAABB		ComputeBounds(const SSpawnTransformsPack *spawnTransformPack, const SAttributesContainer *attributeContainer) const;

	void			SerializeAllScriptCaches();
	static void		SerializeAllScriptCachesOfFile(const PBaseObjectFile &file);

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleEffect);

//----------------------------------------------------------------------------
//
//	Particle descriptor
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleDescriptor), public CBaseObject
{
private:
	// Category: General
	HBO_FIELD(float,									InheritInitialVelocity);	// not really nice, find a better way, to actually _fetch_ the spawner velocity
	HBO_FIELD(bool,										EnableSpawnerIDs);			// /!\: mind GloballyEnableSpawnerIDs not included !
	HBO_FIELD(bool,										EnableParentIDs);
	HBO_FIELD(bool,										EnableDeterminism);
	HBO_FIELD(hh_u32,									PrefferedSimLocation);
	HBO_FIELD(hh_u32,									PrefferedStorageSize);
	HBO_FIELD(hh_u32,									PageLocalization);
	HBO_FIELD(CString,									BoundsPositionField);
	HBO_FIELD(CString,									BoundsSizeField);

	HBO_FIELD(bool,										UseSimulationCache);
	HBO_FIELD(CString,									SimulationCachePath);

	// Category: Internal
	HBO_FIELD(bool,										MergeMediumIFP);	// not serialized
	HBO_FIELD(bool,										Visible);			// not serialized
	HBO_FIELD(hh_u32,									OptimizationLevel);	// not serialized

//	HBO_FIELD(TArray<CParticleRenderer*>,				Renderers);
	HBO_FIELD(CParticleRenderer*,						Renderer);

	HBO_FIELD(TArray<CParticleFieldDeclaration*>,		SimCacheFields);
	HBO_FIELD(TArray<CParticleFieldDeclaration*>,		CustomFields);
	HBO_FIELD(TArray<CParticleEventDeclaration*>,		CustomEvents);
	HBO_FIELD(TArray<CParticleSampler*>,				Samplers);

	HBO_FIELD(TArray<CParticleFieldDeclaration*>,		VirtualParentFields);

	HBO_FIELD(CCompilerSyntaxNodeExpression*,			SpawnEvaluator);

	HBO_FIELD(TArray<CParticleState*>,					States);

	HBO_FIELD(TArray<CCompilerBlobCache*>,				CompileCache);

private:
	bool												_OnSpawnEvaluatorModified();
	bool												_OnDeclarationModified();
	bool												_OnPageLocalizationModified();
	bool												_OnCachedSimModified();
	bool												_OnStringFieldsModified();

	bool												_OnStatesModified();
	void												_OnEffectModified(CParticleEffect *effect);

	bool												_DeclarationSetupFields(CParticleBuildReport &outReport);
	bool												_DeclarationSetupAttributes(CParticleBuildReport &outReport);
	bool												_DeclarationSetupEvents(CParticleBuildReport &outReport);
	bool												_DeclarationSetupParentFields(CParticleBuildReport &outReport);
	bool												_DeclarationSetupSamplers(CParticleBuildReport &outReport);
	bool												_DeclarationSetupRenderers(CParticleBuildReport &outReport);
	bool												_DeclarationSetupStates(CParticleBuildReport &outReport);
	bool												_BuildFieldInitializer(hh_u32 fieldIndex, const char *initializer, CParticleBuildReport &outReport);

	void												_DeclarationHandleParentID();
	void												_DeclarationHandleSpawnerID();
	void												_DeclarationHandleRandContext();

	void												_RebuildDeclarationIfBuilt();
	bool												_ForceRebuildParticleDeclaration_NoLock(CParticleBuildReport *outReportPtr = null);

	EFieldVisibility									_NonCachedSimVisibility() const;
	EFieldVisibility									_CachedSimVisibility() const;

	void												_RedispatchCPU();

	TArray<CParticleRenderer*>							m_Renderers;
	PCCompilerIR										m_SpawnEvaluatorIR;
	bool												m_ParticlesNeedEvolveOnDeath;

	TArray<SParticleDeclaration::SField>				m_ParentFields;
	PSimulationCache									m_SimCache;

	enum	EDeclBuildState
	{
		Decl_None = 0,
		Decl_Building,
		Decl_Built,
	};

	volatile bool										m_RebuildDeclarationNextTime;
	mutable TAtomic<hh_u32>								m_DeclarationBuildState;
	mutable Threads::CCriticalSection					m_DeclarationBuildLock;

	SParticleDeclaration								m_Declaration;
	SParticleStreamDef									*m_StreamDef;
	CParticleBuildReport								m_BuildReport;
	bool												m_HasMultipleSpawner;
	CParticleEffect										*m_ParentEffect;

	CStringId											m_BoundsPositionFieldStringId;
	CStringId											m_BoundsSizeFieldStringId;

protected:
	virtual bool										VirtualLink() override;
	virtual void										VirtualUnlink() override;
//	virtual void										Invalidate() override;

public:
	CParticleDescriptor();
	~CParticleDescriptor();

	enum	ESimLocationHint
	{
		SimLocationHint_Auto = 0,
		SimLocationHint_CPU,
		SimLocationHint_GPU,

		__MaxSimLocationHints
	};
	enum	EStorageSizeHint
	{
		StorageSizeHint_Auto = 0,
		StorageSizeHint_Tiny,
		StorageSizeHint_Small,
		StorageSizeHint_Medium,
		StorageSizeHint_Large,

		__MaxStorageSizeHints
	};
	enum	ELocalizationHint
	{
		LocalizationHint_Default = 0,
		LocalizationHint_ON,
		LocalizationHint_OFF,

		__MaxLocalization
	};
	enum	EStorageLocation
	{
		StorageLocation_CPU = 0,	// used as shift counts in bitfields
		StorageLocation_GPU,

		__MaxStorageLocations
	};

	CGuid												StateID(const CString &stateName) const;
	PParticleState										State(CGuid stateId) const;
//	const CParticleEvaluator							*ExecutableSpawnEvaluator() const;
	const PCCompilerIR									&SpawnEvaluatorIR() const { return m_SpawnEvaluatorIR; }

	const TArray<CParticleRenderer*>					&Renderers() const { return m_Renderers; }
	bool												CompatibleWith(const CParticleDescriptor *other, const CString &stateName) const;
	bool												ParticlesNeedEvolveOnDeath() const { return m_ParticlesNeedEvolveOnDeath; }

	void												SetParentFields(const TMemoryView<const SParticleDeclaration::SField> &parentFields);

	HH_DEPRECATED("deprecated v1.8.0.22627 2015/01/16 20:29 get the root CParticleEffect and do effect->CustomAttributes() instead")
	CParticleAttributeList								*CustomAttributes() const { return m_ParentEffect != null ? m_ParentEffect->CustomAttributes() : null; }
	CParticleEffect										*ParentEffect() const { return m_ParentEffect; }

	// return false if build failed (can mean self recurse too)
	bool												RequestDeclaration_Lock();
	Threads::CCriticalSection							&DeclarationBuildLock() const { return m_DeclarationBuildLock; }
	bool												RequestDeclaration_NoLock();

	bool												ParticleDeclarationIsBuilt() const { return m_DeclarationBuildState == Decl_Built && m_Declaration.m_IsBuilt; }
	const SParticleDeclaration							&ParticleDeclaration() const { HH_ASSERT(ParticleDeclarationIsBuilt()); return m_Declaration; }
	SParticleStreamDef									*StreamDefinition() const { return m_StreamDef; }	// the returned pointer will change. if you need to keep it around, register a callback to 'm_OnParticleDeclarationModified' to refresh it when appropriate.
	const CParticleBuildReport							&BuildReport() const { return m_BuildReport; };
	PSimulationCache									SimCache() const { return m_SimCache;}
	bool												HasMultipleSpawner() const { return m_HasMultipleSpawner; }
	void												SetHasMultipleSpawner(bool hasMultipleSpawner) { m_RebuildDeclarationNextTime |= (m_HasMultipleSpawner != hasMultipleSpawner); m_HasMultipleSpawner = hasMultipleSpawner; }

	bool												ForceRebuildDeclaration() { m_RebuildDeclarationNextTime = true; return RequestDeclaration_Lock(); }
	void												SerializeScriptCache();

	CStringId											BoundsPositionFieldStringId() const { return m_BoundsPositionFieldStringId; }
	CStringId											BoundsSizeFieldStringId() const { return m_BoundsSizeFieldStringId; }

	// semi-gore: these are mutable, some methods are hooked from const descriptors, but it's safe, they're locked.
	mutable TCallbackChainUnordered<void(const CParticleDescriptor*)>	m_OnParticleDeclarationModified;
	mutable TCallbackChainUnordered<void(const CParticleDescriptor*)>	m_OnParticleDeclarationDestroyed;
	mutable TCallbackChainUnordered<void(const PCParticleEvaluator &, const PCCompilerIR &)>	m_OnParticleEvaluatorCompiled;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleDescriptor);


//----------------------------------------------------------------------------
//
//	Particle descriptor
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT HBO_CLASS(CParticleSpatialDescriptor), public CBaseObject
{
public:
	struct	SFieldObject
	{
		const SFieldObject					*m_Self;
		const CParticleSpatialDescriptor	*m_Parent;
		hh_u32								m_SelfID;
		CStringId							m_NameGUID;
	};

private:
	HBO_FIELD(CString,								LayerName);
	HBO_FIELD(bool,									Global);
	HBO_FIELD(float,								CellSize);
	HBO_FIELD(TArray<CParticleFieldDeclaration*>,	CustomFields);
	HBO_FIELD(bool,									VisibleInEditor);	// dummy field used only for correct display editor-side, don't use outside the editor

	bool											_OnLayerNameModified();
	bool											_OnDeclarationModified();
	bool											_RebuildParticleDeclarationIFN(CParticleBuildReport *outReportPtr = null);
	static bool										_IsLayerNameValid(const char *name);

protected:
	SParticleDeclaration							m_Declaration;
	SParticleStreamDef								*m_StreamDef;
	TArray<SFieldObject>							m_FieldObjects;
	bool											m_NeedsDeclaration;
	bool											m_NeedsRebuildDeclaration;
	mutable Threads::CCriticalSection				m_DeclarationBuildLock;
	CParticleBuildReport							m_BuildReport;
	CParticleSpatialDescriptor						*m_Self;

	virtual bool									VirtualLink() override;
	virtual void									VirtualUnlink() override;
//	virtual void									Invalidate() override;

public:
	CParticleSpatialDescriptor();
	~CParticleSpatialDescriptor();

	bool											CompatibleWith(const CParticleSpatialDescriptor *other) const;

	bool											RequestDeclaration(); // return false if build failed (can mean self recurse too)
	Threads::CCriticalSection						&DeclarationBuildLock() const { return m_DeclarationBuildLock; }

	const SParticleDeclaration						&ParticleDeclaration() const { HH_ASSERT(m_Declaration.m_IsBuilt); return m_Declaration; }
	bool											ParticleDeclarationIsBuilt() const { return m_Declaration.m_IsBuilt; }
	SParticleStreamDef								*StreamDefinition() const { return m_StreamDef; }	// the returned pointer will change. if you need to keep it around, register a callback to 'm_OnParticleDeclarationModified' to refresh it when appropriate.
	const CParticleBuildReport						&BuildReport() const { return m_BuildReport; };
	TMemoryView<const SFieldObject>					FieldObjects() const { return m_FieldObjects; }

	// compat with CParticleDescriptor
	bool											RequestDeclaration_Lock() { return RequestDeclaration(); }

	// semi-gore: these are mutable, some methods are hooked from const descriptors, but it's safe, they're locked.
	mutable TCallbackChainUnordered<void(const CParticleSpatialDescriptor*)>	m_OnParticleDeclarationModified;
	mutable TCallbackChainUnordered<void(const CParticleSpatialDescriptor*)>	m_OnParticleDeclarationDestroyed;

	CParticleSpatialDescriptor						* const * Self() const { return &m_Self; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ParticleSpatialDescriptor);

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_DESCRIPTOR_H__
