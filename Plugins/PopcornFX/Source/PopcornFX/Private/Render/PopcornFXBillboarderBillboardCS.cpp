//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXBillboarderBillboardCS.h"

#if (PK_HAS_GPU != 0)

#include "Render/PopcornFXShaderUtils.h"

#include "SceneUtils.h"

#define CS_BB_THREADGROUP_SIZE		128

//----------------------------------------------------------------------------
namespace PopcornFXBillboarder
{
	//----------------------------------------------------------------------------

	EBillboarder::Type		BillboarderModeToType(PopcornFX::CParticleRenderer_Billboard::EBillboardMode mode)
	{
		EBillboarder::Type		bbType = (EBillboarder::Type)~0;
		switch (mode)
		{
		case	PopcornFX::CParticleRenderer_Billboard::ScreenPoint:
		case	PopcornFX::CParticleRenderer_Billboard::ScreenAlignedTriangle:
			// deprecated, not handled
			break;
		case	PopcornFX::CParticleRenderer_Billboard::ScreenAlignedQuad:
			bbType = EBillboarder::ScreenAlignedQuad;
			break;
		case	PopcornFX::CParticleRenderer_Billboard::ViewposAlignedQuad:
			bbType = EBillboarder::ViewposAlignedQuad;
			break;
		case	PopcornFX::CParticleRenderer_Billboard::VelocityAxisAligned:
			bbType = EBillboarder::VelocityAxisAligned;
			break;
		case	PopcornFX::CParticleRenderer_Billboard::VelocitySpheroidalAlign:
			bbType = EBillboarder::VelocitySpheroidalAlign;
			break;
		case	PopcornFX::CParticleRenderer_Billboard::PlanarAlignedQuad:
			bbType = EBillboarder::PlanarAlignedQuad;
			break;
		case	PopcornFX::CParticleRenderer_Billboard::VelocityCapsuleAlign:
			bbType = EBillboarder::VelocityCapsuleAlign;
			break;
		default:
			HH_ASSERT_NOT_REACHED();
			break;
		}
		return bbType;
	}

	//----------------------------------------------------------------------------
	
	hh_u32		BillboarderTypeMaskMustHaveInputMask(hh_u32 typeMask)
	{
		HH_STATIC_ASSERT(EBillboarder::_Count < sizeof(typeMask) * 8);
		HH_STATIC_ASSERT(EInput::_Count < sizeof(typeMask) * 8);

		hh_u32		inputMask = 0;
		inputMask |= (1 << EInput::InPositions);
		if (typeMask & (1 << EBillboarder::ScreenAlignedQuad))
		{
		}
		if (typeMask & (1 << EBillboarder::ViewposAlignedQuad))
		{
		}
		if (typeMask & (1 << EBillboarder::VelocityAxisAligned))
		{
			inputMask |= (1 << EInput::InAxis0s);
		}
		if (typeMask & (1 << EBillboarder::VelocityCapsuleAlign))
		{
			inputMask |= (1 << EInput::InAxis0s);
		}
		if (typeMask & (1 << EBillboarder::VelocitySpheroidalAlign))
		{
			inputMask |= (1 << EInput::InAxis0s);
		}
		if (typeMask & (1 << EBillboarder::PlanarAlignedQuad))
		{
			inputMask |= (1 << EInput::InAxis0s);
			inputMask |= (1 << EInput::InAxis1s);
		}
		HH_STATIC_ASSERT(EBillboarder::_Count == 6); // check above
		return inputMask;
	}

	//----------------------------------------------------------------------------

