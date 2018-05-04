#include "IOLogger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "IOLoggerWin.h"
#endif


IOLogger& IOLogger::generateLogger()
{
    static IOLogger singletonLogger;
    return singletonLogger;
}


void IOLogger::activate()
{
    _ioLoggerPlat->activate();
}


void IOLogger::deactivate()
{
    _ioLoggerPlat->deactivate();
}


void IOLogger::_eventRecv(MacroEvent &event)
{
    emit eventRecv(event);
}


IOLogger::IOLogger()
{
    // Windows specific IO (mouse and keyboard) event logger.
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    _ioLoggerPlat = std::unique_ptr<IOLoggerPlatform>(new IOLoggerWin());
    #endif
    // TODO: Other platform sepcific IO event loggers.
    _ioLoggerPlat->setEventListener(this);
}
