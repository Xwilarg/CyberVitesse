#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2003/07/02 05:25 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_SORT_H__
#define	__KR_SORT_H__

//#include "hh_maths/include/hh_maths_simd.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _TypeIt>
class	TSortLowToHigh
{
public:
	HH_FORCEINLINE static bool	Less(const _TypeIt &it0, const _TypeIt &it1) { return *it0 < *it1; }
	HH_FORCEINLINE static bool	LessOrEqual(const _TypeIt &it0, const _TypeIt &it1) { return *it0 <= *it1; }
	HH_FORCEINLINE static bool	Equal(const _TypeIt &it0, const _TypeIt &it1) { return *it0 == *it1; }
};

//----------------------------------------------------------------------------

template<typename _TypeIt>
class	TSortHighToLow
{
public:
	HH_FORCEINLINE static bool	Less(const _TypeIt &it0, const _TypeIt &it1) { return *it0 > *it1; }
	HH_FORCEINLINE static bool	LessOrEqual(const _TypeIt &it0, const _TypeIt &it1) { return *it0 >= *it1; }
	HH_FORCEINLINE static bool	Equal(const _TypeIt &it0, const _TypeIt &it1) { return *it0 == *it1; }
};

//----------------------------------------------------------------------------

template<typename _TypeIt>
class	TSortDefaultCompare : public TSortLowToHigh<_TypeIt> {};

//----------------------------------------------------------------------------
// dumbest possible sort, fixed iteration count, OK for really small values. (~4 to 8)
// the compiler should be able to aggressively optimize this...

template<hh_u32 _Count, typename _Compare, typename _TypeIt>
void	BubbleSort(_TypeIt it)
{
	bool	sorted = false;
	while (!sorted)
	{
		sorted = true;
		for (hh_u32 i = 1; i < _Count; i++)
		{
			if (_Compare::Less(it + i, it + i - 1))
			{
				HHSwap(it[i], it[i - 1]);
				sorted = false;
			}
		}
	}
}

template<hh_u32 _Count, typename _TypeIt>
HH_FORCEINLINE void	BubbleSort(_TypeIt it)
{
	BubbleSort<_Count, TSortDefaultCompare<_TypeIt>, _TypeIt>(it);
}

//----------------------------------------------------------------------------

template<typename _Compare, typename _TypeIt>
void	BubbleSort(_TypeIt first, _TypeIt last)
{
	hh_u32	indexEnd = last - first;
	hh_u32	lastSwapPos = 1;
	while (lastSwapPos > 0)
	{
		lastSwapPos = 0;
		for (hh_u32 i = 1; i < indexEnd; i++)
		{
			if (_Compare::Less(first + i, first + i - 1))
			{
				HHSwap(first[i], first[i - 1]);
				lastSwapPos = i;
			}
		}
		indexEnd = lastSwapPos;
	}
}

template<typename _TypeIt>
HH_FORCEINLINE void	BubbleSort(_TypeIt first, _TypeIt last)
{
	BubbleSort<TSortDefaultCompare<_TypeIt>, _TypeIt>(first, last);
}

//----------------------------------------------------------------------------

template<typename _Compare, typename _TypeIt>
void	BubbleSortBidirectional(_TypeIt first, _TypeIt last)
{
	hh_u32	indexStart = 0;
	hh_u32	indexEnd = last - first;
	hh_u32	lastStartPos = indexStart;
	hh_u32	lastSwapPos = indexEnd;
	while (lastSwapPos > lastStartPos)
	{
		lastSwapPos = lastStartPos;
		for (hh_u32 i = lastStartPos + 1; i < indexEnd; i++)
		{
			if (_Compare::Less(first + i, first + i - 1))
			{
				HHSwap(first[i], first[i - 1]);
				lastSwapPos = i;
			}
		}
		if (lastSwapPos == lastStartPos)
			break;

		indexEnd = lastSwapPos;

		for (hh_u32 i = lastSwapPos - 1; i > indexStart; i--)
		{
			if (_Compare::Less(first + i, first + i - 1))
			{
				HHSwap(first[i], first[i - 1]);
				lastStartPos = i;
			}
		}

		indexStart = lastStartPos;
	}
}

