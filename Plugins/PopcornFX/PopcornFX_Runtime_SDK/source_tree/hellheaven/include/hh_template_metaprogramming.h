#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2008/01/02 8:28 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__HH_TEMPLATE_METAPROGRAMMING_H__
#define	__HH_TEMPLATE_METAPROGRAMMING_H__

//----------------------------------------------------------------------------

#ifndef	__HELLHEAVEN_H_INTERNAL__
#	error	hh_template_metaprogramming.h must not be included outside hellheaven.h
#endif

//----------------------------------------------------------------------------

template<typename _Type, _Type _Val>
struct	TTypeConstant
{
	typedef TTypeConstant<_Type, _Val>	SelfType;
	static const _Type					Value = _Val;
	typedef _Type						ValueType;
};
template<typename _Type, _Type _Val>	const _Type	TTypeConstant<_Type, _Val>::Value;

//----------------------------------------------------------------------------
// we could have used TTypeConstant<bool, true/false>, but this is just for ease of use/reading, to be able to use ::True or ::False instead of ::Value / ::Value == true/false

template<bool _Val>
struct	TTypeBoolConstant : TTypeConstant<bool, _Val>
{
	static const bool	True = _Val;
	static const bool	False = !_Val;
};
template<bool _Val>	const bool	TTypeBoolConstant<_Val>::True;
template<bool _Val>	const bool	TTypeBoolConstant<_Val>::False;

typedef TTypeBoolConstant<true>		TrueType;
typedef TTypeBoolConstant<false>	FalseType;

//----------------------------------------------------------------------------

template<typename _Type>	struct			TType_RemoveConstness { typedef _Type Type; };
template<typename _Type>	struct			TType_RemoveConstness<_Type const> { typedef _Type Type; };

template<typename _Type>	struct			TType_RemoveVolatile { typedef _Type Type; };
template<typename _Type>	struct			TType_RemoveVolatile<_Type volatile> { typedef _Type Type; };

template<typename _Type>	struct			TType_RemovePointer { typedef _Type Type; };
template<typename _Type>	struct			TType_RemovePointer<_Type*> { typedef _Type Type; };

template<typename _Type>	struct			TType_RemoveReference { typedef _Type Type; };
template<typename _Type>	struct			TType_RemoveReference<_Type&> { typedef _Type Type; };

template<typename _Type>	struct			TType_StripQual { typedef _Type Type; };
template<typename _Type>	struct			TType_StripQual<_Type const> : TType_StripQual<_Type>{};
template<typename _Type>	struct			TType_StripQual<_Type volatile> : TType_StripQual<_Type>{};

template<typename _Type>	struct			TType_StripDecorations { typedef _Type Type; };
template<typename _Type>	struct			TType_StripDecorations<_Type const> : TType_StripDecorations<_Type> {};
template<typename _Type>	struct			TType_StripDecorations<_Type volatile> : TType_StripDecorations<_Type> {};
template<typename _Type, int _Size>	struct	TType_StripDecorations<_Type[_Size]> : TType_StripDecorations<_Type> {};

template<typename _Type>	struct			TType_Pure : TType_StripDecorations<_Type> {};
template<typename _Type>	struct			TType_Pure<_Type*> : TType_Pure<_Type> {};
template<typename _Type>	struct			TType_Pure<_Type&> : TType_Pure<_Type> {};

//----------------------------------------------------------------------------

template<typename _Type>	struct			TType_BuildConst { typedef const _Type Type; };
//template<typename _Type>	struct			TType_BuildConst<_Type*> { typedef _Type const *Type; };
template<typename _Type>	struct			TType_BuildConst<_Type const> : TType_BuildConst<_Type> {};

template<typename _Type, typename _TransferTo>	struct	TType_TransferConstness { typedef _TransferTo Type; };
template<typename _Type, typename _TransferTo>	struct	TType_TransferConstness<_Type const, _TransferTo> : TType_BuildConst<_TransferTo> {};

template<typename _Type, bool _ShouldBeConst>	struct	TType_MakeConstIf { typedef _Type Type; };
template<typename _Type>						struct	TType_MakeConstIf<_Type, true> : TType_BuildConst<_Type> {};

//----------------------------------------------------------------------------
//
//	Type Traits
//
//----------------------------------------------------------------------------

