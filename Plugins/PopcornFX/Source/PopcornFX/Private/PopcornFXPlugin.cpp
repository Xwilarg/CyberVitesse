//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXPlugin.h"

#include "Assets/PopcornFXFile.h"
#include "PopcornFXSettingsEditor.h"
#include "Internal/Startup.h"
#include "Internal/PopcornFXProfiler.h"
#include "PopcornFXVersionGenerated.h"
#if WITH_EDITOR
#	include "Editor/PopcornFXEffectEditor.h"
#endif

#include "Engine/Texture.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#if WITH_EDITOR
#	include "IPluginManager.h"
#endif

#include "PopcornFXSDK.h"
#include <hh_particles/include/ps_system.h>
#include <hh_toolkit/include/hh_toolkit_version.h>
#include <hh_kernel/include/kr_resources.h>

//----------------------------------------------------------------------------

IMPLEMENT_MODULE(FPopcornFXPlugin, PopcornFX)

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXPlugin, Log, All);
#define LOCTEXT_NAMESPACE "PopcornFXPlugin"

//----------------------------------------------------------------------------

FPopcornFXPlugin				*FPopcornFXPlugin::s_Self = null;
float							FPopcornFXPlugin::s_GlobalScale = 1.f;
float							FPopcornFXPlugin::s_GlobalScaleRcp = 1.f;
FString							FPopcornFXPlugin::s_PluginVersionString;
FString							FPopcornFXPlugin::s_PopornFXRuntimeVersionString;
uint32							FPopcornFXPlugin::s_PopornFXRuntimeRevID = 0;
uint16							FPopcornFXPlugin::s_PopornFXRuntimeMajMinPatch = 0;

FString							FPopcornFXPlugin::s_URLDocumentation = "http://wiki.popcornfx.com/";
FString							FPopcornFXPlugin::s_URLPluginWiki = "http://wiki.popcornfx.com/index.php/UE4";
FString							FPopcornFXPlugin::s_URLAnswerHub = "http://answers.popcornfx.com/";

namespace
{
	hh_i32			g_RenderThreadId = -1;
	FString			g_PopcornFXPluginRoot;
	FString			g_PopcornFXPluginContentDir;

} // namespace

namespace
{
	PopcornFX::TAtomic<hh_i32>	s_TotalParticleCount = 0;
}

//----------------------------------------------------------------------------

//static
PopcornFX::SEngineVersion		FPopcornFXPlugin::PopcornFXBuildVersion()
{
	PopcornFX::SDllVersion		version; // ctor default to build version
	return PopcornFX::SEngineVersion(version.Major, version.Minor, version.Patch, version.RevID);
}

//----------------------------------------------------------------------------

// static
int32	FPopcornFXPlugin::TotalParticleCount()
{
	hh_i32		c = s_TotalParticleCount;
	HH_ASSERT(c >= 0);
	return c;
}

//----------------------------------------------------------------------------

// static
void	FPopcornFXPlugin::IncTotalParticleCount(hh_i32 incCount)
{
	s_TotalParticleCount += incCount;
	HH_ASSERT(s_TotalParticleCount >= 0);
}

//----------------------------------------------------------------------------

// static
void	FPopcornFXPlugin::RegisterRenderThreadIFN()
{
	PopcornFX::CThreadID		renderThreadId = PopcornFX::CCurrentThread::ThreadID();
	if (!renderThreadId.Valid())
	{
		//HH_ASSERT(g_RenderThreadId < 0);
		if (g_RenderThreadId >= 0)
		{
			PopcornFX::CThreadManager::UnsafeUnregisterUserThread(g_RenderThreadId);
			g_RenderThreadId = -1;
		}
		HH_ASSERT(g_RenderThreadId < 0);
		renderThreadId = PopcornFX::CCurrentThread::RegisterUserThread();
		if (!HH_VERIFY(renderThreadId.Valid()))
			return;
		g_RenderThreadId = renderThreadId;
	}
	HH_ASSERT(g_RenderThreadId >= 0);
	if (!HH_VERIFY(hh_u32(g_RenderThreadId) == hh_u32(renderThreadId)))
		g_RenderThreadId = renderThreadId; // force anyway
}

//----------------------------------------------------------------------------

