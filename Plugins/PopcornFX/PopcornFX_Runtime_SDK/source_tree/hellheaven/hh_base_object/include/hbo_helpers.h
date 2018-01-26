#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/11/13 21:47 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_HELPERS_H__
#define __HBO_HELPERS_H__

#include "hh_base_object/include/hbo_object.h"
#include "hh_base_object/include/hbo_handler.h"
#include "hh_kernel/include/kr_delegates.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CDynamicMemoryStream;
class	CStreamReadOnly;

//----------------------------------------------------------------------------

namespace	HBO
{

	struct	SFieldDescriptor;

	//----------------------------------------------------------------------------

	enum	FieldCaracs
	{
		Caracs_None = 0,

		// Resrouces/Paths:
		Caracs_ResourceEffect,			// path to an effect
		Caracs_ResourceTexture,			// path to a texture : display image browser and preview
		Caracs_ResourceTextureAtlas,	// path to a texture atlas
		Caracs_ResourceFont,			// path to a font file
		Caracs_ResourceMesh,			// path to a mesh
		Caracs_ResourceAlembic,			// path to a alembic
		Caracs_ResourceVideo,			// path to a video
		Caracs_ResourceSound,			// path to an audio source
		Caracs_ResourceSimCache,		// path to a simulation cache
		Caracs_Path,					// path to a random file
		Caracs_ExternalLink,			// can reference an external HBO, in a different file

		// Others
		Caracs_Color,					// RGB or RGBA color
		Caracs_Hex,						// should be displayed as Hexadecimal
		Caracs_Slider,					// should be displayed as a slider
		Caracs_TextBuffer,				// not just a string, should be editable with a larger text editor (ex: script)
	};

	HH_FORCEINLINE bool		CaracsIsPath(FieldCaracs caracs)
	{
		return (caracs >= Caracs_ResourceEffect && caracs <= Caracs_ExternalLink);
	}

	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT CRegistrationListener
	{
	private:
		Threads::CCriticalSection	m_Lock;
	public:
		CHandler					*m_Handler;

		CRegistrationListener() : m_Handler(null) {}

		void						Add(CClassDefinition *cDef, CHandler *cHandler, void *rawBasePtr);

		static void					Begin(CHandler *handler);
		static void					End(CBaseObject *basePtr);
		static bool					RegistrationInProgress();
	};
	extern HH_BASEOBJECT_EXPORT CRegistrationListener		*_RegistrationListener;

	//----------------------------------------------------------------------------

	template<typename _TypeDst, typename _TypeSrc>
	struct	TValueStorageHelper
	{
		typedef	_TypeDst	Type;
		HH_FORCEINLINE static void	Copy(_TypeDst &dst, const _TypeSrc &src) { dst = src; }
	};
	template<typename _TypeDst>
	struct	TValueStorageHelper<_TypeDst, void>
	{
		typedef	_TypeDst	Type;
		HH_FORCEINLINE static void	Copy(_TypeDst &dst, const hh_u8 &src) {}
	};
	// don't specialize for <void, _TypeSrc> -> should fail build, can't copy a non-void type to a void type, type promotion should have taken place beforehand
	template<>
	struct	TValueStorageHelper<void, void>
	{
		typedef	hh_u8		Type;
		HH_FORCEINLINE static void	Copy(hh_u8 &dst, const hh_u8 &src) {}
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	struct	SFieldMinMax
	{
		typedef typename TContainerTraits::RemoveArray<_Type>::Type			MinMaxType;
		typedef typename TValueStorageHelper<MinMaxType, MinMaxType>::Type	MinMaxStorage;

		enum
		{
			Flags_HasMin = 0x1,
			Flags_HasMax = 0x2,
		};

		hh_u32				m_Flags;
		MinMaxStorage		m_MinMax[2];

		SFieldMinMax() : m_Flags(0) {}

		HH_FORCEINLINE bool					HasMin() const { return 0 != (m_Flags & Flags_HasMin); }
		HH_FORCEINLINE bool					HasMax() const { return 0 != (m_Flags & Flags_HasMax); }
		HH_FORCEINLINE const MinMaxStorage	&Min() const { return m_MinMax[0]; }
		HH_FORCEINLINE const MinMaxStorage	&Max() const { return m_MinMax[1]; }
	};

	//----------------------------------------------------------------------------

	bool	HH_KERNEL_EXPORT GTT_IsFieldNameValid(const char *name);

	template<typename _Type>
	class	CGTTContainer
	{
	public:
		CGTTContainer() {}
		~CGTTContainer() {}

		typedef TArray<TPair<const char *, _Type> >	ElementListType;

		ElementListType		m_Elements;

		CGuid		FetchIDFromName(const TMemoryView<const char> &name) const
		{
			for (hh_u32 i = 0; i < m_Elements.Count(); i++)
			{
				if (!strncmp(m_Elements[i].First(), name.Data(), name.Count()))
					return i;
			}
			return CGuid::INVALID;
		}

		CGuid		FetchID(const _Type &fromVal) const
		{
			for (hh_u32 i = 0; i < m_Elements.Count(); i++)
			{
				if (m_Elements[i].Second() == fromVal)
					return i;
			}
			return CGuid::INVALID;
		}
	};

	extern HH_BASEOBJECT_EXPORT CGTTContainer<bool>		_GTT_Bool;

	//----------------------------------------------------------------------------

	extern HH_BASEOBJECT_EXPORT const char			*_DefaultCategoryName;	// = "default";

	//----------------------------------------------------------------------------

