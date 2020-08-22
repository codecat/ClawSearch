#pragma once

#include "pluginsdk/bridgemain.h"

#include "Common.h"
#include "SearchResult.h"

class csMain;

enum SearchValueType
{
	SVT_Unknown,

	SVT_Char, // 1 byte
	SVT_Int16, // 2 bytes
	SVT_Int32, // 4 bytes
	SVT_Int64, // 8 bytes

	SVT_Float, // 4 bytes
	SVT_Double, // 8 bytes
};

enum SearchValueMethod
{
	SVM_Unknown,

	SVM_Integer, // char, int16, int32, int64
	SVM_Float, // float, double
};

enum InitialScanType
{
	IST_Unknown,

	IST_Equal, // source == input
	IST_MoreThan, // source > input
	IST_LessThan, // source < input
};

enum ScanType
{
	ST_Unknown,

	ST_Equal, // source == input
	ST_Changed, // source != lastValue
	ST_Unchanged, // source == lastValue
	ST_MoreThan, // source > input
	ST_LessThan, // source < input
	ST_Increased, // source > lastValue
	ST_Decreased, // source < lastValue
};

class csScanner
{
public:
	csMain* m_main;

	MEMMAP m_currentScanMap;

	InitialScanType m_initialScanType;
	ScanType m_currentScanType;
	SearchValueType m_currentScanValueType;
	SearchValueMethod m_currentScanValueMethod;

	const char* m_inputText;
	bool m_inputIsHex;

	bool m_pauseWhileScanning;

	int m_scanStep;

	bool m_currentScanFloatTruncate;
	bool m_currentScanFloatRound;
	bool m_currentScanFloatRound2;
	int m_currentScanFloatRoundNum;

	s2::list<SearchResult> m_results;

	size_t m_scanSize;
	unsigned char* m_currentBuffer;
	unsigned char* m_currentCompare;

public:
	csScanner(csMain* main);
	~csScanner();

	void PerformScan(bool firstScan);

	bool MatchDataInitial(void* p, void* src, int sz);
	bool MatchDataNext(void* p, void* src, SearchResult &result, int sz);

	bool MatchDataEqual(void* p, void* src, int sz);
	bool MatchDataDifference(void* p, void* src, int sz, int dir);
};
