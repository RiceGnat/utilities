#include "BorderlessWindow.h"
#include "FontProvider.h"
#include "WindowUtility.h"

using wll::FontProvider;

void BorderlessWindow::PrePaint(HDC hdc) {
	RECT rect;
	rect.left = WINDOW_ATTRIBUTES.paddingX;
	rect.top = 5;
	rect.right = WINDOW_ATTRIBUTES.width -  WINDOW_ATTRIBUTES.captionButtonWidth;
	rect.bottom = WINDOW_ATTRIBUTES.captionHeight;
	
	HGDIOBJ obj = SelectObject(hdc, FontProvider::GetFont(_T("Segoe UI"), WINDOW_ATTRIBUTES.captionHeight, 200));
	DrawText(hdc, title, _tcslen(title), &rect, DT_SINGLELINE | DT_END_ELLIPSIS);
	if (obj) SelectObject(hdc, obj);
}

LRESULT BorderlessWindow::OnNCHitTest(MESSAGEPARAM_DEFS) {
	RECT localPos = winutil::GetWindowCoords(hWnd, MAKEPOINTS(lParam));

	// Title bar
	if (localPos.top < WINDOW_ATTRIBUTES.captionHeight) {
		// Close button
		if (localPos.right < WINDOW_ATTRIBUTES.captionButtonWidth) {
			return HTCLOSE;
		}
		else return HTCAPTION;
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