//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "PopcornFXSortComputeShader.h"

#include "Render/MyRHIStuff.h"
#include "Render/PopcornFXShaderUtils.h"

#include "ShaderParameterUtils.h"
#include "SceneUtils.h"

#if (PK_GPU_D3D11 == 1)

//----------------------------------------------------------------------------

#define CS_SORT_THREADGROUP_SIZE	256

//----------------------------------------------------------------------------
//
// FPopcornFXSortComputeShader_GenKeys
//
//----------------------------------------------------------------------------

class FPopcornFXSortComputeShader_GenKeys : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FPopcornFXSortComputeShader_GenKeys, Global)

public:
	typedef FGlobalShader			Super;

	static bool			ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}
	static void			ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		Super::ModifyCompilationEnvironment(Platform, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("DO_GenKeysCS"), 1);
		OutEnvironment.SetDefine(TEXT("PK_GPU_THREADGROUP_SIZE"), CS_SORT_THREADGROUP_SIZE);
		//OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
	}

	FPopcornFXSortComputeShader_GenKeys(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: Super(Initializer)
	{
		TotalCount.Bind(Initializer.ParameterMap, TEXT("TotalCount"));
		SortOrigin.Bind(Initializer.ParameterMap, TEXT("SortOrigin"));
		IndexStart.Bind(Initializer.ParameterMap, TEXT("IndexStart"));
		IndexStep.Bind(Initializer.ParameterMap, TEXT("IndexStep"));
		InputOffset.Bind(Initializer.ParameterMap, TEXT("InputOffset"));
		OutputOffset.Bind(Initializer.ParameterMap, TEXT("OutputOffset"));
		InPositions.Bind(Initializer.ParameterMap, TEXT("InPositions"));
		OutKeys.Bind(Initializer.ParameterMap, TEXT("OutKeys"));
		OutValues.Bind(Initializer.ParameterMap, TEXT("OutValues"));
	}

	FPopcornFXSortComputeShader_GenKeys() { }

	virtual bool		Serialize(FArchive& Ar)
	{
		bool bShaderHasOutdatedParameters = Super::Serialize(Ar);
		Ar << TotalCount;
		Ar << SortOrigin;
		Ar << IndexStart;
		Ar << IndexStep;
		Ar << InputOffset;
		Ar << OutputOffset;
		Ar << InPositions;
		Ar << OutKeys;
		Ar << OutValues;
		return bShaderHasOutdatedParameters;
	}

	//void		Dispatch(FRHICommandList& RHICmdList, const FPopcornFXSortComputeShader_GenKeys_Params &params);

public:
	FShaderParameter				TotalCount;
	FShaderParameter				SortOrigin;
	FShaderParameter				IndexStart;
	FShaderParameter				IndexStep;
	FShaderParameter				InputOffset;
	FShaderParameter				OutputOffset;
	FShaderResourceParameter		InPositions;
	FShaderResourceParameter		OutKeys;
	FShaderResourceParameter		OutValues;
};


IMPLEMENT_SHADER_TYPE(, FPopcornFXSortComputeShader_GenKeys, TEXT(POPCRONFX_SHADER_PATH("PopcornFXSortComputeShader")), TEXT("GenKeysCS"), SF_Compute);

//----------------------------------------------------------------------------
//
// FPopcornFXSortComputeShader_Sort_UpSweep
//
//----------------------------------------------------------------------------

class FPopcornFXSortComputeShader_Sort_UpSweep : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FPopcornFXSortComputeShader_Sort_UpSweep, Global)
public:
	typedef FGlobalShader	Super;
	static bool				ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}
	static void				ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		Super::ModifyCompilationEnvironment(Platform, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("DO_SortUpSweep"), 1);
		OutEnvironment.SetDefine(TEXT("PK_GPU_THREADGROUP_SIZE"), CS_SORT_THREADGROUP_SIZE);
		//OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
	}
	FPopcornFXSortComputeShader_Sort_UpSweep(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: Super(Initializer)
	{
		KeyBitOffset.Bind(Initializer.ParameterMap, TEXT("KeyBitOffset"));
		InKeys.Bind(Initializer.ParameterMap, TEXT("InKeys"));
		OutOffsets.Bind(Initializer.ParameterMap, TEXT("OutOffsets"));
	}
	FPopcornFXSortComputeShader_Sort_UpSweep() {}
	virtual bool			Serialize(FArchive& Ar)
	{
		bool bShaderHasOutdatedParameters = Super::Serialize(Ar);
		Ar << KeyBitOffset;
		Ar << InKeys;
		Ar << OutOffsets;
		return bShaderHasOutdatedParameters;
	}
public:
	FShaderParameter				KeyBitOffset;
	FShaderResourceParameter		InKeys;
	FShaderResourceParameter		OutOffsets;
};

