#include "AppWindow.h"

using namespace appwin;

// Window procedure callback
LRESULT CALLBACK AppWindow::WindowProc(DEFAULT_MESSAGEPARAM_DEFS) {
	AppWindow* pWindow = (AppWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (uMsg)
	{
	case WM_NCCREATE:			// Window creation
		{
			// Get AppWindow pointer from lParam
			CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
			pWindow = (AppWindow*)pcs->lpCreateParams;

			// Store window handle in AppWindow object
			pWindow->hWnd = hWnd;

			// Set window user data pointer to AppWindow object
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pcs->lpCreateParams);

			// Return TRUE to continue creation of window
			return TRUE;
		}
	case WM_DESTROY:			// Window destruction
		{
			// Clear AppWindow's window handle
			if (pWindow) pWindow->hWnd = NULL;

			// This signals the application to exit once the window is destroyed
			PostQuitMessage(0);
			return 0;
		}
	default: // pWindow should be created already, but we will check just in case
		{
			// Send all other messages to their respective handlers
			if (pWindow) return pWindow->RoutMessage(MESSAGEPARAMS);
			// Or use default message handler if object is not ready yet
			else return DefWindowProc(DEFAULT_MESSAGEPARAMS);
		}
	}
}

// Default message handler
LRESULT AppWindow::RoutMessage(MESSAGEPARAM_DEFS) const {
	return DefWindowProc(DEFAULT_MESSAGEPARAMS);
}

// Add a window element
void AppWindow::AddWindowElement(WindowElement* element) {
	elements.push_back(element);
}

// Draw all window elements
void AppWindow::DrawWindowElements() const {
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd, &ps);

	layout->DrawAllElements(hdc);

	EndPaint(hWnd, &ps);
}

// Create window
AppWindow* AppWindow::Create(CREATEPARAM_DEFS) {
	return CreateAppWindow<AppWindow>(CREATEPARAMS);
}

// Show window
BOOL AppWindow::Show(int nCmdShow) {
	return ShowWindow(hWnd, nCmdShow);
}

// Update window
BOOL AppWindow::Update() {
	return UpdateWindow(hWnd);
}

// Redraw window
BOOL AppWindow::Redraw() {
	return RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE| RDW_UPDATENOW);
}

// Get window title
const TCHAR* AppWindow::GetTitle() const {
	return title;
}

// Constructor
AppWindow::AppWindow() {
	OutputDebugString(_T("AppWindow constructor\n"));
	hWnd = NULL;
	data = NULL;
	layout = NULL;
	title = NULL;
}

// Destructor
AppWindow::~AppWindow() {
	OutputDebugString(_T("AppWindow destructor\n"));
	if (hWnd) DestroyWindow(hWnd);
	if (data) delete data;
	if (layout) delete layout;
	elements.clear();
}