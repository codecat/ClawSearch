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
	SVT_Int64
};

class csMain
{
public:
	Ihandle* m_hDialog;

	Ihandle* m_hButtonFirstScan;
	Ihandle* m_hButtonNextScan;

	Ihandle* m_hCheckHex;
	Ihandle* m_hTextInput;

	Ihandle* m_hComboValueType;

	Ihandle* m_hListResults;

	MEMMAP m_currentScanMap;
	int m_currentScan;
	SearchValueType m_currentScanValueType;

	s::Array<SearchResult, 100> m_results;

	size_t m_pageSize;
	unsigned char* m_currentPage;
	unsigned char* m_currentCompare;

public:
	csMain();
	~csMain();

	int SearchWindowClosing();

	void PerformScan();

	int FirstScan();
	int NextScan();
	void ResultClicked(char* text, int item, int state);

	void Open();
	void Close();
};

extern csMain* _csMain;

void OpenSearch();
void CloseSearch();

#define CLAW_SETCALLBACK(handle, cb, name) IupSetCallback(handle, cb, _claw_##name);

#define CLAW_CALLBACK(name) static int _claw_##name(Ihandle* handle) { return _csMain->name(); }
#define CLAW_CALLBACK_HANDLE(name) static int _claw_##name(Ihandle* handle) { return _csMain->name(handle); }
