#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2007/11/13 15:46 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __HBO_OBJECT_H__
#define __HBO_OBJECT_H__

#include "hh_kernel/include/kr_file.h"
#include "hh_kernel/include/kr_containers_array_slot.h"
#include "hh_kernel/include/kr_checksum.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
// forward declares:

HH_FORWARD_DECLARE(BaseObjectFile);
HH_FORWARD_DECLARE(BaseObject);

//----------------------------------------------------------------------------

namespace	HBO
{

	class	CContext;			// defined in hbo_context.h
	class	CHandler;			// defined in hbo_handler.h
	class	CClassDefinition;	// defined in hbo_helpers.h
	class	CFieldDefinition;	// defined in hbo_helpers.h

#define	HBO_CLASS(__className)		__className : public ::PopcornFX::HBO::TBaseObjectConstructor<__className>
#define	HBO_CONSTRUCT(__className)	::PopcornFX::HBO::TBaseObjectConstructor<__className>(this)

	//----------------------------------------------------------------------------

	template<class _Type>
	class	TBaseObjectConstructor
	{
	private:
		TBaseObjectConstructor() {}
	protected:
	public:
		HH_FORCEINLINE TBaseObjectConstructor(_Type *virtualThis)
		{
			_Type::_ConstructHierarchy(virtualThis);
		}

		HH_ONLY_IN_DEBUG(
			hh_u32	EveryBaseObjectMustInheritFrom_TBaseObjectConstructor() const
			{
				return 0xB000B1E5;
			}
		)
	};

	//----------------------------------------------------------------------------

	HH_BASEOBJECT_EXPORT CBaseObject		*RawCast(CBaseObject *hbo, const char *targetCastClassName);
	HH_BASEOBJECT_EXPORT const CBaseObject	*RawCast(const CBaseObject *hbo, const char *targetCastClassName);
	HH_BASEOBJECT_EXPORT CBaseObject		*RawCast(CBaseObject *hbo, const CHandler *targetHandler);
	HH_BASEOBJECT_EXPORT const CBaseObject	*RawCast(const CBaseObject *hbo, const CHandler *targetHandler);

	template<typename _Type, typename _TypePtr>
	_Type	*Cast(_TypePtr *ptr)
	{
		return static_cast<_Type*>(RawCast(ptr, _Type::m_Handler));
	}
	template<typename _Type, typename _TypePtr>
	_Type	*Cast(const TRefPtr<_TypePtr> &ptr)
	{
		return static_cast<_Type*>(RawCast(ptr.Get(), _Type::m_Handler));
	}

	//----------------------------------------------------------------------------

	// Converts _Type* to TClassLink<_Type> even through arrays
	template<typename _Type>
	struct	TMakeLinkIFN;

	template<typename _Type>
	struct	TRemoveLinkIFN;

