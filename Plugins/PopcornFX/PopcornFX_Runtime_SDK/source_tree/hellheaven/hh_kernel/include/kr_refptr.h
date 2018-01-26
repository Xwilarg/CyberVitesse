#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2007/10/24 16:00 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_REFPTR_H__
#define __KR_REFPTR_H__

#include "hh_kernel/include/hh_kernel_config.h"
#include "hh_kernel/include/kr_threads_atomic.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

//#ifdef	HH_WIN32
#define	KR_REFPTR_USE_ATOMIC_REFCOUNTS
//#endif
//#define	KR_REFPTR_AVOID_ATOMIC_OPS_IFP

// enable to help profiling ref counters
//#define	KR_REFPTR_USE_NON_INLINE_REF_CALLS

//----------------------------------------------------------------------------

#if	(KR_REFPTR_UBER_DEBUG != 0)
struct	SRefCountDebugTracker;
#	define	ONLY_IN_UBER_REFPTR_DEBUG(__expr)			__expr
#	if (KR_REFPTR_UBER_DEBUG_ALL_REFPOINTERS != 0)
#		define ONLY_IN_UBER_REFPTR_DEBUG_ALL(__expr)	__expr
#	else
#		define	ONLY_IN_UBER_REFPTR_DEBUG_ALL(__expr)
#	endif
#else
#	define	ONLY_IN_UBER_REFPTR_DEBUG(__expr)
#endif

//----------------------------------------------------------------------------

#define	HH_DECLARE_REFPTRCLASS(__classNameWithoutPrefix) \
	typedef PopcornFX::TRefPtr<C ## __classNameWithoutPrefix>			P ## __classNameWithoutPrefix; \
	typedef PopcornFX::TWeakPtr<C ## __classNameWithoutPrefix>			W ## __classNameWithoutPrefix; \
	typedef PopcornFX::TRefPtr<const C ## __classNameWithoutPrefix>		PC ## __classNameWithoutPrefix; \
	typedef PopcornFX::TWeakPtr<const C ## __classNameWithoutPrefix>	WC ## __classNameWithoutPrefix;

#define	HH_DECLARE_REFPTRINTERFACE(__classNameWithoutPrefix) \
	typedef PopcornFX::TRefPtr<I ## __classNameWithoutPrefix>			P ## __classNameWithoutPrefix; \
	typedef PopcornFX::TWeakPtr<I ## __classNameWithoutPrefix>			W ## __classNameWithoutPrefix; \
	typedef PopcornFX::TRefPtr<const I ## __classNameWithoutPrefix>		PC ## __classNameWithoutPrefix; \
	typedef PopcornFX::TWeakPtr<const I ## __classNameWithoutPrefix>	WC ## __classNameWithoutPrefix;

#define	HH_FORWARD_DECLARE_EXPORTED(__classNameWithoutPrefix, __export) \
	class __export C ## __classNameWithoutPrefix; \
	HH_DECLARE_REFPTRCLASS(__classNameWithoutPrefix)
#define	HH_FORWARD_DECLARE_EXPORTED_INTERFACE(__classNameWithoutPrefix, __export) \
	class __export I ## __classNameWithoutPrefix; \
	HH_DECLARE_REFPTRINTERFACE(__classNameWithoutPrefix)

#define	HH_FORWARD_DECLARE(__what)				HH_FORWARD_DECLARE_EXPORTED(__what, NOTHING)
#define	HH_FORWARD_DECLARE_INTERFACE(__what)	HH_FORWARD_DECLARE_EXPORTED_INTERFACE(__what, NOTHING)

template<typename _Type>
class	 TRefPtr;
template<typename _Type>
class	 TWeakPtr;

//----------------------------------------------------------------------------

template<typename _Type, bool _IsWeak2>
struct	TRefModifier
{
	typedef	typename TType_RemoveConstness<_Type>::Type	_WriteableType;

	_Type								*m_Ptr;
	ONLY_IN_UBER_REFPTR_DEBUG(hh_u32	m_RefTrackingSlot;)

