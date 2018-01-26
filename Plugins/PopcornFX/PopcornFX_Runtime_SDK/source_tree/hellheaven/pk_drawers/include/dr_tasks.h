#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2015/01/08 11:34 by Jordan Galby
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include <hh_kernel/include/kr_refptr.h>
#include <hh_kernel/include/kr_threads_job_pool.h>
#include <hh_kernel/include/kr_callbacks.h>

#include "pk_drawers/include/dr_common.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace Drawers
{
	namespace Task
	{
		class	PK_BILLBOARDER_EXPORT CBase : public CAsynchronousPooledJob
		{
		protected:
			virtual bool			_VirtualLaunch() = 0; // TO OVERRIDE

		public:
			/*final*/ bool			ImmediateExecute() { _VirtualLaunch(); return true; }
		private:
			/*final*/ virtual bool	_VirtualLaunch(Threads::SThreadContext &threadContext) override { return ImmediateExecute(); }
		};
		HH_DECLARE_REFPTRCLASS(Base);

		class	PK_BILLBOARDER_EXPORT CEmpty : public CBase
		{
		protected:
			virtual bool			_VirtualLaunch() { return true; }
		};
		HH_DECLARE_REFPTRCLASS(Empty);

		class	PK_BILLBOARDER_EXPORT CExecCallback : public CBase
		{
		public:
			FastDelegate<void()>	m_Cb;
		protected:
			virtual bool			_VirtualLaunch() { if (m_Cb != null) m_Cb(); return true; }
		};
		HH_DECLARE_REFPTRCLASS(ExecCallback);

	} // namespace Task

} // namespace Drawers

//----------------------------------------------------------------------------
__PK_API_END