	namespace	Properties
	{
	};

	template<typename _Type1, typename _Type2, bool _IsNumeric1 = TTypeTraits::IsNumeric<_Type1>::True, bool _IsNumeric2 =  TTypeTraits::IsNumeric<_Type2>::True>
	struct	TTypeMerger
	{
	};

	template<> struct									TTypeMerger<void, void, false, false>				{ typedef void		Type; };
//	template<typename _Type, bool _IsNumeric> struct	TTypeMerger<void, _Type, false, _IsNumeric>			{ typedef _Type		Type; };
//	template<typename _Type, bool _IsNumeric> struct	TTypeMerger<_Type, void, _IsNumeric, false>			{ typedef _Type		Type; };

	// required to be allowed to mix enums with ints:
	template<typename _Type1, typename _Type2> struct	TTypeMerger<_Type1, _Type2, true, false>			{ typedef _Type1	Type; };
	template<typename _Type1, typename _Type2> struct	TTypeMerger<_Type1, _Type2, false, true>			{ typedef _Type2	Type; };

	template<typename _Type, bool _IsNumeric> struct	TTypeMerger<_Type, _Type, _IsNumeric, _IsNumeric>	{ typedef _Type		Type; };
	template<typename _Type1, typename _Type2> struct	TTypeMerger<_Type1, _Type2, true, true>				{ typedef typename TNumericPromoter<_Type1, _Type2>::PromotedType	Type; }; // standard numeric promotion
	template<typename _Type> struct						TTypeMerger<_Type, _Type, true, true>				{ typedef _Type		Type; };

	//----------------------------------------------------------------------------

	struct	HH_BASEOBJECT_EXPORT SFieldUntypedCallbacks
	{
		FastDelegate<bool()>											m_Modified;
		FastDelegate<bool(const void *)>								m_ModifiedWithOldValue;
		FastDelegate<bool(const void *, bool)>							m_InRange;
		FastDelegate<hh_u32()>											m_DynamicTypeMask;

#if	(HH_BASE_OBJECT_STRIP_METADATA == 0)
		FastDelegate<CBaseObject::EFieldVisibility()>					m_Visibility;
#endif

		static void		Merge(SFieldUntypedCallbacks &a, const SFieldUntypedCallbacks &b);
	};

	//----------------------------------------------------------------------------

	template<typename _Type> class	TFieldAttributes;

	class	HH_BASEOBJECT_EXPORT CFieldAttributesBase
	{
	public:
		CFieldAttributesBase(void *abstractGTT = null);

		enum	EFlags
		{
			HasMin						= 0x1,
			HasMax						= 0x2,
			HasDefaultValue				= 0x4,
			HasCaracs					= 0x8,

			ForceSerializationAlways	= 0x10,
			ForceSerializationNever		= 0x20,
		};

		hh_u32							m_Flags;
		SFieldUntypedCallbacks			m_Callbacks;

		// WARNING: hack: the pointer will be == 0x1 if the link is to an unspecialized CBaseObject
		CHandler						**m_TargetLinkClassHandler;	// will only be != null if the field is a TClassLink<_Type>, and will point to _Type's class definition

#if	(HH_BASE_OBJECT_STRIP_METADATA == 0)
		HBO::FieldCaracs				m_Caracs;
		const char						*m_Description;
		const char						*m_Profile;
#endif
		void							*m_AbstractGTT;

		void							MergeUntypedFields(const CFieldAttributesBase &other);
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	class	TFieldAttributes : public CFieldAttributesBase
	{
	public:
		TFieldAttributes() : CFieldAttributesBase(&m_GTT) {}

		typedef	typename TValueStorageHelper<_Type, _Type>::Type	ValueType;
		typedef	typename TContainerTraits::RemoveArray<_Type>::Type	RawElementType;
		typedef	TValueStorageHelper<RawElementType, RawElementType>	ElementStorage;

		typename ElementStorage::Type								m_MinMax[2];	// we rely on 'm_MinMax' being at a fixed offset from the start of this object. all hell will break loose if it isn't.
		ValueType													m_DefaultValue;

		CGTTContainer<typename ElementStorage::Type>				m_GTT;
	};

	//----------------------------------------------------------------------------

	template<typename _Type, typename _Type2/*, bool _HasBothArrays = TTypeTraits::SizeOf<_Type>::Value != 0 && TTypeTraits::SizeOf<_Type2>::Value != 0*/>
	struct	TGTTCopier
	{
		static void	Copy(TFieldAttributes<_Type> &dst, const TFieldAttributes<_Type2> &src)
		{
			const typename CGTTContainer<typename TFieldAttributes<_Type2>::ElementStorage::Type>::ElementListType	&srcArray = src.m_GTT.m_Elements;
			hh_u32	addCount = srcArray.Count();
			if (addCount != 0)
			{
				typename CGTTContainer<typename TFieldAttributes<_Type>::ElementStorage::Type>::ElementListType	&dstArray = dst.m_GTT.m_Elements;
				dstArray.Reserve(dstArray.Count() + addCount);
				for (hh_u32 i = 0; i < addCount; i++)
				{
					const TPair<const char*, _Type2>	&entry = srcArray[i];
					dstArray.PushBack(TPair<const char*, _Type>(entry.First(), entry.Second()));
				}
			}
		}
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	struct	TGTTCopier<_Type, _Type/*, true*/>
	{
		static void	Copy(TFieldAttributes<_Type> &dst, const TFieldAttributes<_Type> &src)
		{
			typedef typename CGTTContainer<typename TFieldAttributes<_Type>::ElementStorage::Type>::ElementListType	ElementListType;

			const ElementListType	&srcArray = src.m_GTT.m_Elements;
				  ElementListType	&dstArray = dst.m_GTT.m_Elements;

			if (dstArray.Count() < srcArray.Count())
				dstArray.Swap(const_cast<TFieldAttributes<_Type>*>(&src)->m_GTT.m_Elements);	// const-perf-hack
			if (!srcArray.Empty())
				dstArray <<= srcArray;
		}
	};

