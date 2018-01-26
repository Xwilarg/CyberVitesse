#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2007/03/23 4:40 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifdef	__HH_OS_H__
#	error	multiple inclusions of hh_os.h
#else
#define	__HH_OS_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_os.h must not be included outside hellheaven.h
#endif

//----------------------------------------------------------------------------
// we need these to not be defined beforehand

#define	HH_OS_WINDOWS	0	// Windows
#define	HH_OS_WINRT		1	// WindowsRT
#define	HH_OS_LINUX		2	// Linux
#define	HH_OS_MACOSX	3	// MacOSX
#define	HH_OS_PS3		4	// PlayStation 3
#define	HH_OS_PSP2		5	// PlayStation Vita
//#define	HH_OS_CAFE		6	// Wii-U
#define	HH_OS_NX		7	// Nintendo Switch
#define	HH_OS_ANDROID	8	// Android
#define	HH_OS_ORBIS		9	// PlayStation 4
#define	HH_OS_IOS		10	// iOs

//----------------------------------------------------------------------------
//
//	NOTE: now, rather than using the following directives to detect win32 targets:
//	#ifdef	WIN32
//	one should use:
//	#ifdef	HH_WIN32
//
//----------------------------------------------------------------------------

//#if	(defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(__win64__))
//#	define HH_WIN64
//#endif

#if ((defined(_XBOX_VER) && _XBOX_VER >= 200) || defined(XBOX) || defined(_XBOX))

#	define	HH_X360		// use this one, not 'XBOX'
#	define	HH_OS		HH_OS_WINDOWS

#elif (defined(_XBOX_ONE) || defined(_DURANGO) || defined(DURANGO))

#	define	HH_WINDOWS
#	define	HH_DURANGO
#	define	HH_OS		HH_OS_WINDOWS

#	if !defined(WINAPI_FAMILY) || ((WINAPI_FAMILY != WINAPI_FAMILY_TV_TITLE) && (WINAPI_FAMILY != WINAPI_FAMILY_TV_APP))
#		error	Building for Durango, but WINAPI_FAMILY not defined or set to neither TV_TITLE nor TV_APP.
#	endif

#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__win32__))

#	define	HH_WINDOWS	// defined for all windows-family OSes

#	if defined(WINAPI_FAMILY)
#		if (WINAPI_FAMILY == WINAPI_FAMILY_APP) || (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#			define	HH_WINRT
#			define	HH_OS	HH_OS_WINRT
#		elif (WINAPI_FAMILY == WINAPI_PARTITION_DESKTOP) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#			define	HH_WIN32	// use this one, not 'WIN32'
#			define	HH_OS	HH_OS_WINDOWS
#		elif (WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE) || (WINAPI_FAMILY == WINAPI_FAMILY_TV_APP)
#			error	should have been caught above in the ifdef _XBOX_ONE
#			define	HH_DURANGO
#			define	HH_OS	HH_OS_WINDOWS
#		endif
#	else
#		define	HH_WIN32	// use this one, not 'WIN32'
#		define	HH_OS		HH_OS_WINDOWS
#	endif

#elif	defined(__ANDROID__)

#	define HH_ANDROID
#	define HH_OS		HH_OS_ANDROID

#elif (defined(LINUX) || defined(_LINUX) || defined(__LINUX__) || defined(__linux__))

#	define	HH_LINUX	// use this one, not 'LINUX'
#	define	HH_OS		HH_OS_LINUX

#elif (defined(MACOSX) || defined(__APPLE__) || defined(__apple__) || defined(macosx) || defined(MACOS_X))

#	include <TargetConditionals.h>

#	if TARGET_IPHONE_SIMULATOR == 1
#		error "iPhone simulator not supported"
#	elif TARGET_OS_IPHONE == 1
#		define	HH_IOS
#		define	HH_OS		HH_OS_IOS
#	elif TARGET_OS_MAC
#		define	HH_MACOSX	// use this one, not 'MACOSX'
#		define	HH_OS		HH_OS_MACOSX
#	endif

#elif defined(__CELLOS_LV2__)

#	define	HH_PS3
#	define	HH_OS		HH_OS_PS3
#	if	!defined(__PPU__) && !defined(__SPU__)
#		error	Building for cell-os lv2, but neither for SPU or PPU ?
#	endif

#elif defined(__ORBIS__)

#	define	HH_ORBIS
#	define	HH_OS		HH_OS_ORBIS

#elif	defined(__psp2__)

#	define	HH_PSP2
#	define	HH_OS		HH_OS_PSP2

#elif	defined(__NX__) || (defined(__horizon__) && (__horizon__ == 1))

#	define	HH_NX
#	define	HH_OS		HH_OS_NX

#else
#	error unsupported target, see ./hellheaven/include/hh_os.h
#endif

//----------------------------------------------------------------------------
// helper macro used to disable some stuff by default on consoles or mobile
// platforms. (ie: non-desktop)
//----------------------------------------------------------------------------

#if	defined(HH_PS3) || \
	defined(HH_PSP2) || \
	defined(HH_ORBIS) || \
	defined(HH_X360) || \
	defined(HH_DURANGO)
#	define	HH_CONSOLE
#endif

#if	defined(HH_WINDOWS) || \
	defined(HH_MACOSX) || \
	defined(HH_LINUX)
#	define	HH_DESKTOP
#endif

#if	defined(HH_ANDROID) || \
	defined(HH_IOS) || \
	defined(HH_PSP2) || \
	defined(HH_NX)
#	define	HH_MOBILE
#endif

//----------------------------------------------------------------------------

#if		(HH_OS == HH_OS_WINDOWS)
#	define	HH_OS_NAME		"Windows"
#elif	(HH_OS == HH_OS_WINRT)
#	define	HH_OS_NAME		"WindowsRT"
#elif	(HH_OS == HH_OS_LINUX)
#	define	HH_OS_NAME		"Linux"
#elif	(HH_OS == HH_OS_MACOSX)
#	define	HH_OS_NAME		"MacOSX"
#elif	(HH_OS == HH_OS_PS3)
#	define	HH_OS_NAME		"PlayStation3"
#elif	(HH_OS == HH_OS_PSP2)
#	define	HH_OS_NAME		"PlayStationVita"
#elif	(HH_OS == HH_OS_ANDROID)
#	define	HH_OS_NAME		"Android"
#elif	(HH_OS == HH_OS_IOS)
#	define HH_OS_NAME		"iOS" // Refine by sdk version (Availability.h)?
#elif	(HH_OS == HH_OS_ORBIS)
#	define	HH_OS_NAME		"PlayStation4"
#elif	(HH_OS == HH_OS_NX)
#	define	HH_OS_NAME		"NintendoSwitch"
#else
#	error no HH_OS_NAME defined
#endif

//----------------------------------------------------------------------------

#endif	/* __HH_OS_H__ */
