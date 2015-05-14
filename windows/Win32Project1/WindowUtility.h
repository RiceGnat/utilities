#include <windows.h>

#ifndef _INC_WINUTIL
#define _INC_WINUTIL

namespace winutil {
	// POINTS operator overloading

	POINTS& operator+= (POINTS& lhs, POINTS const& rhs);
	POINTS& operator-= (POINTS& lhs, POINTS const& rhs);
	POINTS operator+ (POINTS const& lhs, POINTS const& rhs);
	POINTS operator- (POINTS const& lhs, POINTS const& rhs);

	// Transform coordinates from screen space to window space
	RECT GetWindowCoords(HWND hWnd, POINTS screenCoords);
}

#endif // !_INC_WINUTIL