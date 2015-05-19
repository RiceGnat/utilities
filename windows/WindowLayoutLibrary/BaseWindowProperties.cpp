#include "BaseWindow.h"

using namespace wll;

// Override these to customize and extend functionality

// Window class name
const TCHAR* BaseWindow::className = _T("BaseWindow");

// Define window class structure
void BaseWindow::DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const {
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WindowProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = BaseWindow::className;
	wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
}

// Set window attributes
HWND BaseWindow::MakeWindow(CREATEPARAM_DEFS) {
	hWnd = CreateWindowEx(
		NULL,							// Extended window styles
		BaseWindow::className,			// Window class name
		szTitle,						// Window title
		WS_OVERLAPPEDWINDOW,			// Window styles
		CW_USEDEFAULT, CW_USEDEFAULT,	// Default window position
		500, 500,						// Window size
		hParent,						// Handle to parent window
		hMenu,							// Handle to menu
		hInstance,						// Handle to module instance
		this							// Pointer sent to window
	);

	SetWindowText(hWnd, szTitle);

	return hWnd;
}

void BaseWindow::InitLayoutManager(LayoutAttributes& attributes) {
	layout.reset(new LayoutManager(attributes));
}

void BaseWindow::InitData() {
	data = NULL;
}

void BaseWindow::PrePaint() {
	// Called prior to draw call, override to add functionality
}

// Default message handler
LRESULT BaseWindow::RoutMessage(MESSAGEPARAM_DEFS) {
	return DefWindowProc(DEFAULT_MESSAGEPARAMS);
}
