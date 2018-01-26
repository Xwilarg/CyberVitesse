//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXPlatform.h"

#include "Internal/ResourceHandlerImage_UE.h"

#include "Engine/Texture2D.h"

#include "PopcornFXSDK.h"
#include <hh_maths/include/hh_numeric_tools_int.h>

//----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY_STATIC(LogPopcornFXPlatformCommon, Log, All);

//----------------------------------------------------------------------------

PopcornFX::CImage		*_CreateFallbackImage()
{
	const PopcornFX::CUint3				size(16, 16, 1);
	const PopcornFX::CImage::EFormat	format = PopcornFX::CImage::Format_BGRA8;

	const hh_u32						bufferSize = PopcornFX::CImage::GetFormatPixelBufferSizeInBytes(format, size);
	PopcornFX::PRefCountedMemoryBuffer	dstBuffer = PopcornFX::CRefCountedMemoryBuffer::AllocAligned(bufferSize, 0x80);
	if (!HH_VERIFY(dstBuffer != null))
		return null;
	PopcornFX::Mem::Clear(dstBuffer->Data<hh_u8>(), dstBuffer->DataSizeInBytes());

	PopcornFX::CImage					*image = HH_NEW(PopcornFX::CImage);
	if (!HH_VERIFY(image != null))
		return null;
	image->m_Format = format;

	if (!HH_VERIFY(image->m_Frames.Resize(1)) ||
		!HH_VERIFY(image->m_Frames[0].m_Mipmaps.Resize(1)))
	{
		HH_SAFE_DELETE(image);
		return null;
	}

	image->m_Frames[0].m_Mipmaps[0] = PopcornFX::CImageMap(size, dstBuffer);
	return image;
}

//----------------------------------------------------------------------------

#if (PKFX_COMMON_NewImageFromTexture != 0)

