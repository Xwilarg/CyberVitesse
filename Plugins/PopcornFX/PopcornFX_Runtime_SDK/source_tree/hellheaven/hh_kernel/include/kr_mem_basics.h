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

#ifndef	__KR_MEM_BASICS_H__
#define	__KR_MEM_BASICS_H__

#include "include/hh_memory.h"
#include "hh_kernel/include/hh_kernel_config.h"

#ifdef	new		// god dammit PlayAll engine !?
#	error	#define new
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	memory alignment
//
//----------------------------------------------------------------------------

#if	(KR_MEM_DEBUG != 0)
#	define	HH_ONLY_IN_MEM_DEBUG(expr)	expr
#else
#	define	HH_ONLY_IN_MEM_DEBUG(expr)
#endif

//----------------------------------------------------------------------------

class	CMemSpecializer
{
public:
	HH_FORCEINLINE static CMemSpecializer	*Specialize()
	{
		return (CMemSpecializer*)0xFFFFFFFF;
	}
};

//----------------------------------------------------------------------------
__PK_API_END

// placement new
HH_FORCEINLINE void	*operator new(size_t, PopcornFX::CMemSpecializer *, void *object) throw()
{
	return object;
}

// we need the matching delete operator in case there is an exception
HH_FORCEINLINE void	operator delete(void *, PopcornFX::CMemSpecializer *, void *) throw()
{
}

//// placement new
//HH_FORCEINLINE void volatile	*operator new(size_t, PopcornFX::CMemSpecializer *, void volatile * object) throw()
//{
//	return object;
//}
//
//// we need the matching delete operator in case there is an exception
//HH_FORCEINLINE void	operator	delete(void *, PopcornFX::CMemSpecializer *, void volatile *) throw()
//{
//}

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Mem
{
	// construction

	template<typename _Type>
	HH_FORCEINLINE _Type	&Construct(_Type &object)
	{
		return *(::new (CMemSpecializer::Specialize(), &object) _Type);
	}

	template<typename _Type, typename _TypeCopy>
	HH_FORCEINLINE _Type	&Construct(_Type &object, const _TypeCopy &copy)
	{
		return *(::new (CMemSpecializer::Specialize(), &object) _Type(copy));
	}

#if	(PK_HAS_RVALUE_REF != 0)
	template<typename _Type, typename _TypeCopy>
	HH_FORCEINLINE _Type	&Construct(_Type &object, _TypeCopy &&copy)
	{
		return *(::new (CMemSpecializer::Specialize(), &object) _Type(copy));
	}
#endif

	// destruction

	template<typename _Type>
	HH_FORCEINLINE void	Destruct(_Type &object)
	{
		object.~_Type();
	}

	// init

	template<typename _Type>
	HH_FORCEINLINE _Type	&Reinit(_Type &object)
	{
		Mem::Destruct(object);
		Mem::Construct(object);
		return object;
	}
}

//----------------------------------------------------------------------------

#define	HH_PLACEMENT_NEW(__data, __ctor)		::new (PopcornFX::CMemSpecializer::Specialize(), PopcornFX::Mem::_RefToPtr(__data)) __ctor
//#define	HH_PLACEMENT_NEW_DEFAULT(__data)	PopcornFX::Mem::Construct(__data);
//#define	HH_PLACEMENT_DELETE(__object)		PopcornFX::Mem::Destruct(__object);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_MEM_BASICS_H__
