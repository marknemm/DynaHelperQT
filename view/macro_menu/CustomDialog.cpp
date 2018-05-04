#include "CustomDialog.h"
#include "ui_CustomDialog.h"

CustomDialog::CustomDialog(QWidget *parent, const QString &directions, const QRegExpValidator *validator) :
    QDialog(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);
    ui->directionsLabel->setText(directions);
    ui->inputEdit->setValidator(validator);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}


QString CustomDialog::getText() const
{
    return ui->inputEdit->text();
}


void CustomDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
    ui->inputEdit->setFocus();
}
