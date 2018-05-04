//#define TEST_BULK_ADD
//#define TEST_BULK_COPY_MACROS

#include "MacroMetaModel.h"
#include "DBUtil.h"
#include <QDebug>


MacroMetaModel::MacroMetaModel(MacroEventModel &macroEventModel) :
    Model(QSqlDatabase::database(DBUtil::DB_CONNECTION_NAME)),
    NAME_REGEX("^[a-zA-Z0-9 ]*$"),
    _macroEventModel(macroEventModel),
    _lastAddedId(-1)
{
    _db.setDatabaseName(DBUtil::DB_PATH);

#ifdef TEST_BULK_ADD
    safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in MacroMetaModel() constructor");

    for (int i = 0; i < 1200; i++) {
        qDebug() << "Bulk Add Test ID: " << i;
        int rand = std::rand();
        QString name = "T" + QString::number(rand);
        addMacro(name);
    }

    safeCommitAndClose("Error: DB COMMIT failed in MacroMetaModel() constructor");
    _lastAddedId = -1;
#endif

#ifdef TEST_BULK_COPY_MACROS
    QList<int> ids;
    for (int i = 1; i <= 50; i++) {
        ids.append(i);
    }
    copyMacros(ids);
    _lastAddedId = -1
#endif
}


int MacroMetaModel::addMacro(const QString &name, bool matchRegex)
{
    if (!matchRegex || NAME_REGEX.exactMatch(name)) {
        // Make sure this is not a sub-component of an already existing larger transaction!
        bool isPartOfLargerTransaction = _db.isOpen();
        if (!isPartOfLargerTransaction) {
            safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in addMacro()");
        }

        QSqlQuery query(_db);
        QString queryStr = "INSERT INTO " + DBUtil::MACROS_TABLE_NAME + " (macroName) VALUES (:macroName);";
        query.prepare(queryStr);
        query.bindValue(":macroName", name);
        safeExec(query, "Error: " + DBUtil::MACROS_TABLE_NAME + " INSERT failed in addMacro() using query: " + queryStr);
        _lastAddedId = query.lastInsertId().toInt();

        if (!isPartOfLargerTransaction) {
            safeCommitAndClose("Error: DB COMMIT failed in addMacro()");
        }
        return _lastAddedId;
    }
    return -1; // Failed due to incorrectly formatted Macro name.
}


MacroMetadata MacroMetaModel::copyMacro(int id)
{
    QList<int> ids;
    ids.append(id);
    return copyMacros(ids).first();
}


QList<MacroMetadata> MacroMetaModel::copyMacros(const QList<int> &ids)
{
    MacroMetadata copyResult;
    QList<MacroMetadata> copyResultList;
    // Atomic operation: Copy all macros completely, or none at all!
    bool isPartOfLargerTransaction = _db.isOpen();
    if (!isPartOfLargerTransaction) {
        safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in copyMacros()");
    }

    foreach (int id, ids) {
        // Generate new Macro Metadata for copy destination.
        copyResult.name = getMacroName(id);
        copyResult.id = addMacro(copyResult.name, false); // false for ignore match regex requirement.

        // Get the Macro Events for the copy source Macro.
        _macroEventModel.setActiveMacro(id);
        QList<MacroEvent> copyEvents = _macroEventModel.getUniformEvents();

        // Add the Macro Events to the destination Macro.
        _macroEventModel.setActiveMacro(copyResult.id);
        _macroEventModel.addEvents(copyEvents);

        copyResultList.append(copyResult);
    }

    if (!isPartOfLargerTransaction) {
        safeCommitAndClose("Error: DB COMMIT failed in copyMacros()");
    }
    return copyResultList;
}


void MacroMetaModel::removeMacro(int id)
{
    // Simply delegate to removeMacros().
    QList<int> ids;
    ids.append(id);
    removeMacros(ids);
}


void MacroMetaModel::removeMacros(const QList<int> &ids)
{
    // Atomic operation: Delete all Macros completely or none at all!
    bool isPartOfLargerTransaction = _db.isOpen();
    if (!isPartOfLargerTransaction) {
        safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in removeMacros()");
    }

    // Delete Macro Events.
    _macroEventModel.setActiveMacros(ids);
    _macroEventModel.removeEvents();

    // Delete Macro Metadata.
    QSqlQuery query(_db);
    QString queryStr = "DELETE FROM " + DBUtil::MACROS_TABLE_NAME + " \n"
                       "WHERE " + buildMacroIdWhereClause(ids) + ";";
    query.prepare(queryStr);
    safeExec(query, "Error: DELETE failed in removeMacros() using query: \n" + queryStr);

    if (!isPartOfLargerTransaction) {
        safeCommitAndClose("Error: DB COMMIT failed in removeMacros()");
    }
}


QString MacroMetaModel::getMacroName(int id)
{
    bool dbOpenOutsideMethod = _db.isOpen();
    if (!dbOpenOutsideMethod) {
        safeOpen("Error: DB open failed in getMacroName()");
    }

    QSqlQuery query(_db);
    QString name;

    QString queryStr = "SELECT macroName FROM " + DBUtil::MACROS_TABLE_NAME + " WHERE macroId=:macroId;";
    query.prepare(queryStr);
    query.bindValue(":macroId", id);
    safeExec(query, "Error: " + DBUtil::MACROS_TABLE_NAME + " SELECT failed in getMacroName() with query: \n" + queryStr);

    if (query.next()) {
        name = query.value("macroName").toString();
    }
    else {
        qDebug() << "Error: Macro with ID " << QString::number(id) << " does not exist";
        exit(1);
    }
    // Make sure we do not close DB connection if it was opened outside this method!
    if (!dbOpenOutsideMethod) {
        _db.close();
    }
    return name;
}


