#include "MacroActivationController.h"
#include <QDebug>


MacroActivationController::MacroActivationController(GlobalHotKeyMonitor &hotkeyMonitor, MacroEventModel &macroEventModel)
    : _hotkeyMonitor(hotkeyMonitor),
      _macroEventModel(macroEventModel),
      _macroActivator(),
      _loggingInfo()
{
    _loggingInfo.setHeader("Running Selected Macro\nStop Running: ctrl + w");
    qRegisterMetaType<MacroEvent>();
    connect(&_macroActivator, SIGNAL(activatingMacroEvent(MacroEvent)), this, SLOT(handleMacroEvent(MacroEvent)));
}


void MacroActivationController::assumeControlFromParent(int macroId, Controller *parentController)
{
    // Setup state.
    Controller::assumeControlFromParent(parentController);
    _macroEventModel.setActiveMacro(macroId);
    _loggingInfo.showNormal();

    // Connect to global hot key monitor and macro activator signals.
    connect(&_hotkeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(surrenderControlToParent()));
    connect(&_macroActivator, SIGNAL(macroActivatorStopped(bool, const QString&)),
            this, SLOT(surrenderControlToParent(bool, const QString&)));

    // Get the events to activate and run the macro activator with them.
    QList<MacroEvent> events = _macroEventModel.getUniformEvents();
    _macroActivator.runMacro(events);
}


void MacroActivationController::surrenderControlToParent(bool deactivateDueToError, const QString &errorMsg)
{
    _loggingInfo.close();
    _macroActivator.stopMacro();

    // Disconnect from global hot key monitor and macro activator signals.
    disconnect(&_hotkeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(surrenderControlToParent()));
    disconnect(&_macroActivator, SIGNAL(macroActivatorStopped(bool, const QString&)),
               this, SLOT(surrenderControlToParent(bool, const QString&)));

    Controller::surrenderControlToParent(deactivateDueToError, errorMsg);
}


void MacroActivationController::handleMacroEvent(const MacroEvent &event)
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
    qDebug() << "Updated event info!";
    _loggingInfo.update();
}
