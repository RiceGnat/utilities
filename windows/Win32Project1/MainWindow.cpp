#include "MainWindow.h"

// Window class name
const TCHAR* MainWindow::className = _T("MainWindow");

// Define window class structure
void MainWindow::DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const {
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
	wcex.lpszClassName  = MainWindow::className;
	wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
}

// Set window attributes
HWND MainWindow::MakeWindow(HINSTANCE hInstance, TCHAR* szTitle, LPVOID lpParam) const {
	return CreateWindowEx(
		WS_EX_TOOLWINDOW,		// Extended window styles: WS_EX_TOOLWINDOW used to hide taskbar button
		MainWindow::className,// Window class name
		szTitle,				// Window title
		WS_POPUP,				// Window styles: WS_POPUP makes window with no frame
		CW_USEDEFAULT, CW_USEDEFAULT, // Default window position (0, 0 for WS_POPUP)
		300, 200,				// Window size
		NULL,					// Handle to parent window
		NULL,					// Handle to menu
		hInstance,				// Handle to module instance
		lpParam					// Pointer sent to window
	);
}

// Create window
MainWindow* MainWindow::Create(CREATEPARAM_DEFS) {
	return appwin::CreateAppWindow<MainWindow>(CREATEPARAMS);
}

// Message router
LRESULT MainWindow::RoutMessage(MESSAGEPARAM_DEFS) const {
	// Assume pWindow is not NULL
	switch (uMsg) {
	case WM_CREATE:				// Client area creation
		return OnCreate(MESSAGEPARAMS);
	case WM_PAINT:				// Paint call
		return OnPaint(MESSAGEPARAMS);

	case WM_MOUSEMOVE:			// Mouse is moving in window
		return OnMouseMove(MESSAGEPARAMS);

	case WM_LBUTTONDOWN:		// Left mouse button down
		return OnLButtonDown(MESSAGEPARAMS);

	case WM_LBUTTONUP:			// Left mouse button up
		return OnLButtonUp(MESSAGEPARAMS);

	case WM_MOUSELEAVE:			// Mouse left window
	case WM_KILLFOCUS:			// Window lost focus
		return OnMouseLeave(MESSAGEPARAMS);

	case WM_NCHITTEST:			// Test for non-client area elements
		return OnNCHitTest(MESSAGEPARAMS);

	case WM_NCLBUTTONDOWN:
		return OnNCLButtonDown(MESSAGEPARAMS);

	case WM_NCLBUTTONUP:
		return OnNCLButtonUp(MESSAGEPARAMS);
	}

	// Unhandled messages should fall here
	return DefWindowProc(DEFAULT_MESSAGEPARAMS);
}

// Constructor
MainWindow::MainWindow() {
	OutputDebugString(_T("MainWindow constructor\n"));
	data = new Data;
}

// Destructor
MainWindow::~MainWindow() {
	OutputDebugString(_T("MainWindow destructor\n"));
}