#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2010/11/17 7:28 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_CALLBACKS_H__
#define	__KR_CALLBACKS_H__

#include "hh_kernel/include/kr_delegates.h"
#include "hh_kernel/include/kr_threads_basics.h"	// for Threads::CCriticalSection
#include "hh_kernel/include/kr_threads_rwlock.h"
#include "hh_kernel/include/kr_containers_array.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
//
// CFastDelegate_Slotable: slotable invalidable FastDelegate<> Wrapper
//
//----------------------------------------------------------------------------

template <typename _Signature>
class CFastDelegate_Slotable
{
public:
	typedef CFastDelegate_Slotable<_Signature>	SelfType;
	typedef FastDelegate<_Signature>			DelegateType;

	DelegateType				m_Delegate;

	CFastDelegate_Slotable() { }
	CFastDelegate_Slotable(const SelfType &other) : m_Delegate(other.m_Delegate) {}
	void		operator = (const SelfType &other) { m_Delegate = other.m_Delegate; }

	CFastDelegate_Slotable(const DelegateType &dg) : m_Delegate(dg) { }
	void		operator = (const DelegateType &dg) { m_Delegate = dg; }
	void		operator += (const DelegateType &dg) { m_Delegate += dg; }
	void		operator -= (const DelegateType &dg) { m_Delegate -= dg; }

	bool		operator == (const SelfType &other) const { return m_Delegate == other.m_Delegate; }
	bool		operator != (const SelfType &other) const { return m_Delegate != other.m_Delegate; }
	bool		operator == (const DelegateType &dg) const { return m_Delegate == dg; }
	bool		operator != (const DelegateType &dg) const { return m_Delegate != dg; }

	static const SelfType		Invalid;
	HH_FORCEINLINE bool			Valid() const { return m_Delegate != Invalid.m_Delegate; }
};

template <typename _Signature>
bool		operator == (const FastDelegate<_Signature> &dg, const CFastDelegate_Slotable<_Signature> &slot) { return slot.m_Delegate == dg; }
template <typename _Signature>
bool		operator != (const FastDelegate<_Signature> &dg, const CFastDelegate_Slotable<_Signature> &slot) { return slot.m_Delegate != dg; }

template <typename _Signature>
const CFastDelegate_Slotable<_Signature>	CFastDelegate_Slotable<_Signature>::Invalid = CFastDelegate_Slotable<_Signature>();

//----------------------------------------------------------------------------
//
// TCallbackChain_SafeIterator
// - safe same thread add/remove while cb'ing (*)
// - never execute a cb twice
// - never execute a removed cb (*)
// - executes added cb during cb'ing (even if inserted in the middle)
// - (*): IS NOT cb'ing thread-safe: during cb'ing, it could execute a cb removed by an other thread (tricky/impossible to fix !?)
//
//----------------------------------------------------------------------------

template <typename _SelfType>
class TCallbackChain_SafeIterator
{
public:
	typedef	_SelfType							SelfType;
	typedef typename SelfType::DelegateStorage	DelegateStorage;

private:
	// 16 on stack seems amplement sufficent, tested on a 16 mediums scene, it never exceeded 4 cbs ....
	enum { kOnStackCount = 16 };

	const SelfType				*m_Self;
	TSemiDynamicArray<DelegateStorage, kOnStackCount>		m_Executed;
	DelegateStorage				m_CurrCb;
	hh_u32						m_I;
	hh_u32						m_LastRemoveCounter;

public:
	TCallbackChain_SafeIterator(const SelfType *self)
	:	m_Self(self)
	,	m_I(0)
	{
		m_Self->m_Lock.LockRead();
		m_LastRemoveCounter = m_Self->m_RemoveCounter;
		HH_ONLY_IF_ASSERTS(const bool	reserveOk = )m_Executed.Reserve(m_Self->m_Callbacks.Count());
		HH_ASSERT(reserveOk);

		// Jordan: will *NOT crash*, but:
		// Why is there so many callbacks ?
		// Is it legit, or a corner-case ?
		// -> increase kOnStackCount OR comment the assert
		//HH_ASSERT(m_Self->m_Callbacks.Count() < kOnStackCount);

		m_Self->m_Lock.UnlockRead();
	}

