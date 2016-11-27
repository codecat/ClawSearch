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
#include "StackArray.hpp"

SCRATCH_NAMESPACE_BEGIN;

template<class TKey, class TValue>
class Dictionary;

template<class TKey, class TValue>
class DictionaryPair
{
	friend class Dictionary<TKey, TValue>;
public:
	TKey key;
	TValue value;

public:
	DictionaryPair();
	DictionaryPair(const TKey& k);
	DictionaryPair(const TKey& k, const TValue& v);
	~DictionaryPair();

#ifndef SCRATCH_ALLOW_PAIR_COPY
private:
	/// This is most likely an unintended pair copy.. you can define
	/// SCRATCH_ALLOW_PAIR_COPY if this was intended.
	DictionaryPair(const DictionaryPair<TKey, TValue>& copy)
		: key(copy.key),
			value(copy.value)
	{
	}
#endif
};

template<class TKey, class TValue>
class Dictionary
{
private:
	StackArray<DictionaryPair<TKey, TValue>> dic_saPairs;

public:
	bool dic_bAllowDuplicateKeys;

public:
	Dictionary();
	Dictionary(const Dictionary<TKey, TValue> &copy);
	~Dictionary();

	/// Add to the dictionary
	void Add(const TKey &key, const TValue &value);
	/// Push to the dictionary
	DictionaryPair<TKey, TValue> &Push(const TKey &key);

	/// Get the index of the given key
	int32_t IndexByKey(const TKey &key);
	/// Get the index of the given value
	int32_t IndexByValue(const TValue &value);

	/// Does this dictionary have the given key?
	bool HasKey(const TKey &key);
	/// Does this dictionary have the given value?
	bool HasValue(const TValue &value);

	/// Remove a value by its index
	void RemoveByIndex(const int32_t iIndex);
	/// Remove a value from the dictionary by key
	void RemoveByKey(const TKey &key);
	/// Remove a value from the dictionary
	void RemoveByValue(const TValue &value);

	/// Pop a value by its index
	DictionaryPair<TKey, TValue> &PopByIndex(const int32_t iIndex);
	/// Pop a value from the dictionary by key
	DictionaryPair<TKey, TValue> &PopByKey(const TKey &key);
	/// Pop a value from the dictionary
	DictionaryPair<TKey, TValue> &PopByValue(const TValue &value);

	/// Clear all items
	void Clear();

	/// Return how many objects there currently are in the dictionary
	int32_t Count();

	TValue& operator[](const TKey &key);

	/// Get a pair from the dictionary using an index
	DictionaryPair<TKey, TValue> GetPair(const int32_t iIndex);
	/// Get a key from the dictionary using an index
	TKey& GetKeyByIndex(const int32_t iIndex);
	/// Get a value from the dictionary using an index
	TValue& GetValueByIndex(const int32_t iIndex);
};

template<class TKey, class TValue>
DictionaryPair<TKey, TValue>::DictionaryPair()
{
}

template<class TKey, class TValue>
DictionaryPair<TKey, TValue>::DictionaryPair(const TKey& k)
	: key(k)
{
}

template<class TKey, class TValue>
DictionaryPair<TKey, TValue>::DictionaryPair(const TKey& k, const TValue& v)
	: key(k),
	value(v)
{
}

template<class TKey, class TValue>
DictionaryPair<TKey, TValue>::~DictionaryPair()
{
}

template<class TKey, class TValue>
Dictionary<TKey, TValue>::Dictionary()
{
	dic_bAllowDuplicateKeys = false;
}

template<class TKey, class TValue>
Dictionary<TKey, TValue>::Dictionary(const Dictionary<TKey, TValue> &copy)
	: dic_saPairs(copy.dic_saPairs)
{
	dic_bAllowDuplicateKeys = copy.dic_bAllowDuplicateKeys;
}

template<class TKey, class TValue>
Dictionary<TKey, TValue>::~Dictionary()
{
	Clear();
}

/// Add to the dictionary
template<class TKey, class TValue>
void Dictionary<TKey, TValue>::Add(const TKey &key, const TValue &value)
{
	// if the key has already been added
	if (!dic_bAllowDuplicateKeys && HasKey(key)) {
		ASSERT(false);
		return;
	}

	// add it
	dic_saPairs.Push(new DictionaryPair<TKey, TValue>(key, value));
}

/// Push to the dictionary
template<class TKey, class TValue>
DictionaryPair<TKey, TValue> &Dictionary<TKey, TValue>::Push(const TKey &key)
{
	auto* ret = new DictionaryPair<TKey, TValue>(key);
	dic_saPairs.Push(ret);
	return *ret;
}

