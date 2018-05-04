#include "MacroMenuController.h"
#include "view/macro_menu/MacroMenuContextMenu.h"
#include <QDebug>
#include <QMessageBox>


MacroMenuController::MacroMenuController() :
    Controller(),
    RETRIEVAL_SEG_SIZE(100),
    // Models
    _macroEventModel(),
    _macroMetaModel(_macroEventModel),
    // View
    _macroMenu((MacroMenuEventListener&)*this, _macroMetaModel.getMacroNameRegex()),
    _hotKeyMonitor(),
    // Child Controllers
    _ioLoggingController(_hotKeyMonitor),
    _macroActivationController(_hotKeyMonitor, _macroEventModel),
    _macroAddController(_ioLoggingController, _macroMetaModel, _macroEventModel),
    _macroEditorController(_hotKeyMonitor, _ioLoggingController, _macroEventModel)
{
    _hotKeyMonitor.start();
}


void MacroMenuController::assumeControlFromParent(Controller *parentController)
{
    Controller::assumeControlFromParent(parentController);
    connect(&_hotKeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(handleHotKey()));
    syncViewWithModel();
}


void MacroMenuController::assumeControlFromChild(const SrcControllerInfo &srcControllerInfo)
{
    Controller::assumeControlFromChild(srcControllerInfo); // Sets _active flag to true.
    connect(&_hotKeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(handleHotKey()));

    // If we are coming back from Macro Activation.
    if (srcControllerInfo.controllerId == _macroActivationController.CONTROLLER_ID) {
        _macroMenu.showMinimized();
    }
    // If we are coming back from Macro Creation IO logging.
    else if (srcControllerInfo.controllerId == _macroAddController.CONTROLLER_ID) {
        syncViewWithModel(ID_DESC, true); // Sort ID Descending b/c new Macro will have largest ID
    }
    // If we are coming back from any other child controller or a parent.
    else {
        syncViewWithModel();
    }

    // Display error message to user if there is one.
    if (srcControllerInfo.error) {
        QMessageBox::information(&_macroMenu, "Error Message", "An error occured with message: "
                                 + (srcControllerInfo.errorMsg.size() > 0 ? srcControllerInfo.errorMsg
                                                                          : "<empty message>"));
    }
}


void MacroMenuController::surrenderControl()
{
    disconnect(&_hotKeyMonitor, SIGNAL(hotKeyEvent()), this, SLOT(handleHotKey()));
    _macroMenu.hide();
}


void MacroMenuController::handleHotKey()
{
    _macroMenu.show();
}


void MacroMenuController::activateMacro(int id)
{
    surrenderControl();
    _macroActivationController.assumeControlFromParent(id, this);
}


void MacroMenuController::createNewMacro(const QString &name)
{
    surrenderControl();
    _macroAddController.assumeControlFromParent(name, this);
}


void MacroMenuController::renameMacro(int id, const QString &name)
{
    _macroMetaModel.setMacroName(id, name);
}


void MacroMenuController::editMacros(const QList<int> &ids)
{
    surrenderControl();
    _macroEditorController.assumeControlFromParent(ids, this);
}


void MacroMenuController::removeMacros(const QList<int> &ids)
{
    _macroMetaModel.removeMacros(ids);
}


QList<MacroMetadata> MacroMenuController::copyMacros(const QList<int> &ids)
{
    return _macroMetaModel.copyMacros(ids);
}


QList<MacroMetadata> MacroMenuController::requestMacroMetadataWithFilter(const QString &nameOrIdFilter, int offset,
                                                                         MacroMetadataSortOrder sortOrder)
{
    return requestMacroMetadataWithFilter(nameOrIdFilter, offset, RETRIEVAL_SEG_SIZE, sortOrder);
}


QList<MacroMetadata> MacroMenuController::requestMacroMetadataWithFilter(const QString &nameOrIdFilter, int offset, int limit,
                                                                         MacroMetadataSortOrder sortOrder)
{
    return _macroMetaModel.getMacroMetadata(nameOrIdFilter, offset, limit, sortOrder);
}


void MacroMenuController::syncViewWithModel(MacroMetadataSortOrder sortOrder, bool selectFirstRow)
{
    // Refresh the view based off of latest Model data!
    QList<MacroMetadata> macroMetaList = _macroMetaModel.getMacroMetadata("", 0, RETRIEVAL_SEG_SIZE, sortOrder);
    _macroMenu.refresh(macroMetaList, sortOrder, selectFirstRow);
}