	~TCallbackChain_SafeIterator() { }

	bool		Next()
	{
		m_CurrCb = DelegateStorage(); // .Clear()
		/*[LOCK]*/m_Self->m_Lock.LockRead();
		const hh_u32	count = m_Self->m_Callbacks.Count();
		
		// only check with m_Executed if somthing has been modified
		if (m_LastRemoveCounter != m_Self->m_RemoveCounter)
		{
			// performance assert : see above
			HH_PARANOID_ASSERT(m_Self->m_Callbacks.Count() < kOnStackCount);
			// something has been modified
			// HERE WE *MUST NOT*: m_LastRemoveCounter = m_Self->m_RemoveCounter;
			// from now on, we must check m_Executed every time -_-
			for (hh_u32 i = 0; i < count; ++i)
			{
				const DelegateStorage		&cb = m_Self->m_Callbacks[i];
				if (m_Executed.Contains(cb))
					continue;
				m_CurrCb = cb;
				HH_ASSERT(m_CurrCb.Valid());
				m_I = i;
				break;
			}
		}
		else if (m_I < count)
		{
			m_CurrCb = m_Self->m_Callbacks[m_I];
			HH_ASSERT(m_CurrCb.Valid());
		}
		/*[UNLOCK]*/m_Self->m_Lock.UnlockRead();
		if (!m_CurrCb.Valid())
			return false;
		HH_ONLY_IF_ASSERTS(const CGuid	pushedid = )m_Executed.PushBack(m_CurrCb);
		HH_ASSERT(pushedid.Valid());
		++m_I;
		return true;
	}

	HH_FORCEINLINE DelegateStorage		*operator->()
	{
		HH_ASSERT(m_CurrCb.Valid());
		return &m_CurrCb;
	}
};

#if 0
// "Old" Copy-Execute iterator
// BUG: can call removed cb during cb'ing
// BUG: do not call added cb during cb'ing
template <typename _SelfType>
class TCallbackChain_CopyIterator
{
public:
	typedef	_SelfType							SelfType;
	typedef typename SelfType::DelegateStorage	DelegateStorage;

private:
	const SelfType				*m_Self;
	TSemiDinamicArray<DelegateStorage, 16>		m_Copy;
	hh_u32						m_I;

public:
	TCallbackChain_CopyIterator(const SelfType *self)
	:	m_Self(self)
	,	m_I(0)
	{
		m_Self->m_Lock.LockRead();
		m_Copy = self->m_Callbacks;
		m_Self->m_Lock.UnlockRead();
	}

	~TCallbackChain_CopyIterator() { }

	bool		Next()
	{
		if (m_I >= m_Copy.Count())
			return false;
		++m_I;
		return true;
	}

	DelegateStorage		*operator->()
	{
		HH_ASSERT(m_I > 0 && (m_I - 1) < m_Copy.Count());
		return &(m_Copy[m_I - 1]);
	}
};
#endif

//----------------------------------------------------------------------------
//
// Ordered CallbackChain
//
//----------------------------------------------------------------------------

template<typename _Signature, bool _UniqueCallbacks, typename _AccessLocker>
class	TCallbackChainBase
{
public:
	typedef TCallbackChainBase<_Signature, _UniqueCallbacks, _AccessLocker>	SelfType;
protected:
	typedef CFastDelegate_Slotable<_Signature>		DelegateStorage;
	typedef TArray<DelegateStorage>					ChainType;

