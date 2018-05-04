#ifndef MACROADDCONTROLLER_H
#define MACROADDCONTROLLER_H


#include "controller/Controller.h"
#include "controller/io_logging/IOLoggingController.h"
#include "model/MacroMetaModel.h"
#include "model/MacroEventModel.h"


/**
 * @brief The MacroAddController class
 * Controller for the Macro Add task.
 */
class MacroAddController : public Controller
{
    Q_OBJECT


public:

    explicit MacroAddController(IOLoggingController &ioLoggingController,
                                MacroMetaModel &macroMetaModel,
                                MacroEventModel &macroEventModel);


public slots:

    /**
     * @brief assumeControlFromParent
     * Should be invoked whenever a controller assumes control from its parent controller.
     * @param macroName
     * The name of the Macro that is being added.
     * @param srcControllerInfo (OPTIONAL)
     * Info pertaining to the source controller that caused the activation of this controller. See definition of SrcControllerInfo
     * struct for more information.
     */
    void assumeControlFromParent(const QString &macroName, Controller *parentController = nullptr);

    /**
     * @brief assumeControlFromChild
     * Should be invoked whenever a controller assumes control from a child controller and becomes active.
     * @param srcControllerInfo (OPTIONAL)
     * Info pertaining to the source child controller that caused the activation of this controller. See definition of SrcControllerInfo
     * struct for more information.
     */
    void assumeControlFromChild(const SrcControllerInfo &srcControllerInfo) override;


private:

    /**
     * @brief _ioLoggingController
     * Controller for IOLogging (Macro Event logging).
     */
    IOLoggingController &_ioLoggingController;

    /**
     * @brief _macroMetaModel
     * A model for the Macro metadata.
     */
    MacroMetaModel &_macroMetaModel;

    /**
     * @brief _macroEventModel
     * A model for Macro Events
     */
    MacroEventModel &_macroEventModel;
};

#endif // MACROADDCONTROLLER_H
