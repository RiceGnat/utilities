#include "MainWindow.h"
#include "WindowUtility.h"
#include "FontProvider.h"

#include "MainWindowHandlerMacros.h"

using namespace appwin;

namespace {
	TextElement* caption;
	TextElement* mdownText;
	TextElement* mtrackText;
	TextElement* closeButton;
	HFONT captionFont = FontProvider::GetFont(_T("Segoe UI"), 24, FW_THIN);
	HFONT bodyFont = FontProvider::GetFont(_T("Segoe UI"), 18, FW_THIN);
	HFONT symbolFont = FontProvider::GetFont(_T("Segoe UI Symbol"), 18, FW_THIN);


}

LRESULT MainWindow::OnCreate(MESSAGEPARAM_DEFS) {
	RECT rect;
	GetWindowRect(hWnd, &rect);

	LayoutManager* layout = new LayoutManager();
	layout->attributes.paddingX = 10;
	layout->attributes.paddingY = 5;
	layout->attributes.lineHeight = 18;
	layout->attributes.width = rect.right - rect.left;
	layout->attributes.height = rect.bottom - rect.top;

	((MainWindow*)pWindow)->layout = layout;

	caption = new TextElement(
		pWindow->GetTitle(), captionFont,
		layout->attributes.paddingX,
		layout->attributes.paddingY,
		layout->attributes.width - layout->attributes.paddingX * 2 - 24, 24
	);
	layout->AddElement(caption);
	layout->AddSpace(24);

	mdownText = new TextElement(NULL, bodyFont);
	layout->AddNewLine(mdownText);
	
	mtrackText = new TextElement(_T("Mouse is tracking"), bodyFont);
	layout->AddNewLine(mtrackText);

	//closeButton = new TextElement(rect.right - rect.left - 24 + 5, 5, _T("\xe0a4"), symbolFont);

	return 0;
}

LRESULT MainWindow::OnPaint(MESSAGEPARAM_DEFS) {
	mdownText->SetText(WINDOW_MOUSEDOWN ? _T("Mouse is down") : _T("Mouse is up"));
	mtrackText->SetVisible(WINDOW_MOUSETRACKING);

	pWindow->DrawWindowElements();

	return 0;
}

LRESULT MainWindow::OnMouseMove(MESSAGEPARAM_DEFS) {
	if (!WINDOW_MOUSETRACKING) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;

		if (TrackMouseEvent(&tme)) {
			WINDOW_MOUSETRACKING = true;
			WINDOW_REDRAW();
		}
	}

	return 0;
}

LRESULT MainWindow::OnMouseLeave(MESSAGEPARAM_DEFS) {
	WINDOW_MOUSETRACKING = false;
	WINDOW_MOUSEDOWN = false;
	WINDOW_REDRAW();
	return 0;
}

LRESULT MainWindow::OnLButtonDown(MESSAGEPARAM_DEFS) {
	WINDOW_MOUSEDOWN = true;
	WINDOW_REDRAW();
	return 0;
}

LRESULT MainWindow::OnLButtonUp(MESSAGEPARAM_DEFS) {
	WINDOW_MOUSEDOWN = false;
	WINDOW_REDRAW();
	return 0;
}

LRESULT MainWindow::OnNCHitTest(MESSAGEPARAM_DEFS) {
	RECT localPos = winutil::GetWindowCoords(hWnd, MAKEPOINTS(lParam));

	// Title bar
	if (localPos.top < 24) {
		// Close button
		if (localPos.right < 24) {
			return HTCLOSE;
		}
		else if (localPos.right < 24 * 2) {
			return HTMINBUTTON;
		}

		return HTCAPTION;
	}
	else return HTCLIENT;
}

LRESULT MainWindow::OnNCLButtonDown(MESSAGEPARAM_DEFS) {
	// Handle HTCLOSE to stop mouse capture
	if (wParam == HTCLOSE) return 0;

	// Return default behavior otherwise to allow title bar dragging
	else return DefWindowProc(DEFAULT_MESSAGEPARAMS);
}

LRESULT MainWindow::OnNCLButtonUp(MESSAGEPARAM_DEFS) {
	// Handle HTCLOSE in the button up message to mimic default close button behavior
	if (wParam == HTCLOSE) {
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
	return 0;
}