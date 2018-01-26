#pragma once

//----------------------------------------------------------------------------
// Created on Wed 2007/10/24 17:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __KR_STRING_H__
#define __KR_STRING_H__

#include "hh_kernel/include/kr_refptr.h"
#include "hh_kernel/include/kr_containers.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------
// pretty similar to a CRefCountedMemoryBuffer

HH_FORWARD_DECLARE(StringContainer);

//----------------------------------------------------------------------------
//
//	Implementation details:
//	the string data pointer will always be 16-bytes aligned,
//	and can be used with the fast SNativeStringUtils-16 functions (see kr_buffer.h).
//	the total string data byte count will always be a multiple of 16 bytes, padded with zeroes
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CStringContainer : public CRefCountedObject
{
private:
	CStringContainer();
	CStringContainer(hh_u32 stringLen, bool resizable = false) : m_Length(stringLen | (resizable ? 0x80000000 : 0)) {}

	static const hh_u32				LengthMask = 0x3FFFFFFF;
	hh_u32							m_Length;
	static CStringContainer			*SetupFromFreshAllocatedBuffer(void *data, const char *string, hh_u32 stringLen);			// 'string' can be non-null-terminated
	void							_InternalSetLength(hh_u32 newLength);

public:
	~CStringContainer();

	static PStringContainer			New(const char *string);						// 'string' must be null-terminated
	static PStringContainer			New(const char *string, hh_u32 size);			// 'string' may be non-null-terminated
	static PStringContainer			NewResizable(const char *string);				// 'string' must be null-terminated
	static PStringContainer			NewResizable(const char *string, hh_u32 size);	// 'string' may be non-null-terminated

	HH_FORCEINLINE bool				SupportsDynamicResize() const { return (hh_i32)m_Length < 0; }
	bool							DynamicResize(hh_u32 newLength);	// the length should not include the null-terminating character.

	HH_FORCEINLINE char				*Data();	// always 16-bytes aligned
	HH_FORCEINLINE hh_u32			Length() const { return m_Length & LengthMask; }
	HH_FORCEINLINE void				UnsafeSetLength(hh_u32 length) { _InternalSetLength(length); }
	HH_FORCEINLINE void				Clear() { _InternalSetLength(0); }
	void							Rebuild();

	HH_FORCEINLINE static char		*SafeData(CStringContainer *self) { return (self != null) ? self->Data() : null; }
	HH_FORCEINLINE static hh_u32	SafeLength(const CStringContainer *self) { return (self != null) ? (self->m_Length & LengthMask) : 0; }
};

//----------------------------------------------------------------------------

#define	HH_STRING_CONTAINER_INTERNAL_ALIGNMENT_PADDING	(0xF - ((sizeof(CStringContainer) + 0xF) & 0xF))

enum
{
	kResizableString_OffsetSize = sizeof(CStringContainer),
	kResizableString_OffsetPtr = TIntegerTemplateTools::Align<sizeof(char*), kResizableString_OffsetSize + sizeof(hh_u32)>::Value,
	kResizableString_Size = kResizableString_OffsetPtr + sizeof(char*),
};

//----------------------------------------------------------------------------

HH_FORCEINLINE char		*CStringContainer::Data()
{
	if (!SupportsDynamicResize())
	{
		return reinterpret_cast<char*>(this + 1) + HH_STRING_CONTAINER_INTERNAL_ALIGNMENT_PADDING;	// always 16-bytes aligned
	}
	return *reinterpret_cast<char**>(Mem::AdvanceRawPointer(this, kResizableString_OffsetPtr));
}

//----------------------------------------------------------------------------
//
//	Actual string object
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CString
{
private:
	friend class				CBufferInternals;

	void						_Concatenate(const char *str, hh_u32 length);
	void						_ConcatenatePath(const char *str, hh_u32 length);
	void						_Prepend(const char *str, hh_u32 length);
	void						_MakeUnique();

protected:
	PStringContainer			m_Container;

public:
	static const CString		EmptyString;

	HH_FORCEINLINE CString() {}
	CString(const char *source) { if (source != null && *source != '\0') m_Container = CStringContainer::New(source); }
	CString(const CString &source) : m_Container(source.m_Container) {}
	explicit CString(const PStringContainer &container) : m_Container(container) {}
	~CString();

	HH_FORCEINLINE const char	*Data() const { return CStringContainer::SafeData(m_Container.Get()); }
	char						*RawDataForWriting();
	TMemoryView<const char>		View() const { return TMemoryView<const char>(Data(), Length()); }
	TMemoryView<char>			ViewForWriting();
	void						RebuildAfterRawModification();
	HH_INLINE bool				Empty() const { char *data = CStringContainer::SafeData(m_Container.Get()); return (data == null || data[0] == '\0'); }
	bool						Empty(const char *ignore) const;
	HH_INLINE hh_u32			Length() const { return CStringContainer::SafeLength(m_Container.Get()); }
	void						Clear();

	char						operator [] (hh_u32 index) const { HH_ASSERT(index < m_Container->Length()); return m_Container->Data()[index]; }
	char						&operator [] (hh_u32 index) { HH_ASSERT(index < m_Container->Length()); _MakeUnique(); return m_Container->Data()[index]; }
	char						Last() const { HH_ASSERT(!Empty()); return operator [] (Length() - 1); }
	char						&Last() { HH_ASSERT(!Empty()); return operator [] (Length() - 1); }

	PK_WARN_UNUSED_RETURN CString	operator + (const CString &concatenate) const;
	PK_WARN_UNUSED_RETURN CString	operator + (const char *concatenate) const;
	CString						&operator += (const CString &concatenate);
	CString						&operator += (const char *concatenate);

	void						Append(const char *concatenate, hh_u32 concatenateLength);
	void						Prepend(const CString &prepend);
	void						Prepend(const char *prepend);
	void						Prepend(const char *prepend, hh_u32 prependLength);

	bool						Compare(const char *other) const;
	bool						Compare(const CString &other) const;
	bool						CompareCase(const char *other) const;
	bool						CompareCase(const CString &other) const;

	bool						operator == (const char *other) const { return Compare(other); }
	bool						operator == (const CString &other) const { return Compare(other); }

	bool						operator != (const char *other) const { return !Compare(other); }
	bool						operator != (const CString &other) const { return !Compare(other); }

	//bool						EqualCase(const char *other) const;
	//bool						EqualCase(const CString &other) const;

	bool						operator > (const char *other) const;
	bool						operator < (const char *other) const;
	bool						operator > (const CString &other) const;
	bool						operator < (const CString &other) const;

	bool						operator <= (const CString &other) const { return !(*this > other); }
	bool						operator >= (const CString &other) const { return !(*this < other); }
	bool						operator <= (const char *other) const { return !(*this > other); }
	bool						operator >= (const char *other) const { return !(*this < other); }

	// Kind-of smart path concatenation
	// - makes sure that at most one '/' exists beetween concatenees
	// - right-hand path is assumed relative
	// "foo"    / "bar"   -> "foo/bar"
	// "/foo/"  / "/bar/" -> "/foo/bar/"
	// "/"      / ""      -> "/"        (keeps the absolute-ness of left-hand path)
	// ""       / "/"     -> ""         (keeps the relative-ness of left-hand path)
	PK_WARN_UNUSED_RETURN CString	operator / (const char *other) const;
	PK_WARN_UNUSED_RETURN CString	operator / (const CString &other) const;
	CString						&operator /= (const char *other);
	CString						&operator /= (const CString &other);
	// if empty returns empty, else append a trailing "/" if not already
	void						AppendSlash();
	CString						SlashAppended() const;

	static CString				VAFormat(const char *format, va_list argList);
	static CString				Format(const char *format, ...) PK_COMPILER_ATTRIB_PRINTF(1, 2);

	CString						&ToUppercase();
	CString						&ToLowercase();

	CString						Uppercase() const;
	CString						Lowercase() const;

	CString						&Escape();	// adds backslashes, converts "\n" to "\\n", " to \", etc..
	CString						&Unescape();

	CString						Escaped() const;
	CString						Unescaped() const;

	hh_u32						Ident(hh_u32 start, hh_u32 n, hh_u32 tabWidth);	// returns the number of bytes added to the string

	void						Reverse();	// "abcd" -> "dcba"
	CString						Reversed() const { CString out = *this; out.Reverse(); return out; }


	bool						Match(const char *pattern) const;
	bool						Match(const CString &pattern) const;

	bool						MatchCase(const char *pattern) const;
	bool						MatchCase(const CString &pattern) const;

	bool						MatchWithInhibitors(const char *pattern) const;
	bool						MatchWithInhibitors(const CString &pattern) const;

	bool						MatchCaseWithInhibitors(const char *pattern) const;
	bool						MatchCaseWithInhibitors(const CString &pattern) const;

	float						FuzzyMatch(const CString &other) const;	// returns 1.0f for a perfect match, 0.0f for no similarity
	void						FuzzyMatchList(const TStridedMemoryView<const CString> &stringList, const TMemoryView<float> &outMatchResults) const;

	struct		SFuzzyMatchListElement
	{
		hh_u32	m_IndexInStringList;
		float	m_MatchWeight;

		SFuzzyMatchListElement(hh_u32 id, float w) : m_IndexInStringList(id), m_MatchWeight(w) {}
	};
	void						FuzzyMatchList_Ordered(const TStridedMemoryView<const CString> &stringList, TArray<SFuzzyMatchListElement> &outOrderedMatchResults, float tolerance = 0.0f) const;	// orders the results from highest match weight to lowest

	void						SplitCase(TArray<CString> &outWords) const;
	void						Split(char splitWith, TArray<CString> &outList) const;
	void						Split_ThreadSafe(char splitWith, TArray<CString> &outList) const;
	void						Split(bool (*isSeparator)(char), TArray<CString> &outList) const;

	CString						Extract(hh_u32 first, hh_u32 last) const;
	static CString				Extract(const char *from, hh_u32 first, hh_u32 last);

	// for these, if len == 0, will use len = strlen(str)
	bool						StartsWith(const char *str, hh_u32 len = 0) const;		// "Toto42".StartsWith("Toto") -> returns true
	bool						EndsWith(const char *str, hh_u32 len = 0) const;		// "Toto42".EndsWith("42") -> returns true
	bool						Contains(const char *str, hh_u32 len = 0) const;		// "Toto42".Contains("to4") -> returns true
	bool						StartsWithCase(const char *str, hh_u32 len = 0) const;	// case-insensitive version of 'StartsWith'
	bool						EndsWithCase(const char *str, hh_u32 len = 0) const;	// case-insensitive version of 'EndsWith'
	bool						ContainsCase(const char *str, hh_u32 len = 0) const;	// case-insensitive version of 'Contains'

	bool						StartsWith(const CString &str)		const { return StartsWith(str.Data(), str.Length()); }
	bool						EndsWith(const CString &str)		const { return EndsWith(str.Data(), str.Length()); }
	bool						Contains(const CString &str)		const { return Contains(str.Data(), str.Length()); }
	bool						StartsWithCase(const CString &str)	const { return StartsWithCase(str.Data(), str.Length()); }
	bool						EndsWithCase(const CString &str)	const { return EndsWithCase(str.Data(), str.Length()); }
	bool						ContainsCase(const CString &str)	const { return ContainsCase(str.Data(), str.Length()); }

	CString						Replace(const char *searchFor, const char *replaceWith) const;
	CString						ReplaceTr(const char *charListToReplace, const char *charListToReplaceWith) const; // tr unix command like
	void						ReplaceTrInplace(const char *charListToReplace, const char *charListToReplaceWith); // tr unix command like

	void						Strip(const char *charSet);
	void						StripLeft(const char *charSet);
	void						StripRight(const char *charSet);

	CGuid						FindFirstOf(char c) const;	// returns the index of the first character equal to c, CGuid::INVALID if not found
	CGuid						FindFirstOf(const char *str) const; // returns the index of the first string equal to str, CGuid::INVALID if not found
	CGuid						FindNextOf(const char *str, hh_u32 n) const;	// returns the index of the first string equal to str after nth char, CGuid::INVALID if not found
	CGuid						FindLastOf(char c) const;	// returns the index of the last character equal to c, CGuid::INVALID if not found

	void						Uncomment();
};

//----------------------------------------------------------------------------

class	CStringBatchSplitter
{
private:
	hh_u8			m_SplitTable[0x100];
public:
	CStringBatchSplitter();

	void			AddSplitChar(char c);
	void			AddSplitChars(const char *str);
	void			AddSplitChars(const CString &str);

	void			RemoveSplitChar(char c);
	void			RemoveSplitChars(const char *str);
	void			RemoveSplitChars(const CString &str);

	void			Split(const CString &str, TArray<CString> &outList);
	void			Split_ThreadSafe(const CString &str, TArray<CString> &outList) { CString tmp = str; Split(str, outList); }
};

//----------------------------------------------------------------------------

HH_FORCEINLINE CString	operator + (const char *str0, const CString &str1) { return CString(str0) + str1; }	// allows to write 'CString str = "abcd" + CString("efgh")'
HH_FORCEINLINE CString	operator / (const char *str0, const CString &str1) { return CString(str0) / str1; }	// allows to write 'CString str = "abcd" / CString("efgh")'
HH_FORCEINLINE bool		operator != (const char *str0, const CString &str1) { return str1 != str0; }	// "tagada" != CString("tsointsoin")
HH_FORCEINLINE bool		operator == (const char *str0, const CString &str1) { return str1 == str0; }	// "tagada" == CString("tsointsoin")
HH_FORCEINLINE bool		operator > (const char *str0, const CString &str1) { return str1 < str0; }
HH_FORCEINLINE bool		operator < (const char *str0, const CString &str1) { return str1 > str0; }
HH_FORCEINLINE bool		operator >= (const char *str0, const CString &str1) { return str1 <= str0; }
HH_FORCEINLINE bool		operator <= (const char *str0, const CString &str1) { return str1 >= str0; }

//----------------------------------------------------------------------------

template<typename _Type>
class	TStringView
{
private:
	_Type				*m_Data;
	hh_u32				m_Length;
public:
	TStringView() : m_Data(null), m_Length(0) {}
	TStringView(_Type *data, hh_u32 length) : m_Data(data), m_Length(length) {}

#if (PK_HAS_RVALUE_REF != 0) && (PK_HAS_DEF_DEL_FUNCTIONS != 0)
	// Prevents dangerous local CString ctor.
	// When: const char *s; CStringView(s) --> prevents implicit CStringView(CString(s))
	explicit TStringView(const CString &&string) = delete;
#endif

	explicit TStringView(const CString &string) : m_Data(string.Data()), m_Length(string.Length()) {}	// warning when explicitely using this ! the CString must be kept alive for at least the lifetime of the view

	template<hh_u32 _Length>	// FIXME: DANGEROUS ! will break down if we pass static buffers filled with sprintf, the length might be larger than what it actually is
	TStringView(_Type (&data)[_Length]) : m_Data(data), m_Length(_Length - 1) {}	// length counts the terminating null character

	TStringView(const TMemoryView<_Type> &string) : m_Data(string.Data()), m_Length(string.Count()) { }

	void				Clear() { m_Data = null; m_Length = 0; }

	bool				Empty() const { return m_Length == 0; }
	hh_u32				Length() const { return m_Length; }
	_Type				*Data() const { return m_Data; }

	TMemoryView<_Type>	View() const { return TMemoryView<const char>(m_Data, m_Length); }
	CString				ToString() const { if (Empty()) return null; return CString(CStringContainer::New(m_Data, m_Length)); }

	bool				operator == (const TStringView<_Type> &other) const
	{
		return (m_Length == other.m_Length) && (m_Data == null || 0 == memcmp(m_Data, other.m_Data, m_Length));
	}
	bool				operator != (const TStringView<_Type> &other) const { return !(*this == other); }
};

typedef TStringView<const char>		CStringView;

//----------------------------------------------------------------------------

HH_KERNEL_EXPORT bool	operator == (const CString &str, const CStringView &view);

HH_INLINE bool	operator != (const CString &str, const CStringView &view)
{
	return !(str == view);
}

//----------------------------------------------------------------------------
__PK_API_END

#endif // __KR_STRING_H__
