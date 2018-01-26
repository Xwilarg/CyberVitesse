#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2003/07/02 0:37 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_OS_WIN32_H__
#define	__HH_OS_WIN32_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_os_win32.h must not be included outside hellheaven.h
#endif

//----------------------------------------------------------------------------
__PK_API_END	// temporarily leave the PopcornFX namespace

// we don't want the new 'secure' CRT implementations ! just use the classic, standard versions that work on other compilers/platforms too without need for specific code.
// protect from redefinition: hhapi_precompiled.h

#if !defined(_CRT_SECURE_NO_WARNINGS)
#	define	_CRT_SECURE_NO_WARNINGS
#endif
#if !defined(_CRT_SECURE_NO_DEPRECATE)
#	define	_CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef	POINTER_64
#	define POINTER_64	__ptr64
#endif

#ifndef	_WINDOWS_	// don't re-include windows.h if it was already manually included

#	ifndef	WIN32_LEAN_AND_MEAN
#		define	WIN32_LEAN_AND_MEAN			1
#		define	HH_WIN32_LEAN_AND_MEAN
#	endif
#	ifndef	WIN32_EXTRA_LEAN
#		define	WIN32_EXTRA_LEAN
#		define	HH_WIN32_EXTRA_LEAN
#	endif
#	ifndef	VC_EXTRALEAN
#		define	VC_EXTRALEAN
#		define	HH_VC_EXTRALEAN
#	endif

// these should already be ignored by VC_EXTRALEAN
#	if	!defined(HH_WINRT)

#		define	NOSERVICE
#		define	NOMCX
#		define	NOIME
#		define	NOSOUND
#		define	NOCOMM
#		define	NOKANJI
#		define	NORPC
#		define	NOPROXYSTUB
#		define	NOIMAGE
#		define	NOTAPE

// additional trims
#		define	NOGDICAPMASKS
#		define	NOVIRTUALKEYCODES
#		define	NOWINMESSAGES
#		define	NOWINSTYLES
#		define	NOSYSMETRICS
#		define	NOMENUS
#		define	NOICONS
#		define	NOKEYSTATES
#		define	NOSYSCOMMANDS
#		define	NORASTEROPS
#		define	NOSHOWWINDOW
#		define	OEMRESOURCE
#		define	NOATOM
#		define	NOCLIPBOARD
#		define	NOCOLOR
#		define	NOCTLMGR
#		define	NODRAWTEXT
#		define	NOGDI
#		define	NOKERNEL
//#		define	NOUSER		// Don't. Breaks D3D11 builds
#		define	NONLS
#		define	NOMB
#		define	NOMEMMGR
#		define	NOMETAFILE
#		define	NOMINMAX
//#		define	NOMSG		// Don't. Breaks D3D11 builds
#		define	NOOPENFILE
#		define	NOSCROLL
#		define	NOTEXTMETRIC
#		define	NOWH
#		define	NOWINOFFSETS
#		define	NOHELP
#		define	NOPROFILER
#		define	NODEFERWINDOWPOS

#	endif	// !defined(HH_WINRT)

#	pragma warning(push)
#	pragma warning(disable : 4668)
#	include	<windows.h>
#	pragma warning(pop)

#	ifdef	HH_WIN32_LEAN_AND_MEAN
#		undef	WIN32_LEAN_AND_MEAN
#		undef	HH_WIN32_LEAN_AND_MEAN
#	endif
#	ifdef	WIN32_EXTRA_LEAN
#		undef	WIN32_EXTRA_LEAN
#		undef	HH_WIN32_EXTRA_LEAN
#	endif
#	ifdef	VC_EXTRALEAN
#		undef	VC_EXTRALEAN
#		undef	HH_VC_EXTRALEAN
#	endif

#endif	// _WINDOWS_

__PK_API_BEGIN
//----------------------------------------------------------------------------

#define	HH_DLL_EXTENSION		"dll"

//----------------------------------------------------------------------------

#ifdef	HH_COMPILER_MSVC

#undef	strcasecmp
#undef	strncasecmp

#define	strcasecmp	_stricmp
#define	strncasecmp	_strnicmp

// thanks to the new math.h header coming with VC2005, we have:
//
// #define fmodf(x,y)  ((float)fmod((double)(x), (double)(y)))
// #define logf(x)     ((float)log((double)(x)));			<------- a semicolon !!!
// #define log10f(x)   ((float)log10((double)(x)))
//
// niiice...

#if	(HH_COMPILER_VERSION <= 1400)
#	undef	logf
#	define	logf(v)		((float)(log((double)(v))))
#endif

// XBox-One
#if defined(HH_DURANGO)
#	include <xdk.h>

#	ifndef _XDK_VER
#		error XDK version not found
#	endif
#endif // defined(HH_DURANGO)

#endif

//----------------------------------------------------------------------------
#endif	/* __HH_OS_WIN32_H__ */

