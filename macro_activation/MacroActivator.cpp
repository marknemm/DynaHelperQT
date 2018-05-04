#include "MacroActivator.h"
#include "record_img/RecordImageUtil.h"
#include <Windows.h>
#include <QDateTime>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>


MacroActivator::MacroActivator()
    : QObject(),
      _run(false)
{}


void MacroActivator::runMacro(const QList<MacroEvent> &macroEvents)
{
    // Be sure to set _run on main thread so we do not create race condition!
    _run = true;
    // Must be run in a separate thread so that main event thread can process interrupt hotkey!
    QtConcurrent::run(this, &MacroActivator::runMacroInOwnThread, macroEvents);
}


void MacroActivator::stopMacro()
{
    _run = false;
}


void MacroActivator::runMacroInOwnThread(QList<MacroEvent> &macroEvents)
{
    bool err = false;
    QString errMsg;
    int i; // Need in this scope to feed to cleanup method!

    // At start of method, always get timestamp to calculate delay & duration.
    qint64 timeSinceLastEventMs,
           lastEventTimestampMs = QDateTime::currentMSecsSinceEpoch(),
           remainingDelayMs,
           repeatDelayMs;

    try {
        for (i = 0; i < macroEvents.size() && _run; i++) {
            // Move mouse before delay for event if location sensitive mouse event.
            if (   macroEvents.at(i).type == MacroEventType::MouseEvent
                && macroEvents.at(i).mouseEvent.type != MacroMouseEventType::ScrollUp
                && macroEvents.at(i).mouseEvent.type != MacroMouseEventType::ScrollDown)
            {
                // Just sleep a little before each mouse move!
                QThread::msleep(350);
                moveMouseToPos(macroEvents.at(i).mouseEvent.loc.x(), macroEvents.at(i).mouseEvent.loc.y());
            }
            // Simply set duration of all key string events to 0 for now... later we will actually type it out.
            else if (   macroEvents.at(i).type == MacroEventType::KeyboardEvent
                     && macroEvents.at(i).keyboardEvent.type == MacroKeyboardEventType::KeyString)
            {
                macroEvents[i].durationMs = 0;
            }

            // Display info pertaining to the event that we are activating.
            printMacroEvent(macroEvents.at(i));
            emit activatingMacroEvent(macroEvents.at(i));

            // Update elapsed time.
            timeSinceLastEventMs = QDateTime::currentMSecsSinceEpoch() - lastEventTimestampMs;
            remainingDelayMs = (macroEvents.at(i).delayMs - timeSinceLastEventMs);

            // Sleep for remaining delay time before the event starts!
            interruptableSleepMs(remainingDelayMs);

            // Calculate duration time per repeat event. If no repeat, then will be durationMs identity.
            repeatDelayMs = (macroEvents.at(i).durationMs / (macroEvents.at(i).nRepeats + 1));

            for (int r = 0; r <= macroEvents.at(i).nRepeats && _run; r++) {
                // Run either mouse or keyboard event.
                if (macroEvents.at(i).type == MacroEventType::MouseEvent) {
                    runMouseEvent(macroEvents.at(i).mouseEvent);
                }
                else {
                    runKeyboardEvent(macroEvents.at(i).keyboardEvent);
                }

                // Sleep between every repeated event to fill in whole duration time of event!
                interruptableSleepMs(repeatDelayMs);
            }

            // Set timestamp to time after this event has completed.
            lastEventTimestampMs = QDateTime::currentMSecsSinceEpoch();
        }

        i--; // Bring i back by one so that cleanup gets the last activated event!
    }
    catch(std::exception e) {
        _run = false; // Set _run flag false prematurely to signify that Macro Cleanup must occur!
        err = true;
        errMsg = QString("Error when attempting to execute Macro: ") + e.what();
        qDebug() << errMsg;
    }

    // Be sure to do Macro cleanup in case some key or mouse state is left behind!
    cleanupMacro(macroEvents, i);
    stopMacro();
    emit macroActivatorStopped(err, errMsg);
}


