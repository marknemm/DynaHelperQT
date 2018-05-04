#include "MacroEventEditProxy.h"
#include <QDebug>


MacroEventEditProxy::MacroEventEditProxy(MacroEventModel &macroEventModel)
    : _macroEventModel(macroEventModel),
      _macroEvents(),
      _changeLog()
{}


void MacroEventEditProxy::setEditMacros(const QList<int> &editMacroIds)
{
    _macroEventModel.setActiveMacros(editMacroIds);
    _macroEvents = _macroEventModel.getUniformEvents();
    // Fill in gaps in the uniform events with dummy events. Could optimize this by using pointers and assigning null...
    for (int i = 0; i < _macroEvents.size() - 1; i++) {
        if (_macroEvents.at(i).index + 1 != _macroEvents.at(i + 1).index) {
            MacroEvent dummyEvent;
            dummyEvent.type = DummyEvent;
            dummyEvent.index = _macroEvents.at(i).index + 1;
            _macroEvents.insert((i + 1), dummyEvent);
        }
    }
}


QList<int> MacroEventEditProxy::getActiveMacroIds() const
{
    return _macroEventModel.getActiveMacroIds();
}


QList<MacroEvent> MacroEventEditProxy::getLatestMacroEvents() const
{
    return _macroEvents;
}


void MacroEventEditProxy::insertMacroEvents(const QList<MacroEvent> &macroEvents)
{
    // Break out if we have no events to insert!
    if (macroEvents.size() == 0) return;

    int listInsertIndex = macroEvents.first().index;
    incMacroEventIndexes(listInsertIndex, macroEvents.size());

    // Insert the Macros.
    foreach (const MacroEvent &macroEvent, macroEvents) {
        qDebug() << "Inserting macro event at index: " << listInsertIndex;
        _macroEvents.insert(listInsertIndex++, macroEvent);
    }

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::ADD;
    changeLogEntry.addOrDeleteEvents = macroEvents;
    _changeLog.addChange(changeLogEntry);
}


void MacroEventEditProxy::copyMacroEvents(QList<int> &macroEventIndexes)
{
    QList<MacroEvent> addedMacroEvents;
    int copyEventInd,
        listInsertInd = (_macroEvents.size() - 1),
        lastListInsertInd = _macroEvents.size();

    // Sort the Macro Event indexes so we can copy from largest to smallest.
    std::sort(macroEventIndexes.begin(), macroEventIndexes.end());

    for (int i = (macroEventIndexes.size() - 1); i >= 0; i--) {
        copyEventInd = macroEventIndexes.at(i);
        // Search through list backwards until we find Macro Event with correct Macro Event index.
        while (_macroEvents.at(listInsertInd).index > copyEventInd) {
            listInsertInd--;
        }
        // Increment Macro Event indexes of all events within range from our target copy event
        // up to before the last target copy event (or end of list if this is first target copy event).
        // Do this by the magnitude of the index we are on in our macroEventIndexes list iteration so we do not have to
        // keep moving all indexes forward by one after each copy!
        addedMacroEvents.append(_macroEvents.at(listInsertInd));
        incMacroEventIndexes(listInsertInd, (i + 1), lastListInsertInd);
        _macroEvents.insert(listInsertInd, _macroEvents.at(listInsertInd));
        _macroEvents[listInsertInd].index--; // Decrement our copied Macro Event index since it was inserted before source of copy.
        lastListInsertInd = listInsertInd;
    }

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::ADD;
    changeLogEntry.addOrDeleteEvents = addedMacroEvents;
    _changeLog.addChange(changeLogEntry);
}


void MacroEventEditProxy::deleteMacroEvents(QList<int> &macroEventIndexes)
{
    QList<MacroEvent> deletedMacroEvents;
    int delEventInd,
        listDelInd = (_macroEvents.size() - 1),
        lastListDelInd = _macroEvents.size();

    // Sort the Macro Event indexes so we can delete from largest to smallest.
    std::sort(macroEventIndexes.begin(), macroEventIndexes.end());

    for (int i = (macroEventIndexes.size() - 1); i >= 0; i--) {
        delEventInd = macroEventIndexes.at(i);
        // Search through list backwards until we find Macro Event with correct Macro Event index.
        while (_macroEvents.at(listDelInd).index > delEventInd) {
            listDelInd--;
        }
        // Decrement Macro Event indexes of all events within range from our target delete event
        // up to before the last target delete event (or end of list if this is first target delete event).
        // Do this by the magnitude of the index we are on in our macroEventIndexes list iteration so we do not have to
        // keep moving all indexes backwards by one after each delete!
        incMacroEventIndexes((listDelInd + 1), -(i + 1), lastListDelInd);
        deletedMacroEvents.append(_macroEvents.at(listDelInd));
        _macroEvents.removeAt(listDelInd);
        lastListDelInd = listDelInd--; // Make sure we decrement listDelInd or else we will have error!
    }

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::DELETE;
    changeLogEntry.addOrDeleteEvents = deletedMacroEvents;
    _changeLog.addChange(changeLogEntry);
}


