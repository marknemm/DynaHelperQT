#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H


#include "controller/Controller.h"
#include "MacroMenuEventListener.h"
#include "model/MacroMetaModel.h"
#include "model/MacroEventModel.h"
#include "view/macro_menu/MacroMenu.h"
#include "hotkey/GlobalHotKeyMonitor.h"
#include "controller/io_logging/IOLoggingController.h"
#include "controller/macro_activation/MacroActivationController.h"
#include "controller/macro_add/MacroAddController.h"
#include "controller/macro_editor/MacroEditorController.h"


/**
 * @brief The MacroMenuController class
 * The macro menu controller class. Primary controller associated with the main macro menu GUI view.
 */
class MacroMenuController : public Controller, MacroMenuEventListener
{
    Q_OBJECT

public:

    MacroMenuController();

    /**
     * @brief acivateMacro
     * Activates the selected macro.
     * @param id
     * The ID of the macro to activate.
     */
    void activateMacro(int id) override;

    /**
     * @brief createNewMacro
     * Initializes the creation of a new macro.
     * @param name
     * The name of the new macro.
     */
    void createNewMacro(const QString &name) override;

    /**
     * @brief renameMacro
     * Initializes the renaming of a macro.
     * @param id
     * The ID of the macro to rename.
     * @param name
     * The new name for the macro.
     */
    void renameMacro(int id, const QString &name) override;

    /**
     * @brief editMacros
     * Initializes macro editing.
     * @param ids
     * A list of the IDs of macros to be edited.
     */
    void editMacros(const QList<int> &ids) override;

    /**
     * @brief removeMacros
     * Initializes macro removal.
     * @param ids
     * A list of IDs of macros to be removed.
     */
    void removeMacros(const QList<int> &ids) override;

    /**
     * @brief copyMacro
     * Initializes macro copying.
     * @param ids
     * A list of IDs of macros to be copied.
     * @return
     * A list of the result of the copy operation.
     */
    QList<MacroMetadata> copyMacros(const QList<int> &ids) override;

    /**
     * @brief requestMacroMetadataWithFilter
     * Requests Macro metadata with optional filter values.
     * @param nameOrIdFilter (OPTIONAL)
     * The macro id or name to filter by. Default is empty string for no filter.
     * @param offset (OPTIONAL)
     * The offset of the start point macro metadata records to be requested. Default is 0 for no offset.
     * @param sortOrder (OPTIONAL)
     * The order by which Macro metadata records will be sorted in. Default is sorting by ID number in ascending order.
     * @return
     * A list of the Macro metadata corresponding to the request.
     */
    QList<MacroMetadata> requestMacroMetadataWithFilter(const QString &nameOrIdFilter = "", int offset = 0,
                                                        MacroMetadataSortOrder sortOrder = ID_ASC) override;

    /**
     * @brief requestMacroMetadataWithFilter
     * Requests Macro metadata with optional filter values.
     * @param nameOrIdFilter (OPTIONAL)
     * The macro id or name to filter by. Default is empty string for no filter.
     * @param offset (OPTIONAL)
     * The offset of the start point macro metadata records to be requested. Default is 0 for no offset.
     * @param limit (OPTIONAL)
     * The limit on the number of macro metadata records to be requested. Default is -1 for no limit (request all).
     * @param sortOrder (OPTIONAL)
     * The order by which Macro metadata records will be sorted in. Default is sorting by ID number in ascending order.
     * @return
     * A list of the Macro metadata corresponding to the request.
     */
    QList<MacroMetadata> requestMacroMetadataWithFilter(const QString &nameOrIdFilter = "", int offset = 0, int limit = -1,
                                                        MacroMetadataSortOrder sortOrder = ID_ASC) override;

public slots:

    /**
     * @brief assumeControlFromParent
     * Should be invoked whenever a controller assumes control from its parent controller and becomes active.
     * @param parentController (OPTIONAL)
     * The parent of this controller. Will be used when this child controller surrenders control back to the parent.
     */
    void assumeControlFromParent(Controller *parentController = nullptr) override;

    /**
     * @brief assumeControlFromChild
     * Should be invoked whenever a controller assumes control from a child controller and becomes active.
     * @param srcControllerInfo (OPTIONAL)
     * Info pertaining to the source child controller that caused the activation of this controller. See definition of SrcControllerInfo
     * struct for more information.
     */
    void assumeControlFromChild(const SrcControllerInfo &srcControllerInfo = SrcControllerInfo()) override;

private slots:

    /**
     * @brief surrenderControl
     * Invoked whenever this controller shall surrender control to a child controller.
     */
    void surrenderControl();

    /**
     * @brief handleHotKey
     * Handles the global hot key event. Refocuses the main menu.
     */
    void handleHotKey();

private:

    /**
     * @brief RETRIEVAL_SEG_SIZE
     * The size of the segments that the View should retrieve when it makes requests for more Macro Metadata.
     * Tells the view how much Macro Metadata to retrieve at a time.
     */
    const int RETRIEVAL_SEG_SIZE;

    /**
     * @brief _macroMetaModel
     * A model for the Macro metadata.
     */
    MacroMetaModel _macroMetaModel;

    /**
     * @brief _macroEventModel
     * A model for Macro Events
     */
    MacroEventModel _macroEventModel;

    /**
     * @brief _macroMenu
     * The Main Menu GUI view.
     */
    MacroMenu _macroMenu;

    /**
     * @brief _hotKeyMonitor
     * Monitor for global hot key events.
     */
    GlobalHotKeyMonitor _hotKeyMonitor;

    /**
     * @brief _ioLoggingController
     * Handles the logging of Global Mouse and Keyboard IO Events.
     */
    IOLoggingController _ioLoggingController;

    /**
     * @brief _macroActivationController
     * Controller for Macro activation.
     */
    MacroActivationController _macroActivationController;

    /**
     * @brief _macroAddController
     * Controller for adding a new Macro.
     */
    MacroAddController _macroAddController;

    /**
     * @brief _macroEditorController
     * Controller for Macro editing.
     */
    MacroEditorController _macroEditorController;


    /**
     * @brief initListeners
     * Initializes the listeners for the controller.
     */
    void initListeners();

    /**
     * @brief syncViewWithModel
     * Synchronizes the view with the model data.
     * @param sortOrder (OPTIONAL)
     * The order in which to retrieve the Macro Metadata to feed to the view refresh method.
     * @param selectFirst (OPTIONAL)
     * Set true to instruct the view to select the first element if there is one.
     */
    void syncViewWithModel(MacroMetadataSortOrder sortOrder = ID_ASC, bool selectFirst = false);
};


#endif // MAINCONTROLLER_H
