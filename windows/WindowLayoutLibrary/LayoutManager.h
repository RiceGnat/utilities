#include <vector>

#include <windows.h>
#include <tchar.h>

#include "WindowElements.h"

#ifndef _INC_LAYOUT
#define _INC_LAYOUT

#define LM_UPPERLEFT	0
#define LM_UPPERRIGHT	1
#define LM_LOWERLEFT	2
#define LM_LOWERRIGHT	3

namespace wll {
	struct LayoutAttributes {
		int paddingX;
		int paddingY;
		int lineHeight;
		int width;
		int height;
	};

	class LayoutManager {
	/// MEMBERS ----------------------------------------------
	private:
		// Window elements
		std::vector<wll::WindowElement*> elements;

		// Position for next line
		int nextLine;

		// Move next line position
		void MoveNextLine(int lineHeight);

	public:
		// Window metrics
		LayoutAttributes attributes;

		// Add a new element
		virtual void AddElement(WindowElement* element);

		// Add a new absolutely positioned element
		virtual void AddElement(WindowElement* element, LONG x, LONG y);

		// Add a new absolutely positioned element relative to a specified corner
		virtual void AddElement(WindowElement* element, LONG x, LONG y, unsigned int corner);

		// Add a new line
		virtual void AddNewLine(WindowElement* element);

		// Add a new line with a custom line height
		virtual void AddNewLine(WindowElement* element, unsigned int lineHeight);

		// Add a new multi-line element
		virtual void AddNewMultiline(WindowElement* element, unsigned int lines);

		// Add a blank line
		virtual void AddBlankLine();

		// Add space before the next line
		virtual void AddSpace(unsigned int lineHeight);

		// Draw all elements in the given device context
		void DrawAllElements(HDC hdc);

	/// CONSTRUCTOR/DESTRUCTOR -------------------------------
	public: LayoutManager();
	public: ~LayoutManager();
	};
}

#endif // !_INC_LAYOUT