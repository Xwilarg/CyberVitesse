#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2012/04/25 16:35 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ANY_INL__
#define __KR_CONTAINERS_ANY_INL__

#ifndef	__KR_CONTAINERS_ARRAY_H__
#	error	kr_containers_any.inl must not be included manually, include kr_containers_any.h instead
#endif

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
//	CAny
//
//----------------------------------------------------------------------------

namespace	Containers
{
	template<typename _Type>	struct	IsCAnyBaseType : TTypeBoolConstant<IsBaseType<_Type>::True && CBaseType<_Type>::Size <= kCAny_MaxSizeBeforeAutoAlloc> {};
	template<typename _Type>	struct	IsCAnyBaseType<_Type const> : IsCAnyBaseType<_Type> {};	// needed if we want const pointers to work

	//----------------------------------------------------------------------------

	struct	HH_KERNEL_EXPORT SAnyTypeTraits
	{
		bool					Constructed;
		EBaseTypeID				BaseTypeID;
		RTTI::TypeGUID			(*TypeInfo)();
		void					(*Destruct)(void *object);
		void					(*CopyConstruct)(void *from, void *to);

		HH_FORCEINLINE bool		Inert() const { return BaseTypeID >= 0 && CBaseTypeTraits::Traits(BaseTypeID).Size <= kCAny_MaxSizeBeforeAutoAlloc; }
		SAnyTypeTraits(RTTI::TypeGUID (*typeInfo)(), void (*destruct)(void *object), void (*copyConstruct)(void *from, void *to), bool constructed, EBaseTypeID baseTypeID)
		:	Constructed(constructed)
		,	BaseTypeID(baseTypeID)
		,	TypeInfo(typeInfo)
		,	Destruct(destruct)
		,	CopyConstruct(copyConstruct)
		{
		}
	};

	//----------------------------------------------------------------------------

	template<typename _Type, bool _IsInert>
	struct	SpecializedTypeTraits : SAnyTypeTraits
	{
	private:
	public:
		SpecializedTypeTraits() : SAnyTypeTraits(&SpecializedTypeInfo,
												 &SpecializedDestruct,
												 &SpecializedCopyConstruct,
												 TTypeTraits::SizeOf<_Type>::Value > kCAny_MaxSizeBeforeAutoAlloc,
												 CBaseType<_Type>::TypeID) { HH_ASSERT(!Inert()); }
		static RTTI::TypeGUID		SpecializedTypeInfo() { return RTTI::Type<_Type>::ID(); }
		static void					SpecializedDestruct(void *object)
		{
			if (TTypeTraits::SizeOf<_Type>::Value > kCAny_MaxSizeBeforeAutoAlloc)
				HH_DELETE(*static_cast<_Type**>(object));
			else
			{
				Mem::Destruct(*reinterpret_cast<_Type*>(object));
			}
		}
		static void					SpecializedCopyConstruct(void *from, void *to)
		{
			if (TTypeTraits::SizeOf<_Type>::Value > kCAny_MaxSizeBeforeAutoAlloc)
				*(_Type**)(to) = HH_NEW(_Type(**(_Type**)(from)));
			else
			{
				Mem::Construct(*reinterpret_cast<_Type*>(to), *reinterpret_cast<const _Type*>(from));
			}
		}
	};

	//----------------------------------------------------------------------------

	template<typename _Type2>
	struct	SpecializedTypeTraits<_Type2, true> : SAnyTypeTraits
	{
	private:
	public:
		SpecializedTypeTraits() : SAnyTypeTraits(&SpecializedTypeInfo,
												 &SpecializedDestruct<_Type2>,
												 &SpecializedCopyConstruct<_Type2>,
												 TTypeTraits::SizeOf<_Type2>::Value > kCAny_MaxSizeBeforeAutoAlloc,
												 CBaseType<_Type2>::TypeID) { HH_ASSERT(Inert()); }
		static RTTI::TypeGUID		SpecializedTypeInfo() { return RTTI::Type<_Type2>::ID(); }
		template<typename _Type3>
		static void					SpecializedDestruct(void *object);
		template<typename _Type3>
		static void					SpecializedCopyConstruct(void *from, void *to);
	};

