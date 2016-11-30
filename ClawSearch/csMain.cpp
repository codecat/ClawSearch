#include "csMain.h"

#include "plugin.h"

CLAW_CALLBACK(SearchWindowClosing);
CLAW_CALLBACK(FirstScan);
CLAW_CALLBACK(NextScan);
CLAW_CALLBACK(ScanValueTypeChanged);

int _claw_ResultClicked(Ihandle* handle, char* text, int item, int state) { _csMain->ResultClicked(text, item, state); return 0; }

csMain::csMain()
{
	m_hDialog = nullptr;

	m_hButtonFirstScan = nullptr;
	m_hButtonNextScan = nullptr;

	m_hCheckHex = nullptr;
	m_hTextInput = nullptr;

	m_hFrameScanOptions = nullptr;
	m_hFloatMethod = nullptr;
	m_hCheckFastScan = nullptr;
	m_hTextFastScanAlign = nullptr;
	m_hCheckPauseWhileScanning = nullptr;

	m_hListResults = nullptr;

	m_currentScanMap.count = 0;
	m_currentScanMap.page = nullptr;
	m_currentScan = 0;
	m_currentScanValueType = SVT_Unknown;
	m_currentScanValueMethod = SVM_Unknown;
	m_currentScanFloatTruncate = false;

	m_scanSize = 0x1000;
	m_currentBuffer = nullptr;
	m_currentCompare = nullptr;
}

csMain::~csMain()
{
	if (m_hDialog != nullptr) {
		Close();
	}

	if (m_currentBuffer != nullptr) {
		free(m_currentBuffer);
	}

	if (m_currentScanMap.page != nullptr) {
		BridgeFree(m_currentScanMap.page);
	}
}

SearchValueMethod csMain::MethodForType(SearchValueType type)
{
	switch (type) {
	case SVT_Char:
	case SVT_Int16:
	case SVT_Int32:
	case SVT_Int64:
		return SVM_Integer;

	case SVT_Float:
	case SVT_Double:
		return SVM_Float;
	}

	return SVM_Unknown;
}

int csMain::SearchWindowClosing()
{
	m_hDialog = nullptr;
	return IUP_CLOSE;
}

void csMain::PerformScan()
{
	IupSetAttribute(m_hButtonFirstScan, "ACTIVE", "NO");
	IupSetAttribute(m_hButtonNextScan, "ACTIVE", "NO");

	size_t findSize = 0;
	unsigned char* find = nullptr;

	char* inputText = IupGetAttribute(m_hTextInput, "VALUE");
	bool inputIsHex = !strcmp(IupGetAttribute(m_hCheckHex, "VALUE"), "ON");
	bool pauseWhileScanning = DbgIsRunning() && !strcmp(IupGetAttribute(m_hCheckPauseWhileScanning, "VALUE"), "ON");
	bool fastScan = !strcmp(IupGetAttribute(m_hCheckFastScan, "VALUE"), "ON");
	m_currentScanFloatTruncate = !strcmp(IupGetAttribute(m_hFloatMethod, "VALUE"), "trunc");

	int scanStep = 1;
	if (fastScan) {
		sscanf(IupGetAttribute(m_hTextFastScanAlign, "VALUE"), "%d", &scanStep);
	}

	if (pauseWhileScanning) {
		DbgCmdExecDirect("pause");
		_plugin_waituntilpaused();
	}

	//TODO: Clean this up
#define HANDLE_SEARCHFOR_SCANF(format, type) type searchFor; \
	if (sscanf(inputText, format, &searchFor) > 0) { \
		findSize = sizeof(searchFor); \
		find = (unsigned char*)malloc(findSize); \
		memcpy(find, &searchFor, findSize); \
	}

	if (m_currentScanValueType == SVT_Char) {
		if (inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%hhx", uint8_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%hhd", int8_t);
		}
	} else if (m_currentScanValueType == SVT_Int16) {
		if (inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%hx", uint16_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%hd", int16_t);
		}
	} else if (m_currentScanValueType == SVT_Int32) {
		if (inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%x", uint32_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%d", int32_t);
		}
	} else if (m_currentScanValueType == SVT_Int64) {
		if (inputIsHex) {
			HANDLE_SEARCHFOR_SCANF("%llx", uint64_t);
		} else {
			HANDLE_SEARCHFOR_SCANF("%llx", int64_t);
		}
	} else if (m_currentScanValueType == SVT_Float) {
		HANDLE_SEARCHFOR_SCANF("%f", float);
		if (m_currentScanFloatTruncate) {
			*(float*)find = trunc(*(float*)find);
		}
	} else if (m_currentScanValueType == SVT_Double) {
		HANDLE_SEARCHFOR_SCANF("%lf", double);
		if (m_currentScanFloatTruncate) {
			*(double*)find = trunc(*(double*)find);
		}
	}

