#pragma once

//----------------------------------------------------------------------------
// Started on Wed 2003/07/02 05:25 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_HASH_H__
#define	__KR_HASH_H__

#include "hh_kernel/include/kr_string.h"
#include "hh_kernel/include/kr_buffer.h"	// FIXME: temp, only needed for SNativeStringUtils::Length(), forward declare instead?

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_Int
{
public:
	static hh_u32					Hash(hh_u32 data)
	{
		data += ~(data << 15);
		data ^= (data >> 10);
		data += (data << 3);
		data ^= (data >> 6);
		data += ~(data << 11);
		data ^= (data >> 16);
		return data;
	}
	static hh_u32					Hash(hh_u64 data)
	{
		data += ~(data << 32);
		data ^= (data >> 22);
		data += ~(data << 13);
		data ^= (data >> 8);
		data += (data << 3);
		data ^= (data >> 15);
		data += ~(data << 27);
		data ^= (data >> 31);
		return (hh_u32)data;
	}
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_Int3
{
public:
	static hh_u32					Hash(const CInt3 &data)
	{
		hh_i32	a = data.x();
		hh_i32	b = data.y();
		hh_i32	c = data.z();

		/*
		a -= b;  a -=c;  a ^= (c >>> 13);
		b -= c;  b -=a;  b ^= (a << 8); 
		c -= a;  c -=b;  c ^= (b >>> 13);
		a -= b;  a -=c;  a ^= (c >>> 12);
		b -= c;  b -=a;  b ^= (a << 16);
		c -= a;  c -=b;  c ^= (b >>> 5);
		a -= b;  a -=c;  a ^= (c >>> 3);
		b -= c;  b -=a;  b ^= (a << 10);
		c -= a;  c- =b;  c ^= (b >>> 15);
		*/
		a -= b; a -= c; a ^= (c >> 13);
		b -= c; b -= a; b ^= (a << 8);
		c -= a; c -= b; c ^= (b >> 13);
		a -= b; a -= c; a ^= (c >> 12);
		b -= c; b -= a; b ^= (a << 16);
		c -= a; c -= b; c ^= (b >> 5);
		a -= b; a -= c; a ^= (c >> 3);
		b -= c; b -= a; b ^= (a << 10);
		c -= a; c -= b; c ^= (b >> 15);
		return hh_u32(c);
	}
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_Hsieh
{
public:
	static hh_u32					Hash(const void *data, hh_u32 length, hh_u32 incrementalHash);
	HH_FORCEINLINE static hh_u32	Hash(const void *data, hh_u32 length) { return Hash(data, length, length); }
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_Jenkins
{
public:
	static hh_u32					Hash(const void *data, hh_u32 length, hh_u32 incrementalHash);
	HH_FORCEINLINE static hh_u32	Hash(const void *data, hh_u32 length) { return Hash(data, length, length); }
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_DJB2
{
public:
	static hh_u32					Hash(const void *data, hh_u32 length, hh_u32 incrementalHash);
	HH_FORCEINLINE static hh_u32	Hash(const void *data, hh_u32 length) { return Hash(data, length, 5381); }
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_SDBM
{
public:
	static hh_u32					Hash(const void *data, hh_u32 length, hh_u32 incrementalHash);
	HH_FORCEINLINE static hh_u32	Hash(const void *data, hh_u32 length) { return Hash(data, length, 0); }
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CRawHasher_Dummy
{
public:
	static hh_u32					Hash(const void *, hh_u32, hh_u32 /*incrementalHash*/) { return 0; }
	HH_FORCEINLINE static hh_u32	Hash(const void *data, hh_u32 length) { return Hash(data, length, length); }
};

//----------------------------------------------------------------------------
//
//	generic type hashers
//
//----------------------------------------------------------------------------

template<typename _DataType, typename _RawHasher = CRawHasher_Hsieh>
class	TTypeHasher
{
public:
	static hh_u32	Hash(const _DataType &bufferToHash)
	{
		return _RawHasher::Hash(&bufferToHash, sizeof(bufferToHash));
	}

	static hh_u32	Hash(const _DataType &bufferToHash, hh_u32 incrementalHash)
	{
		return _RawHasher::Hash(&bufferToHash, sizeof(bufferToHash), incrementalHash);
	}
};

template<typename _RawHasher>
class	TTypeHasher<char*, _RawHasher>
{
public:
	static hh_u32	Hash(char * const &bufferToHash)
	{
		return _RawHasher::Hash(bufferToHash, SNativeStringUtils::Length(bufferToHash));
	}
};

template<typename _RawHasher>
class	TTypeHasher<CString, _RawHasher>
{
public:
	static hh_u32	Hash(const CString &bufferToHash)
	{
		return _RawHasher::Hash(bufferToHash.Data(), bufferToHash.Length());
	}

	static hh_u32	Hash(const CStringView &bufferToHash)
	{
		return _RawHasher::Hash(bufferToHash.Data(), bufferToHash.Length());
	}

	static hh_u32	Hash(const char *bufferToHash)
	{
		return _RawHasher::Hash(bufferToHash, SNativeStringUtils::Length(bufferToHash));
	}
};

template<typename _RawType, typename _RawHasher>
class	TTypeHasher<TValidityTypeWrapper<_RawType>, _RawHasher>
{
public:
	static hh_u32	Hash(const TValidityTypeWrapper<_RawType> &bufferToHash)
	{
		return TTypeHasher<_RawType, _RawHasher>::Hash(bufferToHash.Data());
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif
