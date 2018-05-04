#ifndef MACROEVENTEDITPROXY_H
#define MACROEVENTEDITPROXY_H


#include "model/MacroEvent.h"
#include "model/MacroEventModel.h"
#include "MacroEventEdit.h"
#include "util/ChangeLog.hpp"
#include <QList>


/**
 * @brief The MacroEventEditProxy class
 * Proxy for model involved in Macro Event Edit. Caches Macro Events being edited in memory.
 * Caches undo and redo events for the editor. Responsible for comitting saves/writes to underlying model.
 */
class MacroEventEditProxy
{

private:

    /**
     * @brief _macroEventModel
     * Macro Event Model that handles direct reads/writes from/to underlying database.
     */
    MacroEventModel &_macroEventModel;

    /**
     * @brief _uniformEventInds
     * The indexes of the uniform events i
     */
    QList<int> _uniformEventInds;

    /**
     * @brief _macroEvents
     * List of Macro Events being edited. Will hold all the latest effective edits.
     * Will be committed to the underlying database upon a save request from the user.
     */
    QList<MacroEvent> _macroEvents;

    /**
     * @brief _changeLog
     * The change log for edits done on Macro Events. Used to implement undo and redo.
     * Also, used to save the pending changes made to the underlying database.
     */
    ChangeLog<MacroEventEdit> _changeLog;


public:

    explicit MacroEventEditProxy(MacroEventModel &macroEventModel);

    /**
     * @brief setEditMacros
     * Sets the Macros to be edited.
     * @param editMacroIds
     * The IDs of the Macros to be edited.
     */
    void setEditMacros(const QList<int> &editMacroIds);


    /**
     * @brief getActiveMacroIds
     * Gets the active Macro IDs that are currently set in the underlying model.
     * @return
     * The active Macro IDs.
     */
    QList<int> getActiveMacroIds() const;


    /**
     * @brief getLatestMacroEvents
     * Gets the latest Macro Events among the Macros being edited complete with all effective edits.
     * @return
     * The list of latest Macro Events.
     */
    QList<MacroEvent> getLatestMacroEvents() const;


    /**
     * @brief insertMacroEvents
     * Inserts new Macro Events for the Macros being edited.
     * @param macroEvents
     * The Macro Events to be inserted. First event contains begin Macro Event index insert spot.
     */
    void insertMacroEvents(const QList<MacroEvent> &macroEvents);


    /**
     * @brief copyMacroEvents
     * Copies Macro Events with the given Macro Event indexes.
     * @param macroEventIndexes
     * The indexes of the Macro Events to copy. The copies are placed immediately after the source Macro Events.
     * This list will be sorted internally.
     */
    void copyMacroEvents(QList<int> &macroEventIndexes);


    /**
     * @brief deleteMacroEvents
     * Deletes Macro Events with the given Macro Event indexes.
     * @param macroEventIndexes
     * The indexes of the Macro Events to delete. This list will be sorted internally.
     */
    void deleteMacroEvents(QList<int> &macroEventIndexes);


    /**
     * @brief updateMacroEventDelay
     * Updates the delay for a Macro Event.
     * @param macroEventIndex
     * The index of the Macro Event to update.
     * @param delay
     * The new delay in milliseconds.
     */
    void updateMacroEventDelay(int macroEventIndex, int  delay);


    /**
     * @brief updateMacroEventDuration
     * Updates the duration of a Macro Event.
     * @param macroEventIndex
     * The index of the Macro Event to update.
     * @param duration
     * The new duration in milliseconds.
     */
    void updateMacroEventDuration(int macroEventIndex, int duration);


    /**
     * @brief updateMacroEventKeyString
     * Updates the key string of a Macro Event.
     * @param macroEventIndex
     * The index of the Macro Event to update.
     * @param keyString
     * The new key string.
     */
    void updateMacroEventKeyString(int macroEventIndex, const QString &keyString);


    /**
     * @brief updateMacroEventAutoCorrect
     * Updates the auto correct flag of a Macro Event.
     * @param macroEventIndex
     * The index of the Macro Event to update.
     * @param autoCorrect
     * The new auto correct flag used to determine if computer vision techniques should be used to fix location sensitive events.
     */
    void updateMacroEventAutoCorrect(int macroEventIndex, bool autoCorrect);


    /**
     * @brief hasUnsavedChanges
     * Checks if there exist any unsaved changes.
     * @return
     * True if unsaved changes exist. False if not.
     */
    bool hasUnsavedChanges();


    /**
     * @brief saveEvents
     * Commits the Macro Events to the underlying model with all of their updates up to the current change.
     */
    void saveEvents();


    /**
     * @brief undoChange
     * Performs an undo, reversing the latest Macro Event edit.
     */
    void undoChange();


    /**
     * @brief redoChange
     * Performs a redo, re-instating the latest undone Macro Event.
     */
    void redoChange();


    /**
     * @brief refresh
     * Refreshes the state of the Macro Event Edit Proxy by clearing out any internal state.
     * This is important to use before fresh use of the Macro Event Edit Proxy so we are not including changes from previous use!
     */
    void refresh();


private:

    /**
     * @brief getMacroEventIndexesFromMacroEvents
     * Gets a list of the event indexes of the events in a given list of Macro Events.
     * @param events
     * The Macro Events to get the event indexes of.
     * @return
     * A list of the Macro Events' indexes.
     */
    QList<int> getMacroEventIndexesFromMacroEvents(const QList<MacroEvent> &events) const;

    /**
     * @brief incMacroEventIndexes
     * Increments Macro Event indexes of Macro Events that fall within range of the arguments by a given amount.
     * @param beginListIndex
     * The list index from which we must increment all Macro Event indexes.
     * @param incAmt
     * The amount to increment the Macro Event indexes. If negative, a decrement will happen instead.
     * @param endListIndex (OPTIONAL)
     * The list index which we will stop incrementing at (won't increment at this index).
     * Default is up to the last index in the list.
     */
    void incMacroEventIndexes(int beginListIndex, int incAmt, int endListIndex = -1);
};


#endif // MACROEVENTEDITPROXY_H
