#include "WindowUtility.h"

namespace winutil {
	POINTS& operator+= (POINTS& lhs, POINTS const& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}

	POINTS& operator-= (POINTS& lhs, POINTS const& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}

	POINTS operator+ (POINTS const& lhs, POINTS const& rhs) {
		POINTS p = lhs;
		p += rhs;
		return p;
	}

	POINTS operator- (POINTS const& lhs, POINTS const& rhs) {
		POINTS p = lhs;
		p -= rhs;
		return p;
	}

	RECT GetWindowCoords(HWND hWnd, POINTS screenCoords) {
		RECT windowRect, windowCoords;
		GetWindowRect(hWnd, &windowRect);

		// Get distance from top left corner
		windowCoords.left = screenCoords.x - windowRect.left;
		windowCoords.top = screenCoords.y - windowRect.top;

		// Get distance from bottom right corner
		windowCoords.right = windowRect.right - screenCoords.x;
		windowCoords.bottom = windowRect.bottom - screenCoords.y;

		// Translate screen coordinates to originate at top left of window
		return windowCoords;
	}
}