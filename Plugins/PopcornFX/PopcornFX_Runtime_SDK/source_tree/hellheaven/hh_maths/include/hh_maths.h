#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2003/07/02 06:40 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PK_MATHS_H__
#define	__PK_MATHS_H__

//----------------------------------------------------------------------------

#include <hh_endian.h>
#include "hh_maths/include/hh_maths_config.h"
#include "hh_maths/include/hh_numeric_traits.h"
#include "hh_maths/include/hh_maths_vector.h"
#include "hh_maths/include/hh_maths_matrix.h"
#include "hh_maths/include/hh_maths_quaternion.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

union	_A32
{
	float	_f32;
	hh_i32	_i32;
	hh_u32	_u32;

	_A32() {}
	_A32(float f) : _f32(f) {}
	_A32(hh_i32 i) : _i32(i) {}
	_A32(hh_u32 u) : _u32(u) {}
};

//----------------------------------------------------------------------------

union	_A64
{
	double	_f64;
	hh_i64	_i64;
	hh_u64	_u64;

	_A64() {}
	_A64(double f) : _f64(f) {}
	_A64(hh_i64 i) : _i64(i) {}
	_A64(hh_u64 u) : _u64(u) {}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PK_MATHS_H__
