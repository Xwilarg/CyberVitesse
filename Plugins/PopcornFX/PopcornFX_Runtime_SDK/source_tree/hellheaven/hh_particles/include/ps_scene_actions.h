#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2013/08/12 07:20 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_SCENE_ACTIONS_H__
#define	__PS_SCENE_ACTIONS_H__

#include "hh_particles/include/ps_actions.h"
#include "hh_particles/include/ps_scene.h"

#include <hh_kernel/include/kr_string_id.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Sound action
//
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ActionInstanceSound);

//----------------------------------------------------------------------------

class	HH_ENGINE_EXPORT HBO_CLASS(CActionFactorySound), public CActionFactoryParticle
{
private:
	HBO_FIELD(bool,						VisibleInEditor);	// don't use outside the editor
	HBO_FIELD(CString,					ChannelGroup);
	HBO_FIELD(CString,					SoundPath);
	HBO_FIELD(float,					Volume);
	HBO_FIELD(float,					TimeWindow_Start);
	HBO_FIELD(float,					TimeWindow_Duration);
	HBO_FIELD(CString,					EventStart);
	HBO_FIELD(CString,					EventStop);
	HBO_FIELD(hh_u32,					UserData);
	HBO_FIELD(CString,					VolumeField);
	HBO_FIELD(CFloat3,					Position);

	bool								_OnStringIDModified();

	CBaseObject::EFieldVisibility		_VisibleInEditorVisibility() const;

protected:
	virtual PActionInstance				VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const override;
	virtual bool						VirtualLink() override;

	CStringId							m_ChannelGroupID;

public:
	CActionFactorySound();
	virtual ~CActionFactorySound();

	CStringId							ChannelGroupID() const { return m_ChannelGroupID; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactorySound);

//----------------------------------------------------------------------------

class	CActionInstanceSound : public CActionInstanceParticle
{
private:
	SSpawnTransformsPack		m_SpawnTransforms;
	float						m_RunningTime;
	bool						m_Spatialized;
	float						m_Volume;
	void						*m_SceneSoundInstance;

protected:
	CActionInstanceSound(const CActionFactorySound *parentFactory, const PActionParameters &parameters);
	virtual void				VirtualTerminate(float dtLeft) override;

public:
	virtual ~CActionInstanceSound();
	virtual void				Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) override;
	virtual bool				Setup(Threads::SThreadContext *threadContext, const PActionParameters &parameters) override;

	HH_ACTIONS_DECLARE_INSTANCE(Sound);
};

//----------------------------------------------------------------------------
//
//	Decal-spawning action
//
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ActionInstanceDecalSpawner);

//----------------------------------------------------------------------------

class	HH_ENGINE_EXPORT HBO_CLASS(CActionFactoryDecalSpawner), public CActionFactoryParticle
{
private:
	HBO_FIELD(hh_u32,						Material);		// matches the enum in 'SDecalDescriptor::EMaterialType'
	HBO_FIELD(CString,						UserMaterial);
	HBO_FIELD(CString,						Diffuse);
	HBO_FIELD(CString,						Normal);

	HBO_FIELD(CFloat3,						BoxDimensions_Base);
	HBO_FIELD(CFloat3,						BoxDimensions_Scale);
	HBO_FIELD(float,						FaceCullLimit);

	HBO_FIELD(CString,						ScaleField);
	HBO_FIELD(CString,						ColorField);

	HBO_FIELD(CFloat3,						Position);

	bool									_OnPropertiesModified();

protected:
	virtual PActionInstance					VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const override;
	virtual bool							VirtualLink() override;

	IParticleScene::SDecalDescriptor		*m_DecalDescriptor;

	CStringId								m_UserMaterialStringId;
	CStringId								m_DiffuseStringId;
	CStringId								m_NMapStringId;

public:
	CActionFactoryDecalSpawner();
	~CActionFactoryDecalSpawner();

	const IParticleScene::SDecalDescriptor	*DecalDescriptor() const { return m_DecalDescriptor; }

	CStringId			UserMaterialStringId() const { return m_UserMaterialStringId; }
	CStringId			DiffuseStringId() const { return m_DiffuseStringId; }
	CStringId			NMapStringId() const { return m_NMapStringId; }

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactoryDecalSpawner);

//----------------------------------------------------------------------------

class	CActionInstanceDecalSpawner : public CActionInstanceParticle
{
private:
	float						m_Scale;
	CFloat4						m_Color;
	CFloat4x4					m_SpawnMatrix;
	void						*m_HitObject;
	void						*m_HitSurface;

protected:
	CActionInstanceDecalSpawner(const CActionFactoryDecalSpawner *parentFactory, const PActionParameters &parameters);

public:
	virtual ~CActionInstanceDecalSpawner();

	virtual void				Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) override;
//	static void					RunImmediate(const CActionFactoryDecalSpawner *parentFactory, const PActionParameters &parameters/*, float dt, float dtOffset*/) override;
	virtual bool				Setup(Threads::SThreadContext *threadContext, const PActionParameters &parameters) override;

	HH_ACTIONS_DECLARE_INSTANCE(DecalSpawner);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_SCENE_ACTIONS_H__

