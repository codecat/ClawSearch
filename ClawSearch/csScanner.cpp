#include "csScanner.h"

#include "plugin.h"

#include "csMain.h"
#include "csMath.h"

csScanner::csScanner(csMain* main)
{
	m_main = main;

	m_currentScanMap.count = 0;
	m_currentScanMap.page = nullptr;

	m_initialScanType = IST_Equal;
	m_currentScanType = ST_Equal;
	m_currentScanValueType = SVT_Int32;
	m_currentScanValueMethod = SVM_Integer;

	m_inputText = nullptr;
	m_inputIsHex = false;

	m_pauseWhileScanning = false;

	m_scanStep = 1;

	m_currentScanFloatTruncate = false;
	m_currentScanFloatRound = false;
	m_currentScanFloatRound2 = false;
	m_currentScanFloatRoundNum = 0;

	m_scanSize = 0x1000;
	m_currentBuffer = nullptr;
	m_currentCompare = nullptr;
}

csScanner::~csScanner()
{
	if (m_currentBuffer != nullptr) {
		free(m_currentBuffer);
	}

	if (m_currentScanMap.page != nullptr) {
		BridgeFree(m_currentScanMap.page);
	}
}

void csScanner::PerformScan(bool firstScan)
{
	size_t findSize = 0;
	unsigned char* find = nullptr;

	if (firstScan) {
		m_results.clear();
	}

	if (m_pauseWhileScanning) {
		DbgCmdExecDirect("pause");
		_plugin_waituntilpaused();
	}

	//TODO: Clean this up
#define HANDLE_SEARCHFOR_SCANF(format, type) type searchFor; \
	if (sscanf(m_inputText, format, &searchFor) > 0) { \
		findSize = sizeof(searchFor); \
		find = (unsigned char*)malloc(findSize); \
		memcpy(find, &searchFor, findSize); \
	}

	if (m_currentScanValueType == SVT_Char) {
		if (m_inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%hhx", uint8_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%hhd", int8_t);
		}
	} else if (m_currentScanValueType == SVT_Int16) {
		if (m_inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%hx", uint16_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%hd", int16_t);
		}
	} else if (m_currentScanValueType == SVT_Int32) {
		if (m_inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%x", uint32_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%d", int32_t);
		}
	} else if (m_currentScanValueType == SVT_Int64) {
		if (m_inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%llx", uint64_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%llx", int64_t);
		}
	} else if (m_currentScanValueType == SVT_Float) {
		HANDLE_SEARCHFOR_SCANF("%f", float);
	} else if (m_currentScanValueType == SVT_Double) {
		HANDLE_SEARCHFOR_SCANF("%lf", double);
	}

#undef HANDLE_SEARCHFOR

	if (find == nullptr) {
		IupMessage("Error", "Unhandled value type!");
		return;
	}

	if (m_currentScanValueMethod == SVM_Float) {
		const char* afterPeriod = strchr(m_inputText, '.');
		if (afterPeriod == nullptr) {
			m_currentScanFloatRoundNum = 0;
		} else {
			m_currentScanFloatRoundNum = (int)strlen(afterPeriod + 1);
		}
	}

	if (m_currentBuffer == nullptr) {
		m_currentBuffer = (unsigned char*)malloc(m_scanSize);
		memset(m_currentBuffer, 0, m_scanSize);
	}

	if (m_currentScanMap.page != nullptr) {
		BridgeFree(m_currentScanMap.page);
		m_currentScanMap.page = nullptr;
	}

	// If this is the very first scan
	if (firstScan) {
		DbgMemMap(&m_currentScanMap);

		// For each memory region
		for (int iMap = 0; iMap < m_currentScanMap.count; iMap++) {
			MEMPAGE &memPage = m_currentScanMap.page[iMap];
			uintptr_t base = (uintptr_t)memPage.mbi.BaseAddress;
			size_t size = memPage.mbi.RegionSize;
			uintptr_t end = base + size;

			// For each page in the memory region
			for (uintptr_t p = base; p < end; p += m_scanSize) {
				size_t sz = m_scanSize;
				if (p + sz >= end) {
					sz = end - p;
				}

				//TODO: Try ReadProcessMemory instead
				DbgMemRead(p, m_currentBuffer, sz);

				// Perform search on buffer
				for (uintptr_t s = 0; s < sz; s += m_scanStep) {
					// Stop if find size is beyond scan size
					if (s + findSize > sz) {
						break;
					}

					// Compare at this position
					if (!MatchDataInitial(m_currentBuffer + s, find, (int)findSize)) {
						continue;
					}

					// Found it!
					SearchResult &result = m_results.add();
					result.m_base = p;
					result.m_offset = s;
					result.m_valueFound = 0;
					if (findSize <= sizeof(uint64_t)) {
						memcpy(&result.m_valueFound, m_currentBuffer + s, findSize);
					}

					// We can step forward now
					s += findSize;
				}
			}
		}
	}

	m_currentCompare = (unsigned char*)malloc(findSize);

	// If this is not our first scan
	if (!firstScan) {
		s2::list<SearchResult> newResults;
		newResults.ensure_memory(m_results.len());

		for (size_t i = 0; i < m_results.len(); i++) {
			SearchResult &result = m_results[i];

			//TODO: This is really slow!

			uintptr_t ptr = result.m_base + result.m_offset;

			// The memory must still be readable
			if (DbgMemIsValidReadPtr(ptr)) {
				DbgMemRead(result.m_base + result.m_offset, m_currentCompare, findSize);

				// Match data
				if (MatchDataNext(m_currentCompare, find, result, (int)findSize)) {
					newResults.add(result);
				}
			}
		}

		m_results = newResults;
	}

	if (m_pauseWhileScanning) {
		DbgCmdExec("run");
	}

	free(find);
	free(m_currentCompare);
	m_currentCompare = nullptr;
}

