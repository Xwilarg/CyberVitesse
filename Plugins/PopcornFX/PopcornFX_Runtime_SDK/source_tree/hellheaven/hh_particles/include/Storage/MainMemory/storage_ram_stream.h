#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2016/04/25 09:10 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_STORAGE_RAM_STREAM_H__
#define	__PS_STORAGE_RAM_STREAM_H__

#include "hh_particles/include/ps_stream.h"	// for SParticleStreamDef & SParticleStreamFieldDef

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Particle stream field:
//	this hold a stream of a single particle's property
//
//----------------------------------------------------------------------------

struct			HH_PARTICLES_EXPORT SParticleStreamField : public CNonCopyable
{
private:
	SParticleStreamField(const SParticleStreamField &other);	// inheriting from CNonCopyable doesn't seem to be enough with visual studio 2010... wtf
	SParticleStreamField	&operator = (const SParticleStreamField &other);

public:
	hh_u8							*m_RawDataPtr;
	hh_u32							m_ParticleCount;
	SParticleStreamFieldDef			m_Definition;

	SParticleStreamField(const SParticleStreamFieldDef &def);
	SParticleStreamField(CStringId nameGUID, EBaseTypeID type, hh_u32 storageSize);
	~SParticleStreamField();

	void							DeepCopy(const SParticleStreamField &other);

	void							Clear() { m_ParticleCount = 0; }
	void							TrashFrom(hh_u32 firstToTrash) { HH_ASSERT(firstToTrash < m_ParticleCount); m_ParticleCount = firstToTrash; }

	hh_u32							StreamSizeInBytes() const { return m_ParticleCount * m_Definition.m_StorageSize; }

	template<typename _Type>
	TStridedMemoryView<const _Type>	StreamForReading() const;
	template<typename _Type>
	TStridedMemoryView<_Type>		StreamForWriting();

#if 0
	void							Swap(hh_u32 first, hh_u32 second)	// keep this here to give the compiler a chance to inline it if it judges it appropriate.
	{
		// FIXME: hum?
		switch (m_Definition.m_StorageSize)
		{
		case	0:
			break;
		case	1:
			HHSwap(*((hh_u8 *)m_RawDataPtr + first), *((hh_u8 *)m_RawDataPtr + second));
			break;
		case	2:
			HHSwap(*((hh_u16*)m_RawDataPtr + first), *((hh_u16*)m_RawDataPtr + second));
			break;
		case	4:
			HHSwap(*((hh_u32*)m_RawDataPtr + first), *((hh_u32*)m_RawDataPtr + second));
			break;
		case	8:
			HHSwap(*((hh_u64*)m_RawDataPtr + first), *((hh_u64*)m_RawDataPtr + second));
			break;
		case	16:
			{
				HH_ASSERT_ALIGNED16(m_RawDataPtr);
				const SIMD::Float4	xmm0 = SIMD::Float4::LoadAligned16(m_RawDataPtr, first  * 0x10);
				const SIMD::Float4	xmm1 = SIMD::Float4::LoadAligned16(m_RawDataPtr, second * 0x10);
				xmm0.StoreAligned16(m_RawDataPtr, second * 0x10);
				xmm1.StoreAligned16(m_RawDataPtr, first  * 0x10);
			}
			break;
		default:
			HH_ASSERT_NOT_REACHED();
			break;
		}
	}

	void							Trash(hh_u32 first, hh_u32 second)	// keep this here to give the compiler a chance to inline it if it judges it appropriate.
	{
		// FIXME: hum?
		switch (m_Definition.m_StorageSize)
		{
		case	0:
			break;
		case	1:
			*(((hh_u8 *)m_RawDataPtr) + first) = *(((hh_u8 *)m_RawDataPtr) + second);
			break;
		case	2:
			*(((hh_u16*)m_RawDataPtr) + first) = *(((hh_u16*)m_RawDataPtr) + second);
			break;
		case	4:
			*(((hh_u32*)m_RawDataPtr) + first) = *(((hh_u32*)m_RawDataPtr) + second);
			break;
		case	8:
			*(((hh_u64*)m_RawDataPtr) + first) = *(((hh_u64*)m_RawDataPtr) + second);
			break;
		case	16:
			HH_ASSERT_ALIGNED16(m_RawDataPtr);
			SIMD::Float4::LoadAligned16(m_RawDataPtr, second * 0x10).StoreAligned16(m_RawDataPtr, first * 0x10);
			break;
		default:
			HH_ASSERT_NOT_REACHED();
			break;
		}
	}
#endif

