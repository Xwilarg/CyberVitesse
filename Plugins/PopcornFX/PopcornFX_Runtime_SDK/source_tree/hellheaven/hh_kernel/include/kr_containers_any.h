#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2008/04/28 10:55 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_ANY_H__
#define __KR_CONTAINERS_ANY_H__

#include "hh_kernel/include/kr_rtti.h"
#include "hh_kernel/include/kr_base_types.h"
#include "hh_kernel/include/kr_refptr.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

namespace	Containers
{
	static const hh_u32		kCAny_SelfSizeInBytes = 0x20;	// note: enough to store a struct containing float4+float3 on 32-bits builds, or float4+float2 on 64-bits builds
	static const hh_u32		kCAny_MaxSizeBeforeAutoAlloc = kCAny_SelfSizeInBytes - sizeof(void*);

	HH_STATIC_ASSERT(kCAny_SelfSizeInBytes >= 2 * sizeof(void*));
	HH_STATIC_ASSERT((hh_i32)kCAny_MaxSizeBeforeAutoAlloc > 0);	// if the above static assert isn't triggered, this one never should.
	HH_STATIC_ASSERT(kCAny_MaxSizeBeforeAutoAlloc >= sizeof(void*));

											struct	SAnyTypeTraits;
	template<typename _Type, bool _IsInert>	struct	SpecializedTypeTraits;
	template<typename _Type> SAnyTypeTraits const	*GetSAnyTypeTraits();

	struct SMemTypeTraits;
	template<typename _Type> SMemTypeTraits const	*GetSMemTypeTraits();
}

//----------------------------------------------------------------------------

class HH_KERNEL_EXPORT CAny
{
private:
	hh_u8											m_RawData[Containers::kCAny_SelfSizeInBytes];

	template<typename _Type>
	HH_FORCEINLINE _Type							*_Data() const { return (_Type*)&(m_RawData[0]); }
	HH_FORCEINLINE const Containers::SAnyTypeTraits	*_Traits() const
	{
		const hh_u8	*storage = &(m_RawData[Containers::kCAny_MaxSizeBeforeAutoAlloc]);
		return reinterpret_cast<Containers::SAnyTypeTraits const * const *>(storage)[0];
	}
	HH_FORCEINLINE void								_SetTraits(const Containers::SAnyTypeTraits *value)
	{
		hh_u8	*storage = &(m_RawData[Containers::kCAny_MaxSizeBeforeAutoAlloc]);
		reinterpret_cast<Containers::SAnyTypeTraits const **>(storage)[0] = value;
	}

	// Setup
	template<typename _Type> void					_Setup(const _Type &object);

	void											_RawCopy(const CAny &other);

public:
	static const CAny	Empty;

public:
	template<typename _Type>						CAny(const _Type &object);
													CAny();
													CAny(const CAny &other);
													~CAny();

	void											operator = (const CAny &other);

	RTTI::TypeGUID									TypeInfo() const;

	template<typename _Type> void					Set(const _Type &object);
	template<typename _Type> bool					IsOfType() const;

	// very unsafe, be careful with what you do with this!
	EBaseTypeID										BaseTypeID() const;

	bool											TypeEqualWith(const CAny &other) const;

	template<typename _Type> const _Type			*Value() const;
	template<typename _Type> const _Type			*UnsafeValue() const;

	template<typename _Type> _Type					*Value();
	template<typename _Type> _Type					*UnsafeValue();
};

//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CTypeDictionnary
{
public:
	struct	SParameterDictionnaryEntry
	{
		hh_u32		GUID;
		EBaseTypeID	InertTypeId;
		CString		Name;

		SParameterDictionnaryEntry(hh_u32 guid, EBaseTypeID inertTypeId, const CString &name) : GUID(guid), InertTypeId(inertTypeId), Name(name) {}
		virtual ~SParameterDictionnaryEntry() {}
		virtual RTTI::TypeGUID						TypeInfo() const { HH_ASSERT_NOT_REACHED_MESSAGE("INFERNO!!! unspecialized typed parameter dictionnary entry!"); return RTTI::TypeGUID::INVALID; }
		virtual Containers::SAnyTypeTraits const	*TypeTraits() const { HH_ASSERT_NOT_REACHED_MESSAGE("INFERNO!!! unspecialized typed parameter dictionnary entry!"); return null; }
		virtual Containers::SMemTypeTraits const	*MemTypeTraits() const { HH_ASSERT_NOT_REACHED_MESSAGE("INFERNO!!! unspecialized typed parameter dictionnary entry!"); return null; }
	};
	template<typename _Type>
	struct	TParameterDictionarySpecialization : public SParameterDictionnaryEntry
	{
		TParameterDictionarySpecialization(hh_u32 guid, const CString &name) : SParameterDictionnaryEntry(guid, CBaseType<_Type>::TypeID, name)
		{
			/*OutputDebugString(CString::Format("Specialization: \"%s\" (\"%s\"), InertTypeId: %d\n", RTTI::Type<_Type>::Name(), RTTI::Type<_Type>::Name(), InertTypeId).Data());*/
			// quickfix
			// call to initialize any static hoping we are not already in a thread
			RTTI::Type<_Type>::ID();
			Containers::GetSAnyTypeTraits<_Type>();
			Containers::GetSMemTypeTraits<_Type>();
		}
		virtual RTTI::TypeGUID						TypeInfo() const override { return RTTI::Type<_Type>::ID(); }
		virtual Containers::SAnyTypeTraits const	*TypeTraits() const override { return Containers::GetSAnyTypeTraits<_Type>(); }
		virtual Containers::SMemTypeTraits const	*MemTypeTraits() const override { return Containers::GetSMemTypeTraits<_Type>(); }
	};

private:
	THashMap<SParameterDictionnaryEntry, CString>	*m_Dictionnary;
	TArray<SParameterDictionnaryEntry*>				m_GUIDLookup;

protected:
public:
	CTypeDictionnary();
	~CTypeDictionnary();

	void								Clear();

	template<typename _Type> CGuid		NameGUID(const CString &name);
	template<typename _Type> CGuid		FindNameGUID(const CString &name);

	const CString						&FindNameFromGUID(CGuid nameGUID) const;

	// we could make a function that does both this and the thing above
	const SParameterDictionnaryEntry	*FindTypeTagFromGUID(CGuid nameGUID) const;
	TPair<CGuid, EBaseTypeID>			FindUntypedNameGUID(const CString &name);

	template<typename _Type> bool		IsOfType(const CString &name) const;
	template<typename _Type> bool		IsOfType(CGuid guid) const;

};

