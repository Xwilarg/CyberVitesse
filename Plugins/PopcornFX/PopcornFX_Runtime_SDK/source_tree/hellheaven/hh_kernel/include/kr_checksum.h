#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/01 03:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_CHECKSUM_H__
#define	__KR_CHECKSUM_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
// digest/checksums
//
//----------------------------------------------------------------------------
//
//	MD5
//
//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT CDigestMD5
{
	static const hh_u32	_DataSize = 16;
	hh_u8				m_Data[_DataSize];

	CDigestMD5() { Clear(); }
	CDigestMD5(const void *buffer, hh_u32 byteCount) { Compute(buffer, byteCount); }

	void				Compute(const void *buffer, hh_u32 byteCount);
	void				Clear();

	bool				operator == (const CDigestMD5 &compareWith) const;
	bool				operator != (const CDigestMD5 &compareWith) const;
};

//----------------------------------------------------------------------------

class	HH_ALIGN(0x10) HH_KERNEL_EXPORT CBufferDigesterMD5
{
public:
	typedef CDigestMD5	_DigestType;
	static const hh_u32	_DataSize = _DigestType::_DataSize;
protected:
	hh_u32				m_WorkingBuffer[_DataSize];	// 64b	ALIGN: 16
	hh_u8				m_Padding[8];				//  8b	ALIGN: 16
	hh_u32				m_LengthInBits[2];			//  8b	ALIGN:  8
	hh_u32				m_ABCD[4];					// 16b	ALIGN: 16

	void				_TransformAligned(const hh_u8 *dataBlocks, hh_u32 blockCount64);
	void				_TransformUnaligned(const hh_u8 *dataBlocks, hh_u32 blockCount64);

public:
	CBufferDigesterMD5() { Reset(); }

	void				Reset();
	CBufferDigesterMD5	&Append(const void *buffer, hh_u32 bufferSize);
	template<typename _Type>
	CBufferDigesterMD5	&Append(const _Type &value) { return Append(&value, sizeof(value)); }

	void				Finalize(_DigestType &outDigest);
};
HH_STATIC_ASSERT(sizeof(CBufferDigesterMD5) == 96);

//----------------------------------------------------------------------------
//
//	SHA1
//
//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT CDigestSHA1
{
	static const hh_u32	_DataSize = 20;
	hh_u8				m_Data[_DataSize];

	CDigestSHA1() { Clear(); }
	CDigestSHA1(const void *buffer, hh_u32 byteCount) { Compute(buffer, byteCount); }

	void				Compute(const void *buffer, hh_u32 byteCount);
	void				Clear();

	bool				operator == (const CDigestSHA1 &compareWith) const;
	bool				operator != (const CDigestSHA1 &compareWith) const;
};

//----------------------------------------------------------------------------

class	HH_ALIGN(0x10) HH_KERNEL_EXPORT CBufferDigesterSHA1
{
public:
	typedef CDigestSHA1	_DigestType;
	static const hh_u32	_DataSize = _DigestType::_DataSize;
protected:
	hh_u32				m_WorkingBuffer/*[32]*/[80];	// 128b	ALIGN: 16	// 320b ALIGN: 16
	hh_u32				m_ABCDE[5];				//  20b	ALIGN: 16
	hh_u32				m_BufferLength;			//   4b ALIGN:  4
	hh_u32				m_LengthInBits[2];		//   8b ALIGN:  8

public:
	CBufferDigesterSHA1() { Reset(); }

	void				Reset();
	CBufferDigesterSHA1	&Append(const void *buffer, hh_u32 bufferSize);
	template<typename _Type>
	CBufferDigesterSHA1	&Append(const _Type &value) { return Append(&value, sizeof(value)); }

	void				Finalize(_DigestType &outDigest);
};
//HH_STATIC_ASSERT(sizeof(CBufferDigesterSHA1) == 160);
HH_STATIC_ASSERT(sizeof(CBufferDigesterSHA1) == 352);

//----------------------------------------------------------------------------
//
//	CRC
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CChecksumCRC32
{
public:
	static hh_u32		Digest(const void *buffer, hh_u32 sizeInBytes, hh_u32 crc = 0, const hh_u32 *crcLookup = null);
	static hh_u32		*BuildLookup(hh_u32 seed);
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CChecksumCRC16
{
public:
	static hh_u16		Digest(const void *buffer, hh_u32 sizeInBytes, hh_u16 crc = 0, const hh_u16 *crcLookup = null);
	static hh_u16		*BuildLookup(hh_u16 seed);
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CChecksumCRC8
{
public:
	static hh_u8		Digest(const void *buffer, hh_u32 sizeInBytes, hh_u8 crc = 0, const hh_u8 *crcLookup = null);
	static hh_u8		*BuildLookup(hh_u8 seed);
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CChecksumADLER32
{
public:
	static hh_u32		Digest(const void *buffer, hh_u32 sizeInBytes, hh_u32 adler = 1);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_CHECKSUM_H__
