#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>

#define APPLICATION_NAME "Test application"

#include "wll.h"
#include "BorderlessWindow.h"

void RebuildWindow();
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

	BorderlessWindow* pWindow = BorderlessWindow::Create(hInstance, _T("New Window"), attribs);
	if (pWindow == NULL) return 0;

	// NOTE: Keep pointers in mind!
	// NOTE: Raw pointers should not own resources
	// NOTE: Avoid new/delete shenanigans inside classes

	// TODO: Fix BorderlessWindow inheritance and constructors
	// TODO: Tighten up LayoutManager reference/memory management
	// TODO: Responsive layout & redraw flow
	// TODO: Set up custom NC area (title bar, etc)

	wll::LayoutManager& layout = pWindow->GetLayoutManager();
	
	wll::WindowElement* element = new wll::TextElement(_T("Test text\nTesttjfjg"), wll::FontProvider::GetFont(_T("Segoe UI"), attribs.lineHeight, 200), 0, 0);
	wll::WindowElement* element2 = new wll::TextElement(_T("adhdgdfgdfg"), wll::FontProvider::GetFont(_T("Segoe UI"), attribs.lineHeight, 200), 0, 0);

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

void RebuildWindow() {

}

void Cleanup() {
	wll::FontProvider::Flush();
}