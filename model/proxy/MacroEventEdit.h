#ifndef MACROEVENTEDIT_H
#define MACROEVENTEDIT_H


// NOTE: Could make non-integer types in MacroEventEdit struct pointer types to make it more memory efficient.
//       This is probably uneeded optimization that will add unnecessary complexity though...


#include "model/MacroEvent.h"
#include <QList>
#include <QPair>


/**
 * @brief The MacroEventEditType enum
 * The Type of the Edit that was done on Macro Events.
 */
enum MacroEventEditType
{
    ADD,             // COPY falls under ADD event.
    DELETE,
    UPDATE_DELAY,
    UPDATE_DURATION,
    UPDATE_KEY_STRING,
    UPDATE_AUTO_CORRECT,
    UPDATE_IMAGE
};


/**
 * @brief The EditProxyChangeLogEntry struct
 * Container for a signle log entry in the change (undo/redo) log within the MacroEventEditProxy.
 */
struct MacroEventEdit
{
    /**
     * @brief _editType
     * The type of edit that was done on the Macro Events for this change log entry.
     * Used to denote what data to look at.
     */
    MacroEventEditType editType;


    /**
     * @brief addOrDeleteEvents
     * The added or deleted events pertaining to an ADD or DELETE edit type.
     */
    QList<MacroEvent> addOrDeleteEvents;


    /**
     * @brief eventInd
     * The index of an event that an update edit type enacted on.
     */
    int eventInd;


    /**
     * @brief oldDelay
     * The old event delay.
     */
    int oldDelay;

    /**
     * @brief newDelay
     * The new event delay.
     */
    int newDelay;


    /**
     * @brief oldDuration
     * The old event duration.
     */
    int oldDuration;

    /**
     * @brief newDuration
     * The new event duration.
     */
    int newDuration;


    /**
     * @brief oldKeyString
     * The old key (character) string.
     */
    QString oldKeyString;

    /**
     * @brief newKeyString
     * The new key (character) string.
     */
    QString newKeyString;


    /**
     * @brief oldAutoCorrect
     * The old auto correct flag used to determine if computer vision techniques should be used to check location sensitive events.
     */
    bool oldAutoCorrect;

    /**
     * @brief newAutoCorrect
     * The new auto correct flag used to determine if computer vision techniques should be used to check location sensitive events.
     */
    bool newAutoCorrect;


    /**
     * @brief oldImage
     * The old auto correction image.
     */
    QImage oldImage;

    /**
     * @brief newImage
     * The new (cropped) auto correction image.
     */
    QImage newImage;
};


typedef struct MacroEventEdit MacroEventEdit;


#endif // MACROEVENTEDIT_H