namespace	TTypeTraits
{
	template<typename _Type>	struct	IsInteger						: FalseType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<bool> 				: TrueType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<char> 				: TrueType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<signed char>			: TrueType	{ static const bool	Signed = true; };	// explicit 'signed', in case it's changed in the compiler settings
	template<>					struct	IsInteger<unsigned char>		: TrueType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<short>				: TrueType	{ static const bool	Signed = true; };
	template<>					struct	IsInteger<unsigned short>		: TrueType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<int>					: TrueType	{ static const bool	Signed = true; };
	template<>					struct	IsInteger<unsigned int>			: TrueType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<long>					: TrueType	{ static const bool	Signed = true; };
	template<>					struct	IsInteger<unsigned long>		: TrueType	{ static const bool	Signed = false; };
	template<>					struct	IsInteger<long long>			: TrueType	{ static const bool	Signed = true; };
	template<>					struct	IsInteger<unsigned long long>	: TrueType	{ static const bool	Signed = false; };
	// FIXME: we should make sure we're not missing any hh-specific datatypes here... (hh_*, like hh_u32, hh_u64, hh_u16... they should map to the above-specialized templates, but.. make sure it actually does...)

	template<typename _Type>	struct	IsFloat			: FalseType {};
	template<>					struct	IsFloat<float>	: TrueType {};
	template<>					struct	IsFloat<double>	: TrueType {};

	template<typename _Type>	struct	IsSigned : TTypeBoolConstant<IsInteger<_Type>::Signed || IsFloat<_Type>::True> {};

	template<typename _Type>	struct	IsNumeric : TTypeBoolConstant<IsInteger<_Type>::True || IsFloat<_Type>::True> {};

	template<typename _Type>	struct	IsPointer : FalseType {};
	template<typename _Type>	struct	IsPointer<_Type*> : TrueType {};

	template<typename _Type>	struct	IsReference : FalseType {};
	template<typename _Type>	struct	IsReference<_Type&> : TrueType {};

	template<typename _Type>	struct	IsConst : FalseType {};
	template<typename _Type>	struct	IsConst<_Type const> : TrueType {};

	template<typename _Type>	struct	IsVolatile : FalseType {};
	template<typename _Type>	struct	IsVolatile<_Type volatile> : TrueType {};

	template<typename _Type>	struct	IsPOD : TTypeBoolConstant<IsPointer<_Type>::True || IsInteger<_Type>::True || IsFloat<_Type>::True> {};
	template<typename _Type>	struct	IsPOD<_Type const> : IsPOD<_Type> {};	// needed if we want const pointers to work

	template<typename _Type>	struct	IsRelocatable : TrueType {};	// true if the type can be moved around


	// these will be specialized outside
	// we need to specialize for references, consts, volatiles, etc.. (but not pointers!) even if IsPOD() already takes care of it,
	// so the user may specialize its own class traits in a simple way, without explicitly taking care of consts, refs, etc...

	template<typename _Type>	struct			HasTrivialConstructor : IsPOD<_Type> {};
	template<typename _Type>	struct			HasTrivialConstructor<_Type const> : HasTrivialConstructor<_Type> {};
	template<typename _Type>	struct			HasTrivialConstructor<_Type volatile> : HasTrivialConstructor<_Type> {};
	template<typename _Type>	struct			HasTrivialConstructor<_Type&> : HasTrivialConstructor<_Type> {};
	template<typename _Type, int _Size>	struct	HasTrivialConstructor<_Type[_Size]> : HasTrivialConstructor<_Type> {};

	template<typename _Type>	struct			HasTrivialDestructor : IsPOD<_Type> {};
	template<typename _Type>	struct			HasTrivialDestructor<_Type const> : HasTrivialDestructor<_Type> {};
	template<typename _Type>	struct			HasTrivialDestructor<_Type volatile> : HasTrivialDestructor<_Type> {};
	template<typename _Type>	struct			HasTrivialDestructor<_Type&> : HasTrivialDestructor<_Type> {};
	template<typename _Type, int _Size>	struct	HasTrivialDestructor<_Type[_Size]> : HasTrivialDestructor<_Type> {};

	template<typename _Type>	struct			HasTrivialCopy : IsPOD<_Type> {};
	template<typename _Type>	struct			HasTrivialCopy<_Type const> : HasTrivialCopy<_Type> {};	// don't remove volatiles, reading a volatile to copy it somewhere may not be trivial, whatever '_Type' is...
	template<typename _Type>	struct			HasTrivialCopy<_Type&> : HasTrivialCopy<_Type> {};
	template<typename _Type, int _Size>	struct	HasTrivialCopy<_Type[_Size]> : HasTrivialCopy<_Type> {};

