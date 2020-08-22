#pragma once

#define S2_USING_LIST

#include <cstdlib>
#include <cstring>
#include <new>
#include <initializer_list>

namespace s2
{
	template<typename T>
	class list;

	template<typename LT, typename T>
	class listiterator
	{
	private:
		LT* m_list;
		size_t m_index;

	public:
		listiterator(LT* list, size_t index)
		{
			m_list = list;
			m_index = index;
		}

		bool operator ==(const listiterator &other)
		{
			return !operator !=(other);
		}

		bool operator !=(const listiterator &other)
		{
			return m_list != other.m_list || m_index != other.m_index;
		}

		listiterator &operator ++()
		{
			m_index++;
			return *this;
		}

		T &operator *()
		{
			return (*m_list)[m_index];
		}
	};

	template<typename T>
	class list
	{
	public:
		typedef listiterator<list<T>, T> iterator;
		typedef listiterator<const list<T>, const T> constiterator;

	private:
		T* m_buffer;
		size_t m_length;
		size_t m_allocSize;

	public:
		list()
		{
			m_buffer = nullptr;
			m_length = 0;
			m_allocSize = 0;
		}

		list(const list &copy)
			: list()
		{
			size_t copylen = copy.len();
			ensure_memory(copylen);
			for (size_t i = 0; i < copylen; i++) {
				add(copy[i]);
			}
		}

		list(std::initializer_list<T> l)
			: list()
		{
			ensure_memory(l.size());
			for (const T &o : l) {
				add(o);
			}
		}

		~list()
		{
			clear_memory();
		}

		list &operator =(const list &copy)
		{
			clear_memory();
			size_t copylen = copy.len();
			ensure_memory(copylen);
			for (size_t i = 0; i < copylen; i++) {
				add(copy[i]);
			}
			return *this;
		}

		list &operator =(std::initializer_list<T> l)
		{
			clear_memory();
			ensure_memory(l.size());
			for (const T &o : l) {
				add(o);
			}
			return *this;
		}

		void clear()
		{
			for (size_t i = 0; i < m_length; i++) {
				m_buffer[i].~T();
			}
			m_length = 0;
		}

		size_t len() const
		{
			return m_length;
		}

		void add(const T &o)
		{
			ensure_memory(m_length + 1);
			new (m_buffer + m_length) T(o);
			m_length++;
		}

		T &add()
		{
			ensure_memory(m_length + 1);
			T* ret = new (m_buffer + m_length) T;
			m_length++;
			return *ret;
		}

		void insert(int index, const T &o)
		{
			if (index == m_length) {
				add(o);
				return;
			}

			ensure_memory(m_length + 1);
			memmove(m_buffer + index + 1, m_buffer + index, (m_length - index) * sizeof(T));
			new (m_buffer + index) T(o);
			m_length++;
		}

		T &insert(int index)
		{
			if (index == m_length) {
				return add();
			}

			ensure_memory(m_length + 1);
			memmove(m_buffer + index + 1, m_buffer + index, (m_length - index) * sizeof(T));
			T* ret = new (m_buffer + index) T;
			m_length++;
			return *ret;
		}

		void remove(size_t index)
		{
			if (index < 0 || index >= m_length) {
				return;
			}
			m_buffer[index].~T();
			if (index < m_length) {
				//NOTE: This is not safe if there are pointers to the items
				memmove(m_buffer + index, m_buffer + index + 1, (m_length - index) * sizeof(T));
			}
			m_length--;
		}

		T &push()
		{
			return add();
		}

		T pop()
		{
			T ret = top();
			remove(m_length - 1);
			return ret;
		}

		T &top()
		{
			return m_buffer[m_length - 1];
		}

		const T &top() const
		{
			return m_buffer[m_length - 1];
		}

		int indexof(const T &o) const
		{
			for (size_t i = 0; i < m_length; i++) {
				if (m_buffer[i] == o) {
					return (int)i;
				}
			}
			return -1;
		}

		bool contains(const T &o) const
		{
			return indexof(o) != -1;
		}

		void sort(int (*f)(const void *, const void *))
		{
			qsort(m_buffer, m_length, sizeof(T), f);
		}

		T &operator [](size_t index)
		{
			return m_buffer[index];
		}

		const T &operator [](size_t index) const
		{
			return m_buffer[index];
		}

		iterator begin()
		{
			return iterator(this, 0);
		}

		const constiterator begin() const
		{
			return constiterator(this, 0);
		}

		iterator end()
		{
			return iterator(this, m_length);
		}

		const constiterator end() const
		{
			return constiterator(this, m_length);
		}

		void ensure_memory(size_t count)
		{
			if (m_allocSize >= count) {
				return;
			}

			size_t resize = m_allocSize + m_allocSize / 2;
			if (resize < SIZE_MAX && resize > count) {
				count = resize;
			}

			m_buffer = (T*)realloc(m_buffer, count * sizeof(T));
			m_allocSize = count;
		}

	private:
		void clear_memory()
		{
			clear();
			if (m_buffer != nullptr) {
				free(m_buffer);
				m_buffer = nullptr;
				m_allocSize = 0;
			}
		}
	};
}
