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

#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/Vector2D.h"
#include "Math/Matrix.h"
#include "Math/IntPoint.h"
#include "Math/Color.h"
#include "Math/Box.h"

#include "PopcornFXSDK.h"
#include <hh_maths/include/hh_maths_primitives.h>

#ifndef POPCORNFX_UE_PROFILER_COLOR
	#define POPCORNFX_UE_PROFILER_COLOR			CFloat3(1.0f, 0.5f, 0.0f)
#endif

#ifndef POPCORNFX_UE_PROFILER_FAST_COLOR
	#define POPCORNFX_UE_PROFILER_FAST_COLOR	CFloat3(1.0f, 0.0f, 0.0f)
#endif

template <typename _OutType, typename _InType>
HH_FORCEINLINE const _OutType		&_Reinterpret(const _InType &vec)
{
	HH_STATIC_ASSERT(sizeof(_InType) == sizeof(_OutType));
	return *reinterpret_cast<const _OutType*>(&vec);
}

HH_FORCEINLINE const FVector		&ToUE(const CFloat3 &vec) { return _Reinterpret<FVector>(vec); }
HH_FORCEINLINE const FVector4		&ToUE(const CFloat4 &vec) { return _Reinterpret<FVector4>(vec); }

HH_FORCEINLINE const CFloat3		&ToPk(const FVector &vec) { return _Reinterpret<CFloat3>(vec); }
HH_FORCEINLINE const CFloat2		&ToPk(const FVector2D &vec) { return _Reinterpret<CFloat2>(vec); }
HH_FORCEINLINE const CFloat4x4		&ToPk(const FMatrix &mat) { return _Reinterpret<CFloat4x4>(mat); }
HH_FORCEINLINE const CFloat4		&ToPk(const FLinearColor &vec) { return _Reinterpret<CFloat4>(vec); }
HH_FORCEINLINE const CFloat4		&ToPk(const FVector4 &vec) { return _Reinterpret<CFloat4>(vec); }

HH_FORCEINLINE const CInt2			&ToPk(const FIntPoint& point) { return _Reinterpret<CInt2>(point); }

HH_FORCEINLINE FBox					ToUE(const PopcornFX::CAABB &bounds)
{
	return FBox(ToUE(bounds.Min()), ToUE(bounds.Max()));
}

HH_FORCEINLINE const FMatrix		&ToUE(const CFloat4x4 &mat) { return _Reinterpret<FMatrix>(mat); }

HH_FORCEINLINE CFloat4x4	&ToPkRef(FMatrix &matrix)
{
	HH_STATIC_ASSERT(sizeof(FMatrix) == sizeof(CFloat4x4));
	return *reinterpret_cast<CFloat4x4*>(&matrix);
}

HH_FORCEINLINE CFloat3		&ToPkRef(FVector &vec)
{
	HH_STATIC_ASSERT(sizeof(FVector) == sizeof(CFloat3));
	return *reinterpret_cast<CFloat3*>(&vec);
}

float					HumanReadF(float v, hh_u32 base = 1024);
const TCHAR				*HumanReadS(float v, hh_u32 base = 1024);
static inline float		SafeRcp(float v) { return v == 0.f ? 0.f : 1.f / v; }
