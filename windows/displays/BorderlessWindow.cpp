#include "BorderlessWindow.h"

// Window class name
const TCHAR* BorderlessWindow::className = _T("BorderlessWindow");

// Define window class structure
void BorderlessWindow::DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const {
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
	wcex.lpszClassName  = BorderlessWindow::className;
	wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
}

// Set window attributes
HWND BorderlessWindow::MakeWindow(CREATEPARAM_DEFS) {
	hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,				// Extended window styles
		BorderlessWindow::className,		// Window class name
		szTitle,						// Window title
		WS_POPUP,						// Window styles
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

// Create window
BorderlessWindow* BorderlessWindow::Create(CREATEPARAM_DEFS_SHORT) {
	return wll::CreateNewWindow<BorderlessWindow>(CREATEPARAMS_SHORT, NULL, NULL);
}

// Create window
BorderlessWindow* BorderlessWindow::Create(CREATEPARAM_DEFS) {
	return wll::CreateNewWindow<BorderlessWindow>(CREATEPARAMS);
}

BorderlessAttributes& BorderlessWindow::GetAttributes() const {
	wll::LayoutAttributes& attribs = GetLayoutManager().GetAttributes();
	return (BorderlessAttributes&) attribs;
}

// Constructor
BorderlessWindow::BorderlessWindow(BorderlessAttributes& attributes) : BaseWindow(attributes) {
	OutputDebugString(_T("MainWindow constructor\n"));
}

// Destructor
BorderlessWindow::~BorderlessWindow() {
	OutputDebugString(_T("MainWindow destructor\n"));
}