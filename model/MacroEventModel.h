#ifndef MACROEVENTMODEL_H
#define MACROEVENTMODEL_H


#include "Model.h"
#include "MacroEvent.h"
#include <QList>


/**
 * @brief The MacroEventModel class
 * Model for Macro Events.
 */
class MacroEventModel : public Model
{
public:

    explicit MacroEventModel();

    /**
     * @brief setActiveMacro
     * Sets the active Macro. Active Macros are the Macros that Macro Events will be read and written to.
     * @param activeMacroId
     * The ID of the new active Macro.
     */
    void setActiveMacro(int activeMacroId);

    /**
     * @brief setActiveMacros
     * Sets the active Macro(s). Active Macros are the Macros that Macro Events will be read and written to.
     * @param activeMacroIds
     * A list of the ID(s) of the new active Macro(s).
     */
    void setActiveMacros(const QList<int> &activeMacroIds);

    /**
     * @brief getActiveMacroIds
     * Gets the IDs of the currently active Macros.
     * @return
     * The currently active Macros' IDs.
     */
    QList<int> getActiveMacroIds() const;


    /**
     * @brief getNumEventsForMacro
     * Gets the number of Macro Events that a given Macro contains.
     * @param id
     * The ID of the Macro to get the number of Macro Events for.
     * @return
     * The number of Macro Events.
     */
    int getNumEventsForMacro(int id);


    /**
     * @brief addEvent
     * Adds a Macro Event to the active Macro(s).
     * @param event
     * The Macro Event to add. The index member of this struct directs where the macro is added.
     * A negative index value means append to end of all active Macros.
     * @param db (OPTIONAL)
     * The already opened database connection to use for this method call. If supplied, this will not open
     * its own connection and it will also not begin a transaction.
     */
    void addEvent(const MacroEvent &event);

    /**
     * @brief addEvents
     * Adds Macro Events to the active Macro(s).
     * @param events
     * A list of the Macro Events to add. The index member of each struct directs where the macro is added.
     * A negative index value means append to end of all active Macros.
     * @param db (OPTIONAL)
     * The already opened database connection to use for this method call. If supplied, this will not open
     * its own connection and it will also not begin a transaction.
     */
    void addEvents(QList<MacroEvent> &events);


    /**
     * @brief getUniformEvents
     * Gets the Macro Events for the set of active Macros that are uniform at each event index.
     * @return
     * A list of uniform Macro Events pertaining to the set of active Macros.
     */
    QList<MacroEvent> getUniformEvents();

    /**
     * @brief setEvent
     * Sets the Macro Event for the active Macro(s) at a specific event index.
     * @param event
     * The new Macro Event value to be set. The index member specifies which Macro event to set.
     */
    void setEvent(const MacroEvent &event);


    /**
     * @brief removeEvent
     * Removes an event from the active Macros at the specified event index.
     * @param eventInd
     * The event index of the event to remove.
     */
    void removeEvent(int eventInd);

    /**
     * @brief removeEvents
     * Removes all events from the active Macros at the specified event indicies.
     * @param eventInds
     * A list of the event indicies of the events to remove. If no list is given, then all of the
     * events are removed for all active Macros.
     */
    void removeEvents(QList<int> &eventInds = QList<int>());


    /**
     * @brief generateNewScreenshotId
     * Generates a new screenshot ID. The screenshot ID will be used to locate screenshot information
     * in the database Screenshot table, and will be used as the name of the screenshot image file
     * in the file system.
     * @return
     * The randomly generated screenshot ID.
     */
    static int generateNewScreenshotId();

    /**
     * @brief generateScreenshotPathFromId
     * Generates a screenshot path and filename based off of a given screenshot ID.
     * @param screenshotId
     * The given screenshot ID to generate the screenshot path off of.
     * @return
     * The screenshot path and file name.
     */
    static QString generateScreenshotPathFromId(int screenshotId);


private:

    /**
     * @brief _activeMacroIds
     * List of the active Macro's IDs.
     */
    QList<int> _activeMacroIds;


