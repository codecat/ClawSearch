#pragma once

#include "Common.h"

class SearchResult
{
public:
	uintptr_t m_base;
	uintptr_t m_offset;
	uint64_t m_valueFound;
};
