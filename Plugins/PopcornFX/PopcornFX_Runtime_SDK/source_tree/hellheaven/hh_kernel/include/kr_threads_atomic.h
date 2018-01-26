#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2009/02/02 7:02 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_THREADS_ATOMIC_H__
#define __KR_THREADS_ATOMIC_H__

#include "include/hellheaven.h"

#if		defined(HH_WINRT)

#	define	HH_THREADS_WINRT
#	include <intrin.h>
#	undef	Yield	// wtf WinBase.h ?! god dammit

#elif	defined(HH_WIN32)

#	define	HH_THREADS_WIN32
#	include <intrin.h>
#	undef	Yield	// wtf WinBase.h ?! god dammit

#elif	defined(HH_LINUX) || defined(HH_ANDROID)

#	define	HH_THREADS_POSIX
#	include <sys/resource.h>

#elif defined(HH_MACOSX) || defined(HH_IOS)

#	define	HH_THREADS_POSIX
#	define	HH_THREADS_SEM_MACOSX

#	include <sys/resource.h>
// semaphore
#	include <mach/semaphore.h>
//#	include <mach/task.h>
//#	include <mach/mach_init.h>
//#	include <mach/error.h>

#elif	defined(HH_X360)

#	define	HH_THREADS_XBOX
//#	include <intrin.h>
#	undef	Yield	// wtf WinBase.h ?! god dammit

#elif	defined(HH_DURANGO)

#	define	HH_THREADS_DURANGO
#	include <intrin.h>
#	undef	Yield	// wtf WinBase.h ?! god dammit

#elif	defined(HH_PS3)

//#	define	HH_THREADS_POSIX
#	define	HH_THREADS_PS3

#elif	defined(HH_ORBIS)

//#	define	HH_THREADS_POSIX
#	define	HH_THREADS_ORBIS

#elif	defined(HH_PSP2)

#	define	HH_THREADS_PSP2
#	include <sce_atomic.h>

#elif	defined(HH_NX)

#	define	HH_THREADS_NX

#else
#	error	unsupported Threading API
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// sanity check:

#if defined(HH_THREADS_WIN32) && !defined(HH_WIN32)
#	error	corrupt threads config
#elif defined(HH_THREADS_DURANGO) && !defined(HH_DURANGO)
#	error	corrupt threads config
#elif defined(HH_THREADS_WINRT) && !defined(HH_WINRT)
#	error	corrupt threads config
#endif

//----------------------------------------------------------------------------

#if defined(HH_COMPILER_INTEL)

// returns '*__dst' prior to modification
#	define __HH_INTERLOCKED_EXCHANGE(__dst, __val)							_InterlockedExchange((void*)(__dst), __val)
#	define __HH_INTERLOCKED_EXCHANGE64(__dst, __val)						_InterlockedExchange64((void*)(__dst), __val)
#	define __HH_INTERLOCKED_COMPARE_EXCHANGE(__dst, __val, __cmp)			_InterlockedCompareExchange((void*)(__dst), __val, __cmp)
#	define __HH_INTERLOCKED_COMPARE_EXCHANGE64(__dst, __val, __cmp)			_InterlockedCompareExchange64((void*)(__dst), __val, __cmp)
#	if defined(HH_CPU_64BITS)
#		define __HH_INTERLOCKED_COMPARE_EXCHANGE128(__dst, __val, __cmp)	_InterlockedCompareExchange128((void*)(__dst), __val, __cmp)
#	endif
#	define __HH_INTERLOCKED_EXCHANGE_ADD(__dst, __val)						_InterlockedExchangeAdd((void*)(__dst), __val)
#	define __HH_INTERLOCKED_EXCHANGE_ADD64(__dst, __val)					_InterlockedExchangeAdd64((void*)(__dst), __val)
#	define __HH_INTERLOCKED_AND(__dst, __val)								_InterlockedAnd((void*)(__dst), __val)
#	define __HH_INTERLOCKED_AND64(__dst, __val)								_InterlockedAnd64((void*)(__dst), __val)
#	define __HH_INTERLOCKED_OR(__dst, __val)								_InterlockedOr((void*)(__dst), __val)
#	define __HH_INTERLOCKED_OR64(__dst, __val)								_InterlockedOr64((void*)(__dst), __val)
#	define __HH_INTERLOCKED_XOR(__dst, __val)								_InterlockedXor((void*)(__dst), __val)
#	define __HH_INTERLOCKED_XOR64(__dst, __val)								_InterlockedXor64((void*)(__dst), __val)
// returns '*__dst' after modification
#	define __HH_INTERLOCKED_INCREMENT(__dst)								_InterlockedIncrement((void*)(__dst))	// implemented as 'xadd' under x86, but keep it separate in case some architectures have a specific instruction
#	define __HH_INTERLOCKED_INCREMENT64(__dst)								_InterlockedIncrement64((void*)(__dst))
#	define __HH_INTERLOCKED_DECREMENT(__dst)								_InterlockedDecrement((void*)(__dst))	// implemented as 'xadd' under x86, but keep it separate in case some architectures have a specific instruction
#	define __HH_INTERLOCKED_DECREMENT64(__dst)								_InterlockedDecrement64((void*)(__dst))

