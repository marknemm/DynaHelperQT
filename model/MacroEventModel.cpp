//#define TEST_MACRO_EVENT
#define TEST_MACRO_EVENT_ADD
#define TEST_MACRO_EVENT_REMOVE
#define TEST_MACRO_EVENT_GET
#define TEST_MACRO_EVENT_SET
#define PRINT_ALL

#include "MacroEventModel.h"
#include "DBUtil.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <algorithm>


MacroEventModel::MacroEventModel() :
    Model(QSqlDatabase::database(DBUtil::DB_CONNECTION_NAME)),
    _activeMacroIds()
{
    _db.setDatabaseName(DBUtil::DB_PATH);
    macroEventTest();
}


void MacroEventModel::setActiveMacro(int activeMacroId)
{
    QList<int> activeMacroIds;
    activeMacroIds.append(activeMacroId);
    setActiveMacros(activeMacroIds);
}


void MacroEventModel::setActiveMacros(const QList<int> &activeMacroIds)
{
    _activeMacroIds = activeMacroIds;
}


QList<int> MacroEventModel::getActiveMacroIds() const
{
    return _activeMacroIds;
}


int MacroEventModel::getNumEventsForMacro(int id)
{
    // Make sure this is not simply part of a larger DB transaction!
    bool dbOpenOutsideMethod = _db.isOpen();
    if (!dbOpenOutsideMethod) {
        safeOpen("Error: DB open failed in getNumEventsForMacro()");
    }

    int count = _getNumEventsForMacro(id);

    if (!dbOpenOutsideMethod) {
        _db.close();
    }
    return count;
}


void MacroEventModel::addEvent(const MacroEvent &event)
{
    QList<MacroEvent> events;
    events.append(event);
    addEvents(events);
}


void MacroEventModel::addEvents(QList<MacroEvent> &events)
{
    int index,
        macroEventId;
    bool append = (events.size() > 0 && events.first().index < 0); // Append if event.index < 0.

    // Determine if we do not have pre-established database connection from caller.
    bool isPartOfLargerTransaction = _db.isOpen();
    if (!isPartOfLargerTransaction) {
        safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in addEvents()");
    }
    QSqlQuery macroEventsQuery(_db);

    // If append, get number of events in Macro for insert index.
    // Else, make sure we make room for Macro Event insertions.
    if (append) index = _getNumEventsForMacro(_activeMacroIds.first()) - 1; // Sub 1 here b/c increments right away in foreach below.
    else        correctEventIndexesBeforeAdd(events);

    // Must perform INSERT query foreach new Macro Event.
    foreach (MacroEvent event, events) {
        // If append, just increment index which was set first above.
        // Else get insert index from the event.
        append ? index++
               : index = event.index;

        // Build query for insert into MacroEvents table.
        QString macroEventsQueryStr =
            "INSERT INTO " + DBUtil::MACRO_EVENTS_TABLE_NAME + str("(\n\t") +
                "macroId,        \n\t"
                "macroEventInd,  \n\t"
                "macroEventType, \n\t"
                "delayMs,        \n\t"
                "durationMs,     \n\t"
                "nRepeats,       \n\t"
                "targetPID       \n"
            ") \n"
            "VALUES ( \n\t"
                ":macroId,                      \n\t" +
                str(index)                 + ", \n\t" +
                str(event.type)            + ", \n\t" +
                str(event.delayMs)         + ", \n\t" +
                str(event.durationMs)      + ", \n\t" +
                str(event.nRepeats)        + ", \n\t" +
                str("'") + event.targetPID + "' \n"
            ");";
        macroEventsQuery.prepare(macroEventsQueryStr);

        // Must perform INSERT query foreach active Macro.
        foreach (int macroId, _activeMacroIds) {
            // Execute INSERT query for specific active Macro.
            macroEventsQuery.bindValue(":macroId", macroId);
            safeExec(macroEventsQuery, "Error: INSERT failed in addEvents() with query: \n" + macroEventsQueryStr);

            // Only add to correct table based off of Event Type.
            macroEventId = macroEventsQuery.lastInsertId().toInt();
            if (event.type == MacroEventType::MouseEvent) {
                addMouseEvent(event.mouseEvent, macroEventId);
            }
            else {
                addKeyboardEvent(event.keyboardEvent, macroEventId);
            }
        }
    }

    // Determine if caller did not supply a pre-existing DB connection and we should commit and close our connection.
    if (!isPartOfLargerTransaction) {
        safeCommitAndClose("Error: DB COMMIT failed in addEvents()");
    }
}