	//----------------------------------------------------------------------------

	template<typename _Type2>
	template<typename _Type3>
	void					SpecializedTypeTraits<_Type2, true>::SpecializedDestruct(void *object)
	{
		if (TTypeTraits::SizeOf<_Type3>::Value > kCAny_MaxSizeBeforeAutoAlloc)
			HH_DELETE(*static_cast<_Type3**>(object));
		else
		{
			Mem::Destruct(*reinterpret_cast<_Type3*>(object));
		}
	}

	template<>
	template<>
	HH_FORCEINLINE void		SpecializedTypeTraits<void, true>::SpecializedDestruct<void>(void *) {}

	template<typename _Type2>
	template<typename _Type3>
	void					SpecializedTypeTraits<_Type2, true>::SpecializedCopyConstruct(void *from, void *to)
	{
		if (TTypeTraits::SizeOf<_Type3>::Value > kCAny_MaxSizeBeforeAutoAlloc)
			*(_Type3**)(to) = HH_NEW(_Type3(**(_Type3**)(from)));
		else
		{
			Mem::Construct(*reinterpret_cast<_Type3*>(to), *reinterpret_cast<_Type3*>(from));
		}
	}

	template<>
	template<>
	HH_FORCEINLINE void		SpecializedTypeTraits<void, true>::SpecializedCopyConstruct<void>(void *, void *) {}

	//----------------------------------------------------------------------------

	// IsOfType() specializations:
	template<typename _Type, bool _IsInert>
	struct	TTypeChecker
	{
		HH_FORCEINLINE static bool	Equal(const SAnyTypeTraits *traits)
		{
			HH_ASSERT(traits != null);
			return RTTI::Type<_Type>::ID() == traits->TypeInfo();
		}
	};
	template<typename _Type>
	struct	TTypeChecker<_Type, true>
	{
		HH_FORCEINLINE static bool	Equal(const SAnyTypeTraits *traits)
		{
			HH_ASSERT(traits != null);
			return CBaseType<_Type>::TypeID == traits->BaseTypeID;	// if we are inert, as _Type isn't, this will compare a value >= 0 with -1, and will correctly return false
		}
	};

	template<typename _Type>
	SAnyTypeTraits const	*GetSAnyTypeTraits()
	{
		static Containers::SpecializedTypeTraits<_Type, Containers::IsCAnyBaseType<_Type>::True>	specializedTraits;
		return &specializedTraits;
	}

	//----------------------------------------------------------------------------

	struct SMemTypeTraits
	{
		void			(*Destruct)(void *object);
		void			(*CopyConstruct)(void *from, void *to);
		SMemTypeTraits(void (*destruct)(void*), void (*copyConstruct)(void *, void*))
		:	Destruct(destruct)
		,	CopyConstruct(copyConstruct)
		{}
	};

	template<typename _Type>
	struct SpecializedMemTypeTraits : public SMemTypeTraits
	{
		SpecializedMemTypeTraits()
		:	SMemTypeTraits(&SpecializedDesctruct, &SpecializedCopyConstruct)
		{}
		static void		SpecializedDesctruct(void* object)
		{
			Mem::Destruct(*reinterpret_cast<_Type*>(object));
		}
		static void		SpecializedCopyConstruct(void* from, void* to)
		{
			Mem::Construct(*reinterpret_cast<_Type*>(to), *reinterpret_cast<_Type*>(from));
		}
	};

	template<typename _Type>
	SMemTypeTraits const	*GetSMemTypeTraits()
	{
		static Containers::SpecializedMemTypeTraits<_Type>	specializedTraits;
		return &specializedTraits;
	}

}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

