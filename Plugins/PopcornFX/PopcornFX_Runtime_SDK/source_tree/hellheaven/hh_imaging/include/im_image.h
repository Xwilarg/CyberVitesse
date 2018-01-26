#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/01 07:34 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __IM_IMAGE_H__
#define __IM_IMAGE_H__

#include <hh_kernel/include/kr_callbacks.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CImagesInternals;
struct	CImageFrame;
struct	CImageMap;
HH_FORWARD_DECLARE(Image);
HH_FORWARD_DECLARE(RefCountedMemoryBuffer);

//----------------------------------------------------------------------------

class	HH_IMAGING_EXPORT CImage : public CRefCountedObject
{
public:
	enum	EFormat
	{
		Format_Invalid = 0,

		// Basic integer formats:
		Format_Lum8,
		Format_LumAlpha8,
		Format_BGR8,
		Format_BGRA8,
		Format_BGRA8_sRGB,

		// Packed integer format:
		Format_BGRA4,		// 16bpp 4.4.4.4 BGRA Linear
		Format_BGRA4_sRGB,	// 16bpp 4.4.4.4 BGRA sRGB

		// 4 BC/DXT compressed formats:
		Format_DXT1,
		Format_DXT1_sRGB,
		Format_DXT3,
		Format_DXT3_sRGB,
		Format_DXT5,
		Format_DXT5_sRGB,

		Format_BC5_UNorm,
		Format_BC5_SNorm,

		// Mobile-friendly compressed formats:
		Format_RGB8_ETC1,
		Format_RGB8_ETC2,
		Format_RGBA8_ETC2,
		Format_RGB8A1_ETC2,

		Format_RGB4_PVRTC1,
		Format_RGB2_PVRTC1,
		Format_RGBA4_PVRTC1,
		Format_RGBA2_PVRTC1,

		// Fp32 versions of the basic formats:
		Format_Fp32Lum,
		Format_Fp32LumAlpha,
		Format_Fp32RGBA,

		// Fp16 versions of the basic formats:
		Format_Fp16Lum,
		Format_Fp16LumAlpha,
		Format_Fp16RGBA,

		__MaxImageFormats,
	};

	enum	EFFlags
	{
		FF_Compressed		= 0x1,
		FF_Fp				= 0x2,
		FF_Compressed_DXT	= 0x4,
		FF_Compressed_3Dc	= 0x8,
		FF_Compressed_ETC	= 0x10,
		FF_Compressed_PVRTC	= 0x20,
		FF_sRGB				= 0x40,
	};

	enum	EFlags
	{
		Flag_Cubemap	= 0x10,
		Flag_Volume		= 0x20,
	};

private:
	friend class			CImagesInternals;

	static hh_u8			m_FormatFlags[__MaxImageFormats];
	static hh_u8			m_FormatToChannelSize[__MaxImageFormats];
	static hh_u8			m_FormatToChannelCount[__MaxImageFormats];
	static hh_u8			m_FormatToPixelSize[__MaxImageFormats];

	static bool				Startup();
	static void				Shutdown();

public:
	CImage();
	~CImage();

	HH_INLINE static hh_u32	FormatFlags(EFormat format) { return m_FormatFlags[format]; }
	HH_INLINE static bool	IsFormatGammaCorrected(EFormat format) { return (m_FormatFlags[format] & FF_sRGB) != 0; }
	HH_INLINE static bool	IsFormatCompressed(EFormat format) { return (m_FormatFlags[format] & FF_Compressed) != 0; }
	HH_INLINE static bool	IsFormatCompressed_DXT(EFormat format) { return (m_FormatFlags[format] & FF_Compressed_DXT) != 0; }
	HH_INLINE static bool	IsFormatCompressed_3Dc(EFormat format) { return (m_FormatFlags[format] & FF_Compressed_3Dc) != 0; }
	HH_INLINE static bool	IsFormatFloatingPoint(EFormat format) { return (m_FormatFlags[format] & FF_Fp) != 0; }
	HH_INLINE static hh_u8	GetFormatChannelCount(EFormat format) { return m_FormatToChannelCount[format]; }
	HH_INLINE static hh_u8	GetFormatPixelSizeInBytes(EFormat format) { return m_FormatToPixelSize[format]; }
	static hh_u32			GetFormatPixelBufferSizeInBytes(EFormat format, const CUint3 &dimensionsInPixels);
	static const char		*GetFormatName(EFormat format);

	// Deprecated v1.11.0
	HH_DEPRECATED("deprecated v1.11.0.31680 2016/10/06, use GetFormatPixelSizeInBytes or GetFormatPixelBufferSizeInBytes instead")
	HH_INLINE static hh_u8	GetFormatChannelSizeInBytes(EFormat format) { HH_ASSERT(!IsFormatCompressed(format)); return m_FormatToChannelSize[format]; }	// returns 1 for 8bpp channels, 2 for fp16, 4 for fp32
	HH_DEPRECATED("deprecated v1.11.0.31681 2016/10/06, use GetFormatPixelBufferSizeInBytes instead")
	static hh_u32			GetPixelBufferSizeInBytes(EFormat format, const CUint3 &dimensionsInPixels) { return GetFormatPixelBufferSizeInBytes(format, dimensionsInPixels); }

