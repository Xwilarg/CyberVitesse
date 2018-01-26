#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2005/05/12 03:23 by Camille Mirey
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_PLUGINS_H__
#define	__KR_PLUGINS_H__

#include "hh_toolkit/include/hh_toolkit_modules.h"
#include "hh_kernel/include/kr_string.h"
#include "hh_kernel/include/kr_containers.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

#if		defined(HH_WIN32)
#	define	DRVSTR_ARCH			"Win32"	// FIXME: X64?
#	define	DRVSTR_LIBEXT		".dll"
#elif	defined(HH_WINRT)
#	define	DRVSTR_ARCH			"WinRT"
#	define	DRVSTR_LIBEXT		".dll"
#elif	defined(HH_X360)
#	define	DRVSTR_ARCH			"X360"
#	define	DRVSTR_LIBEXT		".dll"
#elif		defined(HH_DURANGO)
#	define	DRVSTR_ARCH			"Durango"
#	define	DRVSTR_LIBEXT		".dll"
#elif	defined(HH_LINUX)
#	define	DRVSTR_ARCH			"Linux"
#	define	DRVSTR_LIBEXT		".so"
#elif	defined(HH_ANDROID)
#	define	DRVSTR_ARCH			"ANDROID"
#	define	DRVSTR_LIBEXT		".so"
#elif	defined(HH_MACOSX)
#	define	DRVSTR_ARCH			"MacOSX"
#	define	DRVSTR_LIBEXT		".so"
#elif	defined(HH_IOS)
#	define	DRVSTR_ARCH			"iOS"
#	define	DRVSTR_LIBEXT		".so"
#elif	defined(HH_PS3)
#	define	DRVSTR_ARCH			"PS3"
#	define	DRVSTR_LIBEXT		".prx"
#elif	defined(HH_ORBIS)
#	define	DRVSTR_ARCH			"ORBIS"
#	define	DRVSTR_LIBEXT		".prx"
#elif	defined(HH_PSP2)
#	define	DRVSTR_ARCH			"PSP2"
#	define	DRVSTR_LIBEXT		".suprx"
#elif	defined(HH_NX)
#	define	DRVSTR_ARCH			"NX"
#	define	DRVSTR_LIBEXT		".nro"
#else
#	error	unsupported platform
#endif

#if		defined(HH_DEBUG)
#	define	DRVSTR_CONFIG		"_Debug"
#elif	defined(HH_RETAIL)
#	define	DRVSTR_CONFIG		"_Retail"
#else
#	define	DRVSTR_CONFIG		"_Release"
#endif

#ifdef	HH_API_LIB
#	define	DRVSTR_LINK			"_Static"
#else
#	define	DRVSTR_LINK			""
#endif

//----------------------------------------------------------------------------

namespace	Plugins
{
	HH_KERNEL_EXPORT CString	BuildDirectoryPath(const CString &baseDriverDirectory);
}

//----------------------------------------------------------------------------

#define	HH_PLUGINS_BASE_SYM		hh_plugin

#ifdef	HH_API_LIB