	HH_FORCEINLINE void	AddRef()
	{
		if (m_Ptr != null)
		{
			ONLY_IN_UBER_REFPTR_DEBUG(m_RefTrackingSlot = ) const_cast<_WriteableType*>(m_Ptr)->AddReference(ONLY_IN_UBER_REFPTR_DEBUG(true));
		}
	}
	HH_FORCEINLINE void	DelRef()
	{
		if (m_Ptr != null)
		{
			_WriteableType::RemoveReference(const_cast<_WriteableType*>(m_Ptr) ONLY_IN_UBER_REFPTR_DEBUG(COMMA m_RefTrackingSlot));
			ONLY_IN_UBER_REFPTR_DEBUG(m_RefTrackingSlot = hh_u32(-1));
		}
	}
};

//----------------------------------------------------------------------------

template<typename _Type>
struct	TRefModifier<_Type, true>
{
	typedef	typename TType_RemoveConstness<_Type>::Type	_WriteableType;

	_Type								*m_Ptr;
	ONLY_IN_UBER_REFPTR_DEBUG(hh_u32	m_RefTrackingSlot;)

	HH_FORCEINLINE void	AddRef()
	{
		if (m_Ptr != null)
		{
			ONLY_IN_UBER_REFPTR_DEBUG(m_RefTrackingSlot = ) const_cast<_WriteableType*>(m_Ptr)->AddWeakReference(ONLY_IN_UBER_REFPTR_DEBUG(true));
		}
	}
	HH_FORCEINLINE void	DelRef()
	{
		if (m_Ptr != null)
		{
			const_cast<_WriteableType*>(m_Ptr)->RemoveWeakReference(ONLY_IN_UBER_REFPTR_DEBUG(m_RefTrackingSlot));
			ONLY_IN_UBER_REFPTR_DEBUG(m_RefTrackingSlot = hh_u32(-1));
		}
	}
};

//----------------------------------------------------------------------------

template<typename _Type, bool _IsWeak>
class	TBaseRefPtr : protected TRefModifier<_Type, _IsWeak>
{
public:
	typedef _Type							Type;
	static const bool						IsWeak = _IsWeak;
private:
	friend class 							TWeakPtr<_Type>;
	friend class 							TRefPtr<_Type>;
	friend class							TBaseRefPtr<_Type, !_IsWeak>;	// freely access our soulmate
private:
	typedef TBaseRefPtr<_Type, _IsWeak>		SelfType;
	typedef TRefModifier<_Type, _IsWeak>	_P;

protected:
	template<typename _Type2>
	void	Copy(_Type2 *ptr)
	{
		if (ptr != _P::m_Ptr)
		{
			SelfType	other(ptr);
			Swap(other);
		}
	}

public:
	TBaseRefPtr() { _P::m_Ptr = null; ONLY_IN_UBER_REFPTR_DEBUG(_P::m_RefTrackingSlot = hh_u32(-1)); }
	TBaseRefPtr(_Type *ptr) { _P::m_Ptr = ptr; _P::AddRef(); }
	template<typename _Type2>
	TBaseRefPtr(_Type2 *ptr) { _P::m_Ptr = ptr; _P::AddRef(); }

	~TBaseRefPtr() { _P::DelRef(); }

	HH_FORCEINLINE bool	operator == (_Type *ptr) const { return ptr == _P::m_Ptr; }
	HH_FORCEINLINE bool	operator == (const TBaseRefPtr<_Type, _IsWeak> &ptr) const { return ptr.m_Ptr == _P::m_Ptr; }
	template<typename _Type2>
	HH_FORCEINLINE bool	operator == (_Type2 *ptr) const { return ptr == _P::m_Ptr; }
	template<typename _Type2, bool _IsWeak2>
	HH_FORCEINLINE bool	operator == (const TBaseRefPtr<_Type2, _IsWeak2> &ptr) const { return ptr.Get() == _P::m_Ptr; }

