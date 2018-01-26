#pragma once

//----------------------------------------------------------------------------
// Created on Thu 2009/01/22 17:55 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_CONTAINERS_LOCKLESS_H__
#define __KR_CONTAINERS_LOCKLESS_H__

#include "hh_kernel/include/kr_threads.h"
#include "hh_kernel/include/kr_threads_rwlock.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

template<typename _Type>
struct	TLocklessPtrHelper
{
	typedef	_Type*	NativePtrType;
	typedef _Type*	PtrType;
	HH_FORCEINLINE static void			AddRefIFP(PtrType ptr) {}
	HH_FORCEINLINE static void			DelRefIFP(PtrType ptr) {}
	HH_FORCEINLINE static NativePtrType	ToNativePointer(PtrType ptr) { return ptr; }
	HH_FORCEINLINE static void			StoreNative(PtrType &ptr, NativePtrType nativePtr) { ptr = nativePtr; }
};

//----------------------------------------------------------------------------

template<typename _Type>
struct	TLocklessPtrHelper<TRefPtr<_Type> >
{
	typedef	_Type*			NativePtrType;
	typedef TRefPtr<_Type>	PtrType;
	HH_FORCEINLINE static void			AddRefIFP(PtrType ptr) { ptr.Get()->AddReference(); }
	HH_FORCEINLINE static void			DelRefIFP(PtrType ptr) { _Type::RemoveReference(ptr.Get()); }
	HH_FORCEINLINE static NativePtrType	ToNativePointer(PtrType ptr) { return ptr.Get(); }
	HH_FORCEINLINE static void			StoreNative(PtrType &ptr, NativePtrType nativePtr) { ptr.UnsafeSetPtrFromManuallyRefIncrementedRawPointer(nativePtr); }
};

//----------------------------------------------------------------------------
//
//	simple lock-free LIFO stack, only handles a Push() and Pop() method.
//	_Type should have a member '_Type *m_Next' accessible by 'TLocklessStack<_Type>'
//	ideally, 'm_Next' should not be accessed manually outside TLocklessStack.
//	you can inherit from the helper class 'TLocklessListElement<_Type>'
//
//	this container is refpointer-safe.
//
//----------------------------------------------------------------------------

template<typename _Type, bool _UseCount = true>
class	TLocklessStack
{
private:
protected:
	typedef	typename TLocklessPtrHelper<_Type>::NativePtrType	_NativePtrType;
	typedef typename TLocklessPtrHelper<_Type>::PtrType			_PType;

	_NativePtrType	volatile m_Head;
	hh_u32			volatile m_Count;

public:
	TLocklessStack() : m_Head(null), m_Count(0) {}

	HH_FORCEINLINE hh_u32	Count() const { return _UseCount ? m_Count : 0; }
	HH_FORCEINLINE bool		Empty() const { return m_Head == null; }

	void	Push(_PType node)
	{
		HH_ASSERT(node->m_Next == null);
		TLocklessPtrHelper<_Type>::AddRefIFP(node);
		_NativePtrType	newHead = TLocklessPtrHelper<_Type>::ToNativePointer(node);
		while (true)
		{
			_NativePtrType	oldHead = m_Head;
			newHead->m_Next = oldHead;
			if (Atomic::Interlocked_CMPXCHG(&m_Head, newHead, oldHead) == oldHead)
			{
				if (_UseCount)
				{
					Atomic::Interlocked_XINC(&m_Count);
				}
				return;
			}
		}
	}

	void	PopInto(_PType &retNode)
	{
		while (true)
		{
			_NativePtrType	oldHead = m_Head;
			if (oldHead == null)
			{
				retNode = null;
				return;
			}
			_NativePtrType	newHead = oldHead->m_Next;
			if (Atomic::Interlocked_CMPXCHG(&m_Head, newHead, oldHead) == oldHead)
			{
				if (_UseCount)
				{
					Atomic::Interlocked_XDEC(&m_Count);
				}
				oldHead->m_Next = null;
				TLocklessPtrHelper<_Type>::StoreNative(retNode, oldHead);
				return;
			}
		}
	}

	HH_FORCEINLINE _PType	Pop()	// pop from the tail
	{
		_PType	ret;
		PopInto(ret);
		return ret;
	}
};

//----------------------------------------------------------------------------
//
//	RWLocks based queue
//
//	! BUGGEE
//	TODO: PushBatch
//
//----------------------------------------------------------------------------

#if 0
template<typename _Type, bool _UseCount = true>
class	TLockedQueue_RW
{
private:
protected:
	typedef	typename TLocklessPtrHelper<_Type>::NativePtrType	_NativePtrType;
	typedef typename TLocklessPtrHelper<_Type>::PtrType			_PType;