	template<typename _Type>	struct			HasTrivialAssign : IsPOD<_Type> {};
	template<typename _Type>	struct			HasTrivialAssign<_Type&> : HasTrivialAssign<_Type> {};
	template<typename _Type, int _Size>	struct	HasTrivialAssign<_Type[_Size]> : HasTrivialAssign<_Type> {};

	// true if the type can be moved around without side effects (ex: in arrays, allows to avoid construction of a new object and destruction of the old one during reallocs
	// for simplicity, by default this is always true. you will have to override it for the rare types where it doesn't hold.
	template<typename _Type>	struct			HasTrivialRelocate : TrueType {};

	// handles the 'void' type
//	template<typename _Type>	HH_FORCEINLINE unsigned int	SizeOf() { return sizeof(_Type); }
//	template<>					HH_FORCEINLINE unsigned int	SizeOf<void>() { return 0; }
	template<typename _Type>	struct			SizeOf			{ static const unsigned int	Value = sizeof(_Type); };
	template<>					struct			SizeOf<void>	{ static const unsigned int	Value = 0; };
};

//----------------------------------------------------------------------------

template<typename _Type, bool _Condition>
struct	TConditionallyDeclaredType {};

template<typename _Type>
struct	TConditionallyDeclaredType<_Type, true> { public: _Type data; };

//----------------------------------------------------------------------------

template<typename _Type0, typename _Type1>
struct	TTypesEqual : FalseType {};

template<typename _Type>
struct	TTypesEqual<_Type, _Type> : TrueType {};

//----------------------------------------------------------------------------

template<typename _T0, typename _T1> struct		TTypeCanCastT1ToT0 : public _T0 { static const int	CanCast = sizeof(static_cast<_T0*>((_T1*)null)) != 0; }; //-V568

template<typename _T0, typename _T1, bool _T0IsNumeric = TTypeTraits::IsNumeric<_T0>::True, bool _T1IsNumeric = TTypeTraits::IsNumeric<_T1>::True>
struct											TTypeAInheritsTypeB : public TTypeBoolConstant<TTypeCanCastT1ToT0<_T0, _T1>::CanCast != 0> {};
template<typename _T0, typename _T1> struct		TTypeAInheritsTypeB<_T0, _T1, true, false> : public FalseType {};
template<typename _T0, typename _T1> struct		TTypeAInheritsTypeB<_T0, _T1, false, true> : public FalseType {};
template<typename _T0, typename _T1> struct		TTypeAInheritsTypeB<_T0, _T1, true, true> : public FalseType {};	// both are numeric, but different numerics, consider not a match
template<typename _T0, bool _T0IsNumeric, bool _T1IsNumeric>
struct											TTypeAInheritsTypeB<_T0, _T0, _T0IsNumeric, _T1IsNumeric> : public TrueType {};

template<typename _T0, typename _T1> struct		TTypesAreRelated : public TTypeAInheritsTypeB<_T0, _T1> {};
template<typename _T0> struct					TTypesAreRelated<_T0, _T0> : public TrueType {};

//----------------------------------------------------------------------------

template<typename _Type0, typename _Type1, bool _Condition>
struct	TTypeSwitcher_IfTrue : TrueType
{
	typedef _Type0	Type;
};

template<typename _Type0, typename _Type1>
struct	TTypeSwitcher_IfTrue<_Type0, _Type1, false> : FalseType
{
	typedef _Type1	Type;
};

//----------------------------------------------------------------------------

template<typename _Type0, typename _Type1>
struct	TTypePair
{
	typedef _Type0	First;
	typedef _Type1	Second;
};

template<typename _Type0, typename _Type1, typename _Type2>
struct	TTypeTuple
{
	typedef _Type0	First;
	typedef _Type1	Second;
	typedef _Type2	Third;
};

template<typename _Type0, typename _Type1, typename _Type2, typename _Type3>
struct	TTypeQuad
{
	typedef _Type0	First;
	typedef _Type1	Second;
	typedef _Type2	Third;
	typedef _Type3	Fourth;
};

//----------------------------------------------------------------------------
//
//	Non-Copyable
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CNonCopyable
{
private:
	CNonCopyable(const CNonCopyable &copy);
	CNonCopyable const &operator = (const CNonCopyable &);

protected:
	CNonCopyable() {}
	~CNonCopyable() {}

public:
};

//----------------------------------------------------------------------------
// we can't relocate these:

namespace	TTypeTraits
{
	template<>	struct	IsRelocatable<CNonCopyable> : FalseType {};
	template<>	struct	HasTrivialRelocate<CNonCopyable> : FalseType {};
}

//----------------------------------------------------------------------------
#endif // __HH_TEMPLATE_METAPROGRAMMING_H__