	//----------------------------------------------------------------------------
	// TODO: make a MergeAndCastIFN that takes every possible simple attribute, and performs the appropriate merge task
	// instead of merging excessively large structures each time with a shitload of 'if's?

	template<typename _Type, typename _Type2>
	void						_MergeAndCastIFN(TFieldAttributes<_Type> &mergeInto, const TFieldAttributes<_Type2> &other)
	{
		TGTTCopier<_Type, _Type2>::Copy(mergeInto, other);

		if ((other.m_Flags & CFieldAttributesBase::HasDefaultValue) != 0)
			TValueStorageHelper<_Type, _Type2>::Copy(mergeInto.m_DefaultValue, other.m_DefaultValue);
		if ((other.m_Flags & CFieldAttributesBase::HasMin) != 0)
			TFieldAttributes<_Type>::ElementStorage::Copy(mergeInto.m_MinMax[0], other.m_MinMax[0]);
		if ((other.m_Flags & CFieldAttributesBase::HasMax) != 0)
			TFieldAttributes<_Type>::ElementStorage::Copy(mergeInto.m_MinMax[1], other.m_MinMax[1]);

		mergeInto.MergeUntypedFields(other);
	}

	//----------------------------------------------------------------------------
	// operators ',' that merge together the field attributes

#if 0

	HH_BASEOBJECT_EXPORT CFieldAttributesBase		operator , (CFieldAttributesBase attribsA, const CFieldAttributesBase &attribsB);

	template<typename _Type>
	TFieldAttributes<_Type>							operator , (TFieldAttributes<_Type> attribsA, const CFieldAttributesBase &attribsB)
	{
		attribsA.MergeUntypedFields(attribsB);
		return attribsA;
	}
	template<typename _Type2>
	TFieldAttributes<_Type2>						operator , (const CFieldAttributesBase &attribsA, TFieldAttributes<_Type2> attribsB)
	{
		attribsB.MergeUntypedFields(attribsA);
		return attribsB;
	}
	template<typename _Type>
	TFieldAttributes<_Type>							operator , (TFieldAttributes<_Type> attribsA, const TFieldAttributes<_Type> &attribsB)	// optimization when merging with the same type
	{
		_MergeAndCastIFN(attribsA, attribsB);
		return attribsA;
	}

	template<typename _Type1, typename _Type2>
	TFieldAttributes<typename TTypeMerger<_Type1, _Type2>::Type>	operator , (const TFieldAttributes<_Type1> &attribsA, const TFieldAttributes<_Type2> &attribsB)	// concatenate
	{
		TFieldAttributes<typename TTypeMerger<_Type1, _Type2>::Type>	newAttribs;
		_MergeAndCastIFN(newAttribs, attribsA);
		_MergeAndCastIFN(newAttribs, attribsB);
		return newAttribs;
	}

#else

	HH_BASEOBJECT_EXPORT CFieldAttributesBase		operator , (const CFieldAttributesBase &attribsA, const CFieldAttributesBase &attribsB);

	template<typename _Type>
	TFieldAttributes<_Type>							operator , (const TFieldAttributes<_Type> &attribsA, const CFieldAttributesBase &attribsB)
	{
		TFieldAttributes<_Type>	merged(attribsA);
		merged.MergeUntypedFields(attribsB);
		return merged;
	}
	template<typename _Type2>
	TFieldAttributes<_Type2>						operator , (const CFieldAttributesBase &attribsA, const TFieldAttributes<_Type2> &attribsB)
	{
		TFieldAttributes<_Type2>	merged(attribsB);
		merged.MergeUntypedFields(attribsA);
		return merged;
	}
	template<typename _Type>
	TFieldAttributes<_Type>							operator , (const TFieldAttributes<_Type> &attribsA, const TFieldAttributes<_Type> &attribsB)	// optimization when merging with the same type
	{
		TFieldAttributes<_Type>	merged(attribsA);
		_MergeAndCastIFN(merged, attribsB);
		return merged;
	}

	template<typename _Type1, typename _Type2>
	TFieldAttributes<typename TTypeMerger<_Type1, _Type2>::Type>	operator , (const TFieldAttributes<_Type1> &attribsA, const TFieldAttributes<_Type2> &attribsB)	// concatenate
	{
		TFieldAttributes<typename TTypeMerger<_Type1, _Type2>::Type>	newAttribs;
		_MergeAndCastIFN(newAttribs, attribsA);
		_MergeAndCastIFN(newAttribs, attribsB);
		return newAttribs;
	}

#endif

	//----------------------------------------------------------------------------
	// - read
	// - write
	// - needs writing
	// -

	class	HH_BASEOBJECT_EXPORT CFieldDefinitionArray
	{
	private:
		friend class			CFieldDefinition;
		const CFieldDefinition	*m_Base;

		CFieldDefinitionArray(const CFieldDefinition *base);