#elif defined(HH_COMPILER_MSVC)

// returns '*__dst' prior to modification
#	define __HH_INTERLOCKED_EXCHANGE(__dst, __val)							_InterlockedExchange((volatile long*)(__dst), __val)
#	define __HH_INTERLOCKED_EXCHANGE64(__dst, __val)						_InterlockedExchange64((volatile __int64*)(__dst), __val)
#	define __HH_INTERLOCKED_COMPARE_EXCHANGE(__dst, __val, __cmp)			_InterlockedCompareExchange((volatile long*)(__dst), __val, __cmp)
#	define __HH_INTERLOCKED_COMPARE_EXCHANGE64(__dst, __val, __cmp)			_InterlockedCompareExchange64((volatile __int64*)(__dst), __val, __cmp)
#	if defined(HH_CPU_64BITS)
#		define __HH_INTERLOCKED_COMPARE_EXCHANGE128(__dst, __val, __cmp)	_InterlockedCompareExchange128((volatile long*)(__dst), __val, __cmp)
#	endif
#	define __HH_INTERLOCKED_EXCHANGE_ADD(__dst, __val)						_InterlockedExchangeAdd((volatile long*)(__dst), __val)
#	define __HH_INTERLOCKED_EXCHANGE_ADD64(__dst, __val)					_InterlockedExchangeAdd64((volatile __int64*)(__dst), __val)
#	define __HH_INTERLOCKED_AND(__dst, __val)								_InterlockedAnd((volatile long*)(__dst), __val)
#	define __HH_INTERLOCKED_AND64(__dst, __val)								_InterlockedAnd64((volatile __int64*)(__dst), __val)
#	define __HH_INTERLOCKED_OR(__dst, __val)								_InterlockedOr((volatile long*)(__dst), __val)
#	define __HH_INTERLOCKED_OR64(__dst, __val)								_InterlockedOr64((volatile __int64*)(__dst), __val)
#	define __HH_INTERLOCKED_XOR(__dst, __val)								_InterlockedXor((volatile long*)(__dst), __val)
#	define __HH_INTERLOCKED_XOR64(__dst, __val)								_InterlockedXor64((volatile __int64*)(__dst), __val)
// returns '*__dst' after modification
#	define __HH_INTERLOCKED_INCREMENT(__dst)								_InterlockedIncrement((volatile long*)(__dst))	// implemented as 'xadd' under x86, but keep it separate in case some architectures have a specific instruction
#	define __HH_INTERLOCKED_INCREMENT64(__dst)								_InterlockedIncrement64((volatile __int64*)(__dst))
#	define __HH_INTERLOCKED_DECREMENT(__dst)								_InterlockedDecrement((volatile long*)(__dst))	// implemented as 'xadd' under x86, but keep it separate in case some architectures have a specific instruction
#	define __HH_INTERLOCKED_DECREMENT64(__dst)								_InterlockedDecrement64((volatile __int64*)(__dst))

#elif	defined(HH_COMPILER_GCC) || defined(HH_COMPILER_SNC)

// Clang ftw
#	if defined(__has_builtin) // what ?
#		if __has_builtin(__sync_swap)
#			define __HH_HAS_BUILTIN_SYNC_SWAP
#		endif
#	endif
#	ifdef __HH_HAS_BUILTIN_SYNC_SWAP
#		define __HH_INTERLOCKED_EXCHANGE(__dst, __val)						__sync_swap((volatile int32_t*)(__dst), __val)
#		define __HH_INTERLOCKED_EXCHANGE64(__dst, __val)					__sync_swap((volatile int64_t*)(__dst), __val)
#	else
#		define __HH_INTERLOCKED_EXCHANGE(__dst, __val)						({ __sync_synchronize(); __sync_lock_test_and_set((volatile int32_t*)(__dst), __val); })
#		define __HH_INTERLOCKED_EXCHANGE64(__dst, __val)					({ __sync_synchronize(); __sync_lock_test_and_set((volatile int64_t*)(__dst), __val); })
#	endif
#	undef __HH_HAS_BUILTIN_SYNC_SWAP

