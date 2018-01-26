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

#include "PopcornFXSDK.h"
#if (PK_HAS_GPU != 0)
#	include "Render/PopcornFXBuffer.h"
#	include "Render/MyRHIStuff.h"
#	include <hh_particles/include/ps_renderers_classes.h>
#	include "GlobalShader.h"
#	include "ShaderParameterUtils.h"
#endif

//----------------------------------------------------------------------------

#define PK_BILLBOARDER_CS_OUTPUT_PACK_PTN				1
#define PK_BILLBOARDER_CS_OUTPUT_PACK_COLOR_F16			1
#define PK_BILLBOARDER_CS_OUTPUT_PACK_TEXCOORD			1

#define	PK_BILLBOARDER_CS_ASSERT_OUTPUT_STRIDE(__output, __stride)		do {	\
		if (__output == PopcornFXBillboarder::EOutput::OutPositions || __output == PopcornFXBillboarder::EOutput::OutNormals || __output == PopcornFXBillboarder::EOutput::OutTangents)	\
			HH_ASSERT(__stride == (PK_BILLBOARDER_CS_OUTPUT_PACK_PTN ? 12 : 16));	\
		else if (__output == PopcornFXBillboarder::EOutput::OutColors)	\
			HH_ASSERT(__stride == (PK_BILLBOARDER_CS_OUTPUT_PACK_COLOR_F16 ? 8 : 16));	\
		else if (__output == PopcornFXBillboarder::EOutput::OutTexcoords || __output == PopcornFXBillboarder::EOutput::OutTexcoord2s)	\
			HH_ASSERT(__stride == (PK_BILLBOARDER_CS_OUTPUT_PACK_TEXCOORD ? 4 : 8));	\
	} while (0)

//----------------------------------------------------------------------------

#if (PK_HAS_GPU != 0)

//----------------------------------------------------------------------------
namespace PopcornFXBillboarder
{
	//----------------------------------------------------------------------------
	//
	//
	//
	//----------------------------------------------------------------------------

#define EXEC_X_PK_BILLBOARDER_TYPE()				\
	X_PK_BILLBOARDER_TYPE(ScreenAlignedQuad)		\
	X_PK_BILLBOARDER_TYPE(ViewposAlignedQuad)		\
	X_PK_BILLBOARDER_TYPE(VelocityAxisAligned)		\
	X_PK_BILLBOARDER_TYPE(VelocityCapsuleAlign)		\
	X_PK_BILLBOARDER_TYPE(VelocitySpheroidalAlign)	\
	X_PK_BILLBOARDER_TYPE(PlanarAlignedQuad)

	namespace EBillboarder
	{
		enum Type
		{
#define X_PK_BILLBOARDER_TYPE(__type)		__type,
			EXEC_X_PK_BILLBOARDER_TYPE()
#undef X_PK_BILLBOARDER_TYPE
		};
#define X_PK_BILLBOARDER_TYPE(__type)		1 +
		enum { _Count = EXEC_X_PK_BILLBOARDER_TYPE() + 0 };
#undef X_PK_BILLBOARDER_TYPE
	}

	//----------------------------------------------------------------------------

	EBillboarder::Type		BillboarderModeToType(PopcornFX::CParticleRenderer_Billboard::EBillboardMode mode);

	//----------------------------------------------------------------------------

#define EXEC_X_PK_BILLBOARDER_INPUT()				\
		X_PK_BILLBOARDER_INPUT(InIndices)			\
		X_PK_BILLBOARDER_INPUT(InPositions)		\
		X_PK_BILLBOARDER_INPUT(InColors)			\
		X_PK_BILLBOARDER_INPUT(InSizes)			\
		X_PK_BILLBOARDER_INPUT(In2Sizes)			\
		X_PK_BILLBOARDER_INPUT(InRotations)		\
		X_PK_BILLBOARDER_INPUT(InAxis0s)			\
		X_PK_BILLBOARDER_INPUT(InAxis1s)			\
		X_PK_BILLBOARDER_INPUT(InTextureIds)

	namespace EInput
	{
		enum Type
		{
#define X_PK_BILLBOARDER_INPUT(__input) __input,
			EXEC_X_PK_BILLBOARDER_INPUT()
#undef X_PK_BILLBOARDER_INPUT
		};
#define X_PK_BILLBOARDER_INPUT(__type)		1 +
		enum { _Count = EXEC_X_PK_BILLBOARDER_INPUT() 0 };
#undef X_PK_BILLBOARDER_INPUT
	}

