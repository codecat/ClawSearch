#include "plugin.h"

#include "csMain.h"

static void executeOnGuiThreadAndWait(void(*worker)())
{
	struct Context
	{
		HANDLE event;
		void(*worker)();
	};
	auto context = Context{ CreateEventW(nullptr, true, false, nullptr), worker };
	GuiExecuteOnGuiThreadEx([](void* data)
	{
		auto context = (Context*)data;
		context->worker();
		SetEvent(context->event);
	}, &context);
	WaitForSingleObject(context.event, INFINITE);
	CloseHandle(context.event);
}

enum
{
	MENU_OPENSEARCH
};

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
	return true; //Return false to cancel loading the plugin.
}

//Do GUI/Menu related things here.
void pluginSetup()
{
	_plugin_menuaddentry(hMenu, MENU_OPENSEARCH, "Open search dialog");
	_plugin_menuaddentry(hMenuDump, MENU_OPENSEARCH, "ClawSearch");

	// Initialize the UI on the same thread as x64dbg's UI
	executeOnGuiThreadAndWait([]
	{
		IupOpen(nullptr, nullptr);
		IupControlsOpen();

		IupSetGlobal("LOCKLOOP", "YES");
	});
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
	_plugin_menuclear(hMenu);
	_plugin_menuclear(hMenuDisasm);
	_plugin_menuclear(hMenuDump);
	_plugin_menuclear(hMenuStack);

	executeOnGuiThreadAndWait([]
	{
		CloseSearch();
		IupClose();
	});

	return true;
}

// Hack for Iup
int main() { return 0; }