	HH_FORCEINLINE bool	operator != (_Type *ptr) const { return ptr != _P::m_Ptr; }
	HH_FORCEINLINE bool	operator != (const TBaseRefPtr<_Type, _IsWeak> &ptr) const { return ptr.m_Ptr != _P::m_Ptr; }
	template<typename _Type2>
	HH_FORCEINLINE bool	operator != (_Type2 *ptr) const { return ptr != _P::m_Ptr; }
	template<typename _Type2, bool _IsWeak2>
	HH_FORCEINLINE bool	operator != (const TBaseRefPtr<_Type2, _IsWeak2> &ptr) const { return ptr.Get() != _P::m_Ptr; }

	HH_FORCEINLINE _Type	&operator * () const
	{
		HH_ASSERT(_P::m_Ptr != null);
		return *_P::m_Ptr;
	}
	HH_FORCEINLINE _Type	*operator -> () const
	{
		HH_ASSERT(_P::m_Ptr != null);
		return _P::m_Ptr;
	}
	HH_FORCEINLINE _Type	*Get() const
	{
		return _P::m_Ptr;
	}

	// WARNING ! THIS MUST BE USED WITH THE UNTRACKED REFPTR ADD.
	template<typename _Type2>
	HH_FORCEINLINE void		UnsafeSetPtrFromManuallyRefIncrementedRawPointer(_Type2 *ptr)	// insanely long name. on purpose.
	{
		_P::DelRef();
		_P::m_Ptr = ptr;
	}

	HH_FORCEINLINE void		Swap(TBaseRefPtr<_Type, _IsWeak> &other)
	{
		HHSwap(_P::m_Ptr, other.m_Ptr);
		ONLY_IN_UBER_REFPTR_DEBUG
		(
			HHSwap(_P::m_RefTrackingSlot, other.m_RefTrackingSlot)
		);
	}
};

//----------------------------------------------------------------------------
// Simple wrapper for TBaseRefPtr<_Type, false> (Strong pointers)

template<typename _Type>
class	TRefPtr : public TBaseRefPtr<_Type, false>
{
private:
	typedef TBaseRefPtr<_Type, false>	BaseRefPtr;
public:
	TRefPtr() {}
	TRefPtr(_Type *ptr) : TBaseRefPtr<_Type, false>(ptr) {}
	TRefPtr(const TRefPtr<_Type> &rptr) : TBaseRefPtr<_Type, false>(rptr.Get()) {}

	template<typename _Type2>
	TRefPtr(_Type2 *ptr) : TBaseRefPtr<_Type, false>(ptr) {}
	template<typename _Type2, bool _IsWeak2>
	TRefPtr(const TBaseRefPtr<_Type2, _IsWeak2> &rptr) : TBaseRefPtr<_Type, false>(rptr.Get()) {}
	~TRefPtr() {}


	TRefPtr<_Type>	&operator = (_Type *assign) { BaseRefPtr::template Copy<_Type>(assign); return *this; }
	TRefPtr<_Type>	&operator = (const TRefPtr<_Type> &assign) { BaseRefPtr::template Copy<_Type>(assign.m_Ptr); return *this; }

	template<typename _Type2>
	TRefPtr<_Type>	&operator = (_Type2 *assign) { BaseRefPtr::template Copy<_Type2>(assign); return *this; }
	template<typename _Type2, bool _IsWeak2>
	TRefPtr<_Type>	&operator = (const TBaseRefPtr<_Type2, _IsWeak2> &assign) { BaseRefPtr::template Copy<_Type2>(assign.Get()); return *this; }
};

//----------------------------------------------------------------------------
// Simple wrapper TBaseRefPtr<_Type, true> (Weak pointers)

template<typename _Type>
class	TWeakPtr : public TBaseRefPtr<_Type, true>
{
private:
	typedef TBaseRefPtr<_Type, true>	BaseRefPtr;
public:
	TWeakPtr() {}
	TWeakPtr(_Type *ptr) : TBaseRefPtr<_Type, true>(ptr) {}
	TWeakPtr(const TWeakPtr<_Type> &rptr) : TBaseRefPtr<_Type, true>(rptr.Get()) {}

	template<typename _Type2>
	TWeakPtr(_Type2 *ptr) : TBaseRefPtr<_Type, true>(ptr) {}
	template<typename _Type2, bool _IsWeak2>
	TWeakPtr(const TBaseRefPtr<_Type2, _IsWeak2> &rptr) : TBaseRefPtr<_Type, true>(rptr.Get()) {}
	~TWeakPtr() {}


