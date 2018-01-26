#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/08/05 17:11 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_EXEC_SCOPE_DETAILS_H__
#define __HBO_EXEC_SCOPE_DETAILS_H__

#include "hh_base_object/include/hbo_scope.h"

#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_containers_hash.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace HBOScope
{

enum {
	kNewThreadDataAligment = 0x80
};

//----------------------------------------------------------------------------

struct SNodeRecord
{
	hh_u64						m_TimingStart;
	hh_u64						m_TimingEnd;
	const SNodeDesctriptor		*m_Descriptor;
	CString						m_BaseObjectPath;

	// Report sutff
	hh_u64						m_ExclusiveTime;

	// explicit noinline to fix vs2012 incorrect code generation when inlining !?
	// (crashed in dtor on invalid pointer in the CString)
	HH_NOINLINE SNodeRecord();
	HH_NOINLINE ~SNodeRecord();

	void						Clear()
	{
		m_TimingStart = 0;
		m_TimingEnd = 0;
		m_Descriptor = null;
		m_BaseObjectPath.Clear();
		m_ExclusiveTime = 0;
	}
};

//----------------------------------------------------------------------------

struct	SNodeRecords
{
	TArray<SNodeRecord>		m_Nodes;

	// Report stuff
	hh_u64			m_TimingStart;
	hh_u64			m_TimingEnd;

	// explicit noinline to fix vs2012 incorrect code generation when inlining !?
	// (crashed in dtor on invalid pointer in the CString)
	HH_NOINLINE SNodeRecords();
	HH_NOINLINE ~SNodeRecords();

	void		Clear()
	{
		m_Nodes.Clear();
		m_TimingStart = 0;
		m_TimingEnd = 0;
	}

	SNodeRecord			*PushBackNode();
};

//----------------------------------------------------------------------------

struct	SThreadNodeRecords
{
	enum { kMaxDepth = 12 };

	volatile bool								m_ReentryGuard;

	hh_u32										m_NodeCount;
	hh_u32										m_MaxDepthCount;
	TStaticArray<SNodeRecords, kMaxDepth>		m_Depths;

	hh_u32										m_ThreadId;

	// Report stuff
	hh_u64			m_TimingStart;
	hh_u64			m_TimingEnd;

	SThreadNodeRecords(hh_u32 threadId = 0)
	:	m_ReentryGuard(false), m_NodeCount(0),  m_MaxDepthCount(0), m_ThreadId(threadId)
	,	m_TimingStart(0), m_TimingEnd(0)
	{
	}

	void			SpinWaitFinish();
	void			Clear();
};

//----------------------------------------------------------------------------

class	CRecord : public CRefCountedObject
{
public:
	enum { kMaxThreads = CThreadManager::MaxThreadCount };

private:
	const Profiler::CProfiler							*m_ProfilerForTimings;

	TStaticArray<SThreadNodeRecords* volatile, kMaxThreads>		m_Threads;

public:
	CRecord();
	~CRecord();

	bool						Setup(const Profiler::CProfiler *profilerForTimings);

	void						Clear();
	void						Swap(CRecord &other);

	HH_FORCEINLINE bool			Valid() const { return m_ProfilerForTimings != null; }

	HH_FORCEINLINE const Profiler::CProfiler	*ProfilerForTimings() const { return m_ProfilerForTimings; }
	HH_FORCEINLINE hh_u32						ThreadCount() const { return m_Threads.Count(); }

	HH_FORCEINLINE SThreadNodeRecords			*RecordThread(hh_u32 threadId)
	{
		HH_ASSERT(Valid());
		if (!HH_VERIFY(threadId < m_Threads.Count()))
			return null;
		if (m_Threads[threadId] == null)
			m_Threads[threadId] = HH_NEW_ALIGNED(SThreadNodeRecords(threadId), kNewThreadDataAligment);
		return m_Threads[threadId];
	}

	HH_FORCEINLINE SThreadNodeRecords			*GetThread(hh_u32 threadId)
	{
		HH_ASSERT(Valid());
		if (!HH_VERIFY(threadId < m_Threads.Count()))
			return null;
		return m_Threads[threadId];
	}

	HH_FORCEINLINE const SThreadNodeRecords		*GetThread(hh_u32 threadId) const
	{
		HH_ASSERT(Valid());
		if (!HH_VERIFY(threadId < m_Threads.Count()))
			return null;
		return m_Threads[threadId];
	}

};
HH_DECLARE_REFPTRCLASS(Record);

//----------------------------------------------------------------------------

class	CContext
{
private:
	bool						m_ActiveStack;
	bool						m_ActiveRecord;

	bool						m_Init;

	const Profiler::CProfiler	*m_ProfilerForTimings;

	CRecord						m_CurrentRecord;
	CRecord						m_PrevRecord;

	struct SNodeStack
	{
		hh_u32					m_NextDepthId;
		TStaticArray<SNodeRecord, SThreadNodeRecords::kMaxDepth>	m_Depths;
		TStaticArray<hh_u64, SThreadNodeRecords::kMaxDepth + 1>		m_Cumuls;

		SNodeStack()
		: m_NextDepthId(0)
		{
			for (hh_u32 i = 0; i < m_Cumuls.Count(); ++i)
				m_Cumuls[i] = 0;
		}
	};

	TStaticArray<SNodeStack*, CRecord::kMaxThreads>		m_CurrentStacks;

	void						_Activate(bool activateStack, bool activateRecord);

public:
	CContext();
	~CContext();

	bool						IsInit() const { HH_ASSERT(m_Init || (!m_ActiveStack && !m_ActiveRecord)); return m_Init; }

	void						SetProfilerForTimings(const Profiler::CProfiler *overrideMainProfilerForTimings);

	HH_INLINE void				Activate(bool activate) { _Activate(true/*stack*/, activate); }
	bool						Active() const { return m_ActiveRecord; }

	void						Reset();

	HH_FORCEINLINE bool			RecordEnabled() const { return m_ActiveRecord; }
	HH_FORCEINLINE bool			StackEnabled() const { return m_ActiveStack; }

	const Profiler::CProfiler	*CurrentProfilerForTimings() const { return m_ProfilerForTimings; }

	TMemoryView<const SNodeRecord>	CurrentStackNowAndHere() const;

	// Available after Reset()
	const CRecord				&PrevRecord() const { return m_PrevRecord; }


	void						RecordStart(SRecordingNodeId &outRecordId, const SNodeDesctriptor *descriptor, const CBaseObject &baseObject);
	void						RecordEnd(const SRecordingNodeId &recordNodeId);
};

//----------------------------------------------------------------------------
//
// Report
// 
//----------------------------------------------------------------------------

struct	SBaseObjectReport_Desc
{
	hh_u64						m_ExclusiveTime;
	const SNodeDesctriptor		*m_Descriptor;

	SBaseObjectReport_Desc()
	: m_ExclusiveTime(0)
	, m_Descriptor(null)
	{
	}
};

struct	SBaseObjectReport
{
	hh_u32			m_NodeCount;
	hh_u64			m_InclusiveTime;
	hh_u64			m_ExclusiveTime;

	TArray<SBaseObjectReport_Desc>		m_Descriptors; // requires ActivateObjectDescriptors

	SBaseObjectReport_Desc		*FindBoDesc(const SNodeDesctriptor *desc);
	SBaseObjectReport_Desc		*FindOrCreateBoDesc(const SNodeDesctriptor *desc);
	void						SortDescsTo(TArray<SBaseObjectReport_Desc> &output) const;

	SBaseObjectReport()
	:	m_NodeCount(0), m_InclusiveTime(0), m_ExclusiveTime(0)
	{ }
};
typedef TArray<TPair<CString, SBaseObjectReport*> >		CBaseObjectReportArray;

struct	SBoReportDescReport
{
	hh_u64						m_ExclusiveTime;
	CString						m_BaseObjectName;
	const SNodeDesctriptor		*m_Descriptor;
};
typedef TArray<SBoReportDescReport>						CBoReportDescReportArray;

class	CReport
{
private:
	const Profiler::CProfiler	*m_ProfilerForTimings;
	bool						m_ReportDescriptors;

	hh_u32						m_TotalNodeCount;
	hh_u64						m_TimingStart;
	hh_u64						m_TimingEnd;

	hh_u64						m_TotalInclusiveTime;
	hh_u64						m_TotalExclusiveTime;

	struct SReport
	{
		hh_u64										m_TimingStart;
		hh_u64										m_TimingEnd;

		TStaticArray<SThreadNodeRecords, CRecord::kMaxThreads>		*m_Threads;

		hh_u32						ThreadCount() const { HH_ASSERT(m_Threads != null); return m_Threads != null ? m_Threads->Count() : 0; }
		SThreadNodeRecords			&Thread(hh_u32 i) { HH_ASSERT(m_Threads != null); return (*m_Threads)[i]; }
		const SThreadNodeRecords	&Thread(hh_u32 i) const { HH_ASSERT(m_Threads != null); return (*m_Threads)[i]; }

		SReport()
		:	m_TimingStart(0), m_TimingEnd(0), m_Threads(null)
		{ m_Threads = HH_NEW(TStaticArray<SThreadNodeRecords COMMA CRecord::kMaxThreads>()); HH_ASSERT(m_Threads != null); }
		SReport(const SReport &other)
		:	m_TimingStart(other.m_TimingStart), m_TimingEnd(other.m_TimingEnd), m_Threads(null)
		{
			if (other.m_Threads != null)
				m_Threads = HH_NEW(TStaticArray<SThreadNodeRecords COMMA CRecord::kMaxThreads>(*other.m_Threads));
			else
				m_Threads = HH_NEW(TStaticArray<SThreadNodeRecords COMMA CRecord::kMaxThreads>());
			HH_ASSERT(m_Threads != null);
		}
		~SReport() { HH_DELETE(m_Threads); }
	};

	//SReport										m_Threads;

	TArray<SReport>									m_Reports;
	hh_u32											m_CurrentReport;


	typedef THashMap<SBaseObjectReport, CString>	CBoReportHashMap;
	CBoReportHashMap								m_BaseObjects;

	void				ClearReport(hh_u32 id);
	void				ClearReportThread(SThreadNodeRecords &thread);
	void				ClearReportDepth(SNodeRecords &depth);
	void				ClearReportNode(SNodeRecord &node);
	hh_u32				NextReport();

	SReport				*FindOldReport(hh_u64 timmingStart);

public:
	CReport();
	~CReport();

	void						SetReportHistoryCount(hh_u32 reportCount);
	hh_u32						ReportHistoryCount() const { return m_Reports.Count(); }

	void						ActivateObjectDescriptors(bool enable);
	bool						ActiveObjectDescriptors() const { return m_ReportDescriptors; }

	void						Clear();
	void						MergeRecord(const CRecord &record);

	bool						Empty() const { return m_TotalNodeCount == 0; }

	hh_u64						TimingStart() const { return m_TimingStart; }
	hh_u64						TimingEnd() const { return m_TimingEnd; }

	double						CyclesToSeconds() const;

	const SNodeRecord			*FindLastOverlapNode(hh_u32 threadId, hh_u64 timingBegin, hh_u64 timingEnd) const;

	const SBaseObjectReport		*FindBaseObjectReport(const CString &objectName) const;

	hh_u32						BaseObjectsCount() const { return m_BaseObjects.Count(); }

	bool						SortBaseObjectsInto(CBaseObjectReportArray &outSortedBaseObjects) const;
	// requires ActivateObjectDescriptors
	bool						SortBoReportDescsInto(CBoReportDescReportArray &outSortedBoReportDesc) const;

	hh_u64						TotalInclusiveTime() const { return m_TotalInclusiveTime; }
	hh_u64						TotalExclusiveTime() const { return m_TotalExclusiveTime; }
};

//----------------------------------------------------------------------------

} // namespace HBOScope

//----------------------------------------------------------------------------
__PK_API_END


#endif // __HBO_EXEC_SCOPE_DETAILS_H__
