
namespace wll {
	// Window creation function template
	template <typename T>
	T* CreateNewWindow(CREATEPARAM_DEFS) {
		T* pWindow = new T(attributes);

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
		if (!pWindow->MakeWindow(CREATEPARAMS))
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