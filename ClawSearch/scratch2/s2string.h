#pragma once

#define S2_USING_STRING

#include <cstddef>
#include <cstdint>

namespace s2
{
	class stringsplit;

	class string
	{
	friend class stringsplit;

	private:
		char* m_buffer;
		size_t m_length;
		size_t m_allocSize;

	public:
		string();
		string(const char* sz);
		string(const char* sz, size_t len);
		string(const char* sz, size_t start, size_t len);
		string(const string &str);
		~string();

		size_t len() const;
		size_t allocsize() const;
		const char* c_str() const;

		int indexof(char c) const;
		int indexof(const char* sz) const;

		bool contains(const char* sz) const;
		bool startswith(const char* sz) const;
		bool endswith(const char* sz) const;
		stringsplit split(const char* delim, int limit = 0) const;
		stringsplit commandlinesplit() const;

		string substr(intptr_t start) const;
		string substr(intptr_t start, intptr_t len) const;

		void append(char c);

		void append(const char* sz);
		void append(const char* sz, size_t len);
		void append(const char* sz, size_t start, size_t len);

		void insert(const char* sz, size_t pos);
		void insert(const char* sz, size_t pos, size_t len);
		void remove(size_t pos, size_t len);

		string replace(char find, char replace) const;
		string replace(const char* find, const char* replace) const;

		void setf(const char* format, ...);
		void appendf(const char* format, ...);

		string &operator =(const char* sz);
		string &operator =(const string &str);

		string &operator +=(const char* sz);
		string &operator +=(const string &str);

		string operator +(const char* sz);
		string operator +(const string &str);

		string trim() const;
		string trim(const char* sz) const;

		string tolower() const;
		string toupper() const;

		bool operator ==(const char* sz) const;
		bool operator ==(const string &str) const;

		bool operator !=(const char* sz) const;
		bool operator !=(const string &str) const;

		operator const char*() const;

		char &operator [](int index);
		const char &operator [](int index) const;

		void ensure_memory(size_t size);

	private:
		void resize_memory(size_t size);
	};

	bool operator ==(const char* sz, const string &str);

	string operator +(const char* lhs, const string &rhs);

	string strprintf(const char* format, ...);

	class stringsplit
	{
	private:
		char** m_buffer = nullptr;
		size_t m_length = 0;

	public:
		stringsplit(const char* sz, const char* delim, int limit = 0);
		stringsplit(const char* sz, bool commandLine);
		stringsplit(const stringsplit &copy);
		~stringsplit();

		size_t len() const;
		string operator[](size_t index) const;

	private:
		void add(const char* sz, size_t len);
	};
}

#ifdef S2_IMPL
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cctype>

const size_t min_buffer_size = 24;

s2::string::string()
{
	m_length = 0;
	m_buffer = nullptr;
	resize_memory(1);
	m_buffer[0] = '\0';
}

s2::string::string(const char* sz)
	: string(sz, 0, strlen(sz))
{
}

s2::string::string(const char* sz, size_t len)
	: string(sz, 0, len)
{
}

s2::string::string(const char* sz, size_t start, size_t len)
{
	m_length = len;
	m_buffer = nullptr;
	resize_memory(len + 1);
	memcpy(m_buffer + start, sz, len);
	m_buffer[len] = '\0';
}

s2::string::string(const s2::string &str)
	: string(str.m_buffer)
{
}

s2::string::~string()
{
	free(m_buffer);
}

size_t s2::string::len() const
{
	return m_length;
}

size_t s2::string::allocsize() const
{
	return m_allocSize;
}

const char* s2::string::c_str() const
{
	return m_buffer;
}

int s2::string::indexof(char c) const
{
	char* p = m_buffer;
	while (*p != '\0') {
		if (*p == c) {
			return (int)(p - m_buffer);
		}
		p++;
	}
	return -1;
}

int s2::string::indexof(const char* sz) const
{
	char* p = strstr(m_buffer, sz);
	if (p == nullptr) {
		return -1;
	}
	return (int)(p - m_buffer);
}

bool s2::string::contains(const char* sz) const
{
	return strstr(m_buffer, sz) != nullptr;
}

bool s2::string::startswith(const char* sz) const
{
	return strstr(m_buffer, sz) == m_buffer;
}

bool s2::string::endswith(const char* sz) const
{
	return !strcmp(m_buffer + m_length - strlen(sz), sz);
}

s2::stringsplit s2::string::split(const char* delim, int limit) const
{
	return stringsplit(m_buffer, delim, limit);
}

s2::stringsplit s2::string::commandlinesplit() const
{
	return stringsplit(m_buffer, true);
}