IMPLEMENT_SHADER_TYPE(, FPopcornFXSortComputeShader_Sort_UpSweep, TEXT(PKUE_SHADER_PATH("PopcornFXSortComputeShader")), TEXT("SortUpSweep"), SF_Compute);

//----------------------------------------------------------------------------
//
// FPopcornFXSortComputeShader_Sort_UpSweepOffsets
//
//----------------------------------------------------------------------------

class FPopcornFXSortComputeShader_Sort_UpSweepOffsets : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FPopcornFXSortComputeShader_Sort_UpSweepOffsets, Global)
public:
	typedef FGlobalShader	Super;
	static bool				ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}
	static void				ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		Super::ModifyCompilationEnvironment(Platform, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("DO_SortUpSweepOffsets"), 1);
		OutEnvironment.SetDefine(TEXT("PK_GPU_THREADGROUP_SIZE"), CS_SORT_THREADGROUP_SIZE);
		//OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
	}
	FPopcornFXSortComputeShader_Sort_UpSweepOffsets(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: Super(Initializer)
	{
		GroupCount.Bind(Initializer.ParameterMap, TEXT("GroupCount"));
		InOutOffsets.Bind(Initializer.ParameterMap, TEXT("InOutOffsets"));
	}
	FPopcornFXSortComputeShader_Sort_UpSweepOffsets() {}
	virtual bool			Serialize(FArchive& Ar)
	{
		bool bShaderHasOutdatedParameters = Super::Serialize(Ar);
		Ar << GroupCount;
		Ar << InOutOffsets;
		return bShaderHasOutdatedParameters;
	}
public:
	FShaderParameter				GroupCount;
	FShaderResourceParameter		InOutOffsets;
};

IMPLEMENT_SHADER_TYPE(, FPopcornFXSortComputeShader_Sort_UpSweepOffsets, TEXT(PKUE_SHADER_PATH("PopcornFXSortComputeShader")), TEXT("SortUpSweepOffsets"), SF_Compute);

//----------------------------------------------------------------------------
//
// FPopcornFXSortComputeShader_Sort_DownSweep
//
//----------------------------------------------------------------------------

class FPopcornFXSortComputeShader_Sort_DownSweep : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FPopcornFXSortComputeShader_Sort_DownSweep, Global)
public:
	typedef FGlobalShader	Super;
	static bool				ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}
	static void				ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		Super::ModifyCompilationEnvironment(Platform, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("DO_SortDownSweep"), 1);
		OutEnvironment.SetDefine(TEXT("PK_GPU_THREADGROUP_SIZE"), CS_SORT_THREADGROUP_SIZE);
		//OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
	}
	FPopcornFXSortComputeShader_Sort_DownSweep(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: Super(Initializer)
	{
		KeyBitOffset.Bind(Initializer.ParameterMap, TEXT("KeyBitOffset"));
		GroupCount.Bind(Initializer.ParameterMap, TEXT("GroupCount"));
		InOffsets.Bind(Initializer.ParameterMap, TEXT("InOffsets"));
		InKeys.Bind(Initializer.ParameterMap, TEXT("InKeys"));
		InValues.Bind(Initializer.ParameterMap, TEXT("InValues"));
		OutKeys.Bind(Initializer.ParameterMap, TEXT("OutKeys"));
		OutValues.Bind(Initializer.ParameterMap, TEXT("OutValues"));
	}
	FPopcornFXSortComputeShader_Sort_DownSweep() {}
	virtual bool			Serialize(FArchive& Ar)
	{
		bool bShaderHasOutdatedParameters = Super::Serialize(Ar);
		Ar << KeyBitOffset;
		Ar << GroupCount;
		Ar << InOffsets;
		Ar << InKeys;
		Ar << InValues;
		Ar << OutKeys;
		Ar << OutValues;
		return bShaderHasOutdatedParameters;
	}
public:
	FShaderParameter				KeyBitOffset;
	FShaderParameter				GroupCount;
	FShaderResourceParameter		InOffsets;
	FShaderResourceParameter		InKeys;
	FShaderResourceParameter		InValues;
	FShaderResourceParameter		OutKeys;
	FShaderResourceParameter		OutValues;
};

