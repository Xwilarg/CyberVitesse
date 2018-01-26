//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------
#include "DrawCallSliceHelper.h"

#include "PopcornFXSettings.h"

#include "PopcornFXSDK.h"
#include <hh_kernel/include/kr_containers_onstack.h>

bool	SDrawCallSliceHelper::GenSlices()
{
	HH_ASSERT(m_Indices != null);
	HH_ASSERT(m_IndexCount > 0);
	HH_ASSERT(m_IndexCount >= m_PrimitiveSize);
	HH_ASSERT(m_VPP4_ParticleCount <= m_IndexCount); // at most ?
	HH_ASSERT(m_TotalVertexCount > 0);
	HH_ASSERT(!m_PositionCollection.Empty());
	HH_ASSERT(m_RenderSettings != null);
	HH_ASSERT(m_OutDrawCallSlices != null);

	//m_Indices = exec.m_IndexStream.RawData();
	//m_LargeIndices = exec.m_IndexStream.Large();
	//m_IndexCount = exec.m_IndexStream.Count();
	//m_VPP4_ParticleCount = bb.VPP4_ParticleCount();
	//m_TotalVertexCount = bb.TotalVertexCount();
	//m_ViewPos = bbMatrix.StrippedTranslations();
	//m_ViewAxis = -bbMatrix.StrippedZAxis();
	//m_PositionCollection = m_BB.PositionsCollection();

	m_OutMaxSliceTreeDepth = 0;

	// "unbreakable min primitive"
	// (could cut capsules in half, but not quad billboards)
	enum : hh_u32 { kPrim = 6 };
	HH_ASSERT(m_PrimitiveSize == kPrim); // all implems should use that anway
	//	const hh_u32						kPrim = m_PrimitiveSize;

	PopcornFX::TArray<SDrawCallSlice>	&outDrawSlices = *m_OutDrawCallSlices;

	outDrawSlices.Clear();

	const float				farCullDepth = m_RenderSettings->FarCullDistance > 0 ? m_RenderSettings->FarCullDistance : PopcornFX::TNumericTraits<float>::Infinity();
	const float				preferedSliceDepth = m_RenderSettings->SlicedDraw_PreferedSliceDepth;

	const hh_u32			indexCount = m_IndexCount;

	// triangle list
	HH_ASSERT(indexCount >= kPrim);
	HH_ASSERT(indexCount % kPrim == 0);

	const hh_u32			totalPrimCount = indexCount / kPrim;
	const hh_u32			lastIndex = indexCount - kPrim + 1; // first vertex of last triangle

	const float				_maxDepth = GetDepth(0); // Back
	const float				_minDepth = GetDepth(lastIndex); // Front

	// Inf and NaN should be all at the beginning or at the end
	// don't bother slicing, this should not happen
	if (!PopcornFX::TNumericTraits<float>::IsFinite(_maxDepth) ||
		!PopcornFX::TNumericTraits<float>::IsFinite(_minDepth))
		return false; // will fallback and draw everything

	if (_maxDepth < 0 || _minDepth > farCullDepth)
	{
		return true; // legit, everything is culled
	}

	HH_ASSERT(_maxDepth >= _minDepth);
	const float				minDepth = _minDepth;
	const float				maxDepth = PopcornFX::HHMax(_maxDepth, _minDepth);

	float					totalDepth = maxDepth - minDepth;
	const hh_u32			maxSliceCount = 100;

#if 0

	// "Exact" slicing
	// Best results, but reads all indices and all particle positions (and rebuild all depths)

	float					frontDepth = minDepth;
	hh_i32					backPrim = totalPrimCount;
	hh_i32					frontPrim = backPrim - 1;
	while (backPrim > 0)
	{
		float		backDepth = frontDepth;
		while (backPrim > 0 && backDepth - frontDepth <= preferedSliceDepth)
		{
			--backPrim;
			backDepth = GetDepth(backPrim * kPrim);
		}
		if (backPrim == 0)
			backPrim = -1;
		if (backDepth > 0 && frontPrim < farCullDepth)
		{
			const PopcornFX::CGuid	slicei = outDrawSlices.PushBack();
			if (HH_VERIFY(slicei.Valid()))
			{
				SDrawCallSlice		&slice = outDrawSlices[slicei];
				slice.m_ElementStart = (backPrim + 1) * kPrim;
				slice.m_ElementCount = (frontPrim - backPrim) * kPrim;
				HH_ASSERT(slice.m_ElementStart + slice.m_ElementCount <= indexCount);
				slice.m_Depth = frontDepth;
			}
		}
		frontDepth = backDepth;
		frontPrim = backPrim;
	}

#elif 1

	// Aproximated dichotomic slicing
	// Better with non-uniform particle distribution
	// Reads much much less data
	// But slicing is unstable, can result in sorting flickering

	struct _SStack
	{
		hh_u32		primStart;
		hh_u32		primEnd;
		float		depthBack;
		float		depthFront;
	};
	HH_STACKMEMORYVIEW(_SStack, stack, 10);

	{
		_SStack		&c = stack[0];
		c.primStart = 0;
		c.primEnd = totalPrimCount;
		c.depthBack = maxDepth;
		c.depthFront = minDepth;
	}

	//PopcornFX::CLog::Log(HH_INFO, "--------------------");
	//PopcornFX::CLog::Log(HH_INFO, "Spliting %d prim, %.2f > %.2f", totalPrimCount, maxDepth, minDepth);

	hh_u32		stacki = 1;
	do
	{
		--stacki;
		// (copy)
		_SStack			c = stack[stacki];

		m_OutMaxSliceTreeDepth = PopcornFX::HHMax(m_OutMaxSliceTreeDepth, stacki);

		const hh_u32	primCount = c.primEnd - c.primStart;

		HH_ASSERT(c.depthBack >= c.depthFront);
		const float		depthRange = c.depthBack - c.depthFront;

		if (depthRange <= preferedSliceDepth || primCount < 2 || stacki + 2 >= stack.Count())
		{
			const PopcornFX::CGuid	slicei = outDrawSlices.PushBack();
			if (HH_VERIFY(slicei.Valid()))
			{
				SDrawCallSlice		&slice = outDrawSlices[slicei];
				slice.m_ElementStart = c.primStart * kPrim;
				slice.m_ElementCount = primCount * kPrim;
				slice.m_Depth = c.depthFront;
				//PopcornFX::CLog::Log(HH_INFO, "> %*sSLICE %d %d, %.2f > %.2f", stacki * 2, "", c.primStart, c.primEnd, c.depthBack, c.depthFront);
			}
			continue;
		}

		HH_ASSERT(primCount >= 2);
		const hh_u32		primCenterOff = primCount / 2;

		const hh_u32		primCenter = c.primStart + primCenterOff;
		const float			_depthCenter = GetDepth(primCenter * kPrim);
		HH_ASSERT(_depthCenter >= c.depthFront && _depthCenter <= c.depthBack);
		const float			depthCenter = PopcornFX::HHClamp(_depthCenter, c.depthFront, c.depthBack);

		const float			lookForDepth = c.depthFront + depthRange * 0.5f;

		hh_u32				newPrimCenterOff;
		if (depthCenter < lookForDepth)
		{
			const float		a = float(primCenterOff) * (lookForDepth - c.depthBack) / (depthCenter - c.depthBack);
			newPrimCenterOff = PopcornFX::PKFloatToIntTrunc(a);
		}
		else
		{
			const float		a = float(primCenterOff) * (lookForDepth - depthCenter) / (c.depthFront - depthCenter);
			newPrimCenterOff = primCenterOff + PopcornFX::PKFloatToIntTrunc(a);
		}
		HH_ASSERT(newPrimCenterOff <= primCount);

		if (newPrimCenterOff == 0)
			newPrimCenterOff = 1;
		else if (newPrimCenterOff >= primCount)
			newPrimCenterOff = primCount - 1;

		const hh_u32		newPrimCenter = c.primStart + newPrimCenterOff;
		const float			_newDepthCenter = GetDepth(newPrimCenter * kPrim);
		HH_ASSERT(_newDepthCenter >= c.depthFront && _newDepthCenter <= c.depthBack);
		const float			newDepthCenter = PopcornFX::HHClamp(_newDepthCenter, c.depthFront, c.depthBack);

		if (c.depthBack > 0 && newDepthCenter < farCullDepth)
		{
			HH_ASSERT(stacki < stack.Count());
			_SStack		&next = stack[stacki];
			++stacki;
			next.primStart = c.primStart;
			next.primEnd = newPrimCenter;
			next.depthBack = c.depthBack;
			next.depthFront = newDepthCenter;
		}

		if (newDepthCenter > 0 && c.depthFront < farCullDepth)
		{
			HH_ASSERT(stacki < stack.Count());
			_SStack		&next = stack[stacki];
			++stacki;
			next.primStart = newPrimCenter;
			next.primEnd = c.primEnd;
			next.depthBack = newDepthCenter;
			next.depthFront = c.depthFront;
		}

		//PopcornFX::CLog::Log(HH_INFO, "  %*s%d %d, %.2f > %.2f | %d %d, %.2f > %.2f (was %.2f)",
		//	(stacki - 2) * 2, "",
		//	stack[stacki - 2].primStart, stack[stacki - 2].primEnd, stack[stacki - 2].depthBack, stack[stacki - 2].depthFront,
		//	stack[stacki - 1].primStart, stack[stacki - 1].primEnd, stack[stacki - 1].depthBack, stack[stacki - 1].depthFront,
		//	depthCenter
		//);


	} while (stacki > 0);

#else

	// particle count slicing
	// baaad for non-uniform particle distribution

	const hh_u32			baseSliceCount = PopcornFX::HHMin(maxSliceCount, PopcornFX::HHMax(1U, hh_u32(totalDepth / preferedSliceDepth)));
	hh_u32					baseSliceStep = PopcornFX::HHMax(1U, totalPrimCount / baseSliceCount) * kPrim;

	if (!HH_VERIFY(outDrawSlices.Resize(baseSliceCount)))
		return;

	hh_u32			sliceCount = 0;
	//float			lastDepth = minDepth;
	hh_u32			currentIndex = 0;
	float			lastDepth = maxDepth;
	while (currentIndex < indexCount)
	{
		const hh_u32	frontIndex = PopcornFX::HHMin(currentIndex + baseSliceStep, lastIndex);
		float			fontDepth = GetDepth(frontIndex);
		//const float		endDepth = GetDepth(nextIndex);

		HH_ASSERT(fontDepth <= lastDepth);
		fontDepth = PopcornFX::HHMin(fontDepth, lastDepth);
		const float		backDepth = lastDepth;
		lastDepth = fontDepth;

		if (backDepth > 0 && fontDepth < farCullDepth)
		{
			SDrawCallSlice		&slice = outDrawSlices[sliceCount++];
			slice.m_Depth = fontDepth;
			slice.m_ElementStart = currentIndex;
			slice.m_ElementCount = (frontIndex - currentIndex) + kPrim;
			HH_ASSERT(slice.m_ElementCount > 0);
		}
		currentIndex = frontIndex + kPrim;
	}
	HH_ASSERT(sliceCount <= outDrawSlices.Count());
	outDrawSlices.Resize(sliceCount);

#endif

	return true;
}
