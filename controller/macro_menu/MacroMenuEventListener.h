#ifndef MAINMENUEVENTLISTENER_H
#define MAINMENUEVENTLISTENER_H


#include "model/MacroMetadata.h"
#include <QString>
#include <QList>


/**
 * @brief The MacroMenuEventListener class
 * Abstracts the Main Menu Controller away from the Main Menu View.
 * Contains listeners for events that come from the Main Menu View.
 */
class MacroMenuEventListener
{
public:

    /**
     * @brief acivateMacro
     * Activates the selected macro.
     * @param id
     * The ID of the macro to activate.
     */
    virtual void activateMacro(int id) = 0;

    /**
     * @brief createNewMacro
     * Initializes the creation of a new macro.
     * @param name
     * The name of the new macro.
     */
    virtual void createNewMacro(const QString &name) = 0;

    /**
     * @brief renameMacro
     * Initializes the renaming of a macro.
     * @param id
     * The ID of the macro to rename.
     * @param name
     * The new name for the macro.
     */
    virtual void renameMacro(int id, const QString &name) = 0;

    /**
     * @brief editMacros
     * Initializes macro editing.
     * @param ids
     * A list of the IDs of macros to be edited.
     */
    virtual void editMacros(const QList<int> &ids) = 0;

    /**
     * @brief removeMacros
     * Initializes macro removal.
     * @param ids
     * A list of IDs of macros to be removed.
     */
    virtual void removeMacros(const QList<int> &ids) = 0;

    /**
     * @brief copyMacro
     * Initializes macro copying.
     * @param ids
     * A list of IDs of macros to be copied.
     * @return
     * A list of the result of the copy operation.
     */
    virtual QList<MacroMetadata> copyMacros(const QList<int> &ids) = 0;

    /**
     * @brief requestMacroMetadataWithFilter
     * Requests Macro metadata with optional filter values.
     * @param nameOrIdFilter (OPTIONAL)
     * The macro id or name to filter by. Default is empty string for no filter.
     * @param offset (OPTIONAL)
     * The offset of the start point macro metadata records to be requested. Default is 0 for no offset.
     * @param sortOrder (OPTIONAL)
     * The order by which Macro metadata records will be sorted in. Default is sorting by ID number in ascending order.
     * @return
     * A list of the Macro metadata corresponding to the request.
     */
    virtual QList<MacroMetadata> requestMacroMetadataWithFilter(const QString &nameOrIdFilter = "", int offset = 0,
                                                                MacroMetadataSortOrder sortOrder = ID_ASC) = 0;

    /**
     * @brief requestMacroMetadataWithFilter
     * Requests Macro metadata with optional filter values.
     * @param nameOrIdFilter (OPTIONAL)
     * The macro id or name to filter by. Default is empty string for no filter.
     * @param offset (OPTIONAL)
     * The offset of the start point macro metadata records to be requested. Default is 0 for no offset.
     * @param limit (OPTIONAL)
     * The limit on the number of macro metadata records to be requested. Default is -1 for no limit (request all).
     * @param sortOrder (OPTIONAL)
     * The order by which Macro metadata records will be sorted in. Default is sorting by ID number in ascending order.
     * @return
     * A list of the Macro metadata corresponding to the request.
     */
    virtual QList<MacroMetadata> requestMacroMetadataWithFilter(const QString &nameOrIdFilter = "", int offset = 0, int limit = -1,
                                                                MacroMetadataSortOrder sortOrder = ID_ASC) = 0;
};


#endif // MAINMENUEVENTLISTENER_H
