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
	BorderlessAttributes attribs = BorderlessAttributes();
	attribs.width = 200;
	attribs.height = 400;
	attribs.autoSize = false;
	attribs.minWidth = 20;
	attribs.minHeight = 20;
	attribs.paddingX = 10;
	attribs.paddingY = 5;
	attribs.lineHeight = 18;
	attribs.indentWidth = 0;
	attribs.indentLevel = 0;
	attribs.captionHeight = 24;
	attribs.captionButtonWidth = 24;

	wll::BaseWindow* pWindow = BorderlessWindow::Create(hInstance, _T("New Window"), attribs);
	if (pWindow == NULL) return 0;

	// NOTE: Keep pointers in mind!
	// NOTE: Raw pointers should not own resources
	// NOTE: Avoid new/delete shenanigans inside classes
	// NOTE: LayoutManager is internally managed
	// NOTE: Data object is externally managed, internal dependency ok but make sure proper control

	// TODO: Add attributes for font size, caption font size
	// TODO: Figure out title bar offset for BorderlessWindow
	// TODO: Redraw flow
	// TODO: Set up custom NC area (title bar, etc)

	wll::LayoutManager& layout = pWindow->GetLayoutManager();
	
	//wll::WindowElement* element = new wll::TextElement(_T("Test text\nTesttjfjg"), wll::FontProvider::GetFont(_T("Segoe UI"), attribs.lineHeight, 200), 0, 0);
	//wll::WindowElement* element2 = new wll::TextElement(_T("adhdgdfgdfg"), wll::FontProvider::GetFont(_T("Segoe UI"), attribs.lineHeight, 200), 0, 0);

	//layout.AddNewLine(element, 0);
	//layout.AddNewLine(element2);

	//BorderlessAttributes test = pWindow->GetAttributes();

	pWindow->Show(nCmdShow);
	pWindow->Update();

	// Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete pWindow;
	//delete element;
	//delete element2;

	Cleanup();

	return (int) msg.wParam;
}

void RebuildWindow() {

}

void Cleanup() {
	wll::FontProvider::Flush();
}