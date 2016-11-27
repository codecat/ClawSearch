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

#include <stdlib.h>
#include <string.h>

#ifndef SCRATCH_NO_THREADSAFE
#include "Mutex.hpp"
#endif

SCRATCH_NAMESPACE_BEGIN;

template<class Type>
class StackArray;

template<class Type>
class StackArrayIterator
{
public:
	StackArray<Type>* m_stack;
	int m_index;

public:
	StackArrayIterator(StackArray<Type>* stack, int index)
	{
		m_stack = stack;
		m_index = index;
	}

	bool operator!=(const StackArrayIterator<Type> &i)
	{
		return m_index != i.m_index;
	}

	StackArrayIterator<Type> &operator++()
	{
		m_index++;
		return *this;
	}

	Type &operator*()
	{
		return (*m_stack)[m_index];
	}
};

template<class Type>
class StackArray
{
public:
	Type** sa_pItems;
	int32_t sa_ctSlots;
	int32_t sa_ctUsed;
	bool sa_bOnlyPop;
#ifndef SCRATCH_NO_THREADSAFE
	Mutex sa_mutex;
#endif

public:
	StackArray();
	StackArray(const StackArray<Type> &copy); // Note: If this ever gets called, you're most likely writing bad code.
	StackArray<Type> &operator=(const StackArray<Type> &copy);
	~StackArray();

	/// Push to the beginning of the stack, return a reference to the newly made object
	Type& PushBegin();
	/// Push to the stack, return a reference to the newly made object
	Type& Push();
	/// Push a pointer to the stack
	void Push(Type* pObj);
	/// Pop top object from the stack
	Type* Pop();
	/// Pop a certain index from the stack
	Type* PopAt(int32_t iIndex);

	/// Pop all objects from the stack
	void PopAll();
	/// Clear all objects in the stack
	void Clear();

	/// Return how many objects there currently are in the stack
	int32_t Count();

	/// Find the index of the given object in the stack
	int32_t Find(const Type &obj);
	/// Find the index of the given pointer in the stack
	int32_t FindPointer(const Type* pObj);
	/// Find the index of the given condition in the stack
	template<typename Func>
	int32_t FindAny(Func f);

	/// Returns whether the given object is currently in the stack
	bool Contains(const Type &obj);
	/// Returns whether the given pointer is currently in the stack
	bool ContainsPointer(const Type* pObj);
	/// Returns whether the given condition is currently in the stack
	template<typename Func>
	bool ContainsAny(Func f);

	/// Sort the array ascending using operator >
	void SortAscending();
	/// Sort the array descending using operator >
	void SortDescending();
	/// Sort the array using a functor
	template<typename Func>
	void Sort(Func f);

	Type& operator[](int32_t iIndex);

	StackArrayIterator<Type> begin();
	StackArrayIterator<Type> end();

private:
	void AllocateSlots(int32_t ctSlots);
};

template<class Type>
StackArray<Type>::StackArray()
{
	sa_pItems = nullptr;
	sa_ctSlots = 0;
	sa_ctUsed = 0;
	sa_bOnlyPop = false;
	AllocateSlots(256);
}

template<class Type>
StackArray<Type>::StackArray(const StackArray<Type> &copy)
{
	operator=(copy);
}

template<class Type>
StackArray<Type> &StackArray<Type>::operator=(const StackArray<Type> &copy)
{
	sa_pItems = nullptr;
	sa_ctSlots = 0;
	sa_ctUsed = 0;
	sa_bOnlyPop = copy.sa_bOnlyPop;

	AllocateSlots(copy.sa_ctSlots);

	// copy meta information
	sa_ctUsed = copy.sa_ctUsed;

	// copy data to other slots
	for (int32_t i = 0; i < sa_ctUsed; i++) {
		// allocate memory for it
		sa_pItems[i] = new Type;

		// this should call the copy constructor
		*sa_pItems[i] = *copy.sa_pItems[i];
	}

	return *this;
}

template<class Type>
StackArray<Type>::~StackArray()
{
	if (sa_bOnlyPop) {
		PopAll();
	} else {
		Clear();
	}

	// free allocated memory for data
	if (sa_pItems != nullptr) {
		free(sa_pItems);
	}
}

