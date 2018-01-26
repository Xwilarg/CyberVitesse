#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2007/12/07 12:24 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_ACTIONS_H__
#define __KR_ACTIONS_H__

#include "hh_base_object/include/hbo_object.h"
#include "hh_engine_utils/include/kr_actions_parameters.h"
#include "hh_kernel/include/kr_containers_list.h"
#include "hh_kernel/include/kr_callbacks.h"

#if	(PK_HAS_DETERMINISM != 0)
namespace	PopcornFX
{
	class	CAbstractRandomGenerator;
	typedef	CAbstractRandomGenerator	CActionPRNG;
}
#else
namespace	PopcornFX
{
	class	CRandomGenerator;
	typedef	CRandomGenerator		CActionPRNG;
}
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ActionInstance);

#define	EU_ACTION_FACTORY_KEEPS_TRACK_OF_CHILDS

namespace	Threads
{
	struct	SThreadContext;
	class	CAbstractPool;
}

class	CDiscreteProbabilityFunction1D_O1;	// used for CActionWithRandomChilds

//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT CActionExecutor
{
private:
	Threads::CEvent				m_TasksCompletionEvent;
	TSlotArray<PActionInstance>	m_ActionList;
	TSlotArray<PActionInstance>	m_NewActionListSinceLastRun;
	float						m_CurrentDate;

	TArray<PActionInstance>		m_ActionsToGarbage;
	CThreadID					m_RunningThread;

	hh_u32						m_TargetDoneCount;
	TAtomic<hh_u32>				*m_DoneCount;

	CActionPRNG					*m_RandomGenerator;
	bool						m_OwnsGenerator;
	void						*m_UserHandle;

protected:
	void						_ResetCurrentDateAndResyncAllActions();

	void						_GarbageCollectActions();
	void						_Remove(PActionInstance actionInstance);

public:
	CActionExecutor(CActionPRNG *generator = null);
	~CActionExecutor();

	Threads::CCriticalSection	m_Lock;

	void						SetUserHandle(void *handle) { m_UserHandle = handle; }
	void						*UserHandle() const { return m_UserHandle; }

	void						Clear();
	void						RunAsync(float dt);
	void						RunWait();
	void						Run(float dt) { RunAsync(dt); RunWait(); }
	float						CurrentDate() const { return m_CurrentDate; }
	CActionPRNG					*RandomGenerator() const { return m_RandomGenerator; }

	// 'startDelay' may be negative in the following functions:
	void						AddActionInstance(PActionInstance instance, float startDelay = 0.0f);		// will be run next frame
	bool						AddAndRunActionInstance(PActionInstance instance, float startDelay = 0.0f, Threads::SThreadContext *threadContext = null);	// will be run immediately, and if still alive after the first run, will be run next frame, returns 'true' if it still points to a valid action instance, otherwise returns false (the action will have been destroyed...)

	void						Remove(PActionInstance actionInstance);

	// walks the action list and returns the total number of actions (active + pending)
	hh_u32						ComputeTotalActionCount() const;
};

//----------------------------------------------------------------------------

#define	HH_ACTIONS_DECLARE_INSTANCE(__suffix) \
public: \
	typedef	CActionFactory ## __suffix	FactoryType; \
	typedef	CActionInstance ## __suffix	InstanceType; \
	HH_FORCEINLINE const FactoryType	*ParentFactory() const \
	{ \
		HH_ASSERT(_ParentFactory() == null || PopcornFX::HBO::Cast<const FactoryType>(_ParentFactory()) != null); \
		return static_cast<const FactoryType*>(_ParentFactory()); \
	} \
	static TRefPtr<InstanceType>	NewInstance(const FactoryType *parentFactory, const PActionParameters &parameters) { return HH_NEW(InstanceType(parentFactory, parameters)); } \

#define	HH_ACTIONS_DECLARE_CUSTOM_INSTANCE_WITH_FACTORY(__instance, __factory) \
public: \
	typedef	__factory	FactoryType; \
	typedef	__instance	InstanceType; \
	HH_FORCEINLINE const FactoryType	*ParentFactory() const \
	{ \
		HH_ASSERT(_ParentFactory() == null || PopcornFX::HBO::Cast<const FactoryType>(_ParentFactory()) != null); \
		return static_cast<const FactoryType*>(_ParentFactory()); \
	} \
	static TRefPtr<InstanceType>	NewInstance(const FactoryType *parentFactory, const PActionParameters &parameters) { return HH_NEW(InstanceType(parentFactory, parameters)); } \

//----------------------------------------------------------------------------

class	CActionFactory;

class	HH_ENGINEUTILS_EXPORT CActionInstance : public CRefCountedObject
{
private:
	friend class	CActionExecutor;
	friend class	CActionFactory;

	CActionExecutor						*m_Executor;
	float								m_StartDate;
	float								m_InitialStartDelay;
	CGuid								m_ExecutorSlot;
	CGuid								m_FactorySlot;