IMPLEMENT_SHADER_TYPE(, FPopcornFXSortComputeShader_Sort_DownSweep, TEXT(PKUE_SHADER_PATH("PopcornFXSortComputeShader")), TEXT("SortDownSweep"), SF_Compute);

//----------------------------------------------------------------------------
//
// FPopcornFXSortComputeShader_Sorter
//
//----------------------------------------------------------------------------

void	FPopcornFXSortComputeShader_Sorter::Clear()
{
	m_TotalCount = 0;
	m_CurrBuff = 0;
	m_CurrGenOutputOffset = 0;
	for (hh_u32 i = 0; i < 2; ++i)
	{
		m_Keys[i].Clear();
		m_Values[i].Clear();
	}
	m_Cache.Clear();
}

//----------------------------------------------------------------------------

bool	FPopcornFXSortComputeShader_Sorter::Prepare(CVertexBufferPool &vbpool, hh_u32 totalCount)
{
	m_CurrBuff = 0;
	m_CurrGenOutputOffset = 0;

	m_TotalCount = 0;
	const hh_u32		alignedCount = PopcornFX::Mem::Align<CS_SORT_THREADGROUP_SIZE>(totalCount);
	const hh_u32		cacheCount = PopcornFX::Mem::Align<CS_SORT_THREADGROUP_SIZE>(alignedCount / CS_SORT_THREADGROUP_SIZE);

	for (hh_u32 i = 0; i < 2; ++i)
	{
		if (!vbpool.AllocateIf(true, m_Keys[i], alignedCount, sizeof(hh_u32)))
			return false;
		if (!vbpool.AllocateIf(true, m_Values[i], alignedCount, sizeof(hh_u32)))
			return false;
	}
	if (!vbpool.AllocateIf(true, m_Cache, cacheCount, sizeof(hh_u32)))
		return false;
	m_TotalCount = totalCount;
	return true;
}

//----------------------------------------------------------------------------

void	FPopcornFXSortComputeShader_Sorter::DispatchGenIndiceBatch(FRHICommandList& RHICmdList, const FPopcornFXSortComputeShader_GenKeys_Params &params)
{
	HH_ASSERT(Ready());

	SCOPED_DRAW_EVENT(RHICmdList, PopcornFXSortComputeShader_Sorter_GenIndices);

	const ERHIFeatureLevel::Type	featureLevel = ERHIFeatureLevel::SM5; // @FIXME, true feature level ?
	TShaderMapRef< FPopcornFXSortComputeShader_GenKeys >	genKeys(GetGlobalShaderMap(featureLevel));
	FComputeShaderRHIParamRef								shader = genKeys->GetComputeShader();

	HH_ASSERT(params.m_Count > 0);

	RHICmdList.SetComputeShader(shader);

	if (HH_VERIFY(genKeys->InPositions.IsBound()) &&
		HH_VERIFY(IsValidRef(params.m_InputPositions)))
		RHICmdList.SetShaderResourceViewParameter(shader, genKeys->InPositions.GetBaseIndex(), params.m_InputPositions);

	if (HH_VERIFY(genKeys->OutKeys.IsBound()))
		RHICmdList.SetUAVParameter(shader, genKeys->OutKeys.GetBaseIndex(), m_Keys[m_CurrBuff]->UAV());

	if (HH_VERIFY(genKeys->OutValues.IsBound()))
		RHICmdList.SetUAVParameter(shader, genKeys->OutValues.GetBaseIndex(), m_Values[m_CurrBuff]->UAV());

	SetShaderValue(RHICmdList, shader, genKeys->TotalCount, params.m_Count);
	SetShaderValue(RHICmdList, shader, genKeys->SortOrigin, ToUE(params.m_SortOrigin));
	SetShaderValue(RHICmdList, shader, genKeys->IndexStart, params.m_IndexStart);
	SetShaderValue(RHICmdList, shader, genKeys->IndexStep, params.m_IndexStep);
	SetShaderValue(RHICmdList, shader, genKeys->InputOffset, params.m_InputOffset);

	SetShaderValue(RHICmdList, shader, genKeys->OutputOffset, m_CurrGenOutputOffset);
	m_CurrGenOutputOffset += params.m_Count;
	HH_ASSERT(m_CurrGenOutputOffset <= m_TotalCount);

	{
		const uint32	threadGroupCount = PopcornFX::Mem::Align(params.m_Count, CS_SORT_THREADGROUP_SIZE) / CS_SORT_THREADGROUP_SIZE;
		RHICmdList.DispatchComputeShader(threadGroupCount, 1, 1);
	}

	FUnorderedAccessViewRHIParamRef			nullUAV = FUnorderedAccessViewRHIParamRef();
	FShaderResourceViewRHIParamRef			nullSRV = FShaderResourceViewRHIParamRef();

	if (genKeys->OutValues.IsBound())
		RHICmdList.SetUAVParameter(shader, genKeys->OutValues.GetBaseIndex(), nullUAV);

	if (genKeys->OutKeys.IsBound())
		RHICmdList.SetUAVParameter(shader, genKeys->OutKeys.GetBaseIndex(), nullUAV);

	if (genKeys->InPositions.IsBound())
		RHICmdList.SetShaderResourceViewParameter(shader, genKeys->InPositions.GetBaseIndex(), nullSRV);
}