template<class Type>
void StackArray<Type>::AllocateSlots(int32_t ctSlots)
{
	// allocate some memory
	sa_ctSlots += ctSlots;
	Type** pNewMem = (Type**)malloc(sizeof(Type*) * sa_ctSlots);

	// if we allocated memory before
	if (sa_pItems != nullptr) {
		// copy data to the new memory
		memcpy(pNewMem, sa_pItems, sizeof(Type*) * (sa_ctSlots - ctSlots));
		// free previously allocated memory
		free(sa_pItems);
	}

	// update the pointer
	sa_pItems = pNewMem;
}

/// Push to the beginning of the stack, return a reference to the newly made object
template<class Type>
Type& StackArray<Type>::PushBegin()
{
	MUTEX_LOCK(sa_mutex);

	// PushBegin() in combination with sa_bOnlyPop will cause memory leaking if not manually Clear()'d
	ASSERT(!sa_bOnlyPop);

	// if we need more slots
	if (sa_ctUsed >= sa_ctSlots) {
		// allocate some more
		AllocateSlots(256);
	}

	// create the new object
	Type* tNewObject = new Type;

	// make some room
	for (int i = sa_ctUsed; i > 0; i--) {
		sa_pItems[i] = sa_pItems[i - 1];
	}

	// push it onto the beginning of the stack
	sa_pItems[0] = tNewObject;

	// increase iterator
	sa_ctUsed++;

	// return new object
	return *tNewObject;
}

/// Push to the stack, return a reference to the newly made object
template<class Type>
Type& StackArray<Type>::Push()
{
	MUTEX_LOCK(sa_mutex);

	// Push() in combination with sa_bOnlyPop will cause memory leaking if not manually Clear()'d
	ASSERT(!sa_bOnlyPop);

	// if we need more slots
	if (sa_ctUsed >= sa_ctSlots) {
		// allocate some more
		AllocateSlots(256);
	}

	// create the new object
	Type* tNewObject = new Type;

	// push it onto the stack
	sa_pItems[sa_ctUsed] = tNewObject;

	// increase iterator
	sa_ctUsed++;

	// return new object
	return *tNewObject;
}

/// Push a pointer to the stack
template<class Type>
void StackArray<Type>::Push(Type* pObj)
{
	MUTEX_LOCK(sa_mutex);

	// if we need more slots
	if (sa_ctUsed >= sa_ctSlots) {
		// allocate some more
		AllocateSlots(256);
	}

	// push it onto the stack
	sa_pItems[sa_ctUsed] = pObj;

	// increase iterator
	sa_ctUsed++;
}

/// Pop top object from the stack
template<class Type>
Type* StackArray<Type>::Pop()
{
	MUTEX_LOCK(sa_mutex);

	ASSERT(sa_ctUsed > 0);

	// decrease iterator
	sa_ctUsed--;

	// get the item on top of the stack
	Type* tObject = sa_pItems[sa_ctUsed];

	// set the remaining pointer to nullptr (just to be sure)
	sa_pItems[sa_ctUsed] = nullptr;

	// return the object
	return tObject;
}

/// Pop a certain index from the stack
template<class Type>
Type* StackArray<Type>::PopAt(int32_t iIndex)
{
	MUTEX_LOCK(sa_mutex);

	ASSERT(iIndex < sa_ctUsed);

	// decrease iterator
	sa_ctUsed--;

	// get the item in the stack
	Type* tObject = sa_pItems[iIndex];

	// move pointers at the right one point to the left
	for (int32_t i = iIndex; i < sa_ctUsed; i++) {
		// set pointer to next one in stack
		sa_pItems[i] = sa_pItems[i + 1];
	}

	// set last pointer to nullptr (just in case)
	sa_pItems[sa_ctUsed] = nullptr;

	// return the object
	return tObject;
}

/// Pop all objects from the stack
template<class Type>
void StackArray<Type>::PopAll()
{
	MUTEX_LOCK(sa_mutex);

	// for every object
	for (int32_t i = 0; i < sa_ctUsed; i++) {
		// set remaining pointer to nullptr (just to be sure)
		sa_pItems[i] = nullptr;
	}

	// reset iterator to 0
	sa_ctUsed = 0;
}

/// Clear all objects in the stack
template<class Type>
void StackArray<Type>::Clear()
{
	MUTEX_LOCK(sa_mutex);

	// for every object
	for (int32_t i = 0; i < sa_ctUsed; i++) {
		// delete it
		delete sa_pItems[i];
		// set remaining pointer to nullptr (just to be sure)
		sa_pItems[i] = nullptr;
	}

	// reset iterator to 0
	sa_ctUsed = 0;
}

