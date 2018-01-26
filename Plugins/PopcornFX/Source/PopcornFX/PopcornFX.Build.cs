//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
﻿
using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;

namespace UnrealBuildTool.Rules
{
	public class PopcornFX : ModuleRules
	{
		bool					IAmDeveloping = false;

		static string			DefaultClientName = "UE4";
		private static char[]	DirSeparators = {'/', '\\'};
		private string[]		PkLibs = new string[] {
			"HH-BaseObject",
			"HH-Compiler",
			//"HH-Discretizers",
			"HH-EngineUtils" ,
			"HH-Geometrics",
			"HH-Imaging",
			"HH-Kernel",
			//"HH-Live",
			"HH-Maths",
			"HH-Particles",
			//"HH-ParticlesToolbox",
			"HH-Toolkit",
			"PK-Drawers",
			//"PK-Plugin_CodecImage_DDS",
			//"PK-Plugin_CodecImage_JPG",
			//"PK-Plugin_CodecImage_PNG",
			//"PK-Plugin_CodecImage_TGA",
			"PK-Plugin_CompilerBackend_CPU_Emulation",
			//"PK-IntegrationUnrealEngine",
		};

		enum SDK
		{
			No,
			Embed,
			Full,
		}
		SDK						CurrentSDK = SDK.No;

		private void			Log(string message)
		{
			Console.WriteLine("PopcornFX - " + message);
		}

		private void			LogError(string message)
		{
			Console.WriteLine("PopcornFX - ERROR - " + message);
		}

		private string			WindowsPlatform_GetVisualStudioCompilerVersionName_Prefer2015()
		{
#if UE_4_18_OR_LATER
			string		vs_version = Target.WindowsPlatform.GetVisualStudioCompilerVersionName().ToString();
#else
			string vs_version = WindowsPlatform.GetVisualStudioCompilerVersionName().ToString();
#endif
			if (vs_version.Equals("2017")) // vs2017 is compatible with vs2015 libs, so use vs2015
				return "2015";
			else
				return vs_version;
		}