static PopcornFX::TAtomic<hh_u32>		g_RegisteredUserThreadsCount = 0;
static hh_u32							g_RegisteredUserThreads[PopcornFX::CThreadManager::MaxThreadCount] = {0};

void	FPopcornFXPlugin::RegisterCurrentThreadAsUserIFN()
{
	if (HH_PREDICT_LIKELY(PopcornFX::CCurrentThread::ThreadID().Valid()))
		return;
	PopcornFX::CThreadID	tid = PopcornFX::CCurrentThread::RegisterUserThread();
	if (!HH_VERIFY(tid.Valid()))
		return;
	const hh_u32	i = g_RegisteredUserThreadsCount++;
	const hh_u32	maxUserThreads = hh_u32(HH_ARRAY_COUNT(g_RegisteredUserThreads));
	if (!HH_VERIFY_MESSAGE(i < maxUserThreads, "Exeeded the maximum of %d user threads", maxUserThreads))
		return;
	HH_ASSERT(tid > 0); // 0 is main thread
	g_RegisteredUserThreads[i] = hh_u32(tid);
}

//----------------------------------------------------------------------------

// static
bool	FPopcornFXPlugin::IsMainThread()
{
	return PopcornFX::CCurrentThread::IsMainThread();
}

//----------------------------------------------------------------------------

#if WITH_EDITOR
//static
const FString	&FPopcornFXPlugin::PopcornFXPluginRoot()
{
	check(!g_PopcornFXPluginRoot.IsEmpty());
	return g_PopcornFXPluginRoot;
}
//static
const FString	&FPopcornFXPlugin::PopcornFXPluginContentDir()
{
	check(!g_PopcornFXPluginContentDir.IsEmpty());
	return g_PopcornFXPluginContentDir;
}
#endif // WITH_EDITOR

//----------------------------------------------------------------------------

FPopcornFXPlugin::FPopcornFXPlugin()
	: m_RootPackLoaded(false)
	, m_LaunchedPopcornFX(false)
	, m_Settings(null)
#if WITH_EDITORONLY_DATA
	, m_SettingsEditor(null)
	, m_PackIsUpToDate(false)
#endif // WITH_EDITORONLY_DATA
	, m_Profiler(null)
{
}

//----------------------------------------------------------------------------

void	FPopcornFXPlugin::StartupModule()
{
	s_Self = this;

	s_PluginVersionString = FString::Printf(TEXT("%d.%d.%d%s"), POPCORNFX_PLUGIN_VERSION_MAJOR, POPCORNFX_PLUGIN_VERSION_MINOR, POPCORNFX_PLUGIN_VERSION_PATCH, TEXT(POPCORNFX_PLUGIN_VERSION_TAG));
	PopcornFX::SDllVersion		pkfxVersion;
	s_PopornFXRuntimeVersionString = FString::Printf(TEXT("%d.%d.%d-%d%s"), pkfxVersion.Major, pkfxVersion.Minor, pkfxVersion.Patch, pkfxVersion.RevID, pkfxVersion.Debug ? TEXT("-DEBUG") : TEXT(""));
	s_PopornFXRuntimeRevID = pkfxVersion.RevID;

	{
		static const uint32			kMajBits = 2;
		static const uint32			kMinBits = 6;
		static const uint32			kPatchBits = 8;
		static_assert(kMajBits + kMinBits + kPatchBits == sizeof(s_PopornFXRuntimeMajMinPatch) * 8, "Invalid ver");
		check(pkfxVersion.Major > 0 && (pkfxVersion.Major - 1) < (1U << kMajBits));
		check(pkfxVersion.Minor < (1U << kMinBits));
		check(pkfxVersion.Patch < (1U << kPatchBits));
		s_PopornFXRuntimeMajMinPatch = pkfxVersion.Patch | (pkfxVersion.Minor << (kPatchBits)) | ((pkfxVersion.Major - 1) << (kPatchBits + kMinBits));
	}

	UE_LOG(LogPopcornFXPlugin, Log, TEXT("Starting up PopcornFX Plugin %s (PopcornFX Runtime %s) ..."), *s_PluginVersionString, *s_PopornFXRuntimeVersionString);

#if WITH_EDITOR
	{
		TSharedPtr<IPlugin>	pluginPopcornFX = IPluginManager::Get().FindPlugin(TEXT("PopcornFX"));
		if (!pluginPopcornFX.IsValid())
		{
			UE_LOG(LogPopcornFXPlugin, Fatal, TEXT("Could not find PopcornFX Plugin !"));
			return;
		}
		g_PopcornFXPluginRoot = pluginPopcornFX->GetBaseDir();
		check(!g_PopcornFXPluginRoot.IsEmpty());
		g_PopcornFXPluginContentDir = pluginPopcornFX->GetContentDir();
		check(!g_PopcornFXPluginContentDir.IsEmpty());
	}
#endif // WITH_EDITOR

	if (!m_LaunchedPopcornFX)
	{
		m_LaunchedPopcornFX = PopcornFXStartup();
		if (!m_LaunchedPopcornFX)
		{
			UE_LOG(LogPopcornFXPlugin, Error, TEXT("Couldn't startup PopcornFX"));
		}
	}

#if WITH_EDITOR
	// Global On Object Modified

	// Modified does not work: called pre-edit
	//m_OnObjectModifiedDelegateHandle = FCoreUObjectDelegates::OnObjectModified.AddRaw(this, &FPopcornFXPlugin::_OnObjectModified);

	// PropertyChanged does not work: UTexture called Super::PostEditChangeProperty too early !!
	//m_OnObjectPropertyChangedDelegateHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &FPopcornFXPlugin::_OnObjectPropertyChanged);
	
	// Use on save for now
	m_OnObjectSavedDelegateHandle = FCoreUObjectDelegates::OnObjectSaved.AddRaw(this, &FPopcornFXPlugin::_OnObjectSaved);

#endif

}