template<typename _TypeIt>
HH_FORCEINLINE void	BubbleSortBidirectional(_TypeIt first, _TypeIt last)
{
	BubbleSortBidirectional<TSortDefaultCompare<_TypeIt>, _TypeIt>(first, last);
}

//----------------------------------------------------------------------------
//
//	Insertion sort
//
//----------------------------------------------------------------------------

template<typename _Compare, typename _TypeIt>
HH_INLINE void InsertionSort(_TypeIt first, _TypeIt last)
{
	while (--last > first)
	{
		_TypeIt	max = last;
		for (_TypeIt p = last; p > first; )
		{
			--p;
			if (_Compare::Less(p, max))
				continue;
			max = p;
		}

		if (max != last)
		{
			HHSwap(*max, *last);
		}
	}
}

template<typename _TypeIt>
HH_FORCEINLINE void	InsertionSort(_TypeIt first, _TypeIt last)
{
	InsertionSort<TSortDefaultCompare<_TypeIt>, _TypeIt>(first, last);
}

//----------------------------------------------------------------------------

template<hh_u32 _Count, typename _Compare, typename _TypeIt>
HH_INLINE void InsertionSort(_TypeIt it)
{
	hh_u32	i = _Count;
	while (--i > 0)
	{
		hh_u32	max = i;
		for (hh_u32 j = i; j-- > 0; )
		{
			if (_Compare::Less(it + j, it + max))
				continue;
			max = j;
		}

		if (max == 0)
		{
			HHSwap(it[max], it[i]);
		}
	}
}

template<hh_u32 _Count, typename _TypeIt>
HH_FORCEINLINE void	InsertionSort(_TypeIt first, _TypeIt last)
{
	InsertionSort<_Count, TSortDefaultCompare<_TypeIt>, _TypeIt>(first, last);
}

//----------------------------------------------------------------------------
//
//	Quick sort
//
//----------------------------------------------------------------------------

template<typename _Compare, typename _TypeIt>
HH_INLINE void QuickSort(_TypeIt first, _TypeIt last)
{
	if (last - first < 2)
		return;

	enum
	{
		kMaxElementCount	= TNumericTraits<hh_u32>::kMax,
		kSubdivisionLimit	= 12,
		kStackSize			= TIntegerTemplateTools::Log2<kMaxElementCount>::Value - TIntegerTemplateTools::Log2<kSubdivisionLimit>::Value,
	};

	_TypeIt	loStack[kStackSize];
	_TypeIt	hiStack[kStackSize];
	hh_i32	stackId = 0;
	_TypeIt	lo = first;
	_TypeIt	hi = last - 1;

_StackRecursionLoop:
	_TypeIt	hiEnd = hi + 1;
	hh_u32	size = hiEnd - lo;
	if (size <= hh_u32(kSubdivisionLimit))
	{
		InsertionSort<_Compare, _TypeIt>(lo, hiEnd);
	}
	else
	{
		_TypeIt	mid = lo + (size >> 1);
		if (_Compare::Less(mid, lo))
			HHSwap(*lo, *mid);
		if (_Compare::Less(hi, lo))
			HHSwap(*lo, *hi);
		if (_Compare::Less(hi, mid))
			HHSwap(*mid, *hi);

		_TypeIt	loElem = lo;
		_TypeIt	hiElem = hi;
		while (1)
		{
			if (mid > loElem)
			{
				while (++loElem < mid && _Compare::LessOrEqual(loElem, mid))
					;
			}
			if (mid <= loElem)
			{
				while (++loElem <= hi && _Compare::LessOrEqual(loElem, mid))
					;
			}

			while (mid < --hiElem && _Compare::Less(mid, hiElem))
				;

			if (hiElem < loElem)
			{
				break;
			}

			HHSwap(*loElem, *hiElem);
			if (mid == hiElem)
			{
				mid = loElem;
			}
		}

		if (mid < ++hiElem)
		{
			while (mid < --hiElem && _Compare::Equal(mid, hiElem))
				;
		}
		if (hiElem <= mid)
		{
			while (lo < --hiElem && _Compare::Equal(mid, hiElem))
				;
		}

		if (hi - loElem <= hiElem - lo)
		{
			if (lo < hiElem)
			{
				loStack[stackId] = lo;
				hiStack[stackId] = hiElem;
				stackId++;
			}
			if (loElem < hi)
			{
				lo = loElem;
				goto _StackRecursionLoop;
			}
		}
		else
		{
			if (loElem < hi)
			{
				loStack[stackId] = loElem;
				hiStack[stackId] = hi;
				stackId++;
			}
			if (lo < hiElem)
			{
				hi = hiElem;
				goto _StackRecursionLoop;
			}
		}
	}

	if (--stackId >= 0)
	{
		lo = loStack[stackId];
		hi = hiStack[stackId];
		goto _StackRecursionLoop;
	}
}

