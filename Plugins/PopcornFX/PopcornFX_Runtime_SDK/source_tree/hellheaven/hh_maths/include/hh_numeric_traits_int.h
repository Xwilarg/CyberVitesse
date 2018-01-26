#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2007/11/14 17:28 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PK_NUMERIC_TRAITS_INT_H__
#define __PK_NUMERIC_TRAITS_INT_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<>
class	TNumericTraits<bool> : public CGenericNumericTraits<bool>, public TNumericTypeQuality<0>
{
public:
	typedef	bool	SelfType;
	typedef bool	SignedType;
	typedef bool	UnsignedType;

	static const SelfType	kMax = true;
	static const SelfType	kMin = false;

	HH_FORCEINLINE static SelfType	Zero() { return false; }
	HH_FORCEINLINE static SelfType	One() { return true; }
	HH_FORCEINLINE static SelfType	Max() { return true; }
	HH_FORCEINLINE static SelfType	Min() { return false; }

	static SelfType	InvalidDebugInitializer() { return true; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_u8> : public CGenericNumericTraits<hh_u8>, public TNumericTypeQuality<10>
{
public:
	typedef	hh_u8	SelfType;
	typedef hh_i8	SignedType;
	typedef hh_u8	UnsignedType;

	static const SelfType	kMax = 0xFF;
	static const SelfType	kMin = 0;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0xFF; }
	HH_FORCEINLINE static SelfType	Min() { return 0; }

	static SelfType	InvalidDebugInitializer() { return 0xCD; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_i8> : public CGenericNumericTraits<hh_i8>, public TNumericTypeQuality<11>
{
public:
	typedef	hh_i8	SelfType;
	typedef hh_i8	SignedType;
	typedef hh_u8	UnsignedType;

	static const SelfType	kMax = 0x7F;
	static const SelfType	kMin = 0 - 0x80;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0x7F; }
	HH_FORCEINLINE static SelfType	Min() { return 0 - 0x80; }

	static SelfType	InvalidDebugInitializer() { hh_u8 data = 0xCD; return data; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_u16> : public CGenericNumericTraits<hh_u16>, public TNumericTypeQuality<20>
{
public:
	typedef	hh_u16	SelfType;
	typedef hh_i16	SignedType;
	typedef hh_u16	UnsignedType;

	static const SelfType	kMax = 0xFFFF;
	static const SelfType	kMin = 0;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0xFFFF; }
	HH_FORCEINLINE static SelfType	Min() { return 0; }

	static SelfType	InvalidDebugInitializer() { return 0xCDCD; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_i16> : public CGenericNumericTraits<hh_i16>, public TNumericTypeQuality<21>
{
public:
	typedef	hh_i16	SelfType;
	typedef hh_i16	SignedType;
	typedef hh_u16	UnsignedType;

	static const SelfType	kMax = 0x7FFF;
	static const SelfType	kMin = 0 - 0x8000;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0x7FFF; }
	HH_FORCEINLINE static SelfType	Min() { return 0 - 0x8000; }

	static SelfType	InvalidDebugInitializer() { hh_u16 data = 0xCDCD; return data; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_u32> : public CGenericNumericTraits<hh_u32>, public TNumericTypeQuality<30>
{
public:
	typedef	hh_u32	SelfType;
	typedef hh_i32	SignedType;
	typedef hh_u32	UnsignedType;

	static const SelfType	kMax = 0xFFFFFFFF;
	static const SelfType	kMin = 0;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0xFFFFFFFF; }
	HH_FORCEINLINE static SelfType	Min() { return 0; }

	static SelfType	InvalidDebugInitializer() { return 0xCDCDCDCD; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_i32> : public CGenericNumericTraits<hh_i32>, public TNumericTypeQuality<31>
{
public:
	typedef	hh_i32	SelfType;
	typedef hh_i32	SignedType;
	typedef hh_u32	UnsignedType;

	static const SelfType	kMax = 0x7FFFFFFF;
	static const SelfType	kMin = -2147483647L - 1L;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0x7FFFFFFF; }
	HH_FORCEINLINE static SelfType	Min() { return -2147483647L - 1L; }

	static SelfType	InvalidDebugInitializer() { const hh_u32 data = 0xCDCDCDCD; return data; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_u64> : public CGenericNumericTraits<hh_u64>, public TNumericTypeQuality<40>
{
public:
	typedef	hh_u64	SelfType;
	typedef hh_i64	SignedType;
	typedef hh_u64	UnsignedType;

	static const SelfType	kMax = 0xFFFFFFFFFFFFFFFFULL;
	static const SelfType	kMin = 0;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0xFFFFFFFFFFFFFFFFULL; }
	HH_FORCEINLINE static SelfType	Min() { return 0; }

	static SelfType	InvalidDebugInitializer() { return 0xCDCDCDCDCDCDCDCDULL; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------

template<>
class	TNumericTraits<hh_i64> : public CGenericNumericTraits<hh_i64>, public TNumericTypeQuality<41>
{
public:
	typedef	hh_i64	SelfType;
	typedef hh_i64	SignedType;
	typedef hh_u64	UnsignedType;

	static const SelfType	kMax = 0x7FFFFFFFFFFFFFFFLL;
	static const SelfType	kMin = -9223372036854775807LL - 1LL;

	HH_FORCEINLINE static SelfType	Zero() { return 0; }
	HH_FORCEINLINE static SelfType	One() { return 1; }
	HH_FORCEINLINE static SelfType	Max() { return 0x7FFFFFFFFFFFFFFFLL; }
	HH_FORCEINLINE static SelfType	Min() { return -9223372036854775807LL - 1LL; }

	static SelfType	InvalidDebugInitializer() { return 0xCDCDCDCDCDCDCDCDLL; }
	static void		SetInvalidDebug(SelfType &dst) { dst = InvalidDebugInitializer(); }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PK_NUMERIC_TRAITS_INT_H__
