//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXBuffer.h"

#include "Render/MyRHIStuff.h"

//----------------------------------------------------------------------------
//
// FPopcornFXVertexBuffer
//
//----------------------------------------------------------------------------

void	FPopcornFXVertexBuffer::ReleaseRHI()
{
	HH_ASSERT(!Mapped());
	m_CapacityInBytes = 0;
	m_AllocatedCount = 0;
	FVertexBuffer::ReleaseRHI();
}

void	FPopcornFXVertexBuffer::InitRHI()
{
	m_UAV = null;
	m_SRV = null;
	if (m_CapacityInBytes > 0)
	{
		FRHIResourceCreateInfo	emptyInformations;
		uint32			usage = 0;
		if (ForGPU())
			usage |= BUF_UnorderedAccess | BUF_ByteAddressBuffer | BUF_ShaderResource;
		else
			usage |= BUF_Dynamic;
		VertexBufferRHI = RHICreateVertexBuffer(m_CapacityInBytes, usage, emptyInformations);
		if (!HH_VERIFY(IsValidRef(VertexBufferRHI)))
			m_CapacityInBytes = 0;
	}
}

bool	FPopcornFXVertexBuffer::HardResize(hh_u32 sizeInBytes, bool forGPU)
{
	if (!HH_VERIFY(!Mapped()))
		return false;
	HH_ASSERT(m_CapacityInBytes == 0 || ForGPU() == forGPU);
	SetForGPU(forGPU);
	const hh_u32		capacity = PopcornFX::Mem::Align<Alignment>(sizeInBytes);
	m_CapacityInBytes = capacity;
	if (!IsInitialized())
		InitResource();
	else
		UpdateRHI();
	return m_CapacityInBytes == capacity;
}

void	*FPopcornFXVertexBuffer::RawMap(hh_u32 count, hh_u32 stride)
{
	HH_ASSERT((m_Flags & Flag_StrideMask) == stride);
	HH_ASSERT(count <= m_AllocatedCount);

	if (!HH_VERIFY(Mappable()))
		return null;
	if (!HH_VERIFY(!Mapped()))
		return null;

	const hh_u32		sizeInBytes = count * stride;
	const hh_u32		sizeToMap = PopcornFX::Mem::Align<Alignment>(sizeInBytes);
	if (!HH_VERIFY(sizeToMap > 0 && sizeToMap <= m_CapacityInBytes))
		return null;
	if (!HH_VERIFY(IsValidRef(VertexBufferRHI)))
		return null;
	void	*map;
	{
		HH_NAMEDSCOPEDPROFILE("FPopcornFXVertexBuffer::RawMap RHILockVertexBuffer");
		map = RHILockVertexBuffer(VertexBufferRHI, 0, sizeToMap, RLM_WriteOnly);
		//map = GDynamicRHI->RHILockVertexBuffer(VertexBufferRHI, 0, sizeToMap, RLM_WriteOnly);
	}
	HH_ASSERT(map != null);
	HH_ASSERT(PopcornFX::Mem::IsAligned<Alignment>(map));
	m_CurrentMap = map;
	return map;
}

void	FPopcornFXVertexBuffer::Unmap()
{
	if (Mappable() && Mapped())
	{
		RHIUnlockVertexBuffer(VertexBufferRHI);
		//GDynamicRHI->RHIUnlockVertexBuffer(VertexBufferRHI);
		m_CurrentMap = null;
	}
}

const FUnorderedAccessViewRHIRef	&FPopcornFXVertexBuffer::UAV()
{
#if (PK_HAS_GPU != 0)
	HH_ASSERT(ForGPU());
	if (!IsValidRef(m_UAV))
	{
		m_UAV = My_RHICreateUnorderedAccessView(VertexBufferRHI, PF_R32_UINT);
	}
	HH_ASSERT(IsValidRef(m_UAV));
#else
	HH_RELEASE_ASSERT_NOT_REACHED();
#endif
	return m_UAV;
}

const FShaderResourceViewRHIRef		&FPopcornFXVertexBuffer::SRV()
{
#if (PK_HAS_GPU != 0)
	HH_ASSERT(ForGPU());
	if (!IsValidRef(m_SRV))
	{
		m_SRV = My_RHICreateShaderResourceView(VertexBufferRHI, sizeof(hh_u32), PF_R32_UINT);
	}
	HH_ASSERT(IsValidRef(m_SRV));
#else
	HH_RELEASE_ASSERT_NOT_REACHED();
#endif
	return m_SRV;
}


