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
#if WINDOWS
#include <ShlObj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif
#endif

#include "String.hpp"

SCRATCH_NAMESPACE_BEGIN;

class Filename : public String
{
public:
	Filename();
	Filename(const Filename &copy);
	Filename(const char* szStr);
	Filename(const String &str);

	String Extension() const;
	String Path() const;
	String PathName() const;
	String Name() const;

	void FromHome(const String &strPath);
};

#ifdef SCRATCH_IMPL

Filename::Filename()
{
	str_iInstances++;
	// Create a new empty buffer
	this->str_szBuffer = String::str_szEmpty;
}

Filename::Filename(const Filename &copy)
{
	str_iInstances++;
	// Create a new buffer and copy data into it.
	this->str_szBuffer = String::str_szEmpty;
	this->CopyToBuffer(copy);
}

Filename::Filename(const char* szStr)
{
	str_iInstances++;
	// Create a new buffer and copy data into it.
	this->str_szBuffer = String::str_szEmpty;
	this->CopyToBuffer(szStr);
}

Filename::Filename(const String &str)
{
	str_iInstances++;
	// Create a new buffer and copy data into it.
	this->str_szBuffer = String::str_szEmpty;
	this->CopyToBuffer(str.str_szBuffer);
}

String Filename::Extension() const
{
	char* sz = strrchr(str_szBuffer, '.');
	if (sz == nullptr) {
		return "";
	}
	return String(sz + 1);
}

String Filename::Path() const
{
	int index = IndexOfLast('/');
	return String(str_szBuffer, 0, index + 1);
}

String Filename::PathName() const
{
	String ret = Path().TrimRight('/');
	int index = ret.IndexOfLast('/');
	return String(ret.str_szBuffer + index + 1);
}

String Filename::Name() const
{
	int index = IndexOfLast('/');
	return String(str_szBuffer + index + 1);
}

void Filename::FromHome(const String &strPath)
{
#if WINDOWS
	char szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_PROFILE, nullptr, 0, szPath))) {
		this->CopyToBuffer(szPath);
		this->AppendToBuffer("\\" + strPath);
	}
#else
	const char* homedir;
	if ((homedir = getenv("HOME")) == nullptr) {
		homedir = getpwuid(getuid())->pw_dir;
	}
	if (homedir == nullptr) {
		this->CopyToBuffer("~/" + strPath);
	} else {
		this->CopyToBuffer(String(homedir) + "/" + strPath);
	}
#endif
}

#endif

SCRATCH_NAMESPACE_END;
