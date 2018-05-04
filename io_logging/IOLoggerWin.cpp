#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include "io_logging/IOLoggerWin.h"
#include "io_logging/IOUtil.h"
#include <QDebug>


HHOOK IOLoggerWin::_hKeyHook;
HHOOK IOLoggerWin::_hMouseHook;


IOLoggerWin::IOLoggerWin()
{
    _hKeyHook = NULL;
    _hMouseHook = NULL;
}


IOLoggerWin::~IOLoggerWin()
{
    deactivate();
}


void IOLoggerWin::activate()
{
    _hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
    if (_hKeyHook == NULL) {
        qDebug() << "SetWindowsHookEx failed for keyboard hook with error code: " << QString::number(GetLastError());
    }

    _hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, NULL, 0);
    if (_hMouseHook == NULL) {
        qDebug() << "SetWindowsHookEx failed for mouse hook with error code: " << QString::number(GetLastError());
    }
}


void IOLoggerWin::deactivate()
{
    if (_hKeyHook != NULL) {
        BOOL succ = UnhookWindowsHookEx(_hKeyHook);
        if (!succ) {
            qDebug() << "UnhookWindowsHookEx failed for keyboard hook with error code: " << QString::number(GetLastError());
        }
        _hKeyHook = NULL;
    }

    if (_hMouseHook != NULL) {
        BOOL succ = UnhookWindowsHookEx(_hMouseHook);
        if (!succ) {
            qDebug() << "UnhookWindowsHookEx failed for mouse hook with error code: " << QString::number(GetLastError());
        }
        _hKeyHook = NULL;
    }
}


LRESULT CALLBACK IOLoggerWin::keyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //LoggerLock lock(_procMutex); // Make sure we don't interleave between handling mouse and keyboard events!

    if (nCode >= HC_ACTION) {
        //Get the key information
        KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

        wchar_t buffer[5];

        //get the keyboard state
        BYTE keyboard_state[256];
        GetKeyboardState(keyboard_state);
        keyboard_state[VK_SHIFT]   = GetKeyState(VK_SHIFT);
        keyboard_state[VK_CAPITAL] = GetKeyState(VK_CAPITAL);
        keyboard_state[VK_CONTROL] = GetKeyState(VK_CONTROL);
        keyboard_state[VK_MENU]    = GetKeyState(VK_MENU);

        //Get keyboard layout
        HKL keyboard_layout = GetKeyboardLayout(0);

        //Get the name
        char lpszName[0X100] = {0};

        DWORD dwMsg = 1;
        dwMsg += cKey.scanCode << 16;
        dwMsg += cKey.flags << 24;

        int keyNameSize = GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);
        if (keyNameSize == 0) {
            qDebug() << "Error getting key name in Windows global keyboard hook with code: " << QString::number(GetLastError());
        }

        //Try to convert the key information
        ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
        buffer[4] = L'\0';

        //Print the output
        //qDebug() << "Key State: " << wParam;
        //qDebug() << "Key: " << cKey.vkCode << " " << QString::fromUtf16((ushort*)buffer) << " " << QString::fromUtf16((ushort*)lpszName);

        MacroEvent event;
        fillMacroEventFromKeyboard(event, wParam, cKey.vkCode);
        notifyListener(event);
    }

    return CallNextHookEx(_hKeyHook, nCode, wParam, lParam);
    // LoggerLock automatically released when scope ends!
}


LRESULT CALLBACK IOLoggerWin::mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //LoggerLock lock(_procMutex); // Make sure we don't interleave between handling mouse and keyboard events!

    if (nCode >= HC_ACTION) {
        //Get the mouse information
        MSLLHOOKSTRUCT cMouse = *((MSLLHOOKSTRUCT*)lParam);

        //Get mouse location
        POINT loc = cMouse.pt;
        WORD mouseDelta = HIWORD(cMouse.mouseData);

        //Print the output
        //qDebug() << "Mouse State: " << wParam << " = " << WM_LBUTTONDOWN;
        //qDebug() << "Mouse location: (" << loc.x << ", " << loc.y << ")    Delta: " << mouseDelta;

        MacroEvent event;
        fillMacroEventFromMouse(event, wParam, loc, mouseDelta);
        notifyListener(event);
    }

    return CallNextHookEx(_hMouseHook, nCode, wParam, lParam);
    // LoggerLock automatically released when scope ends!
}


