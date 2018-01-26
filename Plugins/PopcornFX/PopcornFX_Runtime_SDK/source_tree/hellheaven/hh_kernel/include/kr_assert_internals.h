#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2008/08/15 14:15 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_ASSERT_INTERNALS_H__
#define __KR_ASSERT_INTERNALS_H__

#include "hh_assert.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

// Matches PK_ASSERT_CATCHER_PARAMETERS PK_ASSERT_CATCHER_ARGUMENTS
#define	__HH_ASSERT_MESSAGE(file, function, line, failed, expanded, message) \
	"PopcornFX Assertion Failed:\n"										\
	"%s(%d)\n"															\
	"\t%s(...)\n"														\
	"\n"																\
	"Message:  %s\n"													\
	"Failed:   %s\n"													\
	"Expanded: %s\n",													\
	file, line, function, message, failed, expanded

//----------------------------------------------------------------------------

struct	SAssertLocation
{
	const char	*file;
	const char	*function;
	int			line;
	const char	*failed;
	hh_u32		flags;
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CAssertDictionnary
{
private:
	static void				_IgnoreLocation(SAssertLocation &location, const char *file, const char *function, int line, const char *failed);
public:
	static bool				IsLocationIgnored(const char *file, const char *function, int line, const char *failed);
	static void				IgnoreLocation(const char *file, const char *function, int line, const char *failed);
	static void				UnignoreLocation(hh_u32 index);
	static void				IterateThroughAllIgnoredLocations(void (*cb)(void *customArg, hh_u32 index, const SAssertLocation &location), void *customArg);
};

//----------------------------------------------------------------------------

#if	(HH_ASSERTS_IN_DEBUG != 0) || (HH_ASSERTS_IN_RELEASE != 0)
Assert::EResult		HHAssertImpl(PK_ASSERT_CATCHER_PARAMETERS);
#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_ASSERT_INTERNALS_H__
