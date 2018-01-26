//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_profiler.h>
#include <hh_kernel/include/kr_profiler_details.h>
#include <hh_base_object/include/hbo_scope.h>
#include <hh_base_object/include/hbo_scope_details.h>

FWD_PK_API_BEGIN
// Might move in PopcornFX Runtime some time
namespace Profiler
{
	class CMultiReport
	{
	public:
		CMultiReport();
		~CMultiReport();

		void						SetReportHistoryCount(hh_u32 reportCount);

		void						Clear();

		void						MergeReport(CProfiler *profiler);

		struct SReportEntry
		{
			CProfilerReport			m_Report;
		};

	private:
		void						ClearReport(hh_u32 i);

		TArray<SReportEntry>		m_Reports;
		hh_u32						m_NextReport;
	};
}
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class CPopcornFXProfiler
{
public:
	CPopcornFXProfiler();
	~CPopcornFXProfiler();

	void			ForceStopAll();

	void			RequestHBOStart(UWorld *world);
	void			RequestHBOStop();

	void			RecordAndSaveProfilerReport(UWorld *world, hh_u32 frameCount, const FString &outputFilePath);

	const PopcornFX::HBOScope::CReport			&HBOReports() const { return m_HBOReports; }
	//const PopcornFX::Profiler::CMultiReport		&ProfilerReports() const { return m_ProfilerReports; }

private:
	void			_OnWorldTick(float dt);
	void			_OnWorldAdded(UWorld *world);
	void			_OnWorldDestroyed(UWorld *world);
	
	void			_End_RecordAndSaveProfilerReport();

	FDelegateHandle						m_WorldAddedHandle;
	FDelegateHandle						m_WorldDestroyedHandle;

	hh_u32			m_TickMaxEveryFrame;
	float			m_TickMaxEveryTime;
	hh_u32			m_CurrentTickFrame;
	float			m_CurrentTickTime;

	hh_u32			m_HBORequestCount;

	hh_i32			m_RecordingFrameCountToSave;
	FString			m_RecordingOutputPath;

	PopcornFX::HBOScope::CReport		m_HBOReports;

	struct SWorldTick
	{
		UWorld				*m_World;
		FDelegateHandle		m_TickHandle;
	};
	TArray<SWorldTick>					m_WorldTicks;

};
