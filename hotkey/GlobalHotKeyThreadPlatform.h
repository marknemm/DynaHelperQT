#ifndef GLOBALHOTKEYTHREADPLATFORM_H
#define GLOBALHOTKEYTHREADPLATFORM_H


#include <QThread>


/**
 * @brief The GlobalHotKeyThreadPlatform class
 * Pure virtual class that will provide way to uniformally implement hot key code on different platforms.
 */
class GlobalHotKeyThreadPlatform : public QThread
{

    Q_OBJECT

public:
    /**
     * @brief stop
     * Stops the global hot key thread and ensures cleanup is performed.
     */
    virtual void stop() = 0;

signals:
    /**
     * @brief hotKeyEvent
     * To be emitted from derived class whenever a global hot key event is processed.
     */
    void hotKeyEvent();

protected:
    /**
     * @brief run
     * Event loop for monitoring and processing global hot key.
     */
    virtual void run() = 0;
};


#endif // GLOBALHOTKEYTHREADPLATFORM_H