		CGuid					UnsafeInsert(CBaseObject *basePtr, const void *value, CGuid index);
		CGuid					UnsafeIndexOf(CBaseObject *basePtr, const void *value);
		bool					UnsafeSet(CBaseObject *basePtr, const void *value, CGuid index);
		bool					UnsafeOverwrite(CBaseObject *basePtr, const void *valueList);

	public:
		bool					Valid() const;

		const CFieldDefinition	*BaseDefinition();
		const SGenericType		&Type() const;

		template<typename _Type>
		CGuid					Insert(CBaseObject *basePtr, const _Type &v, CGuid index = CGuid::INVALID)	// by default, with index=CGuid::INVALID, inserts at the end, same as PushBack()
		{
			HH_ASSERT(SGenericType::FromType<_Type>() == Type());
			return UnsafeInsert(basePtr, &v, index);
		}

		template<typename _Type>
		CGuid					IndexOf(const CBaseObject *basePtr, const _Type &v)
		{
			HH_ASSERT(SGenericType::FromType<_Type>() == Type());
			return UnsafeIndexOf(basePtr, &v);
		}

		template<typename _Type>
		bool					Set(CBaseObject *basePtr, const _Type &v, CGuid index)
		{
			HH_ASSERT(SGenericType::FromType<_Type>() == Type());
			return UnsafeSet(basePtr, &v, index);
		}

		bool					Remove(CBaseObject *basePtr, CGuid index);

		template<typename _Type>
		bool					Overwrite(CBaseObject *basePtr, const TMemoryView<const _Type> &newValues)
		{
			HH_ASSERT(SGenericType::FromType<_Type>() == Type());
			return UnsafeOverwrite(basePtr, &newValues);
		}
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	class	TFieldDefinition_Impl;

//#if	!defined(HH_WINDOWS)
#define	HBO_STORE_EXPLICIT_BASE_ATTRIBUTES_PTR
//#endif

	class	HH_BASEOBJECT_EXPORT CFieldDefinition
	{
	private:
		SGenericType					m_Type;

		bool							_RawFieldCopy(void *dst, const void *src) const;

	protected:
		void							*m_FieldOffset;
#ifdef	HBO_STORE_EXPLICIT_BASE_ATTRIBUTES_PTR
		CFieldAttributesBase			*m_BaseAttributes;
#endif
	public:
		CFieldDefinition(CClassDefinition *owner, const char *name, CFieldAttributesBase *baseAttributes, SGenericType type, void *fieldPtr);
		virtual ~CFieldDefinition() {}

		CClassDefinition				*m_Owner;
		const char						*m_Name;
#if	(HH_BASE_OBJECT_STRIP_METADATA == 0)
		const char						*m_Category;
#endif

#ifdef	HBO_STORE_EXPLICIT_BASE_ATTRIBUTES_PTR
		const CFieldAttributesBase		&GetBaseAttributes() const { return *m_BaseAttributes; }
#else
		const CFieldAttributesBase		&GetBaseAttributes() const { return *reinterpret_cast<const CFieldAttributesBase*>(this + 1); }
#endif

		// FIXME: shouldn't we have the description, category, and profile, in here? rather than within the templated field attributes ?
		const SGenericType				&Type() const { return m_Type; }

		bool							IsDefaultValue(CBaseObject *basePtr) const;
		bool							Equals(const void *a, const void *b) const;
		void							SetToDefault(CBaseObject *basePtr, bool skipModificationCallbacks) const;
		bool							GetMin(void *outputRawUninitializedData) const;
		bool							GetMax(void *outputRawUninitializedData) const;
		void							GetMinMax(void *outputRawUninitializedData) const;
		bool							InRange(const CBaseObject *basePtr, const void *src, bool srcIsArray) const;
		bool							Link(CBaseObject *basePtr) const;
		bool							Unlink(CBaseObject *basePtr) const;
		bool							MayReceive(CBaseObject *basePtr, CBaseObject *object) const;
		bool							Invalidate(CBaseObject *basePtr) const;
		bool							ResolveDynamicInputsAndOutputs(CBaseObject *basePtr) const;
		SGenericType					GetDynamicType(const CBaseObject *basePtr) const;
		bool							ActivateOutputField(CBaseObject *basePtr, CBaseObject *requester, CLink *activator) const;
		bool							TriggerModification(CBaseObject *basePtr, const void *oldValue = null) const;
		CBaseObject::EFieldVisibility	VisibleInEditor(const CBaseObject *basePtr) const;

		const void						*UnsafeGet(const CBaseObject *basePtr) const;
		bool							UnsafeSet(CBaseObject *basePtr, const void *src) const;

		CFieldDefinitionArray			GetArrayInterfaceIFP() const;

		CFieldDefinition				&Category(const char *name = _DefaultCategoryName);
		template<typename _Type>
		TFieldDefinition_Impl<_Type>	&Field(const char *name, _Type *fieldPtr);
		template<typename _TypeClass, typename _Type>
		TFieldDefinition_Impl<_Type>	&Field(const char *name, _Type (_TypeClass::*cbGet)());
		template<typename _TypeClass, typename _Type>
		TFieldDefinition_Impl<_Type>	&Field(const char *name, _Type (_TypeClass::*cbGet)(), bool (_TypeClass::*cbSet)(const _Type &));

