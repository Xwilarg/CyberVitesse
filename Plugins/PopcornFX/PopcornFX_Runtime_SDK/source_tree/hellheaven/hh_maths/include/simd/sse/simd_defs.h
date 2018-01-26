#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2009/07/22 19:11 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_SIMD_SSE_DEFS_H__
#define __PK_MATHS_SIMD_SSE_DEFS_H__

#ifndef __PK_MATHS_SIMD_DEFS_H__
#	error	cannot include simd/sse/simd_defs.h without hh_maths_simd_defs.h
#endif

//----------------------------------------------------------------------------
//
//	SSE version
//
//----------------------------------------------------------------------------

#define	HH_SSE			10
#define	HH_SSE2			20
#define	HH_SSE3			30
#define	HH_SSSE3		33
#define	HH_SSE4			40
#define	HH_SSE41		HH_SSE4
#define	HH_SSE42		42
#define	HH_SSE4A		44
#define	HH_SSE5			50
#define	HH_AVX			100
#define	HH_AVX2			110		// VC2012 & GCC 4.7.0
#define	HH_AVX512		200

//----------------------------------------------------------------------------

//#define	HH_SIMD_F16C
//#define	HH_SIMD_FMA3
//#define	HH_SIMD_FMA4

//----------------------------------------------------------------------------

#if		(HH_CPU_MODEL == HH_CPU_JAGUAR)
// AMD Jaguar (PlayStation 4 | Xbox-One)
#	ifndef	HH_SIMD_SSE_VERSION
#		define	HH_SIMD_SSE_VERSION				HH_AVX
#		if !defined(HH_SIMD_F16C)
#			define	HH_SIMD_F16C					// half <-> float converts through _mm_cvtph_ps and _mm_cvtps_ph
#		endif
#	endif
#	ifndef	HH_SIMD_SSE_COMPILABLE_VERSION
#		define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX
#	endif

#elif	defined(HH_COMPILER_MSVC)
//	Visual Studio
#	ifndef	HH_SIMD_SSE_VERSION
#		if	defined(__AVX2__)
#			define	HH_SIMD_SSE_VERSION			HH_AVX2
#		elif	defined(__AVX__)
#			define	HH_SIMD_SSE_VERSION			HH_AVX
#		elif	defined(_M_IX86_FP)
#			if	(_M_IX86_FP >= 2)	// SSE2
#				define	HH_SIMD_SSE_VERSION		HH_SSE2
#			else	// 1 == SSE, 0 == fp
#				define	HH_SIMD_SSE_VERSION		HH_SSE
#			endif
#		else
#			define	HH_SIMD_SSE_VERSION			HH_SSE2
#		endif
#	endif

#	ifndef	HH_SIMD_SSE_COMPILABLE_VERSION
#		if	defined(__AVX512__) || defined(__AVX512F__)	// MSVC won't define this automatically, even vs2015
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX512
#		elif	(_MSC_VER >= 1700) || defined(__AVX2__)	// if greater or equal to Visual Studio 2012
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX2
#		elif	(_MSC_VER >= 1700) || defined(__AVX__)	// if greater than Visual Studio 2010 (vs2010 CAN compile AVX intrinsics, but doesn't support /arch:AVX, so can't generate proper VEX code)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX
#		else
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSE42
#		endif
#	endif

#	if	(_MSC_VER >= 1700)	// from Visual Studio 2012, can compile FMA3 and FMA4
#		define	HH_SIMD_CAN_COMPILE_FMA3	1
#		define	HH_SIMD_CAN_COMPILE_FMA4	1
#	endif

#else
//	GCC/ICC
#	ifndef	HH_SIMD_SSE_COMPILABLE_VERSION
#		if		defined(__AVX512__) || defined(__AVX512F__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX512
#		elif	defined(__AVX2__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX2
#		elif	defined(__AVX__)// || defined (__AES__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_AVX
#		elif	defined(__SSE4_2__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSE42
#		elif	defined(__SSE4_1__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSE41
#		elif	defined(__SSSE3__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSSE3
#		elif	defined(__SSE3__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSE3
#		elif	defined(__SSE2__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSE2
#		elif	defined(__SSE__)
#			define	HH_SIMD_SSE_COMPILABLE_VERSION	HH_SSE
#		endif
#	endif

