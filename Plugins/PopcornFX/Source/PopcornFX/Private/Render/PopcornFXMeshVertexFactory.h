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

#include "PopcornFXVertexFactory.h"

#include "RenderResource.h"
#include "Components.h"

#include "PopcornFXSDK.h"

// If FPopcornFXMeshVertexFactory is unused, it will be optimized out in Game build, and crash when trying to find it ...
//#define UNUSED_PopcornFXMeshVertexFactory

#ifndef UNUSED_PopcornFXMeshVertexFactory

struct FPopcornFXMeshUserData
{
	bool								m_Instanced;
	TStridedMemoryView<const CFloat4x4>	m_Instance_Matrices;
	TStridedMemoryView<const CFloat4>	m_Instance_Param_DiffuseColors;
};

class	FPopcornFXMeshVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FPopcornFXMeshVertexFactory);
public:

	static bool			IsInstanced(ERHIFeatureLevel::Type featureLevel)
	{
		return featureLevel >= ERHIFeatureLevel::SM4;
	}

public:
	struct FDataType
#if (ENGINE_MINOR_VERSION >= 12)
#else
		: public FVertexFactory::DataType
#endif
	{
		// Instanced particle streams:

		FPopcornFXVertexBufferView	m_InstancedMatrices;
		FPopcornFXVertexBufferView	m_InstancedColors;

		// Mesh streams:

		FVertexStreamComponent		PositionComponent;
		FVertexStreamComponent		TangentBasisComponents[2];
		TArray<FVertexStreamComponent, TFixedAllocator<MAX_TEXCOORDS> >		TextureCoordinates;
		FVertexStreamComponent		VertexColorComponent;

		bool bInitialized;

		FDataType()
			: bInitialized(false)
		{

		}
	};

	FPopcornFXMeshVertexFactory()
		: FVertexFactory()
	{}

	static bool			ShouldCache(EShaderPlatform platform, const class FMaterial *material, const class FShaderType *shaderType);
	static bool			IsCompatible(UMaterialInterface *material);

	virtual void		InitRHI() override;
	void				SetData(const FDataType& InData);

	static FVertexFactoryShaderParameters		*ConstructShaderParameters(EShaderFrequency ShaderFrequency);
	static void									ModifyCompilationEnvironment(EShaderPlatform Platform, const FMaterial* Material, FShaderCompilerEnvironment& OutEnvironment);

	static bool			SupportsTessellationShaders() { return false; }

protected:
	FDataType			Data;
};

#endif // UNUSED_PopcornFXMeshVertexFactory
