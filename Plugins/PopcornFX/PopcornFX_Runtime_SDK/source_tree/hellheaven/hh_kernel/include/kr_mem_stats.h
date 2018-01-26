#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/04/02 7:36 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_MEM_STATS_H__
#define __KR_MEM_STATS_H__

#include <hh_kernel/include/hh_kernel_config.h>
#include <hh_kernel/include/kr_callstack.h>
#include <hh_kernel/include/kr_threads.h>	// for max thread count

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if	(KR_MEM_DEFAULT_ALLOCATOR_DEBUG != 0)

struct	SMemStatNodeAlloc
{
	void					*m_Ptr;
	SMemStatNodeAlloc		*m_Next;
	hh_u32					m_SizeInBytes;
#if	(HH_KERNEL_BUILD_CALLSTACKS != 0)
	TSmallCallstack<16>		m_Callstack;
#endif

	SMemStatNodeAlloc() : m_Ptr(null), m_Next(null), m_SizeInBytes(0) {}
};

// for backward compat
typedef SMemStatNodeAlloc		SMemProfileNodeAlloc;

//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT CMemStatNode
{
	const char				*m_Name;	// not allocated, static string
	hh_u32					m_Line;

	hh_u32					m_AllocCount;
	hh_u32					m_ReallocCount;
	hh_u32					m_FreeCount;

	hh_u64					m_AllocatedBytes;
	hh_u64					m_FreedBytes;
	hh_u64					m_RealAllocatedBytes;
	hh_u64					m_RealFreedBytes;

	hh_u32					m_ReallocBackwardCopies;
	hh_u32					m_ReallocForwardCopies;

	double					m_TimeInAllocs;

	SMemStatNodeAlloc		*m_AllocChain;

	hh_i64					Footprint() const { return hh_i64(m_AllocatedBytes) - hh_i64(m_FreedBytes); }
	hh_i64					RealFootprint() const { return hh_i64(m_RealAllocatedBytes) - hh_i64(m_RealFreedBytes); }
	hh_i64					Overhead() const { return RealFootprint() - Footprint(); }

	SMemStatNodeAlloc		*_CloneAllocChain() const;
	SMemStatNodeAlloc		*_FindAllocChain(void *ptr) const;
	SMemStatNodeAlloc		*_AddAllocChain(void *ptr);
	void					_RemoveAllocChainIFP(void *ptr);
	void					_DestroyAllocChain();

	CMemStatNode(const char *name, hh_u32 line);
	CMemStatNode(const CMemStatNode &other);
	~CMemStatNode();
	CMemStatNode	&operator = (const CMemStatNode &other);
};

// for backward compat
typedef CMemStatNode		CMemProfileNode;

#endif	// (KR_MEM_DEFAULT_ALLOCATOR_DEBUG != 0)

//----------------------------------------------------------------------------

// Memory Stats of the default allocator

class	HH_KERNEL_EXPORT CMemStats
{
public:
	static TAtomic<hh_u32>	m_AllocCount;
	static TAtomic<hh_u32>	m_ReallocCount;
	static TAtomic<hh_u32>	m_FreeCount;

	static TAtomic<hh_u32>	m_PrevFrameAllocCount;
	static TAtomic<hh_u32>	m_PrevFrameReallocCount;
	static TAtomic<hh_u32>	m_PrevFrameFreeCount;

	static hh_u64			m_AllocatedBytes;
	static hh_u64			m_FreedBytes;
	static hh_u64			m_RealAllocatedBytes;
	static hh_u64			m_RealFreedBytes;

	static TAtomic<hh_u32>	m_Footprint;			// is never reset
	static TAtomic<hh_u32>	m_RealFootprint;		// is never reset

	static TAtomic<hh_u32>	m_ReallocBackwardCopies;
	static TAtomic<hh_u32>	m_ReallocForwardCopies;

	static float			m_TimeInAllocs[CThreadManager::MaxThreadCount];		// reset on Reset() and FrameReset()

	static hh_u32			Footprint() { return m_Footprint; }
	static hh_u32			RealFootprint() { return m_RealFootprint; }
	static hh_u32			Overhead() { return RealFootprint() - Footprint(); }
	static float			TimeSpentInAllocs();
	static hh_u32			FrameAllocCount() { return m_AllocCount - m_PrevFrameAllocCount; }
	static hh_u32			FrameReallocCount() { return m_ReallocCount - m_PrevFrameReallocCount; }
	static hh_u32			FrameFreeCount() { return m_FreeCount - m_PrevFrameFreeCount; }

	static void				Reset();
	static void				FrameReset();

	static void				RegisterAllocTiming(float t);

#if	(KR_MEM_DEFAULT_ALLOCATOR_DEBUG != 0)
public:
	static void				CaptureStatNodes(TArray<CMemStatNode> &outputList, hh_u32 minimalThreshold = 0);
	// for backward compat
	static HH_INLINE void	CaptureProfileNodes(TArray<CMemProfileNode> &outputList, hh_u32 minimalThreshold = 0) { CaptureStatNodes(outputList, minimalThreshold); }

private:
	friend class CMemInternals;
	static bool				StartupStatNodes();
	static void				ShutdownStatNodes();
	static void				DumpAllocNodesLeft();
	static CGuid			_RegisterAlloc(	void *memptr, CGuid prevAllocTraceSlot, hh_u32 debugAllocType, hh_u32 alignment,
											const char *module, const char *file, int line, const char *function,
											hh_u32 backwrdCopy, hh_u32 forwardCopy,
											hh_u32 allocatedBytes, hh_u32 realAllocatedBytes,
											hh_u32 freedBytes, hh_u32 realFreedBytes);
#endif
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_MEM_STATS_H__
