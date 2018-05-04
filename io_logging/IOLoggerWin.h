#ifndef IOLOGGERWIN_H
#define IOLOGGERWIN_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)


#include "IOLoggerPlatform.h"
#include <windows.h>
#pragma comment(lib, "user32.lib")


/**
 * @brief The IOLoggerWin class
 * Contains functionality for global logging of keyboard and mouse events.
 */
class IOLoggerWin : public IOLoggerPlatform
{
public:

    IOLoggerWin();
    ~IOLoggerWin();

    /**
     * @brief activate
     * Activates the keyboard and mouse logger.
     */
    void activate() override;
    /**
     * @brief deactivate
     * Deactivates the keyboard and mouse logger.
     */
    void deactivate() override;

    /**
     * @brief keyboardProc
     * The global keyboard event listener procedure to be called upon the activation of any keyboard event.
     * @param nCode Code to determine if procedure should process the message or ignore it.
     * @param wParam The state of the key event (WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP).
     * @param lParam Pointer to KBDLLHOOKSTRUCT struct. Basically, contains the key code information.
     * @return Just return value from CallNextHookEx() so event is processed correctly in each application!
     */
    static LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    /**
     * @brief mouseProc
     * The global mouse event listener procedure to be called upon the activateion of any mouse event.
     * @param nCode Code to determine if procedure should process the message or ignore it.
     * @param wParam The state of the mouse event (WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE, WM_MOUSEWHEEL,
     *                                             WM_MOUSEHWHEEL, WM_RBUTTONDOWN, or WM_RBUTTONUP).
     * @param lParam Pointer to MSLLHOOKSTRUCT struct. Basically, contains the mouse event location information.
     * @return Just return value from CallNextHookEx() so event is processed correctly in each application!
     */
    static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    /**
     * @brief _hKeyHook
     * The global keyboard hook procedure handle.
     */
    static HHOOK _hKeyHook;
    /**
     * @brief _hMouseHook
     * The global mouse hook procedure handle.
     */
    static HHOOK _hMouseHook;

    /**
     * @brief fillMacroEventFromKeyboard
     * Fills a macro event with given data from a keyboard event.
     * @param event The event to fill.
     * @param type The type of the keyboard event.
     * @param keyCode The key code of the keyboard key of interest.
     */
    static void fillMacroEventFromKeyboard(MacroEvent &event, WPARAM type, DWORD keyCode);
    /**
     * @brief fillMacroEventFromMouse
     * Fills a macro event with given data from a mouse event.
     * @param event The event to fill.
     * @param type The type of the mouse event.
     * @param loc The location of the event.
     * @param wheelDelta A mouse wheel delta value of the event.
     */
    static void fillMacroEventFromMouse(MacroEvent &event, WPARAM type, const POINT &loc, WORD wheelDelta);
};


#endif // WINDOWS CHECK


#endif // IOLOGGERWIN_H