	_NativePtrType volatile		m_Head;
	_NativePtrType volatile		m_Tail;
	Threads::CCriticalSection	m_HeadLock;
	Threads::CRWSpinLock		m_TailLock;
	hh_u32 volatile 			m_Count;

public:
	TLockedQueue_RW() : m_Head(null), m_Tail(null), m_Count(0) {}

	HH_FORCEINLINE hh_u32	Count() const { return _UseCount ? m_Count : 0; }
	HH_FORCEINLINE bool		Empty() const { return m_Head == null; }

	//	TODO: PushBatch

	void	Push(_PType node)	// push on the head
	{
		HH_ASSERT(node->m_Next == null);
		TLocklessPtrHelper<_Type>::AddRefIFP(node);
		_NativePtrType	newHead = TLocklessPtrHelper<_Type>::ToNativePointer(node);

		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_HeadLock);
			m_TailLock.LockRead();
			if (m_Tail == null)
			{
				m_Head = newHead;
				m_Tail = newHead;
			}
			else
			{
				if (m_Head != null)
					m_Head->m_Next = newHead;
				m_Head = newHead;
			}
			m_TailLock.UnlockRead();
		}

		if (_UseCount)
		{
			Atomic::Interlocked_XINC(&m_Count);
		}
	}

	void	PopInto(_PType &retNode)	// pop from the tail
	{
		m_TailLock.LockWrite();
		_NativePtrType		tail = m_Tail;
		if (tail != null)
			m_Tail = tail->m_Next;
		m_TailLock.UnlockWrite();

		if (tail != null)
		{
			tail->m_Next = null;
			if (_UseCount)
			{
				Atomic::Interlocked_XDEC(&m_Count);
			}
		}

		TLocklessPtrHelper<_Type>::StoreNative(retNode, tail);

		return;
	}

	HH_FORCEINLINE _PType	Pop()	// pop from the tail
	{
		_PType	ret;
		PopInto(ret);
		return ret;
	}
};

#endif // 0

//----------------------------------------------------------------------------
//
//	lockless FIFO queue
//
//  rw spin locks livelocks, so fallback on rw lock
//
//   !!!! BUGGEE
//	TODO: PushBatch
//
//----------------------------------------------------------------------------

#if 0

template<typename _Type, bool _UseCount = true>
class	TLocklessQueue
{
private:
protected:
	typedef	typename TLocklessPtrHelper<_Type>::NativePtrType	_NativePtrType;
	typedef typename TLocklessPtrHelper<_Type>::PtrType			_PType;

	_NativePtrType volatile		m_Head;
	TAtomic<_NativePtrType>		m_Tail;
	Threads::CCriticalSection	m_HeadLock;
	Threads::CRWLock			m_NextLock;
	hh_u32 volatile 			m_Count;

public:
	TLocklessQueue() : m_Head(null), m_Tail(null), m_Count(0) {}

	HH_FORCEINLINE hh_u32	Count() const { return _UseCount ? m_Count : 0; }
	HH_FORCEINLINE bool		Empty() const { return m_Head == null; }

	//	TODO: PushBatch

	void	Push(_PType node)	// push on the head
	{
		HH_ASSERT(node->m_Next == null);
		TLocklessPtrHelper<_Type>::AddRefIFP(node);
		_NativePtrType	newHead = TLocklessPtrHelper<_Type>::ToNativePointer(node);

		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_HeadLock);
			if (m_Head != null)
				m_Head->m_Next = newHead;
			m_Head = newHead;
			m_Tail.SetValueIfEqualTo(newHead, null);
		}

		if (_UseCount)
		{
			Atomic::Interlocked_XINC(&m_Count);
		}
	}

	void	PopInto(_PType &retNode)	// pop from the tail
	{
		hh_u32						spinCount = 0;
		_NativePtrType volatile		tail;
		_NativePtrType volatile		next;
		{
			//HH_CRITICAL_SECTION_SCOPEDLOCK(m_HeadLock);
			//HH_SCOPEDPROFILESPIN("PopInto");
		_redo_tail:
			tail = m_Tail;
			next = (tail == null ? null : tail->m_Next);
			if (m_Tail.SetValueIfEqualTo(next, tail) != tail)
			{
				// Yield
				CCurrentThread::Yield(spinCount++);
				goto _redo_tail;
			}
		}

		if (tail != null && next == null)
		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_HeadLock);
			if (m_Head == tail)
				m_Head = null;
		}

		if (tail != null)
		{
			tail->m_Next = null;
			if (_UseCount)
				Atomic::Interlocked_XDEC(&m_Count);
		}
		TLocklessPtrHelper<_Type>::StoreNative(retNode, tail);
		return;
	}

	HH_FORCEINLINE _PType	Pop()	// pop from the tail
	{
		_PType	ret;
		PopInto(ret);
		return ret;
	}
};