void MacroActivator::runMouseEvent(const MacroMouseEvent &mEvent) const
{
    switch(mEvent.type) {
    case LeftPress:     leftPress();            break;
    case LeftRelease:   leftRelease();          break;
    case LeftClick:     leftPress();
                        leftRelease();          break;
    case DoubleClick:   leftPress();
                        leftRelease();
                        QThread::msleep(50);
                        leftPress();
                        leftRelease();          break;
    case RightPress:    rightPress();           break;
    case RightRelease:  rightRelease();         break;
    case RightClick:    rightPress();
                        rightRelease();         break;
    case MiddlePress:   middlePress();          break;
    case MiddleRelease: middleRelease();        break;
    case MiddleClick:   middlePress();
                        middleRelease();        break;
    case ScrollUp:      scrollUp();             break;
    case ScrollDown:    scrollDown();           break;
    default:
        qDebug() << "Error: incorrect Mouse Event Type: " << mEvent.type;
        exit(1);
    }
}


void MacroActivator::runKeyboardEvent(const MacroKeyboardEvent &kEvent) const
{
    switch (kEvent.type) {
    case KeyPress:      keyPress(kEvent);               break;
    case KeyRelease:    keyRelease(kEvent);             break;
    case KeyType:       keyPress(kEvent);
                        keyRelease(kEvent);             break;
    case KeyString:     keyString(kEvent.keyString);    break;
    default:
        qDebug() << "Error: incorrect Keyboard Event Type: " << kEvent.type;
        exit(1);
    }
}


void MacroActivator::moveMouseToPos(int x, int y) const
{
    const float RATE = 2.0;
    POINT curPos;
    double dx,
           dy,
           dt,
           propX,
           propY;

    do {
        GetCursorPos(&curPos);

        // X and Y distance remaining.
        dx = x - curPos.x;
        dy = y - curPos.y;
        // Total distance remaining.
        dt = floor(sqrt(pow(dx, 2) + pow(dy, 2)));

        // If remaining distance is greater than 10, then move normally.
        if (dt > 10) {
            // Proportions to move in x and y directions.
            propX = dx / dt;
            propY = dy / dt;

            qDebug() << "Total dist remaining: " << dt;
            qDebug() << "X: " << curPos.x << " + " << ceil(RATE * propX);
            qDebug() << "Y: " << curPos.y << " + " << ceil(RATE * propY);

            // Update current mouse position and set.
            curPos.x += ceil(RATE * propX);
            curPos.y += ceil(RATE * propY);

            qDebug() << "New X position: " << curPos.x;
            qDebug() << "New Y position: " << curPos.y;

            setMousePos(curPos.x, curPos.y);
        }
        // Else, move right to destination if close enough...
        else {
            curPos.x = x;
            curPos.y = y;
            setMousePos(curPos.x, curPos.y);
        }
    }
    while ((curPos.x != x || curPos.y != y) && _run);
}


void MacroActivator::setMousePos(int x, int y) const
{
    // Get screen dimensions.
    RECT dims;
    HWND desktop = GetDesktopWindow();
    GetClientRect(desktop, &dims);

    // Set the absolute mouse position.
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
    input.mi.dx = (int)(65536.0 / dims.right * x + 1);
    input.mi.dy = (int)(65536.0 / dims.bottom * y + 1);
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::leftPress() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::leftRelease() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::rightPress() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::rightRelease() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::middlePress() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::middleRelease() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::scrollUp() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = WHEEL_DELTA;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::scrollDown() const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = -WHEEL_DELTA;
    SendInput(1, &input, sizeof(INPUT));
}


