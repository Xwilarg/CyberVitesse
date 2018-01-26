#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/09/17 10:52 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_LIST_H__
#define __KR_CONTAINERS_LIST_H__

#include "hh_kernel/include/kr_refptr.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>
class	TIntrusiveLinkedListHook : public CRefCountedObject
{
private:
	typedef	TIntrusiveLinkedListHook<_Type>	SelfType;
	typedef	TRefPtr<SelfType>				SelfPtr;
	typedef	TRefPtr<_Type>					TypePtr;

	//typedef	TIntrusiveLinkedListHook	SelfType;
	//typedef	_Type		SelfType;

	SelfPtr	m_Next;
	SelfPtr	m_Prev;

protected:
public:
//	_Type	m_Self;

	TIntrusiveLinkedListHook() {}
	virtual ~TIntrusiveLinkedListHook()
	{
		Unlink();
	}

	void	Exterminate()
	{
		// here, if we haven't been unlinked, propagate destruction to all the elements we are touching...
		if (Linked())
		{
			SelfPtr	cur;

			cur = m_Prev;
			m_Prev = null;
			while (cur != null)
			{
				SelfPtr	nextInLine = cur->m_Prev;
				cur->m_Next = null;
				cur->m_Prev = null;
				cur = nextInLine;
			}

			cur = m_Next;
			m_Next = null;
			while (cur != null)
			{
				SelfPtr	nextInLine = cur->m_Next;
				cur->m_Next = null;
				cur->m_Prev = null;
				cur = nextInLine;
			}
		}
	}

	HH_FORCEINLINE _Type	*Next() const { return static_cast<_Type*>(m_Next.Get()); }
	HH_FORCEINLINE _Type	*Prev() const { return static_cast<_Type*>(m_Prev.Get()); }
	_Type	*First()
	{
		SelfType	*cur = this;
		SelfType	*next = Next();
		while (next != null)
		{
			cur = next;
			next = cur->Next();
		}
		return reinterpret_cast<_Type*>(cur);
	}
	SelfType	*Last()
	{
		SelfType	*cur = this;
		SelfType	*prev = Prev();
		while (prev != null)
		{
			cur = prev;
			prev = cur->Prev();
		}
		return reinterpret_cast<_Type*>(cur);
	}

	void	InsertBefore(TypePtr element)
	{
		Unlink();
		if (element == null)
			return;
		m_Prev = element->m_Prev;
		if (m_Prev != null)
			m_Prev->m_Next = this;
		m_Next = element.Get();
		m_Next->m_Prev = this;
	}

	void	InsertListBefore(TypePtr element)
	{
		if (m_Prev != null)
			m_Prev->m_Next = null;
		if (element == null)
			return;
		m_Prev = element->m_Prev;
		if (m_Prev != null)
			m_Prev->m_Next = this;

		SelfType *cur = this;
		while (cur->m_Next != null)
			cur = cur->m_Next.Get();

		cur->m_Next = element.Get();
		element->m_Prev = cur;
	}

	void	InsertAfter(TypePtr element)
	{
		HH_ASSERT(element != null);
		Unlink();
		m_Next = element->m_Next;
		if (m_Next != null)
			m_Next->m_Prev = this;
		m_Prev = element.Get();
		m_Prev->m_Next = this;
	}

	HH_FORCEINLINE bool	Linked() const { return m_Next != null || m_Prev != null; }
	void	Unlink()
	{
//		SelfPtr		selfForSurvival = this;
		SelfPtr		prev = m_Prev.Get();
		SelfPtr		next = m_Next.Get();
		m_Prev = null;
		m_Next = null;

		if (prev != null)
			prev->m_Next = next;
		if (next != null)
			next->m_Prev = prev;
	}

	//void	Delete() { this->TIntrusiveLinkedListHook::~TIntrusiveLinkedListHook(); }

	hh_u32	ListLength_Slow() { return PositionInList_Slow(null); }
	hh_u32	PositionInList_Slow() { return PositionInList_Slow(this); }
	hh_u32	PositionInList_Slow(SelfType *element)
	{
		SelfType *it = reinterpret_cast<SelfType*>(First());
		HH_ASSERT(it != null);

		hh_u32 counter = 0;
		while (it != null && it != element)
		{
			it = it->m_Next.Get();
			counter++;
		}
		return counter;
	}
//	hh_u32	PositionInList_Slow(_Type *element)
//	{
//		_Type *it = First();
//		HH_ASSERT(it != null);
//
//		hh_u32 counter = 0;
//		while (it != null && it != element)
//		{
//			it = reinterpret_cast<SelfType*>(it)->m_Next;
//			counter++;
//		}
//		return (counter);
//	}

	_Type	*ElementAtIndex_MightBeSlow(hh_u32 index)
	{
		SelfType	*it = First();
		while (it != null && index--)
			it = it->m_Next.Get();
		return reinterpret_cast<_Type*>(it);
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_LIST_H__
