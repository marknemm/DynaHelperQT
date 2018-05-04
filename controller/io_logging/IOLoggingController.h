#ifndef IOLOGGINGCONTROLLER_H
#define IOLOGGINGCONTROLLER_H


#include "controller/Controller.h"
#include "view/io_logging/IOLoggingInfo.h"
#include "io_logging/IOLogger.h"
#include "hotkey/GlobalHotKeyMonitor.h"
#include "model/MacroEvent.h"
#include "model/proxy/MacroEventLogBuffer.h"


/**
 * @brief The IOLoggingController class
 * Controller for IO Logging. Child Controller of Main Menu Controller.
 */
class IOLoggingController : public Controller
{
    Q_OBJECT

public:

    explicit IOLoggingController(GlobalHotKeyMonitor &hotkeyMonitor);

    /**
     * @brief takeAddedMacroEvents
     * Takes ownership of the added Macro Events that were recorded when the IOLoggingController was last active.
     * Should only be called after the deactivation of this controller from a parent (controller).
     * @return
     * The Macro Events that were added during the last active period of this controller.
     */
    QList<MacroEvent> takeAddedMacroEvents();

public slots:

    /**
     * @brief assumeControlFromParent
     * Should be called whenever this controller is given control from its parent controller.
     * @param insertInd
     * The index where events will be inserted within the active Macros.
     * @param parentController
     * The parent controller which will have control handed back to it when this controller surrenders control.
     */
    void assumeControlFromParent(int insertInd, Controller *parentController);


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
     * Handles a received macro event form the IO logger.
     * @param event
     * The received macro event.
     */
    void handleMacroEvent(MacroEvent &event);

private:

    /**
     * @brief _hotkeyMonitor
     * Global hotkey monitor.
     */
    GlobalHotKeyMonitor &_hotkeyMonitor;

    /**
     * @brief _macroEventLogBuffer
     * Buffer for logged Macro Events. All events in this buffer will eventually be taken
     * so that they may be committed to the underlying model. This basically holds them in
     * main memory until then. It also does a lot of preprocessing on raw logged events.
     */
    MacroEventLogBuffer _macroEventLogBuffer;

    /**
     * @brief _loggingInfo
     * Logging info GUI view.
     */
    IOLoggingInfo _loggingInfo;

    /**
     * @brief _logger
     * The IO mouse and keyboard events logger.
     */
    IOLogger &_logger;
};


#endif // IOLOGGINGCONTROLLER_H