//----------------------------------------------------------------------------

template<typename _TypeIt>
HH_FORCEINLINE void QuickSort(_TypeIt first, _TypeIt last)
{
	QuickSort<TSortDefaultCompare<_TypeIt>, _TypeIt>(first, last);
}

//----------------------------------------------------------------------------
//
//	Radix sort
//
//----------------------------------------------------------------------------
//
// 'buffer' should point to a buffer containing at least (last - first) elements
// RadixSort may be called like:
//	RadixSort(itStart, itEnd, itBuffer, KeyExtractor());
// or:
//	RadixSort<KeyExtractor>(itStart, itEnd, itBuffer);
//
//----------------------------------------------------------------------------

enum	ESortOutputMode
{
	Sort_InPlace = 0,
	Sort_InBuffer,
};

template<hh_u32 _Size> struct	SRadixKeyIntegralTranslator		{ HH_STATIC_ASSERT_NOT_REACHED(); };
template<> struct				SRadixKeyIntegralTranslator<1>	{ typedef hh_u8		_Type; static const hh_u32	RadixDegree = 8; };
template<> struct				SRadixKeyIntegralTranslator<2>	{ typedef hh_u16	_Type; static const hh_u32	RadixDegree = 8; };
template<> struct				SRadixKeyIntegralTranslator<4>	{ typedef hh_u32	_Type; static const hh_u32	RadixDegree = 11; };
template<> struct				SRadixKeyIntegralTranslator<8>	{ typedef hh_u64	_Type; static const hh_u32	RadixDegree = 11; };

//----------------------------------------------------------------------------

template<ESortOutputMode _SortOutput = Sort_InBuffer>
struct	TRadixSort
{
	template<typename _KeyExtractor, typename _TypeIt>
	static HH_FORCEINLINE void Sort(_TypeIt first, _TypeIt last, _TypeIt buffer)
	{
		_KeyExtractor	dummyExtractor;
		Sort<_KeyExtractor, _TypeIt>(first, last, buffer, dummyExtractor);
	}

	template<typename _KeyExtractor, typename _TypeIt>
	static HH_FORCEINLINE void	Sort(_TypeIt first, _TypeIt last, _TypeIt buffer, _KeyExtractor &keyExtractor)
	{
		return Sort8(first, last, buffer, keyExtractor);
	}

	//----------------------------------------------------------------------------
	//
	//	8-bit radix
	//
	//----------------------------------------------------------------------------

	template<typename _KeyExtractor, typename _TypeIt>
	static HH_FORCEINLINE void	Sort8(_TypeIt first, _TypeIt last, _TypeIt buffer)
	{
		_KeyExtractor	dummyExtractor;
		Sort8<_KeyExtractor, _TypeIt>(first, last, buffer, dummyExtractor);
	}

