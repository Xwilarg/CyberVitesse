#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2013/10/14 11:23 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_RESOURCES_H__
#define __KR_RESOURCES_H__

#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_delegates.h"
#include "hh_kernel/include/kr_file.h"
#include "hh_kernel/include/kr_string_id.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CResourceManager;

//----------------------------------------------------------------------------

class	IResourceHandler
{
public:
	virtual ~IResourceHandler() {}

	struct	SAsyncLoadStatus
	{
		void	*m_Resource;	// may be null when loading is done => means load has failed
		bool	m_Done;			// will be 'true' when loading is done.
		float	m_Progress;		// between 0 and 1.0f, DO NOT USE TO DETECT RESOURCE LOAD COMPLETENESS, this is just an optional indication that the loader may or may not fill.
		SAsyncLoadStatus() : m_Resource(null), m_Done(false), m_Progress(0.0f) {}
	};

//	virtual bool	CanLoad(CStringId extensionWithoutDot) = 0;

	virtual void	*Load(	hh_u32					resourceTypeID,	// used to check we are called with the correct type
							const CString			&resourcePath,
							bool					pathNotVirtual = false,
							bool					reload = false,
							SAsyncLoadStatus		*asyncLoadStatus = null,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
							const CResourceManager	*resourceManager = null) = 0;

	virtual void	*Load(	hh_u32					resourceTypeID,	// used to check we are called with the correct type
							const CFilePackPath		&resourcePath,
							bool					reload = false,
							SAsyncLoadStatus		*asyncLoadStatus = null,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
							const CResourceManager	*resourceManager = null) = 0;

	virtual void	*SyncWait(	const SAsyncLoadStatus	&asyncLoadStatus,
								const CResourceManager	*resourceManager = null) const
	{
		void	* volatile const *resourcePtr = &asyncLoadStatus.m_Resource;
		HH_ASSERT(resourcePtr != null);
		HH_SCOPEDPROFILESPIN("IResourceHandler::SyncWait");
		while (*resourcePtr == null)
		{
			CCurrentThread::Yield();
		}
		return *resourcePtr;
	}

	virtual void	Unload(	hh_u32					resourceTypeID,	// used to check we are called with the correct type
							void					*resource,
							const CResourceManager	*resourceManager = null) = 0;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										void					*resource,
										TArray<CString>			&outResourcePaths,
										const CResourceManager	*resourceManager = null) const = 0;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const CString			&resourcePath,
										bool					pathNotVirtual,
										TArray<CString>			&outResourcePaths,
										const CResourceManager	*resourceManager = null) const = 0;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const CFilePackPath		&resourcePath,
										TArray<CString>			&outResourcePaths,
										const CResourceManager	*resourceManager = null) const = 0;

	// implement only if you want popcorn's rutime to be able to reload your custom resource handler's resources.
	// the default resource handlers provided by popcorn implement this.
	virtual void	BroadcastResourceChanged(const CFilePackPath &resourcePath) {}
};

//----------------------------------------------------------------------------

struct	SResourceMapper
{
	Threads::CCriticalSection			m_HandlersLock;

	// does not take ownership of the resource handlers pointers
	typedef TArray<IResourceHandler*>	ResourceHandlerList;
	TArray<ResourceHandlerList>			m_ResourceHandlers;

	bool								RegisterHandler(hh_u32 id, IResourceHandler *handler);
	void								UnregisterHandler(hh_u32 id, IResourceHandler *handler);
	IResourceHandler					*Map(hh_u32 id/*, CStringId extensionWithoutDot*/) const;
	void								BroadcastResourceChanged(const CFilePackPath &resourcePath) const;
};

//----------------------------------------------------------------------------

namespace	Internal
{
	extern void		HH_ENGINEUTILS_EXPORT InitializeResourceTypeID(hh_i32 *handlerID);
}

//----------------------------------------------------------------------------

template<typename _ResType>
struct	TResourceRouter
{
	static hh_u32	ResourceTypeID()
	{
		// thread-safe one-time init
		static hh_i32	handlerID = -1;
		if (handlerID < 0)
		{
			Internal::InitializeResourceTypeID(&handlerID);
		}
		HH_ASSERT(handlerID >= 0);	// should never fail.
		return static_cast<hh_u32>(handlerID);
	}
};

//----------------------------------------------------------------------------
// disallow mappings with pointers & refptr

template<typename _ResType> struct	TResourceRouter<_ResType*> { };

template<typename _ResType> struct	TResourceRouter<TRefPtr<_ResType> > { };

template<typename _ResType> struct	TResourceRouter<TWeakPtr<_ResType> > { };

//----------------------------------------------------------------------------

template<typename _Type>
class	TResourcePtr;

//----------------------------------------------------------------------------

template<typename _Type>
class	TResourceRawPtr// : public CNonCopyable
{
private:
	friend class			TResourcePtr<_Type>;

