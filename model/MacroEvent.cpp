#include "MacroEvent.h"
#include <QDebug>


MacroEvent::MacroEvent()
{
    type = MacroEventType::MouseEvent;
    index = -1;
    delayMs = 0;
    durationMs = 0;
    nRepeats = 0;
    targetPID = "";

    mouseEvent.type = MacroMouseEventType::Move;
    mouseEvent.cursorType = CURSOR_DEFAULT;
    mouseEvent.loc = QPoint(0, 0);
    mouseEvent.wheelDelta = 0;
    mouseEvent.screenshotId = -1;
    mouseEvent.screenshotRect = QRect(0, 0, 0, 0);
    mouseEvent.autoCorrect = true;

    keyboardEvent.type = MacroKeyboardEventType::KeyString;
    keyboardEvent.keyCode = -1;
    keyboardEvent.keyString = "";
    keyboardEvent.mod1 = -1;
    keyboardEvent.mod2 = -1;
    keyboardEvent.numLock = false;
    keyboardEvent.capsLock = false;
}


bool MacroEvent::operator < (const MacroEvent& event) const
{
    return (index < event.index);
}


void printMacroEvent(const MacroEvent &macroEvent)
{
    qDebug().noquote().nospace() << getMacroEventInfoStr(macroEvent);
}


QString getMacroEventInfoStr(const MacroEvent &macroEvent) {
    QString eventInfoStr = "";

    // General Event Info.
    eventInfoStr += "-- Event "    + QString::number(macroEvent.index)      + " --\n";
    eventInfoStr += "Event Type: " + getMacroEventTypeStr(macroEvent.type)  + "\n";
    eventInfoStr += "DelayMs:    " + QString::number(macroEvent.delayMs)    + "\n";
    eventInfoStr += "DurationMs: " + QString::number(macroEvent.durationMs) + "\n";
    eventInfoStr += "nRepeats:   " + QString::number(macroEvent.nRepeats)   + "\n";

    // Mouse Event Info.
    if (macroEvent.type == MacroEventType::MouseEvent) {
        eventInfoStr += getMacroMouseEventInfoStr(macroEvent.mouseEvent);
    }
    // Keyboard Event Info.
    else {
        eventInfoStr += getMacroKeyboardEventInfoStr(macroEvent.keyboardEvent);
    }

    return eventInfoStr;
}


QString getMacroMouseEventInfoStr(const MacroMouseEvent &mouseEvent)
{
    QString eventInfoStr = "";
    QString xLocStr = QString::number(mouseEvent.loc.x());
    QString yLocStr = QString::number(mouseEvent.loc.y());

    eventInfoStr += "   Mouse type:  "  + getMacroMouseEventTypeStr(mouseEvent.type) + "\n";
    eventInfoStr += "   Location:    (" + xLocStr + ", " + yLocStr                   + ")\n";
    eventInfoStr += "   Wheel Delta: "  + QString::number(mouseEvent.wheelDelta)     + "\n";

    return eventInfoStr;
}


QString getMacroKeyboardEventInfoStr(const MacroKeyboardEvent &keyboardEvent)
{
    QString eventInfoStr = "";

    eventInfoStr += "   Keyboard type: " + getMacroKeyboardEventTypeStr(keyboardEvent.type)                             + "\n";
    eventInfoStr += "   Key:           " + getKeyCodeStr(keyboardEvent.keyCode, keyboardEvent.mod1, keyboardEvent.mod2,
                                                         keyboardEvent.capsLock, keyboardEvent.numLock)                 + "\n";
    eventInfoStr += "   Mod 1:         " + getKeyCodeStr(keyboardEvent.mod1)                                            + "\n";
    eventInfoStr += "   Mod 2:         " + getKeyCodeStr(keyboardEvent.mod2)                                            + "\n";
    eventInfoStr += "   Caps Lock:     " + QString(keyboardEvent.capsLock ? "true" : "false")                           + "\n";
    eventInfoStr += "   Num Lock:      " + QString(keyboardEvent.numLock  ? "true" : "false")                           + "\n";
    eventInfoStr += "   Key string:    " + keyboardEvent.keyString                                                      + "\n";

    return eventInfoStr;
}


QString getMacroEventTypeStr(MacroEventType macroEventType)
{    
    switch (macroEventType) {
    case MouseEvent:    return "Mouse";
    case KeyboardEvent: return "Keyboard";
    default:            return "Error";
    }
}