		// should only be called internally
		void							_Finalize(CBaseObject *basePtr);
		void							_Relocate(const CBaseObject *basePtr);	// only called during the _Finalize() stage
		hh_u32							_FieldOffset() const { return checked_numcast<hh_u32>(hh_ureg(m_FieldOffset)); }
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	class	TFieldDefinition_Impl : public CFieldDefinition
	{
	public:
		TFieldDefinition_Impl(CClassDefinition *owner, const char *name, _Type *fieldPtr);
		virtual ~TFieldDefinition_Impl();

		TFieldAttributes<_Type>			m_Attributes;

		template<typename _Type2>
		TFieldDefinition_Impl<_Type>	&operator [] (const TFieldAttributes<_Type2> &attributes);
		TFieldDefinition_Impl<_Type>	&operator [] (const CFieldAttributesBase &attributes);
	};

#if 0
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<CString>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<bool>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<hh_i32>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<hh_u32>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<float>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<CFloat2>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<CFloat3>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<CFloat4>;
/*	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<bool>;
	template class	HH_BASEOBJECT_EXPORT TFieldDefinition_Impl<bool>;*/
#endif

	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT CClassDefinition
	{
	private:
	public:
		const char						*m_Name;
#if	(HH_BASE_OBJECT_STRIP_METADATA == 0)
		const char						*m_CurrentCategory;	// hm.. becomes unused after construction
#endif
		hh_u32							m_Sizeof;
		hh_u32							m_FirstLocalFieldInFlattenedList;
//		CClassAttributes				*m_ClassAttributes;
		TArray<CFieldDefinition*>		m_Fields;
		TArray<CFieldDefinition*>		m_FlattenedFields;

		CClassDefinition();
		~CClassDefinition();

//		CClassDefinition				&operator [] (const CAttributes &attributes) { m_ClassAttributes = CAttributes::Merge(m_ClassAttributes, &attributes); }
		CClassDefinition				&Category(const char *name = _DefaultCategoryName);

		void							_Finalize(CBaseObject *basePtr);
		void							_AddField(CFieldDefinition *field);

		template<typename _Type>
		TFieldDefinition_Impl<_Type>	&Field(const char *name, _Type *fieldPtr);
		template<typename _TypeClass, typename _Type>
		TFieldDefinition_Impl<_Type>	&Field(const char *name, _Type (_TypeClass::*cbGet)());
		template<typename _TypeClass, typename _Type>
		TFieldDefinition_Impl<_Type>	&Field(const char *name, _Type (_TypeClass::*cbGet)(), bool (_TypeClass::*cbSet)(const _Type &));

/*		TFieldDefinition_Impl<bool>		&Field(const char *name, bool *fieldPtr);
		TFieldDefinition_Impl<hh_i32>	&Field(const char *name, hh_i32 *fieldPtr);
		TFieldDefinition_Impl<hh_u32>	&Field(const char *name, hh_u32 *fieldPtr);
		TFieldDefinition_Impl<float>	&Field(const char *name, float *fieldPtr);
		TFieldDefinition_Impl<CFloat2>	&Field(const char *name, CFloat2 *fieldPtr);
		TFieldDefinition_Impl<CFloat3>	&Field(const char *name, CFloat3 *fieldPtr);
		TFieldDefinition_Impl<CFloat4>	&Field(const char *name, CFloat4 *fieldPtr);*/
	};

	//----------------------------------------------------------------------------

	namespace	Properties
	{
		// - description			<>
		// - profile				<>
		// - caracs					<>
		// - default value			<T>
		// - min					<T>
		// - max					<T>
		// - minmax					<T>
		// - GTT					<T>
		// - cb visible in editor	<C>
		// - cb constraint on set	<C, T>
		// - cb on modified			<C, T>
		// - cb on modified w/ old	<C, T>

		//----------------------------------------------------------------------------
		// implementations

		HH_BASEOBJECT_EXPORT CFieldAttributesBase		Description(const char *description);
		HH_BASEOBJECT_EXPORT CFieldAttributesBase		Profile(const char *profile);
		HH_BASEOBJECT_EXPORT CFieldAttributesBase		Caracs(HBO::FieldCaracs caracs);
		HH_BASEOBJECT_EXPORT CFieldAttributesBase		AlwaysSerialize();
		HH_BASEOBJECT_EXPORT CFieldAttributesBase		NeverSerialize();

		template<typename _Type>
		TFieldAttributes<_Type>		DefaultValue(const _Type &value)
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_DefaultValue = value;
			attribs.m_Flags = CFieldAttributesBase::HasDefaultValue;
			return attribs;
		}
		HH_BASEOBJECT_EXPORT TFieldAttributes<CString>	DefaultValue(const char *value);

		template<typename _Type>
		TFieldAttributes<_Type>		Min(const _Type &min)
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_MinMax[0] = min;
			attribs.m_Flags = CFieldAttributesBase::HasMin;
			return attribs;
		}
		HH_BASEOBJECT_EXPORT TFieldAttributes<CString>	Min(const char *min);

		template<typename _Type>
		TFieldAttributes<_Type>		Max(const _Type &max)	// FIXME: use some numeric promotion for these?
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_MinMax[1] = max;
			attribs.m_Flags = CFieldAttributesBase::HasMax;
			return attribs;
		}
		HH_BASEOBJECT_EXPORT TFieldAttributes<CString>	Max(const char *max);

		template<typename _Type>
		TFieldAttributes<_Type>	MinMax(const _Type &min, const _Type &max)	// FIXME: use some numeric promotion for these?
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_MinMax[0] = min;
			attribs.m_MinMax[1] = max;
			attribs.m_Flags = CFieldAttributesBase::HasMin | CFieldAttributesBase::HasMax;
			return attribs;
		}
		// if we want to use MinMax("asd", CString(something)), we'll need to call explicitely MinMax(CString("asd"), CString(something)), but we don't really care.
		HH_BASEOBJECT_EXPORT TFieldAttributes<CString>	MinMax(const char *min, const char *max);

