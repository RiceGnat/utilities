#include "MainWindow.h"

/// Data --------------------
// Constructor
MainWindow::Data::Data() {
	isMouseDown = false;
	isMouseTracking = false;
	overCloseButton = false;
}

/// Layout ------------------
MainWindow::Layout::Layout() {
	captionHeight = 24;
	captionButtonWidth = 24;
	lineHeight = 18;
	paddingX = 10;
	paddingY = 5;
}