#undef HANDLE_SEARCHFOR

	if (find == nullptr) {
		IupMessage("Error", "Unhandled value type!");
		return;
	}

	if (m_currentBuffer == nullptr) {
		m_currentBuffer = (unsigned char*)malloc(m_scanSize);
		memset(m_currentBuffer, 0, m_scanSize);
	}

	if (m_currentScanMap.page != nullptr) {
		BridgeFree(m_currentScanMap.page);
	}

	// If this is the very first scan
	if (m_currentScan == 1) {
		DbgMemMap(&m_currentScanMap);
		// For each memory region
		for (int iMap = 0; iMap < m_currentScanMap.count; iMap++) {
			MEMPAGE &memPage = m_currentScanMap.page[iMap];
			ptr_t base = (ptr_t)memPage.mbi.BaseAddress;
			size_t size = memPage.mbi.RegionSize;
			ptr_t end = base + size;

			// For each page in the memory region
			for (ptr_t p = base; p < end; p += m_scanSize) {
				size_t sz = m_scanSize;
				if (p + sz >= end) {
					sz = end - p;
				}

				//TODO: Try ReadProcessMemory instead
				DbgMemRead(p, m_currentBuffer, sz);

				// Perform search on buffer
				for (ptr_t s = 0; s < sz; s += scanStep) {
					// Stop if find size is beyond scan size
					if (s + findSize > sz) {
						break;
					}

					// Compare at this position
					if (!CompareData(m_currentBuffer + s, find, findSize)) {
						continue;
					}

					// Found it!
					SearchResult &result = m_results.Add();
					result.m_base = p;
					result.m_offset = s;
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

	// If this is our not our first scan
	if (m_currentScan > 1) {
		for (int i = 0; i < m_results.Count(); i++) {
			SearchResult &result = m_results[i];

			//TODO: This is really slow!
			DbgMemRead(result.m_base + result.m_offset, m_currentCompare, findSize);

			if (!CompareData(m_currentCompare, find, findSize) != 0) {
				//TODO: A linked list might be faster here!
				m_results.RemoveAt(i);
				i--;
			}
		}
	}

	if (pauseWhileScanning) {
		DbgCmdExec("run");
	}

	free(find);
	free(m_currentCompare);
	m_currentCompare = nullptr;

	IupSetAttribute(m_hListResults, "REMOVEITEM", "ALL");
	IupSetAttribute(m_hListResults, "AUTOREDRAW", "NO");
	int numResults = m_results.Count();
	for (int i = 0; i < numResults; i++) {
		SearchResult &result = m_results[i];

		ptr_t pointer = result.m_base + result.m_offset;

		s::String strLine = s::strPrintF("%p", pointer);

		//TODO: Can we up the performance on this?
		if (numResults < 20) {
			char moduleName[MAX_MODULE_SIZE];
			if (DbgGetModuleAt(pointer, moduleName)) {
				strLine += s::strPrintF(" (%s)", moduleName);
			}

			char label[MAX_LABEL_SIZE];
			if (DbgGetLabelAt(pointer, SEG_DEFAULT, label)) {
				strLine += s::strPrintF(" %s", label);
			}
		}

		IupSetAttribute(m_hListResults, "APPENDITEM", strLine);
	}

	IupSetAttribute(m_hListResults, "AUTOREDRAW", "YES");
	IupRedraw(m_hListResults, 1);

	IupSetAttribute(m_hButtonFirstScan, "ACTIVE", "YES");
	IupSetAttribute(m_hButtonNextScan, "ACTIVE", "YES");
}

bool csMain::CompareData(void* p, void* src, int sz)
{
	if (m_currentScanValueMethod == SVM_Integer) {
		// For basic integer types we can simply compare the bytes
		if (memcmp(p, src, sz) != 0) {
			return false;
		}

	} else if (m_currentScanValueMethod == SVM_Float) {
		// For floating point types it depends on the size
		if (m_currentScanValueType == SVT_Float) {
			float &f = *(float*)p;

			// If our source float is truncated
			if (m_currentScanFloatTruncate) {
				f = trunc(f);
			}

			// Go to next if the float does not compare
			if (!cmpfloat(f, *(float*)src)) {
				return false;
			}

		} else if (m_currentScanValueType == SVT_Double) {
			double &d = *(double*)p;

			// If our source double is truncated
			if (m_currentScanFloatTruncate) {
				d = truncl(d);
			}

			// Go to next if the double does not compare
			if (!cmpdouble(d, *(double*)src)) {
				return false;
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

int csMain::FirstScan()
{
	if (m_currentScan > 0) {
		m_currentScan = 0;
		m_results.Clear();

		IupSetAttribute(m_hListResults, "REMOVEITEM", "ALL");

		IupSetAttribute(m_hButtonFirstScan, "TITLE", "First Scan");

		IupSetAttribute(m_hComboValueType, "ACTIVE", "YES");
		IupSetAttribute(m_hButtonNextScan, "ACTIVE", "NO");
		return 0;
	}

	m_currentScan = 1;

	IupSetAttribute(m_hButtonFirstScan, "TITLE", "New Scan");

	PerformScan();

	IupSetAttribute(m_hComboValueType, "ACTIVE", "NO");
	IupSetAttribute(m_hButtonNextScan, "ACTIVE", "YES");

	return 0;
}

int csMain::NextScan()
{
	m_currentScan++;

	PerformScan();

	return 0;
}

void csMain::ResultClicked(char* text, int item, int state)
{
	int index = item - 1;

	if (index == -1 || state == 0) {
		return;
	}

	SearchResult &result = m_results[index];

	GuiDumpAt(result.m_base + result.m_offset);
}

int csMain::ScanValueTypeChanged()
{
	m_currentScanValueType = (SearchValueType)IupGetInt(m_hComboValueType, "VALUE");
	m_currentScanValueMethod = MethodForType(m_currentScanValueType);

	IupSetAttribute(m_hCheckHex, "ACTIVE", m_currentScanValueMethod == SVM_Integer ? "YES" : "NO");
	IupSetAttribute(m_hFloatMethod, "ACTIVE", m_currentScanValueMethod == SVM_Float ? "YES" : "NO");

	return 0;
}

void csMain::Open()
{
	if (m_hDialog != nullptr) {
		return;
	}

	m_hButtonFirstScan = IupButton("First Scan", "ScanFirst");
	m_hButtonNextScan = IupButton("Next Scan", "ScanNext");
	Ihandle* hButtons = IupSetAttributes(IupHbox(m_hButtonFirstScan, m_hButtonNextScan, nullptr), "MARGIN=0x0, GAP=5");

	IupSetAttribute(m_hButtonNextScan, "ACTIVE", "NO");

	CLAW_SETCALLBACK(m_hButtonFirstScan, "ACTION", FirstScan);
	CLAW_SETCALLBACK(m_hButtonNextScan, "ACTION", NextScan);

	m_hCheckHex = IupToggle("Hex", nullptr);
	m_hTextInput = IupSetAttributes(IupText(nullptr), "EXPAND=HORIZONTAL");
	Ihandle* hInput = IupSetAttributes(IupHbox(m_hCheckHex, m_hTextInput, nullptr), "MARGIN=0x0, GAP=5");

	m_hComboValueType = IupList(nullptr);
	IupSetAttribute(m_hComboValueType, "DROPDOWN", "YES");
	IupSetAttribute(m_hComboValueType, "1", "Byte");
	IupSetAttribute(m_hComboValueType, "2", "2 Bytes");
	IupSetAttribute(m_hComboValueType, "3", "4 Bytes");
	IupSetAttribute(m_hComboValueType, "4", "8 Bytes");
	IupSetAttribute(m_hComboValueType, "5", "Float");
	IupSetAttribute(m_hComboValueType, "6", "Double");
	IupSetAttribute(m_hComboValueType, "VALUE", "3");
	Ihandle* hValueType = IupSetAttributes(IupHbox(m_hComboValueType, nullptr), "MARGIN=0x0, GAP=5");
	CLAW_SETCALLBACK(m_hComboValueType, "ACTION", ScanValueTypeChanged);

	m_hCheckFastScan = IupToggle("Fast Scan", nullptr);
	IupSetAttribute(m_hCheckFastScan, "VALUE", "ON");

	m_hTextFastScanAlign = IupText(nullptr);
	IupSetAttribute(m_hTextFastScanAlign, "VALUE", "4");

	Ihandle* radioFloatTruncated = IupToggle("Truncated", nullptr);
	Ihandle* radioFloatRounded = IupToggle("Rounded", nullptr);
	Ihandle* radioFloatRoundedExtreme = IupToggle("Rounded (Extreme)", nullptr);

	IupSetHandle("trunc", radioFloatTruncated);
	IupSetHandle("round", radioFloatRounded);
	IupSetHandle("round2", radioFloatRoundedExtreme);

	m_hFloatMethod = IupRadio(IupSetAttributes(IupVbox(radioFloatTruncated, radioFloatRounded, radioFloatRoundedExtreme, nullptr), "MARGIN=0x0, GAP=5"));
	IupSetAttribute(m_hFloatMethod, "ACTIVE", "NO");

	Ihandle* hFastScan = IupSetAttributes(IupHbox(m_hCheckFastScan, m_hTextFastScanAlign, nullptr), "MARGIN=0x0, GAP=5");

	m_hCheckPauseWhileScanning = IupToggle("Pause while scanning", nullptr);

	m_hFrameScanOptions = IupFrame(IupVbox(
		m_hFloatMethod,
		hFastScan,
		m_hCheckPauseWhileScanning,
		nullptr)
		);
	IupSetAttribute(m_hFrameScanOptions, "TITLE", "Scan Options");
	IupSetAttribute(m_hFrameScanOptions, "EXPAND", "HORIZONTAL");

	Ihandle* vControls = IupSetAttributes(IupVbox(
		hButtons,
		hInput,
		hValueType,
		m_hFrameScanOptions,
		nullptr), "MARGIN=10x0, GAP=5, EXPAND=HORIZONTAL");

	m_hListResults = IupList(nullptr);
	IupSetAttribute(m_hListResults, "FONT", "Consolas, 9");
	IupSetAttribute(m_hListResults, "EXPAND", "YES");
	IupSetAttribute(m_hListResults, "1", nullptr);
	IupSetCallback(m_hListResults, "ACTION", (Icallback)_claw_ResultClicked);

	m_hDialog = IupDialog(IupSetAttributes(IupHbox(m_hListResults, vControls, nullptr), "PADDING=4x4, MARGIN=10x10"));

	IupSetAttribute(m_hDialog, "TITLE", "ClawSearch");
	IupSetAttribute(m_hDialog, "SIZE", "500x200");

	IupSetAttribute(m_hDialog, "NATIVEPARENT", (char*)GuiGetWindowHandle());
	CLAW_SETCALLBACK(m_hDialog, "CLOSE_CB", SearchWindowClosing);
	IupShowXY(m_hDialog, IUP_CENTERPARENT, IUP_CENTERPARENT);
}

void csMain::Close()
{
	if (m_hDialog == nullptr) {
		return;
	}
	IupDestroy(m_hDialog);
	m_hDialog = nullptr;
}

csMain* _csMain = nullptr;

void OpenSearch()
{
	if (_csMain == nullptr) {
		_csMain = new csMain;
	}

	_csMain->Open();
}

void CloseSearch()
{
	if (_csMain == nullptr) {
		return;
	}

	_csMain->Close();
	delete _csMain;
}
