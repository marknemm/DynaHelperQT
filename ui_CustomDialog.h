/********************************************************************************
** Form generated from reading UI file 'CustomDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMDIALOG_H
#define UI_CUSTOMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_CustomDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *inputEdit;
    QLabel *directionsLabel;

    void setupUi(QDialog *CustomDialog)
    {
        if (CustomDialog->objectName().isEmpty())
            CustomDialog->setObjectName(QStringLiteral("CustomDialog"));
        CustomDialog->resize(404, 163);
        QFont font;
        font.setPointSize(14);
        CustomDialog->setFont(font);
        buttonBox = new QDialogButtonBox(CustomDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 100, 341, 51));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        inputEdit = new QLineEdit(CustomDialog);
        inputEdit->setObjectName(QStringLiteral("inputEdit"));
        inputEdit->setGeometry(QRect(50, 60, 301, 31));
        directionsLabel = new QLabel(CustomDialog);
        directionsLabel->setObjectName(QStringLiteral("directionsLabel"));
        directionsLabel->setGeometry(QRect(30, 20, 341, 20));
        directionsLabel->setAlignment(Qt::AlignCenter);

        retranslateUi(CustomDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CustomDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CustomDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CustomDialog);
    } // setupUi

    void retranslateUi(QDialog *CustomDialog)
    {
        CustomDialog->setWindowTitle(QApplication::translate("CustomDialog", "Macro Creation", 0));
        directionsLabel->setText(QApplication::translate("CustomDialog", "Input a name for the new Macro below", 0));
    } // retranslateUi

};

namespace Ui {
    class CustomDialog: public Ui_CustomDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMDIALOG_H
