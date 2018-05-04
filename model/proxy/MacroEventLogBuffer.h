#ifndef MACROEVENTLOGBUFFER_H
#define MACROEVENTLOGBUFFER_H


#include "model/MacroEventModel.h"
#include "util/ProducerConsumerQueue.hpp"
#include <QMutex>
#include <QWaitCondition>
#include <QList>


/**
 * @brief The MacroEventLogBuffer class
 * Buffers logged Macro Events in memory before submitting them in bulk to an underlying model.
 * Also, does a lot of preprocessing on the Macro Events such as combining press and release events
 * into a click or type, or combining keyboard type events into a key string event.
 * Also, associates a screenshot image with all location sensitive mouse events that it buffers,
 * and does processing on the image to crop it to the event location.
 * Also, measures delay and duration timings for all events.
 * Finally, it assigns an ordered event index to all buffered events based off of a base index.
 */
class MacroEventLogBuffer
{

public:

    explicit MacroEventLogBuffer();
    ~MacroEventLogBuffer();

    /**
     * @brief setProxyInsertInd
     * @param insertInd
     * The insert index from which the macro event indexes will be set when Macros are passed through the proxy.
     */
    void setProxyInsertInd(int insertInd);

    /**
     * @brief addEvent
     * Adds Macro events. Will set the event index to be the next event index in sequence (starting at
     * the start index set via constructor argument).
     * NOTE: Not all events passed to this proxy will actually be added. It is the discretion of the proxy
     *       to determine whether or not an event is added. This proxy will not add any mouse movement events.
     *       It may filter certain keyboard events. It may transform or condense chains of events.
     * NOTE: Will not immediately commit events to underlying Model and database. Must invoke commitEvents() to do so!!!
     * @param event
     * The event to add.
     * NOTE: This can be modified inside the proxy.
     */
    void addEvent(MacroEvent &event);

    /**
     * @brief takeAddedEvents
     * Takes the added Macro Events from this Macro Event Add Proxy.
     * This effectively empties the proxy of all of its events and transfers them to the caller.
     * Should be called as an alternative to commitEvents() whenever the events are not to be immediately
     * stored in the database upon ending IO recording (such as when performing Macro Editing)./
     * @param numEndEventsToPop (OPTIONAL)
     * Default is 1 for removing 1 events from end of event string. The number of added Macro Events to exclude from the commit starting from
     * the end of the string of Macro Events that were added. Used to exclude certain characters that may be used to stop IO Logging.
     * @return
     * The list of Macro Events that have been recorded.
     */
    QList<MacroEvent> takeAddedEvents(int numEndEventsToPop = 1);

private:

    /**
     * @brief _run
     * Flag that signifies if the consumer thread should keep running. More specifically,
     * should the processMacroEventsLoop continue.
     */
    bool _run;

    /**
     * @brief _nextEventInd
     * The index from which the macro event indexes will be set when Macros are passed through the proxy.
     */
    int _nextEventInd;

    /**
     * @brief _events
     * A list of the Macro Events added to this Proxy.
     */
    QList<MacroEvent> _events;

    /**
     * @brief _producerConsumerQueue
     * A queue of pre-processed events that should be consumed by the thread inside of this object.
     */
    ProducerConsumerQueue<MacroEvent> _producerConsumerQueue;

    /**
     * @brief TYPE_CLICK_TIME_THRESH
     * The time delay threshold between a press and release event for mouse and keyboard events
     * to form a click and type event respectively. For example, if a left release happens in
     * less time than TYPE_CLICK_TIME_THRESH from a previous left press, then the left press
     * and release events can be combined into a left click event.
     */
    static const int TYPE_CLICK_TIME_THRESH;

    /**
     * @brief processMacroEventsLoop
     * The central loop for consuming and processing Macro Events in a loop that runs in its own thread.
     * The processing should be done in its own separate thread so that it does not stall up the system
     * thread that is involved in posting IO logging events to our process!
     */
    void processMacroEventsLoop();

    /**
     * @brief processMouseEvent
     * Processes Macro Mouse Events that are passed into addEvent(). Modifies the Macro Mouse Event if needed.
     * Determines if the event should be added to the Macro Events (_events) list.
     * @param mEvent
     * The Macro Mouse Event.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     * @return
     * True if it should be added to the _events list, false if not.
     */
    bool processMouseEvent(MacroMouseEvent &mEvent, int timeSinceLastEventAddMs);

    /**
     * @brief processMouseEvent
     * Processes Macro Keyboard Events that are passed into addEvent(). Modifies the Macro Keyboard Event if needed.
     * Modifies the Macro Events (_events) list if the event is part of a key string event.
     * Determines if the event should be added to the Macro Events (_events) list.
     * @param kEvent
     * The Macro Keyboard Event.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     * @return
     * True if it should be added to the _events list, false if not.
     */
    bool processKeyboardEvent(MacroKeyboardEvent &kEvent, int timeSinceLastEventAddMs);