void MacroEventEditProxy::updateMacroEventDelay(int macroEventIndex, int delay)
{
    int oldDelay = _macroEvents.at(macroEventIndex).delayMs;
    _macroEvents[macroEventIndex].delayMs = delay;

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::UPDATE_DELAY;
    changeLogEntry.eventInd = macroEventIndex;
    changeLogEntry.oldDelay = oldDelay;
    changeLogEntry.newDelay = delay;
    _changeLog.addChange(changeLogEntry);
}


void MacroEventEditProxy::updateMacroEventDuration(int macroEventIndex, int duration)
{
    int oldDuration = _macroEvents.at(macroEventIndex).durationMs;
    _macroEvents[macroEventIndex].durationMs = duration;

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::UPDATE_DURATION;
    changeLogEntry.eventInd = macroEventIndex;
    changeLogEntry.oldDuration = oldDuration;
    changeLogEntry.newDuration = duration;
    _changeLog.addChange(changeLogEntry);
}


void MacroEventEditProxy::updateMacroEventAutoCorrect(int macroEventIndex, bool autoCorrect)
{
    bool oldAutoCorrect = _macroEvents.at(macroEventIndex).mouseEvent.autoCorrect;
    _macroEvents[macroEventIndex].mouseEvent.autoCorrect = autoCorrect;

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::UPDATE_AUTO_CORRECT;
    changeLogEntry.eventInd = macroEventIndex;
    changeLogEntry.oldAutoCorrect = oldAutoCorrect;
    changeLogEntry.newAutoCorrect = autoCorrect;
    _changeLog.addChange(changeLogEntry);
}


void MacroEventEditProxy::updateMacroEventKeyString(int macroEventIndex, const QString &keyString)
{
    const QString &oldKeyString = _macroEvents.at(macroEventIndex).keyboardEvent.keyString;
    _macroEvents[macroEventIndex].keyboardEvent.keyString = keyString;

    // Update change log.
    MacroEventEdit changeLogEntry;
    changeLogEntry.editType = MacroEventEditType::UPDATE_KEY_STRING;
    changeLogEntry.eventInd = macroEventIndex;
    changeLogEntry.oldKeyString = oldKeyString;
    changeLogEntry.newKeyString = keyString;
    _changeLog.addChange(changeLogEntry);
}


bool MacroEventEditProxy::hasUnsavedChanges()
{
    return _changeLog.hasSaveChanges();
}


void MacroEventEditProxy::saveEvents()
{
    // Begin large all or nothing transaction when saving all events (also makes much more efficient).
    _macroEventModel.safeOpenAndBegin("Could not open DB conneciton and begin transaction in saveEvents()");

    // Go through all changes in the change log and commit to underlying model.
    QList<MacroEventEdit> changes = _changeLog.getSaveChanges();
    for (int i = 0; i < changes.size(); i++) {
        MacroEventEdit &change = changes[i];

        switch (change.editType) {
        case ADD:
            _macroEventModel.addEvents(change.addOrDeleteEvents);
            break;
        case DELETE:
        {
            QList<int> macroEventInds = getMacroEventIndexesFromMacroEvents(change.addOrDeleteEvents);
            _macroEventModel.removeEvents(macroEventInds);
            break;
        }
        case UPDATE_DELAY:
        case UPDATE_DURATION:
        case UPDATE_KEY_STRING:
        case UPDATE_AUTO_CORRECT:
        {
            // For all Macro Event updates, just update entire event!
            _macroEventModel.setEvent(_macroEvents.at(change.eventInd));
            break;
        }
        case UPDATE_IMAGE:
            // TODO.
            break;
        default:
            throw std::runtime_error("Error: invalid Change Log Edit Type in MacroEventEditProxy::saveEvents()");
        }
    }

    // Commit all of our save changes now!
    _macroEventModel.safeCommitAndClose("Could not commit transaction and close DB connection in saveEvents()");
}


void MacroEventEditProxy::undoChange()
{
    _changeLog.undoChange();
}


void MacroEventEditProxy::redoChange()
{
    _changeLog.redoChange();
}


void MacroEventEditProxy::refresh()
{
    _changeLog.clear();
}


QList<int> MacroEventEditProxy::getMacroEventIndexesFromMacroEvents(const QList<MacroEvent> &events) const
{
    QList<int> eventInds;
    foreach (const MacroEvent &event, events) {
        eventInds.append(event.index);
    }
    return eventInds;
}


void MacroEventEditProxy::incMacroEventIndexes(int beginListIndex, int incAmt, int endListIndex)
{
    // Set the end list index (to stop incrementing at).
    if (endListIndex == -1 || endListIndex > _macroEvents.size()) {
        endListIndex = _macroEvents.size();
    }

    // Move all Macro Event indexes that are greater than the given beginListIndex and  by incAmt.
    for (int i = beginListIndex; i < endListIndex; i++) {
        _macroEvents[i].index += incAmt;
    }
}
