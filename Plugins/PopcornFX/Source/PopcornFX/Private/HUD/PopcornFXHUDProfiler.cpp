//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXHUDProfiler.h"

#include "PopcornFXPlugin.h"
#include "Internal/PopcornFXProfiler.h"

#if WITH_EDITOR
#	include "DesktopPlatformModule.h"
#endif
#include "EngineUtils.h"
#include "Engine/Canvas.h"
#include "Debug/DebugDrawService.h"
#include "Engine/Engine.h"

#include "PopcornFXSDK.h"
#include <hh_base_object/include/hbo_scope_details.h>
#include <hh_kernel/include/kr_profiler_details.h>
#include <hh_particles/include/ps_debug.h>
#include <hh_kernel/include/kr_hash.h> // _AutogenColor
#include <hh_kernel/include/kr_sort.h>
#include <hh_kernel/include/kr_containers_array_wrap.h>

#define DEFAULT_PROFILER_FRAME_COUNT		10
#define MAXIMUM_PROFILER_FRAME_COUNT		1000 // Not too much frames

// TODO : What to do with the global lock + Commands lock

//----------------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "LogPopcornFXProfilerHUD"
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXProfilerHUD, Log, All);

//----------------------------------------------------------------------------

class	_CSortBaseObjectFiles_Time
{
public:
	template <typename _TypeIt>
	HH_FORCEINLINE static bool	Less(const _TypeIt &it0, const _TypeIt &it1)
	{
		return (*it0)->m_TotalTime > (*it1)->m_TotalTime;
	}
	template <typename _TypeIt>
	HH_FORCEINLINE static bool	LessOrEqual(const _TypeIt &it0, const _TypeIt &it1)
	{
		return (*it0)->m_TotalTime >= (*it1)->m_TotalTime;
	}
	template <typename _TypeIt>
	HH_FORCEINLINE static bool	Equal(const _TypeIt &it0, const _TypeIt &it1)
	{
		return (*it0)->m_TotalTime == (*it1)->m_TotalTime;
	}
};

class CPopcornFXHUDProfiler_Data
{
public:

	struct SFile
	{
		float					m_TotalTime = 0;
		PopcornFX::CString		m_Path;
		hh_u32					m_DisplayCount;
		PopcornFX::HBOScope::CBaseObjectReportArray		m_BaseObjects;

		void			Setup(const PopcornFX::CString &filePath)
		{
			m_Path = filePath;
			m_TotalTime = 0;
			m_DisplayCount = 0;
			m_BaseObjects.Clear();
		}

		void			Clear()
		{
		}
	};

	PopcornFX::HBOScope::CBaseObjectReportArray			m_AllBaseObjects;
	PopcornFX::TWrapArrayStackReuse< PopcornFX::TArray<SFile> >			m_Files;
	float												m_TotalFilesTime;

	PopcornFX::TArray<const SFile*>						m_SortedFiles;

	bool		Update(const PopcornFX::HBOScope::CReport &hboReports)
	{
		m_TotalFilesTime = 0;
		m_AllBaseObjects.Clear();
		hboReports.SortBaseObjectsInto(m_AllBaseObjects);

		const hh_u32			totalBaseObjectCount = m_AllBaseObjects.Count();

		m_Files.Clear();
		m_SortedFiles.Clear();
		// at most
		if (!HH_VERIFY(m_Files.Reserve(totalBaseObjectCount)) ||
			!HH_VERIFY(m_SortedFiles.Reserve(totalBaseObjectCount)))
			return false;

		for (hh_u32 boi = 0; boi < totalBaseObjectCount; ++boi)
		{
			const PopcornFX::CString		&baseObjectPath = m_AllBaseObjects[boi].First();
			const char						*baseObjectPath_Data = baseObjectPath.Data();
			const char						*baseObjectName = PopcornFX::CBaseObject::StrippedNamePointer(baseObjectPath_Data);
			const hh_u32					filePathLen = baseObjectName > baseObjectPath_Data ? (baseObjectName - baseObjectPath_Data) - 1 : 0;

			SFile				*file = null;
			for (hh_u32 filei = 0; filei < m_Files.Count(); ++filei)
			{
				SFile			&f = m_Files[filei];
				if (f.m_Path.Length() != filePathLen)
					continue;
				if (!PopcornFX::SNativeStringUtils::Equal_16(f.m_Path.Data(), baseObjectPath_Data, filePathLen))
					continue;
				file = &f;
				break;
			}
			if (file == null)
			{
				file = m_Files.PushBackReserved();
				file->Setup(PopcornFX::CString(PopcornFX::CStringContainer::New(baseObjectPath_Data, filePathLen)));
				m_SortedFiles.PushBack(file);
			}

			file->m_BaseObjects.PushBack(m_AllBaseObjects[boi]);
			const float			time = m_AllBaseObjects[boi].Second()->m_ExclusiveTime;
			file->m_TotalTime += time;
			m_TotalFilesTime += time;
		}

		PopcornFX::QuickSort<_CSortBaseObjectFiles_Time >(m_SortedFiles.Begin(), m_SortedFiles.End());

		return true;
	}
};

