#ifndef CHANGELOG_HPP
#define CHANGELOG_HPP


#include <QList>
#include <QDebug>


/**
 * @brief The ChangeLog class
 * A generic class for a Change Log. A Change Log is basically a double ended queue.
 * It also keeps track of the current change that the user is on. All redo changes will
 * be after this current change, and all undo changes will be before it. Built on top of QList.
 */
template <typename T>
class ChangeLog
{

private:

    /**
     * @brief NO_ACTIVE_CHANGES
     * Current change log position value that signifies that there are no current active changes in the change log.
     * This means that there are no undo changes, or no save changes available.
     */
    const static int NO_ACTIVE_CHANGES;

    /**
     * @brief _changeLog
     * The underlying change log list.
     */
    QList<T> _changeLog;

    /**
     * @brief _curChangeLogPos
     * The position of the most current change in the change log. Everything after it is the redo part of the log.
     * These edits will not be included in a save. Everything on or before it is the undo part of the log and will
     * be included in a save.
     */
    int _curChangeLogPos;

    /**
     * @brief _lastSavePos
     * The position of the latest event that was part of the last save.
     */
    int _lastSavePos;

public:

    explicit ChangeLog();


    /**
     * @brief addChange
     * Adds a change to the change log. If the change log has any redo changes, they are discarded before
     * this new change is added to the end of the log. The current change position is also updated.
     * @param change
     * The new change to add to the change log.
     */
    void addChange(const T &change);


    /**
     * @brief hasUndoChange
     * Determines if an undo change is available. Should be called before undoChange().
     * @return
     * True if an undo change is available, false if not.
     */
    bool hasUndoChange() const;

    /**
     * @brief hasRedoChange
     * Determines if a redo change is available. Should be called before redoChange().
     * @return
     * True if a redo change is available, false if not.
     */
    bool hasRedoChange() const;

    /**
     * @brief hasSaveChanges
     * Determines if save changes are available. Should be called before getSaveChanges().
     * @return
     * True if save changes are available, false if not.
     */
    bool hasSaveChanges() const;


    /**
     * @brief undoChange
     * Updates the change log to reflect an undo change by updating the current change position.
     * If no undo changes exist, an exception is thrown.
     * @return
     * The change that is to be undone.
     */
    const T& undoChange();

    /**
     * @brief redoChange
     * Updates the change log to reflect a redo change by updating the current change position.
     * If no redo changes exist, an exception is thrown.
     * @return
     * The change that is to be redone.
     */
    const T& redoChange();


    /**
     * @brief size
     * The size of the change log.
     * @return
     * Number of elements in change log.
     */
    int size() const;


    /**
     * @brief getSaveChanges
     * Gets a list of all of the save changes for a commit to the underlying database.
     * The save changes will include everything up to and including the current change position.
     * @return
     * A list of save changes.
     */
    QList<T> getSaveChanges();


    /**
     * @brief clear
     * Clears the change log of all of its entries and resets the current change position.
     */
    void clear();
};


template<typename T>
const int ChangeLog<T>::NO_ACTIVE_CHANGES = -1;


template <typename T>
ChangeLog<T>::ChangeLog()
    : _changeLog(),
      _curChangeLogPos(NO_ACTIVE_CHANGES),
      _lastSavePos(NO_ACTIVE_CHANGES)
{}


template <typename T>
void ChangeLog<T>::addChange(const T &change)
{
    // Discard all undo changes before addition of new change.
    if ((_changeLog.size() - 1) > _curChangeLogPos) {
        _changeLog = _changeLog.mid(0, _curChangeLogPos + 1);
    }

    // Append change and update current change log position to end of log.
    _changeLog.append(change);
    _curChangeLogPos = (_changeLog.size() - 1);
}


template <typename T>
bool ChangeLog<T>::hasUndoChange() const
{
    return (_curChangeLogPos != NO_ACTIVE_CHANGES);
}


template <typename T>
bool ChangeLog<T>::hasRedoChange() const
{
    return (_curChangeLogPos < (_changeLog.size() - 1));
}


template <typename T>
bool ChangeLog<T>::hasSaveChanges() const
{
    return (_curChangeLogPos > _lastSavePos);
}


template <typename T>
const T& ChangeLog<T>::undoChange()
{
    if (!hasUndoChange()) {
        throw std::runtime_error("Error: no changes are available for undo in the Change Log");
    }
    return _changeLog.at(_curChangeLogPos--);
}


template <typename T>
const T& ChangeLog<T>::redoChange()
{
    if (!hasRedoChange()) {
        throw std::runtime_error("Error: no changes are available for redo in the Change Log");
    }
    return _changeLog.at(++_curChangeLogPos);
}


template <typename T>
int ChangeLog<T>::size() const
{
    return _changeLog.size();
}


template <typename T>
QList<T> ChangeLog<T>::getSaveChanges()
{
    qDebug() << "last save pos: " << _lastSavePos;
    qDebug() << "cur change pos: " << _curChangeLogPos;

    // If no save changes.
    if (!hasSaveChanges()) {
        return QList<T>();
    }

    int lastSavePos = _lastSavePos;
    _lastSavePos = _curChangeLogPos;
    return _changeLog.mid(lastSavePos + 1, _curChangeLogPos + 1);
}


template <typename T>
void ChangeLog<T>::clear()
{
    _changeLog.clear();
    _curChangeLogPos = NO_ACTIVE_CHANGES;
    _lastSavePos = NO_ACTIVE_CHANGES;
}


#endif // CHANGELOG_HPP
