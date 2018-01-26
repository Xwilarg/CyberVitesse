#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/08/20 16:55 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_DETAILS_H__
#define __HBO_DETAILS_H__

#include "hh_base_object/include/hbo_object.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	HH_BASEOBJECT_EXPORT CBaseObjectStats
{
public:
	static hh_u32	m_HBO_RawCount;	// FIXME: TAtomic<> ?
	static hh_u32	m_HBO_Count;
	static hh_u32	m_HBOFiles_Count;
};

//----------------------------------------------------------------------------

namespace	HBO
{

	class	HH_BASEOBJECT_EXPORT CContextCallbacks
	{
	public:
		virtual ~CContextCallbacks() {}
		virtual void	OnObjectModified(const HBO::CContext *context, const char *oldEntry, const char *newEntry, CBaseObject *object, HBO::EContextAction action) {}	// a single object changed, might be called multiple times if only a handful of objects changed
		virtual void	OnFieldModified(CBaseObject *object, hh_u32 fieldId) {}
		virtual void	OnFileModified(const CBaseObjectFile *file) {}
	};

	//----------------------------------------------------------------------------

	namespace	Internal
	{
		void	HH_BASEOBJECT_EXPORT _GatherChilds_Impl(const CHandler *targetHandler, const PBaseObject &object, TArray<PBaseObject> &outList, bool followExternalLinks);
		void	HH_BASEOBJECT_EXPORT _GatherChilds_Impl(const CHandler *targetHandler, const PCBaseObject &object, TArray<PCBaseObject> &outList, bool followExternalLinks);

		template<typename _DstType, typename _SrcType>
		TArray<TRefPtr<_DstType> >	&covariant_array_cast(TArray<TRefPtr<_SrcType> > &srcArray)
		{
			HH_STATIC_ASSERT((TTypeAInheritsTypeB<_SrcType, _DstType>::True));	// requires "hh_template_metaprogramming.h"
			return *reinterpret_cast<TArray<TRefPtr<_DstType> >*>(&srcArray);	// horrific...
		}
		template<typename _DstType, typename _SrcType>
		const TArray<TRefPtr<_DstType> >	&covariant_array_cast(const TArray<TRefPtr<_SrcType> > &srcArray)
		{
			HH_STATIC_ASSERT((TTypeAInheritsTypeB<_SrcType, _DstType>::True));
			return *reinterpret_cast<const TArray<TRefPtr<_DstType> >*>(&srcArray);	// horrific...
		}
	}

	template<typename _Type>
	void	GatherChilds(const PBaseObject &object, TArray< TRefPtr<_Type> > &outList, bool followExternalLinks = true)
	{
		Internal::_GatherChilds_Impl(_Type::m_Handler, object, Internal::covariant_array_cast<CBaseObject>(outList), followExternalLinks);
	}

	template<typename _Type>
	void	GatherChilds(const PCBaseObject &object, TArray< TRefPtr<const _Type> > &outList, bool followExternalLinks = true)
	{
		Internal::_GatherChilds_Impl(_Type::m_Handler, object, Internal::covariant_array_cast<const CBaseObject>(outList), followExternalLinks);
	}

}

//----------------------------------------------------------------------------

namespace	HBO
{
	bool	IsLinkCharValid(char c);
	bool	IsNodeNameFirstCharValid(char c);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_DETAILS_H__
