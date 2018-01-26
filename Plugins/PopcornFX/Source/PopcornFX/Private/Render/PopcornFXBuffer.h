//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"

#include "RenderResource.h"

#include "PopcornFXSDK.h"
#include <pk_drawers/include/dr_bufferpool.h>

//----------------------------------------------------------------------------

class	FPopcornFXVertexBuffer : public FVertexBuffer
{
public:
	enum : uint32 { Alignment = 0x10 };
	enum : uint32
	{
		Flag_ForGPU		= (1U << 31),
		Flag_StrideMask	= (1U << 31) - 1,
	};

	FPopcornFXVertexBuffer()
		: m_CapacityInBytes(0), m_Flags(0), m_AllocatedCount(0), m_CurrentMap(null)
	{ }

	hh_u32			CapacityInBytes() const { return m_CapacityInBytes; }

	bool			HardResize(hh_u32 sizeInBytes, bool forGPU = false);
	void			Release() { ReleaseResource(); }

	bool			ForGPU() const { return (m_Flags & Flag_ForGPU) != 0; }
	void			SetForGPU(bool forGpu) { if (forGpu) m_Flags |= Flag_ForGPU; else m_Flags &= ~Flag_ForGPU; }
	bool			Mappable() const { return !ForGPU(); }
	bool			Mapped() const { return m_CurrentMap != null; }
	void			*RawMap(hh_u32 count, hh_u32 stride);
	void			Unmap();

	void			_SetAllocated(hh_u32 count, hh_u32 stride) { m_AllocatedCount = count; HH_ASSERT(stride < Flag_StrideMask); m_Flags = (m_Flags & ~Flag_StrideMask) | stride; }
	hh_u32			AllocatedStride() const { return m_Flags & Flag_StrideMask; }
	hh_u32			AllocatedCount() const { return m_AllocatedCount; }
	hh_u32			AllocatedSize() const { return AllocatedCount() * AllocatedStride(); }

	template <typename _Type, hh_i32 _FootprintInBytes = -1>
	bool			Map(TStridedMemoryView<_Type, _FootprintInBytes> &outMap, hh_u32 count, hh_u32 stride)
	{
		void	*map = RawMap(count, stride);
		if (map == null)
		{
			outMap.Clear();
			return false;
		}
		outMap = TStridedMemoryView<_Type, _FootprintInBytes>(reinterpret_cast<_Type*>(map), count, stride);
		return true;
	}

	template <typename _Type, hh_i32 _FootprintInBytes = -1>
	bool			Map(TStridedMemoryView<_Type, _FootprintInBytes> &outPreSetupedMap)
	{
		HH_ASSERT(outPreSetupedMap.Count() > 0 && outPreSetupedMap.Stride() > 0 && outPreSetupedMap.Data() == null);
		return Map(outPreSetupedMap, outPreSetupedMap.Count(), outPreSetupedMap.Stride());
	}

	const FUnorderedAccessViewRHIRef		&UAV();
	const FShaderResourceViewRHIRef			&SRV();

protected:
	virtual void	ReleaseRHI() override;
	virtual void	InitRHI() override;

private:

private:
	hh_u32							m_CapacityInBytes;
	hh_u32							m_Flags;
	hh_u32							m_AllocatedCount;
	void							*m_CurrentMap;
	FUnorderedAccessViewRHIRef		m_UAV;
	FShaderResourceViewRHIRef		m_SRV;
};

//----------------------------------------------------------------------------

class	FPopcornFXIndexBuffer : public FIndexBuffer
{
public:
	enum : uint32 { Alignment = 0x10 };
	enum : uint32
	{
		Flag_Large		= (1U << 0),
		Flag_ForGPU		= (1U << 1),
	};

	FPopcornFXIndexBuffer()
		: m_Capacity(0), m_Flags(0), m_AllocatedCount(0), m_CurrentMap(null)
	{ }

	virtual void	ReleaseRHI() override;
	virtual void	InitRHI() override;

	bool			Large() const { return m_Flags & Flag_Large; }
	hh_u32			Stride() const { return Large() ? sizeof(hh_u32) : sizeof(hh_u16); }
	hh_u32			Capacity() const { return m_Capacity; }

	bool			HardResize(hh_u32 count, bool forGPU = false);
	void			Release() { ReleaseResource(); }

	bool			ForGPU() const { return (m_Flags & Flag_ForGPU) != 0; }
	void			SetForGPU(bool forGpu) { if (forGpu) m_Flags |= Flag_ForGPU; else m_Flags &= ~Flag_ForGPU; }
	bool			Mappable() const { return !ForGPU(); }
	void			*RawMap(hh_u32 count);
	bool			Mapped() const { return m_CurrentMap != null; }
	void			Unmap();

	void			_SetAllocated(hh_u32 count) { m_AllocatedCount = count; }
	hh_u32			AllocatedCount() const { return m_AllocatedCount; }
	hh_u32			AllocatedStride() const { return Stride(); }
	hh_u32			AllocatedSize() const { return AllocatedCount() * AllocatedStride(); }

