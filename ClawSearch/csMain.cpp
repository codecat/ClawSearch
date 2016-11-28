#include "csMain.h"

#include "plugin.h"

CLAW_CALLBACK(SearchWindowClosing);
CLAW_CALLBACK(FirstScan);
CLAW_CALLBACK(NextScan);

int _claw_ResultClicked(Ihandle* handle, char* text, int item, int state) { _csMain->ResultClicked(text, item, state); return 0; }

csMain::csMain()
{
	m_hDialog = nullptr;

	m_hButtonFirstScan = nullptr;
	m_hButtonNextScan = nullptr;

	m_hCheckHex = nullptr;
	m_hTextInput = nullptr;

	m_hListResults = nullptr;

	m_currentScanMap.count = 0;
	m_currentScanMap.page = nullptr;
	m_currentScan = 0;
	m_currentScanValueType = SVT_Unknown;

	m_pageSize = 0x800;
	m_currentPage = nullptr;
	m_currentCompare = nullptr;
}

csMain::~csMain()
{
	if (m_hDialog != nullptr) {
		Close();
	}

	if (m_currentPage != nullptr) {
		free(m_currentPage);
	}

	if (m_currentScanMap.page != nullptr) {
		BridgeFree(m_currentScanMap.page);
	}
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
	}

#undef HANDLE_SEARCHFOR

	if (find == nullptr) {
		IupMessage("Error", "Unhandled value type!");
		return;
	}

	if (m_currentPage == nullptr) {
		m_currentPage = (unsigned char*)malloc(m_pageSize);
	}

	if (m_currentScanMap.page != nullptr) {
		BridgeFree(m_currentScanMap.page);
	}

	if (m_currentScan == 1) {
		DbgMemMap(&m_currentScanMap);
		// For each memory region
		for (int iMap = 0; iMap < m_currentScanMap.count; iMap++) {
			MEMPAGE &memPage = m_currentScanMap.page[iMap];
			ptr_t base = (ptr_t)memPage.mbi.BaseAddress;
			size_t size = memPage.mbi.RegionSize;
			ptr_t end = base + size;

			// For each page in the memory region
			for (ptr_t p = base; p < end; p += m_pageSize) {
				size_t sz = m_pageSize;
				if (p + sz >= end) {
					sz = end - p;
				}
				DbgMemRead(p, m_currentPage, sz);

				// Perform search
				for (ptr_t s = 0; s < sz; s++) {
					if (m_currentPage[s] == find[0]) {
						if (memcmp(m_currentPage + s + 1, find + 1, findSize - 1) == 0) {
							SearchResult &result = m_results.Add();
							result.m_base = p;
							result.m_offset = s;

							result.m_valueFound = 0;
							if (findSize <= sizeof(uint64_t)) {
								memcpy(&result.m_valueFound, m_currentPage + s, findSize);
							}
						}
					} else {
						if (s + findSize > m_pageSize) {
							break;
						}
					}
				}
			}
		}
	}

	m_currentCompare = (unsigned char*)malloc(findSize);

	if (m_currentScan > 1) {
		for (int i = 0; i < m_results.Count(); i++) {
			SearchResult &result = m_results[i];
			DbgMemRead(result.m_base + result.m_offset, m_currentCompare, findSize);
			if (memcmp(m_currentCompare, find, findSize) != 0) {
				m_results.RemoveAt(i);
				i--;
			}
		}
	}

	free(find);
	free(m_currentCompare);
	m_currentCompare = nullptr;

	IupSetAttribute(m_hListResults, "REMOVEITEM", "ALL");
	for (int i = 0; i < m_results.Count(); i++) {
		SearchResult &result = m_results[i];

		ptr_t pointer = result.m_base + result.m_offset;

		s::String strLine;
		char label[MAX_LABEL_SIZE];
		if (DbgGetLabelAt(pointer, SEG_DEFAULT, label)) {
			strLine = s::strPrintF("%p %s", pointer, label);
		} else {
			strLine = s::strPrintF("%p", pointer);
		}

		IupSetAttribute(m_hListResults, "APPENDITEM", strLine);
	}

	IupSetAttribute(m_hButtonFirstScan, "ACTIVE", "YES");
	IupSetAttribute(m_hButtonNextScan, "ACTIVE", "YES");
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
	m_currentScanValueType = (SearchValueType)IupGetInt(m_hComboValueType, "VALUE");

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

	m_hCheckHex = IupToggle("Hex", "ScanHex");
	m_hTextInput = IupSetAttributes(IupText("ScanText"), "EXPAND=HORIZONTAL");
	Ihandle* hInput = IupSetAttributes(IupHbox(m_hCheckHex, m_hTextInput, nullptr), "MARGIN=0x0, GAP=5");

	m_hComboValueType = IupList(nullptr);
	IupSetAttribute(m_hComboValueType, "DROPDOWN", "YES");
	IupSetAttribute(m_hComboValueType, "1", "Byte");
	IupSetAttribute(m_hComboValueType, "2", "2 Bytes");
	IupSetAttribute(m_hComboValueType, "3", "4 Bytes");
	IupSetAttribute(m_hComboValueType, "4", "8 Bytes");
	IupSetAttribute(m_hComboValueType, "VALUE", "3");
	Ihandle* hValueType = IupSetAttributes(IupHbox(m_hComboValueType, nullptr), "MARGIN=0x0, GAP=5");

	Ihandle* vControls = IupSetAttributes(IupVbox(hButtons, hInput, hValueType, nullptr), "MARGIN=10x0, GAP=5");

	m_hListResults = IupList(nullptr);
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
