#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2008/06/26 10:25 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HH_TOOLKIT_VERSION_H__
#define __HH_TOOLKIT_VERSION_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Dll version checker
//
//----------------------------------------------------------------------------

struct	SDllVersion
{
	hh_u32	Major;
	hh_u32	Minor;
	hh_u32	Patch;
	hh_u32	RevID;
	bool	Debug;

	SDllVersion();
	SDllVersion(hh_u32 major, hh_u32 minor, hh_u32 patch, hh_u32 revId, bool debug);

	static SDllVersion	Invalid;

	bool	Valid() const { return Major != 0 || Minor != 0 || Patch != 0 || RevID != 0; }
	bool	operator == (const SDllVersion &other) const { return Major == other.Major && Minor == other.Minor && Debug == other.Debug; }
	bool	operator != (const SDllVersion &other) const { return !(*this == other); }

	bool	StrictlyEqual(const SDllVersion &other) const { return Major == other.Major && Minor == other.Minor && Patch == other.Patch && RevID == other.RevID && Debug == other.Debug; }
};

//----------------------------------------------------------------------------

SDllVersion	HHTKGetFileVersionInfo(const char *path);

#if	defined(HH_WINDOWS)
SDllVersion	HHTKGetVersionInfo(HINSTANCE self);
#else
SDllVersion	HHTKGetVersionInfo(void *self);
#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HH_TOOLKIT_VERSION_H__
