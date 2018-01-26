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

#ifndef	__IM_CONFIG_H__
#define	__IM_CONFIG_H__

//----------------------------------------------------------------------------

#ifndef	HH_IMAGING_BUILD_DOWNSAMPLERS
#	if	defined(HH_DESKTOP)
#		define	HH_IMAGING_BUILD_DOWNSAMPLERS	1	// enable downsamplers for PC/Max/Linux
#	else
#		define	HH_IMAGING_BUILD_DOWNSAMPLERS	0	// disable on mobile/consoles
#	endif
#endif

#ifndef	HH_IMAGING_BUILD_COMPRESSORS
#	if	defined(HH_DESKTOP)
#		define	HH_IMAGING_BUILD_COMPRESSORS	1	// enable compressors for PC/Max/Linux
#	else
#		define	HH_IMAGING_BUILD_COMPRESSORS	0	// disable on mobile/consoles
#	endif
#endif

#ifndef	HH_IMAGING_BUILD_TRANSCODERS
#	if	defined(HH_DESKTOP)
#		define	HH_IMAGING_BUILD_TRANSCODERS	1	// enable transcoders for PC/Max/Linux
#	else
#		define	HH_IMAGING_BUILD_TRANSCODERS	0	// disable on mobile/consoles
#	endif
#endif

//----------------------------------------------------------------------------
// Format-specific compile flags:

#ifndef	PK_IMAGING_BUILD_DECODER_BCN
#	define	PK_IMAGING_BUILD_DECODER_BCN		1	// enable BCN decompressors everywhere
#endif

#ifndef	PK_IMAGING_BUILD_DECODER_ETC
#	if	defined(HH_ANDROID) || defined(HH_IOS)
#		define	PK_IMAGING_BUILD_DECODER_ETC	1	// enable ETC1/ETC2 decompressor on mobile
#	else
#		define	PK_IMAGING_BUILD_DECODER_ETC	0
#	endif
#endif

#ifndef	PK_IMAGING_BUILD_DECODER_PVRTC
#	if	defined(HH_IOS)
#		define	PK_IMAGING_BUILD_DECODER_PVRTC	1	// enable PVRTC decompressor on iOS
#	else
#		define	PK_IMAGING_BUILD_DECODER_PVRTC	0
#	endif
#endif

//----------------------------------------------------------------------------

#endif	// __IM_CONFIG_H__
