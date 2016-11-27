#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define SCRATCH_NAMESPACE s
#include "Scratch/Scratch.hpp"

#ifdef _X86_
typedef uint32_t ptr_t;
#else
typedef uint64_t ptr_t;
#endif
