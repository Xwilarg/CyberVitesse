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

#if (PK_GPU_D3D11 == 1)

#include "PopcornFXMinimal.h"

#include "Render/PopcornFXBuffer.h"

#include "GlobalShader.h"
#include "ShaderParameterUtils.h"

#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

struct FPopcornFXSortComputeShader_GenKeys_Params
{
	CFloat4		m_SortOrigin;

	hh_u32		m_Count = 0;
	hh_u32		m_IndexStart = 0;
	hh_u32		m_IndexStep = 0;
	hh_u32		m_InputOffset = 0;

	FShaderResourceViewRHIRef		m_InputPositions;
};

//----------------------------------------------------------------------------
//
// FPopcornFXSortComputeShader_Sorter
//
//----------------------------------------------------------------------------

struct FPopcornFXSortComputeShader_Sorter
{
public:
	void		Clear();

	bool		Prepare(CVertexBufferPool &vpool, hh_u32 totalCount);
	bool		Ready() const { return m_TotalCount > 0; }

	void		DispatchGenIndiceBatch(FRHICommandList& RHICmdList, const FPopcornFXSortComputeShader_GenKeys_Params &genParams);
	void		DispatchSort(FRHICommandList& RHICmdList);

	const FShaderResourceViewRHIRef		&SortedValuesSRV();

private:
	hh_u32		m_TotalCount = 0;

	hh_u32		m_CurrGenOutputOffset = 0;

	hh_u32					m_CurrBuff = 0;
	CPooledVertexBuffer		m_Keys[2];
	CPooledVertexBuffer		m_Values[2];
	CPooledVertexBuffer		m_Cache;
};

//----------------------------------------------------------------------------

#endif // (PK_GPU_D3D11 == 1)