int MacroMetaModel::getLastAddedMacroId() const
{
    return _lastAddedId;
}


void MacroMetaModel::setMacroName(int id, const QString &name)
{
    bool isPartOfLargerTransaction = _db.isOpen();
    if (!isPartOfLargerTransaction) {
        safeOpenAndBegin("Error: DB open and BEGIN TRANSACTION failed in setMacroName()");
    }

    QSqlQuery query(_db);
    QString queryStr = "UPDATE " + DBUtil::MACROS_TABLE_NAME + " SET macroName=:macroName WHERE macroId=:macroId;";
    query.prepare(queryStr);
    query.bindValue(":macroName", name);
    query.bindValue(":macroId", id);
    safeExec(query, "Error: " + DBUtil::MACROS_TABLE_NAME + " UPDATE failed in setMacroName() with query: " + queryStr);

    if (!isPartOfLargerTransaction) {
        safeCommitAndClose("Error: DB COMMIT failed in setMacroName()");
    }
}


QRegExp MacroMetaModel::getMacroNameRegex() const
{
    return NAME_REGEX;
}


QList<MacroMetadata> MacroMetaModel::getMacroMetadata(const QString &nameOrIdFilter, int offset, int limit,
                                                      MacroMetadataSortOrder sortOrder)
{
    QList<MacroMetadata> macroMetaList;
    bool dbOpenOutsideMethod = _db.isOpen();
    if (!dbOpenOutsideMethod) {
        safeOpen("Error: DB open failed in getMacroMetadata()");
    }
    QString whereClause, limitClause, offsetClause, sortClause = "";

    // Check inputs for SELECT filters.
    if (nameOrIdFilter != "") {
        bool isNum;
        nameOrIdFilter.toInt(&isNum);
        if (isNum) {
            whereClause = "WHERE macroId = " + nameOrIdFilter;
        }
        else {
            whereClause = "WHERE macroName LIKE '%" + nameOrIdFilter + "%'";
        }
    }
    if (limit != -1 || offset != 0) {
        limitClause = "LIMIT :lim";
        offsetClause = "OFFSET :off";
    }
    sortClause = "ORDER BY ";
    switch (sortOrder) {
    case ID_DESC:   sortClause += "macroId DESC";    break;
    case NAME_ASC:  sortClause += "macroName ASC";   break;
    case NAME_DESC: sortClause += "macroName DESC";  break;
    default:        sortClause += "macroId ASC";     break;
    }

    // Prepare query and bind values.
    QSqlQuery query(_db);
    QString queryStr = "SELECT macroId, macroName FROM " + DBUtil::MACROS_TABLE_NAME + " " + whereClause + " " +
                       sortClause + " " + limitClause + " " + offsetClause + ";";
    query.prepare(queryStr);
    if (limitClause != "") {
        query.bindValue(":lim", limit);
    }
    if (offsetClause != "") {
        query.bindValue(":off", offset);
    }

    // Execute and fill return list with resulting data.
    safeExec(query, "Error: " + DBUtil::MACROS_TABLE_NAME + " SELECT failed in getMacroMetadata() with query: " + queryStr);
    while(query.next()) {
        MacroMetadata macroMeta;
        macroMeta.id = query.value("macroId").toInt();
        macroMeta.name = query.value("macroName").toString();
        macroMetaList.append(macroMeta);
    }

    // Don't close DB if it was opened outside of this method call!
    if (!dbOpenOutsideMethod) {
        _db.close();
    }
    return macroMetaList;
}


QString MacroMetaModel::generateMacroCopyName(const QString &srcName)
{
    QString baseSrcName;
    int copyNum = 1;

    // Strip off copy number information of format '(N)' if it exists to get base name.
    if (srcName.at(srcName.size() - 1) == ')') {
        // Base copy name comes before '('.
        baseSrcName = srcName.split('(').first();
    }
    else {
        baseSrcName = srcName;
    }

    // Copies have format "Name (N)" where N is the number of the copy.
    // Get the latest copy of the Macro from the database to find out what copy this is.
    QString queryStr = "SELECT macroName \n"
                       "FROM " + DBUtil::MACROS_TABLE_NAME + "\n"
                       "WHERE macroName like '" + baseSrcName + "%' \n"
                       "ORDER BY macroName DESC \n"
                       "LIMIT 1;";
    QSqlQuery query(_db);
    query.prepare(queryStr);
    safeExec(query, "Error: SELECT failed in generateMacroCopyName() with query: \n" + queryStr);

    if(query.next()) {
        QString lastCopyName = query.value("macroName").toString();
        // See if the last copy in the database is not the original (it is in fact a copy with format "Name (N)").
        // Update the next copy number based off of it if it is a copy.
        if (lastCopyName.at(lastCopyName.size() - 1) == ')') {
            // Strip off ')' and split to get next copy number.
            lastCopyName.remove(lastCopyName.size() - 1, 1);
            copyNum = lastCopyName.split('(').last().toInt() + 1;
        }
    }

    return baseSrcName + "(" + QString::number(copyNum) + ")";
}


QString MacroMetaModel::buildMacroIdWhereClause(const QList<int> &ids) const
{
    QString idWhere = " (";
    foreach (int id, ids) {
        idWhere += "macroId=" + QString::number(id) + " OR ";
    }
    return idWhere.left(idWhere.size() - 4) + ") ";
}