QList<MacroEvent> MacroEventModel::getUniformEvents()
{
    QList<MacroEvent> events;
    bool dbOpenOutsideMethod = _db.isOpen();
    if (!dbOpenOutsideMethod) {
        safeOpen("Error: DB open failed in getUniformEvents()");
    }

    QString delayMsCol = DBUtil::MACRO_EVENTS_TABLE_NAME + ".delayMs";
    QString durationMsCol = DBUtil::MACRO_EVENTS_TABLE_NAME + ".durationMs";
    QString autoCorrectCol = DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".autoCorrect";

    QString groupByClause = DBUtil::MACRO_EVENTS_TABLE_NAME          + ".macroEventInd,          \n\t" +
                            DBUtil::MACRO_EVENTS_TABLE_NAME          + ".macroEventType,         \n\t" +
                            DBUtil::MACRO_EVENTS_TABLE_NAME          + ".nRepeats,               \n\t" +
                            DBUtil::MACRO_EVENTS_TABLE_NAME          + ".targetPID,              \n\t" +
                            DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME    + ".macroMouseEventType,    \n\t" +
                            DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME    + ".xLoc,                   \n\t" +
                            DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME    + ".yLoc,                   \n\t" +
                            DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME    + ".wheelDelta,             \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".screenshotId,           \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".screenshotX,            \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".screenshotY,            \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".screenshotW,            \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".screenshotH,            \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".targetImgType,          \n\t" +
                            DBUtil::SCREENSHOT_TABLE_NAME            + ".cursorType,             \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".macrokeyboardEventType, \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".keyCode,                \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".mod1,                   \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".mod2,                   \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".capsLock,               \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".numLock,                \n\t" +
                            DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".keyString ";

    QString selectClause = "COUNT(*) AS cnt, \n\t"
                           // Still want to get back events that don't have uniform delay, duration, or auto correct.
                           // In these cases, we will make these NULL for empty.
                           "CASE \n\t"
                           "    WHEN MAX(" + delayMsCol + ") - MIN(" + delayMsCol + ") = 0 THEN MAX(" + delayMsCol + ") \n\t"
                           "    ELSE NULL \n\t"
                           "END AS delayMs, \n\t"
                           "CASE \n\t"
                           "    WHEN MAX(" + durationMsCol + ") - MIN(" + durationMsCol + ") = 0 THEN MAX(" + durationMsCol + ") \n\t"
                           "    ELSE NULL \n\t"
                           "END AS durationMs, \n\t"
                           "CASE \n\t"
                           "    WHEN MAX(" + autoCorrectCol + ") - MIN(" + autoCorrectCol + ") = 0 THEN MAX(" + autoCorrectCol + ") \n\t"
                           "    ELSE NULL \n\t"
                           "END AS autoCorrect, \n\t" +
                           groupByClause;

    QString queryStr = "SELECT \n\t" +
                            selectClause + "\n"
                       "FROM "      + DBUtil::MACRO_EVENTS_TABLE_NAME + "\n"
                       "LEFT JOIN " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + "\n\t"
                                      "ON " + DBUtil::MACRO_EVENTS_TABLE_NAME       + ".macroEventId = "
                                            + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".macroEventId \n"
                       "LEFT JOIN " + DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + "\n\t"
                                      "ON " + DBUtil::MACRO_EVENTS_TABLE_NAME          + ".macroEventId = "
                                            + DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".macroEventId \n"
                       "LEFT JOIN " + DBUtil::SCREENSHOT_TABLE_NAME + "\n\t"
                                      "ON " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".screenshotId = "
                                            + DBUtil::SCREENSHOT_TABLE_NAME         + ".screenshotId \n"
                       "WHERE \n\t" +
                            buildMacroIdWhereClause() + "\n"
                       "GROUP BY \n\t" +
                            groupByClause + "\n"
                       "ORDER BY \n\t"
                            "macroEventInd ASC;";

    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: SELECT failed in getUniformEvents() with query: \n" + queryStr);
    while(query.next()) {
        if (query.value("cnt").toInt() == _activeMacroIds.size()) {
            MacroEvent event;
            fillMacroEvent(event, query);
            events.append(event);
        }
    }

    if (!dbOpenOutsideMethod) {
        _db.close();
    }
    return events;
}


void MacroEventModel::setEvent(const MacroEvent &event)
{
    // Check if this is part of a larger transaction.
    bool partOfLargerTransaction = _db.isOpen();
    if (!partOfLargerTransaction) {
        safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in setEvent()");
    }

    QString macroEventIdWhere = buildEventIdWhereClause(event.index);

    QString queryStr = "UPDATE " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                       "SET \n\t"
                            " macroEventType="      + str(event.type)            + ", \n\t"
                            " delayMs="             + str(event.delayMs)         + ", \n\t"
                            " durationMs="          + str(event.durationMs)      + ", \n\t"
                            " nRepeats="            + str(event.nRepeats)        + ", \n\t"
                            " targetPID="           + str("'") + event.targetPID + "' \n"
                       "WHERE \n\t" +
                            macroEventIdWhere + ";";

    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: UPDATE failed in setEvent() with query: \n" + queryStr);

    // Update corresponding event based off of Event Type (should never be able to modify event type).
    if (event.type == MacroEventType::MouseEvent) {
        setMouseEvent(event.mouseEvent, macroEventIdWhere);
    }
    else { // (event.type == MacroEventType::KeyboardEvent)
        setKeyboardEvent(event.keyboardEvent, macroEventIdWhere);
    }

    // Make sure we do not close a DB connection if it was started outside this method!
    if (!partOfLargerTransaction) {
        safeCommitAndClose("Error: DB COMMIT failed in setEvent()");
    }
}


