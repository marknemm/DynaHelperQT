#ifndef MACROEDITORCONTEXTMENU_H
#define MACROEDITORCONTEXTMENU_H


#include <QMenu>
#include <QAction>


/**
 * @brief The CustomContextMenu class
 * Custom (right click) context menu for the Macro Editor Macro Events Table.
 */
class MacroEditorContextMenu : public QMenu
{

public:

    explicit MacroEditorContextMenu(const QString &title, QWidget *parent=nullptr);

    QAction insertBeforeAction;
    QAction insertAfterAction;
    QAction copyAction;
    QAction deleteAction;
};


#endif // MACROEDITORCONTEXTMENU_H
