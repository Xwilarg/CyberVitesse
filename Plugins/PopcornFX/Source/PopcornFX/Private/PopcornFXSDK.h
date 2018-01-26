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

#include "CoreMinimal.h"

#include "Build.h"
#include "Platform.h"

#include "Runtime/Launch/Resources/Version.h"
#if !defined(ENGINE_MAJOR_VERSION)
#	error no ENGINE_MAJOR_VERSION
#endif
#if (ENGINE_MAJOR_VERSION != 4)
#	error invalid ENGINE_MAJOR_VERSION
#endif
#if (ENGINE_MINOR_VERSION < 15)
#	error PopcornFX Plugin only supported in UE >= 4.15
#endif

#if PLATFORM_WINDOWS
#	if (ENGINE_MINOR_VERSION >= 15)
#		include "MinimalWindowsApi.h"
#	endif
#	include "PreWindowsApi.h"
#	include "MinWindows.h"
#	include "PostWindowsApi.h"
#elif PLATFORM_XBOXONE
#	include "XboxOnePreApi.h"
#	include "XboxOneMinApi.h"
#	include "XboxOnePostApi.h"
#endif

#if PLATFORM_WINDOWS
#	ifdef WINDOWS_PLATFORM_TYPES_GUARD
#		include "HideWindowsPlatformTypes.h"
#	endif
#	include "AllowWindowsPlatformTypes.h"
#elif PLATFORM_XBOXONE
#	ifdef XBOXONE_PLATFORM_TYPES_GUARD
#		include "XboxOneHidePlatformTypes.h"
#	endif
#	include "XboxOneAllowPlatformTypes.h"
#endif

#ifdef GetObject
#	undef GetObject
#endif

#undef	PV_MODULE_INIT_NAME
#undef	PV_MODULE_NAME
#undef	PV_MODULE_SYM
#define	PV_MODULE_INIT_NAME	"UE4 PopcornFX Plugin"
#define	PV_MODULE_NAME		"UE4"
#define	PV_MODULE_SYM		UE4

#include <hellheaven_api/include/hh_precompiled_default.h>

// disable non-default warning enabled by PopcornFX.
#if defined(_MSC_VER)
#	pragma warning(disable : 4061 4062)
#endif

HH_LOG_MODULE_DEFINE();

#undef HH_TODO
#define HH_TODO(...)
#undef HH_FIXME
#define HH_FIXME(...)

#ifdef HH_NULL_AS_VARIABLE
using PopcornFX:: null;
#endif

// using usefull types unlikely to collide with UE types
using PopcornFX:: hh_ureg;
using PopcornFX:: hh_ireg;
using PopcornFX:: hh_u64;
using PopcornFX:: hh_i64;
using PopcornFX:: hh_u32;
using PopcornFX:: hh_i32;
using PopcornFX:: hh_u16;
using PopcornFX:: hh_i16;
using PopcornFX:: hh_u8;
using PopcornFX:: hh_i8;

using PopcornFX:: CFloat4x4;
using PopcornFX:: CFloat4;
using PopcornFX:: CFloat3;
using PopcornFX:: CFloat2;
using PopcornFX:: CFloat1;
using PopcornFX:: CInt4;
using PopcornFX:: CInt3;
using PopcornFX:: CInt2;
using PopcornFX:: CInt1;
using PopcornFX:: CUint4;
using PopcornFX:: CUint3;
using PopcornFX:: CUint2;
using PopcornFX:: CUint1;

using PopcornFX:: TMemoryView;
using PopcornFX:: TStridedMemoryView;
using PopcornFX:: TStridedMemoryViewWithFootprint;

// /!\ at least those types collides:
// TArray TRefPtr TWeakPtr

// FastDelegate warning about reinterpret_cast
#if PLATFORM_WINDOWS || PLATFORM_XBOXONE
#	pragma warning( disable : 4191 )
#endif

#include "PopcornFXMinimal.h"
#include "PopcornFXHelper.h"
