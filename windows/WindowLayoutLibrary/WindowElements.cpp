#include <tchar.h>

#include "WindowElements.h"

using namespace wll;

LONG WindowElement::GetX() const { return rect.left; }
LONG WindowElement::GetY() const { return rect.top; }
LONG WindowElement::GetWidth() const { return rect.right - rect.left; }
LONG WindowElement::GetHeight() const { return rect.bottom - rect.top; }

void WindowElement::SetPosition(LONG x, LONG y) {
	rect.right += x - rect.left;
	rect.left = x;
	rect.bottom += y - rect.top;
	rect.top = y;
}

void WindowElement::SetSize(LONG width, LONG height) {
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

void WindowElement::SetVisible(BOOL visibility) {
	isVisible = visibility;
}

WindowElement::WindowElement() {
	SetVisible(true);
}

int TextElement::CalculateRect(HDC hdc) {
	// Set font
	if (font) SelectObject(hdc, font);

	// Create a new 1x1 rect (DT_CALCRECT + DT_WORDBREAK doesn't work with 0x0)
	RECT calcRect = rect;
	calcRect.right = rect.left + 1;
	calcRect.bottom = rect.top + 1;

	// Calculate the text rect
	DrawText(hdc, text, _tcslen(text), &calcRect, DT_CALCRECT | (wrap * DT_WORDBREAK));

	// If width or height were defined (nonzero), adjust calculated rect
	if (rect.right != rect.left)
		calcRect.right = rect.right;
	if (rect.bottom != rect.top)
		calcRect.bottom = rect.bottom;
	
	// Save calculated rect
	rect = calcRect;
	
	// Return height
	return rect.bottom - rect.top;
}

void TextElement::Draw(HDC hdc) {
	if (isVisible) {
		// Set font
		if (font) SelectObject(hdc, font);

		// Draw text
		DrawText(hdc, text, _tcslen(text), &rect, DT_END_ELLIPSIS | (wrap * DT_WORDBREAK));
	}
}

void TextElement::SetFont(HFONT font) {
	this->font = font;
}

void TextElement::SetText(const TCHAR* text) {
	if (text) {
		this->text = new TCHAR[_tcslen(text)];
		_tcscpy_s(this->text, _tcslen(text) + 1, text);
	}
	else this->text = _T("");
}

TextElement::TextElement(const TCHAR* text, const HFONT font, LONG x, LONG y) {
	SetPosition(x, y);
	SetSize(0, 0);
	SetText(text);
	SetFont(font);
	wrap = FALSE;
	CalculateRect(GetDC(NULL));
}

TextElement::TextElement(const TCHAR* text, const HFONT font, LONG x, LONG y, LONG width, LONG height, BOOL wrap) {
	SetPosition(x, y);
	SetSize(width, height);
	SetText(text);
	SetFont(font);
	this->wrap = wrap;
}

TextElement::~TextElement() {
	delete text;
}