#endif // 0

//----------------------------------------------------------------------------
//
//	Critical-Section based queue
//
//----------------------------------------------------------------------------

template<typename _Type, bool _UseCount = true>
class	TLockedQueue_CS
{
private:
protected:
	typedef	typename TLocklessPtrHelper<_Type>::NativePtrType	_NativePtrType;
	typedef typename TLocklessPtrHelper<_Type>::PtrType			_PType;

	_NativePtrType volatile		m_Head;
	_NativePtrType volatile		m_Tail;
	TAtomic<hh_u32> 			m_Count;
	Threads::CCriticalSection	m_Lock;

public:
	TLockedQueue_CS() : m_Head(null), m_Tail(null), m_Count(0) {}

	HH_FORCEINLINE hh_u32	Count() const { HH_ASSERT(_UseCount); return _UseCount ? hh_u32(m_Count) : 0; }
	HH_FORCEINLINE bool		Empty() const { return m_Tail != null; }

	void	PushBatch(const TMemoryView<_PType> &nodeBatch)
	{
		hh_u32				count = 0;
		_NativePtrType		batchHead = null;
		_NativePtrType		batchTail = null;
		for (hh_u32 nodei = 0; nodei < nodeBatch.Count(); ++nodei)
		{
			if (nodeBatch[nodei] != null)
			{
				TLocklessPtrHelper<_Type>::AddRefIFP(nodeBatch[nodei]);
				_NativePtrType	node = TLocklessPtrHelper<_Type>::ToNativePointer(nodeBatch[nodei]);
				HH_ASSERT(node->m_Next == null);
				if (batchHead != null)
					batchHead->m_Next = node;
				batchHead = node;
				if (batchTail == null)
					batchTail = node;
				if (_UseCount)
					++count;
			}
		}
		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
			if (m_Tail == null)
			{
				HH_ASSERT(m_Head == null);
				m_Tail = batchTail;
			}
			else
			{
				HH_ASSERT(m_Head != null);
				HH_ASSERT(m_Tail != null);
				m_Head->m_Next = batchTail;
			}
			m_Head = batchHead;
		}
		if (_UseCount)
			m_Count += count;
	}

	void	Push(_PType node)
	{
		HH_ASSERT(node->m_Next == null);
		TLocklessPtrHelper<_Type>::AddRefIFP(node);
		_NativePtrType	newHead = TLocklessPtrHelper<_Type>::ToNativePointer(node);
		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
			if (m_Tail == null)
			{
				HH_ASSERT(m_Head == null);
				m_Tail = newHead;
			}
			else
			{
				HH_ASSERT(m_Head != null);
				HH_ASSERT(m_Tail != null);
				m_Head->m_Next = newHead;
			}
			m_Head = newHead;
		}
		if (_UseCount)
			m_Count++;
	}

	void	PopInto(_PType &retNode)
	{
		// might need memory fence before touching m_Tail, better let the m_Lock do it
//		retNode = null;
//		if (m_Tail == null)
//			return;
		_NativePtrType		tail = null;
		{
			HH_CRITICAL_SECTION_SCOPEDLOCK(m_Lock);
			tail = m_Tail;
			m_Tail = (tail == null ? null : tail->m_Next);
			// m_Head could be dangling
			// HH_ONLY_IF_ASSERTS(
			if (m_Tail == null)
			{
				HH_ASSERT(m_Head == tail);
				m_Head = null;
			}
			//);
		}
		if (tail != null)
		{
			tail->m_Next = null;
			if (_UseCount)
				m_Count--;
		}

		TLocklessPtrHelper<_Type>::StoreNative(retNode, tail);
		return;
	}

	HH_FORCEINLINE _PType	Pop()	// pop from the tail
	{
		_PType	ret;
		PopInto(ret);
		return ret;
	}

	// for debug purposes
	void					_Debug_Lock() const { m_Lock.Lock(); }
	const _NativePtrType	_Debug_Tail() const { return m_Tail; }
	void					_Debug_Unlock() const { m_Lock.Unlock(); }

};

//----------------------------------------------------------------------------
//
//	element for the above lockless list containers
//
//----------------------------------------------------------------------------

template<typename _Type>
class	TLocklessListElement
{
private:
	typedef	typename TLocklessPtrHelper<_Type>::NativePtrType	_NativePtrType;
	typedef _Type	_PType;

public:
	TLocklessListElement() : m_Next(null) {}
	virtual ~TLocklessListElement() { HH_ASSERT(m_Next == null); }

	_NativePtrType volatile		m_Next;
};

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_CONTAINERS_LOCKLESS_H__
