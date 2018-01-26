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

#include "PixelFormat.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_resources.h>
#include <hh_imaging/include/im_image.h>
#include <hh_kernel/include/kr_containers_hash.h>
#include <hh_kernel/include/kr_threads_basics.h>
#include <hh_kernel/include/kr_refcounted_buffer.h>
#if (PK_GPU_D3D11 != 0)
#	include <hh_particles/include/Samplers/D3D11/image_gpu_d3d11.h>
#endif

class UTexture;

class	CResourceHandlerImage_UE : public PopcornFX::IResourceHandler
{
public:
	static void					Startup();
	static void					Shutdown();

public:
	static PopcornFX::CImage	*NewFromPath(const PopcornFX::CString &path, bool pathNotVirtual);
	static PopcornFX::CImage	*NewFromTexture(UTexture *texture);

	struct	SResourceEntry
	{
		PopcornFX::PImage						m_Image;
		hh_u32									m_ReferenceCount;
		bool									m_Virtual;
		PopcornFX::PRefCountedMemoryBuffer		m_CacheLastVirtual;

		SResourceEntry(const PopcornFX::PImage &image, hh_u32 refCount, bool isVirtual = false) : m_Image(image), m_ReferenceCount(refCount), m_Virtual(isVirtual) {}
	};

	typedef PopcornFX::THashMap<SResourceEntry, PopcornFX::CString>		CResourcesHashMap;

private:
	PopcornFX::Threads::CCriticalSection		m_Lock;
	CResourcesHashMap							m_Images;

public:
	CResourceHandlerImage_UE();
	virtual ~CResourceHandlerImage_UE();

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

	virtual void	Unload(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		void					*resource,
		const PopcornFX::CResourceManager	*resourceManager = null) override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										void					*resource,
										PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
										const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const PopcornFX::CString			&resourcePath,
										bool					pathNotVirtual,
										PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
										const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const PopcornFX::CFilePackPath		&resourcePath,
										PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
										const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	BroadcastResourceChanged(const PopcornFX::CFilePackPath &resourcePath);



	bool			IsUsed(const PopcornFX::CString &virtualPath, bool pathNotVirtual) const;

	PopcornFX::CImage		m_VirtImageCache;
	bool			RegisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual, PopcornFX::CImage::EFormat format, const CUint2 &size, const void *pixelsData, hh_u32 pixelsDataSizeInBytes);
	bool			UnregisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual);


};

#if (PK_GPU_D3D11 != 0)

class	CResourceHandlerImage_UE_D3D11 : public PopcornFX::IResourceHandler
{
public:
	static void					Startup();
	static void					Shutdown();

public:
	static PopcornFX::CImageGPU_D3D11	*NewFromTexture(UTexture *texture);
	static PopcornFX::CImageGPU_D3D11	*NewFromPath(const PopcornFX::CString &path, bool pathNotVirtual);

	struct	SResourceEntry
	{
		PopcornFX::PImageGPU_D3D11		m_Image;
		hh_u32							m_ReferenceCount;
		bool							m_Virtual;

		SResourceEntry(const PopcornFX::PImageGPU_D3D11 &image, hh_u32 refCount, bool isVirtual = false) : m_Image(image), m_ReferenceCount(refCount), m_Virtual(isVirtual) {}
	};

private:
	PopcornFX::Threads::CCriticalSection					m_Lock;
	PopcornFX::THashMap<SResourceEntry, PopcornFX::CString>	m_Images;

public:
	CResourceHandlerImage_UE_D3D11();
	virtual ~CResourceHandlerImage_UE_D3D11();

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

	virtual void	Unload(
		hh_u32					resourceTypeID,	// used to check we are called with the correct type
		void					*resource,
		const PopcornFX::CResourceManager	*resourceManager = null) override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										void					*resource,
										PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
										const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const PopcornFX::CString			&resourcePath,
										bool					pathNotVirtual,
										PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
										const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	AppendDependencies(	hh_u32					resourceTypeID,
										const PopcornFX::CFilePackPath		&resourcePath,
										PopcornFX::TArray<PopcornFX::CString>			&outResourcePaths,
										const PopcornFX::CResourceManager	*resourceManager = null) const override;

	virtual void	BroadcastResourceChanged(const PopcornFX::CFilePackPath &resourcePath);



	bool			IsUsed(const PopcornFX::CString &virtualPath, bool pathNotVirtual) const;

	bool			RegisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual, UTexture *texture);
	bool			UnregisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual);

};

#endif

extern PopcornFX::CImage::EFormat	_UE2PKImageFormat(const EPixelFormat pixelFormat, bool srgb);
extern const TCHAR*					_My_GetPixelFormatString(EPixelFormat InPixelFormat);
