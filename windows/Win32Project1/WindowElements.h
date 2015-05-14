#include <windows.h>

#ifndef _INC_WINELEM
#define _INC_WINELEM

namespace appwin {
	class WindowElement {
	protected:
		RECT rect;
		BOOL isVisible;
	public:
		virtual void Draw(HDC hdc) = 0;
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
	public:
		virtual void Draw(HDC hdc);
		void SetFont(HFONT font);
		void SetText(const TCHAR* text);

	public: TextElement(const TCHAR* text, const HFONT font, LONG x = 0, LONG y = 0, LONG width = 0, LONG height = 0);
	public: ~TextElement();
	}; 
}

#endif // !_INC_WINELEM