void MacroActivator::keyString(const QString &qText) const
{
    std::string text(qText.toLocal8Bit().constData());
    if(OpenClipboard(NULL)) {
        HGLOBAL clipbuffer;
        char* buffer;
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_DDESHARE, text.length() + 1);
        buffer = (char*)GlobalLock(clipbuffer);
        strcpy_s(buffer, text.length() + 1, text.c_str());
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT,clipbuffer);
        CloseClipboard();
    }
    INPUT input[4];
    for(int i(0) ; i < 4 ; i++) {
        ZeroMemory(&input[i], sizeof(INPUT));
        input[i].type = INPUT_KEYBOARD;
    }
    input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    input[0].ki.wScan = MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC);
    input[1].ki.dwFlags = KEYEVENTF_SCANCODE;
    input[1].ki.wScan = MapVirtualKey(0x56, MAPVK_VK_TO_VSC);; // 0x2F == 'V'
    input[2].ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
    input[2].ki.wScan = MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC);
    input[3].ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
    input[3].ki.wScan = MapVirtualKey(0x56, MAPVK_VK_TO_VSC);;
    SendInput(4, &input[0], sizeof(INPUT));
}


void MacroActivator::keyPress(const MacroKeyboardEvent &kEvent) const
{
    int winKeyCode = mapQtVkToWinVk(kEvent.keyCode);
    int scanCode = MapVirtualKey((UINT)winKeyCode, MAPVK_VK_TO_VSC);

    applyKeyMods(kEvent);

    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_SCANCODE;
    input.ki.wScan = scanCode;
    SendInput(1, &input, sizeof(INPUT));

    removeKeyMods(kEvent);
}


void MacroActivator::keyRelease(const MacroKeyboardEvent &kEvent) const
{
    int winKeyCode = mapQtVkToWinVk(kEvent.keyCode);
    int scanCode = MapVirtualKey((UINT)winKeyCode, MAPVK_VK_TO_VSC);

    applyKeyMods(kEvent);

    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
    input.ki.wScan = scanCode;
    SendInput(1, &input, sizeof(INPUT));

    removeKeyMods(kEvent);
}


