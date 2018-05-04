#ifndef MACROMETAMODEL_H
#define MACROMETAMODEL_H


#include "Model.h"
#include "MacroMetadata.h"
#include "MacroEventModel.h"
#include <QString>
#include <QList>
#include <QRegExp>


/**
 * @brief The MacroMetaModel class
 * Model for Macro metadata. Macro metadata includes macro ID and Name pairs.
 * Macro events are associated with a specific macro metadata pair.
 */
class MacroMetaModel : public Model
{
public:

    MacroMetaModel(MacroEventModel &macroEventModel);


    /**
     * @brief addMacro
     * Adds a macro to the database by instantiating its ID and Name metadata.
     * @param name
     * The Name of the macro.
     * @param matchRegex (OPTIONAL)
     * Default will require the given name to match the regex requirement. Set to false to ignore regex requirement.
     * @return
     * The ID number assigned to the macro. -1 If the add failed due to an incorrectly formatted name.
     */
    int addMacro(const QString &name, bool matchRegex = true);


    /**
     * @brief copyMacro
     * Copies a Macro.
     * @param id
     * The ID of the Macro to copy.
     * @return
     * The resulting Macro Metadata (Macro ID and name) of the copy.
     */
    MacroMetadata copyMacro(int id);

    /**
     * @brief copyMacros
     * Copies all Macros with given IDs.
     * @param ids
     * The resulting Macro Metadata (Macro IDs and names) of the copy.
     */
    QList<MacroMetadata> copyMacros(const QList<int> &ids);


    /**
     * @brief removeMacro
     * Removes a macro from the database. Not only is macro metadata removed, but all associated events as well!
     * @param id
     * The ID of the macro to remove.
     */
    void removeMacro(int id);

    /**
     * @brief removeMacros
     * Removes all given macros from the database. All associated macro events are removed in addition to basic macro metadata.
     * @param ids
     * A list of the ids of the macros to remove.
     */
    void removeMacros(const QList<int> &ids);


    /**
     * @brief getMacro
     * Gets the macro Name corresponding to a given macro ID.
     * @param id
     * The id of the macro to get the name of.
     * @return
     * The Name corresponding to the given macro ID.
     */
    QString getMacroName(int id);

    /**
     * @brief getLastAddedMacroId
     * Gets the ID of the last added macro.
     * @return
     * The ID of the last added macro. -1 If there is no last add.
     */
    int getLastAddedMacroId() const;

    /**
     * @brief setMacroName
     * Sets the Name of a macro.
     * @param id
     * The ID of the macro to set the Name of.
     * @param name
     * The new Name.
     */
    void setMacroName(int id, const QString &name);

    /**
     * @brief getMacroNameRegex
     * Gets a regular expression for Macro name validation.
     * @return
     * A regular expression for validating Macro names.
     */
    QRegExp getMacroNameRegex() const;


    /**
     * @brief getMacroMetadata
     * Gets Macro metadata with optional filter values.
     * @param nameOrIdFilter (OPTIONAL)
     * The macro id or name to filter by. Default is empty string for no filter.
     * @param offset (OPTIONAL)
     * The offset of the start point macro metadata records to be retrieved. Default is 0 for no offset.
     * @param limit (OPTIONAL)
     * The limit on the number of macro metadata records to be retrieved. Default is -1 for no limit (request all).
     * @param sortOrder (OPTIONAL)
     * The order by which Macro metadata records will be sorted in. Default is sorting by ID number in ascending order.
     * @return
     * A list of the Macro metadata that was retrieved.
     */
    QList<MacroMetadata> getMacroMetadata(const QString &nameOrIdFilter = "", int offset = 0, int limit = -1,
                                          MacroMetadataSortOrder sortOrder = ID_ASC);

private:

    /**
     * @brief NAME_REGEX
     * The regular expression for Macro name validation.
     */
    const QRegExp NAME_REGEX;


    /**
     * @brief _macroEventModel
     * Model for the Macro's contained Macro Events.
     */
    MacroEventModel &_macroEventModel;

    /**
     * @brief _lastAddedId
     * The ID of the last added Macro. -1 If no last add.
     */
    int _lastAddedId;


    /**
     * @brief generateMacroCopyName
     * Generates the name for the copy of a Macro. The name generated will be of format:
     * 'srcName (N)' where srcName does not include a copy number and (N) is the copy number for this copy.
     * @param srcName
     * The name of the source Macro for the copy.
     * @return
     * The generated name of the destination Macro for the copy.
     */
    QString generateMacroCopyName(const QString &srcName);

    /**
     * @brief buildMacroIdWhereClause
     * Builds the Macro ID component of a where clause based off of a list of Macro IDs.
     * @param ids
     * The list of Macro IDs.
     * @return
     * The Macro ID where clause component.
     */
    QString buildMacroIdWhereClause(const QList<int> &ids) const;
};


#endif // MACROMETAMODEL_H
