#include "Controller.h"
#include <QDebug>


const ControllerId SrcControllerInfo::NO_SRC_CONTROLLER_ID = -1;

// Start controller ID value at 1, and increment every time we construct a new Controller!
ControllerId Controller::NEXT_CONTROLLER_ID = 1;


SrcControllerInfo::SrcControllerInfo(ControllerId controllerId,
                                     bool error,
                                     const QString &errorMsg,
                                     QWidget *parentWidget)
: controllerId(controllerId),
  error(error),
  errorMsg(errorMsg),
  parentWidget(parentWidget)
{}


Controller::Controller() :
    CONTROLLER_ID(NEXT_CONTROLLER_ID++), // Increment unique controller ID for new Controller.
    _parentController(nullptr)
{}


void Controller::assumeControlFromParent(Controller *parentController)
{
    _parentController = parentController;
}


void Controller::assumeControlFromChild(const SrcControllerInfo &srcControllerInfo)
{
    if (srcControllerInfo.error) {
        qDebug() << "Assuming control from child controller with error: " << srcControllerInfo.errorMsg;
    }
}


void Controller::surrenderControlToParent(bool deactivateDueToError, const QString &errorMsg)
{
    // If we actually have a parent controller to surrender control to.
    if (_parentController != nullptr) {
        _parentController->assumeControlFromChild(SrcControllerInfo(CONTROLLER_ID, deactivateDueToError, errorMsg));
    }
}
