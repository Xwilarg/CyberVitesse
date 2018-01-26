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

#include <hh_kernel/include/kr_containers_array.h>
#include <hh_kernel/include/kr_containers_static.h>
#include <hh_maths/include/hh_numeric_tools_int.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

#ifndef	PK_ENABLE_BUFFERPOOL_STATS
#	if 1 // !defined(HH_RETAIL)
#		define	PK_ENABLE_BUFFERPOOL_STATS		1
#	else
#		define	PK_ENABLE_BUFFERPOOL_STATS		0
#	endif
#endif

//----------------------------------------------------------------------------

struct	SBufferPoolAllocatorDefault
{
protected:
	// Number of FrameTick() calls to pass before re-using a buffer
	// (simulates buffers still in use by the GPU for "kFrameTicksBeforeReuse" frames, like "IntRef" on D3D)
	enum { kFrameTicksBeforeReuse = 2U };

	// Number of GarbageCollect() calls before releasing an unused buffer
	enum { kGCTicksBeforeRelease = 4U };

	// Number of pool levels (array of buffer of the same size)
	// Size of the buffers in each level defined by LevelToSize, SizeToLevel
	enum { kLevelCount = 20U };

	enum { _kBasePow = 9U };
	hh_u32		LevelToSize(hh_u32 level)
	{
		return 1 << (level + _kBasePow);
	}

	hh_u32		SizeToLevel(hh_u32 size)
	{
		return HHMax(IntegerTools::Log2(size), hh_u32(_kBasePow)) - _kBasePow + 1;
	}

	// Number of levels to look ahead if cannot find an unused buffer in the given level
	hh_u32		LevelCountToLookahead(hh_u32 level)
	{
		return (kLevelCount - level) / 4; // biggest level: 0 lookhead, smallest: kLevelCount/4 lookhead
	}
};

//----------------------------------------------------------------------------

template <typename _Buffer, typename _Allocator = SBufferPoolAllocatorDefault>
class	TBaseBufferPoolPolicy : public _Allocator
{
public:
	typedef	_Buffer		CBuffer;
	bool		BufferAllocate(CBuffer &buffer, hh_u32 sizeInBytes)
	{
		return buffer.Resize(sizeInBytes);
	}
	void		BufferRelease(CBuffer &buffer)
	{
		buffer.Release();
	}
};

//----------------------------------------------------------------------------

template <typename _Buffer, typename _BaseBufferPoolPolicy> class TBufferPool;
template <typename _Buffer, typename _BaseBufferPoolPolicy> class TPooledBufferData;
struct	SBufferPool_Stats;

//----------------------------------------------------------------------------
//
// Refcounted Pooled Buffer
//
//----------------------------------------------------------------------------

template <typename _Buffer, typename _BaseBufferPoolPolicy>
class	TPooledBuffer
{
public:
	typedef TBufferPool<_Buffer, _BaseBufferPoolPolicy>			CBufferPool;
	typedef TPooledBuffer<_Buffer, _BaseBufferPoolPolicy>		CSelf;
	typedef TPooledBufferData<_Buffer, _BaseBufferPoolPolicy>	CPooledBufferData;

	typedef _Buffer						CBuffer;

	TPooledBuffer();

	explicit TPooledBuffer(CPooledBufferData *wrap);
	TPooledBuffer(const CSelf &other);
	~TPooledBuffer();
	const CSelf	&operator = (const CSelf &other);

	void		Reset(CPooledBufferData *wrap);
	void		Release() { Clear(); }
	void		Clear();
	bool		Valid() const { return m_BufferData != null; }

	CBuffer		*Buffer() const { HH_ASSERT(Valid()); return m_BufferData->Buffer(); }