	//----------------------------------------------------------------------------

	static HH_FORCEINLINE void	AlignedMemset64(hh_u8 *start, hh_u8 *end)
	{
		Mem::Clear(start, end - start);
#if 0
#if (HH_SIMD != HH_SIMD_NONE)
	//	__m128	zero /*= _mm_set1_ps(0.0f)*/;
	//	zero = _mm_xor_ps(zero, zero);	// broken VC optimizer forces us to do this, otherwise it generates: xorps xmm0, xmm0; movss xmm0, xmm0, shufps xmm0, 0... how nice \o/ (not that we care anyway, we'll be doing a few hundreds iterations below.. but still, it's frustrating
		SIMD::Float4	kZero = SIMD::Float4::Zero();
		while (start < end)
		{
			kZero.StoreAligned16(start, 0x00);
			kZero.StoreAligned16(start, 0x10);
			kZero.StoreAligned16(start, 0x20);
			kZero.StoreAligned16(start, 0x30);
			start += 0x40;
		}
#else
		memset(start, 0, end - start);
#endif
#endif
	}

	//----------------------------------------------------------------------------

	template<typename _KeyExtractor, typename _TypeIt>
	static void	Sort8(_TypeIt first, _TypeIt last, _TypeIt buffer, _KeyExtractor &keyExtractor)
	{
		static const hh_u32	kHistogramBucketsCount = sizeof(typename _KeyExtractor::SortKeyType);

		HH_STATIC_ASSERT_CUSTOM((kHistogramBucketsCount & 1) == 0, "The_sort_key_byte_width_must_be_a_multiple_of_two");
		HH_ALIGN(16) hh_u32	histograms[kHistogramBucketsCount][256];
		HH_ALIGN(16) hh_u32	offsets[256];

		AlignedMemset64(reinterpret_cast<hh_u8*>(&histograms), reinterpret_cast<hh_u8*>(&histograms) + sizeof(histograms));

		_TypeIt	bufIt = buffer;
		_TypeIt	curIt = first;
		hh_u32	elementCount = last - first;

		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	key = keyExtractor.ExtractKey(curIt[i]);
#ifdef	HH_LITTLE_ENDIAN
			for (hh_u32	radix = 0; radix < kHistogramBucketsCount; radix++)
#else
			for (hh_u32	radix = kHistogramBucketsCount; radix-- != 0; )
#endif
			{
				histograms[radix][*(reinterpret_cast<hh_u8*>(&key) + radix)]++;
			}
		}

#ifdef	HH_LITTLE_ENDIAN
		for (hh_u32	radix = 0; radix < kHistogramBucketsCount; radix++)
#else
		for (hh_u32	radix = kHistogramBucketsCount; radix-- != 0; )
#endif
		{
			hh_u32	*currentCounter = histograms[radix];
			offsets[0] = 0;
			for (hh_u32 i = 1; i < 256; i++)
			{
				offsets[i] = offsets[i - 1] + currentCounter[i - 1];
			}

			for (hh_u32 i = 0; i < elementCount; i++)
			{
				typename _KeyExtractor::SortKeyType	key = keyExtractor.ExtractKey(curIt[i]);
				hh_u32	rkey = *(reinterpret_cast<hh_u8*>(&key) + radix);
				bufIt[offsets[rkey]] = curIt[i];
				++offsets[rkey];
			}

			HHSwap(curIt, bufIt);
		}
	}


	//----------------------------------------------------------------------------
	//
	//	11-bit Radix
	//
	//----------------------------------------------------------------------------
	// WARNING!! unlike Sort8(), the sorted data will be located in 'output'

	template<typename _KeyExtractor, typename _TypeIt>
	static HH_FORCEINLINE void Sort11(_TypeIt first, _TypeIt last, _TypeIt output)
	{
		_KeyExtractor	dummyExtractor;
		Sort11<_KeyExtractor, _TypeIt>(first, last, output, dummyExtractor);
	}

	//----------------------------------------------------------------------------

	template<hh_u32 _Bitcount>
	static HH_INLINE void	SumCounters(hh_u32 *counter)
	{
		HH_STATIC_ASSERT(_Bitcount >= 2);
		HH_ASSERT_ALIGNED16(counter);
		hh_u32	sum = 0;

		// FIXME: bad dependency chains, change memory layout of radix11 histograms

#if 0
		for (hh_u32 i = 0; i < (1 << _Bitcount); i++)
		{
			hh_u32 tSum = counter[i];
			counter[i] = sum;
			sum += tSum;
		}
#else
		for (hh_u32 i = 0; i < (1 << (_Bitcount - 2)); i++)
		{
			hh_u32 tSum;

			tSum = counter[0];
			counter[0] = sum;
			sum += tSum;
			tSum = counter[1];
			counter[1] = sum;
			sum += tSum;
			tSum = counter[2];
			counter[2] = sum;
			sum += tSum;
			tSum = counter[3];
			counter[3] = sum;
			sum += tSum;

			counter += 4;
		}
#endif
	}

	//----------------------------------------------------------------------------

	template<hh_u32 _BaseBits, hh_u32 _Radix, typename _Type>
	static HH_FORCEINLINE hh_u32	ExtractRadixBase(_Type key)
	{
		return (key >> (_Radix * _BaseBits)) & ((1 << _BaseBits) - 1);
	}

	template<hh_u32 _RemainderShift, typename _Type>
	static HH_FORCEINLINE hh_u32	ExtractRadixRemainder(_Type key)
	{
		return key >> _RemainderShift;
	}

	//----------------------------------------------------------------------------

