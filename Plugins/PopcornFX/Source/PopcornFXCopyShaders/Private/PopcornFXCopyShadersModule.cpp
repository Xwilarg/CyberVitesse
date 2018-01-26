//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXCopyShadersModule.h"

#include "Misc/ConfigCacheIni.h"
#include "HAL/PlatformFilemanager.h"

#include "IPluginManager.h"
#include "HAL/IConsoleManager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Launch/Resources/Version.h"

//----------------------------------------------------------------------------
//
// # Crashes ? follow those steps:
//
// 1) put "PopcornFX.AutoCopyShaders=0" in ConsoleVariables.ini to disable the copy
// 2) then, manually copy the "UE4PopcornFXPlugin/PopcornFX/Shaders/*.usf" shaders in
//   your C:/Program Files (x86)/Epic Games/4.X/Engine/Shaders/
//
//----------------------------------------------------------------------------

IMPLEMENT_MODULE(FPopcornFXCopyShadersModule, PopcornFXCopyShadersModule)

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXCopyShaders, Log, All);

//----------------------------------------------------------------------------

static const FString		kShaderList[] = {
	FString(TEXT("PopcornFXVertexFactory.ush")),
	FString(TEXT("PopcornFXMeshVertexFactory.ush")),
// #if (PK_HAS_GPU != 0) // not detected
	FString(TEXT("PopcornFXSortComputeShader.ush")),
	FString(TEXT("PopcornFXBillboarderBillboardComputeShader.ush")),
// #endif // PK_HAS_GPU
};
static const uint32			kShaderListCount = sizeof(kShaderList) / sizeof(kShaderList[0]);

//----------------------------------------------------------------------------

static TAutoConsoleVariable<int32>	CVarAutoImportShaderFiles(
	TEXT("PopcornFX.AutoCopyShaders"),
	1,
	TEXT("If 1, PopcornFX Plugin Shaders will be synced in your UE4 Engine Shaders folder at UE4 Editor Startup and Shutdown."),
	ECVF_Default);

//----------------------------------------------------------------------------

void	FPopcornFXCopyShadersModule::StartupModule()
{
	// Not sure we have the right to do that,
	// but there is no module loading phase between the "official" LoadConsoleVariablesFromINI and Shader loading
	FConfigCacheIni::LoadConsoleVariablesFromINI();

	UpdateShaders();
}

//----------------------------------------------------------------------------

void	FPopcornFXCopyShadersModule::ShutdownModule()
{
	UpdateShaders();
}

//----------------------------------------------------------------------------

/*
** Sync UE4PopcornFXPlugin/PopcornFX/Shaders/ and /Epic Games/4.X/Engine/Shaders, by copying last modified shader
*/
void	FPopcornFXCopyShadersModule::UpdateShaders()
{
#if (ENGINE_MINOR_VERSION >= 17)

	// No need to copy shaders anymore !

#else

	if (CVarAutoImportShaderFiles.GetValueOnGameThread() > 0)
	{
		UE_LOG(LogPopcornFXCopyShaders, Log, TEXT("We're going to Copy PopcornFX Shaders ... (To disable: put \"PopcornFX.AutoCopyShaders=0\" in the ConsoleVariables.ini)"));
	}
	else
	{
		UE_LOG(LogPopcornFXCopyShaders, Log, TEXT("We did not Copy PopcornFX Shaders (disabled by the \"PopcornFX.AutoCopyShaders=0\")"));
		return;
	}

	IPlatformFile		&platformFile = FPlatformFileManager::Get().GetPlatformFile();

	const FString		engineShaderDir = (FPaths::EngineDir() / TEXT("Shaders")) + "/";
	TSharedPtr<IPlugin>	pluginPopcornFX = IPluginManager::Get().FindPlugin(TEXT("PopcornFX"));

	if (!pluginPopcornFX.IsValid())
	{
		UE_LOG(LogPopcornFXCopyShaders, Fatal, TEXT("Could not find PopcornFX Plugin !"));
		return;
	}
	const FString		pluginShaderDir = (pluginPopcornFX->GetBaseDir() / TEXT("Shaders")) + "/Private/";

	for (uint32 shaderi = 0; shaderi < kShaderListCount; ++shaderi)
	{
		const FString	&shaderFilename = kShaderList[shaderi];
		const FString	pluginShaderPath = pluginShaderDir + shaderFilename;
		// Shaders in Plugin are 4.17-style Shaders/Private/....ush
		// When in UE <= 4.16 we copy them to .usf Engine shaders
		check(FPaths::GetExtension(shaderFilename) == TEXT("ush"));
		const FString	engineShaderFilename = FPaths::GetBaseFilename(shaderFilename) + ".usf";
		const FString	engineShaderPath = engineShaderDir + engineShaderFilename;

		if (!platformFile.FileExists(*pluginShaderPath))
		{
			UE_LOG(LogPopcornFXCopyShaders, Error, TEXT("PopcornFX Plugin shader %s not found: %s"), *shaderFilename, *pluginShaderPath);
			continue;
		}

		bool			doCopy = false;
		bool			copyToEngine = false;

		FDateTime		pluginDate = platformFile.GetTimeStamp(*pluginShaderPath);
		FDateTime		engineDate;

		if (!platformFile.FileExists(*engineShaderPath))
		{
			doCopy = true;
			copyToEngine = true;
		}
		else
		{
			engineDate = platformFile.GetTimeStamp(*engineShaderPath);
			doCopy = (pluginDate != engineDate);
			copyToEngine = (engineDate < pluginDate);
		}

		if (doCopy)
		{
			FString			src;
			FString			dst;
			FDateTime		srcDate;
			FString			debugName;
			if (copyToEngine)
			{
				debugName = "to your UE4 Engine folder";
				src = pluginShaderPath;
				srcDate = pluginDate;
				dst = engineShaderPath;
			}
			else
			{
				// rechenge shader name if back to plugin
				debugName = "back to the PopcornFX Plugin";
				src = engineShaderPath;
				srcDate = engineDate;
				dst = pluginShaderPath;
			}

			if (platformFile.CopyFile(*dst, *src))
			{
				// Force dst TimeStamp, so next UpdateShaders will say "is up to date"
				platformFile.SetTimeStamp(*dst, srcDate);

				//
				// # Crashes ? see comment at the top of this file
				//
				checkfSlow(platformFile.GetTimeStamp(*dst).GetTicks() == srcDate.GetTicks(), TEXT("Timestamps have not been properly updated"));

				UE_LOG(LogPopcornFXCopyShaders, Log, TEXT("PopcornFX Shader %s copied %s ! %s > %s"), *shaderFilename, *debugName, *src, *dst);
			}
			else
			{
				// # Crashes ? see comment at the top of this file
				UE_LOG(LogPopcornFXCopyShaders, Error, TEXT("Failed to copy PopcornFX Shader %s %s ! %s %s "), *shaderFilename, *debugName, *src, *dst);
			}
		}
		else
			UE_LOG(LogPopcornFXCopyShaders, Log, TEXT("PopcornFX Shader %s is up to date"), *shaderFilename);
	}
#endif
}

//----------------------------------------------------------------------------
