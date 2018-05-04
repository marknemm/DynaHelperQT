#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <QObject>
#include <QWidget>


typedef int ControllerId;


/**
 * @brief SrcControllerInfo
 * Wrapper for information pertaining to
 */
typedef struct SrcControllerInfo
{
    /**
     * @brief NO_SRC_CONTROLLER_ID
     * Maps to a negative integer. Signifies that there is no srcControllerId
     */
    static const ControllerId NO_SRC_CONTROLLER_ID;

    /**
     * @brief controllerId
     * The ID of the controller that is the source of some event in another controller (such as activate()).
     */
    ControllerId controllerId;

    /**
     * @brief error
     * True if the source controller encountered an error before invoking an event in the target controller,
     * and false if no error.
     */
    bool error;

    /**
     * @brief errorMsg
     * A brief message describing the error that occured if one did occur.
     * See error for more information.
     */
    QString errorMsg;

    /**
     * @brief parentWidget
     * The parent widget for the UI component owned by the target controller. Usually will be nullptr.
     * Used mostly for modal dialog windows.
     */
    QWidget *parentWidget;

    explicit SrcControllerInfo(ControllerId controllerId = NO_SRC_CONTROLLER_ID, bool error = false,
                               const QString &errorMsg = "", QWidget *parentWidget = nullptr);
} SrcControllerInfo;


/**
 * @brief The Controller class
 * An abstract class that defines basic controller functionality.
 */
class Controller : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief CONTROLLER_ID
     * The unique ID belonging to this controller. Primarily used to identify which controller caused a controller
     * to activate when the activate() method is called.
     */
    const ControllerId CONTROLLER_ID;


    explicit Controller();


public slots:

    /**
     * @brief assumeControlFromParent
     * Should be invoked whenever a controller assumes control from its parent controller and becomes active.
     * @param parentController (OPTIONAL)
     * The parent of this controller. Will be used when this child controller surrenders control back to the parent.
     */
    virtual void assumeControlFromParent(Controller *parentController = nullptr);


    /**
     * @brief assumeControlFromChild
     * Should be invoked whenever a controller assumes control from a child controller and becomes active.
     * @param srcControllerInfo (OPTIONAL)
     * Info pertaining to the source child controller that caused the activation of this controller. See definition of SrcControllerInfo
     * struct for more information.
     */
    virtual void assumeControlFromChild(const SrcControllerInfo &srcControllerInfo);


protected slots:

    /**
     * @brief surrenderControlToParent
     * Called whenever this controller wishes to surrender control to the parent.
     * @param deactivateDueToError (OPTIONAL)
     * Set to true if an error occured which caused this method to be called. Otherwise use default of false for no error.
     * @param errorMsg (OPTIONAL)
     * Set to a brief description of the error that occured if one did occur. Otherwise, leave empty by default.
     */
    virtual void surrenderControlToParent(bool deactivateDueToError = false, const QString &errorMsg = "");


private:

    /**
     * @brief NEXT_CONTROLLER_ID
     * The next ID avialble to be given to an instance of a Controller.
     * Increments after each instantiation of a new controller object.
     */
    static ControllerId NEXT_CONTROLLER_ID;

    /**
     * @brief _parentController
     * The parent of this controller.
     */
    Controller *_parentController;
};


#endif // CONTROLLER_H
