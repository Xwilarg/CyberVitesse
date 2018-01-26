//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "ResourceHandlerImage_UE.h"

#include "PopcornFXPlugin.h"
#include "Render/MyRHIStuff.h"
#include "Platforms/PopcornFXPlatform.h"

#include "Engine/Texture.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_refcounted_buffer.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXResourceHandlerImage, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXResourceHandlerImageGPU, Log, All);

//----------------------------------------------------------------------------
//
// static
//
//----------------------------------------------------------------------------

namespace
{
	CResourceHandlerImage_UE			*g_ResourceHandlerImage_UE = null;

#if (PK_GPU_D3D11 != 0)
	CResourceHandlerImage_UE_D3D11		*g_ResourceHandlerImage_UE_D3D11 = null;
#endif

}

//----------------------------------------------------------------------------

//static
void	CResourceHandlerImage_UE::Startup()
{
	HH_ASSERT(g_ResourceHandlerImage_UE == null);
	g_ResourceHandlerImage_UE = HH_NEW(CResourceHandlerImage_UE);
	if (!HH_VERIFY(g_ResourceHandlerImage_UE != null))
		return;
	PopcornFX::Resource::DefaultManager()->RegisterHandler<PopcornFX::CImage>(g_ResourceHandlerImage_UE);

#if (PK_GPU_D3D11 != 0)
	HH_ASSERT(g_ResourceHandlerImage_UE_D3D11 == null);
	g_ResourceHandlerImage_UE_D3D11 = HH_NEW(CResourceHandlerImage_UE_D3D11);
	if (!HH_VERIFY(g_ResourceHandlerImage_UE_D3D11 != null))
		return;
	PopcornFX::Resource::DefaultManager()->RegisterHandler<PopcornFX::CImageGPU_D3D11>(g_ResourceHandlerImage_UE_D3D11);
#endif

}

//----------------------------------------------------------------------------

//static
void	CResourceHandlerImage_UE::Shutdown()
{
	if (HH_VERIFY(g_ResourceHandlerImage_UE != null))
	{
		PopcornFX::Resource::DefaultManager()->UnregisterHandler<PopcornFX::CImage>(g_ResourceHandlerImage_UE);
		HH_DELETE(g_ResourceHandlerImage_UE);
		g_ResourceHandlerImage_UE = null;
	}

#if (PK_GPU_D3D11 != 0)
	if (HH_VERIFY(g_ResourceHandlerImage_UE_D3D11 != null))
	{
		PopcornFX::Resource::DefaultManager()->UnregisterHandler<PopcornFX::CImageGPU_D3D11>(g_ResourceHandlerImage_UE_D3D11);
		HH_DELETE(g_ResourceHandlerImage_UE_D3D11);
		g_ResourceHandlerImage_UE_D3D11 = null;
	}
#endif

}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

#define _MY_CASE_ENUM_TO_TEXT(txt) case txt: return TEXT(#txt);
const TCHAR*	 _My_GetPixelFormatString(EPixelFormat InPixelFormat)
{
	switch (InPixelFormat)
	{
		FOREACH_ENUM_EPIXELFORMAT(_MY_CASE_ENUM_TO_TEXT)
	default:
		return TEXT("PF_Unknown");
	}
}

#define _MY_CASE_ENUM_TO_ANSI(txt) case txt: return #txt;
const char*	 _My_GetPixelFormatStringANSI(EPixelFormat InPixelFormat)
{
	switch (InPixelFormat)
	{
		FOREACH_ENUM_EPIXELFORMAT(_MY_CASE_ENUM_TO_ANSI)
	default:
		return "PF_Unknown";
	}
}

