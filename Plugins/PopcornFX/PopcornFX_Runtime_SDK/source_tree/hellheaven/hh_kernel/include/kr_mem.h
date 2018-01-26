#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2003/07/05 12:02 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_MEM_H__
#define	__KR_MEM_H__

#include "hh_kernel/include/kr_mem_basics.h"

#ifndef	PV_MODULE_NAME
#	define	PV_MODULE_NAME	"Kernel|Mem|Default"
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

#define KR_MEM_FUNC			PK_PRETTY_FUNCTION

//----------------------------------------------------------------------------

#if	(KR_MEM_DEBUG != 0)
#	define	KR_MEM_DEBUG_ARGS_DECL			, const char *module, const char *file, int line, const char *function
#	define	KR_MEM_DEBUG_ARGS_PASSTHROUGH	, module, file, line, function
#	define	KR_MEM_DEBUG_ARGS				, PV_MODULE_NAME, __FILE__, __LINE__, KR_MEM_FUNC
#else
#	define	KR_MEM_DEBUG_ARGS_DECL
#	define	KR_MEM_DEBUG_ARGS
#	define	KR_MEM_DEBUG_ARGS_PASSTHROUGH
#endif

//----------------------------------------------------------------------------
// generic allocator

namespace	Mem
{
	HH_KERNEL_EXPORT bool		Initialized();

	enum	EOrigin
	{
		Origin_Alloc	= 0,
		Origin_New		= 0x80000000,

		_Origin_Mask	= Origin_Alloc | Origin_New,
	};

#if	(KR_MEM_NO_DEFAULT_ALLOCATORS == 0)
	typedef void				*(*FnSysMalloc)(size_t size);
	typedef void				*(*FnSysRealloc)(void *ptr, size_t size);
	typedef void				(*FnSysFree)(void *ptr);
	typedef void				(*FnOutOfMemory)(bool *outRetryAlloc, hh_u32 size, hh_u32 alignment);
#endif

	typedef void				*(*FnRawAlloc)(hh_u32 size, hh_u32 alignment);
	typedef void				*(*FnRawRealloc)(void *ptr, hh_u32 size, hh_u32 alignment);
	typedef void				(*FnRawFree)(void *ptr);

	HH_KERNEL_EXPORT void		*_RawAlloc(hh_u32 size, hh_u32 alignment, EOrigin origin KR_MEM_DEBUG_ARGS_DECL);
	HH_KERNEL_EXPORT void		*_RawCalloc(hh_u32 size, hh_u32 alignment KR_MEM_DEBUG_ARGS_DECL);
	HH_KERNEL_EXPORT void		*_RawRealloc(void *ptr, hh_u32 size, hh_u32 alignment KR_MEM_DEBUG_ARGS_DECL);

	HH_KERNEL_EXPORT void		_RawFree(void *ptr, Mem::EOrigin origin KR_MEM_DEBUG_ARGS_DECL);

	template<typename _Type>
	HH_FORCEINLINE void			_RawDelete_Separated_Dtor(_Type *ptr)
	{
		// Catch HH_DELETE() on void* pointers, some compilers do not warn on this (typically msvc)
		// If this is hit, it is typically a programming error.
		HH_STATIC_ASSERT(sizeof(_Type) != 0);
		ptr->~_Type();
	}

