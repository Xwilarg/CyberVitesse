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

#ifndef	__CP_CONFIG_H__
#define	__CP_CONFIG_H__

//----------------------------------------------------------------------------

#ifndef	PK_COMPILER_BUILD_COMPILER
#	define	PK_COMPILER_BUILD_COMPILER	1
#endif

#ifndef	HH_COMPILER_ENABLE_TYPE_CHECKS_FOR_CPP_TO_SCRIPT_CALLS
#	if	defined(HH_DEBUG)
#		define	HH_COMPILER_ENABLE_TYPE_CHECKS_FOR_CPP_TO_SCRIPT_CALLS	1
#	else
#		define	HH_COMPILER_ENABLE_TYPE_CHECKS_FOR_CPP_TO_SCRIPT_CALLS	0
#	endif
#endif

//----------------------------------------------------------------------------

#endif	// __CP_CONFIG_H__
