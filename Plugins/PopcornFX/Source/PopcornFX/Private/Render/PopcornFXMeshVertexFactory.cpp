//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXMeshVertexFactory.h"

#include "PopcornFXVertexFactoryShaderParameters.h"

#include "Render/PopcornFXShaderUtils.h"

#include "ShaderParameterUtils.h"
#include "SceneView.h"
#include "MeshBatch.h"
#include "Materials/MaterialInterface.h"

#ifndef UNUSED_PopcornFXMeshVertexFactory

//----------------------------------------------------------------------------
//
// FPopcornFXMeshVertexFactoryShaderParameters
//
//----------------------------------------------------------------------------

class FPopcornFXMeshVertexFactoryShaderParameters : public FVertexFactoryShaderParameters
{
public:

	virtual void Bind(const FShaderParameterMap& ParameterMap) override
	{
		Transform1.Bind(ParameterMap, TEXT("Transform1"));
		Transform2.Bind(ParameterMap, TEXT("Transform2"));
		Transform3.Bind(ParameterMap, TEXT("Transform3"));
		Transform4.Bind(ParameterMap, TEXT("Transform4"));
		//SubUVParams.Bind(ParameterMap, TEXT("SubUVParams"));
		//SubUVLerp.Bind(ParameterMap, TEXT("SubUVLerp"));
		//ParticleDirection.Bind(ParameterMap, TEXT("ParticleDirection"));
		//RelativeTime.Bind(ParameterMap, TEXT("RelativeTime"));
		//DynamicParameter.Bind(ParameterMap, TEXT("DynamicParameter"));
		ParticleColor.Bind(ParameterMap, TEXT("ParticleColor"));
	}

	virtual void Serialize(FArchive& Ar) override
	{
		Ar << Transform1;
		Ar << Transform2;
		Ar << Transform3;
		Ar << Transform4;
		//Ar << SubUVParams;
		//Ar << SubUVLerp;
		//Ar << ParticleDirection;
		//Ar << RelativeTime;
		//Ar << DynamicParameter;
		Ar << ParticleColor;
	}

	virtual void		SetMesh(FRHICommandList& RHICmdList, FShader* Shader, const FVertexFactory* VertexFactory, const FSceneView& View, const FMeshBatchElement& BatchElement, uint32 DataFlags) const override
	{
		const bool		bInstanced = FPopcornFXMeshVertexFactory::IsInstanced(View.GetFeatureLevel());
		//FMeshParticleVertexFactory* MeshParticleVF = (FMeshParticleVertexFactory*)VertexFactory;
		FVertexShaderRHIParamRef VertexShaderRHI = Shader->GetVertexShader();
		//SetUniformBufferParameter(RHICmdList, VertexShaderRHI, Shader->GetUniformBufferParameter<FMeshParticleUniformParameters>(), MeshParticleVF->GetUniformBuffer());

		const FPopcornFXMeshUserData		*instanceDatas = static_cast<const FPopcornFXMeshUserData*>(BatchElement.UserData);
		HH_ASSERT(instanceDatas != null);
		if (instanceDatas == null)
			return;
		HH_ASSERT(instanceDatas->m_Instanced == bInstanced);

		if (!instanceDatas->m_Instanced)
		{
			const hh_u32		particlei = BatchElement.UserIndex;
			if (HH_VERIFY(particlei < instanceDatas->m_Instance_Matrices.Count()))
			{
				const CFloat4x4		&matrix = instanceDatas->m_Instance_Matrices[particlei];
				const CFloat4		&color = instanceDatas->m_Instance_Param_DiffuseColors[particlei];

				SetShaderValue(RHICmdList, VertexShaderRHI, Transform1, ToUE(matrix.XAxis()));
				SetShaderValue(RHICmdList, VertexShaderRHI, Transform2, ToUE(matrix.YAxis()));
				SetShaderValue(RHICmdList, VertexShaderRHI, Transform3, ToUE(matrix.ZAxis()));
				SetShaderValue(RHICmdList, VertexShaderRHI, Transform4, ToUE(matrix.WAxis()));
				//SetShaderValue(RHICmdList, VertexShaderRHI, SubUVParams, FVector4((float)Vertex->SubUVParams[0], (float)Vertex->SubUVParams[1], (float)Vertex->SubUVParams[2], (float)Vertex->SubUVParams[3]));
				//SetShaderValue(RHICmdList, VertexShaderRHI, SubUVLerp, Vertex->SubUVLerp);
				//SetShaderValue(RHICmdList, VertexShaderRHI, ParticleDirection, Vertex->Velocity);
				//SetShaderValue(RHICmdList, VertexShaderRHI, RelativeTime, Vertex->RelativeTime);

				//SetShaderValue(RHICmdList, VertexShaderRHI, DynamicParameter, FVector4(DynamicVertex->DynamicValue[0], DynamicVertex->DynamicValue[1], DynamicVertex->DynamicValue[2], DynamicVertex->DynamicValue[3]));
				//SetShaderValue(RHICmdList, VertexShaderRHI, DynamicParameter, FVector4(1.f));

				SetShaderValue(RHICmdList, VertexShaderRHI, ParticleColor, ToUE(color));
			}
		}
	}

private:
	// Used only when instancing is off (ES2)
	FShaderParameter Transform1;
	FShaderParameter Transform2;
	FShaderParameter Transform3;
	FShaderParameter Transform4;
	//FShaderParameter SubUVParams;
	//FShaderParameter SubUVLerp;
	//FShaderParameter ParticleDirection;
	//FShaderParameter RelativeTime;
	//FShaderParameter DynamicParameter;
	FShaderParameter ParticleColor;
};