		/*
		template<typename _TypeClass, typename _Type>
		TFieldAttributes<_Type>	ValueConstraint(bool (_TypeClass::*callback)(_Type &))
		{
			TFieldAttributes<_Type>	attribs;
			TFastDelegate<bool(_Type&)>	delegate =
			attribs.m_ValueConstraint = HH_NEW(TValueConstraint_Impl_MinMax<_Type>(min, max));
			return attribs;
		}
		*/

#define	HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(__type) \
		HH_BASEOBJECT_EXPORT TFieldAttributes<__type>	DefaultValue(const __type &value); \
		HH_BASEOBJECT_EXPORT TFieldAttributes<__type>	Min(const __type &min); \
		HH_BASEOBJECT_EXPORT TFieldAttributes<__type>	Max(const __type &max); \
		HH_BASEOBJECT_EXPORT TFieldAttributes<__type>	MinMax(const __type &min, const __type &max)

		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(CString);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(bool);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(hh_i32);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(hh_u32);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(float);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(CFloat2);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(CFloat3);
		HBO_DECLARE_EXPLICIT_PROPERTIES_SPECIALIZATION(CFloat4);

		template<typename _Type>
		TFieldAttributes<_Type>	GTT(const char *name, const _Type &value)
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_GTT.m_Elements.PushBack(TPair<const char*, _Type>(name, value));
			return attribs;
		}
		HH_BASEOBJECT_EXPORT TFieldAttributes<CString>	GTT(const char *name, const char *value);

		template<typename _TypeClass>
		CFieldAttributesBase	Modified(bool (_TypeClass::*cbModified)())
		{
			CFieldAttributesBase	attribs;
			attribs.m_Callbacks.m_Modified = FastDelegate<bool()>((_TypeClass*)null, cbModified);
			return attribs;
		}
		template<typename _TypeClass>
		CFieldAttributesBase	Modified(bool (_TypeClass::*cbModified)() const)
		{
			CFieldAttributesBase	attribs;
			attribs.m_Callbacks.m_Modified = FastDelegate<bool()>((_TypeClass*)null, cbModified);
			return attribs;
		}

		template<typename _TypeClass, typename _Type>
		TFieldAttributes<_Type>	Modified(bool (_TypeClass::*cbModified)(const _Type &oldValue))
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_Callbacks.m_ModifiedWithOldValue = FastDelegate<bool(const void *)>((_TypeClass*)null, *reinterpret_cast<bool (_TypeClass::**)(const void *)>(&cbModified));	// horrible cast
			return attribs;
		}
		template<typename _TypeClass, typename _Type>
		TFieldAttributes<_Type>	Modified(bool (_TypeClass::*cbModified)(const _Type &oldValue) const)
		{
			TFieldAttributes<_Type>	attribs;
			attribs.m_Callbacks.m_ModifiedWithOldValue = FastDelegate<bool(const void *)>((_TypeClass*)null, *reinterpret_cast<bool (_TypeClass::**)(const void *)>(&cbModified));	// horrible cast
			return attribs;
		}

		template<typename _TypeClass, typename _Type>
		TFieldAttributes<_Type>	InRange(bool (_TypeClass::*cbInRange)(const _Type &value, bool valueIsArray))
		{
			HH_STATIC_ASSERT(TContainerTraits::IsArray<_Type>::False);
			TFieldAttributes<_Type>	attribs;
			attribs.m_Callbacks.m_InRange = FastDelegate<bool(const void *, bool)>((_TypeClass*)null, *reinterpret_cast<bool (_TypeClass::**)(const void *, bool)>(&cbInRange));	// horrible cast
			return attribs;
		}
		template<typename _TypeClass, typename _Type>
		TFieldAttributes<_Type>	InRange(bool (_TypeClass::*cbInRange)(const _Type &value, bool valueIsArray) const)
		{
			HH_STATIC_ASSERT(TContainerTraits::IsArray<_Type>::False);
			TFieldAttributes<_Type>	attribs;
			attribs.m_Callbacks.m_InRange = FastDelegate<bool(const void *, bool)>((_TypeClass*)null, *reinterpret_cast<bool (_TypeClass::**)(const void *, bool)>(&cbInRange));	// horrible cast
			return attribs;
		}

		template<typename _TypeClass>
		CFieldAttributesBase	DynamicTypeMask(hh_u32 (_TypeClass::*cbDynamicTypeMask)())
		{
			CFieldAttributesBase	attribs;
			attribs.m_Callbacks.m_DynamicTypeMask = FastDelegate<hh_u32()>((_TypeClass*)null, cbDynamicTypeMask);
			return attribs;
		}
		template<typename _TypeClass>
		CFieldAttributesBase	DynamicTypeMask(hh_u32 (_TypeClass::*cbDynamicTypeMask)() const)
		{
			CFieldAttributesBase	attribs;
			attribs.m_Callbacks.m_DynamicTypeMask = FastDelegate<hh_u32()>((_TypeClass*)null, cbDynamicTypeMask);
			return attribs;
		}