QString getMacroMouseEventTypeStr(MacroMouseEventType macroMouseEventType)
{
    switch (macroMouseEventType) {
    case Move:          return "Move";
    case LeftPress:     return "Left Press";
    case LeftRelease:   return "Left Release";
    case LeftClick:     return "Left Click";
    case DoubleClick:   return "Double Click";
    case RightPress:    return "Right Press";
    case RightRelease:  return "Right Release";
    case RightClick:    return "Right Click";
    case MiddlePress:   return "Middle Press";
    case MiddleRelease: return "Middle Release";
    case MiddleClick:   return "Middle Click";
    case ScrollUp:      return "Scroll Up";
    case ScrollDown:    return "Scroll Down";
    default:            return "Error";
    }
}


QString getMacroKeyboardEventTypeStr(MacroKeyboardEventType macroKeyboardEventType)
{
    switch (macroKeyboardEventType) {
    case KeyPress:   return "Key Press";
    case KeyRelease: return "Key Release";
    case KeyType:    return "Key Type";
    case KeyString:  return "Key String";
    default:         return "Error";
    }
}

QString getKeyCodeStr(int keyCode, int mod1, int mod2, bool capsLock, bool numLock)
{
    bool shiftMod = (mod1 == Qt::Key_Shift || mod2 == Qt::Key_Shift);

    switch (keyCode)
    {
    case Qt::Key_Space:                         return " ";
    case Qt::Key_Tab:                           return "Tab";
    case Qt::Key_Backspace:                     return "Backspace";
    case Qt::Key_Escape:                        return "Escape";
    case Qt::Key_Return:                        return "Enter";
    case Qt::Key_CapsLock:                      return "Caps Lock";
    case Qt::Key_Print:                         return "Print Screen";
    case Qt::Key_Delete:                        return "Delete";
    case Qt::Key_Home:                          return "Home";
    case Qt::Key_End:                           return "End";
    case Qt::Key_PageUp:                        return "Page Up";
    case Qt::Key_PageDown:                      return "Page Down";
    case Qt::Key_NumLock:                       return "Num Lock";
    case Qt::Key_Plus | Qt::KeypadModifier:     return "+";
    case Qt::Key_Minus | Qt::KeypadModifier:    return "-";
    case Qt::Key_Asterisk | Qt::KeypadModifier: return "*";
    case Qt::Key_Slash | Qt::KeypadModifier:    return "/";
    case Qt::Key_Period | Qt::KeypadModifier:   return numLock ? "." : "Delete";
    case Qt::Key_0 | Qt::KeypadModifier:        return numLock ? "0" : "Insert";
    case Qt::Key_1 | Qt::KeypadModifier:        return numLock ? "1" : "End";
    case Qt::Key_2 | Qt::KeypadModifier:        return numLock ? "2" : "Down";
    case Qt::Key_3 | Qt::KeypadModifier:        return numLock ? "3" : "Page Down";
    case Qt::Key_4 | Qt::KeypadModifier:        return numLock ? "4" : "Left";
    case Qt::Key_5 | Qt::KeypadModifier:        return numLock ? "5" : "N/A";
    case Qt::Key_6 | Qt::KeypadModifier:        return numLock ? "6" : "Right";
    case Qt::Key_7 | Qt::KeypadModifier:        return numLock ? "7" : "Home";
    case Qt::Key_8 | Qt::KeypadModifier:        return numLock ? "8" : "Up";
    case Qt::Key_9 | Qt::KeypadModifier:        return numLock ? "9" : "Page Down";
    case Qt::Key_Up:                            return QString((char)30);
    case Qt::Key_Down:                          return QString((char)31);
    case Qt::Key_Left:                          return QString((char)17);
    case Qt::Key_Right:                         return QString((char)16);
    case Qt::Key_Comma:                         return shiftMod ? "<" : ",";
    case Qt::Key_Period:                        return shiftMod ? ">" : ".";
    case Qt::Key_Minus:                         return shiftMod ? "_" : "-";
    case Qt::Key_Equal:                         return shiftMod ? "+" : "=";
    case Qt::Key_Semicolon:                     return shiftMod ? ":" : ";";
    case Qt::Key_Slash:                         return shiftMod ? "?" : "/";
    case Qt::Key_QuoteLeft:                     return shiftMod ? "~" : "`";
    case Qt::Key_BraceLeft:                     return shiftMod ? "{" : "[";
    case Qt::Key_Backslash:                     return shiftMod ? "|" : "\\";
    case Qt::Key_BraceRight:                    return shiftMod ? "}" : "]";
    case Qt::Key_QuoteDbl:                      return shiftMod ? "\"" : "'";
    case Qt::Key_F1:                            return "F1";
    case Qt::Key_F2:                            return "F2";
    case Qt::Key_F3:                            return "F3";
    case Qt::Key_F4:                            return "F4";
    case Qt::Key_F5:                            return "F5";
    case Qt::Key_F6:                            return "F6";
    case Qt::Key_F7:                            return "F7";
    case Qt::Key_F8:                            return "F8";
    case Qt::Key_F9:                            return "F9";
    case Qt::Key_F10:                           return "F10";
    case Qt::Key_F11:                           return "F11";
    case Qt::Key_F12:                           return "F12";
    case Qt::Key_F13:                           return "F13";
    case Qt::Key_F14:                           return "F14";
    case Qt::Key_F15:                           return "F15";
    case Qt::Key_F16:                           return "F16";
    case Qt::Key_F17:                           return "F17";
    case Qt::Key_F18:                           return "F18";
    case Qt::Key_F19:                           return "F19";
    case Qt::Key_F20:                           return "F20";
    case Qt::Key_F21:                           return "F21";
    case Qt::Key_F22:                           return "F22";
    case Qt::Key_F23:                           return "F23";
    case Qt::Key_F24:                           return "F24";
    case Qt::Key_Shift:                         return "Shift";
    case Qt::Key_Alt:                           return "Alt";
    case Qt::Key_Control:                       return "Ctrl";
    case Qt::Key_0:                             return shiftMod ? ")" : "0";
    case Qt::Key_1:                             return shiftMod ? "!" : "1";
    case Qt::Key_2:                             return shiftMod ? "@" : "2";
    case Qt::Key_3:                             return shiftMod ? "#" : "3";
    case Qt::Key_4:                             return shiftMod ? "$" : "4";
    case Qt::Key_5:                             return shiftMod ? "%" : "5";
    case Qt::Key_6:                             return shiftMod ? "^" : "6";
    case Qt::Key_7:                             return shiftMod ? "&" : "7";
    case Qt::Key_8:                             return shiftMod ? "*" : "8";
    case Qt::Key_9:                             return shiftMod ? "(" : "9";
    default:
        if (keyCode >= 'A' && keyCode <= 'Z') {
            return QString(shiftMod || capsLock ? (char)keyCode : (char)(keyCode + 32)); // [A, Z].
        }
    }

    return "N/A";
}


