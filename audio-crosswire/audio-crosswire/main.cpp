#include "common.h"

//int do_everything(int argc, LPCWSTR argv[]);
int start_threads(IMMDevice* pOutputDevice, const ChannelSources& sourceMap, const SourceTypes& sourceTypes);

int _cdecl wmain(int argc, LPCWSTR argv[]) {
    // Accepts result message from function calls
	HRESULT hr = S_OK;

	// Initializes the COM library on the current thread
    hr = CoInitialize(NULL);
	EXIT_ON_ERROR(hr);
    CoUninitializeOnExit cuoe;

	// Get a reference to an IMMDeviceEnumerator interface
	IMMDeviceEnumerator* pEnumerator;
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit roeEnumerator(pEnumerator);

	// Get output device
	IMMDevice* pMMDevice;
	hr = pEnumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eConsole, &pMMDevice);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit roeMMDevice(pMMDevice);

	// Get capture device
	IMMDevice* pCaptureDevice;
	hr = pEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eConsole, &pCaptureDevice);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit roeCaptureDevice(pCaptureDevice);


	// Map sources to output channels
	ChannelSources sourceMap;
	//sourceMap.m_pChannel0 = pCaptureDevice;
	//sourceMap.m_pChannel2 = pMMDevice;
	sourceMap.m_pChannel4 = pMMDevice;
	sourceMap.m_pChannel6 = pCaptureDevice;

	// Identify source types (default is capture)
	SourceTypes sourceTypes;
	//sourceTypes.m_channel0 = EDataFlow::eCapture;
	//sourceTypes.m_channel2 = EDataFlow::eRender;
	sourceTypes.m_channel4 = EDataFlow::eRender;
	sourceTypes.m_channel6 = EDataFlow::eCapture;

	// Get device properties
	IPropertyStore* propStore;
	hr = pMMDevice->OpenPropertyStore(STGM_READ, &propStore);
	EXIT_ON_ERROR(hr);

	// Get device name
	PROPVARIANT value;
	PropVariantInit(&value);
	hr = propStore->GetValue(PKEY_Device_FriendlyName, &value);
	EXIT_ON_ERROR(hr);

	// Show some information
	LOG(L"Output device is %s", value.pwszVal);
	for (int i = 0; i < MAX_SOURCES; i++) {
		wprintf(L"Channel %d: ", i * 2);

		if (((IMMDevice**)&sourceMap)[i] == NULL) {
			LOG(L"No source mapped");
			continue;
		}

		hr = ((IMMDevice**)&sourceMap)[i]->OpenPropertyStore(STGM_READ, &propStore);
		EXIT_ON_ERROR(hr);

		PropVariantInit(&value);
		hr = propStore->GetValue(PKEY_Device_FriendlyName, &value);
		EXIT_ON_ERROR(hr);

		LOG("%s", value.pwszVal);
	}

	return start_threads(pMMDevice, sourceMap, sourceTypes);

    //return do_everything(argc, argv);
}

