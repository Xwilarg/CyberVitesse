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

#include "RenderResource.h"
#include "VertexFactory.h"

#include "PopcornFXSDK.h"

/** Batch element parameters */
struct	FPopcornFXBatchElementParams
{
	enum EType
	{
		Type_Std,
		//Type_BB,
	};

	EType		m_Type;

	// Vertex shader parameters
	bool		m_IsSoftAnimBlended;

	// Fragment shader parameters
	bool		m_NeedNorTan;
	bool		m_IsHighQuality;
	bool		m_IsRotated;

	FPopcornFXBatchElementParams()
		: m_Type(Type_Std)
		, m_IsSoftAnimBlended(false)
		, m_NeedNorTan(false)
		, m_IsHighQuality(false)
		, m_IsRotated(false)
	{
	}
};

class	FPopcornFXVertexFactoryShaderParametersVertex : public FVertexFactoryShaderParameters
{
public:
	/**
	* Bind shader constants by name
	* @param	ParameterMap - mapping of named shader constants to indices
	*/
	virtual void			Bind(const FShaderParameterMap &parameterMap) override;

	/**
	* Serialize shader params to an archive
	* @param	Ar - archive to serialize to
	*/
	virtual void			Serialize(FArchive &archive) override;

	/**
	* Set any shader data specific to this vertex factory
	*/
	virtual void			SetMesh(FRHICommandList &RHICmdList, FShader *pixelShader, const class FVertexFactory *vertexFactory, const class FSceneView &view, const struct FMeshBatchElement &batchElement, uint32 dataFlags) const override;

private:
	FShaderParameter		NeedNorTan;
	FShaderParameter		IsSoftAnimBlended;
};

class	FPopcornFXVertexFactoryShaderParametersPixel : public FVertexFactoryShaderParameters
{
public:
	/**
	* Bind shader constants by name
	* @param	ParameterMap - mapping of named shader constants to indices
	*/
	virtual void				Bind(const FShaderParameterMap &parameterMap) override;

	/**
	* Serialize shader params to an archive
	* @param	Ar - archive to serialize to
	*/
	virtual void				Serialize(FArchive &archive) override;

	/**
	* Set any shader data specific to this vertex factory
	*/
	virtual void				SetMesh(FRHICommandList &RHICmdList, FShader *vertexShader, const class FVertexFactory *vertexFactory, const class FSceneView &view, const struct FMeshBatchElement &batchElement, uint32 dataFlags) const override;
private:
	FShaderParameter	NeedNorTan;
	FShaderParameter	IsHighQuality;
	FShaderParameter	IsRotated;
};
