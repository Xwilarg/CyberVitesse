#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2008/06/13 10:56 by Alexandre Bourlon
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_GUID_H__
#define	__HH_GUID_H__

//----------------------------------------------------------------------------

template<typename _Type> class TNumericTraits;

//----------------------------------------------------------------------------

template<typename _Type>
class TGuid
{
private:
	_Type					m_Guid;

	// disallow bool expressions, avoids incorrectly testing CGuids in 'if's against zero and getting false negatives for validity.
#if	(PK_HAS_EXPLICIT_CONVERSIONS == 1)
	explicit operator		bool() const;	// if compiler has C++11 feature set, use proper conversion
#else
	bool operator			!() const;		// if not, fallback to operator !()
#endif

public:
	typedef	TGuid<_Type>	SelfType;
	typedef	_Type			Type;

	static const TGuid		INVALID;

	HH_FORCEINLINE TGuid() : m_Guid(TNumericTraits<_Type>::Max()) {}	// Don't use 'INVALID' here ! static initialization order might produce undefined behaviors on statically constructed CGuids
	HH_FORCEINLINE TGuid(_Type guid) : m_Guid(guid) {}

	HH_FORCEINLINE bool		Valid() const { return m_Guid != INVALID.m_Guid; }
	HH_FORCEINLINE void		Clear() { m_Guid = INVALID.m_Guid; }

	HH_FORCEINLINE operator	_Type() const { return m_Guid; }
//	HH_FORCEINLINE explicit operator	_Type() const { return m_Guid; }

//	HH_FORCEINLINE bool		operator == (const SelfType &rhs) const { return m_Guid == rhs.m_Guid; }
//	HH_FORCEINLINE bool		operator != (const SelfType &rhs) const { return m_Guid != rhs.m_Guid; }

	template<typename _OtherType> HH_FORCEINLINE operator	TGuid<_OtherType>() const { if (!Valid()) return TGuid<_OtherType>::Invalid; return checked_numcast<_OtherType>(m_Guid); }

	template<typename _OtherType> HH_FORCEINLINE void		operator = (_OtherType guid) { m_Guid = checked_numcast<_Type>(guid); }
	template<typename _OtherType> HH_FORCEINLINE SelfType	operator + (_OtherType value) const { SelfType guid(*this); guid += value; return guid; }
	template<typename _OtherType> HH_FORCEINLINE SelfType	operator - (_OtherType value) const { SelfType guid(*this); guid -= value; return guid; }
	template<typename _OtherType> HH_FORCEINLINE void		operator += (_OtherType value) { m_Guid += value; HH_PARANOID_ASSERT(static_cast<typename TNumericTraits<_Type>::SignedType>(m_Guid) >= 0); }
	template<typename _OtherType> HH_FORCEINLINE void		operator -= (_OtherType value) { m_Guid -= value; HH_PARANOID_ASSERT(static_cast<typename TNumericTraits<_Type>::SignedType>(m_Guid) >= 0); }
	HH_FORCEINLINE SelfType									operator ++ () { m_Guid += 1; HH_PARANOID_ASSERT(static_cast<typename TNumericTraits<_Type>::SignedType>(m_Guid) >= 0); return *this; }
	HH_FORCEINLINE SelfType									operator -- () { m_Guid -= 1; HH_PARANOID_ASSERT(static_cast<typename TNumericTraits<_Type>::SignedType>(m_Guid) >= 0); return *this; }
	HH_FORCEINLINE SelfType									operator ++ (int) { SelfType guid(*this); this->operator += (1); return guid; }
	HH_FORCEINLINE SelfType									operator -- (int) { SelfType guid(*this); this->operator -= (1); return guid; }
};

//----------------------------------------------------------------------------

template<typename _Type>
const TGuid<_Type>	TGuid<_Type>::INVALID = TGuid<_Type>(TNumericTraits<_Type>::Max());

//----------------------------------------------------------------------------

typedef	TGuid<hh_u32>			CGuid;
typedef	TGuid<volatile hh_u32>	CGuidVolatile;

//----------------------------------------------------------------------------

#endif // __HH_GUID_H__