PopcornFX::CImage::EFormat	_UE2PKImageFormat(const EPixelFormat pixelFormat, bool srgb)
{
#define		UE2PK_IMAGE(__ue, __pk) case __ue: return PopcornFX::CImage:: __pk
#define		UE2PK_IMAGE_SRGB(__ue, __pk) case __ue: return (srgb ? (PopcornFX::CImage:: __pk ## _sRGB) : (PopcornFX::CImage:: __pk))
	switch (pixelFormat)
	{
		UE2PK_IMAGE(PF_A32B32G32R32F		, Format_Fp32RGBA);
		UE2PK_IMAGE_SRGB(PF_B8G8R8A8		, Format_BGRA8);
		UE2PK_IMAGE(PF_G8					, Format_Lum8);
//		UE2PK_IMAGE(PF_G16					, );
		UE2PK_IMAGE_SRGB(PF_DXT1			, Format_DXT1);
		UE2PK_IMAGE_SRGB(PF_DXT3			, Format_DXT3);
		UE2PK_IMAGE_SRGB(PF_DXT5			, Format_DXT5);
//		UE2PK_IMAGE(PF_UYVY					, );
//		UE2PK_IMAGE(PF_FloatRGB				, );
		UE2PK_IMAGE(PF_FloatRGBA			, Format_Fp32RGBA);
//		UE2PK_IMAGE(PF_DepthStencil			, );
//		UE2PK_IMAGE(PF_ShadowDepth			, );
		UE2PK_IMAGE(PF_R32_FLOAT			, Format_Fp32Lum);
//		UE2PK_IMAGE(PF_G16R16				, );
		UE2PK_IMAGE(PF_G16R16F				, Format_Fp16LumAlpha);
//		UE2PK_IMAGE(PF_G16R16F_FILTER		, );
		UE2PK_IMAGE(PF_G32R32F				, Format_Fp32LumAlpha);
//		UE2PK_IMAGE(PF_A2B10G10R10			, );
//		UE2PK_IMAGE(PF_A16B16G16R16			, );
//		UE2PK_IMAGE(PF_D24					, );
		UE2PK_IMAGE(PF_R16F					, Format_Fp16Lum);
//		UE2PK_IMAGE(PF_R16F_FILTER			, );
		UE2PK_IMAGE(PF_BC5					, Format_BC5_UNorm); // hint: Windows/D3D11RHI/Private/D3D11Device.cpp:148
//		UE2PK_IMAGE(PF_V8U8					, );
//		UE2PK_IMAGE(PF_A1					, );
//		UE2PK_IMAGE(PF_FloatR11G11B10		, );
//		UE2PK_IMAGE(PF_A8					, );
		UE2PK_IMAGE(PF_R32_UINT				, Format_Lum8);
//		UE2PK_IMAGE(PF_R32_SINT				, );
//		UE2PK_IMAGE(PF_PVRTC2				, );
//		UE2PK_IMAGE(PF_PVRTC4				, );
//		UE2PK_IMAGE(PF_R16_UINT				, );
//		UE2PK_IMAGE(PF_R16_SINT				, );
//		UE2PK_IMAGE(PF_R16G16B16A16_UINT	, );
//		UE2PK_IMAGE(PF_R16G16B16A16_SINT	, );
//		UE2PK_IMAGE(PF_R5G6B5_UNORM			, );
		UE2PK_IMAGE_SRGB(PF_R8G8B8A8		, Format_BGRA8);
		UE2PK_IMAGE_SRGB(PF_A8R8G8B8		, Format_BGRA8);
//		UE2PK_IMAGE(PF_BC4					, );
//		UE2PK_IMAGE(PF_R8G8					, );
//		UE2PK_IMAGE(PF_ATC_RGB				, );
//		UE2PK_IMAGE(PF_ATC_RGBA_E			, );
//		UE2PK_IMAGE(PF_ATC_RGBA_I			, );
//		UE2PK_IMAGE(PF_X24_G8				, );
//		UE2PK_IMAGE(PF_ETC1					, );
//		UE2PK_IMAGE(PF_ETC2_RGB				, );
//		UE2PK_IMAGE(PF_ETC2_RGBA			, );
//		UE2PK_IMAGE(PF_R32G32B32A32_UINT	, );
//		UE2PK_IMAGE(PF_R16G16_UINT			, );
//		UE2PK_IMAGE(PF_ASTC_4x4				, );
//		UE2PK_IMAGE(PF_ASTC_6x6				, );
//		UE2PK_IMAGE(PF_ASTC_8x8				, );
//		UE2PK_IMAGE(PF_ASTC_10x10			, );
//		UE2PK_IMAGE(PF_ASTC_12x12			, );
//		UE2PK_IMAGE(PF_BC6H					, );
//		UE2PK_IMAGE(PF_BC7					, );
//		UE2PK_IMAGE(PF_MAX					, );
		default:
			break;
	}
	HH_ASSERT_NOT_REACHED_MESSAGE("UE EPixelFormat '%s' not recognized by PopcornFX", _My_GetPixelFormatStringANSI(pixelFormat));
#undef UE2PK_IMAGE
	return PopcornFX::CImage::Format_Invalid;
}

//static
static PopcornFX::CImage		*_NewFromTexture(UTexture *texture)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::_NewFromTexture", POPCORNFX_UE_PROFILER_COLOR);

	return PopcornFXPlatform_NewImageFromTexture(texture);
}

//----------------------------------------------------------------------------

PopcornFX::CImage		*CResourceHandlerImage_UE::NewFromTexture(UTexture *texture)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::NewFromTexture", POPCORNFX_UE_PROFILER_COLOR);

	PopcornFX::CImage		*image = _NewFromTexture(texture);
	if (image == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImage, Warning, TEXT("Failed converting to PopcornFX image \"%s\""), *texture->GetPathName());
	}
	return image;
}

//----------------------------------------------------------------------------

