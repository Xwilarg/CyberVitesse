#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2010/01/06 12:50 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_RTTI_H__
#define __KR_RTTI_H__

#include "hh_template_metaprogramming.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	RTTI
{
	/*struct	TypeGUID
	{
		hh_u32	m_Value;
	};*/

	typedef CGuid	TypeGUID;

	template<typename _Type>
	class	Type
	{
	};

	class	HH_KERNEL_EXPORT CManager
	{
	public:
		static TypeGUID		FindAndRegisterIFN(const char *typeName);
		static TypeGUID		Find(const char *typeName);
		static const char	*TypeName(TypeGUID typeId);
	};

}

//----------------------------------------------------------------------------
// only registers the type passed, does not care about pointers and all
// must be used inside the 'PopcornFX' namespace

#define	HH_REGISTER_RTTI_SINGLE(__type) \
namespace	RTTI \
{ \
	template<> \
	class	Type<__type> \
	{ \
	public: \
		HH_STATIC_ASSERT(TTypeTraits::IsConst<__type>::False); \
		static const char	*Name() \
		{ \
			return STRINGIFY(__type); \
		} \
		static TypeGUID		ID() \
		{ \
			static const TypeGUID	_TypeID = CManager::FindAndRegisterIFN(STRINGIFY(__type)); \
			return _TypeID; \
		} \
	}; \
}

//----------------------------------------------------------------------------

#define	HH_REGISTER_RTTI_SINGLE_ALIAS(__type, __typeToAlias) \
namespace	RTTI \
{ \
	template<> \
	class	Type<__type> \
	{ \
	public: \
		HH_STATIC_ASSERT(TTypeTraits::IsConst<__type>::False); \
		static const char	*Name() \
		{ \
			return STRINGIFY(__typeToAlias); \
		} \
		static TypeGUID		ID() \
		{ \
			static const TypeGUID	_TypeID = CManager::FindAndRegisterIFN(STRINGIFY(__typeToAlias)); \
			return _TypeID; \
		} \
	}; \
}

//----------------------------------------------------------------------------
// declares a type and a few pointer combinations
// NOTE: don't register references ! storing a ref in the CAny would be pretty hazardous.
// or perhaps we should alias it to __type directly?

#define	HH_REGISTER_RTTI(__type) \
	HH_REGISTER_RTTI_SINGLE(__type); \
	HH_REGISTER_RTTI_SINGLE(__type*) \

#define	HH_REGISTER_RTTI_ALIAS(__type, __typeToAlias) \
	HH_REGISTER_RTTI_SINGLE_ALIAS(__type, __typeToAlias); \
	HH_REGISTER_RTTI_SINGLE_ALIAS(__type*, __typeToAlias*) \

#define	HH_REGISTER_RTTI_REFPTR(__type) \
	HH_REGISTER_RTTI(__type); \
	HH_REGISTER_RTTI_SINGLE(TRefPtr<__type>); \
	HH_REGISTER_RTTI_SINGLE(TWeakPtr<__type>)

//----------------------------------------------------------------------------
//
//	Register common types:
//
//----------------------------------------------------------------------------

/*HH_REGISTER_RTTI_SINGLE(void);
HH_REGISTER_RTTI_SINGLE(void*);*/
HH_REGISTER_RTTI(void);

HH_REGISTER_RTTI(bool);
HH_REGISTER_RTTI(hh_u8);
HH_REGISTER_RTTI(hh_i8);
HH_REGISTER_RTTI(hh_u16);
HH_REGISTER_RTTI(hh_i16);
HH_REGISTER_RTTI(hh_u32);
HH_REGISTER_RTTI(hh_i32);
HH_REGISTER_RTTI(hh_u64);
HH_REGISTER_RTTI(hh_i64);
HH_REGISTER_RTTI(float);
HH_REGISTER_RTTI(double);

//----------------------------------------------------------------------------

HH_REGISTER_RTTI_ALIAS(CByte1, hh_i8);
HH_REGISTER_RTTI(CByte2);
HH_REGISTER_RTTI(CByte3);
HH_REGISTER_RTTI(CByte4);
HH_REGISTER_RTTI_ALIAS(CUbyte1, hh_u8);
HH_REGISTER_RTTI(CUbyte2);
HH_REGISTER_RTTI(CUbyte3);
HH_REGISTER_RTTI(CUbyte4);

HH_REGISTER_RTTI_ALIAS(CWord1, hh_i16);
HH_REGISTER_RTTI(CWord2);
HH_REGISTER_RTTI(CWord3);
HH_REGISTER_RTTI(CWord4);
HH_REGISTER_RTTI_ALIAS(CUword1, hh_u16);
HH_REGISTER_RTTI(CUword2);
HH_REGISTER_RTTI(CUword3);
HH_REGISTER_RTTI(CUword4);

HH_REGISTER_RTTI_ALIAS(CInt1, hh_i32);
HH_REGISTER_RTTI(CInt2);
HH_REGISTER_RTTI(CInt3);
HH_REGISTER_RTTI(CInt4);
HH_REGISTER_RTTI_ALIAS(CUint1, hh_u32);
HH_REGISTER_RTTI(CUint2);
HH_REGISTER_RTTI(CUint3);
HH_REGISTER_RTTI(CUint4);

HH_REGISTER_RTTI_ALIAS(CFloat1, float);
HH_REGISTER_RTTI(CFloat2);
HH_REGISTER_RTTI(CFloat3);
HH_REGISTER_RTTI(CFloat4);

//----------------------------------------------------------------------------

HH_REGISTER_RTTI(CQuaternion);

//----------------------------------------------------------------------------

HH_REGISTER_RTTI(CFloat3x3);
HH_REGISTER_RTTI(CFloat4x3);
HH_REGISTER_RTTI(CFloat4x4);

//----------------------------------------------------------------------------

HH_REGISTER_RTTI(CGuid);
HH_REGISTER_RTTI(CString);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_RTTI_H__