	hh_u32		BillboarderTypeMaskMustHaveInputMask(hh_u32 typeMask);

	//----------------------------------------------------------------------------

#define EXEC_X_PK_BILLBOARDER_OUTPUT()					\
		X_PK_BILLBOARDER_OUTPUT(OutIndices)				\
		X_PK_BILLBOARDER_OUTPUT(OutPositions)			\
		X_PK_BILLBOARDER_OUTPUT(OutColors)				\
		X_PK_BILLBOARDER_OUTPUT(OutTexcoords)			\
		X_PK_BILLBOARDER_OUTPUT(OutTexcoord2s)			\
		X_PK_BILLBOARDER_OUTPUT(OutNormals)				\
		X_PK_BILLBOARDER_OUTPUT(OutTangents)			\
		X_PK_BILLBOARDER_OUTPUT(OutAtlasIDs)

	namespace EOutput
	{
		enum Type
		{
#define X_PK_BILLBOARDER_OUTPUT(__output) __output,
			EXEC_X_PK_BILLBOARDER_OUTPUT()
#undef X_PK_BILLBOARDER_OUTPUT
		};
#define X_PK_BILLBOARDER_OUTPUT(__type)		1 +
		enum { _Count = EXEC_X_PK_BILLBOARDER_OUTPUT() 0 };
#undef X_PK_BILLBOARDER_OUTPUT
	}

	//----------------------------------------------------------------------------
	
	namespace ERendererFlag
	{
		enum
		{
			FlipV,
			SoftAnimationBlending,
		};
	}

	//----------------------------------------------------------------------------

	void		AddDefinesEnumTypeAndMasks(FShaderCompilerEnvironment& OutEnvironment);

	//----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	//
	//
	//
	//----------------------------------------------------------------------------

	struct FBillboarderCS_Params
	{
		uint32			m_ParticleCount = 0;
		uint32			m_BillboarderType = 0;
		uint32			m_InIndicesOffset = 0;
		uint32			m_InputOffset = 0;
		uint32			m_OutputVertexOffset = 0;
		uint32			m_OutputIndexOffset = 0;
	
		CFloat4x4		m_BillboardingMatrix;
	
		CFloat2			m_RendererAspectRatio = CFloat2(1.f);
		float			m_RendererAxisScale = 0.f;
		hh_u32			m_RendererFlags = 0;
		float			m_RendererNormalsBendingFactor = 0.f;
	
		hh_u32								m_RendererAtlasRectCount = 0;
		FShaderResourceViewRHIRef			m_RendererAtlasBuffer;
	
		FUnorderedAccessViewRHIRef			m_Outputs[EOutput::_Count];
		FShaderResourceViewRHIRef			m_Inputs[EInput::_Count];
		CFloat4								m_InputsDefault[EInput::_Count];
	
		FShaderResourceViewRHIRef			m_LiveParticleCount;

		FBillboarderCS_Params()
		{
			PopcornFX::Mem::Clear(m_InputsDefault);
			m_BillboardingMatrix = CFloat4x4::IDENTITY;
		}

		void	SetOutput(EOutput::Type output, std::nullptr_t _null)
		{
			m_Outputs[output] = null;
		}
	
		void	SetOutput(EOutput::Type output, bool condition, std::nullptr_t _null)
		{
			HH_ASSERT(!condition);
			m_Outputs[output] = null;
		}
	
		//void	SetOutput(EOutput::Type output, bool condition, const FUnorderedAccessViewRHIParamRef &uav)
		//{
		//	HH_ASSERT(IsValidRef(uav) == condition);
		//	m_Outputs[output] = uav;
		//}
	
		void	SetOutput(EOutput::Type output, bool condition, CPooledVertexBuffer &buffer)
		{
			if (condition && HH_VERIFY(buffer.Valid()))
			{
				PK_BILLBOARDER_CS_ASSERT_OUTPUT_STRIDE(output, buffer->AllocatedStride());
				m_Outputs[output] = buffer->UAV();
			}
			else
				m_Outputs[output] = null;
		}
	
		void	SetOutput(EOutput::Type output, bool condition, CPooledIndexBuffer &buffer)
		{
			if (condition && HH_VERIFY(buffer.Valid()))
			{
				PK_BILLBOARDER_CS_ASSERT_OUTPUT_STRIDE(output, buffer->AllocatedStride());
				m_Outputs[output] = buffer->UAV();
			}
			else
				m_Outputs[output] = null;
		}

