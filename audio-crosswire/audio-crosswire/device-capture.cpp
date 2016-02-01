#include "common.h"

HRESULT DeviceCapture(
	IMMDevice* pOutputDevice,
	IMMDevice* pSourceDevice,
	EDataFlow eSourceType,
	BYTE nOutputChannel,
	HANDLE hStartedEvent,
	HANDLE hStopEvent
	);

DWORD WINAPI DeviceCaptureThreadFunction(LPVOID pContext) {
	DeviceCaptureThreadFunctionArguments *pArgs =
		(DeviceCaptureThreadFunctionArguments*)pContext;

	// Initializes the COM library on the current thread
	pArgs->hr = CoInitialize(NULL);
	EXIT_ON_ERROR(pArgs->hr);
	CoUninitializeOnExit cuoe;

	pArgs->hr = DeviceCapture(
		pArgs->pOutputDevice,
		pArgs->pSourceDevice,
		pArgs->eSourceType,
		pArgs->nOutputChannel,
		pArgs->hStartedEvent,
		pArgs->hStopEvent
		);

	return 0;
}

HRESULT DeviceCapture(
	IMMDevice* pOutputDevice,
	IMMDevice* pSourceDevice,
	EDataFlow eSourceType,
	BYTE nOutputChannel,
	HANDLE hStartedEvent,
	HANDLE hStopEvent
	) {
	HRESULT hr;

	// Set up output audio device
	// Activate an audio client for the output device
	IAudioClient* pOutputClient;
	hr = pOutputDevice->Activate(
		__uuidof(IAudioClient),
		CLSCTX_ALL, NULL,
		(void**)&pOutputClient
		);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit releaseOutputClient(pOutputClient);

	// Get the output device's periodicity
	REFERENCE_TIME hnsOutputDevicePeriod;
	hr = pOutputClient->GetDevicePeriod(&hnsOutputDevicePeriod, NULL);
	EXIT_ON_ERROR(hr);

	// Get the output device's format
	WAVEFORMATEXTENSIBLE* pwfOut;
	hr = pOutputClient->GetMixFormat((WAVEFORMATEX**)&pwfOut);
	EXIT_ON_ERROR(hr);
	CoTaskMemFreeOnExit freeOutputMixFormat(pwfOut);

	// Initialize audio client
	hr = pOutputClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		0, 0, (WAVEFORMATEX*)pwfOut, 0
		);
	EXIT_ON_ERROR(hr);

	// Get a render service from the output client
	IAudioRenderClient *pAudioRenderClient;
	hr = pOutputClient->GetService(
		__uuidof(IAudioRenderClient),
		(void**)&pAudioRenderClient
		);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit releaseAudioRenderClient(pAudioRenderClient);

	// Set up source device
	// Activate an audio client for the source device
	IAudioClient *pSourceClient;
	hr = pSourceDevice->Activate(
		__uuidof(IAudioClient),
		CLSCTX_ALL, NULL,
		(void**)&pSourceClient
		);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit releaseSourceClient(pSourceClient);

	// Get the source device's periodicity
	REFERENCE_TIME hnsSourceDevicePeriod;
	hr = pSourceClient->GetDevicePeriod(&hnsSourceDevicePeriod, NULL);
	EXIT_ON_ERROR(hr);

	// Get the source device's format
	WAVEFORMATEX* pwfSource;
	hr = pSourceClient->GetMixFormat(&pwfSource);
	EXIT_ON_ERROR(hr);
	CoTaskMemFreeOnExit freeSourceMixFormat(pwfSource);

	// Set stream flags
	// If the source is another output, set capture to loopback mode
	DWORD streamFlags = 0;
	if (eSourceType == EDataFlow::eRender)
		streamFlags = AUDCLNT_STREAMFLAGS_LOOPBACK;

	// Initialize audio client
	// AUDCLNT_STREAMFLAGS_LOOPBACK and AUDCLNT_STREAMFLAGS_EVENTCALLBACK
	// cannot be used together, so a loop with a timed wait will be used
	hr = pSourceClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		streamFlags,
		0, 0, pwfSource, 0
		);
	EXIT_ON_ERROR(hr);

	// Get a capture service from the output client
	IAudioCaptureClient *pSourceCaptureClient;
	hr = pSourceClient->GetService(
		__uuidof(IAudioCaptureClient),
		(void**)&pSourceCaptureClient
		);
	EXIT_ON_ERROR(hr);
	ReleaseOnExit releaseAudioCaptureClient(pSourceCaptureClient);

	// Start the source audio client
	hr = pSourceClient->Start();
	AudioClientStopOnExit stopSourceClient(pSourceClient);
	EXIT_ON_ERROR(hr);

	// Start the output audio client
	hr = pOutputClient->Start();
	AudioClientStopOnExit stopOutputClient(pOutputClient);
	EXIT_ON_ERROR(hr);

	// Register thread with MMCSS
	DWORD nTaskIndex = 0;
	HANDLE hTask = AvSetMmThreadCharacteristics(L"Audio", &nTaskIndex);
	if (NULL == hTask) {
		DWORD dwErr = GetLastError();
		return HRESULT_FROM_WIN32(dwErr);
	}
	AvRevertMmThreadCharacteristicsOnExit unregisterMmcss(hTask);

	// Create a periodic waitable timer
	HANDLE hWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);
	CloseHandleOnExit closeWakeUp(hWakeUp);

	// Set the waitable timer
	LARGE_INTEGER liFirstFire;
	liFirstFire.QuadPart = -hnsOutputDevicePeriod / 2; // negative means relative time
	LONG lTimeBetweenFires = (LONG)hnsOutputDevicePeriod / 2 / (10 * 1000); // convert to milliseconds
	BOOL bOK = SetWaitableTimer(
		hWakeUp,
		&liFirstFire,
		lTimeBetweenFires,
		NULL, NULL, FALSE
		);
	if (!bOK) {
		DWORD dwErr = GetLastError();
		return HRESULT_FROM_WIN32(dwErr);
	}
	CancelWaitableTimerOnExit cancelWakeUp(hWakeUp);

	// Set up loop variables
	HANDLE waitArray[2] = { hStopEvent, hWakeUp };
	DWORD dwWaitResult;

	bool bDone = false;
	bool bFirstPacket = true;

	UINT32 nBlockSize = pwfSource->nBlockAlign;
	UINT32 nSampleSize = nBlockSize / pwfSource->nChannels;
	UINT32 nOutputBlockSize = pwfOut->Format.nBlockAlign;

	// Start capture loop
	LOG(L"Starting capture for channel %d...", nOutputChannel);
	SetEvent(hStartedEvent);
	for (UINT32 nPasses = 0; !bDone; nPasses++) {
		// Drain data while it is available
		UINT32 nNextPacketSize;
		for (
			hr = pSourceCaptureClient->GetNextPacketSize(&nNextPacketSize);
			SUCCEEDED(hr) && nNextPacketSize > 0;
			hr = pSourceCaptureClient->GetNextPacketSize(&nNextPacketSize)
			) {

			BYTE *pData;
			UINT32 nNumFramesToRead;
			DWORD dwFlags;

			// Get data from buffer
			hr = pSourceCaptureClient->GetBuffer(
				&pData,
				&nNumFramesToRead,
				&dwFlags,
				NULL,
				NULL
				);
			EXIT_ON_ERROR(hr);

			// Ignore AUDCLNT_BUFFERFLAGS_SILENT flag
			dwFlags &= 5;

			if (bFirstPacket && AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY == dwFlags) {
				LOG(L"%s", L"Ignoring discontinuity reported on first packet");
			}
			else if (0 != dwFlags) {
				LOG(L"IAudioCaptureClient::GetBuffer set flags to 0x%08x", dwFlags);
				return E_UNEXPECTED;
			}

			if (0 == nNumFramesToRead) {
				ERR(L"IAudioCaptureClient::GetBuffer said to read 0 frames");
				return E_UNEXPECTED;
			}
			 
			LONG lBytesRead = nNumFramesToRead * nBlockSize;

			// Get output buffer
			BYTE *pWriteAddress;
			hr = pAudioRenderClient->GetBuffer(nNumFramesToRead, &pWriteAddress);
			EXIT_ON_ERROR(hr);

			// Copy data into output buffer
			for (
				LONG offset = 0, writeOffset = 0;
				offset < lBytesRead;
				offset += nBlockSize, writeOffset += nOutputBlockSize) {
				memcpy(pWriteAddress + writeOffset + nSampleSize * nOutputChannel, pData + offset, nSampleSize * 2);
			}

			// Release buffers
			hr = pSourceCaptureClient->ReleaseBuffer(nNumFramesToRead);
			EXIT_ON_ERROR(hr);

			hr = pAudioRenderClient->ReleaseBuffer(nNumFramesToRead, 0);
			EXIT_ON_ERROR(hr);

			bFirstPacket = false;
		}

		EXIT_ON_ERROR(hr);

		dwWaitResult = WaitForMultipleObjects(
			ARRAYSIZE(waitArray), waitArray,
			FALSE, INFINITE
			);

		if (WAIT_OBJECT_0 == dwWaitResult) {
			LOG(L"Received stop event");
			bDone = true;
			continue; // exits loop
		}

		if (WAIT_OBJECT_0 + 1 != dwWaitResult) {
			ERR(L"Unexpected WaitForMultipleObjects return value %u", dwWaitResult);
			return E_UNEXPECTED;
		}
	} // capture loop

	return hr;
}
