#ifndef DBUtil_H
#define DBUtil_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPoint>
#include <QList>
#include <tuple>
#include <QPair>
#include <QSqlTableModel>
#include "MacroEvent.h"


/**
 * @brief The DBUtil class
 *  Utility for initialization and general information pertaining to database.
 */
class DBUtil
{

public:

    const static QString DB_PATH;
    const static QString MACROS_TABLE_NAME;
    const static QString MACRO_EVENTS_TABLE_NAME;
    const static QString MACRO_MOUSE_EVENTS_TABLE_NAME;
    const static QString MACRO_KEYBOARD_EVENTS_TABLE_NAME;
    const static QString SCREENSHOT_TABLE_NAME;
    const static QString SCREENSHOT_DIR_PATH;
    const static QString DB_CONNECTION_NAME;

    /**
     * @brief init
     * Initializes the database.
     */
    static void init();

private:

    /**
     * @brief initDatabase
     * Initializes the database.
     */
    static void initDatabase();
    /**
     * @brief initTables
     * Initializes the tables within the database.
     */
    static void initTables();
    /**
     * @brief initScreenshotDir
     * Creates the screenshot directory if it does not exist.
     */
    static void initScreenshotDir();

    /**
     * @brief safeExec
     * Performs a safe execution of a QSqlite query. A safe execution will check for any error and output
     * all detected error information.
     * @param query The query to execute.
     * @param errMsg A custom error message to display in addition to any sqlite specific error information.
     * @param exit A flag to be set true if the program should terminate upon an error, false otherwise.
     */
    static void safeExec(QSqlQuery query, const QString &errMsg, bool exit=true);

    /**
     * @brief listMacros
     * Forms a list of the macros within the database.
     * @param lim A limit for the number of macros to be added to the outMacros list.
     * @param offset An offset from which the retrieval of macros from the database should start.
     * @return List filled with the database macros.
     */
    static QList<std::tuple<int, QString>> listMacros(int lim=-1, int offset=0);
    /**
     * @brief getNumMacros
     * Gets the number of macros in the database.
     * @return The number of macros.
     */
    static int getNumMacros();
    /**
     * @brief printMacroInfo
     * Debugging convenience function used to print all macros in database.
     */
    static void printMacroInfo();

    /**
     * @brief _db
     * The database connection.
     */
    static QSqlDatabase _db;

    // Static utility class; no constructors, no copies!
    DBUtil();
    DBUtil(const DBUtil &copyFrom){}
    DBUtil& operator=(const DBUtil &rhs){}
};

#endif // DBUtil_H