	ChainType				m_Callbacks;
	mutable _AccessLocker	m_Lock;
	volatile hh_u32			m_RemoveCounter;

public:
	typedef TCallbackChain_SafeIterator<SelfType>	Iterator;
	// friend class Iterator; // !!! friend class 'typedef' is NOT standard
	friend class TCallbackChain_SafeIterator<SelfType>;

	TCallbackChainBase() : m_RemoveCounter(0) { }

	bool					Empty() const { return m_Callbacks.Empty(); } // no need to lock, right ?

	SelfType				&operator += (const FastDelegate<_Signature> &callback)
	{
		if (!HH_VERIFY(callback != FastDelegate<_Signature>())) // MUST be valid for insertion
			return *this;
		m_Lock.LockWrite();
		if (!_UniqueCallbacks || !m_Callbacks.Contains(callback))
		{
			m_Callbacks.PushBack(callback);
		}
		m_Lock.UnlockWrite();
		return *this;
	}

	SelfType				&operator -= (const FastDelegate<_Signature> &callback)
	{
		HH_ASSERT(callback != FastDelegate<_Signature>()); // is valid
		m_Lock.LockWrite();
		const CGuid	id = m_Callbacks.IndexOfLast(callback);
		if (id.Valid())
		{
			m_Callbacks.Remove_AndKeepOrder(id);
			++m_RemoveCounter;
		}
		// HH_ASSERT(!m_Callbacks.IndexOfLast(callback).Valid());
		m_Lock.UnlockWrite();
		return *this;
	}

};

//----------------------------------------------------------------------------
// UN-ordered CallbackChain

template<typename _Signature, bool _UniqueCallbacks, typename _AccessLocker>
class	TCallbackChainUnorderedBase
{
public:
	typedef TCallbackChainUnorderedBase<_Signature, _UniqueCallbacks, _AccessLocker>	SelfType;
protected:
	typedef CFastDelegate_Slotable<_Signature>		DelegateStorage;
	typedef TArray<DelegateStorage>					ChainType;

	ChainType				m_Callbacks;
	mutable _AccessLocker	m_Lock;
	volatile hh_u32			m_RemoveCounter;

public:
	typedef TCallbackChain_SafeIterator<SelfType>	Iterator;
	// friend class Iterator; // !!! friend class 'typedef' is NOT standard
	friend class TCallbackChain_SafeIterator<SelfType>;

	TCallbackChainUnorderedBase() : m_RemoveCounter(0) { }

	bool					Empty() const { return m_Callbacks.Empty(); } // no need to lock, right ?

	SelfType				&operator += (const FastDelegate<_Signature> &callback)
	{
		if (!HH_VERIFY(callback != FastDelegate<_Signature>())) // MUST be valid for insertion
			return *this;
		m_Lock.LockWrite();
		if (!_UniqueCallbacks || !m_Callbacks.Contains(callback))
		{
			HH_ONLY_IF_RELEASE_ASSERTS(CGuid id = ) m_Callbacks.PushBack(callback);
			HH_RELEASE_ASSERT(id.Valid());
		}
		m_Lock.UnlockWrite();
		return *this;
	}
	SelfType				&operator -= (const FastDelegate<_Signature> &callback)
	{
		HH_ASSERT(callback != FastDelegate<_Signature>()); // is valid
		m_Lock.LockWrite();
		const CGuid	id = m_Callbacks.IndexOfLast(callback);
		HH_RELEASE_ASSERT(_UniqueCallbacks || id.Valid());
		if (id.Valid())
		{
			m_Callbacks.Remove(id);	// will swap with the last element
			++m_RemoveCounter;
		}
		//HH_ASSERT(!m_Callbacks.IndexOfLast(callback).Valid());
		m_Lock.UnlockWrite();
		return *this;
	}
};

//----------------------------------------------------------------------------
//
// Variadic parameters overloads CallbackChain front-ends
//
//----------------------------------------------------------------------------

template<typename _BaseClass, typename _Signature>
class	TCallbackChainImpl {};