    /**
     * @brief addMouseEvent
     * Adds a MacroMouseEvent to the MacroMouseEvents table without committing or closing the database connection.
     * @param mEvent
     * The MacroMouseEvent to add.
     * @param macroEventId
     * The ID of the corresponding parent MacroEvent.
     */
    void addMouseEvent(const MacroMouseEvent &mEvent, int macroEventId);

    /**
     * @brief addScreenshotIfNotExist
     * Adds a screenshot to the Screenshots table if it does not already exist.
     * @param mEvent
     * The Macro Mouse Event that the sceenshot is associated with.
     */
    void addScreenshotIfNotExist(const MacroMouseEvent &mEvent);

    /**
     * @brief addKeyboardEvent
     * Adds a MacroKeyboardEvent to the MacroKeyboardEvents table without committing or closing the database connection.
     * @param kEvent
     * The MacroKeyboardEvent to add.
     * @param macroEventId
     * The ID of the corresponding parent MacroEvent.
     */
    void addKeyboardEvent(const MacroKeyboardEvent &kEvent, int macroEventId);


    /**
     * @brief setMouseEvent
     * Sets a MacroMouseEvent to the MacroMouseEvents table without committing or closing the database connection.
     * @param mEvent
     * The MacroMouseEvent to set.
     * @param macroEventIdWhereClause
     * The where clause filter by macroEventIds.
     */
    void setMouseEvent(const MacroMouseEvent &mEvent, const QString &macroEventIdWhereClause);

    /**
     * @brief setKeyboardEvent
     * Sets a MacroKeyboardEvent to the MacroKeyboardEvents table without committing or closing the database connection.
     * @param kEvent
     * The MacroKeyboardEvent to set.
     * @param macroEventIdWhereClause
     * The where clause filter by macroEventIds.
     */
    void setKeyboardEvent(const MacroKeyboardEvent &kEvent, const QString &macroEventIdWhereClause);


    /**
     * @brief getscreenshotIdsForEvents
     * Gets the keys of screenshot table entries associated with a given list of Macro Events. Basically,
     * if the list contains Macro Events that are location sensitive mouse events, they will have screenshot
     * table entries associated with them.
     * @param eventInds
     * The indexes of Macro Events to get associated screenshot keys of.
     * @return
     * A list of associated screenshot keys.
     */
    QList<int> getscreenshotIdsForEvents(QList<int> eventInds);


    /**
     * @brief deleteScreenshotsIfNotReferenced
     * Deletes screenshots listed in a given screenshots list if they are no longer referenced by any Macro Event.
     * This entails deleting the screenshot from the filesystem and the Screenshots table.
     * @param screenshotIds
     * The list of keys of possible screenshots to be delted (if no longer referenced).
     */
    void deleteScreenshotsIfNotReferenced(const QList<int> &screenshotIds);


    /**
     * @brief _getNumEventsForMacro
     * See getNumEventsForMacro(). Only difference is database is not opened and closed.
     * @param id
     * See getNumEventsForMacro().
     * @return See getNumEventsForMacro().
     */
    int _getNumEventsForMacro(int id);


    /**
     * @brief correctEventIndexesBeforeAdd
     * Corrects the Macro Event indexes for all active Macros before a Macro Event Add operation occurs.
     * @param addedMacroEvents
     * The Macro Events to be added.
     */
    void correctEventIndexesBeforeAdd(QList<MacroEvent> &addMacroEvents);

    /**
     * @brief correctEventIndexesAfterDelete
     * Corrects the Macro Event indexes for all active Macros after a Macro Event Delete operation occurs.
     * @param deletedMacroEventInds
     * The indexes of the deleted Macro Events.
     */
    void correctEventIndexesAfterDelete(QList<int> &deletedMacroEventInds);


    /**
     * @brief buildMacroIdWhereClause
     * Builds the Macro ID WHERE clause component based off of a given Macro ID.
     * @param id
     * The Macro ID to include in the WHERE clause.
     * @return
     * The Macro ID WHERE clause string.
     */
    QString buildMacroIdWhereClause(int id) const;

