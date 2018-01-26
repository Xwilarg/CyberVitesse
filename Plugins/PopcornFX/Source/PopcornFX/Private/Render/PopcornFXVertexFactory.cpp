//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXVertexFactory.h"

#include "PopcornFXVertexFactoryShaderParameters.h"

#include "Render/PopcornFXShaderUtils.h"

#include "MaterialShared.h"
#include "Materials/MaterialInterface.h"

void			FPopcornFXVertexFactory::_SetupStream(const hh_u32 attributeIndex, FPopcornFXVertexBufferView &buffer, EVertexElementType type, FVertexDeclarationElementList &decl, TArray<FVertexStream,TFixedAllocator<MaxVertexElementCount> > &streams)
{
	if (!buffer.Valid())
	{
		FVertexStreamComponent NullColorComponent(&GNullColorVertexBuffer, 0, 0, VET_Color);
		decl.Add(AccessStreamComponent(NullColorComponent, attributeIndex));
		return;
	}

	const hh_u32		streamIndex = streams.Num();
	FVertexStream		*stream = new (streams) FVertexStream();
	stream->Offset = buffer.Offset();
	stream->Stride = buffer.Stride();
	stream->VertexBuffer = buffer.VertexBufferPtr();

	decl.Add(FVertexElement(streamIndex, 0, type, attributeIndex, buffer.Stride(), false));

}

//----------------------------------------------------------------------------
//
// FPopcornFXVertexFactory
//
//----------------------------------------------------------------------------

IMPLEMENT_VERTEX_FACTORY_TYPE(FPopcornFXVertexFactory, PKUE_SHADER_PATH("PopcornFXVertexFactory"), true, false, true, false, false);

//----------------------------------------------------------------------------

FPopcornFXVertexFactory::FPopcornFXVertexFactory()
{
	check(IsInRenderingThread());
}

//----------------------------------------------------------------------------

FPopcornFXVertexFactory::~FPopcornFXVertexFactory()
{
	check(IsInRenderingThread());

}

//----------------------------------------------------------------------------

void	FPopcornFXVertexFactory::ModifyCompilationEnvironment(EShaderPlatform Platform, const FMaterial* Material, FShaderCompilerEnvironment& OutEnvironment) 
{
	FVertexFactory::ModifyCompilationEnvironment(Platform, Material, OutEnvironment);

	// @FIXME: it wont trigger recompilation !!
	OutEnvironment.SetDefine(TEXT("ENGINE_MINOR_VERSION"), ENGINE_MINOR_VERSION);

	// 4.9: Needed to have DynamicParameters
	OutEnvironment.SetDefine(TEXT("PARTICLE_FACTORY"),TEXT("1"));
}

//----------------------------------------------------------------------------

bool	FPopcornFXVertexFactory::ShouldCache(EShaderPlatform platform, const class FMaterial *material, const class FShaderType *shaderType)
{
	return material->IsUsedWithParticleSprites() || material->IsSpecialEngineMaterial();
}

//----------------------------------------------------------------------------

bool	FPopcornFXVertexFactory::IsCompatible(UMaterialInterface *material)
{
	return material->CheckMaterialUsage_Concurrent(MATUSAGE_ParticleSprites);
}

//----------------------------------------------------------------------------

void	FPopcornFXVertexFactory::InitRHI()
{
	FVertexDeclarationElementList	vDeclElements;
	Streams.Empty();

	_SetupStream(0, m_Positions, VET_Float3, vDeclElements, Streams);
	_SetupStream(1, m_Normals, VET_Float3, vDeclElements, Streams);
	_SetupStream(2, m_Tangents, VET_Float4, vDeclElements, Streams);

	HH_RELEASE_ASSERT(m_Colors.Stride() == 0 || m_Colors.Stride() == 0x10 || m_Colors.Stride() == 8);
	_SetupStream(3, m_Colors, m_Colors.Stride() == 8 ? VET_Half4 : VET_Float4, vDeclElements, Streams);

	HH_RELEASE_ASSERT(m_Texcoords.Stride() == 0 || m_Texcoords.Stride() == 8 || m_Texcoords.Stride() == 4);
	_SetupStream(4, m_Texcoords, m_Texcoords.Stride() == 4 ? VET_UShort2N : VET_Float2, vDeclElements, Streams);
	HH_RELEASE_ASSERT(m_Texcoord2s.Stride() == 0 || m_Texcoord2s.Stride() == 8 || m_Texcoord2s.Stride() == 4);
	_SetupStream(5, m_Texcoord2s, m_Texcoord2s.Stride() == 4 ? VET_UShort2N : VET_Float2, vDeclElements, Streams);

	_SetupStream(6, m_UVFactors, VET_Float4, vDeclElements, Streams);
	_SetupStream(7, m_UVScalesAndOffsets, VET_Float4, vDeclElements, Streams);
	_SetupStream(8, m_AtlasIDs, VET_Float1, vDeclElements, Streams);
	_SetupStream(9, m_AlphaCursors, VET_Float1, vDeclElements, Streams);

#if (ENGINE_MINOR_VERSION >= 12)
	InitDeclaration(vDeclElements);
#else
	InitDeclaration(vDeclElements, FVertexFactory::DataType());
#endif
}

//----------------------------------------------------------------------------

FVertexFactoryShaderParameters	*FPopcornFXVertexFactory::ConstructShaderParameters(EShaderFrequency shaderFrequency)
{
	// Create the corresponding shader parameters, depending on the shader type
	switch (shaderFrequency)
	{
		case	SF_Vertex:
			return new FPopcornFXVertexFactoryShaderParametersVertex();
			break;
		case	SF_Pixel:
			return new FPopcornFXVertexFactoryShaderParametersPixel();
			break;
		default:
			return null;
	}
	return null;
}
