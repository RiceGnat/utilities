#include "BaseWindow.h"
#include "FontProvider.h"

using namespace wll;

// Create the window
HWND BaseWindow::MakeWindow(CREATEPARAM_DEFS, const TCHAR* className, DWORD windowStyles, DWORD windowExStyles) {
	hWnd = CreateWindowEx(
		windowExStyles,					// Extended window styles
		className,						// Window class name
		szTitle,						// Window title
		windowStyles,					// Window styles
		CW_USEDEFAULT, CW_USEDEFAULT,	// Default window position
		WINDOW_ATTRIBUTES.autoSize ? WINDOW_ATTRIBUTES.minWidth : WINDOW_ATTRIBUTES.width,
		WINDOW_ATTRIBUTES.autoSize ? WINDOW_ATTRIBUTES.minHeight : WINDOW_ATTRIBUTES.height,
		hParent,						// Handle to parent window
		hMenu,							// Handle to menu
		hInstance,						// Handle to module instance
		this							// Pointer sent to window
	);

	SetWindowText(hWnd, szTitle);

	return hWnd;
}

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
					RECT clientRect;
					RECT windowRect;

					GetClientRect(hWnd, &clientRect);
					GetWindowRect(hWnd, &windowRect);

					// Correct for window borders
					int width = windowRect.right - windowRect.left - (clientRect.right - clientRect.left) + pWindow->WINDOW_ATTRIBUTES.width;
					int height = windowRect.bottom - windowRect.top - (clientRect.bottom - clientRect.top) + pWindow->WINDOW_ATTRIBUTES.height;

					// Set window size
					SetWindowPos(
						hWnd, NULL, 0, 0,
						width,
						height,
						SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
				}


				PAINTSTRUCT ps;
				HDC hdc;

				hdc = BeginPaint(pWindow->hWnd, &ps);
				
				pWindow->PrePaint(hdc);
				pWindow->WINDOW_LAYOUT.DrawAllElements(hdc);

				EndPaint(pWindow->hWnd, &ps);
				return 0;
			}
			else break;
		}
	case WM_SIZE:				// Window resize
		{
			if (pWindow) {
				if (pWindow->WINDOW_ATTRIBUTES.autoSize) {

				}
				else {
					POINTS size = MAKEPOINTS(lParam);	// Size of the client area
					pWindow->WINDOW_ATTRIBUTES.width = size.x;
					pWindow->WINDOW_ATTRIBUTES.height = size.y;
					pWindow->WINDOW_LAYOUT.Refresh();
				}
				return 0;
			}
			else break;
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

// Set data object
void BaseWindow::SetData(void* pData) {
	data = pData;
}

// Constructor
BaseWindow::BaseWindow(LayoutAttributes& attributes) {
	OutputDebugString(_T("BaseWindow constructor\n"));
	hWnd = NULL;
	title = NULL;
	InitLayoutManager(attributes);
	data = NULL;
}

// Destructor
BaseWindow::~BaseWindow() {
	OutputDebugString(_T("BaseWindow destructor\n"));
	if (hWnd) DestroyWindow(hWnd);
}