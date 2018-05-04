/********************************************************************************
** Form generated from reading UI file 'IOLoggingInfo.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IOLOGGINGINFO_H
#define UI_IOLOGGINGINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IOLoggingInfo
{
public:
    QWidget *centralwidget;
    QLabel *header;
    QLabel *eventInfo;

    void setupUi(QMainWindow *IOLoggingInfo)
    {
        if (IOLoggingInfo->objectName().isEmpty())
            IOLoggingInfo->setObjectName(QStringLiteral("IOLoggingInfo"));
        IOLoggingInfo->resize(454, 207);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(IOLoggingInfo->sizePolicy().hasHeightForWidth());
        IOLoggingInfo->setSizePolicy(sizePolicy);
        IOLoggingInfo->setMouseTracking(true);
        IOLoggingInfo->setContextMenuPolicy(Qt::NoContextMenu);
        IOLoggingInfo->setWindowOpacity(0.67);
        IOLoggingInfo->setAnimated(false);
        centralwidget = new QWidget(IOLoggingInfo);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        header = new QLabel(centralwidget);
        header->setObjectName(QStringLiteral("header"));
        header->setGeometry(QRect(6, 2, 441, 51));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        header->setFont(font);
        header->setMouseTracking(true);
        header->setAlignment(Qt::AlignCenter);
        header->setWordWrap(true);
        eventInfo = new QLabel(centralwidget);
        eventInfo->setObjectName(QStringLiteral("eventInfo"));
        eventInfo->setGeometry(QRect(6, 62, 441, 141));
        eventInfo->setMouseTracking(true);
        eventInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        eventInfo->setWordWrap(true);
        IOLoggingInfo->setCentralWidget(centralwidget);

        retranslateUi(IOLoggingInfo);

        QMetaObject::connectSlotsByName(IOLoggingInfo);
    } // setupUi

    void retranslateUi(QMainWindow *IOLoggingInfo)
    {
        IOLoggingInfo->setWindowTitle(QString());
        header->setText(QApplication::translate("IOLoggingInfo", "Recording All Input \n"
"Stop Recording: ctrl + w", 0));
        eventInfo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class IOLoggingInfo: public Ui_IOLoggingInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IOLOGGINGINFO_H