#	ifndef	HH_SIMD_SSE_VERSION
#		if (HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSE2)
#			define	HH_SIMD_SSE_VERSION			HH_SSE2
#		else
#			define	HH_SIMD_SSE_VERSION			HH_SIMD_SSE_COMPILABLE_VERSION
#		endif
#	endif

#	if	defined(__FMA__)
#		define	HH_SIMD_CAN_COMPILE_FMA3	1
#		include <fmaintrin.h>
#	endif

#	if	defined(__FMA4__)
#		define	HH_SIMD_CAN_COMPILE_FMA4	1
#		include <fma4intrin.h>
#	endif
#endif

//----------------------------------------------------------------------------

#if !defined(HH_SIMD_SSE_VERSION) || !defined(HH_SIMD_SSE_COMPILABLE_VERSION)
#	error	internal SIMD SSE config error: missing define
#endif

#if	(HH_SIMD_SSE_VERSION < HH_SSE)
#	error	cannot build hh_maths with SIMD less than SSE1
#endif

//----------------------------------------------------------------------------

#define	HH_SIMD_HAS_VECTOR4				1

#if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
#	define	HH_SIMD_HAS_VECTOR8			1
#else
#	define	HH_SIMD_HAS_VECTOR8			0
#endif

//----------------------------------------------------------------------------

#ifndef	HH_SIMD_MATRIX_VECTOR_WIDTH
#	if	(HH_SIMD_SSE_VERSION >= HH_AVX512)	// the base compile is AVX512
#		define	HH_SIMD_MATRIX_VECTOR_WIDTH	128//512
#	elif	(HH_SIMD_SSE_VERSION >= HH_AVX)	// the base compile is AVX
#		define	HH_SIMD_MATRIX_VECTOR_WIDTH	128//256
#	else								// the base compile is SSE
#		define	HH_SIMD_MATRIX_VECTOR_WIDTH	128
#	endif
#endif

#if	!defined(HH_SIMD_CAN_COMPILE_FMA3)
#	define	HH_SIMD_CAN_COMPILE_FMA3	0
#endif

#if	!defined(HH_SIMD_CAN_COMPILE_FMA4)
#	define	HH_SIMD_CAN_COMPILE_FMA4	0
#endif

//----------------------------------------------------------------------------
// include the correct header based on the max SIMD compilable version:

#if		(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX2)
#	if	defined(HH_COMPILER_GCC)
#		include <x86intrin.h>	// includes appropriate headers for whatever is defined in the compiler's commandline
#	else
#		include <immintrin.h>
#	endif
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
#	include <immintrin.h>	// AVX
//#	include <wmmintrin.h>	// AES
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSE42)
#	include <nmmintrin.h>
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSE41)
#	include <smmintrin.h>
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSSE3)
#	include <tmmintrin.h>
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSE3)
#	include <pmmintrin.h>
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSE2)
#	include <emmintrin.h>
#elif	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_SSE)
#	include <xmmintrin.h>
#endif

//----------------------------------------------------------------------------

// SSE
HH_STATIC_ASSERT(HH_SIMD_SSE == 2);
#if	0//defined(HH_COMPILER_MSVC)
#	define	HH_SIMD_NATIVE_VECREG_DECL__2		__declspec(intrin_type) HH_ALIGN(0x10)
#	define	HH_SIMD_NATIVE_SCAREG_DECL__2		__declspec(intrin_type) HH_ALIGN(0x10)
#else
#	define	HH_SIMD_NATIVE_VECREG_DECL__2		HH_ALIGN(0x10)
#	define	HH_SIMD_NATIVE_SCAREG_DECL__2		HH_ALIGN(0x10)
#endif

#if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
#	define	HH_SIMD_NATIVE_VECREG8_DECL__2		HH_ALIGN(0x20)	// 256-bit vector
#endif
#if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX512)
#	define	HH_SIMD_NATIVE_VECREG16_DECL__2		HH_ALIGN(0x40)	// 512-bit vector
#endif