	HH_FORCEINLINE void			_RawDelete_Separated_Free(void *ptr KR_MEM_DEBUG_ARGS_DECL)
	{
		_RawFree(ptr, Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
	}

	template<typename _Type>
	HH_FORCEINLINE void			_RawDelete(_Type *ptr KR_MEM_DEBUG_ARGS_DECL)
	{
		if (ptr != null)
		{
			_RawDelete_Separated_Dtor(ptr);
			_RawDelete_Separated_Free(ptr KR_MEM_DEBUG_ARGS_PASSTHROUGH);
		}
	}

	//	returns the size of the 'ptr' buffer.
	//	ptr must point to the begining of a memory area allocated by kr_mem! (ptr must be a pointer returned by Mem::Alloc-style functions)
//	HH_KERNEL_EXPORT hh_u32	Size(const void *ptr);

	template<typename _Type>
	HH_DEPRECATED("deprecated v1.11.0.31621 2016/10/04, use HH_TALLOC<_Type>(...) instead")
	_Type	*Alloc(hh_u32 count, hh_u32 alignment = 0) { return reinterpret_cast<_Type*>(_RawAlloc(count * sizeof(_Type), alignment, Origin_Alloc KR_MEM_DEBUG_ARGS)); }
	template<typename _Type>
	HH_DEPRECATED("deprecated v1.11.0.31621 2016/10/04")
	_Type	*Calloc(hh_u32 count, hh_u32 alignment = 0) { return reinterpret_cast<_Type*>(_RawCalloc(count * sizeof(_Type), alignment KR_MEM_DEBUG_ARGS)); }
	template<typename _Type>
	HH_DEPRECATED("deprecated v1.11.0.31621 2016/10/04, use HH_TREALLOC<_Type>(...) instead")
	_Type	*Realloc(_Type *ptr, hh_u32 count, hh_u32 alignment = 0) { return reinterpret_cast<_Type*>(_RawRealloc(ptr, count * sizeof(_Type), alignment KR_MEM_DEBUG_ARGS)); }

	HH_FORCEINLINE void			Free(void *ptr) { _RawFree(ptr, Origin_Alloc KR_MEM_DEBUG_ARGS); }

	template<typename _Type>
	HH_FORCEINLINE void			Delete(_Type *ptr) { _RawDelete(ptr KR_MEM_DEBUG_ARGS); }


	HH_KERNEL_EXPORT bool		OnStack(const void *ptr);
	HH_KERNEL_EXPORT void		CaptureStackBounds(void* &stackBegin, void* &stackEnd);
	HH_KERNEL_EXPORT hh_ureg	StackSpaceLeftAtSP(void *esp = null);	// if null is passed, uses the sp inside the function call
	HH_KERNEL_EXPORT hh_ureg	StackSpaceUsedAtSP(void *esp = null);	// if null is passed, uses the sp inside the function call

	// Helper for template-style typed alloc macros HH_TALLOC<_Type>() and HH_TREALLOC<_Type>()
	struct	TAllocHelper
	{
#if	(KR_MEM_DEBUG != 0)
		const char	*m_Module;
		const char	*m_File;
		int			m_Line;
		const char	*m_Function;

		TAllocHelper(hh_u32 dummy, const char *module, const char *file, int line, const char *function)
		:	m_Module(module)
		,	m_File(file)
		,	m_Line(line)
		,	m_Function(function)
		{
		}

		template<typename _Type>
		HH_FORCEINLINE _Type *Alloc(hh_u32 count, hh_u32 alignment = 0)
		{
			return reinterpret_cast<_Type*>(_RawAlloc(count * sizeof(_Type), alignment, Origin_Alloc, m_Module, m_File, m_Line, m_Function));
		}

		template<typename _Type>
		HH_FORCEINLINE _Type *Calloc(hh_u32 count, hh_u32 alignment = 0)
		{
			return reinterpret_cast<_Type*>(_RawCalloc(count * sizeof(_Type), alignment, m_Module, m_File, m_Line, m_Function));
		}

		template<typename _Type>
		HH_FORCEINLINE _Type *Realloc(_Type *ptr, hh_u32 count, hh_u32 alignment = 0)
		{
			return reinterpret_cast<_Type*>(_RawRealloc(ptr, count * sizeof(_Type), alignment, m_Module, m_File, m_Line, m_Function));
		}
#else
		TAllocHelper(hh_u32 dummy) {}

		template<typename _Type>
		HH_FORCEINLINE _Type *Alloc(hh_u32 count, hh_u32 alignment = 0)
		{
			return reinterpret_cast<_Type*>(_RawAlloc(count * sizeof(_Type), alignment, Origin_Alloc));
		}

		template<typename _Type>
		HH_FORCEINLINE _Type *Calloc(hh_u32 count, hh_u32 alignment = 0)
		{
			return reinterpret_cast<_Type*>(_RawCalloc(count * sizeof(_Type), alignment));
		}

		template<typename _Type>
		HH_FORCEINLINE _Type *Realloc(_Type *ptr, hh_u32 count, hh_u32 alignment = 0)
		{
			return reinterpret_cast<_Type*>(_RawRealloc(ptr, count * sizeof(_Type), alignment));
		}
#endif
	};

};

__PK_API_END
//----------------------------------------------------------------------------
// default 'new' for popcorn runtime

HH_INLINE void	*operator new(size_t size KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	return PopcornFX::Mem::_RawAlloc(PopcornFX::checked_numcast<PopcornFX::hh_u32>(size), 0, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}
HH_INLINE void	*operator new[](size_t size KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	return PopcornFX::Mem::_RawAlloc(PopcornFX::checked_numcast<PopcornFX::hh_u32>(size), 0, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}

// these are only needed if exception handling is enabled, and the compiler needs a matching signature with 'new'
#if	(PK_HAS_EXCEPTIONS != 0)
HH_INLINE void	operator delete(void *object KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	PopcornFX::Mem::_RawFree(object, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}
HH_INLINE void	operator delete[](void *object KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	PopcornFX::Mem::_RawFree(object, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}
#endif

//----------------------------------------------------------------------------
// aligned

HH_INLINE void	*operator new(size_t size, PopcornFX::hh_u32 alignment KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	return PopcornFX::Mem::_RawAlloc(PopcornFX::checked_numcast<PopcornFX::hh_u32>(size), alignment, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}
HH_INLINE void	*operator new[](size_t size, PopcornFX::hh_u32 alignment KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	return PopcornFX::Mem::_RawAlloc(PopcornFX::checked_numcast<PopcornFX::hh_u32>(size), alignment, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}

// these are only needed if exception handling is enabled, and the compiler needs a matching signature with 'new'
#if	(PK_HAS_EXCEPTIONS != 0)
HH_INLINE void	operator delete(void *object, PopcornFX::hh_u32 alignment KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	PopcornFX::Mem::_RawFree(object, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}
HH_INLINE void	operator delete[](void *object, PopcornFX::hh_u32 alignment KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
{
	PopcornFX::Mem::_RawFree(object, PopcornFX::Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
}
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
// debug allocators:

#define	HH_MALLOC_ALIGNED(s, __align)						PopcornFX::Mem::_RawAlloc(s, __align, PopcornFX::Mem::Origin_Alloc KR_MEM_DEBUG_ARGS)
#define	HH_CALLOC_ALIGNED(s, __align)						PopcornFX::Mem::_RawCalloc(s, __align KR_MEM_DEBUG_ARGS)
#define	HH_REALLOC_ALIGNED(p, s, __align)					PopcornFX::Mem::_RawRealloc(p, s, __align KR_MEM_DEBUG_ARGS)

#define	HH_FREE(__buffer)									do { PopcornFX::Mem::_RawFree(__buffer, PopcornFX::Mem::Origin_Alloc KR_MEM_DEBUG_ARGS); (__buffer) = null; } while (0)

#if	(KR_MEM_DEBUG != 0)
#	define	HH_NEW(__ctor)									new(PV_MODULE_NAME, __FILE__, __LINE__, KR_MEM_FUNC, PopcornFX::CMemSpecializer::Specialize()) __ctor
#else
#	define	HH_NEW(__ctor)									new(PopcornFX::CMemSpecializer::Specialize()) __ctor
#endif

#define	HH_NEW_ALIGNED(__ctor, __align)						new(__align KR_MEM_DEBUG_ARGS, PopcornFX::CMemSpecializer::Specialize()) __ctor

//#define	HH_DELETE(__ptrExpr)							delete(CMemSpecializer::Specialize()) __ptrExpr	// placement delete not supported :(
#define	HH_DELETE(__ptr)									PopcornFX::Mem::_RawDelete(__ptr KR_MEM_DEBUG_ARGS)
#define	HH_SAFE_DELETE(__ptr)								do { PopcornFX::Mem::_RawDelete(__ptr KR_MEM_DEBUG_ARGS); (__ptr) = null; } while (0)

#define	HH_MALLOC(s)										HH_MALLOC_ALIGNED(s, 0)
#define	HH_CALLOC(s)										HH_CALLOC_ALIGNED(s, 0)
#define	HH_REALLOC(p, s)									HH_REALLOC_ALIGNED(p, s, 0)

#define HH_TALLOC											PopcornFX::Mem::TAllocHelper(0 KR_MEM_DEBUG_ARGS).Alloc
#define HH_TCALLOC											PopcornFX::Mem::TAllocHelper(0 KR_MEM_DEBUG_ARGS).Calloc
#define HH_TREALLOC											PopcornFX::Mem::TAllocHelper(0 KR_MEM_DEBUG_ARGS).Realloc

//----------------------------------------------------------------------------
// align object allocation

template<hh_u32 _Alignment>
class	TAlignedObject
{
public:
	TAlignedObject() { HH_ASSERT(Mem::IsAligned<_Alignment>(this)); }
	void	*operator new(size_t size KR_MEM_DEBUG_ARGS_DECL, PopcornFX::CMemSpecializer *)
	{
		return Mem::_RawAlloc(PopcornFX::checked_numcast<PopcornFX::hh_u32>(size), _Alignment, Mem::Origin_New KR_MEM_DEBUG_ARGS_PASSTHROUGH);
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_MEM_H__
