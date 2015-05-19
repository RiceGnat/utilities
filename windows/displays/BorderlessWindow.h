#include "BaseWindow.h"

#ifndef _INC_BLWINDOW
#define _INC_BLWINDOW

struct BorderlessAttributes;

class BorderlessWindow : public wll::BaseWindow {
/// FRIENDS ----------------------------------------------
	// Give access to creation function template
	friend BorderlessWindow* wll::CreateNewWindow<BorderlessWindow>(CREATEPARAM_DEFS, wll::LayoutAttributes& attributes);
		
/// INTERNAL CLASSES -------------------------------------
protected:

		
/// STATIC MEMBERS ---------------------------------------
private:
	// Window class name
	static const TCHAR* className;
		
	// Hide base creation function
	BaseWindow::Create;

public:
	// Create a new window object
	static BorderlessWindow* Create(CREATEPARAM_DEFS_SHORT, BorderlessAttributes& attributes);

	// Create a new window object
	static BorderlessWindow* Create(CREATEPARAM_DEFS, BorderlessAttributes& attributes);

/// MEMBERS ----------------------------------------------
private:
	// Populate a WNDCLASSEX struct with the window class properties
	virtual void DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const;

	// Make the window
	virtual HWND MakeWindow(CREATEPARAM_DEFS);

	// Initialize data object
	//virtual void InitData();

	// Pre-paint handler
	//virtual void PrePaint();

	// Rout messages to handlers
	virtual LRESULT RoutMessage(MESSAGEPARAM_DEFS);

	// Message handlers
	
	//LRESULT OnCreate(MESSAGEPARAM_DEFS);
	//LRESULT OnPaint(MESSAGEPARAM_DEFS);

	//LRESULT OnMouseMove(MESSAGEPARAM_DEFS);
	//LRESULT OnMouseLeave(MESSAGEPARAM_DEFS);

	//LRESULT OnLButtonDown(MESSAGEPARAM_DEFS);
	//LRESULT OnLButtonUp(MESSAGEPARAM_DEFS);

	LRESULT OnNCHitTest(MESSAGEPARAM_DEFS);
	LRESULT OnNCLButtonDown(MESSAGEPARAM_DEFS);
	LRESULT OnNCLButtonUp(MESSAGEPARAM_DEFS);

	// Hide base attributes function
	BaseWindow::GetAttributes;

public:
	BorderlessAttributes& GetAttributes() const;
		
/// CONSTRUCTOR/DESTRUCTOR -------------------------------
protected: BorderlessWindow(wll::LayoutAttributes& attributes);
public: virtual ~BorderlessWindow();
};

struct BorderlessAttributes : public wll::LayoutAttributes {
	int captionHeight;
	int captionButtonWidth;

	bool isMouseDown;
	bool isMouseTracking;
};

#endif // !_INC_BLWINDOW