	void	SwapBatch(const TMemoryView<const hh_u32> &indices);
	void	TrashBatch(const TMemoryView<const hh_u32> &indices);

	void	Swap(SParticleStreamField &other)
	{
		HHSwap(m_RawDataPtr, other.m_RawDataPtr);
		HHSwap(m_ParticleCount, other.m_ParticleCount);
		HHSwap(m_Definition, other.m_Definition);
	}
};

//----------------------------------------------------------------------------

HH_FORCEINLINE void	HHSwap(SParticleStreamField &a, SParticleStreamField &b)
{
	a.Swap(b);
}

//----------------------------------------------------------------------------
// implementation

template<typename _Type>
TStridedMemoryView<const _Type>	SParticleStreamField::StreamForReading() const
{
	HH_ONLY_IF_ASSERTS(	const EBaseTypeID	type = TBaseTypeID<_Type>::TypeID;
						HH_ASSERT(type == m_Definition.m_Type));

	HH_ASSERT(Mem::IsAligned<0x10>(m_RawDataPtr));
	if (m_RawDataPtr != null && (sizeof(_Type) <= m_Definition.m_StorageSize || m_Definition.m_StorageSize == 0) && m_ParticleCount != 0)
		return TStridedMemoryView<const _Type>((const _Type*)m_RawDataPtr, m_ParticleCount, m_Definition.m_StorageSize);
	return TStridedMemoryView<const _Type>();
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedMemoryView<_Type>		SParticleStreamField::StreamForWriting()
{
	HH_ONLY_IF_ASSERTS(	const EBaseTypeID	type = TBaseTypeID<_Type>::TypeID;
						HH_ASSERT(type == m_Definition.m_Type));

	HH_ASSERT(Mem::IsAligned<0x10>(m_RawDataPtr));
	if (m_RawDataPtr != null && (sizeof(_Type) <= m_Definition.m_StorageSize || m_Definition.m_StorageSize == 0) && m_ParticleCount != 0)
		return TStridedMemoryView<_Type>((_Type*)m_RawDataPtr, m_ParticleCount, m_Definition.m_StorageSize);
	return TStridedMemoryView<_Type>();
}

//----------------------------------------------------------------------------
//
//	Particle page
//
//----------------------------------------------------------------------------
//	This will have to be manipulated by a single thread at a time

class	CParticleStream_MainMemory;

class	HH_PARTICLES_EXPORT CParticlePage : public CRefCountedObject//CNonCopyable
{
private:
	CParticlePage(const CParticlePage &other);	// inheriting from CNonCopyable doesn't seem to be enough with visual studio 2010... wtf
	CParticlePage	&operator = (const CParticlePage &other);

	friend class	CParticleStream_MainMemory;

	hh_u32									m_ParticleCount;
	hh_u32									m_Capacity;
	PParticleRawPage_MainMemory				m_RawPage;
	CParticleStream_MainMemory				*m_ParentStream;

	// if you touch datas, check ReadyToWrite(). If needed, use PrepareForWriting()
	TArray<SParticleStreamField>			m_Streams_Unsafe;

	CFloat4									m_BBoxMin;
	CFloat4									m_BBoxMax;

public:
	CParticlePage(CParticleStream_MainMemory *parent, hh_u32 capacity);
	~CParticlePage();

	void									ReassignParentStream(CParticleStream_MainMemory *parent);
	CParticleStream_MainMemory				*ParentStream() const { return m_ParentStream; }

	void									DeepCopy(const CParticlePage &other);	// NO PARENT UPDATE // update pcount

	bool									Empty() const { return m_ParticleCount == 0; }
	bool									Full() const { return m_ParticleCount == m_Capacity; }
	hh_u32									ParticleCount() const { return m_ParticleCount; }
	hh_u32									Capacity() const { return m_Capacity; }

	const SParticleStreamDef				*GetStreamDef() const;
	TMemoryView<const SParticleStreamField>	Streams() const { return m_Streams_Unsafe; }
	TMemoryView<SParticleStreamField>		StreamsForWriting() { HH_RELEASE_ASSERT(ReadyToWrite()); return m_Streams_Unsafe; }

	CGuid									StreamId(CStringId nameGUID) const;

	hh_u32									StreamsCount() const { return m_Streams_Unsafe.Count(); }
	const SParticleStreamField				&Stream(CGuid streamId) const { return RawStream(streamId); }
	const SParticleStreamField				&RawStream(CGuid streamId) const { HH_ASSERT(streamId.Valid()); return m_Streams_Unsafe[streamId]; }
	SParticleStreamField					&RawStreamForWriting(CGuid streamId) { HH_ASSERT(streamId.Valid()); HH_RELEASE_ASSERT(ReadyToWrite()); return m_Streams_Unsafe[streamId]; }

	void									SetBounds(const CAABB &box);
	void									AddBounds(const CAABB &box);
	void									AddBounds(const CParticlePage &other);
	CAABB									Bounds() const { return CAABB::FromMinMaxUnchecked(m_BBoxMin.xyz(), m_BBoxMax.xyz()); }
	const CFloat3							&BoundsMin() const { return m_BBoxMin.xyz(); }
	const CFloat3							&BoundsMax() const { return m_BBoxMax.xyz(); }
	CFloat4									*BoundsMinPtr() { return &m_BBoxMin; }
	CFloat4									*BoundsMaxPtr() { return &m_BBoxMax; }

	void									Clear();	// update pcount
	CGuid									AllocateParticles(hh_u32 count);	// NO PARENT UPDATE // update pcount	// 'count' MUST FIT in the available particle slots
	void									TrashFrom(hh_u32 firstToTrash);		// NO PARENT UPDATE // update pcount

	void									Swap(hh_u32 first, hh_u32 second);
	void									Trash(hh_u32 first, hh_u32 second);

	void									SwapBatch(const TMemoryView<const hh_u32> &indices);
	void									TrashBatch(const TMemoryView<const hh_u32> &indices);

	bool									ReadyToWrite() const;
	bool									PrepareForWriting();

	template<typename _Type>
	TStridedMemoryView<const _Type>			StreamForReading(CGuid streamIndex) const;
	template<typename _Type>
	TStridedMemoryView<_Type>				StreamForWriting(CGuid streamIndex);

	hh_ureg									StreamOffsetInPage(hh_u32 streamIndex) const;
	void				 					GetRawPage(PCParticleRawPage_MainMemory &outRawPage) const;

};
HH_DECLARE_REFPTRCLASS(ParticlePage);

//----------------------------------------------------------------------------
// implementation

template<typename _Type>
TStridedMemoryView<const _Type>		CParticlePage::StreamForReading(CGuid streamIndex) const
{
	if (streamIndex.Valid())
		return RawStream(streamIndex).StreamForReading<_Type>();
	return TStridedMemoryView<const _Type>();
}

template<typename _Type>
TStridedMemoryView<_Type>			CParticlePage::StreamForWriting(CGuid streamIndex)
{
	if (streamIndex.Valid())
		return RawStreamForWriting(streamIndex).StreamForWriting<_Type>();
	return TStridedMemoryView<_Type>();
}

//----------------------------------------------------------------------------
//
//	Ranged view into a particle page
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticlePageView
{
private:
	PParticlePage	m_Page;
	hh_u32			m_First;
	hh_u32			m_Count;

public:
	CParticlePageView() : m_Page(null), m_First(0), m_Count(0) {}

	// FIXME: temp gore const casts
	CParticlePageView(const CParticlePage &page)
	:	m_Page(const_cast<CParticlePage*>(&page))
	,	m_First(0)
	,	m_Count(page.ParticleCount())
	{
	}
	CParticlePageView(const CParticlePage &page, hh_u32 first, hh_u32 count)
	:	m_Page(const_cast<CParticlePage*>(&page))
	,	m_First(first)
	,	m_Count(count)
	{
		HH_ASSERT(first + count <= page.ParticleCount());
	}

	CParticlePageView(const CParticlePageView &other)
	:	m_Page(other.m_Page)
	,	m_First(other.m_First)
	,	m_Count(other.m_Count)
	{
	}

	CParticlePageView	&operator = (const CParticlePageView &other)
	{
		m_Page = other.m_Page;
		m_First = other.m_First;
		m_Count = other.m_Count;
		return *this;
	}

	TMemoryView<const SParticleStreamField>	Streams() const { return m_Page->Streams(); }
	TMemoryView<SParticleStreamField>		StreamsForWriting() { return m_Page->StreamsForWriting(); }

	const PCParticlePage					OriginalPage() const { return m_Page; }
	const PParticlePage						&OriginalPage() { return m_Page; }
	const SParticleStreamDef				*GetStreamDef() const { return OriginalPage()->GetStreamDef(); }

	bool									IsFullPageView() const { return m_Page != null && m_First == 0 && m_Count == m_Page->ParticleCount(); }

	void									Clear() { m_Page = null; m_First = 0; m_Count = 0; }
	bool									Empty() const { return m_Count == 0; }
	hh_u32									FirstParticle() const { return m_First; }
	hh_u32									ParticleCount() const { return m_Count; }

	CGuid									StreamId(CStringId nameGUID) const { return m_Page->StreamId(nameGUID); }
	const SParticleStreamField				&Stream(CGuid streamId) const { return RawStream(streamId); }
	const SParticleStreamField				&RawStream(CGuid streamId) const { return m_Page->RawStream(streamId); }
	SParticleStreamField					&RawStreamForWriting(CGuid streamId) { return m_Page->RawStreamForWriting(streamId); }

	template<typename _Type>
	TStridedMemoryView<const _Type>			StreamForReading(CGuid streamIndex) const;
	template<typename _Type>
	TStridedMemoryView<_Type>				StreamForWriting(CGuid streamIndex);

	const hh_u8								*RawStreamBegin(CGuid streamId) const
	{
		if (streamId.Valid())
		{
			const SParticleStreamField	&field = m_Page->RawStream(streamId);
			if (field.m_RawDataPtr != null)
			{
				return field.m_RawDataPtr + m_First * field.m_Definition.m_StorageSize;
			}
		}
		return null;
	}

	hh_u8									*RawStreamBeginForWriting(CGuid streamId)
	{
		if (streamId.Valid())
		{
			const SParticleStreamField	&field = m_Page->RawStreamForWriting(streamId);
			if (field.m_RawDataPtr != null)
			{
				return field.m_RawDataPtr + m_First * field.m_Definition.m_StorageSize;
			}
		}
		return null;
	}

};

//----------------------------------------------------------------------------
// implementation

template<typename _Type>
TStridedMemoryView<const _Type>	CParticlePageView::StreamForReading(CGuid streamIndex) const
{
	TStridedMemoryView<const _Type>	view;
	if (m_Page != null && streamIndex.Valid() && m_Count != 0)
	{
		view = m_Page->StreamForReading<_Type>(streamIndex);
		if (!view.Empty())
			view = TStridedMemoryView<const _Type>(&view[m_First], m_Count, view.Stride());
	}
	return view;
}

template<typename _Type>
TStridedMemoryView<_Type>		CParticlePageView::StreamForWriting(CGuid streamIndex)
{
	TStridedMemoryView<_Type>	view;
	if (m_Page != null && streamIndex.Valid() && m_Count != 0)
	{
		view = m_Page->StreamForWriting<_Type>(streamIndex);
		if (!view.Empty())
			view = TStridedMemoryView<_Type>(&view[m_First], m_Count, view.Stride());
	}
	return view;
}

//----------------------------------------------------------------------------
//
//	Particle stream
//
//----------------------------------------------------------------------------

struct	SPageData
{
	hh_u32			m_Capacity;
	hh_u32			m_Count;

	hh_u32			*m_PagesNotFull;
	float			*m_PagesCenterX;
	float			*m_PagesCenterY;
	float			*m_PagesCenterZ;
	hh_u32			*m_PageAvailCount;
	CFloat4			*m_PageBounds;

	TArray<hh_u32>	m_ParticlePageLookup;

	SPageData();
	~SPageData();

	void	Clean();
	bool	ResizeParticles(hh_u32 particleCount);
	bool	ResizePages(hh_u32 pageCount, bool keepExistingData);
};

//----------------------------------------------------------------------------

struct	SPageHashGrid
{
	struct SCelldata
	{
		TArray<hh_u32>	m_PagesId;
	};

	enum
	{
		kMaxCellCountByDimension = 15
	};

	float				m_CellSize;		// same for each dimension (x,y,z)
	hh_u32				m_CellCount;	// same for each dimension (x,y,z)

	CFloat4				m_BBoxMin;

	TArray<SCelldata>	m_Grid;

	SPageHashGrid();
	~SPageHashGrid();

	void	Clean();
	void	Clear();
	void	ResizeGrid(hh_u32 newSize, float sizeCellInGrid, const CFloat3 &minPoint, bool keepData);
	hh_u32	IndexFromCoord(const CFloat3 &coord);
	hh_u32	IndexFromCoord(const CFloat4 &coord);
	hh_u32	IndexFromIndices(const CFloat3 &indices);
	hh_u32	IndexFromIndices(const CFloat4 &indices);
	void	IndicesFromCoord(const CFloat3 &coord, CFloat3 &indices);
	void	IndicesFromCoord(const CFloat4 &coord, CFloat4 &indices);

	void	InsertPageInGrid(const CFloat3 &minBox, const CFloat3 &maxBox, hh_u32 pageId);
};

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleStream_MainMemory : public CParticleStreamAbstract
{
private:
	CParticleStream_MainMemory(const CParticleStream_MainMemory &other);	// inheriting from CNonCopyable doesn't seem to be enough with visual studio 2010... wtf
	CParticleStream_MainMemory	&operator = (const CParticleStream_MainMemory &other);

	static bool		Startup();
	static void		Shutdown();

	hh_u32								m_PageSize;
	hh_u32								m_PageSizeInBytes;
	const SParticleStreamDef			*m_StreamDef;
	//TArray<SParticleStreamFieldDef>		m_StreamDefs;
	TArray<PParticlePage>				m_Pages;
	Threads::CCriticalSection			m_PagesProtectCS;

	SPageData							m_PageData;
	SPageHashGrid						m_PageHashGrid;

	TAtomic<hh_u32>						m_ParticleCount;

	HH_ONLY_IF_ASSERTS(
		mutable hh_u32					m_PagesLocked;
		mutable CGuid					m_ThreadOwningLock;
	)

protected:
	static bool							_CopySubRangeToPage(CParticlePage &dst, const CParticlePage &src, hh_u32 startIdx, hh_u32 transferCount);
	static hh_u32						_TransferPageToPage(CParticlePage &dst, const CParticlePage &src, hh_u32 srcStart);
	hh_u32								_TransferPageToPagesLocalized(const CParticlePage &src);
	hh_u32								_TransferPageToPagesLocalizedWithHashGrid(const CParticlePage &src);
	static hh_u32						_ComputePageSizeInBytes(hh_u32 pageSize, const SParticleStreamDef *streamDef);
	PParticlePage						_AllocNewPage_NoLock(bool addToPageList = true);
	hh_u32								_Merge_NoLock(const CParticlePage &otherPage, bool localizePages);	// update pcount
	hh_u32								_RecomputeParticleCount() const;	//[LOCKPAGES]
	bool								_ClampPageSizeToAllocatorCapsIFN();

#ifndef	HH_RETAIL
	void								_DEBUG_CheckParticleCountConsistency() const;	//[LOCKPAGES]
#endif

public:
	enum
	{
		kStreamFieldSizeAlignment = 0x10U,
		kVirtualStreamFieldSizeInBytes = 64U, // We shouldn't have bigger types anyway... it could actually be sizeof(float4) = 16 rather than 64
	};

public:
	CParticleStream_MainMemory(hh_u32 pageSize = 0);
	~CParticleStream_MainMemory();

	virtual const SParticleStreamDef	*GetStreamDef() const override { return m_StreamDef; }
//	virtual void						Clear() override;
	virtual hh_u32						ParticleCount() const override { return m_ParticleCount; }
	virtual void						Lock() const override { LockPages(); }
	virtual void						Unlock() const override { UnlockPages(); }

	void								DeepCopy(const CParticleStream_MainMemory &other);			//[LOCKPAGES]
	void								BindStreamDef(const SParticleStreamDef *def);	//[LOCKPAGES]
//	const SParticleStreamDef			*GetStreamDef() const { return m_StreamDef; }

	HH_FORCEINLINE bool					Empty() const { return m_ParticleCount == 0; }
	void								Clear();	//[LOCKPAGES]
	void								Cleanup();	//[LOCKPAGES]
	CGuid								StreamId(CStringId nameGUID) const;
	hh_u32								StreamsCount() const { return (m_StreamDef != null) ? m_StreamDef->m_StreamDef.Count() : 0; /*return m_StreamDefs.Count();*/ }
	bool								Merge(const CParticlePage &otherPage);	//[LOCKPAGES]
	bool								PullAndMerge(CParticleStream_MainMemory &other, bool localizePages, float ratioThresholdForWholePageTransfer = 0.75f);	//[LOCKPAGES]

	const SParticleStreamFieldDef		&StreamDef(CGuid streamIndex) const { HH_ASSERT(streamIndex.Valid() && m_StreamDef != null); return m_StreamDef->m_StreamDef[streamIndex]; /*m_StreamDefs[streamIndex];*/ }

	void								SetPageSize(hh_u32 size);	//[LOCKPAGES]
	hh_u32								PageSize() const { return m_PageSize; }
	hh_u32								PageSizeInBytes() const { return m_PageSizeInBytes; }


	void								LockPages() const;
	void								UnlockPages() const;
	HH_ONLY_IF_ASSERTS(bool				LockedByCurrentThread() const);

	hh_u32								PageCount() const { /*HH_ASSERT(m_PagesLocked);*/ return m_Pages.Count(); }
	const PParticlePage					&Page(CGuid index) const { HH_ASSERT(m_PagesLocked != 0); HH_ASSERT(index.Valid()); HH_ASSERT(m_Pages[index]->ParentStream() == this); return m_Pages[index]; }
	// FIXME
	const PParticlePage					&_UnsafePage(CGuid index) const { HH_ASSERT(index.Valid()); return m_Pages[index]; }

	CParticlePageView					AllocateParticlesIFP(hh_u32 wishedCount, hh_u32 minCountHint = 0);	//[LOCKPAGES] on failure, returns an empty page. the function may allocate less than 'wishedCount' contiguous particles, and may need to be called multiple times
	void								RegisterDeadCount(hh_u32 deadCount);	// update pcount

	void								DefragmentPages(float defragThreshold = 0.35f, hh_u32 minFreeCount = 16);	//[LOCKPAGES]
	void								DefragmentPages_NoLock(float defragThreshold = 0.35f, hh_u32 minFreeCount = 16);
	void								ReturnAllEmptyPagesToFreePool() { GarbageCollectOldPages(); }
	void								ReturnAllEmptyPagesToFreePool_NoLock() { GarbageCollectOldPages_NoLock(); }
	void								GarbageCollectOldPages();	//[LOCKPAGES]
	void								GarbageCollectOldPages_NoLock();

	void								Swap(CParticleStream_MainMemory &other);	//[LOCKPAGES]

	void								LockPagesToRender() const;
	void								UnlockPagesToRender() const;
};

//----------------------------------------------------------------------------

HH_FORCEINLINE void	HHSwap(CParticleStream_MainMemory &a, CParticleStream_MainMemory &b)
{
	a.Swap(b);
}

//----------------------------------------------------------------------------

typedef CParticleStream_MainMemory	CParticleStream;	// To avoid breaking everyone's code...

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_STORAGE_STREAM_RAM_H__

