#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2009/03/16 16:00 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ARRAY_STORAGE_H__
#define __KR_CONTAINERS_ARRAY_STORAGE_H__

//#define	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	Dynamic storage
//
//----------------------------------------------------------------------------

template<typename _Type, typename _ElementStoragePrimitives, typename _Controller>
class	TArrayContinuousStorage_Dynamic
{
protected:
	_Type	*m_Data;
#ifndef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
	hh_u32	m_DataSizeInBytes;
#endif

	TArrayContinuousStorage_Dynamic()
	:	m_Data(null)
#ifndef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
	,	m_DataSizeInBytes(0)
#endif
	{
	}
	~TArrayContinuousStorage_Dynamic()
	{
		_Clean();
	}

#ifdef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
	HH_FORCEINLINE hh_u32	_MaxCount() const { hh_u32 maxSize = Mem::Size(m_Data); HH_PARANOID_ASSERT(maxSize % sizeof(_Type) == 0); return maxSize / sizeof(_Type); }
#else
	HH_FORCEINLINE hh_u32	_MaxCount() const { HH_PARANOID_ASSERT(m_DataSizeInBytes % sizeof(_Type) == 0); return m_DataSizeInBytes / sizeof(_Type); }
#endif

	bool	_GrowIFP(hh_u32 targetSize, hh_u32 prevSize)
	{
		// try to grow the slots
		HH_ASSERT(prevSize == _MaxCount());
		const hh_u32	newSize = _Controller::GrowSize(targetSize);
		const hh_u32	newSizeInBytes = newSize * sizeof(_Type);
		_Type			*tmpBuffer = (_Type*)HH_REALLOC_ALIGNED(m_Data, newSizeInBytes, _Controller::Alignment());
		if (tmpBuffer == null)
		{
			HH_ASSERT_MESSAGE(false, "could not grow TSlotArray<%s> to %d elements (%d bytes)\n" COMMA "" COMMA newSize COMMA newSizeInBytes);
			return false;
		}
		m_Data = tmpBuffer;
#ifndef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
		m_DataSizeInBytes = newSizeInBytes;
#endif

		_ElementStoragePrimitives::ConstructRange(m_Data + prevSize, m_Data + newSize);
		return true;
	}

	void	_Clean()
	{
		if (m_Data != null)
		{
			_ElementStoragePrimitives::DestroyRange(m_Data, m_Data + _MaxCount());
			HH_FREE(m_Data);
			m_Data = null;
#ifndef	KR_CONTAINERS_FETCH_BUFSIZE_FROM_KR_MEM
			m_DataSizeInBytes = 0;
#endif
		}
	}

public:
};

//----------------------------------------------------------------------------
//
//	Static storage
//
//----------------------------------------------------------------------------

template<typename _Type, typename _ElementStoragePrimitives, hh_u32 _Size, hh_u32 _Alignment>
class	TArrayContinuousStorage_Static
{
protected:
	_Type	*m_Data;
private:
	hh_u8	m_RawDataStorage[_Size * sizeof(_Type) + (_Alignment - 1)];	// here, this should most likely be aligned with sizeof(m_Data), but don't rely on this, and provide sufficient space for dynamic construction alignment...
protected:
	TArrayContinuousStorage_Static()
	{
		m_Data = Mem::Align<_Alignment>((_Type*)&m_RawDataStorage[0]);
		_ElementStoragePrimitives::ConstructRange(m_Data, m_Data + _MaxCount());
	}
	~TArrayContinuousStorage_Static() { _Clean(); }

	HH_FORCEINLINE hh_u32		_MaxCount() const { return _Size; }

	bool	_GrowIFP(hh_u32 targetSize, hh_u32 prevSize) { return false; }
	void	_Clean()
	{
		_ElementStoragePrimitives::DestroyRange(m_Data, m_Data + _MaxCount());
	}

public:
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

template<typename _Type>
struct	TDefaultElementStoragePrimitives
{
	HH_FORCEINLINE static void	ConstructRange(_Type *first, _Type *last)
	{
		while (first < last)
		{
			Mem::Construct(*first++);
		}
	}
	HH_FORCEINLINE static void	DestroyRange(_Type *first, _Type *last)
	{
		while (first < last)
			Mem::Destruct(*first++);
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ARRAY_STORAGE_H__