#if 0
	template<typename _KeyExtractor, typename _TypeIt>
	static void	RadixSort11_Legacy(_TypeIt first, _TypeIt last, _TypeIt output, _KeyExtractor &keyExtractor)
	{
		HH_ALIGN(16) hh_u32	rawHistograms[2048 + 2048 + 1024];
		hh_u32				*histogram0 = rawHistograms;
		hh_u32				*histogram1 = histogram0 + 2048;
		hh_u32				*histogram2 = histogram1 + 2048;

		HH_STATIC_ASSERT(sizeof(_KeyExtractor::SortKeyType) <= 4);

		AlignedMemset64(reinterpret_cast<hh_u8*>(&rawHistograms), reinterpret_cast<hh_u8*>(&rawHistograms) + sizeof(rawHistograms));

		hh_u32	elementCount = last - first;
		_TypeIt	bufIt = output;
		_TypeIt	curIt = first;

#define EXTRACT11_0(x)	((x) & 0x7FF)
#define EXTRACT11_1(x)	(((x) >> 11) & 0x7FF)
#define EXTRACT11_2(x)	((x) >> 22)

		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(curIt[i]);
			hh_u32	key = bit_cast<hh_u32>(rawKey);
			histogram0[EXTRACT11_0(key)]++;
			histogram1[EXTRACT11_1(key)]++;
			histogram2[EXTRACT11_2(key)]++;
		}

		SumCounters<11>(histogram0);
		SumCounters<11>(histogram1);
		SumCounters<10>(histogram2);

		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(curIt[i]);
			hh_u32	key = bit_cast<hh_u32>(rawKey);
			hh_u32	pos = EXTRACT11_0(key);
			bufIt[histogram0[pos]++] = curIt[i];
		}

		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(bufIt[i]);
			hh_u32	key = bit_cast<hh_u32>(rawKey);
			hh_u32	pos = EXTRACT11_1(key);
			curIt[histogram1[pos]++] = bufIt[i];
		}

		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(curIt[i]);
			hh_u32	key = bit_cast<hh_u32>(rawKey);
			hh_u32	pos = EXTRACT11_2(key);
			bufIt[histogram2[pos]++] = curIt[i];
		}
