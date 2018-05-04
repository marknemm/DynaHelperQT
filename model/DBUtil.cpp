//#define RECREATE_DB

#include "DBUtil.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QSqlError>


const QString DBUtil::DB_PATH = "QT_DYNA_MACROS";
const QString DBUtil::MACROS_TABLE_NAME = "Macros";
const QString DBUtil::MACRO_EVENTS_TABLE_NAME = "MacroEvents";
const QString DBUtil::MACRO_MOUSE_EVENTS_TABLE_NAME = "MacroMouseEvents";
const QString DBUtil::MACRO_KEYBOARD_EVENTS_TABLE_NAME = "MacroKeyboardEvents";
const QString DBUtil::SCREENSHOT_TABLE_NAME = "Screenshots";
const QString DBUtil::SCREENSHOT_DIR_PATH = "./screenshots/";
const QString DBUtil::DB_CONNECTION_NAME = "MACROS_CONNECTION";
QSqlDatabase DBUtil::_db;


void DBUtil::init()
{
    #ifdef RECREATE_DB
        qDebug() << "Success removing database " << DB_PATH << ": " << QFile::remove(DB_PATH);
    #endif

    _db = QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION_NAME);
    _db.setDatabaseName(DB_PATH);
    if (!_db.open()) {
        qDebug() << "Error: Connection with database " << DB_PATH << " failed!";
        exit(1);
    }
    initTables();
    initScreenshotDir();
    printMacroInfo();
    _db.close();
}


void DBUtil::initTables()
{
    QSqlQuery query(_db);

    // Create Macros Table.
    query.prepare("CREATE TABLE IF NOT EXISTS " + MACROS_TABLE_NAME + " ( \n"
                                                                      "   macroId INTEGER PRIMARY KEY AUTOINCREMENT, \n"
                                                                      "   macroName TEXT NOT NULL, \n"
                                                                      "   createTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP \n"
                                                                      ");");
    safeExec(query, "Error: " + MACROS_TABLE_NAME + " table create failed with query: \n" + query.lastQuery());
    query.prepare("CREATE INDEX IF NOT EXISTS macroNameIndex ON " + MACROS_TABLE_NAME + " (macroName);");
    safeExec(query, "Error: Creation of Index on " + MACROS_TABLE_NAME + ".macroName failed!");

    // Create Sceenshots Table.
    query.prepare("CREATE TABLE IF NOT EXISTS " + SCREENSHOT_TABLE_NAME + " ( \n"
                                                                          "   screenshotId INTEGER PRIMARY KEY, \n" // This is screenshot file name!
                                                                          "   screenshotX INTEGER, \n"
                                                                          "   screenshotY INTEGER, \n"
                                                                          "   screenshotW INTEGER, \n"
                                                                          "   screenshotH INTEGER, \n"
                                                                          "   targetImgType INTEGER, \n"
                                                                          "   cursorType INTEGER \n"
                                                                          " );");
    safeExec(query, "Error: " + SCREENSHOT_TABLE_NAME + " table create failed with query: \n" + query.lastQuery());

    // Create MacroEvents Table.
    query.prepare("CREATE TABLE IF NOT EXISTS " + MACRO_EVENTS_TABLE_NAME + " ( \n"
                                                                            "   macroEventId INTEGER PRIMARY KEY AUTOINCREMENT, \n"
                                                                            "   macroId INTEGER NOT NULL, \n"
                                                                            "   macroEventInd INTEGER NOT NULL, \n"
                                                                            "   macroEventType INTEGER, \n"
                                                                            "   delayMs INTEGER, \n"
                                                                            "   durationMs INTEGER, \n"
                                                                            "   nRepeats INTEGER, \n"
                                                                            "   targetPID VARCHAR(50), \n"
                                                                            "   FOREIGN KEY (macroId) REFERENCES " + MACROS_TABLE_NAME + "(macroId) \n"
                                                                            ");");
    safeExec(query, "Error: " + MACRO_EVENTS_TABLE_NAME + " table create failed with query: \n" + query.lastQuery());
    query.prepare("CREATE INDEX IF NOT EXISTS macroIdIndex ON " + MACRO_EVENTS_TABLE_NAME + " (macroId);");
    safeExec(query, "Error: Creation of Index on " + MACRO_EVENTS_TABLE_NAME + ".macroId failed!");

    // Create MacroMouseEvents Table.
    query.prepare("CREATE TABLE IF NOT EXISTS " + MACRO_MOUSE_EVENTS_TABLE_NAME + " ( \n"
                                                                                  "   macroEventId INTEGER PRIMARY KEY, \n"
                                                                                  "   macroMouseEventType INTEGER NOT NULL, \n"
                                                                                  "   xLoc INTEGER, \n"
                                                                                  "   yLoc INTEGER, \n"
                                                                                  "   wheelDelta INTEGER, \n"
                                                                                  "   screenshotId INTEGER, \n"
                                                                                  "   autoCorrect TINYINT DEFAULT 1, \n"
                                                                                  "   FOREIGN KEY (macroEventId) REFERENCES " + MACRO_EVENTS_TABLE_NAME + "(macroEventId) ON DELETE CASCADE, \n"
                                                                                  "   FOREIGN KEY (screenshotId) REFERENCES " + SCREENSHOT_TABLE_NAME + "(screenshotId) \n"
                                                                                  ");");
    safeExec(query, "Error: " + MACRO_MOUSE_EVENTS_TABLE_NAME + " table create failed with query: \n" + query.lastQuery());
    query.prepare("CREATE INDEX IF NOT EXISTS screenshotIdIndex ON " + SCREENSHOT_TABLE_NAME + " (screenshotId);");
    safeExec(query, "Error: Creation of Index on " + MACRO_MOUSE_EVENTS_TABLE_NAME + ".screenshotFile failed!");

    // Create MacroKeyboardEvents Table.
    query.prepare("CREATE TABLE IF NOT EXISTS " + MACRO_KEYBOARD_EVENTS_TABLE_NAME + " ( \n"
                                                                                     "   macroEventId INTEGER PRIMARY KEY, \n"
                                                                                     "   macrokeyboardEventType INTEGER NOT NULL, \n"
                                                                                     "   keyCode INTEGER, \n"
                                                                                     "   mod1 INTEGER, \n"
                                                                                     "   mod2 INTEGER, \n"
                                                                                     "   capsLock TINYINT, \n"
                                                                                     "   numLock TINYINT, \n"
                                                                                     "   keyString VARCHAR(100), \n"
                                                                                     "   FOREIGN KEY (macroEventId) REFERENCES " + MACRO_EVENTS_TABLE_NAME + "(macroEventId) ON DELETE CASCADE \n"
                                                                                     ");");
    safeExec(query, "Error: " + MACRO_KEYBOARD_EVENTS_TABLE_NAME + " table create failed with query: \n" + query.lastQuery());
}


