#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2008/04/29 19:32 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __PS_PARTICLES_STREAM_H__
#define __PS_PARTICLES_STREAM_H__

#include <hh_maths/include/hh_maths_primitives.h>
#include <hh_kernel/include/kr_containers_any.h>
#include <hh_kernel/include/kr_string_id.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CParticlePageToRender;
class	CParticlePageToRender_MainMemory;
class	CParticleStreamToRender_MainMemory;

HH_FORWARD_DECLARE(ParticleStreamToRender);
HH_FORWARD_DECLARE(ParticleStreamToRender_MainMemory);

//----------------------------------------------------------------------------

HH_FORWARD_DECLARE(ParticleRawPage_MainMemory);

//----------------------------------------------------------------------------

struct	SParticleStreamFieldDef
{
	hh_u32			m_StorageSize;
	EBaseTypeID		m_Type;
	CStringId		m_NameGUID;

	SParticleStreamFieldDef(CStringId nameGUID, EBaseTypeID type, hh_u32 storageSize)
	:	m_StorageSize(storageSize)
	,	m_Type(type)
	,	m_NameGUID(nameGUID)
	{
		HH_ASSERT(m_StorageSize == 0 || m_StorageSize >= CBaseTypeTraits::Traits(m_Type).Size);
		HH_ASSERT(m_StorageSize == 0 || IntegerTools::IsPowerOfTwo(m_StorageSize));	// required for fast-moves/swaps/kills
		HH_ASSERT(m_StorageSize <= 16);
	}

	bool		operator == (const SParticleStreamFieldDef &other) const
	{
		return	m_StorageSize == other.m_StorageSize &&
				m_Type == other.m_Type &&
				m_NameGUID == other.m_NameGUID;
	}
	bool		operator != (const SParticleStreamFieldDef &other) const { return !(*this == other); }
};

//----------------------------------------------------------------------------

struct	SParticleStreamDef
{
#ifndef	HH_RETAIL
//	mutable TAtomic<hh_u32>			m_RefCount;
#endif

	TArray<SParticleStreamFieldDef>	m_StreamDef;
	CGuid							m_BoundsPositionStreamId;
	CGuid							m_BoundsSizeStreamId;

	bool		Empty() const { return m_StreamDef.Empty(); }
	void		Clear() { m_StreamDef.Clear(); m_BoundsPositionStreamId.Clear(); m_BoundsSizeStreamId.Clear(); }
	bool		Copy(const SParticleStreamDef &other)
	{
		m_StreamDef = other.m_StreamDef;
		m_BoundsPositionStreamId = other.m_BoundsPositionStreamId;
		m_BoundsSizeStreamId = other.m_BoundsSizeStreamId;
		return HH_VERIFY(m_StreamDef.Count() == other.m_StreamDef.Count());
	}

	CGuid		StreamId(CStringId nameGUID) const
	{
		if (nameGUID.Valid())
		{
			for (hh_u32 i = 0; i < m_StreamDef.Count(); i++)
			{
				if (m_StreamDef[i].m_NameGUID == nameGUID)
				{
					return i;
				}
			}
		}
		return CGuid::INVALID;
	}

private:
	// unsafe, use Copy and check return value
	const SParticleStreamDef		&operator = (const SParticleStreamDef &other)
	{
		Copy(other);
		return *this;
	}
};

//----------------------------------------------------------------------------
//
//	Particle abstract stream:
//
//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleStreamAbstract : public CNonCopyable
{
public:
	virtual ~CParticleStreamAbstract() {}

	virtual const SParticleStreamDef	*GetStreamDef() const = 0;
//	virtual void						Clear() = 0;
	virtual hh_u32						ParticleCount() const = 0;	// watchout, might be expensive depending on the implementation
	virtual void						Lock() const = 0;
	virtual void						Unlock() const = 0;
};

//----------------------------------------------------------------------------
__PK_API_END

//#include "hh_particles/include/Storage/MainMemory/storage_ram_stream.h"	// To avoid breaking everyone's code...

#endif // __PS_PARTICLES_STREAM_H__