#undef	EXTRACT11_0
#undef	EXTRACT11_1
#undef	EXTRACT11_2
	}
#endif

	//----------------------------------------------------------------------------
	//
	//	Generic radix sort
	//
	//----------------------------------------------------------------------------

	template<typename _Functor, typename _KeyExtractor, typename _TypeIt, hh_u32 _UnrollCount, bool _OddUnroll = ((_UnrollCount) & 1) != 0>
	struct	SRadixUnrollHelper
	{
		static HH_FORCEINLINE void	Unroll(hh_u32 *histograms, hh_u32 elementCount, _KeyExtractor &keyExtractor, _TypeIt &curIt, _TypeIt &bufIt)
		{
			SRadixUnrollHelper<_Functor, _KeyExtractor, _TypeIt, (_UnrollCount - 1)>::Unroll(histograms, elementCount, keyExtractor, curIt, bufIt);
			_Functor::template Run<(_UnrollCount - 1)>(histograms, elementCount, keyExtractor, curIt, bufIt);
		}
	};

	template<typename _Functor, typename _KeyExtractor, typename _TypeIt, hh_u32 _UnrollCount>
	struct	SRadixUnrollHelper<_Functor, _KeyExtractor, _TypeIt, _UnrollCount, false>
	{
		static HH_FORCEINLINE void	Unroll(hh_u32 *histograms, hh_u32 elementCount, _KeyExtractor &keyExtractor, _TypeIt &curIt, _TypeIt &bufIt)
		{
			SRadixUnrollHelper<_Functor, _KeyExtractor, _TypeIt, (_UnrollCount - 1)>::Unroll(histograms, elementCount, keyExtractor, curIt, bufIt);
			_Functor::template Run<(_UnrollCount - 1)>(histograms, elementCount, keyExtractor, bufIt, curIt);	// <-- iterator flip
		}
	};

	template<typename _Functor, typename _KeyExtractor, typename _TypeIt>
	struct	SRadixUnrollHelper<_Functor, _KeyExtractor, _TypeIt, 1, true>
	{
		static HH_FORCEINLINE void	Unroll(hh_u32 *histograms, hh_u32 elementCount, _KeyExtractor &keyExtractor, _TypeIt &curIt, _TypeIt &bufIt)
		{
			_Functor::template Run<0>(histograms, elementCount, keyExtractor, curIt, bufIt);
		}
	};

	//----------------------------------------------------------------------------

	template<typename _KeyExtractor, typename _TypeIt, hh_u32 _BaseBits, hh_u32 _AntiAliasOffset>
	struct	SRadixSortUnrollableFunctor
	{
		typedef typename SRadixKeyIntegralTranslator<sizeof(typename _KeyExtractor::SortKeyType)>::_Type	IntegralKeyType;
		static const hh_u32	_BaseSize = 1 << _BaseBits;

		template<hh_u32 _Iteration>
		static HH_FORCEINLINE void	Run(hh_u32 *histograms, hh_u32 elementCount, _KeyExtractor &keyExtractor, _TypeIt &curIt, _TypeIt &bufIt)
		{
			hh_u32	*hist = histograms + _Iteration * (_BaseSize + _AntiAliasOffset);
			for (hh_u32 i = 0; i < elementCount; i++)
			{
/*#if	(HH_SIMD != HH_SIMD_SSE)
				Memory::PrefetchCacheLine((char*)(&curIt[i + 64]), 0);
#endif*/
				typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(curIt[i]);
				IntegralKeyType	key = bit_cast<IntegralKeyType>(rawKey);
				hh_u32			pos = ExtractRadixBase<_BaseBits, _Iteration>(key);
				bufIt[hist[pos]++] = curIt[i];
			}
		}
	};

	//----------------------------------------------------------------------------
	// WARNING ! for 32 bits keys, needs 2 * 2^11 + 2^10 elements = 2*2048+1024 = 5120 elements = 20 Kbytes
	// this might not fit on the stack, or within the L1 cache !
	// with small stacks &| L1 caches, the radix8 sort might be better ("only" 4*256 elements = 1024 elements = 2 Kbytes)

	template<typename _KeyExtractor, typename _TypeIt/*, bool _InPlace = false*/>
	static void	Sort11(_TypeIt first, _TypeIt last, _TypeIt output, _KeyExtractor &keyExtractor)
	{
		hh_u32	elementCount = last - first;
		if (elementCount == 0)
			return;

		typedef typename SRadixKeyIntegralTranslator<sizeof(typename _KeyExtractor::SortKeyType)>::_Type	IntegralKeyType;
		HH_STATIC_ASSERT(TTypeTraits::IsSigned<IntegralKeyType>::False);	// we rely on this for non-sign-extending rightshifts

		enum
		{
			kTotalKeyBits = sizeof(typename _KeyExtractor::SortKeyType) * 8,
			kBaseBits = SRadixKeyIntegralTranslator<sizeof(typename _KeyExtractor::SortKeyType)>::RadixDegree,
			kRemainderBits = kTotalKeyBits % kBaseBits,
			kBaseCount = kTotalKeyBits / kBaseBits,
			kBaseSize = 1 << kBaseBits,
			kAntiAliasOffset = 128 + 64,	// at least one cache-line wide to avoid cache aliasing
			kRemainderSize = 1 << kRemainderBits,
			kRemainderShift = kBaseCount * kBaseBits,
		};

		_TypeIt	bufIt = output;
		_TypeIt	curIt = first;
		HH_ALIGN(64) hh_u32	rawHistograms[kBaseCount * (kBaseSize + kAntiAliasOffset) + kRemainderSize];

		// TODO: redesign the histograms data-layout to improve generation & accumulation. storing in AOS instead of SOA will be better in this case,
		// it will reduce dependency chains and cache aliasing

		AlignedMemset64(reinterpret_cast<hh_u8*>(&rawHistograms[0]), reinterpret_cast<hh_u8*>(&rawHistograms[0]) + sizeof(rawHistograms));

#define EXTRACT_BASE(__k, __radix)		(((__k) >> ((__radix) * kBaseBits)) & (kBaseSize - 1))
#define EXTRACT_REMAINDER(__k)			((__k) >> kRemainderShift)

		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(curIt[i]);
			IntegralKeyType	key = bit_cast<IntegralKeyType>(rawKey);

			hh_u32	* __restrict hPtr = rawHistograms;	// Fix: help crappy compiler
			for (hh_u32 j = 0; j < hh_u32(kBaseCount); j++)
			{
				hPtr[EXTRACT_BASE(key, j)]++;
				hPtr += kBaseSize + kAntiAliasOffset;
			}
			hPtr[EXTRACT_REMAINDER(key)]++;
		}

		for (hh_u32 j = 0; j < hh_u32(kBaseCount); j++)
		{
			SumCounters<kBaseBits>(rawHistograms + j * (kBaseSize + kAntiAliasOffset));
		}
		SumCounters<kRemainderBits>(rawHistograms + kBaseCount * (kBaseSize + kAntiAliasOffset));

		SRadixUnrollHelper<	SRadixSortUnrollableFunctor<_KeyExtractor, _TypeIt, kBaseBits, kAntiAliasOffset>,
							_KeyExtractor,
							_TypeIt,
							kBaseCount>::Unroll(rawHistograms, elementCount, keyExtractor, curIt, bufIt);
		if (kBaseCount & 1)
		{
			HHSwap(curIt, bufIt);
		}

		hh_u32	*hist = rawHistograms + kBaseCount * (kBaseSize + kAntiAliasOffset);
		for (hh_u32 i = 0; i < elementCount; i++)
		{
			typename _KeyExtractor::SortKeyType	rawKey = keyExtractor.ExtractKey(curIt[i]);
			IntegralKeyType	key = bit_cast<IntegralKeyType>(rawKey);
			hh_u32			pos = EXTRACT_REMAINDER(key);
			bufIt[hist[pos]++] = curIt[i];
		}