#	define __HH_INTERLOCKED_COMPARE_EXCHANGE(__dst, __val, __cmp)			__sync_val_compare_and_swap((volatile int32_t*)(__dst), __cmp, __val)
#	define __HH_INTERLOCKED_COMPARE_EXCHANGE64(__dst, __val, __cmp)			__sync_val_compare_and_swap((volatile int64_t*)(__dst), __cmp, __val)
#	if defined(HH_CPU_64BITS)
#		define __HH_INTERLOCKED_COMPARE_EXCHANGE128(__dst, __val, __cmp)	__sync_val_compare_and_swap((volatile __int128_t*)(__dst), __cmp, __val)
#	endif
#	define __HH_INTERLOCKED_EXCHANGE_ADD(__dst, __val)						__sync_fetch_and_add((volatile int32_t*)(__dst), __val)
#	define __HH_INTERLOCKED_EXCHANGE_ADD64(__dst, __val)					__sync_fetch_and_add((volatile int64_t*)(__dst), __val)
#	define __HH_INTERLOCKED_AND(__dst, __val)								__sync_fetch_and_and((volatile int32_t*)(__dst), __val)
#	define __HH_INTERLOCKED_AND64(__dst, __val)								__sync_fetch_and_and((volatile int64_t*)(__dst), __val)
#	define __HH_INTERLOCKED_OR(__dst, __val)								__sync_fetch_and_or((volatile int32_t*)(__dst), __val)
#	define __HH_INTERLOCKED_OR64(__dst, __val)								__sync_fetch_and_or((volatile int64_t*)(__dst), __val)
#	define __HH_INTERLOCKED_XOR(__dst, __val)								__sync_fetch_and_xor((volatile int32_t*)(__dst), __val)
#	define __HH_INTERLOCKED_XOR64(__dst, __val)								__sync_fetch_and_xor((volatile int64_t*)(__dst), __val)
// returns '*__dst' after modification
#	define __HH_INTERLOCKED_INCREMENT(__dst)								__sync_add_and_fetch((volatile int32_t*)(__dst), 1)
#	define __HH_INTERLOCKED_INCREMENT64(__dst)								__sync_add_and_fetch((volatile int64_t*)(__dst), 1)
#	define __HH_INTERLOCKED_DECREMENT(__dst)								__sync_sub_and_fetch((volatile int32_t*)(__dst), 1)
#	define __HH_INTERLOCKED_DECREMENT64(__dst)								__sync_sub_and_fetch((volatile int64_t*)(__dst), 1)

#else
#	error implement for this compiler
#endif

// NOTE: also use the '__lwsync()' intrinsic when compiling for XBOX360

//----------------------------------------------------------------------------
//
//	Atomic / interlocked primitives
//
//----------------------------------------------------------------------------

template<typename _Type, hh_u32 typeSize = sizeof(_Type)>
struct	SInterlockedPrimitives	// default for 32 bits types
{
	HH_STATIC_ASSERT(sizeof(_Type) == typeSize && sizeof(_Type) == 4);
	HH_STATIC_ASSERT_CUSTOM(TTypeTraits::IsFloat<_Type>::False, "__Interlocked_Primitives_Do_Not_Work_On_Floating_Point_Types___Use_The_Integer_Representation__");
	HH_FORCEINLINE static _Type	_cmpxchg(_Type volatile *dst, _Type src, _Type cmp) { return (_Type)__HH_INTERLOCKED_COMPARE_EXCHANGE(dst, (hh_u32)(hh_ureg)src, (hh_u32)(hh_ureg)cmp); }
	HH_FORCEINLINE static _Type	_xchg(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_EXCHANGE(dst, (hh_u32)(hh_ureg)src); }
	HH_FORCEINLINE static _Type	_xadd(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_EXCHANGE_ADD(dst, (hh_u32)(hh_ureg)src); }
	HH_FORCEINLINE static _Type	_and(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_AND(dst, (hh_u32)(hh_ureg)src); }
	HH_FORCEINLINE static _Type	_or(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_OR(dst, (hh_u32)(hh_ureg)src); }
	HH_FORCEINLINE static _Type	_xor(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_XOR(dst, (hh_u32)(hh_ureg)src); }
	HH_FORCEINLINE static _Type	_xinc(_Type volatile *dst) { return (_Type)__HH_INTERLOCKED_INCREMENT(dst); }
	HH_FORCEINLINE static _Type	_xdec(_Type volatile *dst) { return (_Type)__HH_INTERLOCKED_DECREMENT(dst); }
};

