//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "MyRHIStuff.h"

#include "PopcornFXSDK.h"
#if (PK_HAS_GPU != 0)
#	include <hh_particles/include/ps_mediums.h>
#	include <hh_particles/include/ps_stream_to_render.h>

#if (PK_GPU_D3D11 == 1)
#	include <hh_particles/include/Storage/D3D11/storage_d3d11.h>
#endif

//----------------------------------------------------------------------------

#if ENABLE_MYGPUMARKER && (PK_GPU_D3D11 == 1)
#	pragma comment (lib, "d3d9.lib")
#endif

//----------------------------------------------------------------------------
//
//		D3D11
//
//----------------------------------------------------------------------------

#if (PK_GPU_D3D11 == 1)

DECLARE_LOG_CATEGORY_EXTERN(LogD3D11RHI, Log, All);
#	include "D3D11RHI.h"
#	include "D3D11State.h"
#	include "D3D11Resources.h"
//#	include "D3D11Util.h"
//#	include "D3D11Resources.h"
//#	include "D3D11Viewport.h"
//#	include "D3D11ConstantBuffer.h"
//#	include "D3D11StateCache.h"

//----------------------------------------------------------------------------

#if WITH_EDITOR
// DAMNSADASD

// FIX EDITOR BUILD
void UpdateBufferStats(TRefCountPtr<ID3D11Buffer> Buffer, bool bAllocating) { }
#endif // WITH_EDITOR

//----------------------------------------------------------------------------

static uint32	_D3D11GetRefCount(IUnknown &res)
{
	uint32		count = res.AddRef();
	res.Release();
	return count - 1;
}

//----------------------------------------------------------------------------

