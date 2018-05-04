#include "IOLoggerPlatform.h"


IOLoggerListener *IOLoggerPlatform::_listener = nullptr;


void IOLoggerPlatform::setEventListener(IOLoggerListener *listener)
{
    _listener = listener;
}


void IOLoggerPlatform::notifyListener(MacroEvent &event)
{
    if (_listener != nullptr) {
        _listener->_eventRecv(event);
    }
}
