#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2007/11/21 9:30 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_CONFIG_H__
#define __PK_MATHS_CONFIG_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

#ifndef	PK_HAS_DETERMINISM
#	define	PK_HAS_DETERMINISM	0
#endif

//----------------------------------------------------------------------------
// FP16

#define	USE_CORRECT_FP32FP16_ROUNDING
#define	FP3216_DENORMALS_FTZ

//----------------------------------------------------------------------------
// Interpolators

#ifndef	HH_MATHS_INTERPOLATORS_ALLOW_QUATERNIONS
#	define	HH_MATHS_INTERPOLATORS_ALLOW_QUATERNIONS	1
#endif

//----------------------------------------------------------------------------
// PRNG

#define	PK_RANDOM_MT_USES_SFMT
//#define	PK_RANDOM_MT_USES_FP_CONVERSION_FOR_INTEGER_MODULUS

#ifndef	PK_RANDOM_MT_USES_SFMT
//#	define	PK_RANDOM_MT_USE_TEMPERING
#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_CONFIG_H__