//----------------------------------------------------------------------------

APopcornFXHUDProfiler::APopcornFXHUDProfiler(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bHidden = false;
	bShowHUD = true;

}

APopcornFXHUDProfiler::~APopcornFXHUDProfiler()
{
	if (!/*HH_VERIFY*/(!m_ProfilerStarted)) // Destroyed() might not have been called !
	{
		CPopcornFXProfiler		*profiler = FPopcornFXPlugin::Get().Profiler();
		if (HH_VERIFY(profiler != null))
		{
			profiler->RequestHBOStop();
			m_ProfilerStarted = false;
		}
	}
	HH_SAFE_DELETE(m_Data);
}

void	APopcornFXHUDProfiler::PostActorCreated()
{
	Super::PostActorCreated();
	if (HH_VERIFY(!m_ProfilerStarted))
	{
		CPopcornFXProfiler		*profiler = FPopcornFXPlugin::Get().Profiler();
		if (profiler != null)
		{
			profiler->RequestHBOStart(GetWorld());
			m_ProfilerStarted = true;
		}
	}
	HH_ASSERT(m_Data == null);
	HH_SAFE_DELETE(m_Data);
	m_Data = HH_NEW(CPopcornFXHUDProfiler_Data);
	check(m_Data != null);
}

void	APopcornFXHUDProfiler::Destroyed()
{
	CPopcornFXProfiler		*profiler = FPopcornFXPlugin::Get().Profiler();
	if (profiler != null && HH_VERIFY(m_ProfilerStarted))
	{
		profiler->RequestHBOStop();
		m_ProfilerStarted = false;
	}
	HH_SAFE_DELETE(m_Data);
	Super::Destroyed();
}

void	APopcornFXHUDProfiler::DrawVBar(float minX, float maxX, float y, float value, float maxValue, float thickness)
{
	float			sizeX = (maxX - minX) * value * SafeRcp(maxValue);
	float			c = PopcornFX::HHMax(0.f, PopcornFX::HHMin(value * SafeRcp(maxValue), 1.f));
	FLinearColor	color(c, 1.f - c, 0.f, 0.6f);
	{
		FCanvasTileItem	tile(
			FVector2D(minX, y),
			FVector2D(sizeX, thickness),
			color
			);
		tile.BlendMode = SE_BLEND_AlphaBlend;
		Canvas->DrawItem(tile);
	}
	{
		color.A = 0.1f;
		FCanvasTileItem	tile(
			FVector2D(minX + sizeX, y),
			FVector2D((maxX - minX) - sizeX, thickness),
			color
			);
		tile.BlendMode = SE_BLEND_AlphaBlend;
		Canvas->DrawItem(tile);
	}
}

void	APopcornFXHUDProfiler::DrawVBarPercCol(float minX, float maxX, float y, float perc, float percForColor, float thickness)
{
	float			sizeX = (maxX - minX) * perc * 0.01f;
	float			c = PopcornFX::HHMax(0.f, PopcornFX::HHMin(percForColor * 0.01f, 1.f));
	FLinearColor	color(c, 1.f - c, 0.f, 0.6f);
	{
		FCanvasTileItem	tile(
			FVector2D(minX, y),
			FVector2D(sizeX, thickness),
			color
		);
		tile.BlendMode = SE_BLEND_AlphaBlend;
		Canvas->DrawItem(tile);
	}
	{
		color.A = 0.1f;
		FCanvasTileItem	tile(
			FVector2D(minX + sizeX, y),
			FVector2D((maxX - minX) - sizeX, thickness),
			color
		);
		tile.BlendMode = SE_BLEND_AlphaBlend;
		Canvas->DrawItem(tile);
	}
}

