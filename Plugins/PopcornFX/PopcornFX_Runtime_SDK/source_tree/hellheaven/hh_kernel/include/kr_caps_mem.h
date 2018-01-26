#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2008/09/28 17:57 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CAPS_MEM_H__
#define __KR_CAPS_MEM_H__

#include "hh_kernel/include/kr_caps.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Mem
{
	//	memory
	//	FIXME: add info about memory bandwidth?
	class	HH_KERNEL_EXPORT CCaps : public CBaseDigestableCaps
	{
	public:
		enum	EType
		{
			Type_Physical = 0,
			Type_PageFile,
			Type_Virtual,

			__MaxTypes
		};

	protected:
		mutable hh_u64		m_Total[__MaxTypes];
		mutable hh_u64		m_Available[__MaxTypes];

		void				*m_MinimumAddress;
		void				*m_MaximumAddress;

#if	(KR_CAPS_DETECT_MEM != 0)
		virtual hh_u64		GetTotalMemory(EType type) const;
		virtual hh_u64		GetAvailableMemory(EType type) const;
#else
		virtual hh_u64		GetTotalMemory(EType type) const = 0;
		virtual hh_u64		GetAvailableMemory(EType type) const = 0;
#endif

	public:
		CCaps();
		~CCaps();

#if	(KR_CAPS_DETECT_MEM != 0)
		bool				Detect();
#endif

		virtual void		Refresh() const;
		hh_u64				Total(EType type) const { return m_Total[type]; }
		hh_u64				Available(EType type) const { return m_Available[type]; }

		void				Dump();
		virtual void		FillDigestStream(CStream &stream) const override {}
	};

	//----------------------------------------------------------------------------

	HH_KERNEL_EXPORT const CCaps		&Caps();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CAPS_MEM_H__