//----------------------------------------------------------------------------

void	FPopcornFXPlugin::ShutdownModule()
{
#if WITH_EDITOR
	// Global On Object Modified
	FCoreUObjectDelegates::OnObjectModified.Remove(m_OnObjectSavedDelegateHandle);
	m_OnObjectSavedDelegateHandle.Reset();
#endif

	HH_ASSERT(s_TotalParticleCount >= 0);

	if (m_Profiler != null)
		HH_SAFE_DELETE(m_Profiler);

	HH_ASSERT(g_RenderThreadId != 0);
	if (g_RenderThreadId > 0)
		PopcornFX::CThreadManager::UnsafeUnregisterUserThread(g_RenderThreadId);

	const hh_u32	registeredUserThreadsCount = g_RegisteredUserThreadsCount;
	g_RegisteredUserThreadsCount = 0;
	for (hh_u32 i = 0; i < registeredUserThreadsCount; ++i)
		PopcornFX::CThreadManager::UnsafeUnregisterUserThread(g_RegisteredUserThreads[i]);

	// shutdown PopcornFX
	if (m_LaunchedPopcornFX)
	{
		m_LaunchedPopcornFX = false; // avoid doing things during shutdown

		if (m_RootPackLoaded)
		{
			PopcornFX::CFileSystem::UnloadAllPacks();
			m_RootPackLoaded = false;
			m_FilePack = null;
			m_FilePackPath.Empty();
		}
		PopcornFXShutdown();
	}
#if WITH_EDITOR
	m_SettingsEditor = null;
#endif // WITH_EDITOR
	m_Settings = null;
	s_Self = null;
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

const UPopcornFXSettingsEditor	*FPopcornFXPlugin::SettingsEditor()
{
	if (!HH_VERIFY(LoadSettingsAndPackIFN()))
		return null;
	return m_SettingsEditor;
}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

const UPopcornFXSettings	*FPopcornFXPlugin::Settings()
{
	if (!HH_VERIFY(LoadSettingsAndPackIFN()))
		return null;
	return m_Settings;
}

//----------------------------------------------------------------------------

const UPopcornFXSettings	*FPopcornFXPlugin::RenderThread_Settings()
{
	if (!m_LaunchedPopcornFX || m_Settings == null)
	{
		return null;
	}
	return m_Settings;
}

//----------------------------------------------------------------------------

PopcornFX::PFilePack	FPopcornFXPlugin::FilePack()
{
	HH_VERIFY(LoadSettingsAndPackIFN());
	return m_FilePack;
}

//----------------------------------------------------------------------------

bool	FPopcornFXPlugin::LoadSettingsAndPackIFN()
{
	if (!m_LaunchedPopcornFX) // could happen on shutdown
		return false;
	if (m_Settings != null)
	{
#if WITH_EDITOR
		HH_ASSERT(m_SettingsEditor != null);
#endif
		HH_ASSERT(!m_RootPackLoaded || m_FilePack != null);
		return m_RootPackLoaded;
	}
#if WITH_EDITOR
	{
		UPopcornFXSettingsEditor	*editorSettings = GetMutableDefault<UPopcornFXSettingsEditor>();
		check(editorSettings);
		editorSettings->UpdateSourcePack();
		m_SettingsEditor = editorSettings;
	}
	HH_ASSERT(m_SettingsEditor != null);
#endif
	m_Settings = GetMutableDefault<UPopcornFXSettings>();
	check(m_Settings);

	s_GlobalScale = m_Settings->GlobalScale;
	s_GlobalScaleRcp = 1.f / s_GlobalScale;

	// Do not hot-reload Pack: Dangerous
	// Wait for next restart
	if (!m_RootPackLoaded)
	{
		HH_ASSERT(m_FilePack == null);

		const PopcornFX::CString	packPath = TCHAR_TO_ANSI(*m_Settings->PackMountPoint);
		PopcornFX::PFilePack		pack = PopcornFX::CFileSystem::LoadPack(packPath);
		if (!HH_VERIFY(pack != null))
			return false;

		m_FilePack = pack;
		m_FilePackPath = ANSI_TO_TCHAR(m_FilePack->Path().Data());
		m_FilePackPath /= "";
		HH_ASSERT(m_FilePackPath[m_FilePackPath.Len() - 1] == '/');
		m_RootPackLoaded = true;
	}
#if WITH_EDITOR
	m_PackIsUpToDate = (m_Settings->PackMountPoint / "") == m_FilePackPath;
	if (!m_PackIsUpToDate)
	{
		UE_LOG(LogPopcornFXPlugin, Warning, TEXT("PackMountPoint has changed, needs to restart editor !"));
	}
#endif

	HH_ASSERT(m_RootPackLoaded);
	HH_ASSERT(m_FilePack != null);
	return m_RootPackLoaded;
}

//----------------------------------------------------------------------------

FString		FPopcornFXPlugin::BuildPathFromPkPath(const char *pkPath, bool prependPackPath)
{
	HH_NAMEDSCOPEDPROFILE_C("FPopcornFXPlugin::BuildPathFromPkPath", POPCORNFX_UE_PROFILER_COLOR);

	if (!LoadSettingsAndPackIFN())
		return FString();

	if (!HH_VERIFY(FilePack() != null))
	{
		return FString();
	}

	PopcornFX::CString		path(PopcornFX::CStringContainer::NewResizable(pkPath));
	if (path.Empty())
		return FString();
	if (prependPackPath)
	{
		if (HH_VERIFY(FilePack() != null))
		{
			if (path[0] != '/')
				path.Prepend("/");
			path.Prepend(FilePack()->Path());
		}
	}

	// remove extension
	PopcornFX::CFilePath::StripExtensionInPlace(path);

	// Replace '.' '/'
	path.ReplaceTrInplace(".", "_");
	HH_ASSERT(!path.Contains("//"));

	// re-append filename : "Toto/filename.filename"
	const PopcornFX::CGuid	lastSlash = path.FindLastOf('/');
	path += ".";
	if (lastSlash.Valid())
		path.Append(path.Data() + lastSlash + 1, path.Length() - lastSlash - 2 /*the '/' and '.'*/);
	else
		path.Append(path.Data(), path.Length() - 1 /*the '.'*/);

	FString			p = path.Data();
	return p;
}

//----------------------------------------------------------------------------

namespace
{
	void		_SetupFile(const UPopcornFXFile *file, const PopcornFX::PCBaseObjectFile pkFile)
	{
		if (HH_VERIFY(pkFile != null))
		{
			HH_ASSERT(pkFile->InternalUserData() == null || pkFile->InternalUserData() == file);
			pkFile->SetInternalUserData(const_cast<UPopcornFXFile*>(file));
		}
	}
}

//----------------------------------------------------------------------------

PopcornFX::PBaseObjectFile		FPopcornFXPlugin::FindPkFile(const UPopcornFXFile *file)
{
	check(PopcornFX::HBO::g_Context != null);
	check(file->IsBaseObject());
	PopcornFX::PBaseObjectFile	pkFile = PopcornFX::HBO::g_Context->FindFile(file->PkPath());
	if (pkFile != null) // find file only
		_SetupFile(file, pkFile);
	return pkFile;
}

//----------------------------------------------------------------------------

PopcornFX::PBaseObjectFile		FPopcornFXPlugin::LoadPkFile(const UPopcornFXFile *file, bool reload)
{
	check(PopcornFX::HBO::g_Context != null);
	check(file->IsBaseObject());

	const PopcornFX::CString	pkPath = file->PkPath();
	if (pkPath.Empty())
		return null;
	PopcornFX::PBaseObjectFile	pkFile = PopcornFX::HBO::g_Context->LoadFile(pkPath.Data(), reload);
	_SetupFile(file, pkFile);
	return pkFile;
}

//----------------------------------------------------------------------------

PopcornFX::PBaseObject		FPopcornFXPlugin::FindPkBaseObject(const UPopcornFXFile *file, const char *objectName)
{
	check(PopcornFX::HBO::g_Context != null);
	check(file->IsBaseObject());
	PopcornFX::CString		path = file->PkPath();
	path += "/";
	path += objectName;
	PopcornFX::PBaseObject	obj = PopcornFX::HBO::g_Context->FindBaseObject(path);
	if (obj != null)
		_SetupFile(file, obj->File());
	return obj;
}

//----------------------------------------------------------------------------

PopcornFX::PBaseObject		FPopcornFXPlugin::LoadPkBaseObject(const UPopcornFXFile *file, const char *objectName)
{
	check(PopcornFX::HBO::g_Context != null);
	check(file->IsBaseObject());
	PopcornFX::CString		path = file->PkPath();
	path += "/";
	path += objectName;
	PopcornFX::PBaseObject	obj = PopcornFX::HBO::g_Context->LoadBaseObject(path);
	HH_ASSERT(obj != null);
	if (obj != null)
		_SetupFile(file, obj->File());
	return obj;
}

//----------------------------------------------------------------------------

UPopcornFXFile		*FPopcornFXPlugin::GetPopcornFXFile(const PopcornFX::CBaseObjectFile *boFile)
{
	if (boFile == null)
		return null;

	if (boFile->InternalUserData() == null)
	{
		UObject			*object = LoadUObjectFromPkPath(boFile->Path().Data(), false);
		if (object == null)
			return null;
		UPopcornFXFile	*file = Cast<UPopcornFXFile>(object);
		if (!HH_VERIFY(file != null))
			return null;
		_SetupFile(file, boFile);
		return file;
	}

	UPopcornFXFile		*file = reinterpret_cast<UPopcornFXFile*>(boFile->InternalUserData());

	HH_ASSERT(Cast<UPopcornFXFile>(LoadUObjectFromPkPath(boFile->Path().Data(), false)) == file);

	check(file == null || file->IsBaseObject());

	return file;
}

//----------------------------------------------------------------------------

UObject		*FPopcornFXPlugin::LoadUObjectFromPkPath(const char *pkPath, bool pathNotVirtual)
{
	if (!m_LaunchedPopcornFX) // could happen at shutdown
		return null;

	if (!HH_VERIFY(!IsGarbageCollecting() && !GIsSavingPackage))
		return null;
	//
	// about BuildPathFromPkPath(..., __ bool prependPackPath __):
	//
	//	if pathNotVirtual: // real path
	//		already prepended with "/Game/"
	//	else // virtual path
	//		no "/Game/", just "Particles/": so prendpend "/Game/"
	FString				p = BuildPathFromPkPath(pkPath, !pathNotVirtual);
	if (p.IsEmpty())
		return null;
	UObject				*anyObject = ::FindObject<UObject>(null, *p);
	if (anyObject == null)
		anyObject = ::LoadObject<UObject>(null, *p);
	if (anyObject != null)
	{
		anyObject->ConditionalPostLoad();
		anyObject->ConditionalPostLoadSubobjects();
		// // preload here right now
		// auto linker = anyObject->GetLinker();
		// if (linker && !anyObject->HasAnyFlags(RF_LoadCompleted))
		// {
		// 	//CLog::Log(HH_INFO, "FileSystem FindUObject: Preloading '%s'", TCHAR_TO_ANSI(*p));
		// 	anyObject->SetFlags(RF_NeedLoad);
		// 	linker->Preload(anyObject);
		// }
	}

	//CLog::Log(HH_INFO, "FileSystem FindUObject %s > %s > %p", path.Data(), TCHAR_TO_ANSI(*p), anyObject);
	return anyObject;
}

//----------------------------------------------------------------------------

void	FPopcornFXPlugin::NotifyObjectChanged(UObject *object)
{
	if (m_FilePack == null)
		return;

	if (!object->IsA<UTexture>() &&
		!object->IsA<UStaticMesh>() &&
		!object->IsA<USkeletalMesh>() &&
		true)
	{
		if (object->IsA<UPopcornFXFile>())
		{
			UPopcornFXFile		*file = Cast<UPopcornFXFile>(object);
			HH_ASSERT(file != null);
			if (file->PkExtension() != TEXT("pkat")) // do not trigger for base object
				return;
		}
		else
			return;
	}

	HH_ASSERT(!m_FilePackPath.IsEmpty());
	HH_ASSERT(m_FilePackPath[m_FilePackPath.Len() - 1] == '/');

	const FString		objectPath = object->GetPathName();
	if (!objectPath.StartsWith(m_FilePackPath))
		return;
	const FString				virtualPath = objectPath.Right(objectPath.Len() - m_FilePackPath.Len());
	const PopcornFX::CString	virtualPathPk(TCHAR_TO_ANSI(*virtualPath));
	PopcornFX::Resource::DefaultManager()->NotifyResourceChanged(PopcornFX::CFilePackPath(m_FilePack, virtualPathPk));
}

//----------------------------------------------------------------------------

bool	FPopcornFXPlugin::HandleSettingsModified()
{
#if WITH_EDITOR
	m_SettingsEditor = null; // LoadSettingsAndPackIFN force reload settings
#endif // WITH_EDITOR
	m_Settings = null; // LoadSettingsAndPackIFN force reload settings

	if (!LoadSettingsAndPackIFN())
		return false;

	if (m_Settings != null)
		m_OnSettingsChanged.Broadcast();

#if WITH_EDITOR
	return m_SettingsEditor != null && m_Settings != null;
#else
	return m_Settings != null;
#endif // WITH_EDITOR
}

//----------------------------------------------------------------------------
#if WITH_EDITOR

TSharedRef<class IPopcornFXEffectEditor>	FPopcornFXPlugin::CreateEffectEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, class UPopcornFXEffect* Effect)
{
	TSharedRef<FPopcornFXEffectEditor> EffectEditor(new FPopcornFXEffectEditor());
	EffectEditor->InitEffectEditor(Mode, InitToolkitHost, Effect);
	return EffectEditor;
}

