#include "MacroEditorController.h"
#include "view/macro_menu/MacroMenu.h"
#include <QDebug>


MacroEditorController::MacroEditorController(GlobalHotKeyMonitor &hotKeyMonitor,
                                             IOLoggingController &ioLoggingController,
                                             MacroEventModel &macroEventModel)
    : Controller(),
      MacroEditorEventListener(),
      _macroEditor(*this),
      _macroEventEditProxy(macroEventModel),
      _ioLoggingController(ioLoggingController)
{
    connect(&_macroEditor, SIGNAL(closed()), this, SLOT(surrenderControlToParent()));
}


QList<MacroEvent> MacroEditorController::getLatestMacroEvents()
{
    return _macroEventEditProxy.getLatestMacroEvents();
}


void MacroEditorController::insertEventsAt(int macroEventIndex)
{
    // Give up control to the IO Logging Controller.
    _macroEditor.hide();
    _ioLoggingController.assumeControlFromParent(macroEventIndex, this);
}


void MacroEditorController::copyEvents(QList<int> &macroEventIndexes)
{
    _macroEventEditProxy.copyMacroEvents(macroEventIndexes);
    refreshView();
}


void MacroEditorController::deleteEvents(QList<int> &macroEventIndexes)
{
    _macroEventEditProxy.deleteMacroEvents(macroEventIndexes);
    refreshView();
}


void MacroEditorController::updateEventDelay(int macroEventIndex, int delayMs)
{
    _macroEventEditProxy.updateMacroEventDelay(macroEventIndex, delayMs);
}


void MacroEditorController::updateEventDuration(int macroEventIndex, int durationMs)
{
    _macroEventEditProxy.updateMacroEventDuration(macroEventIndex, durationMs);
}


void MacroEditorController::updateEventKeyString(int macroEventIndex, const QString &keyString)
{
    _macroEventEditProxy.updateMacroEventKeyString(macroEventIndex, keyString);
}


void MacroEditorController::updateEventAutoCorrect(int macroEventIndex, bool autoCorrect)
{
    _macroEventEditProxy.updateMacroEventAutoCorrect(macroEventIndex, autoCorrect);
}


bool MacroEditorController::hasUnsavedChanges()
{
    return _macroEventEditProxy.hasUnsavedChanges();
}


void MacroEditorController::saveEvents()
{
    _macroEventEditProxy.saveEvents();
}


void MacroEditorController::undoLastEventChange()
{
    _macroEventEditProxy.undoChange();
    refreshView();
}


void MacroEditorController::redoLastEventChange()
{
    _macroEventEditProxy.redoChange();
    refreshView();
}


void MacroEditorController::assumeControlFromParent(const QList<int> &macroIds, Controller *parentController)
{
    Controller::assumeControlFromParent(parentController);
    _macroEventEditProxy.setEditMacros(macroIds);

    QList<MacroEvent> macroEvents = _macroEventEditProxy.getLatestMacroEvents();
    _macroEditor.refresh(macroEvents, &macroIds);
}


void MacroEditorController::assumeControlFromChild(const SrcControllerInfo &srcControllerInfo)
{
    Controller::assumeControlFromChild(srcControllerInfo);

    // If we are coming back from IOLogging for new Macro Events.
    if (srcControllerInfo.controllerId == _ioLoggingController.CONTROLLER_ID) {
        // Get logged events, add them to the edit proxy, and refresh the view.
        QList<MacroEvent> addedEvents = _ioLoggingController.takeAddedMacroEvents();
        _macroEventEditProxy.insertMacroEvents(addedEvents);
        refreshView();
    }
}


void MacroEditorController::surrenderControlToParent(bool deactivateDueToError, const QString &errorMsg)
{
    _macroEditor.hide();
    // Make sure we refresh the proxy so all contained change/save logs are cleared so we do not reuse old state when editing later!
    _macroEventEditProxy.refresh();
    Controller::surrenderControlToParent(deactivateDueToError, errorMsg);
}


void MacroEditorController::refreshView()
{
    QList<MacroEvent> latestEvents = _macroEventEditProxy.getLatestMacroEvents();
    _macroEditor.refresh(latestEvents);
}