void DBUtil::initScreenshotDir()
{
    QDir dir(SCREENSHOT_DIR_PATH);
    if (!dir.exists()) {
        dir.mkdir(".");
    }
}


void DBUtil::safeExec(QSqlQuery query, const QString &errMsg, bool exit)
{
    if (!query.exec()) {
        qDebug().noquote().nospace() << errMsg;
        QSqlError sqlErr = query.lastError();
        qDebug() << "SQLite Error: " << sqlErr.text() << "       SQLite Error Code: " << sqlErr.number();
        if (exit) { ::exit(1); }
    }
}


QList<std::tuple<int, QString>> DBUtil::listMacros(int lim, int offset)
{
    QList<std::tuple<int, QString>> outMacros;
    QSqlQuery query(_db);
    QString queryStr = "SELECT macroId, macroName FROM " + MACROS_TABLE_NAME + " LIMIT :limit OFFSET :offset;";
    query.prepare(queryStr);
    query.bindValue(":limit", lim);
    query.bindValue(":offset", offset);
    safeExec(query, "Error: " + MACROS_TABLE_NAME + " batch table select failed in listMacros() with query: \n" + queryStr);

    while (query.next()) {
        int id = query.value("macroId").toInt();
        QString name = query.value("macroName").toString();
        outMacros.append(std::make_tuple(id, name));
    }

    return outMacros;
}


int DBUtil::getNumMacros()
{
    QSqlQuery query(_db);
    query.prepare("SELECT COUNT(*) FROM " + MACROS_TABLE_NAME + ";");
    safeExec(query, "Error: " + MACROS_TABLE_NAME + " table count failed!");

    query.next();
    return query.value("COUNT(*)").toInt();
}


void DBUtil::printMacroInfo()
{
    QList<std::tuple<int, QString>> macroList = listMacros();
    for (QList<std::tuple<int, QString>>::iterator iter = macroList.begin(); iter != macroList.end(); iter++) {
        std::tuple<int, QString> macro = *iter;
        qDebug() << "id: " << QString::number(std::get<0>(macro)) << "     name: " << std::get<1>(macro);
    }

    int numMacros = getNumMacros();
    qDebug() << "Number of Macros: " << QString::number(numMacros);
}
