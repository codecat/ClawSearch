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

SCRATCH_NAMESPACE_BEGIN;

namespace Internal
{
	template<typename Result, typename ...Args>
	struct abstract_function
	{
		virtual Result operator()(Args... args) = 0;
		virtual abstract_function *clone() const = 0;
		virtual ~abstract_function() {}
	};

	template<typename Func, typename Result, typename ...Args>
	class concrete_function : public abstract_function<Result, Args...>
	{
	private:
		Func m_func;

	public:
		concrete_function(const Func &x)
			: m_func(x)
		{
		}

		virtual Result operator()(Args... args)
		{
			return m_func(args...);
		}

		virtual concrete_function* clone() const
		{
			return new concrete_function(m_func);
		}
	};

	template<typename Func>
	struct func_filter
	{
		typedef Func type;
	};

	template<typename Result, typename ...Args>
	struct func_filter<Result(Args...)>
	{
		typedef Result(*type)(Args...);
	};
}

template<typename signature>
class Function;

template<typename Result, typename ...Args>
class Function<Result(Args...)>
{
private:
	Internal::abstract_function<Result, Args...>* m_func;

public:
	Function()
	{
		m_func = nullptr;
	}

	template<typename Func>
	Function(const Func &x)
	{
		m_func = new Internal::concrete_function<typename Internal::func_filter<Func>::type, Result, Args...>(x);
	}

	Function(const Function &rhs)
	{
		m_func = nullptr;
		if (rhs.m_func != nullptr) {
			m_func = rhs.m_func->clone();
		}
	}

	Function(decltype(nullptr))
	{
		m_func = nullptr;
	}

	~Function()
	{
		if (m_func != nullptr) {
			delete m_func;
		}
	}

	Function &operator=(const Function &rhs)
	{
		if (&rhs != this && rhs.m_func != nullptr)
		{
			auto temp = rhs.m_func->clone();
			if (m_func != nullptr) {
				delete m_func;
			}
			m_func = temp;
		}
		return *this;
	}

	Function &operator=(decltype(nullptr))
	{
		m_func = nullptr;
		return *this;
	}

	template<typename Func>
	Function &operator=(const Func &x)
	{
		auto temp = new Internal::concrete_function<typename Internal::func_filter<Func>::type, Result, Args...>(x);
		if (m_func != nullptr) {
			delete m_func;
		}
		m_func = temp;
		return *this;
	}

	bool operator==(decltype(nullptr)) const
	{
		return m_func == nullptr;
	}

	bool operator!=(decltype(nullptr)) const
	{
		return m_func != nullptr;
	}

	Result operator()(Args... args) const
	{
		if (m_func == nullptr) {
			return Result();
		}
		return (*m_func)(args...);
	}
};

SCRATCH_NAMESPACE_END;
