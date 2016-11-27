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

/* Basic usage:
 *	 Include Scratch.hpp in at least 1 implementation file while SCRATCH_IMPL
 *	 is defined. For example:
 *
 *	 #define SCRATCH_IMPL
 *	 #include <Scratch.hpp>
 *	 using namespace Scratch;
 *
 * Other options:
 *	 There are a few specific precompiler definitions you can use during
 *	 implementation compilation (SCRATCH_IMPL) that modify the behavior of
 *	 the library.
 *
 *	 - SCRATCH_NO_THREADSAFE: Don't enable mutex locking in classes.
 *	 - SCRATCH_NO_EXCEPTIONS: Don't throw any exceptions, silently fail instead.
 *	 - SCRATCH_NO_ASSERT: Don't define a custom ASSERT().
 */

#include "Common.h"

/* String: high level string management
 * ------------------------------------
 * Basic usage:
 *	 String strFoo = "Scratch"; // Scratch
 *	 String strBar = "lib" + strFoo.ToLower(); // libscratch
 *	 String strTest = strBar.Replace("libs", "S") + "!"; // Scratch!
 *
 *	 StackArray<String> astrParse = strTest.Split("c");
 *	 ASSERT(astrParse.Count() == 3);
 *	 ASSERT(astrParse[0] == "S");
 *	 ASSERT(astrParse[1] == "rat");
 *	 ASSERT(astrParse[2] == "h");
 */
#include "String.hpp"

/* Filename: high level string management with filename functions
 * --------------------------------------------------------------
 * Basic usage:
 *	 Filename fnmFoo = "foo/bar/test.c";
 *	 String strExtension = fnmFoo.Extension();
 *	 String strPath = fnmFoo.Path();
 *	 String strFilename = fnmFoo.Filename();
 *	 ASSERT(strExtension == "c");
 *	 ASSERT(strPath == "foo/bar/");
 *	 ASSERT(strFilename == "test.c");
 */
#include "Filename.hpp"

/* Array: high level simple array management
 * -----------------------------------------
 * Basic usage:
 *   Array<int> aiTest;
 *   aiTest.Add(5);
 *   aiTest.Add(10);
 *   aiTest.Add(123);
 *   ASSERT(aiTest[0] == 5);
 *   aiTest.RemoveAt(1);
 *   ASSERT(aiTest[1] == 123);
 *   ASSERT(aiTest.Count() == 2);
 */
#include "Array.hpp"

/* StackArray: high level array management
 * ---------------------------------------
 * Basic usage:
 *	 StackArray<int> aiTest;
 *	 aiTest.Push() = 5;
 *	 aiTest.Push() = 10;
 *	 aiTest.Push() = 123;
 *	 ASSERT(aiTest[0] == 5);
 *	 ASSERT(aiTest.Pop() == 123);
 *	 ASSERT(aiTest.Count() == 2);
 */
#include "StackArray.hpp"

/* Dictionary: high level table management
 * ---------------------------------------
 * Basic usage:
 *	 Dictionary<String, String> dstrTest;
 *	 dstrTest.Add("Name", "libscratch");
 *	 dstrTest["Author"] = "Angelo Geels";
 */
#include "Dictionary.hpp"

/* FileStream: high level file stream management
 * ---------------------------------------------
 * Basic usage:
 *	 FileStream fs;
 *	 fs.Open("test.bin", "r+");
 *	 int32_t iTest;
 *	 fs >> iTest;
 *	 fs << iTest * 2;
 *	 fs.Close();
 */
#include "FileStream.hpp"

/* MemoryStream: high level memory stream management
 * -------------------------------------------------
 * Basic usage:
 *	 MemoryStream ms;
 *	 ms << int32_t(5);
 *	 FileStream fs;
 *	 fs.Open("test.bin", "w");
 *	 fs << ms.Size();
 *	 fs << ms;
 */
#include "MemoryStream.hpp"

/* NetworkStream: high level network connections management
 * ---------------------------------------------------------
 * Basic usage:
 *	 NetworkStream ns;
 *	 ns.Connect("127.0.0.1", 1234);
 *	 ns << int32_t(5);
 *	 int32_t iResult;
 *	 ns >> iResult;
 *	 ns.Close();
 */
#include "NetworkStream.hpp"

/* Mutex: high level mutex management
 * ----------------------------------
 * Basic usage:
 *	 Mutex mutex;
 *	 mutex.Lock();
 *	 // do some work
 *	 mutex.Unlock();
 *
 * Or:
 *	 Mutex mutex;
 *	 MutexWait(mutex);
 *	 // do some work
 *
 * Or: (define SCRATCH_DEBUG_MUTEX for additional debugging)
 *	 Mutex mutex;
 *	 MUTEX_LOCK(mutex);
 *	 // do some work
 */
#include "Mutex.hpp"

/* Exception: high level exception management
 * ------------------------------------------
 * Basic usage:
 *	 try {
 *		 throw Exception("Found %d items", 20);
 *	 } catch(Exception &ex) {
 *		 // do something with ex.Message
 *	 }
 */
#include "Exception.hpp"

/* Function: high level anonymous functions
 * ----------------------------------------
 * Basic usage:
 *   Function<int(int)> f = [](int x) {
 *     printf("Hello, world %d!\n", x);
 *     return x * 10;
 *   };
 *   int y = f(2);
 */
#include "Function.hpp"

/* Assert: nicer assertions
 * ------------------------
 * Basic usage:
 *	 ASSERT(iNumber == 10);
 */
#ifndef SCRATCH_NO_ASSERT
#include "Assert.hpp"
#endif