FVertexBufferRHIRef				StreamBufferToRHI(const PopcornFX::SParticleStreamBuffer_D3D11 *stream, hh_u32 bytes)
{
	// !!!!
	// @TODO double check: Might leak ref count, creating double free etc
	// !!!!

	D3D11_BUFFER_DESC			desc;
	stream->m_Buffer->GetDesc(&desc);
	HH_ASSERT((desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) == 0); // no structured
	HH_ASSERT((desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0); // is BUF_UnorderedAccess
	HH_ASSERT((desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) != 0); // is BUF_ByteAddressBuffer
	HH_ASSERT((desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0); // is BUF_ShaderResource
	HH_ASSERT((desc.Usage & D3D11_USAGE_DYNAMIC) == 0); // no BUF_AnyDynamic
	HH_ASSERT((desc.CPUAccessFlags) == 0); // no BUF_AnyDynamic

	uint32						bufferUsage = BUF_UnorderedAccess | BUF_ByteAddressBuffer | BUF_ShaderResource;
	FD3D11VertexBuffer			*buffer = new FD3D11VertexBuffer(stream->m_Buffer, bytes, bufferUsage);
	HH_ASSERT(_D3D11GetRefCount(*(buffer->Resource)) > 1); // Referenced here and by PopcornFX
	return buffer;
}

//----------------------------------------------------------------------------

FShaderResourceViewRHIRef		StreamBufferSRVToRHI(const PopcornFX::SParticleStreamBuffer_D3D11 *stream, hh_u32 bytes)
{
	D3D11_BUFFER_DESC			desc;
	stream->m_Buffer->GetDesc(&desc);
	HH_ASSERT((desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) == 0); // no structured
	HH_ASSERT((desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0); // is BUF_UnorderedAccess
	HH_ASSERT((desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) != 0); // is BUF_ByteAddressBuffer
	HH_ASSERT((desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0); // is BUF_ShaderResource
	HH_ASSERT((desc.Usage & D3D11_USAGE_DYNAMIC) == 0); // no BUF_AnyDynamic
	HH_ASSERT((desc.CPUAccessFlags) == 0); // no BUF_AnyDynamic

	uint32						bufferUsage = BUF_UnorderedAccess | BUF_ByteAddressBuffer | BUF_ShaderResource;
	FD3D11VertexBuffer			*buffer = new FD3D11VertexBuffer(stream->m_Buffer, bytes, bufferUsage);
	HH_ASSERT(_D3D11GetRefCount(*(buffer->Resource)) > 1); // Referenced here and by PopcornFX
	FD3D11ShaderResourceView	*srv = new FD3D11ShaderResourceView(stream->m_BufferSRV, buffer);
	HH_ASSERT(_D3D11GetRefCount(*(srv->View)) > 1); // Referenced here and by PopcornFX
	return srv;
}

//----------------------------------------------------------------------------

template <typename _Type, hh_u32 _Stride>
FVertexBufferRHIRef				StreamBufferToRHI(const PopcornFX::CParticleStreamToRender_D3D11 *_stream, PopcornFX::CGuid streamId)
{
	HH_ASSERT(streamId.Valid());
	HH_FIXME("const_cast pas bien !");
	PopcornFX::CParticleStreamToRender_D3D11		*stream = const_cast<PopcornFX::CParticleStreamToRender_D3D11*>(_stream);
	const PopcornFX::SParticleStreamBuffer_D3D11	*buffer = stream->Stream<_Type, _Stride>(streamId);
	if (!HH_VERIFY(buffer != null))
		return false;
	return StreamBufferToRHI(buffer, stream->StreamSizeEst() * _Stride);
}

//----------------------------------------------------------------------------

template <typename _Type, hh_u32 _Stride>
FShaderResourceViewRHIRef		StreamBufferSRVToRHI(const PopcornFX::CParticleStreamToRender_D3D11 *_stream, PopcornFX::CGuid streamId)
{
	HH_ASSERT(streamId.Valid());
	HH_FIXME("const_cast pas bien !");
	PopcornFX::CParticleStreamToRender_D3D11		*stream = const_cast<PopcornFX::CParticleStreamToRender_D3D11*>(_stream);
	const PopcornFX::SParticleStreamBuffer_D3D11	*buffer = stream->Stream<_Type, _Stride>(streamId);
	if (!HH_VERIFY(buffer != null))
		return false;
	return StreamBufferSRVToRHI(buffer, stream->StreamSizeEst() * _Stride);
}

//----------------------------------------------------------------------------

#define DEFINE_StreamBufferSRVToRHI(__type, __stride) \
	template FVertexBufferRHIRef			StreamBufferToRHI<__type, __stride>(const PopcornFX::CParticleStreamToRender_D3D11 *_stream, PopcornFX::CGuid streamId);			\
	template FShaderResourceViewRHIRef		StreamBufferSRVToRHI<__type, __stride>(const PopcornFX::CParticleStreamToRender_D3D11 *_stream, PopcornFX::CGuid streamId)

DEFINE_StreamBufferSRVToRHI(CFloat4, 16);
DEFINE_StreamBufferSRVToRHI(CFloat3, 12);
DEFINE_StreamBufferSRVToRHI(CFloat2, 8);
DEFINE_StreamBufferSRVToRHI(float, 4);
#undef DEFINE_StreamBufferSRVToRHI

//----------------------------------------------------------------------------

// Engine\Source\Runtime\Windows\D3D11RHI\Private\D3D11RHIPrivate.h
/** Find an appropriate DXGI format for the input format and SRGB setting. */
inline static DXGI_FORMAT My_FindShaderResourceDXGIFormat(DXGI_FORMAT InFormat, bool bSRGB)
{
	if (bSRGB)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:    return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:    return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case DXGI_FORMAT_BC1_TYPELESS:         return DXGI_FORMAT_BC1_UNORM_SRGB;
		case DXGI_FORMAT_BC2_TYPELESS:         return DXGI_FORMAT_BC2_UNORM_SRGB;
		case DXGI_FORMAT_BC3_TYPELESS:         return DXGI_FORMAT_BC3_UNORM_SRGB;
		case DXGI_FORMAT_BC7_TYPELESS:         return DXGI_FORMAT_BC7_UNORM_SRGB;
		};
	}
	else
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_BC1_TYPELESS:      return DXGI_FORMAT_BC1_UNORM;
		case DXGI_FORMAT_BC2_TYPELESS:      return DXGI_FORMAT_BC2_UNORM;
		case DXGI_FORMAT_BC3_TYPELESS:      return DXGI_FORMAT_BC3_UNORM;
		case DXGI_FORMAT_BC7_TYPELESS:      return DXGI_FORMAT_BC7_UNORM;
		};
	}
	switch (InFormat)
	{
	case DXGI_FORMAT_R24G8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DXGI_FORMAT_R32_TYPELESS: return DXGI_FORMAT_R32_FLOAT;
	case DXGI_FORMAT_R16_TYPELESS: return DXGI_FORMAT_R16_UNORM;
#if DEPTH_32_BIT_CONVERSION
		// Changing Depth Buffers to 32 bit on Dingo as D24S8 is actually implemented as a 32 bit buffer in the hardware
	case DXGI_FORMAT_R32G8X24_TYPELESS: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
#endif
	}
	return InFormat;
}

//----------------------------------------------------------------------------

