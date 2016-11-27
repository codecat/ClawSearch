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

#include "Common.h"
#include "String.hpp"

SCRATCH_NAMESPACE_BEGIN;

enum ENewLineMode
{
	ENLM_CRLF,
	ENLM_LF,
	ENLM_CR,
};

class Stream
{
public:
	ENewLineMode strm_nlmNewLineMode;

public:
	Stream();
	~Stream();

	virtual uint32_t Size() = 0;
	virtual uint32_t Location() = 0;
	virtual void Seek(int32_t iPos, int32_t iOrigin) = 0;
	virtual bool AtEOF() = 0;

	virtual void Flush();
	virtual void Close();

	virtual void Write(const void* p, uint32_t iLen) = 0;
	inline void WriteIndex(const int32_t &i) { Write(&i, sizeof(int32_t)); }
	inline void WriteLong(const int64_t &l)	{ Write(&l, sizeof(int64_t)); }
	inline void WriteFloat(const float &f)	 { Write(&f, sizeof(float)); }
	inline void WriteDouble(const double &d) { Write(&d, sizeof(double)); }
	void WriteString(const String &str);
	void WriteStream(Stream &strm);

	virtual int Read(void* pDest, uint32_t iLen) = 0;
	void ReadToEnd(void* pDest);
	inline int32_t ReadIndex()	{ int32_t i = 0; Read(&i, sizeof(int32_t)); return i; }
	inline int64_t ReadLong()	 { int64_t l = 0; Read(&l, sizeof(int64_t)); return l; }
	inline float	 ReadFloat()	{ float	 f = 0; Read(&f, sizeof(float)); return f; }
	inline double	ReadDouble() { double	d = 0; Read(&d, sizeof(double)); return d; }
	String ReadString();

	inline char ReadChar() { char c = '\0'; Read(&c, 1); return c; }
	inline char PeekChar() { char c = '\0'; Read(&c, 1); Seek(-1, 1/*SEEK_CUR*/); return c; }

	bool Expect(const String &str);
	char ReadUntil(String &strOut, const String &strCharacters);

	void WriteText(const String &str);
	void WriteLine(const String &str);
	void WriteLine();
	String ReadLine();

	inline Stream& operator <<(int32_t i)		{ WriteIndex(i); return *this; }
	inline Stream& operator <<(float f)			{ WriteFloat(f); return *this; }
	inline Stream& operator <<(double d)		 { WriteDouble(d); return *this; }
	inline Stream& operator <<(String str)	 { WriteString(str); return *this; }
	inline Stream& operator <<(Stream &strm) { WriteStream(strm); return *this; }

	inline Stream& operator >>(int32_t &i)	 { i = ReadIndex(); return *this; }
	inline Stream& operator >>(float &f)		 { f = ReadFloat(); return *this; }
	inline Stream& operator >>(double &d)		{ d = ReadDouble(); return *this; }
	inline Stream& operator >>(String &str)	{ str = ReadString(); return *this; }
};

#ifdef SCRATCH_IMPL

Stream::Stream()
{
	strm_nlmNewLineMode = ENLM_LF;
}

Stream::~Stream()
{
	Close();
}

void Stream::Flush()
{
}

void Stream::Close()
{
}

void Stream::WriteString(const String &str)
{
	Write((const char*)str, strlen(str) + 1);
}

void Stream::WriteStream(Stream &strm)
{
	// get buffer size
	uint32_t ulBufferSize = strm.Size() - strm.Location();
	uint32_t ulBytesLeft = ulBufferSize;

	// allocate memory for chunks
	uint32_t ulChunkSize = 1024;
	void* pBuffer = malloc(ulChunkSize);

	// loop through chunks required to write
	while (ulBytesLeft > 0) {
		// check for end of stream
		if (ulBytesLeft < ulChunkSize) {
			ulChunkSize = ulBytesLeft;
		}
		// read chunk from other stream
		strm.Read(pBuffer, ulChunkSize);
		// write chunk into our stream
		Write(pBuffer, ulChunkSize);
	}

	// we're done, deallocate buffer
	free(pBuffer);
}

void Stream::ReadToEnd(void* pDest)
{
	Read(pDest, Size() - Location());
}

String Stream::ReadString()
{
	String strRet;
	char c;
	do {
		// read 1 character
		Read(&c, sizeof(char));

		// if it's not the terminator
		if (c != '\0') {
			// he'll be back
			strRet += c;
		}
	} while (c != '\0');

	// done, return
	return strRet;
}

bool Stream::Expect(const String &str)
{
	int iLen = strlen(str);

	char* szBuffer = new char[iLen + 1];
	szBuffer[iLen] = '\0';

	Read(szBuffer, iLen);
	bool ret = (str == szBuffer);

	if (!ret) {
		Seek(-iLen, SEEK_CUR);
	}

	delete[] szBuffer;
	return ret;
}

char Stream::ReadUntil(String &strOut, const String &strCharacters)
{
	String ret;
	char ccc = '\0';
	while (!AtEOF()) {
		char cc = ReadChar();
		if (strCharacters.Contains(cc)) {
			ccc = cc;
			break;
		}
		ret += cc;
	}
	strOut = ret;
	return ccc;
}

void Stream::WriteText(const String &str)
{
	// write text
	Write((const char*)str, str.Size());
}

void Stream::WriteLine(const String &str)
{
	// write text
	WriteText(str);
	// write newline
	WriteLine();
}

void Stream::WriteLine()
{
	// write newline
	switch (strm_nlmNewLineMode) {
	case ENLM_CRLF: Write("\r\n", 2); break;
	case ENLM_LF: Write("\n", 1); break;
	case ENLM_CR: Write("\r", 1); break;
	}
}

String Stream::ReadLine()
{
	String strRet;
	char c;
	do {
		// read 1 character
		Read(&c, sizeof(char));

		// if at EOF
		if (AtEOF()) {
			// we're done here
			break;
		}

		// skip \r
		if (c == '\r') {
			//WARNING: This can potentially cause problems on certain operating systems
			//				 that work with \r _only_ as a newline character. We should incorporate
			//				 strm_nlmNewLineMode with this in a future version.
			continue;
		}

		// if it's not the terminator
		if (c != '\n') {
			// he'll be back
			strRet += c;
		}
	} while (c != '\n');

	// done, return
	return strRet;
}

#endif

SCRATCH_NAMESPACE_END;