template<typename _Type>
struct	SInterlockedPrimitives<_Type, 8>	// specialization for 64 bits types
{
	HH_STATIC_ASSERT(sizeof(_Type) == 8);
	HH_STATIC_ASSERT_CUSTOM(TTypeTraits::IsFloat<_Type>::False, "__Interlocked_Primitives_Do_Not_Work_On_Floating_Point_Types___Use_The_Integer_Representation__");
	HH_FORCEINLINE static _Type	_cmpxchg(_Type volatile *dst, _Type src, _Type cmp) { return (_Type)__HH_INTERLOCKED_COMPARE_EXCHANGE64(dst, (hh_i64)src, (hh_i64)cmp); }
	HH_FORCEINLINE static _Type	_xchg(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_EXCHANGE64(dst, (hh_i64)src); }
	HH_FORCEINLINE static _Type	_xadd(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_EXCHANGE_ADD64(dst, (hh_i64)src); }
	HH_FORCEINLINE static _Type	_and(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_AND64(dst, (hh_i64)src); }
	HH_FORCEINLINE static _Type	_or(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_OR64(dst, (hh_i64)src); }
	HH_FORCEINLINE static _Type	_xor(_Type volatile *dst, _Type src) { return (_Type)__HH_INTERLOCKED_XOR64(dst, (hh_i64)src); }
	HH_FORCEINLINE static _Type	_xinc(_Type volatile *dst) { return (_Type)__HH_INTERLOCKED_INCREMENT64(dst); }
	HH_FORCEINLINE static _Type	_xdec(_Type volatile *dst) { return (_Type)__HH_INTERLOCKED_DECREMENT64(dst); }
};

#if defined(HH_CPU_64BITS)
template<typename _Type>
struct	SInterlockedPrimitives<_Type, 16>	// specialization for 128 bits types (mainly used for swapping two pointers on 64-bits mode)
{
	HH_STATIC_ASSERT(sizeof(_Type) == 16);
	HH_STATIC_ASSERT_CUSTOM(TTypeTraits::IsFloat<_Type>::False, "__Interlocked_Primitives_Do_Not_Work_On_Floating_Point_Types___Use_The_Integer_Representation__");
	HH_FORCEINLINE static _Type	_cmpxchg(_Type volatile *dst, _Type src, _Type cmp) { return (_Type)__HH_INTERLOCKED_COMPARE_EXCHANGE128(dst, (hh_i64)src, (hh_i64)cmp); }
};
#endif

//----------------------------------------------------------------------------
// use these:

namespace	Atomic
{
	// returns the initial value
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_CMPXCHG(_Type volatile *dst, _Type src, _Type cmp) { return SInterlockedPrimitives<_Type>::_cmpxchg(dst, src, cmp); }
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_XCHG(_Type volatile *dst, _Type src) { return SInterlockedPrimitives<_Type>::_xchg(dst, src); }
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_XADD(_Type volatile *dst, _Type src) { return SInterlockedPrimitives<_Type>::_xadd(dst, src); }
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_AND(_Type volatile *dst, _Type src) { return SInterlockedPrimitives<_Type>::_and(dst, src); }
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_OR(_Type volatile *dst, _Type src) { return SInterlockedPrimitives<_Type>::_or(dst, src); }
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_XOR(_Type volatile *dst, _Type src) { return SInterlockedPrimitives<_Type>::_xor(dst, src); }
	// returns the modified value
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_XINC(_Type volatile *dst) { return SInterlockedPrimitives<_Type>::_xinc(dst); }
	template<typename _Type> HH_FORCEINLINE _Type	Interlocked_XDEC(_Type volatile *dst) { return SInterlockedPrimitives<_Type>::_xdec(dst); }
}