		void	SetOutput(EOutput::Type output, CPooledVertexBuffer &buffer)
		{
			if (buffer.Valid())
			{
				PK_BILLBOARDER_CS_ASSERT_OUTPUT_STRIDE(output, buffer->AllocatedStride());
				m_Outputs[output] = buffer->UAV();
			}
			else
				m_Outputs[output] = null;
		}

		void	SetOutput(EOutput::Type output, CPooledIndexBuffer &buffer)
		{
			if (buffer.Valid())
			{
				PK_BILLBOARDER_CS_ASSERT_OUTPUT_STRIDE(output, buffer->AllocatedStride());
				m_Outputs[output] = buffer->UAV();
			}
			else
				m_Outputs[output] = null;
		}

		void	SetInput(EInput::Type input, bool condition, const FShaderResourceViewRHIParamRef &srv, const CFloat4 &defaultValue = CFloat4(0.f))
		{
			HH_ASSERT(!condition || IsValidRef(srv));
			if (condition)
				m_Inputs[input] = srv;
			else
				m_Inputs[input] = null;
			m_InputsDefault[input] = defaultValue;
		}

		void	SetInput(EInput::Type input, const FShaderResourceViewRHIParamRef &srv, const CFloat4 &defaultValue = CFloat4(0.f))
		{
			m_Inputs[input] = srv;
			m_InputsDefault[input] = defaultValue;
		}

		void	SetInput(EInput::Type input, std::nullptr_t _null, const CFloat4 &defaultValue = CFloat4(0.f))
		{
			m_Inputs[input] = null;
			m_InputsDefault[input] = defaultValue;
		}
	
		template <typename _Type, hh_u32 _Stride, typename _StreamType>
		bool		SetInputIFP(
			EInput::Type input,
			const _StreamType *stream,
			PopcornFX::CGuid streamId,
			const CFloat4 &defaultValue)
		{
			m_InputsDefault[input] = defaultValue;
			if (streamId.Valid())
			{
				m_Inputs[input] = StreamBufferSRVToRHI<_Type, _Stride>(stream, streamId);
				return true;
			}
			else
				m_Inputs[input] = null;
			return false;
		}
	
	};

	//----------------------------------------------------------------------------
	//
	//
	//
	//----------------------------------------------------------------------------

	namespace EBillboarderCSBuild
	{
		enum Type
		{
			Std,
			VertexPP,
		};
	}

	template <EBillboarderCSBuild::Type _Build>
	class FBillboarderCS : public FGlobalShader
	{
		DECLARE_SHADER_TYPE(FBillboarderCS, Global)

	public:
		typedef FGlobalShader			Super;

		static bool			ShouldCache(EShaderPlatform Platform);
		static void			ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment);

		FBillboarderCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
		FBillboarderCS();
	
		void			Dispatch(FRHICommandList& RHICmdList, const FBillboarderCS_Params &params);
		virtual bool	Serialize(FArchive& Ar);
	
	public:
		FShaderParameter				BillboarderType;
		FShaderParameter				OutputMask;
		FShaderParameter				InputMask;
		FShaderParameter				InIndicesOffset;
		FShaderParameter				InputOffset;
		FShaderParameter				OutputVertexOffset;
		FShaderParameter				OutputIndexOffset;
		FShaderParameter				BillboardingMatrix;
		FShaderParameter				RendererAspectRatio;
		FShaderParameter				RendererAxisScale;
		FShaderParameter				RendererFlags;
		FShaderParameter				RendererNormalsBendingFactor;
		FShaderParameter				RendererAtlasRectCount;
		FShaderResourceParameter		RendererAtlasBuffer;
		FShaderResourceParameter		Outputs[EOutput::_Count];
		FShaderResourceParameter		Inputs[EInput::_Count];
		FShaderParameter				InputsDefault[EInput::_Count];
		FShaderParameter				HasLiveParticleCount;
		FShaderResourceParameter		LiveParticleCount;
	};

	extern template class FBillboarderCS<EBillboarderCSBuild::Std>;
	extern template class FBillboarderCS<EBillboarderCSBuild::VertexPP>;

	typedef FBillboarderCS<EBillboarderCSBuild::Std>			FBillboarderCS_Std;
	typedef FBillboarderCS<EBillboarderCSBuild::VertexPP>		FBillboarderCS_VPP;
	
	//----------------------------------------------------------------------------

} // namespace PopcornFXBillboarder

#endif // (PK_HAS_GPU != 0)
