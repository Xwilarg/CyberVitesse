//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXHelper.h"

#include "PopcornFXSDK.h"
#include <hh_maths/include/hh_maths_primitives.h>

static const TCHAR	*kHumanStr[] = {
	TEXT(" "), TEXT("k"), TEXT("m"), TEXT("g"), TEXT("t")
};

float		HumanReadF(float v, hh_u32 base)
{
	hh_u32		i = 0;
	while (v > base && i < HH_ARRAY_COUNT(kHumanStr) - 1)
	{
		++i;
		v /= base;
	}
	return v;
}

const TCHAR	*HumanReadS(float v, hh_u32 base)
{
	hh_u32		i = 0;
	while (v > base && i < HH_ARRAY_COUNT(kHumanStr) - 1)
	{
		++i;
		v /= base;
	}
	return kHumanStr[i];
}
