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

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef _MSC_VER
SCRATCH_NAMESPACE_BEGIN;
BOOL _scratch_assert(const char* expr, const char* filename, unsigned lineno);
#ifdef SCRATCH_IMPL
#include "String.hpp"
BOOL _scratch_assert(const char* expr, const char* filename, unsigned lineno)
{
	String strText;

	char szProgname[MAX_PATH + 1];
	szProgname[MAX_PATH] = '\0';
	GetModuleFileName(nullptr, szProgname, MAX_PATH);

	strText.SetF(
		"Assertion Failed!\n\n"
		"Program: %s\n"
		"File: %s\n"
		"Line: %d\n\n"
		"Expression: %s\n\n"
		"(Press Retry to break)",
		szProgname, filename, lineno, expr);

	int nCode = MessageBox(nullptr, strText, "Scratch Assertion", MB_ABORTRETRYIGNORE | MB_ICONHAND | MB_SETFOREGROUND | MB_TASKMODAL);

	if (nCode == IDABORT) { exit(3); return FALSE; }
	if (nCode == IDRETRY) { return TRUE; }
	if (nCode == IDIGNORE) { return FALSE; }

	return FALSE;
}
#endif
SCRATCH_NAMESPACE_END;
// Windows
#define ASSERT(expr) {            \
	if(!(expr)) {                   \
		static unsigned char bWasHere = 0; \
		if(!bWasHere) {               \
			if(SCRATCH_NAMESPACE::_scratch_assert( \
				#expr,                    \
				__FILE__,                 \
				__LINE__)) {              \
				__asm { int 3 }           \
			}                           \
			bWasHere = 1;               \
		}                             \
	}                               \
}
#else
// Other sensible operating systems
#define ASSERT(expr) assert(expr)
#endif
