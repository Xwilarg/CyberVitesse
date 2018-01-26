#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/09/24 08:55 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_REFCOUNTED_BUFFER_H__
#define	__KR_REFCOUNTED_BUFFER_H__

#include "hh_kernel/include/kr_refptr.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	ref-counted memory buffer.
//
//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(RefCountedMemoryBuffer);

class	HH_KERNEL_EXPORT CRefCountedMemoryBuffer : public CRefCountedObject
{
private:
	void	*m_DataStart;
	hh_u32	m_DataSizeInBytes;

	HH_FORCEINLINE CRefCountedMemoryBuffer(void *dataStart, hh_u32 byteCount) : m_DataStart(dataStart), m_DataSizeInBytes(byteCount) { HH_ASSERT(dataStart != null && byteCount != 0); }
	static CRefCountedMemoryBuffer					*_SetupFromFreshAllocatedBuffer(void *data, hh_u32 byteCount, hh_u32 alignment = 1, const CRefCountedMemoryBuffer *dupFrom = null);
	static void										*_RawAlloc(hh_u32 bc) { return Mem::_RawAlloc(bc, 0, Mem::Origin_New KR_MEM_DEBUG_ARGS); }
	static void										*_RawCalloc(hh_u32 bc) { void *data = Mem::_RawAlloc(bc, 0, Mem::Origin_New KR_MEM_DEBUG_ARGS); Mem::Clear(data, bc); return data; }

public:
	// Will not free the memory
	static HH_FORCEINLINE PRefCountedMemoryBuffer	FromExistingBuffer(void *buffer, hh_u32 byteCount) { return HH_NEW(CRefCountedMemoryBuffer(buffer, byteCount)); }

	// Non-aligned allocations
	static HH_FORCEINLINE PRefCountedMemoryBuffer	Alloc(hh_u32 byteCount) { const hh_u32 bc = sizeof(CRefCountedMemoryBuffer) + byteCount; return _SetupFromFreshAllocatedBuffer(_RawAlloc(bc), byteCount); }
	static HH_FORCEINLINE PRefCountedMemoryBuffer	Calloc(hh_u32 byteCount) { const hh_u32 bc = sizeof(CRefCountedMemoryBuffer) + byteCount; return _SetupFromFreshAllocatedBuffer(_RawCalloc(bc), byteCount); }

	// Aligned allocations, alignments must be powers of two > 0 ('1' is included)
	static HH_FORCEINLINE PRefCountedMemoryBuffer	AllocAligned(hh_u32 byteCount, hh_u32 alignment = 0x10) { const hh_u32 bc = sizeof(CRefCountedMemoryBuffer) + byteCount + alignment - 1; return _SetupFromFreshAllocatedBuffer(_RawAlloc(bc), byteCount, alignment); }
	static HH_FORCEINLINE PRefCountedMemoryBuffer	CallocAligned(hh_u32 byteCount, hh_u32 alignment = 0x10) { const hh_u32 bc = sizeof(CRefCountedMemoryBuffer) + byteCount + alignment - 1; return _SetupFromFreshAllocatedBuffer(_RawCalloc(bc), byteCount, alignment); }

	// Duplicate another existing buffer
	static PRefCountedMemoryBuffer					Dup(const PRefCountedMemoryBuffer &other, hh_u32 newAlignment = 0);	// 0 means: reuse the previous buffer's alignment

	// Accessors to get the data and its size
	template<typename _Type> HH_FORCEINLINE _Type	*Data() { return reinterpret_cast<_Type*>(m_DataStart); }
	template<typename _Type> HH_FORCEINLINE const typename TType_RemoveConstness<_Type>::Type	*Data() const { return reinterpret_cast<const typename TType_RemoveConstness<_Type>::Type*>(m_DataStart); }
	HH_FORCEINLINE hh_u32							DataSizeInBytes() const { return m_DataSizeInBytes; }
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_REFCOUNTED_BUFFER_H__