int start_threads(IMMDevice* pOutputDevice, const ChannelSources& sourceMap, const SourceTypes& sourceTypes) {
	// Set up arrays
	HANDLE hStartedEvents[MAX_SOURCES];
	HANDLE hThreads[MAX_SOURCES];
	HANDLE hDummyEvents[MAX_SOURCES];
	HANDLE waitArray[MAX_SOURCES * 2];
	DeviceCaptureThreadFunctionArguments threadArgs[MAX_SOURCES];

	CloseHandlesOnExit closeStartedEvent(hStartedEvents, MAX_SOURCES);
	CloseHandlesOnExit closeDummyEvents(hDummyEvents, MAX_SOURCES);

	UINT32 nAvailableChannels;

	// Get output device info
	{
		HRESULT hr;
		
		// Activate an audio client for the output device
		IAudioClient* pOutputClient;
		hr = pOutputDevice->Activate(
			__uuidof(IAudioClient),
			CLSCTX_ALL, NULL,
			(void**)&pOutputClient
			);
		EXIT_ON_ERROR(hr);
		ReleaseOnExit releaseOutputClient(pOutputClient);

		// Get the output device's format
		WAVEFORMATEXTENSIBLE* pwfOut;
		hr = pOutputClient->GetMixFormat((WAVEFORMATEX**)&pwfOut);
		EXIT_ON_ERROR(hr);
		CoTaskMemFreeOnExit freeOutputMixFormat(pwfOut);
		
		nAvailableChannels = pwfOut->Format.nChannels;
	}
	// Get ID of output device
	LPWSTR pwszOutputId;
	pOutputDevice->GetId(&pwszOutputId);

	// Create "stop capture" event for all threads (manual reset mode)
	HANDLE hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	CloseHandleOnExit closeStopEvent(hStopEvent);

	// Create events for each possible source device
	for (UINT32 i = 0; i < MAX_SOURCES; i++) {
		// Create "capture started" event
		HANDLE hStartedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		hStartedEvents[i] = hStartedEvent;

		// Create dummy event (manual reset)
		HANDLE hDummyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		hDummyEvents[i] = hDummyEvent;

		// Get source device
		IMMDevice* pSourceDevice = ((IMMDevice**)&sourceMap)[i];

		bool startThread = true;

		if (pSourceDevice == NULL) {
			startThread = false;
		}
		else {
			// Get ID of source device
			LPWSTR pwszSourceId;
			pSourceDevice->GetId(&pwszSourceId);
			if (lstrcmpW(pwszOutputId, pwszSourceId) == 0 && i == 0) {
				// This is a feedback loop from channel 0 of output going to channel 0
				// Channel 0 of output going to channel 2+ is okay
				LOG(L"Feedback loop detected to channel 0, skipping this thread");
				startThread = false;
			}
		}

		// Start a capture thread for each available channel pair
		if (i * 2 < nAvailableChannels && startThread) {
			// Set arguments for each device capture thread
			threadArgs[i].hr = E_UNEXPECTED; // thread will overwrite this
			threadArgs[i].pOutputDevice = pOutputDevice;
			threadArgs[i].pSourceDevice = pSourceDevice;
			threadArgs[i].eSourceType = ((EDataFlow*)&sourceTypes)[i];
			threadArgs[i].nOutputChannel = i * 2;
			threadArgs[i].hStartedEvent = hStartedEvent;
			threadArgs[i].hStopEvent = hStopEvent;

			HANDLE hThread = CreateThread(
				NULL, 0,
				DeviceCaptureThreadFunction, &(threadArgs[i]),
				0, NULL
				);

			hThreads[i] = hThread;
			waitArray[i + MAX_SOURCES] = hThreads[i];
		}
		else {
			hThreads[i] = NULL;
			waitArray[i + MAX_SOURCES] = hDummyEvent;
		}

		waitArray[i] = hStartedEvents[i];
	}
	
	// Wait for any capture thread to start or for any thread to exit early
	DWORD dwWaitResult;
	dwWaitResult = WaitForMultipleObjects(
		ARRAYSIZE(waitArray), waitArray,
		FALSE, INFINITE
		);

	// If a thread ended
	if (dwWaitResult >= WAIT_OBJECT_0 + MAX_SOURCES &&
		dwWaitResult < WAIT_OBJECT_0 + MAX_SOURCES * 2) {
		int threadIndex = dwWaitResult - WAIT_OBJECT_0 - MAX_SOURCES;
		ERR(L"Thread for channel %d aborted before starting device capture : 0x%08x",
			threadIndex * 2, threadArgs[threadIndex].hr);
		return -__LINE__;
	}

	// Something else (besides a start event) happened
	if (dwWaitResult >= WAIT_OBJECT_0 + MAX_SOURCES * 2) {
		ERR(L"Unexpected WaitForMultipleObjects return value %u", dwWaitResult);
		return -__LINE__;
	}

	// At this point at least one of the capture threads is running
	// Wait for the user to press the exit key or for capture to error out
	{
		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

		if (INVALID_HANDLE_VALUE == hStdIn) {
			ERR(L"GetStdHandle returned INVALID_HANDLE_VALUE: last error is %u", GetLastError());
			return -__LINE__;
		}

		HANDLE waitHandles[MAX_SOURCES + 1];
		for (UINT32 i = 0; i < MAX_SOURCES; i++) {
			if (hThreads[i] == NULL) waitHandles[i] = hDummyEvents[i];
			else waitHandles[i] = hThreads[i];
		}
		waitHandles[MAX_SOURCES] = hStdIn;

		WaitForMultipleObjectOnExit waitForThreads(waitHandles, MAX_SOURCES);
		SetEventsOnExit setDummyEvent(hDummyEvents, MAX_SOURCES);
		SetEventOnExit setStopEvent(hStopEvent);

		LOG(L"%s", L"Press Enter to quit...");
		bool bKeepWaiting = true;
		while (bKeepWaiting) {

			dwWaitResult = WaitForMultipleObjects(MAX_SOURCES + 1, waitHandles, FALSE, INFINITE);

			if (dwWaitResult == WAIT_OBJECT_0 + MAX_SOURCES) { // hStdIn
				INPUT_RECORD rInput[128];
				DWORD nEvents;
				// Get the input and see if any of them were the Enter key-up event
				if (!ReadConsoleInput(hStdIn, rInput, ARRAYSIZE(rInput), &nEvents)) {
					ERR(L"ReadConsoleInput failed: last error is %u", GetLastError());
					bKeepWaiting = false;
				}
				else {
					for (DWORD i = 0; i < nEvents; i++) {
						if (
							KEY_EVENT == rInput[i].EventType &&
							VK_RETURN == rInput[i].Event.KeyEvent.wVirtualKeyCode &&
							!rInput[i].Event.KeyEvent.bKeyDown
							) {
							LOG(L"%s", L"Stopping capture...");
							bKeepWaiting = false;
							break;
						}
					}
					// If none of them were Enter key-up events, continue waiting
				}
			}
			else if (dwWaitResult >= WAIT_OBJECT_0 &&
				dwWaitResult < WAIT_OBJECT_0 + MAX_SOURCES) { // A thread crashed
				ERR(L"Thread for channel %d crashed: 0x%08x",
					(dwWaitResult - WAIT_OBJECT_0) * 2, threadArgs[dwWaitResult - WAIT_OBJECT_0].hr);
				bKeepWaiting = false;
			}
			else { // Something else
				ERR(L"WaitForMultipleObjects returned unexpected value 0x%08x", dwWaitResult);
				bKeepWaiting = false;
			} // switch
		} // while
	} // naked scope

#ifdef _DEBUG
	system("pause");
#endif

	return 0;
}