//----------------------------------------------------------------------------
//
// FPopcornFXIndexBuffer
//
//----------------------------------------------------------------------------

void	FPopcornFXIndexBuffer::ReleaseRHI()
{
	HH_ASSERT(!Mapped());
	m_Capacity = 0;
	m_Flags &= ~Flag_Large;
	m_AllocatedCount = 0;
	FIndexBuffer::ReleaseRHI();
}

void	FPopcornFXIndexBuffer::InitRHI()
{
	m_UAV = null;
	m_SRV = null;
	HH_TODO("u16 indices for GPU");
	if (ForGPU() || m_Capacity > 0xFFFF)
		m_Flags |= Flag_Large;
	else
		m_Flags &= ~Flag_Large;
	if (m_Capacity > 0)
	{
		FRHIResourceCreateInfo	emptyInformations;
		uint32			usage = 0;
		if (ForGPU())
			usage |= BUF_UnorderedAccess | BUF_ByteAddressBuffer | BUF_ShaderResource;
		else
			usage |= BUF_Dynamic;
		IndexBufferRHI = RHICreateIndexBuffer(Stride(), m_Capacity * Stride(), usage, emptyInformations);
		if (!HH_VERIFY(IsValidRef(IndexBufferRHI)))
		{
			m_Capacity = 0;
			m_Flags &= ~Flag_Large;
		}
	}
}

bool	FPopcornFXIndexBuffer::HardResize(hh_u32 count, bool forGPU)
{
	if (!HH_VERIFY(Mappable()))
		return false;

	if (!HH_VERIFY(!Mapped()))
		return false;
	HH_ASSERT(m_Capacity == 0 || ForGPU() == forGPU);
#if (PK_HAS_GPU != 0)
#else
	HH_ASSERT(!forGPU);
#endif
	SetForGPU(forGPU);
	const hh_u32		capacity = PopcornFX::Mem::Align<Alignment>(count);
	m_Capacity = capacity;
	if (!IsInitialized())
		InitResource();
	else
		UpdateRHI();
	return m_Capacity == capacity;
}

void	*FPopcornFXIndexBuffer::RawMap(hh_u32 countToMap)
{
	HH_ASSERT(countToMap <= m_AllocatedCount);

	if (!HH_VERIFY(Mappable()))
		return null;

	if (!HH_VERIFY(!Mapped()))
		return null;
	const hh_u32		sizeToMap = PopcornFX::Mem::Align<Alignment>(countToMap * Stride());
	if (!HH_VERIFY(sizeToMap > 0 && sizeToMap <= m_Capacity * Stride()))
		return null;
	if (!HH_VERIFY(IsValidRef(IndexBufferRHI)))
		return null;
	void	*map;
	{
		HH_NAMEDSCOPEDPROFILE("FPopcornFXIndexBuffer::RawMap RHILockIndexBuffer");
		map = RHILockIndexBuffer(IndexBufferRHI, 0, sizeToMap, RLM_WriteOnly);
	}
	HH_ASSERT(map != null);
	HH_ASSERT(PopcornFX::Mem::IsAligned<Alignment>(map));
	m_CurrentMap = map;
	return map;
}

void	FPopcornFXIndexBuffer::Unmap()
{
	if (Mappable() && Mapped())
	{
		RHIUnlockIndexBuffer(IndexBufferRHI);
		m_CurrentMap = null;
	}
}

const FUnorderedAccessViewRHIRef	&FPopcornFXIndexBuffer::UAV()
{
#if (PK_HAS_GPU != 0)
	HH_ASSERT(ForGPU());
	if (!IsValidRef(m_UAV))
	{
		m_UAV = My_RHICreateUnorderedAccessView(IndexBufferRHI, PF_R32_UINT);
	}
	HH_ASSERT(IsValidRef(m_UAV));
#else
	HH_RELEASE_ASSERT_NOT_REACHED();
#endif
	return m_UAV;
}

const FShaderResourceViewRHIRef		&FPopcornFXIndexBuffer::SRV()
{
#if (PK_HAS_GPU != 0)
	HH_ASSERT(ForGPU());
	if (!IsValidRef(m_SRV))
	{
		HH_RELEASE_ASSERT_NOT_IMPLEMENTED();
		//m_SRV = My_RHICreateShaderResourceView(IndexBufferRHI, PF_R32_UINT);
	}
	HH_ASSERT(IsValidRef(m_SRV));
#else
	HH_RELEASE_ASSERT_NOT_REACHED();
#endif
	return m_SRV;
}
