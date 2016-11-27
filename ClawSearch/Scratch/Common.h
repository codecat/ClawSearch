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

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

#ifdef _MSC_VER
	#ifndef AF_IPX
		#include <WinSock2.h>
	#endif
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <Windows.h>
	#define WINDOWS 1
#else
	#define WINDOWS 0
#endif

#ifdef __APPLE__
	#define MACOSX 1
#else
	#define MACOSX 0
#endif

#ifndef SCRATCH_NAMESPACE
	#define SCRATCH_NAMESPACE Scratch
#endif

#define SCRATCH_NAMESPACE_BEGIN namespace SCRATCH_NAMESPACE {
#define SCRATCH_NAMESPACE_END }

#ifndef SCRATCH_NO_GLOBFUNC
template<class T> inline T Max(const T &v1, const T &v2) { return v1 >= v2 ? v1 : v2; }
template<class T> inline T Min(const T &v1, const T &v2) { return v1 <= v2 ? v1 : v2; }
template<class T> inline void Swap(T &v1, T &v2) { T t = v1; v1 = v2; v2 = t; }

template<class T> inline T Abs(const T &v) { return v < 0 ? -v : v; }
#endif

#ifndef ASSERT
#define ASSERT assert
#endif

#ifdef SCRATCH_DEBUG_MUTEX
#define MUTEX_DEBUG_LOCK(name) printf(TERMCOL_BOLDRED "[MUTEX]" TERMCOL_RESET " Lock '%s' from: %s (@ %s:%d)\n", name, __PRETTY_FUNCTION__, __FILE__, __LINE__);
#else
#define MUTEX_DEBUG_LOCK(name)
#endif

#ifdef SCRATCH_NO_THREADSAFE
#define MUTEX_LOCK(mutex)
#define MUTEX_LOCK_NAMED(name, mutex)
#else
#define MUTEX_LOCK(mutex) MUTEX_DEBUG_LOCK("wait"); MutexWait wait(mutex);
#define MUTEX_LOCK_NAMED(name, mutex) MUTEX_DEBUG_LOCK(#name); MutexWait name(mutex);
#endif

#ifndef SCRATCH_NO_TERMCOL
	#if !WINDOWS
		#define TERMCOL_RESET   "\033[0m"
		#define TERMCOL_BLACK   "\033[30m"
		#define TERMCOL_RED     "\033[31m"
		#define TERMCOL_GREEN   "\033[32m"
		#define TERMCOL_YELLOW  "\033[33m"
		#define TERMCOL_BLUE    "\033[34m"
		#define TERMCOL_MAGENTA "\033[35m"
		#define TERMCOL_CYAN    "\033[36m"
		#define TERMCOL_WHITE   "\033[37m"
		#define TERMCOL_BOLDBLACK   "\033[1m\033[30m"
		#define TERMCOL_BOLDRED     "\033[1m\033[31m"
		#define TERMCOL_BOLDGREEN   "\033[1m\033[32m"
		#define TERMCOL_BOLDYELLOW  "\033[1m\033[33m"
		#define TERMCOL_BOLDBLUE    "\033[1m\033[34m"
		#define TERMCOL_BOLDMAGENTA "\033[1m\033[35m"
		#define TERMCOL_BOLDCYAN    "\033[1m\033[36m"
		#define TERMCOL_BOLDWHITE   "\033[1m\033[37m"
	#else
		#define TERMCOL_RESET
		#define TERMCOL_BLACK
		#define TERMCOL_RED
		#define TERMCOL_GREEN
		#define TERMCOL_YELLOW
		#define TERMCOL_BLUE
		#define TERMCOL_MAGENTA
		#define TERMCOL_CYAN
		#define TERMCOL_WHITE
		#define TERMCOL_BOLDBLACK
		#define TERMCOL_BOLDRED
		#define TERMCOL_BOLDGREEN
		#define TERMCOL_BOLDYELLOW
		#define TERMCOL_BOLDBLUE
		#define TERMCOL_BOLDMAGENTA
		#define TERMCOL_BOLDCYAN
		#define TERMCOL_BOLDWHITE
	#endif
#endif
