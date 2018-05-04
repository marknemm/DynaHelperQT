#include "IOUtil.h"
#include <QDebug>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>


QString IOUtil::getForegroundProcNameWin32(bool useCursorPosition)
{
    QString procName;

// Are we compiling with Visual C++ Compiler?
#if _MSC_VER > 0
    HWND hwnd;
    if (useCursorPosition) {
        POINT pos;
        GetCursorPos(&pos);
        hwnd = WindowFromPoint(pos);
    }
    else {
        hwnd = GetForegroundWindow();
    }

    DWORD dwPID;
    GetWindowThreadProcessId(hwnd, &dwPID);
    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
    if (handle)
    {
        WCHAR buffer[MAX_PATH];
        DWORD bufferSize = (DWORD)MAX_PATH;
        if (QueryFullProcessImageName(handle, 0, (LPWSTR)&buffer, &bufferSize))
        {
            procName = QString::fromWCharArray(buffer);
        }
        CloseHandle(handle);
    }
#endif

    return procName;
}


#endif // WINDOWS CHECK


IOUtil::IOUtil()
{}
