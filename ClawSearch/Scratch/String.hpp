/*	libscratch - Multipurpose objective C++ library.

		Copyright (c) 2013 - 2016 Angelo Geels <spansjh@gmail.com>

		Permission is hereby granted, free of charge, to any person
		obtaining a copy of this software and associated documentation
		files (the "Software"), to deal in the Software without
		restriction, including without limitation the rights to use,
		copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the
		Software is furnished to do so, subject to the following
		conditions:

		The above copyright notice and this permission notice shall be
		included in all copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
		OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
		NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
		HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
		WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
		FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
		OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#ifdef SCRATCH_IMPL
#include <stdio.h> // for vsprintf
#include <stdarg.h> // for va_list
#include <string.h> // for strlen and strcmp
#include <ctype.h>
#endif

#include "StackArray.hpp"

#ifndef SCRATCH_NO_THREADSAFE
#include "Mutex.hpp"
#endif

#ifndef SCRATCH_NO_UTF8
#include "utf8.h"
typedef int s_char;
#else
typedef char s_char;
#endif

#ifndef STRING_FORMAT_BUFFER_SIZE
#define STRING_FORMAT_BUFFER_SIZE 1024
#endif

SCRATCH_NAMESPACE_BEGIN;

class String
{
	friend class Filename;
protected:
	char* str_szBuffer;
#ifndef SCRATCH_NO_THREADSAFE
	Mutex str_mutex;
#endif

	void CopyToBuffer(const char* szSrc);
	void AppendToBuffer(const char* szSrc);
	void AppendToBuffer(const char* szSrc, size_t iCountBytes);
	void AppendToBuffer(const s_char cSrc);

public:
	static char* str_szEmpty;
	static int str_iInstances;

	String();
	String(const char* szValue);
	String(const char* szValue, int iStartBytes, int iBytes);
	String(const String &strCopy);
	virtual ~String();

	int Length() const; // Returns the length of the string in characters.
	int Size() const; // Returns the length of the string in bytes.

	void SetF(const char* szFormat, ...);
	void AppendF(const char* szFormat, ...);

	void Split(const String &strNeedle, StackArray<String> &astrResult) const;
	void Split(const String &strNeedle, StackArray<String> &astrResult, bool bTrimAll) const;
	void CommandLineSplit(StackArray<String> &astrResult) const;
private:
	String InternalTrim(bool bLeft, bool bRight, s_char c = ' ') const;
public:
	String Trim() const;
	String Trim(s_char c) const;
	String TrimLeft() const;
	String TrimLeft(s_char c) const;
	String TrimRight() const;
	String TrimRight(s_char c) const;
	String Replace(const String &strNeedle, const String &strReplace) const;
	String SubString(int iStart) const;
	String SubString(int iStart, int iLen) const;
	String ToLower() const;
	String ToUpper() const;

	int IndexOf(s_char c) const;
	int IndexOf(const String &strNeedle) const;

	int IndexOfLast(s_char c) const;
	int IndexOfLast(const String &strNeedle) const;

	void Fill(s_char c, int ct);

	bool Contains(const String &strNeedle) const;
	bool Contains(s_char c) const;
	bool StartsWith(const String &strNeedle) const;
	bool EndsWith(const String &strNeedle) const;

	operator const char*();
	operator const char*() const;

	String& operator=(char* szSrc);
	String& operator=(const char* szSrc);
	String& operator=(const String &strSrc);

	String& operator+=(const char* szSrc);
	String& operator+=(const s_char cSrc);
	String& operator+=(const String &strSrc);

	String& operator*=(int ctRepeat);

	bool operator==(const char* szSrc) const;
	bool operator!=(const char* szSrc) const;

	bool CaseCompare(int n, const char* szSrc) const;
	int CaseCompare(const char* szSrc) const;

	bool operator>(const char* szSrc) const;
	bool operator>=(const char* szSrc) const;
	bool operator<(const char* szSrc) const;
	bool operator<=(const char* szSrc) const;

	const s_char operator[](int iIndex);
};

String operator+(const String &strLHS, const char* szRHS);
String operator+(const String &strLHS, const s_char cRHS);

String operator+(const char* szLHS, String &strRHS);
String operator+(const s_char cLHS, String &strRHS);

String operator*(const String &strLHS, int ctRepeat);
String operator*(int ctRepeat, const String &strRHS);

String strPrintF(const char* szFormat, ...);

#ifdef SCRATCH_IMPL

int String::str_iInstances = 0;
char* String::str_szEmpty = (char*)"";

void String::CopyToBuffer(const char* szSrc)
{
	// Validate the source string
	if (szSrc == nullptr) {
		return;
	}

#ifdef SCRATCH_NO_UTF8
	size_t iLen = strlen(szSrc);
#else
	size_t iLen = utf8size(szSrc) - 1;
#endif
	if (iLen == 0) {
		// Clean up
		if (this->str_szBuffer != String::str_szEmpty) {
			delete[] this->str_szBuffer;
		}

		// Set to empty char*
		this->str_szBuffer = String::str_szEmpty;
		return;
	}

	// Find the current size and the required size for the string.
#ifdef SCRATCH_NO_UTF8
	size_t iBufLen = strlen(this->str_szBuffer) + 1;
#else
	size_t iBufLen = utf8size(this->str_szBuffer);
#endif
	size_t iReqLen = iLen + 1;

	// Check if we need to make more room.
	if (iReqLen > iBufLen) {
		// Get rid of the previously allocated space and allocate new memory.
		if (this->str_szBuffer != String::str_szEmpty) {
			delete[] this->str_szBuffer;
		}
		this->str_szBuffer = new char[iReqLen];
	}

	// Copy data to the buffer.
	memcpy(this->str_szBuffer, szSrc, iLen);

	// Always end with a null terminator.
	this->str_szBuffer[iLen] = '\0';
}

void String::AppendToBuffer(const char* szSrc)
{
#ifdef SCRATCH_NO_UTF8
	size_t len = strlen(szSrc);
#else
	size_t len = utf8size(szSrc) - 1;
#endif
	this->AppendToBuffer(szSrc, len);
}

void String::AppendToBuffer(const char* szSrc, size_t iCountBytes)
{
	// Validate source string
	if (szSrc == nullptr) {
		return;
	}

	if (iCountBytes <= 0) {
		return;
	}

	// Keep track of our previous buffer pointer so we can use it later for appending.
	char* szOldBuffer = this->str_szBuffer;

	// Find the current size and the required size for the string.
#ifdef SCRATCH_NO_UTF8
	size_t iBufBytes = strlen(this->str_szBuffer);
#else
	size_t iBufBytes = utf8size(this->str_szBuffer) - 1;
#endif

	// Get ourselves a new buffer
	this->str_szBuffer = new char[iBufBytes + iCountBytes + 1];

	// Copy the buffers
	int iOffset = 0;

	for (size_t i = 0; i < iBufBytes; i++) {
		this->str_szBuffer[iOffset++] = szOldBuffer[i];
	}

	for (size_t i = 0; i < iCountBytes; i++) {
		this->str_szBuffer[iOffset++] = szSrc[i];
	}

	// Always end with a null terminator.
	this->str_szBuffer[iOffset] = '\0';

	// Clean up
	if (szOldBuffer != String::str_szEmpty) {
		delete[] szOldBuffer;
	}
}

void String::AppendToBuffer(const s_char cSrc)
{
	// Validate source string
	if (cSrc == 0) {
		return;
	}

	// Keep track of our previous buffer pointer so we can use it later for appending.
	char* szOldBuffer = this->str_szBuffer;

	// Find the current size and the required size for the string.
#ifdef SCRATCH_NO_UTF8
	size_t iBufBytes = strlen(this->str_szBuffer);
#else
	size_t iBufBytes = utf8size(this->str_szBuffer) - 1;
#endif

	// Get ourselves a new buffer
#ifdef SCRATCH_NO_UTF8
	this->str_szBuffer = new char[iBufBytes + 2];
#else
	size_t charSize = utf8codepointsize(cSrc);
	this->str_szBuffer = new char[iBufBytes + charSize + 1];
#endif

	// Copy the buffer
	size_t iOffset = 0;

	for (size_t i = 0; i < iBufBytes; i++) {
		this->str_szBuffer[iOffset++] = szOldBuffer[i];
	}

	// Append the new character
#ifdef SCRATCH_NO_UTF8
	this->str_szBuffer[iOffset++] = cSrc;
#else
	utf8catcodepoint(this->str_szBuffer + iOffset, cSrc, charSize);
	iOffset += charSize;
#endif

	// Always end with a null terminator.
	this->str_szBuffer[iOffset] = '\0';

	// Clean up
	if (szOldBuffer != String::str_szEmpty) {
		delete[] szOldBuffer;
	}
}

String::String()
{
	str_iInstances++;
	// Create a new empty buffer
	this->str_szBuffer = String::str_szEmpty;
}

String::String(const char* szStr)
{
	str_iInstances++;
	// Create a new buffer and copy data into it.
	this->str_szBuffer = String::str_szEmpty;
	this->CopyToBuffer(szStr);
}

String::String(const char* szValue, int iStartBytes, int iBytes)
{
	str_iInstances++;
	// Create a new buffer and copy data into it.
	this->str_szBuffer = String::str_szEmpty;
	this->AppendToBuffer(szValue + iStartBytes, iBytes);
}

String::String(const String &copy)
{
	str_iInstances++;
	MUTEX_LOCK(copy.str_mutex);
	// Create a new buffer and copy data into it.
	this->str_szBuffer = String::str_szEmpty;
	this->CopyToBuffer(copy);
}

String::~String()
{
	str_iInstances--;
	// Clean up
	if (this->str_szBuffer != String::str_szEmpty) {
		delete[] this->str_szBuffer;
	}
}

int String::Length() const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return (int)strlen(this->str_szBuffer);
#else
	return (int)utf8len(this->str_szBuffer);
#endif
}

int String::Size() const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return (int)strlen(this->str_szBuffer);
#else
	return (int)utf8size(this->str_szBuffer) - 1;
#endif
}

void String::SetF(const char* szFormat, ...)
{
	MUTEX_LOCK(str_mutex);

	int iSize = STRING_FORMAT_BUFFER_SIZE;
	char* szBuffer = new char[iSize];

	// Get the args list and do a vsprintf to get the right format.
	va_list vL;
	va_start(vL, szFormat);

	// Make sure we don't overflow the buffer by checking against result length
	int iPrintSize = vsnprintf(szBuffer, iSize, szFormat, vL);
	if (iSize <= iPrintSize) {
		// Fail delete buffer and try again
		delete[] szBuffer;
		szBuffer = new char[iPrintSize + 1]; // +1 for \0
		vsnprintf(szBuffer, iPrintSize + 1, szFormat, vL);
	}
	va_end(vL);

	// Copy the just-created buffer to the main buffer
	this->CopyToBuffer(szBuffer);

	// Clean up
	delete[] szBuffer;
}

void String::AppendF(const char* szFormat, ...)
{
	MUTEX_LOCK(str_mutex);

	int iSize = STRING_FORMAT_BUFFER_SIZE;
	char* szBuffer = new char[iSize];

	// Get the args list and do a vsprintf to get the right format.
	va_list vL;
	va_start(vL, szFormat);

	// Make sure we don't overflow the buffer by checking against result length
	int iPrintSize = vsnprintf(szBuffer, iSize, szFormat, vL);
	if (iSize <= iPrintSize) {
		// Fail delete buffer and try again
		delete[] szBuffer;
		szBuffer = new char[iPrintSize + 1]; // +1 for \0
		vsnprintf(szBuffer, iPrintSize + 1, szFormat, vL);
	}
	va_end(vL);

	// Copy the just-created buffer to the main buffer
	this->AppendToBuffer(szBuffer);

	// Clean up
	delete[] szBuffer;
}

void String::Split(const String &strNeedle, StackArray<String> &astrResult) const
{
	Split(strNeedle, astrResult, false);
}

void String::Split(const String &strNeedle, StackArray<String> &astrResult, bool bTrimAll) const
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strNeedle.str_mutex);

	// Keep a pointer to the current offset and a "previous offset"
	char* szOffset = str_szBuffer;
	char* szOffsetPrev = szOffset;
	int iCount = 1;

	do {
		// Find the needle from the string in the current offset pointer
#ifdef SCRATCH_NO_UTF8
		szOffset = strstr(szOffset, strNeedle);
#else
		szOffset = (char*)utf8str(szOffset, strNeedle);
#endif

		// If the needle is found
		if (szOffset != nullptr) {
			// Get the length for the string
			size_t iLen = szOffset - szOffsetPrev;

			// And get a buffer started
			char* szPart = new char[iLen + 1];

			// Copy over the characters to the part buffer
			size_t i = 0;
			for (; i < iLen; i++) {
				szPart[i] = *(szOffset - (iLen - i));
			}
			szPart[i] = '\0';

			// Add it to the return array
			astrResult.Push() = szPart;
			delete[] szPart;

			// Keep a seperate count
			iCount++;

			// Increase the offset pointer by the needle size
#ifdef SCRATCH_NO_UTF8
			szOffset += strlen(strNeedle);
#else
			szOffset += utf8size(strNeedle) - 1;
#endif

			// Keep track of the pointer
			szOffsetPrev = szOffset;
		} else {
			// Get the length for the string
#ifdef SCRATCH_NO_UTF8
			size_t iLen = strlen(szOffsetPrev);
#else
			size_t iLen = utf8size(szOffsetPrev) - 1;
#endif

			// And get a buffer started
			char* szPart = new char[iLen + 1];

			// Copy over the characters to the part buffer
			size_t i = 0;
			for (; i < iLen; i++) {
				szPart[i] = szOffsetPrev[i];
			}
			szPart[i] = '\0';

			// Add it to the return vector
			String &strAdd = astrResult.Push();
			strAdd = szPart;
			if (bTrimAll) {
				strAdd = strAdd.Trim();
			}
			delete[] szPart;
		}
	} while (szOffset != nullptr);
}

void String::CommandLineSplit(StackArray<String> &astrResult) const
{
	MUTEX_LOCK(str_mutex);

	char* sz = str_szBuffer;
	bool bInString = false;
	String strBuffer;

	s_char c;
	do {
#ifdef SCRATCH_NO_UTF8
		c = *sz;
		s_char cn = *(sz + 1);
#else
		s_char cn;
		sz = (char*)utf8codepoint(sz, &c);
		utf8codepoint(sz, &cn);
#endif

		if (c == '\\') {
			strBuffer += cn;
#ifdef SCRATCH_NO_UTF8
			sz++;
#else
			sz = (char*)utf8codepoint(sz, nullptr);
#endif
			continue;
		}

		if (bInString) {
			if (c == '"' && (cn == ' ' || cn == '\0')) {
				bInString = false;
				continue;
			}
		} else {
			if (c == '"' && strBuffer.Length() == 0) {
				bInString = true;
				continue;
			}
			if (c == ' ' && strBuffer.Length() != 0) {
				astrResult.Push() = strBuffer;
				strBuffer = "";
				continue;
			}
		}

		strBuffer += c;
	}
#ifdef SCRATCH_NO_UTF8
	while (*(++sz) != '\0');
#else
	//while (*(sz = (char*)utf8codepoint(sz, nullptr)) != '\0');
	while (c != 0);
#endif

	if (strBuffer.Length() != 0) {
		astrResult.Push() = strBuffer;
	}
}

String String::InternalTrim(bool bLeft, bool bRight, s_char c) const
{
	// Copy ourselves into a new buffer
#ifdef SCRATCH_NO_UTF8
	size_t bytes = strlen(this->str_szBuffer);
#else
	size_t bytes = utf8size(this->str_szBuffer) - 1;
#endif
	char* szBuffer = new char[bytes + 1];

#ifdef SCRATCH_NO_UTF8
#if WINDOWS
	strcpy_s(szBuffer, bytes + 1, this->str_szBuffer);
#else
	strcpy(szBuffer, this->str_szBuffer);
#endif
#else
	utf8ncpy(szBuffer, this->str_szBuffer, bytes);
#endif

	// Keep a pointer to the current offset
	char* szOffset = szBuffer;

	if (bLeft) {
#ifdef SCRATCH_NO_UTF8
		// While there's a space, keep incrementing the offset
		char lc = *szOffset;
		while (lc == c && lc != '\0') {
			// This way, we'll trim all the spaces on the left
			lc = *(++szOffset);
		}
#else
		// Find the space
		int codepoint;
		void* v = utf8codepoint(szOffset, &codepoint);
		while (codepoint == c && codepoint != '\0') {
			szOffset = (char*)v;
			v = utf8codepoint(szOffset, &codepoint);
		}
#endif
	}

	if (bRight) {
		// Loop from right to left in the string
#ifdef SCRATCH_NO_UTF8
		for (int i = strlen(szOffset) - 1; i >= 0; i--) {
			// When we find a space
			if (szOffset[i] == c) {
				// Put the null terminator here to trim the right part
				szOffset[i] = '\0';
			} else {
				// Something other than a space, we can stop now
				break;
			}
		}
#else
		int codepoint;
		char* lastValid = szOffset;
		for (void* v = utf8codepoint(szOffset, &codepoint); ; v = utf8codepoint(v, &codepoint)) {
			if (codepoint == '\0') {
				break;
			}
			if (codepoint != c) {
				lastValid = (char*)v;
			}
		}
		*(char*)lastValid = '\0';
#endif
	}

	// Return
	String ret(szOffset);
	delete[] szBuffer;
	return ret;
}

String String::Trim() const
{
	MUTEX_LOCK(str_mutex);
	return InternalTrim(true, true);
}

String String::Trim(s_char c) const
{
	MUTEX_LOCK(str_mutex);
	return InternalTrim(true, true, c);
}

String String::TrimLeft() const
{
	MUTEX_LOCK(str_mutex);
	return InternalTrim(true, false);
}

String String::TrimLeft(s_char c) const
{
	MUTEX_LOCK(str_mutex);
	return InternalTrim(true, false, c);
}

String String::TrimRight() const
{
	MUTEX_LOCK(str_mutex);
	return InternalTrim(false, true);
}

String String::TrimRight(s_char c) const
{
	MUTEX_LOCK(str_mutex);
	return InternalTrim(false, true, c);
}

String String::Replace(const String &strNeedle, const String &strReplace) const
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strNeedle.str_mutex);
	MUTEX_LOCK_NAMED(wait3, strReplace.str_mutex);

	String strRet("");

	// Keep a pointer to the current offset and a "previous offset"
	char* szOffset = this->str_szBuffer;
	char* szOffsetPrev = szOffset;

	do {
		// Find the offset of the needle
#ifdef SCRATCH_NO_UTF8
		szOffset = strstr(szOffset, strNeedle);
#else
		szOffset = (char*)utf8str(szOffset, strNeedle);
#endif

		// If it's found
		if (szOffset != nullptr) {
			// Append everything before the needle of the original characters to the return value
			strRet.AppendToBuffer(szOffsetPrev, szOffset - szOffsetPrev);

			// Append the replace value
			strRet.AppendToBuffer(strReplace);

			// Increase the offset pointer by the needle size
#ifdef SCRATCH_NO_UTF8
			szOffset += strlen(strNeedle);
#else
			szOffset += utf8size(strNeedle) - 1;
#endif

			// Keep track of the pointer
			szOffsetPrev = szOffset;
		} else {
			// Append the remaining part of the source string
#ifdef SCRATCH_NO_UTF8
			size_t bytes = strlen(szOffsetPrev);
#else
			size_t bytes = utf8size(szOffsetPrev) - 1;
#endif
			strRet.AppendToBuffer(szOffsetPrev, bytes);
		}
	} while (szOffset != nullptr);

	return strRet;
}

String String::SubString(int iStart) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return String(this->str_szBuffer + iStart);
#else
	void* ret = this->str_szBuffer;
	s_char codepoint;
	do {
		ret = utf8codepoint(ret, &codepoint);
	} while (--iStart > 0 && codepoint != '\0');
	return (char*)ret;
#endif
}

String String::SubString(int iStart, int iLen) const
{
	MUTEX_LOCK(str_mutex);

	// Empty strings
	if (iStart < 0 || iLen <= 0) {
		return "";
	}

	// Get the first offset
#ifdef SCRATCH_NO_UTF8
	String strRet(this->str_szBuffer + iStart);
#else
	void* subFirst = this->str_szBuffer;
	s_char codepoint;
	do {
		subFirst = utf8codepoint(subFirst, &codepoint);
	} while (--iStart > 0 && codepoint != '\0');
	String strRet((const char*)subFirst);
#endif

	// Check for stupid developers
#ifdef SCRATCH_NO_UTF8
	if ((uint32_t)iLen > strlen(strRet)) {
		return strRet;
	}
#else
	if ((uint32_t)iLen > utf8len(strRet)) {
		return strRet;
	}
#endif

	// Then set the null terminator at the length the user wants
#ifdef SCRATCH_NO_UTF8
	strRet.str_szBuffer[iLen] = '\0';
#else
	void* sz = strRet.str_szBuffer;
	do {
		sz = utf8codepoint(sz, &codepoint);
	} while (--iLen > 0 && codepoint != '\0');
	*(char*)sz = '\0';
#endif

	// Return
	return strRet;
}

#if !WINDOWS
void strlwr(char* sz)
{
	int len = strlen(sz);
	for (int i = 0; i < len; i++) {
		sz[i] = tolower(sz[i]);
	}
}

void strupr(char* sz)
{
	int len = strlen(sz);
	for (int i = 0; i < len; i++) {
		sz[i] = toupper(sz[i]);
	}
}
#endif

String String::ToLower() const
{
	MUTEX_LOCK(str_mutex);

	String strRet(this->str_szBuffer);
#ifdef SCRATCH_NO_UTF8
#if WINDOWS
	int len = strlen(this->str_szBuffer);
	_strlwr_s(strRet.str_szBuffer, len + 1);
#else
	strlwr(strRet.str_szBuffer);
#endif
#else
	utf8lwr(strRet.str_szBuffer);
#endif
	return strRet;
}

String String::ToUpper() const
{
	MUTEX_LOCK(str_mutex);

	String strRet(this->str_szBuffer);
#ifdef SCRATCH_NO_UTF8
#if WINDOWS
	int len = strlen(this->str_szBuffer);
	_strupr_s(strRet.str_szBuffer, len + 1);
#else
	strupr(strRet.str_szBuffer);
#endif
#else
	utf8upr(strRet.str_szBuffer);
#endif
	return strRet;
}

int String::IndexOf(s_char c) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	const char* sz = strchr(this->str_szBuffer, c);
	if (sz != nullptr) {
		return sz - this->str_szBuffer;
	}
	return -1;
#else
	int ret = 0;
	void* sz = this->str_szBuffer;
	s_char codepoint;
	while (true) {
		sz = utf8codepoint(sz, &codepoint);
		if (codepoint == '\0') {
			return -1;
		} else if (codepoint == c) {
			return ret;
		}
		ret++;
	}
#endif
}

int String::IndexOf(const String &strNeedle) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	const char* sz = strstr(this->str_szBuffer, strNeedle);
	if (sz != nullptr) {
		return sz - this->str_szBuffer;
	}
	return -1;
#else
	void* szFound = utf8str(this->str_szBuffer, strNeedle);
	if (szFound == nullptr) {
		return -1;
	}
	int ret = 0;
	void* sz = this->str_szBuffer;
	s_char codepoint;
	while (sz != szFound) {
		sz = utf8codepoint(sz, &codepoint);
		if (codepoint == '\0') {
			return -1;
		}
		ret++;
	}
	return ret;
#endif
}

int String::IndexOfLast(s_char c) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	const char* sz = strrchr(this->str_szBuffer, c);
	if (sz != nullptr) {
		return sz - this->str_szBuffer;
	}
	return -1;
#else
	int ct = 0;
	int ret = -1;
	void* sz = this->str_szBuffer;
	int cp;
	while (true) {
		sz = utf8codepoint(sz, &cp);
		if (cp == 0) {
			break;
		} else if (cp == c) {
			ret = ct;
		}
		ct++;
	}
	return ret;
#endif
}

#ifdef SCRATCH_NO_UTF8
// strrstr taken from: http://stuff.mit.edu/afs/sipb/user/cordelia/Diplomacy/mapit/strrstr.c
static char* strrstr(const char* s1, const char* s2)
{
	const char *sc2, *psc1, *ps1;

	if (*s2 == '\0') {
		return (char*)s1;
	}

	ps1 = s1 + strlen(s1);

	while (ps1 != s1) {
		--ps1;
		for (psc1 = ps1, sc2 = s2; ; ) {
			if (*(psc1++) != *(sc2++)) {
				break;
			} else if (*sc2 == '\0') {
				return (char*)ps1;
			}
		}
	}
	return nullptr;
}
#endif

int String::IndexOfLast(const String &strNeedle) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	const char* sz = strrstr(this->str_szBuffer, strNeedle);
	if (sz != nullptr) {
		return sz - this->str_szBuffer;
	}
	return -1;
#else
	if (strNeedle.Length() == 0) {
		return -1;
	}

	int len = strNeedle.Length();
	int ret = -1;
	int index = 0;
	int findStart = -1;
	int findIndex = 0;

	int cp;
	void* p = this->str_szBuffer;

	while (true) {
		p = utf8codepoint(p, &cp);
		if (cp == 0) {
			break;
		}
		if (strNeedle[findIndex] == cp) {
			if (findIndex == 0) {
				findStart = index;
			}
			findIndex++;
			if (findIndex == len) {
				ret = findStart;
				findIndex = 0;
			}
		}
		index++;
	}

	return ret;
#endif
}

void String::Fill(s_char c, int ct)
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	char* szBuffer = new char[ct + 1];
	memset(szBuffer, c, ct);
	szBuffer[ct] = '\0';
	CopyToBuffer(szBuffer);
	delete[] szBuffer;
#else
	size_t charSize = utf8codepointsize(c);
	char* szChar = new char[charSize];
	utf8catcodepoint(szChar, c, charSize);
	size_t size = ct * charSize;
	char* szBuffer = new char[size + 1];
	for (int i = 0; i < ct; i++) {
		memcpy(szBuffer + i * charSize, szChar, charSize);
	}
	delete[] szChar;
	szBuffer[size] = '\0';
	CopyToBuffer(szBuffer);
	delete[] szBuffer;
#endif
}

bool String::Contains(const String &strNeedle) const
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strNeedle.str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strstr(this->str_szBuffer, strNeedle) != nullptr;
#else
	return utf8str(this->str_szBuffer, strNeedle) != nullptr;
#endif
}

bool String::Contains(s_char c) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	int iLen = strlen(str_szBuffer);
	for (int i = 0; i < iLen; i++) {
		if (str_szBuffer[i] == c) {
			return true;
		}
	}
	return false;
#else
	return utf8chr(str_szBuffer, c) != nullptr;
#endif
}

bool String::StartsWith(const String &strNeedle) const
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strNeedle.str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strstr(this->str_szBuffer, strNeedle) == this->str_szBuffer;
#else
	return utf8str(this->str_szBuffer, strNeedle) == this->str_szBuffer;
#endif
}

bool String::EndsWith(const String &strNeedle) const
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strNeedle.str_mutex);

	// Get the offset
#ifdef SCRATCH_NO_UTF8
	const char* szTemp = this->str_szBuffer + strlen(this->str_szBuffer) - strlen(strNeedle);
#else
	const char* szTemp = this->str_szBuffer + (utf8size(this->str_szBuffer) - 1) - (utf8size(strNeedle) - 1);
#endif

	// Make sure the needle is found
	if (szTemp == nullptr) {
		return false;
	}

	// Then compare the offset with our needle
#ifdef SCRATCH_NO_UTF8
	return !strcmp(strNeedle, szTemp);
#else
	return !utf8cmp(strNeedle, szTemp);
#endif
}

String::operator const char *()
{
	return this->str_szBuffer;
}

String::operator const char *() const
{
	return this->str_szBuffer;
}

String& String::operator=(char* src)
{
	MUTEX_LOCK(str_mutex);
	// Copy the right hand side to the buffer.
	this->CopyToBuffer(src);
	return *this;
}

String& String::operator=(const char* src)
{
	MUTEX_LOCK(str_mutex);
	// Copy the right hand side to the buffer.
	this->CopyToBuffer(src);
	return *this;
}

String& String::operator=(const String &strSrc)
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strSrc.str_mutex);
	// If the right hand side is not the left hand side...
	if (this != &strSrc) {
		// Copy the right hand side to the buffer.
		this->CopyToBuffer(strSrc);
	}
	return *this;
}

String& String::operator+=(const char* szSrc)
{
	MUTEX_LOCK(str_mutex);
	// Append the right hand side to the buffer.
	this->AppendToBuffer(szSrc);
	return *this;
}

String& String::operator+=(const s_char cSrc)
{
	MUTEX_LOCK(str_mutex);
	// Append the right hand side to the buffer.
	this->AppendToBuffer(cSrc);
	return *this;
}

String& String::operator+=(const String &strSrc)
{
	MUTEX_LOCK(str_mutex);
	MUTEX_LOCK_NAMED(wait2, strSrc.str_mutex);
	// Append the right hand side to the buffer.
	this->AppendToBuffer(strSrc.str_szBuffer);
	return *this;
}

String& String::operator*=(int ctRepeat)
{
	String strOriginal = *this;
	MUTEX_LOCK(str_mutex);
	for (int i = 1; i < ctRepeat; i++) {
		this->AppendToBuffer(strOriginal);
	}
	return *this;
}

bool String::operator==(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return !strcmp(this->str_szBuffer, szSrc);
#else
	return !utf8cmp(this->str_szBuffer, szSrc);
#endif
}

bool String::CaseCompare(int n, const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	int i = strcasecmp(str_szBuffer, szSrc);
#else
	int i = utf8casecmp(str_szBuffer, szSrc);
#endif

	return (n == i) || (n < 0 && i < 0) || (n > 0 && i > 0);
}

int String::CaseCompare(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strcasecmp(str_szBuffer, szSrc);
#else
	return utf8casecmp(str_szBuffer, szSrc);
#endif
}

bool String::operator>(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strcmp(str_szBuffer, szSrc) > 0;
#else
	return utf8cmp(str_szBuffer, szSrc) > 0;
#endif
}

bool String::operator>=(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strcmp(str_szBuffer, szSrc) >= 0;
#else
	return utf8cmp(str_szBuffer, szSrc) >= 0;
#endif
}

bool String::operator<(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strcmp(str_szBuffer, szSrc) < 0;
#else
	return utf8cmp(str_szBuffer, szSrc) < 0;
#endif
}

bool String::operator<=(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strcmp(str_szBuffer, szSrc) <= 0;
#else
	return utf8cmp(str_szBuffer, szSrc) <= 0;
#endif
}

bool String::operator!=(const char* szSrc) const
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return strcmp(this->str_szBuffer, szSrc) != 0;
#else
	return utf8cmp(this->str_szBuffer, szSrc) != 0;
#endif
}

const s_char String::operator[](int iIndex)
{
	MUTEX_LOCK(str_mutex);

#ifdef SCRATCH_NO_UTF8
	return this->str_szBuffer[iIndex];
#else
	void* sz = this->str_szBuffer;
	s_char codepoint;
	do {
		sz = utf8codepoint(sz, &codepoint);
		ASSERT(codepoint != '\0');
	} while (--iIndex > 0);
	return codepoint;
#endif
}

String operator+(const String &strLHS, const char* szRHS)
{
	return String(strLHS) += szRHS;
}

String operator+(const String &strLHS, const s_char cRHS)
{
	return String(strLHS) += cRHS;
}

String operator+(const char* szLHS, String &strRHS)
{
	return String(szLHS) += strRHS;
}

String operator+(const s_char cLHS, String &strRHS)
{
	return String(strRHS) += cLHS;
}

String operator*(const String &strLHS, int ctRepeat)
{
	return String(strLHS) *= ctRepeat;
}

String operator*(int ctRepeat, const String &strRHS)
{
	return String(strRHS) *= ctRepeat;
}

String strPrintF(const char* szFormat, ...)
{
	int iSize = STRING_FORMAT_BUFFER_SIZE;
	char* szBuffer = new char[iSize];

	// Get the args list and do a vsprintf to get the right format.
	va_list vL;
	va_start(vL, szFormat);

	// Make sure we don't overflow the buffer by checking against result length
	int iPrintSize = vsnprintf(szBuffer, iSize, szFormat, vL);
	if (iSize <= iPrintSize) {
		// Fail delete buffer and try again
		delete[] szBuffer;
		szBuffer = new char[iPrintSize + 1]; // +1 for \0
		vsnprintf(szBuffer, iPrintSize + 1, szFormat, vL);
	}
	va_end(vL);

	// Copy the just-created buffer to the main buffer
	String ret(szBuffer);

	// Clean up
	delete[] szBuffer;

	return ret;
}

#endif // include once check

SCRATCH_NAMESPACE_END;
