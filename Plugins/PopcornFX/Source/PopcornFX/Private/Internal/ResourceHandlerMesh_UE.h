//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#pragma once

#include "PopcornFXMinimal.h"

#include "Assets/PopcornFXMesh.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_resources.h>
#include <hh_geometrics/include/ge_mesh_resource.h>

class	CResourceHandlerMesh_UE : public PopcornFX::IResourceHandler
{
public:
	static void					Startup();
	static void					Shutdown();

public:
	CResourceHandlerMesh_UE();
	virtual ~CResourceHandlerMesh_UE();

	virtual void	*Load(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		const PopcornFX::CString			&resourcePath,
		bool					pathNotVirtual = false,
		bool					reload = false,
		SAsyncLoadStatus		*asyncLoadStatus = null,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
		const PopcornFX::CResourceManager	*resourceManager = null) override;

	virtual void	*Load(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		const PopcornFX::CFilePackPath		&resourcePath,
		bool					reload = false,
		SAsyncLoadStatus		*asyncLoadStatus = null,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
		const PopcornFX::CResourceManager	*resourceManager = null) override;

#if 0
	virtual void	*SyncWait(
		const SAsyncLoadStatus	&asyncLoadStatus,
		const PopcornFX::CResourceManager	*resourceManager = null) const override
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
#endif

	virtual void	Unload(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		void					*resource,
		const PopcornFX::CResourceManager	*resourceManager = null) override;

	virtual void	AppendDependencies(
		hh_u32					resourceTypeID,
		void					*resource,
		PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
		const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(
		hh_u32					resourceTypeID,
		const PopcornFX::CString			&resourcePath,
		bool					pathNotVirtual,
		PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
		const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(
		hh_u32					resourceTypeID,
		const PopcornFX::CFilePackPath		&resourcePath,
		PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
		const PopcornFX::CResourceManager	*resourceManager = null) const override;

	// implement only if you want popcorn's rutime to be able to reload your custom resource handler's resources.
	// the default resource handlers provided by popcorn implement this.
	//virtual void	BroadcastResourceChanged(const CFilePackPath &resourcePath) {}

};