// static lib plugin: only define a global-scope object
#define	HH_PLUGIN_DEFINE(__pluginClassName) \
	__pluginClassName	*g_PluginObject_ ## __pluginClassName = null; \
	PopcornFX::IPluginModule	*StartupPlugin_ ## __pluginClassName () \
	{ \
		HH_ASSERT(g_PluginObject_ ## __pluginClassName == null); \
		if (g_PluginObject_ ## __pluginClassName == null) \
		{ \
			g_PluginObject_ ## __pluginClassName = HH_NEW(__pluginClassName); \
		} \
		return g_PluginObject_ ## __pluginClassName; \
	} \
	PopcornFX::IPluginModule	*GetPlugin_ ## __pluginClassName () \
	{ \
		return g_PluginObject_ ## __pluginClassName; \
	} \
	void	ShutdownPlugin_ ## __pluginClassName () \
	{ \
		HH_ASSERT(g_PluginObject_ ## __pluginClassName != null); \
		HH_DELETE(g_PluginObject_ ## __pluginClassName); \
		g_PluginObject_ ## __pluginClassName = null; \
	}

#define	HH_PLUGIN_DECLARE(__pluginClassName) \
	PopcornFX::IPluginModule	*StartupPlugin_ ## __pluginClassName (); \
	PopcornFX::IPluginModule	*GetPlugin_ ## __pluginClassName (); \
	void						ShutdownPlugin_ ## __pluginClassName ()

#else

// dll-plugin: define an exported symbol that will retreive the IPluginModule
#define	HH_PLUGIN_DEFINE(__pluginClassName) \
	__pluginClassName	g_PluginObject_ ## __pluginClassName; \
	__HH_CDECL_BEGIN HH_API_EXPORT PopcornFX::IPluginModule	*HH_PLUGINS_BASE_SYM() \
	{ \
		return &g_PluginObject_ ## __pluginClassName; \
	} __HH_CDECL_END \
	HH_API_EXPORT PopcornFX::IPluginModule	*GetPlugin_ ## __pluginClassName () \
	{ \
		return &g_PluginObject_ ## __pluginClassName; \
	}

#define	HH_PLUGIN_DECLARE(__pluginClassName) \
	__HH_CDECL_BEGIN \
		HH_API_EXPORT PopcornFX::IPluginModule	*HH_PLUGINS_BASE_SYM(); \
	__HH_CDECL_END \
	HH_API_EXPORT PopcornFX::IPluginModule	*GetPlugin_ ## __pluginClassName ()

#endif

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT IPluginModule
{
public:
	enum	EModuleStatus
	{
		NotLoaded,
		Loaded,
		Error
	};

private:
	CString							m_HandlerName;
	const char						*m_StaticName;
	const char						*m_StaticDescription;
	hh_u32							m_VersionMajor;
	hh_u32							m_VersionMinor;

protected:
	TModule							m_Module;
	CString							m_Path;
	EModuleStatus					m_Status;

	virtual bool					_VirtualLoad() = 0;
	virtual void					_VirtualUnload() = 0;

	IPluginModule(const char *handlerName, const char *name, const char *description, hh_u32 vMaj, hh_u32 vMin);

public:
	virtual ~IPluginModule();

	EModuleStatus					Status() const { return m_Status; }
	EModuleStatus					Load();
	void							Unload();

	const char						*Name() const { return m_StaticName; }
	const char						*Description() const { return m_StaticDescription; }
	hh_u32							VersionMajor() const { return m_VersionMajor; }
	hh_u32							VersionMinor() const { return m_VersionMinor; }

	const CString					&HandlerName() const { return m_HandlerName; }

	void							SetupRuntime(const CString &path, TModule module);
	const CString					&Path() const { return m_Path; }
	TModule							Module() const { return m_Module; }

//#if	!defined(HH_COMPILER_SNC)
//	HH_NOINLINE
//#endif
//	bool				DebugCompiled()	// FIXME: messed-up by nature, totally useless, remove
//	{
//#ifdef HH_DEBUG
//		return true;
//#else
//		return false;
//#endif
//	}

	bool							operator == (const IPluginModule &other) const;
	bool							operator != (const IPluginModule &other) const { return !(*this == other); }
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CPluginHandler : public CRefCountedObject
{
private:
	CString							m_HandlerName;
protected:
	TArray<IPluginModule*>			m_PluginList;

	// these get called whenever a plugin is added or removed
	virtual void					_OnPluginAdded(IPluginModule *) {}		// when called, 'plugin' has just been added to 'm_PluginList', and can be found inside.
	virtual void					_OnPluginRemoved(IPluginModule *) {}	// when called, 'plugin' isn't inside 'm_PluginList' anymore

public:
	CPluginHandler(const CString &handlerName);
	virtual ~CPluginHandler();

	TArray<IPluginModule*>			&Plugins() { return m_PluginList; }
	bool							PluginRegister(IPluginModule *module);
	bool							PluginUnregister(IPluginModule *module);

	const CString					&HandlerName() const { return m_HandlerName; }

	bool	operator == (const CPluginHandler &other) const { return HandlerName() == other.HandlerName(); }
 	bool	operator != (const CPluginHandler &other) const { return !(*this == other); }
};
HH_DECLARE_REFPTRCLASS(PluginHandler);

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CPluginManager
{
private:
	static bool							_LinkHandler(IPluginModule *module);

public:
	static void							RegisterDirectory(const CString &dir, bool recursive, bool initModules = true);

	static bool							HandlerRegister(PPluginHandler handler);
	static bool							HandlerRelease(const CString &name);
	static bool							HandlerRelease(PPluginHandler handler);
	static CPluginHandler				*HandlerByName(const CString &name);
	static bool							HandlerModuleUnregister(IPluginModule *module);

	static bool							PluginRegister(const CString &path, bool initModules = true);
	static bool							PluginRegister(IPluginModule *plugin, bool initModules = true, const CString &path = CString::EmptyString, TModule module = TModule::Invalid);
	static bool							PluginRelease(const CString &name);
	static bool							PluginRelease(IPluginModule *plugin);

	static TMemoryView<PPluginHandler>	Handlers();
	static TMemoryView<IPluginModule*>	Plugins();
	static TMemoryView<IPluginModule*>	UnattachedPlugins();
};

//----------------------------------------------------------------------------
__PK_API_END

#endif /* __KR_PLUGINS_H__ */