s2::string s2::string::substr(intptr_t start) const
{
	if (m_length == 0) {
		return "";
	}
	while (start < 0) {
		start += (intptr_t)m_length;
	}
	if ((size_t)start >= m_length) {
		return "";
	}
	return string(m_buffer + start);
}

s2::string s2::string::substr(intptr_t start, intptr_t len) const
{
	if (m_length == 0) {
		return "";
	}
	while (start < 0) {
		start += (intptr_t)m_length;
	}
	if ((size_t)start >= m_length) {
		return "";
	}
	intptr_t remainder = (intptr_t)strlen(m_buffer) + start;
	if (len > remainder) {
		len = remainder;
	}
	return string(m_buffer + start, len);
}

void s2::string::append(char c)
{
	append(&c, 0, 1);
}

void s2::string::append(const char* sz)
{
	append(sz, 0, strlen(sz));
}

void s2::string::append(const char* sz, size_t len)
{
	append(sz, 0, len);
}

void s2::string::append(const char* sz, size_t start, size_t len)
{
	ensure_memory(m_length + len + 1);
	memcpy(m_buffer + m_length, sz + start, len);
	m_length += len;
	m_buffer[m_length] = '\0';
}

void s2::string::insert(const char* sz, size_t pos)
{
	insert(sz, pos, strlen(sz));
}

void s2::string::insert(const char* sz, size_t pos, size_t len)
{
	ensure_memory(m_length + len + 1);
	memmove(m_buffer + pos + len, m_buffer + pos, m_length - pos);
	memcpy(m_buffer + pos, sz, len);
	m_length += len;
	m_buffer[m_length] = '\0';
}

void s2::string::remove(size_t pos, size_t len)
{
	memmove(m_buffer + pos, m_buffer + pos + len, m_length - pos - len);
	m_length -= len;
	m_buffer[m_length] = '\0';
}

s2::string s2::string::replace(char find, char replace) const
{
	s2::string ret(*this);
	char* p = ret.m_buffer;
	while (*p != '\0') {
		if (*p == find) {
			*p = replace;
		}
		p++;
	}
	return ret;
}

s2::string s2::string::replace(const char* find, const char* replace) const
{
	size_t findlen = strlen(find);
	size_t replacelen = strlen(replace);

	s2::string ret(*this);
	int index = 0;
	while (true) {
		char* p = strstr(ret.m_buffer + index, find);
		if (p == nullptr) {
			break;
		}
		index = (int)(p - ret.m_buffer);

		ret.remove(index, findlen);
		ret.insert(replace, index, replacelen);

		index += (int)replacelen;
	}
	return ret;
}

void s2::string::setf(const char* format, ...)
{
	ensure_memory(min_buffer_size);

	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(m_buffer, m_allocSize, format, vl);
	va_end(vl);

	if (len >= min_buffer_size) {
		ensure_memory(len + 1);

		va_list vl;
		va_start(vl, format);
		vsnprintf(m_buffer, m_allocSize, format, vl);
		va_end(vl);
	}

	m_length = len;
}

void s2::string::appendf(const char* format, ...)
{
	char* buffer = (char*)malloc(min_buffer_size);

	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(buffer, min_buffer_size, format, vl);
	va_end(vl);

	if (len >= min_buffer_size) {
		buffer = (char*)realloc(buffer, len + 1);

		va_list vl;
		va_start(vl, format);
		len = vsnprintf(buffer, len + 1, format, vl);
		va_end(vl);
	}

	append(buffer, 0, len);

	free(buffer);
}

s2::string &s2::string::operator =(const char* sz)
{
	m_length = strlen(sz);
	ensure_memory(m_length + 1);
	memcpy(m_buffer, sz, m_length);
	m_buffer[m_length] = '\0';
	return *this;
}

s2::string &s2::string::operator =(const s2::string &str)
{
	return operator =(str.m_buffer);
}

s2::string &s2::string::operator +=(const char* sz)
{
	append(sz);
	return *this;
}

s2::string &s2::string::operator +=(const s2::string &str)
{
	return operator +=(str.m_buffer);
}

s2::string s2::string::operator +(const char* sz)
{
	return string(*this) += sz;
}

s2::string s2::string::operator +(const string &str)
{
	return string(*this) += str;
}

s2::string s2::string::trim() const
{
	return trim("\n\r\t ");
}

s2::string s2::string::trim(const char* sz) const
{
	if (strlen(sz) == 0) {
		return *this;
	}

	int len = (int)strlen(m_buffer);
	char* p = m_buffer;
	while (*p != '\0') {
		if (strchr(sz, *p) == nullptr) {
			break;
		}
		p++;
	}

	char* pp = m_buffer + (len - 1);
	while (pp > p) {
		if (strchr(sz, *pp) == nullptr) {
			break;
		}
		pp--;
	}

	return string(p, (pp - p) + 1);
}

