#ifndef IOLOGGERLISTENER_H
#define IOLOGGERLISTENER_H


#include "model/MacroEvent.h"


/**
 * @brief The IOLoggerListener class
 * A listener interface for the reception of lower level macro events.
 */
class IOLoggerListener
{
    friend class IOLoggerPlatform;

protected:
    /**
     * @brief _eventRecv
     * Listener method to be called upon the reception of a macro event within the lower level logger.
     * @param event The event received.
     */
    virtual void _eventRecv(MacroEvent &event) = 0;
};


#endif // IOLOGGERLISTENER_H
