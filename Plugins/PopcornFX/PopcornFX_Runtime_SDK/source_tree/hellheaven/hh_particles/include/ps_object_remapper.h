#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2012/12/10 17:14 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__PS_OBJECT_REMAPPER_H__
#define	__PS_OBJECT_REMAPPER_H__

#include <hh_kernel/include/kr_callbacks.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type, typename _CbType>
class	TObjectMapping;

//----------------------------------------------------------------------------

template<typename _Type>
class	TRemappableObjectSpecialization
{
private:
	template<typename _Type2> friend class	TRemappableObject;

	static CGuid	m_StaticRemapID;

public:
	static HH_FORCEINLINE CGuid	StaticRemapID() { return m_StaticRemapID; }
};

//----------------------------------------------------------------------------

template<typename _Type>
class	TRemappableObject
{
private:
	template<typename _Type2, typename _CbType> friend class	TObjectMapping;
	static hh_u32	m_NumMappings;

	CGuid	m_DynamicRemapID;

public:
	TRemappableObject(CGuid remapId) : m_DynamicRemapID(remapId) { /*HH_ASSERT(m_DynamicRemapID.Valid());*/ }

	HH_FORCEINLINE CGuid	DynamicRemapID() const { return m_DynamicRemapID; }

	// called at PK startup
	template<typename _SpecializedType>
	static void	RegisterSpecialization()
	{
		_SpecializedType::m_StaticRemapID = m_NumMappings++;
	}

	// called at PK shutdown
	template<typename _SpecializedType>
	static void	UnregisterSpecialization()
	{
		HH_ASSERT(m_NumMappings != 0);
		--m_NumMappings;
		_SpecializedType::m_StaticRemapID = CGuid::INVALID;
	}
};

template<typename _Type>
hh_u32	TRemappableObject<_Type>::m_NumMappings;

//----------------------------------------------------------------------------

template<typename _RemapType, typename _CbType>
class	TObjectMapping
{
private:
	TArray<FastDelegate<_CbType> >	m_Mappings;
	FastDelegate<_CbType>			m_DefaultMapping;

public:
	TObjectMapping(FastDelegate<_CbType> defaultRemap = null)
	:	m_DefaultMapping(defaultRemap)
	{
		if (m_Mappings.Resize(_RemapType::m_NumMappings))
		{
			for (hh_u32 i = 0; i < m_Mappings.Count(); i++)
			{
				m_Mappings[i] = defaultRemap;
			}
		}
	}

	template<typename _Type>
	CGuid	Find()
	{
//		const CGuid	id = _Type::template TRemappableObjectSpecialization<_Type>::StaticRemapID();
		const CGuid	id = _Type::StaticRemapID();
		HH_ASSERT(id.Valid());
		if (id < m_Mappings.Count() && m_Mappings[id] != null)
			return id;
		return CGuid::INVALID;
	}

	template<typename _Type>
	CGuid	Find(const TRemappableObject<_Type> *object)
	{
		const CGuid	id = object->DynamicRemapID();
		HH_ASSERT(id.Valid());
		if (id < m_Mappings.Count() && m_Mappings[id] != null)
			return id;
		return CGuid::INVALID;
	}

	template<typename _Type>
	bool	IsCustomRemapped() const
	{
		const CGuid	id = _Type::StaticRemapID();
		HH_ASSERT(id.Valid());
		if (id < m_Mappings.Count() && *const_cast<FastDelegate<_CbType>*>(&(m_Mappings[id])) != null) // FastDelegate::operator != non-const WTF ?!
			return m_DefaultMapping != m_Mappings[id];
		return false;
	}

	const FastDelegate<_CbType>	&Remap(CGuid id) const { HH_ASSERT(id.Valid()); return m_Mappings[id]; }

	template<typename _Type>
	const FastDelegate<_CbType>	&Remap(const TRemappableObject<_Type> *object) const
	{
		const CGuid	id = object->DynamicRemapID();
		HH_ASSERT(id.Valid());
		HH_ASSERT(id < m_Mappings.Count());
		if (id.Valid())
			return m_Mappings[id];
		return m_DefaultMapping;
	}

	template<typename _Type>
	bool	SetRemap(const FastDelegate<_CbType> &remap)
	{
		if (m_Mappings.Count() < _RemapType::m_NumMappings)
			return false;

//		const CGuid	id = _Type::template TRemappableObjectSpecialization<_Type>::StaticRemapID();
		const CGuid	id = _Type::StaticRemapID();
		if (!id.Valid())
			return false;

		m_Mappings[id] = remap;
		return true;
	}

	void	ClearMappings()
	{
		for (hh_u32 i = 0; i < m_Mappings.Count(); ++i)
			m_Mappings[i] = m_DefaultMapping;
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __PS_OBJECT_REMAPPER_H__
