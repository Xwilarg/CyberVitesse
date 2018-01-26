#pragma once

//----------------------------------------------------------------------------
// Created on Sun 2004/03/21 09:38 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_SCHEDULER_H__
#define	__KR_SCHEDULER_H__

#include "hh_kernel/include/kr_thread_pool.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Scheduler
{

	typedef	Threads::CAbstractPool			*(*FnCreateThreadPool)();
	typedef	void							(*FnDestroyThreadPool)(Threads::CAbstractPool *pool);

	// Fast, simple getter.
	HH_KERNEL_EXPORT Threads::CAbstractPool	*ThreadPool();

	// Slow, don't call this at runtime if possible, prefer hooking your custom pool
	// to HH-Kernel's startup config structure through an 'FnCreateThreadPool' callback.
	// calling with a null pool will restore Popcorn's default pool
	HH_KERNEL_EXPORT void					SetThreadPool(Threads::CAbstractPool *pool, FnDestroyThreadPool cbDestroyPool = null);

}

//----------------------------------------------------------------------------
__PK_API_END

#endif