bool csScanner::MatchDataInitial(void* p, void* src, int sz)
{
	switch (m_initialScanType) {
	case IST_Equal: return MatchDataEqual(p, src, sz);
	case IST_MoreThan: return MatchDataDifference(p, src, sz, 1);
	case IST_LessThan: return MatchDataDifference(p, src, sz, -1);
	default: assert(false); return false;
	}
}

bool csScanner::MatchDataNext(void* p, void* src, SearchResult &result, int sz)
{
	switch (m_currentScanType) {
	case ST_Equal: return MatchDataEqual(p, src, sz);
	case ST_Changed: assert(sz <= 8); return !MatchDataEqual(p, &result.m_valueFound, sz);
	case ST_MoreThan: return MatchDataDifference(p, src, sz, 1);
	case ST_LessThan: return MatchDataDifference(p, src, sz, -1);
	case ST_Increased: assert(sz <= 8); return MatchDataDifference(p, &result.m_valueFound, sz, 1);
	case ST_Decreased: assert(sz <= 8); return MatchDataDifference(p, &result.m_valueFound, sz, -1);
	}
	return false;
}

bool csScanner::MatchDataEqual(void* p, void* src, int sz)
{
	if (m_currentScanValueMethod == SVM_Integer) {
		// For basic integer types we can simply compare the bytes
		if (memcmp(p, src, sz) != 0) {
			return false;
		}

	} else if (m_currentScanValueMethod == SVM_Float) {
		// For floating point types it depends on the size
		if (m_currentScanValueType == SVT_Float) {
			const float &orig = *(float*)src;
			const float &f = *(float*)p;

			if (m_currentScanFloatRound) {
				// If our source float is rounded
				float rounded = roundf_to(f, m_currentScanFloatRoundNum);

				// Go to next if the float does not compare
				if (!cmpfloat(rounded, orig)) {
					return false;
				}
			} else if (m_currentScanFloatRound2) {
				// If our source float is extreme rounded
				float rounded1 = ceilf_to(f, m_currentScanFloatRoundNum);
				float rounded2 = floorf_to(f, m_currentScanFloatRoundNum);

				// Go to next if the float does not compare
				if (!cmpfloat(rounded1, orig) && !cmpfloat(rounded2, orig)) {
					return false;
				}
			} else if (m_currentScanFloatTruncate) {
				// If our source float is truncated
				float truncated = truncf_to(f, m_currentScanFloatRoundNum);

				// Go to next if the float does not compare
				if (!cmpfloat(truncated, orig)) {
					return false;
				}
			} else {
				// Regular float compare using epsilon
				if (!cmpfloat(f, orig)) {
					// Go to next if the float does not compare
					return false;
				}
			}

		} else if (m_currentScanValueType == SVT_Double) {
			const double &orig = *(double*)src;
			const double &d = *(double*)p;

			if (m_currentScanFloatRound) {
				// If our source double is rounded
				double rounded = roundl_to(d, m_currentScanFloatRoundNum);

				// Go to next if the double does not compare
				if (!cmpdouble(rounded, orig)) {
					return false;
				}
			} else if (m_currentScanFloatRound2) {
				// If our source double is extreme rounded
				double rounded1 = ceill_to(d, m_currentScanFloatRoundNum);
				double rounded2 = floorl_to(d, m_currentScanFloatRoundNum);

				// Go to next if the double does not compare
				if (!cmpdouble(rounded1, orig) && !cmpdouble(rounded2, orig)) {
					return false;
				}
			} else if (m_currentScanFloatTruncate) {
				// If our source double is truncated
				double truncated = truncl_to(d, m_currentScanFloatRoundNum);

				// Go to next if the double does not compare
				if (!cmpdouble(truncated, orig)) {
					return false;
				}
			} else {
				// Regular double compare using epsilon
				if (!cmpdouble(d, orig)) {
					// Go to next if the double does not compare
					return false;
				}
			}

		} else {
			// Unknown floating point type
			assert(false);
			return false;
		}

	} else {
		// Unknown scan value method
		assert(false);
		return false;
	}

	// It matches!
	return true;
}

bool csScanner::MatchDataDifference(void* p, void* src, int sz, int dir)
{
	//TODO: Clean this up
#define HANDLE_DIFFERENCE(type) const type &orig = *(type*)src; \
	const type &v = *(type*)p; \
	switch (dir) { \
	case -1: return v < orig; \
	case 1: return v > orig; \
	default: assert(false); return false; \
	}

	if (m_currentScanValueType == SVT_Char) {
		HANDLE_DIFFERENCE(char);
	} else if (m_currentScanValueType == SVT_Int16) {
		HANDLE_DIFFERENCE(int16_t);
	} else if (m_currentScanValueType == SVT_Int32) {
		HANDLE_DIFFERENCE(int32_t);
	} else if (m_currentScanValueType == SVT_Int64) {
		HANDLE_DIFFERENCE(int64_t);

	} else if (m_currentScanValueType == SVT_Float) {
		HANDLE_DIFFERENCE(float);
	} else if (m_currentScanValueType == SVT_Double) {
		HANDLE_DIFFERENCE(double);

	} else {
		// Unhandled type
		assert(false);
	}

	return false;

#undef HANDLE_DIFFERENCE
}
