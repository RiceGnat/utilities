#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#define APPLICATION_NAME "Test application"

#include "wll.h"

void Cleanup();

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	wll::LayoutAttributes attribs;
	attribs.lineHeight = 18;
	attribs.paddingX = 10;
	attribs.paddingY = 5;

	wll::BaseWindow* pWindow = wll::BaseWindow::Create(hInstance, _T("New Window"), attribs);
	if (pWindow == NULL) return 0;

	// NOTE: Keep pointers in mind!
	// NOTE: Raw pointers should not own resources
	// NOTE: Avoid new/delete shenanigans inside classes

	// TODO: Responsive layout & redraw flow
	// TODO: Figure out how to set window attributes vs class attributes
	// TODO: Establish proper inheritance rules and clean up classes
	// TODO: Set up custom NC area (title bar, etc)

	wll::LayoutManager& layout = pWindow->GetLayoutManager();
	
	wll::WindowElement* element = new wll::TextElement(_T("Test text\nTesttjfjg"), wll::FontProvider::GetFont(_T("Segoe UI"), 18, 200), 0, 0);
	wll::WindowElement* element2 = new wll::TextElement(_T("adhdgdfgdfg"), wll::FontProvider::GetFont(_T("Segoe UI"), 18, 200), 0, 0);

	layout.AddNewLine(element, 0);
	layout.AddNewLine(element2);

	pWindow->Show(nCmdShow);
	pWindow->Update();

	// Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete pWindow;
	delete element;
	delete element2;

	Cleanup();

	return (int) msg.wParam;
}

void Cleanup() {
	wll::FontProvider::Flush();
}