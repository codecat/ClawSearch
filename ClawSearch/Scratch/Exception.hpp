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

#ifndef SCRATCH_NO_EXCEPTIONS

#ifdef SCRATCH_IMPL

#include <stdio.h> // for vsprintf
#include <stdarg.h> // for va_list

#endif

#include "Common.h"
#include "String.hpp"

SCRATCH_NAMESPACE_BEGIN;

class Exception
{
public:
	String Message;

public:
	Exception();
	Exception(const char* format, ...);
	virtual ~Exception();
};

#ifdef SCRATCH_IMPL

Exception::Exception()
{
}

Exception::Exception(const char* format, ...)
{
	int iSize = 256;
	char* szBuffer = new char[iSize];

	// Get the args list and do a vsprintf to get the right format.
	va_list vL;
	va_start(vL, format);

	// Make sure we don't overflow the buffer by checking against result length
	int iPrintSize = vsnprintf(szBuffer, iSize, format, vL);
	if (iSize <= iPrintSize) {
		// Fail delete buffer and try again
		delete[] szBuffer;
		szBuffer = new char[iPrintSize + 1]; // +1 for \0
		vsnprintf(szBuffer, iPrintSize + 1, format, vL);
	}
	va_end(vL);

	// Copy the just-created buffer to the main buffer
	Message = szBuffer;

	// Clean up
	delete[] szBuffer;
}

Exception::~Exception()
{
}

#endif

#endif

SCRATCH_NAMESPACE_END;
