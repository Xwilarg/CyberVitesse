#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/09/17 16:13 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PS_STATS_H__
#define __PS_STATS_H__

#include "hh_particles/include/ps_config.h"

#include <hh_kernel/include/kr_threads_basics.h>

#ifndef	HH_RETAIL
#	define	PS_UPDATE_MEDIUM_STATS
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticlesStats
{
private:
public:
	CParticlesStats();
	~CParticlesStats();

	TAtomic<hh_u32>		m_TotalParticleCount_CPU;
	TAtomic<hh_u32>		m_TotalParticleCount_GPU;
	TAtomic<hh_u32>		m_TotalPendingParticleCount;
	TAtomic<hh_u32>		m_TotalNewParticleCount;
	TAtomic<hh_u32>		m_TotalDeadParticleCount;

	hh_u32				TotalParticleCount() const { return m_TotalParticleCount_CPU + m_TotalParticleCount_GPU; }

	TAtomic<hh_u32>		m_TotalMediumUpdates;
	TAtomic<hh_u32>		m_TotalMediumMicroUpdates;
	TAtomic<hh_u32>		m_TotalMediumEvolutionSlices;

	hh_u32				m_TotalParticleRenderMediums;
	hh_u32				m_TotalParticleMediums;
	hh_u32				m_TotalParticleSystems;
	hh_u32				m_TotalParticleSpawners;

	TAtomic<hh_u32>		m_FrameUpdateTasksCount;
	TAtomic<hh_u32>		m_CurrentActiveUpdateTasks;

	hh_u32				m_TotalPageMemory;
	hh_u32				m_TotalUnusedPageMemory;

	// Timings:
	// These are unused. you can compute 'TotalUpdateTimeInMs' by adding together the 'm_UpdateTime*' values
	hh_u64				m_TotalCycleCount;
	float				m_TotalUpdateTimeInMs;

	float				m_UpdateTime_PreTick;
	float				m_UpdateTime_MediumPreUpdate;
	float				m_UpdateTime_RenderMediumPreUpdate;
	float				m_UpdateTime_ActionUpdate;
	float				m_UpdateTime_MediumUpdateKick;
	float				m_UpdateTime_MediumUpdate;
	float				m_UpdateTime_PostTick;
	float				m_UpdateTime_RenderMediumUpdate;

	void				Reset();
};

//----------------------------------------------------------------------------
//
//	Per-medium stats, containing medium-wide stats,
//	as well as timings for all evolve kernels.
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_HAS_STATS != 0)

struct	HH_PARTICLES_EXPORT SEvolveKernelStatsReport
{
	float		m_Time;
	const char	*m_ClassName;

	SEvolveKernelStatsReport() : m_Time(0), m_ClassName(null) {}
	SEvolveKernelStatsReport	&operator += (const SEvolveKernelStatsReport &other);
	void						Clear();
};

//----------------------------------------------------------------------------

struct	HH_PARTICLES_EXPORT SEvolveStatsReport
{
	hh_u32								m_EvolvePassesCount;
	hh_u32								m_ParticlesEvolved;

	float								m_EvolveTime;				// page evolve time. contains 'LifeUpdateTime', 'DeadTime', 'BoundsTime'
	float								m_NewbornTime;				// evolve-newborns time.

	float								m_PreparePageForWriting;
	float								m_LifeUpdateTime;
	float								m_DeadTime;
	float								m_BoundsTime;
	hh_u32								m_Reserved;
	hh_u32								_m_Padd_fix_64bit_aligment; // 

	TArray<SEvolveKernelStatsReport>	m_KernelStats;

	hh_u8								m_Padding[Memory::CacheLineSize - ((4*sizeof(hh_u32) + 6*sizeof(float) + sizeof(TArray<SEvolveKernelStatsReport>)) % Memory::CacheLineSize)];

	SEvolveStatsReport();

	void								Clear();
	SEvolveStatsReport					&operator += (const SEvolveStatsReport &other);
};
HH_STATIC_ASSERT((sizeof(SEvolveStatsReport) % Memory::CacheLineSize) == 0);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CMediumStats
{
protected:
	// CThreadManager::MaxThreadCount
	TArray<SEvolveStatsReport, TArrayCacheAligned>	m_ThreadArray;		// reset on Reset()
	bool											m_Dirty;

public:
	CMediumStats();
	~CMediumStats();

	bool						Setup();

	const SEvolveStatsReport	&ThreadLocalStats(CThreadID threadId) const;
	const SEvolveStatsReport	&CurrentThreadLocalStats() const;
	SEvolveStatsReport			&ThreadLocalStatsForWriting(CThreadID threadId);
	SEvolveStatsReport			&CurrentThreadLocalStatsForWriting();

	void						ComputeGlobalStats(SEvolveStatsReport &outReport) const;
	void						Reset();
};

#endif	// (HH_PARTICLES_HAS_STATS != 0)

//----------------------------------------------------------------------------
__PK_API_END

#endif // __EN_STATS_BASE_H__

