#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2014/05/14 10:26 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PS_PARTICLES_STREAM_TO_RENDER_H__
#define __PS_PARTICLES_STREAM_TO_RENDER_H__

#include "hh_particles/include/ps_config.h"
#include "hh_particles/include/ps_stream.h"
#include "hh_particles/include/Storage/MainMemory/storage_ram_stream.h"
#include "hh_particles/include/Storage/MainMemory/storage_ram_allocator.h"	// CParticleRawPage_MainMemory
#if	(HH_PARTICLES_UPDATER_USE_D3D11 != 0)
#	include "hh_particles/include/Storage/D3D11/storage_d3d11_stream.h" // SParticleStreamBuffer_D3D11
#endif

#include <hh_kernel/include/kr_containers_array_chunked.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	To Render
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleStreamToRender // : public CRefCountedObject
{
public:
	CParticleStreamToRender() : m_BBox(CAABB::DEGENERATED) { }
	virtual ~CParticleStreamToRender() {}

	virtual CStringId	StorageClass() const = 0;

	virtual const SParticleStreamDef			*GetStreamDef() const = 0;

	virtual void		Clear() = 0;
	virtual hh_u32		ParticleCount() const = 0;	// watchout, might be expensive depending on the implementation

	virtual bool		Empty() const = 0;

	//virtual CGuid												StreamId(CStringId nameGUID) const = 0;
	//virtual const SParticleStreamFieldDef						&StreamDef(hh_u32 streamIndex) const = 0;
	//virtual TStridedMemoryView<const SParticleStreamFieldDef>	StreamDefs() const = 0;

	const CAABB			&BBox() const { return m_BBox; }

	void				_AddBBox(const CAABB &bbox) { m_BBox.Add(bbox); }

protected:
	CAABB				m_BBox;
};
//HH_DECLARE_REFPTRCLASS(ParticleStreamToRender);

//----------------------------------------------------------------------------
//
// Main Memory
//
//----------------------------------------------------------------------------

class CParticlePageToRender_MainMemory
{
	friend class CParticleStreamToRender_MainMemory;
public:
	CParticlePageToRender_MainMemory();
	CParticlePageToRender_MainMemory(const CParticlePageToRender_MainMemory &other);
	~CParticlePageToRender_MainMemory();

	void		Clear();
	bool		Empty() const { HH_ASSERT((m_Stream == null) == (m_ParticleCount == 0)); return m_Stream == null; }
	hh_u32		ParticleCount() const { return m_ParticleCount; }
	const CAABB	&BBox() const { return m_BBox; }

	HH_INLINE const SParticleStreamFieldDef			&StreamDef(hh_u32 streamIndex) const;
	HH_INLINE const hh_u8							*StreamRawPointer(hh_u32 streamIndex) const;

	template<typename _Type>
	HH_INLINE TStridedMemoryView<const _Type>		StreamForReading(CGuid streamIndex) const;

	template<typename _Type>
	HH_INLINE TStridedMemoryView<const _Type>		Stream(CGuid streamIndex) const { return StreamForReading<_Type>(streamIndex); }

private:
	PCParticleRawPage_MainMemory				m_RawPageToRender;
	hh_u32										m_ParticleCount;
	const CParticleStreamToRender_MainMemory	*m_Stream;
	CAABB										m_BBox;

	const CParticlePageToRender_MainMemory		&operator = (const CParticlePageToRender_MainMemory &other);
};

//----------------------------------------------------------------------------

