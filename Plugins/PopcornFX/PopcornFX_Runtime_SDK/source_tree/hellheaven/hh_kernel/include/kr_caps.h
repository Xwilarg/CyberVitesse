#pragma once

//----------------------------------------------------------------------------
// Created on Sat 2003/10/25 03:31 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_CAPS_H__
#define	__KR_CAPS_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CStream;

class	CBaseDigestableCaps
{
public:
	virtual ~CBaseDigestableCaps() {}
	virtual void	FillDigestStream(CStream &stream) const = 0;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif

