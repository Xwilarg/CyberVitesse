#pragma once

//----------------------------------------------------------------------------
// Created on Tue 2003/07/01 03:21 by Julien Bilalte
//
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef	__KR_BUFFER_H__
#define	__KR_BUFFER_H__

__PK_API_BEGIN

//----------------------------------------------------------------------------

struct	HH_KERNEL_EXPORT SNativeStringUtils
{
	// all these functions, unless otherwise stated, expect null-terminated strings

	static hh_u32	Length(const char *data);
	static bool		Equal(const char *data0, const char *data1);
	static bool		EqualCase(const char *data0, const char *data1);
	static hh_i32	Compare(const char *data0, const char *data1);
	static hh_i32	CompareCase(const char *data0, const char *data1);

	// versions for 16-bytes aligned & padded strings
	static hh_u32	Length_16(const char *data);	// data can be null
	static bool		Equal_16(const void *data0, const void *data1, hh_u32 length);		// length0 == length1 == length, length >= 0, data0 != null && data1 != null
	static bool		EqualCase_16(const char *data0, const char *data1, hh_u32 length);	// length0 == length1 == length, length >= 0, data0 != null && data1 != null
	static hh_i32	Compare_16(const void *data0, const void *data1, hh_u32 length);	// length0 == length1 == length, length >= 0, data0 != null && data1 != null
	static hh_i32	CompareCase_16(const char *data0, const char *data1, hh_u32 length);// length0 == length1 == length, length >= 0, data0 != null && data1 != null
	static void		Reverse_16(char *data, hh_u32 length);

	static bool		Uncomment(char *data);				// replaces comments with whitespaces ' ' (accepts null inputs). Returns false on failure (unclosed multiline comment)
//	static bool		UncommentAndCondense(char *data);	// entirely removes comments

	static hh_u32	Unescape(const char *src, char *dst);									// returns the new length (newlen <= oldlen). src and dst can point to the same place.
	static hh_u32	Unescape(const char *src, char *dst, hh_u32 length);					// 'data' does not have to be null-terminated
	static hh_u32	Escape(const char *src, char *dst_MustBeLargeEnough);					// returns the new length (newlen >= oldlen && newlen <= 2*oldlen)
	static hh_u32	Escape(const char *src, char *dst_MustBeLargeEnough, hh_u32 length);	// 'data' does not have to be null-terminated

	static void		Uppercase(char *data, hh_u32 length);	// converts to uppercase
	static void		Lowercase(char *data, hh_u32 length);	// converts to uppercase

	// if there isn't enough space between 'storageBegin' and 'storageEnd', the function will allocate a buffer on the heap.
	// if the returned pointer is == storageBegin, no allocation was made.
	// otherwise, the allocated buffer is returned, and the caller is responsible for freeint it with a call to HH_FREE()
	// if null is returned, the contents of 'outSize' are undefined

	static char		*VAFormat(char *storageBegin, const char *storageEnd, hh_u32 &outCharCount, const char *format, va_list argList);
	static char		*Format(char *storageBegin, const char *storageEnd, hh_u32 &outCharCount, const char *format, ...)  PK_COMPILER_ATTRIB_PRINTF(4, 5);	// calls VAFormat()

	// returns TRUE if the buffer matches with the pattern, FALSE otherwise
	// patterns may have wildcards such as '*' and '?'
	// the '?' character can match with any single character in the buffer.
	// for example:
	// "tsoin?soin" will match with "tsointsoin"
	// but:
	// "tsoin?soin" will NOT match with "tsointxxsoin"
	//
	// the '*' character can match with a random count (0 included) of random characters.
	// for example:
	// "*ouga" will match with "ouga", "asdasdasdouga", "bouga", etc...
	// "t*soin" will match with "tsoin", "tsointsoin", "txxxxxsoin", etc...
	//
	// multiple self-following '*' can be used:
	// "t********soin" will match with the same buffers "t*soin" would have.

	static bool		Match(const char *pattern, const char *buffer);
	static bool		MatchCase(const char *pattern, const char *buffer);	// case-insensitive


	// same as kr_buffer_match(), but with inhibitors enabled.
	// "marmotte\\?" will not be treated the same as "marmotte?", and will _only_ match
	// with the same string: "marmotte\\?"
	// "marmotte\\?*aaa!" will match with "marmotte\\? poowwaaaaaaaa!"
	// "t\\*soin*" will match with "t*soin" and "t*sointsoin", not with "tsoin" or "tsointsoin"

	static bool		MatchWithInhibitors(const char *pattern, const char *buffer);
	static bool		MatchCaseWithInhibitors(const char *pattern, const char *buffer);	// case-insensitive

	//	this function takes a string, a separator list, and splits the string at each separator.
	//	separators are only 1 character long, there can be any of them. having lots of separators
	//	will slow down the function though...
	//	the function is sub-string aware (see the usage example below)
	//
	//	parameters:
	//	'string' :		this is the input string to be split. the function will modify the string,
	//					and insert '\0' characters into the buffer where separators are found. so the string
	//					should be strduped before calling the function if you plan on using it again later.
	//	'separators' :	holds the list of character separators. the input string will be split
	//					whenever one of these separators is encountered.
	//	'outCount' :	pointer to an int where the tokenizer will store the number of output tokens.
	//					if set to 'null', the function will simply ignore it.
	//
	//	the tokenizer returns a pointer to an array of pointers to strings.
	//	note that this 'char**' is null-terminated. each string pointer points to somewhere
	//	inside the original input string. therefor, as soon as the input string is free-ed,
	//	all the pointers in the returned pointers array become potentially invalid.
	//	if there is not enough memory to allocate the output pointer array, 'null' is returned.
	//	if no separators are found, the input string remains unchanged, and only one token is returned.
	//	'null' is also returned if the string is empty (if string == ""), if the string is made
	//	only of separators, or if string == 'null'. even if 'null' is returned, the input string might
	//	have changed.
	//	The only case where it will not be changed is if string == 'null', or if the string contained
	//	no separators.
	//
	//	here are some examples of the function behaviour:
	//
	//	Explode("tagada tsointsoin ;;\t;;ouga\"a;b cd tt\t#\"6942", " \t;td:#");
	//
	//	will output the following string array:
	//	{
	//		"tagada"
	//		"soin"
	//		"soin"
	//		"ouga"
	//		"\"a;b cd tt\t#\"6942"
	//	}

	static char		**Explode(char *string, const char *separators, hh_u32 *outCount = null);


	template<hh_u32 _Length>
	static void		StrCpy(char (&outBuffer)[_Length], const char *str)
	{
		strncpy(outBuffer, str, _Length);
		outBuffer[_Length-1] = '\0';
	}

	template<hh_u32 LengthDst, hh_u32 _LengthSrc>
	static void		StrCpyStatic(char (&outBuffer)[LengthDst], const char (&str)[_LengthSrc])
	{
		HH_STATIC_ASSERT(_LengthSrc <= LengthDst);
		memcpy(outBuffer, str, _LengthSrc);	// must be null-terminated, terminating '\0' will get copied along the way, no need to write it again afterwards
	}

	static hh_u32	VSPrintf(char *outBuffer, hh_u32 bufferLen, const char *format, va_list arglist);
	static hh_u32	SPrintf(char *outBuffer, hh_u32 bufferLen, const char *format, ...) PK_COMPILER_ATTRIB_PRINTF(3, 4);

	template<hh_u32 _Length>
	static hh_u32	SPrintf(char (&outBuffer)[_Length], const char *format, ...) PK_COMPILER_ATTRIB_PRINTF(2, 3);
	template<hh_u32 _Length>
	static hh_u32	VSPrintf(char (&outBuffer)[_Length], const char *format, va_list arglist) { return VSPrintf(outBuffer, _Length, format, arglist); }

};

//----------------------------------------------------------------------------

inline hh_u32	SNativeStringUtils::SPrintf(char *outBuffer, hh_u32 bufferLen, const char *format, ...)
{
	va_list	arglist;
	va_start(arglist, format);
	hh_u32	len = VSPrintf(outBuffer, bufferLen, format, arglist);
	va_end(arglist);
	return len;
}

//----------------------------------------------------------------------------

template<hh_u32 _Length>
inline hh_u32	SNativeStringUtils::SPrintf(char (&outBuffer)[_Length], const char *format, ...)
{
	va_list	arglist;
	va_start(arglist, format);
	hh_u32	len = VSPrintf(outBuffer, format, arglist);
	va_end(arglist);
	return len;
}

//----------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------

class	HH_KERNEL_EXPORT CNumericScanner
{
private:
	static const char	*UnsafeScanFloat(float &output, const char *bufferStart, const char *bufferEnd);
	static const char	*UnsafeScanI32(hh_i32 &output, const char *bufferStart, const char *bufferEnd);
	static const char	*UnsafeScanU32(hh_u32 &output, const char *bufferStart, const char *bufferEnd);

public:
	struct	SNumber
	{
		enum	EType
		{
			Type_Fp = 0,
			Type_Int = 1,
		};
		EType	m_Type;
		union
		{
			double	m_ValueFp;
			hh_i64	m_ValueInt;
		};

		SNumber() : m_Type(Type_Int), m_ValueInt(0) {}
		SNumber(hh_i32 value) : m_Type(Type_Int), m_ValueInt(value) {}
		SNumber(hh_i64 value) : m_Type(Type_Int), m_ValueInt(value) {}
		SNumber(float value) : m_Type(Type_Fp), m_ValueFp(value) {}
		SNumber(double value) : m_Type(Type_Fp), m_ValueFp(value) {}
	};

	// on failure, returns 0, otherwise, returns the first character's index after the number in the input string.
	// expects a null-terminated string...
	static hh_u32		ScanNumber(const char *buffer, SNumber &output);

	// if 'exprEnd' is null, expects a null-terminated string...
	static const char	*ScanScalar(float &output, const char *exprStart, const char *exprEnd = null) { HH_ASSERT(exprStart != null); output = 0; return UnsafeScanFloat(output, exprStart, exprEnd); }
	static const char	*ScanScalar(hh_i32 &output, const char *exprStart, const char *exprEnd = null) { HH_ASSERT(exprStart != null); output = 0; return UnsafeScanI32(output, exprStart, exprEnd); }
	static const char	*ScanScalar(hh_u32 &output, const char *exprStart, const char *exprEnd = null) { HH_ASSERT(exprStart != null); output = 0; return UnsafeScanU32(output, exprStart, exprEnd); }

	template<typename _Type, hh_u32 _Count>
	static const char	*ScanVector(TVector<_Type, _Count> &output, char separator, const char *bufferStart, const char *bufferEnd = null)
	{
		HH_ASSERT(bufferStart != null);
		if (bufferEnd == null)
			bufferEnd = bufferStart + SNativeStringUtils::Length(bufferStart);

//		if (separator == '\0')
//			separator = ',';

		bufferStart = ScanScalar(output.Axis(0), bufferStart, bufferEnd);
		if (bufferStart != null)
		{
			for (hh_u32 i = 1; i < _Count; i++)
			{
				HH_ASSERT(bufferStart <= bufferEnd);
				if (bufferStart >= bufferEnd - 1 || *bufferStart++ != separator)
					goto _failure;
				bufferStart = ScanScalar(output.Axis(i), bufferStart, bufferEnd);
				if (bufferStart == null)
					goto _failure;
			}
			return bufferStart;
		}

_failure:
		for (hh_u32 i = 0; i < _Count; i++)
			output.Axis(i) = TNumericTraits<_Type>::Zero();
		return null;
	}
};

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __KR_BUFFER_H__
