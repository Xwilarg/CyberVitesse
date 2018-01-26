#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2005/01/13 0:47 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_VERSION_H__
#define	__HH_VERSION_H__

// include the hh_configurator-generated version header:
#include "hh_version_offset.h"

//----------------------------------------------------------------------------

#define HH_MAKE_VERSION(__major, __minor, __patch)		((__major) * 10000 + (__minor) * 100 + (__patch))

//#define	HH_VERSION_BASE_JAN_2005	5466						// repo migration : cvs -> svn
//#define	HH_VERSION_BASE_AUG_2011	(19447 - 12805)	// 6642		// repo migration : svn -> mercurial
//#define	HH_VERSION_BASE				(HH_VERSION_BASE_JAN_2005 + HH_VERSION_BASE_AUG_2011)

//	use these macros to get the current runtime version.

#define	HH_VERSION					HH_MAKE_VERSION(HH_VERSION_MAJOR, HH_VERSION_MINOR, HH_VERSION_PATCH)
#define	HH_COMPANY_STRING			"Persistant Studios"

#endif

