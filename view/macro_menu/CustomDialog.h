#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H


#include <QDialog>
#include <QRegExpValidator>
#include <QShowEvent>


namespace Ui {
class CustomDialog;
}

class CustomDialog : public QDialog
{
    Q_OBJECT

public:

    explicit CustomDialog(QWidget *parent = 0, const QString &directions = "", const QRegExpValidator *validator=nullptr);
    ~CustomDialog();

    /**
     * @brief getText
     * Gets the user input text from the dialog.
     * @return The user input text.
     */
    QString getText() const;

private:

    Ui::CustomDialog *ui;

    /**
     * @brief showEvent
     * Called when the Dialog is shown.
     * @param e The show event container.
     */
    void showEvent(QShowEvent *e) override;
};


#endif // CUSTOMDIALOG_H
