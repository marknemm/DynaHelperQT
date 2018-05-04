#ifndef MACROEVENT_H
#define MACROEVENT_H


#include <QString>
#include <QPoint>
#include <QRect>
#include <QList>
#include <QImage>


enum MacroMouseEventType {
    Move,
    LeftPress,
    LeftRelease,
    LeftClick,
    DoubleClick,
    RightPress,
    RightRelease,
    RightClick,
    MiddlePress,
    MiddleRelease,
    MiddleClick,
    ScrollUp,
    ScrollDown
};


enum MacroKeyboardEventType {
    KeyPress,
    KeyRelease,
    KeyType,
    KeyString
};


enum MacroEventType {
    MouseEvent,
    KeyboardEvent,
    DummyEvent
};


enum TargetImgType {
    TARGET_IMG_UNDEF,
    TARGET_IMG_TEXT,
    TARGET_IMG_FILTER_TEXT,
    TARGET_IMG_CONTROL
};


enum CursorType {
    CURSOR_IBEAM,
    CURSOR_HAND,
    CURSOR_DEFAULT
};


typedef struct MacroMouseEvent
{
    MacroMouseEventType type;
    QPoint loc;
    int wheelDelta;
    int screenshotId;
    QImage screenshotImg;
    QRect screenshotRect;
    TargetImgType targetImgType;
    CursorType cursorType;
    bool autoCorrect;
} MacroMouseEvent;


typedef struct MacroKeyboardEvent
{
    MacroKeyboardEventType type;
    int mod1;
    int mod2;
    bool capsLock;
    bool numLock;
    int keyCode;
    QString keyString;
} MacroKeyboardEvent;


typedef struct MacroEvent
{
    explicit MacroEvent(); // Default constructor to "zero" initialize everything.
    int index;
    int delayMs;
    int durationMs;
    int nRepeats;
    int timestampMs;
    QString targetPID;
    MacroEventType type;
    MacroMouseEvent mouseEvent;
    MacroKeyboardEvent keyboardEvent;

    bool operator < (const MacroEvent& event) const;
} MacroEvent;

/**
 * @brief printMacroEvent
 * Prints the contents of Macro Events.
 * @param macroEvents The Macro Event to print the contents of.
 */
void printMacroEvent(const MacroEvent &macroEvent);

/**
 * @brief getMacroEventInfoStr
 * Gets a string containing all information pertaining to a given Macro Event.
 * @param macroEvent The Macro Event to get the info string for.
 * @return The Macro Event information string.
 */
QString getMacroEventInfoStr(const MacroEvent &macroEvent);

/**
 * @brief getMacroMouseEventInfoStr
 * Gets a string containing all information pertaining to a given Macro Mouse Event.
 * @param mouseEvent The Macro Mouse Event to get the info string for.
 * @return The Macro Mouse Event information string.
 */
QString getMacroMouseEventInfoStr(const MacroMouseEvent &mouseEvent);

/**
 * @brief getMacroKeyboardEventInfoStr
 * Gets a string containing all information pertaining to a given Macro Keyboard Event.
 * @param keyboardEvent The Macro Keyboard Event to get the info string for.
 * @return The Macro Keyboard Event information string.
 */
QString getMacroKeyboardEventInfoStr(const MacroKeyboardEvent &keyboardEvent);

/**
 * @brief getMacroEventTypeStr
 * Gets the readable string version of the Macro Event Type enum value.
 * @param macroEventType The Macro Event Type enum value.
 * @return The Macro Event Type string.
 */
QString getMacroEventTypeStr(MacroEventType macroEventType);

/**
 * @brief getMacroMouseEventTypeStr
 * Gets the readable string version of the Macro Mouse Event Type enum value.
 * @param macroMouseEventType The Macro Mouse Event Type enum value.
 * @return The Macro Mouse Event Type string.
 */
QString getMacroMouseEventTypeStr(MacroMouseEventType macroMouseEventType);

/**
 * @brief getMacroKeyboardEventTypeStr
 * Gets the readable string version of the Macro Keyboard Event Type enum value.
 * @param macroKeyboardEventType The Macro Keyboard Event Type enum value.
 * @return The Macro Keyboard Event Type string.
 */
QString getMacroKeyboardEventTypeStr(MacroKeyboardEventType macroKeyboardEventType);

/**
 * @brief getKeyCodeStr
 * Gets the human readable string version of an integer key code.
 * @param keyCode The integer key code.
 * @param mod1 (OPTIONAL) The first modifier key possibly associated with the given keyCode.
 * @param mod2 (OPTIONAL) The second modifier key possibly associated with the given keyCode.
 * @param capsLock (OPTIONAL) Set true if the caps lock key modifier is active.
 * @param numLock (OPTIONAL) Set true if the num lock key modifier is active.
 * @return The string version of the key code.
 */
QString getKeyCodeStr(int keyCode, int mod1=-1, int mod2=-1, bool capsLock=false, bool numLock=false);

/**
 * @brief isKeyCodeChar
 * Determines if a key code corresponds to a char (or rather, a key, that when typed will show up in a word editor).
 * @param kEvent The keyboard event to inspect to see if its keyCode member is a char.
 * @return True if it corresponds to a char, false if it does not.
 */
bool isKeyCodeChar(const MacroKeyboardEvent &kEvent);


/**
 * @brief isKeyCodeAssocWithNumpadOff
 * Determines if a given key code is associated with working when the NUM PAD modifier is turned off.
 * @param keyCode The key code to check.
 * @return True if NUM PAD must be off for the key code to function correctly, false if not.
 */
bool isKeyCodeAssocWithNumpadOff(int keyCode);


/**
 * @brief getEventIndListFromMacroEventList
 * Gets a list of Macro Event indexes from a given list of Macro Events.
 * @param macroEvents The list of Macro Events to get the indexes from.
 * @return The list of Macro Event indexes.
 */
QList<int> getEventIndListFromMacroEventList(const QList<MacroEvent> &macroEvents);


#endif // MACROEVENT_H
