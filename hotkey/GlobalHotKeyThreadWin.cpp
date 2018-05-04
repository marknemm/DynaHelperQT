#include "GlobalHotKeyThreadWin.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)


#include <QDebug>
//#include <process.h>

#define WM_END_THREAD (WM_USER+2)


GlobalHotKeyThreadWin::GlobalHotKeyThreadWin()
    : _mThreadId(0)
{}


GlobalHotKeyThreadWin::~GlobalHotKeyThreadWin()
{}


void GlobalHotKeyThreadWin::stop()
{
    if(_mThreadId != 0) {
        ::PostThreadMessage(_mThreadId, WM_END_THREAD, 0, 0);
    }
}


void GlobalHotKeyThreadWin::run()
{
    // store a thread id so we can exit later
    _mThreadId = ::GetCurrentThreadId();

    //qDebug() << "ThreadIDs" << QString::number(_mThreadId, 16) << QString::number((int) this->currentThreadId(), 16);

    // register an atom, and a hotkey
    BOOL retVal;
    int counter = 0;
    int magicNum = 1128;
    ATOM id = ::GlobalAddAtom(MAKEINTATOM(magicNum + counter++));

    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
    int modifier = MOD_CONTROL;// Control Modifier
    int key = 0x57;// W-Key Code
    retVal = ::RegisterHotKey(NULL, id, modifier, key);

    if (!retVal) {
        qDebug() << "Failed to add a hotkey!";
        return;
    }

    // wait on hotkeys
    MSG msg;
    while (0 < ::GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            /*bool control = LOWORD(msg.lParam) & MOD_CONTROL;
            bool shift = LOWORD(msg.lParam) & MOD_SHIFT;
            bool alt = LOWORD(msg.lParam) & MOD_ALT;
            bool win = LOWORD(msg.lParam) & MOD_WIN;
            qDebug() << "HotKey!" << (control ? "Ctrl +": "")
                << (alt ? "Alt +": "")
                << (shift ? "Shift +":"")
                << (win ? "Win +":"") << QString::number(HIWORD(msg.lParam),16);*/
            emit hotKeyEvent();
        }
        else if (msg.message == WM_END_THREAD) {
            break; // exit
        }
    }

    // Clean up Hotkey
    ::UnregisterHotKey(NULL, id);
    ::GlobalDeleteAtom(id);
}


#endif