	const CActionFactory				*m_ParentFactory;
	TAtomic<bool>						m_Finished;

protected:
	CActionPRNG							*m_RandomGenerator;
#if	(PK_HAS_DETERMINISM != 0)
	hh_u32								m_SourceSeed;
#endif

	// for CActionExecutor
	void								SetExecutor(CActionExecutor *executor, float startDelay);	// 'startDelay' may be negative, the action's first 'dt' passed to its 'Run' function will be offsetted accordingly
	void								UnsetExecutor(CActionExecutor *executor);

	virtual void						VirtualTerminate(float dtLeft);// { (void)dtLeft; }
	HH_FORCEINLINE const CActionFactory	*_ParentFactory() const { return m_ParentFactory; }

	CActionInstance(const CActionFactory *parentFactory, const PActionParameters &parameters);

public:
	virtual ~CActionInstance();

	TCallbackChain<void(const PActionInstance &)>	m_DeathNotifier;

	float								StartDate() const { return m_StartDate; }

	HH_FORCEINLINE bool					Alive() const { return !m_Finished; }
	HH_FORCEINLINE bool					Dead() const { return m_Finished; }

	// CouldSkipRun can (or not) be called before Run, if return true, Run could be skipped (for optimisation purposes)
	virtual bool						CouldSkipRun(float dt) { return false; }
	virtual void						Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) = 0;
	virtual void						Terminate(float dtLeft = 0.0f);
	virtual void						Stop(float dtLeft = 0.0f) { Terminate(dtLeft); }

	static PActionInstance				NewInstance(const CActionFactory *parentFactory, const PActionParameters &parameters);
	HH_FORCEINLINE const CActionFactory	*ParentFactory() const { return m_ParentFactory; }	// no need to make this virtual, leave it as is.

	HH_FORCEINLINE CActionExecutor		*Executor() const { return m_Executor; }

//	virtual void						UpdateParameter(CGuid nameGUID, const CAny &newValue) { (void)nameGUID; (void)newValue; }
	float								_GetStartDelayAndReset();
};
HH_DECLARE_REFPTRCLASS(ActionInstance);

//----------------------------------------------------------------------------

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CActionFactory), public CBaseObject
{
private:
	HBO_FIELD(float,						Delay);
	HBO_FIELD(float,						RandomDelay);
	HBO_FIELD(float,						Weight);

	bool									_OnWeightModified();

	friend class CActionInstance;
#ifdef	EU_ACTION_FACTORY_KEEPS_TRACK_OF_CHILDS
	mutable TSlotArray<CActionInstance*>	m_Instances;
	void									_UnregisterInstance(CActionInstance *instance) const;
	Threads::CCriticalSection				m_Lock;
#endif

protected:
	virtual void							VirtualUnlink() override;
	virtual PActionInstance					VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const;
	virtual	void							VirtualSetupInstance(CActionInstance *) const {}

public:
	CActionFactory();
	~CActionFactory();

	PActionInstance	Instantiate(const PActionParameters &parameters = PActionParameters(null), Threads::SThreadContext *threadContext = null) const;
	PActionInstance	InstantiateAndRun(CActionExecutor *whereToRun, const PActionParameters &parameters = PActionParameters(null), float startDelay = 0.0f, Threads::SThreadContext *threadContext = null) const;
//	virtual void	Preload(CParticleMediumCollection *context) {}

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactory);

//----------------------------------------------------------------------------
//
//	Action with a child-list
//
//----------------------------------------------------------------------------

#define	KEEP_ACTION_ALIVE_UNTIL_LAST_CHILD_FINISHES

class	CActionInstanceWithChilds;

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CActionFactoryWithChilds), public CActionFactory
{
private:
	HBO_FIELD(TArray<CActionFactory*>, ChildList);
	HBO_FIELD(hh_u32, RepeatCount);
//	HBO_FIELD(bool, Infinite);

	bool					_OnChildListModified(const _TypeOfChildList &oldChildList);

protected:
	virtual bool			VirtualLink() override;
	virtual PActionInstance	VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const override;

	virtual bool			_VirtualOnChildListModified(const _TypeOfChildList &oldChildList);

public:
	CActionFactoryWithChilds() : HBO_CONSTRUCT(CActionFactoryWithChilds) {}
	~CActionFactoryWithChilds() {}

	mutable TCallbackChain<void(const CActionFactoryWithChilds *parentFactory, const CActionFactoryWithChilds::_TypeOfChildList &oldChildList)>	m_ModificationNotifier;

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactoryWithChilds);

//----------------------------------------------------------------------------

class	CActionInstanceWithChilds : public CActionInstance
{
private:
protected:
	Threads::CCriticalSection	m_ReentryLock;
	bool						m_Executed;
	TArray<PActionInstance>		m_Childs;
	PActionParameters			m_Parameters;
	hh_u32						m_RepeatsLeft;

	CActionInstanceWithChilds(const CActionFactoryWithChilds *parentFactory, const PActionParameters &parameters);