		private void SetupBackwardCompatFiles()
		{
			string moduleSourceDir = ModuleDirectory + "/";
			string backwardCompatSourceDir = moduleSourceDir + "Private/BackwardCompat/";
			string backwardCompatDir = moduleSourceDir + "../../BackwardCompat/";

			if (!Directory.Exists(backwardCompatSourceDir))
				Directory.CreateDirectory(backwardCompatSourceDir);
			// For now hardcoded, should we automate that ?..
			string targetFile = backwardCompatSourceDir + "PopcornFXDefaultMaterialsSettings.h";

			if (File.Exists(targetFile))
				File.SetAttributes(targetFile, FileAttributes.Normal);
			File.Delete(targetFile);
#if UE_4_18_OR_LATER
			File.Copy(backwardCompatDir + "PopcornFXDefaultMaterialsSettings_After_4_18.h", targetFile, true);
#else
			File.Copy(backwardCompatDir + "PopcornFXDefaultMaterialsSettings_Before_4_18.h", targetFile, true);
#endif
			File.SetAttributes(targetFile, FileAttributes.ReadOnly);
		}

#if WITH_FORWARDED_MODULE_RULES_CTOR
		private bool			SetupPopcornFX(ReadOnlyTargetRules Target)
#else
		private bool			SetupPopcornFX(TargetInfo Target)
#endif // WITH_FORWARDED_MODULE_RULES_CTOR
		{
			CurrentSDK = SDK.No;

			string		sdkRoot = null;

			string		pluginRoot = ModuleDirectory + "/../..";
			pluginRoot = Path.GetFullPath(pluginRoot);
			pluginRoot = Utils.CleanDirectorySeparators(pluginRoot, '/').TrimEnd(DirSeparators) + "/";

			string		sdkEmbedRoot = pluginRoot + "PopcornFX_Runtime_SDK/";
			if (System.IO.Directory.Exists(sdkEmbedRoot + "source_tree"))
			{
				CurrentSDK = SDK.Embed;
				sdkRoot = sdkEmbedRoot;
			}
			else
			{
				string		sdkFullRoot = Environment.GetEnvironmentVariable("HH_SDK_ROOT");
				if (!String.IsNullOrEmpty(sdkFullRoot))
				{
					sdkFullRoot = Utils.CleanDirectorySeparators(sdkFullRoot, '/').TrimEnd(DirSeparators) + "/";
					if (System.IO.Directory.Exists(sdkFullRoot + "source_tree"))
					{
						CurrentSDK = SDK.Full;
						sdkRoot = sdkFullRoot;
						// assume that
						IAmDeveloping = true;
					}
					else
					{
						LogError("Found HH_SDK_ROOT but does not seem valid !");
					}
				}
			}

			if (CurrentSDK == SDK.No || string.IsNullOrEmpty(sdkRoot))
			{
				Log("SDK not found, continuing without.");
				return true;
			}

			if (IAmDeveloping)
			{
				// maybe not faster, but we want to make sure there is no missing includes
				bFasterWithoutUnity = true;
				PrivatePCHHeaderFile = "Private/EmptyPCH.h";
			}

			string		clientName = DefaultClientName;
			string		binDir = sdkRoot + "source_tree/hellheaven/bin/";
			if (!System.IO.Directory.Exists(binDir + clientName))
			{
				string[]	clients = System.IO.Directory.GetDirectories(binDir);
				if (clients == null || clients.Length == 0)
				{
					LogError("Invalid PopcornFX SDK, nothing found in " + binDir);
					return false;
				}
				clientName = System.IO.Path.GetFileName(clients[0].TrimEnd(DirSeparators));
			}

			string		sourceDir = sdkRoot + "source_tree/hellheaven/";
			string		clientLibDir = binDir + clientName + "/";

			PublicIncludePaths.AddRange(
				new string[] {
					sourceDir,
					sourceDir + "include/",
					sourceDir + "include/license/" + clientName + "/",
					sourceDir + "../HellHeaven-SDK/Samples/IntegrationUnrealEngine/",
				}
			);

			string		configShort = null;
			switch (Target.Configuration)
			{
				case UnrealTargetConfiguration.Unknown:
					Debug.Assert(false, "Error Unknown Target Configuration");
					break;
				case UnrealTargetConfiguration.Debug:
					configShort = "d";
					Definitions.Add("HH_DEBUG");
					break;
				case UnrealTargetConfiguration.DebugGame:
					configShort = "d";
					Definitions.Add("HH_DEBUG");
					break;
				case UnrealTargetConfiguration.Development:
					configShort = "r";
					Definitions.Add("HH_RELEASE");
					break;
				case UnrealTargetConfiguration.Shipping:
					configShort = "s";
					Definitions.Add("HH_RETAIL");
					break;
				case UnrealTargetConfiguration.Test:
					configShort = "s";
					Definitions.Add("HH_RETAIL");
					break;
			}
			Debug.Assert(!String.IsNullOrEmpty(configShort), "Error: invalid configuration");

			string		libPrefix = "";
			string		libExt = "";
			switch (Target.Platform)
			{
				case UnrealTargetPlatform.Win32:
					libPrefix = clientLibDir + "vs" +  WindowsPlatform_GetVisualStudioCompilerVersionName_Prefer2015() + "_x32/";
					libExt = ".lib";
					break;
				case UnrealTargetPlatform.Win64:
					libPrefix = clientLibDir + "vs" +  WindowsPlatform_GetVisualStudioCompilerVersionName_Prefer2015() + "_x64/";
					libExt = ".lib";
					break;
				case UnrealTargetPlatform.Mac:
					libPrefix = clientLibDir + "gmake_macosx_x64/lib";
					libExt = ".a";
					break;
				case UnrealTargetPlatform.XboxOne:
					libPrefix = clientLibDir + "vs2015_durango/";
					// "vs" + WindowsPlatform.GetVisualStudioCompilerVersionName(); // error (exception) on >= 4.16
					// "vs" + XboxOnePlatform.GetVisualStudioCompilerVersionName(); // "XboxOnePlatform" exists on >= 4.16 but not public
					libExt = ".lib";
					break;
				case UnrealTargetPlatform.PS4:
					libPrefix = clientLibDir + "vs2015_Orbis/lib";
					// "vs" + WindowsPlatform.GetVisualStudioCompilerVersionName(); // error (exception) on >= 4.16
					libExt = ".a";
					break;
				case UnrealTargetPlatform.IOS:
					// UE4 starts dropping armv7 from UE4.16, only link with arm64
					libPrefix = clientLibDir + "gmake_ios64/lib";
					libExt = ".a";
					break;
				case UnrealTargetPlatform.Android:
					// Multiple Architectures ! (armeabi, arm64)
					// Using PublicLibraryPaths so ld will "skipping incompatible" libraries
					// FIXME: UE Build will not watch for lib changes to relink
					PublicLibraryPaths.Add(clientLibDir + "gmake_android");		// armv7a (armeabi)
					PublicLibraryPaths.Add(clientLibDir + "gmake_android64");	// armv8a (arm64)
					libPrefix = "";
					libExt = "";
					break;
				case UnrealTargetPlatform.Linux:
					libPrefix = clientLibDir + "gmake_linux_x64/lib";
					libExt = ".a";
					break;
				case UnrealTargetPlatform.Switch:
					libPrefix = clientLibDir + "vs2015_NX64/";
					libExt = ".a";
					break;
				default:
					LogError("Target Platform " + Target.Platform.ToString() + " not supported by PopcornFX");
					Debug.Assert(false, "Platform not supported");
					break;
			}

			Log("SDK " + CurrentSDK + " - " +  String.Join(", ", PublicLibraryPaths) + "" + libPrefix + " ... _" + configShort + libExt);

			foreach (string lib in PkLibs)
			{
				PublicAdditionalLibraries.Add(libPrefix + lib + "_" + configShort + libExt);
			}

			if (Target.Platform == UnrealTargetPlatform.Win32 ||
				Target.Platform == UnrealTargetPlatform.Win64)
			{
				PublicAdditionalLibraries.AddRange(
					new string[] {
						"Version.lib",
						"Psapi.lib",
					}
				);
			}
			else if (Target.Platform == UnrealTargetPlatform.Linux)
			{
				PublicAdditionalLibraries.AddRange(
					new string[] {
						//"bfd",
						//"erty",
					}
				);
			}

			bool		hasGPU = false;

#if false // FIXME: "platform" is now "platforms"
			string		libD3D11 = fullLibPrefix + "PK-Plugin_CompilerBackend_GPU_D3D11" + fullLibSuffix;
			if (System.IO.File.Exists(libD3D11))
			{
				hasGPU = true;
				Log("Building with GPU D3D11");
				PublicAdditionalLibraries.Add(libD3D11);
				Definitions.Add("HH_PARTICLES_UPDATER_USE_D3D11=1");
				PublicAdditionalLibraries.AddRange(
					new string[] {
						"d3dcompiler.lib",
					}
				);
				PrivateIncludePaths.Add("Runtime/Windows/D3D11RHI/Private/Windows");
				PrivateDependencyModuleNames.Add("D3D11RHI");
				Definitions.Add("PK_GPU_D3D11=1");
			}
			else
#endif
				Definitions.Add("PK_GPU_D3D11=0");

			if (hasGPU)
				Definitions.Add("PK_HAS_GPU=1");
			else
				Definitions.Add("PK_HAS_GPU=0");
			return true;
		}

#if WITH_FORWARDED_MODULE_RULES_CTOR
		private bool			SetupPlugin(ReadOnlyTargetRules Target)
#else
		private bool			SetupPlugin(TargetInfo Target)
#endif // WITH_FORWARDED_MODULE_RULES_CTOR
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicIncludePaths.AddRange(
				new string[] {
					//"Runtime/PopcornFX/Public",
					//"Runtime/PopcornFX/Classes",
				}
				);
			PrivateIncludePaths.AddRange(
				new string[] {
					"PopcornFX/Private",
				}
				);
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
				}
				);
			PrivateIncludePathModuleNames.AddRange(
				new string[] {
					"Settings",
				}
				);
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"InputCore",
					"CoreUObject",
					//"TargetPlatform",
					"Engine",
					"Renderer",
					"RenderCore",
					"ShaderCore",
					"RHI",
					"PhysX",
					"MovieScene",
					"MovieSceneTracks",
					"ClothingSystemRuntime",
					//"D3D11RHI",
				}
				);

			// EDITOR ONLY
			//if (Target.Type == TargetRules.TargetType.Editor)