typedef FastDelegate<bool (const CParticleStream_MainMemory &stream, const CParticlePage &page)>	CbStreamToRenderCullPage_MainMemory;

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleStreamToRender_MainMemory : public CParticleStreamToRender
{
public:
	CParticleStreamToRender_MainMemory();
	virtual ~CParticleStreamToRender_MainMemory();

	virtual const SParticleStreamDef	*GetStreamDef() const override { return &m_StreamDef; }
	virtual CStringId					StorageClass() const override;
	virtual void						Clear() override;
	virtual hh_u32						ParticleCount() const override { return m_TotalParticleCount; }
	virtual bool						Empty() const override
	{
		const bool		empty = (m_TotalParticleCount == 0);
		HH_ASSERT(m_StreamDef.Empty() == empty);
		HH_ASSERT(m_StreamOffsetsInPage.Empty() == empty);
		HH_ASSERT(m_Pages.Empty() == empty);
		return empty;
	}

	bool										Setup(const CParticleStream_MainMemory &stream, const CbStreamToRenderCullPage_MainMemory &cbCullPage = CbStreamToRenderCullPage_MainMemory());

	hh_u32										PageCount() const { return m_Pages.Count(); }

	const CParticlePageToRender_MainMemory		*Page(hh_u32 i) const;
	const TArray<SParticleStreamFieldDef>		&StreamDefs() const { return m_StreamDef.m_StreamDef; }
	const TArray<hh_ureg>						&StreamOffsetsInPage() const { return m_StreamOffsetsInPage; }

private:
	hh_u32										m_TotalParticleCount;
	SParticleStreamDef							m_StreamDef;
	TArray<hh_ureg>								m_StreamOffsetsInPage;
	TArray<CParticlePageToRender_MainMemory>	m_Pages;
};


//----------------------------------------------------------------------------
// implementation  PageToRender  MainMemory

const SParticleStreamFieldDef	&CParticlePageToRender_MainMemory::StreamDef(hh_u32 streamIndex) const
{
	HH_ASSERT(streamIndex < m_Stream->StreamDefs().Count());
	return m_Stream->StreamDefs()[streamIndex];
}

//----------------------------------------------------------------------------

const hh_u8		*CParticlePageToRender_MainMemory::StreamRawPointer(hh_u32 streamIndex) const
{
	HH_ASSERT(streamIndex < m_Stream->StreamDefs().Count());
	return m_RawPageToRender->RawData() + m_Stream->StreamOffsetsInPage()[streamIndex];
}

//----------------------------------------------------------------------------

template<typename _Type>
TStridedMemoryView<const _Type>		CParticlePageToRender_MainMemory::StreamForReading(CGuid streamIndex) const
{
	if (streamIndex.Valid())
	{
		HH_ASSERT(m_RawPageToRender != null && m_RawPageToRender->Valid());
		HH_ASSERT(hh_u32(streamIndex) < m_Stream->StreamDefs().Count());
		HH_ASSERT(hh_u32(streamIndex) < m_Stream->StreamOffsetsInPage().Count());
		const SParticleStreamFieldDef		&streamDef = m_Stream->StreamDefs()[streamIndex];
		const EBaseTypeID					expectedType = TBaseTypeID<_Type>::TypeID;
		if (!HH_VERIFY(expectedType == streamDef.m_Type))
			return TStridedMemoryView<const _Type>();
		const hh_u8			*rawPtr = m_RawPageToRender->RawData() + m_Stream->StreamOffsetsInPage()[streamIndex];
		HH_ASSERT(Mem::IsAligned<0x10>(rawPtr));
		HH_ASSERT(streamDef.m_StorageSize == 0 || streamDef.m_StorageSize >= sizeof(_Type));
		HH_ASSERT(rawPtr != null);
		return TStridedMemoryView<const _Type>((const _Type*)rawPtr, m_ParticleCount, streamDef.m_StorageSize);
	}
	return TStridedMemoryView<const _Type>();
}

//----------------------------------------------------------------------------
//
// D3D11
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_UPDATER_USE_D3D11 != 0)

class	CParticleStream_D3D11;

class	HH_PARTICLES_EXPORT CParticleStreamToRender_D3D11 : public CParticleStreamToRender
{
public:
	CParticleStreamToRender_D3D11();
	virtual ~CParticleStreamToRender_D3D11();

	virtual CStringId	StorageClass() const;

