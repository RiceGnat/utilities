// Typed data getter function template definition
template <typename T>
T& BaseWindow::GetData() const {
	return *(reinterpret_cast<T*>(data));
}

// Window creation function template
template <typename T>
T* CreateNewWindow(CREATEPARAM_DEFS) {
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