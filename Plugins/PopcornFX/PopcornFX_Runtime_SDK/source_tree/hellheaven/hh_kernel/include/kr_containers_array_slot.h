#pragma once

//----------------------------------------------------------------------------
// Created on Mon 2008/07/28 18:31 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_SLOT_ARRAY_H__
#define __KR_CONTAINERS_SLOT_ARRAY_H__

#include "hh_kernel/include/kr_containers.h"
#include "hh_kernel/include/kr_containers_array_storage.h"
#include "hh_kernel/include/kr_string.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>	struct	TSlotArrayInvalidExtractor
{
	static HH_FORCEINLINE const _Type	&Invalid() { return _Type::Invalid; }
	static HH_FORCEINLINE void			Invalidate(_Type &object) { object = _Type::Invalid; }
	static HH_FORCEINLINE bool			IsValid(const _Type &object) { return object.Valid(); }
};

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage, typename _VType>
class TSlotArray_BaseIterator;

//----------------------------------------------------------------------------

template<typename _Type, typename _Storage>
class	TSlotArray_Base : public _Storage
{
public:
//	typedef _Type								Type;
	typedef _Type								ValueType;
	typedef _Type const							ValueTypeConst;

	typedef	TSlotArray_BaseIterator<_Type, _Storage, ValueType>			Iterator;
	typedef	TSlotArray_BaseIterator<_Type, _Storage, ValueTypeConst>	ConstIterator;

private:
	typedef TSlotArray_Base<_Type, _Storage>	SelfType;
	hh_u32										m_UsedSlots;
	hh_u32										m_FirstFreeSlot;

	// Not using copies of slot arrays anywhere in the codebase, don't implement it.
	TSlotArray_Base(const SelfType &other);
	void										operator = (const SelfType &other);

protected:
	hh_u32										_JumpToNextFreeSlot(hh_u32 curSlot) const;
	CGuid										_FindFreeSlotIFN();
	CGuid										_FirstValidSlot() const;

public:
	TSlotArray_Base();
//	~TSlotArray_Base() {}

	HH_FORCEINLINE bool							Empty() const { return UsedCount() == 0; }
	HH_FORCEINLINE hh_u32						Count() const { return _Storage::_MaxCount(); }
	HH_FORCEINLINE hh_u32						UsedCount() const { return m_UsedSlots; }

	template<typename _Type2> CGuid				Insert(const _Type2 &element);
#if	(PK_HAS_RVALUE_REF != 0)
	CGuid										Insert(_Type &&element);
#endif
	HH_FORCEINLINE void							Remove(CGuid slot);
	void										Clear();
	void										Clean();
	bool										Reserve(hh_u32 totalCapacity);

	template<typename _Type2> CGuid				IndexOf(_Type2 const &what) const;
	template<typename _Type2> bool				Contains(_Type2 const &what) const;

	HH_FORCEINLINE const _Type					&operator [] (CGuid slot) const;
	HH_FORCEINLINE _Type						&operator [] (CGuid slot);

	Iterator									Begin();
	Iterator									End();
	ConstIterator								Begin() const;
	ConstIterator								End() const;
};

//----------------------------------------------------------------------------

template<typename _Type, bool _DestructElements>
struct	SSlotDestructor
{
	HH_FORCEINLINE static void	ConstructRange(_Type *first, _Type *last)
	{
		while (first < last)
		{
			Mem::Construct(*first++, TSlotArrayInvalidExtractor<_Type>::Invalid());
		}
	}
	HH_FORCEINLINE static void	DestroyRange(_Type *first, _Type *last)
	{
	}
};

template<typename _Type>
struct	SSlotDestructor<_Type, true>
{
	HH_FORCEINLINE static void	ConstructRange(_Type *first, _Type *last)
	{
		while (first < last)
		{
			Mem::Construct(*first++, TSlotArrayInvalidExtractor<_Type>::Invalid());
		}
	}
	HH_FORCEINLINE static void	DestroyRange(_Type *first, _Type *last)
	{
		while (first < last)
		{
			Mem::Destruct(*first++);
		}
	}
};

//----------------------------------------------------------------------------
//
//	Use these classes
//
//----------------------------------------------------------------------------

template<typename _Type, typename _Controller = TArrayStaticController<> >
class	TSlotArray : public TSlotArray_Base<_Type, TArrayContinuousStorage_Dynamic<_Type, SSlotDestructor<_Type, TTypeTraits::HasTrivialDestructor<_Type>::False>, _Controller> >
{
public:
	typedef _Controller	Controller;
};

//----------------------------------------------------------------------------

template<typename _Type, hh_u32 _Size, hh_u32 _Alignment = sizeof(void*)>
class	TStaticSlotArray : public TSlotArray_Base<_Type, TArrayContinuousStorage_Static<_Type, SSlotDestructor<_Type, TTypeTraits::HasTrivialDestructor<_Type>::False>, _Size, _Alignment> >
{
public:
};

//----------------------------------------------------------------------------
__PK_API_END

#include "hh_kernel/include/kr_containers_array_slot.inl"

#endif // __KR_CONTAINERS_SLOT_ARRAY_H__
