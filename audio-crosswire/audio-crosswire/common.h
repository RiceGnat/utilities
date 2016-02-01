#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>

#include "log.h"
#include "cleanup.h"
#include "sourcemap.h"
#include "device-capture.h"

#define EXIT_ON_ERROR(HR) if (FAILED(HR)) return HR;
#define MAX_SOURCES 4