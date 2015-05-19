#include "BorderlessWindow.h"
#include "WindowUtility.h"

LRESULT BorderlessWindow::OnNCHitTest(MESSAGEPARAM_DEFS) {
	RECT localPos = winutil::GetWindowCoords(hWnd, MAKEPOINTS(lParam));

	// Title bar
	if (localPos.top < WINDOW_ATTRIBUTES.captionHeight) {
		// Close button
		if (localPos.right < WINDOW_ATTRIBUTES.captionButtonWidth) {
			return HTCLOSE;
		}
		else if (localPos.right < WINDOW_ATTRIBUTES.captionButtonWidth * 2) {
			return HTMINBUTTON;
		}

		return HTCAPTION;
	}
	else return HTCLIENT;
}

LRESULT BorderlessWindow::OnNCLButtonDown(MESSAGEPARAM_DEFS) {
	// Handle HTCLOSE to stop mouse capture
	if (wParam == HTCLOSE) return 0;

	// Return default behavior otherwise to allow title bar dragging
	else return DefWindowProc(DEFAULT_MESSAGEPARAMS);
}

LRESULT BorderlessWindow::OnNCLButtonUp(MESSAGEPARAM_DEFS) {
	// Handle HTCLOSE in the button up message (not button down) to mimic default close button behavior
	if (wParam == HTCLOSE) {
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
	return 0;
}