void IOLoggerWin::fillMacroEventFromKeyboard(MacroEvent &event, WPARAM type, DWORD keyCode)
{
    event.type = KeyboardEvent;
    event.targetPID = IOUtil::getForegroundProcNameWin32(false);

    // Set Keyboard Event Type.
    switch (type) {
    case 256: event.keyboardEvent.type = KeyPress;      break;
    case 257: event.keyboardEvent.type = KeyRelease;    break;
    }

    // Set Key Code.
    switch(keyCode) {
    case VK_TAB:        event.keyboardEvent.keyCode = Qt::Key_Tab;                              break;
    case VK_BACK:       event.keyboardEvent.keyCode = Qt::Key_Backspace;                        break;
    case VK_ESCAPE:     event.keyboardEvent.keyCode = Qt::Key_Escape;                           break;
    case VK_RETURN:     event.keyboardEvent.keyCode = Qt::Key_Return;                           break;
    case VK_CAPITAL:    event.keyboardEvent.keyCode = Qt::Key_CapsLock;                         break;
    case VK_SNAPSHOT:   event.keyboardEvent.keyCode = Qt::Key_Print;                            break;
    case VK_DELETE:     event.keyboardEvent.keyCode = Qt::Key_Delete;                           break;
    case VK_HOME:       event.keyboardEvent.keyCode = Qt::Key_Home;                             break;
    case VK_END:        event.keyboardEvent.keyCode = Qt::Key_End;                              break;
    case VK_PRIOR:      event.keyboardEvent.keyCode = Qt::Key_PageUp;                           break;
    case VK_NEXT:       event.keyboardEvent.keyCode = Qt::Key_PageDown;                         break;
    case VK_NUMLOCK:    event.keyboardEvent.keyCode = Qt::Key_NumLock;                          break;
    case VK_ADD:        event.keyboardEvent.keyCode = Qt::Key_Plus | Qt::KeypadModifier;        break;
    case VK_SUBTRACT:   event.keyboardEvent.keyCode = Qt::Key_Minus | Qt::KeypadModifier;       break;
    case VK_MULTIPLY:   event.keyboardEvent.keyCode = Qt::Key_Asterisk | Qt::KeypadModifier;    break;
    case VK_DIVIDE:     event.keyboardEvent.keyCode = Qt::Key_Slash | Qt::KeypadModifier;       break;
    case VK_DECIMAL:    event.keyboardEvent.keyCode = Qt::Key_Period | Qt::KeypadModifier;      break;
    case VK_NUMPAD0:    event.keyboardEvent.keyCode = Qt::Key_0 | Qt::KeypadModifier;           break;
    case VK_NUMPAD1:    event.keyboardEvent.keyCode = Qt::Key_1 | Qt::KeypadModifier;           break;
    case VK_NUMPAD2:    event.keyboardEvent.keyCode = Qt::Key_2 | Qt::KeypadModifier;           break;
    case VK_NUMPAD3:    event.keyboardEvent.keyCode = Qt::Key_3 | Qt::KeypadModifier;           break;
    case VK_NUMPAD4:    event.keyboardEvent.keyCode = Qt::Key_4 | Qt::KeypadModifier;           break;
    case VK_NUMPAD5:    event.keyboardEvent.keyCode = Qt::Key_5 | Qt::KeypadModifier;           break;
    case VK_NUMPAD6:    event.keyboardEvent.keyCode = Qt::Key_6 | Qt::KeypadModifier;           break;
    case VK_NUMPAD7:    event.keyboardEvent.keyCode = Qt::Key_7 | Qt::KeypadModifier;           break;
    case VK_NUMPAD8:    event.keyboardEvent.keyCode = Qt::Key_8 | Qt::KeypadModifier;           break;
    case VK_NUMPAD9:    event.keyboardEvent.keyCode = Qt::Key_9 | Qt::KeypadModifier;           break;
    case VK_UP:         event.keyboardEvent.keyCode = Qt::Key_Up;                               break;
    case VK_DOWN:       event.keyboardEvent.keyCode = Qt::Key_Down;                             break;
    case VK_LEFT:       event.keyboardEvent.keyCode = Qt::Key_Left;                             break;
    case VK_RIGHT:      event.keyboardEvent.keyCode = Qt::Key_Right;                            break;
    case VK_OEM_COMMA:  event.keyboardEvent.keyCode = Qt::Key_Comma;                            break;
    case VK_OEM_PERIOD: event.keyboardEvent.keyCode = Qt::Key_Period;                           break;
    case VK_OEM_MINUS:  event.keyboardEvent.keyCode = Qt::Key_Minus;                            break;
    case VK_OEM_PLUS:   event.keyboardEvent.keyCode = Qt::Key_Equal;                            break;
    case VK_OEM_1:      event.keyboardEvent.keyCode = Qt::Key_Semicolon;                        break;
    case VK_OEM_2:      event.keyboardEvent.keyCode = Qt::Key_Slash;                            break;
    case VK_OEM_3:      event.keyboardEvent.keyCode = Qt::Key_QuoteLeft;                        break;
    case VK_OEM_4:      event.keyboardEvent.keyCode = Qt::Key_BraceLeft;                        break;
    case VK_OEM_5:      event.keyboardEvent.keyCode = Qt::Key_Backslash;                        break;
    case VK_OEM_6:      event.keyboardEvent.keyCode = Qt::Key_BraceRight;                       break;
    case VK_OEM_7:      event.keyboardEvent.keyCode = Qt::Key_QuoteDbl;                         break;
    case VK_F1:         event.keyboardEvent.keyCode = Qt::Key_F1;                               break;
    case VK_F2:         event.keyboardEvent.keyCode = Qt::Key_F2;                               break;
    case VK_F3:         event.keyboardEvent.keyCode = Qt::Key_F3;                               break;
    case VK_F4:         event.keyboardEvent.keyCode = Qt::Key_F4;                               break;
    case VK_F5:         event.keyboardEvent.keyCode = Qt::Key_F5;                               break;
    case VK_F6:         event.keyboardEvent.keyCode = Qt::Key_F6;                               break;
    case VK_F7:         event.keyboardEvent.keyCode = Qt::Key_F7;                               break;
    case VK_F8:         event.keyboardEvent.keyCode = Qt::Key_F8;                               break;
    case VK_F9:         event.keyboardEvent.keyCode = Qt::Key_F9;                               break;
    case VK_F10:        event.keyboardEvent.keyCode = Qt::Key_F10;                              break;
    case VK_F11:        event.keyboardEvent.keyCode = Qt::Key_F11;                              break;
    case VK_F12:        event.keyboardEvent.keyCode = Qt::Key_F12;                              break;
    case VK_F13:        event.keyboardEvent.keyCode = Qt::Key_F13;                              break;
    case VK_F14:        event.keyboardEvent.keyCode = Qt::Key_F14;                              break;
    case VK_F15:        event.keyboardEvent.keyCode = Qt::Key_F15;                              break;
    case VK_F16:        event.keyboardEvent.keyCode = Qt::Key_F16;                              break;
    case VK_F17:        event.keyboardEvent.keyCode = Qt::Key_F17;                              break;
    case VK_F18:        event.keyboardEvent.keyCode = Qt::Key_F18;                              break;
    case VK_F19:        event.keyboardEvent.keyCode = Qt::Key_F19;                              break;
    case VK_F20:        event.keyboardEvent.keyCode = Qt::Key_F20;                              break;
    case VK_F21:        event.keyboardEvent.keyCode = Qt::Key_F21;                              break;
    case VK_F22:        event.keyboardEvent.keyCode = Qt::Key_F22;                              break;
    case VK_F23:        event.keyboardEvent.keyCode = Qt::Key_F23;                              break;
    case VK_F24:        event.keyboardEvent.keyCode = Qt::Key_F24;                              break;
    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
                        event.keyboardEvent.keyCode = Qt::Key_Shift;                            break;
    case VK_MENU:
    case VK_LMENU:
    case VK_RMENU:
                        event.keyboardEvent.keyCode = Qt::Key_Alt;                              break;
    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
                        event.keyboardEvent.keyCode = Qt::Key_Control;                          break;
    default:            event.keyboardEvent.keyCode = keyCode; // [A, Z] [0, 9].
    }

    bool shiftDown = GetKeyState(VK_SHIFT) < 0;
    bool ctrlDown  = GetKeyState(VK_CONTROL) < 0;
    bool altDown   = GetKeyState(VK_MENU) < 0;
    bool capsLock  = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
    bool numLock   = (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;

    // Set Mod1.
    if(shiftDown) {
        event.keyboardEvent.mod1 = Qt::Key_Shift;
    }
    else if (ctrlDown) {
        event.keyboardEvent.mod1 = Qt::Key_Control;
    }
    else if (altDown) {
        event.keyboardEvent.mod1 = Qt::Key_Alt;
    }
    else {
        event.keyboardEvent.mod1 = -1;
    }

    // Set Mod2.
    if (ctrlDown && event.keyboardEvent.mod1 != Qt::Key_Control) {
        event.keyboardEvent.mod2 = Qt::Key_Control;
    }
    else if (altDown && event.keyboardEvent.mod1 != Qt::Key_Alt) {
        event.keyboardEvent.mod2 = Qt::Key_Alt;
    }
    else {
        event.keyboardEvent.mod2 = -1;
    }

    // Set Caps Lock Flag.
    event.keyboardEvent.capsLock = capsLock;
    // Set Num Lock Flag.
    event.keyboardEvent.numLock = numLock;
}


void IOLoggerWin::fillMacroEventFromMouse(MacroEvent &event, WPARAM type, const POINT &loc, WORD wheelDelta)
{
    event.type = MouseEvent;
    event.targetPID = IOUtil::getForegroundProcNameWin32();
    event.mouseEvent.wheelDelta = 0;

    switch (type) {
    case WM_MOUSEMOVE:   event.mouseEvent.type = Move;          break;
    case WM_LBUTTONDOWN: event.mouseEvent.type = LeftPress;     break;
    case WM_LBUTTONUP:   event.mouseEvent.type = LeftRelease;   break;
    case WM_RBUTTONDOWN: event.mouseEvent.type = RightPress;    break;
    case WM_RBUTTONUP:   event.mouseEvent.type = RightRelease;  break;
    case WM_MBUTTONDOWN: event.mouseEvent.type = MiddlePress;   break;
    case WM_MBUTTONUP:   event.mouseEvent.type = MiddleRelease; break;
    case WM_MOUSEWHEEL:
        if (wheelDelta == 120) {
            event.mouseEvent.type = ScrollUp;
            event.mouseEvent.wheelDelta = 1;
        }
        else {
            event.mouseEvent.type = ScrollDown;
            event.mouseEvent.wheelDelta = -1;
        }
        break;
    }

    event.mouseEvent.loc.setX(loc.x);
    event.mouseEvent.loc.setY(loc.y);
}


#endif // WINDOWS CHECK