void MacroEventModel::removeEvent(int eventInd)
{
    QList<int> eventInds;
    eventInds.append(eventInd);
    removeEvents(eventInds);
}


void MacroEventModel::removeEvents(QList<int> &eventInds)
{
    // Check if this is part of a larger transaction.
    bool partOfLargerTransaction = _db.isOpen();
    if (!partOfLargerTransaction) {
        safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in removeEvents()");
    }

    // Get screenshot filenames associated with the events we are going to remove.
    // After delete, we can see what filenames are no longer referenced to see which screenshot files to delete.
    QList<int> screenshotIds = getscreenshotIdsForEvents(eventInds);

    // Do the delete.
    QString queryStr = "DELETE FROM " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                       "WHERE \n\t" +
                            buildMacroIdWhereClause() + " \n";

    // Are we limiting to specific events, or removing all events for each active Macro?
    QString eventIndWhere = eventInds.size() != 0 ? "  AND \n" + buildEventIndWhereClause(eventInds) + ";"
                                                  : "";
    queryStr += eventIndWhere;

    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: DELETE failed in removeEvents() with query: \n" + queryStr);

    // Delete screenshots and associate screenshot filenames that are no longer referenced.
    deleteScreenshotsIfNotReferenced(screenshotIds);

    // Make sure we adjust Event Indexes to keep contiguous ordering of Macro Events.
    correctEventIndexesAfterDelete(eventInds);

    if (!partOfLargerTransaction) {
        safeCommitAndClose("Error: DB COMMIT failed in removeEvents()");
    }
}


int MacroEventModel::generateNewScreenshotId()
{
    int screenshotId;
    QString screenshotPath;
    QFileInfo checkFileExist;

    // Keep generating random file name until we get one that does not exist!
    do {
        screenshotId = qrand();
        screenshotPath = DBUtil::SCREENSHOT_DIR_PATH + QString::number(screenshotId) + ".png";
        checkFileExist.setFile(screenshotPath);
    }
    while (checkFileExist.exists());

    return screenshotId;
}


QString MacroEventModel::generateScreenshotPathFromId(int screenshotId)
{
    return (DBUtil::SCREENSHOT_DIR_PATH + QString::number(screenshotId) + ".png");
}


void MacroEventModel::addMouseEvent(const MacroMouseEvent &mEvent, int macroEventId)
{
    QSqlQuery mouseEventsQuery(_db);

    // Add the screenshot to database if not exist already.
    addScreenshotIfNotExist(mEvent);

    // Insert default of NULL if screenshotId is -1 in mEvent!
    QString screenshotId = (mEvent.screenshotId >= 0 ? str(mEvent.screenshotId) : str("NULL"));

    // Build query for insert into MacroMouseEvents table.
    QString mouseEventsQueryStr =
        "INSERT INTO " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + " ( \n\t"
            "macroEventId,        \n\t"
            "macroMouseEventType, \n\t"
            "xLoc,                \n\t"
            "yLoc,                \n\t"
            "wheelDelta,          \n\t"
            "screenshotId,        \n\t"
            "autoCorrect          \n"
        ") \n"
        "VALUES ( \n\t" +
            str(macroEventId)                   + ", \n\t" +
            str(mEvent.type)                    + ", \n\t" +
            str(mEvent.loc.x())                 + ", \n\t" +
            str(mEvent.loc.y())                 + ", \n\t" +
            str(mEvent.wheelDelta)              + ", \n\t" +
            screenshotId                        + ", \n\t" +
            str(mEvent.autoCorrect ? 1 : 0)     + "  \n"
        ");";
    mouseEventsQuery.prepare(mouseEventsQueryStr);

    safeExec(mouseEventsQuery, "Error: INSERT fialed in addMouseEvent() with query: \n" + mouseEventsQueryStr);
}