	void		AddDefinesEnumTypeAndMasks(FShaderCompilerEnvironment& OutEnvironment)
	{
#define X_PK_BILLBOARDER_TYPE(__type)		\
	HH_ASSERT(!OutEnvironment.GetDefinitions().Contains(TEXT("BILLBOARD_") TEXT(#__type))); \
	OutEnvironment.SetDefine(TEXT("BILLBOARD_") TEXT(#__type), uint32(EBillboarder::__type));
		EXEC_X_PK_BILLBOARDER_TYPE()
#undef X_PK_BILLBOARDER_TYPE

#define X_PK_BILLBOARDER_TYPE(__type) \
	HH_ASSERT(!OutEnvironment.GetDefinitions().Contains(TEXT("BBMASK_") TEXT(#__type))); \
	OutEnvironment.SetDefine(TEXT("BBMASK_") TEXT(#__type), 1 << uint32(EBillboarder::__type));
		EXEC_X_PK_BILLBOARDER_TYPE()
#undef X_PK_BILLBOARDER_TYPE

#define X_PK_BILLBOARDER_OUTPUT(__output) \
	HH_ASSERT(!OutEnvironment.GetDefinitions().Contains(TEXT("MASK_") TEXT(#__output))); \
	OutEnvironment.SetDefine(TEXT("MASK_") TEXT(#__output), uint32(1 << (EOutput:: __output)));
		EXEC_X_PK_BILLBOARDER_OUTPUT()
#undef X_PK_BILLBOARDER_OUTPUT

#define X_PK_BILLBOARDER_INPUT(__input) \
	HH_ASSERT(!OutEnvironment.GetDefinitions().Contains(TEXT("MASK_") TEXT(#__input))); \
	OutEnvironment.SetDefine(TEXT("MASK_") TEXT(#__input), uint32(1 << (EInput:: __input)));
		EXEC_X_PK_BILLBOARDER_INPUT()
#undef X_PK_BILLBOARDER_INPUT
	}


	//----------------------------------------------------------------------------
	//
	//
	//
	//----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	//
	//
	//
	//----------------------------------------------------------------------------

	// static
	template <EBillboarderCSBuild::Type _Build>
	bool	FBillboarderCS<_Build>::ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}

	//----------------------------------------------------------------------------

	//static
	template <EBillboarderCSBuild::Type _Build>
	void	FBillboarderCS<_Build>::ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		Super::ModifyCompilationEnvironment(Platform, OutEnvironment);

		OutEnvironment.SetDefine(TEXT("PK_GPU_THREADGROUP_SIZE"), CS_BB_THREADGROUP_SIZE);

		AddDefinesEnumTypeAndMasks(OutEnvironment);

		OutEnvironment.SetDefine(TEXT("RENDERERFLAG_FlipV"), 1 << ERendererFlag::FlipV);
		OutEnvironment.SetDefine(TEXT("RENDERERFLAG_SoftAnimationBlending"), 1 << ERendererFlag::SoftAnimationBlending);

		OutEnvironment.SetDefine(TEXT("PK_BILLBOARDER_CS_OUTPUT_PACK_PTN"), uint32(PK_BILLBOARDER_CS_OUTPUT_PACK_PTN));
		OutEnvironment.SetDefine(TEXT("PK_BILLBOARDER_CS_OUTPUT_PACK_COLOR_F16"), uint32(PK_BILLBOARDER_CS_OUTPUT_PACK_COLOR_F16));
		OutEnvironment.SetDefine(TEXT("PK_BILLBOARDER_CS_OUTPUT_PACK_TEXCOORD"), uint32(PK_BILLBOARDER_CS_OUTPUT_PACK_TEXCOORD));

		switch (_Build)
		{
		case EBillboarderCSBuild::Std:
			break;
		case EBillboarderCSBuild::VertexPP:
			OutEnvironment.SetDefine(TEXT("VERTEX_PER_PARTICLE"), 1);
			break;
		}

		//OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);

	}

	//----------------------------------------------------------------------------

	template <EBillboarderCSBuild::Type _Build>
	FBillboarderCS<_Build>::FBillboarderCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		BillboarderType.Bind(Initializer.ParameterMap, TEXT("BillboarderType"));
		OutputMask.Bind(Initializer.ParameterMap, TEXT("OutputMask"));
		InputMask.Bind(Initializer.ParameterMap, TEXT("InputMask"));
		InIndicesOffset.Bind(Initializer.ParameterMap, TEXT("InIndicesOffset"));
		InputOffset.Bind(Initializer.ParameterMap, TEXT("InputOffset"));
		OutputVertexOffset.Bind(Initializer.ParameterMap, TEXT("OutputVertexOffset"));
		OutputIndexOffset.Bind(Initializer.ParameterMap, TEXT("OutputIndexOffset"));
		BillboardingMatrix.Bind(Initializer.ParameterMap, TEXT("BillboardingMatrix"));
		RendererAspectRatio.Bind(Initializer.ParameterMap, TEXT("RendererAspectRatio"));
		RendererAxisScale.Bind(Initializer.ParameterMap, TEXT("RendererAxisScale"));
		RendererFlags.Bind(Initializer.ParameterMap, TEXT("RendererFlags"));
		RendererNormalsBendingFactor.Bind(Initializer.ParameterMap, TEXT("RendererNormalsBendingFactor"));

		RendererAtlasRectCount.Bind(Initializer.ParameterMap, TEXT("RendererAtlasRectCount"));
		RendererAtlasBuffer.Bind(Initializer.ParameterMap, TEXT("RendererAtlasBuffer"));

#define X_PK_BILLBOARDER_OUTPUT(__output) Outputs[EOutput :: __output].Bind(Initializer.ParameterMap, TEXT(#__output));
		EXEC_X_PK_BILLBOARDER_OUTPUT()
#undef X_PK_BILLBOARDER_OUTPUT
#define X_PK_BILLBOARDER_INPUT(__input)																\
			Inputs[EInput::__input].Bind(Initializer.ParameterMap, TEXT(#__input));		\
			InputsDefault[EInput::__input].Bind(Initializer.ParameterMap, TEXT("Default") TEXT(#__input));
			EXEC_X_PK_BILLBOARDER_INPUT()
#undef X_PK_BILLBOARDER_INPUT

		HasLiveParticleCount.Bind(Initializer.ParameterMap, TEXT("HasLiveParticleCount"));
		LiveParticleCount.Bind(Initializer.ParameterMap, TEXT("LiveParticleCount"));
	}

	//----------------------------------------------------------------------------

	template <EBillboarderCSBuild::Type _Build>
	FBillboarderCS<_Build>::FBillboarderCS()
	{
	}

	//----------------------------------------------------------------------------

	template <EBillboarderCSBuild::Type _Build>
	void	FBillboarderCS<_Build>::Dispatch(FRHICommandList& RHICmdList, const FBillboarderCS_Params &params)
	{
		SCOPED_DRAW_EVENT(RHICmdList, PopcornFXBillboarderBillboard_Dispatch);

		const bool		isVPP = (_Build == EBillboarderCSBuild::VertexPP);

		FComputeShaderRHIParamRef	shader = GetComputeShader();

		RHICmdList.SetComputeShader(shader);

		uint32				outputMask = 0;
		uint32				inputMask = 0;

		if (isVPP)
		{
			HH_ASSERT(!IsValidRef(params.m_Outputs[EOutput::OutTexcoords]));
			HH_ASSERT(!IsValidRef(params.m_Outputs[EOutput::OutTexcoord2s]));
			HH_ASSERT(!IsValidRef(params.m_Outputs[EOutput::OutTangents]));
		}

		for (hh_u32 i = 0; i < EOutput::_Count; ++i)
		{
			if (IsValidRef(params.m_Outputs[i]))
			{
				outputMask |= (1 << i); // if buffer is provided, always set the mask, even if not used (not bound)
				if (Outputs[i].IsBound())
					RHICmdList.SetUAVParameter(shader, Outputs[i].GetBaseIndex(), params.m_Outputs[i]);
			}
			else if (Outputs[i].IsBound())
				RHICmdList.SetUAVParameter(shader, Outputs[i].GetBaseIndex(), null); // avoids D3D11 warnings !?
		}
		for (hh_u32 i = 0; i < EInput::_Count; ++i)
		{
			if (IsValidRef(params.m_Inputs[i]))
			{
				inputMask |= (1 << i); // if buffer is provided, always set the mask, even if not used (not bound)
				if (Inputs[i].IsBound())
					RHICmdList.SetShaderResourceViewParameter(shader, Inputs[i].GetBaseIndex(), params.m_Inputs[i]);
			}
			else if (Inputs[i].IsBound())
				RHICmdList.SetShaderResourceViewParameter(shader, Inputs[i].GetBaseIndex(), null); // avoids D3D11 warnings !?

			SetShaderValue(RHICmdList, shader, InputsDefault[i], _Reinterpret<FVector4>(params.m_InputsDefault[i]));
		}

		HH_ONLY_IF_ASSERTS(const hh_u32		mustHaveInputMask = BillboarderTypeMaskMustHaveInputMask(1 << params.m_BillboarderType));
		HH_ASSERT((inputMask & mustHaveInputMask) == mustHaveInputMask);

		SetShaderValue(RHICmdList, shader, OutputMask, outputMask);
		SetShaderValue(RHICmdList, shader, InputMask, inputMask);
		SetShaderValue(RHICmdList, shader, BillboarderType, params.m_BillboarderType);
		SetShaderValue(RHICmdList, shader, InIndicesOffset, params.m_InIndicesOffset);
		SetShaderValue(RHICmdList, shader, InputOffset, params.m_InputOffset);
		SetShaderValue(RHICmdList, shader, OutputVertexOffset, params.m_OutputVertexOffset);
		SetShaderValue(RHICmdList, shader, OutputIndexOffset, params.m_OutputIndexOffset);
		SetShaderValue(RHICmdList, shader, BillboardingMatrix, ToUE(params.m_BillboardingMatrix));
		SetShaderValue(RHICmdList, shader, RendererAspectRatio, _Reinterpret<FVector2D>(params.m_RendererAspectRatio));
		SetShaderValue(RHICmdList, shader, RendererAxisScale, params.m_RendererAxisScale);
		SetShaderValue(RHICmdList, shader, RendererFlags, params.m_RendererFlags);
		SetShaderValue(RHICmdList, shader, RendererNormalsBendingFactor, params.m_RendererNormalsBendingFactor);

		if (IsValidRef(params.m_RendererAtlasBuffer) &&
			RendererAtlasBuffer.IsBound())
		{
			RHICmdList.SetShaderResourceViewParameter(shader, RendererAtlasBuffer.GetBaseIndex(), params.m_RendererAtlasBuffer);
			SetShaderValue(RHICmdList, shader, RendererAtlasRectCount, params.m_RendererAtlasRectCount);
		}
		else
		{
			if (RendererAtlasBuffer.IsBound())
				RHICmdList.SetShaderResourceViewParameter(shader, RendererAtlasBuffer.GetBaseIndex(), null); // avoids D3D11 warnings !?
			SetShaderValue(RHICmdList, shader, RendererAtlasRectCount, hh_u32(0));
		}

		uint32		hasLiveParticleCount = 0;
		if (IsValidRef(params.m_LiveParticleCount))
			hasLiveParticleCount = 1;
		SetShaderValue(RHICmdList, shader, HasLiveParticleCount, hasLiveParticleCount);
		if (IsValidRef(params.m_LiveParticleCount) && LiveParticleCount.IsBound())
			RHICmdList.SetShaderResourceViewParameter(shader, LiveParticleCount.GetBaseIndex(), params.m_LiveParticleCount);

		{
			const uint32	threadGroupCount = PopcornFX::Mem::Align(params.m_ParticleCount, CS_BB_THREADGROUP_SIZE) / CS_BB_THREADGROUP_SIZE;
			RHICmdList.DispatchComputeShader(threadGroupCount, 1, 1);
		}

		FUnorderedAccessViewRHIParamRef			nullUAV = FUnorderedAccessViewRHIParamRef();
		FShaderResourceViewRHIParamRef			nullSRV = FShaderResourceViewRHIParamRef();
		for (hh_u32 i = 0; i < EOutput::_Count; ++i)
			if (IsValidRef(params.m_Outputs[i]) && Outputs[i].IsBound())
				RHICmdList.SetUAVParameter(shader, Outputs[i].GetBaseIndex(), nullUAV);
		for (hh_u32 i = 0; i < EInput::_Count; ++i)
			if (IsValidRef(params.m_Inputs[i]) && Inputs[i].IsBound())
				RHICmdList.SetShaderResourceViewParameter(shader, Inputs[i].GetBaseIndex(), nullSRV);
	}

	//----------------------------------------------------------------------------

	template <EBillboarderCSBuild::Type _Build>
	bool	FBillboarderCS<_Build>::Serialize(FArchive& Ar)
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << BillboarderType;
		Ar << OutputMask;
		Ar << InputMask;
		Ar << InIndicesOffset;
		Ar << InputOffset;
		Ar << OutputVertexOffset;
		Ar << OutputIndexOffset;
		Ar << BillboardingMatrix;
		Ar << RendererAspectRatio;
		Ar << RendererAxisScale;
		Ar << RendererFlags;
		Ar << RendererNormalsBendingFactor;
		Ar << RendererAtlasRectCount;
		Ar << RendererAtlasBuffer;
		for (hh_u32 i = 0; i < EOutput::_Count; ++i)
			Ar << Outputs[i];
		for (hh_u32 i = 0; i < EInput::_Count; ++i)
			Ar << Inputs[i];
		for (hh_u32 i = 0; i < EInput::_Count; ++i)
			Ar << InputsDefault[i];
		Ar << HasLiveParticleCount;
		Ar << LiveParticleCount;
		return bShaderHasOutdatedParameters;
	}


	//----------------------------------------------------------------------------

	template class FBillboarderCS<EBillboarderCSBuild::Std>;
	template class FBillboarderCS<EBillboarderCSBuild::VertexPP>;

	//----------------------------------------------------------------------------
} // namespace PopcornFXBillboarder


//----------------------------------------------------------------------------

IMPLEMENT_SHADER_TYPE(template<>, PopcornFXBillboarder::FBillboarderCS<PopcornFXBillboarder::EBillboarderCSBuild::Std>, TEXT(PKUE_SHADER_PATH("PopcornFXBillboarderBillboardComputeShader")), TEXT("GogoBillobardCS"), SF_Compute);
IMPLEMENT_SHADER_TYPE(template<>, PopcornFXBillboarder::FBillboarderCS<PopcornFXBillboarder::EBillboarderCSBuild::VertexPP>, TEXT(PKUE_SHADER_PATH("PopcornFXBillboarderBillboardComputeShader")), TEXT("GogoBillobardCS"), SF_Compute);

//----------------------------------------------------------------------------

#endif // (PK_HAS_GPU != 0)
