#include <windows.h>

#ifndef _INC_WINELEM
#define _INC_WINELEM

#ifndef TE_MAXLENGTH
#define TE_MAXLENGTH 255
#endif

namespace wll {
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
		TCHAR text[TE_MAXLENGTH];
		BOOL wrap;
		virtual void CalculateRect();
	public:
		virtual void Draw(HDC hdc);
		void SetFont(HFONT font);
		void SetText(const TCHAR* text);

	public: TextElement(const TCHAR* text, const HFONT font, LONG x, LONG y, LONG width = 0, LONG height = 0, BOOL wrap = FALSE);
	public: ~TextElement();
	}; 
}

#endif // !_INC_WINELEM