    /**
     * @brief buildMacroIdWhereClause
     * Builds the Macro ID WHERE clause component based off of the given Macro IDs.
     * @param ids (OPTIONAL)
     * A list of IDs to build the where clause from. Default is the Active Macro IDs.
     * @return
     * The Macro ID WHERE clause in format: "id=<id1> OR id=<id2> OR id=<id3> ... OR id=<idN>".
     */
    QString buildMacroIdWhereClause(const QList<int> &ids=QList<int>()) const;

    /**
     * @brief buildEventIndWhereClause
     * Builds the event indicies part of the where clause based off of a supplied event index.
     * @param eventInd
     * The event index to include in the where clause.
     * @return
     * The event index where clause string.
     */
    QString buildEventIndWhereClause(int eventInd) const;

    /**
     * @brief buildEventIndWhereClause
     * Builds the event indicies part of the where clause based off of a supplied list of event indicies.
     * @param eventInds
     * A list of the event indicies to include in the where clause.
     * @return
     * The event indicies where clause string.
     */
    QString buildEventIndWhereClause(const QList<int> &eventInds) const;

    /**
     * @brief buildEventIdWhereClause
     * Biulds the event ID part of the where clause based off of an event index and the current active Macro IDs.
     * @param eventInd
     * The event index to use to fetch event IDs.
     * @return
     * The event ID where clause string.
     */
    QString buildEventIdWhereClause(int eventInd);

    /**
     * @brief buildscreenshotIdWhereClause
     * Builds the screenshot key part of the where clause based off of a supplied list of screenshot keys.
     * @param screenshotIds
     * The list of screenshot keys to include in the where clause.
     * @return
     * The screenshot keys where clause string.
     */
    QString buildscreenshotIdWhereClause(const QList<int> &screenshotIds) const;


    /**
     * @brief fillMacroEvent
     * Fills a Macro from a query result.
     * @param macroEvent
     * The Macro Event to fill.
     * @param query
     * The query with valid Macro Event results.
     */
    void fillMacroEvent(MacroEvent &macroEvent, const QSqlQuery &query) const;


    /**
     * @brief str
     * Convenience method for converting C Strings to Qt Strings.
     * @param c_str
     * The C String to convert.
     * @return
     * The Qt String.
     */
    QString str(const char *c_str) const;

    /**
     * @brief str
     * Convenience method for converting integers to strings.
     * @param num
     * The integer to convert.
     * @return
     * The string conversion.
     */
    QString str(int num) const;


    /**
     * @brief macroEventTest
     * Tests the Macro Event Model operations that are activated (by c++ Macro definitions).
     */
    void macroEventTest();

#ifdef TEST_MACRO_EVENT

    /**
     * @brief setAcitveMacrosToFirst3
     * Sets the first 3 macros to be active.
     */
    void setAcitveMacrosToFirst3();

    #ifdef TEST_MACRO_EVENT_ADD
        /**
         * @brief macroEventAddTest
         * Tests the addition of macro events.
         */
        void macroEventAddTest();
    #endif

    #ifdef TEST_MACRO_EVENT_REMOVE
        /**
         * @brief macroEventRemoveTest
         * Tests the removal of macro events.
         */
        void macroEventRemoveTest();
    #endif

    #ifdef TEST_MACRO_EVENT_GET
        /**
         * @brief macroEventGetTest
         * Tests the retrieval of macro events.
         */
        void macroEventGetTest();
    #endif

    #ifdef TEST_MACRO_EVENT_SET
        /**
         * @brief macroEventSetTest
         * Tests the setting of macro events.
         */
        void macroEventSetTest();
    #endif

    #ifdef PRINT_ALL
        /**
         * @brief printAllMacroEvents
         * Prints all macro events.
         */
        void printAllMacroEvents();
    #endif

#endif // TEST_MACRO_EVENT
};


#endif // MACROEVENTMODEL_H