// Source/Runtime/Windows/D3D11RHI/Private/D3D11UAV.cpp
//
// Creates a ***Raw Byte Address*** SRV
//
FShaderResourceViewRHIRef		My_RHICreateShaderResourceView(FVertexBufferRHIParamRef VertexBufferRHI, uint32 Stride, uint8 Format)
{
	//DYNAMIC_CAST_D3D11RESOURCE(VertexBuffer, VertexBuffer);
	FD3D11VertexBuffer			*VertexBuffer = (FD3D11VertexBuffer*)VertexBufferRHI;
	check(VertexBuffer);
	check(VertexBuffer->Resource);

	D3D11_BUFFER_DESC BufferDesc;
	VertexBuffer->Resource->GetDesc(&BufferDesc);

	const bool bByteAccessBuffer = (BufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) != 0;

	// Create a Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	PopcornFX::Mem::Clear(SRVDesc);

	if (bByteAccessBuffer)
	{
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		SRVDesc.BufferEx.NumElements = BufferDesc.ByteWidth / 4;
		SRVDesc.BufferEx.FirstElement = 0;
		SRVDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		SRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	}
	else
	{
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		SRVDesc.Buffer.FirstElement = 0;
		
		SRVDesc.Format = My_FindShaderResourceDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat, false);
		SRVDesc.Buffer.NumElements = BufferDesc.ByteWidth / Stride;
	}

	TRefCountPtr<ID3D11ShaderResourceView> ShaderResourceView;

	//
	ID3D11Device		*Direct3DDevice = (ID3D11Device*)RHIGetNativeDevice();
	//

	HRESULT hr = Direct3DDevice->CreateShaderResourceView(VertexBuffer->Resource, &SRVDesc, (ID3D11ShaderResourceView**)ShaderResourceView.GetInitReference());
	if (FAILED(hr))
	{
		if (hr == E_OUTOFMEMORY)
		{
			// There appears to be a driver bug that causes SRV creation to fail with an OOM error and then succeed on the next call.
			hr = Direct3DDevice->CreateShaderResourceView(VertexBuffer->Resource, &SRVDesc, (ID3D11ShaderResourceView**)ShaderResourceView.GetInitReference());
		}
		if (FAILED(hr))
		{
			//UE_LOG(LogD3D11RHI, Error, TEXT("Failed to create shader resource view for vertex buffer: ByteWidth=%d NumElements=%d"), BufferDesc.ByteWidth, BufferDesc.ByteWidth / Stride);
			//VerifyD3D11Result(hr, "Direct3DDevice->CreateShaderResourceView", __FILE__, __LINE__, Direct3DDevice);
			HH_ASSERT_NOT_REACHED();
		}
	}

	return new FD3D11ShaderResourceView(ShaderResourceView, VertexBuffer);
}

//----------------------------------------------------------------------------

// Source/Runtime/Windows/D3D11RHI/Private/D3D11RHIPrivate.h
inline static DXGI_FORMAT My_FindUnorderedAccessDXGIFormat(DXGI_FORMAT InFormat)
{
	switch (InFormat)
	{
	case DXGI_FORMAT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_UNORM;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	return InFormat;
}

//----------------------------------------------------------------------------

FUnorderedAccessViewRHIRef		My_RHICreateUnorderedAccessView(FVertexBufferRHIParamRef VertexBufferRHI, uint8 Format)
{
	HH_ASSERT(IsValidRef(VertexBufferRHI));
	return RHICreateUnorderedAccessView(VertexBufferRHI, Format);
}

//----------------------------------------------------------------------------

// Source/Runtime/Windows/D3D11RHI/Private/D3D11UAV.cpp
//
// UAV from Index Buffer
//
FUnorderedAccessViewRHIRef		My_RHICreateUnorderedAccessView(FIndexBufferRHIParamRef IndexBufferRHI, uint8 Format)
{
	HH_ASSERT(IsValidRef(IndexBufferRHI));

	FD3D11IndexBuffer			*IndexBuffer = (FD3D11IndexBuffer*)IndexBufferRHI;

	D3D11_BUFFER_DESC BufferDesc;
	IndexBuffer->Resource->GetDesc(&BufferDesc);

	const bool bByteAccessBuffer = (BufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) != 0;

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Format = My_FindUnorderedAccessDXGIFormat((DXGI_FORMAT)GPixelFormats[Format].PlatformFormat);
	UAVDesc.Buffer.FirstElement = 0;

	UAVDesc.Buffer.NumElements = BufferDesc.ByteWidth / GPixelFormats[Format].BlockBytes;
	UAVDesc.Buffer.Flags = 0;

	// It seems UE does not enable RAW_VIEWS on IndexBuffer (for now ?)
	// If this assert Pops up, Great ! just change the shader:
	// from "RWBuffer<uint> OutIndices;"
	// to "RWByteAddressBuffer OutIndices;" with the code that depends on that
	HH_ASSERT(!bByteAccessBuffer);
	if (bByteAccessBuffer)
	{
		UAVDesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
		UAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	}

	//
	ID3D11Device		*Direct3DDevice = (ID3D11Device*)RHIGetNativeDevice();
	//

	TRefCountPtr<ID3D11UnorderedAccessView> UnorderedAccessView;
	//VERIFYD3D11RESULT(Direct3DDevice->CreateUnorderedAccessView(IndexBuffer->Resource, &UAVDesc, (ID3D11UnorderedAccessView**)UnorderedAccessView.GetInitReference()));
	HRESULT hr = Direct3DDevice->CreateUnorderedAccessView(IndexBuffer->Resource, &UAVDesc, (ID3D11UnorderedAccessView**)UnorderedAccessView.GetInitReference());

	HH_ASSERT(!FAILED(hr));

	return new FD3D11UnorderedAccessView(UnorderedAccessView, IndexBuffer);
}

#endif // (PK_GPU_D3D11 == 1)

#endif // (PK_HAS_GPU != 0)