void MacroEventModel::addScreenshotIfNotExist(const MacroMouseEvent &mEvent)
{
    QString queryStr;
    QSqlQuery query(_db);

    // Make sure that we have a location sensitve mouse event with an associae screenshot.
    if (mEvent.screenshotId >= 0) {
        queryStr = "SELECT 1 \n"
                   "FROM " + DBUtil::SCREENSHOT_TABLE_NAME + " \n"
                   "WHERE screenshotId=:screenshotId;";
        query.prepare(queryStr);
        query.bindValue(":screenshotId", str(mEvent.screenshotId));
        safeExec(query, "Error: SELECT failed in addScreenshotIfNotExist() with query: \n" + queryStr);

        // Will not get an entry in result set if it does NOT exists already.
        if (!query.next()) {
            // Save the screenshot to the filesystem first.
            QString screenshotPath = MacroEventModel::generateScreenshotPathFromId(mEvent.screenshotId);
            mEvent.screenshotImg.save(screenshotPath);

            // Next, make a record for it in the Screenshots table.
            queryStr = "INSERT INTO " + DBUtil::SCREENSHOT_TABLE_NAME + " ( \n\t" +
                           "screenshotId,  \n\t"
                           "screenshotX,   \n\t"
                           "screenshotY,   \n\t"
                           "screenshotW,   \n\t"
                           "screenshotH,   \n\t"
                           "targetImgType, \n\t"
                           "cursorType     \n"
                       ") \n"
                       "VALUES ( \n\t" +
                           str(mEvent.screenshotId)            + ", \n\t" +
                           str(mEvent.screenshotRect.x())      + ", \n\t" +
                           str(mEvent.screenshotRect.y())      + ", \n\t" +
                           str(mEvent.screenshotRect.width())  + ", \n\t" +
                           str(mEvent.screenshotRect.height()) + ", \n\t" +
                           str(mEvent.targetImgType)           + ", \n\t" +
                           str(mEvent.cursorType)              + "  \n"
                       ");";
            query.prepare(queryStr);
            safeExec(query, "Error: INSERT failed in addScreenshotIfNotExist() with query: \n" + queryStr);
        }
    }
}


void MacroEventModel::addKeyboardEvent(const MacroKeyboardEvent &kEvent, int macroEventId)
{
    QSqlQuery keyboardEventsQuery(_db);

    // Build query for insert into MacroKeyboardEvents table.
    QString keyboardEventsQueryStr =
        "INSERT INTO " + DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + "( \n"
            "macroEventId,           \n\t"
            "macrokeyboardEventType, \n\t"
            "keyCode,                \n\t"
            "mod1,                   \n\t"
            "mod2,                   \n\t"
            "capsLock,               \n\t"
            "numLock,                \n\t"
            "keyString               \n"
        ") "
        "VALUES ( \n" +
            str(macroEventId)                + ", \n\t" +
            str(kEvent.type)                 + ", \n\t" +
            str(kEvent.keyCode)              + ", \n\t" +
            str(kEvent.mod1)                 + ", \n\t" +
            str(kEvent.mod2)                 + ", \n\t" +
            str(kEvent.capsLock ? "1" : "0") + ", \n\t" +
            str(kEvent.numLock  ? "1" : "0") + ", \n\t"
            "'" + kEvent.keyString           + "' \n"
        ");";
    keyboardEventsQuery.prepare(keyboardEventsQueryStr);

    safeExec(keyboardEventsQuery, "Error: INSERT failed in addKeyboardEvent() with query: \n" + keyboardEventsQueryStr);
}


void MacroEventModel::setMouseEvent(const MacroMouseEvent &mEvent, const QString &macroEventIdWhereClause)
{
    QString queryStr = "UPDATE " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + " \n"
                       "SET \n\t"
                            "macroMouseEventType=" + str(mEvent.type)                    + ", \n\t"
                            "xLoc= "               + str(mEvent.loc.x())                 + ", \n\t"
                            "yLoc="                + str(mEvent.loc.y())                 + ", \n\t"
                            "wheelDelta="          + str(mEvent.wheelDelta)              + ", \n\t"
                            "autoCorrect="         + str(mEvent.autoCorrect ? 1 : 0)     + "  \n\t"
                       "WHERE \n\t" +
                            macroEventIdWhereClause + ";";

    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: UPDATE failed in setMouseEvent() with query: \n" + queryStr);
}


void MacroEventModel::setKeyboardEvent(const MacroKeyboardEvent &kEvent, const QString &macroEventIdWhereClause)
{
    QString queryStr = "UPDATE " + DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + " \n"
                       "SET "
                            "macrokeyboardEventType= " + str(kEvent.type)                 + ", \n\t"
                            "keyCode="                 + str(kEvent.keyCode)              + ", \n\t"
                            "mod1="                    + str(kEvent.mod1)                 + ", \n\t"
                            "mod2="                    + str(kEvent.mod2)                 + ", \n\t"
                            "capsLock="                + str(kEvent.capsLock ? "1" : "0") + ", \n\t"
                            "numLock="                 + str(kEvent.numLock  ? "1" : "0") + ", \n\t"
                            "keyString='"              + kEvent.keyString                 + "' \n"
                       "WHERE \n\t" +
                            macroEventIdWhereClause + ";";

    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: UPDATE failed in setKeyboardEvent() with query: \n" + queryStr);
}


