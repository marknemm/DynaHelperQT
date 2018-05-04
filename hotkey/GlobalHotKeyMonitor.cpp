#include "GlobalHotKeyMonitor.h"
#include "GlobalHotKeyThreadWin.h"

GlobalHotKeyMonitor::GlobalHotKeyMonitor()
{
    // Windows Platform
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    _platformSpecific = std::unique_ptr<GlobalHotKeyThreadPlatform>(new GlobalHotKeyThreadWin());
    #endif
    // TODO: other platforms...

    // Fire signal from this cross platform wrapper when platform specific signal is fired.
    QObject::connect(_platformSpecific.get(), SIGNAL(hotKeyEvent()), this, SIGNAL(hotKeyEvent()));
}


GlobalHotKeyMonitor::~GlobalHotKeyMonitor()
{}


void GlobalHotKeyMonitor::start()
{
    _platformSpecific->start();
}


void GlobalHotKeyMonitor::stop()
{
    _platformSpecific->stop();
}
