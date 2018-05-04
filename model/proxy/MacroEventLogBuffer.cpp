#include "MacroEventLogBuffer.h"
#include "record_img/RecordImageUtil.h"
#include "model/DBUtil.h"
#include "model/MacroEventModel.h"
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>
#include <QFileInfo>
#include <QDebug>


const int MacroEventLogBuffer::TYPE_CLICK_TIME_THRESH = 200;


MacroEventLogBuffer::MacroEventLogBuffer()
     : _run(true),
       _nextEventInd(0),
       _events(),
       _producerConsumerQueue()
{
    // Start up our consumer thread loop.
    QtConcurrent::run(this, &MacroEventLogBuffer::processMacroEventsLoop);
}


MacroEventLogBuffer::~MacroEventLogBuffer()
{
    // Make sure that we stop consumer thread loop if we go out of scope!
    _run = false;
}


void MacroEventLogBuffer::setProxyInsertInd(int insertInd)
{
    _nextEventInd = insertInd;
}


void MacroEventLogBuffer::addEvent(MacroEvent &event)
{
    // Must set timestampe here to accurately reflect when event occured.
    // It is accurate here because we are still on system event thread from IO Logging!
    event.timestampMs = QDateTime::currentMSecsSinceEpoch();
    // Make sure we take screenshot before processing in another thread!
    // We must ensure that we get state of system UI before mouse event is processed in main system event thread!
    takeScreenshotIfLocationSensitiveMouseEvent(event);
    _producerConsumerQueue.enqueue(event);
}


QList<MacroEvent> MacroEventLogBuffer::takeAddedEvents(int numEventsToPop)
{
    // TODO: Block until our consumer thread is done processing all added IO events!

    // Remove all of the unwanted events (likely associated with stopping IO Logging ctrl + w).
    for(int i = 0; i < numEventsToPop && !_events.isEmpty(); i++) {
        _events.pop_back();
    }

    // Post process _events list to try to condense any keyboard stroke event chains and add event indexes.
    formKeyStringsAndSetEventIndexes();

    // Transfer events out to be taken.
    QList<MacroEvent> takenEvents = _events;
    _events.clear();
    return takenEvents;
}


void MacroEventLogBuffer::processMacroEventsLoop()
{
    // Our consumer thread loop.
    while (_run)
    {
        // Thread sleeps here until an event is made ready by a producer thread.
        MacroEvent event = _producerConsumerQueue.dequeue();

        // If we are not running consumer thread any more.
        if (!_run) {  break;  }

        // Time since the last event was added.
        int timeSinceLastEventAddMs = _events.size() == 0 ? 0
                                                          : (event.timestampMs - _events.last().timestampMs);
        bool addMacro = false;

        /*qDebug() << "Event type: " << getMacroEventTypeStr(event.type);
        if (event.type == MacroEventType::MouseEvent) {
            qDebug() << "Mouse Event Type: " << getMacroMouseEventTypeStr(event.mouseEvent.type);
        }
        else {
            qDebug() << "Keyboard Event Type: " << getMacroKeyboardEventTypeStr(event.keyboardEvent.type);
        }*/

        // Process and filter events based off of Event Type.
        switch(event.type) {
        case MouseEvent:    addMacro = processMouseEvent(event.mouseEvent, timeSinceLastEventAddMs);        break;
        case KeyboardEvent: addMacro = processKeyboardEvent(event.keyboardEvent, timeSinceLastEventAddMs);  break;
        default:
            qDebug() << "Error: Macro Event Type " << event.type << " undefined";
            exit(1);
        }

        // If we should potentially add the Macro Event to underlying Model.
        if (addMacro) {
            addEventToList(event, timeSinceLastEventAddMs);
        }
    }
}


