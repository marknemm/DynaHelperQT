#include "MacroMenuContextMenu.h"


MacroMenuContextMenu::MacroMenuContextMenu(const QString &title, QWidget *parent) :
    QMenu(title, parent),
    runAction("Run", parent),
    createAction("New", parent),
    renameAction("Rename", parent),
    editAction("Edit", parent),
    deleteAction("Delete", parent),
    copyAction("Copy", parent)
{
    addAction(&runAction);
    addAction(&createAction);
    addAction(&renameAction);
    addAction(&editAction);
    addAction(&deleteAction);
    addAction(&copyAction);
}