	CBuffer		&operator * () { return *Buffer(); }
	CBuffer		*operator -> () { return Buffer(); }
	CBuffer		&operator * () const { return *Buffer(); }
	CBuffer		*operator -> () const { return Buffer(); }

//	bool		operator != (std::nullptr_t nullp) const { HH_ASSERT(nullp == null); return m_BufferData != null; }
//	bool		operator == (std::nullptr_t nullp) const { HH_ASSERT(nullp == null); return m_BufferData == null; }
	bool		operator != (void *nullp) const { HH_ASSERT(nullp == null); return m_BufferData != null; }
	bool		operator == (void *nullp) const { HH_ASSERT(nullp == null); return m_BufferData == null; }
	bool		operator != (const CSelf &other) const { return m_BufferData != other.m_BufferData; }
	bool		operator == (const CSelf &other) const { return m_BufferData == other.m_BufferData; }

private:
	CPooledBufferData	*m_BufferData;
};

//----------------------------------------------------------------------------

template <typename _Buffer, typename _BaseBufferPoolPolicy>
class	TBufferPool_CLevelPool;

template <typename _Buffer, typename _BaseBufferPoolPolicy>
class	TBufferPool : public _BaseBufferPoolPolicy
{
protected:
	using _BaseBufferPoolPolicy::LevelToSize;
	using _BaseBufferPoolPolicy::SizeToLevel;
	using _BaseBufferPoolPolicy::LevelCountToLookahead;
	using _BaseBufferPoolPolicy::BufferAllocate;
	using _BaseBufferPoolPolicy::BufferRelease;
	enum { _kLevelCount = hh_u32(_BaseBufferPoolPolicy::kLevelCount) };
	enum { _kRefCountOffset = _BaseBufferPoolPolicy::kFrameTicksBeforeReuse };
	enum { _kGCTicksBeforeRelease = _BaseBufferPoolPolicy::kGCTicksBeforeRelease };

public:
	typedef TBufferPool<_Buffer, _BaseBufferPoolPolicy>				CSelf;
	typedef	_Buffer													CBuffer;
	typedef TPooledBuffer<_Buffer, _BaseBufferPoolPolicy>			CPooledBuffer;

protected:
	typedef TPooledBufferData<_Buffer, _BaseBufferPoolPolicy>		CPooledBufferData;
	typedef	_BaseBufferPoolPolicy									CBaseBufferPoolPolicy;
	typedef TBufferPool_CLevelPool<_Buffer, _BaseBufferPoolPolicy>	CLevelPool;
	friend class TBufferPool_CLevelPool<_Buffer, _BaseBufferPoolPolicy>;

public:
	TBufferPool();
	~TBufferPool();

	bool		Allocate(CPooledBuffer &outPBuffer, hh_u32 sizeInBytes);

	// to call each render
	void		FrameTick();
	// to call each N seconds
	void		GarbageCollect();

	void		Clean();

	void		AccumStats(SBufferPool_Stats *stats);
	void		LogDebugBufferPoolAllocatorLevels(const char *name);

	hh_u32		GCTickNow() const { return m_GCTickNow; }

private:
	CPooledBufferData		*_Allocate(hh_u32 level, hh_u32 exactSize);

private:
	hh_u32									m_GCTickNow;
	TStaticArray<CLevelPool, _kLevelCount>	m_LevelPools;
};

//----------------------------------------------------------------------------

struct	SBufferPool_Stats
{
	hh_u32		m_AccumCount;

	float		m_TotalBytes;

	// "Hard" used memory: still are referenced somewhere now
	float		m_HardBufferCount;
	float		m_HardUsedBytes;
	float		m_HardExactUsedBytes;

	// "Soft" used memory: "Hard" + was referenced "kFrameTicksBeforeReuse" ticks ago (emulation of GPU still using buffers)
	float		m_SoftBufferCount;
	float		m_SoftUsedBytes;				// (The full buffer is marked as used)
	float		m_SoftExactUsedBytes;			// (Exact deduced from Allocate())

	float		m_AllocCount;
	float		m_ReleaseCount;
	float		m_AllocatedBytes;
	float		m_ReleasedBytes;
	float		m_BufferCount;

	SBufferPool_Stats()
	{
		Mem::Clear(*this);
	}

	void		Clear()
	{
		Mem::Clear(*this);
	}

	void		LogPrettyStats(const char *name);
};

//----------------------------------------------------------------------------
__PK_API_END

#define __RD_BUFFERPOOL_H__
#include "dr_bufferpool.inl"
#undef __RD_BUFFERPOOL_H__
