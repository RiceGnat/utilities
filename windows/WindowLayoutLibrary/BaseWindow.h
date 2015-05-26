#include <memory>

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
#define MESSAGEPARAM_DEFS DEFAULT_MESSAGEPARAM_DEFS
#define MESSAGEPARAMS DEFAULT_MESSAGEPARAMS
#define CREATEPARAM_DEFS_SHORT HINSTANCE hInstance, TCHAR* szTitle
#define CREATEPARAMS_SHORT hInstance, szTitle
#define CREATEPARAM_DEFS CREATEPARAM_DEFS_SHORT, HWND hParent, HMENU hMenu
#define CREATEPARAMS CREATEPARAMS_SHORT, hParent, hMenu

// Property getter macros
#define WINDOW_TITLE GetTitle()
#define WINDOW_LAYOUT GetLayoutManager()
#define WINDOW_ATTRIBUTES GetAttributes()

#include "CreateNewWindowTemplate.h"

namespace wll {
	using std::unique_ptr;

	class BaseWindow {
	/// FRIENDS ----------------------------------------------
		// Give access to creation function template
		friend BaseWindow* CreateNewWindow<BaseWindow>(CREATEPARAM_DEFS, wll::LayoutAttributes& attributes);
		
	/// STATIC MEMBERS ---------------------------------------
	private:
		// Window class name
		static const TCHAR* className;

	protected:
		// Window procedure callback (must be static)
		static LRESULT CALLBACK WindowProc(DEFAULT_MESSAGEPARAM_DEFS);

	public:
		// Create a new window object
		static BaseWindow* Create(CREATEPARAM_DEFS_SHORT, wll::LayoutAttributes& attributes);

		// Create a new window object
		static BaseWindow* Create(CREATEPARAM_DEFS, wll::LayoutAttributes& attributes);
		
	/// MEMBERS ----------------------------------------------
	private:
		// Populate a WNDCLASSEX struct with the window class properties
		virtual void DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const;
		
		// Create the window
		virtual HWND MakeWindow(CREATEPARAM_DEFS);
		
		// Initialize layout manager object
		virtual void InitLayoutManager(LayoutAttributes& attributes);

		// Pre-paint handler
		virtual void PrePaint(HDC hdc);

		// Rout messages to handlers
		virtual LRESULT RoutMessage(MESSAGEPARAM_DEFS);

	protected:
		HWND hWnd;							// Window handle
		TCHAR* title;						// Window title
		unique_ptr<LayoutManager> layout;	// Layout manager pointer
		void* data;							// Data object pointer
		
		// Create the window
		HWND MakeWindow(CREATEPARAM_DEFS, const TCHAR* className, DWORD windowStyles, DWORD windowExStyles);
		
	public:
		// Show window
		BOOL Show(int nCmdShow);

		// Update window
		BOOL Update();

		// Redraw window
		BOOL Redraw();
		
		// Get window title
		const TCHAR* WINDOW_TITLE const;

		// Get layout manager
		LayoutManager& WINDOW_LAYOUT const;

		// Get layout attributes
		LayoutAttributes& WINDOW_ATTRIBUTES const;

		// Get data object as type
		template <typename T>
		T& GetData() const;

		// Set data object
		void SetData(void* pData);
		
	/// CONSTRUCTOR/DESTRUCTOR -------------------------------
	protected: BaseWindow(LayoutAttributes& attributes);
	public: virtual ~BaseWindow();
	};
	
	// Typed data getter function template definition
	template <typename T>
	T& BaseWindow::GetData() const {
		// NOTE: fix this eventually
		return *(reinterpret_cast<T*>(data));
	}
}

#endif // !_INC_WINDOWBASE