	virtual const SParticleStreamDef		*GetStreamDef() const { return &m_StreamDef; }
	virtual void		Clear() override;
	virtual hh_u32		ParticleCount() const override { return StreamSizeEst(); }
	virtual bool		Empty() const override { HH_ASSERT(m_StreamBuffers.Empty() == (StreamSizeEst() == 0)); return m_StreamBuffers.Empty(); }


	bool				Setup(const CParticleStream_D3D11 &stream);

	const TArray<SParticleStreamFieldDef>		&StreamDefs() const { return m_StreamDef.m_StreamDef; }

	hh_u32				StreamSizeEst() const { return m_StreamSizeEst; }
	const hh_u32		StreamsCount() const { return m_StreamBuffers.Count(); }
	TMemoryView<const SParticleStreamBuffer_D3D11>	Streams() const { return m_StreamBuffers; }

	HH_INLINE const SParticleStreamFieldDef		&StreamDef(hh_u32 streamIndex) const;
	const SParticleStreamBuffer_D3D11			*AbstractStream(CGuid streamIndex) const;

	template<typename _Type, hh_u32 _Stride>
	const SParticleStreamBuffer_D3D11			*Stream(CGuid streamIndex) const;

	const SParticleStreamBuffer_D3D11			*StreamSizeBuf() const { return &m_StreamSizeBuf; }

private:
	hh_u32										m_StreamSizeEst;
	SParticleStreamBuffer_D3D11					m_StreamSizeBuf;
	SParticleStreamDef							m_StreamDef;
	TArray<SParticleStreamBuffer_D3D11>			m_StreamBuffers;
};

//----------------------------------------------------------------------------
// implementation

const SParticleStreamFieldDef	&CParticleStreamToRender_D3D11::StreamDef(hh_u32 streamIndex) const
{
	HH_ASSERT(streamIndex < m_StreamDef.m_StreamDef.Count());
	return m_StreamDef.m_StreamDef[streamIndex];
}

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Stride>
HH_FORCEINLINE const SParticleStreamBuffer_D3D11	*CParticleStreamToRender_D3D11::Stream(CGuid streamIndex) const
{
	HH_STATIC_ASSERT(_Stride >= sizeof(_Type));
	if (streamIndex.Valid())
	{
		HH_ASSERT(streamIndex < m_StreamDef.m_StreamDef.Count());
		HH_ASSERT(streamIndex < m_StreamBuffers.Count());

		const SParticleStreamFieldDef	&streamDef = m_StreamDef.m_StreamDef[streamIndex];

		const EBaseTypeID				expectedType = TBaseTypeID<_Type>::TypeID;
		if (!HH_VERIFY(expectedType == streamDef.m_Type))
			return null;

		if (HH_VERIFY(_Stride <= streamDef.m_StorageSize) /*|| streamDef.m_StorageSize == 0*/)
			return AbstractStream(streamIndex);
	}
	return null;
}

#endif	// (HH_PARTICLES_UPDATER_USE_D3D11 != 0)

//----------------------------------------------------------------------------
//
//	Generic draw requests load and locker
//
//----------------------------------------------------------------------------

class	CParticleStorage;

class	CParticleStreamToRenderContainer
{
public:
	CParticleStreamToRenderContainer();
	~CParticleStreamToRenderContainer();

	CbStreamToRenderCullPage_MainMemory		m_CbCullPage_MainMemory;

	void							Clear();

	const CParticleStreamToRender	*AcquireStream(const CParticleStorage &particleStorage);
	void							ReleaseBack(const CParticleStreamToRender *stream);

private:
	hh_u32														m_MainMemoryCount;
	TChunkedArray<CParticleStreamToRender_MainMemory, 32>		m_MainMemory;
#if	(HH_PARTICLES_UPDATER_USE_D3D11 != 0)
	hh_u32														m_D3D11Count;
	TChunkedArray<CParticleStreamToRender_D3D11, 32>			m_D3D11;
#endif
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __PS_PARTICLES_STREAM_TO_RENDER_H__
