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

#ifndef __KR_CALLSTACK_WIN_IMPL_H__
#define __KR_CALLSTACK_WIN_IMPL_H__

#ifndef	__KR_CALLSTACK_H__
#	error	kr_callstack.impl.h must not be included manually, include kr_callstack.h instead
#endif
#if	!defined(HH_WINDOWS)
#	error	This file is for Windows only
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CCallstackSymbol
{
private:
	HANDLE		m_Process;
	hh_ureg		m_PCAddr;

public:
	CCallstackSymbol(HANDLE process = null, hh_ureg pcAddr = 0) : m_Process(process), m_PCAddr(pcAddr) {}

	void		Clear() { m_Process = null; m_PCAddr = 0; }
	bool		Empty() const { return m_PCAddr == 0; }

	hh_ureg		PCAddr() const { return m_PCAddr; }
	bool		ResolveModuleName(char *buffer, hh_u32 bufferSize) const;
	bool		ResolveSymbolName(char *buffer, hh_u32 bufferSize) const;
	bool		ResolveFileNameAndLine(char *buffer, hh_u32 bufferSize, hh_u32 &outLine) const;
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CCallstackWalker
{
public:
	static hh_u32	Walk(hh_u32 framesToSkip, hh_ureg *outIPs, hh_u32 outCount);
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CALLSTACK_WIN_IMPL_H__
