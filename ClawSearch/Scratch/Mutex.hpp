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
#ifndef _MSC_VER
#include <pthread.h>
#endif

#include <errno.h>
#endif

#include "Common.h"

SCRATCH_NAMESPACE_BEGIN;

class Mutex
{
private:
	void* m_pMutex;
	bool m_bIsLocked;

public:
	Mutex();
	Mutex(const Mutex &copy); // copy constructor actually does the same as regular constructor.
	Mutex &operator=(const Mutex &copy);
	~Mutex();

	void Lock();
	bool TryLock();
	void Unlock();
};

class MutexWait
{
public:
	Mutex* m_pMutex;

public:
	MutexWait(const Mutex &mutex);
	~MutexWait();
};

#ifdef SCRATCH_IMPL

Mutex::Mutex()
{
#ifndef _MSC_VER
	m_pMutex = (void*)new pthread_mutex_t;
	pthread_mutex_init((pthread_mutex_t*)m_pMutex, nullptr);
#else
	m_pMutex = CreateMutex(0, false, 0);
#endif
	m_bIsLocked = false;
}

Mutex::Mutex(const Mutex &copy)
{
	operator=(copy);
}

Mutex &Mutex::operator=(const Mutex &copy)
{
	ASSERT(!copy.m_bIsLocked);
#ifndef _MSC_VER
	m_pMutex = (void*)new pthread_mutex_t;
	pthread_mutex_init((pthread_mutex_t*)m_pMutex, nullptr);
#else
	m_pMutex = CreateMutex(0, false, 0);
#endif
	m_bIsLocked = false;
	return *this;
}

Mutex::~Mutex()
{
	ASSERT(!m_bIsLocked);

#ifndef _MSC_VER
	pthread_mutex_destroy((pthread_mutex_t*)m_pMutex);
	delete (pthread_mutex_t*)m_pMutex;
#else
	CloseHandle(m_pMutex);
#endif
}

void Mutex::Lock()
{
#ifdef SCRATCH_DEBUG_MUTEX
	printf(TERMCOL_BOLDRED "[MUTEX]" TERMCOL_RESET " Lock: %p\n", m_pMutex);
#endif

#ifndef _MSC_VER
	pthread_mutex_lock((pthread_mutex_t*)m_pMutex);
#else
	WaitForSingleObject(m_pMutex, INFINITE);
#endif

	m_bIsLocked = true;
}

bool Mutex::TryLock()
{
	bool worked;

#ifndef _MSC_VER
	worked = pthread_mutex_trylock((pthread_mutex_t*)m_pMutex) != EBUSY;
#else
	worked = WaitForSingleObject(m_pMutex, 1) == WAIT_OBJECT_0;
	if (m_bIsLocked) return false;
#endif

	if (worked) {
		m_bIsLocked = true;
#ifdef SCRATCH_DEBUG_MUTEX
	printf(TERMCOL_BOLDRED "[MUTEX]" TERMCOL_RESET " TryLock: %p\n", m_pMutex);
#endif
	} else {
#ifdef SCRATCH_DEBUG_MUTEX
	printf(TERMCOL_BOLDRED "[MUTEX]" TERMCOL_RESET " TryLock failed: %p\n", m_pMutex);
#endif
	}

	return worked;
}

void Mutex::Unlock()
{
	ASSERT(m_bIsLocked);

#ifndef _MSC_VER
	pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);
#else
	ReleaseMutex(m_pMutex);
#endif

#ifdef SCRATCH_DEBUG_MUTEX
	printf(TERMCOL_BOLDRED "[MUTEX]" TERMCOL_RESET " Unlock: %p\n", m_pMutex);
#endif

	m_bIsLocked = false;
}

MutexWait::MutexWait(const Mutex &mutex)
{
	m_pMutex = &const_cast<Mutex&>(mutex);
	m_pMutex->Lock();
}

MutexWait::~MutexWait()
{
	m_pMutex->Unlock();
}

#endif

SCRATCH_NAMESPACE_END;
