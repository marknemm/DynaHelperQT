#ifndef GLOBALHOTKEYTHREADWIN_H
#define GLOBALHOTKEYTHREADWIN_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "GlobalHotKeyThreadPlatform.h"


/**
 * @brief The GlobalHotKeyThreadWin class
 * Windows specific code for registering and monitoring hot key.
 */
class GlobalHotKeyThreadWin : public GlobalHotKeyThreadPlatform
{

    Q_OBJECT

public:
    GlobalHotKeyThreadWin();
    ~GlobalHotKeyThreadWin();
    /**
     * @brief stop
     * Stops the monitoring for the global hot key.
     */
    void stop() override;

private:
    /**
     * @brief run
     * Event loop for monitoring and processing global hot key.
     */
    void run() override;
//    volatile bool m_stopped;
    /**
     * @brief _mThreadId
     * The ID of the global hot key thread once thread has started.
     * If no thread has been started, then set to 0.
     */
    DWORD _mThreadId;
};


#endif


#endif // GLOBALHOTKEYTHREADWIN_H
