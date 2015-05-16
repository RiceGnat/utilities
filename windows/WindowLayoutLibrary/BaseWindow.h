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
#define MESSAGEPARAM_DEFS DEFAULT_MESSAGEPARAM_DEFS, BaseWindow* pWindow
#define MESSAGEPARAMS DEFAULT_MESSAGEPARAMS, pWindow
#define CREATEPARAM_DEFS_SHORT HINSTANCE hInstance, TCHAR* szTitle, LayoutAttributes& attributes
#define CREATEPARAMS_SHORT hInstance, szTitle, attributes
#define CREATEPARAM_DEFS CREATEPARAM_DEFS_SHORT, HWND hParent, HMENU hMenu
#define CREATEPARAMS CREATEPARAMS_SHORT, hParent, hMenu

#include "CreateNewWindowTemplate.h"

namespace wll {
	using std::unique_ptr;

	class BaseWindow {
	/// FRIENDS ----------------------------------------------
		// Give access to creation function template
		friend BaseWindow* CreateNewWindow<BaseWindow>(CREATEPARAM_DEFS);
		
	/// INTERNAL CLASSES -------------------------------------
	protected:
		// Abstract base data class
		struct Data {
		public: Data();
		public: virtual ~Data() = 0;
		};
		
	/// STATIC MEMBERS ---------------------------------------
	private:
		// Window class name
		static const TCHAR* className;

	protected:
		// Window procedure callback (must be static)
		static LRESULT CALLBACK WindowProc(DEFAULT_MESSAGEPARAM_DEFS);

	public:
		// Create a new window object
		static BaseWindow* Create(CREATEPARAM_DEFS_SHORT);

		// Create a new window object
		static BaseWindow* Create(CREATEPARAM_DEFS);
		
	/// MEMBERS ----------------------------------------------
	private:
		// Populate a WNDCLASSEX struct with the window class properties
		virtual void DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const;

		// Make the window
		virtual HWND MakeWindow(CREATEPARAM_DEFS);
		
		// Initialize layout manager object
		virtual void InitLayoutManager(LayoutAttributes& attributes);

		// Initialize data object
		virtual void InitData();

		// Rout messages to handlers
		virtual LRESULT RoutMessage(MESSAGEPARAM_DEFS) const;

	protected:
		HWND hWnd;							// Window handle
		TCHAR* title;						// Window title
		unique_ptr<LayoutManager> layout;	// Layout manager pointer
		unique_ptr<Data> data;				// Data object pointer
		
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
	protected: BaseWindow(LayoutAttributes& attributes);
	public: virtual ~BaseWindow();
	};
	
	// Typed data getter function template definition
	template <typename T>
	T& BaseWindow::GetData() const {
		return *(reinterpret_cast<T*>(data));
	}
}

#endif // !_INC_WINDOWBASE