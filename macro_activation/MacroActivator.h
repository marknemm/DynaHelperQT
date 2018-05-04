#ifndef MACROACTIVATOR_H
#define MACROACTIVATOR_H


#include <QObject>
#include "model/MacroEvent.h"


/**
 * @brief The MacroActivator class
 * Activates a Macro.
 */
class MacroActivator : public QObject
{
    Q_OBJECT

public:

    explicit MacroActivator();

    /**
     * @brief runMacro
     * Runs a given Macro record.
     * @param macroEvents A list of the Macro Events associated with the Macro record to run.
     */
    void runMacro(const QList<MacroEvent> &macroEvents);

    /**
     * @brief stopMacro
     * Interrupts and stops a running Macro record.
     */
    void stopMacro();

signals:

    /**
     * @brief activatingMacroEvent
     * Gives the Macro Event that is currently being activated.
     * @param event
     * The Macro Event that is being activated.
     */
    void activatingMacroEvent(const MacroEvent &event);

    /**
     * @brief macroActivatorStopped
     * Signal that is emitted whenever the Macro Activator has cleaned up after its activation of a Macro
     * and completely finished with execution.
     * @param error
     * Flag that signifies if the Macro Activator stopped due to an internal error. True if error occured, false if not.
     * @param errorMsg
     * The message associated with the error if there is one. Otherwise, an empty string.
     */
    void macroActivatorStopped(bool error, const QString &errorMsg);

private:

    /**
     * @brief _run
     * A flag to be set true if running Macro should continue.
     * If set false, then will forcibly stop the macro.
     */
    bool _run;

    /**
     * @brief runMacroInOwnThread
     * This will be invoked in its own separate thread so that the main event thread isn't
     * hogged by the loop inside. It will run the Macro.
     * @param macroEvents The Macro Events of the Macro to run.
     */
    void runMacroInOwnThread(QList<MacroEvent> &macroEvents);

    /**
     * @brief runMouseEvent
     * Runs a Macro Mouse Event.
     * @param mEvent The Macro Mouse Event.
     */
    void runMouseEvent(const MacroMouseEvent &mEvent) const;
    /**
     * @brief runKeyboardEvent
     * Runs a Macro Keyboard Event.
     * @param kEvent The Macro Keyboard Event.
     */
    void runKeyboardEvent(const MacroKeyboardEvent &kEvent) const;

    /**
     * @brief moveMouseToPos
     * Moves the mouse to the specified position in screen coordinates.
     * @param x The x-cooridinate.
     * @param y The y-cooridinate.
     */
    void moveMouseToPos(int x, int y) const;
    /**
     * @brief setMousePos
     * Sets the mouse position to absolute screen coordinates x and y.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     */
    void setMousePos(int x, int y) const;

    /**
     * @brief leftPress
     * Executes a left mouse press event.
     */
    void leftPress() const;
    /**
     * @brief leftRelease
     * Executes a left mouse release event.
     */
    void leftRelease() const;

    /**
     * @brief rightPress
     * Executes a right mouse press event.
     */
    void rightPress() const;
    /**
     * @brief rightRelease
     * Executes a right mouse release event.
     */
    void rightRelease() const;

    /**
     * @brief middlePress
     * Executes a middle mouse press event.
     */
    void middlePress() const;
    /**
     * @brief middleRelease
     * Executes a middle mouse release event.
     */
    void middleRelease() const;

    /**
     * @brief scrollUp
     * Executes a mouse scroll up event.
     */
    void scrollUp() const;
    /**
     * @brief scrollDown
     * Executes a mouse scroll down event.
     */
    void scrollDown() const;

    /**
     * @brief keyString
     * Executes a key string event. Types out a string of characters.
     * @param qText The key string to type out.
     */
    void keyString(const QString &qText) const;

    /**
     * @brief keyPress
     * Executes a keyboard press event.
     * @param kEvent The keyboard press event details. Will include the key code and any key modifiers.
     */
    void keyPress(const MacroKeyboardEvent &kEvent) const;
    /**
     * @brief keyRelease
     * Executes a keyboard release event.
     * @param kEvent The keyboard release event details. Will include the key code and any key modifiers.
     */
    void keyRelease(const MacroKeyboardEvent &kEvent) const;

    /**
     * @brief mapQtVkToWinVk
     * Maps a QT virtual key code to a Windows virtual key code.
     * @param qtVk The QT virtual key code.
     * @return The Windows virtual key code.
     */
    int mapQtVkToWinVk(int qtVk) const;

    /**
     * @brief applyKeyMods
     * Apply all keyboard mods necessary for execution of the event.
     * @param kEvent The keyboard event to apply mods for.
     */
    void applyKeyMods(const MacroKeyboardEvent &kEvent) const;
    /**
     * @brief removeKeyMods
     * Remove all keyboard mods associated with the execution of an event.
     * @param kEvent The keyboard event to remove the mods after.
     */
    void removeKeyMods(const MacroKeyboardEvent &kEvent) const;

    /**
     * @brief cleanupMacro
     * Cleans up any left behind Mouse and Keyboard State for the Macro when it is either automatically finished
     * with execution or forcefully finished (by either user or exception).
     * @param macroEvents The list of Macro Events that should have been fully executed.
     * @param lastEventInd The index of the last event to execute. This event may
     *                     or may not have completed fully.
     */
    void cleanupMacro(const QList<MacroEvent> &macroEvents, int lastEventInd) const;

    /**
     * @brief interruptableSleepMs
     * Sleeps for the given amount of milliseconds. The sleep is in short increments
     * so that if the user interrupts the Macro Activation, a long sleep will not
     * continue.
     * @param sleepMs The sleep time in milliseconds.
     */
    void interruptableSleepMs(int sleepMs) const;
};


#endif // MACROACTIVATOR_H