#if	(HH_SIMD_MATRIX_VECTOR_WIDTH == 512)		// matrix uses 512-bit vectors
#	define	HH_SIMD_NATIVE_MATREG_DECL__2		HH_ALIGN(0x40)
#elif	(HH_SIMD_MATRIX_VECTOR_WIDTH == 256)	// matrix uses 256-bit vectors
#	define	HH_SIMD_NATIVE_MATREG_DECL__2		HH_ALIGN(0x20)
#elif	(HH_SIMD_MATRIX_VECTOR_WIDTH == 128)	// matrix uses 128-bit vectors
#	define	HH_SIMD_NATIVE_MATREG_DECL__2		HH_ALIGN(0x10)
#else
#	error
#endif

#if	defined(HH_COMPILER_CLANG) || defined(HH_COMPILER_ICC) || defined(HH_COMPILER_GCC) || \
	(defined(HH_CPU_64BITS) && defined(HH_COMPILER_MSVC) && HH_COMPILER_VERSION >= 1800)
#	define	HH_SIMD_PASS_VECTORS_BY_VALUE	1
#else
#	define	HH_SIMD_PASS_VECTORS_BY_VALUE	0
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

#define	HH_MM_SHUF_CODE_x	0
#define	HH_MM_SHUF_CODE_y	1
#define	HH_MM_SHUF_CODE_z	2
#define	HH_MM_SHUF_CODE_w	3

#define	HH_MM_SHUFFLE(_x, _y, _z, _w)		(HH_MM_SHUF_CODE_ ## _w << 6) | (HH_MM_SHUF_CODE_ ## _z << 4) | (HH_MM_SHUF_CODE_ ## _y << 2) | (HH_MM_SHUF_CODE_ ## _x)
#define	HH_MM_SHUFFLE_ID(_x, _y, _z, _w)	(_w << 6) | (_z << 4) | (_y << 2) | (_x)

//----------------------------------------------------------------------------

union	__m128fi
{
	__m128	f;
	__m128i	i;
};

//----------------------------------------------------------------------------

#if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
union	__m256fi
{
	__m256	f;
	__m256i	i;
};
#endif

#if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX512)
union	__m512fi
{
	__m512	f;
	__m512i	i;
};
#endif

//----------------------------------------------------------------------------

namespace	SIMD_SSE
{
	namespace	FullPrecision {}
	namespace	LowPrecision {}

	namespace	DefaultPrecision = FullPrecision;

	extern const HH_ALIGN(0x10) float	_Constant_Fp32_Sqrt05[4];
	extern const HH_ALIGN(0x10) float	_Constant_Fp32_1dot5Over_Sqrt05[4];

	class	Scalar;
	class	Float4;
#if	(HH_SIMD_PASS_VECTORS_BY_VALUE != 0)
	typedef const Scalar	ScalarArg;
	typedef const Float4	VectorArg;
#else
	typedef const Scalar	&ScalarArg;
	typedef const Float4	&VectorArg;
#endif

	// base scalar class
	typedef __m128fi		ScalarBase;
	HH_STATIC_ASSERT(sizeof(ScalarBase) == 16);

	// base vector class
	typedef __m128fi		VectorBase;
	HH_STATIC_ASSERT(sizeof(VectorBase) == 16);

#if	(HH_SIMD_HAS_VECTOR8 != 0)
	class	Float8;
#	if	(HH_SIMD_PASS_VECTORS_BY_VALUE != 0)
	typedef const Float8	Vector8Arg;
#	else
	typedef const Float8	&Vector8Arg;
#	endif

	typedef __m256fi		Vector8Base;
	HH_STATIC_ASSERT(sizeof(Vector8Base) == 32);
#endif

