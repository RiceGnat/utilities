struct DeviceCaptureThreadFunctionArguments {
	IMMDevice* pOutputDevice;
	IMMDevice* pSourceDevice;
	EDataFlow eSourceType;
	BYTE nOutputChannel;
	HANDLE hStartedEvent;
	HANDLE hStopEvent;
	HRESULT hr;
};

DWORD WINAPI DeviceCaptureThreadFunction(LPVOID pContext);
