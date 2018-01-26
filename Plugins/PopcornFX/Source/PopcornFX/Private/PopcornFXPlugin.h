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

#include "IPopcornFXPlugin.h"
#include "PopcornFXSettingsEditor.h"
#include "PopcornFXSettings.h"

#include "PopcornFXSDK.h"
#include <hh_base_object/include/hbo_file.h>
#include <hh_base_object/include/hbo_object.h>

FWD_PK_API_BEGIN
class	CParticleSceneInterface;
HH_FORWARD_DECLARE(FilePack);
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;

class	UPopcornFXFile;
class	FPopcornFXDependencyModule;
class	CPopcornFXProfiler;
#if WITH_EDITOR
class	UPopcornFXSettingsEditor;
#endif

DECLARE_MULTICAST_DELEGATE(FOnPopcornFXSettingChange);

class	FPopcornFXPlugin : public IPopcornFXPlugin
{
private:
	static FPopcornFXPlugin				*s_Self;
	static float						s_GlobalScale;
	static float						s_GlobalScaleRcp;
	static FString						s_PluginVersionString;
	static FString						s_PopornFXRuntimeVersionString;
	static uint32						s_PopornFXRuntimeRevID;
	static uint16						s_PopornFXRuntimeMajMinPatch;

	static FString						s_URLDocumentation;
	static FString						s_URLPluginWiki;
	static FString						s_URLAnswerHub;

public:
	static FPopcornFXPlugin				&Get() { check(s_Self != null); return *s_Self; }
	static float						GlobalScale() { return s_GlobalScale; }
	static float						GlobalScaleRcp() { return s_GlobalScaleRcp; }

	static const FString				&PluginVersionString() { return s_PluginVersionString; }
	static const FString				&PopornFXRuntimeVersionString() { return s_PopornFXRuntimeVersionString; }
	static const uint32					&PopornFXRuntimeRevID() { return s_PopornFXRuntimeRevID; }
	static const uint16					&PopornFXRuntimeMajMinPatch() { return s_PopornFXRuntimeMajMinPatch; }

	static const FString				&DocumentationURL() { return s_URLDocumentation; }
	static const FString				&PluginWikiURL() { return s_URLPluginWiki; }
	static const FString				&AnswerHubURL() { return s_URLAnswerHub; }

	static int32						TotalParticleCount();
	static void							IncTotalParticleCount(hh_i32 newTotalParticleCount);

	static void							RegisterRenderThreadIFN();
	static void							RegisterCurrentThreadAsUserIFN();

	static bool							IsMainThread();

#if WITH_EDITOR
	static const FString				&PopcornFXPluginRoot();
	static const FString				&PopcornFXPluginContentDir();
#endif // WITH_EDITOR

public:
	FOnPopcornFXSettingChange			m_OnSettingsChanged;

public:
	FPopcornFXPlugin();

	virtual void						StartupModule() override;
	virtual void						ShutdownModule() override;

	static PopcornFX::SEngineVersion	PopcornFXBuildVersion();

	const UPopcornFXSettings			*Settings();
	const UPopcornFXSettings			*RenderThread_Settings();
#if WITH_EDITOR
	const UPopcornFXSettingsEditor		*SettingsEditor();
#endif // WITH_EDITOR
	PopcornFX::PFilePack				FilePack();

	/**
	*	Take a PopcornFX file path (virtual or not)
	*	return the corresponding path for Unreal assets
	*/
	FString								BuildPathFromPkPath(const char *pkPath, bool prependPackPath);

	PopcornFX::PBaseObjectFile			FindPkFile(const UPopcornFXFile *file);
	PopcornFX::PBaseObjectFile			LoadPkFile(const UPopcornFXFile *file, bool reload = false);

	PopcornFX::PBaseObject				FindPkBaseObject(const UPopcornFXFile *file, const char *objectName);
	PopcornFX::PBaseObject				LoadPkBaseObject(const UPopcornFXFile *file, const char *objectName);

	template <typename _ObjectType>
	PopcornFX::TRefPtr<_ObjectType>		FindPkObject(const UPopcornFXFile *file, const char *objectName) { return PopcornFX::HBO::Cast<_ObjectType>(FindPkBaseObject(file, objectName)); }
	template <typename _ObjectType>
	PopcornFX::TRefPtr<_ObjectType>		LoadPkObject(const UPopcornFXFile *file, const char *objectName) { return PopcornFX::HBO::Cast<_ObjectType>(LoadPkBaseObject(file, objectName)); }

