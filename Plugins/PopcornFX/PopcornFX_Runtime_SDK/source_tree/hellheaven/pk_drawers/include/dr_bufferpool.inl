#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/01/14 10:22 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __RD_BUFFERPOOL_H__
#	error rd_bufferpool.inl can only be included by rd_bufferpool.h
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
// Pooled Buffer Ptr-like
//
//----------------------------------------------------------------------------

template <typename _B, typename _BP>
TPooledBuffer<_B, _BP>::TPooledBuffer()
:	m_BufferData(null)
{
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
TPooledBuffer<_B, _BP>::TPooledBuffer(CPooledBufferData *wrap)
:	m_BufferData(wrap)
{
	if (m_BufferData != null)
		m_BufferData->RefCount_Inc();
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
TPooledBuffer<_B, _BP>::TPooledBuffer(const CSelf &other)
:	m_BufferData(other.m_BufferData)
{
	if (m_BufferData != null)
		m_BufferData->RefCount_Inc();
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
TPooledBuffer<_B, _BP>::~TPooledBuffer()
{
	if (m_BufferData != null)
		m_BufferData->RefCount_Dec();
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
const typename TPooledBuffer<_B, _BP>::CSelf		&TPooledBuffer<_B, _BP>::operator = (const CSelf &other)
{
	if (m_BufferData != null)
		m_BufferData->RefCount_Dec();
	m_BufferData = other.m_BufferData;
	if (m_BufferData != null)
		m_BufferData->RefCount_Inc();
	return *this;
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TPooledBuffer<_B, _BP>::Reset(CPooledBufferData *wrap)
{
	if (m_BufferData != null)
		m_BufferData->RefCount_Dec();
	m_BufferData = wrap;
	if (m_BufferData != null)
		m_BufferData->RefCount_Inc();
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TPooledBuffer<_B, _BP>::Clear()
{
	if (m_BufferData != null)
	{
		m_BufferData->RefCount_Dec();
		m_BufferData = null;
	}
}

//----------------------------------------------------------------------------
//
// Internal buffer data
//
//----------------------------------------------------------------------------

template <typename _Buffer, typename _BaseBufferPoolPolicy>
class	TPooledBufferData
{
public:
	typedef TPooledBufferData<_Buffer, _BaseBufferPoolPolicy>		CSelf;
	typedef TBufferPool<_Buffer, _BaseBufferPoolPolicy>			CBufferPool;
	typedef _Buffer							CBuffer;

	TPooledBufferData(CBufferPool *pool)
	: m_RefCount(0), m_Buffer(), m_Pool(pool), m_ExactSize(0)
	{ }

	CBuffer				*Buffer() { return &m_Buffer; }
	const CBuffer		*Buffer() const { return &m_Buffer; }
	CBufferPool			*Pool() const { return m_Pool; }
	hh_u32				ExactSize() const { return m_ExactSize; }
	bool				Released() const { return m_ExactSize == 0; }

	hh_u32				RefCount() { return m_RefCount; }
	void				RefCount_Inc() { ++m_RefCount; }
	void				RefCount_Dec()
	{
		const hh_u32	newRefCount = --m_RefCount;
		if (newRefCount == 0)
			m_GCTickUnusedDate = m_Pool->GCTickNow();
	}
	void				RefCount_Set(hh_u32 v) { m_RefCount = v; }

public:
	hh_u32				m_RefCount;
	hh_u32				m_GCTickUnusedDate;
	CBuffer				m_Buffer;
	CBufferPool			*m_Pool;
	hh_u32				m_ExactSize;
};

//----------------------------------------------------------------------------
//
// Internal level pool
//
//----------------------------------------------------------------------------

template <typename _Buffer, typename _BaseBufferPoolPolicy>
class	TBufferPool_CLevelPool
{
public:
	typedef TBufferPool<_Buffer, _BaseBufferPoolPolicy>				CSelf;
	typedef	_Buffer													CBuffer;
	typedef TPooledBuffer<_Buffer, _BaseBufferPoolPolicy>			CPooledBuffer;
	typedef TPooledBufferData<_Buffer, _BaseBufferPoolPolicy>		CPooledBufferData;
	typedef	_BaseBufferPoolPolicy									CBaseBufferPoolPolicy;

	TBufferPool_CLevelPool()
	{
	}

	~TBufferPool_CLevelPool()
	{
		HH_ASSERT(m_Pool.Empty());
	}

	hh_u32					Count() const { return m_Pool.Count(); }
	CPooledBufferData		*operator[](hh_u32 index) { return m_Pool[index]; }

	CPooledBufferData		*FindBuffer(CSelf *self, hh_u32 exactSize)
	{
		CPooledBufferData	*useBuffer = null;
		for (hh_u32 buffi = 0; buffi < m_Pool.Count(); ++buffi)
		{
			CPooledBufferData	*buffer = m_Pool[buffi];
			HH_ASSERT(buffer != null);
			if (buffer->RefCount() == 0)
			{
				HH_ASSERT(!buffer->Released());
				buffer->RefCount_Set(CSelf::_kRefCountOffset);
				useBuffer = buffer;
				HH_ASSERT(useBuffer->ExactSize() > 0);
				useBuffer->m_ExactSize = exactSize;
				HH_ASSERT(useBuffer->m_Pool == self);
				break;
			}
		}
		return useBuffer;
	}

	CPooledBufferData		*NewBuffer(CSelf *self, hh_u32 exactSize)
	{
		CPooledBufferData	*buffer = HH_NEW(CPooledBufferData(self));
		if (!HH_VERIFY(buffer != null))
			return null;
		CGuid		buffIndex = m_Pool.PushBack();
		if (!HH_VERIFY(buffIndex.Valid()))
		{
			HH_DELETE(buffer);
			return null;
		}
		buffer->RefCount_Set(CSelf::_kRefCountOffset);
		if (!HH_VERIFY(self->BufferAllocate(*(buffer->Buffer()), m_Size)))
		{
			HH_DELETE(buffer);
			m_Pool.PopBackAndDiscard();
			return null;
		}

#if (PK_ENABLE_BUFFERPOOL_STATS == 1)
		m_AllocBytes += m_Size;
		++m_AllocCount;
#endif

		//CLog::Log(HH_INFO, "A %2d", m_Level);
		m_Pool[buffIndex] = buffer;
		buffer->m_ExactSize = exactSize;
		return buffer;
	}

	void		_ReleaseBuffer(CSelf *self, CPooledBufferData *buffer)
	{
		HH_ASSERT(buffer != null);
		HH_ASSERT(buffer->RefCount() <= CSelf::_kRefCountOffset);
		self->BufferRelease(*(buffer->Buffer()));
		buffer->RefCount_Set(0);
#if (PK_ENABLE_BUFFERPOOL_STATS == 1)
		m_ReleaseBytes += m_Size;
		++m_ReleaseCount;
#endif
		buffer->m_ExactSize = 0;
	}

	void		FrameTick(CSelf *self)
	{
		for (hh_u32 buffi = 0; buffi < m_Pool.Count(); ++buffi)
		{
			CPooledBufferData	*buffer = m_Pool[buffi];
			HH_ASSERT(buffer != null);
			const hh_u32		refCount = buffer->RefCount();
			if (refCount > 0 && refCount <= CSelf::_kRefCountOffset)
				buffer->RefCount_Dec();
		}
	}

	void		GarbageCollect(CSelf *self)
	{
		const hh_u32		gcTickNow = self->m_GCTickNow;
		for (hh_u32 i = 0; i < m_Pool.Count(); ++i)
		{
			CPooledBufferData	*buffer = m_Pool[i];
			HH_ASSERT(buffer != null);
			if (buffer->RefCount() > 0 ||
				(gcTickNow - buffer->m_GCTickUnusedDate) < CSelf::_kGCTicksBeforeRelease)
				continue;
			if (HH_VERIFY(!buffer->Released()))
				_ReleaseBuffer(self, buffer);
			HH_DELETE(buffer);
			m_Pool.Remove_AndKeepOrder(i);
			--i;
		}
	}

	void		Clean(CSelf *self)
	{
		for (hh_u32 buffi = 0; buffi < m_Pool.Count(); ++buffi)
		{
			CPooledBufferData	*buffer = m_Pool[buffi];
			HH_ASSERT(buffer != null);
			HH_ASSERT(buffer->RefCount() <= CSelf::_kRefCountOffset);
			_ReleaseBuffer(self, buffer);
			HH_DELETE(buffer);
		}
		m_Pool.Clear();
	}

	void		AccumStats(SBufferPool_Stats *stats)
	{
#if (PK_ENABLE_BUFFERPOOL_STATS == 1)
		stats->m_TotalBytes += m_Size * m_Pool.Count();
		stats->m_BufferCount += m_Pool.Count();

		stats->m_AllocCount += m_AllocCount;
		stats->m_AllocatedBytes += m_AllocBytes;
		stats->m_ReleaseCount += m_ReleaseCount;
		stats->m_ReleasedBytes += m_ReleaseBytes;
		m_AllocCount = 0;
		m_AllocBytes = 0;
		m_ReleaseCount = 0;
		m_ReleaseBytes = 0;

		for (hh_u32 buffi = 0; buffi < m_Pool.Count(); ++buffi)
		{
			CPooledBufferData	*buffer = m_Pool[buffi];
			const hh_u32		refCount = buffer->RefCount();
			if (refCount > 0)
			{
				stats->m_SoftUsedBytes += m_Size;
				stats->m_SoftExactUsedBytes += buffer->ExactSize();
				++stats->m_SoftBufferCount;
			}
			if (refCount > CSelf::_kRefCountOffset)
			{
				stats->m_HardUsedBytes += m_Size;
				stats->m_HardExactUsedBytes += buffer->ExactSize();
				++stats->m_HardBufferCount;
			}
		}
#endif
	}

	hh_u32						m_Size;
	hh_u32						m_Level;
private:
	TArray<CPooledBufferData*>	m_Pool;

#if (PK_ENABLE_BUFFERPOOL_STATS == 1)
public:
	hh_u32						m_AllocCount;
	hh_u32						m_AllocBytes;
	hh_u32						m_ReleaseCount;
	hh_u32						m_ReleaseBytes;
#endif
};

//----------------------------------------------------------------------------
//
// TBufferPool
//
//----------------------------------------------------------------------------

void		_LogDebugBufferPoolAllocatorLevels(const char *name, hh_u32 levelCount, const hh_u32 *levels);

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
TBufferPool<_B, _BP>::TBufferPool()
:	m_GCTickNow(0)
{
	for (hh_u32 pooli = 0; pooli < m_LevelPools.Count(); ++pooli)
	{
		m_LevelPools[pooli].m_Level = pooli;
		m_LevelPools[pooli].m_Size = LevelToSize(pooli);
	}
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
TBufferPool<_B, _BP>::~TBufferPool()
{
	Clean();
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
bool	TBufferPool<_B, _BP>::Allocate(CPooledBuffer &outPBuffer, hh_u32 sizeInBytes)
{
	HH_ASSERT(sizeInBytes > 0);
	hh_u32		level = SizeToLevel(sizeInBytes);
	HH_ASSERT(LevelToSize(level) >= sizeInBytes);
	if (!HH_VERIFY(level >= 0))
		return false;
	CPooledBufferData	*wrap = _Allocate(level, sizeInBytes);
	if (wrap == null)
		return false;
	outPBuffer.Reset(wrap);
	return true;
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TBufferPool<_B, _BP>::AccumStats(SBufferPool_Stats *stats)
{
#if (PK_ENABLE_BUFFERPOOL_STATS == 1)
	++stats->m_AccumCount;
	for (hh_u32 pooli = 0; pooli < m_LevelPools.Count(); ++pooli)
	{
		CLevelPool		&pool = m_LevelPools[pooli];
		pool.AccumStats(stats);
	}
#endif
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TBufferPool<_B, _BP>::LogDebugBufferPoolAllocatorLevels(const char *name)
{
	hh_u32		levels[_kLevelCount * 2];
	for (hh_u32 i = 0; i < _kLevelCount; ++i)
	{
		levels[i * 2 + 0] = LevelToSize(i);
		levels[i * 2 + 1] = LevelCountToLookahead(i);
	}
	_LogDebugBufferPoolAllocatorLevels(name, _kLevelCount, levels);
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TBufferPool<_B, _BP>::FrameTick()
{
	for (hh_u32 pooli = 0; pooli < m_LevelPools.Count(); ++pooli)
	{
		CLevelPool		&pool = m_LevelPools[pooli];
		pool.FrameTick(this);
	}
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TBufferPool<_B, _BP>::GarbageCollect()
{
	++m_GCTickNow;
	for (hh_u32 pooli = 0; pooli < m_LevelPools.Count(); ++pooli)
	{
		CLevelPool		&pool = m_LevelPools[pooli];
		pool.GarbageCollect(this);
	}
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
void	TBufferPool<_B, _BP>::Clean()
{
	for (hh_u32 pooli = 0; pooli < m_LevelPools.Count(); ++pooli)
	{
		CLevelPool	&pool = m_LevelPools[pooli];
		pool.Clean(this);
	}
}

//----------------------------------------------------------------------------

template <typename _B, typename _BP>
typename TBufferPool<_B, _BP>::CPooledBufferData		*TBufferPool<_B, _BP>::_Allocate(hh_u32 level, hh_u32 exactSize)
{
	if (!HH_VERIFY(level < _kLevelCount)) // exceed max buffer size
		return null;
	CPooledBufferData	*buffer = null;
	buffer = m_LevelPools[level].FindBuffer(this, exactSize);
	if (buffer != null)
		return buffer;
	const hh_u32	lookahead = LevelCountToLookahead(level);
	for (hh_u32 levelOff = 0; levelOff < lookahead && level + levelOff < _kLevelCount; ++levelOff)
	{
		buffer = m_LevelPools[level + levelOff].FindBuffer(this, exactSize);
		if (buffer != null)
			return buffer;
	}
	buffer = m_LevelPools[level].NewBuffer(this, exactSize);
	HH_ASSERT(buffer != null);
	return buffer;
}

//----------------------------------------------------------------------------
__PK_API_END
