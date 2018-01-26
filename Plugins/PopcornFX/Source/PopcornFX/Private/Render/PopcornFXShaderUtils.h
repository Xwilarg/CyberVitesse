#pragma once

#include "PopcornFXMinimal.h"

#if (ENGINE_MINOR_VERSION >= 17)
#	define	PKUE_SHADER_PATH(__name)	"/Plugin/PopcornFX/Private/" TEXT(__name) TEXT(".ush")
#else
#	define	PKUE_SHADER_PATH(__name)	__name
#endif
