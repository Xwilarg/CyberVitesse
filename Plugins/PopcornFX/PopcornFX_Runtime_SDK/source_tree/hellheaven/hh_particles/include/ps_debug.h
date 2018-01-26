#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2010/03/15 11:35 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __EN_PARTICLES_DEBUG_H__
#define __EN_PARTICLES_DEBUG_H__

#include "hh_particles/include/ps_config.h"
#include "hh_particles/include/ps_system.h"

#include "hh_particles/include/Storage/MainMemory/storage_ram_stream.h"	// for CParticleStream_MainMemory

__PK_API_BEGIN
//----------------------------------------------------------------------------

struct	SDebugParticleStreamInfo
{
	CStringId	m_NameGUID;
	CStringId	m_TypeString;
	hh_u32		m_TypeSizeInBytes;

	SDebugParticleStreamInfo() : m_TypeSizeInBytes(0) {}
};

//----------------------------------------------------------------------------

struct	HH_PARTICLES_EXPORT SDebugParticleInfos
{
	hh_u32								m_BytesPerParticle;
	TArray<SDebugParticleStreamInfo>	m_StreamInfos;

	SDebugParticleInfos() { Clear(); }
	void	Clear()
	{
		m_BytesPerParticle = 0;
		m_StreamInfos.Clear();
	}
};

//----------------------------------------------------------------------------
//
//	Debug history frames
//
//----------------------------------------------------------------------------

#if	(HH_PARTICLES_BUILD_DEBUG_HISTORY != 0)

class	HH_PARTICLES_EXPORT CParticleDebugHistoryFrame : public CRefCountedObject
{
public:
	CParticleDebugHistoryFrame();
	virtual ~CParticleDebugHistoryFrame();

	CParticleStream_MainMemory	m_Stream;
	SParticleStreamDef			*m_StreamDefinition;
	CAABB						m_BBox;
	CFloat2						m_VisibilityCoeffs;
	float						m_Dt;

	void						Setup(const CParticleStream_MainMemory &stream, const CAABB &bbox, const CFloat2 &visibilityCoeffs, float dt);
};
HH_DECLARE_REFPTRCLASS(ParticleDebugHistoryFrame);

//----------------------------------------------------------------------------

class	HH_PARTICLES_EXPORT CParticleDebugHistory
{
private:
	hh_u32								m_LastFrameIndex;
	TArray<PParticleDebugHistoryFrame>	m_Frames;

	hh_u32								_RemapVirtualToRealFrameIndex(hh_u32 virtualFrameIndex) const;

public:
	CParticleDebugHistory() : m_LastFrameIndex(0) {}
	virtual ~CParticleDebugHistory() {}

	hh_u32								FrameCount() const { return m_Frames.Count(); }
	bool								Empty() const { return m_Frames.Empty(); }

	void								SetMaxHistoryFramesCount(hh_u32 frameCount);
	PParticleDebugHistoryFrame			GetFrame(hh_u32 frameIndex);
	PCParticleDebugHistoryFrame			GetFrame(hh_u32 frameIndex) const;
	void								PushFrame(const CParticleMedium *medium, const CParticleStream_MainMemory &stream, const CAABB &bbox, float dt);
};

#endif	// HH_PARTICLES_BUILD_DEBUG_HISTORY

//----------------------------------------------------------------------------

const char			*GetObjectCustomNameIFP(const CBaseObject &object);

bool				BuildParticleObjectTreeViewName(CString &outPrettyName, const CString &objectPath, bool prependFilePath = true);
bool				BuildParticleObjectTreeViewName(CString &outPrettyName, const CBaseObject &object, bool prependFilePath = true);
HH_INLINE bool		BuildParticleObjectTreeViewName(CString &outPrettyName, const CBaseObject *object, bool prependFilePath = true) { if (HH_VERIFY(object != null)) return BuildParticleObjectTreeViewName(outPrettyName, *object, prependFilePath); return false; }

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __EN_PARTICLES_DEBUG_H__