	// base matrix class
#if	(HH_SIMD_MATRIX_VECTOR_WIDTH == 512)	// if the base compile is AVX512
	struct HH_ALIGN(0x10)	Float4x4Base
	{
		__m512fi			_AxAyAzAw;
	};
#elif	(HH_SIMD_MATRIX_VECTOR_WIDTH == 256)
	struct HH_ALIGN(0x10)	Float4x4Base
	{
		__m256fi			_AxAy;
		__m256fi			_AzAw;
	};
#elif	(HH_SIMD_MATRIX_VECTOR_WIDTH == 128)
	struct HH_ALIGN(0x10)	Float4x4Base
	{
		__m128fi			_Ax;
		__m128fi			_Ay;
		__m128fi			_Az;
		__m128fi			_Aw;
	};
#else
#	error Unsupported SIMD matrix internal HW width
#endif
	HH_STATIC_ASSERT(sizeof(Float4x4Base) == 64);
}	// namespace	SIMD_SSE


//----------------------------------------------------------------------------
//	handling of '_mm_undefined_*()' that returns an uninitialized vector.
//	this is VERY useful. we've wanted to be able to use such an intrinsic for a long time,
//	and apparently since AVX in Intel-Compiler, you can use that intrinsic.
//	only on Intel-Compiler though :(
//	emulate the behavior as best as we can on other compilers:
//----------------------------------------------------------------------------

#if	defined(HH_COMPILER_INTEL) && (HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)

	// ICC folks : awesome job ! \o/

#elif	defined(HH_COMPILER_MSVC) && (HH_COMPILER_VERSION >= 1700)	// vs2012 and above

	// vs2012 : not so awesome...
	// has _mm_undefined_ps() #define'd to _mm_setzero_ps(), which backend doesn't optimize away
	// when using partial loads/sets that end-up setting whole vectors. ex: movl/movh/movhl/movlh

#elif	defined(HH_COMPILER_GCC) && (GCC_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(4, 9, 0))

	// gcc 4.9.0 has _mm_undefined_*
	// TODO: earlier version might have them too, check the exact version?

#elif	defined(HH_COMPILER_CLANG) && (CLANG_FULL_VERSION >= HH_COMPILER_MAKE_VERSION(3, 9, 0))

	// clang 3.9.0 has _mm_undefined_*
	// TODO: earlier version might have them too, check the exact version?

#else

	// other compiler folks : please do the same as ICC :(

//#	if	defined(HH_COMPILER_MSVC) && (HH_COMPILER_VERSION >= 1700)	// vs2012 and above
//		// vs2012 has a super-dumb implementation of _mm_undefined_ps(). it is #define'd to _mm_setzero_ps()
//		// don't hard-check for later versions, we'll update this header in later SDK versions to accommodate
//		// potential lack of proper implementation of _mm_undefined_ps().
//		// by default, for unknown implementations, be optimistic and assume the implementation is 'correct'
//#		if	(HH_COMPILER_VERSION < 1800)	// we know that one. and we don't want it.
//#			undef	_mm_undefined_ps
//#			undef	_mm_undefined_si128
//#			undef	_mm256_undefined_ps
//#			undef	_mm256_undefined_si256
//#		endif
//#	endif

	// in case the compiler #define'd these for some reason (watchout for vs2012-like 'fake' intrinsics)
#	if	defined(_mm_undefined_ps) || defined(_mm_undefined_si128) || defined(_mm256_undefined_ps) || defined(_mm256_undefined_si256)
		// compiler defines at least one of them. assume it has support for all of them. (it should)
#		if	!defined(_mm_undefined_ps) || !defined(_mm_undefined_si128) || !defined(_mm256_undefined_ps) || !defined(_mm256_undefined_si256)
#			error Broken compiler or spec: partial _mm_undefined_* API
#		endif
#	else

	// compiler has no known support for _mm_undefined_* intrinsics.
	// if it does, we'll get build errors below. and we'll need to patch this header.
	// otherwise, define our own versions, that return an undefined vector.

#		if	defined(HH_DEBUG) || defined(HH_WINRT)
		// runtime initialization check exception...
		// apparently when building for winrt this is always checked, even in release/retail builds.
		// => build configuration problem on our side? or always the case?

		HH_FORCEINLINE __m128	_mm_undefined_ps()		{ return _mm_setzero_ps(); }
		HH_FORCEINLINE __m128i	_mm_undefined_si128()	{ return _mm_setzero_si128(); }

