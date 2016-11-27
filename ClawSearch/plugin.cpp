#include "plugin.h"

#include "csMain.h"

enum
{
	MENU_OPENSEARCH
};

PLUG_EXPORT void CBINITDEBUG(CBTYPE cbType, PLUG_CB_INITDEBUG* info)
{
}

PLUG_EXPORT void CBSTOPDEBUG(CBTYPE cbType, PLUG_CB_STOPDEBUG* info)
{
}

PLUG_EXPORT void CBEXCEPTION(CBTYPE cbType, PLUG_CB_EXCEPTION* info)
{
}

PLUG_EXPORT void CBDEBUGEVENT(CBTYPE cbType, PLUG_CB_DEBUGEVENT* info)
{
}

PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
	switch (info->hEntry) {
	case MENU_OPENSEARCH:
		OpenSearch();
		break;

	default:
		break;
	}
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
	IupOpen(nullptr, nullptr);
	IupControlsOpen();

	IupSetGlobal("LOCKLOOP", "YES");

	return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
	IupClose();

	_plugin_menuclear(hMenu);
	_plugin_menuclear(hMenuDisasm);
	_plugin_menuclear(hMenuDump);
	_plugin_menuclear(hMenuStack);

	return true;
}

//Do GUI/Menu related things here.
void pluginSetup()
{
	_plugin_menuaddentry(hMenu, MENU_OPENSEARCH, "Open search dialog");
	_plugin_menuaddentry(hMenuDump, MENU_OPENSEARCH, "ClawSearch");
}

// Hack for Iup
int main() { return 0; }
