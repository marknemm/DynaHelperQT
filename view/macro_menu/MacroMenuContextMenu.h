#ifndef MACROMENUCONTEXTMENU_H
#define MACROMENUCONTEXTMENU_H


#include <QMenu>
#include <QAction>


class MacroMenuContextMenu : public QMenu
{

public:

    explicit MacroMenuContextMenu(const QString &title, QWidget *parent = nullptr);

    QAction runAction;
    QAction createAction;
    QAction renameAction;
    QAction editAction;
    QAction deleteAction;
    QAction copyAction;
};

#endif // MACROMENUCONTEXTMENU_H
