#include <vector>

#include <windows.h>
#include <tchar.h>

#include "WindowElements.h"
#include "LayoutManager.h"

#ifndef _INC_APPWIN
#define _INC_APPWIN

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "Win32 Application"
#endif //!APPLICATION_NAME

// Function parameter macros
#define DEFAULT_MESSAGEPARAM_DEFS HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
#define DEFAULT_MESSAGEPARAMS hWnd, uMsg, wParam, lParam
#define MESSAGEPARAM_DEFS DEFAULT_MESSAGEPARAM_DEFS, AppWindow* pWindow
#define MESSAGEPARAMS DEFAULT_MESSAGEPARAMS, pWindow
#define CREATEPARAM_DEFS HINSTANCE hInstance, TCHAR* szTitle
#define CREATEPARAMS hInstance, szTitle

namespace appwin {
	template <typename T> T* CreateAppWindow(CREATEPARAM_DEFS);

	class AppWindow {
	/// FRIENDS ----------------------------------------------
		// Give access to creation function template
		friend AppWindow* CreateAppWindow<AppWindow>(CREATEPARAM_DEFS);
		
	/// STATIC MEMBERS ---------------------------------------
	private:
		// Window class name
		static const TCHAR* className;
		
	protected:
		// Window procedure callback (must be static)
		static LRESULT CALLBACK WindowProc(DEFAULT_MESSAGEPARAM_DEFS);

	public:
		// Create a new window object
		static AppWindow* Create(CREATEPARAM_DEFS);

	/// MEMBERS ----------------------------------------------
	private:
		// Populate a WNDCLASSEX struct with the window class properties
		virtual void DefineWindowClass(HINSTANCE hInstance, WNDCLASSEX &wcex) const;

		// Make the window
		virtual HWND MakeWindow(HINSTANCE hInstance, TCHAR* szTitle, LPVOID lpParam) const;

	protected:
		// Window handle
		HWND hWnd;

		// Window title
		TCHAR* title;

		// Window data object
		void* data;

		// Layout manager
		LayoutManager* layout;

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

		// Add a window element
		void AddWindowElement(WindowElement* element);

		// Draw all window elements
		virtual void DrawWindowElements() const;

		// Get data object as type
		template <typename T>
		T& GetData() const;
		
	/// CONSTRUCTOR/DESTRUCTOR -------------------------------
	protected: AppWindow();
	public: virtual ~AppWindow();
	};

	// Typed data getter function template definition
	template <typename T>
	T& AppWindow::GetData() const {
		return *(reinterpret_cast<T*>(data));
	}

	// Window creation function template
	template <typename T>
	T* CreateAppWindow(CREATEPARAM_DEFS) {
		T* pWindow = new T;

		pWindow->title = szTitle;

		WNDCLASSEX wcex;

		// Check if window class is already defined
		if (!GetClassInfoEx(hInstance, T::className, &wcex)) {
			pWindow->DefineWindowClass(hInstance, wcex);

			// Register window class
			if (!RegisterClassEx(&wcex))
			{
				MessageBox(NULL,
					_T("Call to RegisterClassEx failed!"),
					_T(APPLICATION_NAME),
					NULL);

				// Delete object and return NULL to indicate failure
				delete pWindow;
				return NULL;
			}
		}

		// Create window (window handle and AppWindow object are linked during the window's creation message)
		if (!pWindow->MakeWindow(hInstance, szTitle, pWindow))
		{
			MessageBox(NULL,
				_T("Call to CreateWindow failed!"),
				_T(APPLICATION_NAME),
				NULL);

			// Delete object and return NULL to indicate failure
			delete pWindow;
			return NULL;
		}

		// Return pointer to new object
		return pWindow;
	}
}

#endif // !_INC_APPWIN