QList<int> MacroEventModel::getscreenshotIdsForEvents(QList<int> eventInds)
{
    QList<int> screenshotIds;

    QString queryStr = "SELECT screenshotId \n"
                       "FROM " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                       "INNER JOIN " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + " \n\t"
                            "ON " + DBUtil::MACRO_EVENTS_TABLE_NAME + ".macroEventId="
                                  + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".macroEventId \n"
                       "WHERE \n\t" +
                            buildMacroIdWhereClause() + "\n"
                       "  AND screenshotId >= 0 \n";

    // Are we getting screenshot IDs for all events of active Macros or certain given ones?
    QString eventIndsWhereClause = eventInds.size() != 0 ? "   AND \n\t" + buildEventIndWhereClause(eventInds) + "\n"
                                                         : "";
    queryStr += eventIndsWhereClause +
                "GROUP BY screenshotId;";

    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: SELECT failed in getScreenshotIdsForEvents() with query: \n" + queryStr);

    while (query.next()) {
        screenshotIds.append(query.value("screenshotId").toInt());
    }
    return screenshotIds;

}


void MacroEventModel::deleteScreenshotsIfNotReferenced(const QList<int> &screenshotIds)
{
    QList<int> deleteScreenshotIds;
    QString fileName,
            selectQueryStr,
            deleteQueryStr,
            screenshotPath;

    qDebug() << "Possible delete count: " << screenshotIds.size();
    foreach (int screenshotId, screenshotIds) {
        qDebug() << "ID: " << screenshotId;
    }

    // Stop right away if we don't have any possible deletes!
    if (screenshotIds.size() == 0) return;

    QSqlQuery query(_db);

    // Should take log(N) ^ 2 time ideally.
    // Select all Screenshots that are no longer refrenced in the MacroMouseEvents table.
    selectQueryStr = "SELECT screenshotId \n"
                     "FROM " + DBUtil::SCREENSHOT_TABLE_NAME + " \n"
                     "WHERE " + buildscreenshotIdWhereClause(screenshotIds) + " \n"
                     "  AND NOT EXISTS ( \n" +
                     "                   SELECT 1 \n"
                     "                   FROM " +  DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + " \n"
                     "                   WHERE " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".screenshotId=" +
                                                   DBUtil::SCREENSHOT_TABLE_NAME + ".screenshotId \n"
                     "                 );";
    query.prepare(selectQueryStr);
    safeExec(query, "Error: SELECT failed in deleteScreenshotsIfNotReferenced() with query: \n" + selectQueryStr);

    // Delete screenshots from filesystem and prepare screenshotIds list for delte from Screenshots table.
    while (query.next()) {
        fileName = str(query.value("screenshotId").toInt());
        screenshotPath = DBUtil::SCREENSHOT_DIR_PATH + fileName + ".png";
        QFile file(screenshotPath);
        // Check if it exists first to ensure user did not manually delete it perviously!
        if (file.exists()) {
            safeExec(file.remove(), "Error: Filesystem remove failed in deleteSceenshotsIfNotReferenced() for file: " + fileName);
            deleteScreenshotIds.append(query.value("screenshotId").toInt());
        }
    }

    if (deleteScreenshotIds.size() != 0) {
        // Delete Screenshot table rows.
        deleteQueryStr = "DELETE FROM " + DBUtil::SCREENSHOT_TABLE_NAME + " \n"
                         "WHERE " + buildscreenshotIdWhereClause(deleteScreenshotIds) + ";";
        query.prepare(deleteQueryStr);
        safeExec(query, "Error: DELETE failed in deleteScreenshotsIfNotReferenced() with query: \n" + deleteQueryStr);
    }
}


int MacroEventModel::_getNumEventsForMacro(int id)
{
    QString queryStr = "SELECT COUNT(*) \n"
                       "FROM " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                       "WHERE \n\t"  +
                            buildMacroIdWhereClause(id) + ";";
    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: SELECT failed in _getNumEventsForMacro() with query: \n" + queryStr);
    safeExec(query.next(), "Error: Macro Events not found for Macro with ID: " + str(id));
    return query.value(0).toInt();
}