// Setup
template<typename _Type>
void	CAny::_Setup(const _Type &object)
{
	if (TTypeTraits::SizeOf<_Type>::Value <= Containers::kCAny_MaxSizeBeforeAutoAlloc)
	{
		Mem::Construct(*_Data<_Type>(), object);
	}
	else
	{
		_Type	*data = HH_NEW(_Type(object));
		HH_ASSERT(data != null);
		*_Data<_Type*>() = data;
	}

	_SetTraits(Containers::GetSAnyTypeTraits<_Type>());
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE CAny::CAny(const _Type &object)
{
	_Setup(object);
}

//----------------------------------------------------------------------------

template<typename _Type>
HH_FORCEINLINE void	CAny::Set(const _Type &object)
{
	HH_ASSERT(_Traits() != null); _Traits()->Destruct(_Data<void*>()); _Setup(object);
}

template<typename _Type>
HH_FORCEINLINE bool	CAny::IsOfType() const
{
	return Containers::TTypeChecker<_Type, Containers::IsCAnyBaseType<_Type>::True>::Equal(_Traits());
}

template<typename _Type>
const _Type	*CAny::Value() const
{
	HH_ASSERT_MESSAGE(IsOfType<_Type>(), "CAny::Value<%s>(): incompatible types" COMMA RTTI::Type<_Type>::Name());
	return UnsafeValue<_Type>();
}

template<typename _Type>
const _Type	*CAny::UnsafeValue() const
{
	HH_ASSERT(_Traits() != null);
	if (!_Traits()->Constructed)
		return _Data<const _Type>();
	return *_Data<const _Type*>();
}

template<typename _Type>
_Type	*CAny::Value()
{
	HH_ASSERT_MESSAGE(IsOfType<_Type>(), "CAny::Value<%s>(): incompatible types" COMMA RTTI::Type<_Type>::Name());
	return UnsafeValue<_Type>();
}

template<typename _Type>
_Type	*CAny::UnsafeValue()
{
	HH_ASSERT(_Traits() != null);
	if (!_Traits()->Constructed)
		return _Data<_Type>();
	return *_Data<_Type*>();
}

//----------------------------------------------------------------------------
//
//	CTypeDictionnary
//
//----------------------------------------------------------------------------

template<typename _Type>
CGuid		CTypeDictionnary::NameGUID(const CString &name)
{
	SParameterDictionnaryEntry	*entry = m_Dictionnary->Find(name);
	if (entry != null)
	{
		HH_ASSERT_MESSAGE(entry->TypeInfo() == RTTI::Type<_Type>::ID(), "Mismatching types during type dictionnary GUID fetch: \"%s\" != \"%s\"" COMMA RTTI::CManager::TypeName(entry->TypeInfo()) COMMA RTTI::Type<_Type>::Name());
		return entry->GUID;
	}

	entry = m_Dictionnary->Insert(name, SParameterDictionnaryEntry(0, BaseType_Evolved, null));
	if (entry != null)
	{
		// this is pretty dirty, but it should be safe, the whole point of this is to trash the vtable in the entry that's been added.
		// it will break down if the container reconstructs the object internally... which THashMaps should not do if we don't resize them.

		HH_STATIC_ASSERT(sizeof(TParameterDictionarySpecialization<_Type>) == sizeof(SParameterDictionnaryEntry));
		CGuid	slot = m_GUIDLookup.PushBack((SParameterDictionnaryEntry*)null);
		if (slot.Valid())
		{
			TParameterDictionarySpecialization<_Type>	virtualEntry(slot, null);
			memcpy((void*)entry, (void*)&virtualEntry, sizeof(*entry)); //-V598	// NOTE : vtable overwrite (OK in practise)
			m_GUIDLookup[slot] = entry;
			entry->Name = name;
			return entry->GUID;
		}
	}
	HH_ASSERT_NOT_REACHED_MESSAGE("could not add typed parameter to hashmap");
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type>
CGuid		CTypeDictionnary::FindNameGUID(const CString &name)
{
	SParameterDictionnaryEntry	*entry = m_Dictionnary->Find(name);
	if (entry != null)
	{
		HH_ASSERT_MESSAGE(entry->TypeInfo() == RTTI::Type<_Type>::ID(), "Mismatching types during type dictionnary GUID fetch: \"%s\" != \"%s\"" COMMA RTTI::CManager::TypeName(entry->TypeInfo()) COMMA RTTI::Type<_Type>::Name());
		return entry->GUID;
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Type>
bool	CTypeDictionnary::IsOfType(const CString &name) const
{
	const SParameterDictionnaryEntry	*entry = m_Dictionnary->Find(name);
	if (entry != null)
		return RTTI::Type<_Type>::ID() == entry->TypeInfo();
	return false;
}

//----------------------------------------------------------------------------

template<typename _Type>
bool	CTypeDictionnary::IsOfType(CGuid guid) const
{
	const SParameterDictionnaryEntry	*entry = FindTypeTagFromGUID(guid);
	if (entry != null)
		return RTTI::Type<_Type>::ID() == entry->TypeInfo();
	return false;
}

//----------------------------------------------------------------------------
//
//	TTypedParameterList
//
//----------------------------------------------------------------------------

template<typename _Dictionnary>
TTypedParameterList<_Dictionnary>::TTypedParameterList()
:	CRefCountedObject()
,	_Dictionnary()
,	m_FirstExpectedCount(0)
,	m_FirstExpectedDataSize(0)
,	m_Count(0)
,	m_DataSize(0)
,	m_Data(null)
{
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
TTypedParameterList<_Dictionnary>::~TTypedParameterList()
{
	_CallAllDestructors();
	if (m_Data != null)
		Mem::Free(m_Data);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
void	TTypedParameterList<_Dictionnary>::_CallAllDestructors()
{
	if (m_Data != null)
	{
		for (hh_u32 i = 0; i < m_Count; ++i)
		{
			const hh_u32						offset = m_Headers[i].dataOff;
			const CGuid							guid = static_cast<CGuid>(m_Headers[i].guid);
			const CTypeDictionnary::SParameterDictionnaryEntry	*dictionnaryEntry = _Dictionnary::m_NameDictionnary->FindTypeTagFromGUID(guid);
			if (dictionnaryEntry == null)
				return;
			Containers::SMemTypeTraits const	*typeTraits = dictionnaryEntry->MemTypeTraits();
			if (typeTraits == null)
				return;
			HH_ASSERT(typeTraits->Destruct != null);
			if (typeTraits->Destruct == null)
				return;
			typeTraits->Destruct(m_Data + offset);
		}
	}
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
void	TTypedParameterList<_Dictionnary>::Clear()
{
	_CallAllDestructors();
	m_Count = 0;
	m_DataSize = 0;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
bool	TTypedParameterList<_Dictionnary>::DeepCopy(const TTypedParameterList<_Dictionnary> &other)
{
	if (this == &other)	// no-op
		return true;

	// don't copy directly everything. some things might fail, and we'd be left in an invalid state.
	// perform the copy locally, and overwrite all our fields with the new ones only at the end when we know
	// we've succeded.

	hh_u8	*newData = null;
	hh_u32	newCount = other.m_Count;
	hh_u32	newDataSize = other.m_DataSize;

	if (other.m_Data != null)
	{
		// see if we need a reallocation:
		hh_u32	currentSize = m_Count * sizeof(ParamHeader) + m_DataSize;
		hh_u32	neededSize = newCount * sizeof(ParamHeader) + newDataSize;
		if (neededSize > currentSize)
		{
			newData = HH_TALLOC<hh_u8>(neededSize);
			if (newData == null)
				return false;
		}
		else
		{
			newData = m_Data;
			// destruct all the data we have in the array:
			_CallAllDestructors();
		}

		ParamHeader		*newHeaders = reinterpret_cast<ParamHeader*>(newData);
		const hh_u32	deltaOff = other.m_Headers->dataOff - newCount * sizeof(ParamHeader);

		memcpy(newData, other.m_Data, newCount * sizeof(ParamHeader));
		for (hh_u32 i = 0; i < newCount; ++i)
		{
			newHeaders[i].dataOff -= deltaOff;
			const hh_u32	offset = newHeaders[i].dataOff;
			const CGuid		guid = static_cast<CGuid>(newHeaders[i].guid);
			_Dictionnary::m_NameDictionnary->FindTypeTagFromGUID(guid)->MemTypeTraits()->CopyConstruct(other.m_Data + offset + deltaOff, newData + offset);
		}
	}

	// go ahead and finalize the copy.
	if (newData != m_Data && m_Data != null)
	{
		_CallAllDestructors();
		Mem::Free(m_Data);
	}

	m_Data = newData;
	m_Count = newCount;
	m_DataSize = newDataSize;
	m_FirstExpectedCount = other.m_FirstExpectedCount;
	m_FirstExpectedDataSize = other.m_FirstExpectedDataSize;

	if (this->_Dictionnary::DeepCopy(other))
		return true;

	Clear();
	return false;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
CGuid	TTypedParameterList<_Dictionnary>::RegisterNameGUID(const CString &name)
{
	CGuid	index = _Dictionnary::m_NameDictionnary->template FindNameGUID<_Type>(name);
	if (index.Valid())
	{
		HH_ASSERT_NOT_REACHED_MESSAGE("Typed parameter '%s' is already registered!\n" COMMA name.Data());
		return index;//CGuid::INVALID;
	}
	return _Dictionnary::m_NameDictionnary->template NameGUID<_Type>(name);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
CGuid						TTypedParameterList<_Dictionnary>::NameGUID(const CString &name)
{
	return _Dictionnary::m_NameDictionnary->template NameGUID<_Type>(name);
}

template<typename _Dictionnary>
template<typename _Type>
CGuid						TTypedParameterList<_Dictionnary>::FindNameGUID(const CString &name)
{
	return _Dictionnary::m_NameDictionnary->template FindNameGUID<_Type>(name);
}

template<typename _Dictionnary>
const CString				&TTypedParameterList<_Dictionnary>::FindNameFromGUID(CGuid nameGUID)
{
	return _Dictionnary::m_NameDictionnary->FindNameFromGUID(nameGUID);
}

template<typename _Dictionnary>
TPair<CGuid, EBaseTypeID>	TTypedParameterList<_Dictionnary>::FindUntypedNameGUID(const CString &name)
{
	return _Dictionnary::m_NameDictionnary->FindUntypedNameGUID(name);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
CGuid 						TTypedParameterList<_Dictionnary>::ParameterNameGUID(CGuid parameterIndex) const
{
	HH_ASSERT(hh_u32(parameterIndex) < ParameterCount());
	return static_cast<CGuid>(m_Headers[parameterIndex].guid);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
CGuid	TTypedParameterList<_Dictionnary>::AddParameter(const _Type &parameter, CGuid nameGUID)
{
	HH_STATIC_ASSERT(TTypeTraits::HasTrivialRelocate<_Type>::True);
	HH_PARANOID_ASSERT_MESSAGE(_Dictionnary::m_NameDictionnary->template IsOfType<_Type>(nameGUID), "mismatching types");
	HH_ASSERT(hh_u32(nameGUID) < 0xFFFF);

	CGuid	index = FindParameterId(nameGUID);
	if (index.Valid())
		return index;

	const hh_u32	oldDataSize = m_DataSize;
	const hh_u32	newDataSize = m_DataSize + sizeof(_Type);
	index = m_Count++;

	HH_ASSERT(index < 0xFFFF);

	hh_u32	dataOff = 0;
	if (index > 0)
	{
		dataOff = m_Headers->dataOff;
		if (m_Count > m_FirstExpectedCount)
		{
			const hh_u32	newDataOffset = m_Count * sizeof(ParamHeader);
			m_Data = HH_TREALLOC(m_Data, newDataOffset + newDataSize);
			if (m_Data == null)
				return CGuid::INVALID;

			m_FirstExpectedDataSize = newDataSize;
			m_FirstExpectedCount = m_Count;

			memmove(m_Data + newDataOffset,
					m_Data + dataOff,
					oldDataSize);

			for (hh_u32 i = 0; i < index; i++)
				m_Headers[i].dataOff = m_Headers[i].dataOff - dataOff + newDataOffset;

			dataOff = newDataOffset;
		}
		else if (newDataSize > m_FirstExpectedDataSize)
		{
			m_Data = HH_TREALLOC(m_Data, dataOff + newDataSize);
			if (m_Data == null)
				return CGuid::INVALID;
			m_FirstExpectedDataSize = newDataSize;
		}

		dataOff += oldDataSize;
	}
	else
	{
		if (m_Data != null)
		{
			if (m_FirstExpectedCount >= m_Count &&
				m_FirstExpectedDataSize >= newDataSize)
			{
				dataOff = m_FirstExpectedCount * sizeof(ParamHeader);
			}
			else
			{
				Mem::Free(m_Data);
				m_Data = 0;
			}
		}

		if (m_Data == null)
		{
			dataOff = HHMax(m_FirstExpectedCount, m_Count) * sizeof(ParamHeader);
			m_Data = HH_TALLOC<hh_u8>(dataOff + HHMax(m_FirstExpectedDataSize, newDataSize));
			if (m_Data == null)
				return CGuid::INVALID;
		}
	}

	ParamHeader	&header = m_Headers[index];
	header.guid = nameGUID;
	header.dataOff = dataOff;
	Mem::Construct(*reinterpret_cast<_Type*>(m_Data + header.dataOff), parameter);
	m_DataSize = newDataSize;
	return index;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
CGuid	TTypedParameterList<_Dictionnary>::AddParameter(const _Type &parameter, const CString &parameterName)
{
	CGuid	index = _Dictionnary::m_NameDictionnary->template NameGUID<_Type>(parameterName);
	if (index.Valid())
		return AddParameter(parameter, index);
	HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Name Not Registered: %s" COMMA parameterName.Data());
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
void	TTypedParameterList<_Dictionnary>::UpdateParameter(CGuid nameGUID, const _Type &parameter)
{
	CGuid	parameterIndex = FindParameterId(nameGUID);
	if (parameterIndex.Valid())
	{
		HH_ASSERT(hh_u32(parameterIndex) < ParameterCount());
		UnsafeUpdateParameterFromId<_Type>(parameterIndex, parameter);
	}
	else
		HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Not Found: %s" COMMA FindNameFromGUID(nameGUID).Data());
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
void	TTypedParameterList<_Dictionnary>::UpdateParameterFromId(CGuid parameterIndex, const _Type &parameter)
{
	HH_ASSERT(parameterIndex.Valid() && hh_u32(parameterIndex) < ParameterCount());
	HH_PARANOID_ASSERT_MESSAGE(_Dictionnary::m_NameDictionnary->template IsOfType<_Type>(static_cast<CGuid>(m_Headers[parameterIndex].guid)), "mismatching types");
	UnsafeUpdateParameterFromId(parameterIndex, parameter);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
void	TTypedParameterList<_Dictionnary>::UpdateParameterAndAddIFN(const CString &name, const _Type &value)
{
	CGuid	nameId = NameGUID<_Type>(name);
	if (nameId.Valid())
	{
		CGuid	id = FindParameterId(nameId);
		if (!id.Valid())
			id = AddParameter<_Type>(value, nameId);
		else
			UnsafeUpdateParameterFromId<_Type>(id, value);
	}
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
void	TTypedParameterList<_Dictionnary>::UpdateParameterAndAddIFN(CGuid nameGUID, const _Type &value)
{
	CGuid	parameterIndex = FindParameterId(nameGUID);
	if (!parameterIndex.Valid())
		AddParameter<_Type>(value, nameGUID);
	else
		UnsafeUpdateParameterFromId<_Type>(parameterIndex, value);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
const _Type *TTypedParameterList<_Dictionnary>::Parameter(CGuid nameGUID)
{
	CGuid	parameterIndex = FindParameterId(nameGUID);
	if (parameterIndex.Valid())
		return ParameterFromId(parameterIndex);
	//else
		//HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Not Found: %s" COMMA FindNameFromGUID(nameGUID).Data());
	return null;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
const _Type	*TTypedParameterList<_Dictionnary>::Parameter(const CString& parameterName) const
{
	CGuid	nameGUID = _Dictionnary::m_NameDictionnary->template FindNameGUID<_Type>(parameterName);
	if (nameGUID.Valid())
	{
		CGuid	parameterIndex = FindParameterId(nameGUID);
		if (parameterIndex.Valid())
		{
			HH_ASSERT(parameterIndex.Valid() && hh_u32(parameterIndex) < ParameterCount());
			return reinterpret_cast<_Type const *>(m_Data + m_Headers[parameterIndex].dataOff);
		}
		//else
			//HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Not Found: %s" COMMA parameterName.Data());
	}
	//else
		//HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Name Not Registered: %s" COMMA parameterName.Data());
	return null;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
const _Type	*TTypedParameterList<_Dictionnary>::ParameterFromId(CGuid parameterIndex) const
{
	HH_ASSERT(parameterIndex.Valid() && hh_u32(parameterIndex) < ParameterCount());
	HH_PARANOID_ASSERT_MESSAGE(_Dictionnary::m_NameDictionnary->template IsOfType<_Type>(static_cast<CGuid>(m_Headers[parameterIndex].guid)), "mismatching types");
	return reinterpret_cast<_Type const * >(m_Data + m_Headers[parameterIndex].dataOff);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
const _Type *TTypedParameterList<_Dictionnary>::UnsafeParameter(CGuid nameGUID)
{
	CGuid	parameterIndex = FindParameterId(nameGUID);
	if (parameterIndex.Valid())
		return UnsafeParameterFromId(parameterIndex);
	//else
		//HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Not Found: %s" COMMA FindNameFromGUID(nameGUID).Data());
	return null;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
const _Type	*TTypedParameterList<_Dictionnary>::UnsafeParameter(const CString& parameterName) const
{
	CGuid	nameGUID = _Dictionnary::m_NameDictionnary->template FindNameGUID<_Type>(parameterName);
	if (nameGUID.Valid())
	{
		CGuid	parameterIndex = FindParameterId(nameGUID);
		if (parameterIndex.Valid())
		{
			HH_ASSERT(parameterIndex.Valid() && hh_u32(parameterIndex) < ParameterCount());
			return reinterpret_cast<_Type const *>(m_Data + m_Headers[parameterIndex].dataOff);
		}
		//else
			//HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Not Found: %s" COMMA parameterName.Data());
	}
	//else
		//HH_ASSERT_NOT_REACHED_MESSAGE("Typed Parameter Name Not Registered: %s" COMMA parameterName.Data());
	return null;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
const _Type	*TTypedParameterList<_Dictionnary>::UnsafeParameterFromId(CGuid parameterIndex) const
{
	HH_ASSERT(parameterIndex.Valid() && hh_u32(parameterIndex) < ParameterCount());
	return reinterpret_cast<_Type const *>(m_Data + m_Headers[parameterIndex].dataOffset);
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
CGuid	TTypedParameterList<_Dictionnary>::FindParameterId(CGuid nameGUID) const
{
	HH_ASSERT(nameGUID.Valid());
	for (hh_u32 i = 0; i < ParameterCount(); i++)
	{
		if (static_cast<CGuid>(m_Headers[i].guid) == nameGUID)
			return i;
	}
	return CGuid::INVALID;
}

//----------------------------------------------------------------------------

template<typename _Dictionnary>
template<typename _Type>
void	TTypedParameterList<_Dictionnary>::UnsafeUpdateParameterFromId(CGuid parameterIndex, const _Type &parameter)
{
	Mem::Construct(*reinterpret_cast<_Type*>(m_Data + m_Headers[parameterIndex].dataOff), parameter);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_ANY_INL__