int MacroActivator::mapQtVkToWinVk(int qtVk) const
{
    switch(qtVk) {
    case Qt::Key_Tab:                           return VK_TAB;
    case Qt::Key_Backspace:                     return VK_BACK;
    case Qt::Key_Escape:                        return VK_ESCAPE;
    case Qt::Key_Return:                        return VK_RETURN;
    case Qt::Key_CapsLock:                      return VK_CAPITAL;
    case Qt::Key_Print:                         return VK_SNAPSHOT;
    case Qt::Key_Delete:                        return VK_DELETE;
    case Qt::Key_Home:                          return VK_HOME;
    case Qt::Key_End:                           return VK_END;
    case Qt::Key_PageUp:                        return VK_PRIOR;
    case Qt::Key_PageDown:                      return VK_NEXT;
    case Qt::Key_NumLock:                       return VK_NUMLOCK;
    case Qt::Key_Plus | Qt::KeypadModifier:     return VK_ADD;
    case Qt::Key_Minus | Qt::KeypadModifier:    return VK_SUBTRACT;
    case Qt::Key_Asterisk | Qt::KeypadModifier: return VK_MULTIPLY;
    case Qt::Key_Slash | Qt::KeypadModifier:    return VK_DIVIDE;
    case Qt::Key_Period | Qt::KeypadModifier:   return VK_DECIMAL;
    case Qt::Key_0 | Qt::KeypadModifier:        return VK_NUMPAD0;
    case Qt::Key_1 | Qt::KeypadModifier:        return VK_NUMPAD1;
    case Qt::Key_2 | Qt::KeypadModifier:        return VK_NUMPAD2;
    case Qt::Key_3 | Qt::KeypadModifier:        return VK_NUMPAD3;
    case Qt::Key_4 | Qt::KeypadModifier:        return VK_NUMPAD4;
    case Qt::Key_5 | Qt::KeypadModifier:        return VK_NUMPAD5;
    case Qt::Key_6 | Qt::KeypadModifier:        return VK_NUMPAD6;
    case Qt::Key_7 | Qt::KeypadModifier:        return VK_NUMPAD7;
    case Qt::Key_8 | Qt::KeypadModifier:        return VK_NUMPAD8;
    case Qt::Key_9 | Qt::KeypadModifier:        return VK_NUMPAD9;
    case Qt::Key_Up:                            return VK_UP;
    case Qt::Key_Down:                          return VK_DOWN;
    case Qt::Key_Left:                          return VK_LEFT;
    case Qt::Key_Right:                         return VK_RIGHT;
    case Qt::Key_Comma:                         return VK_OEM_COMMA;
    case Qt::Key_Period:                        return VK_OEM_PERIOD;
    case Qt::Key_Minus:                         return VK_OEM_MINUS;
    case Qt::Key_Equal:                         return VK_OEM_PLUS;
    case Qt::Key_Semicolon:                     return VK_OEM_1;
    case Qt::Key_Slash:                         return VK_OEM_2;
    case Qt::Key_QuoteLeft:                     return VK_OEM_3;
    case Qt::Key_BraceLeft:                     return VK_OEM_4;
    case Qt::Key_Backslash:                     return VK_OEM_5;
    case Qt::Key_BraceRight:                    return VK_OEM_6;
    case Qt::Key_QuoteDbl:                      return VK_OEM_7;
    case Qt::Key_F1:                            return VK_F1;
    case Qt::Key_F2:                            return VK_F2;
    case Qt::Key_F3:                            return VK_F3;
    case Qt::Key_F4:                            return VK_F4;
    case Qt::Key_F5:                            return VK_F5;
    case Qt::Key_F6:                            return VK_F6;
    case Qt::Key_F7:                            return VK_F7;
    case Qt::Key_F8:                            return VK_F8;
    case Qt::Key_F9:                            return VK_F9;
    case Qt::Key_F10:                           return VK_F10;
    case Qt::Key_F11:                           return VK_F11;
    case Qt::Key_F12:                           return VK_F12;
    case Qt::Key_F13:                           return VK_F13;
    case Qt::Key_F14:                           return VK_F14;
    case Qt::Key_F15:                           return VK_F15;
    case Qt::Key_F16:                           return VK_F16;
    case Qt::Key_F17:                           return VK_F17;
    case Qt::Key_F18:                           return VK_F18;
    case Qt::Key_F19:                           return VK_F19;
    case Qt::Key_F20:                           return VK_F20;
    case Qt::Key_F21:                           return VK_F21;
    case Qt::Key_F22:                           return VK_F22;
    case Qt::Key_F23:                           return VK_F23;
    case Qt::Key_F24:                           return VK_F24;
    case Qt::Key_Shift:                         return VK_SHIFT;
    case Qt::Key_Alt:                           return VK_MENU;
    case Qt::Key_Control:                       return VK_CONTROL;
    default:                                    return qtVk; // [A, Z] [0, 9].
    }
}