void MacroEventModel::correctEventIndexesBeforeAdd(QList<MacroEvent> &addMacroEvents)
{
    int amtToMove,
        iEndContiguousChunk,
        baseContiguousAddInds,
        endContiguousAddInds,
        indexToCorrectFrom = 0,
        indexToCorrectTo;
    QString queryStr;
    QSqlQuery query(_db);

    // Must sort Macro Events by event indexes in increasing order. The add event indexes will be used to
    // determine how much to move current indexes that already fall in their place by to make room.
    std::sort(addMacroEvents.begin(), addMacroEvents.end());

    for (int i = addMacroEvents.size() - 1; i >= 0; i--) {
        // Move by the number of addMacroEvents excluding the contiguous chunks that have already been encountered.
        amtToMove = (i + 1);

        // This is the end of the chunk of contiguous macro event indexes of events to be added.
        endContiguousAddInds = addMacroEvents.at(i).index;
        iEndContiguousChunk = i;

        // Find the beginning of the chunk of contiguous macro event indexes of events to be added (moving backwards).
        while ((i - 1) >= 0 && addMacroEvents.at(i - 1).index == addMacroEvents.at(i).index - 1)
        { i--; }
        baseContiguousAddInds = addMacroEvents.at(i).index;

        // Set the index to correct to to be the previous index that we were correcting from.
        // This will simply be the beginning of the next chunk of contiguous add indexes.
        indexToCorrectTo = (indexToCorrectFrom > 0) ? (indexToCorrectFrom - 1) : 1000000000;
        // Set the index to correct from to be the base of the current contiguous add indexes.
        indexToCorrectFrom = baseContiguousAddInds;

        qDebug() << "Moving indexes " << indexToCorrectFrom << " to " << indexToCorrectTo << " forward by " << amtToMove;

        // Do the update.
        queryStr = "UPDATE " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                   "SET macroEventInd = macroEventInd + " + str(amtToMove) + " \n"
                   "WHERE "  + buildMacroIdWhereClause() + " AND macroEventInd >= " + str(indexToCorrectFrom)
                                                         + " AND macroEventInd <= " + str(indexToCorrectTo) + ";";
        query.prepare(queryStr);
        safeExec(query, "Error: UPDATE failed in correctEventIndexesBeforeAdd() with query: \n" + queryStr);
    }
}


void MacroEventModel::correctEventIndexesAfterDelete(QList<int> &deletedMacroEventInds)
{
    int amtToMove = 0,
        baseContiguousDeleteInds,
        endContiguousDeleteInds,
        indexToCorrectFrom,
        indexToCorrectTo;
    QString queryStr;
    QSqlQuery query(_db);

    // Algorithm expects the indicies to be in sorted order since we are looking for incremental gaps between
    // the indexes to be deleted. We will be moving the non deleted elements that fall in these gaps back by
    // the total number of deleted elements encountered before them.
    std::sort(deletedMacroEventInds.begin(), deletedMacroEventInds.end());

    for (int i = 0; i < deletedMacroEventInds.length(); i++) {
        // The beginning of a chunk of macro indexes to be deleted.
        baseContiguousDeleteInds = deletedMacroEventInds.at(i);

        // Find the end of the chunk of macro indexes to be deleted.
        while (   (i + 1) < deletedMacroEventInds.length()
               && deletedMacroEventInds.at(i + 1) == deletedMacroEventInds.at(i) + 1)
        { i++; }
        endContiguousDeleteInds = deletedMacroEventInds.at(i);

        // This is the index immediately after the contiguous chunk of delete indexes (that must be moved back).
        indexToCorrectFrom = endContiguousDeleteInds + 1;
        // This is the index that falls immediately before the next contiguous chunk of delete indexes (that must be moved back).
        indexToCorrectTo = ((i + 1) < deletedMacroEventInds.length() ? deletedMacroEventInds.at(i + 1) - 1 : 1000000000);
        // This is the amount to shift all indexes within range indexToCorrectFrom to indexToCorrecTo back by (inclusive).
        amtToMove += (indexToCorrectFrom - baseContiguousDeleteInds);

        qDebug() << "Moving indexes " << indexToCorrectFrom << " to " << indexToCorrectTo << " back by " << amtToMove;

        // Do the update.
        queryStr = "UPDATE " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                   "SET macroEventInd = macroEventInd - " + str(amtToMove) + " \n"
                   "WHERE "  + buildMacroIdWhereClause() + " AND macroEventInd >= " + str(indexToCorrectFrom)
                                                         + " AND macroEventInd <= " + str(indexToCorrectTo) + ";";
        query.prepare(queryStr);
        safeExec(query, "Error: UPDATE failed in correctEventIndexesAfterDelete() with query: \n" + queryStr);
    }
}


QString MacroEventModel::buildMacroIdWhereClause(int id) const
{
    QList<int> ids;
    ids.append(id);
    return buildMacroIdWhereClause(ids);
}


QString MacroEventModel::buildMacroIdWhereClause(const QList<int> &ids) const
{
    const QList<int> &buildIds = (ids.size() == 0) ? _activeMacroIds : ids;
    QString idWhere = " (";
    foreach (int id, buildIds) {
        idWhere += "macroId=" + str(id) + " OR ";
    }
    return idWhere.left(idWhere.size() - 4) + ") ";
}


QString MacroEventModel::buildEventIndWhereClause(int eventInd) const
{
    QList<int> eventInds;
    eventInds.append(eventInd);
    return buildEventIndWhereClause(eventInds);
}


QString MacroEventModel::buildEventIndWhereClause(const QList<int> &eventInds) const
{
    QString indWhere = " (";
    foreach (int ind, eventInds) {
        indWhere += "macroEventInd=" + str(ind) + " OR ";
    }
    return indWhere.left(indWhere.size() - 4) + ") ";
}