	virtual void			VirtualTerminate(float dtLeft) override;
	virtual bool			_VirtualSetup(Threads::SThreadContext *threadContext);

	void					_CallbackOnChildDeath(const PActionInstance &);
	void					_CallbackOnParentChildListModified(const CActionFactoryWithChilds *parentFactory, const CActionFactoryWithChilds::_TypeOfChildList &oldChildList);

	virtual void			_VirtualCallbackOnChildDeath(const PActionInstance &);
	virtual void			_VirtualCallbackOnParentChildListModified(const CActionFactoryWithChilds *parentFactory, const CActionFactoryWithChilds::_TypeOfChildList &oldChildList);

public:
	~CActionInstanceWithChilds();

	bool					Setup(Threads::SThreadContext *threadContext);

	virtual bool			CouldSkipRun(float dt) override;
	virtual void			Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) override;
	virtual void			Stop(float dtLeft) override;

	TMemoryView<const PActionInstance>	Childs() const { return m_Childs; }

	HH_ACTIONS_DECLARE_INSTANCE(WithChilds);
};
HH_DECLARE_REFPTRCLASS(ActionInstanceWithChilds);

//----------------------------------------------------------------------------
//
//	Sequential Action list
//
//----------------------------------------------------------------------------

class	CActionInstanceWithSequentialChilds;

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CActionFactoryWithSequentialChilds), public CActionFactoryWithChilds
{
private:
protected:
	virtual PActionInstance	VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const override;
public:
	CActionFactoryWithSequentialChilds() : HBO_CONSTRUCT(CActionFactoryWithSequentialChilds) {}
	~CActionFactoryWithSequentialChilds() {}
	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactoryWithSequentialChilds);

//----------------------------------------------------------------------------

class	CActionInstanceWithSequentialChilds : public CActionInstanceWithChilds
{
private:
	hh_u32			m_CurrentChildRunningIndex;

protected:
	CActionInstanceWithSequentialChilds(const CActionFactoryWithSequentialChilds *parentFactory, const PActionParameters &parameters);
	virtual void	VirtualTerminate(float dtLeft) override;
	virtual void	_VirtualCallbackOnChildDeath(const PActionInstance &) override;
	virtual bool	_VirtualSetup(Threads::SThreadContext *threadContext) override;
public:
	~CActionInstanceWithSequentialChilds();
	virtual bool	CouldSkipRun(float dt) override;
	virtual void	Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) override;

	HH_ACTIONS_DECLARE_INSTANCE(WithSequentialChilds);
};
HH_DECLARE_REFPTRCLASS(ActionInstanceWithSequentialChilds);

//----------------------------------------------------------------------------
//
//	Random Action
//
//----------------------------------------------------------------------------

class	CActionInstanceWithRandomChilds;

class	HH_ENGINEUTILS_EXPORT HBO_CLASS(CActionFactoryWithRandomChilds), public CActionFactoryWithChilds
{
private:
#if	(PK_HAS_DETERMINISM == 0)
	mutable CRandomGenerator			*m_TMP_GORE_RandomGenerator;
#endif

protected:
	virtual PActionInstance				VirtualInstantiate(Threads::SThreadContext *threadContext, const PActionParameters &parameters) const override;
	virtual bool						_VirtualOnChildListModified(const CActionFactoryWithChilds::_TypeOfChildList &oldChildList) override;

	CDiscreteProbabilityFunction1D_O1	*m_PDF;

public:
	CActionFactoryWithRandomChilds();
	~CActionFactoryWithRandomChilds();

#if	(PK_HAS_DETERMINISM == 0)
	CRandomGenerator					*TMP_GORE_RandomGenerator() const { return m_TMP_GORE_RandomGenerator; }
#endif

	hh_u32								MapPDF(float r0, float r1) const;	// needs two random numbers

	HBO_CLASS_DECLARATION();
};
HH_DECLARE_REFPTRCLASS(ActionFactoryWithRandomChilds);

//----------------------------------------------------------------------------

class	CActionInstanceWithRandomChilds : public CActionInstanceWithChilds
{
protected:
	CActionInstanceWithRandomChilds(const CActionFactoryWithRandomChilds *parentFactory, const PActionParameters &parameters);

	virtual void			_VirtualCallbackOnParentChildListModified(const CActionFactoryWithChilds *parentFactory, const CActionFactoryWithChilds::_TypeOfChildList &oldChildList) override;
	virtual bool			_VirtualSetup(Threads::SThreadContext *threadContext) override;

public:
	~CActionInstanceWithRandomChilds();
	virtual bool		CouldSkipRun(float dt) override;
	virtual void		Run(Threads::SThreadContext &threadContext, float dt, float dtOffset) override;

	HH_ACTIONS_DECLARE_INSTANCE(WithRandomChilds);
};
HH_DECLARE_REFPTRCLASS(ActionInstanceWithRandomChilds);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_ACTIONS_H__