//----------------------------------------------------------------------------
//
// FPopcornFXMeshVertexFactory
//
//----------------------------------------------------------------------------

IMPLEMENT_VERTEX_FACTORY_TYPE(FPopcornFXMeshVertexFactory, PKUE_SHADER_PATH("PopcornFXMeshVertexFactory"), true, false, true, false, false);

//----------------------------------------------------------------------------

bool	FPopcornFXMeshVertexFactory::ShouldCache(EShaderPlatform Platform, const class FMaterial *Material, const class FShaderType *ShaderType)
{
	return (Material->IsUsedWithMeshParticles() || Material->IsSpecialEngineMaterial());
}

//----------------------------------------------------------------------------

bool	FPopcornFXMeshVertexFactory::IsCompatible(UMaterialInterface *material)
{
	return material->CheckMaterialUsage_Concurrent(MATUSAGE_MeshParticles);
}

//----------------------------------------------------------------------------

void	FPopcornFXMeshVertexFactory::SetData(const FDataType& InData)
{
	check(IsInRenderingThread());
	Data = InData;
	UpdateRHI();
}

//----------------------------------------------------------------------------

void	FPopcornFXMeshVertexFactory::InitRHI()
{
	FVertexDeclarationElementList Elements;

	const bool bInstanced = IsInstanced(GetFeatureLevel());

	if (Data.bInitialized)
	{
		Streams.Empty();
		if (bInstanced)
		{
			// Stream 0 - Matrices
			HH_ASSERT(Data.m_InstancedMatrices.Valid());
			{
				const hh_u32			streamIndex = 0;
				FVertexStream			*stream = new (Streams)FVertexStream();
				stream->Offset = Data.m_InstancedMatrices.Offset();
				stream->Stride = Data.m_InstancedMatrices.Stride();
				stream->VertexBuffer = Data.m_InstancedMatrices.VertexBufferPtr();

				const hh_u32	stride = Data.m_InstancedMatrices.Stride();
				Elements.Add(FVertexElement(0, sizeof(float) * 4 * 0, VET_Float4, 8, stride, true));
				Elements.Add(FVertexElement(0, sizeof(float) * 4 * 1, VET_Float4,  9, stride, true));
				Elements.Add(FVertexElement(0, sizeof(float) * 4 * 2, VET_Float4, 10, stride, true));
				Elements.Add(FVertexElement(0, sizeof(float) * 4 * 3, VET_Float4, 11, stride, true));
			}

#if 0
			// Stream 1 - Dynamic parameter
			{
				const hh_u32		streamIndex = see color below and adjust;
				FVertexStream			*stream = new (Streams)FVertexStream();
				stream->Offset = Data.m_InstancedDynParams.Offset();
				stream->Stride = Data.m_InstancedDynParams.Stride();
				stream->VertexBuffer = Data.m_InstancedDynParams.VertexBufferPtr();

				const hh_u32		stride = Data.m_InstancedDynParams->Stride();
				Elements.Add(FVertexElement(1, 0, VET_Float4, 12, stride, true));
			}
#endif

			// Stream 1 - Color
			HH_ASSERT(Data.m_InstancedColors.Valid());
			{
				const hh_u32		streamIndex = 1;
				FVertexStream		*stream = new (Streams) FVertexStream();
				stream->Offset = Data.m_InstancedColors.Offset();
				stream->Stride = Data.m_InstancedColors.Stride();
				stream->VertexBuffer = Data.m_InstancedColors.VertexBufferPtr();

				const hh_u32		stride = Data.m_InstancedColors.Stride();
				Elements.Add(FVertexElement(streamIndex, 0, VET_Float4, 13, stride, true));
			}
		}

		if (Data.PositionComponent.VertexBuffer != NULL)
		{
			Elements.Add(AccessStreamComponent(Data.PositionComponent, 0));
		}

		// only tangent,normal are used by the stream. the binormal is derived in the shader
		uint8 TangentBasisAttributes[2] = { 1, 2 };
		for (int32 AxisIndex = 0; AxisIndex < 2; AxisIndex++)
		{
			if (Data.TangentBasisComponents[AxisIndex].VertexBuffer != NULL)
			{
				Elements.Add(AccessStreamComponent(Data.TangentBasisComponents[AxisIndex], TangentBasisAttributes[AxisIndex]));
			}
		}

		// Vertex color
		if (Data.VertexColorComponent.VertexBuffer != NULL)
		{
			Elements.Add(AccessStreamComponent(Data.VertexColorComponent, 3));
		}
		else
		{
			//If the mesh has no color component, set the null color buffer on a new stream with a stride of 0.
			//This wastes 4 bytes of bandwidth per vertex, but prevents having to compile out twice the number of vertex factories.
			FVertexStreamComponent NullColorComponent(&GNullColorVertexBuffer, 0, 0, VET_Color);
			Elements.Add(AccessStreamComponent(NullColorComponent, 3));
		}

		if (Data.TextureCoordinates.Num())
		{
			const int32 BaseTexCoordAttribute = 4;
			for (int32 CoordinateIndex = 0; CoordinateIndex < Data.TextureCoordinates.Num(); CoordinateIndex++)
			{
				Elements.Add(AccessStreamComponent(
					Data.TextureCoordinates[CoordinateIndex],
					BaseTexCoordAttribute + CoordinateIndex
					));
			}

			for (int32 CoordinateIndex = Data.TextureCoordinates.Num(); CoordinateIndex < MAX_TEXCOORDS; CoordinateIndex++)
			{
				Elements.Add(AccessStreamComponent(
					Data.TextureCoordinates[Data.TextureCoordinates.Num() - 1],
					BaseTexCoordAttribute + CoordinateIndex
					));
			}
		}

		if (Streams.Num() > 0)
		{
#if (ENGINE_MINOR_VERSION >= 12)
			InitDeclaration(Elements);
#else
			InitDeclaration(Elements, Data);
#endif
			check(IsValidRef(GetDeclaration()));
		}
	}
}