QString MacroEventModel::buildEventIdWhereClause(int eventInd)
{
    QString queryStr = "SELECT macroEventId \n"
                       "FROM " + DBUtil::MACRO_EVENTS_TABLE_NAME + " \n"
                       "WHERE " + buildMacroIdWhereClause() + " AND " + buildEventIndWhereClause(eventInd) + ";";
    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: SELECT failed in buildEventIdWhereClause() with query: \n" + queryStr);

    QString macroEventIdWhere = "(";
    while (query.next()) {
        macroEventIdWhere += "macroEventId=" + query.value("macroEventId").toString() + " OR ";
    }
    return macroEventIdWhere.left(macroEventIdWhere.size() - 4) + ")";
}


QString MacroEventModel::buildscreenshotIdWhereClause(const QList<int> &screenshotIds) const
{
    QString screenshotIdWhere = "(";
    foreach (int screenshotId, screenshotIds) {
        screenshotIdWhere += "screenshotId=" + str(screenshotId) + " OR ";
    }
    return screenshotIdWhere.left(screenshotIdWhere.size() - 4) + ")";
}


void MacroEventModel::fillMacroEvent(MacroEvent &macroEvent, const QSqlQuery &query) const
{
    macroEvent.index = query.value("macroEventInd").toInt();
    macroEvent.type = (MacroEventType)query.value("macroEventType").toInt();
    // Do we have a uniform delay?
    macroEvent.delayMs = !query.value("delayMs").isNull() ? query.value("delayMs").toInt()
                                                          : -1;
    // Do we have a uniform duration?
    macroEvent.durationMs = !query.value("durationMs").isNull() ? query.value("durationMs").toInt()
                                                                : -1;
    macroEvent.nRepeats = query.value("nRepeats").toInt();
    macroEvent.targetPID = query.value("targetPID").toString();

    MacroMouseEvent &mouseEvent = macroEvent.mouseEvent;
    mouseEvent.type = (MacroMouseEventType)query.value("macroMouseEventType").toInt();
    mouseEvent.loc.setX(query.value("xLoc").toInt());
    mouseEvent.loc.setY(query.value("yLoc").toInt());
    mouseEvent.wheelDelta = query.value("wheelDelta").toInt();
    mouseEvent.screenshotId = query.value("screenshotId").toInt();
    mouseEvent.screenshotRect.setX(query.value("screenshotX").toInt());
    mouseEvent.screenshotRect.setY(query.value("screenshotY").toInt());
    mouseEvent.screenshotRect.setWidth(query.value("screenshotW").toInt());
    mouseEvent.screenshotRect.setHeight(query.value("screenshotH").toInt());
    mouseEvent.targetImgType = (TargetImgType)query.value("targetImgType").toInt();
    mouseEvent.cursorType = (CursorType)query.value("cursorType").toInt();
    mouseEvent.autoCorrect = (query.value("autoCorrect").toInt() == 1);
    // If we have a screenshot.
    if (mouseEvent.screenshotId >= 0) {
        mouseEvent.screenshotImg.load(generateScreenshotPathFromId(mouseEvent.screenshotId));
        mouseEvent.screenshotImg.convertToFormat(QImage::Format_Indexed8);
    }

    MacroKeyboardEvent &keyboardEvent = macroEvent.keyboardEvent;
    keyboardEvent.type = (MacroKeyboardEventType)query.value("macrokeyboardEventType").toInt();
    keyboardEvent.keyCode = query.value("keyCode").toInt();
    keyboardEvent.mod1 = query.value("mod1").toInt();
    keyboardEvent.mod2 = query.value("mod2").toInt();
    keyboardEvent.capsLock = query.value("capsLock").toInt();
    keyboardEvent.numLock = query.value("numLock").toInt();
    keyboardEvent.keyString = query.value("keyString").toString();
}


QString MacroEventModel::str(const char *c_str) const
{
    return QString(c_str);
}


QString MacroEventModel::str(int num) const
{
    return QString::number(num);
}


void MacroEventModel::macroEventTest()
{
    #ifdef TEST_MACRO_EVENT
        #ifdef TEST_MACRO_EVENT_ADD
            macroEventAddTest();
        #endif
        #ifdef TEST_MACRO_EVENT_REMOVE
            macroEventRemoveTest();
        #endif
        #ifdef TEST_MACRO_EVENT_SET
            macroEventSetTest();
        #endif
        #ifdef TEST_MACRO_EVENT_GET
            macroEventGetTest();
        #endif
        #ifdef PRINT_ALL
            printAllMacroEvents();
        #endif
    #endif // TEST_MACRO_EVENT
}


