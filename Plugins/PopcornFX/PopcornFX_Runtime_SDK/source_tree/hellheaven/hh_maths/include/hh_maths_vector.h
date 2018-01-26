#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2007/10/19 19:42 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_MATHS_VECTOR_H__
#define __PK_MATHS_VECTOR_H__

#ifndef __PK_MATHS_H__
#	error	hh_maths_vector.h cannot be included without hh_maths.h
#endif

#include <hh_kernel/include/kr_memoryviews.h>

// Attention aux yeux, ce fichier a ete pourri suite au besoin d'instantier explicitement les TVector dans une
// tentative de reduire la taille des binaires sous PS3 avec les infos de debug.

#define	HH_MATHS_VECTOR_USE_SWIZZLES

#ifndef	HH_MATHS_VECTOR_EXPLICIT_INSTANTIATION
#	if	defined(HH_COMPILER_GCC) && !defined(HH_ORBIS) && !defined(__llvm__) /* && defined(_DEBUG)*//* || defined(HH_COMPILER_GHS)*/
#		define	HH_MATHS_VECTOR_EXPLICIT_INSTANTIATION	1
#	else
#		define	HH_MATHS_VECTOR_EXPLICIT_INSTANTIATION	0
#	endif
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>		class	TVectorImpl;
template<typename _Type, hh_u32 _Dimension>		class	TVector;

//----------------------------------------------------------------------------
// swizzles:
//
//	numeric swizzling: xyz1(), xyz0(), xx1w(), x0y(), x1y(), _0yzw(), etc...
//	WARNING:
//	generating the numeric swizzling accessors will generate a huge number of methods in the TVector templates.
//	they might induce a compile-time slowdown...
//	NOTE:
//	multi-numeric swizzles like xy01() are not supported. as they would spawn a few thousand accessors.
//
//	pure accessor count:
//	float4:	4^4 =	256
//	float3:	4^3 =	64
//	float2: 4^2 =	16
//					336
//
//	accessors with a single numeric swizzle:
//	float4:	(4 + 2 * 4) * 4^3 =	768
//	float3:	(4 + 2 * 3) * 4^2 =	144
//	float2: (4 + 2 * 2) * 4^1 =	24
//								936
//
//	accessors with full numeric swizzling:
//	float4:	6^4 =	1296
//	float3:	6^3 =	216
//	float2: 6^2 =	36
//					1548

#ifdef	HH_MATHS_VECTOR_USE_SWIZZLES

// #define	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS	// uncomment to activate ultragore numeric accessors allowing to do horrendous swizzles like: vec.x0y1() or vec._0110(), etc..

#define	__SWZ_AS_x					x	// swizzle access symbols
#define	__SWZ_AS_y					y
#define	__SWZ_AS_z					z
#define	__SWZ_AS_w					w
#define	__SWZ_AS_0					0
#define	__SWZ_AS_1					1
#define	__SWZ_PAS_x					x	// swizzle primary access symbol
#define	__SWZ_PAS_y					y
#define	__SWZ_PAS_z					z
#define	__SWZ_PAS_w					w
#define	__SWZ_PAS_0					_0
#define	__SWZ_PAS_1					_1

#define	__SWZ_IAS_x					_V::m_Data[0]	// swizzle internal access symbol
#define	__SWZ_IAS_y					_V::m_Data[1]
#define	__SWZ_IAS_z					_V::m_Data[2]
#define	__SWZ_IAS_w					_V::m_Data[3]
#define	__SWZ_IAS_0					0
#define	__SWZ_IAS_1					1

#define	__SWZ_STATICRANGECHECK_x	true
#define	__SWZ_STATICRANGECHECK_y	(Dimension >= 2)
#define	__SWZ_STATICRANGECHECK_z	(Dimension >= 3)
#define	__SWZ_STATICRANGECHECK_w	(Dimension >= 4)
#define	__SWZ_STATICRANGECHECK_0	true
#define	__SWZ_STATICRANGECHECK_1	true

#ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
#	define	__ONLY_IF_NOT_BUILDING_FULL_NUMERIC_ACCESSORS(__expr)
#	define	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(__expr)		__expr
#else
#	define	__ONLY_IF_NOT_BUILDING_FULL_NUMERIC_ACCESSORS(__expr)	__expr
#	define	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(__expr)
#endif

// #define	__SWZ_FA(__wut)	GLUE(__SWZ_PAS_, __wut)
// #define	__SWZ_SA(__wut)	GLUE(__SWZ_IAS_, __wut)

#define	NUMERIC_SWIZZLE4(__x, __y, __z, __w) \
	HH_FORCEINLINE TVector<_Type, 4>	GLUE(__SWZ_PAS_ ## __x, GLUE(__SWZ_AS_ ## __y, GLUE(__SWZ_AS_ ## __z, __SWZ_AS_ ## __w)))() const { HH_STATIC_ASSERT(__SWZ_STATICRANGECHECK_ ## __x && __SWZ_STATICRANGECHECK_ ## __y && __SWZ_STATICRANGECHECK_ ## __z && __SWZ_STATICRANGECHECK_ ## __w); return TVector<_Type, 4>(__SWZ_IAS_ ## __x, __SWZ_IAS_ ## __y, __SWZ_IAS_ ## __z, __SWZ_IAS_ ## __w); }
#define	NUMERIC_SWIZZLE3(__x, __y, __z) \
	HH_FORCEINLINE TVector<_Type, 3>	GLUE(__SWZ_PAS_ ## __x, GLUE(__SWZ_AS_ ## __y, __SWZ_AS_ ## __z))() const { HH_STATIC_ASSERT(__SWZ_STATICRANGECHECK_ ## __x && __SWZ_STATICRANGECHECK_ ## __y && __SWZ_STATICRANGECHECK_ ## __z); return TVector<_Type, 3>(__SWZ_IAS_ ## __x, __SWZ_IAS_ ## __y, __SWZ_IAS_ ## __z); }
#define	NUMERIC_SWIZZLE2(__x, __y) \
	HH_FORCEINLINE TVector<_Type, 2>	GLUE(__SWZ_PAS_ ## __x, __SWZ_AS_ ## __y)() const { HH_STATIC_ASSERT(__SWZ_STATICRANGECHECK_ ## __x && __SWZ_STATICRANGECHECK_ ## __y); return TVector<_Type, 2>(__SWZ_IAS_ ## __x, __SWZ_IAS_ ## __y); }

#define	NUMERIC_SWIZZLE_ACCEL4__(__x, __y, __z) \
	NUMERIC_SWIZZLE_FILTER_X(NUMERIC_SWIZZLE4(__x, __y, __z, x)) \
	NUMERIC_SWIZZLE_FILTER_Y(NUMERIC_SWIZZLE4(__x, __y, __z, y)) \
	NUMERIC_SWIZZLE_FILTER_Z(NUMERIC_SWIZZLE4(__x, __y, __z, z)) \
	NUMERIC_SWIZZLE_FILTER_W(NUMERIC_SWIZZLE4(__x, __y, __z, w)) \
	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE4(__x, __y, __z, 0) \
												NUMERIC_SWIZZLE4(__x, __y, __z, 1)) \
	__ONLY_IF_NOT_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE4(__x, __y, __z, 0) NUMERIC_SWIZZLE4(__x, __y, __z, 1) \
													NUMERIC_SWIZZLE4(__x, __y, 0, __z) NUMERIC_SWIZZLE4(__x, __y, 1, __z) \
													NUMERIC_SWIZZLE4(__x, 0, __y, __z) NUMERIC_SWIZZLE4(__x, 1, __y, __z) \
													NUMERIC_SWIZZLE4(0, __x, __y, __z) NUMERIC_SWIZZLE4(1, __x, __y, __z))
#define	NUMERIC_SWIZZLE_ACCEL4_(__x, __y) \
	NUMERIC_SWIZZLE_FILTER_X(NUMERIC_SWIZZLE_ACCEL4__(__x, __y, x)) \
	NUMERIC_SWIZZLE_FILTER_Y(NUMERIC_SWIZZLE_ACCEL4__(__x, __y, y)) \
	NUMERIC_SWIZZLE_FILTER_Z(NUMERIC_SWIZZLE_ACCEL4__(__x, __y, z)) \
	NUMERIC_SWIZZLE_FILTER_W(NUMERIC_SWIZZLE_ACCEL4__(__x, __y, w)) \
	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE_ACCEL4__(__x, __y, 0) \
												NUMERIC_SWIZZLE_ACCEL4__(__x, __y, 1))
#define	NUMERIC_SWIZZLE_ACCEL4(__x) \
	NUMERIC_SWIZZLE_FILTER_X(NUMERIC_SWIZZLE_ACCEL4_(__x, x)) \
	NUMERIC_SWIZZLE_FILTER_Y(NUMERIC_SWIZZLE_ACCEL4_(__x, y)) \
	NUMERIC_SWIZZLE_FILTER_Z(NUMERIC_SWIZZLE_ACCEL4_(__x, z)) \
	NUMERIC_SWIZZLE_FILTER_W(NUMERIC_SWIZZLE_ACCEL4_(__x, w)) \
	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE_ACCEL4_(__x, 0) \
												NUMERIC_SWIZZLE_ACCEL4_(__x, 1))

#define	NUMERIC_SWIZZLE_ACCEL3_(__x, __y) \
	NUMERIC_SWIZZLE_FILTER_X(NUMERIC_SWIZZLE3(__x, __y, x)) \
	NUMERIC_SWIZZLE_FILTER_Y(NUMERIC_SWIZZLE3(__x, __y, y)) \
	NUMERIC_SWIZZLE_FILTER_Z(NUMERIC_SWIZZLE3(__x, __y, z)) \
	NUMERIC_SWIZZLE_FILTER_W(NUMERIC_SWIZZLE3(__x, __y, w)) \
	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE3(__x, __y, 0) \
												NUMERIC_SWIZZLE3(__x, __y, 1)) \
	__ONLY_IF_NOT_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE3(__x, __y, 0) NUMERIC_SWIZZLE3(__x, __y, 1) \
													NUMERIC_SWIZZLE3(__x, 0, __y) NUMERIC_SWIZZLE3(__x, 1, __y) \
													NUMERIC_SWIZZLE3(0, __x, __y) NUMERIC_SWIZZLE3(1, __x, __y))

#define	NUMERIC_SWIZZLE_ACCEL3(__x) \
	NUMERIC_SWIZZLE_FILTER_X(NUMERIC_SWIZZLE_ACCEL3_(__x, x)) \
	NUMERIC_SWIZZLE_FILTER_Y(NUMERIC_SWIZZLE_ACCEL3_(__x, y)) \
	NUMERIC_SWIZZLE_FILTER_Z(NUMERIC_SWIZZLE_ACCEL3_(__x, z)) \
	NUMERIC_SWIZZLE_FILTER_W(NUMERIC_SWIZZLE_ACCEL3_(__x, w)) \
	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE_ACCEL3_(__x, 0) \
												NUMERIC_SWIZZLE_ACCEL3_(__x, 1))

#define	NUMERIC_SWIZZLE_ACCEL2(__x) \
	NUMERIC_SWIZZLE_FILTER_X(NUMERIC_SWIZZLE2(__x, x)) \
	NUMERIC_SWIZZLE_FILTER_Y(NUMERIC_SWIZZLE2(__x, y)) \
	NUMERIC_SWIZZLE_FILTER_Z(NUMERIC_SWIZZLE2(__x, z)) \
	NUMERIC_SWIZZLE_FILTER_W(NUMERIC_SWIZZLE2(__x, w)) \
	__ONLY_IF_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE2(__x, 0) \
												NUMERIC_SWIZZLE2(__x, 1)) \
	__ONLY_IF_NOT_BUILDING_FULL_NUMERIC_ACCESSORS(	NUMERIC_SWIZZLE2(__x, 0) NUMERIC_SWIZZLE2(__x, 1) \
													NUMERIC_SWIZZLE2(0, __x) NUMERIC_SWIZZLE2(1, __x))

