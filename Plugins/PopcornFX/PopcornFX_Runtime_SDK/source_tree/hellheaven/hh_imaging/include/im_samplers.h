#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/09/24 10:31 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __GF_IMAGES_SAMPLERS_H__
#define __GF_IMAGES_SAMPLERS_H__

#include "hh_imaging/include/im_image.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_IMAGING_EXPORT CImageSamplerInterface
{
public:
	virtual ~CImageSamplerInterface() {}

	enum	ETexcoordMode
	{
		TexcoordMode_Clamp = 0,
		TexcoordMode_Wrap = 1,
//		TexcoordMode_WrapMirror = 2,
		__MaxTexcoordModes
	};

	enum	EFilterType
	{
		FilterType_Point = 0,
		FilterType_Linear,
		__MaxFilterTypes
	};

	// Texture dimensions in texels (can be a normalized aspect ratio, called by image sampler's 'dimensions()' getter.
	virtual CFloat2		Dimensions() const = 0;

	// called when expecting a float4 result (maps BGRA8u[0,255] -> RGBA32f[0,1]. can return HDR values above 1.0 if applicable)
	virtual bool		Sample(	const TStridedMemoryView<const CFloat2>	&texcoords,
								const TStridedMemoryView<CFloat4>		&outputSamples,
								EFilterType								filterType = FilterType_Point,
								ETexcoordMode							tcMode = TexcoordMode_Clamp) const = 0;

	// called when expecting a float result (Grayscale. can return values above 1.0)
	virtual bool		Sample(	const TStridedMemoryView<const CFloat2>	&texcoords,
								const TStridedMemoryView<float>			&outputSamples,
								EFilterType								filterType = FilterType_Point,
								ETexcoordMode							tcMode = TexcoordMode_Clamp) const = 0;
};

//----------------------------------------------------------------------------
//
//	Default implementations
//
//----------------------------------------------------------------------------

class	HH_IMAGING_EXPORT CImageSampler : public CImageSamplerInterface, public CNonCopyable
{
protected:
	// Internal storage
	PRefCountedMemoryBuffer			m_TexelsRef;
	const hh_u8						*m_Texels;
	hh_u8							m_InternalDataAlignmentBuffer[0xF + 2 * 0x10];
	hh_u8							m_Log2X;	// don't move
	CUint2							m_Dimensions;
	CImage::EFormat					m_Format;
	bool							m_CanTakeFastPath;

	HH_FORCEINLINE const hh_u8		*_InternalAlignedData() const { return Mem::Align<0x10>(m_InternalDataAlignmentBuffer); }
	HH_FORCEINLINE hh_u8			*_InternalAlignedData() { return Mem::Align<0x10>(m_InternalDataAlignmentBuffer); }

	HH_FORCEINLINE const CFloat4	&_DimensionsF4_XYXY() const { return *reinterpret_cast<const CFloat4*>(_InternalAlignedData() + 0); }
	HH_FORCEINLINE CFloat4			&_DimensionsF4_XYXY() { return *reinterpret_cast<CFloat4*>(_InternalAlignedData() + 0); }
	HH_FORCEINLINE const CFloat4	&_InvHalfDimensionsF4_XYXY() const { return *reinterpret_cast<const CFloat4*>(_InternalAlignedData() + 0x10); }
	HH_FORCEINLINE CFloat4			&_InvHalfDimensionsF4_XYXY() { return *reinterpret_cast<CFloat4*>(_InternalAlignedData() + 0x10); }

