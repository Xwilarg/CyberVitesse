#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2008/09/06 12:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_LOG_LISTENERS_H__
#define __KR_LOG_LISTENERS_H__

#include "hh_kernel/include/kr_log.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CLogListenerFile : public ILogListener
{
private:
	enum	EFormat
	{
		Format_PlainText,
		Format_HTML,
	};

	FILE		*m_FileStream;
	EFormat		m_LogFormat;

	void		WriteHeader(const char *title);
	void		WriteFooter();

public:
	CLogListenerFile(const char *filePath, const char *title);
	~CLogListenerFile();

	virtual void	OnAttach(PLogger attachedTo) override;
	virtual void	OnDetach(PLogger detachedFrom) override;
	virtual void	Notify(CLog::ELogLevel level, CGuid logClass, const char *message) override;
};

//----------------------------------------------------------------------------

#ifndef	HH_RETAIL

class	HH_KERNEL_EXPORT CLogListenerDebug : public ILogListener
{
private:
public:
	virtual void	OnAttach(PLogger attachedTo) override;
	virtual void	OnDetach(PLogger detachedFrom) override;
	virtual void	Notify(CLog::ELogLevel level, CGuid logClass, const char *message) override;
};

#endif

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_LOG_LOGGERS_H__
