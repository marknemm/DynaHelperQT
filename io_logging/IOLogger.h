#ifndef IOLOGGER_H
#define IOLOGGER_H


#include "IOLoggerListener.h"
#include "IOLoggerPlatform.h"
#include <QObject>
#include <memory>


/**
 * @brief The IOLogger class
 * Singleton class for platform independent global mouse and keyboard event logger.
 */
class IOLogger : public QObject, IOLoggerListener
{
    Q_OBJECT

public:

    /**
     * @brief generateLogger
     * Singleton generator static method that ensures only one instance of logger is generated.
     * @return The logger instance.
     */
    static IOLogger& generateLogger();

    /**
     * @brief activate
     * Activates the logger.
     */
    void activate();
    /**
     * @brief deactivate
     * Deactivates the logger.
     */
    void deactivate();

signals:

    /**
     * @brief eventRecv
     * Emitted whenever a macro event is received from the lower level logger.
     * @param event The macro event received.
     */
    void eventRecv(MacroEvent &event);

private:

    /**
     * @brief _ioLoggerPlat
     * The platform specific lower level logger.
     */
    std::unique_ptr<IOLoggerPlatform> _ioLoggerPlat;

    /**
     * @brief _eventRecv
     * Listener method to be called upon the reception of a macro event within the lower level logger.
     * @param event The event received.
     */
    void _eventRecv(MacroEvent &event) override; // Visible to IOLoggerPlatform!

    // Don't expose any constructor, copy, or assignment mechanisms due to singleton nature!
    IOLogger();
    IOLogger(const IOLogger &copy);
    void operator=(const IOLogger &rhs);
};


#endif // IOLOGGER_H
