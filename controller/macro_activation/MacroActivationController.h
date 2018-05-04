#ifndef MacroActivationController_H
#define MacroActivationController_H


#include "model/MacroEventModel.h"
#include "controller/Controller.h"
#include "view/io_logging/IOLoggingInfo.h"
#include "macro_activation/MacroActivator.h"
#include "hotkey/GlobalHotKeyMonitor.h"
#include "model/MacroEvent.h"


Q_DECLARE_METATYPE(MacroEvent)


/**
 * @brief The MacroActivationController class
 * Contains functionality for the activation of a Macro record.
 */
class MacroActivationController : public Controller
{
    Q_OBJECT

public:

    MacroActivationController(GlobalHotKeyMonitor &hotkeyMonitor, MacroEventModel &macroEventModel);


public slots:

    /**
     * @brief activate
     * Invoked whenever the controller is given control by the parent.
     * @param macroId
     * The ID of the Macro to activate.
     * @param parentController
     * The parent controller. Used to hand control back to the parent when finished.
     */
    void assumeControlFromParent(int macroId, Controller *parentController);


private slots:

    /**
     * @brief surrenderControlToParent
     * Called whenever this controller wishes to surrender control to the parent.
     * @param deactivateDueToError (OPTIONAL)
     * Set to true if an error occured which caused this method to be called. Otherwise use default of false for no error.
     * @param errorMsg (OPTIONAL)
     * Set to a brief description of the error that occured if one did occur. Otherwise, leave empty by default.
     */
    void surrenderControlToParent(bool deactivateDueToError = false, const QString &errorMsg = "") override;

    /**
     * @brief handleMacroEvent
     * Handles the display of info for a Macro Event that is currently being activated.
     * @param event
     * The Macro Event that is being activated.
     */
    void handleMacroEvent(const MacroEvent &event);


private:

    /**
     * @brief _hotkeyMonitor
     * Monitor for global hotkey events.
     */
    GlobalHotKeyMonitor &_hotkeyMonitor;

    /**
     * @brief _macroEventModel
     * Model for Macro Events. Used to retrieve all Macro Event data for
     * activation of a Macro record.
     */
    MacroEventModel &_macroEventModel;

    /**
     * @brief _loggingInfo
     * Logging info GUI view.
     */
    IOLoggingInfo _loggingInfo;

    /**
     * @brief _macroActivator
     * The driver behind Macro activation or playback.
     */
    MacroActivator _macroActivator;
};


#endif // MacroActivationController_H
