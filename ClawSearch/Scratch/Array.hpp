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

SCRATCH_NAMESPACE_BEGIN;

template<typename T, int _AllocStep = 4>
class Array
{
private:
	T* m_buffer;
	int m_used;
	int m_allocated;

public:
	Array()
	{
		m_buffer = nullptr;
		m_used = 0;
		m_allocated = 0;
	}

	Array(const Array<T> &copy)
	{
		int size = sizeof(T) * copy.m_used;

		m_buffer = (T*)malloc(size);
		memcpy(m_buffer, copy.m_buffer, size);

		m_used = copy.m_used;
		m_allocated = size;
	}

	~Array()
	{
		if (m_buffer != nullptr) {
			free(m_buffer);
		}
	}

	inline int Count() const { return m_used; }

	inline void Add(const T &obj)
	{
		Add() = obj;
	}

	inline T &Add()
	{
		m_used++;

		if (m_buffer == nullptr) {
			m_buffer = (T*)malloc(sizeof(T) * _AllocStep);
			m_allocated += _AllocStep;
		} else if (m_used > m_allocated) {
			m_buffer = (T*)realloc(m_buffer, sizeof(T) * (m_allocated + _AllocStep));
			m_allocated += _AllocStep;
		}

		return m_buffer[m_used - 1];
	}

	inline void Clear()
	{
		if (m_buffer == nullptr) {
			return;
		}

		m_used = 0;
	}

	inline void RemoveAt(int index)
	{
		ASSERT(index < m_used);

		if (index == m_used - 1) {
			m_used--;
			return;
		}

		//TODO: This is slow, but I'm not sure how memmove/memcpy works if addresses overlap
		for (int i = index; i < m_used; i++) {
			m_buffer[i] = m_buffer[i + 1];
		}
		m_used--;
	}

	inline int Find(const T &obj)
	{
		for (int i = 0; i < m_used; i++) {
			if (m_buffer[i] == obj) {
				return i;
			}
		}
		return -1;
	}

	inline T &operator[](int index)
	{
		ASSERT(index < m_used);
		return m_buffer[index];
	}

	inline const T &operator[](int index) const
	{
		ASSERT(index < m_used);
		return m_buffer[index];
	}
};

SCRATCH_NAMESPACE_END;