static PopcornFX::CUbyte3		_AutogenColorU8(const char *name, hh_u32 len)
{
	const hh_u32	key = PopcornFX::CRawHasher_Hsieh::Hash(name, len);
	enum : hh_u32 {
		kRBits = 11U,
		kGBits = 11U,
		kBBits = 10U,

		kRMask = (1U << kRBits) - 1U,
		kGMask = (1U << kGBits) - 1U,
		kBMask = (1U << kBBits) - 1U,

		kRScale = kRMask * 8U / 10U,
		kGScale = kGMask * 8U / 10U,
		kBScale = kBMask * 8U / 10U,

		kRBase8 = 0xFFU - (kRMask - kRScale) / kRMask,
		kGBase8 = 0xFFU - (kGMask - kGScale) / kGMask,
		kBBase8 = 0xFFU - (kBMask - kBScale) / kBMask,

	};
	const PopcornFX::CUbyte3	color(
		(kRBase8 + (kRMask & (key >> (0					))) * kRScale * 0xFFU / kRMask),
		(kGBase8 + (kGMask & (key >> (kRBits			))) * kGScale * 0xFFU / kGMask),
		(kBBase8 + (kBMask & (key >> (kRBits + kGBits	))) * kBScale * 0xFFU / kBMask)
	);
	return color;
}

static PopcornFX::CFloat3		_AutogenColorF(const char *name, hh_u32 len)
{
	const hh_u32	key = PopcornFX::CRawHasher_Hsieh::Hash(name, len);
	enum : hh_u32 {
		kRBits = 11U,
		kGBits = 11U,
		kBBits = 10U,

		kRMask = (1U << kRBits) - 1U,
		kGMask = (1U << kGBits) - 1U,
		kBMask = (1U << kBBits) - 1U,
	};
	static const float		kBase = 0.f;
	static const float		kRScale = (1.0f - kBase) / float(kRMask);
	static const float		kGScale = (1.0f - kBase) / float(kGMask);
	static const float		kBScale = (1.0f - kBase) / float(kBMask);

	const PopcornFX::CFloat3	color(
		(kBase + float(kRMask & (key >> (0					))) * kRScale),
		(kBase + float(kGMask & (key >> (kRBits				))) * kGScale),
		(kBase + float(kBMask & (key >> (kRBits + kGBits	))) * kBScale)
	);
	return color;
}

//#define PROFILE_PRHUD

#ifdef PROFILE_PRHUD
#	define	IF_PROFILE_SERIALIZER(...)				__VA_ARGS__
#	define	PROFILE_PRHUD_DECLARE				HH_PROFILESTATS_DECLARE
#	define	PROFILE_PRHUD_CAPTURE				HH_PROFILESTATS_CAPTURE
#	define	PROFILE_PRHUD_CAPTURE_N				HH_PROFILESTATS_CAPTURE_N
#	define	PROFILE_PRHUD_CAPTURE_CYCLES		HH_PROFILESTATS_CAPTURE_CYCLES
#	define	PROFILE_PRHUD_CAPTURE_CYCLES_N		HH_PROFILESTATS_CAPTURE_CYCLES_N
#else
#	define	IF_PROFILE_SERIALIZER(...)
#	define	PROFILE_PRHUD_DECLARE(...)
#	define	PROFILE_PRHUD_CAPTURE(...)
#	define	PROFILE_PRHUD_CAPTURE_N(...)
#	define	PROFILE_PRHUD_CAPTURE_CYCLES(...)
#	define	PROFILE_PRHUD_CAPTURE_CYCLES_N(...)
#endif

