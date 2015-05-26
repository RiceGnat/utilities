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
	return BaseWindow::MakeWindow(CREATEPARAMS, BorderlessWindow::className, WS_POPUP, WS_EX_TOOLWINDOW);
}

// Create window
BorderlessWindow* BorderlessWindow::Create(CREATEPARAM_DEFS_SHORT, BorderlessAttributes& attributes) {
	return wll::CreateNewWindow<BorderlessWindow>(CREATEPARAMS_SHORT, NULL, NULL, attributes);
}

// Create window
BorderlessWindow* BorderlessWindow::Create(CREATEPARAM_DEFS, BorderlessAttributes& attributes) {
	return wll::CreateNewWindow<BorderlessWindow>(CREATEPARAMS, attributes);
}

// Message router
LRESULT BorderlessWindow::RoutMessage(MESSAGEPARAM_DEFS) {
	// Assume pWindow is not NULL
	switch (uMsg) {
	//case WM_CREATE:				// Client area creation
	//	return OnCreate(MESSAGEPARAMS);
	//case WM_PAINT:				// Paint call
	//	return OnPaint(MESSAGEPARAMS);

	//case WM_MOUSEMOVE:			// Mouse is moving in window
	//	return OnMouseMove(MESSAGEPARAMS);

	//case WM_LBUTTONDOWN:		// Left mouse button down
	//	return OnLButtonDown(MESSAGEPARAMS);

	//case WM_LBUTTONUP:			// Left mouse button up
	//	return OnLButtonUp(MESSAGEPARAMS);

	//case WM_MOUSELEAVE:			// Mouse left window
	//case WM_KILLFOCUS:			// Window lost focus
	//	return OnMouseLeave(MESSAGEPARAMS);

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

BorderlessAttributes& BorderlessWindow::GetAttributes() const {
	wll::LayoutAttributes& attribs = GetLayoutManager().attributes;
	return (BorderlessAttributes&) attribs;
}

// Constructor
BorderlessWindow::BorderlessWindow(wll::LayoutAttributes& attributes) : BaseWindow(attributes) {
	OutputDebugString(_T("BorderlessWindow constructor\n"));
}

// Destructor
BorderlessWindow::~BorderlessWindow() {
	OutputDebugString(_T("BorderlessWindow destructor\n"));
}