		template<typename _TypeClass>
		CFieldAttributesBase	VisibleInEditor(CBaseObject::EFieldVisibility (_TypeClass::*cbVisibility)())
		{
#if	(HH_BASE_OBJECT_STRIP_METADATA == 0)
			CFieldAttributesBase	attribs;
			attribs.m_Callbacks.m_Visibility = FastDelegate<CBaseObject::EFieldVisibility()>((_TypeClass*)null, cbVisibility);
			return attribs;
#else
			return CFieldAttributesBase();
#endif
		}
		template<typename _TypeClass>
		CFieldAttributesBase	VisibleInEditor(CBaseObject::EFieldVisibility (_TypeClass::*cbVisibility)() const)
		{
#if	(HH_BASE_OBJECT_STRIP_METADATA == 0)
			CFieldAttributesBase	attribs;
			attribs.m_Callbacks.m_Visibility = FastDelegate<CBaseObject::EFieldVisibility()>((_TypeClass*)null, cbVisibility);
			return attribs;
#else
			return CFieldAttributesBase();
#endif
		}

	}

	//----------------------------------------------------------------------------
	// TFieldAttributes

	//----------------------------------------------------------------------------
	// TFieldDefinition_Impl

	template<typename _Type>
	TFieldDefinition_Impl<_Type>	&CFieldDefinition::Field(const char *name, _Type *fieldPtr)
	{
		TFieldDefinition_Impl<_Type>	*def = HH_NEW(TFieldDefinition_Impl<_Type>(m_Owner, name, fieldPtr));
		m_Owner->_AddField(def);
		return *def;
	}

	template<typename _TypeClass, typename _Type>
	TFieldDefinition_Impl<_Type>	&CFieldDefinition::Field(const char *name, _Type (_TypeClass::*cbGet)())
	{
		TFieldDefinition_Impl<_Type>	*def = HH_NEW(TFieldDefinition_Impl<_Type>(m_Owner, name, null));
		m_Owner->_AddField(def);
		return *def;
	}

	template<typename _TypeClass, typename _Type>
	TFieldDefinition_Impl<_Type>	&CFieldDefinition::Field(const char *name, _Type (_TypeClass::*cbGet)(), bool (_TypeClass::*cbSet)(const _Type &))
	{
		TFieldDefinition_Impl<_Type>	*def = HH_NEW(TFieldDefinition_Impl<_Type>(m_Owner, name, null));
		m_Owner->_AddField(def);
		return *def;
	}

	template<typename _Type>
	template<typename _Type2>
	TFieldDefinition_Impl<_Type>	&TFieldDefinition_Impl<_Type>::operator [] (const TFieldAttributes<_Type2> &attributes)
	{
		_MergeAndCastIFN(m_Attributes, attributes);
		return *this;
	}
	template<typename _Type>
	TFieldDefinition_Impl<_Type>	&TFieldDefinition_Impl<_Type>::operator [] (const CFieldAttributesBase &attributes)
	{
		m_Attributes.MergeUntypedFields(attributes);
		return *this;
	}

#ifndef	MOVE_TFIELDDEFINITION_TO_CPP
	template<typename _Type>
	struct	TTargetLinkClassHandler
	{
		static CHandler	**Get() { return null; }
	};
	template<typename _Type>
	struct	TTargetLinkClassHandler<TClassLink<_Type> >
	{
		static CHandler	**Get() { /*HH_ASSERT(_Type::m_Handler != null);*/ return &_Type::m_Handler; }	// we should be able to get away with the pointer itself, as we should execute this after the target class registration.
	};
	template<>
	struct	TTargetLinkClassHandler<TClassLink<CBaseObject> >
	{
		static CHandler	**Get() { return (CHandler**)1; }
	};
	template<typename _Type, typename _Controller>
	struct	TTargetLinkClassHandler<TArray<_Type, _Controller> > : public TTargetLinkClassHandler<_Type>
	{
	};

	template<typename _Type>
	TFieldDefinition_Impl<_Type>::TFieldDefinition_Impl(CClassDefinition *owner, const char *name, _Type *fieldPtr)
	:	CFieldDefinition(owner, name, &m_Attributes, SGenericType::FromType<_Type>(), fieldPtr)
	{
		m_Attributes.m_TargetLinkClassHandler = TTargetLinkClassHandler<_Type>::Get();
	}

	template<typename _Type>
	TFieldDefinition_Impl<_Type>::~TFieldDefinition_Impl()
	{
	}

#endif

	//----------------------------------------------------------------------------
	// CClassDefinition

	template<typename _Type>
	TFieldDefinition_Impl<_Type>	&CClassDefinition::Field(const char *name, _Type *fieldPtr)
	{
		TFieldDefinition_Impl<_Type>	*def = HH_NEW(TFieldDefinition_Impl<_Type>(this, name, fieldPtr));
		_AddField(def);
		return *def;
	}

	template<typename _TypeClass, typename _Type>
	TFieldDefinition_Impl<_Type>	&CClassDefinition::Field(const char *name, _Type (_TypeClass::*cbGet)())
	{
		TFieldDefinition_Impl<_Type>	*def = HH_NEW(TFieldDefinition_Impl<_Type>(this, name, null));
		_AddField(def);
		return *def;
	}

	template<typename _TypeClass, typename _Type>
	TFieldDefinition_Impl<_Type>	&CClassDefinition::Field(const char *name, _Type (_TypeClass::*cbGet)(), bool (_TypeClass::*cbSet)(const _Type &))
	{
		TFieldDefinition_Impl<_Type>	*def = HH_NEW(TFieldDefinition_Impl<_Type>(this, name, null));
		_AddField(def);
		return *def;
	}

	//----------------------------------------------------------------------------
	// Helpers, to avoid excessive code duplication

	namespace	Internal
	{

		HH_BASEOBJECT_EXPORT CClassDefinition	*RegisterHandler();
		HH_BASEOBJECT_EXPORT void				UnregisterHandler(CClassDefinition *&cDef, CHandler *&handler);
		HH_BASEOBJECT_EXPORT CGuid				GetFieldLeafID(const char *name, const TMemoryView<CFieldDefinition * const> &fList);

		HH_BASEOBJECT_EXPORT void				_RawFieldSwap(const SGenericType &type, void *a, void *b);
		HH_BASEOBJECT_EXPORT void				_RawFieldConstruct(const SGenericType &type, void *storage);
		HH_BASEOBJECT_EXPORT void				_RawFieldDestruct(const SGenericType &type, void *storage);

	}
}