void	APopcornFXHUDProfiler::DrawDebugHUD(UCanvas* InCanvas, APlayerController* PC)
{
	HH_NAMEDSCOPEDPROFILE_C("APopcornFXHUDProfiler", POPCORNFX_UE_PROFILER_COLOR);

	PROFILE_PRHUD_DECLARE(
		100,
		All,
		Update,
		Draw,
		Draw_File,
		Draw_BO,
		Draw_BO_Pretty,
		Draw_BO_Text
	);
	PROFILE_PRHUD_CAPTURE(All);

	Canvas = InCanvas;
	if (!Canvas)
		return;

	Canvas->SetDrawColor(255, 255, 255, 255);

	UFont				*font = GEngine->GetSmallFont();
	FFontRenderInfo		fri = Canvas->CreateFontRenderInfo(true, true);
	FFontRenderInfo		friNoShadow = Canvas->CreateFontRenderInfo(true, false);

	FString		title = TEXT("PopcornFX BaseObject Scope Profiler");
	float _w, _h;
	Canvas->StrLen(font, title, _w, _h);

	const UPopcornFXSettings	*settings = FPopcornFXPlugin::Get().Settings();
	check(settings);
	const float		screenRatio = settings->HUD_DisplayScreenRatio;
	const float		hideBelowPerc = settings->HUD_HideNodesBelowPercent;

	const float		lineh = _h;
	float			y = Canvas->SizeY * screenRatio;
	float			veryMaxY = Canvas->SizeY * (1.f - screenRatio);
	float			x = y;

	Canvas->DrawText(font, title, x, y, 1.f, 1.f, fri);
	y += lineh;

	CPopcornFXProfiler		*profiler = FPopcornFXPlugin::Get().Profiler();
	if (profiler == null)
		return;

	const PopcornFX::HBOScope::CReport				&hboReports = profiler->HBOReports();

	const hh_u32			subDescPerObject = settings->ProfilerHUD_SubDescriptorsPerObject;
	// will be effective next Merge
	const_cast<PopcornFX::HBOScope::CReport&>(hboReports).ActivateObjectDescriptors(subDescPerObject > 0);

	CPopcornFXHUDProfiler_Data			&data = *m_Data;

	{
		HH_NAMEDSCOPEDPROFILE_C("APopcornFXHUDProfiler Update", POPCORNFX_UE_PROFILER_COLOR);
		PROFILE_PRHUD_CAPTURE(Update);

		if (!data.Update(hboReports))
			return;
	}

	HH_NAMEDSCOPEDPROFILE_C("APopcornFXHUDProfiler Draw", POPCORNFX_UE_PROFILER_COLOR);
	PROFILE_PRHUD_CAPTURE(Draw);

	PopcornFX::TArray<PopcornFX::HBOScope::SBaseObjectReport_Desc>		descsCache;

	{
		PopcornFX::CString	prettyName;

		const double		ctos = hboReports.CyclesToSeconds();
		const double		totalExc = hboReports.TotalExclusiveTime();
		const float			totalTimeSec = (hboReports.TimingEnd() - hboReports.TimingStart()) * ctos;
		const float			totalExcSec = totalExc * ctos;

		Canvas->DrawText(font,
			TEXT("Displays % of total profiled CPU time (% of parent, color gradiant of total time)"),
			x, y, 1.f, 1.f, fri);
		y += lineh;

		Canvas->DrawText(font,
			FString::Printf(TEXT("%3d Files, %3d BaseObjects, over the last %5.2fs, profiled %5.2fs over all threads"),
				data.m_SortedFiles.Count(),
				data.m_AllBaseObjects.Count(),
				//hboReports.ReportHistoryCount(),
				totalTimeSec,
				totalExcSec
			),
			x, y, 1.f, 1.f, fri);
		y += lineh;

		const float			colExc = x + 0;
		const float			colPath = colExc + 60;

		const float			colBoOff = 30; // BaseObjects indent
		const float			colBoDescOff = 60; // BaseObjects indent

		Canvas->DrawText(font,
			TEXT("%CPU"),
			colExc, y, 1.f, 1.f, fri);
		Canvas->DrawText(font,
			TEXT("File > BaseObject > Profile slot"),
			colPath, y, 1.f, 1.f, fri);
		y += lineh;

		const double		totalTime = data.m_TotalFilesTime;
		double				displayedFiles = 0;
		double				displayedBos = 0;

		const hh_u32		maxBoPerFile = FPopcornFXPlugin::Get().Settings()->ProfilerHUD_BaseObjectsPerFile;

		const float			maxBoY = veryMaxY - lineh * 2; // 2 last total lines

		for (hh_u32 filei = 0; filei < data.m_SortedFiles.Count(); ++filei)
		{
			if (filei > 0 && maxBoPerFile > 0)
				y += lineh / 2;

			const auto		&file = *data.m_SortedFiles[filei];

			const float		filePerc = 100.0 * (file.m_TotalTime / totalTime);
			if (filePerc < hideBelowPerc)
				break;

			const auto		tfilepath = ANSI_TO_TCHAR(file.m_Path.Data());

			{
				PROFILE_PRHUD_CAPTURE(Draw_File);

				const int32		size = font->GetStringSize(tfilepath);
				const PopcornFX::CFloat3		c = _AutogenColorF(file.m_Path.Data(), file.m_Path.Length());
				FLinearColor	color(c.x(), c.y(), c.z(), 0.6f);
				FCanvasTileItem	tile(
					FVector2D(colPath, y),
					FVector2D(size, lineh),
					color
					);
				tile.BlendMode = SE_BLEND_AlphaBlend;
				Canvas->DrawItem(tile);

				const float			perc = filePerc;
				displayedFiles += file.m_TotalTime;

				DrawVBar(colExc, colPath - 1, y, perc, 100.f, lineh);

				Canvas->DrawText(font,
					FString::Printf(TEXT("%5.1f"), perc),
					colExc, y, 1.f, 1.f, friNoShadow);

				Canvas->SetDrawColor(0, 0, 0, 255);
				Canvas->DrawText(font,
					FString::Printf(TEXT("%s (%d BOs)"), tfilepath, file.m_BaseObjects.Count()),
					colPath, y, 1.f, 1.f, friNoShadow);

				Canvas->SetDrawColor(255, 255, 255, 255);

				y += lineh;
				if (y > maxBoY)
					break;
			}

			const hh_u32	boCount = PopcornFX::HHMin(file.m_BaseObjects.Count(), maxBoPerFile);
			for (hh_u32 boi = 0; boi < boCount; ++boi)
			{
				PROFILE_PRHUD_CAPTURE(Draw_BO);
				
				const PopcornFX::CString							&baseObjectPath = file.m_BaseObjects[boi].First();
				const PopcornFX::HBOScope::SBaseObjectReport		&bo = *(file.m_BaseObjects[boi].Second());

				const float			boexcperc = 100.0 * bo.m_ExclusiveTime / file.m_TotalTime;
				if (boexcperc < hideBelowPerc)
					break;
				const float			boexcpercTotal = 100.0 * bo.m_ExclusiveTime / totalTime;

				{
					PROFILE_PRHUD_CAPTURE(Draw_BO_Pretty);
					PopcornFX::BuildParticleObjectTreeViewName(prettyName, baseObjectPath, false);
				}

				const hh_u32		totalDescCount = bo.m_Descriptors.Count();

				{
					displayedBos += bo.m_ExclusiveTime;

					DrawVBarPercCol(colBoOff + colExc, colBoOff + colPath - 1, y, boexcperc, boexcpercTotal, lineh);

					{
						Canvas->DrawText(font,
							FString::Printf(TEXT("%5.1f"), boexcperc),
							colBoOff + colExc, y, 1.f, 1.f, friNoShadow);

						PROFILE_PRHUD_CAPTURE(Draw_BO_Text);
						if (totalDescCount == 1)
						{
							FString			name = ANSI_TO_TCHAR(prettyName.Data());
							name.Append(TEXT(" ("), 2);
							name.Append(ANSI_TO_TCHAR(bo.m_Descriptors[0].m_Descriptor->m_Name));
							name.Append(TEXT(")"), 1);
							Canvas->DrawText(font,
								*name,
								colBoOff + colPath, y, 1.f, 1.f, fri);
						}
						else
						{
							Canvas->DrawText(font,
								ANSI_TO_TCHAR(prettyName.Data()),
								colBoOff + colPath, y, 1.f, 1.f, fri);
						}
					}
					y += lineh;
					if (y > maxBoY)
						break;
				}

				if (subDescPerObject > 0 && totalDescCount > 1)
				{
					bo.SortDescsTo(descsCache);
					const hh_u32		descCount = PopcornFX::HHMin(subDescPerObject, descsCache.Count());
					for (hh_u32 desci = 0; desci < descCount; ++desci)
					{
						const PopcornFX::HBOScope::SBaseObjectReport_Desc		&desc = descsCache[desci];
						const float			excperc = 100.0 * desc.m_ExclusiveTime / bo.m_ExclusiveTime;
						if (excperc < hideBelowPerc)
							break;
						const float			excpercTotal = 100.0 * desc.m_ExclusiveTime / totalTime;

						//DrawVBar(colBoDescOff + colExc, colBoDescOff + colPath - 1, y, excperc, 100.f, lineh);
						DrawVBarPercCol(colBoDescOff + colExc, colBoDescOff + colPath - 1, y, excperc, excpercTotal, lineh);

						Canvas->DrawText(font,
							FString::Printf(TEXT("%5.1f"), excperc),
							colBoDescOff + colExc, y, 1.f, 1.f, friNoShadow);

						Canvas->DrawText(font,
							ANSI_TO_TCHAR(desc.m_Descriptor->m_Name),
							colBoDescOff + colPath, y, 1.f, 1.f, fri);

						y += lineh;
						if (y > maxBoY)
							break;

					} // for descs
				}

				if (y > maxBoY)
					break;

			} // for bos

			if (y > maxBoY)
				break;
		} // for files

		{
			y += lineh / 2;

			const float			percfile = 100.f * displayedFiles / totalTime;
			const float			percbo = 100.f * displayedBos / totalTime;

			DrawVBar(colExc, colPath - 1, y, percfile, 100.f, lineh);
			Canvas->DrawText(font,
				FString::Printf(TEXT("%5.1f"), percfile),
				colExc, y, 1.f, 1.f, friNoShadow);
			Canvas->DrawText(font,
				TEXT("Files timings displayed here"),
				colPath, y, 1.f, 1.f, fri);
			y += lineh;

			//DrawVBar(colBoOff + colExc, colBoOff + colPath - 1, y, percbo, 100.f, lineh);
			//Canvas->DrawText(font,
			//	FString::Printf(TEXT("%5.1f"), percbo),
			//	colBoOff + colExc, y, 1.f, 1.f, friNoShadow);
			//Canvas->DrawText(font,
			//	TEXT("BaseObjects timings displayed"),
			//	colBoOff + colPath, y, 1.f, 1.f, fri);
			//y += lineh;
		}

	}

}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

