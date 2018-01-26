#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/11/13 21:47 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_UNKNOWN_H__
#define __HBO_UNKNOWN_H__

#include "hh_base_object/include/hbo_object.h"
#include "hh_base_object/include/hbo_config.h"

#if	(HH_BASE_OBJECT_HANDLE_UNKNOWN != 0)

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	HBO
{

	class	HH_BASEOBJECT_EXPORT CUnknownClass
	{
	private:
		void			CopyFrom(const CUnknownClass &other);

	public:
		CString			m_Type;
		CString			m_Name;
		CString			m_Body;
		hh_u32			m_BinarySize;
		void			*m_BinaryBody;
		CInt2			m_PositionInEditor;

		CUnknownClass();
		CUnknownClass(const CUnknownClass &other) { CopyFrom(other); }
		~CUnknownClass();

		void			operator = (const CUnknownClass &other) { CopyFrom(other); }

		bool			IsBinary() const;
		bool			IsText() const;

		bool			operator == (const CUnknownClass &other) const;
	};

}

//----------------------------------------------------------------------------
__PK_API_END

#endif	// HH_BASE_OBJECT_HANDLE_UNKNOWN

#endif // __HBO_UNKNOWN_H__
