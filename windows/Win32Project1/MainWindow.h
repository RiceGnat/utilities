#include "AppWindow.h"

#ifndef _INC_MAINWIN
#define _INC_MAINWIN

class MainWindow : public appwin::AppWindow {
/// FRIENDS ----------------------------------------------
	// Give access to creation function template
	friend MainWindow* appwin::CreateAppWindow<MainWindow>(CREATEPARAM_DEFS);

/// INTERNAL CLASSES -------------------------------------
private:
	struct Data;
	class Fonts;
public:
	struct Layout;
		
/// STATIC MEMBERS ---------------------------------------
private:
	// Window class name
	static const TCHAR* className;

	// Hide base creation function
	AppWindow::Create;

	// Message handlers
	
	static LRESULT OnCreate(MESSAGEPARAM_DEFS);
	static LRESULT OnPaint(MESSAGEPARAM_DEFS);

	static LRESULT OnMouseMove(MESSAGEPARAM_DEFS);
	static LRESULT OnMouseLeave(MESSAGEPARAM_DEFS);

	static LRESULT OnLButtonDown(MESSAGEPARAM_DEFS);
	static LRESULT OnLButtonUp(MESSAGEPARAM_DEFS);

	static LRESULT OnNCHitTest(MESSAGEPARAM_DEFS);
	static LRESULT OnNCLButtonDown(MESSAGEPARAM_DEFS);
	static LRESULT OnNCLButtonUp(MESSAGEPARAM_DEFS);

public:
	// Create a new window object
	static MainWindow* Create(CREATEPARAM_DEFS);

/// MEMBERS ----------------------------------------------
private:
	// Populate a WNDCLASSEX struct with the window class properties
	virtual void DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const;

	// Call the CreateWindowEx function
	virtual HWND MakeWindow(HINSTANCE hInstance, TCHAR* szTitle, LPVOID lpParam) const;

protected:
	// Rout messages to handlers
	virtual LRESULT RoutMessage(MESSAGEPARAM_DEFS) const;

/// CONSTRUCTOR/DESTRUCTOR -------------------------------
protected: MainWindow();
public: virtual ~MainWindow();
};


struct MainWindow::Data {
/// MEMBERS ----------------------------------------------
public:
	// Is the left mouse button currently held down
	BOOL isMouseDown;
	
	// Is the window tracking the mouse
	BOOL isMouseTracking;

	// Is the mouse over the close button
	BOOL overCloseButton;

	// Windoww font objects
	
/// CONSTRUCTOR/DESTRUCTOR -------------------------------
public: Data();
};

struct MainWindow::Layout {
/// MEMBERS ----------------------------------------------
public:
	LONG captionHeight;
	LONG captionButtonWidth;
	LONG lineHeight;
	LONG paddingX;
	LONG paddingY;
	
/// CONSTRUCTOR/DESTRUCTOR -------------------------------
public: Layout();
};

#endif // !_INC_MAINWIN