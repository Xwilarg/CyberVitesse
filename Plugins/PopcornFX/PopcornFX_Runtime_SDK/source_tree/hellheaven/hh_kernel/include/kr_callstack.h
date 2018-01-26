#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2008/07/18 20:54 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CALLSTACK_H__
#define __KR_CALLSTACK_H__

#include "hh_kernel_config.h"

#if	(HH_KERNEL_BUILD_CALLSTACKS != 0)

#if		defined(HH_WINDOWS)
#	include "hh_kernel/include/platforms/win32/kr_callstack.impl.h"
#elif	defined(HH_LINUX) || defined(HH_MACOSX) || defined(HH_ANDROID) || defined(HH_IOS)
#	include "hh_kernel/include/platforms/linux/kr_callstack.impl.h"
#elif	defined(HH_X360)
#	include "hh_kernel/include/platforms/xbox/kr_callstack.impl.h"
#elif	defined(HH_PS3)
#	include "hh_kernel/include/platforms/ps3/kr_callstack.impl.h"
#elif	defined(HH_ORBIS)
#	include "hh_kernel/include/platforms/orbis/kr_callstack.impl.h"
#elif	defined(HH_PSP2)
#	include "hh_kernel/include/platforms/psp2/kr_callstack.impl.h"
#elif	defined(HH_NX)
#	include "hh_kernel/include/platforms/nx/kr_callstack.impl.h"
#else
#	error	unsupported OS Callstacks
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CCallstack
{
public:
	CCallstack();

	TArray<hh_ureg>		m_CallList;

	hh_u32				Capture(hh_u32 framesToSkip);
	void				CaptureSymbol(hh_u32 callIndex, CCallstackSymbol &toBeFilled) const;
};

//----------------------------------------------------------------------------
//
//	small callstack
//
//----------------------------------------------------------------------------

template<hh_u32 _MaxCalls>
class	TSmallCallstack
{
public:
	enum
	{
		MaxCalls = _MaxCalls
	};

	hh_ureg					m_CallList[_MaxCalls];

	TSmallCallstack() { Clear(); }

	bool					Empty() const { return m_CallList[0] == 0; }
	void					Clear() { Mem::Clear(m_CallList); }

	HH_FORCEINLINE hh_u32	Capture(hh_u32 framesToSkip)
	{
		//Clear();
		HH_STATIC_ASSERT(sizeof(m_CallList[0]) == sizeof(void*));
		const hh_u32	callCount = CCallstackWalker::Walk(framesToSkip, m_CallList, _MaxCalls);
		for (hh_u32 i = callCount; i < _MaxCalls; i++)
		{
			m_CallList[i] = 0;
		}
		return callCount;
	}

	void				CaptureSymbol(hh_u32 callIndex, CCallstackSymbol &toBeFilled) const
	{
		HH_ASSERT(callIndex < _MaxCalls);
#if	defined(HH_WINDOWS)
		toBeFilled = CCallstackSymbol(::GetCurrentProcess(), m_CallList[callIndex]);
#else
		toBeFilled = CCallstackSymbol(m_CallList[callIndex]);
#endif
	}

	bool				operator == (const TSmallCallstack<_MaxCalls> &other) const
	{
		for (hh_u32 i = 0; i < _MaxCalls; i++)
		{
			if (m_CallList[i] != other.m_CallList[i])
				return false;
		}
		return true;
	}

	bool				operator != (const TSmallCallstack<_MaxCalls> &other) const
	{
		return !(*this == other);
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif
#endif // __KR_CALLSTACK_H__