	HH_FORCEINLINE bool		GammaCorrected() const { return IsFormatGammaCorrected(m_Format); }
	HH_FORCEINLINE bool		Compressed() const { return IsFormatCompressed(m_Format); }
	HH_FORCEINLINE bool		FloatingPoint() const { return IsFormatFloatingPoint(m_Format); }
	HH_FORCEINLINE hh_u8	ChannelCount() const { return GetFormatChannelCount(m_Format); }
	HH_FORCEINLINE hh_u8	PixelSizeInBytes() const { return GetFormatPixelSizeInBytes(m_Format); }
	HH_FORCEINLINE hh_u32	PixelBufferSizeInBytes(const CUint3 &dimensionsInPixels) { return GetFormatPixelBufferSizeInBytes(m_Format, dimensionsInPixels); }

	hh_u32					m_Flags;
	EFormat					m_Format;
	TArray<CImageFrame>		m_Frames;

	TCallbackChainUnordered<void(CImage *self)>	m_OnReloading;
	TCallbackChainUnordered<void(CImage *self)>	m_OnReloaded;

	void					Clear();
	bool					Save(const CString &path) const;
	bool					Save(const CFilePackPath &path) const;
};
HH_DECLARE_REFPTRCLASS(Image);

//----------------------------------------------------------------------------

struct	HH_IMAGING_EXPORT CImageMap
{
	CUint3					m_Dimensions;
	PRefCountedMemoryBuffer	m_RawBuffer;

	CImageMap();
	CImageMap(const CUint3 &dimensions, const PRefCountedMemoryBuffer &rawBuffer);
	CImageMap(const CUint3 &dimensions, void *rawBuffer, hh_u32 rawBufferSizeInBytes);
	CImageMap(const CImageMap &other);
	~CImageMap();

	CImageMap	&operator = (const CImageMap &other);

	hh_u32	PixelBufferSize(CImage::EFormat format) const { return CImage::GetFormatPixelBufferSizeInBytes(format, m_Dimensions); }
};

//----------------------------------------------------------------------------

struct	HH_IMAGING_EXPORT CImageFrame
{
	float				m_Delay;
	TArray<CImageMap>	m_Mipmaps;

	CImageFrame();
	~CImageFrame();
};

//----------------------------------------------------------------------------
//
//	surface conversion settings
//
//----------------------------------------------------------------------------

// CIE RGB luminance conversion weights
#define	GF_IMAGES_GRAYSCALE_WEIGHT_R	0.212671f
#define	GF_IMAGES_GRAYSCALE_WEIGHT_G	0.715160f
#define	GF_IMAGES_GRAYSCALE_WEIGHT_B	0.072169f

//----------------------------------------------------------------------------

struct	SImageConvertSettings
{
	enum	ELumMode
	{
		LumMode_Red,
		LumMode_Green,
		LumMode_Blue,
		LumMode_Alpha,
		LumMode_RGBA_Avg,
		__MaxLumModes
	};

	CFloat4		m_RGBAToLumWeights;
	ELumMode	m_RGBAToLumMode;
};

//----------------------------------------------------------------------------
//
//	surface
//
//----------------------------------------------------------------------------

class	HH_IMAGING_EXPORT CImageSurface
{
public:
	CImage::EFormat			m_Format;
	CUint3					m_Dimensions;
	PRefCountedMemoryBuffer	m_RawBuffer;

	CImageSurface();
	CImageSurface(const CUint3 &dimensions, const PRefCountedMemoryBuffer &rawBuffer, CImage::EFormat format);
	CImageSurface(const CImageMap &map, CImage::EFormat format);
	CImageSurface(const CImageSurface &other, CImage::EFormat newFormat);
	CImageSurface(const CImageSurface &other);
	~CImageSurface();

	CImageSurface			&operator = (const CImageSurface &other);

	bool					Empty() const { return m_RawBuffer == null || m_Dimensions == 0; }
	void					Clear();

	CImage::EFormat			ToImageMap(CImageMap &map) const;

	HH_FORCEINLINE bool		Compressed() const { return CImage::IsFormatCompressed(m_Format); }
	HH_FORCEINLINE bool		FloatingPoint() const { return CImage::IsFormatFloatingPoint(m_Format); }
	HH_FORCEINLINE hh_u8	ChannelCount() const { return CImage::GetFormatChannelCount(m_Format); }
	HH_FORCEINLINE hh_u8	PixelSizeInBytes() const { return CImage::GetFormatPixelSizeInBytes(m_Format); }
	HH_FORCEINLINE hh_u32	PixelCount() const { return m_Dimensions.AxialProduct(); }

	bool					CopyAndConvertIFN(const CImageSurface &other);	// converts to the format already present in 'm_Format'
	bool					Convert(CImage::EFormat format, const SImageConvertSettings *conversionSettings = null);

	void					PrepareForWriting();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __IM_IMAGE_H__
