#pragma once

#include <iup.h>
#include <iupcontrols.h>

#include "pluginsdk/bridgemain.h"

#include "csScanner.h"

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
	Ihandle* m_hComboScanType;
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

	// Scanner
	csScanner m_scanner;
	int m_currentScan;

public:
	csMain();
	~csMain();

	SearchValueMethod MethodForType(SearchValueType type);

	int SearchWindowClosing();

	void PerformScan(bool firstScan);

	int FirstScan();
	int NextScan();
	void ResultClicked(char* text, int item, int state);
	int ScanValueTypeChanged();

	void SetScanTypeCombo(bool firstScan);

	void Open();
	void Close();
};

extern csMain* _csMain;

void OpenSearch();
void CloseSearch();

inline bool cmpfloat(const float &a, const float &b, float e = FLT_EPSILON) { return fabsf(a - b) < e; }
inline bool cmpdouble(const double &a, const double &b, double e = DBL_EPSILON) { return fabsl(a - b) < e; }

#define CLAW_SETCALLBACK(handle, cb, name) IupSetCallback(handle, cb, _claw_##name);

#define CLAW_CALLBACK(name) static int _claw_##name(Ihandle* handle) { return _csMain->name(); }
#define CLAW_CALLBACK_HANDLE(name) static int _claw_##name(Ihandle* handle) { return _csMain->name(handle); }