bool MacroEventLogBuffer::processMouseEvent(MacroMouseEvent &mEvent, int timeSinceLastEventAddMs)
{
    bool addMacro = false;

    // Form left click.
    if (isLeftClick(mEvent, timeSinceLastEventAddMs)) {
        _events.last().mouseEvent.type = MacroMouseEventType::LeftClick;
        if (areLastTwoEventsDoubleClick()) {
            // Remove the second single click that composes the double click and its associated screenshot!
            QString screenshotPath = MacroEventModel::generateScreenshotPathFromId(_events.last().mouseEvent.screenshotId);
            QFile deleteScreenshot(screenshotPath);
            if (deleteScreenshot.exists()) {
                deleteScreenshot.remove();
            }
            _events.removeLast();
            _events.last().mouseEvent.type = MacroMouseEventType::DoubleClick;
        }
    }
    // Form right click.
    else if (isRightClick(mEvent, timeSinceLastEventAddMs)) {
        _events.last().mouseEvent.type = MacroMouseEventType::RightClick;
    }
    // Form middle click.
    else if (isMiddleClick(mEvent, timeSinceLastEventAddMs)) {
        _events.last().mouseEvent.type = MacroMouseEventType::MiddleClick;
    }
    // Form continuation of scroll up or down.
    else if (isContinuationOfScroll(mEvent)) {
        _events.last().nRepeats++;
        _events.last().durationMs = timeSinceLastEventAddMs;
    }
    // Exclude mouse move events.
    else if (mEvent.type != MacroMouseEventType::Move) {
        addMacro = true;
        // These two method calls will possibly modify mEvent screenshot data!
        isolateTargetImgIfScreenshotTaken(mEvent);
        //saveScreenshotIfTakenAndFreeScreenshotMemory(mEvent);
    }

    return addMacro;
}


bool MacroEventLogBuffer::processKeyboardEvent(MacroKeyboardEvent &kEvent, int timeSinceLastEventAddMs)
{
    bool addMacro = false;

    // Form key type.
    if (isKeyType(kEvent, timeSinceLastEventAddMs)) {
        _events.last().keyboardEvent.type = MacroKeyboardEventType::KeyType;
    }
    // Form continuation of a key press.
    else if (isContinuationOfPress(kEvent)) {
        _events.last().durationMs = timeSinceLastEventAddMs;
        _events.last().nRepeats++;
    }
    else {
        // Exclude keyboard modifier key events.
        addMacro = kEvent.keyCode != Qt::Key_Shift;
        addMacro = addMacro && kEvent.keyCode != Qt::Key_Control;
        addMacro = addMacro && kEvent.keyCode != Qt::Key_Alt;
        addMacro = addMacro && kEvent.keyCode != Qt::Key_CapsLock;
        addMacro = addMacro && kEvent.keyCode != Qt::Key_NumLock;
    }

    return addMacro;
}


bool MacroEventLogBuffer::isLeftClick(const MacroMouseEvent &mEvent, int timeSinceLastEventAddMs) const
{
    return    mEvent.type == MacroMouseEventType::LeftRelease
           && _events.size() > 0
           && _events.last().type == MacroEventType::MouseEvent
           && _events.last().mouseEvent.type == MacroMouseEventType::LeftPress
           && timeSinceLastEventAddMs <= TYPE_CLICK_TIME_THRESH
           && lastMouseEventWithinRangeOfThisEvent(mEvent);
}


bool MacroEventLogBuffer::isRightClick(const MacroMouseEvent &mEvent, int timeSinceLastEventAddMs) const
{
    return    mEvent.type == MacroMouseEventType::RightRelease
           && _events.size() > 0
           && _events.last().type == MacroEventType::MouseEvent
           && _events.last().mouseEvent.type == MacroMouseEventType::RightPress
           && timeSinceLastEventAddMs <= TYPE_CLICK_TIME_THRESH
           && lastMouseEventWithinRangeOfThisEvent(mEvent);
}


bool MacroEventLogBuffer::isMiddleClick(const MacroMouseEvent &mEvent, int timeSinceLastEventAddMs) const
{
    return    mEvent.type == MacroMouseEventType::MiddleRelease
           && _events.size() > 0
           && _events.last().type == MacroEventType::MouseEvent
           && _events.last().mouseEvent.type == MacroMouseEventType::MiddlePress
           && timeSinceLastEventAddMs <= TYPE_CLICK_TIME_THRESH
           && lastMouseEventWithinRangeOfThisEvent(mEvent);
}