	bool			Map(void *&outMap, bool &outLarge, hh_u32 count)
	{
		outLarge = Large();
		void	*map = RawMap(count);
		if (map == null)
			return false;
		outMap = map;
		return true;
	}

	bool			Map(TMemoryView<hh_u32> &outMap, hh_u32 count)
	{
		HH_ASSERT(Large());
		void	*map = RawMap(count);
		if (map == null)
		{
			outMap.Clear();
			return false;
		}
		outMap = TMemoryView<hh_u32>(reinterpret_cast<hh_u32*>(map), count);
		return true;
	}
	bool			Map(TMemoryView<hh_u16> &outMap, hh_u32 count)
	{
		HH_ASSERT(!Large());
		void	*map = RawMap(count);
		if (map == null)
		{
			outMap.Clear();
			return false;
		}
		outMap = TMemoryView<hh_u16>(reinterpret_cast<hh_u16*>(map), count);
		return true;
	}

	const FUnorderedAccessViewRHIRef		&UAV();
	const FShaderResourceViewRHIRef			&SRV();

private:
	hh_u32			m_Capacity;
	hh_u32			m_Flags;
	hh_u32			m_AllocatedCount;
	void			*m_CurrentMap;
	FUnorderedAccessViewRHIRef		m_UAV;
	FShaderResourceViewRHIRef		m_SRV;
};

//----------------------------------------------------------------------------
//
// BufferPools
//
//----------------------------------------------------------------------------

struct SPopcornFXBufferPoolAllocatorPolicy
{
public:
	enum { kFrameTicksBeforeReuse = 2U };
	enum { kGCTicksBeforeRelease = 4U };
	enum { kLevelCount = 20U };
	enum { kBasePow = 9 };
	hh_u32		LevelToSize(hh_u32 level)
	{
		return 1 << (level + kBasePow);
	}
	hh_u32		SizeToLevel(hh_u32 size)
	{
		return PopcornFX::HHMax(PopcornFX::IntegerTools::Log2(size), hh_u32(kBasePow)) - kBasePow + 1;
	}
	hh_u32		LevelCountToLookahead(hh_u32 level)
	{
		return (kLevelCount - level) * 1 / 4;
		//return 1U + (HHMax(hh_u32(kMaxLevel) - level, 8U) - 8U) / 2U;
	}
};

//----------------------------------------------------------------------------

class CBufferPool_VertexBufferPolicy : public PopcornFX::TBaseBufferPoolPolicy<FPopcornFXVertexBuffer, SPopcornFXBufferPoolAllocatorPolicy>
{
public:
	bool		m_ForGPU = false;
protected:
	bool		BufferAllocate(CBuffer &buffer, hh_u32 sizeInBytes)
	{
		check(IsInRenderingThread());
		return buffer.HardResize(sizeInBytes, m_ForGPU);
	}

	void		BufferRelease(CBuffer &buffer)
	{
		check(IsInRenderingThread());
		buffer.Release();
	}
};

//----------------------------------------------------------------------------

class CBufferPool_IndexBufferPolicy : public PopcornFX::TBaseBufferPoolPolicy<FPopcornFXIndexBuffer, SPopcornFXBufferPoolAllocatorPolicy>
{
public:
	bool		m_ForGPU = false;
protected:
	bool		BufferAllocate(CBuffer &buffer, hh_u32 sizeInBytes)
	{
		check(IsInRenderingThread());
		return buffer.HardResize(sizeInBytes, m_ForGPU);
	}

	void		BufferRelease(CBuffer &buffer)
	{
		check(IsInRenderingThread());
		buffer.Release();
	}
};

//----------------------------------------------------------------------------

class CVertexBufferPool : public PopcornFX::TBufferPool < FPopcornFXVertexBuffer, CBufferPool_VertexBufferPolicy >
{
public:
	typedef PopcornFX::TBufferPool < FPopcornFXVertexBuffer, CBufferPool_VertexBufferPolicy > Super;

	template <typename _Type, hh_i32 _FootprintInBytes>
	bool		AllocateAndMapIf(bool doIt, CPooledBuffer &outPBuffer, TStridedMemoryView<_Type, _FootprintInBytes> &outPreSetupedMap)
	{
		outPBuffer.Clear();
		if (doIt)
		{
			HH_ASSERT(outPreSetupedMap.Count() > 0 && outPreSetupedMap.Stride() > 0 && outPreSetupedMap.Data() == null);
			if (!Super::Allocate(outPBuffer, outPreSetupedMap.CoveredBytes()))
				return false;
			outPBuffer->_SetAllocated(outPreSetupedMap.Count(), outPreSetupedMap.Stride());

			if (!outPBuffer->Map(outPreSetupedMap))
				return false;
		}
		else
			outPreSetupedMap = TStridedMemoryView<_Type, _FootprintInBytes>();
		return true;
	}

	bool		AllocateIf(bool doIt, CPooledBuffer &outPBuffer, hh_u32 count, hh_u32 stride)
	{
		outPBuffer.Clear();
		if (doIt)
		{
			HH_ASSERT(count > 0);
			if (!Super::Allocate(outPBuffer, count * stride))
				return false;
			outPBuffer->_SetAllocated(count, stride);
		}
		return true;
	}

