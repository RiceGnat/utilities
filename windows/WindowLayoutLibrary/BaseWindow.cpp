#include "BaseWindow.h"
#include "FontProvider.h"

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
	case WM_PAINT:				// Window draw call
		{
			if (pWindow) {
				// Resize window if autosize is enabled
				if (pWindow->WINDOW_ATTRIBUTES.autoSize) {
					SetWindowPos(
						hWnd, NULL, 0, 0,
						pWindow->WINDOW_ATTRIBUTES.width,
						pWindow->WINDOW_ATTRIBUTES.height,
						SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
				}

				pWindow->PrePaint();

				PAINTSTRUCT ps;
				HDC hdc;

				hdc = BeginPaint(pWindow->hWnd, &ps);

				pWindow->WINDOW_LAYOUT.DrawAllElements(hdc);

				EndPaint(pWindow->hWnd, &ps);
				return 0;
			}
		}
	case WM_SIZE:				// Window resize
		{
			if (pWindow) {
				if (pWindow->WINDOW_ATTRIBUTES.autoSize) {

				}
				else {
					POINTS size = MAKEPOINTS(lParam);
					pWindow->WINDOW_ATTRIBUTES.width = size.x;
					pWindow->WINDOW_ATTRIBUTES.height = size.y;
					pWindow->WINDOW_LAYOUT.Refresh();
				}
			}
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

// Create window
BaseWindow* BaseWindow::Create(CREATEPARAM_DEFS_SHORT, wll::LayoutAttributes& attributes) {
	return CreateNewWindow<BaseWindow>(CREATEPARAMS_SHORT, NULL, NULL, attributes);
}

// Create window
BaseWindow* BaseWindow::Create(CREATEPARAM_DEFS, wll::LayoutAttributes& attributes) {
	return CreateNewWindow<BaseWindow>(CREATEPARAMS, attributes);
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

// Get layout manager
LayoutManager& BaseWindow::GetLayoutManager() const {
	return *(layout.get());
}

// Get layout attributes
LayoutAttributes& BaseWindow::GetAttributes() const {
	return GetLayoutManager().attributes;
}

// Constructor
BaseWindow::BaseWindow(LayoutAttributes& attributes) {
	OutputDebugString(_T("BaseWindow constructor\n"));
	hWnd = NULL;
	title = NULL;
	InitLayoutManager(attributes);
	InitData();
}

// Destructor
BaseWindow::~BaseWindow() {
	OutputDebugString(_T("BaseWindow destructor\n"));
	if (hWnd) DestroyWindow(hWnd);
}