/********************************************************************************
** Form generated from reading UI file 'MacroEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MACROEDITOR_H
#define UI_MACROEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <view/macro_editor/MacroEventsTable.h>
#include <view/macro_editor/MacroIdsLabel.h>

QT_BEGIN_NAMESPACE

class Ui_MacroEditor
{
public:
    QAction *actionUndoButton;
    QAction *actionSaveButton;
    QAction *actionRedoButton;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    MacroIdsLabel *macroIdsLabel;
    MacroEventsTable *macroEventsTable;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MacroEditor)
    {
        if (MacroEditor->objectName().isEmpty())
            MacroEditor->setObjectName(QStringLiteral("MacroEditor"));
        MacroEditor->resize(1199, 779);
        QFont font;
        font.setPointSize(12);
        MacroEditor->setFont(font);
        MacroEditor->setUnifiedTitleAndToolBarOnMac(true);
        actionUndoButton = new QAction(MacroEditor);
        actionUndoButton->setObjectName(QStringLiteral("actionUndoButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/undoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndoButton->setIcon(icon);
        actionSaveButton = new QAction(MacroEditor);
        actionSaveButton->setObjectName(QStringLiteral("actionSaveButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/saveIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveButton->setIcon(icon1);
        actionRedoButton = new QAction(MacroEditor);
        actionRedoButton->setObjectName(QStringLiteral("actionRedoButton"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/prefix1/redoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedoButton->setIcon(icon2);
        centralWidget = new QWidget(MacroEditor);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        macroIdsLabel = new MacroIdsLabel(centralWidget);
        macroIdsLabel->setObjectName(QStringLiteral("macroIdsLabel"));
        QFont font1;
        font1.setPointSize(14);
        macroIdsLabel->setFont(font1);
        macroIdsLabel->setScaledContents(false);
        macroIdsLabel->setWordWrap(true);
        macroIdsLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout_2->addWidget(macroIdsLabel);

        macroEventsTable = new MacroEventsTable(centralWidget);
        if (macroEventsTable->columnCount() < 7)
            macroEventsTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        macroEventsTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        macroEventsTable->setObjectName(QStringLiteral("macroEventsTable"));
        macroEventsTable->setFont(font);
        macroEventsTable->setContextMenuPolicy(Qt::CustomContextMenu);
        macroEventsTable->setAutoFillBackground(false);
        macroEventsTable->setStyleSheet(QStringLiteral(""));
        macroEventsTable->setFrameShape(QFrame::StyledPanel);
        macroEventsTable->setFrameShadow(QFrame::Sunken);
        macroEventsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        macroEventsTable->setDragEnabled(true);
        macroEventsTable->setDragDropOverwriteMode(false);
        macroEventsTable->setDragDropMode(QAbstractItemView::NoDragDrop);
        macroEventsTable->setDefaultDropAction(Qt::IgnoreAction);
        macroEventsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        macroEventsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        macroEventsTable->setShowGrid(true);
        macroEventsTable->setSortingEnabled(false);
        macroEventsTable->setCornerButtonEnabled(true);
        macroEventsTable->horizontalHeader()->setVisible(true);
        macroEventsTable->horizontalHeader()->setCascadingSectionResizes(true);
        macroEventsTable->horizontalHeader()->setDefaultSectionSize(100);
        macroEventsTable->horizontalHeader()->setMinimumSectionSize(1);
        macroEventsTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        macroEventsTable->horizontalHeader()->setStretchLastSection(true);
        macroEventsTable->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(macroEventsTable);

        MacroEditor->setCentralWidget(centralWidget);
        statusbar = new QStatusBar(MacroEditor);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        statusbar->setSizeGripEnabled(true);
        MacroEditor->setStatusBar(statusbar);
        toolBar = new QToolBar(MacroEditor);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMinimumSize(QSize(0, 30));
        toolBar->setStyleSheet(QStringLiteral("background-color: white;"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setOrientation(Qt::Horizontal);
        toolBar->setFloatable(false);
        MacroEditor->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionSaveButton);

        retranslateUi(MacroEditor);

        QMetaObject::connectSlotsByName(MacroEditor);
    } // setupUi

    void retranslateUi(QMainWindow *MacroEditor)
    {
        MacroEditor->setWindowTitle(QApplication::translate("MacroEditor", "Macro Editor", 0));
        actionUndoButton->setText(QApplication::translate("MacroEditor", "undoButton", 0));
#ifndef QT_NO_TOOLTIP
        actionUndoButton->setToolTip(QApplication::translate("MacroEditor", "Undo", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionUndoButton->setStatusTip(QApplication::translate("MacroEditor", "Undo: ctrl + z", 0));
#endif // QT_NO_STATUSTIP
        actionSaveButton->setText(QApplication::translate("MacroEditor", "saveButton", 0));
#ifndef QT_NO_TOOLTIP
        actionSaveButton->setToolTip(QApplication::translate("MacroEditor", "Save", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionSaveButton->setStatusTip(QApplication::translate("MacroEditor", "Save: ctrl + s", 0));
#endif // QT_NO_STATUSTIP
        actionRedoButton->setText(QApplication::translate("MacroEditor", "redoButton", 0));
#ifndef QT_NO_TOOLTIP
        actionRedoButton->setToolTip(QApplication::translate("MacroEditor", "Redo", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionRedoButton->setStatusTip(QApplication::translate("MacroEditor", "Redo: ctrl + y", 0));
#endif // QT_NO_STATUSTIP
        macroIdsLabel->setText(QApplication::translate("MacroEditor", "Editing Macro(s): ", 0));
        QTableWidgetItem *___qtablewidgetitem = macroEventsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MacroEditor", "Index", 0));
        QTableWidgetItem *___qtablewidgetitem1 = macroEventsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MacroEditor", "Description", 0));
        QTableWidgetItem *___qtablewidgetitem2 = macroEventsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MacroEditor", "Key String", 0));
        QTableWidgetItem *___qtablewidgetitem3 = macroEventsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MacroEditor", "Delay", 0));
        QTableWidgetItem *___qtablewidgetitem4 = macroEventsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MacroEditor", "Duration", 0));
        QTableWidgetItem *___qtablewidgetitem5 = macroEventsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MacroEditor", "Auto Correct", 0));
        QTableWidgetItem *___qtablewidgetitem6 = macroEventsTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MacroEditor", "Screenshot", 0));
#ifndef QT_NO_STATUSTIP
        macroEventsTable->setStatusTip(QApplication::translate("MacroEditor", "Left Click to Select Macro Event   ---   Right Click for Options", 0));
#endif // QT_NO_STATUSTIP
        toolBar->setWindowTitle(QApplication::translate("MacroEditor", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MacroEditor: public Ui_MacroEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MACROEDITOR_H