    /**
     * @brief isLeftClick
     * Checks if the event form a left click event. More specifically, checks if we should
     * combine a left release followed by a left press.
     * @param mEvent
     * The mouse event to check.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     * @return
     * True if it does form a left click event, false if not.
     */
    bool isLeftClick(const MacroMouseEvent &mEvent, int timeSinceLastEventAddMs) const;

    /**
     * @brief isRightClick
     * Checks if the event form a right click event. More specifically, checks if we should
     * combine a right release followed by a right press.
     * @param mEvent
     * The mouse event to check.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     * @return
     * True if it does form a right click event, false if not.
     */
    bool isRightClick(const MacroMouseEvent &mEvent, int timeSinceLastEventAddMs) const;

    /**
     * @brief isMiddleClick
     * Checks if the event form a middle click event. More specifically, checks if we should
     * combine a middle release followed by a middle press.
     * @param mEvent
     * The mouse event to check.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     * @return
     * True if it does form a middle click event, false if not.
     */
    bool isMiddleClick(const MacroMouseEvent &mEvent, int timeSinceLastEventAddMs) const;

    /**
     * @brief isDoubleClick
     * Checks if the last two events in the _events list form a double click event. More specifically, checks if we should
     * combine a left click followed by a left click.
     * @return
     * True if they do form a double click event, false if not.
     */
    bool areLastTwoEventsDoubleClick() const;

    /**
     * @brief lastMouseEventWithinRangeOfThisEvent
     * Checks if the last mouse event, which must be the last event in the _events list, is within
     * range of the given mouse event. The range is 10 pixels in every direction.
     * @param mEvent
     * The mouse event to check if the last event is in range of.
     * @return True
     * if it is in range, false if it is not.
     */
    bool lastMouseEventWithinRangeOfThisEvent(const MacroMouseEvent &mEvent) const;

    /**
     * @brief isContinuationOfScroll
     * Checks if the event is a continuation of a mouse wheel scroll event.
     * @param mEvent
     * The mouse event to check.
     * @return
     * True if it is a continuation of a mouse wheel scroll event, false if not.
     */
    bool isContinuationOfScroll(const MacroMouseEvent &mEvent) const;

    /**
     * @brief takeScreenshotIfLocationSensitiveMouseEvent
     * Takes a screenshot if a given event is a location sensitive mouse event, and fills screenshotKey
     * and screenshotImg data in the given Macro Event.
     * A location sensitive mouse event is any click event. This excludes wheel/scroll events.
     * @param event (INPUT & OUTPUT)
     * The Macro event to possibly generate a screenshot for and fill with screenshot data (screenshotKey and screenshotImg).
     */
    void takeScreenshotIfLocationSensitiveMouseEvent(MacroEvent &event) const;

    /**
     * @brief isolateTargetImgIfScreenshotTaken
     * Gets the localized bounds for the target of a given location sensitive mouse event. A location sensitive
     * mouse event should have a screenshot generated previously for it using the takeScreenshotIfLocationSensitiveMouseEvent()
     * method.
     * @param mEvent (INPUT & OUTPUT)
     * The mouse event to possibly isolate the target of the event for (if a screenshot has been taken for it).
     * Will have its data members filled pertaining to the target of the event.
     */
    void isolateTargetImgIfScreenshotTaken(MacroMouseEvent &mEvent) const;

    /**
     * @brief isContinuationOfPress
     * Checks if a new Macro Keyboard Event is a continuation of the last event key press.
     * @param kEvent
     * The new Macro Keyboard Event.
     * @return
     * True if it is a continuation of a key press, false if it is not.
     */
    bool isContinuationOfPress(const MacroKeyboardEvent &kEvent) const;

    /**
     * @brief isKeyType
     * Checks if the event is a key type event. More specifically, checks if a key release event
     * should be combined with a key press event to form a key type event.
     * @param kEvent
     * The key event to check.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     * @return
     * True if it is a key type event, false if not.
     */
    bool isKeyType(const MacroKeyboardEvent &kEvent, int timeSinceLastEventAddMs) const;

    /**
     * @brief addEventToList
     * Actually adds the event to the list of Macro Events that will be included in the commit to the underlying
     * database when commitEvents() is called.
     * @param event
     * The Macro Event to add.
     * @param timeSinceLastEventAddMs
     * The amount of time (in milliseconds) since the addition of the last event to the Macro Event list called _events.
     */
    void addEventToList(MacroEvent &event, int timeSinceLastEventAddMs);

    /**
     * @brief formKeyStringsAndSetEventIndexes
     * During the final commit to the underlying model and DB, this will combine
     * all keyboard events that belong in a character key string and set event indexes.
     */
    void formKeyStringsAndSetEventIndexes();

    /**
     * @brief escapeChars
     * Escapes all characters that would mess up a SQL query. So, all quotes and back slashes are escaped.
     * @param keyStr
     * The string to escape.
     */
    void escapeChars(QString &keyStr);
};


#endif // MACROEVENTLOGBUFFER_H
