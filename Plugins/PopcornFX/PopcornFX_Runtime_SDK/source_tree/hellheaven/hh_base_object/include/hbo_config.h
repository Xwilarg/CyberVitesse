#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2012/02/26 14:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HBO_CONFIG_H__
#define	__HBO_CONFIG_H__

//----------------------------------------------------------------------------

// use this define to build without HBO meta-data
#ifndef	HH_BASE_OBJECT_STRIP_METADATA
#	if	defined(HH_DESKTOP)
#		define	HH_BASE_OBJECT_STRIP_METADATA		0	// metadata not stripped desktop targets, we need them for editors and assetbaker
#	else
#		define	HH_BASE_OBJECT_STRIP_METADATA		1	// by default, metadata stripped on consoles
#	endif
#endif

#ifndef	HH_BASE_OBJECT_BUILD_SERIALIZER_TEXT
#	define	HH_BASE_OBJECT_BUILD_SERIALIZER_TEXT	1
#endif

#ifndef	HH_BASE_OBJECT_BUILD_SERIALIZER_BINARY
#	define	HH_BASE_OBJECT_BUILD_SERIALIZER_BINARY	1
#endif

#ifndef	HH_BASE_OBJECT_HANDLE_UNKNOWN
#	define	HH_BASE_OBJECT_HANDLE_UNKNOWN			1
#endif

//----------------------------------------------------------------------------

#endif	// __HBO_CONFIG_H__
