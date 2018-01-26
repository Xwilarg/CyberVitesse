#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2017/06/14 06:32 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PS_FONT_METRICS_H__
#define __PS_FONT_METRICS_H__

#include <hh_kernel/include/kr_callbacks.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CFontMetrics : public CRefCountedObject
{
public:
	virtual float	LookupAdvance(char c0, char c1) const { return LookupWidth(c0) + LookupKerning(c0, c1); }
	virtual float	LookupKerning(char c0, char c1) const = 0;
	virtual float	LookupWidth(char c0) const = 0;
	virtual float	LineHeight() const  = 0;

	TCallbackChainUnordered<void(CFontMetrics *self)>	m_OnReloading;
	TCallbackChainUnordered<void(CFontMetrics *self)>	m_OnReloaded;
};
HH_DECLARE_REFPTRCLASS(FontMetrics);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PS_FONT_METRICS_H__

