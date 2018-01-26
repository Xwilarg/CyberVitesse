//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "ResourceHandlerMesh_UE.h"

#include "PopcornFXPlugin.h"
#include "Assets/PopcornFXStaticMeshData.h"

#include "Engine/StaticMesh.h"

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXResourceHandlerMesh, Log, All);

#define LOCTEXT_NAMESPACE "PopcornFXMeshHandler"

//----------------------------------------------------------------------------
//
// CResourceHandlerMesh_UE
//
//----------------------------------------------------------------------------

namespace
{
	CResourceHandlerMesh_UE		*g_ResourceHandlerMesh_UE = null;
}

//----------------------------------------------------------------------------

// static
void	CResourceHandlerMesh_UE::Startup()
{
	HH_ASSERT(g_ResourceHandlerMesh_UE == null);
	g_ResourceHandlerMesh_UE = HH_NEW(CResourceHandlerMesh_UE);
	if (!HH_VERIFY(g_ResourceHandlerMesh_UE != null))
		return;

	PopcornFX::Resource::DefaultManager()->RegisterHandler<PopcornFX::CResourceMesh>(g_ResourceHandlerMesh_UE);
}

//----------------------------------------------------------------------------

// static
void	CResourceHandlerMesh_UE::Shutdown()
{
	if (HH_VERIFY(g_ResourceHandlerMesh_UE != null))
	{
		PopcornFX::Resource::DefaultManager()->UnregisterHandler<PopcornFX::CResourceMesh>(g_ResourceHandlerMesh_UE);
		HH_DELETE(g_ResourceHandlerMesh_UE);
		g_ResourceHandlerMesh_UE = null;
	}
}

//----------------------------------------------------------------------------


CResourceHandlerMesh_UE::CResourceHandlerMesh_UE()
{
}

//----------------------------------------------------------------------------

CResourceHandlerMesh_UE::~CResourceHandlerMesh_UE()
{
}

//----------------------------------------------------------------------------

void	*CResourceHandlerMesh_UE::Load(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		const PopcornFX::CString	&resourcePath,
		bool					pathNotVirtual,
		bool					reload,
		SAsyncLoadStatus		*asyncLoadStatus,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
		const PopcornFX::CResourceManager	*resourceManager)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerMesh_UE::Load", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CResourceMesh>::ResourceTypeID());

	UObject			*obj = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(resourcePath.Data(), pathNotVirtual);
	if (obj == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerMesh, Warning, TEXT("UObject not found \"%s\""), ANSI_TO_TCHAR(resourcePath.Data()));
		return null;
	}

	UStaticMesh		*staticMesh = Cast<UStaticMesh>(obj);
	if (staticMesh == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerMesh, Warning, TEXT("UObject is not a UStaticMesh \"%s\""), *obj->GetPathName());
		return null;
	}

	UPopcornFXMesh	*pkMesh = UPopcornFXMesh::FindStaticMesh(staticMesh);
	if (pkMesh == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerMesh, Warning, TEXT("Failed to load StaticMesh for PopcornFX \"%s\""), *staticMesh->GetPathName());
		return null;
	}

	void				*resourcePtr = pkMesh->LoadResourceMeshIFN(true).Get();
	if (asyncLoadStatus != null)
	{
		asyncLoadStatus->m_Resource = resourcePtr;
		asyncLoadStatus->m_Done = true;
		asyncLoadStatus->m_Progress = 1.0f;
	}
	return resourcePtr;
}

//----------------------------------------------------------------------------

void	*CResourceHandlerMesh_UE::Load(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		const PopcornFX::CFilePackPath		&resourcePath,
		bool					reload,
		SAsyncLoadStatus		*asyncLoadStatus,	// if null, will be a synchronous load, if not, the async loader will update the structure to tell the progress
		const PopcornFX::CResourceManager	*resourceManager)
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CResourceMesh>::ResourceTypeID());
	return Load(resourceTypeID, resourcePath.Path(), false, reload, asyncLoadStatus, resourceManager);
}

//----------------------------------------------------------------------------

void	CResourceHandlerMesh_UE::Unload(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		void					*resource,
		const PopcornFX::CResourceManager	*resourceManager)
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CResourceMesh>::ResourceTypeID());

}

//----------------------------------------------------------------------------

void	CResourceHandlerMesh_UE::AppendDependencies(
		hh_u32					resourceTypeID,
		void					*resource,
		PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
		const PopcornFX::CResourceManager	*resourceManager) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CResourceMesh>::ResourceTypeID());

}

//----------------------------------------------------------------------------

void	CResourceHandlerMesh_UE::AppendDependencies(
		hh_u32					resourceTypeID,
		const PopcornFX::CString			&resourcePath,
		bool					pathNotVirtual,
		PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
		const PopcornFX::CResourceManager	*resourceManager) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CResourceMesh>::ResourceTypeID());

}

//----------------------------------------------------------------------------

void	CResourceHandlerMesh_UE::AppendDependencies(
	hh_u32					resourceTypeID,
	const PopcornFX::CFilePackPath		&resourcePath,
	PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
	const PopcornFX::CResourceManager	*resourceManager) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CResourceMesh>::ResourceTypeID());

}

//----------------------------------------------------------------------------

// implement only if you want popcorn's rutime to be able to reload your custom resource handler's resources.
// the default resource handlers provided by popcorn implement this.
//virtual void	BroadcastResourceChanged(const CFilePackPath &resourcePath) {}

//----------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE
