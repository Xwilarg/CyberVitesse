#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/15 23:09 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_PROFILER_DETAILS_H__
#define	__KR_PROFILER_DETAILS_H__

#include "hh_kernel/include/kr_profiler.h"
#include "hh_kernel/include/kr_threads_atomic.h"
#include "hh_kernel/include/kr_threads_basics.h"
#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_timers.h"
#include "hh_kernel/include/kr_callstack.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Profiler
{
	class	CProfiler;
	class	CProfilerReport;

	struct	HH_KERNEL_EXPORT SThreadProfileRecordEntry
	{
		enum
		{
#if	(HH_KERNEL_BUILD_CALLSTACKS != 0)
#	if	defined(HH_CPU_32BITS)
			CallstackOffset = 20,
			TargetSizeInBytes = 64,
#	else
			CallstackOffset = 24,
			TargetSizeInBytes = 128,
#	endif

			CallstackDepth = (TargetSizeInBytes - CallstackOffset) / sizeof(hh_ureg)
#else	// (HH_KERNEL_BUILD_CALLSTACKS == 0)
#	if	defined(HH_CPU_32BITS)
			// we've got 4 extra padding bytes due to the u64
			TargetSizeInBytes = 24,//20,//16,
#	else
			TargetSizeInBytes = 24,
#	endif
#endif
		};

		enum	ETransactionID
		{
			Transaction_Free = 0,
			Transaction_Alloc = 1,
		};
																	// 32 bits			64 bits
		hh_u64								m_TimingBegin;			// 8		@ 0		8		@ 0
		hh_u32								m_TimingDuration;		// 4		@ 8		4		@ 8
		union
		{
			hh_u16							m_MemoryTransaction[2];
			hh_u32							m_PackedMemoryTransaction;
		};															// 4		@ 12	4		@ 12
		const SNodeDescriptor				*m_Descriptor;			// 4		@ 16	8		@ 16
#if	(HH_KERNEL_BUILD_CALLSTACKS != 0)
		TSmallCallstack<CallstackDepth>		m_Callstack;			// 4 * 11	@ 24	8 * 13	@ 24	win32/x360: 
#elif	defined(HH_CPU_32BITS)
		hh_u32								m_Padding;				// 4
#endif

		SThreadProfileRecordEntry(const SNodeDescriptor *descriptor)
		:	m_TimingBegin(0)
		,	m_TimingDuration(0)
		,	m_PackedMemoryTransaction(0)
		,	m_Descriptor(descriptor)
		{
		}

		SThreadProfileRecordEntry()
		:	m_TimingBegin(0)
		,	m_TimingDuration(0)
		,	m_PackedMemoryTransaction(0)
		,	m_Descriptor(null)
		{
		}

		hh_u64			TimingBeginInCycles(const CProfilerReport &report) const;
		hh_u64			TimingEndInCycles(const CProfilerReport &report) const;
		hh_u32			TimingDurationInCycles(const CProfilerReport &report) const;

		double			TimingBeginInSeconds(const CProfilerReport &report) const;
		double			TimingEndInSeconds(const CProfilerReport &report) const;
		float			TimingDurationInSeconds(const CProfilerReport &report) const;

		static hh_u32	MemoryTransactionDecode(hh_u16 code);
		static hh_u16	MemoryTransactionEncode(hh_u32 bytes);

		hh_u32			MemoryTransactionRead(ETransactionID id) const;
		void			MemoryTransactionWrite(ETransactionID id, hh_u32 bytes);

		void			SetEndTime(hh_u64 endTime);
	};

	HH_STATIC_ASSERT(sizeof(SThreadProfileRecordEntry) == SThreadProfileRecordEntry::TargetSizeInBytes);

	//----------------------------------------------------------------------------

	struct	HH_KERNEL_EXPORT SThreadProfileRecord
	{
		typedef TArray<SThreadProfileRecordEntry, TArrayAligned16>	RecordList;

		TArray<RecordList>		m_Nodes;
		hh_u32					m_CurrentProfileDepth;
		volatile bool			m_ReentryGuard;

		hh_u32					m_MemoryTransaction[2];

		SThreadProfileRecord();
		void					Reset(hh_u64 resetTime);
	};

	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CProfilerReport
	{
	public:
		CProfilerReport() : m_CyclesStart(0), m_CyclesToSeconds(0) {}
		~CProfilerReport() {}

		typedef TArray<SThreadProfileRecordEntry, TArrayAligned16>	RecordList;

		struct	SThreadProfilerReport
		{
			TArray<RecordList>			m_Records;
			hh_u32						m_MemoryTransaction[2];

			SThreadProfilerReport()
			{
				m_MemoryTransaction[0] = 0;
				m_MemoryTransaction[1] = 0;
			}
		};

		hh_u64							m_CyclesStart;
		double							m_CyclesToSeconds;
		TArray<SThreadProfilerReport>	m_Nodes;

		void							Clear();

		void							Swap(CProfilerReport &other);
	};

	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CSerializedNodeDescriptors
	{
	private:
		TArray<SNodeDescriptor*>		m_Descriptors;
		TArray<CString>					m_Names;

	public:
		~CSerializedNodeDescriptors();

		void							Clear();
		TArray<CString>					&Names() { return m_Names; };
		TArray<SNodeDescriptor*>		&Descriptors() { return m_Descriptors; };
	};

	//----------------------------------------------------------------------------

	HH_KERNEL_EXPORT bool				WriteProfileReport(const CProfilerReport &report, const CString &filePath);
	HH_KERNEL_EXPORT bool				WriteProfileReport(const CProfilerReport &report, CStream &outputStream);
	HH_KERNEL_EXPORT bool				ReadProfileReport(const CString &filePath, CProfilerReport &outReport, CSerializedNodeDescriptors &outDescriptors);
	HH_KERNEL_EXPORT bool				ReadProfileReport(CStreamReadOnly &inputStream, CProfilerReport &outReport, CSerializedNodeDescriptors &outDescriptors);

	//----------------------------------------------------------------------------

	typedef bool						(*FnRecordEventStart)(void *arg, const SNodeDescriptor *nodeDescriptor);
	typedef void						(*FnRecordEventEnd)(void *arg);
	typedef void						(*FnRecordMemoryTransaction)(void *arg, hh_i32 bytes);
	typedef void						(*FnRecordThreadDependency)(void *arg, CThreadID other, hh_u32 dFlags);

	//----------------------------------------------------------------------------

	struct	STaskGroupInfo
	{
		CFloat4		m_Color;
		CString		m_Name;
	};

	//----------------------------------------------------------------------------

	class	HH_KERNEL_EXPORT CProfiler
	{
	public:
#if	(KR_PROFILER_ENABLED != 0)
		Threads::CCriticalSection			m_RecordsLock;
#endif

	private:
		bool								m_Active;
		bool								m_GrabCallstacks;
		CProfilerReport						m_LastReport;

		TArray<STaskGroupInfo>				m_TaskGroupInfos;

#if	(KR_PROFILER_ENABLED != 0)
		TStaticArray<SThreadProfileRecord*, CThreadManager::MaxThreadCount>		m_ThreadProfileRecords;
		TStaticArray<SThreadProfileRecord*, CThreadManager::MaxThreadCount>		m_ThreadProfileRecordsPrev;
		SThreadProfileRecord				*_GetThreadProfileRecord(CThreadID threadId);
#endif

		CTimer								m_Timer;
		CTimerCPUCycles						m_TimerCycles;
		hh_u64								m_CyclesStart;
		hh_u64								m_CyclesStartPrev;
		double								m_CyclesToSeconds;

		HH_FORCEINLINE double				_ReadCurrentTime() const { return m_Timer.Read(); }
		HH_FORCEINLINE hh_u64				_ReadCurrentCycles() const { return m_TimerCycles.Read(); }

	protected:
#if	(KR_PROFILER_ENABLED != 0)
		static bool							_RecordEventStart_Impl(void *arg, const SNodeDescriptor *nodeDescriptor);
		static void							_RecordEventEnd_Impl(void *arg);
		static void							_RecordMemoryTransaction_Impl(void *arg, hh_i32 bytes);
		static void							_RecordThreadDependency_Impl(void *arg, CThreadID other, hh_u32 dFlags);

		void								*m_RecordArg;
		FnRecordEventStart					m_FnRecordEventStart;
		FnRecordEventEnd					m_FnRecordEventEnd;
		FnRecordMemoryTransaction			m_FnRecordMemoryTransaction;
		FnRecordThreadDependency			m_FnRecordThreadDependency;
#endif

	public:
		CProfiler();
		~CProfiler();

		CGuid								RegisterTaskGroup(const CString &name, const CFloat4 &color);	// not thread-safe !
		hh_u32								TaskGroupsCount() const { return m_TaskGroupInfos.Count(); }	// not thread-safe !
		const STaskGroupInfo				&GetTaskGroupInfo(CGuid id) const { if (hh_u32(id) < m_TaskGroupInfos.Count()) return m_TaskGroupInfos[id]; return m_TaskGroupInfos[0]; }	// not thread-safe !

		void								Reset();
		void								BuildReport(CProfilerReport *outputReport = null);
		const CProfilerReport				&LatestReport() const { return m_LastReport; }
		void								StealSwapReport(CProfilerReport &swapWith) { m_LastReport.Swap(swapWith); }

		void								GrabCallstacks(bool grabCallstacks);
		void								Activate(bool active);
		bool								Active() const { return m_Active; }

		bool								SetRecorder(void						*recordArg,
														FnRecordEventStart			recEventStart,
														FnRecordEventEnd			recEventEnd,
														FnRecordMemoryTransaction	recMemTransaction,
														FnRecordThreadDependency	eventStart);

#if	(KR_PROFILER_ENABLED != 0)
		HH_FORCEINLINE bool					RecordEventStart(const SNodeDescriptor *nodeDescriptor) { return m_FnRecordEventStart(m_RecordArg, nodeDescriptor); }
		HH_FORCEINLINE void					RecordEventEnd() { m_FnRecordEventEnd(m_RecordArg); }
		HH_FORCEINLINE void					RecordMemoryTransaction(hh_i32 bytes) { m_FnRecordMemoryTransaction(m_RecordArg, bytes); }
		HH_FORCEINLINE void					RecordThreadDependency(CThreadID other, hh_u32 dFlags) { m_FnRecordThreadDependency(m_RecordArg, other, dFlags); }
#else
		HH_FORCEINLINE bool					RecordEventStart(const SNodeDescriptor *nodeDescriptor) { return false; }
		HH_FORCEINLINE void					RecordEventEnd() {}
		HH_FORCEINLINE void					RecordMemoryTransaction(hh_i32 bytes) {}
		HH_FORCEINLINE void					RecordThreadDependency(CThreadID other, hh_u32 dFlags) {}
#endif

		hh_u64								CyclesStart() const { return m_CyclesStart; }
		double								CyclesToSeconds() const { return m_CyclesToSeconds; };

		HH_FORCEINLINE hh_u64				ReadCurrentCycles() const { return _ReadCurrentCycles(); }
	};

}

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_PROFILER_DETAILS_H__