PopcornFX::CImage	*CResourceHandlerImage_UE::NewFromPath(const PopcornFX::CString &path, bool pathNotVirtual)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::NewFromPath", POPCORNFX_UE_PROFILER_COLOR);

	UObject			*obj = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(path.Data(), pathNotVirtual);
	if (obj == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImage, Warning, TEXT("UObject not found \"%s\" %d"), ANSI_TO_TCHAR(path.Data()), pathNotVirtual);
		return null;
	}
	UTexture		*texture = Cast<UTexture>(obj);
	if (!HH_VERIFY(texture != null))
	{
		UE_LOG(LogPopcornFXResourceHandlerImage, Warning, TEXT("UObject is not a UTexture \"%s\""), *obj->GetPathName());
		return null;
	}
	PopcornFX::CImage	*image = _NewFromTexture(texture);
	if (!HH_VERIFY(texture != null))
	{
		UE_LOG(LogPopcornFXResourceHandlerImage, Warning, TEXT("Failed to create texture for PopcornFX \"%s\""), *texture->GetPathName());
		return null;
	}
	return image;
}

//----------------------------------------------------------------------------

CResourceHandlerImage_UE::CResourceHandlerImage_UE()
{
}

//----------------------------------------------------------------------------

CResourceHandlerImage_UE::~CResourceHandlerImage_UE()
{
	for (CResourcesHashMap::ConstIterator it = m_Images.Begin(), itEnd = m_Images.End(); it != itEnd; ++it)
	{
		HH_ASSERT(it->m_ReferenceCount > 0);
		UE_LOG(LogPopcornFXResourceHandlerImage, Warning, TEXT("Texture leak: \"%s\" %s"),
			ANSI_TO_TCHAR(it.Key().Data()),
			it->m_Virtual ? TEXT("(Virtual Texture)") : TEXT(""));
	}
}

//----------------------------------------------------------------------------


void	*CResourceHandlerImage_UE::Load(
	hh_u32					resourceTypeID,
	const PopcornFX::CString	&resourcePath,
	bool					pathNotVirtual,
	bool					reload,
	SAsyncLoadStatus		*asyncLoadStatus,
	const PopcornFX::CResourceManager	*resourceManager)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::Load", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImage>::ResourceTypeID());

	PopcornFX::CString			_fullPath = resourcePath;
	if (!pathNotVirtual) // if virtual path
	{
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	}
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
	{
		if (asyncLoadStatus != null)
		{
			asyncLoadStatus->m_Resource = null;
			asyncLoadStatus->m_Done = true;
			asyncLoadStatus->m_Progress = 1.0f;
		}
		return null;
	}

	if (!reload)
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*existingEntry = m_Images.Find(fullPath);
		if (existingEntry != null)
		{
			if (asyncLoadStatus != null)
			{
				asyncLoadStatus->m_Resource = null;
				asyncLoadStatus->m_Done = true;
				asyncLoadStatus->m_Progress = 1.0f;
			}
			++existingEntry->m_ReferenceCount;
			return existingEntry->m_Image.Get();
		}
	}

	PopcornFX::PImage	resource = NewFromPath(fullPath, true);

	if (resource != null)
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry == null)
		{
			entry = m_Images.Insert(fullPath, SResourceEntry(resource, 1));
			if (!HH_VERIFY(entry != null))
				resource = null;
		}
		else
		{
			++entry->m_ReferenceCount;

			HH_ASSERT(entry->m_Image != null);

			entry->m_Image->m_OnReloading(entry->m_Image.Get());

			PopcornFX::HHSwap(entry->m_Image->m_Flags, resource->m_Flags);
			PopcornFX::HHSwap(entry->m_Image->m_Format, resource->m_Format);
			PopcornFX::HHSwap(entry->m_Image->m_Frames, resource->m_Frames);

			entry->m_Image->m_OnReloaded(entry->m_Image.Get());

			resource = entry->m_Image;
		}
	}

	if (asyncLoadStatus != null)
	{
		asyncLoadStatus->m_Resource = resource.Get();
		asyncLoadStatus->m_Done = true;
		asyncLoadStatus->m_Progress = 1.0f;
	}

	return resource.Get();
}

//----------------------------------------------------------------------------