#ifdef TEST_MACRO_EVENT

    void MacroEventModel::setAcitveMacrosToFirst3()
    {
        // Set active Macros (NOTE: MUST HAVE AT LEAST 3 MACRO META DATAS IN MACRO TABLE TO TEST).
        QList<int> activeIds;
        activeIds.append(1);
        activeIds.append(2);
        activeIds.append(3);
        setActiveMacros(activeIds);
    }

    #ifdef TEST_MACRO_EVENT_ADD

        void MacroEventModel::macroEventAddTest()
        {
            QList<MacroEvent> eventsToAdd;
            setAcitveMacrosToFirst3();

            // Create dummy first event and append.
            MacroEvent event;
            event.index = -1;
            event.targetPID = "DummyPIDA";
            event.type = MacroEventType::MouseEvent;
            event.mouseEvent.type = MacroMouseEventType::LeftPress;
            event.mouseEvent.loc = QPoint(150, 150);
            eventsToAdd.append(event);

            // Create dummy second event and append.
            event.mouseEvent.type = MacroMouseEventType::LeftRelease;
            eventsToAdd.append(event);

            // Append first and second events and clear list.
            addEvents(eventsToAdd);
            eventsToAdd.clear();

            // Create dummy third and fourth events and insert at index 0 and 1.
            event.index = 0;
            event.type = MacroEventType::KeyboardEvent;
            event.keyboardEvent.type = MacroKeyboardEventType::KeyPress;
            event.keyboardEvent.keyCode = Qt::Key_Return;
            eventsToAdd.append(event);
            event.index = 1;
            eventsToAdd.append(event);

            // Add third and fourth events and clear list.
            addEvents(eventsToAdd);
            eventsToAdd.clear();

            // Insert last created dummy event at index 3 for only one Macro.
            // Makes events for all Macros 1, 2, and 3 non-homogeneous past event 2.
            setActiveMacro(3);
            event.index = 3;
            addEvent(event);
        }

    #endif // TEST_MACRO_EVENT_ADD

    #ifdef TEST_MACRO_EVENT_REMOVE

        void MacroEventModel::macroEventRemoveTest()
        {
            setAcitveMacrosToFirst3();
            QList<int> eventInds;
            eventInds.append(1);
            eventInds.append(3);
            removeEvents(eventInds);
        }

    #endif // TEST_MACRO_EVENT_REMOVE

    #ifdef TEST_MACRO_EVENT_GET

        void MacroEventModel::macroEventGetTest()
        {
            setAcitveMacrosToFirst3();

            // Get uniform Macro Events from active Macros and print contents.
            QList<MacroEvent> eventsList = getUniformEvents();
            qDebug() << "\n\nMACRO EVENT GET TEST OUTPUT\n";

            foreach (MacroEvent macroEvent, eventsList) {
                printMacroEvent(macroEvent);
            }
        }

    #endif // TEST_MACRO_EVENT_GET

    #ifdef TEST_MACRO_EVENT_SET

        void MacroEventModel::macroEventSetTest()
        {
            setAcitveMacrosToFirst3();

            // Create dummy Macro Event and set at index 0 in active Macro Events.
            MacroEvent event;
            event.index = 0;
            event.targetPID = "DummyPIDS";
            event.type = MacroEventType::KeyboardEvent;
            event.keyboardEvent.type = MacroKeyboardEventType::KeyPress;
            event.keyboardEvent.keyCode = 77;
            setEvent(event);
        }

    #endif // TEST_MACRO_EVENT_SET

    #ifdef PRINT_ALL

        void MacroEventModel::printAllMacroEvents()
        {
            qDebug() << "\n\nMACRO EVENT PRINT ALL\n";
            safeOpen("Error: DB open failed in getUniformEvents()");

            QString queryStr = "SELECT * "
                               "FROM "      + DBUtil::MACRO_EVENTS_TABLE_NAME + "\n"
                               "LEFT JOIN " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + "\n\t"
                                            "ON " + DBUtil::MACRO_EVENTS_TABLE_NAME + ".macroEventId = "
                                                  + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".macroEventId \n"
                               "LEFT JOIN " + DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + "\n\t"
                                            "ON " + DBUtil::MACRO_EVENTS_TABLE_NAME + ".macroEventId = "
                                                  + DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME + ".macroEventId \n"
                               "LEFT JOIN " + DBUtil::SCREENSHOT_TABLE_NAME + "\n\t"
                                            "ON " + DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME + ".screenshotId = "
                                                  + DBUtil::SCREENSHOT_TABLE_NAME         + ".screenshotId \n"
                               "ORDER BY " + DBUtil::MACRO_EVENTS_TABLE_NAME + ".macroEventInd ASC;";

            QSqlQuery query(_db);
            query.prepare(queryStr);
            safeExec(query, "Error: SELECT failed in printAllMacroEvents() with query: " + queryStr);

            while(query.next()) {
                MacroEvent event;
                fillMacroEvent(event, query);
                printMacroEvent(event);
            }

            _db.close();
        }

    #endif

#endif // TEST_MACRO_EVENT