	UPopcornFXFile						*GetPopcornFXFile(const PopcornFX::CBaseObjectFile *file);

	UObject								*LoadUObjectFromPkPath(const char *pkPath, bool notVirtual);

	void								NotifyObjectChanged(UObject *object);

#if WITH_EDITOR
	virtual TSharedRef<class IPopcornFXEffectEditor>	CreateEffectEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, class UPopcornFXEffect* Effect) override;
#endif
	
	bool								HandleSettingsModified();

	static FName						Name_DiffuseTexture() { static FName n(TEXT("DiffuseTexture")); return n; }
	static FName						Name_NormalTexture() { static FName n(TEXT("NormalTexture")); return n; }
	static FName						Name_SpecularTexture() { static FName n(TEXT("SpecularTexture")); return n; }
	static FName						Name_AlphaRemapper() { static FName n(TEXT("AlphaRemapper")); return n; }
	static FName						Name_DecalColor() { static FName n(TEXT("DecalColor")); return n; }
	static FName						Name_SoftnessDistance() { static FName n(TEXT("PopcornFX_SoftnessDistance")); return n; }

	static FName						Name_POPCORNFX_IS_SOFT_PARTICLES() { static FName n(TEXT("POPCORNFX_IS_SOFT_PARTICLES")); return n; }
	static FName						Name_POPCORNFX_IS_SOFT_ANIM() { static FName n(TEXT("POPCORNFX_IS_SOFT_ANIM")); return n; }
	static FName						Name_POPCORNFX_HAS_NORMAL_TEXTURE() { static FName n(TEXT("POPCORNFX_HAS_NORMAL_TEXTURE")); return n; }
	static FName						Name_POPCORNFX_HAS_ALPHA_REMAPPER() { static FName n(TEXT("POPCORNFX_HAS_ALPHA_REMAPPER")); return n; }
	static FName						Name_POPCORNFX_IS_NO_ALPHA() { static FName n(TEXT("POPCORNFX_IS_NO_ALPHA")); return n; }
	static FName						Name_POPCORNFX_CAST_SHADOW() { static FName n(TEXT("POPCORNFX_CAST_SHADOW")); return n; }

	static FName						Name_POPCORNFX_IS_BILLBOARD() { static FName n(TEXT("POPCORNFX_IS_BILLBOARD")); return n; }
	static FName						Name_POPCORNFX_IS_RIBBON() { static FName n(TEXT("POPCORNFX_IS_RIBBON")); return n; }
	static FName						Name_POPCORNFX_IS_MESH() { static FName n(TEXT("POPCORNFX_IS_MESH")); return n; }
	static FName						Name_POPCORNFX_IS_DECAL() { static FName n(TEXT("POPCORNFX_IS_DECAL")); return n; }

	static const FLinearColor			&Color_PopcornFX() { static const FLinearColor c(0.011765f, 0.501961f, 0.839216f, 1.0f); return c; }


#if WITH_EDITOR
	static void							SetAskImportAssetDependencies_YesAll(bool yesAll);
	static bool							AskImportAssetDependencies_YesAll();
	static void							SetAskBuildMeshData_YesAll(bool yesAll);
	static bool							AskBuildMeshData_YesAll();
#endif

	CPopcornFXProfiler					*Profiler();

private:
	bool								LoadSettingsAndPackIFN();

private:
	bool								m_RootPackLoaded;
	bool								m_LaunchedPopcornFX;

	const UPopcornFXSettings			*m_Settings;
#if WITH_EDITORONLY_DATA
	const UPopcornFXSettingsEditor		*m_SettingsEditor;
	bool								m_PackIsUpToDate;
#endif // WITH_EDITORONLY_DATA
	FString								m_PackPath;
	PopcornFX::PFilePack				m_FilePack;
	FString								m_FilePackPath;

#if WITH_EDITOR
	// Global Editor callbacks
	FDelegateHandle						m_OnObjectSavedDelegateHandle;
	void								_OnObjectSaved(UObject *object);
#endif // WITH_EDITOR

	CPopcornFXProfiler					*m_Profiler;


};