void	*CResourceHandlerImage_UE::Load(
	hh_u32					resourceTypeID,
	const PopcornFX::CFilePackPath	&resourcePath,
	bool					reload,
	SAsyncLoadStatus		*asyncLoadStatus,
	const PopcornFX::CResourceManager	*resourceManager)
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImage>::ResourceTypeID());

	return Load(resourceTypeID, resourcePath.Path(), false, reload, asyncLoadStatus, resourceManager);
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE::Unload(
	hh_u32					resourceTypeID,	// used to check we are called with the correct type
	void					*rawResource,
	const PopcornFX::CResourceManager	*resourceManager)
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImage>::ResourceTypeID());

	PopcornFX::PImage	resource = static_cast<PopcornFX::CImage*>(rawResource);

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		PopcornFX::THashMap<SResourceEntry, PopcornFX::CString>::Iterator	it = m_Images.Begin();
		PopcornFX::THashMap<SResourceEntry, PopcornFX::CString>::Iterator	end = m_Images.End();

		hh_u32	refsLeft = 0;
		PopcornFX::CString	fullPath;
		while (it != end)
		{
			if (it->m_Image == resource)
			{
				fullPath = it.Key();
				refsLeft = --(it->m_ReferenceCount);
				break;
			}
			++it;
		}

		if (!fullPath.Empty() && refsLeft == 0)
		{
			m_Images.Remove(fullPath);
		}
	}
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE::AppendDependencies(
	hh_u32 resourceTypeID,
	void *resource,
	PopcornFX::TArray<PopcornFX::CString> &outResourcePaths,
	const PopcornFX::CResourceManager *resourceManager /*= null*/) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImage>::ResourceTypeID());
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE::AppendDependencies(
	hh_u32 resourceTypeID,
	const PopcornFX::CString &resourcePath,
	bool pathNotVirtual,
	PopcornFX::TArray<PopcornFX::CString> &outResourcePaths,
	const PopcornFX::CResourceManager *resourceManager /*= null*/) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImage>::ResourceTypeID());
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE::AppendDependencies(
	hh_u32 resourceTypeID,
	const PopcornFX::CFilePackPath &resourcePath,
	PopcornFX::TArray<PopcornFX::CString> &outResourcePaths,
	const PopcornFX::CResourceManager *resourceManager /*= null*/) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImage>::ResourceTypeID());
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE::BroadcastResourceChanged(const PopcornFX::CFilePackPath &resourcePath)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::BroadcastResourceChanged", POPCORNFX_UE_PROFILER_COLOR);

	const PopcornFX::CString		fullPath = PopcornFX::CFilePath::StripExtension(resourcePath.FullPath());

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource == null || !HH_VERIFY(foundResource->m_Image != null))
			return;
	}

	PopcornFX::PImage	resource = NewFromPath(fullPath, true);

	if (resource != null)
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource != null)	// could have been Unload-ed in the meantime
		{
			// replace the entry in the resource-list
			HH_ASSERT(foundResource->m_Image != null);
			PopcornFX::CImage		*dstImage = foundResource->m_Image.Get();

			dstImage->m_OnReloading(dstImage);
			PopcornFX::HHSwap(dstImage->m_Flags, resource->m_Flags);
			PopcornFX::HHSwap(dstImage->m_Format, resource->m_Format);
			PopcornFX::HHSwap(dstImage->m_Frames, resource->m_Frames);
			dstImage->m_OnReloaded(dstImage);
		}
	}
}

//----------------------------------------------------------------------------

bool	CResourceHandlerImage_UE::IsUsed(const PopcornFX::CString &virtualPath, bool pathNotVirtual) const
{
	PopcornFX::CString			_fullPath = virtualPath;
	if (!pathNotVirtual) // if virtual path
	{
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	}
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
		return false;
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		const SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry == null)
			return false;
		HH_ASSERT(entry->m_ReferenceCount > 0);
		if (entry->m_Virtual) // Register Virtual inc m_ReferenceCount once
			return entry->m_ReferenceCount > 1;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	CResourceHandlerImage_UE::RegisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual, PopcornFX::CImage::EFormat format, const CUint2 &size2d, const void *pixelsData, hh_u32 pixelsDataSizeInBytes)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::RegisterVirtualTexture", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(FPopcornFXPlugin::IsMainThread()); // no thread-safe !!

	PopcornFX::CString			_fullPath = virtualPath;
	if (!pathNotVirtual) // if virtual path
	{
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	}
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
		return false;

	HH_ASSERT(pixelsData != null);
	HH_ASSERT(pixelsDataSizeInBytes > 0);
	const CUint3			size = CUint3(size2d, 1);
	const hh_u32			totalImageBytes = PopcornFX::CImage::GetFormatPixelBufferSizeInBytes(format, size);
	if (!HH_VERIFY(pixelsDataSizeInBytes >= totalImageBytes))
	{
		UE_LOG(LogPopcornFXResourceHandlerImage, Error, TEXT("Invalid image byte size for virtual texture \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
		return false;
	}

	PopcornFX::CImage			&image = m_VirtImageCache;

	image.m_Format = format;

	if (!HH_VERIFY(image.m_Frames.Resize(1)))
		return false;
	if (!HH_VERIFY(image.m_Frames[0].m_Mipmaps.Resize(1)))
		return false;
	PopcornFX::CImageMap		&map = image.m_Frames[0].m_Mipmaps[0];

	map.m_Dimensions = size;

	PopcornFX::PRefCountedMemoryBuffer		cacheLastVirtual;
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry != null && entry->m_Virtual &&
			entry->m_CacheLastVirtual != null)
		{
			HHSwap(entry->m_CacheLastVirtual, cacheLastVirtual);
		}
	}
	if (cacheLastVirtual != null && cacheLastVirtual->DataSizeInBytes() >= totalImageBytes)
		map.m_RawBuffer = cacheLastVirtual;
	else
		map.m_RawBuffer = PopcornFX::CRefCountedMemoryBuffer::AllocAligned(totalImageBytes);
	cacheLastVirtual = null;
	
	if (!HH_VERIFY(map.m_RawBuffer != null))
		return false;

	{
		HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::RegisterVirtualTexture [Copy Pixels]", POPCORNFX_UE_PROFILER_COLOR);
		PopcornFX::Mem::Copy(map.m_RawBuffer->Data<void>(), pixelsData, totalImageBytes);
	}

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry == null)
		{
			PopcornFX::CImage	*resource = HH_NEW(PopcornFX::CImage);
			if (!HH_VERIFY(resource != null))
				return false;
			entry = m_Images.Insert(fullPath, SResourceEntry(resource, 1, true));
			HH_ASSERT(entry != null);
		}
		else
		{
			HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::RegisterVirtualTexture [Notify]", POPCORNFX_UE_PROFILER_COLOR);

			const bool		wasVirtual = entry->m_Virtual;
			if (!wasVirtual)
			{
				// Inc only if the asset was not virtual
				++entry->m_ReferenceCount;
				entry->m_Virtual = true;
			}
			HH_ASSERT(entry->m_Virtual);
			HH_ASSERT(entry->m_ReferenceCount > 0);

			HH_ASSERT(entry->m_Image != null);

			// cache the buffer for next load
			if (wasVirtual && !entry->m_Image->m_Frames.Empty() && !entry->m_Image->m_Frames[0].m_Mipmaps.Empty())
				entry->m_CacheLastVirtual = entry->m_Image->m_Frames[0].m_Mipmaps[0].m_RawBuffer;

			entry->m_Image->m_OnReloading(entry->m_Image.Get());

			PopcornFX::HHSwap(entry->m_Image->m_Flags, image.m_Flags);
			PopcornFX::HHSwap(entry->m_Image->m_Format, image.m_Format);
			PopcornFX::HHSwap(entry->m_Image->m_Frames, image.m_Frames);

			entry->m_Image->m_OnReloaded(entry->m_Image.Get());
		}
	}

	// do not do that: map actually reference the current buffer, because HHSwap
	// map.m_RawBuffer = null;

	return true;
}