	TRefPtr<_Type>			m_ResPtr;
	const CResourceManager	*m_ResManager;

public:
	TResourceRawPtr(_Type *resPtr, const CResourceManager *resManager)
	:	m_ResPtr(resPtr)
	,	m_ResManager(resManager)
	{
	}
	TResourceRawPtr(const TResourceRawPtr<_Type> &other)
	:	m_ResPtr(other.m_ResPtr)
	,	m_ResManager(other.m_ResManager)
	{
	}
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TResourcePtr : public CNonCopyable
{
private:
	TResourceRawPtr<_Type>	m_Ptr;
#ifdef	HH_DEBUG
	TResourcePtr<_Type>	&operator = (const TResourcePtr<_Type> &other);	// no need for this, CNonCopyable takes care of it. just used to get better error-reporting under MSVC
#endif

public:
	TResourcePtr()
	:	m_Ptr(null, null)
	{
	}

#if	(PK_HAS_RVALUE_REF != 0)
	TResourcePtr(TResourcePtr &&other)
	:	m_Ptr(null, null)
	{
		HH_ASSERT((m_Ptr.m_ResPtr == null) == (m_Ptr.m_ResManager == null));
		m_Ptr.m_ResPtr.Swap(other.m_Ptr.m_ResPtr);
		HHSwap(m_Ptr.m_ResManager, other.m_Ptr.m_ResManager);
	}
	TResourcePtr		&operator = (TResourcePtr &&other)
	{
		HH_ASSERT((m_Ptr.m_ResPtr == null) == (m_Ptr.m_ResManager == null));
		Clear();
		m_Ptr.m_ResPtr.Swap(other.m_Ptr.m_ResPtr);
		HHSwap(m_Ptr.m_ResManager, other.m_Ptr.m_ResManager);
		return *this;
	}
#endif

	TResourcePtr(const TResourceRawPtr<_Type> &rawPtr)	// would benefit from move semantics
	:	m_Ptr(rawPtr.m_ResPtr.Get(), rawPtr.m_ResManager)
	{
		HH_ASSERT((m_Ptr.m_ResPtr == null) == (m_Ptr.m_ResManager == null));
	}

	~TResourcePtr()
	{
		Clear();
	}

	bool	Empty() const
	{
		return m_Ptr.m_ResPtr == null;
	}

	void	Clear()
	{
		HH_ASSERT((m_Ptr.m_ResPtr == null) == (m_Ptr.m_ResManager == null));
		if (m_Ptr.m_ResPtr != null && m_Ptr.m_ResManager != null)
		{
			m_Ptr.m_ResManager->Unload(m_Ptr.m_ResPtr);
		}
		m_Ptr.m_ResPtr = null;
		m_Ptr.m_ResManager = null;
	}

	_Type	*operator -> () const
	{
		HH_ASSERT(m_Ptr.m_ResPtr != null);
		return m_Ptr.m_ResPtr.Get();
	}

	const _Type		&operator * () const
	{
		HH_ASSERT(m_Ptr.m_ResPtr != null);
		return *m_Ptr.m_ResPtr.Get();
	}

	bool	operator == (const TResourcePtr<_Type> &other) const
	{
		return m_Ptr.m_ResPtr == other.m_Ptr.m_ResPtr;
	}
	bool	operator == (const _Type *other) const
	{
		return m_Ptr.m_ResPtr == other;
	}
	bool	operator != (const TResourcePtr<_Type> &other) const
	{
		return !(operator == (other));
	}
	bool	operator != (const _Type *other) const
	{
		return !(operator == (other));
	}

	TResourcePtr<_Type>	&operator = (const TResourceRawPtr<_Type> &rawPtr)
	{
		Clear();
		m_Ptr.m_ResPtr = rawPtr.m_ResPtr;
		m_Ptr.m_ResManager = rawPtr.m_ResManager;
		return *this;
	}

	void		StealOwnershipFrom(TResourcePtr<_Type> &other)
	{
		Clear();
		m_Ptr.m_ResPtr = other.m_Ptr.m_ResPtr;
		m_Ptr.m_ResManager = other.m_Ptr.m_ResManager;
		other.m_Ptr.m_ResPtr = null;
		other.m_Ptr.m_ResManager = null;
	}
};

//----------------------------------------------------------------------------

//template<typename _Type>
//static bool	operator == (const _Type *lhs, const TResourcePtr<_Type> &rhs)
//{
//	return lhs == rhs.m_ResPtr;
//}
//template<typename _Type>
//static bool	operator != (const _Type *lhs, const TResourcePtr<_Type> &rhs)
//{
//	return !(lhs == rhs);
//}

//----------------------------------------------------------------------------

class	CResourceManager : public CNonCopyable
{
protected:
	SResourceMapper		m_ResourceMapper;

public:
	CResourceManager() {}
	~CResourceManager() {}