//----------------------------------------------------------------------------

void	FPopcornFXSortComputeShader_Sorter::DispatchSort(FRHICommandList& RHICmdList)
{
	HH_ASSERT(Ready());

	SCOPED_DRAW_EVENT(RHICmdList, PopcornFXSortComputeShader_Sorter_Sort);

	HH_ASSERT(m_CurrGenOutputOffset == m_TotalCount);

	const hh_u32		totalAlignedCount = PopcornFX::Mem::Align<CS_SORT_THREADGROUP_SIZE>(m_TotalCount);

	const ERHIFeatureLevel::Type	featureLevel = ERHIFeatureLevel::SM5; // @FIXME, true feature level ?

	TShaderMapRef< FPopcornFXSortComputeShader_Sort_UpSweep >			upSweep(GetGlobalShaderMap(featureLevel));
	TShaderMapRef< FPopcornFXSortComputeShader_Sort_UpSweepOffsets >	upSweepOffsets(GetGlobalShaderMap(featureLevel));
	TShaderMapRef< FPopcornFXSortComputeShader_Sort_DownSweep >			downSweep(GetGlobalShaderMap(featureLevel));

	FComputeShaderRHIParamRef	upSweepShader = upSweep->GetComputeShader();
	FComputeShaderRHIParamRef	upSweepOffsetsShader = upSweepOffsets->GetComputeShader();
	FComputeShaderRHIParamRef	downSweepShader = downSweep->GetComputeShader();

	const uint32		threadGroupCount = totalAlignedCount / CS_SORT_THREADGROUP_SIZE;

	FUnorderedAccessViewRHIParamRef			nullUAV = FUnorderedAccessViewRHIParamRef();
	FShaderResourceViewRHIParamRef			nullSRV = FShaderResourceViewRHIParamRef();

	for (hh_u32 bitOffset = 0; bitOffset < 16; bitOffset += 1)
	{
		//----------------------------------------------------------------------------
		// Up Sweep
		RHICmdList.SetComputeShader(upSweepShader);

		if (HH_VERIFY(upSweep->InKeys.IsBound()))
			RHICmdList.SetShaderResourceViewParameter(upSweepShader, upSweep->InKeys.GetBaseIndex(), m_Keys[m_CurrBuff]->SRV());
		if (HH_VERIFY(upSweep->OutOffsets.IsBound()))
			RHICmdList.SetUAVParameter(upSweepShader, upSweep->OutOffsets.GetBaseIndex(), m_Cache->UAV());

		SetShaderValue(RHICmdList, upSweepShader, upSweep->KeyBitOffset, bitOffset);

		{
			SCOPED_DRAW_EVENT(RHICmdList, PopcornFXSortComputeShader_Sorter_Sort_UpSweep);
			RHICmdList.DispatchComputeShader(threadGroupCount, 1, 1);
		}

		if (upSweep->InKeys.IsBound())
			RHICmdList.SetShaderResourceViewParameter(upSweepShader, upSweep->InKeys.GetBaseIndex(), nullSRV);
		if (upSweep->OutOffsets.IsBound())
			RHICmdList.SetUAVParameter(upSweepShader, upSweep->OutOffsets.GetBaseIndex(), nullUAV);

		//----------------------------------------------------------------------------
		// Up Sweep Offsets
		RHICmdList.SetComputeShader(upSweepOffsetsShader);

		if (HH_VERIFY(upSweepOffsets->InOutOffsets.IsBound()))
			RHICmdList.SetUAVParameter(upSweepOffsetsShader, upSweepOffsets->InOutOffsets.GetBaseIndex(), m_Cache->UAV());

		SetShaderValue(RHICmdList, upSweepOffsetsShader, upSweepOffsets->GroupCount, threadGroupCount);

		{
			SCOPED_DRAW_EVENT(RHICmdList, PopcornFXSortComputeShader_Sorter_Sort_UpSweepOffsets);
			RHICmdList.DispatchComputeShader(1, 1, 1);
		}

		if (upSweepOffsets->InOutOffsets.IsBound())
			RHICmdList.SetUAVParameter(upSweepOffsetsShader, upSweepOffsets->InOutOffsets.GetBaseIndex(), nullUAV);

		//----------------------------------------------------------------------------
		// Down Sweep
		RHICmdList.SetComputeShader(downSweepShader);

		if (HH_VERIFY(downSweep->InOffsets.IsBound()))
			RHICmdList.SetShaderResourceViewParameter(downSweepShader, downSweep->InOffsets.GetBaseIndex(), m_Cache->SRV());

		if (HH_VERIFY(downSweep->InKeys.IsBound()))
			RHICmdList.SetShaderResourceViewParameter(downSweepShader, downSweep->InKeys.GetBaseIndex(), m_Keys[m_CurrBuff]->SRV());
		if (HH_VERIFY(downSweep->InValues.IsBound()))
			RHICmdList.SetShaderResourceViewParameter(downSweepShader, downSweep->InValues.GetBaseIndex(), m_Values[m_CurrBuff]->SRV());

		m_CurrBuff = (m_CurrBuff + 1) % HH_ARRAY_COUNT(m_Keys);

		if (HH_VERIFY(downSweep->OutKeys.IsBound()))
			RHICmdList.SetUAVParameter(downSweepShader, downSweep->OutKeys.GetBaseIndex(), m_Keys[m_CurrBuff]->UAV());
		if (HH_VERIFY(downSweep->OutValues.IsBound()))
			RHICmdList.SetUAVParameter(downSweepShader, downSweep->OutValues.GetBaseIndex(), m_Values[m_CurrBuff]->UAV());

		SetShaderValue(RHICmdList, downSweepShader, downSweep->KeyBitOffset, bitOffset);
		SetShaderValue(RHICmdList, downSweepShader, downSweep->GroupCount, threadGroupCount);

		{
			SCOPED_DRAW_EVENT(RHICmdList, PopcornFXSortComputeShader_Sorter_Sort_DownSweep);
			RHICmdList.DispatchComputeShader(threadGroupCount, 1, 1);
		}

		if (downSweep->InOffsets.IsBound())
			RHICmdList.SetShaderResourceViewParameter(downSweepShader, downSweep->InOffsets.GetBaseIndex(), nullSRV);
		if (downSweep->InKeys.IsBound())
			RHICmdList.SetShaderResourceViewParameter(downSweepShader, downSweep->InKeys.GetBaseIndex(), nullSRV);
		if (downSweep->InValues.IsBound())
			RHICmdList.SetShaderResourceViewParameter(downSweepShader, downSweep->InValues.GetBaseIndex(), nullSRV);
		if (downSweep->OutKeys.IsBound())
			RHICmdList.SetUAVParameter(downSweepShader, downSweep->OutKeys.GetBaseIndex(), nullUAV);
		if (downSweep->OutValues.IsBound())
			RHICmdList.SetUAVParameter(downSweepShader, downSweep->OutValues.GetBaseIndex(), nullUAV);
	}
}

//----------------------------------------------------------------------------

const FShaderResourceViewRHIRef		&FPopcornFXSortComputeShader_Sorter::SortedValuesSRV()
{
	return m_Values[m_CurrBuff]->SRV();
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

#endif //(PK_GPU_D3D11 == 1)

