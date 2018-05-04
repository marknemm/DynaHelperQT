#include "MacroAddController.h"


// Initialize our dependencies here!
MacroAddController::MacroAddController(IOLoggingController &ioLoggingController,
                                       MacroMetaModel &macroMetaModel,
                                       MacroEventModel &macroEventModel)
: _ioLoggingController(ioLoggingController),
  _macroMetaModel(macroMetaModel),
  _macroEventModel(macroEventModel)
{}


void MacroAddController::assumeControlFromParent(const QString &macroName, Controller *parentController)
{
    Controller::assumeControlFromParent(parentController);

    int idResult = _macroMetaModel.addMacro(macroName);
    bool addSuccess = (idResult != -1);

    if (addSuccess) {
        _macroEventModel.setActiveMacro(idResult);
        // Switch to IO Logging Controller for Macro Event recording.
        _ioLoggingController.assumeControlFromParent(-1, this);
    }
    else {
        // Should not happen since the validator in macro menu view should prevent incorrect input!
        surrenderControlToParent(true, "Error: failed to add Macro metadata");
    }
}


void MacroAddController::assumeControlFromChild(const SrcControllerInfo &srcControllerInfo)
{
    if (!srcControllerInfo.error) {
        _macroEventModel.addEvents(_ioLoggingController.takeAddedMacroEvents());
    }
    surrenderControlToParent(srcControllerInfo.error, srcControllerInfo.errorMsg);
}
