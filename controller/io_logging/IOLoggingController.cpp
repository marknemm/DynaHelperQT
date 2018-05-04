#include "IOLoggingController.h"
#include <QDebug>


IOLoggingController::IOLoggingController(GlobalHotKeyMonitor &hotkeyMonitor) :
    Controller(),
    _hotkeyMonitor(hotkeyMonitor),
    _macroEventLogBuffer(),
    _loggingInfo(),
    _logger(IOLogger::generateLogger())
{}


QList<MacroEvent> IOLoggingController::takeAddedMacroEvents()
{
    // Give up ownership of the added events (which were not committed to the underlying database).
    return _macroEventLogBuffer.takeAddedEvents();
}


void IOLoggingController::assumeControlFromParent(int insertInd, Controller *parentController = nullptr)
{
    // Initialize controller state.
    Controller::assumeControlFromParent(parentController);
    _macroEventLogBuffer.setProxyInsertInd(insertInd);

    // Form connections with the logger and hot key monitor signals.
    connect(&_logger, SIGNAL(eventRecv(MacroEvent&)), this, SLOT(handleMacroEvent(MacroEvent&)));
    connect(&_hotkeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(surrenderControlToParent()));

    // Show the UI and activate the logger.
    _loggingInfo.show();
    _logger.activate();
}


void IOLoggingController::surrenderControlToParent(bool deactivateDueToError, const QString &errorMsg)
{
    // Deactivate logger and close UI.
    _logger.deactivate();
    _loggingInfo.close();

    // Disconnect from logger and hot key monitor signals.
    disconnect(&_logger, SIGNAL(eventRecv(MacroEvent&)), this, SLOT(handleMacroEvent(MacroEvent&)));
    disconnect(&_hotkeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(surrenderControlToParent()));

    Controller::surrenderControlToParent(deactivateDueToError, errorMsg);
}


void IOLoggingController::handleMacroEvent(MacroEvent &event)
{
    QString eventInfo;

    // Get event info string to display based off of event type.
    if (event.type == MacroEventType::MouseEvent) {
        eventInfo = getMacroMouseEventInfoStr(event.mouseEvent);
    }
    else {
        eventInfo = getMacroKeyboardEventInfoStr(event.keyboardEvent);
    }

    _loggingInfo.updateEventInfo(eventInfo);
    _macroEventLogBuffer.addEvent(event);
}