//----------------------------------------------------------------------------

bool	CResourceHandlerImage_UE::UnregisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE::UnregisterVirtualTexture", POPCORNFX_UE_PROFILER_COLOR);

	PopcornFX::CString			_fullPath = virtualPath;
	if (!pathNotVirtual) // if virtual path
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
		return false;

	bool		loadTrueAsset = false;

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource == null)
		{
			UE_LOG(LogPopcornFXResourceHandlerImage, Error, TEXT("Virtual Texture not found to Unregister \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
			return false;
		}
		if (!HH_VERIFY(foundResource->m_Virtual))
		{
			UE_LOG(LogPopcornFXResourceHandlerImage, Error, TEXT("Texture is not a Virtual Texture to Unregister \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
			return false;
		}
		HH_ASSERT(foundResource->m_Image != null);
		HH_ASSERT(foundResource->m_ReferenceCount > 0);
		if (foundResource->m_ReferenceCount > 1) // stuff refences it
			loadTrueAsset = true;
	}

	PopcornFX::PImage	resource;
	
	if (loadTrueAsset)
		resource = NewFromPath(fullPath, true);

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource == null || !foundResource->m_Virtual)	// could have been Unload-ed in the meantime
			return false;

		--foundResource->m_ReferenceCount;
		if (foundResource->m_ReferenceCount == 0)
		{
			// LOG unloading virtual
			m_Images.Remove(fullPath);
		}
		else
		{
			foundResource->m_Virtual = false;
			foundResource->m_CacheLastVirtual = null;

			if (resource == null && foundResource->m_ReferenceCount > 1) // could have be Load in the meantime
				resource = NewFromPath(fullPath, true);

			if (resource == null)
			{
				UE_LOG(LogPopcornFXResourceHandlerImage, Error, TEXT("Could not load true asset to replace Unregistered Virtual Texture, keeping old datas \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
				return false;
			}
			HH_ASSERT(foundResource->m_Image != null);

			PopcornFX::CImage		*dstImage = foundResource->m_Image.Get();

			dstImage->m_OnReloading(dstImage);
			if (resource != null)
			{
				PopcornFX::HHSwap(dstImage->m_Flags, resource->m_Flags);
				PopcornFX::HHSwap(dstImage->m_Format, resource->m_Format);
				PopcornFX::HHSwap(dstImage->m_Frames, resource->m_Frames);
			}
			else
			{
				dstImage->Clear();
			}
			dstImage->m_OnReloaded(dstImage);
		}
	}

	return true;
}

//----------------------------------------------------------------------------
//
//	CResourceHandlerImage_UE_D3D11
//
//----------------------------------------------------------------------------
#if (PK_GPU_D3D11 != 0)

//----------------------------------------------------------------------------

//static
PopcornFX::CImageGPU_D3D11		*CResourceHandlerImage_UE_D3D11::NewFromTexture(UTexture *texture)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::_NewFromTexture", POPCORNFX_UE_PROFILER_COLOR);

	FTextureReferenceRHIRef		texRef = texture->TextureReference.TextureReferenceRHI;
	if (!IsValidRef(texRef))
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UTexture TextureReference not available \"%s\""), *texture->GetPathName());
		HH_ASSERT_NOT_REACHED();
		return null;
	}
	FRHITexture					*texRHI = texRef->GetReferencedTexture();
	if (texRHI == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UTexture TextureReference FRHITexture not available \"%s\""), *texture->GetPathName());
		HH_ASSERT_NOT_REACHED();
		return null;
	}
	if (texRHI->GetTexture2D() == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UTexture TextureReference FRHITexture is not a Texture2D \"%s\""), *texture->GetPathName());
		HH_ASSERT_NOT_REACHED();
		return null;
	}
	ID3D11Texture2D				*gpuTexture = static_cast<ID3D11Texture2D*>(texRHI->GetNativeResource());
	ID3D11ShaderResourceView	*gpuTextureSRV = static_cast<ID3D11ShaderResourceView*>(texRHI->GetNativeShaderResourceView());
	if (gpuTexture == null || gpuTextureSRV == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UTexture TextureReference FRHITexture D3D11 not available \"%s\""), *texture->GetPathName());
		HH_ASSERT_NOT_REACHED();
		return null;
	}

	HH_TODO("Find the true channel count !");
	const hh_u32			channelCount = 4;

	PopcornFX::CImageGPU_D3D11		*image = HH_NEW(PopcornFX::CImageGPU_D3D11());
	if (!HH_VERIFY(image != null) ||
		!image->Setup(gpuTexture, gpuTextureSRV, channelCount))
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UTexture TextureReference final Setup failed \"%s\""), *texture->GetPathName());
		HH_ASSERT_NOT_REACHED();
		HH_SAFE_DELETE(image);
		return null;
	}

	return image;
}

