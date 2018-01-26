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

#include "PopcornFXBuffer.h"
#include "Render/PopcornFXBillboarderBillboardCS.h"

#include "RenderResource.h"
#include "VertexFactory.h"

#include "PopcornFXSDK.h"
#include <hh_defs.h>
#include <hh_kernel/include/kr_memoryviews.h>

class UMaterialInterface;

//----------------------------------------------------------------------------

class	FPopcornFXVertexFactory : public FVertexFactory
{
	DECLARE_VERTEX_FACTORY_TYPE(FPopcornFXVertexFactory);

public:
	FPopcornFXVertexFactory();
	~FPopcornFXVertexFactory();

	//FPopcornFXVertexBuffer					*m_Positions = null;
	//FPopcornFXVertexBuffer					*m_Normals = null;
	//FPopcornFXVertexBuffer					*m_Tangents = null;
	//FPopcornFXVertexBuffer					*m_Colors = null;
	//FPopcornFXVertexBuffer					*m_Texcoords = null;
	//FPopcornFXVertexBuffer					*m_Texcoord2s = null;
	//FPopcornFXVertexBuffer					*m_UVFactors = null;
	//FPopcornFXVertexBuffer					*m_UVScalesAndOffsets = null;
	//FPopcornFXVertexBuffer					*m_AtlasIDs = null;

	FPopcornFXVertexBufferView		m_Positions;
	FPopcornFXVertexBufferView		m_Normals;
	FPopcornFXVertexBufferView		m_Tangents;
	FPopcornFXVertexBufferView		m_Colors;
	FPopcornFXVertexBufferView		m_Texcoords;
	FPopcornFXVertexBufferView		m_Texcoord2s;
	FPopcornFXVertexBufferView		m_UVFactors;
	FPopcornFXVertexBufferView		m_UVScalesAndOffsets;
	FPopcornFXVertexBufferView		m_AtlasIDs;
	FPopcornFXVertexBufferView		m_AlphaCursors;

	/** Should we cache the material's shadertype on this platform with this vertex factory? */
	static void			ModifyCompilationEnvironment(EShaderPlatform Platform, const FMaterial* Material, FShaderCompilerEnvironment& OutEnvironment);
	static bool			ShouldCache(EShaderPlatform platform, const class FMaterial *material, const class FShaderType *shaderType);
	static bool			IsCompatible(UMaterialInterface *material);

	/** Copy the data from another vertex factory */
	//void									Copy(const FPopcornFXVertexFactory &other);

	/** FRenderResource interface */
	virtual void							InitRHI() override;

	/** Does the vertex factory supports tesselation shaders */
	static bool								SupportsTessellationShaders() { return false; }

	/** Construct the corresponding shader parameters */
	static FVertexFactoryShaderParameters	*ConstructShaderParameters(EShaderFrequency shaderFrequency);

private:
	void			_SetupStream(const hh_u32 attributeIndex, FPopcornFXVertexBufferView &buffer, EVertexElementType type, FVertexDeclarationElementList &decl, TArray<FVertexStream,TFixedAllocator<MaxVertexElementCount> > &streams);

};
