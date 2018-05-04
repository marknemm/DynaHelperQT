#ifndef KEYLOGGERPLATFORM_H
#define KEYLOGGERPLATFORM_H


#include "IOLoggerListener.h"


/**
 * @brief The IOLoggerPlatform class
 * Platform independent keyboard and mouse logger pure virtual base class.
 */
class IOLoggerPlatform
{
public:

    /**
     * @brief activate
     * Activates the platform independent keyboard and mouse logger.
     */
    virtual void activate() = 0;
    /**
     * @brief deactivate
     * Deactivates the platform independent keyboard and mouse logger.
     */
    virtual void deactivate() = 0;

    /**
     * @brief setEventListener
     * Sets the listener for recorded macro events.
     * @param listener The listener.
     */
    void setEventListener(IOLoggerListener *listener);

protected:

    /**
     * @brief notifyListener
     * Notifies the listener of a received macro event.
     * @param event The received macro event.
     */
    static void notifyListener(MacroEvent &event);

private:

    /**
     * @brief _listener
     * The macro event logging listener.
     */
    static IOLoggerListener *_listener;
};

#endif // KEYLOGGERPLATFORM_H