bool isKeyCodeChar(const MacroKeyboardEvent &kEvent)
{
    if (   kEvent.mod1 == Qt::Key_Alt
        || kEvent.mod1 == Qt::Key_Control
        || kEvent.mod2 == Qt::Key_Alt
        || kEvent.mod2 == Qt::Key_Control)
    {
        return false;
    }

    // It is a non-alphabetic character.
    switch (kEvent.keyCode)
    {
    case Qt::Key_Space:
    case Qt::Key_Backspace:
    //case Qt::Key_Tab: Not a character because more often used to traverse form elements!
    case Qt::Key_Plus | Qt::KeypadModifier:
    case Qt::Key_Minus | Qt::KeypadModifier:
    case Qt::Key_Asterisk | Qt::KeypadModifier:
    case Qt::Key_Slash | Qt::KeypadModifier:
    case Qt::Key_Comma:
    case Qt::Key_Period:
    case Qt::Key_Minus:
    case Qt::Key_Equal:
    case Qt::Key_Semicolon:
    case Qt::Key_Slash:
    case Qt::Key_QuoteLeft:
    case Qt::Key_BraceLeft:
    case Qt::Key_Backslash:
    case Qt::Key_BraceRight:
    case Qt::Key_QuoteDbl:
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        return true;
    }

    // It is a numpad character.
    if (kEvent.numLock) {
        switch (kEvent.keyCode)
        {
        case Qt::Key_Period | Qt::KeypadModifier:
        case Qt::Key_0 | Qt::KeypadModifier:
        case Qt::Key_1 | Qt::KeypadModifier:
        case Qt::Key_2 | Qt::KeypadModifier:
        case Qt::Key_3 | Qt::KeypadModifier:
        case Qt::Key_4 | Qt::KeypadModifier:
        case Qt::Key_5 | Qt::KeypadModifier:
        case Qt::Key_6 | Qt::KeypadModifier:
        case Qt::Key_7 | Qt::KeypadModifier:
        case Qt::Key_8 | Qt::KeypadModifier:
        case Qt::Key_9 | Qt::KeypadModifier:
            return true;
        }
    }

    // It is an alphabetic character.
    return (kEvent.keyCode >= 'A' && kEvent.keyCode <= 'Z');
}


bool isKeyCodeAssocWithNumpadOff(int keyCode)
{
    switch (keyCode) {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Delete:
    case Qt::Key_PageDown:
    case Qt::Key_PageUp:
    case Qt::Key_Home:
    case Qt::Key_End:
        return true;
    default:
        return false;
    }
}


QList<int> getEventIndListFromMacroEventList(const QList<MacroEvent> &macroEvents)
{
    QList<int> eventInds;
    eventInds.reserve(macroEvents.size());
    foreach (MacroEvent event, macroEvents) {
        eventInds.append(event.index);
    }
    return eventInds;
}
