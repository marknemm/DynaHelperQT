#include "MacroEditorContextMenu.h"


MacroEditorContextMenu::MacroEditorContextMenu(const QString &title, QWidget *parent) :
    QMenu(title, parent),
    insertBeforeAction("Insert Before", parent),
    insertAfterAction("Insert After", parent),
    copyAction("Copy", parent),
    deleteAction("Delete", parent)
{
    addAction(&insertBeforeAction);
    addAction(&insertAfterAction);
    addAction(&copyAction);
    addAction(&deleteAction);
}
