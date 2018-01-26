#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2012/10/29 12:03 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_PLATFORM_X86_64_H__
#define	__HH_PLATFORM_X86_64_H__

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_arch_*.h must not be included outside hellheaven.h
#endif

//----------------------------------------------------------------------------

HH_FORCEINLINE double	HHAbs(double x) { return fabs(x); }
HH_FORCEINLINE float	HHAbs(float x) { return fabsf(x); }

HH_FORCEINLINE double	HHSaturate(double x) { return HHClamp(x, 0.0f, 1.0f); }
HH_FORCEINLINE float	HHSaturate(float x) { return _mm_cvtss_f32(_mm_min_ss(_mm_set_ss(1.0f), _mm_max_ss(_mm_setzero_ps(), _mm_set_ss(x)))); }

//----------------------------------------------------------------------------

HH_FORCEINLINE double	HHFrac(double x) { return fmod(x, 1.0); }
HH_FORCEINLINE float	HHFrac(float x)
{
#if	defined(__AVX__) || defined(__SSE4_1__) // >= SSE4.1

	const __m128	v = _mm_set_ss(x);
	const __m128	floor = _mm_round_ss(v, v, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
	return _mm_cvtss_f32(_mm_sub_ps(v, floor));

#else

#if 0	// TODO: do some unit-tests and use this instead of the horrendous fmodf
	const __m128	v = _mm_set_ss(x);
	const __m128	largestIntForConversion = _mm_set1_ps(2147483647.0f);	// float(0x7FFFFFFF)
#if	(HH_SIMD_SSE_VERSION >= HH_SSE2)
	const __m128	InvSignMask = _mm_castsi128_ps(_mm_cvtsi32_si128(0x7FFFFFFF));
#else
	const __m128	InvSignMask = _mm_set_ss(bit_cast<float>(0x7FFFFFFF));
#endif
	const __m128	floor = _mm_cvtsi32_ss(v, _mm_cvttss_si32(floor));
	const __m128	frac = _mm_sub_ss(v, floor);
	return _mm_cvtss_f32(_mm_and_ps(frac, _mm_cmplt_ss(_mm_and_ps(v, InvSignMask), largestIntForConversion))));
#elif 0
	const __m128	vx = _mm_set_ss(x);
	__m128			floor = vx;
	floor = _mm_cvtsi32_ss(floor, _mm_cvttss_si32(floor));
	return _mm_cvtss_f32(_mm_sub_ss(vx, floor));
#else
	return fmodf(x, 1.0f);
/*	const __m128	largestIntForConversion = _mm_set1_ps(2147483647.0f);	// float(0x7FFFFFFF)
	const __m128	InvSignMask = _mm_castsi128_ps(_mm_cvtsi32_si128(0x7FFFFFFF));//Constants::Uint4_Fp32InvSignMasks().m_Data.f;
	__m128			floor = vx;
	floor = _mm_cvtsi32_ss(floor, _mm_cvttss_si32(floor));
	const __m128	frac = _mm_sub_ss(vx, floor);
	return _mm_and_ps(frac, _mm_cmplt_ss(_mm_and_ps(frac, InvSignMask), largestIntForConversion));*/
#endif
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE float	HHFloor(float x)
{
#if	defined(__AVX__) || defined(__SSE4_1__)	// >= SSE4.1
	const __m128	v = _mm_set_ss(x);
	return _mm_cvtss_f32(_mm_round_ss(v, v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
#else
	return floorf(x);
#endif
}

//----------------------------------------------------------------------------

HH_FORCEINLINE float	HHCeil(float x)
{
#if	defined(__AVX__) || defined(__SSE4_1__)	// >= SSE4.1
	const __m128	v = _mm_set_ss(x);
	return _mm_cvtss_f32(_mm_round_ss(v, v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
#else
	return ceilf(x);
#endif
}

//----------------------------------------------------------------------------

/*HH_FORCEINLINE float	HHSign(float x)
{
	__m128	vx = _mm_set_ss(x);
	__m128	vOne = _mm_set_ss(1.0f);
	__m128	vSign = _mm_castsi128_ps(_mm_cvtsi32_si128(0x80000000);
	return _mm_or_ss(vOne, _mm_and_ss(vx, vSign));
}*/

//----------------------------------------------------------------------------

HH_FORCEINLINE float	HHSqrt(float x) { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x))); }
HH_FORCEINLINE float	HHRSqrtEst(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }

//----------------------------------------------------------------------------

HH_FORCEINLINE float	HHRSqrt(float x)
{
	const __m128	vx = _mm_set_ss(x);
	const __m128	rsqrtEst = _mm_rsqrt_ss(vx);

#if 0
	// unsafe, doesn't handle '0' and 'inf' properly
	const __m128	vr2 = _mm_mul_ss(_mm_mul_ss(vx, rsqrtEst), rsqrtEst);
	return _mm_cvtss_f32(_mm_mul_ss(_mm_mul_ss(rsqrtEst, _mm_set_ss(0.5f)),
									_mm_sub_ss(_mm_set_ss(3.0f), vr2)));
#else
	const __m128	kZero = _mm_setzero_ps();
	const __m128	kInf = _mm_castsi128_ps(_mm_cvtsi32_si128(0x7F800000));

	const __m128	zMask0 = _mm_cmpneq_ss(vx, kInf);	// handle x = inf -> rsqrt(inf) = 0
	const __m128	zMask1 = _mm_cmpneq_ss(vx, kZero);	// handle x = 0   -> rsqrt(0) = inf
	const __m128	zMask = _mm_and_ps(zMask0, zMask1);

	const __m128	vr2 = _mm_mul_ss(_mm_mul_ss(vx, rsqrtEst), rsqrtEst);
	const __m128	sub3vr2 = _mm_sub_ss(_mm_set_ss(3.0f), _mm_and_ps(vr2, zMask));
	return _mm_cvtss_f32(_mm_mul_ss(_mm_mul_ss(rsqrtEst, _mm_set_ss(0.5f)), sub3vr2));
#endif
}

//----------------------------------------------------------------------------

#if 1
HH_FORCEINLINE float	HHRcp(float x) { return 1.0f / x; }
#else
HH_FORCEINLINE float	HHRcp(float x)
{
	const __m128	vx = _mm_set_ss(x);
	const __m128	kZero = _mm_setzero_ps();

	const __m128	rcpEst = _mm_rcp_ss(vx);	// 12 bits precision

	// Here, we have to add special checks to handle correctly rcp(0) and rcp(inf) (some code do rely on this returning the correct value)
	// not doing anything special will procude a 0*inf computation in the newton-raphson iteration below,
	// effectively outputting a NaN instead of the correct result

	const __m128	zMask0 = _mm_cmpneq_ss(vx, kZero);	// handle x = 0   -> 1/0 = inf
	const __m128	zMask1 = _mm_cmpneq_ss(rcpEst, kZero);	// handle x = inf -> 1/inf = 0
	const __m128	zeroMask = _mm_and_ps(zMask0, zMask1);
	// apply one newton-raphson iteration to improve precision up to 23 bits
	const __m128	vr2 = _mm_mul_ss(_mm_mul_ss(vx, rcpEst), rcpEst);
	return _mm_cvtss_f32(_mm_sub_ss(_mm_add_ss(rcpEst, rcpEst), _mm_and_ps(vr2, zeroMask)));
}
#endif

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_i32	PKFloatToIntTrunc(float x)
{
	return _mm_cvtt_ss2si(_mm_set_ss(x));
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_i32	PKFloatToIntFloor(float x)
{
	return _mm_cvt_ss2si(_mm_set_ss(-0.5f + (x + x))) >> 1;
}

//----------------------------------------------------------------------------

HH_FORCEINLINE hh_i32	PKFloatToIntCeil(float x)
{
	return -(_mm_cvt_ss2si(_mm_set_ss(-0.5f - (x + x))) >> 1);
}

//----------------------------------------------------------------------------
#endif	// __HH_PLATFORM_X86_64_H__

