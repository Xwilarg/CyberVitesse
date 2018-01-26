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

namespace UnrealBuildTool.Rules
{
	public class PopcornFXOnDefault : ModuleRules
	{
		bool					IAmDeveloping = false;

#if WITH_FORWARDED_MODULE_RULES_CTOR
		public PopcornFXOnDefault(ReadOnlyTargetRules Target) : base(Target)
#else
		public PopcornFXOnDefault(TargetInfo Target)
#endif // WITH_FORWARDED_MODULE_RULES_CTOR
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			string		sdkFullRoot = Environment.GetEnvironmentVariable("HH_SDK_ROOT");
			if (!String.IsNullOrEmpty(sdkFullRoot))
			{
				// assume that
				IAmDeveloping = true;
			}
			if (IAmDeveloping)
			{
				// maybe not faster, but we want to make sure there is no missing includes
				bFasterWithoutUnity = true;
				PrivatePCHHeaderFile = "Private/EmptyPCH.h";
			}

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"Projects", // IPlugin
				}
			);
		}
	}
}