	FastDelegate<void(CString &, bool &)>	m_RemapResourcePath;
	void									RemapAndPurifyPathIFN(CString &inOutPath, bool &inOutPathNotVirtual) const;
	void									NotifyResourceChanged(const CFilePackPath &resourcePath) const;

	template<typename _ResType>
	bool		RegisterHandler(IResourceHandler *handler)
	{
		return m_ResourceMapper.RegisterHandler(TResourceRouter<_ResType>::ResourceTypeID(), handler);
	}

	template<typename _ResType>
	void		UnregisterHandler(IResourceHandler *handler)
	{
		m_ResourceMapper.UnregisterHandler(TResourceRouter<_ResType>::ResourceTypeID(), handler);
	}

	template<typename _ResType>
	IResourceHandler	*FindHandler(/*const CString &resourcePath, bool pathNotVirtual = false*/) const
	{
		hh_u32	resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		return m_ResourceMapper.Map(resTypeId);
	}

	template<typename _ResType>
	TResourceRawPtr<_ResType>	Load(const CString &resourcePath, bool pathNotVirtual = false, bool reload = false) const
	{
		hh_u32				resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		IResourceHandler	*handler = m_ResourceMapper.Map(resTypeId);
		_ResType			*resPtr = null;
		if (handler != null)
		{
			CString	remappedResPath = resourcePath;
			bool	remappedResPathNotVirtual = pathNotVirtual;
			RemapAndPurifyPathIFN(remappedResPath, remappedResPathNotVirtual);
			resPtr = static_cast<_ResType*>(handler->Load(resTypeId, remappedResPath, remappedResPathNotVirtual, reload, null, this));
		}
		return TResourceRawPtr<_ResType>(resPtr, resPtr != null ? this : null);
	}

	template<typename _ResType>
	TResourceRawPtr<_ResType>	Load(const CFilePackPath &resourcePath, bool reload = false) const
	{
		hh_u32				resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		IResourceHandler	*handler = m_ResourceMapper.Map(resTypeId);
		_ResType			*resPtr = null;
		if (handler != null)
			resPtr = static_cast<_ResType*>(handler->Load(resTypeId, resourcePath, reload, null, this));
		return TResourceRawPtr<_ResType>(resPtr, resPtr != null ? this : null);
	}

	template<typename _ResType>
	void	Unload(_ResType *resource) const	// the resource handler should take care of refcounting if applicable.
	{
		hh_u32				resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		IResourceHandler	*handler = m_ResourceMapper.Map(resTypeId);
		if (handler != null)
			handler->Unload(resTypeId, resource, this);
	}

	template<typename _ResType>
	void	Unload(const TRefPtr<_ResType> &resource) const	// the resource handler should take care of refcounting if applicable.
	{
		Unload(resource.Get());
	}

#if 0	// not available here yet, for the time being, use the AssetBaker lib functionality
	template<typename _ResType>
	void	AppendDependencies(_ResType *resource, TArray<CString> &outResourcePaths) const
	{
		outResourcePaths.Clear();
		hh_u32				resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		IResourceHandler	*handler = m_ResourceMapper.Map(resTypeId);
		if (handler != null)
			handler->AppendDependencies(resTypeId, resource, outResourcePaths, this);
	}

	template<typename _ResType>
	void	AppendDependencies(const TResourceRawPtr<_ResType> &resource, TArray<CString> &outResourcePaths) const
	{
		AppendDependencies(resource.operator -> (), outResourcePaths);
	}

	template<typename _ResType>
	void	AppendDependencies(const CString &resourcePath, bool pathNotVirtual, TArray<CString> &outResourcePaths) const
	{
		outResourcePaths.Clear();
		hh_u32				resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		IResourceHandler	*handler = m_ResourceMapper.Map(resTypeId);
		if (handler != null)
		{
			CString	remappedResPath = resourcePath;
			bool	remappedResPathNotVirtual = pathNotVirtual;
			RemapAndPurifyPathIFN(remappedResPath, remappedResPathNotVirtual);

			handler->AppendDependencies(resTypeId, remappedResPath, remappedResPathNotVirtual, outResourcePaths, this);
		}
	}

	template<typename _ResType>
	void	AppendDependencies(const CFilePackPath &resourcePath, TArray<CString> &outResourcePaths) const
	{
		outResourcePaths.Clear();
		hh_u32				resTypeId = TResourceRouter<_ResType>::ResourceTypeID();
		IResourceHandler	*handler = m_ResourceMapper.Map(resTypeId);
		if (handler != null)
			handler->AppendDependencies(resTypeId, resourcePath, outResourcePaths, this);
	}
#endif
};

//----------------------------------------------------------------------------

namespace	Resource
{
	// don't use this:
	extern HH_ENGINEUTILS_EXPORT CResourceManager	*g_ResourceManager;

	// use that for future compatibility:
	static HH_FORCEINLINE CResourceManager	*DefaultManager() { return g_ResourceManager; }
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_RESOURCES_H__
