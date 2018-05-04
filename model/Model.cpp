#include "Model.h"
#include <QDebug>
#include <QSqlError>


Model::Model(QSqlDatabase db) :
    _db(db)
{
    // Close to start since we will not use right away!
    _db.close();
}


Model::~Model()
{
    // Close in case database is left open.
    if (_db.isOpen()) {
        _db.close();
    }
}


bool Model::safeOpen(const QString &errMsg, bool exit)
{
    bool success = true;
    if (!_db.isOpen()) {
        success = safeExec(_db.open(), errMsg, exit);
        if (success) {
            QSqlQuery query(_db);
            success = safeExec(query.exec("PRAGMA foreign_keys = ON"), errMsg);
        }
    }
    return success;
}


bool Model::safeOpenAndBegin(const QString &errMsg, bool exit)
{
    bool success = safeOpen(errMsg, exit);
    if (success) {
        success = safeExec(_db.transaction(), errMsg, exit);
    }
    return success;
}


bool Model::safeCommitAndClose(const QString &errMsg, bool exit)
{
    bool success = safeExec(_db.commit(), errMsg, exit);
    _db.close();
    return success;
}


bool Model::safeExec(QSqlQuery &query, const QString &errMsg, bool exit) const
{
    bool result = query.exec();
    if (!result) {
        qDebug().noquote().nospace() << "\n" << errMsg << "\n";
        QSqlError sqlErr = query.lastError();
        qDebug() << "SQLite Error: " << sqlErr.text() << "       SQLite Error Code: " << sqlErr.number();
        if (exit) { ::exit(1); }
    }
    return result;
}


bool Model::safeExec(bool result, const QString &errMsg, bool exit) const
{
    if (!result) {
        qDebug().noquote().nospace() << "\n" << errMsg << "\n";
        QSqlError sqlErr = _db.lastError();
        qDebug() << "SQLite Error: " << sqlErr.text() << "       SQLite Error Code: " << sqlErr.number();
        if (exit) { ::exit(1); }
    }
    return result;
}