#endif

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
class	TVector
{
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TVector<_Type, 1> : public TVectorImpl<_Type, 1>
{
public:
	typedef TVectorImpl<_Type, 1>			_V;
	typedef TVector<_Type, 1>				SelfType;
	typedef _Type							ScalarType;
	enum {									Dimension = 1 };

public:
	TVector();
	TVector(const _V &other);
	explicit TVector(_Type value);

	template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
	TVector(const TVector<_Type2, 1> &copyFrom);

	HH_FORCEINLINE const _Type	&x() const { return _V::m_Data[0]; }
	HH_FORCEINLINE _Type		&x() { return _V::m_Data[0]; }

#ifdef	HH_MATHS_VECTOR_USE_SWIZZLES
#	define	NUMERIC_SWIZZLE_FILTER_X(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Y(__expr)
#	define	NUMERIC_SWIZZLE_FILTER_Z(__expr)
#	define	NUMERIC_SWIZZLE_FILTER_W(__expr)

	NUMERIC_SWIZZLE_ACCEL4(x);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL4(0);
		NUMERIC_SWIZZLE_ACCEL4(1);
#	endif

	NUMERIC_SWIZZLE_ACCEL3(x);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL3(0);
		NUMERIC_SWIZZLE_ACCEL3(1);
#	endif

	NUMERIC_SWIZZLE_ACCEL2(x);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL2(0);
		NUMERIC_SWIZZLE_ACCEL2(1);
#	endif

#	undef	NUMERIC_SWIZZLE_FILTER_X
#	undef	NUMERIC_SWIZZLE_FILTER_Y
#	undef	NUMERIC_SWIZZLE_FILTER_Z
#	undef	NUMERIC_SWIZZLE_FILTER_W
#endif

	HH_ALIGN(0x10) static const SelfType	XAXIS;
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TVector<_Type, 2> : public TVectorImpl<_Type, 2>
{
public:
	typedef TVectorImpl<_Type, 2>			_V;
	typedef TVector<_Type, 2>				SelfType;
	typedef _Type							ScalarType;
	enum {									Dimension = 2 };

public:
	TVector();
	TVector(const _V &other);
	explicit TVector(_Type value);
	TVector(_Type v0, _Type v1);

	template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
	TVector(const TVector<_Type2, 2> &copyFrom);

	HH_FORCEINLINE const _Type	&x() const { return _V::m_Data[0]; }
	HH_FORCEINLINE _Type		&x() { return _V::m_Data[0]; }
	HH_FORCEINLINE const _Type	&y() const { return _V::m_Data[1]; }
	HH_FORCEINLINE _Type		&y() { return _V::m_Data[1]; }

#ifdef	HH_MATHS_VECTOR_USE_SWIZZLES
#	define	NUMERIC_SWIZZLE_FILTER_X(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Y(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Z(__expr)
#	define	NUMERIC_SWIZZLE_FILTER_W(__expr)

	NUMERIC_SWIZZLE_ACCEL4(x);
	NUMERIC_SWIZZLE_ACCEL4(y);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL4(0);
		NUMERIC_SWIZZLE_ACCEL4(1);
#	endif

	NUMERIC_SWIZZLE_ACCEL3(x);
	NUMERIC_SWIZZLE_ACCEL3(y);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL3(0);
		NUMERIC_SWIZZLE_ACCEL3(1);
#	endif

	// we redefine these lower, returning references and without using temporaries...
#	define	xy	__xy
	NUMERIC_SWIZZLE_ACCEL2(x);
	NUMERIC_SWIZZLE_ACCEL2(y);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL2(0);
		NUMERIC_SWIZZLE_ACCEL2(1);
#	endif
#	undef	xy

#	undef	NUMERIC_SWIZZLE_FILTER_X
#	undef	NUMERIC_SWIZZLE_FILTER_Y
#	undef	NUMERIC_SWIZZLE_FILTER_Z
#	undef	NUMERIC_SWIZZLE_FILTER_W
#endif

	//----------------------------------------------------------------------------
	// Special-case swizzle accessors for fast access to ordered sub-vectors

	HH_FORCEINLINE TVector<_Type, 2>		&xy() { return *((TVector<_Type, 2>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE const TVector<_Type, 2>	&xy() const { return *((TVector<_Type, 2>*)(_V::m_Data + 0)); }

	HH_ALIGN(0x10) static const SelfType	XAXIS;
	HH_ALIGN(0x10) static const SelfType	YAXIS;
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TVector<_Type, 3> : public TVectorImpl<_Type, 3>
{
public:
	typedef TVectorImpl<_Type, 3>			_V;
	typedef TVector<_Type, 3>				SelfType;
	typedef _Type							ScalarType;
	enum {									Dimension = 3 };

public:
	TVector();
	TVector(const _V &other);
	explicit TVector(_Type value);
	TVector(_Type v0, _Type v1, _Type v2);

	template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
	TVector(const TVector<_Type2, 3> &copyFrom);

	TVector(const TVector<_Type, 3 - 1> &vec, _Type v);
	TVector(_Type v0, const TVector<_Type, 3 - 1> &vec);

	HH_FORCEINLINE const _Type	&x() const { return _V::m_Data[0]; }
	HH_FORCEINLINE _Type		&x() { return _V::m_Data[0]; }
	HH_FORCEINLINE const _Type	&y() const { return _V::m_Data[1]; }
	HH_FORCEINLINE _Type		&y() { return _V::m_Data[1]; }
	HH_FORCEINLINE const _Type	&z() const { return _V::m_Data[2]; }
	HH_FORCEINLINE _Type		&z() { return _V::m_Data[2]; }

#ifdef	HH_MATHS_VECTOR_USE_SWIZZLES
#	define	NUMERIC_SWIZZLE_FILTER_X(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Y(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Z(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_W(__expr)

	NUMERIC_SWIZZLE_ACCEL4(x);
	NUMERIC_SWIZZLE_ACCEL4(y);
	NUMERIC_SWIZZLE_ACCEL4(z);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL4(0);
		NUMERIC_SWIZZLE_ACCEL4(1);
#	endif

	// we redefine these lower, returning references and without using temporaries...
#	define	xyz	__xyz
	NUMERIC_SWIZZLE_ACCEL3(x);
	NUMERIC_SWIZZLE_ACCEL3(y);
	NUMERIC_SWIZZLE_ACCEL3(z);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL3(0);
		NUMERIC_SWIZZLE_ACCEL3(1);
#	endif
#	undef	xyz

	// we redefine these lower, returning references and without using temporaries...
#	define	xy	__xy
#	define	yz	__yz
	NUMERIC_SWIZZLE_ACCEL2(x);
	NUMERIC_SWIZZLE_ACCEL2(y);
	NUMERIC_SWIZZLE_ACCEL2(z);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL2(0);
		NUMERIC_SWIZZLE_ACCEL2(1);
#	endif
#	undef	xy
#	undef	yz

#	undef	NUMERIC_SWIZZLE_FILTER_X
#	undef	NUMERIC_SWIZZLE_FILTER_Y
#	undef	NUMERIC_SWIZZLE_FILTER_Z
#	undef	NUMERIC_SWIZZLE_FILTER_W
#endif

	//----------------------------------------------------------------------------
	// Special-case swizzle accessors for fast access to ordered sub-vectors

	HH_FORCEINLINE TVector<_Type, 2>		&xy() { return *((TVector<_Type, 2>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE const TVector<_Type, 2>	&xy() const { return *((TVector<_Type, 2>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE TVector<_Type, 2>		&yz() { return *((TVector<_Type, 2>*)(_V::m_Data + 1)); }
	HH_FORCEINLINE const TVector<_Type, 2>	&yz() const { return *((TVector<_Type, 2>*)(_V::m_Data + 1)); }

	HH_FORCEINLINE TVector<_Type, 3>		&xyz() { return *((TVector<_Type, 3>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE const TVector<_Type, 3>	&xyz() const { return *((TVector<_Type, 3>*)(_V::m_Data + 0)); }

	HH_ALIGN(0x10) static const SelfType	XAXIS;
	HH_ALIGN(0x10) static const SelfType	YAXIS;
	HH_ALIGN(0x10) static const SelfType	ZAXIS;
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TVector<_Type, 4> : public TVectorImpl<_Type, 4>
{
public:
	typedef TVectorImpl<_Type, 4>			_V;
	typedef TVector<_Type, 4>				SelfType;
	typedef _Type							ScalarType;
	enum {									Dimension = 4 };

public:
	TVector();
	TVector(const _V &other);
	explicit TVector(_Type value);
	TVector(_Type v0, _Type v1, _Type v2, _Type v3);

	template<typename _Type2>	// allows to cast CInt3 into CFloat3, etc...
	TVector(const TVector<_Type2, 4> &copyFrom);

	TVector(const TVector<_Type, 4 - 1> &vec, _Type v);
	TVector(_Type v0, const TVector<_Type, 4 - 1> &vec);
	TVector(const TVector<_Type, 2> &vec, _Type v2, _Type v3);
	TVector(_Type v0, _Type v1, const TVector<_Type, 2> &vec);
	TVector( const TVector<_Type, 2> &vec0, const TVector<_Type, 2> &vec1);
	TVector(_Type v0, const TVector<_Type, 2> &vec, _Type v3);

	HH_FORCEINLINE const _Type	&x() const { return _V::m_Data[0]; }
	HH_FORCEINLINE _Type		&x() { return _V::m_Data[0]; }
	HH_FORCEINLINE const _Type	&y() const { return _V::m_Data[1]; }
	HH_FORCEINLINE _Type		&y() { return _V::m_Data[1]; }
	HH_FORCEINLINE const _Type	&z() const { return _V::m_Data[2]; }
	HH_FORCEINLINE _Type		&z() { return _V::m_Data[2]; }
	HH_FORCEINLINE const _Type	&w() const { return _V::m_Data[3]; }
	HH_FORCEINLINE _Type		&w() { return _V::m_Data[3]; }

#ifdef	HH_MATHS_VECTOR_USE_SWIZZLES
#	define	NUMERIC_SWIZZLE_FILTER_X(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Y(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_Z(__expr)	__expr
#	define	NUMERIC_SWIZZLE_FILTER_W(__expr)	__expr

	// we redefine these lower, returning references and without using temporaries...
#	define	xyzw	__xyzw
	// actual expansion, generates (4 + 2 * 4) * 4 * 4 * 4 -> 768 accessors (ouch)
	NUMERIC_SWIZZLE_ACCEL4(x);
	NUMERIC_SWIZZLE_ACCEL4(y);
	NUMERIC_SWIZZLE_ACCEL4(z);
	NUMERIC_SWIZZLE_ACCEL4(w);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL4(0);
		NUMERIC_SWIZZLE_ACCEL4(1);
#	endif
#	undef	xyzw

	// we redefine these lower, returning references and without using temporaries...
#	define	xyz	__xyz
#	define	yzw	__yzw
	// actual expansion, generates (4 + 2 * 3) * 4 * 4 -> 144 accessors (hum)
	NUMERIC_SWIZZLE_ACCEL3(x);
	NUMERIC_SWIZZLE_ACCEL3(y);
	NUMERIC_SWIZZLE_ACCEL3(z);
	NUMERIC_SWIZZLE_ACCEL3(w);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL3(0);
		NUMERIC_SWIZZLE_ACCEL3(1);
#	endif
#	undef	xyz
#	undef	yzw

	// we redefine these lower, returning references and without using temporaries...
#	define	xy	__xy
#	define	yz	__yz
#	define	zw	__zw
	// actual expansion, generates (4 + 2 * 2) * 4 -> 24 accessors (ok)
	NUMERIC_SWIZZLE_ACCEL2(x);
	NUMERIC_SWIZZLE_ACCEL2(y);
	NUMERIC_SWIZZLE_ACCEL2(z);
	NUMERIC_SWIZZLE_ACCEL2(w);
#	ifdef	HH_MATHS_VECTOR_ENABLE_HARDCORE_NUMERIC_ACCESSORS
		NUMERIC_SWIZZLE_ACCEL2(0);
		NUMERIC_SWIZZLE_ACCEL2(1);
#	endif
#	undef	xy
#	undef	yz
#	undef	zw

#	undef	NUMERIC_SWIZZLE_FILTER_X
#	undef	NUMERIC_SWIZZLE_FILTER_Y
#	undef	NUMERIC_SWIZZLE_FILTER_Z
#	undef	NUMERIC_SWIZZLE_FILTER_W
#endif

	//----------------------------------------------------------------------------
	// Special-case swizzle accessors for fast access to ordered sub-vectors

	HH_FORCEINLINE TVector<_Type, 2>		&xy() { return *((TVector<_Type, 2>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE const TVector<_Type, 2>	&xy() const { return *((TVector<_Type, 2>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE TVector<_Type, 2>		&yz() { return *((TVector<_Type, 2>*)(_V::m_Data + 1)); }
	HH_FORCEINLINE const TVector<_Type, 2>	&yz() const { return *((TVector<_Type, 2>*)(_V::m_Data + 1)); }
	HH_FORCEINLINE TVector<_Type, 2>		&zw() { return *((TVector<_Type, 2>*)(_V::m_Data + 2)); }
	HH_FORCEINLINE const TVector<_Type, 2>	&zw() const { return *((TVector<_Type, 2>*)(_V::m_Data + 2)); }

	HH_FORCEINLINE TVector<_Type, 3>		&xyz() { return *((TVector<_Type, 3>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE const TVector<_Type, 3>	&xyz() const { return *((TVector<_Type, 3>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE TVector<_Type, 3>		&yzw() { return *((TVector<_Type, 3>*)(_V::m_Data + 1)); }
	HH_FORCEINLINE const TVector<_Type, 3>	&yzw() const { return *((TVector<_Type, 3>*)(_V::m_Data + 1)); }

	HH_FORCEINLINE TVector<_Type, 4>		&xyzw() { return *((TVector<_Type, 4>*)(_V::m_Data + 0)); }
	HH_FORCEINLINE const TVector<_Type, 4>	&xyzw() const { return *((TVector<_Type, 4>*)(_V::m_Data + 0)); }

	HH_ALIGN(0x10) static const SelfType	XAXIS;
	HH_ALIGN(0x10) static const SelfType	YAXIS;
	HH_ALIGN(0x10) static const SelfType	ZAXIS;
	HH_ALIGN(0x10) static const SelfType	WAXIS;
	HH_ALIGN(0x10) static const SelfType	ZERO;
	HH_ALIGN(0x10) static const SelfType	ONE;
};

//----------------------------------------------------------------------------

#undef	NUMERIC_SWIZZLE_ACCEL4__
#undef	NUMERIC_SWIZZLE_ACCEL4_
#undef	NUMERIC_SWIZZLE_ACCEL4
#undef	NUMERIC_SWIZZLE_ACCEL3_
#undef	NUMERIC_SWIZZLE_ACCEL3
#undef	NUMERIC_SWIZZLE_ACCEL2

//----------------------------------------------------------------------------
//
//	TODO: inherit TVectors from this, and it should instead define real operators, so
//	that using an integer operator on a floating-point vector will generate a compile-time error.
//	whereas right now, it compiles fine, but does nothing.
//
//	therefore, also store the data here.
//
//----------------------------------------------------------------------------

#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Add	+
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Sub	-
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Mul	*
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Div	/

#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Mod	%
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Or	|
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_And	&
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Xor	^
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Lsr	>>
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_Lsl	<<
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_UNeg	-
#define	HH_MATHS_VECTOR_OP_CPP_SYMBOL_UNot	~

#define	HH_MATHS_VECTOR_BUILD_OP(__which) \
	template<typename _Type2> \
	HH_FORCEINLINE typename TVectorPromotableOpImpl<_Type, _Dimension, _Type2>::_PromotedVector	operator GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which) (const _Type2 &operand) const \
	{ \
		return TVectorPromotableOpImpl<_Type, _Dimension, _Type2>::__which(*this, operand); \
	} \
	HH_FORCEINLINE void	operator GLUE(GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which), =) (const TVector<_Type, _Dimension> &operand) \
	{ \
		for (hh_u32 i = 0; i < _Dimension; i++) { m_Data[i] GLUE(GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which), =) operand.m_Data[i]; } \
	} \
	HH_FORCEINLINE void	operator GLUE(GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which), =) (_Type operand) \
	{ \
		for (hh_u32 i = 0; i < _Dimension; i++) { m_Data[i] GLUE(GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_, __which), =) operand; } \
	}

#define	HH_MATHS_VECTOR_BUILD_UNARY(__which) \
	HH_FORCEINLINE TVector<_Type, _Dimension>	operator GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_U, __which) () const \
	{ \
		TVector<_Type, _Dimension>	output; \
		for (hh_u32 i = 0; i < _Dimension; i++) { output.m_Data[i] = GLUE(HH_MATHS_VECTOR_OP_CPP_SYMBOL_U, __which) m_Data[i]; } \
		return output; \
	}

//----------------------------------------------------------------------------

template<typename _Type1, hh_u32 _Dimension1, typename _Type2>
struct		TVectorPromotableOpImpl;

//----------------------------------------------------------------------------
//
//	integer vectors
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension, bool _IsFloat = TTypeTraits::IsFloat<_Type>::True>
class	TVectorBase
{
protected:
	_Type		m_Data[_Dimension];

public:
	HH_FORCEINLINE const _Type			&Axis(hh_u32 which) const { HH_PARANOID_ASSERT(which < _Dimension); return m_Data[which]; }
	HH_FORCEINLINE _Type				&Axis(hh_u32 which) { HH_PARANOID_ASSERT(which < _Dimension); return m_Data[which]; }

	HH_MATHS_VECTOR_BUILD_OP(Add);
	HH_MATHS_VECTOR_BUILD_OP(Sub);
	HH_MATHS_VECTOR_BUILD_OP(Mul);
	HH_MATHS_VECTOR_BUILD_OP(Div);

	HH_MATHS_VECTOR_BUILD_UNARY(Neg);

	HH_MATHS_VECTOR_BUILD_OP(Mod);
	HH_MATHS_VECTOR_BUILD_OP(Or);
	HH_MATHS_VECTOR_BUILD_OP(And);
	HH_MATHS_VECTOR_BUILD_OP(Xor);

	HH_MATHS_VECTOR_BUILD_OP(Lsr);
	HH_MATHS_VECTOR_BUILD_OP(Lsl);

	HH_MATHS_VECTOR_BUILD_UNARY(Not);
};

//----------------------------------------------------------------------------
//
//	floating-point vectors
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
class	TVectorBase<_Type, _Dimension, true>
{
protected:
	_Type		m_Data[_Dimension];

public:
	HH_FORCEINLINE const _Type		&Axis(hh_u32 which) const { HH_PARANOID_ASSERT(which < _Dimension); return m_Data[which]; }
	HH_FORCEINLINE _Type			&Axis(hh_u32 which) { HH_PARANOID_ASSERT(which < _Dimension); return m_Data[which]; }

	HH_MATHS_VECTOR_BUILD_OP(Add);
	HH_MATHS_VECTOR_BUILD_OP(Sub);
	HH_MATHS_VECTOR_BUILD_OP(Mul);
	HH_MATHS_VECTOR_BUILD_OP(Div);

	HH_MATHS_VECTOR_BUILD_UNARY(Neg);
};

//----------------------------------------------------------------------------

#undef	HH_MATHS_VECTOR_BUILD_OP
#undef	HH_MATHS_VECTOR_BUILD_UNARY

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
class	TVectorImpl : public TVectorBase<_Type, _Dimension>
{
public:
	typedef TVectorImpl<_Type, _Dimension>										SelfType;
	typedef TVector<_Type, _Dimension>											SelfFullType;
	typedef TVector<typename TIntegerFootprint<_Type>::SignedType, _Dimension>	MaskType;
	typedef _Type																ScalarType;
	enum {																		Dimension = _Dimension };

protected:
	typedef TVectorBase<_Type, _Dimension>										_B;
	template<typename _Type2, hh_u32 _Dimension2> friend class					TVectorImpl;

public:
	template<typename _Type0, typename _Type1>
	HH_INLINE SelfType								Madd(const _Type0 &mul, const _Type1 &add) const
	{
		return this->operator*(mul) + add;
	}

	// compare masks. Note that we use signed masks for auto-sign-extend in type conversions
	template<typename _Type2>
	MaskType						MaskEqual(const TVectorImpl<_Type2, _Dimension> &vec) const;
	MaskType						MaskEqual(_Type scalar) const;
	template<typename _Type2>
	MaskType						MaskNotEqual(const TVectorImpl<_Type2, _Dimension> &vec) const;
	MaskType						MaskNotEqual(_Type scalar) const;
	template<typename _Type2>
	MaskType						MaskGreater(const TVectorImpl<_Type2, _Dimension> &vec) const;
	MaskType						MaskGreater(_Type scalar) const { return MaskGreater(TVector<_Type, _Dimension>(scalar)); }
	template<typename _Type2>
	MaskType						MaskGreaterOrEqual(const TVectorImpl<_Type2, _Dimension> &vec) const;
	MaskType						MaskGreaterOrEqual(_Type scalar) const { return MaskGreaterOrEqual(TVector<_Type, _Dimension>(scalar)); }
	template<typename _Type2>
	MaskType						MaskLower(const TVectorImpl<_Type2, _Dimension> &vec) const;
	MaskType						MaskLower(_Type scalar) const { return MaskLower(TVector<_Type, _Dimension>(scalar)); }
	template<typename _Type2>
	MaskType						MaskLowerOrEqual(const TVectorImpl<_Type2, _Dimension> &vec) const;
	MaskType						MaskLowerOrEqual(_Type scalar) const { return MaskLowerOrEqual(TVector<_Type, _Dimension>(scalar)); }


	template<typename _Type2>
	MaskType						operator > (const TVectorImpl<_Type2, _Dimension> &vec) const { return MaskGreater(vec); }
	MaskType						operator > (_Type scalar) const { return MaskGreater(scalar); }
	friend MaskType					operator > (_Type scalar, const SelfType &vec) { return vec.MaskLower(scalar); }
	template<typename _Type2>
	MaskType						operator < (const TVectorImpl<_Type2, _Dimension> &vec) const { return MaskLower(vec); }
	MaskType						operator < (_Type scalar) const { return MaskLower(scalar); }
	friend MaskType					operator < (_Type scalar, const SelfType &vec) { return vec.MaskGreater(scalar); }
	template<typename _Type2>
	MaskType						operator >= (const TVectorImpl<_Type2, _Dimension> &vec) const { return MaskGreaterOrEqual(vec); }
	MaskType						operator >= (_Type scalar) const { return MaskGreaterOrEqual(scalar); }
	friend MaskType					operator >= (_Type scalar, const SelfType &vec) { return vec.MaskLowerOrEqual(scalar); }
	template<typename _Type2>
	MaskType						operator <= (const TVectorImpl<_Type2, _Dimension> &vec) const { return MaskLowerOrEqual(vec); }
	MaskType						operator <= (_Type scalar) const { return MaskLowerOrEqual(scalar); }
	friend MaskType					operator <= (_Type scalar, const SelfType &vec) { return vec.MaskGreaterOrEqual(scalar); }

	// these should keep the 'bool' return-type for ease of use and readability in user code. use 'MaskEqual' or 'MaskNotEqual' for more control
	template<typename _Type2>
	bool							operator == (const TVectorImpl<_Type2, _Dimension> &vec) const;
	bool							operator == (_Type scalar) const;
	friend bool						operator == (_Type scalar, const SelfType &vec) { return vec == scalar; }
	template<typename _Type2>
	bool							operator != (const TVectorImpl<_Type2, _Dimension> &vec) const;
	bool							operator != (_Type scalar) const;
	friend bool						operator != (_Type scalar, const SelfType &vec) { return vec != scalar; }

	bool							Equals(const SelfType &other) const { return *this == other; }
	bool							Equals(const SelfType &other, _Type epsilon) const;

	//----------------------------------------------------------------------------
	// tools

	template<typename _Type2>
	HH_FORCEINLINE typename TNumericPromoter<_Type, _Type2>::PromotedType	Dot(const TVectorImpl<_Type2, _Dimension> &v) const
	{
		typedef typename TNumericPromoter<_Type, _Type2>::PromotedType	ResultType;
		ResultType	sum = 0;
		for (hh_u32 i = 0; i < _Dimension; i++)
		{
			sum += this->m_Data[i] * v.m_Data[i];
		}
		return sum;
	}

	template<typename _Type2>
	TVector<typename TNumericPromoter<_Type, _Type2>::PromotedType, _Dimension>	Cross(const TVector<_Type2, _Dimension> &v) const;
	template<typename _Type2, typename _Type3>
	TVector<typename TNumericPromoter<_Type, typename TNumericPromoter<_Type2, _Type3>::PromotedType>::PromotedType, _Dimension>	Cross(const TVector<_Type2, _Dimension> &v0, const TVector<_Type3, _Dimension> &v1) const;

	HH_FORCEINLINE _Type			LengthSquared() const { return Dot(*this); }
	HH_FORCEINLINE _Type			Length() const { return HHSqrt(LengthSquared()); }

	SelfFullType					Lerp(const SelfType &v, float fraction) const;

	void							Normalize();
	SelfFullType					Normalized() const;
	bool							IsNormalized(_Type epsilonSquared = 1.0e-2f) const;
	bool							IsFinite() const;	// returns true if not (IsNan() || IsInfinite())
	bool							IsNan() const;
	bool							IsInfinite() const;

	_Type							AxialSum() const;
	_Type							AxialProduct() const;

	_Type							HighestComponent() const;
	_Type							LowestComponent() const;

	hh_u32							HighestComponentId() const;
	hh_u32							LowestComponentId() const;

	bool							IsZero(_Type tolerance = 1.0e-10f) const;

	//----------------------------------------------------------------------------
	// endian conversions

	void							ConvertLittleEndian();
	void							ConvertBigEndian();
	SelfFullType					ConvertLittleEndian() const;
	SelfFullType					ConvertBigEndian() const;

	static SelfFullType				UnitAxis(hh_u32 axisId) { HH_ASSERT(axisId < _Dimension); SelfFullType	vec(0); vec.m_Data[axisId] = 1; return vec; }
};

//----------------------------------------------------------------------------
//
//	vector traits
//
//----------------------------------------------------------------------------

namespace	TTypeTraits
{
	template<typename _Type, hh_u32 _Dimension>	struct	IsInteger<TVector<_Type, _Dimension> > : IsInteger<_Type> {};
	template<typename _Type, hh_u32 _Dimension>	struct	IsFloat<TVector<_Type, _Dimension> > : IsFloat<_Type> {};
	template<typename _Type, hh_u32 _Dimension>	struct	IsSigned<TVector<_Type, _Dimension> > : IsSigned<_Type> {};

	template<typename _Type>					struct	IsVector : FalseType {};
	template<typename _Type, hh_u32 _Dimension>	struct	IsVector<TVector<_Type, _Dimension> > : TrueType {};
}

//----------------------------------------------------------------------------

#define	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, __sctype) \
template<typename _Type, hh_u32 _Dimension> \
HH_FORCEINLINE TVector<typename TNumericPromoter<__sctype, _Type>::PromotedType, _Dimension>	operator __op (__sctype scalar, const TVectorImpl<_Type, _Dimension> &vec) \
{ \
	TVector<typename TNumericPromoter<__sctype, _Type>::PromotedType, _Dimension>	result; \
	for (hh_u32 i = 0; i < _Dimension; i++) \
	{ \
		result.Axis(i) = scalar __op vec.Axis(i); \
	} \
	return result; \
}

// integer-only:
#define	HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(__op) \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, hh_i32); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, hh_u32); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, hh_i16); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, hh_u16); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, hh_i8); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, hh_u8)

// all-types:
#define	HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_OP(__op) \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, float); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_TYPED_LEFT_SCALAR_OP(__op, double); \
	HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(__op)

HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_OP(+);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_OP(-);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_OP(*);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_OP(/);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(%);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(|);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(&);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(^);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(>>);
HH_MATHS_VECTOR_DECLARE_GLOBAL_LEFT_SCALAR_IOP(<<);

//----------------------------------------------------------------------------

typedef TVector<float, 1>	CFloat1;
typedef TVector<float, 2>	CFloat2;
typedef TVector<float, 3>	CFloat3;
typedef TVector<float, 4>	CFloat4;
HH_STATIC_ASSERT(sizeof(CFloat1) == 4);
HH_STATIC_ASSERT(sizeof(CFloat2) == 8);
HH_STATIC_ASSERT(sizeof(CFloat3) == 12);
HH_STATIC_ASSERT(sizeof(CFloat4) == 16);

typedef TVector<double, 1>	CDouble1;
typedef TVector<double, 2>	CDouble2;
typedef TVector<double, 3>	CDouble3;
typedef TVector<double, 4>	CDouble4;
HH_STATIC_ASSERT(sizeof(CDouble1) == 8);
HH_STATIC_ASSERT(sizeof(CDouble2) == 16);
HH_STATIC_ASSERT(sizeof(CDouble3) == 24);
HH_STATIC_ASSERT(sizeof(CDouble4) == 32);

typedef TVector<hh_u32, 1>	CUint1;
typedef TVector<hh_u32, 2>	CUint2;
typedef TVector<hh_u32, 3>	CUint3;
typedef TVector<hh_u32, 4>	CUint4;
HH_STATIC_ASSERT(sizeof(CUint1) == 4);
HH_STATIC_ASSERT(sizeof(CUint2) == 8);
HH_STATIC_ASSERT(sizeof(CUint3) == 12);
HH_STATIC_ASSERT(sizeof(CUint4) == 16);

typedef TVector<hh_i32, 1>	CInt1;
typedef TVector<hh_i32, 2>	CInt2;
typedef TVector<hh_i32, 3>	CInt3;
typedef TVector<hh_i32, 4>	CInt4;
HH_STATIC_ASSERT(sizeof(CInt1) == 4);
HH_STATIC_ASSERT(sizeof(CInt2) == 8);
HH_STATIC_ASSERT(sizeof(CInt3) == 12);
HH_STATIC_ASSERT(sizeof(CInt4) == 16);

typedef TVector<hh_u16, 1>	CUword1;
typedef TVector<hh_u16, 2>	CUword2;
typedef TVector<hh_u16, 3>	CUword3;
typedef TVector<hh_u16, 4>	CUword4;
HH_STATIC_ASSERT(sizeof(CUword1) == 2);
HH_STATIC_ASSERT(sizeof(CUword2) == 4);
HH_STATIC_ASSERT(sizeof(CUword3) == 6);
HH_STATIC_ASSERT(sizeof(CUword4) == 8);

typedef TVector<hh_i16, 1>	CWord1;
typedef TVector<hh_i16, 2>	CWord2;
typedef TVector<hh_i16, 3>	CWord3;
typedef TVector<hh_i16, 4>	CWord4;
HH_STATIC_ASSERT(sizeof(CWord1) == 2);
HH_STATIC_ASSERT(sizeof(CWord2) == 4);
HH_STATIC_ASSERT(sizeof(CWord3) == 6);
HH_STATIC_ASSERT(sizeof(CWord4) == 8);

typedef TVector<hh_u8, 1>	CUbyte1;
typedef TVector<hh_u8, 2>	CUbyte2;
typedef TVector<hh_u8, 3>	CUbyte3;
typedef TVector<hh_u8, 4>	CUbyte4;
HH_STATIC_ASSERT(sizeof(CUbyte1) == 1);
HH_STATIC_ASSERT(sizeof(CUbyte2) == 2);
HH_STATIC_ASSERT(sizeof(CUbyte3) == 3);
HH_STATIC_ASSERT(sizeof(CUbyte4) == 4);

typedef TVector<hh_i8, 1>	CByte1;
typedef TVector<hh_i8, 2>	CByte2;
typedef TVector<hh_i8, 3>	CByte3;
typedef TVector<hh_i8, 4>	CByte4;
HH_STATIC_ASSERT(sizeof(CByte1) == 1);
HH_STATIC_ASSERT(sizeof(CByte2) == 2);
HH_STATIC_ASSERT(sizeof(CByte3) == 3);
HH_STATIC_ASSERT(sizeof(CByte4) == 4);

//----------------------------------------------------------------------------

//template<typename _Type, hh_u32 _Dimension>
//TVector<_Type, _Dimension>	PKSwapEndian(const TVector<_Type, _Dimension> &object)
//{
//	TVector<_Type, _Dimension>	out = object;
//	PKSwapEndianInPlace(out);
//	return out;
//}

namespace	Internal	// kr_memoryviews.h
{
	template<typename _Type, hh_u32 _Dimension>
	struct	TStreamEndianSwapperHelpers<TVector<_Type, _Dimension> >
	{
		static void	SwapInPlace(TVector<_Type, _Dimension> *data, hh_u32 count) { TNativeSwapper<sizeof(_Type)>::SwapInPlace(data, count * _Dimension); }
	};
}

template<typename _Type, hh_u32 _Dimension>
void	PKSwapEndianInPlace(TVector<_Type, _Dimension> &object)
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		PKSwapEndianInPlace(object.Axis(i));
	}
}

//----------------------------------------------------------------------------

//template<typename _TypeA, typename _TypeB, hh_u32 _Dimension>
//TVector<typename TNumericPromoter<_TypeA, _TypeB>::PromotedType, _Dimension>	HHMin(const TVector<_TypeA, _Dimension> &a, const TVector<_TypeB, _Dimension> &b)
//{
//	TVector<typename TNumericPromoter<_TypeA, _TypeB>::PromotedType, _Dimension>	output;
//	for (hh_u32 i = 0; i < _Dimension; i++)
//	{
//		output.Axis(i) = HHMin(a.Axis(i), b.Axis(i));
//	}
//	return output;
//}
//
//template<typename _TypeA, typename _TypeB, hh_u32 _Dimension>
//TVector<typename TNumericPromoter<_TypeA, _TypeB>::PromotedType, _Dimension>	HHMax(const TVector<_TypeA, _Dimension> &a, const TVector<_TypeB, _Dimension> &b)
//{
//	TVector<typename TNumericPromoter<_TypeA, _TypeB>::PromotedType, _Dimension>	output;
//	for (hh_u32 i = 0; i < _Dimension; i++)
//	{
//		output.Axis(i) = HHMax(a.Axis(i), b.Axis(i));
//	}
//	return output;
//}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHMin(const TVector<_Type, _Dimension> &a, const TVector<_Type, _Dimension> &b)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHMin(a.Axis(i), b.Axis(i));
	}
	return output;
}

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHMax(const TVector<_Type, _Dimension> &a, const TVector<_Type, _Dimension> &b)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHMax(a.Axis(i), b.Axis(i));
	}
	return output;
}

//----------------------------------------------------------------------------
// non-const versions, to avoid conflicts with non-const versions of generic HHMin/HHMax (usable as l-value)

#ifdef	PK_BUILD_MINMAX_AS_REFS

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHMin(TVector<_Type, _Dimension> &a, TVector<_Type, _Dimension> &b)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHMin(a.Axis(i), b.Axis(i));
	}
	return output;
}

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHMax(TVector<_Type, _Dimension> &a, TVector<_Type, _Dimension> &b)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHMax(a.Axis(i), b.Axis(i));
	}
	return output;
}

#endif

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension, typename _TypeFrac>
TVector<_Type, _Dimension>	HHLerp(const TVector<_Type, _Dimension> &a, const TVector<_Type, _Dimension> &b, const _TypeFrac &f)
{
	return a.Lerp(b, f);
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHAbs(const TVector<_Type, _Dimension> &v)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; ++i)
	{
		output.Axis(i) = HHAbs(v.Axis(i));
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHFloor(const TVector<_Type, _Dimension> &v)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; ++i)
	{
		output.Axis(i) = HHFloor(v.Axis(i));
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHCeil(const TVector<_Type, _Dimension> &v)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; ++i)
	{
		output.Axis(i) = HHCeil(v.Axis(i));
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension, typename _TypeMin, typename _TypeMax>
TVector<_Type, _Dimension>	HHClamp(const TVector<_Type, _Dimension> &v, const _TypeMin &min, const _TypeMax &max)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHClamp(v.Axis(i), min, max);
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension, typename _TypeMin, typename _TypeMax>
TVector<_Type, _Dimension>	HHClamp(const TVector<_Type, _Dimension> &v, const TVector<_TypeMin, _Dimension> &min, const TVector<_TypeMax, _Dimension> &max)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHClamp(v.Axis(i), min.Axis(i), max.Axis(i));
	}
	return output;
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
TVector<_Type, _Dimension>	HHSaturate(const TVector<_Type, _Dimension> &v)
{
	TVector<_Type, _Dimension>	output;
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		output.Axis(i) = HHSaturate(v.Axis(i));
	}
	return output;
}

//----------------------------------------------------------------------------
//
//	Traits
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Dimension>
class	TVectorTraits : public TNumericTypeQuality<TNumericTraits<_Type>::Quality + (_Dimension - 1) * 256>
{
	typedef	TVector<_Type, _Dimension>	SelfType;
public:
	static const bool	IsInteger = TTypeTraits::IsInteger<_Type>::True;
	static const bool	IsFloat = TTypeTraits::IsFloat<_Type>::True;
	static const bool	IsSigned = TTypeTraits::IsSigned<_Type>::True;

	static SelfType		Zero() { return SelfType(TNumericTraits<_Type>::Zero()); }
	static SelfType		One() { return SelfType(TNumericTraits<_Type>::One()); }
	static SelfType		Max() { return SelfType(TNumericTraits<_Type>::Max()); }
	static SelfType		Min() { return SelfType(TNumericTraits<_Type>::Min()); }
	static SelfType		Smallest() { return SelfType(TNumericTraits<_Type>::Smallest()); }
	static SelfType		SmallestDenormal() { return SelfType(TNumericTraits<_Type>::SmallestDenormal()); }
	static SelfType		PositiveInfinity() { return SelfType(TNumericTraits<_Type>::PositiveInfinity()); }
	static SelfType		NegativeInfinity() { return SelfType(TNumericTraits<_Type>::NegativeInfinity()); }
	static SelfType		Infinity() { return SelfType(TNumericTraits<_Type>::Infinity()); }	// alias for PositiveInfinity(). should we keep this?
	static SelfType		NaN() { return SelfType(TNumericTraits<_Type>::NaN()); }
	static void			SetInvalidDebug(SelfType &dst) { for (hh_u32 i = 0; i < _Dimension; i++) { TNumericTraits<_Type>::SetInvalidDebug(dst.Axis(i)); } }
	static SelfType		InvalidDebugInitializer() { SelfType invalidDbg; SetInvalidDebug(invalidDbg); return invalidDbg; }

	static bool			IsInfinite(const SelfType &value) { return value.IsInfinite(); }
	static bool			IsFinite(const SelfType &value) { return value.IsFinite(); }
	static bool			IsNan(const SelfType &value) { return value.IsNan(); }
};

//----------------------------------------------------------------------------

#define	HH_MATHS_VECTOR_BUILD_TRAITS(__type, __dimension)	template<> class TNumericTraits<TVector<__type, __dimension> > : public TVectorTraits<__type, __dimension> {};

HH_MATHS_VECTOR_BUILD_TRAITS(float, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(float, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(float, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(float, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(double, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(double, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(double, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(double, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(hh_u32, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u32, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u32, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u32, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(hh_i32, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i32, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i32, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i32, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(hh_u16, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u16, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u16, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u16, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(hh_i16, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i16, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i16, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i16, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(hh_u8, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u8, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u8, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_u8, 4);

HH_MATHS_VECTOR_BUILD_TRAITS(hh_i8, 1);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i8, 2);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i8, 3);
HH_MATHS_VECTOR_BUILD_TRAITS(hh_i8, 4);

//----------------------------------------------------------------------------
//
//	vector compare tools
//
//----------------------------------------------------------------------------

// returns true if any element of 'v' is non-zero
template<typename _Type, hh_u32 _Dimension>
bool	Any(const TVector<_Type, _Dimension> &v)
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (v.Axis(i) != 0)
			return true;
	}
	return false;
}

// returns true if all elements of 'v' are non-zero
template<typename _Type, hh_u32 _Dimension>
bool	All(const TVector<_Type, _Dimension> &v)
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (v.Axis(i) == 0)
			return false;
	}
	return true;
}

// returns true if all elements of 'v' are zero, equivalent to !Any()
template<typename _Type, hh_u32 _Dimension>
bool	None(const TVector<_Type, _Dimension> &v)
{
	for (hh_u32 i = 0; i < _Dimension; i++)
	{
		if (v.Axis(i) != 0)
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_maths/include/hh_maths_vector.inl"

#endif // __PK_MATHS_VECTOR_H__
