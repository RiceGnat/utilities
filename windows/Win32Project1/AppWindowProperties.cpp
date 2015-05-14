#include "AppWindow.h"

using namespace appwin;

// Window class name
const TCHAR* AppWindow::className = _T("AppWindow");

// Define window class structure
void AppWindow::DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const {
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
	wcex.lpszClassName  = AppWindow::className;
	wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
}

// Set window attributes
HWND AppWindow::MakeWindow(HINSTANCE hInstance, TCHAR* szTitle, LPVOID lpParam) const {
	return CreateWindowEx(
		NULL,					// Extended window styles
		AppWindow::className,	// Window class name
		szTitle,				// Window title
		WS_OVERLAPPEDWINDOW,	// Window styles
		CW_USEDEFAULT, CW_USEDEFAULT, // Default window position
		500, 100,				// Window size
		NULL,					// Handle to parent window
		NULL,					// Handle to menu
		hInstance,				// Handle to module instance
		lpParam					// Pointer sent to window
	);
}