#endif // WITH_EDITOR
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#if WITH_EDITOR
namespace
{
	bool		g_SetAskImportAssetDependencies_YesAll = false;
	bool		g_SetAskBuildMeshData_YesAll = false;
}

void	FPopcornFXPlugin::SetAskImportAssetDependencies_YesAll(bool yesAll) { g_SetAskImportAssetDependencies_YesAll = yesAll; }
bool	FPopcornFXPlugin::AskImportAssetDependencies_YesAll() { return g_SetAskImportAssetDependencies_YesAll; }
void	FPopcornFXPlugin::SetAskBuildMeshData_YesAll(bool yesAll) { g_SetAskBuildMeshData_YesAll = yesAll; }
bool	FPopcornFXPlugin::AskBuildMeshData_YesAll() { return g_SetAskBuildMeshData_YesAll; }
#endif // WITH_EDITOR

//----------------------------------------------------------------------------

#if WITH_EDITOR
void	FPopcornFXPlugin::_OnObjectSaved(UObject *object)
{
	NotifyObjectChanged(object);
}
#endif // WITH_EDITOR

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

CPopcornFXProfiler		*FPopcornFXPlugin::Profiler()
{
	HH_ASSERT(IsMainThread());
	if (m_Profiler == null)
		m_Profiler = HH_NEW(CPopcornFXProfiler);
	return m_Profiler;
}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
