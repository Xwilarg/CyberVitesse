#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2011/05/10 11:49 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_STRING_ID_H__
#define __KR_STRING_ID_H__

#include "hh_kernel/include/kr_string.h"	// not really needed, except for the ctor. we could implement the ctor in a .cpp and declare it whithin an #ifdef __KR_STRING_H__ ?

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if	!defined(KR_STRINGID_USE_U32_INDEX)
#	if	defined(HH_CPU_64BITS)
#		define	KR_STRINGID_USE_U32_INDEX	1
#	else
#		define	KR_STRINGID_USE_U32_INDEX	1
#	endif
#endif

//----------------------------------------------------------------------------

class			HH_KERNEL_EXPORT CStringId
{
private:
#if	(KR_STRINGID_USE_U32_INDEX == 1)
	hh_u32		m_Id;
#else
	const char	*m_Id;
#endif

public:
	CStringId() : m_Id(0) {}
	CStringId(const CStringId &other) : m_Id(other.m_Id) {}
	explicit CStringId(const char *str) { Reset(str); }
	explicit CStringId(const CString &str) { Reset(str.Data()); }

	HH_FORCEINLINE bool		operator == (const CStringId &other) const { return other.m_Id == m_Id; }
	HH_FORCEINLINE bool		operator != (const CStringId &other) const { return other.m_Id != m_Id; }

#if	(KR_STRINGID_USE_U32_INDEX == 1)
	const CString			&ToString() const;
#else
	CString					ToString() const;
#endif

	const char				*ToStringData() const;
	CStringView				ToStringView() const;
	void					FillStringView(CStringView &outView) const;
	hh_u32					Hash() const;

	void					Reset(const char *str);	// doesn't accept null strings
	HH_FORCEINLINE void		Clear() { m_Id = 0; }

	HH_FORCEINLINE bool		Empty() const { HH_ASSERT(Null.m_Id == 0); return m_Id == 0; }
	HH_FORCEINLINE bool		Valid() const { return !Empty(); }

	static CStringId		Null;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_STRING_ID_H__