//----------------------------------------------------------------------------

// static
PopcornFX::CImageGPU_D3D11		*CResourceHandlerImage_UE_D3D11::NewFromPath(const PopcornFX::CString &path, bool pathNotVirtual)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::NewFromPath", POPCORNFX_UE_PROFILER_COLOR);

	UObject			*obj = FPopcornFXPlugin::Get().LoadUObjectFromPkPath(path.Data(), pathNotVirtual);
	if (obj == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UObject not found \"%s\" %d"), ANSI_TO_TCHAR(path.Data()), pathNotVirtual);
		return null;
	}
	UTexture		*texture = Cast<UTexture>(obj);
	if (!HH_VERIFY(texture != null))
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("UObject is not a UTexture \"%s\""), *obj->GetPathName());
		return null;
	}
	PopcornFX::CImageGPU_D3D11		*image = NewFromTexture(texture);
	if (image == null)
	{
		UE_LOG(LogPopcornFXResourceHandlerImageGPU, Warning, TEXT("Failed creating texture for PopcornFX \"%s\""), *texture->GetPathName());
		return null;
	}
	return image;
}

//----------------------------------------------------------------------------

CResourceHandlerImage_UE_D3D11::CResourceHandlerImage_UE_D3D11()
{
}

//----------------------------------------------------------------------------

CResourceHandlerImage_UE_D3D11::~CResourceHandlerImage_UE_D3D11()
{
}

//----------------------------------------------------------------------------


void	*CResourceHandlerImage_UE_D3D11::Load(
	hh_u32					resourceTypeID,
	const PopcornFX::CString	&resourcePath,
	bool					pathNotVirtual,
	bool					reload,
	SAsyncLoadStatus		*asyncLoadStatus,
	const PopcornFX::CResourceManager	*resourceManager)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::Load", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImageGPU_D3D11>::ResourceTypeID());

	PopcornFX::CString			_fullPath = resourcePath;
	if (!pathNotVirtual) // if virtual path
	{
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	}
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
	{
		if (asyncLoadStatus != null)
		{
			asyncLoadStatus->m_Resource = null;
			asyncLoadStatus->m_Done = true;
			asyncLoadStatus->m_Progress = 1.0f;
		}
		return null;
	}

	if (!reload)
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*existingEntry = m_Images.Find(fullPath);
		if (existingEntry != null)
		{
			if (asyncLoadStatus != null)
			{
				asyncLoadStatus->m_Resource = null;
				asyncLoadStatus->m_Done = true;
				asyncLoadStatus->m_Progress = 1.0f;
			}
			++existingEntry->m_ReferenceCount;
			return existingEntry->m_Image.Get();
		}
	}

	PopcornFX::PImageGPU_D3D11	resource = NewFromPath(fullPath, true);

	if (resource != null)
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry == null)
		{
			entry = m_Images.Insert(fullPath, SResourceEntry(resource, 1));
			if (!HH_VERIFY(entry != null))
				resource = null;
		}
		else
		{
			++entry->m_ReferenceCount;

			HH_ASSERT(entry->m_Image != null);

			entry->m_Image->m_OnReloading(entry->m_Image.Get());

			entry->m_Image->Swap(*resource);

			entry->m_Image->m_OnReloaded(entry->m_Image.Get());

			resource = entry->m_Image;
		}
	}

	if (asyncLoadStatus != null)
	{
		asyncLoadStatus->m_Resource = resource.Get();
		asyncLoadStatus->m_Done = true;
		asyncLoadStatus->m_Progress = 1.0f;
	}

	return resource.Get();
}

