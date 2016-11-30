#pragma once

#include <iup.h>
#include <iupcontrols.h>

#include "Common.h"
#include "SearchResult.h"

#include "pluginsdk/bridgemain.h"

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

class csMain
{
public:
	// The main dialog
	Ihandle* m_hDialog;

	// Scan buttons
	Ihandle* m_hButtonFirstScan;
	Ihandle* m_hButtonNextScan;

	// Value input
	Ihandle* m_hCheckHex;
	Ihandle* m_hTextInput;

	// Value type input
	Ihandle* m_hComboValueType;

	// Scan options
	Ihandle* m_hFrameScanOptions;

	// Scan options -> Float method
	Ihandle* m_hFloatMethod;

	// Scan options -> Fast scan
	Ihandle* m_hCheckFastScan;
	Ihandle* m_hTextFastScanAlign;

	// Scan options -> Pause while scanning
	Ihandle* m_hCheckPauseWhileScanning;

	// List results
	Ihandle* m_hListResults;

	//
	MEMMAP m_currentScanMap;
	int m_currentScan;
	SearchValueType m_currentScanValueType;
	SearchValueMethod m_currentScanValueMethod;
	bool m_currentScanFloatTruncate;

	s::Array<SearchResult, 100> m_results;

	size_t m_scanSize;
	unsigned char* m_currentBuffer;
	unsigned char* m_currentCompare;

public:
	csMain();
	~csMain();

	SearchValueMethod MethodForType(SearchValueType type);

	int SearchWindowClosing();

	void PerformScan();
	bool CompareData(void* p, void* src, int sz);

	int FirstScan();
	int NextScan();
	void ResultClicked(char* text, int item, int state);
	int ScanValueTypeChanged();

	void Open();
	void Close();
};

extern csMain* _csMain;

void OpenSearch();
void CloseSearch();

inline bool cmpfloat(const float &a, const float &b) { return fabsf(a - b) < FLT_EPSILON; }
inline bool cmpdouble(const double &a, const double &b) { return fabsl(a - b) < DBL_EPSILON; }

#define CLAW_SETCALLBACK(handle, cb, name) IupSetCallback(handle, cb, _claw_##name);

#define CLAW_CALLBACK(name) static int _claw_##name(Ihandle* handle) { return _csMain->name(); }
#define CLAW_CALLBACK_HANDLE(name) static int _claw_##name(Ihandle* handle) { return _csMain->name(handle); }