PopcornFX::CImage	*PopcornFXPlatform_NewImageFromTexture(UTexture *texture)
{
#define	TEXTURE_ERROR_COMMONF(__msg, ...)	do {														\
		/*UE_LOG(LogPopcornFXPlatformCommon, Warning, __msg TEXT(": Texture 'LOD Group' must be 'ColorLookupTable': \"%s\""), ## __VA_ARGS__, *texture->GetPathName()); */\
		UE_LOG(LogPopcornFXPlatformCommon, Warning, TEXT("Could not load texture for sampling: ") __msg TEXT(": '%s'"), ## __VA_ARGS__, *texture->GetPathName()); \
	} while (0)

#define	TEXTURE_ERROR_COMMON(__msg)			TEXTURE_ERROR_COMMONF(TEXT("%s"), __msg)

#if PLATFORM_SWITCH
	TEXTURE_ERROR_COMMON(TEXT("Texture sampling not supported yet on Switch"));
	return _CreateFallbackImage();
#endif

	texture->ConditionalPostLoad();

	FTexturePlatformData	**platformDataPP = texture->GetRunningPlatformData();
	if (!/*HH_VERIFY*/(platformDataPP != null))
	{
		TEXTURE_ERROR_COMMON(TEXT("No PlatformData available"));
		return null;
	}

	FTexturePlatformData	*platformData = *platformDataPP;
	if (!/*HH_VERIFY*/(platformData != null))
	{
		TEXTURE_ERROR_COMMON(TEXT("Invalid PlatformData"));
		return null;
	}

#if WITH_EDITOR
	// Ugly workarround for cooking: create a fallback image.
	if (IsRunningCommandlet() && (platformData->Mips.Num() == 0 || !platformData->Mips[0].BulkData.IsBulkDataLoaded()))
	{
		//
		// During cooking, we cannot get a valid platformData because:
		// - either UTexture functions check for FApp::CanEverRender() (false while cooking)
		// - or we cannot call FTexturePlatformData functions because not exported
		// - or hacking PRIVATE_GIsRunningCommandlet makes UTexture::FinishCachePlatformData() check() fail
		//
		return _CreateFallbackImage();
	}
#endif

	if (!/*HH_VERIFY*/(platformData->Mips.Num() > 0))
	{
		TEXTURE_ERROR_COMMON(TEXT("No Mips in PlatformData"));
		return null;
	}

	/*const*/ FByteBulkData		&mipData = platformData->Mips[0].BulkData;

	const EPixelFormat			srcFormat = platformData->PixelFormat;
	const CUint3				imageSize(platformData->SizeX, platformData->SizeY, 1);
	const hh_u32				bulkDataSize = mipData.GetBulkDataSize();

	if (!/*HH_VERIFY*/(All(imageSize.xy() > 0)) ||
		!/*HH_VERIFY*/(bulkDataSize > 0))
	{
		TEXTURE_ERROR_COMMON(TEXT("Empty PlatformData Mips BulkData"));
		return null;
	}

	// This seems to have been fixed in UE 4.17
#if ((ENGINE_MINOR_VERSION >= 15) && (ENGINE_MINOR_VERSION < 17))
	TEXTURE_ERROR_COMMON(TEXT("Texture sampling disable on UE 4.15 because crashes, bug report has been submited. Fallbacking on black texture."));
	return _CreateFallbackImage();
#endif

#if 0 // Non-power-of-two texture does not seem to cause bugs nor crashes
	if (!PopcornFX::IntegerTools::IsPowerOfTwo(imageSize.xy()))
	{
		TEXTURE_ERROR_COMMONF(TEXT("Texture is not a power of two (%dx%d)"), imageSize.x(), imageSize.y());
		return null;
	}
#endif

	const PopcornFX::CImage::EFormat	dstFormat = _UE2PKImageFormat(srcFormat, false);
	if (dstFormat == PopcornFX::CImage::Format_Invalid)
	{
		TEXTURE_ERROR_COMMONF(TEXT("Format '%s' not supported"), _My_GetPixelFormatString(srcFormat));
		return null;
	}

	const hh_u32				expectedSizeInBytes = PopcornFX::CImage::GetFormatPixelBufferSizeInBytes(dstFormat, imageSize);
	if (!/*HH_VERIFY*/(bulkDataSize >= expectedSizeInBytes)) // don't mind if there is padding
	{
		TEXTURE_ERROR_COMMONF(TEXT("Mismatching texture size for format '%s' %dx%dx%d (got 0x%x bytes, expected 0x%x)"),
			_My_GetPixelFormatString(srcFormat),
			imageSize.x(), imageSize.y(), imageSize.z(),
			bulkDataSize, expectedSizeInBytes);
		return null;
	}

	enum { kAlignment = 0x80 };
	PopcornFX::PRefCountedMemoryBuffer	dstBuffer = PopcornFX::CRefCountedMemoryBuffer::AllocAligned(bulkDataSize + kAlignment, kAlignment);
	if (!HH_VERIFY(dstBuffer != null))
		return null;

	{
		// GetCopy crashes on 4.15 !
		void		*bulkDataCopyPtr = dstBuffer->Data<void>();
		mipData.GetCopy(&bulkDataCopyPtr, false);
		// no way to check if successful !?
		if (!HH_VERIFY(bulkDataCopyPtr == dstBuffer->Data<void>()))
		{
			TEXTURE_ERROR_COMMON(TEXT("Failed to get a copy of the texture"));
			return null;
		}
	}

	PopcornFX::CImage	*image = HH_NEW(PopcornFX::CImage);
	if (!HH_VERIFY(image != null) ||
		!HH_VERIFY(image->m_Frames.Resize(1)) ||
		!HH_VERIFY(image->m_Frames[0].m_Mipmaps.Resize(1)))
	{
		HH_DELETE(image);
		return null;
	}

	image->m_Format = dstFormat;

	PopcornFX::CImageMap	&dstMap = image->m_Frames[0].m_Mipmaps[0];
	dstMap.m_RawBuffer = dstBuffer;
	dstMap.m_Dimensions = imageSize;

#undef	TEXTURE_ERROR_COMMON
#undef	TEXTURE_ERROR_COMMONF

	return image;
}

#endif // (PKFX_COMMON_NewImageFromTexture != 0)

//----------------------------------------------------------------------------