//----------------------------------------------------------------------------
// .cpp

#define	HBO_BUILD_REGISTRATION_ERROR_STRING_AND_ARGS(__classname)	"HBO handler for class '" STRINGIFY(__classname) "' is not registered!"

//----------------------------------------------------------------------------

#define	HBO_FIELD_DEFINITION(__name)	Field(#__name, &m_ ## __name)

//----------------------------------------------------------------------------
// class definition start tag

#define	HBO_CLASS_DEFINITION_BEGIN(__classname) \
void	__classname::_ConstructHierarchy(void *virtualThis) \
{ \
	if (PopcornFX::HBO::CRegistrationListener::RegistrationInProgress()) \
	{ \
		PopcornFX::HBO::CClassDefinition	*cDef = __classname::_LocalDefinition; \
		if (cDef == null) \
			cDef = static_cast<__classname*>(virtualThis)->_SetupClassDefinition(); \
		PopcornFX::HBO::_RegistrationListener->Add(cDef, m_Handler, virtualThis); \
	} \
} \
bool	__classname::RegisterHandler(bool visible /* = true*/) \
{ \
	HH_ASSERT(_LocalDefinition == null); \
	m_Handler = PopcornFX::HBO::HandlerRegister(PopcornFX::HBO::CHandler(STRINGIFY(__classname), &__classname::_NewRawObject, !visible)); \
	HH_ASSERT(m_Handler != null); \
	if (m_Handler != null) \
	{ \
		PopcornFX::HBO::CRegistrationListener::Begin(m_Handler); \
		__classname	builderInstance; \
		_LocalDefinition = PopcornFX::HBO::Internal::RegisterHandler(); \
		PopcornFX::HBO::CRegistrationListener::End(&builderInstance); \
	} \
	return _LocalDefinition != null; \
} \
void	__classname::UnregisterHandler() \
{ \
	PopcornFX::HBO::Internal::UnregisterHandler(_LocalDefinition, __classname::m_Handler); \
} \
PopcornFX::HBO::CHandler	*__classname::Handler() const \
{ \
	HH_ASSERT_MESSAGE(m_Handler != null, HBO_BUILD_REGISTRATION_ERROR_STRING_AND_ARGS(__classname)); \
	return m_Handler; \
} \
PopcornFX::HBO::CHandler				*__classname::m_Handler = null; \
PopcornFX::HBO::CClassDefinition		*__classname::_LocalDefinition = null; \
const PopcornFX::HBO::CClassDefinition	*__classname::GetClassDefinition() const \
{ \
	HH_ONLY_IN_DEBUG(HH_ASSERT(PopcornFX::HBO::TBaseObjectConstructor<__classname>::EveryBaseObjectMustInheritFrom_TBaseObjectConstructor() == 0xB000B1E5));/*HH_STATIC_ASSERT(TBaseObjectConstructor<__classname>::EveryBaseObjectMustInheritFrom_TBaseObjectConstructor == 0xB000B1E5);*/ \
	HH_ASSERT_MESSAGE(_LocalDefinition != null, HBO_BUILD_REGISTRATION_ERROR_STRING_AND_ARGS(__classname)); \
	return _LocalDefinition; \
} \
hh_u32	__classname::_GetFieldLeafID_FromLocalList(const char *name) \
{ \
	HH_ASSERT(__classname::_LocalDefinition != null); \
	const CGuid	id = PopcornFX::HBO::Internal::GetFieldLeafID(name, __classname::_LocalDefinition->m_Fields); \
	HH_ASSERT(id.Valid()); \
	return id + _LocalDefinition->m_FirstLocalFieldInFlattenedList; \
} \
CBaseObject *__classname::_NewRawObject() \
{ \
	return HH_NEW(__classname); \
} \
PopcornFX::HBO::CClassDefinition	*__classname::_SetupClassDefinition() \
{ \
	PopcornFX::HBO::CClassDefinition	*localDefinition = HH_NEW(PopcornFX::HBO::CClassDefinition); \
	localDefinition->m_Sizeof = sizeof(__classname); \
	localDefinition->m_Name = STRINGIFY(__classname); \
	(void)(*localDefinition)

//----------------------------------------------------------------------------
// class definition end tag

#define	HBO_CLASS_DEFINITION_END \
	; \
	/* here, '_LocalDefinition' is complete, and we might want to digest it for runtime-efficiency */ \
	localDefinition->_Finalize(this); \
	return localDefinition; \
} \

//----------------------------------------------------------------------------

namespace	HBO
{
	// merde, ca en fait c'est utilise dans CMGameDatabase ?
	hh_u32	ScanFloat3(const char *expr, hh_u32 exprLen, hh_u32 expr_start_id, CFloat3 &output);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __HBO_HELPERS_H__
