#ifndef MACROEDITORCONTROLLER_H
#define MACROEDITORCONTROLLER_H


#include "controller/Controller.h"
#include "hotkey/GlobalHotKeyMonitor.h"
#include "view/macro_editor/MacroEditor.h"
#include "model/MacroEventModel.h"
#include "model/MacroEvent.h"
#include "model/proxy/MacroEventEditProxy.h"
#include "MacroEditorEventListener.h"
#include "controller/io_logging/IOLoggingController.h"
#include <QList>


/**
 * @brief The MacroEditorController class
 * Controller for the Macro Editor.
 */
class MacroEditorController : public Controller, MacroEditorEventListener
{
    Q_OBJECT

private:

    /**
     * @brief _macroEditor
     * The Macro Editor View.
     */
    MacroEditor _macroEditor;

    /**
     * @brief _macroEventEditProxy
     * A Proxy for transfering edit actions on Macro Events to the underlying database. Will store not yet saved changes.
     * Will also store undo and redo logs.
     */
    MacroEventEditProxy _macroEventEditProxy;

    /**
     * @brief _ioLoggingController
     * Controller for IOLogging (Macro Event logging).
     */
    IOLoggingController &_ioLoggingController;


public:

    explicit MacroEditorController(GlobalHotKeyMonitor &hotKeyMonitor,
                                   IOLoggingController &ioLoggingController,
                                   MacroEventModel &macroEventModel);


    /**
     * @brief getLatestMacroEvents
     * Gets the latest or most updated list of Macro Events from the Model.
     * @param macroEvents
     * The list of the latest Macro Events.
     */
    QList<MacroEvent> getLatestMacroEvents() override;

    /**
     * @brief addEventsAt
     * Adds new Macro Events for all Macros being edited at a specified event index.
     * @param macroEventIndex
     * The index where the new Macro Events should be added.
     */
    void insertEventsAt(int macroEventIndex) override;

    /**
     * @brief copyEvents
     * Copies a set of Macro Events in all Macros being edited. Copied events are placed immediately
     * after the source of the copies.
     * @param macroEventIndexes
     * The indexes of the Macro Events to copy.
     */
    void copyEvents(QList<int> &macroEventIndexes) override;

    /**
     * @brief deleteEvents
     * Deletes a set of Macro Events in all Macros being edited.
     * @param macroEventIndexes
     * The indexes of the Macro Events to delete.
     */
    void deleteEvents(QList<int> &macroEventIndexes) override;

    /**
     * @brief updateEventDelay
     * Updates the delay before a given Macro Event is exectued. The event is updated among all
     * of the Macros currently being edited.
     * @param macroEventIndex
     * The index of the event to update the delay for.
     * @param delayMs
     * The new delay for the event in milliseconds.
     */
    void updateEventDelay(int macroEventIndex, int delayMs) override;

    /**
     * @brief updateEventDuration
     * Updates the duration for a Macro Event. The event is updated among all of the Macros currently
     * being edited.
     * @param macroEventIndex
     * The index of the event to update the duration for.
     * @param durationMs
     * The new duration for the event in milliseconds.
     */
    void updateEventDuration(int macroEventIndex, int durationMs) override;

    /**
     * @brief updateEventKeyString
     * Updates the key string for a Macro Event. The event is updated among all of the Macros currently
     * being edited.
     * @param macroEventIndex
     * The index of the event to update the key string for.
     * @param keyString
     * The new key string for the event.
     */
    void updateEventKeyString(int macroEventIndex, const QString &keyString) override;

    /**
     * @brief updateEventAutoCorrect
     * Updates the auto correct flag for a Macro Event. The event is updated among all of the Macros currently
     * being edited.
     * @param macroEventIndex
     * The index of the event to update the auto correct flag for.
     * @param autoCorrect
     * The new auto correct flag for the event.
     */
    void updateEventAutoCorrect(int macroEventIndex, bool autoCorrect) override;

    /**
     * @brief hasUnsavedChanges
     * Checks if there exist any unsaved changes tracked in the model.
     * @return
     * True if unsaved changes exist, false if not.
     */
    bool hasUnsavedChanges() override;

    /**
     * @brief saveEvents
     * Saves all changes to the Macro Events of the Macros that are being edited.
     */
    void saveEvents() override;

    /**
     * @brief undoLastEventChange
     * Performs an undo of the last change to the events being edited.
     */
    void undoLastEventChange() override;

    /**
     * @brief redoLastEventChange
     * Performs a redo of the last event edit that was undone.
     */
    void redoLastEventChange() override;


public slots:

    /**
     * @brief assumeControlFromParent
     * Should be invoked whenever this controller is granted control by a parent controller.
     * @param macroIds
     * A list of the IDs of the Macros that are being edited.
     * @param parentController
     * The parent controller which will resume control whe this controller surrenders control.
     */
    void assumeControlFromParent(const QList<int> &macroIds, Controller *parentController);

    /**
     * @brief assumeControlFromChild
     * Should be invoked whenever a controller assumes control from a child controller and becomes active.
     * @param srcControllerInfo (OPTIONAL)
     * Info pertaining to the source child controller that caused the activation of this controller. See definition of SrcControllerInfo
     * struct for more information.
     */
    void assumeControlFromChild(const SrcControllerInfo &srcControllerInfo) override;


private slots:

    /**
     * @brief surrenderControlToParent
     * Called whenever this controller wishes to surrender control to the parent.
     * @param deactivateDueToError (OPTIONAL)
     * Set to true if an error occured which caused this method to be called. Otherwise use default of false for no error.
     * @param errorMsg (OPTIONAL)
     * Set to a brief description of the error that occured if one did occur. Otherwise, leave empty by default.
     */
    void surrenderControlToParent(bool deactivateDueToError = false, const QString &errorMsg = "") override;


private:

    /**
     * @brief refreshView
     * Refreshes the editor view.
     */
    void refreshView();
};


#endif // MACROEDITORCONTROLLER_H
