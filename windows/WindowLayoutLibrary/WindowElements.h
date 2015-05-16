#include <windows.h>

#ifndef _INC_WINELEM
#define _INC_WINELEM

namespace wll {
	class WindowElement {
	protected:
		RECT rect;
		BOOL isVisible;
	public:
		virtual int CalculateRect(HDC hdc) = 0;
		virtual void Draw(HDC hdc) = 0;
		//virtual void Draw(HDC hdc, RECT& rect) = 0;
		LONG GetX() const;
		LONG GetY() const;
		LONG GetWidth() const;
		LONG GetHeight() const;
		void SetPosition(LONG x, LONG y);
		void SetSize(LONG width, LONG height);
		void SetVisible(BOOL visibility);

	protected: WindowElement();
	};

	class TextElement : public WindowElement {
	private:
		HFONT font;
		TCHAR* text;
		BOOL wrap;
	public:
		virtual int CalculateRect(HDC hdc);
		virtual void Draw(HDC hdc);
		//virtual void Draw(HDC hdc, RECT& rect);
		void SetFont(HFONT font);
		void SetText(const TCHAR* text);

	public: TextElement(const TCHAR* text, const HFONT font, LONG x, LONG y);
	public: TextElement(const TCHAR* text, const HFONT font, LONG x, LONG y, LONG width, LONG height = 0, BOOL wrap = false);
	public: ~TextElement();
	}; 
}

#endif // !_INC_WINELEM