#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2003/07/05 12:02 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_STREAMS_H__
#define	__KR_STREAMS_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CStreamReadOnly : public CRefCountedObject
{
public:
	virtual ~CStreamReadOnly() {}

	enum	ESeekMode
	{
		SeekSet = 0,
		SeekCur,
		__MaxSeeks
	};

	virtual hh_u64	Read(void *targetBuffer, hh_u64 byteCount) = 0;
	virtual bool	Seek(hh_i64 offset, ESeekMode whence) = 0;
	virtual hh_u64	Tell() const = 0;
	virtual bool	Eof() const = 0;
	virtual void	Close() = 0;

	virtual hh_u64	SizeInBytes() const = 0;	// this might be problematic for memory buffers

	template<typename _Type>
	bool			Read(_Type &object) { return Read(&object, sizeof(object)) == sizeof(object); }
	template<typename _Type>
	bool			ReadLittleEndian(_Type &object) { if (Read(&object, sizeof(object)) != sizeof(object)) return false; PKSwapLittleEndianInPlace(object); return true; }
	template<typename _Type>
	bool			ReadBigEndian(_Type &object) { if (Read(&object, sizeof(object)) != sizeof(object)) return false; PKSwapBigEndianInPlace(object); return true; }

	// FIXME: move CFileStream::Format right here ?

	// FIXME: directly overload Read() ?
	virtual bool	ReadPString(CString &string);	// allow higher classes to re-implement this in a more efficient way
};

HH_DECLARE_REFPTRCLASS(StreamReadOnly);

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CStream : public CStreamReadOnly
{
public:
	virtual ~CStream() {}

	virtual hh_u64	Write(const void *sourceBuffer, hh_u64 byteCount) = 0;

	// WARNING: do _not_ add a templatized method Write(const _Type &object) { return Write(&object, sizeof(object)); }
	// we want to keep these explicit to avoid inadvertently writing objects (that would require endianness swaps) through silent calls to Write() instead of Write*Endian

	template<typename _Type>
	bool			Write(const _Type &object) { return Write(&object, sizeof(object)) == sizeof(object); }
	template<typename _Type>
	bool			WriteLittleEndian(const _Type &object) { _Type swapped = object; PKSwapLittleEndianInPlace(swapped); return Write(&swapped, sizeof(swapped)) == sizeof(object); }
	template<typename _Type>
	bool			WriteBigEndian(const _Type &object) { _Type swapped = object; PKSwapBigEndianInPlace(swapped); return Write(&swapped, sizeof(swapped)) == sizeof(object); }

	// FIXME: directly overload Write() ?
	virtual bool	WritePString(const CString &string);	// allow higher classes to re-implement this in a more efficient way
};

HH_DECLARE_REFPTRCLASS(Stream);

//----------------------------------------------------------------------------
__PK_API_END

#endif
