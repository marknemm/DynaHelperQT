#ifndef MODEL_H
#define MODEL_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>


/**
 * @brief The Model class
 * Base Model class to be used by all models.
 */
class Model
{

protected:

    /**
     * @brief _db
     * The SQL database connection associated with the model.
     */
    QSqlDatabase _db;

public:

    /**
     * @brief Model
     * Constructor.
     * @param db The SQL database connection that the model will use.
     */
    explicit Model(QSqlDatabase db);
    ~Model();

    /**
     * @brief safeOpen
     * Safely opens the SQL database connection so that queries can be performed.
     * @param errMsg The custom error message to display upon failure in addition to default error information.
     * @param exit (OPTIONAL) Set false to not terminate the program upon error. Default true will terminate program upon error.
     * @return A success flag of true if the operation is successful, false if it is unsuccessful and an error occured.
     */
    bool safeOpen(const QString &errMsg, bool exit=true);

    /**
     * @brief safeOpenAndBegin
     * Safely opens the SQL database connection so that queries can be performed. Also, safely begins a transaction in the database.
     * @param errMsg See safeOpen()
     * @param exit See safeOpen()
     * @return See safeOpen()
     */
    bool safeOpenAndBegin(const QString &errMsg, bool exit=true);

    /**
     * @brief safeCommitAndClose
     * Safely commits all queries in a running SQL transaction and closes the database connection.
     * @param errMsg See safeOpen()
     * @param exit See safeOpen()
     * @return See safeOpen()
     */
    bool safeCommitAndClose(const QString &errMsg, bool exit=true);


protected:

    /**
     * @brief safeExec
     * Checks the evaluation result of the execution of SQL Database queries.
     * @param query The query to safely execute.
     * @param errMsg See safeOpen()
     * @param exit See safeOpen()
     * @return See safeOpen()
     */
    bool safeExec(QSqlQuery &query, const QString &errMsg, bool exit=true) const;

    /**
     * @brief safeExec
     * Checks the evaluation result of the execution of SQL Database operations.
     * @param result The result of an execution.
     * @param errMsg See safeOpen()
     * @param exit See safeOpen()
     * @return See safeOpen()
     */
    bool safeExec(bool result, const QString &errMsg, bool exit=true) const;
};


#endif // MODEL_H