#undef	EXTRACT_BASE
#undef	EXTRACT_REMAINDER

		// we need to check if 'bufIt' == 'output', and if not, we need to perform a copy.
		// also, if we need to sort in place, and 'bufIt' != 'first', copy back into 'first'
		static const bool	_InPlace = (_SortOutput == Sort_InPlace);
		if (((kBaseCount & 1) ^ (!_InPlace)) == 0)
		{
			HH_ASSERT(((bufIt != output) ^ (!_InPlace)) == 0);
			for (hh_u32 i = 0; i < elementCount; i++)
			{
				*output = *bufIt;
				++output;
				++bufIt;
			}
		}
		else
		{
			HH_ASSERT(((bufIt == output) ^ (!_InPlace)) == 0);
		}
	}

};

//----------------------------------------------------------------------------
// predefined RadixSort key extractors:

template<typename _Type>
struct	SRadixSortExtractorPOT
{
	typedef _Type						SortKeyType;
	HH_FORCEINLINE static SortKeyType	ExtractKey(const _Type &element) { return element; }
};

template<typename _Type>
struct	SRadixSortExtractorUnsignedFP
{
	typedef typename TNumericTraits<_Type>::SelfFpTypeBits	SortKeyType;
	HH_FORCEINLINE static SortKeyType	ExtractKey(const _Type &element) { return *(SortKeyType*)&element; }
};

