#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2013/10/21 06:30 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GE_RECTANGLE_LIST_H__
#define __GE_RECTANGLE_LIST_H__

#include <hh_maths/include/hh_maths_primitives.h>
#include <hh_kernel/include/kr_resources.h>
#include <hh_kernel/include/kr_callbacks.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_GEOMETRICS_EXPORT CRectangleList : public CRefCountedObject
{
public:
	TArray<TRectangleMapper<float>::TCorners, TArrayAligned16>		m_CornersFp32;	// 4 * 2 * 4 = 32 bytes
	TArray<CFloat4, TArrayAligned16>								m_RectsFp32;

	// Trimming / cutouts:
	hh_u32															m_TrimmedVertexCount;
	TArray<CFloat2>													m_TrimmingData;
	TArray<CFloat2>													m_TrimmingData_Blended;

	CRectangleList() : m_TrimmedVertexCount(0) {}

	void	Clear()
	{
		m_CornersFp32.Clear();
		m_RectsFp32.Clear();
		m_TrimmedVertexCount = 0;
		m_TrimmingData.Clear();
		m_TrimmingData_Blended.Clear();
	}

	void	Swap(CRectangleList &other)
	{
		// don't swap notifiers, only swap the actual data representation
		HHSwap(m_CornersFp32, other.m_CornersFp32);
		HHSwap(m_RectsFp32, other.m_RectsFp32);
		HHSwap(m_TrimmedVertexCount, other.m_TrimmedVertexCount);
		HHSwap(m_TrimmingData, other.m_TrimmingData);
		HHSwap(m_TrimmingData_Blended, other.m_TrimmingData_Blended);
	}

	bool	CopyFrom(const CRectangleList &other)
	{
		// don't copy notifiers
		m_CornersFp32 = other.m_CornersFp32;
		m_RectsFp32 = other.m_RectsFp32;
		m_TrimmedVertexCount = other.m_TrimmedVertexCount;
		m_TrimmingData = other.m_TrimmingData;
		m_TrimmingData_Blended = other.m_TrimmingData_Blended;

		if (!HH_VERIFY(m_CornersFp32.Count() == other.m_CornersFp32.Count()) ||
			!HH_VERIFY(m_RectsFp32.Count() == other.m_RectsFp32.Count()) ||
			!HH_VERIFY(m_TrimmingData.Count() == other.m_TrimmingData.Count()) ||
			!HH_VERIFY(m_TrimmingData_Blended.Count() == other.m_TrimmingData_Blended.Count()))
		{
			Clear();
			return false;
		}
		return true;
	}

	TCallbackChainUnordered<void(CRectangleList *self)>	m_OnReloading;
	TCallbackChainUnordered<void(CRectangleList *self)>	m_OnReloaded;
};
HH_DECLARE_REFPTRCLASS(RectangleList);

//----------------------------------------------------------------------------
//
//	Rect-List resource manager
//
//----------------------------------------------------------------------------

class	CResourceHandlerRectangleList : public IResourceHandler
{
protected:
	struct	SResourceEntry
	{
		PRectangleList	m_RectList;
		hh_u32			m_ReferenceCount;

		SResourceEntry(const PRectangleList &rectList, hh_u32 refCount) : m_RectList(rectList), m_ReferenceCount(refCount) {}
	};

	Threads::CCriticalSection			m_Lock;
	THashMap<SResourceEntry, CString>	m_RectLists;

	bool			_LoadRectList(const CFilePackPath &resourcePath, CRectangleList &outRectList);

public:
	CResourceHandlerRectangleList();
	virtual ~CResourceHandlerRectangleList();

//	virtual bool	CanLoad(CStringId extensionWithoutDot) = 0;

	virtual void	*Load(	hh_u32					resourceTypeID,	// used to check we are called with the correct type
							const CString			&resourcePath,
							bool					pathNotVirtual = false,
							bool					reload = false,
							SAsyncLoadStatus		*asyncLoadStatus = null,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
							const CResourceManager	*resourceManager = null) override;

	virtual void	*Load(	hh_u32					resourceTypeID,	// used to check we are called with the correct type
							const CFilePackPath		&resourcePath,
							bool					reload = false,
							SAsyncLoadStatus		*asyncLoadStatus = null,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
							const CResourceManager	*resourceManager = null) override;

	virtual void	Unload(	hh_u32					resourceTypeID,	// used to check we are called with the correct type
							void					*resource,
							const CResourceManager	*resourceManager = null) override;


	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										void					*resource,
										TArray<CString>			&outResourcePaths,
										const CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const CString			&resourcePath,
										bool					pathNotVirtual,
										TArray<CString>			&outResourcePaths,
										const CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const CFilePackPath		&resourcePath,
										TArray<CString>			&outResourcePaths,
										const CResourceManager	*resourceManager = null) const override;

	virtual void	BroadcastResourceChanged(const CFilePackPath &resourcePath) override;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __GE_RECTANGLE_LIST_H__

