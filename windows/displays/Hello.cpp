#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#include <windows.h>
#include <tchar.h>

#define APPLICATION_NAME "Test application"

#include "BaseWindow.h"


int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	wll::BaseWindow* pWindow = wll::BaseWindow::Create(hInstance, _T("New Window"));
	if (pWindow == NULL) return 0;

	// TODO: Figure out redraw flow
	// TODO: Figure out how to set window attributes vs class attributes
	// TODO: Establish proper inheritance rules and clean up classes
	// TODO: Set up custom NC area (title bar, etc)

	pWindow->Show(nCmdShow);
	pWindow->Update();

	// Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete pWindow;

	return (int) msg.wParam;
	return 0;
}