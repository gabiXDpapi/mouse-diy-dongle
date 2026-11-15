/* Read-only, mouse-only host arrival capture; normal pointer input stays enabled. */
#define _WIN32_WINNT 0x0601
#define __USE_MINGW_ANSI_STDIO 1
#include <windows.h>
#include <shellapi.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
/* Legacy MinGW headers omit these documented Vista+ notification constants. */
#ifndef WM_INPUT_DEVICE_CHANGE
#define WM_INPUT_DEVICE_CHANGE 0x00FE
#endif
#ifndef RIDEV_DEVNOTIFY
#define RIDEV_DEVNOTIFY 0x00002000
#endif
#ifndef GET_RAWINPUT_CODE_WPARAM
#define GET_RAWINPUT_CODE_WPARAM(w) ((w) & 0xff)
#endif

static FILE *output;
static LARGE_INTEGER frequency, start;
static unsigned long long records;
static unsigned int read_errors;
static int failed;
static struct { HANDLE handle; const char *label; } cache[32];
static unsigned int cache_count;

static int has_id(const char *name, const char *id)
{
    size_t length = strlen(id);
    for (; *name; name++) {
        if (_strnicmp(name, id, length) == 0 && !isxdigit((unsigned char)name[length])) return 1;
    }
    return 0;
}

static const char *classify_name(const char *name)
{
    if (has_id(name, "VID_258A&PID_2018")) return "wired";
    if (has_id(name, "VID_2FE3&PID_0008")) return "receiver";
    return NULL;
}

static const char *device_label(HANDLE device)
{
    if (!device) return NULL;
    for (unsigned int i = 0; i < cache_count; i++) {
        if (cache[i].handle == device) return cache[i].label;
    }
    UINT length = 0;
    if (GetRawInputDeviceInfoA(device, RIDI_DEVICENAME, NULL, &length) == (UINT)-1 || !length) {
        read_errors++; return NULL;
    }
    char *name = calloc((size_t)length + 1, 1);
    if (!name) { failed = 1; return NULL; }
    const char *label = NULL;
    if (GetRawInputDeviceInfoA(device, RIDI_DEVICENAME, name, &length) == (UINT)-1) read_errors++;
    else label = classify_name(name);
    free(name);
    if (cache_count < sizeof(cache) / sizeof(cache[0])) {
        cache[cache_count].handle = device;
        cache[cache_count++].label = label;
    }
    return label;
}

static int write_mouse(FILE *file, const RAWINPUT *raw, UINT bytes,
                       const char *label, long long time_us, UINT input_code)
{
    size_t needed = offsetof(RAWINPUT, data) + sizeof(RAWMOUSE);
    if (!label || bytes < needed || raw->header.dwType != RIM_TYPEMOUSE ||
        raw->header.dwSize < needed || raw->header.dwSize > bytes) return 0;
    const RAWMOUSE *m = &raw->data.mouse;
    return fprintf(file, "%lld,%s,%ld,%ld,%u,%u,%u,%u\n", time_us, label,
                   m->lLastX, m->lLastY, (unsigned int)m->usButtonFlags,
                   (unsigned int)m->usButtonData, (unsigned int)m->usFlags, input_code) < 0 ? -1 : 1;
}

static LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM w, LPARAM l)
{
    if (message == WM_INPUT) {
        LARGE_INTEGER arrival;
        QueryPerformanceCounter(&arrival);
        RAWINPUT raw;
        UINT size = sizeof(raw);
        UINT got = GetRawInputData((HRAWINPUT)l, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER));
        if (got == (UINT)-1 || got < sizeof(RAWINPUTHEADER)) read_errors++;
        else if (raw.header.dwType == RIM_TYPEMOUSE) {
            const char *label = device_label(raw.header.hDevice);
            long long elapsed = (arrival.QuadPart - start.QuadPart) * 1000000 / frequency.QuadPart;
            int result = write_mouse(output, &raw, got, label, elapsed, GET_RAWINPUT_CODE_WPARAM(w));
            if (result > 0) records++;
            if (result < 0) failed = 1;
        }
        if (failed) PostMessageA(window, WM_CLOSE, 0, 0);
        /* WM_INPUT cleanup must still run; do not consume normal pointer input. */
        return DefWindowProcA(window, message, w, l);
    }
    if (message == WM_INPUT_DEVICE_CHANGE) { cache_count = 0; return 0; }
    if (message == WM_TIMER) return 0;
    if (message == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProcA(window, message, w, l);
}

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); return 1; } } while (0)
static int self_test(void)
{
    CHECK(GET_RAWINPUT_CODE_WPARAM(0x100) == RIM_INPUT);
    CHECK(GET_RAWINPUT_CODE_WPARAM(0x101) == RIM_INPUTSINK);
    CHECK(device_label(NULL) == NULL && read_errors == 0);
    CHECK(strcmp(classify_name("\\\\?\\HID#VID_258A&PID_2018&MI_00#test"), "wired") == 0);
    CHECK(strcmp(classify_name("hid#vid_2fe3&pid_0008&mi_02#test"), "receiver") == 0);
    CHECK(!classify_name("HID#VID_258A&PID_20180#test"));
    CHECK(!classify_name("HID#VID_046D&PID_0008#test"));
    RAWINPUT raw = {0};
    raw.header.dwType = RIM_TYPEMOUSE; raw.header.dwSize = sizeof(raw);
    raw.data.mouse.lLastX = (-2147483647L - 1); raw.data.mouse.lLastY = 2147483647L;
    raw.data.mouse.usFlags = MOUSE_MOVE_ABSOLUTE;
    FILE *file = tmpfile(); CHECK(file);
    CHECK(write_mouse(file, &raw, sizeof(raw), "wired", 10, RIM_INPUT) == 1);
    CHECK(write_mouse(file, &raw, sizeof(raw), "receiver", 11, RIM_INPUTSINK) == 1);
    CHECK(write_mouse(file, &raw, sizeof(RAWINPUTHEADER), "wired", 12, RIM_INPUT) == 0);
    CHECK(write_mouse(file, &raw, sizeof(raw), NULL, 13, RIM_INPUT) == 0);
    raw.header.dwType = RIM_TYPEKEYBOARD;
    CHECK(write_mouse(file, &raw, sizeof(raw), "wired", 14, RIM_INPUT) == 0);
    raw.header.dwType = RIM_TYPEMOUSE; raw.header.dwSize = sizeof(raw) + 1;
    CHECK(write_mouse(file, &raw, sizeof(raw), "wired", 15, RIM_INPUT) == 0);
    rewind(file);
    char line[160]; CHECK(fgets(line, sizeof(line), file));
    CHECK(strcmp(line, "10,wired,-2147483648,2147483647,0,0,1,0\n") == 0);
    CHECK(fgets(line, sizeof(line), file));
    CHECK(strcmp(line, "11,receiver,-2147483648,2147483647,0,0,1,1\n") == 0);
    CHECK(fgets(line, sizeof(line), file) == NULL);
    CHECK(fclose(file) == 0);
    puts("PASS: device filtering, packet bounds/type, signed motion, flags and delivery codes; no input registered");
    return 0;
}

