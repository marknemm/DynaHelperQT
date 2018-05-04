#ifndef MACROMETADATA_H
#define MACROMETADATA_H


#include <QString>


/**
 * @brief The MacroMetadataSortOrder enum
 * Defines various possible sort orders for MacroMetadata objects.
 */
enum MacroMetadataSortOrder { ID_ASC, ID_DESC, NAME_ASC, NAME_DESC };


typedef struct MacroMetadata {
    int id;         // Macro ID.
    QString name;   // Macro name.
} MacroMetadata;


#endif // MACROMETADATA_H
