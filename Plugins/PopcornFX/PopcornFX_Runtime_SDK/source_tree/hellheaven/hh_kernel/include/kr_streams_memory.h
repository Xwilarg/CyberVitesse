#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2008/07/29 20:20 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_STREAMS_MEMORY_H__
#define __KR_STREAMS_MEMORY_H__

#include "hh_kernel/include/kr_streams.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<bool _ReadOnly> class					TMemoryStreamConstSwitcher : public CStream {};
template<> class								TMemoryStreamConstSwitcher<true> : public CStreamReadOnly {};

template<typename _Type, bool _ReadOnly> struct	TMemoryStreamConstTyper { typedef _Type	Type; };
template<typename _Type> struct					TMemoryStreamConstTyper<_Type, true> { typedef const _Type	Type; };

//----------------------------------------------------------------------------

template<bool _ReadOnly>
class	TMemoryStream : public TMemoryStreamConstSwitcher<_ReadOnly>
{
private:
protected:
	typedef typename TMemoryStreamConstTyper<hh_u8, _ReadOnly>::Type *	_RawDataType;
	typedef typename TMemoryStreamConstTyper<void, _ReadOnly>::Type *	_RawInputType;

	_RawDataType	m_Data;
	hh_u32			m_Size;
	hh_u32			m_Pos;

public:
	TMemoryStream(_RawInputType data, hh_u32 size) : m_Data((_RawDataType)data), m_Size(size), m_Pos(0) {}

	virtual hh_u64	Read(void *targetBuffer, hh_u64 byteCount) override;
	virtual bool	Seek(hh_i64 offset, CStream::ESeekMode whence) override;
	virtual hh_u64	Tell() const override { return m_Pos; }
	virtual bool	Eof() const override { HH_ASSERT(m_Pos <= m_Size); return m_Size == m_Pos; }
	virtual void	Close() override { m_Data = null; m_Size = 0; m_Pos = 0; }
	virtual hh_u64	SizeInBytes() const override { return m_Size; }

	HH_FORCEINLINE bool			Empty() const { return m_Size == 0 || m_Data == null; }	// FIXME: add in CStream ?
	HH_FORCEINLINE _RawDataType	RawData() { return m_Data; }
};

//----------------------------------------------------------------------------
//
//	TMemoryStream Implementation:
//
//----------------------------------------------------------------------------

template<bool _ReadOnly>
hh_u64	TMemoryStream<_ReadOnly>::Read(void *targetBuffer, hh_u64 byteCount)
{
	hh_u32	bcount = checked_numcast<hh_u32>(byteCount);
	if (m_Pos + bcount > m_Size)
		return 0;

	memcpy(targetBuffer, m_Data + m_Pos, bcount);
	m_Pos += bcount;
	return bcount;
}

//----------------------------------------------------------------------------

template<bool _ReadOnly>
bool	TMemoryStream<_ReadOnly>::Seek(hh_i64 offset, CStream::ESeekMode whence)
{
	switch (whence)
	{
	case	CStream::SeekSet:
		HH_ASSERT(checked_numcast<hh_u32>(offset) <= m_Size);
		m_Pos = checked_numcast<hh_u32>(offset);
		break;
	case	CStream::SeekCur:
		HH_ASSERT(checked_numcast<hh_u32>(m_Pos + offset) <= m_Size);
		m_Pos += offset;
		break;
//	case	CStream::SeekEnd:
//		HH_ASSERT(checked_numcast<hh_u32>(m_Pos + offset) <= m_Size);
//		m_Pos = m_Size + offset;
//		break;
	default:
		HH_ASSUME_NOT_REACHED();
	}

	if (m_Pos <= m_Size)
		return true;

	m_Pos = m_Size;	// keep a 'valid' pos (here, EOF)
	return false;
}

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CMemoryStream : public TMemoryStream<false>
{
public:
	CMemoryStream(void *data, hh_u32 size) : TMemoryStream<false>(data, size) {}

	virtual hh_u64	Write(const void *sourceBuffer, hh_u64 byteCount) override;
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CConstMemoryStream : public TMemoryStream<true>
{
public:
	CConstMemoryStream(const void *data, hh_u32 size) : TMemoryStream<true>(data, size) {}
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CDynamicMemoryStream : public TMemoryStream<false>
{
public:
	CDynamicMemoryStream() : TMemoryStream<false>(null, 0) {}
	~CDynamicMemoryStream();

	void			OpenResizableMemoryBuffer(void *buffer, hh_u32 byteCount) { Close(); m_Data = (hh_u8*)buffer; m_Size = byteCount; m_Pos = 0; }

	virtual hh_u64	Write(const void *sourceBuffer, hh_u64 byteCount) override;
	virtual void	Close() override;
	virtual hh_u64	SizeInBytes() const override;	// this might be problematic for memory buffers

	// call this to retrieve the buffer
	hh_u8			*ExportDataAndClose(hh_u32 &finalSize) { hh_u8 *buffer = m_Data; finalSize = m_Pos; m_Data = null; m_Size = 0; m_Pos = 0; return buffer; }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_STREAMS_MEMORY_H__
