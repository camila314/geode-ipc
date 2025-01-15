#include <geode-ipc.h>

#if __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>

static CFMessagePortRef port = NULL;

char* geode_ipc_send_timeout(char const* text, double timeout) {
    if (port == NULL || !CFMessagePortIsValid(port))
        port = CFMessagePortCreateRemote(NULL, CFStringCreateWithCString(NULL, "GeodeIPCPipe", kCFStringEncodingUTF8));
    if (port == NULL || !CFMessagePortIsValid(port))
        return NULL;

    CFDataRef data = CFDataCreate(NULL, (uint8_t*)text, strlen(text));
    CFDataRef ret = NULL;
    CFMessagePortSendRequest(
        port,
        0,
        (CFDataRef)data,
        1, // sendTimeout
        timeout, // receiveTimeout
        kCFRunLoopDefaultMode,
        &ret
    );
    
    if (ret == NULL) {
        return NULL;
    }

    char* response = malloc(CFDataGetLength(ret));
    CFDataGetBytes(ret, CFRangeMake(0, CFDataGetLength(ret)), (UInt8*)response);
    CFRelease(ret);

    return response;
}
#else
#include <windows.h>
#include <stdio.h>

char* geode_ipc_send_timeout(char const* text, double timeout) {
    // create pipe
    HANDLE namedPipe = CreateFile(TEXT("\\\\.\\pipe\\GeodeIPCPipe"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (namedPipe == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    char* output = NULL;

    if (!WriteFile(namedPipe, text, strlen(text) + 1, NULL, NULL)) {
        goto leave;
    }

    OVERLAPPED overlapped = {0};
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (overlapped.hEvent == NULL) {
        goto leave;
    }

    DWORD chunkSize = 512;
    DWORD totalSize = 0;
    DWORD outSize = chunkSize;

    while(outSize >= chunkSize) {
        output = realloc(output, totalSize + outSize + 1);

        if (!ReadFile(namedPipe, output + totalSize, chunkSize, &outSize, &overlapped) && GetLastError() == ERROR_IO_PENDING) {
            if (WaitForSingleObject(overlapped.hEvent, timeout * 1000) == WAIT_TIMEOUT) {
                CancelIo(namedPipe);
                CloseHandle(overlapped.hEvent);
                free(output);
                output = NULL;
                goto leave;
            }

            GetOverlappedResult(namedPipe, &overlapped, &outSize, FALSE);
        }

        totalSize += outSize;
    }

    output[totalSize] = '\0';

leave:
    CloseHandle(namedPipe);
    return output;
}
#endif

char* geode_ipc_send(char const* text) {
    return geode_ipc_send_timeout(text, 1);
}
