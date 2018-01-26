#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2015/08/26 13:36 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_INTERPOLABLE_H__
#define __PK_MATHS_INTERPOLABLE_H__

#include "hh_maths/include/hh_maths_config.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Interpolable Array Base
//
//----------------------------------------------------------------------------

class	CInterpolableVectorArray
{
public:
	enum	EInterpolator
	{
		Interpolator_Linear = 0,
		Interpolator_Hermite,
	};
private:
//	typedef	typename TTypeSwitcher_IfTrue<EInterpolator*, const EInterpolator*, _IsConst>::Type	InterpolatorType;
	const EInterpolator					*m_Interpolator;
public:
	HH_FORCEINLINE CInterpolableVectorArray() : m_Interpolator(null) {}
	HH_FORCEINLINE CInterpolableVectorArray(const EInterpolator *mode) : m_Interpolator(mode) {}

	HH_FORCEINLINE void					SetSource(const EInterpolator *mode) { m_Interpolator = mode; }
	HH_FORCEINLINE bool					Valid() const { return m_Interpolator != null; }
	HH_FORCEINLINE EInterpolator		Interpolator() const { HH_ASSERT(Valid()); return *m_Interpolator; }

	bool								operator == (const CInterpolableVectorArray& other) const
	{
		return m_Interpolator == other.m_Interpolator;
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_INTERPOLABLE_H__