	bool		Allocate(CPooledBuffer &outPBuffer, hh_u32 count, hh_u32 stride)
	{
		HH_ASSERT(count > 0);
		outPBuffer.Clear();
		if (!Super::Allocate(outPBuffer, count * stride))
			return false;
		outPBuffer->_SetAllocated(count, stride);
		return true;
	}
};

//----------------------------------------------------------------------------

class CIndexBufferPool : public PopcornFX::TBufferPool < FPopcornFXIndexBuffer, CBufferPool_IndexBufferPolicy >
{
public:
	typedef PopcornFX::TBufferPool < FPopcornFXIndexBuffer, CBufferPool_IndexBufferPolicy >		Super;

	bool		AllocateAndMapIf(bool doIt, CPooledBuffer &outPBuffer, void *&outMap, bool &outLargeIndices, hh_u32 indexCount)
	{
		outPBuffer.Clear();
		outLargeIndices = false;
		outMap = null;
		if (doIt)
		{
			if (!Super::Allocate(outPBuffer, indexCount))
				return false;
			outPBuffer->_SetAllocated(indexCount);
			if (!outPBuffer->Map(outMap, outLargeIndices, indexCount))
				return false;
		}
		return true;
	}

	bool		AllocateIf(bool doIt, CPooledBuffer &outPBuffer, bool &outLargeIndices, hh_u32 indexCount)
	{
		outPBuffer.Clear();
		outLargeIndices = false;
		if (doIt)
		{
			if (!Super::Allocate(outPBuffer, indexCount))
				return false;
			outPBuffer->_SetAllocated(indexCount);
			outLargeIndices = outPBuffer->Large();
		}
		return true;
	}


	bool		Allocate(CPooledBuffer &outPBuffer, bool &outLargeIndices, hh_u32 indexCount)
	{
		outPBuffer.Clear();
		outLargeIndices = false;
		if (!Super::Allocate(outPBuffer, indexCount))
			return false;
		outPBuffer->_SetAllocated(indexCount);
		outLargeIndices = outPBuffer->Large();
		return true;
	}
};

//----------------------------------------------------------------------------

class CPooledVertexBuffer : public CVertexBufferPool::CPooledBuffer
{
public:
	void		UnmapAndClear()
	{
		if (Valid())
		{
			if (Buffer()->Mapped())
				Buffer()->Unmap();
			Clear();
		}
	}
	void		Unmap()
	{
		if (Valid())
		{
			if (Buffer()->Mapped())
				Buffer()->Unmap();
		}
	}
};

class CPooledIndexBuffer : public CIndexBufferPool::CPooledBuffer
{
public:
	void		UnmapAndClear()
	{
		if (Valid())
		{
			if (Buffer()->Mapped())
				Buffer()->Unmap();
			Clear();
		}
	}
	void		Unmap()
	{
		if (Valid())
		{
			if (Buffer()->Mapped())
				Buffer()->Unmap();
		}
	}
};

//typedef CVertexBufferPool::CPooledBuffer	CPooledVertexBuffer;
//typedef CIndexBufferPool::CPooledBuffer		CPooledIndexBuffer;

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// Generic Buffer View
//
//----------------------------------------------------------------------------

class	FPopcornFXVertexBufferView
{
protected:
	FVertexBuffer			*m_Buffer = null;
	uint32					m_Offset = 0;
	uint32					m_Stride = 0;
	CPooledVertexBuffer		m_ForRefCount;

public:
	bool				Valid() const { return !Empty(); }
	bool				Empty() const { return m_Buffer == null; }

	uint32				Offset() const { return m_Offset; }
	uint32				Stride() const { return m_Stride; }
	const FVertexBuffer		*VertexBufferPtr() const { return m_Buffer; }
	FVertexBuffer			*VertexBufferPtr() { return m_Buffer; }

	void		Clear()
	{
		if (Empty())
		{
			HH_ASSERT(m_Buffer == null);
			HH_ASSERT(m_Offset == 0);
			HH_ASSERT(m_Stride == 0);
			HH_ASSERT(!m_ForRefCount.Valid());
			return;
		}
		m_Buffer = null;
		m_Offset = 0;
		m_Stride = 0;
		m_ForRefCount.Clear();
	}

	void		Setup(FVertexBuffer *buffer, uint32 offset, uint32 stride)
	{
		Clear();
		HH_ASSERT(buffer != null);
		m_Buffer = buffer;
		HH_ASSERT(m_Buffer->IsInitialized());
		m_Offset = offset;
		m_Stride = stride;
	}

	void		Setup(const FPopcornFXVertexBufferView &other)
	{
		*this = other;
	}

	void		Setup(const CPooledVertexBuffer &buffer)
	{
		Clear();
		if (buffer.Valid())
		{
			m_ForRefCount = buffer;
			m_Buffer = buffer.Buffer();
			HH_ASSERT(m_Buffer->IsInitialized());
			m_Offset = 0;
			m_Stride = buffer->AllocatedStride();
		}
	}

};

inline bool		IsValidRef(const FPopcornFXVertexBufferView &buffer)
{
	return buffer.Valid();
}