void MacroActivator::applyKeyMods(const MacroKeyboardEvent &kEvent) const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;

    bool capsLockNeedsToTurnOn =   kEvent.capsLock
                                && (GetKeyState(VK_CAPITAL) & 0x0001) == 0;

    bool capsLockNeedsToTurnOff =   !kEvent.capsLock
                                 && (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    bool numLockNeedsToTurnOn =    kEvent.numLock
                                 && (GetKeyState(VK_NUMLOCK) & 0x0001) == 0
                                 && !isKeyCodeAssocWithNumpadOff(kEvent.keyCode);

    bool numLockNeedsToTurnOff =    (!kEvent.numLock || isKeyCodeAssocWithNumpadOff(kEvent.keyCode))
                                 && (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;

    // Should we toggle CAPS LOCK?
    if (   capsLockNeedsToTurnOn
        || capsLockNeedsToTurnOff)
    {
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        input.ki.wScan = MapVirtualKey((UINT)VK_CAPITAL, MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
        SendInput(1, &input, sizeof(INPUT));
    }

    // Should we toggle NUM LOCK?
    if (   numLockNeedsToTurnOn
        || numLockNeedsToTurnOff)
    {
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        input.ki.wScan = MapVirtualKey((UINT)VK_NUMLOCK, MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
        SendInput(1, &input, sizeof(INPUT));
    }

    input.ki.dwFlags = KEYEVENTF_SCANCODE;

    // Do we have Shift, Alt, or Control in mod1?
    if (kEvent.mod1 != -1) {
        input.ki.wScan = MapVirtualKey((UINT)mapQtVkToWinVk(kEvent.mod1), MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
    }

    // Do we have Shift, Alt, or Control in mod2?
    if (kEvent.mod2 != -1) {
        input.ki.wScan = MapVirtualKey((UINT)mapQtVkToWinVk(kEvent.mod2), MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
    }
}


void MacroActivator::removeKeyMods(const MacroKeyboardEvent &kEvent) const
{
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;

    // Do we need to turn CAPS LOCK off?
    if (kEvent.capsLock) {
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        input.ki.wScan = MapVirtualKey((UINT)VK_CAPITAL, MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
        SendInput(1, &input, sizeof(INPUT));
    }

    // No need to really clean up NUM LOCK...

    input.ki.dwFlags = (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);

    // Do we need to release mod1 if we have one?
    if (kEvent.mod1 != -1) {
        input.ki.wScan = MapVirtualKey((UINT)mapQtVkToWinVk(kEvent.mod1), MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
    }

    // Do we need to release mod2 if we have one?
    if (kEvent.mod2 != -1) {
        input.ki.wScan = MapVirtualKey((UINT)mapQtVkToWinVk(kEvent.mod2), MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
    }
}


void MacroActivator::cleanupMacro(const QList<MacroEvent> &macroEvents, int lastEventInd) const
{
    // We met a hard stop either by exception or user kill hotkey.
    if (!_run) {
        qDebug() << "Cleaning up at index: " << lastEventInd;
        const MacroEvent &lastExecEvent = macroEvents.at(lastEventInd);

        // If mouse event that could leave state behind, we must attempt to erase that state!
        if (   lastExecEvent.type == MacroEventType::MouseEvent
            && lastExecEvent.mouseEvent.type != MacroMouseEventType::ScrollUp
            && lastExecEvent.mouseEvent.type != MacroMouseEventType::ScrollDown)
        {
            // Set mouse event to be run to erase any remaining state with a specific button release.
            MacroMouseEvent mEvent = lastExecEvent.mouseEvent;
            switch (mEvent.type) {
            case LeftPress:
            case LeftRelease: // May not have finished release!
            case LeftClick:
                mEvent.type = LeftRelease;      break;
            case RightPress:
            case RightRelease:
            case RightClick:
                mEvent.type = RightRelease;     break;
            case MiddlePress:
            case MiddleRelease:
            case MiddleClick:
                mEvent.type = MiddleRelease;    break;
            }

            // Now run mouse event that will release any possible remaining state!
            runMouseEvent(mEvent);
        }
        // If keyboard event that could leave state behind, we must attempt to erase that state!
        else if (   lastExecEvent.type == MacroEventType::KeyboardEvent
                 && lastExecEvent.keyboardEvent.type != MacroKeyboardEventType::KeyString)
        {
            MacroKeyboardEvent kEvent = lastExecEvent.keyboardEvent;
            kEvent.type = MacroKeyboardEventType::KeyRelease;
            runKeyboardEvent(kEvent);
        }
    }
}


void MacroActivator::interruptableSleepMs(int sleepMs) const
{
    // Sleeps in approximately 50ms chunks.
    while (sleepMs > 0 && _run) {
        QThread::msleep(sleepMs > 50 ? 50 : sleepMs);
        sleepMs -= 50;
    }
}