#			if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
			// AVX versions
			HH_FORCEINLINE __m256	_mm256_undefined_ps()		{ return _mm256_setzero_ps(); }
			HH_FORCEINLINE __m256i	_mm256_undefined_si256()	{ return _mm256_setzero_si256(); }
#			endif	// AVX

#			if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX512)
			// AVX512 versions
			HH_FORCEINLINE __m512	_mm512_undefined_ps()		{ return _mm512_setzero_ps(); }
			HH_FORCEINLINE __m512i	_mm512_undefined_epi32()	{ return _mm512_setzero_si512(); }
#			endif	// AVX512

#		else	// !defined(HH_DEBUG) && !defined(HH_WINRT)

			// silence compiler warnings about the uninitialized vars below
#			if	defined(HH_COMPILER_CLANG)
#				pragma	clang diagnostic push
#				pragma	clang diagnostic ignored "-Wuninitialized"
#				define	PK_UNINITIALIZED_ATTRIBUTE
#			elif	defined(HH_COMPILER_MSVC)
#				pragma	warning(push)
#				pragma	warning(disable : 4700) // uninitialized local variable '%v' used
#				define	PK_UNINITIALIZED_ATTRIBUTE
#			elif	defined(HH_COMPILER_GCC)
				// one of those three should work, hopefully?
#				define	PK_UNINITIALIZED_ATTRIBUTE	__attribute__((uninitialized))	// TODO: test if it works and correctly silences the compiler.
//#				define	PK_UNINITIALIZED_ATTRIBUTE	__attribute__((ignore("-Wuninitialized")))
//#				define	PK_UNINITIALIZED_ATTRIBUTE	__attribute__((diagnostic("-Wno-uninitialized")))
#			elif	defined(HH_COMPILER_INTEL)	// older version, before AVX
#				define	PK_UNINITIALIZED_ATTRIBUTE	__attribute__((uninitialized))
#			endif

			HH_FORCEINLINE __m128	_mm_undefined_ps()
			{
				__m128	dummy PK_UNINITIALIZED_ATTRIBUTE;	// voluntarily uninitialized
				return dummy;
			}
			HH_FORCEINLINE __m128i	_mm_undefined_si128()
			{
				__m128i	dummy PK_UNINITIALIZED_ATTRIBUTE;	// voluntarily uninitialized
				return dummy;
			}

#			if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX)
				// AVX versions
				HH_FORCEINLINE __m256	_mm256_undefined_ps()
				{
					__m256	dummy PK_UNINITIALIZED_ATTRIBUTE;	// voluntarily uninitialized
					return dummy;
				}
				HH_FORCEINLINE __m256i	_mm256_undefined_si256()
				{
					__m256i	dummy PK_UNINITIALIZED_ATTRIBUTE;	// voluntarily uninitialized
					return dummy;
				}
#			endif	// AVX

#			if	(HH_SIMD_SSE_COMPILABLE_VERSION >= HH_AVX512)
				// AVX512 versions
				HH_FORCEINLINE __m512	_mm512_undefined_ps()
				{
					__m512	dummy PK_UNINITIALIZED_ATTRIBUTE;	// voluntarily uninitialized
					return dummy;
				}
				HH_FORCEINLINE __m512i	_mm512_undefined_epi32()
				{
					__m512i	dummy PK_UNINITIALIZED_ATTRIBUTE;	// voluntarily uninitialized
					return dummy;
				}
#			endif	// AVX512

#			if	defined(HH_COMPILER_CLANG)
#				pragma	clang diagnostic pop
#			elif	defined(HH_COMPILER_MSVC)
#				pragma	warning(pop)
#			endif

#			if	defined(PK_UNINITIALIZED_ATTRIBUTE)
#				undef	PK_UNINITIALIZED_ATTRIBUTE
#			endif

#		endif	// !defined(HH_DEBUG) && !defined(HH_WINRT)
#	endif	// !defined(_mm_undefined_ps, ...)

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_MATHS_SIMD_SSE_DEFS_H__