	// Sampling
	typedef void					(*FnSamplePoint)(const CImageSampler *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	FnSamplePoint					m_BoundSamplersPoint[__MaxTexcoordModes];

	// src dst
	static void						_InternalSampleStream_BGRA8_RGBA32F_Point_Wrap(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_BGRA8_RGBA32F_Point_Clamp(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_BGRA4_RGBA32F_Point_Wrap(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_BGRA4_RGBA32F_Point_Clamp(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_DXT1_RGBA32F_Point_Wrap(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_DXT1_RGBA32F_Point_Clamp(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_RGBA32F_RGBA32F_Point_Wrap(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_RGBA32F_RGBA32F_Point_Clamp(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_R32F_R32F_Point_Wrap(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);
	static void						_InternalSampleStream_R32F_R32F_Point_Clamp(const CImageSampler *self, const float *uv, float *dst, hh_u32 sampleCount);

	CFloat4							_InternalSamplePoint_BGRA8_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;
	CFloat4							_InternalSamplePoint_BGRA4_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;
	CFloat4							_InternalSamplePoint_DXT1_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;
	CFloat4							_InternalSamplePoint_RGBA32F_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;
	float							_InternalSamplePoint_R32F_R32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;

public:
	CImageSampler();
	~CImageSampler();

	bool							Empty() const { return m_Texels == null; };
	virtual void					Clear();

	// keeps a reference to the refcounted texels, you don't have to worry about the memory being freed
	virtual bool					SetupFromSurface(const CImageSurface &surface);

	// you have to make sure the texel memory remains valid as long as the sampler is active.
	virtual bool					SetupFromRawPointer(const void *texels, const CUint2 &dimensions, CImage::EFormat format);

	// direct-call point implementations
	CFloat4							SamplePoint_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode = TexcoordMode_Clamp) const;
	float							SamplePoint_R32F(const CFloat2 &texcoord, ETexcoordMode tcMode = TexcoordMode_Clamp) const;

	void							SamplePoint(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<CFloat4> &outputSamples, ETexcoordMode tcMode = TexcoordMode_Clamp) const;
	void							SamplePoint(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<float> &outputSamples, ETexcoordMode tcMode = TexcoordMode_Clamp) const;

	virtual CFloat2					Dimensions() const override;

	// generic Sample() methods from CImageSamplerInterface
	virtual bool					Sample(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<CFloat4> &outputSamples, EFilterType filterType, ETexcoordMode tcMode) const override;
	virtual bool					Sample(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<float> &outputSamples, EFilterType filterType, ETexcoordMode tcMode) const override;
};

//----------------------------------------------------------------------------

class	HH_IMAGING_EXPORT CImageSamplerBilinear : public CImageSampler
{
protected:
	typedef void			(*FnSampleLinear)(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	FnSampleLinear			m_BoundSamplersLinear[__MaxTexcoordModes];

	static void				_InternalSampleStream_BGRA8_RGBA32F_Linear_Wrap(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	static void				_InternalSampleStream_BGRA8_RGBA32F_Linear_Clamp(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	static void				_InternalSampleStream_BGRA4_RGBA32F_Linear_Wrap(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	static void				_InternalSampleStream_BGRA4_RGBA32F_Linear_Clamp(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	static void				_InternalSampleStream_RGBA32F_RGBA32F_Linear_Wrap(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);
	static void				_InternalSampleStream_RGBA32F_RGBA32F_Linear_Clamp(const CImageSamplerBilinear *self, const float *uv, float *outputSamples, hh_u32 sampleCount);

	CFloat4					_InternalSampleLinear_BGRA8_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;
	CFloat4					_InternalSampleLinear_BGRA4_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;
	CFloat4					_InternalSampleLinear_RGBA32F_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode) const;

public:
	CImageSamplerBilinear();
	~CImageSamplerBilinear();

	virtual void			Clear() override;

	// keeps a reference to the refcounted texels, you don't have to worry about the memory being freed
	virtual bool			SetupFromSurface(const CImageSurface &surface) override;

	// you have to make sure the texel memory remains valid as long as the sampler is active.
	virtual bool			SetupFromRawPointer(const void *texels, const CUint2 &dimensions, CImage::EFormat format) override;

	// direct-call bilerp implementations
	CFloat4					SampleLinear_RGBA32F(const CFloat2 &texcoord, ETexcoordMode tcMode = TexcoordMode_Clamp) const;
//	float					SampleLinear_R32F(const CFloat2 &texcoord, ETexcoordMode tcMode = TexcoordMode_Clamp) const;

	void					SampleLinear(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<CFloat4> &outputSamples, ETexcoordMode tcMode = TexcoordMode_Clamp) const;
//	void					SampleLinear(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<float> &outputSamples, ETexcoordMode tcMode = TexcoordMode_Clamp) const;

	// generic Sample() methods from CImageSamplerInterface
	virtual bool			Sample(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<CFloat4> &outputSamples, EFilterType filterType, ETexcoordMode tcMode) const override;
	virtual bool			Sample(const TStridedMemoryView<const CFloat2> &texcoords, const TStridedMemoryView<float> &outputSamples, EFilterType filterType, ETexcoordMode tcMode) const override;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __GF_IMAGES_SAMPLERS_H__