//----------------------------------------------------------------------------

// static
FVertexFactoryShaderParameters* FPopcornFXMeshVertexFactory::ConstructShaderParameters(EShaderFrequency ShaderFrequency)
{
	return ShaderFrequency == SF_Vertex ? new FPopcornFXMeshVertexFactoryShaderParameters() : NULL;
}

//----------------------------------------------------------------------------

// static
void FPopcornFXMeshVertexFactory::ModifyCompilationEnvironment(EShaderPlatform Platform, const FMaterial* Material, FShaderCompilerEnvironment& OutEnvironment)
{
	FVertexFactory::ModifyCompilationEnvironment(Platform, Material, OutEnvironment);

	// @FIXME: it wont trigger recompilation !!
	OutEnvironment.SetDefine(TEXT("ENGINE_MINOR_VERSION"), ENGINE_MINOR_VERSION);

	// Set a define so we can tell in MaterialTemplate.usf when we are compiling a mesh particle vertex factory
	OutEnvironment.SetDefine(TEXT("PARTICLE_MESH_FACTORY"), TEXT("1"));

	//const bool		bInstanced = IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM4);
	const bool		bInstanced = FPopcornFXMeshVertexFactory::IsInstanced(GetMaxSupportedFeatureLevel(Platform));

	OutEnvironment.SetDefine(TEXT("PARTICLE_MESH_INSTANCED"), (uint32)(bInstanced ? 1 : 0));
}

#endif // UNUSED_PopcornFXMeshVertexFactory
