#pragma once

//----------------------------------------------------------------------------
// Created on Fri 2008/08/15 16:11 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_MODULE_API_H__
#define	__HH_MODULE_API_H__

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _BaseModule>
class	TBaseBootableModule
{
protected:
	bool			m_Started;
public:
	TBaseBootableModule() : m_Started(false) {}
	virtual ~TBaseBootableModule() { HH_ONLY_IN_DEBUG(if (m_Started == true) HH_BREAKPOINT()); }

	virtual bool	Startup(const typename _BaseModule::Config &config) = 0;
	virtual void	Shutdown() = 0;
};

//----------------------------------------------------------------------------

#define	HH_MODULE_CLASS(__name)	C ## __name ## Internals

//----------------------------------------------------------------------------

#define	HH_MODULE_API_PARTIAL_DEFINE_EX(__name)	extern HH_MODULE_CLASS(__name)	GLUE(g_BootableModule_, __name)
#define	HH_MODULE_API_DEFINE_EX(__name, __base) \
												class	HH_MODULE_CLASS(__name) : public TBaseBootableModule<__base> \
												{ \
												public: \
													virtual bool	Startup(const __base::Config &config) override; \
													virtual void	Shutdown() override; \
												}; \
												HH_MODULE_API_PARTIAL_DEFINE_EX(__name)

#define	HH_MODULE_API_PARTIAL_DEFINE()			HH_MODULE_API_PARTIAL_DEFINE_EX(PV_MODULE_SYM)
#define	HH_MODULE_API_DEFINE(__base)			HH_MODULE_API_DEFINE_EX(PV_MODULE_SYM, __base)

//----------------------------------------------------------------------------

#define	HH_MODULE_API(__name)					PopcornFX::GLUE(g_BootableModule_, __name)
#define	HH_MODULE_API_DECLARE_EX(__name)		PopcornFX::HH_MODULE_CLASS(__name)	HH_MODULE_API(__name)
#define	HH_MODULE_API_DECLARE()					HH_MODULE_API_DECLARE_EX(PV_MODULE_SYM)

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HH_MODULE_API_H__