//----------------------------------------------------------------------------

void	*CResourceHandlerImage_UE_D3D11::Load(
	hh_u32					resourceTypeID,
	const PopcornFX::CFilePackPath	&resourcePath,
	bool					reload,
	SAsyncLoadStatus		*asyncLoadStatus,
	const PopcornFX::CResourceManager	*resourceManager)
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImageGPU_D3D11>::ResourceTypeID());

	return Load(resourceTypeID, resourcePath.FullPath(), true, reload, asyncLoadStatus, resourceManager);
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE_D3D11::Unload(
	hh_u32					resourceTypeID,	// used to check we are called with the correct type
	void					*rawResource,
	const PopcornFX::CResourceManager	*resourceManager)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::Unload", POPCORNFX_UE_PROFILER_COLOR);

	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImageGPU_D3D11>::ResourceTypeID());

	PopcornFX::PImageGPU_D3D11	resource = static_cast<PopcornFX::CImageGPU_D3D11*>(rawResource);

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		PopcornFX::THashMap<SResourceEntry, PopcornFX::CString>::Iterator	it = m_Images.Begin();
		PopcornFX::THashMap<SResourceEntry, PopcornFX::CString>::Iterator	end = m_Images.End();

		hh_u32	refsLeft = 0;
		PopcornFX::CString	fullPath;
		while (it != end)
		{
			if (it->m_Image == resource)
			{
				fullPath = it.Key();
				refsLeft = --(it->m_ReferenceCount);
				break;
			}
			++it;
		}

		if (!fullPath.Empty() && refsLeft == 0)
		{
			m_Images.Remove(fullPath);
		}
	}
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE_D3D11::AppendDependencies(
	hh_u32 resourceTypeID,
	void *resource,
	PopcornFX::TArray<PopcornFX::CString> &outResourcePaths,
	const PopcornFX::CResourceManager *resourceManager /*= null*/) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImageGPU_D3D11>::ResourceTypeID());
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE_D3D11::AppendDependencies(
	hh_u32 resourceTypeID,
	const PopcornFX::CString &resourcePath,
	bool pathNotVirtual,
	PopcornFX::TArray<PopcornFX::CString> &outResourcePaths,
	const PopcornFX::CResourceManager *resourceManager /*= null*/) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImageGPU_D3D11>::ResourceTypeID());
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE_D3D11::AppendDependencies(
	hh_u32 resourceTypeID,
	const PopcornFX::CFilePackPath &resourcePath,
	PopcornFX::TArray<PopcornFX::CString> &outResourcePaths,
	const PopcornFX::CResourceManager *resourceManager /*= null*/) const
{
	HH_ASSERT(resourceTypeID == PopcornFX::TResourceRouter<PopcornFX::CImageGPU_D3D11>::ResourceTypeID());
}

//----------------------------------------------------------------------------

void	CResourceHandlerImage_UE_D3D11::BroadcastResourceChanged(const PopcornFX::CFilePackPath &resourcePath)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::BroadcastResourceChanged", POPCORNFX_UE_PROFILER_COLOR);

	const PopcornFX::CString		fullPath = PopcornFX::CFilePath::StripExtensionNew(resourcePath.FullPath());

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource == null || !HH_VERIFY(foundResource->m_Image != null))
			return;
	}

	PopcornFX::PImageGPU_D3D11	resource = NewFromPath(fullPath, true);

	if (resource != null)
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource != null)	// could have been Unload-ed in the meantime
		{
			// replace the entry in the resource-list
			HH_ASSERT(foundResource->m_Image != null);
			PopcornFX::CImageGPU_D3D11		*dstImage = foundResource->m_Image.Get();

			dstImage->m_OnReloading(dstImage);
			dstImage->Swap(*resource);
			dstImage->m_OnReloaded(dstImage);
		}
	}
}

//----------------------------------------------------------------------------

