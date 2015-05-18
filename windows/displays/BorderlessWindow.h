#include "BaseWindow.h"
#include "BorderlessAttributes.h"

#ifndef _INC_BLWINDOW
#define _INC_BLWINDOW

class BorderlessWindow : public wll::BaseWindow {
/// FRIENDS ----------------------------------------------
	// Give access to creation function template
	friend BorderlessWindow* wll::CreateNewWindow<BorderlessWindow>(CREATEPARAM_DEFS);
		
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
	static BorderlessWindow* Create(CREATEPARAM_DEFS_SHORT);

	// Create a new window object
	static BorderlessWindow* Create(CREATEPARAM_DEFS);

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
	//virtual LRESULT RoutMessage(MESSAGEPARAM_DEFS) const;
	// Message handlers
	
	//LRESULT OnCreate(MESSAGEPARAM_DEFS);
	//LRESULT OnPaint(MESSAGEPARAM_DEFS);

	//LRESULT OnMouseMove(MESSAGEPARAM_DEFS);
	//LRESULT OnMouseLeave(MESSAGEPARAM_DEFS);

	//LRESULT OnLButtonDown(MESSAGEPARAM_DEFS);
	//LRESULT OnLButtonUp(MESSAGEPARAM_DEFS);

	LRESULT OnNCHitTest(MESSAGEPARAM_DEFS);
	//LRESULT OnNCLButtonDown(MESSAGEPARAM_DEFS);
	//LRESULT OnNCLButtonUp(MESSAGEPARAM_DEFS);

public:
	BorderlessAttributes& GetAttributes() const;
		
/// CONSTRUCTOR/DESTRUCTOR -------------------------------
protected: BorderlessWindow(BorderlessAttributes& attributes);
public: virtual ~BorderlessWindow();
};

#endif // !_INC_BLWINDOW