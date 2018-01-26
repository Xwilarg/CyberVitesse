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

#if (PK_HAS_GPU != 0)

#include "RHI.h"
//#include "RHICommandList.h"
#include "RHIResources.h"

#include "PopcornFXSDK.h"
#if (PK_GPU_D3D11 == 1)
#	include <d3d11.h>
#	include "D3D11RHI.h"
//#	include "D3D11Util.h"
//#	include "D3D11State.h"
//#	include "D3D11Resources.h"
//#	include "D3D11Viewport.h"
//#	include "D3D11ConstantBuffer.h"
//#	include "D3D11StateCache.h"
#	include "DynamicRHI.h"
#	include <hh_particles/include/Updaters/D3D11/updater_d3d11.h>
#endif // (PK_GPU_D3D11 == 1)

//----------------------------------------------------------------------------

//#define ENABLE_MYGPUMARKER		1

//----------------------------------------------------------------------------
#if ENABLE_MYGPUMARKER && (PK_GPU_D3D11 == 1)
//#	include "AllowWindowsPlatformTypes.h" // already in PCH
#	include "d3d9.h"
//#	include "HideWindowsPlatformTypes.h"
struct FMyD3D11MyGPUMarker
{
	FMyD3D11MyGPUMarker(LPCWSTR name) { D3DPERF_BeginEvent(D3DCOLOR_ARGB(255, 3, 128, 212), name); }
	~FMyD3D11MyGPUMarker() { D3DPERF_EndEvent(); }
};
#	define MYGPUMARKER(__name)		FMyD3D11MyGPUMarker		_gpumarker_ ## __COUNTER__ ( TEXT( __name ) )

//----------------------------------------------------------------------------
//#if ENABLE_MYGPUMARKER && (PK_GPU_GL == 1)

//----------------------------------------------------------------------------
#else // nothing
#	define MYGPUMARKER(__name)
#endif

//----------------------------------------------------------------------------

// RHICreateShaderResourceView + handles Byte Access Buffers
FShaderResourceViewRHIRef		My_RHICreateShaderResourceView(FVertexBufferRHIParamRef VertexBufferRHI, uint32 Stride, uint8 Format);

// RHICreateUnorderedAccessView
FUnorderedAccessViewRHIRef		My_RHICreateUnorderedAccessView(FVertexBufferRHIParamRef VertexBufferRHI, uint8 Format);

// RHICreateUnorderedAccessView for index buffers
FUnorderedAccessViewRHIRef		My_RHICreateUnorderedAccessView(FIndexBufferRHIParamRef IndexBufferRHI, uint8 Format);

//----------------------------------------------------------------------------

#if (PK_GPU_D3D11 != 0)
FWD_PK_API_BEGIN
struct SParticleStreamBuffer_D3D11;
class CParticleStreamToRender_D3D11;
FWD_PK_API_END
// Statement to help the UE Header Parser not crash on FWD_PK_API_...
class	FPopcornFXPlugin;
// @TODO double check: Might leak ref count, creating double free etc
FVertexBufferRHIRef				StreamBufferToRHI(const PopcornFX::SParticleStreamBuffer_D3D11 *stream, hh_u32 bytes);
FShaderResourceViewRHIRef		StreamBufferSRVToRHI(const PopcornFX::SParticleStreamBuffer_D3D11 *stream, hh_u32 bytes);
template <typename _Type, hh_u32 _Stride>
FVertexBufferRHIRef				StreamBufferToRHI(const PopcornFX::CParticleStreamToRender_D3D11 *_stream, PopcornFX::CGuid streamId);
template <typename _Type, hh_u32 _Stride>
FShaderResourceViewRHIRef		StreamBufferSRVToRHI(const PopcornFX::CParticleStreamToRender_D3D11 *_stream, PopcornFX::CGuid streamId);
#endif // (PK_GPU_D3D11 != 0)

//----------------------------------------------------------------------------

#endif // (PK_HAS_GPU != 0)