/// Get the index of the given key
template<class TKey, class TValue>
int32_t Dictionary<TKey, TValue>::IndexByKey(const TKey &key)
{
	int ctElements = Count();

	// find the right index
	int32_t iIndex = 0;
	for (; iIndex < ctElements; iIndex++) {
		if (dic_saPairs[iIndex].key == key) {
			// this is the item, we found the index
			break;
		}
	}

	// return index, or element count if it doesn't exist
	if (iIndex < ctElements) {
		return iIndex;
	}
	return -1;
}

/// Get the index of the given value
template<class TKey, class TValue>
int32_t Dictionary<TKey, TValue>::IndexByValue(const TValue &value)
{
	int ctElements = Count();

	// find the right index
	int32_t iIndex = 0;
	for (; iIndex < ctElements; iIndex++) {
		if (dic_saPairs[iIndex].value == value) {
			// this is the item, we found the index
			break;
		}
	}

	// return index, or -1 if it doesn't exist
	if (iIndex < ctElements) {
		return iIndex;
	}
	return -1;
}

/// Does this dictionary have the given key?
template<class TKey, class TValue>
bool Dictionary<TKey, TValue>::HasKey(const TKey &key)
{
	return IndexByKey(key) != -1;
}

/// Does this dictionary have the given value?
template<class TKey, class TValue>
bool Dictionary<TKey, TValue>::HasValue(const TValue &value)
{
	return IndexByValue(value) != -1;
}

/// Remove a value by its index
template<class TKey, class TValue>
void Dictionary<TKey, TValue>::RemoveByIndex(const int32_t iIndex)
{
	// check if someone passed an invalid range
	if (iIndex < 0 || iIndex >= Count()) {
		ASSERT(false);
		return;
	}
	// pop the values at that index
	delete dic_saPairs.PopAt(iIndex);
}

/// Remove a value from the dictionary by key
template<class TKey, class TValue>
void Dictionary<TKey, TValue>::RemoveByKey(const TKey &key)
{
	// remove by index
	RemoveByIndex(IndexByKey(key));
}

/// Remove a value from the dictionary
template<class TKey, class TValue>
void Dictionary<TKey, TValue>::RemoveByValue(const TValue &value)
{
	// remove by index
	RemoveByIndex(IndexByValue(value));
}

/// Pop a value by its index
template<class TKey, class TValue>
DictionaryPair<TKey, TValue> &Dictionary<TKey, TValue>::PopByIndex(const int32_t iIndex)
{
	return *(dic_saPairs.PopAt(iIndex));
}

/// Pop a value from the dictionary by key
template<class TKey, class TValue>
DictionaryPair<TKey, TValue> &Dictionary<TKey, TValue>::PopByKey(const TKey &key)
{
	return PopByIndex(IndexByKey(key));
}

/// Pop a value from the dictionary
template<class TKey, class TValue>
DictionaryPair<TKey, TValue> &Dictionary<TKey, TValue>::PopByValue(const TValue &value)
{
	return PopByIndex(IndexByValue(value));
}

/// Clear all items
template<class TKey, class TValue>
void Dictionary<TKey, TValue>::Clear()
{
	// clear pairs
	dic_saPairs.Clear();
}

/// Return how many objects there currently are in the dictionary
template<class TKey, class TValue>
int32_t Dictionary<TKey, TValue>::Count()
{
	// return the amount of pairs
	return dic_saPairs.Count();
}

template<class TKey, class TValue>
TValue& Dictionary<TKey, TValue>::operator[](const TKey &key)
{
	// get the index
	int32_t iIndex = IndexByKey(key);

	// if the key doesn't exist
	if (iIndex == -1) {
		// make a new pair
		auto &ret = Push(key);

		// and return the (empty) value
		return ret.value;
	}

	// return the value
	return dic_saPairs[iIndex].value;
}

/// Get a pair from the dictionary using an index
template<class TKey, class TValue>
DictionaryPair<TKey, TValue> Dictionary<TKey, TValue>::GetPair(const int32_t iIndex)
{
	return dic_saPairs[iIndex];
}

/// Get a key from the dictionary using an index
template<class TKey, class TValue>
TKey& Dictionary<TKey, TValue>::GetKeyByIndex(const int32_t iIndex)
{
	// return the key
	return dic_saPairs[iIndex].key;
}

/// Return value by index
template<class TKey, class TValue>
TValue& Dictionary<TKey, TValue>::GetValueByIndex(const int32_t iIndex)
{
	// return the value
	return dic_saPairs[iIndex].value;
}

SCRATCH_NAMESPACE_END;
