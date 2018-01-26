//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXVertexFactoryShaderParameters.h"

#include "PopcornFXCustomVersion.h"

#include "ShaderParameterUtils.h"
#include "MeshBatch.h"

#include "PopcornFXSDK.h"

//----------------------------------------------------------------------------
//
// Vertex
//
//----------------------------------------------------------------------------

void	FPopcornFXVertexFactoryShaderParametersVertex::Bind(const FShaderParameterMap &parameterMap)
{
	IsSoftAnimBlended.Bind(parameterMap, TEXT("IsSoftAnimBlended"));
	NeedNorTan.Bind(parameterMap, TEXT("NeedNorTan"));
}

//----------------------------------------------------------------------------

void	FPopcornFXVertexFactoryShaderParametersVertex::Serialize(FArchive &archive)
{
	archive.UsingCustomVersion(FPopcornFXCustomVersion::GUID);

	archive << IsSoftAnimBlended;
	archive << NeedNorTan;
}

//----------------------------------------------------------------------------

void	FPopcornFXVertexFactoryShaderParametersVertex::SetMesh(FRHICommandList &RHICmdList, FShader *vertexShader, const class FVertexFactory *vertexFactory, const class FSceneView &view, const struct FMeshBatchElement &batchElement, uint32 dataFlags) const
{
	const FPopcornFXBatchElementParams	*userData = (FPopcornFXBatchElementParams*)batchElement.UserData;

	if (IsSoftAnimBlended.IsBound())
		SetShaderValue(RHICmdList, vertexShader->GetVertexShader(), IsSoftAnimBlended, userData->m_IsSoftAnimBlended ? 1 : 0);
	if (NeedNorTan.IsBound())
		SetShaderValue(RHICmdList, vertexShader->GetVertexShader(), NeedNorTan, userData->m_NeedNorTan ? 1 : 0);
}

//----------------------------------------------------------------------------
//
// Pixel
//
//----------------------------------------------------------------------------

void	FPopcornFXVertexFactoryShaderParametersPixel::Bind(const FShaderParameterMap &parameterMap)
{
	NeedNorTan.Bind(parameterMap, TEXT("NeedNorTan"));
	IsHighQuality.Bind(parameterMap, TEXT("IsHighQuality"));
	IsRotated.Bind(parameterMap, TEXT("IsRotated"));
}

//----------------------------------------------------------------------------

void	FPopcornFXVertexFactoryShaderParametersPixel::Serialize(FArchive &archive)
{
	archive.UsingCustomVersion(FPopcornFXCustomVersion::GUID);

	archive << NeedNorTan;
	archive << IsHighQuality;
	archive << IsRotated;
}

//----------------------------------------------------------------------------

void	FPopcornFXVertexFactoryShaderParametersPixel::SetMesh(FRHICommandList &RHICmdList, FShader *pixelShader, const class FVertexFactory *vertexFactory, const class FSceneView &view, const struct FMeshBatchElement &batchElement, uint32 dataFlags) const
{
	const FPopcornFXBatchElementParams	*userData = (FPopcornFXBatchElementParams*)batchElement.UserData;

	if (NeedNorTan.IsBound())
		SetShaderValue(RHICmdList, pixelShader->GetPixelShader(), NeedNorTan, userData->m_NeedNorTan ? 1 : 0);
	if (IsHighQuality.IsBound())
		SetShaderValue(RHICmdList, pixelShader->GetPixelShader(), IsHighQuality, userData->m_IsHighQuality ? 1 : 0);
	if (IsRotated.IsBound())
		SetShaderValue(RHICmdList, pixelShader->GetPixelShader(), IsRotated, userData->m_IsRotated ? 1 : 0);
}
