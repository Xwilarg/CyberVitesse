#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/10/23 12:44 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_H__
#define __KR_CONTAINERS_H__

#include "hh_kernel/include/kr_memoryviews.h"
#include "hh_kernel/include/kr_containers_internals.h"
#include "hh_kernel/include/kr_containers_static.h"
#include "hh_kernel/include/kr_containers_array.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
// FIXME: I don't really like that name... "ValidityTypeWrapper".. find a better one...

template<typename _Type>
class	TValidityTypeWrapper
{
private:
	typedef TValidityTypeWrapper<_Type>	SelfType;
	_Type					m_Data;
	hh_u8					m_Invalid;
public:
	TValidityTypeWrapper() : m_Invalid(1) {}
	TValidityTypeWrapper(const _Type &data) : m_Data(data), m_Invalid(0) {}
	explicit TValidityTypeWrapper(hh_u32 dummySpecializer, hh_u8 invalidKey) : m_Invalid(invalidKey) {}

	static const SelfType	Invalid;
	static const SelfType	Invalid2;

	const _Type				&Data() const { return m_Data; }
	_Type					&Data() { return m_Data; }

	void					Invalidate(hh_u8 invalidKey = 1) { m_Invalid = invalidKey; }
	void					Validate() { m_Invalid = 0; }
	bool					Valid() const { return m_Invalid == 0; }

	bool					operator == (const SelfType &other) const { return m_Invalid == other.m_Invalid && m_Data == other.m_Data; }
	bool					operator != (const SelfType &other) const { return !(*this == other); }
	template<typename _OtherType>
	bool					operator == (const _OtherType &other) const { return m_Data == other; }
	template<typename _OtherType>
	bool					operator != (const _OtherType &other) const { return !(*this == other); }
};

template<typename _Type>
const TValidityTypeWrapper<_Type>	TValidityTypeWrapper<_Type>::Invalid = TValidityTypeWrapper<_Type>(0, 1);
template<typename _Type>
const TValidityTypeWrapper<_Type>	TValidityTypeWrapper<_Type>::Invalid2 = TValidityTypeWrapper<_Type>(0, 2);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_H__