template<typename _Type>
struct	SRadixSortExtractorSignedFP
{
	typedef typename TNumericTraits<_Type>::SelfFpTypeBits	SortKeyType;
	HH_FORCEINLINE static SortKeyType	ExtractKey(const _Type &element)	// watchout.. VC++ compiler fuck-up: generates an unused fld/fstp pair when reading from a float stream through iterators
	{
		typedef typename TNumericTraits<_Type>::SelfFpTypeBits			FpBitsType;
		typedef typename TNumericTraits<_Type>::SelfFpTypeBitsSigned	FpBitsTypeSigned;

		const FpBitsType	*fpBits = reinterpret_cast<const FpBitsType*>(&element);
		FpBitsType			signMask = static_cast<FpBitsTypeSigned>(*fpBits) >> 31;	// signed shift
		return *fpBits ^ (signMask | 0x80000000);	// flip the sign bit, and reverse all bits if we were a negative value
	}
};

typedef	SRadixSortExtractorPOT<hh_u32>			SRadixSortExtractorU32;
typedef	SRadixSortExtractorUnsignedFP<float>	SRadixSortExtractorUFloat;
typedef	SRadixSortExtractorSignedFP<float>		SRadixSortExtractorFloat;

//----------------------------------------------------------------------------
//
//	Composite sort.
//
//	PopcornFX::Sort()
//
//	'output' must NOT alias the 'first' -> 'last' memory
//	'output' must be able to iterate over at least 'last' - 'first' elements.
//
//----------------------------------------------------------------------------

template<typename _KeyExtractor, typename _TypeIt>
HH_FORCEINLINE void Sort(_TypeIt first, _TypeIt last, _TypeIt output)
{
	const hh_u32	count = last - first;
	if (count < 110)	// should we check for small values and use an in-place insertion sort early on ?
	{
		_TypeIt	itSrc = first;
		_TypeIt	itDst = output;
		while (itSrc < last)
		{
			*itDst = *itSrc;
			++itDst;
			++itSrc;
		}

		QuickSort(output, output + count);
	}
	else
	{
		_KeyExtractor	dummyExtractor;
		TRadixSort<Sort_InBuffer>::Sort11<_KeyExtractor, _TypeIt>(first, last, output, dummyExtractor);
	}
}

//----------------------------------------------------------------------------
__PK_API_END

#endif