s2::string s2::string::tolower() const
{
	s2::string ret = *this;
	char* p = ret.m_buffer;
	while (*p != '\0') {
		*p = ::tolower(*p);
		p++;
	}
	return ret;
}

s2::string s2::string::toupper() const
{
	s2::string ret = *this;
	char* p = ret.m_buffer;
	while (*p != '\0') {
		*p = ::toupper(*p);
		p++;
	}
	return ret;
}

bool s2::string::operator ==(const char* sz) const
{
	return !strcmp(m_buffer, sz);
}

bool s2::string::operator ==(const s2::string &str) const
{
	return !strcmp(m_buffer, str.m_buffer);
}

bool s2::string::operator !=(const char* sz) const
{
	return !!strcmp(m_buffer, sz);
}

bool s2::string::operator !=(const string &str) const
{
	return !!strcmp(m_buffer, str.m_buffer);
}

s2::string::operator const char*() const
{
	return m_buffer;
}

char &s2::string::operator [](int index)
{
	return m_buffer[index];
}

const char &s2::string::operator [](int index) const
{
	return m_buffer[index];
}

void s2::string::ensure_memory(size_t size)
{
	if (m_allocSize >= size) {
		return;
	}
	resize_memory(size);
}

void s2::string::resize_memory(size_t size)
{
	m_allocSize = size;
	m_buffer = (char*)realloc(m_buffer, m_allocSize);
}

bool s2::operator ==(const char* sz, const string &str)
{
	return str == sz;
}

s2::string s2::operator +(const char* lhs, const string &rhs)
{
	return string(lhs) += rhs;
}

s2::string s2::strprintf(const char* format, ...)
{
	char* buffer = (char*)malloc(min_buffer_size);

	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(buffer, min_buffer_size, format, vl);
	va_end(vl);

	if (len >= min_buffer_size) {
		buffer = (char*)realloc(buffer, len + 1);

		va_list vl;
		va_start(vl, format);
		len = vsnprintf(buffer, len + 1, format, vl);
		va_end(vl);
	}

	s2::string ret = buffer;
	free(buffer);
	return ret;
}

s2::stringsplit::stringsplit(const char* sz, const char* delim, int limit)
{
	const char* p = sz;
	size_t len = strlen(sz);
	size_t lenDelim = strlen(delim);

	while (*p != '\0') {
		const char* pos = strstr(p, delim);

		if (pos == nullptr || (limit > 0 && m_length + 1 == limit)) {
			add(p, len - (p - sz));
			return;
		}

		add(p, pos - p);
		p = pos + lenDelim;
	}
}

s2::stringsplit::stringsplit(const char* sz, bool commandLine)
{
	const char* p = sz;
	s2::string buffer;
	buffer.ensure_memory(128);
	bool inString = false;

	char c;
	do {
		c = *p;
		char cn = *(p + 1);

		if (c == '\\') {
			buffer.append(cn);
			p++;
			continue;
		}

		if (inString) {
			if (c == '"' && (cn == ' ' || cn == '\0')) {
				inString = false;
				continue;
			}

		} else {
			if (c == '"' && buffer.len() == 0) {
				inString = true;
				continue;
			}

			if (c == ' ' && buffer.len() != 0) {
				add(buffer, buffer.len());
				buffer = "";
				continue;
			}
		}

		buffer.append(c);
	} while (*(++p) != '\0');

	if (buffer.len() > 0) {
		add(buffer, buffer.len());
	}
}

s2::stringsplit::stringsplit(const stringsplit &copy)
{
	for (size_t i = 0; i < copy.m_length; i++) {
		const char* s = copy.m_buffer[i];
		add(s, strlen(s));
	}
}

s2::stringsplit::~stringsplit()
{
	for (size_t i = 0; i < m_length; i++) {
		free(m_buffer[i]);
	}
	free(m_buffer);
}

size_t s2::stringsplit::len() const
{
	return m_length;
}

s2::string s2::stringsplit::operator[](size_t index) const
{
	return m_buffer[index];
}

void s2::stringsplit::add(const char* sz, size_t len)
{
	m_length++;
	m_buffer = (char**)realloc(m_buffer, m_length * sizeof(char*));
	char* p = (char*)malloc(len + 1);
	memcpy(p, sz, len);
	p[len] = '\0';
	m_buffer[m_length - 1] = p;
}
#endif