int main(void)
{
    int argc;
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) return 1;
    if (argc == 2 && wcscmp(argv[1], L"--self-test") == 0) {
        LocalFree(argv); return self_test();
    }
    int foreground = argc == 4 && wcscmp(argv[3], L"--foreground") == 0;
    wchar_t *end = NULL;
    long seconds = argc == 3 || foreground ? wcstol(argv[1], &end, 10) : 0;
    if ((argc != 3 && !foreground) || !end || *end || seconds < 1 || seconds > 180) {
        fputs("Usage: capture_raw_mouse <1..180 seconds> <new CSV path> [--foreground] | --self-test\n", stderr);
        LocalFree(argv); return 2;
    }
    HANDLE file = CreateFileW(argv[2], GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    LocalFree(argv);
    if (file == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Cannot create NEW capture file; Windows error %lu\n", GetLastError()); return 1;
    }
    int descriptor = _open_osfhandle((intptr_t)file, _O_WRONLY | _O_BINARY);
    if (descriptor < 0) { CloseHandle(file); return 1; }
    output = _fdopen(descriptor, "wb");
    if (!output) { _close(descriptor); return 1; }
    if (setvbuf(output, NULL, _IOFBF, 65536) != 0 ||
        fputs("time_us,source,dx,dy,button_flags,button_data,mouse_flags,input_code\n", output) < 0) failed = 1;
    HINSTANCE instance = GetModuleHandleA(NULL);
    WNDCLASSA wc = {0};
    wc.hInstance = instance; wc.lpfnWndProc = window_proc; wc.lpszClassName = "DiyMouseRawCapture";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    ATOM atom = RegisterClassA(&wc);
    HWND window = atom ? CreateWindowExA(0, wc.lpszClassName, "DIY Mouse Raw Input Capture",
        foreground ? WS_OVERLAPPEDWINDOW : 0,
        foreground ? CW_USEDEFAULT : 0, foreground ? CW_USEDEFAULT : 0, 640, 240,
        foreground ? NULL : HWND_MESSAGE, NULL, instance, NULL) : NULL;
    if (window && foreground) {
        if (!CreateWindowExA(0, "STATIC",
            "Click this window, then keep it active during movement.\r\n"
            "Use the same surface and DPI; move circles and straight strokes.\r\n"
            "No clicks or scrolling during motion. Capture closes automatically.",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 15, 30, 590, 130, window, NULL, instance, NULL)) failed = 1;
        ShowWindow(window, SW_SHOWNOACTIVATE);
    }
    RAWINPUTDEVICE device = {1, 2, RIDEV_INPUTSINK | RIDEV_DEVNOTIFY, window};
    BOOL registered = window && !failed && QueryPerformanceFrequency(&frequency) &&
                      RegisterRawInputDevices(&device, 1, sizeof(device));
    if (!registered || !SetTimer(window, 1, 20, NULL)) failed = 1;
    SYSTEMTIME utc;
    GetSystemTime(&utc);
    QueryPerformanceCounter(&start);
    DWORD start_ms = GetTickCount();
    if (!failed) {
        printf("RAW_INPUT_READY start_utc=%04u-%02u-%02uT%02u:%02u:%02u.%03uZ; mouse-only wired/receiver capture; no keyboard or input injection\n",
               (unsigned int)utc.wYear, (unsigned int)utc.wMonth, (unsigned int)utc.wDay,
               (unsigned int)utc.wHour, (unsigned int)utc.wMinute, (unsigned int)utc.wSecond,
               (unsigned int)utc.wMilliseconds);
        fflush(stdout);
        if (foreground) { puts("FOREGROUND_WINDOW_SHOWN: click capture window and keep it active; input_code=0 identifies foreground delivery"); fflush(stdout); }
        MSG message;
        int result;
        while ((result = GetMessageA(&message, NULL, 0, 0)) > 0) {
            if ((DWORD)(GetTickCount() - start_ms) >= (DWORD)seconds * 1000) break;
            TranslateMessage(&message); DispatchMessageA(&message);
        }
        if (result < 0) failed = 1;
    } else fprintf(stderr, "Raw input startup failed; Windows error %lu\n", GetLastError());
    if (registered) {
        device.dwFlags = RIDEV_REMOVE; device.hwndTarget = NULL;
        if (!RegisterRawInputDevices(&device, 1, sizeof(device))) failed = 1;
    }
    if (window && IsWindow(window)) DestroyWindow(window);
    if (atom) UnregisterClassA(wc.lpszClassName, instance);
    if (fclose(output) != 0) failed = 1;
    printf("RAW_INPUT_DONE records=%llu read_errors=%u failed=%d\n", records, read_errors, failed);
    fflush(stdout);
    return failed || read_errors ? 1 : 0;
}
