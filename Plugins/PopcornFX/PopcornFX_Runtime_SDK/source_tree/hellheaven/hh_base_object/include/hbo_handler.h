#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2011/04/02 17:17 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_HANDLER_H__
#define __HBO_HANDLER_H__

#include "hh_base_object/include/hbo_object.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	HBO
{

	struct	/*HH_BASEOBJECT_EXPORT*/ SInheritanceDefinition	// shouldn't be constructed outside hh-kernel
	{
	public:
		CClassDefinition	*m_Definition;
		CHandler			*m_Handler;
		hh_ireg				m_RawObjectOffset;

		SInheritanceDefinition();
		SInheritanceDefinition(CClassDefinition *definition, CHandler *handler, void *rawBasePtr);

		bool	Valid() const
		{
			return m_Definition != null;
		}
	};

	//----------------------------------------------------------------------------
	//
	//	handlers
	//
	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT CHandler
	{
	private:
		CHandler		&operator = (const CHandler &other);	// non-copyable

		CBaseObject		*(*m_NewRawObject)();
		const char		*m_PublishedName;
		CString			m_PublishedNameCSTR;

		typedef	void	(*FnInheritanceNotifier)(void *arg, CHandler *childClass, bool addition);
		struct	SInheritanceCallback
		{
			FnInheritanceNotifier	m_Notify;
			void					*m_Arg;
			SInheritanceCallback(FnInheritanceNotifier notifier, void *arg) : m_Notify(notifier), m_Arg(arg) {}
		};
		TSemiDynamicArray<SInheritanceCallback, 2>	m_InheritanceCallbacks;

		void			_NotifyInheritanceModification(CHandler *h, bool addition) const;
		void			_NotifyFromAllInheritance(SInheritanceCallback &cb, bool addition) const;

	public:
		typedef TArray<CHandler*>					TChildClassList;
		typedef TArray<SInheritanceDefinition>		TBaseClassList;

		TBaseClassList								m_BaseClasses;
		TChildClassList								m_ChildClasses;
		bool										m_Birth;
		bool										m_Hidden;

		CHandler(const char *publishedName, CBaseObject *(*newRawObject)(), bool hidden);
		CHandler(const CHandler &other);
		~CHandler();

		TMemoryView<CFieldDefinition * const>		Fields() const;

		const char			*PublishedName() const { return m_PublishedName; }
		const CString		&PublishedNameCSTR() const { return m_PublishedNameCSTR; }

		bool				Hidden() const { return m_Hidden; }
		bool				AddBaseClass(const SInheritanceDefinition &inheritance);
		void				AddInheritedClass(CHandler *handler);
		void				RemoveInheritedClass(CHandler *handler);

		CGuid				FindInheritanceModificationCallback(FnInheritanceNotifier notifier, void *arg) const;
		void				AddInheritanceModificationCallback(FnInheritanceNotifier notifier, void *arg);
		void				RemoveInheritanceModificationCallback(FnInheritanceNotifier notifier, void *arg);

		void				_FinalizeHandler(const CBaseObject *basePtr);

		PBaseObject			NewObject(	CContext				*parentContext,
										CBaseObjectFile			*parentFile,
										const CString			&name = CString::EmptyString,
										CBaseObjectEditorInfos	*editorInfos = null,
										bool					autolink = true) const;

		PBaseObject			NewRawObject(CContext *parentContext) const;

		bool				IsBaseClassOf(const CBaseObject *object);
		bool				InheritsFrom(const CBaseObject *object);
	};

	//----------------------------------------------------------------------------

	extern HH_BASEOBJECT_EXPORT CHandler		*HandlerRegister(const CHandler &handler);
	extern HH_BASEOBJECT_EXPORT bool			HandlerUnregister(const CString &name);
	extern HH_BASEOBJECT_EXPORT CHandler		*LocateHandler(const char *handlerName);

	typedef TPair<CString, HBO::CHandler*>		CHandlerRef;
	extern HH_BASEOBJECT_EXPORT void			FillRegisteredHandlerList(TArray<CHandlerRef> &handlerList);

	//----------------------------------------------------------------------------
	//
	//	deprecation handlers
	//
	//----------------------------------------------------------------------------

	extern HH_BASEOBJECT_EXPORT bool			DeprecationHandlerRegister(const CString &originalName, const CString &remapName);
	extern HH_BASEOBJECT_EXPORT bool			DeprecationHandlerUnregister(const CString &name);

}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_HANDLER_H__