static void		_PopcornFXToggleProfilerHUD(const TArray<FString>& args, UWorld *InWorld)
{
	if (!InWorld)
	{
		UE_LOG(LogPopcornFXProfilerHUD, Warning, TEXT("ToggleProfilerHUD: no World"));
		return;
	}
	bool		hasForce = false;
	bool		forceOn = false;
	if (args.Num() >= 1)
	{
		hasForce = true;
		if (args[0] == "1")
			forceOn = true;
	}

	static FDelegateHandle	s_drawDebugDelegateHandle;
	APopcornFXHUDProfiler		*HUD = NULL;
	for (TActorIterator<APopcornFXHUDProfiler> It(InWorld); It; ++It)
	{
		HUD = *It;
		break;
	}
	if (hasForce && forceOn == (HUD != null))
		return;
	if (HUD == null)
	{
		HUD = InWorld->SpawnActor<APopcornFXHUDProfiler>();
		FDebugDrawDelegate DrawDebugDelegate = FDebugDrawDelegate::CreateUObject(HUD, &APopcornFXHUDProfiler::DrawDebugHUD);
		s_drawDebugDelegateHandle = UDebugDrawService::Register(TEXT("OnScreenDebug"), DrawDebugDelegate);
		UE_LOG(LogPopcornFXProfilerHUD, Log, TEXT("ToggleProfilerHUD: HUD added to %s"), *InWorld->GetFullName());
	}
	else
	{
		UDebugDrawService::Unregister(s_drawDebugDelegateHandle);
		HUD->Destroy();
		UE_LOG(LogPopcornFXProfilerHUD, Log, TEXT("ToggleProfilerHUD: HUD removed from %s"), *InWorld->GetFullName());
	}
}

FAutoConsoleCommandWithWorldAndArgs		PopcornFXHUDProfilerCommand(
	TEXT("PopcornFX.ToggleProfilerHUD"),
	TEXT("ToggleProfilerHUD [0|1]"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(_PopcornFXToggleProfilerHUD)
	);

#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