	TWeakPtr<_Type>	&operator = (_Type *assign) { BaseRefPtr::template Copy<_Type>(assign); return *this; }
	TWeakPtr<_Type>	&operator = (const TWeakPtr<_Type> &assign) { BaseRefPtr::template Copy<_Type>(assign.Get()); return *this; }

	template<typename _Type2>
	TWeakPtr<_Type>	&operator = (_Type2 *assign) { BaseRefPtr::template Copy<_Type2>(assign); return *this; }
	template<typename _Type2, bool _IsWeak2>
	TWeakPtr<_Type>	&operator = (const TBaseRefPtr<_Type2, _IsWeak2> &assign) { BaseRefPtr::template Copy<_Type2>(assign.Get()); return *this; }
};

//----------------------------------------------------------------------------
// global compare operators to allow 'ptr == refptr' expressions

template<typename _Type, bool _IsWeak, typename _PtrType>
HH_FORCEINLINE bool	operator == (_PtrType *ptr, const TBaseRefPtr<_Type, _IsWeak> &refptr) { return refptr == ptr; }
template<typename _Type, bool _IsWeak, typename _PtrType>
HH_FORCEINLINE bool	operator != (_PtrType *ptr, const TBaseRefPtr<_Type, _IsWeak> &refptr) { return refptr != ptr; }

