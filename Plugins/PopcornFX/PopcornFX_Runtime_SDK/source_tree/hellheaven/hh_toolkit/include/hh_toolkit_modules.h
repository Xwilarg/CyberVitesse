#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2009/11/10 15:11 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HH_TOOLKIT_MODULES_H__
#define __HH_TOOLKIT_MODULES_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	DLL-loading
//
//----------------------------------------------------------------------------

struct	TModule
{
private:
	void					*_GetSymbolPtr(const char *symbol);
public:
	TModule() : m_Handle(null) {}

	void					*m_Handle;
	static const TModule	Invalid;

	bool					Valid() const { return m_Handle != null; }

	static TModule			Load(const char *name);
	bool					Destroy();	// returns true if the module has been destroyed, false if an error occured

	template<typename _PtrType>
	HH_FORCEINLINE _PtrType	GetSymbolPtr(const char *symbol)
	{
		return (_PtrType)_GetSymbolPtr(symbol);
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HH_TOOLKIT_MODULES_H__
