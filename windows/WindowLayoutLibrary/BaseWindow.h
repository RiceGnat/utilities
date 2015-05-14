#include <windows.h>
#include <tchar.h>

#include "WindowElements.h"
#include "LayoutManager.h"

#ifndef _INC_WINDOWBASE
#define _INC_WINDOWBASE

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "Win32 Application"
#endif //!APPLICATION_NAME

// Function parameter macros
#define DEFAULT_MESSAGEPARAM_DEFS HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
#define DEFAULT_MESSAGEPARAMS hWnd, uMsg, wParam, lParam
#define MESSAGEPARAM_DEFS DEFAULT_MESSAGEPARAM_DEFS, BaseWindow* pWindow
#define MESSAGEPARAMS DEFAULT_MESSAGEPARAMS, pWindow
#define CREATEPARAM_DEFS HINSTANCE hInstance, TCHAR* szTitle
#define CREATEPARAMS hInstance, szTitle

namespace wll {
	template <typename T> T* CreateNewWindow(CREATEPARAM_DEFS);

	class BaseWindow {
	/// FRIENDS ----------------------------------------------
		// Give access to creation function template
		friend BaseWindow* CreateNewWindow<BaseWindow>(CREATEPARAM_DEFS);
		
	/// STATIC MEMBERS ---------------------------------------
	private:
		// Window class name
		static const TCHAR* className;

	protected:
		// Window procedure callback (must be static)
		static LRESULT CALLBACK WindowProc(DEFAULT_MESSAGEPARAM_DEFS);

	public:
		// Create a new window object
		static BaseWindow* Create(CREATEPARAM_DEFS);
		
	/// MEMBERS ----------------------------------------------
	private:
		// Populate a WNDCLASSEX struct with the window class properties
		virtual void DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const;

		// Make the window
		virtual HWND MakeWindow(CREATEPARAM_DEFS, LPVOID lpParam);

	protected:
		HWND hWnd;					// Window handle
		TCHAR* title;				// Window title
		void* data;					// Data object
		LayoutManager* layout;		// Layout manager

		// Rout messages to handlers
		virtual LRESULT RoutMessage(MESSAGEPARAM_DEFS) const;

	public:
		// Show window
		BOOL Show(int nCmdShow);

		// Update window
		BOOL Update();

		// Redraw window
		BOOL Redraw();
		
		// Get window title
		const TCHAR* GetTitle() const;

		// Get layout manager
		LayoutManager& GetLayoutManager() const;

		// Get data object as type
		template <typename T>
		T& GetData() const;
		
	/// CONSTRUCTOR/DESTRUCTOR -------------------------------
	protected: BaseWindow();
	public: virtual ~BaseWindow();
	};

#include "BaseWindowTemplateDefinitions.h"

}

#endif // !_INC_WINDOWBASE