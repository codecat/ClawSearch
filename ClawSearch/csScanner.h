#pragma once

#include "pluginsdk/bridgemain.h"

#include "Common.h"
#include "SearchResult.h"

class csMain;

enum SearchValueType
{
	SVT_Unknown,

	SVT_Char,
	SVT_Int16,
	SVT_Int32,
	SVT_Int64,

	SVT_Float,
	SVT_Double,
};

enum SearchValueMethod
{
	SVM_Unknown,
	SVM_Integer, // char, int16, int32, int64
	SVM_Float, // float, double
};

class csScanner
{
public:
	csMain* m_main;

	MEMMAP m_currentScanMap;

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

	s::Array<SearchResult, 100> m_results;

	size_t m_scanSize;
	unsigned char* m_currentBuffer;
	unsigned char* m_currentCompare;

public:
	csScanner(csMain* main);
	~csScanner();

	void PerformScan(bool firstScan);

	bool CompareData(void* p, void* src, int sz);
};