//----------------------------------------------------------------------------
//
//	Intrusive refcount objects
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT HH_NOVTABLE CRefCountedObjectBase : public CNonCopyable
{
private:
#ifdef	KR_REFPTR_USE_ATOMIC_REFCOUNTS
	mutable TAtomic<hh_u32>	m_RefCount;
	mutable TAtomic<hh_u32>	m_WeakCount;
#else
	mutable volatile hh_u32	m_RefCount;
	mutable volatile hh_u32	m_WeakCount;
#endif

protected:
#if	(KR_REFPTR_UBER_DEBUG != 0)
private:
	SRefCountDebugTracker	*m_DebugRefTracker;
	void					_StartTrackingIFN();
	void					_StopTrackingIFN();
protected:
	hh_u32					_InternalAddDebugRef() const;
	hh_u32					_InternalAddDebugWeakRef() const;
	void					_InternalRemoveDebugRef(hh_u32 trackingSlot) const;
	void					_InternalRemoveDebugWeakRef(hh_u32 trackingSlot) const;

	virtual hh_u32			VirtualAddDebugRef() const { CGuid id; ONLY_IN_UBER_REFPTR_DEBUG_ALL(id = _InternalAddDebugRef()); return id; }
	virtual hh_u32			VirtualAddDebugWeakRef() const { CGuid id; ONLY_IN_UBER_REFPTR_DEBUG_ALL(id = _InternalAddDebugWeakRef()); return id; }
	virtual void			VirtualRemoveDebugRef(hh_u32 trackingSlot) const { ONLY_IN_UBER_REFPTR_DEBUG_ALL(_InternalRemoveDebugRef(trackingSlot)); }
	virtual void			VirtualRemoveDebugWeakRef(hh_u32 trackingSlot) const { ONLY_IN_UBER_REFPTR_DEBUG_ALL(_InternalRemoveDebugWeakRef(trackingSlot)); }
#endif

#ifdef	KR_REFPTR_USE_NON_INLINE_REF_CALLS
#	if	defined(HH_COMPILER_SNC)
#		define	REFCALLS_INLINE
#	else
#		define	REFCALLS_INLINE	HH_NOINLINE
#	endif
#else
#	define	REFCALLS_INLINE	HH_FORCEINLINE
#endif

#if (KR_REFPTR_UBER_DEBUG != 0)
#	define	ADDREF_RETURN_TYPE	hh_u32
#else
#	define	ADDREF_RETURN_TYPE	void
#endif

	// note: the tricks below assume that natural pointers will be kept inside the same thread before the first feed into a refptr.
	// the object must be refcounted before going multithread.
	// this requirement allows us to avoid expensive interlocked ops for refcounted objects that have a single ref.
	// these must be const, the refcounts are mutable, and we want to use them transparently in const methods
	REFCALLS_INLINE void	_InternalAddStrongRef() const
	{
#if	defined(KR_REFPTR_USE_ATOMIC_REFCOUNTS) && defined(KR_REFPTR_AVOID_ATOMIC_OPS_IFP)
		if (m_RefCount._UnsafeGetValue() != 0)
			++m_RefCount;
		else
			m_RefCount._UnsafeSetValue(1);
#else
		++m_RefCount;
#endif
	}
	REFCALLS_INLINE void	_InternalAddWeakRef() const
	{
#if	defined(KR_REFPTR_USE_ATOMIC_REFCOUNTS) && defined(KR_REFPTR_AVOID_ATOMIC_OPS_IFP)
		if (m_WeakCount._UnsafeGetValue() != 0)
			++m_WeakCount;
		else
			m_WeakCount._UnsafeSetValue(1);
#else
		++m_WeakCount;
#endif
	}
	// these return the new value
	REFCALLS_INLINE hh_u32	_InternalRemoveStrongRef() const
	{
#if	defined(KR_REFPTR_USE_ATOMIC_REFCOUNTS) && defined(KR_REFPTR_AVOID_ATOMIC_OPS_IFP)
		if (m_RefCount._UnsafeGetValue() != 1)
			return --m_RefCount;

		m_RefCount._UnsafeSetValue(0);
		return 0;
#else
		return --m_RefCount;
#endif
	}
	REFCALLS_INLINE hh_u32	_InternalRemoveWeakRef() const
	{
#if	defined(KR_REFPTR_USE_ATOMIC_REFCOUNTS) && defined(KR_REFPTR_AVOID_ATOMIC_OPS_IFP)
		if (m_WeakCount._UnsafeGetValue() != 1)
			return --m_WeakCount;

		m_WeakCount._UnsafeSetValue(0);
		return 0;
#else
		return --m_WeakCount;
#endif
	}

public:
	// adding weak references may take advantage of the refcount's mutability
	HH_FORCEINLINE ADDREF_RETURN_TYPE	AddWeakReference(ONLY_IN_UBER_REFPTR_DEBUG(bool trackRefs = false)) const
	{
		_InternalAddWeakRef();
		ONLY_IN_UBER_REFPTR_DEBUG
		(
			if (trackRefs)
				return VirtualAddDebugWeakRef();
			return hh_u32(-1);
		);
	}

	HH_FORCEINLINE void					RemoveWeakReference(ONLY_IN_UBER_REFPTR_DEBUG(hh_u32 trackingSlot)) const
	{
		ONLY_IN_UBER_REFPTR_DEBUG(VirtualRemoveDebugWeakRef(trackingSlot));
		HH_ONLY_IF_ASSERTS(const hh_u32 nrc =) _InternalRemoveWeakRef();
		HH_ASSERT_MESSAGE(hh_i32(nrc) >= 0, "/!\\ weak refcount underflow /!\\");
	}

	HH_FORCEINLINE ADDREF_RETURN_TYPE	AddReference(ONLY_IN_UBER_REFPTR_DEBUG(bool trackRefs = false)) const
	{
		_InternalAddStrongRef();
		ONLY_IN_UBER_REFPTR_DEBUG
		(
			if (trackRefs)
				return VirtualAddDebugRef();
			return hh_u32(-1);
		);
	}

	// RemoveReference intentionally does not exists in CRefCountedObjectBase
	// must exists in child classes (CRefCountedObject, CRefCountedObjectWithDestructionNotification, ...)
	// static void						RemoveReference(_Type *realObject ONLY_IN_UBER_REFPTR_DEBUG(COMMA hh_u32 trackingSlot))

protected:
	HH_FORCEINLINE hh_u32				_InternalRemoveReference(ONLY_IN_UBER_REFPTR_DEBUG(COMMA hh_u32 trackingSlot)) const
	{
		ONLY_IN_UBER_REFPTR_DEBUG(VirtualRemoveDebugRef(trackingSlot));
		const hh_u32	nrc = _InternalRemoveStrongRef();
		HH_ASSERT_MESSAGE(hh_i32(nrc) >= 0, "/!\\ strong refcount underflow /!\\");
		return nrc;
	}

#undef	REFCALLS_INLINE
#undef	ADDREF_RETURN_TYPE

public:
	CRefCountedObjectBase() : m_RefCount(0), m_WeakCount(0) ONLY_IN_UBER_REFPTR_DEBUG(COMMA m_DebugRefTracker(null)) { ONLY_IN_UBER_REFPTR_DEBUG(_StartTrackingIFN()); }

#if	(HH_ASSERTS_IN_DEBUG != 0)
	virtual ~CRefCountedObjectBase(); // avoid inline asserts about ref count leak
#else
	virtual ~CRefCountedObjectBase() { ONLY_IN_UBER_REFPTR_DEBUG(_StopTrackingIFN()); }
#endif

	HH_FORCEINLINE hh_u32	RefCount() const
	{
#if	defined(KR_REFPTR_USE_ATOMIC_REFCOUNTS)
		return m_RefCount._UnsafeGetValue();
#else
		return m_RefCount;
#endif
	}
	HH_FORCEINLINE hh_u32	WeakCount() const
	{
#if	defined(KR_REFPTR_USE_ATOMIC_REFCOUNTS)
		return m_WeakCount._UnsafeGetValue();
#else
		return m_WeakCount;
#endif
	}

	char					*TrackedDebugRefsToString() const;	// the returned char* should be manually freed by passing it to 'HH_FREE'. returns null if no refs or debug tracking is inactive
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT HH_NOVTABLE CRefCountedObject : public CRefCountedObjectBase
{
private:
	// originally forceinlined, but to avoid code-size explosion
	static void		_DeleteObject(CRefCountedObject *base, void *ptrToFree ONLY_IN_UBER_REFPTR_DEBUG(COMMA hh_u32 trackingSlot));

public:
	// WARNING! we need a templated type to be able to free the real pointer. do NOT make this a member that free 'this' !
	// freeing 'this' would work for simple single-inheritance trees, but not when the refcounted intrusive block is a second or third branche in a multiple-inheritance tree.
	template<typename _Type>
	HH_FORCEINLINE static void			RemoveReference(_Type *realObject ONLY_IN_UBER_REFPTR_DEBUG(COMMA hh_u32 trackingSlot))
	{
		hh_u32	nrc = realObject->_InternalRemoveReference(ONLY_IN_UBER_REFPTR_DEBUG(trackingSlot));
		if (nrc == 0)
		{
			_DeleteObject(realObject, realObject ONLY_IN_UBER_REFPTR_DEBUG(COMMA trackingSlot));
		}
	}
};

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT HH_NOVTABLE CRefCountedObjectWithDestructionNotification : public CRefCountedObjectBase
{
protected:
	virtual void	_OnRefptrDestruct() = 0;

public:
	// this class is not used to free the ptr, so no need for template
	HH_FORCEINLINE static void			RemoveReference(CRefCountedObjectWithDestructionNotification *realObject ONLY_IN_UBER_REFPTR_DEBUG(COMMA hh_u32 trackingSlot))
	{
		hh_u32	nrc = realObject->_InternalRemoveReference(ONLY_IN_UBER_REFPTR_DEBUG(trackingSlot));
		if (nrc == 0)
		{
			realObject->_OnRefptrDestruct();
		}
	}
};

//----------------------------------------------------------------------------
//
//	usage example:
//
//	// #define	DEBUG_PIG_REFS
//	#ifdef	DEBUG_PIG_REFS
//	#define	PIG_REFCOUNT_DEBUG_FLAG		true
//	#else
//	#define	PIG_REFCOUNT_DEBUG_FLAG		false
//	#endif
//
//	...
//
//	KR_REFPTR_DECLARE_SPECIALIZED_REFCOUNTEDOBJECT(Pig, PIG_REFCOUNT_DEBUG_FLAG);	// declares the specialized ref storage class
//
//	...
//
//	class	CTastyBacon : public HH_REFCOUNTEDOBJECT(Pig)
//	{
//	public:
//		void	Eat();
//	}
//
//	Now, to enable refcount tracking just for 'Pig'-type objects, just uncomment the 'DEBUG_PIG_REFS' macro.
//
//----------------------------------------------------------------------------

#define	HH_REFCOUNTEDOBJECT(__name)									CRefCountedObjectCustom ## __name
#define	HH_REFCOUNTEDOBJECTWITHDESTRUCTIONNOTIFICATION(__name)		CRefCountedObjectCustomWithDestructionNotification ## __name

#if	((KR_REFPTR_UBER_DEBUG != 0) && (KR_REFPTR_UBER_DEBUG_ALL_REFPOINTERS == 0))	// if we want to debug all refpointers, use the default virtual functions

#define	KR_REFPTR_DECLARE_SPECIALIZED_REFCOUNTEDOBJECT(__name, __enabled) \
	class	HH_REFCOUNTEDOBJECT(__name) : public PopcornFX::CRefCountedObject \
	{ \
	private:	/* seal them */ \
		friend class	CRefCountedObject; \
		virtual PopcornFX::hh_u32	VirtualAddDebugRef() const { if (__enabled) return _InternalAddDebugRef(); return 0; } \
		virtual PopcornFX::hh_u32	VirtualAddDebugWeakRef() const { if (__enabled) return _InternalAddDebugWeakRef(); return 0; } \
		virtual void	VirtualRemoveDebugRef(PopcornFX::hh_u32 trackingSlot) const { if (__enabled) _InternalRemoveDebugRef(trackingSlot); } \
		virtual void	VirtualRemoveDebugWeakRef(PopcornFX::hh_u32 trackingSlot) const { if (__enabled) _InternalRemoveDebugWeakRef(trackingSlot); } \
	public: \
	}

#define	KR_REFPTR_DECLARE_SPECIALIZED_REFCOUNTEDOBJECTWITHDESTRUCTIONNOTIFICATION(__name, __enabled) \
	class	HH_REFCOUNTEDOBJECTWITHDESTRUCTIONNOTIFICATION(__name) : public PopcornFX::CRefCountedObjectWithDestructionNotification \
	{ \
	private:	/* seal them */ \
		virtual PopcornFX::hh_u32	VirtualAddDebugRef() const { if (__enabled) return _InternalAddDebugRef(); return 0; } \
		virtual PopcornFX::hh_u32	VirtualAddDebugWeakRef() const { if (__enabled) return _InternalAddDebugWeakRef(); return 0; } \
		virtual void	VirtualRemoveDebugRef(PopcornFX::hh_u32 trackingSlot) const { if (__enabled) _InternalRemoveDebugRef(trackingSlot); } \
		virtual void	VirtualRemoveDebugWeakRef(PopcornFX::hh_u32 trackingSlot) const { if (__enabled) _InternalRemoveDebugWeakRef(trackingSlot); } \
	public: \
	}

#else

#define	KR_REFPTR_DECLARE_SPECIALIZED_REFCOUNTEDOBJECT(__name, __enabled) \
	class	HH_REFCOUNTEDOBJECT(__name) : public PopcornFX::CRefCountedObject \
	{ \
	}

#define	KR_REFPTR_DECLARE_SPECIALIZED_REFCOUNTEDOBJECTWITHDESTRUCTIONNOTIFICATION(__name, __enabled) \
	class	HH_REFCOUNTEDOBJECTWITHDESTRUCTIONNOTIFICATION(__name) : public PopcornFX::CRefCountedObject \
	{ \
	}

#endif

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------
// FIXME: vieux et degeulasse, a cleaner

template<typename _Type, bool _IsWeak>
void	HHSwap(TBaseRefPtr<_Type, _IsWeak> &refptr0, TBaseRefPtr<_Type, _IsWeak> &refptr1)
{
	refptr0.Swap(refptr1);
}

template<typename _Type>
void	HHSwap(TRefPtr<_Type> &refptr0, TRefPtr<_Type> &refptr1)
{
	refptr0.Swap(refptr1);
}

template<typename _Type>
void	HHSwap(TWeakPtr<_Type> &refptr0, TWeakPtr<_Type> &refptr1)
{
	refptr0.Swap(refptr1);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_REFPTR_H__
