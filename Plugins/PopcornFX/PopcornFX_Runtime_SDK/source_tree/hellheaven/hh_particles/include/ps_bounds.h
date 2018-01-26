#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2004/03/25 10:47 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_BOUNDS_H__
#define	__PS_BOUNDS_H__

#include <hh_maths/include/hh_maths_primitives.h>
#include <hh_kernel/include/kr_timers.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CSmartCachedBounds
{
private:
	CAABB					m_ExactBounds;
	CAABB					m_CachedBounds;
	CTimer					m_UpdateTimer;
	float					m_LastTime;

public:
	CSmartCachedBounds();
	~CSmartCachedBounds();

	void					Reset();

	void					SetExactBounds(const CAABB &box);
	void					AddToExactBounds(const CAABB &merge);

	const CAABB				&ExactBounds() const { return m_ExactBounds; }
	const CAABB				&CachedBounds() const { return m_CachedBounds; }

	void					Update();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_BOUNDS_H__