	//----------------------------------------------------------------------------
	// .h

#define	HBO_CLASS_DECLARATION() \
	private: \
		static CBaseObject								*_NewRawObject(); \
	public: \
		static PopcornFX::HBO::CHandler					*m_Handler; \
		static bool										RegisterHandler(bool visible = true); \
		static void										UnregisterHandler(); \
		virtual PopcornFX::HBO::CHandler				*Handler() const override; \
		virtual const PopcornFX::HBO::CClassDefinition	*GetClassDefinition() const override; \
		static PopcornFX::HBO::CClassDefinition			*_LocalDefinition; \
		PopcornFX::HBO::CClassDefinition				*_SetupClassDefinition(); \
		static void										_ConstructHierarchy(void *virtualThis); \
	private: \
		static hh_u32									_GetFieldLeafID_FromLocalList(const char *name)

#define _HBO_FIELD_BASE(__name)		\
	public:	\
		static hh_u32		_GetFieldID_ ## __name ()	\
		{	\
			static const hh_u32	kFieldID = _GetFieldLeafID_FromLocalList(#__name);	\
			return kFieldID;	\
		}	\
		hh_u32		GetFieldID_ ## __name() const { return _GetFieldID_ ## __name ();}

#define	HBO_FIELD(__type, __name) \
		_HBO_FIELD_BASE(__name)	\
	public: \
		typedef PopcornFX::HBO::TMakeLinkIFN<__type>::Type	_TypeOf ## __name; \
		const _TypeOf ## __name		&__name () const { return m_ ## __name; } \
		bool						Set ## __name (const _TypeOf ## __name &value) { return SetField(_GetFieldID_ ## __name (), value); } \
	private: \
		_TypeOf ## __name			m_ ## __name

#define	HBO_FIELD_ABSTRACT(__type, __name) \
		_HBO_FIELD_BASE(__name)	\
	public: \
		typedef PopcornFX::HBO::TMakeLinkIFN<__type>::Type	_TypeOf ## __name; \
		const _TypeOf ## __name		&__name () const { return GetField<_TypeOf ## __name>(_GetFieldID_ ## __name ()); }	\
		bool						Set ## __name (const _TypeOf ## __name &value) { return SetField(_GetFieldID_ ## __name (), value); }

	//----------------------------------------------------------------------------

	extern HH_BASEOBJECT_EXPORT CContext		*g_Context;

	//----------------------------------------------------------------------------
	//
	//	types
	//
	//----------------------------------------------------------------------------

	enum	ETypeName
	{
		GenericTypeFirstValidFieldType = 0,

		GenericType_String = GenericTypeFirstValidFieldType,

		GenericType_Bool,
		GenericType_Int,
		GenericType_Int2,
		GenericType_Int3,
		GenericType_Int4,
		GenericType_Uint,
		GenericType_Uint2,
		GenericType_Uint3,
		GenericType_Uint4,

		GenericType_Float,
		GenericType_Float2,
		GenericType_Float3,
		GenericType_Float4,

		GenericType_Link,
		GenericType_OutLink,
		GenericType_Custom,
		GenericType_ClassRef,

		__MaxGenericTypeFieldTypes
	};

	//----------------------------------------------------------------------------

	class							CLink;
	class							COutLink;
	template<class _Type> class		TClassLink;
	template<typename _Type> class	TTypedLink;
	template<typename _Type> class	TTypedOutLink;

	//----------------------------------------------------------------------------

	template<ETypeName _TypeId>	struct	TTypeSwitcher {};
	template<>	struct	TTypeSwitcher<GenericType_String>	{ typedef CString		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Bool>		{ typedef bool			Type; };
	template<>	struct	TTypeSwitcher<GenericType_Int>		{ typedef hh_i32		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Int2>		{ typedef CInt2			Type; };
	template<>	struct	TTypeSwitcher<GenericType_Int3>		{ typedef CInt3			Type; };
	template<>	struct	TTypeSwitcher<GenericType_Int4>		{ typedef CInt4			Type; };
	template<>	struct	TTypeSwitcher<GenericType_Uint>		{ typedef hh_u32		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Uint2>	{ typedef CUint2		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Uint3>	{ typedef CUint3		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Uint4>	{ typedef CUint4		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Float>	{ typedef float			Type; };
	template<>	struct	TTypeSwitcher<GenericType_Float2>	{ typedef CFloat2		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Float3>	{ typedef CFloat3		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Float4>	{ typedef CFloat4		Type; };
	template<>	struct	TTypeSwitcher<GenericType_Link>		{ typedef CLink			Type; };
	template<>	struct	TTypeSwitcher<GenericType_OutLink>	{ typedef COutLink		Type; };

	//----------------------------------------------------------------------------
	// NOTE: GCC doesn't see const int types as compile-time constants the same way as enums, and tries to find an external symbol associated to them.
	// it should be 'static const ETypeName Id = ...' in the templates below, and not 'enum { Id = ...'
	// because of this, we're forced to typecast each goddamn time we want to use these values...

	template<typename _Type>	struct	TTypeId										{ enum { Id = GenericType_Custom }; };	// FIXME: ?
	template<>					struct	TTypeId<CString>							{ enum { Id = GenericType_String }; };
	template<>					struct	TTypeId<bool>								{ enum { Id = GenericType_Bool }; };
	template<>					struct	TTypeId<hh_i32>								{ enum { Id = GenericType_Int }; };
	template<>					struct	TTypeId<CInt2>								{ enum { Id = GenericType_Int2 }; };
	template<>					struct	TTypeId<CInt3>								{ enum { Id = GenericType_Int3 }; };
	template<>					struct	TTypeId<CInt4>								{ enum { Id = GenericType_Int4 }; };
	template<>					struct	TTypeId<hh_u32>								{ enum { Id = GenericType_Uint }; };
	template<>					struct	TTypeId<CUint2>								{ enum { Id = GenericType_Uint2 }; };
	template<>					struct	TTypeId<CUint3>								{ enum { Id = GenericType_Uint3 }; };
	template<>					struct	TTypeId<CUint4>								{ enum { Id = GenericType_Uint4 }; };
	template<>					struct	TTypeId<float>								{ enum { Id = GenericType_Float }; };
	template<>					struct	TTypeId<CFloat2>							{ enum { Id = GenericType_Float2 }; };
	template<>					struct	TTypeId<CFloat3>							{ enum { Id = GenericType_Float3 }; };
	template<>					struct	TTypeId<CFloat4>							{ enum { Id = GenericType_Float4 }; };
	template<>					struct	TTypeId<CLink>								{ enum { Id = GenericType_Link }; };
	template<typename _Type>	struct	TTypeId<TTypedLink<_Type> >					{ enum { Id = GenericType_Link }; };
	template<typename _Type>	struct	TTypeId<TClassLink<_Type> >					{ enum { Id = GenericType_Link }; };
	template<>					struct	TTypeId<COutLink>							{ enum { Id = GenericType_OutLink }; };
	template<typename _Type>	struct	TTypeId<TTypedOutLink<_Type> >				{ enum { Id = GenericType_OutLink }; };

	template<>					struct	TTypeId<CInt1>								{ enum { Id = GenericType_Int }; };
	template<>					struct	TTypeId<CUint1>								{ enum { Id = GenericType_Uint }; };
	template<>					struct	TTypeId<CFloat1>							{ enum { Id = GenericType_Float }; };

	template<typename _Type>	struct	TTypeId<_Type const> : TTypeId<_Type>		{};


	// to be used in __PK_API_BEGIN scope
#define	_PV_HH_DEFINE_ENUM_TRAITS(__enum)	namespace HBO { template<> struct TTypeId<__enum> { static const int Id = GenericType_Int; }; }

	//----------------------------------------------------------------------------
	// typed wrappers.. thank-you GCC

	template<typename _Type>
	HH_FORCEINLINE ETypeName	TypeId()
	{
		return ETypeName(TTypeId<_Type>::Id);
	}

	template<typename _Type>
	HH_FORCEINLINE ETypeName	TypeId(const _Type &value)
	{
		return ETypeName(TTypeId<_Type>::Id);
	}

#define	HBO_STATIC_TYPEID(__type)	PopcornFX::HBO::ETypeName(TTypeId<__type>::Id)

	//----------------------------------------------------------------------------

	struct	HH_BASEOBJECT_EXPORT SGenericType
	{
		enum
		{
			TypeMask_IsArray = 0x80000000,
			TypeMask_Link_Input = (1 << GenericType_Link),
			TypeMask_Link_Output = (1 << GenericType_OutLink),
			TypeMask_Link = TypeMask_Link_Input | TypeMask_Link_Output,
		};
		HH_STATIC_ASSERT((1UL << __MaxGenericTypeFieldTypes) < hh_u32(TypeMask_IsArray));

		ETypeName			m_Type;
		hh_u32				m_TypeMask;

		SGenericType(ETypeName type, bool isArray);

		template<typename _Type>
		static SGenericType	FromType()
		{
			return SGenericType(TypeId<typename TContainerTraits::RemoveArray<_Type>::PureType>(), TContainerTraits::IsArray<_Type>::True);
		}

		hh_u32				TypeCount() const;
		ETypeName			ExtractSubType(hh_u32 id) const;

		bool	operator == (const SGenericType &other) const;

		bool	IsArray() const { return 0 != (m_TypeMask & TypeMask_IsArray); }
		bool	IsLink() const { return 0 != (m_TypeMask & TypeMask_Link); }
		bool	IsInputLink() const { HH_ASSERT((m_TypeMask & TypeMask_Link) != TypeMask_Link); return 0 != (m_TypeMask & TypeMask_Link_Input); }	// assert: both in and out flags should never be set at the same time.
		bool	IsOutputLink() const { HH_ASSERT((m_TypeMask & TypeMask_Link) != TypeMask_Link); return 0 != (m_TypeMask & TypeMask_Link_Output); }
	};

	//----------------------------------------------------------------------------
	//
	//	links
	//
	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT CLink
	{
	private:
		friend class	::PopcornFX::CBaseObject;
		CString			m_Path;	// TODO: if possible, remove this ! we're only using it at load time, and we're paying the memory footprint (and resulting cache misses) of a CString for EACH freakin' serialized pointer to an HBO class
	public:
		PBaseObject		DataFeed;
		CGuid			FieldID;
		ETypeName		DynamicType;	// we should remove this

		CLink() : m_Path(null), DataFeed(null), DynamicType(GenericType_Link) {}
		CLink(const CLink &copyFrom) : m_Path(copyFrom.m_Path), DataFeed(copyFrom.DataFeed), FieldID(copyFrom.FieldID), DynamicType(GenericType_Link) {}

		bool			Empty() const { return DataFeed == null && m_Path.Empty(); }

		CString			BuildRealPath() const;
		CString			BuildPathForHBO_Text(CBaseObjectFile *targetFile) const;
		bool			BuildPathForHBO_Binary(CBaseObjectFile *targetFile, CString &outPath) const;	// returns true if local
		const CString	&Path() const { return m_Path; }
		void			SetPath(const CString &path);

		bool			DynamicTypeResolved() const { return DynamicType != GenericType_Link; }

		bool	operator == (const CLink &compare) const;
		bool	operator != (const CLink &compare) const { return !(*this == compare); }
	};

	//----------------------------------------------------------------------------

#if 1
	template<typename _Type>
	class	TTypedLink : public CLink
	{
	public:
		TTypedLink() { DynamicType = ETypeName(TTypeId<_Type>::Id); }
	};

#else
	template<typename _Type0, typename _Type1 = _Type0, typename _Type2 = _Type1, typename _Type3 = _Type2>
	class	TTypedLink : public CLink
	{
	public:
		TTypedLink() { DynamicType = GenericType_Link; }
	};

	template<typename _Type0, typename _Type1, typename _Type2>
	class	TTypedLink<_Type0, _Type1, _Type2, _Type2> : public CLink
	{
	public:
		TTypedLink() { DynamicType = GenericType_Link; }
	};

	template<typename _Type0, typename _Type1>
	class	TTypedLink<_Type0, _Type1, _Type1, _Type1> : public CLink
	{
	public:
		TTypedLink() { DynamicType = GenericType_Link; }
	};

	template<typename _Type0>
	class	TTypedLink<_Type0, _Type0, _Type0, _Type0> : public CLink
	{
	public:
		TTypedLink() { DynamicType = ETypeName(TTypeId<_Type0>::Id); }
	};

	typedef TTypedLink<float, CFloat2, CFloat3, CFloat4>	TTypedLinkF1F2F3F4;
#endif

	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT COutLink
	{
	public:
		TArray<TPair<CBaseObject*, CLink*> >	LinkedLinks;
		ETypeName								DynamicType;

		COutLink() : DynamicType(GenericType_Link) {}
//		~COutLink();

		bool	DynamicTypeResolved() const { return DynamicType != GenericType_Link; }

		void	InvalidateAllLinkedBlocks() const;

		bool	Referenced() const { return !LinkedLinks.Empty(); }
		bool	AddReference(CBaseObject *object, CLink *which);
		void	RemoveReference(CBaseObject *object, CLink *which);
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	class	TTypedOutLink : public COutLink
	{
	public:
		TTypedOutLink() { DynamicType = ETypeName(TTypeId<_Type>::Id); }
	};

	//----------------------------------------------------------------------------

	template<class _Type>
	class	TClassLink : public CLink
	{
	private:
		typedef	TClassLink<_Type>	SelfType;
	public:
		TClassLink(_Type *linkedTo = null)
		{
			if (linkedTo != null)
			{
				DataFeed = const_cast<typename TType_RemoveConstness<_Type>::Type*>(linkedTo);	// FIXME: specialization that inherits from some CLinkConst ? or don't care?
			}
		}
		~TClassLink() {}

		HH_FORCEINLINE _Type		*Get() const;
		HH_FORCEINLINE				operator _Type * () const;
		HH_FORCEINLINE				operator TRefPtr<_Type> () const;
		HH_FORCEINLINE _Type		&operator * () const;
		HH_FORCEINLINE _Type*		operator -> () const;
	};

	//----------------------------------------------------------------------------

	template<typename _Type>
	struct	_TMakeLinkIFN
	{
		typedef	_Type	Type;
	};

	template<typename _Type>
	struct	_TMakeLinkIFN<_Type*>
	{
		typedef	TClassLink<_Type>	Type;
		//HH_STATIC_ASSERT(TTypesAreRelated<_Type COMMA CBaseObject>::True);
	};

	template<typename _Type>
	struct	TMakeLinkIFN
	{
		typedef typename TContainerTraits::MakeArrayIfArray<typename _TMakeLinkIFN<typename TContainerTraits::RemoveArray<_Type>::Type>::Type, _Type>::Type	Type;
	};

	template<typename _Type>
	struct	_TRemoveLinkIFN
	{
		typedef	_Type	Type;
	};

	template<typename _Type>
	struct	_TRemoveLinkIFN< TClassLink<_Type> >
	{
		typedef	_Type	Type;
		//HH_STATIC_ASSERT(TTypesAreRelated<_Type COMMA CBaseObject>::True);
	};

	template<typename _Type>
	struct	TRemoveLinkIFN
	{
		typedef typename TContainerTraits::MakeArrayIfArray<typename _TRemoveLinkIFN<typename TContainerTraits::RemoveArray<_Type>::Type>::Type, _Type>::Type	Type;
	};

	//----------------------------------------------------------------------------

	extern HH_BASEOBJECT_EXPORT CString	ConvertObjectToHumanReadableTemplate(const TArray<const CBaseObject*> &objects, bool keepLocalLinks = false, bool keepExternalLinks = false);
	extern HH_BASEOBJECT_EXPORT void	ConvertHumanReadableTemplateToObject(const CString &string, TArray<PBaseObject> &objects, CBaseObjectFile *whereToCreateObjects);

	//----------------------------------------------------------------------------

	class	HH_BASEOBJECT_EXPORT CField
	{
	public:
		PBaseObject		m_DataFeed;
		CGuid			m_FieldID;

		CField() {}
		CField(const PBaseObject &dataFeed, hh_u32 fieldID) : m_DataFeed(dataFeed), m_FieldID(fieldID) {}

		static bool	CanBePluggedInto(const CField &output, const CField &input);
	};

	//----------------------------------------------------------------------------
	// may hold multiple fields

	class	CAbstractFields
	{
	private:
		friend class	::PopcornFX::CBaseObject;
		void			*m_RawData;
		hh_u32			m_DataSizeInBytes;
	public:
		CAbstractFields() : m_RawData(null), m_DataSizeInBytes(0) {}
		~CAbstractFields() { if (m_RawData != null) HH_FREE(m_RawData); }
	};

	//----------------------------------------------------------------------------

	enum
	{
		Flag_Modified						= 0x1,
		Flag_Invalidated					= 0x4,
		Flag_Linked							= 0x8,
		Flag_NameAutoBuilt					= 0x10,
		Flag_BaseVirtualLink_NotCalled		= 0x20,
		Flag_BaseVirtualUnlink_NotCalled	= 0x40,
		Flag_Linking						= 0x80,
		Flag_Suicidal						= 0x100,
		Flag_Locked							= 0x400,
		Flag_SaveAllFields					= 0x800,
		Flag_MergeMask						= 0x1000,
	};

}

//----------------------------------------------------------------------------
//
//	Editor Infos
//
//----------------------------------------------------------------------------

class	HH_BASEOBJECT_EXPORT CBaseObjectEditorInfos
{
public:
	enum	EMinificationMode
	{
		Minimized = 0,
		Normal,
		Maximized
	};

private:
	EMinificationMode	m_MinificationMode;
	CInt2				m_Position;
public:
	CBaseObjectEditorInfos();

	void				MoveTo(const CInt2 &position) { m_Position = position; }
	const CInt2			&Position() const { return m_Position; }

	EMinificationMode	MinificationMode() const { return m_MinificationMode; }
	void				SetMinificationMode(EMinificationMode mode) { m_MinificationMode = mode; }
};

HH_STATIC_ASSERT(sizeof(CBaseObjectEditorInfos) == 12);

//----------------------------------------------------------------------------
//
//	base class
//
//----------------------------------------------------------------------------

#if 0
namespace	HBO
{
	// TODO ? replace CBaseObject by HBO::CBaseClass ?
	class	HH_BASEOBJECT_EXPORT CBaseClass;
}
#endif

class	HH_BASEOBJECT_EXPORT CBaseObject : public CRefCountedObject
{
private:
	friend class						HBO::CContext;
	friend class						HBO::CHandler;
	friend class						CBaseObjectFile;
	friend class						HBO::CFieldDefinition;

public:	// these are public for the serializers :/ FIXME: find a cleaner way.
	CString								_m_HBO_Name;
	CString								_m_HBO_Tags;
	hh_u16								_m_HBO_Flags;
	typedef TArray<TPair<PBaseObject, hh_u32> >	ReferrerList;

private:
	ReferrerList						_m_HBO_BlocksReferencingUs;

	friend HH_BASEOBJECT_EXPORT CString	HBO::ConvertObjectToHumanReadableTemplate(const TArray<const CBaseObject*> &objects, bool keepLocalLinks /*= false*/, bool keepExternalLinks /*= false*/);

	static bool							IsTagStringValid(const CString &rawTags);
public:
	bool								ScanNameForAutoBuiltPattern(const char *name) const;
	void								SetAutobuiltStatus(bool status)
	{
		if (status)
			_m_HBO_Flags |= HBO::Flag_NameAutoBuilt;
		else
			_m_HBO_Flags &= ~HBO::Flag_NameAutoBuilt;
	}

private:
	bool								StrippedToLocalName(const CString &name HH_ONLY_IF_ASSERTS(COMMA bool skipNameValidityChecking = false));
	bool								InternalRename(const CString &newName, bool skipNameValidityChecking = false);

	bool								_InternalCheckFieldType(hh_u32 field, HBO::ETypeName typeId, bool isArray) const;

protected:
	hh_u16								&BaseFlags() { return _m_HBO_Flags; }
	const hh_u16						&BaseFlags() const { return _m_HBO_Flags; }

	HBO::CContext						*m_Context;
	CBaseObjectFile						*m_Container;
private:
	mutable void						*m_InternalUserData;

protected:
	bool								SetRawName(const CString &name);
	void								_InitialFileAttach(CBaseObjectFile *file);

	public: void						Unload();
	void								LinkRemoveReference(HBO::CLink &link);

public:
	bool								Linked() const { return (_m_HBO_Flags & HBO::Flag_Linked) != 0; }
protected:
	bool								Linking() const { return ((_m_HBO_Flags & (HBO::Flag_Linking)) != 0); }
	bool								Suiciding() const { return ((_m_HBO_Flags & (HBO::Flag_Suicidal)) != 0); }
	bool								NeedsRelink() const { return !Linked() || Invalidated(); }

	virtual bool						VirtualLink();
	virtual void						VirtualUnlink();

public:
	CBaseObjectEditorInfos				m_EditorInfos;

	enum	ENameAutobuildMode
	{
		NameAutobuildScanExistingObjects = 0,
		NameAutobuildRandomRejection,
	};

	enum	EFieldsSaveMode
	{
		FieldsSaveNonDefault = 0,
		FieldsSaveAll,
	};

	enum	EFieldVisibility
	{
		FieldVisible = 0,
		FieldFrozen,
		FieldInvisible,
	};

	enum	EExportMode
	{
		NotExported = 0,
		Exported,
	};

	enum	ELockMode
	{
		Unlocked = 0,
		Locked,
	};

	CBaseObject();
	virtual ~CBaseObject();

	virtual	HBO::CHandler				*Handler() const = 0;
	const CString						&HandlerName() const;

	HBO::CContext						*Context() const { return m_Context; }

	const char							*Name() const { return StrippedNamePointer(_m_HBO_Name.Data()); } // don't use a CString
	const CString						&RawName() const { return _m_HBO_Name; }
	static const char					*StrippedNamePointer(const char *name);	// WARNING: use with caution
	bool								Rename(const CString &newName);
	bool								AutoBuildName(ENameAutobuildMode autobuildMode = NameAutobuildRandomRejection);
	bool								WasNameAutoBuilt() const { return (_m_HBO_Flags & HBO::Flag_NameAutoBuilt) != 0; }

	CString								RawTags() const { return _m_HBO_Tags; }
	bool								SetRawTags(const CString &tags);	// sets the modification flag
	void								ExtractTags(TArray<CString> &tagList);
	void								SetTags(const TArray<CString> &tagList);	// sets the modification flag

	EFieldsSaveMode						FieldsSaveMode() const { return (_m_HBO_Flags & HBO::Flag_SaveAllFields) ? FieldsSaveAll : FieldsSaveNonDefault; }
	void								SetFieldsSaveMode(EFieldsSaveMode exported);
	ELockMode							LockMode() const { return (_m_HBO_Flags & HBO::Flag_Locked) ? Locked : Unlocked; }
	void								SetLockMode(ELockMode locked);

	CBaseObjectFile						*File() const { return m_Container; }
	void								MoveToFile(CBaseObjectFile *targetFile);
// 	void								RemoveFromCurrentFile();
	void								RefreshContainerDependentData();

	HH_FORCEINLINE bool					Modified() const { return (_m_HBO_Flags & HBO::Flag_Modified) != 0; }

	HH_FORCEINLINE bool					Invalidated() const { return (_m_HBO_Flags & HBO::Flag_Invalidated) != 0; }
	virtual void						Invalidate(/*hh_u32 what*/);
	HH_FORCEINLINE void					Validate() { _m_HBO_Flags &= ~HBO::Flag_Invalidated; }

	void								SetModificationStatus(bool modified);

	bool								_Link();
	bool								_PostLink();

	void								Unlink();
	void								Suicide();

	// all these are fast compared to the other field-accessing methods:

	hh_u32								FieldCount() const;
	bool								CopyFieldFrom(hh_u32 targetField, const CBaseObject *originalObject, hh_u32 sourceField);

	bool								ActivateOutputField(CBaseObject *requester, /*hh_u32 requesterFieldID, */HBO::CLink *activator);

	HBO::SGenericType					GetFieldDynamicType(hh_u32 field) const;
	const HBO::SGenericType				&GetFieldStaticType(hh_u32 field) const;

	// no need for arrays of output links... hum...
	void								TriggerFieldModification(hh_u32 field);
	void								TriggerModificationOfFieldsPluggedToObject(const CBaseObject *object);

	// call this to notify all the blocks we're plugged into that we changed:
	void								TriggerModificationOfLinkedFieldsInReferencingObjects() const;

	// field export/import:
	bool								ExportField(hh_u32 field, HBO::CAbstractFields &output) const;
	bool								ExportAllFields(HBO::CAbstractFields &output) const;

	// will set multiple fields if 'field' contains multiple fields. (ie: if it was filled through a call to GetAllFields())
	bool								ImportFields(const HBO::CAbstractFields &field);

	static bool							NameValid(const char *name);
	static bool							ValidateName(char *name);	// returns true if the name had to be modified
	CString								BuildPathToField(CGuid fieldId, bool realPath) const;
	bool								LinkBlock(HBO::CLink *link);
	void								RemoveAllLinksReferencingBlock(CBaseObject *reference);


	// list of blocks referencing us through the global object link
	const ReferrerList					&ReferencingBlocks() const { return _m_HBO_BlocksReferencingUs; }
	void								GetConnectedObjects(TArray<PBaseObject> &arrObjects, bool followExternalObjects = false) const;

	static void							GetCommonFields(const TArray<PBaseObject> &objects, TArray<TPair<CString, TArray<HBO::CField> > > &outputFieldList);
// 	static void							CloneObjects(const TArray<const CBaseObject*> &objects, TArray<CBaseObject*> &cloned);

	void								SetAllFieldsToDefaults();

	virtual const HBO::CClassDefinition	*GetClassDefinition() const { HH_ASSERT_NOT_REACHED(); return null; }
	CGuid								GetFieldIndex(const char *fieldName) const;
	const HBO::CFieldDefinition			&GetFieldDefinition(CGuid fieldId) const;

	template <typename _Type>
	bool								_CheckFieldType(hh_u32 field) const { return _InternalCheckFieldType(field, HBO::ETypeName(HBO::TTypeId<typename TContainerTraits::RemoveArray<_Type>::Type>::Id), TContainerTraits::IsArray<_Type>::True); }

	template<typename _Type>
	const _Type							&GetField(hh_u32 field) const
	{
		HH_ASSERT(_CheckFieldType<_Type>(field));
		return *static_cast<const _Type*>(_UnsafeGetField(field));
	}
	template<typename _Type>
	void								GetField(hh_u32 field, _Type &valueOut) const
	{
		HH_ASSERT(_CheckFieldType<_Type>(field));
		valueOut = *static_cast<const _Type *>(_UnsafeGetField(field));
	}

	template<typename _Type>
	bool								SetField(hh_u32 field, const _Type &valueIn)
	{
		HH_ASSERT(_CheckFieldType<_Type>(field));
		return _UnsafeSetField(field, &valueIn);
	}
	bool								SetField(hh_u32 field, const char *valueIn);
	bool								SetField(hh_u32 field, const CString &valueIn);

	// keep these publicly accessible for the moment, we might need them in the editor for manual access without polluting code too much.
	// ideally, should be private
	const void							*_UnsafeGetField(CGuid fieldId) const;
	bool								_UnsafeSetField(CGuid fieldId, const void *valueIn);

	//static HBO::CHandler				*m_Handler;

	// null initialized user data
	void								SetInternalUserData(void *userData) const { m_InternalUserData = userData; }
	void								*InternalUserData() const { return m_InternalUserData; }

};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

namespace	HBO
{

	template<class _Type>
	HH_FORCEINLINE _Type	*TClassLink<_Type>::Get() const { _Type *ptr = checked_cast<_Type*>(DataFeed.Get()); HH_ASSERT((ptr == null && DataFeed == null) || (ptr != null && DataFeed != null)); return ptr; }
	template<class _Type>
	HH_FORCEINLINE TClassLink<_Type>::operator	_Type* () const { _Type *ptr = checked_cast<_Type*>(DataFeed.Get()); HH_ASSERT((ptr == null && DataFeed == null) || (ptr != null && DataFeed != null)); return ptr; }
	template<class _Type>
	HH_FORCEINLINE TClassLink<_Type>::operator	TRefPtr<_Type> () const { _Type *ptr = checked_cast<_Type*>(DataFeed.Get()); HH_ASSERT((ptr == null && DataFeed == null) || (ptr != null && DataFeed != null)); return ptr; }
	template<class _Type>
	HH_FORCEINLINE _Type	&TClassLink<_Type>::operator * () const { _Type *ptr = checked_cast<_Type*>(DataFeed.Get()); HH_ASSERT(ptr != null); return *ptr; }
	template<class _Type>
	HH_FORCEINLINE _Type	*TClassLink<_Type>::operator -> () const { _Type *ptr = checked_cast<_Type*>(DataFeed.Get()); HH_ASSERT(ptr != null); return ptr; }

}

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_base_object/include/hbo_context.h"
#include "hh_base_object/include/hbo_file.h"

#endif // __HBO_OBJECT_H__
