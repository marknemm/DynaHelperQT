#ifndef GLOBALHOTKEYMONITOR_H
#define GLOBALHOTKEYMONITOR_H


#include <memory>
#include "GlobalHotKeyThreadPlatform.h"


/**
 * @brief The GlobalHotKeyThread class
 * Cross platform registration and monitoring of hot key.
 */
class GlobalHotKeyMonitor : public QObject
{

    Q_OBJECT

public:
    GlobalHotKeyMonitor();
    ~GlobalHotKeyMonitor();
    /**
     * @brief start
     * Starts the thread for global hot key monitoring.
     */
    void start();
    /**
     * @brief stop
     * Stops the thread for global hot key monitoring.
     */
    void stop();

signals:
    /**
     * @brief hotKeyEvent
     * Emmited when hot key is detected.
     */
    void hotKeyEvent();

private:
    /**
     * @brief _platformSpecific
     * Platform specific global hot key registration and monitoring thread.
     */
    std::unique_ptr<GlobalHotKeyThreadPlatform> _platformSpecific;
};

#endif // GLOBALHOTKEYMONITOR_H
