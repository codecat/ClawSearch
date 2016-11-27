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
#include "Stream.hpp"

SCRATCH_NAMESPACE_BEGIN;

class MemoryStream : public Stream
{
public:
	uint8_t* strm_pubBuffer;
	uint32_t strm_ulPosition;
	uint32_t strm_ulSize;
	uint32_t strm_ulUsed;

public:
	MemoryStream();
	MemoryStream(const MemoryStream &copy);
	~MemoryStream();

	uint32_t Size();
	uint32_t Location();
	void Seek(int32_t iPos, int32_t iOrigin);
	bool AtEOF();

	void Write(const void* p, uint32_t iLen);
	int Read(void* pDest, uint32_t iLen);

private:
	void AllocateMoreMemory(uint32_t ctBytes);
};

#ifdef SCRATCH_IMPL

MemoryStream::MemoryStream()
{
	strm_pubBuffer = nullptr;
	strm_ulPosition = 0;
	strm_ulSize = 0;
	strm_ulUsed = 0;
	AllocateMoreMemory(1024);
}

MemoryStream::MemoryStream(const MemoryStream &copy)
{
	strm_pubBuffer = nullptr;
	strm_ulPosition = 0;
	strm_ulSize = 0;
	strm_ulUsed = 0;
	AllocateMoreMemory(copy.strm_ulSize);
	memcpy(strm_pubBuffer, copy.strm_pubBuffer, copy.strm_ulSize);
	strm_ulPosition = copy.strm_ulPosition;
	strm_ulUsed = copy.strm_ulUsed;
}

MemoryStream::~MemoryStream()
{
	delete[] strm_pubBuffer;
}

uint32_t MemoryStream::Size()
{
	return strm_ulUsed;
}

uint32_t MemoryStream::Location()
{
	return strm_ulPosition;
}

void MemoryStream::Seek(int32_t iPos, int32_t iOrigin)
{
	switch (iOrigin) {
	case SEEK_CUR: strm_ulPosition += iPos; break;
	case SEEK_END: strm_ulPosition = Size() + iPos; break;
	case SEEK_SET: strm_ulPosition = iPos; break;
	}
}

bool MemoryStream::AtEOF()
{
	return Location() >= Size();
}

void MemoryStream::Write(const void* p, uint32_t iLen)
{
	// check if we need a larger buffer
	if (strm_ulPosition + iLen >= strm_ulSize) {
		AllocateMoreMemory(Max<uint32_t>(iLen, 1024));
	}

	// copy over memory
	memcpy(strm_pubBuffer + strm_ulPosition, p, iLen);

	// increase position
	strm_ulPosition += iLen;

	// update used counter
	strm_ulUsed = Max<uint32_t>(strm_ulPosition, strm_ulUsed);
}

int MemoryStream::Read(void* pDest, uint32_t iLen)
{
	uint32_t ulStart = strm_ulPosition;

	// increase position
	strm_ulPosition += iLen;

	// check boundaries
	if (strm_ulPosition > Size()) {
		strm_ulPosition = Size();
	}

	uint32_t ulRealLength = strm_ulPosition - ulStart;

	// copy data to destination
	memcpy(pDest, strm_pubBuffer + ulStart, ulRealLength);

	return ulRealLength;
}

void MemoryStream::AllocateMoreMemory(uint32_t ctBytes)
{
	ASSERT(ctBytes > 0);

	// create new buffer and remember old one
	uint8_t* pubNewBuffer = new uint8_t[strm_ulSize + ctBytes];
	uint8_t* pubOldBuffer = strm_pubBuffer;

	// if there's old memory to copy
	if (pubOldBuffer != nullptr && strm_ulSize > 0) {
		memcpy(pubNewBuffer, pubOldBuffer, strm_ulSize);
	}

	// increase the size count
	strm_ulSize += ctBytes;

	// set the new buffer pointer
	strm_pubBuffer = pubNewBuffer;

	// delete old memory
	if (pubOldBuffer != nullptr) {
		delete[] pubOldBuffer;
	}
}

#endif

SCRATCH_NAMESPACE_END;
