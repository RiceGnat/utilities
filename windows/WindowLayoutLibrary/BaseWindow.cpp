#include "BaseWindow.h"

using namespace wll;

// Window procedure callback
LRESULT CALLBACK BaseWindow::WindowProc(DEFAULT_MESSAGEPARAM_DEFS) {
	BaseWindow* pWindow = (BaseWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (uMsg)
	{
	case WM_NCCREATE:			// Window creation
		{
			// Get BaseWindow pointer from lParam
			CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
			pWindow = (BaseWindow*)pcs->lpCreateParams;

			// Store window handle in BaseWindow object
			pWindow->hWnd = hWnd;

			// Set window user data pointer to BaseWindow object
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pcs->lpCreateParams);

			// Return TRUE to continue creation of window
			return TRUE;
		}
	case WM_DESTROY:			// Window destruction
		{
			// Clear BaseWindow's window handle
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
LRESULT BaseWindow::RoutMessage(MESSAGEPARAM_DEFS) const {
	return DefWindowProc(DEFAULT_MESSAGEPARAMS);
}

// Create window
BaseWindow* BaseWindow::Create(CREATEPARAM_DEFS) {
	return CreateNewWindow<BaseWindow>(CREATEPARAMS);
}

// Show window
BOOL BaseWindow::Show(int nCmdShow) {
	return ShowWindow(hWnd, nCmdShow);
}

// Update window
BOOL BaseWindow::Update() {
	return UpdateWindow(hWnd);
}

// Redraw window
BOOL BaseWindow::Redraw() {
	return RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE| RDW_UPDATENOW);
}

// Get window title
const TCHAR* BaseWindow::GetTitle() const {
	return title;
}

// Constructor
BaseWindow::BaseWindow() {
	OutputDebugString(_T("BaseWindow constructor\n"));
	hWnd = NULL;
	data = NULL;
	layout = NULL;
	title = NULL;
}

// Destructor
BaseWindow::~BaseWindow() {
	OutputDebugString(_T("BaseWindow destructor\n"));
	if (hWnd) DestroyWindow(hWnd);
	if (data) delete data;
	if (layout) delete layout;
}