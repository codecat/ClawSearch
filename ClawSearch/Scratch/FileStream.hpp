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
#include "String.hpp"

SCRATCH_NAMESPACE_BEGIN;

class FileStream : public Stream
{
public:
	String fs_strFileName;
	FILE* fs_pfh;

public:
	FileStream();
	~FileStream();

	size_t Size();
	size_t Location();
	void Seek(int32_t iPos, int32_t iOrigin);
	bool AtEOF();

	bool Open(const char* szFileName, const char* szMode, bool sharedHandle = false);

	void OpenStdout();
	void OpenStdin();
	void OpenStderr();

	void Flush();

	void Close();
	void Write(const void* p, size_t iLen);
	size_t Read(void* pDest, size_t iLen);
	const void ReadToEnd(void* pDest);
};

#ifdef SCRATCH_IMPL

#if WINDOWS
#include <share.h>
#endif

FileStream::FileStream()
{
	fs_pfh = nullptr;
}

FileStream::~FileStream()
{
	Close();
}

size_t FileStream::Size()
{
	size_t ulPos = Location();
	Seek(0, SEEK_END);
	size_t ulSize = Location();
	Seek((int32_t)ulPos, SEEK_SET);
	return ulSize;
}

size_t FileStream::Location()
{
	return ftell(fs_pfh);
}

void FileStream::Seek(int32_t iPos, int32_t iOrigin)
{
	fseek(fs_pfh, iPos, iOrigin);
}

bool FileStream::AtEOF()
{
	return feof(fs_pfh) > 0;
}

bool FileStream::Open(const char* szFileName, const char* szMode, bool sharedHandle)
{
	// must not already have a handle open
	ASSERT(fs_pfh == nullptr);

#if WINDOWS
	// open file on Windows
	FILE* pfh = nullptr;
#ifndef SCRATCH_NO_UTF8
	size_t sizeFileName = strlen(szFileName) + 1;
	wchar_t* wszFileName = (wchar_t*)malloc(sizeof(wchar_t) * sizeFileName);
	mbstowcs(wszFileName, szFileName, sizeFileName);

	size_t sizeMode = strlen(szMode) + 1;
	wchar_t* wszMode = (wchar_t*)malloc(sizeof(wchar_t) * sizeMode);
	mbstowcs(wszMode, szMode, sizeMode);
#endif
	if (sharedHandle) {
#ifdef SCRATCH_NO_UTF8
		pfh = _fsopen(szFileName, szMode, _SH_DENYWR);
#else
		pfh = _wfsopen(wszFileName, wszMode, _SH_DENYWR);
#endif
	} else {
#ifdef SCRATCH_NO_UTF8
		fopen_s(&pfh, szFileName, szMode);
#else
		_wfopen_s(&pfh, wszFileName, wszMode);
#endif
	}
#ifndef SCRATCH_NO_UTF8
	free(wszFileName);
	free(wszMode);
#endif

#else
	// open file on linux
	FILE* pfh = fopen(szFileName, szMode);
#endif

	// it might not exist
	if (pfh == nullptr) {
		return false;
	}

	// remember info
	fs_strFileName = szFileName;
	fs_pfh = pfh;

	// success
	return true;
}

void FileStream::OpenStdout()
{
	fs_strFileName = "stdout";
	fs_pfh = stdout;
}

void FileStream::OpenStdin()
{
	fs_strFileName = "stdin";
	fs_pfh = stdin;
}

void FileStream::OpenStderr()
{
	fs_strFileName = "stderr";
	fs_pfh = stderr;
}

void FileStream::Flush()
{
	fflush(fs_pfh);
}

void FileStream::Close()
{
	// close the file handle
	if (fs_pfh != nullptr) {
		fclose(fs_pfh);
		fs_pfh = nullptr;
	}
}

void FileStream::Write(const void* p, size_t iLen)
{
	fwrite(p, 1, iLen, fs_pfh);
}

size_t FileStream::Read(void* pDest, size_t iLen)
{
	return fread(pDest, 1, iLen, fs_pfh);
}

#endif

SCRATCH_NAMESPACE_END;