template<typename _BaseClass, typename _TypeRet>
class	TCallbackChainImpl<_BaseClass, _TypeRet()> : public _BaseClass
{
public:
	_TypeRet	operator () () const
	{
		if (this->_BaseClass::Empty())
			return;
		typename _BaseClass::Iterator	it(this);
		while (it.Next())
			it->m_Delegate();
	}
};

template<typename _BaseClass, typename _TypeRet, typename _Type0>
class	TCallbackChainImpl<_BaseClass, _TypeRet(_Type0)> : public _BaseClass
{
public:
	_TypeRet	operator () (_Type0 a0) const
	{
		if (this->_BaseClass::Empty())
			return;
		typename _BaseClass::Iterator	it(this);
		while (it.Next())
			it->m_Delegate(a0);
	}
};

template<typename _BaseClass, typename _TypeRet, typename _Type0, typename _Type1>
class	TCallbackChainImpl<_BaseClass, _TypeRet(_Type0, _Type1)> : public _BaseClass
{
public:
	_TypeRet	operator () (_Type0 a0, _Type1 a1) const
	{
		if (this->_BaseClass::Empty())
			return;
		typename _BaseClass::Iterator	it(this);
		while (it.Next())
			it->m_Delegate(a0, a1);
	}
};

template<typename _BaseClass, typename _TypeRet, typename _Type0, typename _Type1, typename _Type2>
class	TCallbackChainImpl<_BaseClass, _TypeRet(_Type0, _Type1, _Type2)> : public _BaseClass
{
public:
	_TypeRet	operator () (_Type0 a0, _Type1 a1, _Type2 a2) const
	{
		if (this->_BaseClass::Empty())
			return;
		typename _BaseClass::Iterator	it(this);
		while (it.Next())
			it->m_Delegate(a0, a1, a2);
	}
};

template<typename _BaseClass, typename _TypeRet, typename _Type0, typename _Type1, typename _Type2, typename _Type3>
class	TCallbackChainImpl<_BaseClass, _TypeRet(_Type0, _Type1, _Type2, _Type3)> : public _BaseClass
{
public:
	_TypeRet	operator () (_Type0 a0, _Type1 a1, _Type2 a2, _Type3 a3) const
	{
		if (this->_BaseClass::Empty())
			return;
		typename _BaseClass::Iterator	it(this);
		while (it.Next())
			it->m_Delegate(a0, a1, a2, a3);
	}
};

//----------------------------------------------------------------------------
//
//	void locker for lightweight non-threadsafe versions of the callback chains
//
//----------------------------------------------------------------------------

class	CVoidLocker
{
public:
	static void	LockRead() {}
	static void	UnlockRead() {}
	static void	LockWrite() {}
	static void	UnlockWrite() {}
};

//----------------------------------------------------------------------------
//
//	lightweight, non-threadsafe callback chains
//
//----------------------------------------------------------------------------

template<typename _Signature>
class	TUnlockedCallbackChain : public TCallbackChainImpl<TCallbackChainBase<_Signature, false, CVoidLocker>, _Signature>
{
};

//----------------------------------------------------------------------------

template<typename _Signature>
class	TUnlockedCallbackChainUnordered : public TCallbackChainImpl<TCallbackChainUnorderedBase<_Signature, false, CVoidLocker>, _Signature>
{
};

//----------------------------------------------------------------------------
//
//	lock-protected threadsafe callback chains
//
//----------------------------------------------------------------------------

template<typename _Signature>
class	TCallbackChain : public TCallbackChainImpl<TCallbackChainBase<_Signature, false, Threads::CRWSpinLock>, _Signature>
{
};

//----------------------------------------------------------------------------

template<typename _Signature>
class	TCallbackChainUnordered : public TCallbackChainImpl<TCallbackChainUnorderedBase<_Signature, false, Threads::CRWSpinLock>, _Signature>
{
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_CALLBACKS_H__