//----------------------------------------------------------------------------
//
//	Atomic helpers
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TAtomic
{
private:
	volatile _Type			m_Value;
public:
	HH_FORCEINLINE TAtomic() : m_Value(0) {}
	HH_FORCEINLINE TAtomic(_Type value) : m_Value(value) {}

	HH_FORCEINLINE _Type	_UnsafeGetValue() const { return m_Value; }
	HH_FORCEINLINE void		_UnsafeSetValue(_Type value) { m_Value = value; }

	HH_FORCEINLINE operator _Type() const { return m_Value; }

	HH_FORCEINLINE void		operator = (_Type value) { SInterlockedPrimitives<_Type>::_xchg(&m_Value, value); }
	HH_FORCEINLINE void		Swap(_Type &o) { o = SInterlockedPrimitives<_Type>::_xchg(&m_Value, o); }

	// SetValueIfEqualTo returns the value before the modification, if any
	HH_FORCEINLINE _Type	SetValueIfEqualTo(_Type value, _Type ifEqualTo) { return SInterlockedPrimitives<_Type>::_cmpxchg(&m_Value, value, ifEqualTo); }

	HH_FORCEINLINE _Type	operator ++ () { return SInterlockedPrimitives<_Type>::_xinc(&m_Value); }								// ++x -> returns the new value
	HH_FORCEINLINE _Type	operator -- () { return SInterlockedPrimitives<_Type>::_xdec(&m_Value); }								// --x -> returns the new value
	HH_FORCEINLINE _Type	operator ++ (int) { return SInterlockedPrimitives<_Type>::_xadd(&m_Value, static_cast<_Type>(1)); }		// x++ -> returns the old value
	HH_FORCEINLINE _Type	operator -- (int) { return SInterlockedPrimitives<_Type>::_xadd(&m_Value, static_cast<_Type>(-1)); }	// x-- -> returns the old value

	// these mimic C++ std assignment operators w/r to the return value. if you want to grab the original value instead of the modified value,
	// use the SInterlockedPrimitives<> class directly, they return the old value (except _xinc and _xdec)

	HH_FORCEINLINE _Type	operator += (_Type v) { return SInterlockedPrimitives<_Type>::_xadd(&m_Value, v) + v; }					// returns the new value
	HH_FORCEINLINE _Type	operator -= (_Type v) { return SInterlockedPrimitives<_Type>::_xadd(&m_Value, (_Type)-(typename TNumericTraits<_Type>::SignedType)v) - (typename TNumericTraits<_Type>::SignedType)v; }	// returns the new value
	HH_FORCEINLINE _Type	operator &= (_Type v) { return SInterlockedPrimitives<_Type>::_and(&m_Value, v) & v; }
	HH_FORCEINLINE _Type	operator |= (_Type v) { return SInterlockedPrimitives<_Type>::_or(&m_Value, v) | v; }
	HH_FORCEINLINE _Type	operator ^= (_Type v) { return SInterlockedPrimitives<_Type>::_xor(&m_Value, v) ^ v; }

};

//----------------------------------------------------------------------------
// bool specialization

template<>
class	TAtomic<bool>
{
private:
	volatile hh_u32			m_Value;
public:
	HH_FORCEINLINE TAtomic() : m_Value(0) {}
	HH_FORCEINLINE TAtomic(bool value) : m_Value((hh_u32)value) {}

	HH_FORCEINLINE bool		_UnsafeGetValue() const { return !!m_Value; }
	HH_FORCEINLINE void		_UnsafeSetValue(bool value) { m_Value = value; }

	HH_FORCEINLINE operator bool() const { return !!m_Value; }	// FIXME: check the code generated for "!!m_Value"

	HH_FORCEINLINE void		operator = (bool value) { SInterlockedPrimitives<hh_u32>::_xchg(&m_Value, value); }
	HH_FORCEINLINE void		Swap(bool &o) { o = !!SInterlockedPrimitives<hh_u32>::_xchg(&m_Value, o); }

	// SetValueIfEqualTo returns the value before the modification, if any
	HH_FORCEINLINE bool		SetValueIfEqualTo(bool value, bool ifEqualTo) { return !!SInterlockedPrimitives<hh_u32>::_cmpxchg(&m_Value, value, ifEqualTo); }

};

template <typename _Type>
void		HHSwap(TAtomic<_Type> &a, _Type &b) { a.Swap(b); }

template <typename _Type>
void		HHSwap(_Type &b, TAtomic<_Type> &a) { a.Swap(b); }

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_THREADS_ATOMIC_H__