#if UE_4_18_OR_LATER
			if (Target.bBuildEditor == true)
#else
			if (UEBuildConfiguration.bBuildEditor == true)
#endif
			{
				//Definitions.Add("MY_WITH_EDITOR=1");
				PrivateDependencyModuleNames.AddRange(
					new string[]
					{
						"UnrealEd",
						"AssetTools",
						"DirectoryWatcher",
						"PropertyEditor",
						"SceneOutliner",
						"Projects",
						"Slate",
						"SlateCore",
						"EditorStyle",
						//"StandaloneRenderer",
						"EditorWidgets",
						"PlacementMode",
						"XmlParser",
						"MessageLog",
						"LevelEditor",
						"Sequencer",
						"MovieSceneTools",

						"DesktopPlatform", // Profiler HUD ask for a path to save profiler report

						"Projects", // IPlugin
					}
					);
				PrivateIncludePaths.AddRange(
					new string[]
					{
						//"PopcornFX/Private/Editor",
					}
					);
			}

			SetupModulePhysXAPEXSupport(Target);

#if UE_4_17_OR_LATER
			// PhysXFormats replaced by PhysXCooking, but does not seem to need it anymore
#else
			if (UEBuildConfiguration.bCompilePhysX && UEBuildConfiguration.bRuntimePhysicsCooking)
			{
				DynamicallyLoadedModuleNames.Add("PhysXFormats");
				PrivateIncludePathModuleNames.Add("PhysXFormats");
			}
#endif

			// Additional platform dependent modules/include paths
			switch (Target.Platform)
			{
				case UnrealTargetPlatform.PS4:
					PrivateDependencyModuleNames.Add("PS4RHI");
					//PrivateIncludePaths.Add("Runtime/PS4/PS4RHI/Private"); // wont work (4.11)
					PublicIncludePaths.Add("Runtime/PS4/PS4RHI/Private");
					break;
				case UnrealTargetPlatform.XboxOne:
					PrivateDependencyModuleNames.Add("D3D12RHI");
					break;
				default:
					break;
			}

			SetupBackwardCompatFiles();

			return true;
		}

#if WITH_FORWARDED_MODULE_RULES_CTOR
		public PopcornFX(ReadOnlyTargetRules Target) : base(Target)
#else
		public PopcornFX(TargetInfo Target)
#endif // WITH_FORWARDED_MODULE_RULES_CTOR
		{
			if (SetupPopcornFX(Target))
			{
				SetupPlugin(Target);
			}
		}
	}
}
