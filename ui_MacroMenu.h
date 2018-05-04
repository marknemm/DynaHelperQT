/********************************************************************************
** Form generated from reading UI file 'MacroMenu.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MACROMENU_H
#define UI_MACROMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <view/macro_menu/SelectionsTable.h>

QT_BEGIN_NAMESPACE

class Ui_MacroMenu
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *selectionEdit;
    QPushButton *createButton;
    QPushButton *runButton;
    SelectionsTable *selectionTable;
    QStatusBar *tipBar;

    void setupUi(QMainWindow *MacroMenu)
    {
        if (MacroMenu->objectName().isEmpty())
            MacroMenu->setObjectName(QStringLiteral("MacroMenu"));
        MacroMenu->resize(400, 618);
        MacroMenu->setMinimumSize(QSize(400, 618));
        MacroMenu->setContextMenuPolicy(Qt::CustomContextMenu);
        centralWidget = new QWidget(MacroMenu);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(400, 0));
        centralWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        selectionEdit = new QLineEdit(centralWidget);
        selectionEdit->setObjectName(QStringLiteral("selectionEdit"));
        QFont font;
        font.setPointSize(18);
        selectionEdit->setFont(font);
        selectionEdit->setAcceptDrops(false);
        selectionEdit->setAutoFillBackground(false);
        selectionEdit->setStyleSheet(QStringLiteral("padding-left:3px"));
        selectionEdit->setFrame(true);

        horizontalLayout->addWidget(selectionEdit);

        createButton = new QPushButton(centralWidget);
        createButton->setObjectName(QStringLiteral("createButton"));
        createButton->setMinimumSize(QSize(32, 32));
        QFont font1;
        font1.setPointSize(12);
        createButton->setFont(font1);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/addIcon"), QSize(), QIcon::Normal, QIcon::Off);
        createButton->setIcon(icon);
        createButton->setAutoDefault(false);
        createButton->setFlat(false);

        horizontalLayout->addWidget(createButton);

        runButton = new QPushButton(centralWidget);
        runButton->setObjectName(QStringLiteral("runButton"));
        runButton->setEnabled(false);
        runButton->setMinimumSize(QSize(32, 32));
        runButton->setFont(font1);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/runIcon"), QSize(), QIcon::Normal, QIcon::Off);
        runButton->setIcon(icon1);
        runButton->setAutoDefault(false);
        runButton->setFlat(false);

        horizontalLayout->addWidget(runButton);


        verticalLayout->addLayout(horizontalLayout);

        selectionTable = new SelectionsTable(centralWidget);
        if (selectionTable->columnCount() < 2)
            selectionTable->setColumnCount(2);
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setUnderline(false);
        font2.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font2);
        selectionTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QFont font3;
        font3.setPointSize(14);
        font3.setBold(true);
        font3.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font3);
        selectionTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        selectionTable->setObjectName(QStringLiteral("selectionTable"));
        QFont font4;
        font4.setPointSize(14);
        selectionTable->setFont(font4);
        selectionTable->setContextMenuPolicy(Qt::CustomContextMenu);
        selectionTable->setAutoFillBackground(false);
        selectionTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        selectionTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        selectionTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        selectionTable->setAutoScroll(true);
        selectionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        selectionTable->setProperty("showDropIndicator", QVariant(false));
        selectionTable->setDragDropOverwriteMode(false);
        selectionTable->setAlternatingRowColors(false);
        selectionTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        selectionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        selectionTable->setShowGrid(true);
        selectionTable->setSortingEnabled(true);
        selectionTable->setWordWrap(false);
        selectionTable->horizontalHeader()->setCascadingSectionResizes(true);
        selectionTable->horizontalHeader()->setDefaultSectionSize(60);
        selectionTable->horizontalHeader()->setStretchLastSection(true);
        selectionTable->verticalHeader()->setVisible(false);
        selectionTable->verticalHeader()->setHighlightSections(false);

        verticalLayout->addWidget(selectionTable);

        MacroMenu->setCentralWidget(centralWidget);
        tipBar = new QStatusBar(MacroMenu);
        tipBar->setObjectName(QStringLiteral("tipBar"));
        QFont font5;
        font5.setPointSize(11);
        font5.setBold(false);
        font5.setItalic(false);
        font5.setWeight(50);
        tipBar->setFont(font5);
        tipBar->setAutoFillBackground(true);
        tipBar->setSizeGripEnabled(true);
        MacroMenu->setStatusBar(tipBar);

        retranslateUi(MacroMenu);

        QMetaObject::connectSlotsByName(MacroMenu);
    } // setupUi

    void retranslateUi(QMainWindow *MacroMenu)
    {
        MacroMenu->setWindowTitle(QApplication::translate("MacroMenu", "MainMenu", 0));
#ifndef QT_NO_STATUSTIP
        selectionEdit->setStatusTip(QApplication::translate("MacroMenu", "Input macro ID or Name", 0));
#endif // QT_NO_STATUSTIP
        selectionEdit->setText(QString());
        selectionEdit->setPlaceholderText(QApplication::translate("MacroMenu", "Input ID or Name", 0));
#ifndef QT_NO_STATUSTIP
        createButton->setStatusTip(QApplication::translate("MacroMenu", "Create new macro (ctrl++)", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        createButton->setWhatsThis(QApplication::translate("MacroMenu", "ctrl++", 0));
#endif // QT_NO_WHATSTHIS
        createButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        runButton->setStatusTip(QApplication::translate("MacroMenu", "Run selected macro (enter)", 0));
#endif // QT_NO_STATUSTIP
        runButton->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = selectionTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MacroMenu", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem1 = selectionTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MacroMenu", "Name", 0));
#ifndef QT_NO_STATUSTIP
        selectionTable->setStatusTip(QApplication::translate("MacroMenu", "Double Click to Run         Right Click for Options", 0));
#endif // QT_NO_STATUSTIP
    } // retranslateUi

};

namespace Ui {
    class MacroMenu: public Ui_MacroMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MACROMENU_H
