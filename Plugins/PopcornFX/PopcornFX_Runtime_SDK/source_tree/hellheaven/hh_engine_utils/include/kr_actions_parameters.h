#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2008/04/28 10:37 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_ACTIONS_PARAMETERS_H__
#define __KR_ACTIONS_PARAMETERS_H__

#include "hh_kernel/include/kr_containers_any.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CActionsInternals;

class	HH_ENGINEUTILS_EXPORT CActionParameterDictionnary
{
private:
	friend class	CActionsInternals;

	static bool	Startup();
	static void	Shutdown();

protected:
	static CTypeDictionnary		*m_NameDictionnary;

	bool	DeepCopy(const CActionParameterDictionnary &other) { return true; }
public:
};

//----------------------------------------------------------------------------

//template class HH_ENGINEUTILS_EXPORT TTypedParameterList<CActionParameterDictionnary>;
typedef	TTypedParameterList<CActionParameterDictionnary>	CActionParameters;
HH_DECLARE_REFPTRCLASS(ActionParameters);

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_ACTIONS_PARAMETERS_H__