bool	CResourceHandlerImage_UE_D3D11::IsUsed(const PopcornFX::CString &virtualPath, bool pathNotVirtual) const
{
	PopcornFX::CString			_fullPath = virtualPath;
	if (!pathNotVirtual) // if virtual path
	{
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	}
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
		return false;
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		const SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry == null)
			return false;
		HH_ASSERT(entry->m_ReferenceCount > 0);
		if (entry->m_Virtual) // Register Virtual inc m_ReferenceCount once
			return entry->m_ReferenceCount > 1;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------

bool	CResourceHandlerImage_UE_D3D11::RegisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual, UTexture *texture)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::RegisterVirtualTexture", POPCORNFX_UE_PROFILER_COLOR);

	PopcornFX::CString			_fullPath = virtualPath;
	if (!pathNotVirtual) // if virtual path
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
		return false;

	PopcornFX::PImageGPU_D3D11		resource = NewFromTexture(texture);
	if (resource == null)
		return false;

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry		*entry = m_Images.Find(fullPath);
		if (entry == null)
		{
			entry = m_Images.Insert(fullPath, SResourceEntry(resource, 1, true));
			if (!HH_VERIFY(entry != null))
				resource = null;
		}
		else
		{
			const bool		wasVirtual = entry->m_Virtual;
			if (!wasVirtual)
			{
				// Inc only if the asset was not virtual
				++entry->m_ReferenceCount;
				entry->m_Virtual = true;
			}
			HH_ASSERT(entry->m_Virtual);
			HH_ASSERT(entry->m_ReferenceCount > 0);

			HH_ASSERT(entry->m_Image != null);

			entry->m_Image->m_OnReloading(entry->m_Image.Get());
			entry->m_Image->Swap(*resource);
			entry->m_Image->m_OnReloaded(entry->m_Image.Get());
		}
	}

	return true;
}

//----------------------------------------------------------------------------

bool	CResourceHandlerImage_UE_D3D11::UnregisterVirtualTexture(const PopcornFX::CString &virtualPath, bool pathNotVirtual)
{
	HH_NAMEDSCOPEDPROFILE_C("CResourceHandlerImage_UE_D3D11::UnregisterVirtualTexture", POPCORNFX_UE_PROFILER_COLOR);

	PopcornFX::CString			_fullPath = virtualPath;
	if (!pathNotVirtual) // if virtual path
		_fullPath = TCHAR_TO_ANSI(*FPopcornFXPlugin::Get().BuildPathFromPkPath(_fullPath.Data(), true)); // prepend Pack Path
	PopcornFX::CFilePath::StripExtensionInPlace(_fullPath);
	const PopcornFX::CString	&fullPath = _fullPath;
	if (!/*HH_VERIFY*/(!fullPath.Empty()))
		return false;

	bool		loadTrueAsset = false;
	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
		SResourceEntry	*foundResource = m_Images.Find(fullPath);
		if (foundResource == null)
		{
			UE_LOG(LogPopcornFXResourceHandlerImageGPU, Error, TEXT("Virtual Texture GPU not found to Unregister \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
			return false;
		}
		if (!HH_VERIFY(foundResource->m_Virtual))
		{
			UE_LOG(LogPopcornFXResourceHandlerImageGPU, Error, TEXT("Texture GPU is not a Virtual Texture to Unregister \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
			return false;
		}
		HH_ASSERT(foundResource->m_Image != null);
		HH_ASSERT(foundResource->m_ReferenceCount > 0);
		if (foundResource->m_ReferenceCount > 1) // stuff refences it
			loadTrueAsset = true;
	}

	PopcornFX::PImageGPU_D3D11		resource;
	if (loadTrueAsset)
		resource = NewFromPath(fullPath, true);

	{
		HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);

		SResourceEntry	*foundResource = m_Images.Find(fullPath);

		if (foundResource == null || !foundResource->m_Virtual)
			return false;

		--foundResource->m_ReferenceCount;
		if (foundResource->m_ReferenceCount == 0)
		{
			// LOG unloading virtual
			m_Images.Remove(fullPath);
		}
		else
		{
			foundResource->m_Virtual = false;
			//foundResource->m_CacheLastVirtual = null;

			if (resource == null && foundResource->m_ReferenceCount > 1) // could have be Load in the meantime
				resource = NewFromPath(fullPath, true);

			if (resource == null)
			{
				UE_LOG(LogPopcornFXResourceHandlerImage, Error, TEXT("Could not load true asset to replace Unregistered Virtual Texture, keeping old datas \"%s\""), ANSI_TO_TCHAR(virtualPath.Data()));
				return false;
			}
			HH_ASSERT(foundResource->m_Image != null);

			PopcornFX::CImageGPU_D3D11		*dstImage = foundResource->m_Image.Get();

			dstImage->m_OnReloading(dstImage);
			if (resource != null)
			{
				dstImage->Swap(*resource);
			}
			else
			{
				dstImage->Clear();
			}
			dstImage->m_OnReloaded(dstImage);
		}
	}

	return true;
}

//----------------------------------------------------------------------------

#endif // (PK_GPU_D3D11 != 0)

//----------------------------------------------------------------------------