bool MacroEventLogBuffer::areLastTwoEventsDoubleClick() const
{
    return    _events.size() > 1
           && _events.last().type == MacroEventType::MouseEvent
           && _events.last().mouseEvent.type == MacroMouseEventType::LeftClick
           && _events.at(_events.size() - 2).type == MacroEventType::MouseEvent
           && _events.at(_events.size() - 2).mouseEvent.type == MacroMouseEventType::LeftClick
           && _events.last().delayMs <= (TYPE_CLICK_TIME_THRESH * 2)
           && lastMouseEventWithinRangeOfThisEvent(_events.at(_events.size() - 1).mouseEvent);
}


bool MacroEventLogBuffer::lastMouseEventWithinRangeOfThisEvent(const MacroMouseEvent &mEvent) const
{
    return    _events.last().mouseEvent.loc.x() >= mEvent.loc.x() - 10
           && _events.last().mouseEvent.loc.x() <= mEvent.loc.x() + 10
           && _events.last().mouseEvent.loc.y() >= mEvent.loc.y() - 10
           && _events.last().mouseEvent.loc.y() <= mEvent.loc.y() + 10;
}


bool MacroEventLogBuffer::isContinuationOfScroll(const MacroMouseEvent &mEvent) const
{
    bool isScrollEvent =    mEvent.type == MacroMouseEventType::ScrollDown
                         || mEvent.type == MacroMouseEventType::ScrollUp;

    return    isScrollEvent
           && _events.size() > 0
           && _events.last().type == MacroEventType::MouseEvent
           && _events.last().mouseEvent.type == mEvent.type;
}


void MacroEventLogBuffer::takeScreenshotIfLocationSensitiveMouseEvent(MacroEvent &event) const
{
    QSize screenshotSize;
    MacroMouseEvent &mEvent = event.mouseEvent;

    bool isMouseEvent = event.type == MacroEventType::MouseEvent;
    bool isLocationSensitive =    mEvent.type == MacroMouseEventType::LeftPress
                               || mEvent.type == MacroMouseEventType::RightPress
                               || mEvent.type == MacroMouseEventType::MiddlePress;

    if (isMouseEvent && isLocationSensitive) {
        // Generate new screenshot ID and use it for screenshot file name as well!
        mEvent.screenshotId = MacroEventModel::generateNewScreenshotId();
        mEvent.screenshotImg = RecordImageUtil::takeScreenshot();
        screenshotSize = mEvent.screenshotImg.size();
    }
}


void MacroEventLogBuffer::isolateTargetImgIfScreenshotTaken(MacroMouseEvent &mEvent) const
{
    bool screenshotTaken = mEvent.screenshotId != -1;
    if (screenshotTaken) {
        // Take the screenshot and get the bounds of the event and various other info.
        mEvent.screenshotRect = RecordImageUtil::isolateTargetImg(mEvent.targetImgType, mEvent.cursorType,
                                                                  mEvent.screenshotImg, mEvent.loc);
    }
}


bool MacroEventLogBuffer::isContinuationOfPress(const MacroKeyboardEvent &kEvent) const
{
    const MacroKeyboardEvent *lastKEvent = nullptr;
    // If the last received Macro Event is a Macro Keyboard Event.
    if (_events.size() > 0 && _events.last().type == MacroEventType::KeyboardEvent) {
        lastKEvent = &_events.last().keyboardEvent;
    }

    return    lastKEvent != nullptr
           && lastKEvent->type == MacroKeyboardEventType::KeyPress
           && lastKEvent->type == kEvent.type
           && lastKEvent->keyCode == kEvent.keyCode;
}


bool MacroEventLogBuffer::isKeyType(const MacroKeyboardEvent &kEvent, int timeSinceLastEventAddMs) const
{
    return    kEvent.type == MacroKeyboardEventType::KeyRelease
           && _events.size() > 0
           && _events.last().type == MacroEventType::KeyboardEvent
           && _events.last().keyboardEvent.type == MacroKeyboardEventType::KeyPress
           && _events.last().keyboardEvent.keyCode == kEvent.keyCode
           && timeSinceLastEventAddMs <= TYPE_CLICK_TIME_THRESH;
}