/// Return how many objects there currently are in the stack
template<class Type>
int32_t StackArray<Type>::Count()
{
	MUTEX_LOCK(sa_mutex);
	return sa_ctUsed;
}

/// Find the index of the given object in the stack
template<class Type>
int32_t StackArray<Type>::Find(const Type &obj)
{
	MUTEX_LOCK(sa_mutex);

	// for every object
	for (int32_t i = 0; i < sa_ctUsed; i++) {
		// test if it's the given one
		if (obj == *sa_pItems[i]) {
			return i;
		}
	}

	return -1;
}

/// Find the index of the given pointer in the stack
template<class Type>
int32_t StackArray<Type>::FindPointer(const Type* pObj)
{
	MUTEX_LOCK(sa_mutex);

	// for every object
	for (int32_t i = 0; i < sa_ctUsed; i++) {
		// test if it's the given one
		if (pObj == sa_pItems[i]) {
			return i;
		}
	}

	return -1;
}

/// Find the index of the given condition in the stack
template<class Type>
template<typename Func>
int32_t StackArray<Type>::FindAny(Func f)
{
	MUTEX_LOCK(sa_mutex);

	// for every object
	for (int32_t i = 0; i < sa_ctUsed; i++) {
		// test with function
		if (f(*sa_pItems[i])) {
			return i;
		}
	}
	return -1;
}

/// Returns whether the given object is currently in the stack
template<class Type>
bool StackArray<Type>::Contains(const Type &obj)
{
	return Find(obj) != -1;
}

/// Returns whether the given pointer is currently in the stack
template<class Type>
bool StackArray<Type>::ContainsPointer(const Type* pObj)
{
	return FindPointer(pObj) != -1;
}

/// Returns whether the given condition exists on any of the objects currently in the stack
template<class Type>
template<typename Func>
bool StackArray<Type>::ContainsAny(Func f)
{
	return FindAny(f) != -1;
}

template<typename T>
static int StackArray_cmpa(const void* a, const void* b)
{
	T &oa = **(T**)a;
	T &ob = **(T**)b;
	if (oa > ob) return 1;
	if (oa < ob) return -1;
	return 0;
}

template<typename T>
static int StackArray_cmpd(const void* a, const void* b)
{
	T &oa = **(T**)a;
	T &ob = **(T**)b;
	if (oa > ob) return -1;
	if (oa < ob) return 1;
	return 0;
}

/// Sort the array ascending using operator >
template<class Type>
void StackArray<Type>::SortAscending()
{
	qsort(sa_pItems, sa_ctUsed, sizeof(Type*), StackArray_cmpa<Type>);
}

/// Sort the array descending using operator >
template<class Type>
void StackArray<Type>::SortDescending()
{
	qsort(sa_pItems, sa_ctUsed, sizeof(Type*), StackArray_cmpd<Type>);
}

template<typename T>
inline void scratch_swap(T** base, int x, int y)
{
	T* tmp = base[y];
	base[y] = base[x];
	base[x] = tmp;
}

template<typename T, typename Func>
static void scratch_qsort(T** base, int left, int right, Func &f)
{
	if (left >= right) {
		return;
	}

	scratch_swap(base, left, (left + right) / 2);
	int last = left;
	for (int i = left + 1; i <= right; i++) {
		if (f(*base[i], *base[left]) < 0) {
			scratch_swap(base, ++last, i);
		}
	}
	scratch_swap(base, left, last);
	scratch_qsort(base, left, last - 1, f);
	scratch_qsort(base, last + 1, right, f);
}

/// Sort the array using a functor
template<class Type>
template<typename Func>
void StackArray<Type>::Sort(Func f)
{
	scratch_qsort<Type, Func>(sa_pItems, 0, sa_ctUsed - 1, f);
}

template<class Type>
Type& StackArray<Type>::operator[](int32_t iIndex)
{
	MUTEX_LOCK(sa_mutex);
	ASSERT(iIndex >= 0 && iIndex < sa_ctUsed);
	return *sa_pItems[iIndex];
}

template<class Type>
StackArrayIterator<Type> StackArray<Type>::begin()
{
	return StackArrayIterator<Type>(this, 0);
}

template<class Type>
StackArrayIterator<Type> StackArray<Type>::end()
{
	return StackArrayIterator<Type>(this, sa_ctUsed);
}

SCRATCH_NAMESPACE_END;
