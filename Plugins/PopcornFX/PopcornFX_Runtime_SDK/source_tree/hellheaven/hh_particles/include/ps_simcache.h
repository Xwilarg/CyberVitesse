#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2016/10/08 08:57 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_SIMCACHE_H__
#define	__PS_SIMCACHE_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	SimulationCache
{

	struct	SFileHeader
	{
		hh_u8		m_Magic[4];
		hh_u32		m_Version;
		hh_u32		m_FieldCount;
		hh_u32		m_FrameCount;
		hh_u32		m_Reserved[4];
	};
	HH_STATIC_ASSERT(sizeof(SFileHeader) == 32);

	struct	SFieldHeader
	{
		hh_u32		m_ChannelCount;
		hh_u8		m_ChannelType;
		hh_u8		m_Reserved[3];

		enum
		{
			ChannelType_I32 = 0,
			ChannelType_F32,
			ChannelType_F16,
			__MaxFieldTypes,
		};
	};
	HH_STATIC_ASSERT(sizeof(SFieldHeader) == 8);

	struct	SFrameHeader
	{
		hh_u32		m_AbsFrameOffset;
		float		m_FrameTime;
		hh_u32		m_EmittedCount;		// number of particles since the start of the sim
		hh_u32		m_ParticleCount;	// number of particles inside this frame
		hh_u32		m_LeadingDeadCount;	// number of dead particles not written upfront the 'id' stream
	};
	HH_STATIC_ASSERT(sizeof(SFrameHeader) == 20);

}	// namespace SimulationCache

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(SimulationCache);

//----------------------------------------------------------------------------

class	CSimulationCache : public CRefCountedObject
{
public:
	struct	SField
	{
		CStringId	m_NameID;
		hh_u32		m_Channels;
		EBaseTypeID	m_ScalarType;
		EBaseTypeID	m_Type;
		bool		m_SrcIsFp16;

		SField() : m_Channels(0), m_ScalarType(BaseType_Void), m_Type(BaseType_Void), m_SrcIsFp16(false) {}
	};

	struct	SFrame
	{
		hh_u32		m_AbsFrameOffset;
		float		m_FrameTime;
		hh_u32		m_EmittedCount;
		hh_u32		m_ParticleCount;
		hh_u32		m_LeadingDeadCount;
	};

private:
	hh_u8					*m_WholeDataset;	// hardcore. should we stream ?
	hh_u32					m_WholeDatasetSizeInBytes;

	float					m_Duration;

	CGuid					m_IdFieldIndex;
	const hh_u8				*m_FirstFramePtr;

	TArray<SField>			m_Fields;
	TArray<SFrame>			m_Frames;

public:
	CSimulationCache();
	~CSimulationCache();

	static PSimulationCache		Load(const CString &path);

	const hh_u8					*FirstFramePtr() const { return m_FirstFramePtr; }
	TMemoryView<const SField>	Fields() const { return m_Fields; }
	TMemoryView<const SFrame>	Frames() const { return m_Frames; }

	float						Duration() const { return m_Duration; }
	CGuid						IdFieldIndex() const { return m_IdFieldIndex; }
};
HH_DECLARE_REFPTRCLASS(SimulationCache);

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_SIMCACHE_H__


