#include <tchar.h>

#include "WindowElements.h"

using namespace appwin;

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

void TextElement::Draw(HDC hdc) {
	if (isVisible) {
		if (font) SelectObject(hdc, font);
		//TextOut(hdc, position.x, position.y, text, _tcslen(text));
		DrawText(hdc, text, _tcslen(text), &rect, 0);
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

TextElement::TextElement(const TCHAR* text, const HFONT font, LONG x, LONG y, LONG width, LONG height) {
	SetPosition(x, y);
	SetSize(width, height);
	SetText(text);
	SetFont(font);
}

TextElement::~TextElement() {
	delete text;
}