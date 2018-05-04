#ifndef MACROEDITOREVENTLISTENER_H
#define MACROEDITOREVENTLISTENER_H


#include <QList>


/**
 * @brief The MacroEditorEventListener class
 * A pure virtual event listener class for Macro Event Editor UI events.
 */
class MacroEditorEventListener
{

public:

    /**
     * @brief getLatestMacroEvents
     * Gets the latest or most updated list of Macro Events from the Model.
     * @param macroEvents
     * The list of the latest Macro Events.
     * @return
     * The list of the latest Macro Events.
     */
    virtual QList<MacroEvent> getLatestMacroEvents() = 0;

    /**
     * @brief addEventsAt
     * Adds new Macro Events for all Macros being edited at a specified event index.
     * @param macroEventIndex
     * The index where the new Macro Events should be added.
     */
    virtual void insertEventsAt(int macroEventIndex) = 0;

    /**
     * @brief copyEvents
     * Copies a set of Macro Events in all Macros being edited. Copied events are placed immediately
     * after the source of the copies.
     * @param macroEventIndexes
     * The indexes of the Macro Events to copy.
     */
    virtual void copyEvents(QList<int> &macroEventIndexes) = 0;

    /**
     * @brief deleteEvents
     * Deletes a set of Macro Events in all Macros being edited.
     * @param macroEventIndexes
     * The indexes of the Macro Events to delete.
     */
    virtual void deleteEvents(QList<int> &macroEventIndexes) = 0;

    /**
     * @brief updateEventDelay
     * Updates the delay before a given Macro Event is exectued. The event is updated among all
     * of the Macros currently being edited.
     * @param macroEventIndex
     * The index of the event to update the delay for.
     * @param delayMs
     * The new delay for the event in milliseconds.
     */
    virtual void updateEventDelay(int macroEventIndex, int delayMs) = 0;

    /**
     * @brief updateEventDuration
     * Updates the duration for a Macro Event. The event is updated among all of the Macros currently
     * being edited.
     * @param macroEventIndex
     * The index of the event to update the duration for.
     * @param durationMs
     * The new duration for the event in milliseconds.
     */
    virtual void updateEventDuration(int macroEventIndex, int durationMs) = 0;

    /**
     * @brief updateEventKeyString
     * Updates the key string for a Macro Event. The event is updated among all of the Macros currently
     * being edited.
     * @param macroEventIndex
     * The index of the event to update the key string for.
     * @param keyString
     * The new key string for the event.
     */
    virtual void updateEventKeyString(int macroEventIndex, const QString &keyString) = 0;

    /**
     * @brief updateEventAutoCorrect
     * Updates the auto correct flag for a Macro Event. The event is updated among all of the Macros currently
     * being edited.
     * @param macroEventIndex
     * The index of the event to update the auto correct flag for.
     * @param autoCorrect
     * The new auto correct flag for the event.
     */
    virtual void updateEventAutoCorrect(int macroEventIndex, bool autoCorrect) = 0;

    /**
     * @brief hasUnsavedChanges
     * Checks if there exist any unsaved changes tracked in the model.
     * @return
     * True if unsaved changes exist, false if not.
     */
    virtual bool hasUnsavedChanges() = 0;

    /**
     * @brief saveEvents
     * Saves all changes to the Macro Events of the Macros that are being edited.
     */
    virtual void saveEvents() = 0;

    /**
     * @brief undoLastEventChange
     * Performs an undo of the last change to the events being edited.
     */
    virtual void undoLastEventChange() = 0;

    /**
     * @brief redoLastEventChange
     * Performs a redo of the last event edit that was undone.
     */
    virtual void redoLastEventChange() = 0;
};


#endif // MACROEDITOREVENTLISTENER_H