void MacroEventLogBuffer::addEventToList(MacroEvent &event, int timeSinceLastEventAddMs)
{    
    /*
     * We want the delay to be a time period after the duration of the previous event.
     * Since timeSinceLastEventAddMs contains number of milliseconds since the last event in _events
     * was added, we must factor out the duration of the event. This ensures that we get a distinct
     * time period of duration for the previous event, followed by a delay until this next added event!
     */
    event.delayMs = timeSinceLastEventAddMs - (_events.size() > 0 ? _events.last().durationMs
                                                                  : 0);
    _events.append(event);
}


void MacroEventLogBuffer::formKeyStringsAndSetEventIndexes()
{
    const int KEY_STRING_START_UNDEF = -1;
    bool formKeyString,
         lastEventWasKeyString,
         keyEventTypeRightForKeyString;
    int keyStringStartInd = KEY_STRING_START_UNDEF;

    for (int i = 0; i < _events.size(); i++)
    {
        lastEventWasKeyString =    i > 0
                                && _events.at(i - 1).type == MacroEventType::KeyboardEvent
                                && _events.at(i - 1).keyboardEvent.type == MacroKeyboardEventType::KeyString;

        keyEventTypeRightForKeyString =    _events.at(i).keyboardEvent.type == MacroKeyboardEventType::KeyPress
                                        || _events.at(i).keyboardEvent.type == MacroKeyboardEventType::KeyType
                                        || lastEventWasKeyString;

        formKeyString =    _events.at(i).type == MacroEventType::KeyboardEvent
                        && _events.at(i).nRepeats == 0
                        && isKeyCodeChar(_events.at(i).keyboardEvent)
                        && keyEventTypeRightForKeyString;

        if (formKeyString) {
            MacroKeyboardEvent &kEvent = _events[i].keyboardEvent;

            // If we are starting the formation of a key string.
            if (keyStringStartInd == KEY_STRING_START_UNDEF) {
                keyStringStartInd = i;
                kEvent.keyString = getKeyCodeStr(kEvent.keyCode, kEvent.mod1, kEvent.mod2, kEvent.capsLock, kEvent.numLock);
                kEvent.keyCode = -1;
                kEvent.type = MacroKeyboardEventType::KeyString;
            }
            // Else we are continuing the formation of a key string.
            else {
                MacroKeyboardEvent &lastKEvent = _events[i - 1].keyboardEvent;

                // Only add to key string on key press/type (don't count twice from key release).
                if (   kEvent.type == MacroKeyboardEventType::KeyPress
                    || kEvent.type == MacroKeyboardEventType::KeyType)
                {
                    // If backspace, then correct user's mistake and just remove last char from key string.
                    if (kEvent.keyCode == Qt::Key_Backspace) {
                        lastKEvent.keyString = lastKEvent.keyString.remove(lastKEvent.keyString.size() - 1, 1);
                    }
                    // Else just add to end of key string.
                    else {
                        lastKEvent.keyString += getKeyCodeStr(kEvent.keyCode, kEvent.mod1, kEvent.mod2,
                                                              kEvent.capsLock, kEvent.numLock);
                    }
                }
                // Factor in delayMs between key strokes into durationMs of typing key string.
                _events[i - 1].durationMs += _events.at(i).delayMs;

                _events.removeAt(i--);
                continue; // We do not want any further processing for removed event!
            }
        }
        // Else if we are finishing the formation of a key string.
        else if (keyStringStartInd != KEY_STRING_START_UNDEF) {
            escapeChars(_events[i - 1].keyboardEvent.keyString);
            keyStringStartInd = KEY_STRING_START_UNDEF;
        }

        _events[i].index = _nextEventInd;
        // If we are not appending events (but are inserting at specific index).
        if (_nextEventInd >= 0) {
            _nextEventInd++;
        }
    }
}


void MacroEventLogBuffer::escapeChars(QString &keyStr)
{
    keyStr = keyStr.replace("'", "''");
}