//----------------------------------------------------------------------------

//class	CParameterListSizer
//{
//private:
//	hh_u32	m_Size;
//public:
//	CParameterListSizer() : m_Size(0) {}
//
//	HH_FORCEINLINE hh_u32	Size() const { return m_Size & 0xFFFF; }
//	HH_FORCEINLINE hh_u32	Count() const { return m_Size >> 16; }
//
//	template<typename _Type>
//	CParameterListSizer	&Expect() const { m_Size += sizeof(_Type) + 0x10000; return *this; }
//};

//----------------------------------------------------------------------------

template<typename _Dictionnary>
class TTypedParameterList : public CRefCountedObject, public _Dictionnary
{
private:
	struct ParamHeader
	{
		hh_u16		guid;
		hh_u16		dataOff;
	};

	hh_u32				m_FirstExpectedCount;
	hh_u32				m_FirstExpectedDataSize;
	hh_u32				m_Count;
	hh_u32				m_DataSize;
	union
	{
		hh_u8			*m_Data;
		ParamHeader		*m_Headers;
	};

protected:
	TTypedParameterList();	// cannot manually construct on stack, MUST go through an 'Alloc' call

	void									_CallAllDestructors();

public:
	static TRefPtr<TTypedParameterList<_Dictionnary> >	Alloc() { return HH_NEW(TTypedParameterList<_Dictionnary>); }

	~TTypedParameterList();

	bool									DeepCopy(const TTypedParameterList<_Dictionnary> &other);

	template<typename _Type> static CGuid	RegisterNameGUID(const CString &name);
	template<typename _Type> static CGuid	NameGUID(const CString &name);
	template<typename _Type> static CGuid	FindNameGUID(const CString &name);
	static const CString&					FindNameFromGUID(CGuid nameGUID);
	static TPair<CGuid, EBaseTypeID>		FindUntypedNameGUID(const CString &name);

	HH_FORCEINLINE void						Clear();
	HH_FORCEINLINE bool						Empty() const { return m_Count == 0; }
	HH_FORCEINLINE hh_u32					ParameterCount() const { return m_Count; }

	// Only before AddParameter memory alloc expectations
	template<typename _Type> void			Expect() { if (m_Data == null) { m_FirstExpectedCount++; m_FirstExpectedDataSize += sizeof(_Type); } }

	CGuid									ParameterNameGUID(CGuid parameterIndex) const;
	template<typename _Type> CGuid			AddParameter(const _Type &parameter, CGuid nameGUID);
	template<typename _Type> CGuid			AddParameter(const _Type &parameter, const CString &parameterName);

	template<typename _Type> void			UpdateParameter(CGuid nameGUID, const _Type &parameter);
	template<typename _Type> void			UpdateParameterFromId(CGuid parameterIndex, const _Type &parameter);
	template<typename _Type> void			UpdateParameterAndAddIFN(const CString &name, const _Type &value);
	template<typename _Type> void			UpdateParameterAndAddIFN(CGuid nameGUID, const _Type &value);

	template<typename _Type> const _Type	*Parameter(CGuid nameGUID);
	template<typename _Type> const _Type	*Parameter(const CString& parameterName) const;
	template<typename _Type> const _Type	*ParameterFromId(CGuid parameterIndex) const;

	template<typename _Type> const _Type	*UnsafeParameter(CGuid nameGUID);
	template<typename _Type> const _Type	*UnsafeParameter(const CString& parameterName) const;
	template<typename _Type> const _Type	*UnsafeParameterFromId(CGuid parameterIndex) const;

	CGuid									FindParameterId(CGuid nameGUID) const;

private:
	template<typename _Type> void			UnsafeUpdateParameterFromId(CGuid parameterIndex, const _Type &parameter);
};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_containers_any.inl"

#endif // __KR_CONTAINERS_ANY_H__
